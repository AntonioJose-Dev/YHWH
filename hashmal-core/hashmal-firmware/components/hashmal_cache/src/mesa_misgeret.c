/**
 * ===================================================================
 * PIEZA 03 - MESA - mesa_misgeret.c
 * Sub-pieza: EL MARCO DE UN PALMO - contencion, doble proteccion
 * Exodo 25:25 + Exodo 25:24 + Malaquias 1:7,12 + 2 Cronicas 13:11
 * ===================================================================
 *
 * "ve'asita lo MISGERET TOFAJ saviv ve'asita zer zahav lemisggarto
 *  saviv" - Ex 25:25
 * "Le haras un MARCO de un PALMO alrededor, y haras una corona de
 *  oro SOBRE su marco alrededor."
 *
 *   misgeret (MSGRT)  raiz SGR (SGR) = cerrar, encerrar, CONTENER
 *   tofaj    (TPCh)      un palmo: MEDIDA FIJA (no elastica)
 *   saviv    (SBYB)     alrededor - cubre todos los lados
 *   zer lemisggarto       corona SOBRE el marco: doble frontera
 *
 * --- POR QUE LA MESA TIENE MISGERET Y EL ARCA NO ---
 *
 * La Mesa esta en el QODESH (Lugar Santo), mas expuesta que el
 * Arca (Qodesh haQodashim). El flujo de acceso a la Mesa es mucho
 * mas alto (lehem panim tamid: pan continuo, servido a los
 * sacerdotes - Lv 24:9). Por eso lleva misgeret: contencion que
 * impide que el exceso corrompa la pureza.
 *
 *   Ex 25:24 - "zer zahav saviv"         (corona exterior)
 *   Ex 25:25 - "misgeret tofaj saviv"    (marco de contencion)
 *   Ex 25:25 - "zer zahav lemisggarto"   (corona SOBRE el marco)
 *
 * Tres capas. En firmware: el zer exterior (zer_exterior del
 * shuljan) es la corona externa; el misgeret es el rate limiter;
 * el zer sobre misgeret queda implicito al recalcular el zer
 * exterior tras cada mutacion (el zer cubre TODO, incluidos los
 * parametros del marco).
 *
 * --- VENTANA DESLIZANTE ---
 *
 * Implementacion: fixed window con reset por vencimiento. Cada
 * llamada a `permitir`:
 *   1. Si la ventana actual expiro (ahora - ts_inicio >= ventana_ms):
 *      reinicia la ventana (contador=0, ts_inicio=ahora).
 *   2. Si el contador < max_ventana: permite e incrementa.
 *   3. Si el contador == max_ventana: rechaza (ERR_MISGERET).
 *
 * El tofaj (palmo) NO es la duracion - es la CAPACIDAD. La duracion
 * viene de HASHMAL_MISGERET_VENTANA_MS. "Un palmo" (medida fija) se
 * operacionaliza como "N peticiones por ventana" - tambien fija.
 *
 * --- SUPORTE PROFETICO ---
 *
 * Malaquias 1:7,12 - "shuljan YHWH nivze hu": "la mesa de YHWH es
 * despreciable." Los sacerdotes descuidaron su servicio; descuidar
 * la contencion es despreciar la provision.
 *
 * 2 Cronicas 13:11 - "hashuljan hattahor": "la mesa PURA." La
 * pureza requiere que no se sature - el misgeret preserva.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_cache.h              -> struct + API publica
 *   hashmal_cache_internal.h     -> shuljan_mut, get_shuljan, ahora_ms
 *   esp_log.h (ORO DE EGIPTO)    -> MESA_LOG_*
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
#include "hashmal_cache_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *MESA_TAG = "hashmal.mesa.misgeret";

#define MESA_LOG_I(fmt, ...)  ESP_LOGI(MESA_TAG, fmt, ##__VA_ARGS__)
#define MESA_LOG_W(fmt, ...)  ESP_LOGW(MESA_TAG, fmt, ##__VA_ARGS__)
#define MESA_LOG_E(fmt, ...)  ESP_LOGE(MESA_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * API PUBLICA - misgeret_permitir (Ex 25:25 - tofaj saviv)
 * ================================================================== */

/**
 * hashmal_cache_misgeret_permitir - ?hay espacio dentro del marco?
 *
 * Llamada por cada mutadora de mesa_api.c (qe'ara, kaf, qasa,
 * menaqit) antes de trabajar. Implementa ventana fija con reset
 * por vencimiento:
 *
 *   ts = s->misgeret.timestamp_ventana_ms
 *   if (ahora - ts) >= ventana_ms:
 *       nueva ventana (contador=0, ts=ahora)
 *   if contador < max:  permitir (contador++) -> OK
 *   else:               rechazar -> ERR_MISGERET
 *
 * Efecto de borde cuando el reloj "salta atras" (improbable con
 * monotonic, pero defensivo): si ahora < ts (unsigned wraparound
 * semantico produciria gigantes), lo tratamos como ventana nueva
 * para no quedar atrapados en un rechazo permanente.
 */
int hashmal_cache_misgeret_permitir(void)
{
    hashmal_shuljan_t *s = hashmal_cache_shuljan_mut();
    if (s == NULL) {
        MESA_LOG_E("misgeret_permitir: Mesa no levantada");
        return HASHMAL_CACHE_ERR_INIT;
    }

    hashmal_misgeret_t *m = &s->misgeret;
    const uint64_t ahora = hashmal_cache_ahora_ms();

    /* Detectar ventana vencida. Dos condiciones: tiempo elapsed >=
     * duracion, O reloj hacia atras (ts en futuro respecto a ahora
     * -> aunque improbable con monotonic, no queremos bloquear). */
    const bool reloj_hacia_atras = (ahora < m->timestamp_ventana_ms);
    const bool ventana_vencida   = reloj_hacia_atras
        || ((ahora - m->timestamp_ventana_ms) >= (uint64_t)m->ventana_ms);

    if (ventana_vencida) {
        m->peticiones_ventana   = 0u;
        m->timestamp_ventana_ms = ahora;
        /* log a INFO (no warning): es comportamiento normal del
         * ciclo - el marco se "limpia" cada ventana. */
    }

    /* Capacidad. tofaj = medida FIJA: max_ventana es la encarnacion
     * operacional del "palmo" - no negociable en runtime. */
    if (m->peticiones_ventana >= m->max_ventana) {
        MESA_LOG_W("misgéret LLENO: %" PRIu32 "/%" PRIu32 " peticiones en "
                   "ventana de %" PRIu32 " ms (v.25 — tófaj savív)",
                   m->peticiones_ventana,
                   m->max_ventana,
                   m->ventana_ms);
        return HASHMAL_CACHE_ERR_MISGERET;
    }

    m->peticiones_ventana += 1u;
    return HASHMAL_CACHE_OK;
}

/* ==================================================================
 * API INTERNA - misgeret_reset (nueva ventana forzada)
 * ================================================================== */

/**
 * hashmal_cache_misgeret_reset - nueva ventana EXPLICITA
 *
 * Usado por mesa_disponibilidad.c al renovar el ciclo Shabat
 * (Lv 24:8 - "ya'arejennu"). Al inicio de un nuevo ciclo semanal,
 * el marco se limpia - el exceso del ciclo anterior no debe
 * penalizar al nuevo.
 *
 * No altera tofaj, max_ventana ni ventana_ms (son v.25, diseno
 * fijo). Solo resetea los contadores de la ventana activa.
 */
int hashmal_cache_misgeret_reset(void)
{
    hashmal_shuljan_t *s = hashmal_cache_shuljan_mut();
    if (s == NULL) {
        MESA_LOG_E("misgeret_reset: Mesa no levantada");
        return HASHMAL_CACHE_ERR_INIT;
    }

    s->misgeret.peticiones_ventana   = 0u;
    s->misgeret.timestamp_ventana_ms = hashmal_cache_ahora_ms();

    MESA_LOG_I("misgéret reset — nueva ventana (capacidad=%" PRIu32
               ", duración=%" PRIu32 " ms)",
               s->misgeret.max_ventana, s->misgeret.ventana_ms);
    return HASHMAL_CACHE_OK;
}

/* ==================================================================
 * API INTERNA - misgeret_estado (lectura para telemetria)
 * ================================================================== */

/**
 * hashmal_cache_misgeret_estado - lectura simple
 *
 * Pensado para health checks (pieza 12 - Kiyor / hashmal_healthcheck,
 * futuro). Cualquier puntero de salida puede ser NULL; solo se
 * escribe en los no-NULL. No muta: lectura pura.
 *
 * 2 Cr 13:11 "hashuljan hattahor" - la mesa PURA: para vigilarla
 * hay que poder INSPECCIONAR sin TOCAR. Esa es la funcion de
 * estado.
 */
int hashmal_cache_misgeret_estado(uint32_t *peticiones,
                                  uint32_t *max_out,
                                  uint32_t *ventana_ms_out)
{
    const hashmal_shuljan_t *s = hashmal_cache_get_shuljan();
    if (s == NULL) {
        MESA_LOG_E("misgeret_estado: Mesa no levantada");
        return HASHMAL_CACHE_ERR_INIT;
    }

    if (peticiones     != NULL) *peticiones     = s->misgeret.peticiones_ventana;
    if (max_out        != NULL) *max_out        = s->misgeret.max_ventana;
    if (ventana_ms_out != NULL) *ventana_ms_out = s->misgeret.ventana_ms;

    return HASHMAL_CACHE_OK;
}
