/**
 * ═══════════════════════════════════════════════════════════════════
 * PIEZA 08 — ACEITE PERPETUO — hashmal_power_internal.h
 * API INTERNA del componente — NO exportada fuera.
 * ═══════════════════════════════════════════════════════════════════
 *
 * "shémen záyit ZAJ katít lammaʼór lehaʻalót NÉR TAMÍD" — Éx 27:20
 * "Aceite de oliva PURO, prensado, para la luminaria, para hacer
 *  ascender lámpara CONTINUAMENTE."
 *
 * Acceso interno al estado MUTABLE del aceite perpetuo. Solo los
 * 4 archivos hermanos del componente hashmal_power incluyen este
 * header — el mundo exterior solo ve hashmal_power.h, donde el
 * acceso es por API de funciones, no estructura compartida.
 *
 * mijúts lapparójet (Éx 27:21) — fuera del velo: la energía vive
 * en la capa de servicio, no en el núcleo. Aquí dentro del módulo,
 * los archivos se comparten el aceite; afuera, cada pieza consume
 * sus lecturas por API limpia.
 *
 * Autor: Antonio José Marín Soto
 * Proyecto: HASHMAL-CORE
 */
#ifndef HASHMAL_POWER_INTERNAL_H
#define HASHMAL_POWER_INTERNAL_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "hashmal_power.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ══════════════════════════════════════════════════════════════════
 * aceite_energia.c — singleton y helpers compartidos
 *
 * Los 4 archivos hermanos (energia, tamid, supervivencia,
 * mantenimiento) necesitan acceso mutable al singleton para
 * actualizar lecturas de olivos, transicionar estado, actualizar
 * contadores y timestamps. Esta vía privada permite esa
 * colaboración sin exportar la struct mutable al exterior.
 * ══════════════════════════════════════════════════════════════════ */

/**
 * Acceso MUTABLE al singleton del aceite. Solo para archivos
 * hermanos. @return NULL si aún no inicializado.
 */
hashmal_power_estado_energia_t *hashmal_power_aceite_mut(void);

/**
 * Lectura CONST desde dentro del componente. Wrapper equivalente
 * al getter público, con nombre distinto para subrayar ámbito
 * interno.
 */
const hashmal_power_estado_energia_t *hashmal_power_get_aceite_interno(void);

/**
 * aceite_supervivencia.c — Zac 4:12 shibbolei hazzahav
 *
 * Inicializa / libera el handle ADC del olivo SMOL (bateria).
 * Llamado desde aceite_energia.c en init y deinit.
 * Encapsula el "tubo de oro" para que aceite_energia no necesite
 * incluir esp_adc directamente.
 */
void aceite_adc_init(void);
void aceite_adc_deinit(void);

/**
 * Monotonic clock en milisegundos (int64_t, concordando con la
 * signatura int64_t del campo timestamp_ms del estado). Encapsula
 * esp_timer_get_time() (microsegundos) y convierte. Compartido
 * entre los 4 archivos para que las mediciones de ciclo, lectura
 * de olivos y mantenimiento usen la misma unidad.
 */
int64_t hashmal_power_ahora_ms(void);

#ifdef __cplusplus
}
#endif

#endif /* HASHMAL_POWER_INTERNAL_H */
