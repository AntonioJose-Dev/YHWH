/**
 * ═══════════════════════════════════════════════════════════════════
 * PIEZA 11 — ALTAR DEL INCIENSO — hashmal_intercession_internal.h
 * API INTERNA del componente — NO exportada fuera.
 * ═══════════════════════════════════════════════════════════════════
 *
 * "venatattá otó LIFNÉI HAPPÁRÓJET ashér ʻal arón haʻedút lifnéi
 *  hakkappóret ashér ʻal haʻedút ashér IVVAʻÉD lejá shámma" — Éx 30:6
 * "Lo pondrás DELANTE DEL VELO que está sobre el Arca del
 *  Testimonio, delante del Kapóret — donde ME ENCONTRARÉ CONTIGO
 *  ALLÍ."
 *
 * El componente es UN SOLO bloque ante el velo. Los 5 archivos .c
 * hermanos comparten este header privado para operar como una
 * pieza coherente — nadie fuera del componente lo ve.
 *
 * Autor: Antonio José Marín Soto
 * Proyecto: HASHMAL-CORE
 */
#ifndef HASHMAL_INTERCESSION_INTERNAL_H
#define HASHMAL_INTERCESSION_INTERNAL_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "hashmal_intercession.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ══════════════════════════════════════════════════════════════════
 * altar_oro_escudo.c — singleton y helpers compartidos
 *
 * El getter público (hashmal_intercession_get_altar) devuelve const.
 * Para que los archivos hermanos (ascendente, formula, circuitbreaker,
 * oracion) puedan actualizar estado (cuernos, contadores, timestamps)
 * usamos esta vía privada. NO SE EXPORTA fuera del componente.
 * ══════════════════════════════════════════════════════════════════ */

/**
 * Acceso mutable al altar. Solo para archivos hermanos del
 * componente hashmal_intercession. @return NULL si no levantado.
 */
hashmal_altar_t *hashmal_intercession_altar_mut(void);

/**
 * Lectura const desde dentro del componente — wrapper equivalente
 * al getter público pero con nombre distinto para subrayar el
 * ámbito interno.
 */
const hashmal_altar_t *hashmal_intercession_get_altar_interno(void);

/**
 * Monotonic clock en milisegundos. Encapsula esp_timer_get_time()
 * (microsegundos) y convierte. Compartido para que todos los
 * archivos usen la misma unidad en timestamps (cuernos, ciclos
 * diarios, Yom Kippur).
 */
uint64_t hashmal_intercession_ahora_ms(void);

/**
 * Valida id de cuerno en [0, HASHMAL_ALTAR_CUERNOS) = [0, 4).
 * Éx 30:2 — "mimménnu qarnotáv": los 4 cuernos del altar.
 */
bool hashmal_intercession_cuerno_legitimo(uint8_t id);

#ifdef __cplusplus
}
#endif

#endif /* HASHMAL_INTERCESSION_INTERNAL_H */
