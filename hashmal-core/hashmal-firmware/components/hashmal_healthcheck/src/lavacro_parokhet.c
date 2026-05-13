/*
 * ===================================================================
 * PIEZA 12 - EL KIYOR (LAVACRO) - lavacro_parokhet.c
 * Sub-pieza: PAROKHET - detector de shaon electromagnetico
 * Exodo 26:31-33 + 1 Reyes 19:11-12 + Habacuc 2:20 + Sofonias 1:7
 *               + Zacarias 2:13 + Salmo 46:10 + Isaias 30:15
 *               + Exodo 30:18 + Exodo 30:20 + Salmo 91:1 + Salmo 27:5
 * ===================================================================
 *
 * "ve'asita PAROKHET tekelet veargaman vetola'at shani... vehivdilah
 *  happarokhet lakem ben haqqodesh uven qodesh haqqodashim."
 *    - Exodo 26:31-33
 * "Haras un VELO... y el velo SEPARARA entre el Lugar Santo y el
 *  Santisimo." - hivdil (H914).
 *
 * "velo baruaj YHWH... velo bara'ash YHWH... velo ba'esh YHWH...
 *  ve'ajar ha'esh qol DEMAMAH DAQQAH." - 1 Reyes 19:11-12
 * "YHWH no estaba en el viento, ni en el temblor, ni en el fuego.
 *  Despues del fuego, una voz de SILENCIO SUTIL."
 *
 * "YHWH beheikhal qodsho - HAS mippanav kol haarets." - Habacuc 2:20
 * "YHWH esta en su santo templo - CALLE toda la tierra ante El."
 *
 * "HARPU ude'u ki anokhi elohim." - Salmo 46:10
 * "Estad QUIETOS y SABED que Yo soy Dios."
 *
 * "beSHUVA vaNAJAT... beHASHQET ubeBITJAH." - Isaias 30:15
 * "En conversion y QUIETUD... en CALMA y confianza."
 *
 * --- EXEGESIS ---
 *
 * parokhet (H6532) - velo que SEPARA. En el Mishkan fisico, separa
 * el Lugar Santo del Santisimo. En firmware: la capa logica que
 * aisla al nodo del ruido del entorno cuando el Kiyor detecta que
 * el ambiente no es apto para Presencia.
 *
 * shaon (H7588) - tumulto, fragor. Lo opuesto de demama daqqah
 * (silencio fino) donde YHWH se manifiesta. La maxima concentracion
 * es ra'ash (H7494) - 1 Re 19:11 explicito: YHWH NO estaba en el
 * ra'ash. Un ambiente que alcanza ra'ash no es habitable por
 * Presencia - solo queda el velo.
 *
 * --- 5 NIVELES ---
 *
 *   DEMAMAH   0-20    Silencio fino          (1 Re 19:12)
 *   SHAQAT    21-40   Quietud                (Is 30:15)
 *   HAMON     41-60   Tumulto moderado
 *   SHAON     61-80   Ruido alto
 *   RAASH     81-100  Temblor                (1 Re 19:11, YHWH NO esta)
 *
 * --- EL KIYOR MIDE, NO APAGA ---
 *
 * Ex 30:18 - el kiyor va ENTRE altar y tienda. Su oficio es VERIFICAR
 * la limpieza del que entra, no apagar el mundo. Cuando se pide
 * Shabat EM, este sub-modulo delega en Pieza 04 (hashmal_comm_
 * parokhet_shabat_set) - la Menora honra la peticion quiesando
 * WiFi y BLE. LoRa permanece tamid (Ex 27:21 - la lampara no se apaga).
 *
 * --- QUE HACE / QUE NO HACE ---
 *
 *   HACE:                         NO HACE:
 *   - scan 3 fuentes EM           - leer SX1262 / NimBLE directo
 *   - calcular indice 0-100       - anadir 5to vector al enum
 *   - clasificar 5 niveles        - tocar transportes directamente
 *   - pedir Shabat EM             - apagar la Menora
 *
 * --- NO ANADE VECTOR ---
 *
 * Los 4 vectores del enum hashmal_hc_vector_t (yadayim/raglayim/
 * levav/mayim) son Ex 30:19 + Sal 24:4 literales. Parokhet NO
 * extiende ese enum - opera como indice paralelo. El layout binario
 * de hashmal_hc_vector_estado_t permanece intacto; el contrato con
 * callers que iteran "4 elementos biblicos" no se rompe.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_healthcheck.h       API publica (types, constantes)
 *   hashmal_hc_internal.h       lavacro_mut, get_lavacro_interno, ahora_ms
 *   hashmal_comm.h              primitivas EM + flag shabat
 *   esp_log.h (ORO EGIPTO)      PARKT_LOG_*
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <inttypes.h>

#include "esp_log.h"

#include "hashmal_healthcheck.h"
#include "hashmal_hc_internal.h"
#include "hashmal_comm.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *PARKT_TAG = "hashmal.hc.parokhet";

#define PARKT_LOG_I(fmt, ...)  ESP_LOGI(PARKT_TAG, fmt, ##__VA_ARGS__)
#define PARKT_LOG_W(fmt, ...)  ESP_LOGW(PARKT_TAG, fmt, ##__VA_ARGS__)
#define PARKT_LOG_E(fmt, ...)  ESP_LOGE(PARKT_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * Helpers estaticos - normalizacion por fuente y combinacion
 *
 * Ex 30:34 bad bebad (partes iguales, del qetoret) justifica pesos
 * identicos para WiFi / BLE / LoRa. Sin pretension de precision
 * absoluta - es un indicador; el caller puede sumar su propia
 * politica (p.ej. repetir N muestras y promediar).
 * ================================================================== */

static uint8_t u8_clamp_0_100(int32_t v)
{
    if (v < 0)   return 0u;
    if (v > 100) return 100u;
    return (uint8_t)v;
}

/* Cada AP visible suma 5 pts; saturacion a 20 APs. RSSI ignorado en
 * fase 1 (ap_count es el indicador suficiente). */
static uint8_t componente_wifi(uint8_t ap_count, int8_t rssi_max)
{
    (void)rssi_max;
    return u8_clamp_0_100((int32_t)ap_count * 5);
}

/* Cada vecino suma 4 pts; saturacion a 25 vecinos. */
static uint8_t componente_ble(uint8_t vecinos, int8_t rssi_medio)
{
    (void)rssi_medio;
    return u8_clamp_0_100((int32_t)vecinos * 4);
}

/* Mapeo lineal del suelo LoRa al rango 0..100.
 *   -110 dBm o menos -> 0  (pristino)
 *   -80  dBm o mas   -> 100 (saturado)
 *   -95  dBm (typ.)  -> 50  (medio)
 * Ventana de 30 dB cubre lo operativo para EU868 SF7/BW125. */
static uint8_t componente_lora(int8_t rssi_floor)
{
    int32_t f = (int32_t)rssi_floor;
    if (f < -110) f = -110;
    if (f > -80)  f = -80;
    return u8_clamp_0_100(((f + 110) * 100) / 30);
}

static hashmal_hc_shaon_nivel_t nivel_de_indice(uint8_t idx)
{
    if (idx <= HASHMAL_HC_SHAON_DEMAMAH_MAX) return HASHMAL_HC_SHAON_DEMAMAH;
    if (idx <= HASHMAL_HC_SHAON_SHAQAT_MAX)  return HASHMAL_HC_SHAON_SHAQAT;
    if (idx <= HASHMAL_HC_SHAON_HAMON_MAX)   return HASHMAL_HC_SHAON_HAMON;
    if (idx <= HASHMAL_HC_SHAON_SHAON_MAX)   return HASHMAL_HC_SHAON_SHAON;
    return HASHMAL_HC_SHAON_RAASH;
}

static const char *nombre_nivel(hashmal_hc_shaon_nivel_t n)
{
    switch (n) {
    case HASHMAL_HC_SHAON_DEMAMAH: return "DEMAMAH (1 Re 19:12 silencio fino)";
    case HASHMAL_HC_SHAON_SHAQAT:  return "SHAQAT (Is 30:15 quietud)";
    case HASHMAL_HC_SHAON_HAMON:   return "HAMON (tumulto moderado)";
    case HASHMAL_HC_SHAON_SHAON:   return "SHAON (ruido alto)";
    case HASHMAL_HC_SHAON_RAASH:   return "RAASH (1 Re 19:11 temblor)";
    default:                       return "<desconocido>";
    }
}

/* ==================================================================
 * API - hashmal_hc_parokhet_scan (Ex 26:31-33)
 * ================================================================== */
int hashmal_hc_parokhet_scan(hashmal_hc_parokhet_muestras_t *out)
{
    if (out == NULL) {
        PARKT_LOG_E("scan: out NULL");
        return HASHMAL_HC_ERR_NULL;
    }

    const hashmal_hc_t *l = hashmal_hc_get_lavacro_interno();
    if (l == NULL) {
        PARKT_LOG_E("scan: kiyor no levantado");
        return HASHMAL_HC_ERR_INIT;
    }
    if (l->estado == HASHMAL_HC_SECO) {
        PARKT_LOG_E("scan: estado SECO - sin mayim no hay medicion "
                    "(Ex 30:18 venatatta ausente)");
        return HASHMAL_HC_ERR_SECO;
    }

    /* Partimos limpios: sin basura del caller. */
    out->wifi_ap_count   = 0;
    out->wifi_rssi_max   = INT8_MIN;
    out->ble_vecinos     = 0;
    out->ble_rssi_medio  = 0;
    out->lora_rssi_floor = 0;
    out->timestamp_ms    = hashmal_hc_ahora_ms();

    /* 3 fuentes, 3 llamadas a Pieza 04. Un error en una fuente NO
     * invalida las otras (Zac 4:3 dos olivos: redundancia). El rc
     * se traga a proposito: mantenemos valores por defecto ya
     * inicializados arriba. */
    (void)hashmal_comm_wifi_aps_muestra(&out->wifi_ap_count,
                                         &out->wifi_rssi_max);
    (void)hashmal_comm_ble_rssi_muestra(&out->ble_rssi_medio,
                                         &out->ble_vecinos);
    (void)hashmal_comm_spi_rssi_ruido(&out->lora_rssi_floor);

    return HASHMAL_HC_OK;
}

/* ==================================================================
 * API - hashmal_hc_parokhet_indice
 *
 * Toma una muestra fresca, normaliza los 3 componentes y los combina
 * en partes iguales (Ex 30:34 bad bebad). El nivel sale del indice.
 * ================================================================== */
int hashmal_hc_parokhet_indice(uint8_t *indice_0_100,
                                hashmal_hc_shaon_nivel_t *nivel)
{
    if (indice_0_100 == NULL || nivel == NULL) {
        PARKT_LOG_E("indice: parametro NULL");
        return HASHMAL_HC_ERR_NULL;
    }

    hashmal_hc_parokhet_muestras_t m;
    int rc = hashmal_hc_parokhet_scan(&m);
    if (rc != HASHMAL_HC_OK) return rc;

    uint32_t suma = (uint32_t)componente_wifi(m.wifi_ap_count, m.wifi_rssi_max)
                  + (uint32_t)componente_ble(m.ble_vecinos,    m.ble_rssi_medio)
                  + (uint32_t)componente_lora(m.lora_rssi_floor);
    uint8_t idx = (uint8_t)(suma / 3u);

    *indice_0_100 = idx;
    *nivel        = nivel_de_indice(idx);

    if (*nivel == HASHMAL_HC_SHAON_RAASH) {
        /* 1 Re 19:11 YHWH NO estaba en el ra'ash. WARN, no accion
         * automatica - la decision de escalar a Shabat EM o a
         * ataque_registrar la toma el caller (separacion de
         * responsabilidades, Ex 25:40 betavnitam). */
        PARKT_LOG_W("indice=%u/100 -> %s - YHWH NO esta en el ra'ash",
                    (unsigned)idx, nombre_nivel(*nivel));
    } else if (*nivel >= HASHMAL_HC_SHAON_HAMON) {
        PARKT_LOG_I("indice=%u/100 -> %s",
                    (unsigned)idx, nombre_nivel(*nivel));
    }
    return HASHMAL_HC_OK;
}

/* ==================================================================
 * API - hashmal_hc_parokhet_shabat (Ex 26:31-33)
 *
 * Delegacion cooperativa: el Kiyor pide, la Menora honra. Ex 30:18
 * - aqui se mide y avisa; apagar transporte es oficio de Pieza 04.
 * ================================================================== */
int hashmal_hc_parokhet_shabat(bool activar)
{
    const hashmal_hc_t *l = hashmal_hc_get_lavacro_interno();
    if (l == NULL) {
        PARKT_LOG_E("shabat_%s: kiyor no levantado",
                    activar ? "ON" : "OFF");
        return HASHMAL_HC_ERR_INIT;
    }
    if (l->estado == HASHMAL_HC_SECO) {
        PARKT_LOG_E("shabat_%s: estado SECO",
                    activar ? "ON" : "OFF");
        return HASHMAL_HC_ERR_SECO;
    }

    int rc = hashmal_comm_parokhet_shabat_set(activar);
    if (rc != HASHMAL_COMM_OK) {
        PARKT_LOG_E("shabat_%s: la Menora rechazo la peticion (rc=%d)",
                    activar ? "ON" : "OFF", rc);
        return HASHMAL_HC_ERR_INIT;
    }

    PARKT_LOG_I("shabat EM %s - parokhet hivdil (Ex 26:33); "
                "Pieza 04 honro la peticion",
                activar ? "ON" : "OFF");
    return HASHMAL_HC_OK;
}

/* ==================================================================
 *
 *   "vehivdilah happarokhet lakem ben haqqodesh uven qodesh
 *    haqqodashim." - Exodo 26:33
 *
 *   "velo baruaj YHWH... velo bara'ash... qol DEMAMAH DAQQAH."
 *     - 1 Reyes 19:11-12
 *
 *   "HARPU ude'u ki anokhi elohim." - Salmo 46:10
 *
 *   El kiyor MIDE el entorno; el parokhet SEPARA cuando mide ra'ash.
 *   La voz del Padre no llega en el tumulto - por eso el velo existe.
 *   La lampara (geza') no se apaga; el velo cubre lo exterior. Asi
 *   la Presencia se guarda.
 *
 *   Anadidura 01. Mide hacia afuera lo que los espejos miran hacia
 *   adentro. Ambos fundidos en el mismo bronce del kiyor.
 *
 * ================================================================== */
