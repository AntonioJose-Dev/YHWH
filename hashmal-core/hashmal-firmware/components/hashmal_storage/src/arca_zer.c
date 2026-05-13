/**
 * ===================================================================
 * PIEZA 01 - EL ARCA - arca_zer.c
 * Sub-pieza: EL ZER - moldura de integridad (Merkle SHA-256)
 * Exodo 25:11c + Josue 6:15-20
 * ===================================================================
 *
 * "ve'asita 'alav zer zahav saviv" - Exodo 25:11c
 * "Y haras sobre el una MOLDURA de oro ALREDEDOR."
 *
 * "vayisseru et ha'ir bayom hahu sheva pe'amim
 *  raq bayom hahu saveu et ha'ir sheva pe'amim" - Josue 6:15
 * "Y rodearon la ciudad aquel dia SIETE VECES;
 *  solo aquel dia rodearon la ciudad siete veces."
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 * El ZER es la moldura de oro que RODEA (saviv) TODO el dato.
 * Lo que delata si alguien anadio o quito algo entre el oro y la
 * madera - nadie toca la madera directamente (v.11).
 *
 * En firmware: Merkle root SHA-256 sobre (edut || tora).
 * Dos hojas, una raiz, 32 bytes. La moldura rodea SIEMPRE -
 * incluso vacia, el zer tiene un hash determinista (v.11 "saviv"
 * es absoluto, no condicional).
 *
 * --- 3 FUNCIONES ---
 *
 *   zer_calcular  - recalcula el root y actualiza g_arca.zer
 *   zer_verificar - compara el zer actual con el almacenado
 *   jerico        - verificacion ACTIVA: 7 vueltas de recalculo
 *                   y comparacion. Integridad con poder, no solo
 *                   lectura.
 *
 * --- ORO DE EGIPTO (mbedtls) ---
 *
 * mbedtls/sha256.h encapsulado tras primitivas nuestras. El
 * hardware del ESP32-S3 tiene acelerador SHA incorporado que
 * mbedtls usa transparentemente - oro de Egipto fundido.
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <inttypes.h>

#include "mbedtls/sha256.h"    /* ORO DE EGIPTO - SHA-256 */
#include "esp_log.h"

#include "hashmal_storage.h"
#include "hashmal_storage_internal.h"

/* ==================================================================
 * ENCAPSULACION
 * ================================================================== */
static const char *ZER_TAG = "hashmal.arca.zer";

#define ZER_LOG_I(fmt, ...)  ESP_LOGI(ZER_TAG, fmt, ##__VA_ARGS__)
#define ZER_LOG_W(fmt, ...)  ESP_LOGW(ZER_TAG, fmt, ##__VA_ARGS__)
#define ZER_LOG_E(fmt, ...)  ESP_LOGE(ZER_TAG, fmt, ##__VA_ARGS__)

/* Tamano de la hoja Merkle (SHA-256 salida): coincide con el hash del zer. */
#define ZER_LEAF_BYTES   HASHMAL_ZER_HASH_BYTES   /* 32 */

/* ==================================================================
 * HELPERS ESTATICOS - SHA-256 encapsulado
 * ================================================================== */

/**
 * SHA-256 one-shot sobre buffer. Encapsula la API de mbedtls para
 * que ningun otro sitio del modulo toque mbedtls directamente.
 *
 * @param in    buffer de entrada (puede ser NULL si len == 0)
 * @param len   bytes de entrada (0 valido -> SHA-256 del vacio)
 * @param out   salida de 32 bytes (no NULL)
 * @return 0 en exito, codigo mbedtls en fallo.
 */
static int sha256_once(const uint8_t *in, size_t len,
                       uint8_t out[ZER_LEAF_BYTES])
{
    mbedtls_sha256_context ctx;
    mbedtls_sha256_init(&ctx);

    int rc = mbedtls_sha256_starts(&ctx, 0);   /* 0 = SHA-256, 1 = SHA-224 */
    if (rc == 0 && len > 0 && in != NULL) {
        rc = mbedtls_sha256_update(&ctx, in, len);
    } else if (rc == 0 && in == NULL && len > 0) {
        rc = -1;    /* proteccion: len>0 exige buffer */
    }
    if (rc == 0) rc = mbedtls_sha256_finish(&ctx, out);

    mbedtls_sha256_free(&ctx);
    return rc;
}

/**
 * Calcula el Merkle root SHA-256 sobre el contenido del Arca.
 *
 * Arbol de 2 hojas:
 *   leaf_edut  = SHA-256(edut.bytes, 256 B fijos)
 *   leaf_tora  = SHA-256(tora.bytes, tora.tamano)   ; vacio si no adjunta
 *   root       = SHA-256(leaf_edut || leaf_tora)
 *
 * v.11 "saviv" - la moldura RODEA siempre. Incluso sin contenido
 * real, el hash es determinista (SHA-256 del vacio es conocido).
 */
static int calcular_merkle_root(const hashmal_arca_t *arca,
                                uint8_t out_root[ZER_LEAF_BYTES])
{
    if (arca == NULL || out_root == NULL) return HASHMAL_ARCA_ERR_NULL;

    uint8_t leaf_edut [ZER_LEAF_BYTES];
    uint8_t leaf_torah[ZER_LEAF_BYTES];

    /* Hoja 1 - edut (v.16, 256 B fijos en SEALED_CORE). Si aun no
     * se sello, edut.bytes son ceros; la hoja es SHA-256 de ceros -
     * determinista y distinta de cualquier contenido real. */
    int rc = sha256_once(arca->edut.bytes, HASHMAL_EDUT_BYTES, leaf_edut);
    if (rc != 0) return rc;

    /* Hoja 2 - tora al lado (Dt 31:26). Si bytes == NULL o tamano
     * == 0, la hoja es SHA-256 del vacio (moldura saviv incluso
     * sin tora adjunta). */
    const uint8_t *torah_in  = arca->torah.bytes;
    size_t         torah_len = arca->torah.tamano;
    if (torah_in == NULL) torah_len = 0u;       /* silente -> vacio */

    rc = sha256_once(torah_in, torah_len, leaf_torah);
    if (rc != 0) return rc;

    /* Raiz - concatena y hashea. */
    uint8_t concat[ZER_LEAF_BYTES * 2];
    memcpy(&concat[0],                leaf_edut,  ZER_LEAF_BYTES);
    memcpy(&concat[ZER_LEAF_BYTES],   leaf_torah, ZER_LEAF_BYTES);

    rc = sha256_once(concat, sizeof(concat), out_root);
    return rc;
}

/* ==================================================================
 * API PUBLICA - zer_calcular
 * ================================================================== */

/**
 * hashmal_storage_zer_calcular - Exodo 25:11c
 *
 * Recalcula la moldura (Merkle root SHA-256) sobre edut + tora,
 * la almacena en g_arca.zer.hash, incrementa el nonce anti-replay
 * y marca zer.rodea = true ("saviv" - absoluto).
 *
 * Idempotente: puede llamarse en cualquier momento. Si el contenido
 * no cambio, el hash resultante es el mismo; solo el nonce avanza.
 *
 * 2 testigos:
 *   Ex 25:11c - "zer zahav saviv"
 *   v.11 completo - "mibayit umijuts": integridad misma dentro y afuera
 */
int hashmal_storage_zer_calcular(uint8_t out_hash[HASHMAL_ZER_HASH_BYTES])
{
    hashmal_arca_t *arca = hashmal_storage_arca_mut();
    if (arca == NULL) {
        ZER_LOG_E("zer_calcular: Arca no levantada (Éx 40:3 pendiente)");
        return HASHMAL_ARCA_ERR_INIT;
    }

    uint8_t root[HASHMAL_ZER_HASH_BYTES];
    int rc = calcular_merkle_root(arca, root);
    if (rc != 0) {
        ZER_LOG_E("zer_calcular: SHA-256 falló (mbedtls=%d)", rc);
        return -1;
    }

    memcpy(arca->zer.hash, root, HASHMAL_ZER_HASH_BYTES);
    arca->zer.nonce += 1u;                     /* anti-replay */
    arca->zer.rodea  = true;                   /* v.11 - saviv */

    if (out_hash != NULL) {
        memcpy(out_hash, root, HASHMAL_ZER_HASH_BYTES);
    }

    ZER_LOG_I("zer recalculado (Merkle SHA-256 sobre edut || torá); nonce=%" PRIu64,
              arca->zer.nonce);
    return HASHMAL_ARCA_OK;
}

/* ==================================================================
 * API PUBLICA - zer_verificar
 * ================================================================== */

/**
 * hashmal_storage_zer_verificar - Exodo 25:11
 *
 * Recalcula la moldura y la compara con la almacenada. Si difieren,
 * alguien TOCO LA MADERA entre el oro dentro y el oro fuera -
 * la integridad fue violada (ERR_ZER_ROTO).
 *
 * 2 testigos:
 *   Ex 25:11   - oro rodea (saviv); entre oro y oro, nadie toca
 *   2 Sam 6:6-7 - tocar sin mishpat = Uza (mismo principio)
 */
int hashmal_storage_zer_verificar(void)
{
    const hashmal_arca_t *arca = hashmal_storage_get_arca();
    if (arca == NULL) {
        ZER_LOG_E("zer_verificar: Arca no levantada");
        return HASHMAL_ARCA_ERR_INIT;
    }
    if (!arca->zer.rodea) {
        ZER_LOG_E("zer_verificar: zer no rodea (v.11 savív violado)");
        return HASHMAL_ARCA_ERR_ZER_ROTO;
    }

    uint8_t nuevo[HASHMAL_ZER_HASH_BYTES];
    int rc = calcular_merkle_root(arca, nuevo);
    if (rc != 0) {
        ZER_LOG_E("zer_verificar: SHA-256 falló (mbedtls=%d)", rc);
        return -1;
    }

    if (memcmp(nuevo, arca->zer.hash, HASHMAL_ZER_HASH_BYTES) != 0) {
        ZER_LOG_E("zer_verificar: ZER ROTO — alguien tocó la madera (v.11)");
        return HASHMAL_ARCA_ERR_ZER_ROTO;
    }

    return HASHMAL_ARCA_OK;
}

/* ==================================================================
 * API PUBLICA - jerico (verificacion con poder)
 * ================================================================== */

/**
 * hashmal_storage_jerico - Josue 6:3-4, 15-16, 20
 *
 * "saboutem et ha'ir... shesh yamim... ubayom hashevii tasobbu
 *  et ha'ir sheva pe'amim... vehaya bimshoj beqeren hayovel
 *  bishoma'jem et qol hashofar yari'u jol ha'am teru'a gedola
 *  venafela jomat ha'ir tajteha" - Jos 6:3-5
 *
 * Israel rodeo Jerico por 6 dias, 1 vez al dia. El septimo dia
 * la rodearon SIETE veces. Luego: grito, trompeta, y el muro
 * cayo "tajteha" (bajo si misma).
 *
 * En firmware: verificacion ACTIVA. No lee el hash una vez -
 * lo rodea SIETE veces, recalculando desde el contenido cada
 * vuelta y comparando con el almacenado. Si los 7 cierran
 * iguales -> integridad confirmada con poder (la "trompeta"
 * es el OK final). Si alguna vuelta discrepa -> PARA ahi:
 * zer roto, muro de la sospecha NO cae todavia.
 *
 * 2 testigos:
 *   Jos 6:15   - "sheva pe'amim" (siete veces)
 *   Ex 25:11   - zer saviv (lo que se rodea es la moldura)
 */
int hashmal_storage_jerico(void)
{
    const hashmal_arca_t *arca = hashmal_storage_get_arca();
    if (arca == NULL) {
        ZER_LOG_E("jericó: Arca no levantada");
        return HASHMAL_ARCA_ERR_INIT;
    }
    if (!arca->zer.rodea) {
        ZER_LOG_E("jericó: zer no rodea (v.11 savív violado)");
        return HASHMAL_ARCA_ERR_ZER_ROTO;
    }

    ZER_LOG_I("jericó: empezando %d vueltas al zer (Jos 6:15)",
              HASHMAL_VUELTAS_JERICO);

    /* Siete vueltas. Cada una recalcula desde el contenido
     * ACTUAL del Arca y compara con el zer almacenado. Si el
     * contenido cambia durante las vueltas (ataque concurrente),
     * la vuelta correspondiente lo detecta. */
    for (uint8_t vuelta = 1u; vuelta <= HASHMAL_VUELTAS_JERICO; ++vuelta) {
        uint8_t nuevo[HASHMAL_ZER_HASH_BYTES];
        int rc = calcular_merkle_root(arca, nuevo);
        if (rc != 0) {
            ZER_LOG_E("jericó: vuelta %u — SHA-256 falló (mbedtls=%d)",
                      vuelta, rc);
            return -1;
        }
        if (memcmp(nuevo, arca->zer.hash, HASHMAL_ZER_HASH_BYTES) != 0) {
            ZER_LOG_E("jericó: vuelta %u/%d — ZER ROTO; muro no cae",
                      vuelta, HASHMAL_VUELTAS_JERICO);
            return HASHMAL_ARCA_ERR_ZER_ROTO;
        }
        ZER_LOG_I("  vuelta %u/%d — muro firme", vuelta, HASHMAL_VUELTAS_JERICO);
    }

    /* Jos 6:20 - "vatipol hajoma tajteha": al septimo rodeo con
     * el grito y la trompeta, el muro (del ATACANTE) cayo. Aqui
     * el muro que cae es el de la duda sobre la integridad -
     * quedo confirmada con la persistencia de las 7 vueltas. */
    ZER_LOG_I("═══ JERICÓ — 7 vueltas cerradas iguales (Jos 6:15-20)");
    ZER_LOG_I("    integridad del zer confirmada con poder activo");
    return HASHMAL_ARCA_OK;
}
