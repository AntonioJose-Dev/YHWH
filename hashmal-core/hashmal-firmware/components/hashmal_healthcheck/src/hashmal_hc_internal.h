/**
 * ═══════════════════════════════════════════════════════════════════
 * PIEZA 12 — KIYÓR (LAVACRO) — hashmal_hc_internal.h
 * API INTERNA del componente — NO exportada fuera.
 * ═══════════════════════════════════════════════════════════════════
 *
 * "veʻasíta KIYÓR NEJÓSHET... venatattá SHÁMMA MÁYIM" — Éx 30:18
 * "Harás un LAVACRO de bronce... y pondrás allí AGUA."
 *
 * "verájatsú Aharón uvanáv MIMMENNÚ et yadéihem veet ragléihem"
 *   — Éx 30:19
 * "Lavarán Aharón y sus hijos DE ÉL sus manos y sus pies."
 *
 * mimmennú — DE ÉL. Los archivos hermanos de este componente
 * beben del mismo kiyór: el singleton del lavacro es único, y
 * todos se sirven del mismo agua. Este header comparte los
 * símbolos que permiten esa unidad operativa — sin exportarlos
 * al exterior (el kiyór está en el Atrio, no en la Tienda: lo
 * interior permanece dentro).
 *
 * Autor: Antonio José Marín Soto
 * Proyecto: HASHMAL-CORE
 */
#ifndef HASHMAL_HC_INTERNAL_H
#define HASHMAL_HC_INTERNAL_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "hashmal_healthcheck.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ══════════════════════════════════════════════════════════════════
 * lavacro_core.c — singleton y helpers compartidos
 *
 * El getter público (hashmal_hc_get_lavacro) devuelve const. Para
 * que los archivos hermanos (vectores, niveles, espejos, ataques,
 * dos_niveles) puedan actualizar estado legítimamente (niveles
 * por vector, contadores, timestamps, flags) usamos esta vía
 * privada. NO SE EXPORTA fuera del componente.
 * ══════════════════════════════════════════════════════════════════ */

/**
 * Acceso MUTABLE al singleton del kiyór. "mimmennú" (Éx 30:19 —
 * de él). Solo para archivos hermanos. NULL si no levantado.
 */
hashmal_hc_t *hashmal_hc_lavacro_mut(void);

/**
 * Lectura const desde dentro del componente — wrapper equivalente
 * al getter público pero con nombre distinto para subrayar el
 * ámbito interno.
 */
const hashmal_hc_t *hashmal_hc_get_lavacro_interno(void);

/**
 * Monotonic clock en milisegundos. Encapsula esp_timer_get_time()
 * (microsegundos) y convierte. Compartido para que todos los
 * archivos usen la misma unidad en timestamps (checks, espejos,
 * ataques, niptō).
 */
uint64_t hashmal_hc_ahora_ms(void);

/**
 * Éx 30:19 — "yadéihem veragléihem": los vectores legítimos son
 * los que el Padre nombró. Valida v ∈ [0, HASHMAL_HC_VECTORES).
 */
bool hashmal_hc_vector_legitimo(hashmal_hc_vector_t v);

/**
 * Valida n ∈ [TAHOR, MAVET]. Niveles fuera de rango son enum
 * casts ilegítimos.
 */
bool hashmal_hc_nivel_legitimo(hashmal_hc_nivel_t n);

#ifdef __cplusplus
}
#endif

#endif /* HASHMAL_HC_INTERNAL_H */
