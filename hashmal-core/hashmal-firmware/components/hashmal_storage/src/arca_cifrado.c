/**
 * ===================================================================
 * PIEZA 01 - EL ARCA - arca_cifrado.c
 * Sub-pieza: EL ORO - AES-256-GCM "mibayit umijuts"
 * Exodo 25:11a-b + Numeros 4:5-6
 * ===================================================================
 *
 * "vetsipita oto zahav tahor mibayit umijuts tetsapenu
 *  ve'asita 'alav zer zahav saviv" - Exodo 25:11
 * "Lo cubriras de ORO PURO por DENTRO y por FUERA lo cubriras,
 *  y haras sobre el una moldura de oro alrededor."
 *
 * "uva'u Aharon uvanav binsoa' hamajane vehoridu et parojet
 *  hamasaj vejissu bah et aron ha'edut" - Numeros 4:5
 * "Vendran Aaron y sus hijos cuando parta el campamento, bajaran
 *  el velo y cubriran con el el Arca del Testimonio."
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 * El ORO. La misma naturaleza en reposo (mibayit = dentro, data
 * almacenada) y en transito (umijuts = fuera, data en migracion).
 *
 * v.11 dice "tetsapenu" dos veces - cubre, cubre. DOBLE orden,
 * misma cobertura. No hay proteccion fuerte y debil. Misma
 * dignidad, misma clave, misma maquina criptografica: AES-256-GCM.
 *
 * --- CAPAS DEL BADDIM QUE ESTE ARCHIVO REALIZA ---
 *
 *   L4 (Nm 4:5-15)   - cubiertas antes de mover = encryption in transit
 *   L5 (Nm 4:15)     - coatitas no ven = el transportista opera sobre
 *                      ciphertext, nunca sobre claro
 *
 * --- ORO DE EGIPTO (mbedtls) ---
 *
 * Exodo 12:35-36 - Israel pidio a los egipcios "vasos de plata y de
 * oro"; YHWH hizo que se los dieran. Lo de Egipto se FUNDIO segun
 * el diseno del Padre, no del Faraon. mbedtls es oro de Egipto:
 * lo encapsulamos tras primitivas nuestras (hashmal_oro_*).
 *
 * --- CLAVE ---
 *
 * Una clave AES-256 por nodo. Provision transitoria (por boot, con
 * RNG hardware) HASTA que hashmal_keygen (Pieza 13, shemen haMishja,
 * Ex 30:22-33) este operativo y entregue la clave fundacional.
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "mbedtls/gcm.h"       /* ORO DE EGIPTO - AES-256-GCM */
#include "esp_random.h"        /* ORO DE EGIPTO - RNG hardware */
#include "esp_log.h"           /* ORO DE EGIPTO - logging */

#include "hashmal_storage.h"
#include "hashmal_storage_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF + mbedtls
 * ================================================================== */
static const char *ORO_TAG = "hashmal.arca.oro";

#define ORO_LOG_I(fmt, ...)  ESP_LOGI(ORO_TAG, fmt, ##__VA_ARGS__)
#define ORO_LOG_W(fmt, ...)  ESP_LOGW(ORO_TAG, fmt, ##__VA_ARGS__)
#define ORO_LOG_E(fmt, ...)  ESP_LOGE(ORO_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * ESTADO DEL MODULO
 *
 * - key:          clave AES-256 (32 bytes). Misma adentro y afuera.
 * - key_lista:    ?ya se provisiono?
 * - cubierta:     ?edut actualmente cubierta para migracion?
 * - edut_cipher:  envelope de la edut (ciphertext)
 * - edut_iv/tag:  metadata GCM para el envelope actual
 * ================================================================== */
static struct {
    uint8_t  key[HASHMAL_AES_KEY_BYTES];
    bool     key_lista;

    bool     cubierta;
    uint8_t  edut_cipher[HASHMAL_EDUT_BYTES];
    uint8_t  edut_iv    [HASHMAL_AES_IV_BYTES];
    uint8_t  edut_tag   [HASHMAL_AES_TAG_BYTES];
} g_oro = {
    .key_lista = false,
    .cubierta  = false,
};

/* ==================================================================
 * HELPERS ESTATICOS - mbedtls encapsulado
 * ================================================================== */

/**
 * Borra con seguridad (proteccion contra tamper de buffers sensibles).
 * memset puede ser optimizado fuera por el compilador; esta funcion
 * esta marcada para no serlo. Hebreos 4:13 - "panta gymna" (todo
 * descubierto ante El); ante todos los demas, limpieza explicita.
 */
static void zerize(volatile uint8_t *buf, size_t n)
{
    while (n--) { *buf++ = 0u; }
}

/* ==================================================================
 * API INTERNA - primitivas de ORO
 * (Declaradas en hashmal_storage_internal.h)
 * ================================================================== */

void hashmal_oro_key_provisionar(void)
{
    if (g_oro.key_lista) return;

    /* Ex 30:22-33 - la clave fundacional viene del shemen haMishja
     * (Pieza 13). Mientras Pieza 13 no este operativa, usamos RNG
     * hardware por boot. La clave NO persiste entre reinicios -
     * consciente limitacion transitoria. */
    esp_fill_random(g_oro.key, sizeof(g_oro.key));
    g_oro.key_lista = true;

    ORO_LOG_W("clave provisional generada por boot (RNG hw); "
              "Pieza 13 — shémen haMishjá — aún pendiente");
}

bool hashmal_oro_key_lista(void)
{
    return g_oro.key_lista;
}

/**
 * hashmal_oro_key_destruir - 1 Samuel 5:3-4
 *
 * "vehinne Dagon nofe? 'al panav aretsah lifne aron YHWH
 *  verosh Dagon ushetei kapot yadav kerutot el hamiftan" - 1 Sam 5:4
 * "Y he aqui que Dagon estaba caido sobre su rostro en tierra
 *  delante del Arca de YHWH, y la cabeza de Dagon y las dos palmas
 *  de sus manos cortadas sobre el umbral."
 *
 * Sin cabeza no hay voluntad. Sin palmas no hay manipulacion. El
 * idolo queda inutil ante el Arca aun estando "en su poder". Aqui:
 * borramos la clave volatil y el envelope vigente. El oro pierde
 * su capacidad de abrir - el atacante queda con bloques inertes.
 *
 * IRREVERSIBLE por diseno. Solo hashmal_oro_key_provisionar (o,
 * cuando exista, pieza 13 - shemen haMishja) puede restaurar el
 * material para re-operar.
 */
void hashmal_oro_key_destruir(void)
{
    /* "verosh Dagon" - la cabeza: la clave que DECIDE.
     * zerize no puede ser optimizado fuera. */
    zerize(g_oro.key, sizeof(g_oro.key));
    g_oro.key_lista = false;

    /* "ushte kapot yadav kerutot" - las dos palmas: el envelope
     * en transito. Si habia una edut cubierta para migrar, muere
     * aqui. El coatita que cargase este ciphertext llegaria con
     * un cadaver. */
    zerize(g_oro.edut_cipher, sizeof(g_oro.edut_cipher));
    zerize(g_oro.edut_iv,     sizeof(g_oro.edut_iv));
    zerize(g_oro.edut_tag,    sizeof(g_oro.edut_tag));
    g_oro.cubierta = false;

    ORO_LOG_E("CLAVE DESTRUIDA — Dagón cae (1 Sam 5:3-4)");
    ORO_LOG_E("  cifrar/descifrar rechazarán hasta re-provisión");
}

int hashmal_oro_cifrar(const uint8_t *plain, size_t plain_len,
                       uint8_t *cipher,
                       uint8_t iv_out[HASHMAL_AES_IV_BYTES],
                       uint8_t tag_out[HASHMAL_AES_TAG_BYTES])
{
    if (plain == NULL || cipher == NULL || iv_out == NULL || tag_out == NULL) {
        return HASHMAL_ARCA_ERR_NULL;
    }
    if (!g_oro.key_lista) {
        ORO_LOG_E("cifrar: clave no provisionada (llama hashmal_oro_key_provisionar)");
        return HASHMAL_ARCA_ERR_INIT;
    }

    /* v.11 - oro: mismo para dentro y fuera. IV nuevo por operacion
     * (nonce misuse es el unico modo de romper GCM con clave fija). */
    esp_fill_random(iv_out, HASHMAL_AES_IV_BYTES);

    mbedtls_gcm_context ctx;
    mbedtls_gcm_init(&ctx);

    int rc = mbedtls_gcm_setkey(&ctx,
                                MBEDTLS_CIPHER_ID_AES,
                                g_oro.key,
                                HASHMAL_AES_KEY_BITS);
    if (rc != 0) {
        ORO_LOG_E("cifrar: setkey falló (mbedtls=%d)", rc);
        mbedtls_gcm_free(&ctx);
        return -1;
    }

    rc = mbedtls_gcm_crypt_and_tag(&ctx,
                                   MBEDTLS_GCM_ENCRYPT,
                                   plain_len,
                                   iv_out,  HASHMAL_AES_IV_BYTES,
                                   NULL, 0,              /* AAD silente en v.11 */
                                   plain, cipher,
                                   HASHMAL_AES_TAG_BYTES, tag_out);

    mbedtls_gcm_free(&ctx);

    if (rc != 0) {
        ORO_LOG_E("cifrar: crypt_and_tag falló (mbedtls=%d)", rc);
        return -1;
    }
    return 0;
}

int hashmal_oro_descifrar(const uint8_t *cipher, size_t cipher_len,
                          const uint8_t iv[HASHMAL_AES_IV_BYTES],
                          const uint8_t tag[HASHMAL_AES_TAG_BYTES],
                          uint8_t *plain_out)
{
    if (cipher == NULL || iv == NULL || tag == NULL || plain_out == NULL) {
        return HASHMAL_ARCA_ERR_NULL;
    }
    if (!g_oro.key_lista) {
        ORO_LOG_E("descifrar: clave no provisionada");
        return HASHMAL_ARCA_ERR_INIT;
    }

    mbedtls_gcm_context ctx;
    mbedtls_gcm_init(&ctx);

    int rc = mbedtls_gcm_setkey(&ctx,
                                MBEDTLS_CIPHER_ID_AES,
                                g_oro.key,
                                HASHMAL_AES_KEY_BITS);
    if (rc != 0) {
        ORO_LOG_E("descifrar: setkey falló (mbedtls=%d)", rc);
        mbedtls_gcm_free(&ctx);
        return -1;
    }

    rc = mbedtls_gcm_auth_decrypt(&ctx,
                                  cipher_len,
                                  iv,  HASHMAL_AES_IV_BYTES,
                                  NULL, 0,               /* AAD silente */
                                  tag, HASHMAL_AES_TAG_BYTES,
                                  cipher, plain_out);

    mbedtls_gcm_free(&ctx);

    if (rc != 0) {
        /* Tag mismatch = integridad violada = zer roto.
         * 2 Sam 6:6-7 sombra: tocar sin derecho no pasa. */
        ORO_LOG_E("descifrar: autenticación falló (mbedtls=%d) — zer roto", rc);
        return -1;
    }
    return 0;
}

/* ==================================================================
 * API PUBLICA - cubrir / descubrir
 * ================================================================== */

/**
 * hashmal_storage_cubrir - Nm 4:5-6
 *
 * "uva'u Aharon uvanav... vejissu bah et aron ha'edut... venatenu
 *  'alav kisui 'or tajash ufarsu beged-kelil tejelet milma'la"
 *
 * Antes de mover: los sacerdotes CUBREN el Arca. El transportista
 * (coatita) cargara ciphertext, nunca plaintext (L4-L5).
 *
 * 2 testigos:
 *   v.11 (Ex 25) - oro dentro y fuera, misma proteccion
 *   Nm 4:5-6   - cubrir antes de transportar
 */
int hashmal_storage_cubrir(void)
{
    const hashmal_arca_t *arca = hashmal_storage_get_arca();
    if (arca == NULL) {
        ORO_LOG_E("cubrir: Arca no levantada (Éx 40:3 pendiente)");
        return HASHMAL_ARCA_ERR_INIT;
    }

    /* Provisiona clave si aun no lo hizo nadie.
     * (Pieza 13 lo hara formalmente cuando exista.) */
    if (!g_oro.key_lista) hashmal_oro_key_provisionar();

    if (g_oro.cubierta) {
        /* Idempotente - ya estaba cubierta. Nm 4:5-6 se ejecuta UNA
         * vez por migracion; llamarlo de nuevo no corrompe estado. */
        ORO_LOG_W("cubrir: Arca ya cubierta (no-op)");
        return HASHMAL_ARCA_OK;
    }

    /* v.11b - umijuts (fuera): producir envelope cifrado para migracion.
     * La edut viaja cubierta; el coatita solo ve ciphertext + iv + tag. */
    int rc = hashmal_oro_cifrar(arca->edut.bytes,
                                HASHMAL_EDUT_BYTES,
                                g_oro.edut_cipher,
                                g_oro.edut_iv,
                                g_oro.edut_tag);
    if (rc != 0) {
        ORO_LOG_E("cubrir: cifrado de edut falló");
        /* Deja buffers limpios - no dejamos media cobertura */
        zerize(g_oro.edut_cipher, sizeof(g_oro.edut_cipher));
        zerize(g_oro.edut_iv,     sizeof(g_oro.edut_iv));
        zerize(g_oro.edut_tag,    sizeof(g_oro.edut_tag));
        return -1;
    }

    g_oro.cubierta = true;

    /* TORA al lado (Dt 31:26): su cobertura es variable en tamano.
     * SILENTE en esta version - cubrir solo abarca la edut (SEALED_CORE).
     * La extension a tora queda para iteracion posterior cuando el
     * modulo tora maneje buffers de tamano dinamico. */

    ORO_LOG_I("Arca cubierta (Nm 4:5-6): edut en envelope AES-256-GCM");
    ORO_LOG_I("  coatitas pueden transportar — no ven el claro (L4-L5)");
    return HASHMAL_ARCA_OK;
}

/**
 * hashmal_storage_descubrir - 2 Sam 6:6-7 / 1 Sam 6:19
 *
 * v.14 (Ex 25) - "veheveta et habaddim batabba'ot": el acceso
 * legitimo es VIA BADDIM. Todo lo demas es Uza (2 Sam 6:6-7) o
 * Bet-Shemesh (1 Sam 6:19) - toque directo o mirar sin derecho.
 *
 * 2 testigos:
 *   Ex 25:14-15 - baddim siempre presentes, unico modo de manejo
 *   2 Sam 6:6-7 - toque directo (ACCESO_DIRECTO) = muerte
 */
int hashmal_storage_descubrir(hashmal_acceso_t via)
{
    /* 2 Sam 6:6-7 - Uza extendio la mano al Arca y murio.
     * El acceso DIRECTO esta prohibido por el Padre, sin excepcion. */
    if (via == HASHMAL_ACCESO_DIRECTO) {
        ORO_LOG_E("descubrir: ACCESO DIRECTO prohibido (2 Sam 6:6-7, Uzá)");
        return HASHMAL_ARCA_ERR_TOQUE_DIRECTO;
    }
    if (via != HASHMAL_ACCESO_VIA_BADDIM) {
        /* 1 Sam 6:19 - Bet-Shemesh miro sin autorizacion: 70 muertos. */
        ORO_LOG_E("descubrir: acceso no autorizado (via=%d)", (int)via);
        return HASHMAL_ARCA_ERR_NO_AUTORIZADO;
    }

    if (!g_oro.cubierta) {
        /* No estaba cubierta - nada que descubrir. No-op seguro. */
        ORO_LOG_W("descubrir: Arca no estaba cubierta (no-op)");
        return HASHMAL_ARCA_OK;
    }

    /* Verificamos integridad descifrando en buffer local.
     * El plaintext NO se expone por esta funcion - esta solo
     * VALIDA que el envelope es autentico. Quien deba consumirlo
     * lo hara por la interfaz baddim (arca_interfaz.c) con su
     * propia autorizacion. */
    uint8_t edut_claro[HASHMAL_EDUT_BYTES];
    int rc = hashmal_oro_descifrar(g_oro.edut_cipher,
                                   HASHMAL_EDUT_BYTES,
                                   g_oro.edut_iv,
                                   g_oro.edut_tag,
                                   edut_claro);
    if (rc != 0) {
        /* Tag invalido - alguien toco la madera.
         * NO limpiamos g_oro.cubierta: sigue "cubierta" pero corrupta,
         * para diagnostico. El zer roto es senal, no accidente. */
        ORO_LOG_E("descubrir: zer roto — tag GCM inválido");
        zerize(edut_claro, sizeof(edut_claro));
        return HASHMAL_ARCA_ERR_ZER_ROTO;
    }

    /* Validado. Limpieza de buffers sensibles antes de salir
     * (el claro no se retiene aqui; el consumidor lo obtendra por
     * la via legitima de baddim). */
    zerize(edut_claro,          sizeof(edut_claro));
    zerize(g_oro.edut_cipher,   sizeof(g_oro.edut_cipher));
    zerize(g_oro.edut_iv,       sizeof(g_oro.edut_iv));
    zerize(g_oro.edut_tag,      sizeof(g_oro.edut_tag));
    g_oro.cubierta = false;

    ORO_LOG_I("Arca descubierta vía baddím — envelope autenticado");
    return HASHMAL_ARCA_OK;
}
