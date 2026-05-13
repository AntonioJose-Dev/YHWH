/**
 * ═══════════════════════════════════════════════════════════════════
 * PIEZA 09 — BIGDÉI HAQÓDESH — hashmal_roles_internal.h
 * API INTERNA del componente — NO exportada fuera.
 * ═══════════════════════════════════════════════════════════════════
 *
 * "veʻasíta vigdéi qódesh leAharón ajíja leKABÓD uleTIFÁRET"
 *   — Éxodo 28:2
 *
 * Acceso interno al estado MUTABLE del sistema de vestiduras.
 * Solo los 7 archivos hermanos del componente hashmal_roles
 * incluyen este header — el mundo exterior solo ve hashmal_roles.h,
 * donde el acceso es por API de funciones, no estructura compartida.
 *
 * Éx 28:43 — "juqqát ʻolám ló ulezarʻó ajaráv" — estatuto perpetuo
 * para el sacerdote y su simiente: las vestiduras son oficio, no
 * disfraz. Esta vía privada permite a los archivos hermanos
 * (efod, pectoral, monitor, coordinador, shutdown, tzitz) colaborar
 * sobre el mismo singleton sin exportar la struct mutable al exterior.
 *
 * Autor: Antonio José Marín Soto (Betsalél — Éx 31:2)
 * Proyecto: HASHMAL-CORE
 */
#ifndef HASHMAL_ROLES_INTERNAL_H
#define HASHMAL_ROLES_INTERNAL_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "hashmal_roles.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ══════════════════════════════════════════════════════════════════
 * vestiduras_roles.c — singleton y helpers compartidos
 *
 * Los 7 archivos hermanos (roles, efod, pectoral, monitor,
 * coordinador, shutdown, tzitz) necesitan acceso mutable al
 * singleton para registrar nodos, asignar piedras, actualizar
 * heartbeat, transicionar shutdown, sellar tzitz y ejecutar
 * restauración. Esta vía privada mantiene la struct fuera del
 * exterior.
 * ══════════════════════════════════════════════════════════════════ */

/**
 * Acceso MUTABLE al singleton de vestiduras. Solo para archivos
 * hermanos. @return NULL si aún no inicializado (g_vestido==false).
 */
hashmal_roles_estado_roles_t *hashmal_roles_vestiduras_mut(void);

/**
 * Lectura CONST desde dentro del componente. Wrapper equivalente
 * al getter público, con nombre distinto para subrayar ámbito
 * interno.
 */
const hashmal_roles_estado_roles_t *hashmal_roles_get_vestiduras_interno(void);

/**
 * Monotonic clock en milisegundos (int64_t, concordando con los
 * campos timestamp_ms del estado). Encapsula esp_timer_get_time()
 * (microsegundos) y convierte. Compartido entre los 7 archivos
 * para que efod, pectoral, monitor, coordinador, shutdown y tzitz
 * usen la misma unidad.
 */
int64_t hashmal_roles_ahora_ms(void);

#ifdef __cplusplus
}
#endif

#endif /* HASHMAL_ROLES_INTERNAL_H */
