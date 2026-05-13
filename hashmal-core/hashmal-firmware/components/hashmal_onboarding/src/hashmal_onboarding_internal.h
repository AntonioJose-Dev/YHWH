/**
 * ═══════════════════════════════════════════════════════════════════
 * PIEZA 10 — MILLUÍM — hashmal_onboarding_internal.h
 * API INTERNA del componente — NO exportada fuera.
 * ═══════════════════════════════════════════════════════════════════
 *
 * "vezéh haddavár ashér taʻaséh LAHEM leQADDÉSH otám leJAHÉN lí."
 *   — Éxodo 29:1
 *
 * "shivʻát yamím yemalléʼ et yedjém." — Levítico 8:33
 * "Por siete días llenará vuestras manos."
 *
 * ─── VOCABULARIO ───
 *
 *   milluím  (מִלֻּאִים, H4394)      LLENADO, consagración:
 *                                      "llenar las manos" del que servirá.
 *   qiddésh  (קִדֵּשׁ, H6942 Piel)   SANTIFICAR, separar para servicio.
 *
 * ─── PROPÓSITO DE ESTE HEADER ───
 *
 * Acceso interno al estado MUTABLE del subsistema de consagración.
 * Solo los 5 archivos hermanos del componente hashmal_onboarding
 * comparten esta vía privada:
 *
 *   consagracion_onboarding.c   pipeline CERO → EN_PROCESO → ACTIVO
 *   consagracion_fases.c        las 7 fases secuenciales (Éx 29)
 *   consagracion_sellos.c       3 sellos (Éx 29:20) + config firmada
 *   consagracion_lockdown.c     recursos dedicados (Éx 29:33-34)
 *   consagracion_cuarentena.c   entrar/salir/purgar (Éx 29:14)
 *
 * El mundo exterior solo ve hashmal_onboarding.h — el acceso es por
 * API de funciones, no estructura compartida. "vezár lo yokhál"
 * (Éx 29:33): lo consagrado no se comparte con el extraño.
 *
 * Autor: Antonio José Marín Soto (Betsalél — Éx 31:2)
 * Proyecto: HASHMAL-CORE
 */
#ifndef HASHMAL_ONBOARDING_INTERNAL_H
#define HASHMAL_ONBOARDING_INTERNAL_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "hashmal_onboarding.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ══════════════════════════════════════════════════════════════════
 * consagracion_onboarding.c — singleton y helpers compartidos
 *
 * Los 5 archivos hermanos necesitan acceso mutable al singleton
 * para transicionar estados, avanzar fases, aplicar sellos,
 * activar lockdown y gestionar cuarentena. Esta vía privada
 * mantiene la struct fuera del exterior.
 * ══════════════════════════════════════════════════════════════════ */

/**
 * Acceso MUTABLE al singleton de la consagración. Solo para archivos
 * hermanos. @return NULL si aún no iniciado (g_iniciado==false).
 */
hashmal_onboarding_t *hashmal_onboarding_consagracion_mut(void);

/**
 * Lectura CONST desde dentro del componente. Wrapper equivalente
 * al getter público, con nombre distinto para subrayar ámbito
 * interno.
 */
const hashmal_onboarding_t *hashmal_onboarding_get_consagracion_interno(void);

/**
 * Monotonic clock en milisegundos (int64_t, concordando con los
 * timestamps del struct). Encapsula esp_timer_get_time()
 * (microsegundos) y convierte. Compartido entre los 5 archivos
 * para que todas las mediciones usen la misma unidad.
 */
int64_t hashmal_onboarding_ahora_ms(void);

#ifdef __cplusplus
}
#endif

#endif /* HASHMAL_ONBOARDING_INTERNAL_H */
