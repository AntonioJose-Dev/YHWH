/**
 * ═══════════════════════════════════════════════════════════════════
 * PIEZA 03 — MESA — hashmal_cache_internal.h
 * API INTERNA del componente — NO exportada fuera.
 * ═══════════════════════════════════════════════════════════════════
 *
 * Une símbolos compartidos entre los 4 archivos hermanos del
 * componente hashmal_cache. El mundo exterior solo ve
 * include/hashmal_cache.h; esto es privado.
 *
 * "veʻasíta shulján... vetsippíta otó zaháv tahór" — Éx 25:23-24
 * (oro externo — a diferencia del Arca, sin "mibáyit umijúts")
 *
 * Autor: Antonio José Marín Soto
 * Proyecto: HASHMAL-CORE
 */
#ifndef HASHMAL_CACHE_INTERNAL_H
#define HASHMAL_CACHE_INTERNAL_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "hashmal_cache.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ══════════════════════════════════════════════════════════════════
 * mesa_cache.c — singleton accessor mutable
 *
 * El getter público (hashmal_cache_get_shuljan) devuelve const.
 * Para que los archivos hermanos (mesa_api, mesa_misgeret,
 * mesa_disponibilidad) puedan actualizar estado (slots, misgéret,
 * timestamps, zer_exterior) usamos esta vía privada. NO SE EXPORTA
 * fuera del componente.
 * ══════════════════════════════════════════════════════════════════ */

/**
 * Acceso mutable al Shulján. Solo para archivos hermanos del
 * componente hashmal_cache. @return NULL si no inicializado.
 */
hashmal_shuljan_t* hashmal_cache_shuljan_mut(void);

/**
 * Monotonic clock en milisegundos. Encapsula esp_timer_get_time()
 * (microsegundos) y convierte. Compartido para que todos los
 * archivos del componente usen la misma unidad en los timestamps
 * del Shabat y la ventana del misgéret.
 */
uint64_t hashmal_cache_ahora_ms(void);

/**
 * Valida índice de slot. Devuelve true si slot ∈ [0, 11]
 * (Lv 24:5 — "shté ʻesré jallót").
 */
bool hashmal_cache_slot_legitimo(uint8_t slot);

/* ══════════════════════════════════════════════════════════════════
 * mesa_misgeret.c — el marco de un palmo (v.25)
 *
 * misgeret_permitir es pública (declarada en hashmal_cache.h). Las
 * dos siguientes son internas: reset explícito (para cierre/apertura
 * del ciclo Shabat en mesa_disponibilidad.c) y lectura de estado
 * para telemetría local.
 * ══════════════════════════════════════════════════════════════════ */

/**
 * Fuerza una nueva ventana del misgéret: peticiones_ventana = 0,
 * timestamp_ventana_ms = ahora_ms(). No altera tófaj, max_ventana ni
 * ventana_ms (son del diseño, v.25 — medida fija). Llamada al inicio
 * del ciclo Shabat o tras limpieza manual.
 *
 * @return HASHMAL_CACHE_OK en éxito, ERR_INIT si Mesa no levantada.
 */
int hashmal_cache_misgeret_reset(void);

/**
 * Lee el estado operativo del misgéret sin mutar. Cualquier puntero
 * puede ser NULL — solo se escribe en los no-NULL. Útil para health
 * checks (pieza 12 — Kiyór, futuro).
 *
 * @return HASHMAL_CACHE_OK en éxito, ERR_INIT si Mesa no levantada.
 */
int hashmal_cache_misgeret_estado(uint32_t *peticiones,
                                  uint32_t *max_out,
                                  uint32_t *ventana_ms_out);

/* ══════════════════════════════════════════════════════════════════
 * mesa_disponibilidad.c — ciclo Shabat y corona exterior (Lv 24:8)
 *
 * shabat_renovar es pública (declarada en hashmal_cache.h). Las
 * dos siguientes son internas: consulta de vencimiento del ciclo
 * (para monitoreo / driver cron) y recálculo de la corona exterior
 * (usable tras mutaciones).
 * ══════════════════════════════════════════════════════════════════ */

/**
 * Lv 24:8 — ¿el ciclo Shabat elapsó? Lectura pura: no muta.
 * true si (ahora_ms - shabat_ultimo_refresco_ms) >= shabat_ciclo_ms.
 * Útil para el driver de cron (pieza 12 / Kiyór futuro) y para
 * health checks que detecten "Shabat vencido sin renovar".
 */
bool hashmal_cache_shabat_vencido(void);

/**
 * v.24 — zer zaháv savív (corona exterior). Recalcula el Merkle
 * agregado de los 12 slots: SHA-256(zer_slot[0] || ... || zer_slot[11]).
 * Guarda en shuljan.zer_exterior e incrementa zer_nonce (anti-replay).
 *
 * Invocada por shabat_renovar tras procesar estados. Puede llamarse
 * tras cada mutación en mesa_api.c si se desea la corona siempre
 * fresca; el diseño actual la refresca en el Shabat (consistencia
 * eventual dentro del ciclo).
 *
 * @return HASHMAL_CACHE_OK o ERR_INIT / ERR_ZER_ROTO si SHA-256 falla.
 */
int hashmal_cache_zer_exterior_recalcular(void);

#ifdef __cplusplus
}
#endif

#endif /* HASHMAL_CACHE_INTERNAL_H */
