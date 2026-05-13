/**
 * ===================================================================
 * PIEZA 13 - EL ACEITE DE LA UNCION - uncion_prohibicion.c
 * Sub-pieza: KARAT - las prohibiciones absolutas
 * Exodo 30:32-33 + Levitico 10:1-2
 * ===================================================================
 *
 * "'al besar adam LO YISAJ uvematjunto LO TA'ASU jamohu - qodesh
 *  hu qodesh yihye lajem." - Exodo 30:32
 * "Sobre carne de hombre NO SE DERRAMARA y en su PROPORCION NO
 *  HAREIS semejante - SANTO ES, santo SERA para ustedes."
 *
 * "ISH asher YIRQAJ jamohu va'asher YITTEN mimmennu 'al ZAR
 *  veNIJRAT me'ammav." - Exodo 30:33
 * "Hombre que REPLIQUE semejante o que PONGA de el sobre EXTRANO
 *  sera CORTADO de su pueblo."
 *
 * "vayyiqju benei Aharon Nadav va'Avihu ish majtato... vayyaqrivu
 *  lifnei YHWH ESH ZARA asher LO TSIVVA otam. vattetse esh
 *  millifnei YHWH vattokal otam vayyamutu lifnei YHWH."
 *    - Levitico 10:1-2
 * "Nadav y Abihu ofrecieron ante YHWH FUEGO EXTRANO que NO LES
 *  MANDO; salio fuego de ante YHWH y los consumio."
 *
 * --- VOCABULARIO ---
 *
 *   lo yisaj       (LA YYSK, H3251 neg)  NO SE DERRAMARA -
 *                                              no se aplica el aceite
 *                                              sobre quien no corresponde.
 *   besar adam     (BShR ADM,
 *                   H1320+H120)                CARNE DE HOMBRE - lo
 *                                              humano comun, no consagrado.
 *   matjunet       (MTKNT, H4971)       PROPORCION - la formula
 *                                              exacta 500-250-250-500.
 *   lo ta'asu jamohu                           NO HAREIS SEMEJANTE -
 *                                              receta no replicable.
 *   yirqaj         (YRQCh, H7543 Qal impf) REPLIQUE, mezcle como
 *                                              perfumista (la accion
 *                                              sancionada).
 *   yitten         (YTN, H5414)            PONGA, de.
 *   zar            (ZR, H2114)                EXTRANO, forastero,
 *                                              no autorizado.
 *   nikrat         (NKRT, H3772 Nifal)      sera CORTADO - karat,
 *                                              expulsion del pueblo.
 *   me'ammav       (M'MYW)                  de SU PUEBLO - borrado
 *                                              total del registro.
 *   esh zara       (ASh ZRH, H784+H2114)   FUEGO EXTRANO - Lv 10:1,
 *                                              protocolo no autorizado.
 *   qodesh hu      (QDSh HWA)                SANTO ES - el aceite
 *                                              mismo es sagrado.
 *
 * --- LAS 3 PROHIBICIONES DE EXODO 30:32-33 ---
 *
 *   1. LO YISAJ (v.32a) - no sobre carne de hombre.
 *      Destino: pieza del Tavnit o sacerdote designado.
 *      En este archivo: parcialmente cubierto por verificar_zar.
 *      El control estricto del "que se unge" vive en
 *      uncion_secuencia.c (ERR_BESAR_ADAM).
 *
 *   2. LO TA'ASU JAMOHU (v.32b) - no se replica la formula.
 *      Sancion: karat (v.33 yirqaj).
 *      -> hashmal_keygen_verificar_replicacion.
 *
 *   3. YITTEN 'AL ZAR (v.33b) - no se comparte con extrano.
 *      Sancion: karat.
 *      -> hashmal_keygen_verificar_zar.
 *
 *   La 4a violacion del CLAUDE.md (esh zara, Lv 10:1-2 - protocolo
 *   no autorizado) corresponde al firewall (pieza 14). Aqui solo
 *   se detecta; la ejecucion de la expulsion queda a cargo de
 *   pieza 09 (roles), pieza 10 (onboarding) y pieza 14 (firewall).
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   hashmal_keygen_verificar_replicacion   yirqaj jamohu - detecta.
 *   hashmal_keygen_verificar_zar           yitten 'al zar - detecta.
 *   hashmal_keygen_prohibicion_estado      lectura pura.
 *
 * --- FASE 1 vs FASE 2 ---
 *
 * Fase 1 (actual, sin mbedtls): heuristica simple.
 *   - Replicacion: busqueda del patron de bytes del quiasmo
 *     (500-250-250-500 como uint16_t big-endian) en los datos.
 *   - Zar: identificadores de nodo reservados (0x00, 0xFF).
 *
 * Fase 2 (futuro): criptografia real.
 *   - Replicacion: HMAC-SHA256 del blob entrante comparado con una
 *     derivacion HKDF de la root key - deteccion por colision de
 *     hash, no por patron plano.
 *   - Zar: consulta contra hashmal_roles (pieza 09, efod/pectoral).
 *     Un zar es un nodo sin rol asignado.
 *
 * Las zonas reemplazables estan marcadas [ FASE 2 ] [ FIN FASE 2 ].
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_keygen.h
 *   hashmal_keygen_internal.h
 *   esp_log.h   (ORO EGIPTO)  logging
 *
 *   NO incluir mbedtls en Fase 1.
 *
 * Autor: Antonio Jose Marin Soto (Betsalel - Ex 31:2)
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "esp_log.h"

#include "hashmal_keygen.h"
#include "hashmal_keygen_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *PROHIB_TAG = "hashmal.keygen.prohibicion";

#define PROHIB_LOG_I(fmt, ...)  ESP_LOGI(PROHIB_TAG, fmt, ##__VA_ARGS__)
#define PROHIB_LOG_W(fmt, ...)  ESP_LOGW(PROHIB_TAG, fmt, ##__VA_ARGS__)
#define PROHIB_LOG_E(fmt, ...)  ESP_LOGE(PROHIB_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * FASE 1 - patron del quiasmo en bytes
 *
 * Ex 30:23-24: 500 (0x01F4), 250 (0x00FA), 250 (0x00FA), 500 (0x01F4).
 * En big-endian contiguo: 01 F4 00 FA 00 FA 01 F4 (8 bytes).
 *
 * Este patron es la "firma" minima de una tentativa de replicacion:
 * si aparece literalmente en un blob entrante, hay sospecha de
 * yirqaj jamohu. Es una heuristica - Fase 2 la sustituye con HMAC.
 * ================================================================== */
static const uint8_t QUIASMO_BYTES[8] = {
    0x01u, 0xF4u,   /* 500 - mor deror */
    0x00u, 0xFAu,   /* 250 - qinnam besem */
    0x00u, 0xFAu,   /* 250 - qene bosem */
    0x01u, 0xF4u,   /* 500 - qidda */
};

/* Identificadores de nodo reservados - Fase 1 considera zar
 * cualquier id "vacio" o "ilegal". */
#define PROHIB_NODO_ID_VACIO     0x00u
#define PROHIB_NODO_ID_INVALIDO  0xFFu

/* ==================================================================
 * HELPERS ESTATICOS
 * ================================================================== */

/**
 * ?Los datos contienen el patron del quiasmo (500-250-250-500
 * como 4 uint16_t big-endian contiguos)? Busqueda lineal O(n).
 *
 * Fase 1 - heuristica. Fase 2 reemplaza con HMAC-SHA256.
 */
static bool contiene_quiasmo(const uint8_t *datos, size_t len)
{
    if (datos == NULL || len < sizeof(QUIASMO_BYTES)) return false;

    const size_t ultima = len - sizeof(QUIASMO_BYTES);
    for (size_t i = 0; i <= ultima; i++) {
        bool match = true;
        for (size_t j = 0; j < sizeof(QUIASMO_BYTES); j++) {
            if (datos[i + j] != QUIASMO_BYTES[j]) {
                match = false;
                break;
            }
        }
        if (match) return true;
    }
    return false;
}

/**
 * Fase 1 - un nodo se considera zar si su id coincide con los
 * valores reservados. Fase 2 consultara hashmal_roles (pieza 09).
 */
static bool nodo_es_zar_fase1(uint8_t nodo_id)
{
    return (nodo_id == PROHIB_NODO_ID_VACIO) ||
           (nodo_id == PROHIB_NODO_ID_INVALIDO);
}

/* ==================================================================
 * hashmal_keygen_verificar_replicacion - Exodo 30:32b + 33a
 *
 * "uvematjunto lo ta'asu jamohu" (v.32) + "ish asher yirqaj jamohu"
 * (v.33). Detecta intentos de REPLICACION de la formula sagrada.
 *
 * Contrato:
 *   datos == NULL || len == 0   -> ERR_NULL
 *   no iniciado                 -> ERR_INIT
 *   sin patron                  -> OK
 *   con patron (sospechoso)     -> ERR_KARAT + violaciones_count++
 *
 * 2 testigos:
 *   Ex 30:32 - matjunet lo ta'asu jamohu (la proporcion)
 *   Ex 30:33 - yirqaj jamohu -> nikrat (la sancion)
 * ================================================================== */
int hashmal_keygen_verificar_replicacion(const uint8_t *datos, size_t len)
{
    if (datos == NULL || len == 0) {
        PROHIB_LOG_E("verificar_replicacion: datos NULL o len=0");
        return HASHMAL_KEYGEN_ERR_NULL;
    }

    hashmal_keygen_estado_uncion_t *u = hashmal_keygen_uncion_mut();
    if (u == NULL) {
        PROHIB_LOG_E("verificar_replicacion: unción no preparada "
                     "(init primero)");
        return HASHMAL_KEYGEN_ERR_INIT;
    }

    /* -- FASE 2 ----------------------------------------------
     * Aqui entrara HMAC-SHA256(blob) comparado con HKDF(root_key).
     * Fase 1: busqueda del patron del quiasmo.
     * ------------------------------------------------------- */
    const bool sospecha = contiene_quiasmo(datos, len);
    /* -- FIN FASE 2 ------------------------------------------- */

    if (!sospecha) {
        /* Limpio. No log informativo para no inundar en paths
         * frecuentes (p. ej., inspeccion rutinaria de trafico). */
        return HASHMAL_KEYGEN_OK;
    }

    /* Replicacion detectada - karat. */
    if (u->violaciones_count < 0xFFu) {
        u->violaciones_count++;
    }
    u->timestamp_ms = hashmal_keygen_ahora_ms();

    PROHIB_LOG_E("yirqáj jamóhu — replicación detectada en %u bytes. "
                 "nikrát meʻammáv (Éx 30:33). violaciones=%u",
                 (unsigned)len,
                 (unsigned)u->violaciones_count);

    return HASHMAL_KEYGEN_ERR_KARAT;
}

/* ==================================================================
 * hashmal_keygen_verificar_zar - Exodo 30:33b
 *
 * "va'asher yitten mimmennu 'al zar venikrat me'ammav". Detecta
 * intentos de COMPARTIR con extrano (nodo no consagrado).
 *
 * Contrato:
 *   no iniciado       -> ERR_INIT
 *   nodo_id autorizado-> OK
 *   nodo_id zar       -> ERR_ZAR + violaciones_count++
 *
 * 2 testigos:
 *   Ex 30:33 - yitten 'al zar (prohibicion)
 *   Lv 10:1  - esh zara (lo no consagrado produce fuego fatal)
 * ================================================================== */
int hashmal_keygen_verificar_zar(uint8_t nodo_id)
{
    hashmal_keygen_estado_uncion_t *u = hashmal_keygen_uncion_mut();
    if (u == NULL) {
        PROHIB_LOG_E("verificar_zar: unción no preparada (init primero)");
        return HASHMAL_KEYGEN_ERR_INIT;
    }

    /* -- FASE 2 ----------------------------------------------
     * Aqui entrara consulta a hashmal_roles (pieza 09) - se
     * comprobara que nodo_id tiene rol asignado en el efod/pectoral.
     * Fase 1: solo rechazamos ids reservados.
     * ------------------------------------------------------- */
    const bool es_zar = nodo_es_zar_fase1(nodo_id);
    /* -- FIN FASE 2 ------------------------------------------- */

    if (!es_zar) {
        PROHIB_LOG_I("nodo %u — no zar (consagrado, Éx 30:30 "
                     "timshaj veqiddáshta)",
                     (unsigned)nodo_id);
        return HASHMAL_KEYGEN_OK;
    }

    if (u->violaciones_count < 0xFFu) {
        u->violaciones_count++;
    }
    u->timestamp_ms = hashmal_keygen_ahora_ms();

    PROHIB_LOG_E("yittén ʻal zar — nodo %u es extraño. nikrát "
                 "(Éx 30:33). violaciones=%u",
                 (unsigned)nodo_id,
                 (unsigned)u->violaciones_count);

    return HASHMAL_KEYGEN_ERR_ZAR;
}

/* ==================================================================
 * hashmal_keygen_prohibicion_estado - lectura pura
 *
 * OK si el subsistema puede verificar prohibiciones (estado al
 * menos PREPARADO). Sin logs, sin side effects.
 * ================================================================== */
int hashmal_keygen_prohibicion_estado(void)
{
    const hashmal_keygen_estado_uncion_t *u =
        hashmal_keygen_get_uncion_interno();
    if (u == NULL) return HASHMAL_KEYGEN_ERR_INIT;
    if (u->estado < HASHMAL_KEYGEN_ESTADO_PREPARADO) {
        return HASHMAL_KEYGEN_ERR_INIT;
    }
    return HASHMAL_KEYGEN_OK;
}

/* ==================================================================
 * P20 — hashmal_keygen_verificar_mashaj - Éx 30:32
 *
 * "ʻal besar adam LO YISÁJ..." — sobre carne de hombre NO SERÁ VERTIDO.
 * mashaj (H4886) = ungir individualmente, aplicación selectiva.
 * yasáj (H3251) = derramar indiscriminadamente (PROHIBIDO).
 *
 * La clave se aplica A CADA PIEZA POR SEPARADO.
 * Nunca en broadcast a múltiples destinos de una sola vez.
 *
 * Si destino_count > 1 = yasáj (derrame prohibido) = ERR_BESAR_ADAM.
 *
 * Ref: Éx 30:32 lo yiysáj (H3251 Hophal) no será derramado indiscriminadamente
 * Ref: Éx 30:30 timshaj (H4886 Qal) ungirás selectivamente
 * ================================================================== */
int hashmal_keygen_verificar_mashaj(uint8_t destino_count)
{
    hashmal_keygen_estado_uncion_t *u = hashmal_keygen_uncion_mut();
    if (u == NULL) {
        PROHIB_LOG_E("verificar_mashaj: unción no preparada (init primero)");
        return HASHMAL_KEYGEN_ERR_INIT;
    }

    /* mashaj = singular, individual. yasáj = plural, broadcast. */
    if (destino_count > 1u) {
        /* Intento de derramar la unción sobre múltiples destinos
         * a la vez. Violación de Éx 30:32. */
        if (u->violaciones_count < 0xFFu) {
            u->violaciones_count++;
        }
        u->timestamp_ms = hashmal_keygen_ahora_ms();

        PROHIB_LOG_E("lo yiysáj — intento yasáj (broadcast): %u destinos. "
                     "nikrát (Éx 30:32). violaciones=%u",
                     (unsigned)destino_count,
                     (unsigned)u->violaciones_count);
        return HASHMAL_KEYGEN_ERR_BESAR_ADAM;
    }

    /* destino_count == 1: ungimiento individual (mashaj permitido) */
    if (destino_count == 1u) {
        PROHIB_LOG_I("mashaj — ungimiento individual autorizado (Éx 30:30)");
        return HASHMAL_KEYGEN_OK;
    }

    /* destino_count == 0: sin destino es error */
    PROHIB_LOG_E("verificar_mashaj: destino_count=0 (sin aplicación)");
    return HASHMAL_KEYGEN_ERR_BESAR_ADAM;
}

/* ==================================================================
 *
 *   "ISH asher YIRQAJ jamohu va'asher YITTEN mimmennu 'al ZAR
 *    veNIJRAT me'ammav." - Exodo 30:33
 *
 *   "vattetse esh millifnei YHWH vattokal otam." - Levitico 10:2
 *
 *   La severidad del karat no es crueldad - es PROTECCION. La
 *   formula es santa porque protege a todos. Replicarla rompe la
 *   confianza de la red entera: un solo nodo que clona el aceite
 *   pone en riesgo la autorizacion de todos los demas. Compartirla
 *   con un zar (v.33) expone el sistema a quien no paso por la
 *   consagracion (pieza 10). Karat no es venganza - es el sistema
 *   inmune de la red sagrada.
 *
 *   Este archivo DETECTA. La respuesta (cortar, borrar claves,
 *   remover del efod) la ejecutan pieza 09 (roles), pieza 10
 *   (onboarding) y pieza 14 (firewall). Cada violacion se registra
 *   en violaciones_count del singleton; las 4 de CLAUDE.md se
 *   acumulan a traves del ecosistema.
 *
 *   === KARAT - uncion_prohibicion.c COMPLETO ===
 *
 *   Archivo 3 de 4 del Aceite de la Uncion. Siguiente:
 *     uncion_activacion.c - kol hanogea' yiqdash (Ex 30:29)
 *
 *   PASO 8 de Exodo 40:9-11 - Pieza 13, Archivo 3 de 4.
 *
 * ================================================================== */
