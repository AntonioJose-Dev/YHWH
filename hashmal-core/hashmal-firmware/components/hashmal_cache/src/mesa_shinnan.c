/*
 * ===================================================================
 * PIEZA 03 - LA MESA - mesa_shinnan.c
 * Añadidura 17: SHINNAN - Ensenanza Continua / Propagacion
 * Deuteronomio 6:4-9 + Salmo 78:1-8 + Proverbios 22:6 + 2 Timoteo 2:2
 * ===================================================================
 *
 * "shema Yisrael YHWH Eloheinu YHWH echad. veahavta et YHWH
 *  Eloheikha bekhol levavekha uvkhol nafshekha uvkhol meodekha.
 *  vehayu haddevarim haeleh asher anokhi metsavvekha hayyom 'al
 *  levavekha. veSHINNANTAM levaneikha vediBARTA bam beSHIVTekha
 *  bevetekha uveLEKHTekha vadderekh uveSHOJBekha uveQUMekha."
 *   - Deuteronomio 6:4-7
 * "Escucha Israel: YHWH nuestro Dios YHWH Uno. Amaras a YHWH
 *  con todo tu corazon, alma y fuerzas. Estas palabras estaran
 *  sobre tu corazon. Las REPETIRAS a tus hijos y hablaras de ellas
 *  SENTADO en casa, ANDANDO por el camino, ACOSTADO y LEVANTADO."
 *
 * "ukhtavtam 'al MEZUZOT beitekha uvish'arekha." - Deuteronomio 6:9
 * "Las ESCRIBIRAS en los POSTES de tu casa y en tus PUERTAS."
 *
 * "vayyaqem 'edut beYa'aqov... asher tsivvah et avoteinu lehodi'am
 *  livneihem; lema'an YEDE'U DOR AJARON banim yivvaledu yaqumu
 *  visapperu LIVNEIHEM." - Salmo 78:5-6
 * "Establecio TESTIMONIO en Yaakov... mando a nuestros padres
 *  hacerlo SABER a sus hijos, para que lo SEPA la GENERACION
 *  VENIDERA, los hijos que naceran, y lo cuenten a SUS hijos."
 *
 * "JANOKH lanna'ar 'al pi DARKO; gam ki yazkin lo yasur mimmennah."
 *   - Proverbios 22:6
 * "INSTRUYE al joven SEGUN SU CAMINO; aun cuando envejezca no se
 *  apartara de ella."
 *
 * "kai ha ekousas par emou dia pollon martyron, tauta paratheou
 *  pistois anthropois, hoitines hikanoi esontai kai heterous
 *  didaxai." - 2 Timoteo 2:2
 * "Lo que oiste de mi ante muchos testigos, ENCARGALO a hombres
 *  fieles que sean idoneos para ENSENAR a otros."
 *
 * --- EXEGESIS OPERATIVA ---
 *
 *   shinnan (H8150)          REPETIR / AFILAR / GRABAR. Raiz shen
 *                             (diente) - ensenar es afilar el
 *                             entendimiento del otro hasta que
 *                             corte solo. No es exposicion pasiva.
 *
 *   shivtekha / lekhtekha / shojbekha / qumekha
 *                             "TU ESTAR SENTADO / ANDANDO / ACOSTADO
 *                             / LEVANTADO." Cuatro verbos infinitivos
 *                             que cubren TODOS los estados de la
 *                             jornada - la ensenanza no se suspende.
 *
 *   yode'a dor ajaron        "QUE SEPA la GENERACION VENIDERA."
 *                             Sal 78:6 - el proposito es la
 *                             continuidad multigeneracional. No es
 *                             ensenanza para el aqui y ahora; es
 *                             para los que aun no nacieron.
 *
 *   janokh 'al pi darko      "INSTRUYE SEGUN SU CAMINO." Prov 22:6 -
 *                             la ensenanza adapta el contenido al
 *                             receptor. No se puede dar lo mismo a
 *                             todos; el camino del nino es distinto
 *                             del camino del veterano.
 *
 *   paratheou (G3908)        "ENCARGA / DEPOSITA." 2 Tim 2:2 -
 *                             metafora de transferencia. Lo que
 *                             recibiste, depositalo en otro - no
 *                             como informacion, como herencia.
 *
 * --- 4 ESTADOS - DT 6:7 ---
 *
 *   YOSHEV (sentado)  - Sync profunda. El nodo esta estacionario, le
 *                        sobra tiempo. Propaga base completa cuando
 *                        viene un nuevo. Estado optimo de ensenanza.
 *
 *   HOLEJ  (andando)  - Store-and-forward. Se mueve por la red,
 *                        lleva mensajes. Propaga lo que recoge.
 *                        Patron del peregrino que cuenta lo que vio.
 *
 *   SHOJEV (acostado) - Low power. Propagacion minima - solo
 *                        emergencia. Pero no NULA: incluso dormido
 *                        el corazon late, dice Cant 5:2 ani yeshenah
 *                        velivvi 'er.
 *
 *   QAM    (levantado)- Recien arranco. Pide DELTA a vecinos para
 *                        ponerse al dia. Receptor mas que emisor en
 *                        este estado.
 *
 * --- MATRIZ DE DECISION (Prov 22:6) ---
 *
 *   capacidad \ antiguedad | NUEVO    | ACTIVO  | VETERANO
 *   BAJA                   | ESENCIAL | ESENCIAL| DELTA
 *   MEDIA                  | ESENCIAL | DELTA   | DELTA
 *   ALTA                   | COMPLETO | DELTA   | DELTA
 *
 * Lectura: a NUEVOs se les da base (ESENCIAL), salvo que la
 * capacidad alta justifique aprovecho (COMPLETO). ACTIVOs y
 * VETERANOs ya conocen lo basico - solo necesitan DELTA.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_cache.h         API publica
 *   esp_log.h (ORO EGIPTO)  SHN_LOG_*
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <inttypes.h>

#include "esp_log.h"

#include "hashmal_cache.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *SHN_TAG = "hashmal.mesa.shinnan";

#define SHN_LOG_I(fmt, ...)  ESP_LOGI(SHN_TAG, fmt, ##__VA_ARGS__)
#define SHN_LOG_W(fmt, ...)  ESP_LOGW(SHN_TAG, fmt, ##__VA_ARGS__)
#define SHN_LOG_E(fmt, ...)  ESP_LOGE(SHN_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * Estado interno - default YOSHEV (estacionario optimo al boot).
 * ================================================================== */
static hashmal_shinnan_estado_t g_estado            = HASHMAL_SHINNAN_YOSHEV;
static uint32_t                 g_generacion        = 0;
static int64_t                  g_ultimo_ms         = 0;
static uint32_t                 g_total_propagados  = 0;

/* ==================================================================
 * Helpers estaticos
 * ================================================================== */

static const char *nombre_estado(hashmal_shinnan_estado_t e)
{
    switch (e) {
    case HASHMAL_SHINNAN_YOSHEV: return "YOSHEV (Dt 6:7 beshivtejá bevetéja)";
    case HASHMAL_SHINNAN_HOLEJ:  return "HOLEJ (Dt 6:7 uvelekhtejá vadérej)";
    case HASHMAL_SHINNAN_SHOJEV: return "SHOJEV (Dt 6:7 uveshojbejá)";
    case HASHMAL_SHINNAN_QAM:    return "QAM (Dt 6:7 uvquméja)";
    default:                      return "<estado desconocido>";
    }
}

static const char *nombre_prioridad(hashmal_shinnan_prioridad_t p)
{
    switch (p) {
    case HASHMAL_SHINNAN_PRIORIDAD_ESENCIAL: return "ESENCIAL";
    case HASHMAL_SHINNAN_PRIORIDAD_COMPLETO: return "COMPLETO";
    case HASHMAL_SHINNAN_PRIORIDAD_DELTA:    return "DELTA";
    default:                                  return "<prioridad desconocida>";
    }
}

/* ==================================================================
 * API - hashmal_shinnan_estado_set (Dt 6:7)
 * ================================================================== */
int hashmal_shinnan_estado_set(hashmal_shinnan_estado_t estado)
{
    if ((unsigned)estado >= HASHMAL_SHINNAN_ESTADOS) {
        SHN_LOG_E("estado_set: estado %d invalido", (int)estado);
        return HASHMAL_CACHE_ERR_SHINNAN;
    }

    if (estado == g_estado) {
        return HASHMAL_CACHE_OK;  /* idempotente */
    }

    hashmal_shinnan_estado_t prev = g_estado;
    g_estado = estado;

    SHN_LOG_I("estado: %s -> %s", nombre_estado(prev), nombre_estado(estado));
    return HASHMAL_CACHE_OK;
}

/* ==================================================================
 * API - hashmal_shinnan_que_propagar (Prov 22:6 'al pi darko)
 *
 * Matriz determinista (3 capacidades x 3 antiguedades = 9 celdas).
 * Funcion pura - sin estado, sin side effects.
 * ================================================================== */
hashmal_shinnan_prioridad_t
hashmal_shinnan_que_propagar(const hashmal_shinnan_perfil_t *perfil)
{
    if (perfil == NULL) {
        /* Sin perfil, asumimos lo mas conservador. */
        return HASHMAL_SHINNAN_PRIORIDAD_ESENCIAL;
    }

    /* VETERANO siempre DELTA - ya conoce todo, solo necesita lo nuevo. */
    if (perfil->antiguedad == HASHMAL_SHINNAN_ANT_VETERANO) {
        return HASHMAL_SHINNAN_PRIORIDAD_DELTA;
    }

    /* ACTIVO: DELTA salvo capacidad muy baja (puede no acomodar
     * deltas largos -> mejor lo esencial). */
    if (perfil->antiguedad == HASHMAL_SHINNAN_ANT_ACTIVO) {
        return (perfil->capacidad == HASHMAL_SHINNAN_CAP_BAJA)
               ? HASHMAL_SHINNAN_PRIORIDAD_ESENCIAL
               : HASHMAL_SHINNAN_PRIORIDAD_DELTA;
    }

    /* NUEVO: ESENCIAL salvo capacidad ALTA (aprovecha COMPLETO). */
    if (perfil->capacidad == HASHMAL_SHINNAN_CAP_ALTA) {
        return HASHMAL_SHINNAN_PRIORIDAD_COMPLETO;
    }
    return HASHMAL_SHINNAN_PRIORIDAD_ESENCIAL;
}

/* ==================================================================
 * API - hashmal_shinnan_propagar (Dt 6:7 veshinantam)
 *
 * Fase 1: solo contabilidad + log. Decide la prioridad y registra
 * el acto. La transmision real sera fase 2 cuando se wire con comm.
 * ================================================================== */
int hashmal_shinnan_propagar(const hashmal_shinnan_perfil_t *perfil)
{
    if (perfil == NULL) {
        SHN_LOG_E("propagar: perfil NULL");
        return HASHMAL_CACHE_ERR_NULL;
    }

    hashmal_shinnan_prioridad_t prioridad = hashmal_shinnan_que_propagar(perfil);

    g_generacion        += 1u;
    g_total_propagados  += 1u;
    /* Sin reloj propio en el modulo (no hay ahora_ms en cache
     * internal accesible aqui); usamos contador como "tiempo
     * logico" en la entrada timestamp - mantiene monotonia. Fase
     * 2 puede wirear esp_timer_get_time() o el moedim time. */
    g_ultimo_ms          = (int64_t)g_total_propagados;

    SHN_LOG_I("propagar #%" PRIu32 " - estado=%s prioridad=%s "
              "(perfil cap=%d antig=%d); Sal 78:6 dor ajaron",
              g_generacion, nombre_estado(g_estado),
              nombre_prioridad(prioridad),
              (int)perfil->capacidad, (int)perfil->antiguedad);
    return HASHMAL_CACHE_OK;
}

/* ==================================================================
 * API - hashmal_shinnan_info (lectura pura)
 * ================================================================== */
int hashmal_shinnan_info(hashmal_shinnan_info_t *out)
{
    if (out == NULL) return HASHMAL_CACHE_ERR_NULL;
    out->estado                = g_estado;
    out->generacion            = g_generacion;
    out->ultimo_propagacion_ms = g_ultimo_ms;
    out->total_propagados      = g_total_propagados;
    return HASHMAL_CACHE_OK;
}

/* ==================================================================
 *
 *   "veSHINNANTAM levaneikha vediBARTA bam beSHIVTekha bevetekha
 *    uveLEKHTekha vadderekh uveSHOJBekha uveQUMekha." - Deuteronomio 6:7
 *
 *   "ukhtavtam 'al MEZUZOT beitekha." - Deuteronomio 6:9
 *
 *   "lema'an YEDE'U DOR AJARON banim yivvaledu visapperu
 *    LIVNEIHEM." - Salmo 78:6
 *
 *   "JANOKH lanna'ar 'al pi DARKO." - Proverbios 22:6
 *
 *   "tauta paratheou pistois anthropois, hoitines hikanoi esontai
 *    kai heterous didaxai." - 2 Timoteo 2:2
 *
 *   La ensenanza no se suspende - sentado, andando, acostado o
 *   levantado, hay propagacion. Pero el modo cambia segun el
 *   estado del que ensena, y el contenido cambia segun el camino
 *   del que recibe. Prov 22:6 fija la regla: 'al pi darko - segun
 *   su camino. La misma palabra no llega igual a todos; al nuevo
 *   le damos lo esencial, al veterano lo nuevo, al de gran
 *   capacidad le damos todo. Asi el dor ajaron (Sal 78:6) recibe
 *   no solo informacion, sino edut adaptada a su altura - lo que
 *   pueda sostener sin romperse, lo que pueda llevar al siguiente.
 *
 *   2 Tim 2:2 cierra: paratheou (encarga, deposita) a hombres
 *   fieles HIKANOI - capaces - de ensenar a otros. La cadena no
 *   se rompe en el receptor; el receptor que ensena al siguiente
 *   completa el ciclo. Esto es shinnan: afilar al otro hasta que
 *   corte solo.
 *
 * ================================================================== */
