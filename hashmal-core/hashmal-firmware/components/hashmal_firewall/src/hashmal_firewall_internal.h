/**
 * ═══════════════════════════════════════════════════════════════════
 * PIEZA 14 — QETÓRET HASAMMÍM — hashmal_firewall_internal.h
 * API INTERNA del componente — NO exportada fuera.
 * ═══════════════════════════════════════════════════════════════════
 *
 * "veshajaqTÁ mimménnah HADÉQ venataTÁ mimménnah LIFNÉI haʻedút
 *  beʼóhel moʻéd ashér ivvaʻÉD lejá shámmah — QÓDESH QODASHÍM
 *  tihyéh lajém." — Éxodo 30:36
 * "Molerás parte de ello MUY FINO y la pondrás DELANTE del
 *  TESTIMONIO en la Tienda del Encuentro donde ME ENCONTRARÉ
 *  contigo — SANTIDAD DE SANTIDADES será para vosotros."
 *
 * "vehaqqetóret ashér taʻaséh beMATKÚNTAH lo taʻasú lajém —
 *  QÓDESH tihyéh lejá laYHWH." — Éxodo 30:37
 * "Y el incienso que harás en su PROPORCIÓN, no lo haréis para
 *  vosotros — SANTO será para ti, a YHWH."
 *
 * ─── VOCABULARIO ───
 *
 *   hadéq      (הָדֵק, H1851)           MUY FINO, molido —
 *                                        granularidad interna.
 *   ivvaʻéd    (אִוָּעֵד, H3259 Nifal)   Me ENCONTRARÉ — punto de
 *                                        encuentro determinístico.
 *   matkúntah  (מַתְכֻּנְתָּהּ, H4971)   PROPORCIÓN exacta — la
 *                                        fórmula no es replicable.
 *
 * ─── PROPÓSITO DE ESTE HEADER ───
 *
 * Acceso interno al estado MUTABLE del firewall. Solo los 5
 * archivos hermanos del componente hashmal_firewall comparten
 * esta vía privada:
 *
 *   incienso_firewall.c        QETÓRET HADÉQ — singleton +
 *                                filtrado bidireccional
 *   incienso_formato.c         MAʻASÉH ROQÉAJ — validación de formato
 *   incienso_circuitbreaker.c  BEIN HAMMETÍM — circuit breaker
 *   incienso_heartbeat.c       TAMÍD — mañana + tarde
 *   incienso_penalizacion.c    VENIKRAT — escalación progresiva
 *
 * Éx 30:37 "lo taʻasú lajém" — la fórmula no se replica para uso
 * externo. La vía interna queda dentro del componente; el mundo
 * exterior solo ve hashmal_firewall.h.
 *
 * Autor: Antonio José Marín Soto (Betsalél — Éx 31:2)
 * Proyecto: HASHMAL-CORE
 */
#ifndef HASHMAL_FIREWALL_INTERNAL_H
#define HASHMAL_FIREWALL_INTERNAL_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "hashmal_firewall.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ══════════════════════════════════════════════════════════════════
 * incienso_firewall.c — singleton y helpers compartidos
 *
 * Los 5 archivos hermanos necesitan acceso mutable al singleton
 * para actualizar penalizaciones, heartbeat, circuit breaker y
 * estadísticas. Esta vía privada mantiene la struct fuera del
 * exterior.
 * ══════════════════════════════════════════════════════════════════ */

/**
 * Acceso MUTABLE al singleton del incienso. Solo para archivos
 * hermanos. @return NULL si aún no iniciado (g_iniciado==false).
 */
hashmal_firewall_t *hashmal_firewall_incienso_mut(void);

/**
 * Lectura CONST desde dentro del componente. Wrapper equivalente
 * al getter público, con nombre distinto para subrayar ámbito
 * interno.
 */
const hashmal_firewall_t *hashmal_firewall_get_incienso_interno(void);

/**
 * Monotonic clock en milisegundos (int64_t, concordando con los
 * timestamps del struct). Encapsula esp_timer_get_time()
 * (microsegundos) y convierte. Compartido entre los 5 archivos
 * para que todas las mediciones usen la misma unidad.
 */
int64_t hashmal_firewall_ahora_ms(void);

#ifdef __cplusplus
}
#endif

#endif /* HASHMAL_FIREWALL_INTERNAL_H */
