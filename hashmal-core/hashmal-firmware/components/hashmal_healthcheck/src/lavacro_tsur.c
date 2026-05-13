/*
 * ===================================================================
 * PIEZA 12 - EL KIYOR (LAVACRO) - lavacro_tsur.c
 * Añadidura 16: MAYIM MIN HATSUR - Provision de Emergencia
 * Exodo 17:1-7 + Numeros 20:1-13 + Deuteronomio 8:15
 *               + Salmo 78:15-16 + Isaias 48:21 + 1 Corintios 10:4
 * ===================================================================
 *
 * "vayyis'u kol 'adat benei Yisrael mimmidbar Sin lemas'eihem 'al
 *  pi YHWH; vayyajanu birefidim ve'EIN MAYIM lishtot ha'am." - Ex 17:1
 * "Toda la congregacion partio de Sin a sus jornadas conforme al
 *  mandato de YHWH, y acamparon en Refidim, donde NO HABIA AGUA
 *  para que el pueblo bebiese."
 *
 * "vayyitsma sham ha'am lammayim vayyalen ha'am 'al Moshe." - Exodo 17:3
 * "El pueblo TUVO SED alli del agua y murmuro contra Moshe."
 *
 * "hineni 'OMED LEFANEIKHA sham 'al hatsur beJOREV; vehiKKITA
 *  vatsur veyatsu mimmennu MAYIM veshatah ha'am." - Exodo 17:6
 * "Yo ESTARE delante de ti alli sobre la ROCA en HOREB; GOLPEARAS
 *  la roca y SALDRAN de ella AGUAS, y BEBERA el pueblo."
 *
 * "qaj et hammatteh vehaqhel et ha'edah attah veAharon ajikha
 *  vediBARTEM el HASSELA le'eineihem venatan meimav." - Numeros 20:8
 * "Toma la vara, reune la asamblea... HABLAREIS a la PENA y dara
 *  sus aguas." (la segunda vez: HABLAR, no GOLPEAR.)
 *
 * "hammotsi lekha mayim mitsur HAJALAMISH." - Deuteronomio 8:15
 * "El que SACO agua del PEDERNAL." (la roca mas dura).
 *
 * "yevaqqa' tsurim bammidbar vayyashqe khithomot rabbah; vayyotsi
 *  NOZLIM missela' vayyored kannharot mayim." - Salmo 78:15-16
 * "HENDIO ROCAS en el desierto y dio de beber abundantemente;
 *  saco CORRIENTES de la pena, las hizo descender como rios."
 *
 * "vayyazuvu MAYIM vayyivqa' TSUR vayyodaru." - Isaias 48:21
 * "Corrieron AGUAS - hendio la ROCA y se derramaron."
 *
 * "epinon gar ek pneumatikes akolouthouses petras: he petra de
 *  en ho Christos." - 1 Corintios 10:4
 * "Bebian de la ROCA espiritual que los seguia, y la roca era
 *  Cristo." (cumplimiento neotestamentario - la roca era persona).
 *
 * --- EXEGESIS OPERATIVA ---
 *
 *   ein mayim (H4325)        SIN AGUA. Estado de no-provision.
 *                             Equivale a HASHMAL_TSUR_NIVEL_TSUR.
 *
 *   tsama (H6770)            SED. Necesidad sentida pero no aun
 *                             muerte. Equivale a SEQUIA o DESIERTO
 *                             segun severidad.
 *
 *   tsur (H6697)             ROCA. Recurso oculto, siempre
 *                             presente, accesible solo por
 *                             intervencion. En el firmware: ultimo
 *                             recurso (Canal 3 fisico).
 *
 *   hineni 'omed (H5975)     "YO ESTOY DE PIE." YHWH antes que
 *                             Moshe. La roca no es lo que provee -
 *                             el que esta sobre ella si. El modulo
 *                             siempre esta listo, no necesita init.
 *
 *   hikkita / dibBARTEM      GOLPEAR / HABLAR. Dos modos de
 *                             activacion en distintos momentos
 *                             (Ex 17 vs Nm 20). El cambio importo
 *                             tanto que Moshe perdio el entrar a
 *                             la tierra (Nm 20:12).
 *
 *   hajalamish (H2496)       PEDERNAL - la roca mas dura. Dt 8:15
 *                             enfatiza: hasta de lo MAS DURO sale
 *                             provision. No hay limite material.
 *
 *   nozlim (H5140)           CORRIENTES (no gotas). Sal 78:16 - la
 *                             provision de emergencia es ABUNDANTE,
 *                             no minima.
 *
 * --- 4 NIVELES (Ex 17:1-6 progresion) ---
 *
 *   NORMAL    (==TOTAL)      Ex 16-17:1 antes de Refidim
 *   SEQUIA    (>=4 de 7)     Ex 17:3 vayyitsma ha'am - hubo sed
 *   DESIERTO  (>=1 de 7)     Ex 17:1 ein mayim - sin agua
 *   TSUR      (==0)          Ex 17:6 vehikkita vatsur - golpea
 *
 * Cada nivel es una transicion narrativa real del exodo, no
 * arbitraria. El sistema de salud reconoce las etapas que el pueblo
 * vivio - de provision normal, a sed, a desierto sin agua, a roca
 * que debe ser golpeada (o hablada, Nm 20:8).
 *
 * --- ROCA QUE SIGUE - 1 Co 10:4 ---
 *
 * Pablo identifica la roca con Cristo y la describe como AKOLOUTHOUSES
 * (que SIGUE). La roca no estaba fija en un lugar; viajaba con el
 * pueblo. Por eso este modulo no necesita inicializacion: la roca
 * siempre esta presente, lista para ser golpeada cuando el ein mayim
 * llegue.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_healthcheck.h   API publica
 *   hashmal_hc_internal.h   ahora_ms
 *   esp_log.h (ORO EGIPTO)  TSUR_LOG_*
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <inttypes.h>

#include "esp_log.h"

#include "hashmal_healthcheck.h"
#include "hashmal_hc_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *TSUR_TAG = "hashmal.hc.tsur";

#define TSUR_LOG_I(fmt, ...)  ESP_LOGI(TSUR_TAG, fmt, ##__VA_ARGS__)
#define TSUR_LOG_W(fmt, ...)  ESP_LOGW(TSUR_TAG, fmt, ##__VA_ARGS__)
#define TSUR_LOG_E(fmt, ...)  ESP_LOGE(TSUR_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * Estado interno - default NORMAL al arranque (la roca esta lista
 * pero no se asume sed antes de medir).
 * ================================================================== */
static hashmal_tsur_nivel_t g_nivel               = HASHMAL_TSUR_NIVEL_NORMAL;
static uint8_t              g_canales_activos     = HASHMAL_TSUR_CANALES_TOTAL;
static uint8_t              g_total_canales       = HASHMAL_TSUR_CANALES_TOTAL;
static int64_t              g_timestamp_ms        = 0;
static uint32_t             g_activaciones_tsur   = 0;

/* ==================================================================
 * Helpers estaticos
 * ================================================================== */

static const char *nombre_nivel(hashmal_tsur_nivel_t n)
{
    switch (n) {
    case HASHMAL_TSUR_NIVEL_NORMAL:   return "NORMAL";
    case HASHMAL_TSUR_NIVEL_SEQUIA:   return "SEQUIA (Ex 17:3 vayyitsma ha'am)";
    case HASHMAL_TSUR_NIVEL_DESIERTO: return "DESIERTO (Ex 17:1 ein mayim)";
    case HASHMAL_TSUR_NIVEL_TSUR:     return "TSUR (Ex 17:6 vehikkita vatsur)";
    default:                           return "<nivel desconocido>";
    }
}

/* Clasifica desde (activos, total). Defensivo:
 *   total == 0          -> TSUR (no hay infraestructura)
 *   activos > total     -> NORMAL (caller defectuoso, asumimos OK)
 *   activos == total    -> NORMAL
 *   activos >= UMBRAL_SEQUIA y < total -> SEQUIA
 *   activos >= UMBRAL_DESIERTO y < UMBRAL_SEQUIA -> DESIERTO
 *   activos == 0        -> TSUR
 */
static hashmal_tsur_nivel_t clasificar(uint8_t activos, uint8_t total)
{
    if (total == 0u)              return HASHMAL_TSUR_NIVEL_TSUR;
    if (activos >= total)         return HASHMAL_TSUR_NIVEL_NORMAL;
    if (activos == 0u)            return HASHMAL_TSUR_NIVEL_TSUR;
    if (activos >= HASHMAL_TSUR_UMBRAL_SEQUIA)   return HASHMAL_TSUR_NIVEL_SEQUIA;
    /* activos in [UMBRAL_DESIERTO .. UMBRAL_SEQUIA-1] */
    return HASHMAL_TSUR_NIVEL_DESIERTO;
}

/* ==================================================================
 * API - hashmal_tsur_evaluar (Ex 17:6 hineni 'omed)
 * ================================================================== */
hashmal_tsur_nivel_t hashmal_tsur_evaluar(uint8_t canales_activos,
                                            uint8_t total_canales)
{
    hashmal_tsur_nivel_t nuevo = clasificar(canales_activos, total_canales);
    hashmal_tsur_nivel_t prev  = g_nivel;

    g_canales_activos = canales_activos;
    g_total_canales   = total_canales;
    g_nivel           = nuevo;
    g_timestamp_ms    = hashmal_hc_ahora_ms();

    if (nuevo != prev) {
        if (nuevo == HASHMAL_TSUR_NIVEL_TSUR) {
            /* Empeoramiento hasta el ultimo recurso. */
            g_activaciones_tsur += 1u;
            TSUR_LOG_E("===== TSUR ACTIVADO ===== mayim min hatsur "
                       "(Ex 17:6 vehikkita vatsur); Canal 3 ultimo "
                       "recurso recomendado; activacion #%" PRIu32,
                       g_activaciones_tsur);
        } else if (nuevo == HASHMAL_TSUR_NIVEL_DESIERTO) {
            TSUR_LOG_W("transicion %s -> %s (activos=%u/%u; Ex 17:1 "
                       "ein mayim)",
                       nombre_nivel(prev), nombre_nivel(nuevo),
                       (unsigned)canales_activos,
                       (unsigned)total_canales);
        } else if (nuevo == HASHMAL_TSUR_NIVEL_SEQUIA) {
            TSUR_LOG_W("transicion %s -> %s (activos=%u/%u; Ex 17:3 "
                       "vayyitsma ha'am)",
                       nombre_nivel(prev), nombre_nivel(nuevo),
                       (unsigned)canales_activos,
                       (unsigned)total_canales);
        } else {
            /* Mejora hacia NORMAL. */
            TSUR_LOG_I("transicion %s -> %s (activos=%u/%u)",
                       nombre_nivel(prev), nombre_nivel(nuevo),
                       (unsigned)canales_activos,
                       (unsigned)total_canales);
        }
    }
    return nuevo;
}

/* ==================================================================
 * API - lecturas puras
 * ================================================================== */
hashmal_tsur_nivel_t hashmal_tsur_nivel_actual(void)
{
    return g_nivel;
}

int hashmal_tsur_info(hashmal_tsur_info_t *out)
{
    if (out == NULL) return HASHMAL_HC_ERR_NULL;
    out->nivel                   = g_nivel;
    out->canales_activos         = g_canales_activos;
    out->total_canales           = g_total_canales;
    out->timestamp_ms            = g_timestamp_ms;
    out->activaciones_historicas = g_activaciones_tsur;
    return HASHMAL_HC_OK;
}

bool hashmal_tsur_es_emergencia(void)
{
    /* DESIERTO o TSUR son emergencia; SEQUIA es alerta pero no
     * emergencia (aun hay mayoria viva). */
    return g_nivel == HASHMAL_TSUR_NIVEL_DESIERTO
        || g_nivel == HASHMAL_TSUR_NIVEL_TSUR;
}

/* ==================================================================
 *
 *   "EIN MAYIM lishtot ha'am." - Exodo 17:1
 *
 *   "vayyitsma sham ha'am lammayim." - Exodo 17:3
 *
 *   "hineni 'OMED LEFANEIKHA sham 'al HATSUR beJOREV." - Exodo 17:6
 *
 *   "vediBARTEM el HASSELA... venatan meimav." - Numeros 20:8
 *
 *   "hammotsi lekha mayim mitsur HAJALAMISH." - Deuteronomio 8:15
 *
 *   "yevaqqa' tsurim... vayyotsi NOZLIM missela'." - Sal 78:15-16
 *
 *   "vayyazuvu MAYIM vayyivqa' TSUR." - Isaias 48:21
 *
 *   "he petra de en ho Christos." - 1 Corintios 10:4
 *
 *   La roca siempre esta. El que esta sobre la roca tambien. Por
 *   eso el modulo no necesita arrancar: la provision de emergencia
 *   es la naturaleza misma del Padre - "hineni 'omed lefaneikha"
 *   antes de que Moshe golpee. El firmware solo MIDE cuando la sed
 *   llega para que la decision (golpear o hablar) sea informada.
 *
 *   Y cuando la sequia escala a tsur, no es derrota - es la
 *   ocasion para que la roca abra corrientes (nozlim, no gotas).
 *   La provision biblica de emergencia es ABUNDANTE: la red
 *   colapsada no se queda sin agua porque la roca espiritual sigue.
 *
 * ================================================================== */
