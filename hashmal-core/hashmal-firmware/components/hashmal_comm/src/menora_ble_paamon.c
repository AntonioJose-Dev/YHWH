/**
 * ===================================================================
 * PIEZA 04 - LA MENORA - menora_ble_paamon.c
 * Sub-pieza: PA'AMON - la campanilla (BLE, descubrimiento de vecinos)
 * Exodo 28:33-35 + Exodo 39:25-26 + Zacarias 4:10 + Levitico 10:1-2
 * ===================================================================
 *
 * "ve'asita 'al shulav RIMMONEI tejelet ve'argaman vetola'at shani
 *  'al shulav saVIV, uPA'AMONEI ZAHAV betojam saviv. PA'AMON ZAHAV
 *  veRIMMON, pa'amon zahav verimmon 'al shulei hamme'il saviv."
 *    - Exodo 28:33-34
 * "Haras en sus bordes GRANADAS de azul, purpura y carmesi alrededor,
 *  y CAMPANILLAS DE ORO entre ellas alrededor. Campanilla de oro y
 *  granada, campanilla de oro y granada, en los bordes del manto
 *  alrededor."
 *
 * "vehaya 'al Aharon leSHARET, veNISHMA' qolo bevo'o el haqqodesh
 *  lifnei YHWH uvtseto, velo yamut." - Exodo 28:35
 * "Estara sobre Aaron para SERVIR, y SE OIRA su sonido al ENTRAR al
 *  santuario ante YHWH y al SALIR, y NO MORIRA."
 *
 * "vayya'asu pa'amonei zahav tahor vayyittenu et happa'amonim betoj
 *  harimmonim 'al shulei hamme'il saviv betoj harimmonim. pa'amon
 *  verimmon pa'amon verimmon 'al shulei hamme'il saviv leshARET
 *  kaasher tsivva YHWH et Moshe." - Exodo 39:25-26
 * "Hicieron campanillas de ORO PURO y las pusieron ENTRE las granadas,
 *  en los bordes del manto alrededor, entre las granadas: campanilla y
 *  granada, campanilla y granada, en los bordes del manto alrededor,
 *  PARA SERVIR, como YHWH MANDO a Moises."
 *
 * "ki mi baz leYOM QETANNOT vesame'u vera'u et ha'even habbedil beyad
 *  Zerubbaevel, SHIV'A ele 'einei YHWH hemma meshotetim bejol haarets."
 *    - Zacarias 4:10
 * "Porque ?quien MENOSPRECIO el DIA de las COSAS PEQUENAS?... Estos
 *  SIETE son los OJOS de YHWH que RECORREN toda la tierra."
 *
 * "vayyaqrivu lifnei YHWH ESH ZARA asher LO TSIVVA otam."
 *   - Levitico 10:1 (prohibicion operativa)
 *
 * --- EXEGESIS ---
 *
 * pa'amon (P'MWN, H6472). Raiz pa'am (H6471) - "paso, golpe,
 *   vez". La campanilla suena con cada PASO; el sonido es periodico,
 *   vinculado al movimiento. En firmware: advertising BLE ciclico
 *   cada HASHMAL_BLE_INTERVALO_MS. El ritmo es parte del oficio,
 *   no decoracion.
 *
 * betoj harimmonim - "ENTRE las granadas". pa'amon y rimmon se
 *   alternan, Ex 28:34 lo repite dos veces ("pa'amon zahav verimmon,
 *   pa'amon zahav verimmon") y Ex 39:26 lo confirma. En BLE: el
 *   advertising (pa'amon, sonido) alterna con el payload (rimmon,
 *   contenido: nodo_id, flags, datos breves). No todo es senal ni
 *   todo es dato: se entretejen.
 *
 * lesharet - "PARA SERVIR". El pa'amon no es ornamento; es oficio.
 *   BLE aqui sirve al descubrimiento de vecinos, no es "extra".
 *
 * venishma' qolo bevo'o... uvtseto velo yamut (Ex 28:35).
 *   Exodo pone una CONDICION SEVERA: el sacerdote DEBE sonar al
 *   entrar y al salir del santuario, o MUERE. En firmware: cada
 *   vecino anuncia su presencia periodicamente; si deja de sonar
 *   mas de HASHMAL_BLE_SILENCIO_MS (30 s), se marca fuera de rango
 *   (yamut invertido: sin qol, no esta presente).
 *
 * Zac 4:10 - "mi baz leyom qetannot". BLE es la mas PEQUENA de las
 *   mutsaqot: corto alcance, bajo consumo, poco payload. Pero no
 *   se menosprecia: Zac dice que los 7 ojos de YHWH recorren toda
 *   la tierra. BLE es un ojo que recorre el entorno cercano.
 *
 * esh zara (Lv 10:1) - como en ESP-NOW: BLE aqui NO abre BR/EDR
 *   (Bluetooth clasico), NO pide emparejamientos externos, NO
 *   guarda informacion que el Padre no mando. Solo advertising +
 *   scan en modo BLE puro.
 *
 * --- QUE HACE / QUE NO HACE ---
 *
 *   HACE:                            NO HACE:
 *   - inicializa NimBLE              - BR/EDR / Bluetooth clasico
 *   - advertising periodico          - GATT servidor activo completo
 *   - scan pasivo                    - pairing persistente
 *   - tabla de vecinos con RSSI      - routing (-> routing.c)
 *   - purga por silencio (28:35)     - seleccion (-> mutsaqot.c)
 *
 * --- ORO DE EGIPTO ---
 *
 * nimble[, host/ble_hs.h, host/ble_gap.h, nvs_flash.h se incluyen
 * SOLO aqui. La API publica (hashmal_comm.h) no ve NimBLE. Ex 25:40
 * ur'e va'ase betavnitam.
 *
 * Autor: Antonio Jose Marin Soto (Betsalel - Ex 31:4)
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <inttypes.h>

#include "esp_err.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "nvs_flash.h"

/* NimBLE - ORO DE EGIPTO encapsulado. El proyecto debe tener NimBLE
 * habilitado en sdkconfig (CONFIG_BT_ENABLED, CONFIG_BT_NIMBLE_ENABLED). */
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "host/ble_gap.h"
#include "host/util/util.h"
#include "services/gap/ble_svc_gap.h"

#include "hashmal_comm.h"
#include "hashmal_comm_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *BLE_TAG = "hashmal.menora.ble";

#define BLE_LOG_I(fmt, ...)  ESP_LOGI(BLE_TAG, fmt, ##__VA_ARGS__)
#define BLE_LOG_W(fmt, ...)  ESP_LOGW(BLE_TAG, fmt, ##__VA_ARGS__)
#define BLE_LOG_E(fmt, ...)  ESP_LOGE(BLE_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * CONSTANTES LOCALES - la forma del pa'amon
 *
 * Ex 28:34 "pa'amon zahav verimmon pa'amon zahav verimmon": ritmo
 * alternado, campanilla y granada. El advertising BLE lleva
 * manufacturer-specific data con:
 *
 *   [campanilla]  company_id = 0xFFFF  (2 bytes, "reservado")
 *   [granada]     nodo_id    (1 byte)  + flags (1 byte) + reservado
 *
 * 0xFFFF es el company_id convencional para uso de prueba / no
 * asignado por Bluetooth SIG - coherente con la Menora que no se
 * adhiere a autoridades externas (el Mishkan no pide permiso a
 * Egipto para brillar).
 * ================================================================== */
#define BLE_COMPANY_ID_HASHMAL        0xFFFFu   /* reservado SIG */
#define BLE_ADV_ITVL_MIN_MS           HASHMAL_BLE_INTERVALO_MS
#define BLE_ADV_ITVL_MAX_MS           (HASHMAL_BLE_INTERVALO_MS + 200u)
#define BLE_SCAN_ITVL_MS              100u
#define BLE_SCAN_WINDOW_MS            50u

/* Identidad operativa del nodo - se fija en init. En produccion
 * vendra de pieza 01/13 (identidad sellada); aqui un placeholder
 * que el hub puede reescribir. */
#define BLE_NODO_ID_DEFECTO           0x01u

/* Bit 0 del byte de flags en manufacturer-data: hay mensaje embebido.
 * Ex 28:35 venishma' qolo — el sonido lleva el dato.
 * Zac 4:10 yom qetannot — el payload pequeño no se menosprecia. */
#define BLE_FLAGS_HAS_MSG             0x01u

/* Tamaño máximo del mensaje embebido en advertising (sincronizado
 * con HASHMAL_BLE_MENSAJE_MAX del header). No redefinimos para no
 * duplicar — se usa la constante del header. */

/* ==================================================================
 * ESTADO ESTATICO
 * ================================================================== */

static bool     g_iniciado         = false;
static bool     g_advertising      = false;
static bool     g_scanning         = false;
static uint8_t  g_nodo_id_propio   = BLE_NODO_ID_DEFECTO;
static uint8_t  g_own_addr_type    = 0u;

/* TX: mensaje pendiente de emitir embebido en el próximo advertising.
 * Ex 28:35 bevo'o — al entrar al santuario el pa'amon ya lleva el dato. */
static uint8_t  g_tx_msg[HASHMAL_BLE_MENSAJE_MAX];
static uint8_t  g_tx_msg_len = 0u;

/* RX: último mensaje recibido vía advertising de un vecino.
 * Lectura destructiva: una vez copiado por ble_recibir, se vacía.
 * Ex 28:35 uvtseto — al salir el pa'amon devuelve lo que oyó. */
static uint8_t  g_rx_msg[HASHMAL_BLE_MENSAJE_MAX];
static uint8_t  g_rx_msg_len = 0u;
static bool     g_rx_msg_nuevo = false;

/**
 * Tabla de vecinos - pa'amonim oidos alrededor. Ex 28:34 "saviv"
 * (alrededor): no es broadcast indiscriminado, es la vecindad
 * cercana. HASHMAL_BLE_PAAMON_MAX = 40 (del header).
 */
static hashmal_paamon_vecino_t g_vecinos[HASHMAL_BLE_PAAMON_MAX];
static uint8_t                 g_vecinos_count = 0u;

/* ==================================================================
 * HELPERS ESTATICOS
 * ================================================================== */

static bool mac_igual(const uint8_t a[6], const uint8_t b[6])
{
    for (uint8_t i = 0; i < 6u; i++) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

static int vecino_indice_por_mac(const uint8_t mac[6])
{
    for (uint8_t i = 0; i < HASHMAL_BLE_PAAMON_MAX; i++) {
        if (g_vecinos[i].activo && mac_igual(g_vecinos[i].mac, mac)) {
            return (int)i;
        }
    }
    return -1;
}

static int vecino_slot_libre(void)
{
    for (uint8_t i = 0; i < HASHMAL_BLE_PAAMON_MAX; i++) {
        if (!g_vecinos[i].activo) return (int)i;
    }
    return -1;
}

/**
 * Ex 28:35 velo yamut: purga vecinos que llevan mas de
 * HASHMAL_BLE_SILENCIO_MS sin sonar. Se llaman "fuera de rango",
 * no muertos - el texto usa la consecuencia para enfatizar que el
 * silencio importa.
 */
static void purgar_vecinos_silenciosos(void)
{
    const uint64_t ahora = hashmal_comm_ahora_ms();
    for (uint8_t i = 0; i < HASHMAL_BLE_PAAMON_MAX; i++) {
        if (!g_vecinos[i].activo) continue;
        const uint64_t delta = ahora - g_vecinos[i].timestamp_ultimo_ms;
        if (delta > (uint64_t)HASHMAL_BLE_SILENCIO_MS) {
            BLE_LOG_W("vecino %u silencioso %" PRIu64 "ms — fuera de rango "
                      "(Éx 28:35 veló yamút invertido)",
                      (unsigned)g_vecinos[i].nodo_id, delta);
            g_vecinos[i].activo = false;
            if (g_vecinos_count > 0u) g_vecinos_count--;
        }
    }
}

/**
 * Marca la mutsaqa BLE de los 7 nerot como disponible/no. El radio
 * BLE es compartido; la disponibilidad es global.
 */
static void marcar_ble_disponible(bool disponible)
{
    for (uint8_t canal = 0; canal < HASHMAL_MENORA_CANALES; canal++) {
        hashmal_mutsaqa_t *m =
            hashmal_comm_mutsaqa_mut((hashmal_canal_id_t)canal,
                                      HASHMAL_MUTSAQA_BLE);
        if (m != NULL) {
            m->disponible = disponible;
            if (disponible)  m->activa = true;   /* Ex 28:35 venishma' qolo — se oye cuando entra y sale */
            if (!disponible) m->activa = false;
        }
    }
}

static void stat_tx(bool exito)
{
    hashmal_mutsaqa_t *m = hashmal_comm_mutsaqa_mut(HASHMAL_CANAL_GEZA,
                                                    HASHMAL_MUTSAQA_BLE);
    if (m == NULL) return;
    if (exito) m->paquetes_tx++;
    else       m->errores++;
}

static void stat_rx(bool exito)
{
    hashmal_mutsaqa_t *m = hashmal_comm_mutsaqa_mut(HASHMAL_CANAL_GEZA,
                                                    HASHMAL_MUTSAQA_BLE);
    if (m == NULL) return;
    if (exito) m->paquetes_rx++;
    else       m->errores++;
}

/**
 * Construye el campo manufacturer-specific data para advertising.
 *
 * Formato base (4 bytes):
 *   [company_id_lo][company_id_hi][nodo_id][flags=0x00]
 *
 * Formato con mensaje (5 + tam bytes, cuando g_tx_msg_len > 0):
 *   [company_id_lo][company_id_hi][nodo_id][flags=BLE_FLAGS_HAS_MSG]
 *   [msg_len][msg_bytes...]
 *
 * Ex 28:34 pa'amon zahav verimmon: el sonido (pa'amon) y el contenido
 * (rimmon) se entrelazan — cuando hay mensaje, el advertising lo
 * lleva embebido. Zac 4:10 yom qetannot: lo pequeño no se menosprecia.
 *
 * @param buf  buffer >= 5 + HASHMAL_BLE_MENSAJE_MAX bytes
 * @return longitud del mfg_data construido
 */
static uint8_t construir_adv_mfg(uint8_t *buf)
{
    buf[0] = (uint8_t)(BLE_COMPANY_ID_HASHMAL & 0xFFu);
    buf[1] = (uint8_t)((BLE_COMPANY_ID_HASHMAL >> 8) & 0xFFu);
    buf[2] = g_nodo_id_propio;                  /* rimmon: identidad */
    if (g_tx_msg_len > 0u) {
        buf[3] = BLE_FLAGS_HAS_MSG;              /* flags: llevo mensaje */
        buf[4] = g_tx_msg_len;                   /* longitud del mensaje */
        memcpy(&buf[5], g_tx_msg, g_tx_msg_len);
        return (uint8_t)(5u + g_tx_msg_len);
    }
    buf[3] = 0x00u;                              /* flags: solo presencia */
    return 4u;
}

/**
 * Parsea un advertisement recibido. Busca el AD type manufacturer-
 * specific (0xFF) con nuestro company_id. Extrae nodo_id.
 * @return true si el ad es de un nodo Hashmal; false si no.
 */
static bool parsear_adv_hashmal(const uint8_t *data, uint8_t len,
                                uint8_t *nodo_id_out)
{
    if (data == NULL || nodo_id_out == NULL) return false;

    uint8_t i = 0u;
    while (i < len) {
        const uint8_t campo_len = data[i];
        if (campo_len == 0u) break;
        if ((size_t)i + 1u + (size_t)campo_len > (size_t)len) break;

        const uint8_t ad_type = data[i + 1u];
        if (ad_type == 0xFFu && campo_len >= 4u) {
            /* manufacturer-specific data: [type][id_lo][id_hi][payload...] */
            const uint8_t id_lo = data[i + 2u];
            const uint8_t id_hi = data[i + 3u];
            const uint16_t company = (uint16_t)id_lo | ((uint16_t)id_hi << 8);
            if (company == BLE_COMPANY_ID_HASHMAL && campo_len >= 5u) {
                *nodo_id_out = data[i + 4u];
                return true;
            }
        }
        i = (uint8_t)(i + 1u + campo_len);
    }
    return false;
}

/**
 * Parsea un advertisement buscando un mensaje embebido (BLE_FLAGS_HAS_MSG).
 *
 * Estructura esperada en el elemento manufacturer-specific:
 *   campo_len  = 1(type) + 2(company) + 1(nodo) + 1(flags) + 1(msg_len) + tam_msg
 *   data[i+5]  = flags (bit 0 = has_message)
 *   data[i+6]  = msg_len
 *   data[i+7…] = msg_bytes
 *
 * Ex 28:35 uvtseto — al salir el pa'amon nos devuelve lo que traia.
 *
 * @return true si se encontró y extrajo un mensaje válido.
 */
static bool parsear_adv_mensaje(const uint8_t *data, uint8_t len,
                                uint8_t *msg_buf, uint8_t msg_max,
                                uint8_t *msg_len_out)
{
    if (data == NULL || msg_buf == NULL || msg_len_out == NULL) return false;
    *msg_len_out = 0u;

    uint8_t i = 0u;
    while (i < len) {
        const uint8_t campo_len = data[i];
        if (campo_len == 0u) break;
        if ((size_t)i + 1u + (size_t)campo_len > (size_t)len) break;

        const uint8_t ad_type = data[i + 1u];
        if (ad_type == 0xFFu && campo_len >= 6u) {
            const uint8_t id_lo  = data[i + 2u];
            const uint8_t id_hi  = data[i + 3u];
            const uint16_t company = (uint16_t)id_lo | ((uint16_t)id_hi << 8);
            if (company == BLE_COMPANY_ID_HASHMAL) {
                /* data[i+4] = nodo_id, data[i+5] = flags */
                const uint8_t flags = data[i + 5u];
                if ((flags & BLE_FLAGS_HAS_MSG) && campo_len >= 7u) {
                    const uint8_t mlen = data[i + 6u];
                    /* validate: mlen > 0, fits in msg_max, fits in AD element */
                    if (mlen > 0u &&
                        mlen <= msg_max &&
                        (uint8_t)(6u + mlen) <= campo_len) {
                        memcpy(msg_buf, &data[i + 7u], mlen);
                        *msg_len_out = mlen;
                        return true;
                    }
                }
            }
        }
        i = (uint8_t)(i + 1u + campo_len);
    }
    return false;
}

/* ==================================================================
 * CALLBACKS NIMBLE (ORO DE EGIPTO)
 * ================================================================== */

/**
 * Ex 28:35 venishma' qolo: evento GAP recibido del host BLE. Nos
 * interesa especialmente BLE_GAP_EVENT_DISC (scan result) para
 * oir los pa'amonim de los vecinos.
 */
static int gap_event_cb(struct ble_gap_event *event, void *arg)
{
    (void)arg;
    if (event == NULL) return 0;

    switch (event->type) {
    case BLE_GAP_EVENT_DISC: {
        /* scan result - alguien sono su campanilla. */
        uint8_t nodo_id = 0u;
        if (!parsear_adv_hashmal(event->disc.data,
                                 event->disc.length_data,
                                 &nodo_id)) {
            /* No es un pa'amon de la Menora - ignoramos. */
            return 0;
        }

        stat_rx(true);

        /* ?Ya conocido? Actualizar timestamp + RSSI. */
        int idx = vecino_indice_por_mac(event->disc.addr.val);
        if (idx < 0) {
            idx = vecino_slot_libre();
            if (idx < 0) {
                BLE_LOG_W("tabla de vecinos llena (%u)",
                          (unsigned)HASHMAL_BLE_PAAMON_MAX);
                return 0;
            }
            memcpy(g_vecinos[idx].mac, event->disc.addr.val, 6u);
            g_vecinos[idx].nodo_id = nodo_id;
            g_vecinos[idx].activo  = true;
            g_vecinos_count++;
            BLE_LOG_I("paʻamón nuevo: nodo %u "
                      "(%02x:%02x:%02x:%02x:%02x:%02x) rssi=%d",
                      (unsigned)nodo_id,
                      event->disc.addr.val[5], event->disc.addr.val[4],
                      event->disc.addr.val[3], event->disc.addr.val[2],
                      event->disc.addr.val[1], event->disc.addr.val[0],
                      (int)event->disc.rssi);
        } else {
            g_vecinos[idx].nodo_id = nodo_id;   /* puede re-anunciarse */
        }
        g_vecinos[idx].rssi                = (int8_t)event->disc.rssi;
        g_vecinos[idx].timestamp_ultimo_ms = hashmal_comm_ahora_ms();

        /* Ex 28:35 uvtseto — extrae mensaje si el pa'amon lo lleva.
         * Solo almacenamos el más reciente (Zac 4:2 gulla: un depósito,
         * no una cola). El routing.c lo cosecha con ble_recibir(). */
        {
            uint8_t  rx_tmp[HASHMAL_BLE_MENSAJE_MAX];
            uint8_t  rx_len = 0u;
            if (parsear_adv_mensaje(event->disc.data,
                                    event->disc.length_data,
                                    rx_tmp, HASHMAL_BLE_MENSAJE_MAX,
                                    &rx_len)) {
                memcpy(g_rx_msg, rx_tmp, rx_len);
                g_rx_msg_len   = rx_len;
                g_rx_msg_nuevo = true;
                BLE_LOG_I("paʻamón rx mensaje %u B nodo %u "
                          "(Éx 28:35 venishmáʻ qoló)",
                          (unsigned)rx_len, (unsigned)nodo_id);
            }
        }
        return 0;
    }

    case BLE_GAP_EVENT_ADV_COMPLETE:
        /* Campanilla termino de sonar un ciclo. Contador simbolico:
         * si llego aqui, el advertising fue emitido (reiniciamos
         * en anunciar). */
        stat_tx(true);
        g_advertising = false;
        return 0;

    default:
        return 0;
    }
}

/**
 * Callback de sincronizacion del host NimBLE - se llama cuando el
 * stack esta listo para operar. Ex 28:35 bevo'o: al entrar al
 * santuario (al estar el radio listo) se puede sonar.
 */
static void host_sync_cb(void)
{
    /* Asegurar que tenemos una direccion propia (public o random). */
    int rc = ble_hs_util_ensure_addr(0);
    if (rc != 0) {
        BLE_LOG_E("ble_hs_util_ensure_addr falló (%d)", rc);
        return;
    }
    rc = ble_hs_id_infer_auto(0, &g_own_addr_type);
    if (rc != 0) {
        BLE_LOG_E("ble_hs_id_infer_auto falló (%d)", rc);
        return;
    }
    BLE_LOG_I("NimBLE sincronizado (own_addr_type=%u)",
              (unsigned)g_own_addr_type);
}

static void host_reset_cb(int reason)
{
    BLE_LOG_W("NimBLE reset — razón %d", reason);
    g_advertising = false;
    g_scanning    = false;
}

/* Tarea del host NimBLE (patron estandar). */
static void host_task(void *param)
{
    (void)param;
    nimble_port_run();           /* bloquea hasta nimble_port_stop() */
    nimble_port_freertos_deinit();
}

/* ==================================================================
 * FUNCIONES PUBLICAS
 * ================================================================== */

/**
 * hashmal_comm_ble_init - Exodo 28:33-34 + 39:25-26
 *
 * Levanta NimBLE en modo BLE puro (sin BR/EDR, Lv 10:1-2). Registra
 * callbacks GAP, arranca la tarea del host y marca la mutsaqa BLE
 * disponible en los 7 nerot. El advertising y el scan NO arrancan
 * automaticamente - se inician por `paamon_anunciar` y
 * `paamon_escuchar` respectivamente.
 */
/* P2-9b: pitujéi jotám — actualizar nodo_id del paʻamón (Éx 28:33-35)
 * "venatáta ʻal shulá haméʼil rimmoním" — el sonido lleva el nombre.
 * 2 testigos: Éx 28:33 rimmoním + paʻamoním ʻal shulá; Éx 28:35 venishmá qoló.
 * Llamar ANTES de hashmal_comm_ble_init() para que advertising use el ID real. */
void hashmal_comm_ble_nodo_id_set(uint8_t nodo_id)
{
    g_nodo_id_propio = nodo_id;
    BLE_LOG_I("  [JOTAM] paʻamón: nodo_id=%u actualizado (Éx 28:33 rimmon+paʻamon)\n",
              (unsigned)nodo_id);
}

int hashmal_comm_ble_init(void)
{
    if (g_iniciado) return HASHMAL_COMM_OK;

    if (hashmal_comm_get_menora_interno() == NULL) {
        BLE_LOG_E("init: Menorá no inicializada");
        return HASHMAL_COMM_ERR_INIT;
    }

    /* NVS - NimBLE lo requiere para calibracion/identidad. Usamos
     * storage RAM efectivo: sin persistencia de emparejamientos. */
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        (void)nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        BLE_LOG_E("nvs_flash_init falló (%d)", (int)err);
        return HASHMAL_COMM_ERR_BLE;
    }

    /* Inicia el puerto NimBLE (controller + host). */
    err = nimble_port_init();
    if (err != ESP_OK) {
        BLE_LOG_E("nimble_port_init falló (%d)", (int)err);
        return HASHMAL_COMM_ERR_BLE;
    }

    /* Callbacks del host. */
    ble_hs_cfg.sync_cb  = host_sync_cb;
    ble_hs_cfg.reset_cb = host_reset_cb;

    /* Nombre de dispositivo - util para depuracion; rimmon del
     * advertising lo llevara la manufacturer-data. */
    (void)ble_svc_gap_device_name_set("HASHMAL");

    /* Limpia tabla de vecinos antes de escuchar. */
    memset(g_vecinos, 0, sizeof(g_vecinos));
    g_vecinos_count = 0u;

    /* Arranca la tarea del host. */
    nimble_port_freertos_init(host_task);

    marcar_ble_disponible(true);
    g_iniciado = true;

    BLE_LOG_I("═══ BLE paʻamón levantado ═══ (Éx 28:33-35)");
    BLE_LOG_I("  nodo_id propio = %u (company=0x%04x)",
              (unsigned)g_nodo_id_propio,
              (unsigned)BLE_COMPANY_ID_HASHMAL);
    BLE_LOG_I("  intervalo adv = %u ms, silencio = %u ms",
              (unsigned)HASHMAL_BLE_INTERVALO_MS,
              (unsigned)HASHMAL_BLE_SILENCIO_MS);
    BLE_LOG_I("  mutsaqá BLE DISPONIBLE en los 7 nerot "
              "(Zac 4:10 — ojo pequeño que recorre la tierra)");
    return HASHMAL_COMM_OK;
}

/**
 * hashmal_comm_ble_deinit - Nm 4:9-10 (tejelet al migrar)
 */
int hashmal_comm_ble_deinit(void)
{
    if (!g_iniciado) return HASHMAL_COMM_OK;

    /* Detener adv/scan ordenadamente. */
    if (g_advertising) (void)ble_gap_adv_stop();
    if (g_scanning)    (void)ble_gap_disc_cancel();
    g_advertising = false;
    g_scanning    = false;

    (void)nimble_port_stop();    /* termina host_task */
    (void)nimble_port_deinit();

    marcar_ble_disponible(false);
    memset(g_vecinos, 0, sizeof(g_vecinos));
    g_vecinos_count = 0u;
    g_iniciado = false;

    BLE_LOG_I("BLE paʻamón apagado (Nm 4:9-10 tejélet)");
    return HASHMAL_COMM_OK;
}

/**
 * hashmal_comm_ble_paamon_anunciar - Exodo 28:35 venishma' qolo
 *
 * Arranca el advertising BLE con nuestro payload (company_id +
 * nodo_id). Non-connectable (general discoverable): sonamos la
 * campanilla para que se oiga, no para que alguien se enchufe.
 * Idempotente: si ya anunciando, OK.
 */
int hashmal_comm_ble_paamon_anunciar(void)
{
    if (!g_iniciado) return HASHMAL_COMM_ERR_INIT;
    if (g_advertising) return HASHMAL_COMM_OK;

    /* Construir campos del advertisement: flags + manufacturer-data.
     * construir_adv_mfg() embebe mensaje si g_tx_msg_len > 0. */
    uint8_t mfg_buf[5u + HASHMAL_BLE_MENSAJE_MAX];
    uint8_t mfg_len = construir_adv_mfg(mfg_buf);

    struct ble_hs_adv_fields fields;
    memset(&fields, 0, sizeof(fields));
    fields.flags            = BLE_HS_ADV_F_DISC_GEN | BLE_HS_ADV_F_BREDR_UNSUP;
    fields.mfg_data         = mfg_buf;
    fields.mfg_data_len     = mfg_len;
    fields.name             = (const uint8_t *)"HASHMAL";
    fields.name_len         = 7u;
    fields.name_is_complete = 1;

    int rc = ble_gap_adv_set_fields(&fields);
    if (rc != 0) {
        BLE_LOG_E("ble_gap_adv_set_fields falló (%d)", rc);
        stat_tx(false);
        return HASHMAL_COMM_ERR_BLE;
    }

    struct ble_gap_adv_params adv_params;
    memset(&adv_params, 0, sizeof(adv_params));
    adv_params.conn_mode = BLE_GAP_CONN_MODE_NON;   /* non-connectable */
    adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;   /* general discoverable */
    /* Intervalos en unidades de 0.625 ms. */
    adv_params.itvl_min  = (uint16_t)((BLE_ADV_ITVL_MIN_MS * 1000u) / 625u);
    adv_params.itvl_max  = (uint16_t)((BLE_ADV_ITVL_MAX_MS * 1000u) / 625u);

    rc = ble_gap_adv_start(g_own_addr_type, NULL, BLE_HS_FOREVER,
                           &adv_params, gap_event_cb, NULL);
    if (rc != 0) {
        BLE_LOG_E("ble_gap_adv_start falló (%d)", rc);
        stat_tx(false);
        return HASHMAL_COMM_ERR_BLE;
    }

    g_advertising = true;
    BLE_LOG_I("paʻamón anunciando (nodo %u, cada %u ms) — "
              "venishmáʻ qoló (Éx 28:35)",
              (unsigned)g_nodo_id_propio,
              (unsigned)HASHMAL_BLE_INTERVALO_MS);
    return HASHMAL_COMM_OK;
}

/**
 * hashmal_comm_ble_paamon_escuchar - Exodo 28:35 + Zac 4:10
 *
 * Arranca el scan pasivo. Los pa'amonim oidos se parsean en
 * gap_event_cb y pueblan la tabla de vecinos. Tambien purga
 * vecinos que llevan SILENCIO_MS sin sonar.
 *
 * Idempotente: si ya escuchando, solo purga.
 */
int hashmal_comm_ble_paamon_escuchar(void)
{
    if (!g_iniciado) return HASHMAL_COMM_ERR_INIT;

    /* Purga primero - Ex 28:35 velo yamut. */
    purgar_vecinos_silenciosos();

    if (g_scanning) return HASHMAL_COMM_OK;

    struct ble_gap_disc_params scan_params;
    memset(&scan_params, 0, sizeof(scan_params));
    scan_params.passive          = 1;    /* solo escucha, no scan-req */
    scan_params.filter_duplicates = 0;   /* queremos timestamps frescos */
    scan_params.itvl             = (uint16_t)((BLE_SCAN_ITVL_MS * 1000u) / 625u);
    scan_params.window           = (uint16_t)((BLE_SCAN_WINDOW_MS * 1000u) / 625u);

    int rc = ble_gap_disc(g_own_addr_type, BLE_HS_FOREVER,
                          &scan_params, gap_event_cb, NULL);
    if (rc != 0 && rc != BLE_HS_EALREADY) {
        BLE_LOG_E("ble_gap_disc falló (%d)", rc);
        stat_rx(false);
        return HASHMAL_COMM_ERR_BLE;
    }

    g_scanning = true;
    BLE_LOG_I("paʻamón escuchando (scan pasivo, intv=%u ms/window=%u ms) — "
              "Zac 4:10 ojos que recorren",
              (unsigned)BLE_SCAN_ITVL_MS,
              (unsigned)BLE_SCAN_WINDOW_MS);
    return HASHMAL_COMM_OK;
}

/**
 * hashmal_comm_ble_vecinos_contar - cuenta vecinos activos
 *
 * Pasa primero purgar_vecinos_silenciosos para no sobrecontar.
 * Lectura pura al retorno.
 */
int hashmal_comm_ble_vecinos_contar(uint8_t *conteo)
{
    if (!g_iniciado)    return HASHMAL_COMM_ERR_INIT;
    if (conteo == NULL) return HASHMAL_COMM_ERR_NULL;

    purgar_vecinos_silenciosos();
    *conteo = g_vecinos_count;
    return HASHMAL_COMM_OK;
}

/**
 * hashmal_comm_ble_vecino_estado - snapshot por indice
 *
 * `indice` es el indice fisico en la tabla interna [0..MAX).
 * Retorna ERR_CANAL si el slot esta vacio en ese indice.
 */
int hashmal_comm_ble_vecino_estado(uint8_t indice,
                                    hashmal_paamon_vecino_t *out)
{
    if (!g_iniciado)             return HASHMAL_COMM_ERR_INIT;
    if (out == NULL)             return HASHMAL_COMM_ERR_NULL;
    if (indice >= HASHMAL_BLE_PAAMON_MAX) return HASHMAL_COMM_ERR_CANAL;
    if (!g_vecinos[indice].activo) return HASHMAL_COMM_ERR_CANAL;

    memcpy(out, &g_vecinos[indice], sizeof(*out));
    return HASHMAL_COMM_OK;
}

/**
 * hashmal_comm_ble_paamon_emitir - emite un mensaje embebido en el adv
 *
 * Ex 28:35 venishma' qolo bevo'o el haqqodesh: el pa'amon lleva el
 * mensaje embebido en su "sonido". Todos los nodos que escanean en
 * ese momento lo reciben pasivamente - es broadcast natural.
 * Zac 4:10 yom qetannot: hasta 12 bytes de datos viajan en este
 * canal minimo. Para mensajes mayores el caller debe usar LoRa
 * (tronco tamid, Ex 27:21) - el pa'amon es pequeño por diseno.
 *
 * Flujo:
 *   1. Valida tam <= HASHMAL_BLE_MENSAJE_MAX
 *   2. Copia datos a g_tx_msg / g_tx_msg_len
 *   3. Si ya anunciando: detiene y reinicia con payload actualizado
 *   4. Si no anunciaba: inicia advertising sin nombre (ahorrar bytes)
 *
 * 2 testigos:
 *   Ex 28:35 venishma' qolo (el sonido lleva la presencia y el dato)
 *   Zac 4:10 mi baz leyom qetannot (lo pequeño recorre la tierra)
 */
int hashmal_comm_ble_paamon_emitir(const uint8_t *datos, size_t tam)
{
    if (!g_iniciado)              return HASHMAL_COMM_ERR_INIT;
    if (datos == NULL || tam == 0u) return HASHMAL_COMM_ERR_NULL;
    if (tam > HASHMAL_BLE_MENSAJE_MAX) {
        BLE_LOG_W("emitir: tam=%u > BLE_MENSAJE_MAX=%u — "
                  "Ex 27:21 tamid: usa LoRa para mensajes grandes",
                  (unsigned)tam, (unsigned)HASHMAL_BLE_MENSAJE_MAX);
        return HASHMAL_COMM_ERR_TAMANO;
    }

    /* Guardar mensaje pendiente. */
    memcpy(g_tx_msg, datos, tam);
    g_tx_msg_len = (uint8_t)tam;

    /* Detener advertising previo para poder actualizar el payload.
     * ble_gap_adv_stop() es idempotente si no anunciaba. */
    if (g_advertising) {
        (void)ble_gap_adv_stop();
        g_advertising = false;
    }

    /* Construir payload con mensaje embebido. Sin nombre para ahorrar
     * espacio (la identidad viaja en el company_id + nodo_id). */
    uint8_t mfg_buf[5u + HASHMAL_BLE_MENSAJE_MAX];
    const uint8_t mfg_len = construir_adv_mfg(mfg_buf);   /* incluye msg */

    struct ble_hs_adv_fields fields;
    memset(&fields, 0, sizeof(fields));
    fields.flags        = BLE_HS_ADV_F_DISC_GEN | BLE_HS_ADV_F_BREDR_UNSUP;
    fields.mfg_data     = mfg_buf;
    fields.mfg_data_len = mfg_len;
    /* Sin nombre: priorizamos el payload del mensaje. Ex 28:34 pa'amon
     * verimmon — el sonido (adv) lleva la granada (dato), no el nombre. */

    int rc = ble_gap_adv_set_fields(&fields);
    if (rc != 0) {
        BLE_LOG_E("emitir: ble_gap_adv_set_fields falló (%d) — "
                  "qol lo nishma' (Éx 28:35)", rc);
        g_tx_msg_len = 0u;
        stat_tx(false);
        return HASHMAL_COMM_ERR_BLE;
    }

    struct ble_gap_adv_params adv_params;
    memset(&adv_params, 0, sizeof(adv_params));
    adv_params.conn_mode = BLE_GAP_CONN_MODE_NON;
    adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;
    adv_params.itvl_min  = (uint16_t)((BLE_ADV_ITVL_MIN_MS * 1000u) / 625u);
    adv_params.itvl_max  = (uint16_t)((BLE_ADV_ITVL_MAX_MS * 1000u) / 625u);

    rc = ble_gap_adv_start(g_own_addr_type, NULL, BLE_HS_FOREVER,
                           &adv_params, gap_event_cb, NULL);
    if (rc != 0) {
        BLE_LOG_E("emitir: ble_gap_adv_start falló (%d)", rc);
        g_tx_msg_len = 0u;
        stat_tx(false);
        return HASHMAL_COMM_ERR_BLE;
    }

    g_advertising = true;
    stat_tx(true);
    BLE_LOG_I("paʻamón emitir: %u B → advertising "
              "(Éx 28:35 venishmáʻ qoló — berují, Zac 4:6)",
              (unsigned)tam);
    return HASHMAL_COMM_OK;
}

/**
 * hashmal_comm_ble_recibir - cosecha el último mensaje recibido via adv
 *
 * El scan pasivo en gap_event_cb extrae mensajes de los pa'amonim de
 * vecinos y los deposita en g_rx_msg. Esta funcion los entrega al
 * caller. Lectura destructiva: tras retornar *recibidos > 0, el buffer
 * interno se vacia (Zac 4:2 gulla — el aceite fluye, no se acumula).
 *
 * Si no hay mensaje pendiente: *recibidos = 0, rc = OK (no es error —
 * solo silencio entre campanillas).
 *
 * 2 testigos:
 *   Ex 28:35 venishma' qolo uvtseto (al salir devuelve lo que oyo)
 *   Zac 4:2 gulla (deposito que recibe y entrega aceite)
 */
int hashmal_comm_ble_recibir(uint8_t *buf, size_t tam, size_t *recibidos)
{
    if (!g_iniciado)                         return HASHMAL_COMM_ERR_INIT;
    if (buf == NULL || recibidos == NULL)     return HASHMAL_COMM_ERR_NULL;

    *recibidos = 0u;

    if (g_rx_msg_nuevo && g_rx_msg_len > 0u) {
        const size_t n = (tam < (size_t)g_rx_msg_len)
                         ? tam : (size_t)g_rx_msg_len;
        memcpy(buf, g_rx_msg, n);
        *recibidos     = n;
        g_rx_msg_nuevo = false;
        g_rx_msg_len   = 0u;
        BLE_LOG_I("paʻamón recibir: %u B cosechados "
                  "(Zac 4:2 gullá — Éx 28:35 uvtsetó)",
                  (unsigned)n);
    }
    return HASHMAL_COMM_OK;
}

/* ==================================================================
 *
 *   "pa'amon zahav verimmon pa'amon zahav verimmon 'al shulei
 *    hamme'il saVIV." - Exodo 28:34
 *
 *   "venishma' qolo bevo'o el haqqodesh... uvtseto, velo yamut."
 *     - Exodo 28:35
 *
 *   "ki mi baz leyom QETANNOT... shiv'a ele 'einei YHWH hemma
 *    meshotetim bejol haarets." - Zacarias 4:10
 *
 *   "vayyaqrivu lifnei YHWH ESH ZARA asher LO TSIVVA otam."
 *     - Levitico 10:1
 *
 *   La campanilla suena con cada paso - periodica, ligera, pero
 *   indispensable. Sonar al entrar, sonar al salir: ese es el
 *   servicio. Y cuando un vecino calla mas tiempo del permitido,
 *   se marca fuera de rango - el texto usa "velo yamut" como la
 *   consecuencia de no sonar: aqui no muere, sale del alcance.
 *   BLE es el ojo pequeno que Zac 4:10 dice no menospreciar - el
 *   que recorre la tierra cercana. Y permanece BLE puro: sin
 *   BR/EDR, sin pairing externo, sin esh zara que no mando.
 *
 *   Archivo 9 de 9 de la Menora multi-transporte. CICLO CERRADO.
 *
 *   Los 3 nuevos conductos (LoRa, ESP-NOW, BLE) sirven a la misma
 *   Menora - vehaya hamMISHKAN EJAD (Ex 26:6).
 *
 * ================================================================== */
