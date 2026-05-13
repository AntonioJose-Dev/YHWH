/**
 * ═══════════════════════════════════════════════════════════════════
 * PIEZA 13 — SHÉMEN HAMISHJÁ — hashmal_keygen_internal.h
 * API INTERNA del componente — NO exportada fuera.
 * ═══════════════════════════════════════════════════════════════════
 *
 * "qódesh hu qódesh yihyé lajem" — Éxodo 30:32
 * "SANTO ES, santo SERÁ para ustedes."
 *
 * Acceso interno al estado MUTABLE de la Unción. Solo los 4
 * archivos hermanos del componente hashmal_keygen incluyen este
 * header — el mundo exterior solo ve hashmal_keygen.h, donde el
 * acceso es por API de funciones, no estructura compartida.
 *
 * El aceite NUNCA se expone en crudo. Ni siquiera dentro del
 * componente — los helpers devuelven el singleton, pero los
 * archivos hermanos operan sobre él solo para cumplir Éx 30:26-29
 * (secuencia de unción) y Éx 30:32-33 (prohibiciones karát). Nadie
 * "lee la fórmula" para exportarla — eso sería yirqáj jamóhu (v.33).
 *
 * Autor: Antonio José Marín Soto (Betsalél — Éx 31:2)
 * Proyecto: HASHMAL-CORE
 */
#ifndef HASHMAL_KEYGEN_INTERNAL_H
#define HASHMAL_KEYGEN_INTERNAL_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "hashmal_keygen.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ══════════════════════════════════════════════════════════════════
 * uncion_keygen.c — singleton y helpers compartidos
 *
 * Los 4 archivos hermanos (keygen, secuencia, prohibicion,
 * activacion) necesitan acceso mutable al singleton para registrar
 * unciones, incrementar violaciones, actualizar timestamps y
 * transicionar estado. Esta vía privada permite esa colaboración
 * sin exportar la struct mutable al exterior.
 * ══════════════════════════════════════════════════════════════════ */

/**
 * Acceso MUTABLE al singleton de la unción. Solo para archivos
 * hermanos. @return NULL si aún no inicializado (sellado_roto==false).
 */
hashmal_keygen_estado_uncion_t *hashmal_keygen_uncion_mut(void);

/**
 * Lectura CONST desde dentro del componente. Wrapper equivalente al
 * snapshot público, con nombre distinto para subrayar ámbito interno.
 */
const hashmal_keygen_estado_uncion_t *hashmal_keygen_get_uncion_interno(void);

/**
 * Monotonic clock en milisegundos (int64_t, concordando con el tipo
 * de timestamp_ms en las structs del header). Encapsula
 * esp_timer_get_time() (microsegundos) y convierte. Compartido
 * entre los 4 archivos para que unciones, contactos y violaciones
 * usen la misma unidad.
 */
int64_t hashmal_keygen_ahora_ms(void);

#ifdef __cplusplus
}
#endif

#endif /* HASHMAL_KEYGEN_INTERNAL_H */
