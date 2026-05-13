/**
 * ===================================================================
 * PIEZA 01 - EL ARCA - arca_merkle.c
 * Sub-pieza: LA TORA AL LADO - adjuntar, desvincular, comparar
 * Deuteronomio 31:26 + 1 Reyes 8:9 + Deuteronomio 19:15
 * ===================================================================
 *
 * "laqoaj et sefer haTora hazze vesamtem oto MITSAD aron berit YHWH
 *  Eloheijem vehaya sham beja le'ed" - Dt 31:26
 * "Tomad este libro de la Tora y PONEDLO AL LADO del Arca del Pacto
 *  de YHWH vuestro Dios, y estara alli en ti por TESTIGO."
 *
 * "ein ba'aron raq shene lujot ha'avanim asher hinniaj sham Moshe
 *  beJoreb..." - 1 Rey 8:9
 * "No habia en el Arca sino las DOS TABLAS DE PIEDRA..."
 *
 * --- TRES PALABRAS ---
 *
 *   MITSAD (H6654)   - AL LADO. Ni dentro (betoj), ni encima ('al).
 *                      torah.bytes es REFERENCIA, no copia:
 *                      vive al lado, no se inserta en el recinto.
 *   SEFER  (H5612)   - libro, registro completo (raiz: safar, contar).
 *                      Las tablas resumen; el sefer testifica completo.
 *   LE'ED  (H5707)   - POR TESTIGO. La torah no es adorno; testifica
 *                      contra la desobediencia (Dt 31:27). Por eso va
 *                      como hoja del Merkle bajo el zer - cualquier
 *                      alteracion queda delatada.
 *
 * --- ARBOL MERKLE DEL ARCA ---
 *
 *     [raiz] <- zer.hash (moldura saviv, Ex 25:11)
 *     /     \
 *   edut   torah       <- 2 hojas = 2 testigos (Dt 19:15)
 *    |       |
 *  dentro  al lado     <- mitsad, no betoj
 *   256 B  variable
 *  siempre  temporal   <- 1 Rey 8:9: solo las tablas quedan
 *
 * Si torah.bytes == NULL -> leaf_torah = SHA-256("") - determinista,
 * la moldura sigue "saviv" incluso sin segunda hoja. (Esa logica
 * vive en arca_zer.c; aqui orquestamos la adjuncion / desvinculacion
 * y el intercambio del root entre peers.)
 *
 * --- 4 FUNCIONES ---
 *
 *   torah_adjuntar          Dt 31:26       (publico)
 *   torah_desvincular       1 Rey 8:9      (interno)
 *   merkle_raiz_exportar    Ex 25:11       (interno - replicacion)
 *   merkle_comparar         Dt 19:15       (interno - replicacion)
 *
 * --- DEPENDENCIAS ---
 *
 *   arca_core.c      -> arca_mut / get_arca
 *   arca_zer.c       -> zer_calcular (recalcular al cambiar contenido)
 *                      zer_verificar (validar antes de exportar/comparar)
 *   mbedtls/sha256.h -> oro de Egipto, para torah.zer_externo
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <inttypes.h>

#include "mbedtls/sha256.h"    /* ORO DE EGIPTO - SHA-256 para zer_externo */
#include "esp_log.h"

#include "hashmal_storage.h"
#include "hashmal_storage_internal.h"

/* ==================================================================
 * ENCAPSULACION
 * ================================================================== */
static const char *MRK_TAG = "hashmal.arca.merkle";

#define MRK_LOG_I(fmt, ...)  ESP_LOGI(MRK_TAG, fmt, ##__VA_ARGS__)
#define MRK_LOG_W(fmt, ...)  ESP_LOGW(MRK_TAG, fmt, ##__VA_ARGS__)
#define MRK_LOG_E(fmt, ...)  ESP_LOGE(MRK_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * HELPERS ESTATICOS
 * ================================================================== */

/**
 * SHA-256 one-shot, encapsulando mbedtls. Duplicamos la primitiva
 * (existe analoga en arca_zer.c) para mantener cada archivo
 * autocontenido - cada modulo es responsable de su propia puerta
 * al oro de Egipto.
 *
 * @param in   buffer de entrada; puede ser NULL si len == 0
 * @param len  bytes (0 valido -> SHA-256 del vacio, determinista)
 * @param out  salida de 32 bytes (no NULL)
 * @return 0 en exito.
 */
static int sha256_leaf(const uint8_t *in, size_t len,
                       uint8_t out[HASHMAL_ZER_HASH_BYTES])
{
    if (out == NULL) return -1;
    if (in == NULL && len > 0) return -1;    /* len>0 exige buffer */

    mbedtls_sha256_context ctx;
    mbedtls_sha256_init(&ctx);

    int rc = mbedtls_sha256_starts(&ctx, 0);
    if (rc == 0 && len > 0) rc = mbedtls_sha256_update(&ctx, in, len);
    if (rc == 0)            rc = mbedtls_sha256_finish(&ctx, out);

    mbedtls_sha256_free(&ctx);
    return rc;
}

/**
 * Validacion basica de formato. El header define 3 valores validos
 * (acacia en 3 lenguas: hebreo, arameo, griego - v.10 shittim).
 * Cualquier otro valor es formato inventado.
 */
static bool formato_legitimo(hashmal_formato_t f)
{
    return (f == HASHMAL_FORMATO_HEBREO)
        || (f == HASHMAL_FORMATO_ARAMEO)
        || (f == HASHMAL_FORMATO_GRIEGO);
}

/* ==================================================================
 * API PUBLICA - torah_adjuntar (Dt 31:26)
 * ================================================================== */

/**
 * hashmal_storage_torah_adjuntar - Deuteronomio 31:26
 *
 * Coloca el sefer haTora AL LADO del Arca (mitsad), no dentro
 * (betoj). La struct guarda una REFERENCIA al buffer - los bytes
 * viven donde el llamante los tiene; nosotros no copiamos. Esto
 * honra "mitsad" literalmente: al lado, no inserto en el recinto.
 *
 * --- CONTRATO DE PROPIEDAD ---
 *
 * El CALLER retiene la propiedad de `bytes`. Mientras la torah
 * este adjunta (hasta `torah_desvincular` o `deinit`), el caller
 * debe MANTENER VIVO Y SIN MODIFICAR ese buffer. Si lo muta, el
 * zer quedara roto (el Merkle no coincidira) - que es exactamente
 * el diseno: la moldura delata (v.11). Si lo libera sin
 * desvincular, hay dangling pointer; documentamos aqui para que
 * no pase.
 *
 * --- PASOS ---
 *
 *   a) validar Arca viva (Ex 40:3)
 *   b) validar bytes != NULL y tamano > 0 (un libro vacio no se
 *      adjunta - el sefer debe EXISTIR para ser testigo)
 *   c) validar formato (acacia en 3 lenguas)
 *   d) guardar referencia + tamano + formato
 *   e) marcar al_lado = true (mitsad reafirmado)
 *   f) calcular torah.zer_externo = SHA-256(torah.bytes) - hash
 *      propio de la segunda hoja, util para diagnostico
 *   g) recalcular el zer completo - el Merkle root cambia porque
 *      ahora hay una segunda hoja con contenido real
 *
 * 2 testigos:
 *   Dt 31:26 - "vesamtem oto mitsad aron... vehaya le'ed"
 *   Dt 31:27 - "ki anoji yada'ti et meryeja" (testifica contra
 *              la rebeldia - por eso va bajo el zer)
 */
int hashmal_storage_torah_adjuntar(const uint8_t *bytes, size_t tamano,
                                   hashmal_formato_t formato)
{
    hashmal_arca_t *arca = hashmal_storage_arca_mut();
    if (arca == NULL) {
        MRK_LOG_E("torah_adjuntar: Arca no levantada (Éx 40:3 pendiente)");
        return HASHMAL_ARCA_ERR_INIT;
    }

    /* b) - el sefer debe existir. Un testigo vacio no testifica. */
    if (bytes == NULL || tamano == 0u) {
        MRK_LOG_E("torah_adjuntar: séfer vacío — no se adjunta un "
                  "libro sin contenido (Dt 31:26)");
        return HASHMAL_ARCA_ERR_NULL;
    }

    /* Triple vakhetsí — Éx 25:10: storage inherentemente incompleto sin
     * consensus (2.5×1.5×1.5). Kapóret (v.17) sella con footprint
     * idéntico (2.5×1.5). qomáh permanentemente abierta = siempre
     * necesita consensus. Prerequisito de existencia: sin Kapóret
     * encima, no hay recinto donde adjuntar el sefer mitsád. */
    if (arca->requiere_kaporet) {
        MRK_LOG_E("torah_adjuntar: Kapóret NO colocada (Éx 25:21) — "
                  "consensus prerequisito antes de cualquier adjunción");
        return HASHMAL_ARCA_ERR_SIN_KAPORET;
    }

    /* c) - v.10 shittim: acacia legitima = una de las 3 lenguas
     * en que la Palabra fue entregada. Cualquier otra es madera
     * inventada, no shittim. */
    if (!formato_legitimo(formato)) {
        MRK_LOG_E("torah_adjuntar: formato %d ilegítimo "
                  "(v.10 shittím: hebreo/arameo/griego)", (int)formato);
        return HASHMAL_ARCA_ERR_NO_AUTORIZADO;
    }

    /* d-e) - guardar referencia. MITSAD: no copiamos. El sefer vive
     * al lado, no dentro del recinto. El caller es el dueno. */
    arca->torah.bytes   = bytes;
    arca->torah.tamano  = tamano;
    arca->torah.formato = formato;
    arca->torah.al_lado = true;    /* Dt 31:26 - mitsad reafirmado */

    /* f) - zer_externo: hash propio de esta hoja. No es el Merkle
     * root (ese es arca->zer.hash, que cubre edut + torah). Es un
     * sello local de la segunda hoja, util si un peer necesita
     * identificarla sin descargar el contenido. */
    int rc = sha256_leaf(bytes, tamano, arca->torah.zer_externo);
    if (rc != 0) {
        MRK_LOG_E("torah_adjuntar: SHA-256 de hoja falló (mbedtls=%d) — "
                  "revirtiendo adjunción", rc);
        arca->torah.bytes  = NULL;
        arca->torah.tamano = 0u;
        memset(arca->torah.zer_externo, 0, HASHMAL_ZER_HASH_BYTES);
        return -1;
    }

    /* g) - recalcular el zer completo. La moldura ahora rodea dos
     * hojas con contenido. Si edut aun no esta sellada, esa hoja
     * es SHA-256(ceros) - determinista; el root cambiara de todos
     * modos al cambiar la hoja de torah. */
    uint8_t nueva_raiz[HASHMAL_ZER_HASH_BYTES];
    rc = hashmal_storage_zer_calcular(nueva_raiz);
    if (rc != HASHMAL_ARCA_OK) {
        MRK_LOG_E("torah_adjuntar: zer_calcular falló (rc=%d) — "
                  "revirtiendo adjunción", rc);
        arca->torah.bytes  = NULL;
        arca->torah.tamano = 0u;
        memset(arca->torah.zer_externo, 0, HASHMAL_ZER_HASH_BYTES);
        /* recalcular una segunda vez para restaurar la raiz sin torah */
        (void)hashmal_storage_zer_calcular(NULL);
        return rc;
    }

    MRK_LOG_I("séfer haTorá adjuntado mitsád arón (Dt 31:26): "
              "%" PRIu32 " bytes, formato=%d",
              (uint32_t)tamano, (int)formato);
    MRK_LOG_I("  vehayá leʻéd — ahora testifica bajo el zer");
    return HASHMAL_ARCA_OK;
}

/* ==================================================================
 * API INTERNA - torah_desvincular (1 Rey 8:9)
 * ================================================================== */

/**
 * hashmal_storage_torah_desvincular - 1 Reyes 8:9
 *
 * "ein ba'aron raq shene lujot ha'avanim asher hinniaj sham
 *  Moshe beJoreb..." - 1 Rey 8:9
 * "No habia en el Arca sino las DOS TABLAS DE PIEDRA que Moises
 *  puso alli en Horeb..."
 *
 * En tiempos de Salomon el mana y la vara de Aaron ya no estaban;
 * solo las tablas permanecian. La Palabra SIEMPRE queda. Lo
 * accesorio se va - el nucleo es eterno.
 *
 * --- QUE HACE ---
 *
 *   a) limpiar la referencia al sefer (bytes=NULL, tamano=0,
 *      zer_externo a cero)
 *   b) mantener al_lado = true (la invariante "mitsad" es del
 *      CONTENEDOR, no del contenido: el slot sigue siendo el
 *      lugar-al-lado, aunque ahora este vacio)
 *   c) RECALCULAR el zer - el Merkle tree vuelve a una hoja real
 *      (edut) + una hoja vacia (SHA-256 del vacio). saviv persiste.
 *
 * NO se toca la edut. Nunca. 1 Rey 8:9 es la frontera.
 *
 * 2 testigos:
 *   1 Rey 8:9 - solo las tablas permanecen
 *   Ex 25:11  - zer saviv tambien sin segunda hoja
 */
int hashmal_storage_torah_desvincular(void)
{
    hashmal_arca_t *arca = hashmal_storage_arca_mut();
    if (arca == NULL) {
        MRK_LOG_E("torah_desvincular: Arca no levantada");
        return HASHMAL_ARCA_ERR_INIT;
    }

    if (arca->torah.bytes == NULL && arca->torah.tamano == 0u) {
        /* Ya desvinculada. Idempotencia - 1 Rey 8:9 describe un
         * estado, no una operacion; volver a el es no-op. */
        MRK_LOG_W("torah_desvincular: torah ya vacía (idempotente, no-op)");
        return HASHMAL_ARCA_OK;
    }

    /* a) - limpiar referencia. La torah deja de testificar. */
    arca->torah.bytes  = NULL;
    arca->torah.tamano = 0u;
    memset(arca->torah.zer_externo, 0, HASHMAL_ZER_HASH_BYTES);

    /* b) - al_lado permanece: el lugar-al-lado es del recinto. */
    arca->torah.al_lado = true;

    /* c) - recalcular el zer. La moldura sigue rodeando (saviv):
     * ahora sobre 1 hoja real (edut) + hoja vacia determinista. */
    int rc = hashmal_storage_zer_calcular(NULL);
    if (rc != HASHMAL_ARCA_OK) {
        MRK_LOG_E("torah_desvincular: zer_calcular falló tras "
                  "desvinculación (rc=%d)", rc);
        return rc;
    }

    MRK_LOG_I("torah desvinculada (1 Rey 8:9): "
              "'ein baʼarón raq shené lujót'");
    MRK_LOG_I("  edut PERMANECE — solo el séfer se ha ido");
    return HASHMAL_ARCA_OK;
}

/* ==================================================================
 * API INTERNA - merkle_raiz_exportar (Ex 25:11)
 * ================================================================== */

/**
 * hashmal_storage_merkle_raiz_exportar - la moldura, vista desde fuera
 *
 * "ve'asita 'alav zer zahav saviv" - Ex 25:11c
 * La moldura de oro es VISIBLE. Lo que rodea la madera se puede
 * mirar desde fuera; lo que esta dentro, no. Aqui:
 *
 *   - se recalcula y VERIFICA el zer (nunca exportamos stale/roto)
 *   - se copia el root (32 bytes) al buffer del llamante
 *   - no se expone nada del contenido (edut/torah) - solo el hash
 *
 * Es la primitiva que hashmal_comm (Menora) usara para anunciar a
 * un peer "este es mi root" antes de negociar una replica.
 *
 * 2 testigos:
 *   Ex 25:11  - el zer es visible saviv (externo)
 *   Dt 19:15  - dos testigos establecen el asunto (sigue en
 *               merkle_comparar)
 */
int hashmal_storage_merkle_raiz_exportar(uint8_t out_hash[HASHMAL_ZER_HASH_BYTES])
{
    if (out_hash == NULL) {
        MRK_LOG_E("merkle_raiz_exportar: out_hash NULL");
        return HASHMAL_ARCA_ERR_NULL;
    }

    const hashmal_arca_t *arca = hashmal_storage_get_arca();
    if (arca == NULL) {
        MRK_LOG_E("merkle_raiz_exportar: Arca no levantada");
        return HASHMAL_ARCA_ERR_INIT;
    }

    /* Verificar antes de exportar: si mi zer no coincide con mi
     * contenido actual, estoy roto - exportar un root asi seria
     * contaminar al peer. Mejor fallar ruidosamente aqui. */
    int rc = hashmal_storage_zer_verificar();
    if (rc != HASHMAL_ARCA_OK) {
        MRK_LOG_E("merkle_raiz_exportar: zer local inconsistente "
                  "(rc=%d) — no se anuncia root roto", rc);
        return rc;
    }

    memcpy(out_hash, arca->zer.hash, HASHMAL_ZER_HASH_BYTES);
    MRK_LOG_I("merkle root exportado (zer visible savív, Éx 25:11); "
              "nonce=%" PRIu64, arca->zer.nonce);
    return HASHMAL_ARCA_OK;
}

/* ==================================================================
 * API INTERNA - merkle_comparar (Dt 19:15)
 * ================================================================== */

/**
 * hashmal_storage_merkle_comparar - los dos testigos (Dt 19:15)
 *
 * "'al pi shenei 'edim o 'al pi shelosha 'edim yaqum davar" - Dt 19:15
 * "Por boca de DOS testigos o de tres testigos se establecera el
 *  asunto."
 *
 * Dos nodos, dos roots. Si coinciden -> el contenido es el mismo en
 * ambos (colision SHA-256 asumida inviable). Si no -> divergencia:
 * alguien cambio algo, o alguien miente. ERR_ZER_ROTO.
 *
 * Aplicado en recepcion de replica (ve'asu, v.10 plural): el peer
 * envia su root; este nodo compara con el suyo. Si concuerdan, la
 * replica es coherente; si no, se detiene.
 *
 * Verificamos primero NUESTRO zer: si el local esta roto, no tiene
 * sentido comparar - estariamos juzgando con testigo contaminado.
 *
 * 2 testigos:
 *   Dt 19:15 - dos testigos establecen
 *   Ex 25:11 - el zer rodea (lo que se compara es la moldura)
 */
int hashmal_storage_merkle_comparar(const uint8_t remoto_hash[HASHMAL_ZER_HASH_BYTES])
{
    if (remoto_hash == NULL) {
        MRK_LOG_E("merkle_comparar: remoto_hash NULL");
        return HASHMAL_ARCA_ERR_NULL;
    }

    const hashmal_arca_t *arca = hashmal_storage_get_arca();
    if (arca == NULL) {
        MRK_LOG_E("merkle_comparar: Arca no levantada");
        return HASHMAL_ARCA_ERR_INIT;
    }

    /* Mi testimonio debe estar firme antes de juzgar al del otro. */
    int rc = hashmal_storage_zer_verificar();
    if (rc != HASHMAL_ARCA_OK) {
        MRK_LOG_E("merkle_comparar: zer local roto (rc=%d) — "
                  "no se compara con testigo contaminado", rc);
        return rc;
    }

    if (memcmp(remoto_hash, arca->zer.hash, HASHMAL_ZER_HASH_BYTES) != 0) {
        /* Divergencia entre nodos. No decimos aqui QUIEN cambio:
         * solo que el davar no se establece. El disputante superior
         * (protocolos de reconciliacion futuros) decidira. */
        MRK_LOG_E("merkle_comparar: DIVERGENCIA — roots no coinciden "
                  "(Dt 19:15 sin consenso)");
        return HASHMAL_ARCA_ERR_ZER_ROTO;
    }

    MRK_LOG_I("merkle_comparar: dos testigos concuerdan "
              "(Dt 19:15 — yaqúm davár)");
    return HASHMAL_ARCA_OK;
}
