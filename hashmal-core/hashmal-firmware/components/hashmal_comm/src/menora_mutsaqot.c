/**
 * ===================================================================
 * PIEZA 04 - LA MENORA - menora_mutsaqot.c
 * Sub-pieza: GULLA Y MUTSAQOT - el deposito y los conductos (Zac 4)
 * Zacarias 4:2,3,6,14 + Exodo 25:31-40 + Exodo 26:14 + Exodo 27:20-21
 * ===================================================================
 *
 * "menorat zahav kullah veGULLAH 'al ROSHAH, veSHIV'AH neroteiha
 *  'aleiha - SHIV'A veSHIV'A MUTSAQOT lannerot asher 'al rosha."
 *    - Zacarias 4:2
 * "Una Menora de oro, TODA ella, y su CUENCO sobre su cabeza, y
 *  SIETE lamparas sobre ella - SIETE Y SIETE CONDUCTOS para las
 *  lamparas que estan sobre su cabeza."
 *
 * "ushnayim zeitim 'aleiha - eJAD mimyamin haggullah veEJAD 'al
 *  semola." - Zacarias 4:3
 * "Y DOS OLIVOS sobre ella - UNO a la derecha del cuenco, y UNO
 *  a la izquierda."
 *
 * "LO VEJAYIL VELO VEJOAJ ki im beRUJI amar YHWH tseva'ot."
 *    - Zacarias 4:6
 * "NO con EJERCITO, NI con FUERZA, sino con MI ESPIRITU, dice
 *  YHWH de los Ejercitos."
 *
 * "ele shenei BENEI HAYITSHAR ha'omedim 'al adon kol haarets."
 *    - Zacarias 4:14
 * "Estos son los dos HIJOS DEL ACEITE FRESCO, los que ESTAN DE
 *  PIE ante el Senor de toda la tierra."
 *
 * "shemen zayit ZAJ katit lamma'or leHA'ALOT ner TAMID."
 *    - Exodo 27:20
 * "Aceite de oliva PURO, prensado, para la luminaria, para hacer
 *  SUBIR lampara PERPETUA."
 *
 * "orot eilim me'oddamim uMIJSE orot tejashim MILMA'LAH." - Ex 26:14
 * "Pieles de carnero rojas y cubierta de pieles de tajash POR
 *  ENCIMA." - DOS materiales distintos sobre el mismo Mishkan;
 * paralelo de los multiples transportes sobre la misma Menora.
 *
 * --- EXEGESIS - este archivo es la GULLA ---
 *
 * gulla (GLH, H1543) = cuenco, deposito. En Zac 4:2 esta sobre
 * la cabeza de la Menora, y DE ELLA salen las mutsaqot a cada ner.
 * Es el punto central de distribucion. Este archivo HACE eso:
 * recibe "que quiero enviar, a quien, desde que canal" y elige
 * por cual conducto vertir el aceite.
 *
 * mutsaqot (MWTsQWT, H4166) = tubos/conductos, del verbo yatsaq
 * (verter). No son pasivos: hamriqim (Zac 4:12, H7324) - los que
 * ACTIVAMENTE vierten. En firmware: cada mutsaqa reporta su
 * estado (disponible, activa, capacidad, alcance, consumo) y el
 * selector orquesta.
 *
 * --- CRITERIO DE SELECCION (Zac 4:6) ---
 *
 * "lo vejayil velo vejoaj ki im beruji": NO por potencia bruta
 * (jayil H2428 - ejercito; koaj H3581 - fuerza). SI por diseno
 * del Espiritu (ruaj H7307 - viento, aliento).
 *
 *   Mal (no-beruji): "mandar por LoRa siempre porque es la mas
 *     potente" - ignora que el vecino esta a 5 m.
 *   Bien (beruji):   elegir por adecuacion - alcance necesario,
 *     tamano que cabe, energia disponible, consumo menor.
 *
 * Algoritmo:
 *   1. Filtrar las mutsaqot del ner indicado: disponibles (hardware
 *      presente) Y activas (listas) Y que admitan el tam_mensaje Y
 *      con aceite suficiente (Ex 27:20 zaj).
 *   2. Entre las que pasan, elegir la de MENOR consumo_mw
 *      (el Espiritu prefiere lo eficiente).
 *   3. Si ninguna pasa, fallback a LoRa si esta disponible -
 *      LoRa es TAMID (Ex 27:21 juqqat 'olam), la columna vertebral
 *      del geza' (Ex 25:31).
 *   4. Si LoRa tampoco esta disponible -> ERR_SIN_MUTSAQA.
 *
 * --- REDUNDANCIA (Zac 4:3) ---
 *
 * Dos olivos alimentan la gulla: ejad miyamin, ejad 'al smola. Si
 * uno falla, el otro sigue vertiendo. En firmware: si una mutsaqa
 * cae, las otras siguen disponibles. El selector NO se cuelga
 * porque una fuente falte - escoge entre las que permanecen.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_comm.h            -> API publica (enums, structs, errores)
 *   hashmal_comm_internal.h   -> mutsaqa_mut, mutsaqa_legitima,
 *                                canal_legitimo, get_menora_interno
 *   esp_log.h (ORO DE EGIPTO) -> MUTS_LOG_*
 *
 * NO habla SPI ni WiFi ni BLE directo - el selector habla de
 * POLITICA. Los drivers hablan de SILICIO. Separacion de capas
 * como en menora_canales.c / menora_lora_driver.c.
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <inttypes.h>
#include <string.h>

#include "esp_log.h"

#include "hashmal_comm.h"
#include "hashmal_comm_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *MUTS_TAG = "hashmal.menora.mutsaqot";

#define MUTS_LOG_I(fmt, ...)  ESP_LOGI(MUTS_TAG, fmt, ##__VA_ARGS__)
#define MUTS_LOG_W(fmt, ...)  ESP_LOGW(MUTS_TAG, fmt, ##__VA_ARGS__)
#define MUTS_LOG_E(fmt, ...)  ESP_LOGE(MUTS_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * UMBRALES DE BERUJI - por mutsaqa
 *
 * Tamano maximo por conducto y aceite minimo requerido. Los valores
 * son de diseno (v.40 - betavnitam), no del Padre explicitamente:
 * se derivan del limite fisico del transporte y del consumo
 * relativo de energia.
 * ================================================================== */

/* LoRa (geza', Ex 25:31) - alcance largo, bajo throughput. Limite
 * por paquete ya fijado en HASHMAL_MENSAJE_MAX_BYTES. */
#define MUTS_LORA_MAX_BYTES       HASHMAL_MENSAJE_MAX_BYTES   /* 250 */
#define MUTS_LORA_ACEITE_MIN      20u   /* consumo medio */

/* ESP-NOW (isha el ajota, Ex 26:3) - alcance medio, alto throughput.
 * Frame ESP-NOW x fragmentacion maxima admisible. */
#define MUTS_ESPNOW_MAX_BYTES                                            \
    ((size_t)HASHMAL_ESP_NOW_FRAME_MAX * (size_t)HASHMAL_ESP_NOW_FRAG_MAX)
#define MUTS_ESPNOW_ACEITE_MIN    40u   /* WiFi consume mas */

/* BLE (pa'amon, Ex 28:34) - corto alcance, minimo consumo. */
#define MUTS_BLE_MAX_BYTES        200u
#define MUTS_BLE_ACEITE_MIN       10u   /* el mas eficiente */

/* ==================================================================
 * HELPERS ESTATICOS
 * ================================================================== */

/**
 * Nombre legible del conducto, para logs. Cada mutsaqa con su
 * raiz biblica - hablamos en el idioma del Padre.
 */
static const char *nombre_mutsaqa(hashmal_mutsaqa_id_t id)
{
    switch (id) {
    case HASHMAL_MUTSAQA_LORA:    return "LORA (gézaʻ, Éx 25:31)";
    case HASHMAL_MUTSAQA_ESP_NOW: return "ESP_NOW (ishá el ajotá, Éx 26:3)";
    case HASHMAL_MUTSAQA_BLE:     return "BLE (paʻamón, Éx 28:34)";
    default:                      return "<mutsaqá desconocida>";
    }
}

/**
 * Tamano maximo de mensaje admitido por cada mutsaqa.
 * La Palabra calla sobre payload exacto; estos limites vienen del
 * tavnit fisico del transporte (v.40 - betavnitam).
 */
static size_t tamano_max_para_mutsaqa(hashmal_mutsaqa_id_t id)
{
    switch (id) {
    case HASHMAL_MUTSAQA_LORA:    return (size_t)MUTS_LORA_MAX_BYTES;
    case HASHMAL_MUTSAQA_ESP_NOW: return MUTS_ESPNOW_MAX_BYTES;
    case HASHMAL_MUTSAQA_BLE:     return (size_t)MUTS_BLE_MAX_BYTES;
    default:                      return 0u;
    }
}

/**
 * Aceite minimo (nivel 0..100) que cada mutsaqa necesita para operar.
 * Ex 27:20 - shemen zayit ZAJ: el aceite debe ser PURO. Si el
 * tanque esta bajo umbral, no se enciende la llama en ese conducto.
 */
static uint8_t aceite_minimo_para_mutsaqa(hashmal_mutsaqa_id_t id)
{
    switch (id) {
    case HASHMAL_MUTSAQA_LORA:    return MUTS_LORA_ACEITE_MIN;
    case HASHMAL_MUTSAQA_ESP_NOW: return MUTS_ESPNOW_ACEITE_MIN;
    case HASHMAL_MUTSAQA_BLE:     return MUTS_BLE_ACEITE_MIN;
    default:                      return 101u;   /* nunca alcanzable */
    }
}

/**
 * ?Esta mutsaqa puede llevar un mensaje de tam bytes desde este
 * ner en este momento? Las 4 condiciones beruji (Zac 4:6):
 *   a) hardware disponible (el conducto existe fisicamente);
 *   b) actualmente activa (no detenida por politica);
 *   c) capacidad suficiente para el tamano del mensaje;
 *   d) aceite suficiente (ner.aceite_nivel >= umbral).
 */
static bool mutsaqa_es_apta(const hashmal_mutsaqa_t *m,
                            const hashmal_ner_t *ner,
                            size_t tam_mensaje)
{
    if (m == NULL || ner == NULL)                return false;
    if (!m->disponible)                          return false;
    if (!m->activa)                              return false;
    if (tam_mensaje > tamano_max_para_mutsaqa(m->id)) return false;
    if (ner->aceite_nivel < aceite_minimo_para_mutsaqa(m->id))
        return false;
    return true;
}

/* ==================================================================
 * FUNCIONES PUBLICAS
 * ================================================================== */

/**
 * hashmal_comm_mutsaqa_seleccionar - Zacarias 4:6 beruji
 *
 * Elige el conducto optimo para un mensaje dado. No por jayil ni
 * por koaj - por ruaj: adecuacion, no potencia. Entre las aptas,
 * la de menor consumo_mw. Fallback a LoRa (geza', tamid) si
 * ninguna cumple los criterios estrictos.
 *
 * @param canal             Ner fuente (0..6).
 * @param nodo_destino      Identificador del nodo receptor (informativo -
 *                          los drivers resolveran MAC/routing luego).
 * @param tam_mensaje       Bytes a enviar (incluye cabecera del formato).
 * @param mutsaqa_elegida   [out] conducto seleccionado.
 *
 * @return HASHMAL_COMM_OK si hay seleccion valida;
 *         HASHMAL_COMM_ERR_NULL      si mutsaqa_elegida == NULL;
 *         HASHMAL_COMM_ERR_INIT      si Menora no inicializada;
 *         HASHMAL_COMM_ERR_CANAL     si canal fuera de rango;
 *         HASHMAL_COMM_ERR_TAMANO    si tam_mensaje = 0;
 *         HASHMAL_COMM_ERR_SIN_MUTSAQA si ninguna mutsaqa puede llevarlo.
 *
 * nodo_destino se valida levemente (marca informativa); el routing
 * real vive en menora_routing.c y los drivers.
 */
int hashmal_comm_mutsaqa_seleccionar(hashmal_canal_id_t canal,
                                      uint8_t nodo_destino,
                                      size_t tam_mensaje,
                                      hashmal_mutsaqa_id_t *mutsaqa_elegida)
{
    if (mutsaqa_elegida == NULL) {
        MUTS_LOG_E("seleccionar: mutsaqa_elegida NULL");
        return HASHMAL_COMM_ERR_NULL;
    }

    const hashmal_menora_t *men = hashmal_comm_get_menora_interno();
    if (men == NULL) {
        MUTS_LOG_E("seleccionar: Menorá no inicializada");
        return HASHMAL_COMM_ERR_INIT;
    }

    if (!hashmal_comm_canal_legitimo(canal)) {
        MUTS_LOG_W("seleccionar: canal %d fuera de [0, %d)",
                   (int)canal, HASHMAL_MENORA_CANALES);
        return HASHMAL_COMM_ERR_CANAL;
    }

    if (tam_mensaje == 0u) {
        MUTS_LOG_W("seleccionar: tam_mensaje = 0, nada que verter");
        return HASHMAL_COMM_ERR_TAMANO;
    }

    const hashmal_ner_t *ner = &men->nerot[canal];

    /* Pasada beruji - Zac 4:6: el Espiritu prefiere lo eficiente.
     * Recorremos las 3 mutsaqot del ner y nos quedamos con la de
     * menor consumo_mw entre las aptas. */
    hashmal_mutsaqa_id_t mejor_id = HASHMAL_MUTSAQA_LORA;
    uint16_t             mejor_consumo = 0u;
    bool                 hay_apta = false;

    for (uint8_t i = 0; i < HASHMAL_MUTSAQA_TOTAL; i++) {
        const hashmal_mutsaqa_t *m = &ner->mutsaqot[i];
        if (!mutsaqa_es_apta(m, ner, tam_mensaje)) continue;

        if (!hay_apta || m->consumo_mw < mejor_consumo) {
            mejor_id      = (hashmal_mutsaqa_id_t)i;
            mejor_consumo = m->consumo_mw;
            hay_apta      = true;
        }
    }

    if (hay_apta) {
        *mutsaqa_elegida = mejor_id;
        MUTS_LOG_I("seleccionar: canal=%d dest=%u tam=%zu → %s (%u mW) — berují",
                   (int)canal, (unsigned)nodo_destino, tam_mensaje,
                   nombre_mutsaqa(mejor_id), (unsigned)mejor_consumo);
        return HASHMAL_COMM_OK;
    }

    /* Fallback TAMID - Ex 27:21 juqqat 'olam: LoRa es el geza'
     * permanente. Si esta fisicamente disponible la elegimos aunque
     * no pasara el filtro estricto (p.ej. aceite bajo el umbral de
     * ESP-NOW/BLE pero suficiente para tamid minimo). */
    const hashmal_mutsaqa_t *lora = &ner->mutsaqot[HASHMAL_MUTSAQA_LORA];
    if (lora->disponible && tam_mensaje <= (size_t)MUTS_LORA_MAX_BYTES) {
        *mutsaqa_elegida = HASHMAL_MUTSAQA_LORA;
        MUTS_LOG_W("seleccionar: canal=%d tam=%zu → LORA fallback "
                   "(tamíd, Éx 27:21) — ninguna mutsaqá pasó berují",
                   (int)canal, tam_mensaje);
        return HASHMAL_COMM_OK;
    }

    MUTS_LOG_E("seleccionar: canal=%d tam=%zu → SIN_MUTSAQA "
               "(ni siquiera LoRa tamíd disponible)",
               (int)canal, tam_mensaje);
    return HASHMAL_COMM_ERR_SIN_MUTSAQA;
}

/**
 * hashmal_comm_mutsaqa_estado - lectura pura del conducto
 *
 * Copia el estado de UNA mutsaqa (disponible, activa, capacidad,
 * alcance, consumo, contadores) en el buffer del llamante. La
 * gulla es consultable: quien mira de cerca la vision, ve los
 * tsanterot (Zac 4:12).
 */
int hashmal_comm_mutsaqa_estado(hashmal_canal_id_t canal,
                                 hashmal_mutsaqa_id_t mutsaqa,
                                 hashmal_mutsaqa_t *out)
{
    if (out == NULL) {
        MUTS_LOG_E("estado: out NULL");
        return HASHMAL_COMM_ERR_NULL;
    }

    const hashmal_menora_t *men = hashmal_comm_get_menora_interno();
    if (men == NULL) return HASHMAL_COMM_ERR_INIT;

    if (!hashmal_comm_canal_legitimo(canal)) {
        return HASHMAL_COMM_ERR_CANAL;
    }
    if (!hashmal_comm_mutsaqa_legitima(mutsaqa)) {
        MUTS_LOG_W("estado: mutsaqa %d fuera de [0, %d)",
                   (int)mutsaqa, HASHMAL_MUTSAQA_TOTAL);
        return HASHMAL_COMM_ERR_CANAL;
    }

    memcpy(out, &men->nerot[canal].mutsaqot[mutsaqa], sizeof(*out));
    return HASHMAL_COMM_OK;
}

/**
 * hashmal_comm_mutsaqa_activar - enciende el conducto
 *
 * Marca la mutsaqa como `activa = true`. Precondicion: debe estar
 * disponible (hardware presente) - no se activa un conducto que
 * el silicio no soporta (Lv 10:1 esh zara invertido: no forzar
 * lo que el tavnit no dio).
 */
int hashmal_comm_mutsaqa_activar(hashmal_canal_id_t canal,
                                  hashmal_mutsaqa_id_t mutsaqa)
{
    hashmal_mutsaqa_t *m = hashmal_comm_mutsaqa_mut(canal, mutsaqa);
    if (m == NULL) {
        MUTS_LOG_E("activar: canal=%d mutsaqa=%d — parámetros ilegítimos "
                   "o Menorá no inicializada",
                   (int)canal, (int)mutsaqa);
        return HASHMAL_COMM_ERR_INIT;
    }

    if (!m->disponible) {
        MUTS_LOG_W("activar: canal=%d %s no disponible en hardware",
                   (int)canal, nombre_mutsaqa(mutsaqa));
        return HASHMAL_COMM_ERR_SIN_MUTSAQA;
    }

    if (m->activa) {
        /* Ya activa - idempotente */
        return HASHMAL_COMM_OK;
    }

    m->activa = true;
    MUTS_LOG_I("activar: canal=%d %s → ACTIVA",
               (int)canal, nombre_mutsaqa(mutsaqa));
    return HASHMAL_COMM_OK;
}

/**
 * hashmal_comm_mutsaqa_desactivar - apaga el conducto
 *
 * Marca la mutsaqa como `activa = false`. No afecta disponibilidad
 * (el hardware sigue ahi), solo la politica operativa. Idempotente.
 */
int hashmal_comm_mutsaqa_desactivar(hashmal_canal_id_t canal,
                                     hashmal_mutsaqa_id_t mutsaqa)
{
    hashmal_mutsaqa_t *m = hashmal_comm_mutsaqa_mut(canal, mutsaqa);
    if (m == NULL) {
        MUTS_LOG_E("desactivar: canal=%d mutsaqa=%d — parámetros "
                   "ilegítimos o Menorá no inicializada",
                   (int)canal, (int)mutsaqa);
        return HASHMAL_COMM_ERR_INIT;
    }

    if (!m->activa) {
        /* Ya inactiva - idempotente */
        return HASHMAL_COMM_OK;
    }

    m->activa = false;
    MUTS_LOG_I("desactivar: canal=%d %s → inactiva",
               (int)canal, nombre_mutsaqa(mutsaqa));
    return HASHMAL_COMM_OK;
}

/* ==================================================================
 *
 *   "menorat zahav kullah veGULLAH 'al ROSHAH... SHIV'A veSHIV'A
 *    MUTSAQOT lannerot." - Zacarias 4:2
 *
 *   "LO VEJAYIL VELO VEJOAJ ki im beRUJI amar YHWH." - Zacarias 4:6
 *
 *   "orot eilim me'oddamim uMIJSE orot tejashim MILMA'LAH." - Ex 26:14
 *
 *   La gulla no distribuye por potencia - distribuye por Espiritu.
 *   Dos olivos la alimentan (Zac 4:3); siete-y-siete mutsaqot
 *   reparten a siete lamparas. Donde el vecino esta cerca, el
 *   pa'amon basta. Donde la carga es densa, la isha el ajota
 *   vierte. Y el geza' (LoRa) siempre esta - tamid, juqqat 'olam,
 *   porque la Menora no se apaga.
 *
 *   Archivo 7 de 9 de la Menora multi-transporte. Siguientes:
 *     menora_espnow_driver.c   (isha el ajota, Ex 26:3)
 *     menora_ble_paamon.c      (pa'amon, Ex 28:34)
 *
 * ================================================================== */
