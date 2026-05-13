/**
 * ===================================================================
 * PIEZA 03 - MESA - mesa_disponibilidad.c
 * Sub-pieza: EL CICLO DEL SHABAT - renovacion periodica, berit 'olam
 * Levitico 24:8-9 + Exodo 25:30 + Exodo 25:24 + Nehemias 5:17
 * ===================================================================
 *
 * "beyom hashabbat beyom hashabbat YA'AREJENNU lifne YHWH TAMID
 *  me'et benei Yisra'el BERIT 'OLAM" - Lv 24:8
 * "Cada SHABAT, cada SHABAT lo RENOVARA delante de YHWH
 *  CONTINUAMENTE, de parte de los hijos de Israel - PACTO ETERNO."
 *
 *   beyom hashabbat beyom hashabbat
 *     - repeticion que enfatiza la PERIODICIDAD inviolable
 *   ya'arejennu (Y'RKNW)
 *     - raiz 'araj: ORDENAR, DISPONER. No "cambiar" - disponer.
 *   tamid   - continuo; el flujo no se suspende
 *   berit 'olam - pacto ETERNO, juqqat 'olam (Lv 24:9b): el ciclo
 *               no vence con las dinastias. Sigue.
 *
 * "vehayeta le'Aharon ulevanav ve'AJALAHU bemaqom qadosh ki qodesh
 *  qodashim hu lo..." - Lv 24:9
 * "Sera para Aaron y sus hijos, y lo COMERAN en lugar santo, porque
 *  santidad de santidades es para el..."
 *
 * El pan viejo NO se tira - SE COME. Los sacerdotes consumen lo
 * que salio del altar. En firmware: los slots EXPIRADO se vuelven
 * VACIO (liberacion), no se "destruyen" en ningun sentido malo -
 * simplemente se consumen dando lugar al siguiente.
 *
 * "venatata 'al hashuljan lehem panim lefanai TAMID" - Ex 25:30
 * "Pondras sobre la mesa pan de la presencia delante de Mi
 *  continuamente."
 *
 * "zer zahav saviv" - Ex 25:24: la corona exterior RODEA la Mesa.
 * En firmware: Merkle root sobre los 12 zer_slot.
 *
 * "vehayyehudim vehasseganim me'a vajamishshim ish... 'al
 *  shuljani" - Neh 5:17: Nehemias sostuvo mesa comunitaria
 * continua - modelo de provision sin interrupcion.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   shabat_renovar             ya'arejennu periodico       (publico)
 *   shabat_vencido             lectura del vencimiento      (interno)
 *   zer_exterior_recalcular    Merkle agregado (v.24)       (interno)
 *
 * --- TRANSICIONES DE ESTADO EN EL SHABAT ---
 *
 *   EXPIRADO -> VACIO     (pan viejo "comido", Lv 24:9)
 *   SERVIDO  -> EXPIRADO  (ya leido; si no se refresca, siguiente
 *                         Shabat lo retira)
 *   FRESCO   -> FRESCO    (pan bueno no servido PERMANECE; no se
 *                         descarta provision legitima)
 *   VACIO    -> VACIO     (sin cambio)
 *
 * El Shabat NO borra datos - los PROCESA segun su estado. Diseno
 * coherente con Lv 24:9: el viejo se consume, el nuevo sube, el
 * que aun sirve se queda.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_cache.h              -> struct + API publica
 *   hashmal_cache_internal.h     -> shuljan_mut, get_shuljan, ahora_ms,
 *                                  misgeret_reset (nueva ventana tras ciclo)
 *   hashmal_storage.h            -> HASHMAL_ZER_HASH_BYTES
 *   mbedtls/sha256.h (ORO EGIPTO)-> Merkle exterior
 *   esp_log.h (ORO EGIPTO)       -> MESA_LOG_*
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <inttypes.h>

#include "mbedtls/sha256.h"
#include "esp_log.h"

#include "hashmal_cache.h"
#include "hashmal_cache_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *MESA_TAG = "hashmal.mesa.disponibilidad";

#define MESA_LOG_I(fmt, ...)  ESP_LOGI(MESA_TAG, fmt, ##__VA_ARGS__)
#define MESA_LOG_W(fmt, ...)  ESP_LOGW(MESA_TAG, fmt, ##__VA_ARGS__)
#define MESA_LOG_E(fmt, ...)  ESP_LOGE(MESA_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * HELPER ESTATICO - SHA-256 (corona exterior)
 * ================================================================== */

/**
 * SHA-256 one-shot. Encapsulacion por modulo (mismo patron que
 * arca_zer.c / arca_merkle.c / arca_sealed_core.c / mesa_api.c).
 */
static int sha256_once(const uint8_t *in, size_t len,
                       uint8_t out[HASHMAL_ZER_HASH_BYTES])
{
    if (out == NULL) return -1;
    if (in == NULL && len > 0u) return -1;

    mbedtls_sha256_context ctx;
    mbedtls_sha256_init(&ctx);

    int rc = mbedtls_sha256_starts(&ctx, 0);
    if (rc == 0 && len > 0u) rc = mbedtls_sha256_update(&ctx, in, len);
    if (rc == 0)             rc = mbedtls_sha256_finish(&ctx, out);

    mbedtls_sha256_free(&ctx);
    return rc;
}

/* ==================================================================
 * API INTERNA - shabat_vencido (lectura pura)
 * ================================================================== */

/**
 * hashmal_cache_shabat_vencido - ?toca ya'arejennu?
 *
 * true si el tiempo transcurrido desde el ultimo refresco alcanza o
 * supera shabat_ciclo_ms. Reloj hacia atras (improbable con
 * monotonic) tambien cuenta como "vencido" - hay que recalcular
 * cuando el tiempo se extrana.
 *
 * No muta. Lectura para el driver periodico y para health checks
 * (Kiyor detectara "Shabat vencido sin renovar" como senal de
 * enfermedad del cache).
 */
bool hashmal_cache_shabat_vencido(void)
{
    const hashmal_shuljan_t *s = hashmal_cache_get_shuljan();
    if (s == NULL) return false;

    const uint64_t ahora    = hashmal_cache_ahora_ms();
    const uint64_t ultimo   = s->shabat_ultimo_refresco_ms;
    const uint32_t ciclo    = s->shabat_ciclo_ms;

    if (ahora < ultimo) {
        /* Reloj hacia atras - tratamos como vencido, defensivo. */
        return true;
    }
    return (ahora - ultimo) >= (uint64_t)ciclo;
}

/* ==================================================================
 * API INTERNA - zer_exterior_recalcular (v.24 corona agregada)
 * ================================================================== */

/**
 * hashmal_cache_zer_exterior_recalcular - Ex 25:24
 *
 * "zer zahav saviv" - la corona RODEA la Mesa entera. En firmware:
 *
 *   zer_exterior = SHA-256( zer_slot[0] || zer_slot[1] || ... || zer_slot[11] )
 *
 * Doce slots x 32 bytes = 384 bytes concatenados; un solo hash
 * final de 32 bytes. Cualquier cambio en cualquier slot mueve la
 * corona - el zer exterior delata.
 *
 * Diseno de refresco: esta funcion la llama el Shabat (consistencia
 * eventual dentro del ciclo). mesa_api.c puede invocarla tras cada
 * mutacion si se desea corona siempre fresca - trade-off entre
 * coste por escritura y ventana de inconsistencia transitoria.
 */
int hashmal_cache_zer_exterior_recalcular(void)
{
    hashmal_shuljan_t *s = hashmal_cache_shuljan_mut();
    if (s == NULL) {
        MESA_LOG_E("zer_exterior: Mesa no levantada");
        return HASHMAL_CACHE_ERR_INIT;
    }

    /* Concatenacion de los 12 zer_slot. 384 bytes en stack - cabe
     * holgado en el stack de una task ESP-IDF (>= 4 KB por defecto). */
    uint8_t concat[HASHMAL_SHULJAN_SLOTS * HASHMAL_ZER_HASH_BYTES];
    for (uint8_t i = 0; i < HASHMAL_SHULJAN_SLOTS; ++i) {
        memcpy(&concat[i * HASHMAL_ZER_HASH_BYTES],
               s->slots[i].zer_slot,
               HASHMAL_ZER_HASH_BYTES);
    }

    uint8_t nuevo[HASHMAL_ZER_HASH_BYTES];
    int rc = sha256_once(concat, sizeof(concat), nuevo);
    if (rc != 0) {
        MESA_LOG_E("zer_exterior: SHA-256 falló (mbedtls=%d)", rc);
        return HASHMAL_CACHE_ERR_ZER_ROTO;
    }

    memcpy(s->zer_exterior, nuevo, HASHMAL_ZER_HASH_BYTES);
    s->zer_nonce += 1u;

    MESA_LOG_I("zer exterior recalculado (Merkle sobre 12 zer_slot, v.24); "
               "nonce=%" PRIu64, s->zer_nonce);
    return HASHMAL_CACHE_OK;
}

/* ==================================================================
 * API PUBLICA - shabat_renovar (Lv 24:8 - ya'arejennu)
 * ================================================================== */

/**
 * hashmal_cache_shabat_renovar - "beyom hashabbat ya'arejennu"
 *
 * Ciclo de renovacion periodica. Pasos:
 *
 *   1. Mesa levantada.
 *   2. ?Shabat vencido? Si NO -> no-op con log (aun no toca).
 *   3. Procesar los 12 slots:
 *        EXPIRADO -> VACIO     (Lv 24:9: se come el viejo)
 *        SERVIDO  -> EXPIRADO  (ya leido; si no se refresca, cae)
 *        FRESCO   -> FRESCO    (se queda: provision legitima)
 *        VACIO    -> VACIO
 *   4. misgeret_reset (nueva ventana para el nuevo ciclo)
 *   5. shabat_ultimo_refresco_ms = ahora
 *   6. zer_exterior_recalcular (la corona refleja el nuevo estado)
 *
 * 2 testigos:
 *   Lv 24:8 - ya'arejennu... berit 'olam (ciclo perpetuo)
 *   Lv 24:9 - ve'ajalahu... juqqat 'olam (consumo legitimo del viejo)
 *
 * Verificado #03 - Cadena 4 - DUPLICACION TEMPORAL (SEMANAL):
 *   "beYOM hashabbat beYOM hashabbat ya'arejennu" - Lv 24:8.
 *   beyom hashabbat aparece DUPLICADO. La duplicacion no es
 *   redundancia hebrea: es ENFASIS DE INMUTABILIDAD del ciclo.
 *   Trio del Padre:
 *     diario   -> babboqer babboqer        (Ex 30:7,  comm)
 *     semanal  -> beyom hashabbat x2       (Lv 24:8,  este sitio)
 *     anual    -> ajat bashana ajat bashana (Ex 30:10, escudo YK)
 *   En firmware: el caller no decide cada cuanto se ejecuta este
 *   refresh - es Shabat o no es. La cadencia es del Padre, no del
 *   nodo. Romperla seria profanar el ciclo (Ex 31:14 mejaleleha
 *   mot yumat).
 */
int hashmal_cache_shabat_renovar(void)
{
    hashmal_shuljan_t *s = hashmal_cache_shuljan_mut();
    if (s == NULL) {
        MESA_LOG_E("shabat_renovar: Mesa no levantada");
        return HASHMAL_CACHE_ERR_INIT;
    }

    /* ?ya toca? */
    if (!hashmal_cache_shabat_vencido()) {
        const uint64_t ahora  = hashmal_cache_ahora_ms();
        const uint64_t faltan = (ahora < s->shabat_ultimo_refresco_ms)
            ? 0ull
            : (uint64_t)s->shabat_ciclo_ms
              - (ahora - s->shabat_ultimo_refresco_ms);
        MESA_LOG_W("shabat_renovar: aún no es Shabat (faltan ~%" PRIu64
                   " ms) — no-op", faltan);
        return HASHMAL_CACHE_OK;
    }

    /* 3) - procesar estados. Contamos transiciones para el log. */
    uint32_t n_comidos   = 0u;   /* EXPIRADO -> VACIO */
    uint32_t n_expirados = 0u;   /* SERVIDO  -> EXPIRADO */
    uint32_t n_quedan    = 0u;   /* FRESCO permanece */
    uint32_t n_vacios    = 0u;   /* VACIO permanece */

    for (uint8_t i = 0; i < HASHMAL_SHULJAN_SLOTS; ++i) {
        hashmal_lejem_t *l = &s->slots[i];

        switch (l->estado) {

        case HASHMAL_LEJEM_EXPIRADO:
            /* Lv 24:9 - "ve'ajalahu": se come el viejo. En firmware:
             * liberar el slot. Preservamos la azjara (memorial,
             * Lv 24:7) porque registra la HISTORIA, no el contenido. */
            memset(l->bytes,    0, sizeof(l->bytes));
            memset(l->zer_slot, 0, HASHMAL_ZER_HASH_BYTES);
            l->tamano                = 0u;
            l->timestamp_horneado_ms = 0u;
            l->timestamp_servido_ms  = 0u;
            l->estado                = HASHMAL_LEJEM_VACIO;
            /* azjara permanece - memorial perdura */
            n_comidos++;
            break;

        case HASHMAL_LEJEM_SERVIDO:
            /* Ya fue leido; si nadie lo refresco, en el proximo
             * Shabat se retirara. Transicion a EXPIRADO - contenido
             * se preserva hasta el siguiente ciclo para auditoria. */
            l->estado = HASHMAL_LEJEM_EXPIRADO;
            n_expirados++;
            break;

        case HASHMAL_LEJEM_FRESCO:
            /* Provision aun no servida - NO se descarta pan bueno.
             * Permanece intacto. */
            n_quedan++;
            break;

        case HASHMAL_LEJEM_VACIO:
        default:
            n_vacios++;
            break;
        }
    }

    /* 4) - misgeret: nueva ventana para el ciclo entrante. */
    (void)hashmal_cache_misgeret_reset();

    /* 5) - marcador temporal del refresco. */
    const uint64_t ahora = hashmal_cache_ahora_ms();
    s->shabat_ultimo_refresco_ms = ahora;

    /* 6) - corona exterior refleja el estado post-Shabat. */
    int rc = hashmal_cache_zer_exterior_recalcular();
    if (rc != HASHMAL_CACHE_OK) {
        /* No revertimos las transiciones: el estado logico es
         * valido, solo la corona agregada no pudo calcularse. Lo
         * reportamos pero el ciclo Shabat se consumo. */
        MESA_LOG_E("shabat_renovar: zer_exterior falló (rc=%d); "
                   "estados actualizados pero corona stale", rc);
    }

    MESA_LOG_I("═══ 'beyóm hashabbát yaʻarejénnu' (Lv 24:8) — RENOVADO ═══");
    MESA_LOG_I("  comidos (EXP→VAC): %" PRIu32 " (Lv 24:9)",   n_comidos);
    MESA_LOG_I("  expirados (SRV→EXP): %" PRIu32,               n_expirados);
    MESA_LOG_I("  FRESCO permanece:  %" PRIu32 " (provisión)",  n_quedan);
    MESA_LOG_I("  VACIO sin cambio:  %" PRIu32,                 n_vacios);
    MESA_LOG_I("  berít ʻolám — ciclo eterno; próximo Shabat en "
               "%" PRIu32 " ms", s->shabat_ciclo_ms);
    return HASHMAL_CACHE_OK;
}

/* ==================================================================
 *
 *   PIEZA 03 - SHULJAN (MESA DEL PAN) - COMPLETA.
 *
 *   4 archivos, una mesa:
 *
 *     mesa_cache.c            v.23-28 + Lv 24:5-6 - construir
 *     mesa_api.c              v.29 - 4 utensilios CRUD
 *     mesa_misgeret.c         v.25 - marco de un palmo
 *     mesa_disponibilidad.c   Lv 24:8 - ciclo Shabat
 *
 *   "venatata 'al hashuljan lehem panim lefanai tamid" - Ex 25:30
 *   "Pondras sobre la mesa pan de la presencia delante de Mi
 *    continuamente."
 *
 *   "beyom hashabbat beyom hashabbat ya'arejennu... berit 'olam"
 *     - Lv 24:8
 *
 *   lehem panim tamid - berit 'olam.
 *   El pan esta servido. La mesa no se vacia. El ciclo no se detiene.
 *
 *   Piezas 01, 02 y 03 listas - Arca, Kaporet, Mesa.
 *   Exodo 40:2-4 cumplido en su parte central.
 *
 * ================================================================== */
