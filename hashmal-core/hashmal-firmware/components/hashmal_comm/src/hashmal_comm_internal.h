/**
 * ═══════════════════════════════════════════════════════════════════
 * PIEZA 04 — MENORÁ — hashmal_comm_internal.h
 * API INTERNA del componente — NO exportada fuera.
 * ═══════════════════════════════════════════════════════════════════
 *
 * Une símbolos compartidos entre los 6 archivos hermanos del
 * componente hashmal_comm. El mundo exterior solo ve
 * include/hashmal_comm.h; esto es privado.
 *
 * "kullá miqshá ajat zaháv tahór" — Éx 25:36
 * (toda ella una sola pieza batida — invariante que sostiene esta
 *  capa interna: un solo bloque, no seis módulos acoplados)
 *
 * Autor: Antonio José Marín Soto
 * Proyecto: HASHMAL-CORE
 */
#ifndef HASHMAL_COMM_INTERNAL_H
#define HASHMAL_COMM_INTERNAL_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "hashmal_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ══════════════════════════════════════════════════════════════════
 * menora_hub.c — singleton y helpers compartidos
 *
 * El getter público (hashmal_comm_get_menora) devuelve const. Para
 * que los archivos hermanos (lora_driver, canales, aceite, routing,
 * formato) puedan actualizar estado (contadores, estado de ner,
 * aceite, mutsaqa) usamos esta vía privada.
 * ══════════════════════════════════════════════════════════════════ */

/**
 * Acceso mutable a la Menorá. Solo para archivos hermanos del
 * componente hashmal_comm. @return NULL si no inicializada.
 */
hashmal_menora_t *hashmal_comm_menora_mut(void);

/**
 * Lectura const desde dentro del componente — wrapper equivalente
 * al getter público pero nombrado distinto para remarcar el
 * ámbito interno.
 */
const hashmal_menora_t *hashmal_comm_get_menora_interno(void);

/**
 * Monotonic clock en milisegundos. Encapsula esp_timer_get_time()
 * (microsegundos) y convierte. Compartido para que todos los
 * archivos usen la misma unidad en timestamps (encendido, Shabat
 * LoRa, rate limits).
 */
uint64_t hashmal_comm_ahora_ms(void);

/**
 * Valida que un id de canal esté en [0, 7). Lv 24:5 del
 * componente: shivʻá nerot, ni más ni menos (v.37).
 */
bool hashmal_comm_canal_legitimo(hashmal_canal_id_t id);

/* ══════════════════════════════════════════════════════════════════
 * TSANTERÓT — tuberías internas multi-transporte (Zacarías 4:12)
 *
 * "shtéi tsanterót hazzaháv haMRIQÍM meʻaleihém hazzaháv" —
 * dos tubos de oro que ACTIVAMENTE VIERTEN. Los tsanterót (H6804)
 * son INTERNOS: están entre el olivo y la gullá, no los ve el
 * público. Sólo quien mira de cerca la visión los nota. Igual
 * que este header: las funciones siguientes son el cableado
 * interno que une a los 9 archivos hermanos como KULLÁ MIQSHÁ
 * AJAT (Éx 25:36) — una sola pieza batida.
 *
 * Éx 25:35 kaftorim TÁJAT shenéi haqqaním: las junciones unen
 * pares de hermanos por DEBAJO, sin que el exterior vea. Las
 * funciones de esta sección son ese "debajo": consumidas por
 *   menora_mutsaqot.c        (selector, Zac 4:6 berují)
 *   menora_espnow_driver.c   (ishá el ajotá, Éx 26:3)
 *   menora_ble_paamon.c      (campanilla, Éx 28:34)
 * y también por los archivos existentes que necesiten actualizar
 * estadísticas de transporte (menora_routing, menora_aceite).
 * ══════════════════════════════════════════════════════════════════ */

/**
 * Acceso mutable a UNA mutsaqá concreta de un ner. Permite a los
 * drivers (ESP-NOW, BLE, LoRa) actualizar contadores
 * (paquetes_tx / paquetes_rx / errores), marcar disponible/activa,
 * y registrar capacidad/alcance/consumo medidos en caliente. Al
 * selector (Zac 4:6 berují) le sirve para leer el estado del
 * conducto antes de elegir.
 *
 * Validaciones:
 *   - Menorá inicializada (singleton vivo).
 *   - canal en [0, HASHMAL_MENORA_CANALES).
 *   - mutsaqa en [0, HASHMAL_MUTSAQA_TOTAL).
 *
 * @return NULL si algún parámetro es ilegítimo o la Menorá aún
 * no levantada.
 */
hashmal_mutsaqa_t *hashmal_comm_mutsaqa_mut(hashmal_canal_id_t canal,
                                             hashmal_mutsaqa_id_t mutsaqa);

/**
 * Valida que un id de mutsaqa esté en [0, HASHMAL_MUTSAQA_TOTAL).
 * Zac 4:2 fija TRES conductos por ner (LoRa / ESP-NOW / BLE) en
 * el diseño — ni más, ni menos, como las shivʻá nerot de v.37.
 */
bool hashmal_comm_mutsaqa_legitima(hashmal_mutsaqa_id_t id);

/**
 * Validador compartido de direcciones MAC (6 bytes). Usado por
 * ESP-NOW (ishá el ajotá — peer unicast) y BLE (paʻamón — beacon
 * de vecino). Rechaza:
 *   - Puntero NULL.
 *   - Broadcast FF:FF:FF:FF:FF:FF — el Mishkán no grita al aire.
 *   - Multicast (bit 0 del primer byte) — ishá el ajotá es
 *     UNICAST, hermana a hermana, no "a cualquiera".
 *   - MAC nula 00:00:00:00:00:00 — ausencia de identidad.
 */
bool hashmal_comm_mac_legitima(const uint8_t mac[6]);

#ifdef __cplusplus
}
#endif

#endif /* HASHMAL_COMM_INTERNAL_H */
