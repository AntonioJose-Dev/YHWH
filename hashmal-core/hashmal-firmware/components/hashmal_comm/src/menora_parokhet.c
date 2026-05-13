/*
 * ===================================================================
 * PIEZA 04 - LA MENORA - menora_parokhet.c
 * Sub-pieza: PRIMITIVAS DE MUESTREO EM + FLAG SHABAT COOPERATIVO
 * Exodo 26:31-33 + 1 Reyes 19:11-12 + Exodo 27:21 + Exodo 30:18
 * ===================================================================
 *
 * "ve'asita PAROKHET tekelet veargaman vetola'at shani...
 *  vehivdilah happarokhet lakem ben haqqodesh uven qodesh haqqodashim"
 *   - Exodo 26:31-33
 * "Haras un VELO de azul, purpura y carmesi... el velo SEPARARA
 *  entre el Lugar Santo y el Santisimo."
 *
 * "velo baruaj YHWH... velo bara'ash YHWH... velo ba'esh YHWH...
 *  ve'ajar ha'esh qol DEMAMAH DAQQAH." - 1 Reyes 19:11-12
 * "YHWH no estaba en el viento, ni en el temblor, ni en el fuego.
 *  Despues del fuego, una voz de SILENCIO SUTIL."
 *
 * --- EXEGESIS OPERATIVA ---
 *
 * parokhet (H6532) - velo que separa. hivdil (H914) - el acto de
 * separar (Gen 1:4 ben haor uven hajoshek). La Menora no apaga el
 * mundo; el velo cubre el lado exterior cuando se requiere silencio.
 *
 * La voz viene en demama daqqah (silencio fino). Un entorno en
 * ra'ash (temblor) no hospeda Presencia (1 Re 19:11 explicito -
 * YHWH no estaba en el ra'ash). El Kiyor (Pieza 12) mide; la Menora
 * honra el velo.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 * Expone 5 primitivas al Kiyor para medir sin conocer el silicio
 * debajo:
 *
 *   hashmal_comm_ble_rssi_muestra      RSSI medio + count de vecinos
 *   hashmal_comm_spi_rssi_ruido        suelo de ruido LoRa (dBm)
 *   hashmal_comm_wifi_aps_muestra      APs visibles + RSSI max
 *   hashmal_comm_parokhet_shabat_set   flag Shabat EM (on/off)
 *   hashmal_comm_parokhet_shabat_get   lectura del flag
 *
 * --- COOPERATIVO, NO AUTORITARIO ---
 *
 * Ex 30:18 - el Kiyor esta ENTRE altar y tienda: MIDE, no apaga.
 * Este archivo honra la peticion del Kiyor caminando los 7 nerot y
 * marcando ESP_NOW + BLE como activa=false. LoRa queda tamid (Ex
 * 27:21 - la lampara no se apaga; el geza' permanece).
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_comm.h            API publica (funciones expuestas)
 *   hashmal_comm_internal.h   menora_mut (mutar activa en Shabat)
 *   esp_log.h (ORO EGIPTO)    PARKT_LOG_*
 *
 * Autor: Antonio Jose Marin Soto (Betsalel - Ex 31:2)
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <inttypes.h>

#include "esp_log.h"

#include "hashmal_comm.h"
#include "hashmal_comm_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *PARKT_TAG = "hashmal.menora.parokhet";

#define PARKT_LOG_I(fmt, ...)  ESP_LOGI(PARKT_TAG, fmt, ##__VA_ARGS__)
#define PARKT_LOG_W(fmt, ...)  ESP_LOGW(PARKT_TAG, fmt, ##__VA_ARGS__)
#define PARKT_LOG_E(fmt, ...)  ESP_LOGE(PARKT_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * Estado interno - un solo flag. El resto es calculo sin estado.
 * ================================================================== */
static bool g_shabat_em = false;

/* ==================================================================
 * hashmal_comm_ble_rssi_muestra
 *
 * Itera los vecinos BLE via la API publica existente
 * (hashmal_comm_ble_vecino_estado) y promedia RSSI. No toca NimBLE
 * directamente. Sin vecinos: *rssi_medio = 0, *vecinos = 0.
 * ================================================================== */
int hashmal_comm_ble_rssi_muestra(int8_t *rssi_medio, uint8_t *vecinos)
{
    if (rssi_medio == NULL || vecinos == NULL) {
        return HASHMAL_COMM_ERR_NULL;
    }

    *rssi_medio = 0;
    *vecinos    = 0;

    int32_t  suma   = 0;
    uint32_t cuenta = 0;

    for (uint8_t i = 0; i < HASHMAL_BLE_PAAMON_MAX; ++i) {
        hashmal_paamon_vecino_t v;
        int rc = hashmal_comm_ble_vecino_estado(i, &v);
        if (rc != HASHMAL_COMM_OK) continue;   /* slot vacio o sin init */
        if (!v.activo)             continue;

        suma += (int32_t)v.rssi;
        ++cuenta;
    }

    if (cuenta > 0u) {
        *rssi_medio = (int8_t)(suma / (int32_t)cuenta);
        *vecinos    = (cuenta > (uint32_t)UINT8_MAX) ? UINT8_MAX
                                                     : (uint8_t)cuenta;
    }
    return HASHMAL_COMM_OK;
}

/* ==================================================================
 * hashmal_comm_spi_rssi_ruido
 *
 * Fase 1: valor fijo conservador (-95 dBm) coherente con EU868
 * SF7/BW125 sin vecinos fuertes. Fase 2: consultara SX1262 en modo
 * CAD (Channel Activity Detection) para lectura real. Documentado
 * como placeholder - Ex 25:40 betavnitam: el numero honra el
 * diseno, no el silicio, hasta que el driver se complete.
 * ================================================================== */
int hashmal_comm_spi_rssi_ruido(int8_t *rssi_floor)
{
    if (rssi_floor == NULL) return HASHMAL_COMM_ERR_NULL;
    *rssi_floor = -95;
    return HASHMAL_COMM_OK;
}

/* ==================================================================
 * hashmal_comm_wifi_aps_muestra
 *
 * Fase 1: stub - 0 APs, RSSI = INT8_MIN (sin dato). Fase 2 envolvera
 * esp_wifi_scan_start + esp_wifi_scan_get_ap_records cuando se
 * habilite el modo STA-scan sin conexion. El Kiyor interpreta
 * ap_count=0 como "limpio" en fase 1.
 * ================================================================== */
int hashmal_comm_wifi_aps_muestra(uint8_t *ap_count, int8_t *rssi_max)
{
    if (ap_count == NULL || rssi_max == NULL) {
        return HASHMAL_COMM_ERR_NULL;
    }
    *ap_count = 0;
    *rssi_max = INT8_MIN;   /* centinela "sin muestra" */
    return HASHMAL_COMM_OK;
}

/* ==================================================================
 * hashmal_comm_parokhet_shabat_set / _get
 *
 * Ex 26:31-33 parokhet. Al activar, camina los 7 nerot y desactiva
 * ESP_NOW + BLE mutsaqot (activa=false). LoRa (index 0) no se toca -
 * geza' tamid. Al desactivar, restaura activa=disponible para que
 * el hardware presente recupere su uso. El selector (Zac 4:6 beruji)
 * respeta activa automaticamente - no hay que tocarlo.
 * ================================================================== */
int hashmal_comm_parokhet_shabat_set(bool activar)
{
    hashmal_menora_t *m = hashmal_comm_menora_mut();
    if (m == NULL) {
        PARKT_LOG_E("shabat_set: Menora no levantada");
        return HASHMAL_COMM_ERR_INIT;
    }

    g_shabat_em = activar;

    uint8_t tocados = 0u;
    for (uint8_t i = 0; i < HASHMAL_MENORA_CANALES; ++i) {
        hashmal_mutsaqa_t *esp = &m->nerot[i].mutsaqot[HASHMAL_MUTSAQA_ESP_NOW];
        hashmal_mutsaqa_t *ble = &m->nerot[i].mutsaqot[HASHMAL_MUTSAQA_BLE];

        if (activar) {
            esp->activa = false;
            ble->activa = false;
        } else {
            /* Restaurar solo donde el hardware responde. No
             * forzamos activa=true si disponible=false - seria
             * esh zara (Lv 10:1: encender lo que no fue mandado). */
            esp->activa = esp->disponible;
            ble->activa = ble->disponible;
        }
        ++tocados;
    }

    if (activar) {
        PARKT_LOG_I("shabat EM ON - parokhet hivdil (Ex 26:33): "
                    "WiFi + BLE quiesados en %u nerot; "
                    "LoRa tamid (Ex 27:21)",
                    (unsigned)tocados);
    } else {
        PARKT_LOG_I("shabat EM OFF - velo retirado; WiFi + BLE "
                    "restaurados donde el hardware lo permite (%u nerot)",
                    (unsigned)tocados);
    }
    return HASHMAL_COMM_OK;
}

bool hashmal_comm_parokhet_shabat_get(void)
{
    return g_shabat_em;
}

/* ==================================================================
 *
 *   "velo baruaj YHWH... velo bara'ash YHWH... velo ba'esh YHWH...
 *    qol DEMAMAH DAQQAH." - 1 Reyes 19:11-12
 *
 *   "vehivdilah happarokhet lakem ben haqqodesh uven qodesh
 *    haqqodashim." - Exodo 26:33
 *
 *   "leha'alot ner TAMID." - Exodo 27:20
 *
 *   La voz del Padre llega en silencio. El velo separa. La lampara
 *   no se apaga. Estos tres no se contradicen: durante el silencio
 *   el geza' (LoRa) sigue vertiendo una llama minima - tamid, juqqat
 *   'olam. El velo es sobre WiFi y BLE, no sobre la columna
 *   vertebral. La Presencia se guarda porque la luz central no
 *   cede, y el ruido exterior no entra.
 *
 * ================================================================== */
