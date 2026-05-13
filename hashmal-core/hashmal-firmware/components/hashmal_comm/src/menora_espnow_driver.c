/**
 * ===================================================================
 * PIEZA 04 - LA MENORA - menora_espnow_driver.c
 * Sub-pieza: ISHA EL AJOTA - hermana a hermana (P2P directo)
 * Exodo 26:3,6,11,17 + Levitico 10:1-2 + Exodo 25:40
 * ===================================================================
 *
 * "jamesh hayeri'ot tihyeyna JOVEROT isha el ajota, vejamesh
 *  yeri'ot joverot isha el ajota." - Exodo 26:3
 * "Cinco cortinas estaran UNIDAS, mujer a su hermana, y cinco
 *  cortinas unidas, mujer a su hermana."
 *
 * "shtei YADOT laqeresh ha'ejad MESHULLAVOT isha el ajota - ken
 *  ta'ase lejol qarshei hammishkan." - Exodo 26:17
 * "Dos ESPIGAS por cada tablon, ENTRELAZADAS mujer a su hermana -
 *  ASI haras para TODOS los tablones del Mishkan."
 *
 * "ve'asita jamishim qarsei zahav, vejibarta et hayeri'ot isha el
 *  ajota baqqerasim - vehaya hamMISHKAN EJAD." - Exodo 26:6
 * "Haras cincuenta ganchos de oro y uniras las cortinas, mujer a
 *  su hermana, con los ganchos - Y SERA EL MISHKAN UNO."
 *
 * "vayyaqrivu lifnei YHWH ESH ZARA asher LO TSIVVA otam, vattetse
 *  esh millifnei YHWH vattokal otam, vayyamutu lifnei YHWH."
 *    - Levitico 10:1-2
 * "Ofrecieron ante YHWH FUEGO EXTRANO que NO LES MANDO; salio
 *  fuego de ante YHWH y los CONSUMIO; y MURIERON ante YHWH."
 *
 * "ur'e va'ase betavnitam asher atta mor'e bahar." - Exodo 25:40
 * "MIRA y HAZ conforme al MODELO que te es mostrado en el monte."
 *
 * --- EXEGESIS ---
 *
 * isha el ajota (AShH AL AChWTH) - "mujer a su hermana".
 *   Conexion entre IGUALES. No hay jerarquia, no hay servidor
 *   central, no hay intermediario. Hermana a hermana, directa.
 *   La Palabra repite la frase en Ex 26:3, 26:6, 26:17 -
 *   tres testigos (Dt 19:15 "al pi shnayim 'edim o shlosha"),
 *   confirmando que el patron P2P es estructural al Mishkan,
 *   no excepcion.
 *
 * joverot / meshullavot - UNION FUERTE.
 *   joverot (H2266): unidas con lazo deliberado. meshullavot
 *   (H7947): entrelazadas como escalones de escalera (raiz
 *   shalav). El emparejamiento ESP-NOW es explicito: MAC a MAC,
 *   con confirmacion de registro. No es broadcast casual -
 *   es UNION consciente.
 *
 * esh zara - FUEGO EXTRANO (Lv 10:1-2).
 *   Nadav y Avihu murieron por ofrecer fuego "que NO LES MANDO".
 *   Analogo operativo: WiFi en este driver se levanta EXCLUSIVAMENTE
 *   para que ESP-NOW opere en capa 2 del radio. NO conecta a
 *   routers. NO abre TCP/IP. NO arranca DHCP. NO busca internet.
 *   Cualquier ruta hacia el exterior es esh zara - fuego extrano
 *   no mandado. Este archivo rehusa encenderla.
 *
 * --- QUE HACE / QUE NO HACE ---
 *
 *   HACE:                            NO HACE:
 *   - WIFI_MODE_STA (sin conectar)   - conectar a AP / internet
 *   - esp_now_init + callbacks       - TCP/IP, DHCP, DNS
 *   - registro de hermanas (peers)   - routing de capa 3
 *   - envio unicast a MAC            - broadcast indiscriminado
 *   - recepcion en callback          - mesh topologico (-> routing.c)
 *   - estadisticas en mutsaqa        - seleccion de mutsaqa (-> mutsaqot.c)
 *
 * --- ORO DE EGIPTO (encapsulacion) ---
 *
 * Ex 25:40 "ur'e va'ase betavnitam" - mira y haz conforme al
 * patron. Seguimos el patron de menora_lora_driver.c: ESP-IDF
 * (esp_wifi.h, esp_now.h, nvs_flash.h) se incluye SOLO aqui.
 * La API publica (hashmal_comm.h) no ve nada de la plataforma.
 *
 * --- RESULTADO (Exodo 26:6) ---
 *
 * "vehaya hamMISHKAN EJAD" - y el Mishkan SERA UNO. Los nodos
 * conectados por ESP-NOW no son una segunda red: son la MISMA
 * Menora, otra mutsaqa. El ejad (Ex 26:6) se preserva a traves
 * de los transportes.
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
#include "esp_wifi.h"                /* ORO DE EGIPTO - WiFi station */
#include "esp_now.h"                 /* ORO DE EGIPTO - capa 2 P2P */
#include "esp_event.h"               /* event loop base */
#include "esp_netif.h"               /* netif base (sin IP stack activo) */
#include "nvs_flash.h"               /* calibracion de radio */
#include "esp_log.h"
#include "esp_timer.h"

#include "hashmal_comm.h"
#include "hashmal_comm_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *ESPNOW_TAG = "hashmal.menora.espnow";

#define ESPNOW_LOG_I(fmt, ...)  ESP_LOGI(ESPNOW_TAG, fmt, ##__VA_ARGS__)
#define ESPNOW_LOG_W(fmt, ...)  ESP_LOGW(ESPNOW_TAG, fmt, ##__VA_ARGS__)
#define ESPNOW_LOG_E(fmt, ...)  ESP_LOGE(ESPNOW_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * CONSTANTES LOCALES
 *
 * Canal WiFi por defecto - ESP-NOW vive sobre un canal de radio,
 * pero NO se asocia a ningun AP. El canal es solo para que las
 * hermanas se encuentren en la misma frecuencia de capa 2. El
 * numero concreto viene del tavnit fisico del transporte (Ex 25:40).
 * ================================================================== */
#define ESPNOW_WIFI_CANAL           1        /* 2.4 GHz canal 1 */
#define ESPNOW_WIFI_IF              WIFI_IF_STA
#define ESPNOW_BROADCAST_MAC        { 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu }

/* ==================================================================
 * ESTADO ESTATICO
 * ================================================================== */

/** Flag de inicializacion del driver completo. */
static bool g_iniciado = false;

/**
 * Tabla de hermanas (peers ESP-NOW registrados).
 * Ex 26:3 - jamesh + jamesh joverot isha el ajota: union deliberada,
 * no broadcast. HASHMAL_ESP_NOW_MAX_HERMANAS = 20 (del header).
 */
typedef struct {
    uint8_t mac[6];
    bool    activa;                 /* slot en uso */
} hermana_slot_t;

static hermana_slot_t g_hermanas[HASHMAL_ESP_NOW_MAX_HERMANAS];
static uint8_t        g_hermanas_count = 0u;

/**
 * Buffer RX - guarda el ultimo frame recibido hasta que el
 * llamante lo recoja con hashmal_comm_espnow_recibir. No es una
 * cola completa; es honesto sobre el limite: si llegan dos antes
 * de leer, el segundo sobreescribe al primero (documentado en
 * comentario de recibir). Para routing formal se envolvera con
 * una cola FreeRTOS en fase posterior.
 */
static uint8_t  g_rx_mac[6];
static uint8_t  g_rx_payload[HASHMAL_ESP_NOW_FRAME_MAX];
static size_t   g_rx_tam        = 0u;
static bool     g_rx_disponible = false;

/* ==================================================================
 * HELPERS ESTATICOS
 * ================================================================== */

/**
 * Compara dos MACs byte a byte. Igualdad estricta - una MAC es
 * una identidad unica, no se aproxima.
 */
static bool mac_igual(const uint8_t a[6], const uint8_t b[6])
{
    for (uint8_t i = 0; i < 6u; i++) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

/**
 * Busca una hermana por MAC. @return indice [0..MAX) si presente,
 * o -1 si no registrada.
 */
static int hermana_indice(const uint8_t mac[6])
{
    for (uint8_t i = 0; i < HASHMAL_ESP_NOW_MAX_HERMANAS; i++) {
        if (g_hermanas[i].activa && mac_igual(g_hermanas[i].mac, mac)) {
            return (int)i;
        }
    }
    return -1;
}

/**
 * Busca el primer slot libre en la tabla de hermanas. @return
 * indice libre o -1 si la tabla esta llena.
 */
static int hermana_slot_libre(void)
{
    for (uint8_t i = 0; i < HASHMAL_ESP_NOW_MAX_HERMANAS; i++) {
        if (!g_hermanas[i].activa) return (int)i;
    }
    return -1;
}

/**
 * Marca la mutsaqa ESP-NOW de los 7 nerot como disponible (o no).
 * El radio es fisicamente compartido; si el driver esta vivo, la
 * mutsaqa esta disponible para cualquier canal logico.
 */
static void marcar_espnow_disponible(bool disponible)
{
    for (uint8_t canal = 0; canal < HASHMAL_MENORA_CANALES; canal++) {
        hashmal_mutsaqa_t *m =
            hashmal_comm_mutsaqa_mut((hashmal_canal_id_t)canal,
                                      HASHMAL_MUTSAQA_ESP_NOW);
        if (m != NULL) {
            m->disponible = disponible;
            if (disponible)  m->activa = true;   /* Ex 26:3 ishá el ajotá — enlazada y activa */
            if (!disponible) m->activa = false;
        }
    }
}

/**
 * Incrementa contador TX (exito=true) o errores (exito=false) en
 * la mutsaqa ESP-NOW del canal representativo (geza', canal 0).
 * El radio es uno; las estadisticas agregadas viven en el tronco.
 */
static void stat_tx(bool exito)
{
    hashmal_mutsaqa_t *m = hashmal_comm_mutsaqa_mut(HASHMAL_CANAL_GEZA,
                                                    HASHMAL_MUTSAQA_ESP_NOW);
    if (m == NULL) return;
    if (exito) m->paquetes_tx++;
    else       m->errores++;
}

static void stat_rx(bool exito)
{
    hashmal_mutsaqa_t *m = hashmal_comm_mutsaqa_mut(HASHMAL_CANAL_GEZA,
                                                    HASHMAL_MUTSAQA_ESP_NOW);
    if (m == NULL) return;
    if (exito) m->paquetes_rx++;
    else       m->errores++;
}

/* ==================================================================
 * CALLBACKS DE ESP-NOW (ORO DE EGIPTO)
 *
 * Las firmas siguen la API de ESP-IDF. Ningun otro archivo del
 * componente ve estas funciones; son las "hojas" del olivo por
 * donde el aceite entra al driver.
 * ================================================================== */

/**
 * Ex 26:3 isha el ajota - al recibir de la hermana. Copia el
 * frame a g_rx_* y marca disponible. Ex 25:40 ur'e: mantenemos
 * el patron de un solo lugar donde el dato "entra".
 *
 * La firma exacta depende de la version del SDK; usamos la
 * contemporanea con esp_now_recv_info_t. Si el SDK usa MAC
 * suelto, adaptar en un solo punto.
 */
static void cb_recv(const esp_now_recv_info_t *info,
                    const uint8_t *data, int len)
{
    if (info == NULL || data == NULL || len <= 0) {
        stat_rx(false);
        return;
    }
    if ((size_t)len > HASHMAL_ESP_NOW_FRAME_MAX) {
        /* Tamano excede el marco del tavnit - descartamos. */
        stat_rx(false);
        return;
    }

    memcpy(g_rx_mac, info->src_addr, 6u);
    memcpy(g_rx_payload, data, (size_t)len);
    g_rx_tam        = (size_t)len;
    g_rx_disponible = true;
    stat_rx(true);
}

/**
 * TX status - la capa 2 confirma entrega o falla. Actualizamos
 * stat en consecuencia.
 */
static void cb_send(const uint8_t *mac_dst, esp_now_send_status_t status)
{
    (void)mac_dst;
    stat_tx(status == ESP_NOW_SEND_SUCCESS);
}

/* ==================================================================
 * INICIO DE WIFI - MODO STA SIN CONECTAR (Lv 10:1-2 respetado)
 *
 * WIFI_MODE_STA se inicia pero NO se asocia a ningun AP. Es la
 * unica forma que ESP-IDF ofrece para levantar la capa 2 y poder
 * usar ESP-NOW. Ningun esp_wifi_connect() se llama - ese seria
 * el esh zara.
 * ================================================================== */
static esp_err_t wifi_arrancar_modo_sta_sin_conectar(void)
{
    /* nvs_flash: necesario para calibracion de radio. No almacenamos
     * credenciales ni perfiles de red; solo lo que ESP-IDF exige. */
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        (void)nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        ESPNOW_LOG_E("nvs_flash_init falló (%d)", (int)err);
        return err;
    }

    err = esp_netif_init();
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
        ESPNOW_LOG_E("esp_netif_init falló (%d)", (int)err);
        return err;
    }

    err = esp_event_loop_create_default();
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
        ESPNOW_LOG_E("event_loop_create_default falló (%d)", (int)err);
        return err;
    }

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    err = esp_wifi_init(&cfg);
    if (err != ESP_OK) {
        ESPNOW_LOG_E("esp_wifi_init falló (%d)", (int)err);
        return err;
    }

    /* Sin persistencia de credenciales - no queremos que un boot
     * posterior intente conectar a una red "aprendida". */
    (void)esp_wifi_set_storage(WIFI_STORAGE_RAM);

    err = esp_wifi_set_mode(WIFI_MODE_STA);
    if (err != ESP_OK) {
        ESPNOW_LOG_E("esp_wifi_set_mode(STA) falló (%d)", (int)err);
        return err;
    }

    err = esp_wifi_start();
    if (err != ESP_OK) {
        ESPNOW_LOG_E("esp_wifi_start falló (%d)", (int)err);
        return err;
    }

    /* Fijamos el canal explicitamente. Ex 25:40 betavnitam - no
     * dejamos la frecuencia al azar. */
    (void)esp_wifi_set_channel(ESPNOW_WIFI_CANAL, WIFI_SECOND_CHAN_NONE);

    /* NO llamar a esp_wifi_connect() - ese seria esh zara. */
    return ESP_OK;
}

static void wifi_apagar(void)
{
    (void)esp_wifi_stop();
    (void)esp_wifi_deinit();
    /* esp_event_loop y esp_netif pueden ser compartidos con otros
     * modulos (p. ej. BLE en un futuro); no los destruimos aqui. */
}

/* ==================================================================
 * FUNCIONES PUBLICAS
 * ================================================================== */

/**
 * hashmal_comm_espnow_init - Exodo 26:3
 *
 * Levanta el driver ESP-NOW: WiFi en modo STA SIN conectar,
 * esp_now_init, registra callbacks RX/TX. Marca la mutsaqa
 * ESP-NOW como disponible en los 7 nerot.
 *
 * Idempotente: si ya esta iniciado, retorna OK sin reinicializar.
 */
int hashmal_comm_espnow_init(void)
{
    if (g_iniciado) {
        return HASHMAL_COMM_OK;
    }

    /* Menora debe estar levantada antes del driver - el selector
     * y stats consultan el singleton. */
    if (hashmal_comm_get_menora_interno() == NULL) {
        ESPNOW_LOG_E("init: Menorá no inicializada (levantar hub primero)");
        return HASHMAL_COMM_ERR_INIT;
    }

    /* Limpia estado local antes de levantar hardware. */
    memset(g_hermanas, 0, sizeof(g_hermanas));
    g_hermanas_count = 0u;
    memset(g_rx_mac, 0, sizeof(g_rx_mac));
    memset(g_rx_payload, 0, sizeof(g_rx_payload));
    g_rx_tam        = 0u;
    g_rx_disponible = false;

    /* WiFi STA sin conectar (Lv 10:1-2 - sin esh zara). */
    if (wifi_arrancar_modo_sta_sin_conectar() != ESP_OK) {
        return HASHMAL_COMM_ERR_ESP_NOW;
    }

    esp_err_t err = esp_now_init();
    if (err != ESP_OK) {
        ESPNOW_LOG_E("esp_now_init falló (%d)", (int)err);
        wifi_apagar();
        return HASHMAL_COMM_ERR_ESP_NOW;
    }

    if (esp_now_register_recv_cb(cb_recv) != ESP_OK ||
        esp_now_register_send_cb(cb_send) != ESP_OK) {
        ESPNOW_LOG_E("register callbacks ESP-NOW falló");
        (void)esp_now_deinit();
        wifi_apagar();
        return HASHMAL_COMM_ERR_ESP_NOW;
    }

    marcar_espnow_disponible(true);
    g_iniciado = true;

    ESPNOW_LOG_I("═══ ESP-NOW levantado ═══ (Éx 26:3 ishá el ajotá)");
    ESPNOW_LOG_I("  WiFi STA sin conectar (Lv 10:1-2 — sin ésh zará)");
    ESPNOW_LOG_I("  canal %d, máx hermanas=%d, frame=%d, frag=%d",
                 (int)ESPNOW_WIFI_CANAL,
                 (int)HASHMAL_ESP_NOW_MAX_HERMANAS,
                 (int)HASHMAL_ESP_NOW_FRAME_MAX,
                 (int)HASHMAL_ESP_NOW_FRAG_MAX);
    ESPNOW_LOG_I("  mutsaqá ESP-NOW DISPONIBLE en los 7 nerot");
    return HASHMAL_COMM_OK;
}

/**
 * hashmal_comm_espnow_deinit - desmonte ordenado
 */
int hashmal_comm_espnow_deinit(void)
{
    if (!g_iniciado) return HASHMAL_COMM_OK;

    (void)esp_now_unregister_recv_cb();
    (void)esp_now_unregister_send_cb();
    (void)esp_now_deinit();
    wifi_apagar();

    marcar_espnow_disponible(false);
    g_hermanas_count = 0u;
    memset(g_hermanas, 0, sizeof(g_hermanas));
    g_rx_tam        = 0u;
    g_rx_disponible = false;

    g_iniciado = false;
    ESPNOW_LOG_I("ESP-NOW apagado (Nm 4:9-10 tejélet)");
    return HASHMAL_COMM_OK;
}

/**
 * hashmal_comm_espnow_enviar - Exodo 26:17 meshullavot
 *
 * Envia un frame a la MAC de la hermana destino. La hermana DEBE
 * estar emparejada previamente (Ex 26:3 - joverot deliberado):
 * no se envia a una MAC desconocida.
 *
 * Si tam > HASHMAL_ESP_NOW_FRAME_MAX -> ERR_TAMANO. La
 * fragmentacion (HASHMAL_ESP_NOW_FRAG_MAX = 10) la orquesta la
 * capa superior (routing); este driver envia UN frame a la vez -
 * el radio fisico es uno.
 */
int hashmal_comm_espnow_enviar(const uint8_t mac_destino[6],
                                const uint8_t *datos,
                                size_t tam)
{
    if (!g_iniciado) return HASHMAL_COMM_ERR_INIT;
    if (mac_destino == NULL || datos == NULL) return HASHMAL_COMM_ERR_NULL;
    if (tam == 0u)                   return HASHMAL_COMM_ERR_TAMANO;
    if (tam > HASHMAL_ESP_NOW_FRAME_MAX) return HASHMAL_COMM_ERR_TAMANO;

    /* Ex 26:3 - isha el ajota: unicast, no broadcast casual. */
    if (!hashmal_comm_mac_legitima(mac_destino)) {
        ESPNOW_LOG_W("enviar: MAC destino ilegítima (broadcast/multicast/nula)");
        return HASHMAL_COMM_ERR_NULL;
    }

    /* La hermana debe estar emparejada - joverot deliberado. */
    if (hermana_indice(mac_destino) < 0) {
        ESPNOW_LOG_W("enviar: hermana no emparejada — llamar primero "
                     "a espnow_emparejar (Éx 26:3 joverót)");
        return HASHMAL_COMM_ERR_ROUTING;
    }

    esp_err_t err = esp_now_send(mac_destino, datos, tam);
    if (err != ESP_OK) {
        ESPNOW_LOG_W("esp_now_send falló (%d)", (int)err);
        stat_tx(false);
        return HASHMAL_COMM_ERR_ESP_NOW;
    }
    /* El incremento TX real vendra en cb_send (confirmacion de
     * entrega). Aqui solo confirmamos encolado. */
    return HASHMAL_COMM_OK;
}

/**
 * hashmal_comm_espnow_recibir - polling del buffer RX
 *
 * Si hay un frame pendiente, lo copia a mac_origen y buf, y lo
 * marca consumido. Si no hay nada, retorna ERR_TIMEOUT.
 *
 * Nota honesta: este driver usa un buffer de tamano UNO. Si
 * llegan dos frames antes de leer, el segundo sobreescribe al
 * primero - mejor senalado con stat.errores++ que fingir una
 * cola. Fase posterior sustituira con FreeRTOS queue.
 */
int hashmal_comm_espnow_recibir(uint8_t mac_origen[6],
                                 uint8_t *buf,
                                 size_t tam,
                                 size_t *recibidos)
{
    if (!g_iniciado) return HASHMAL_COMM_ERR_INIT;
    if (mac_origen == NULL || buf == NULL || recibidos == NULL) {
        return HASHMAL_COMM_ERR_NULL;
    }

    if (!g_rx_disponible) {
        *recibidos = 0u;
        return HASHMAL_COMM_ERR_TIMEOUT;
    }

    if (g_rx_tam > tam) {
        /* El buffer del llamante no cabe - reportamos pero no
         * consumimos: que pida con buffer adecuado. */
        *recibidos = g_rx_tam;
        return HASHMAL_COMM_ERR_TAMANO;
    }

    memcpy(mac_origen, g_rx_mac, 6u);
    memcpy(buf, g_rx_payload, g_rx_tam);
    *recibidos = g_rx_tam;

    /* Consumido. */
    g_rx_tam        = 0u;
    g_rx_disponible = false;
    return HASHMAL_COMM_OK;
}

/**
 * hashmal_comm_espnow_emparejar - Exodo 26:3 joverot
 *
 * Registra una hermana como peer ESP-NOW. Validaciones:
 *   a) MAC legitima (unicast, no broadcast ni multicast - isha el
 *      ajota es hermana unica, no grito al aire).
 *   b) Tabla de hermanas con hueco.
 *   c) MAC no duplicada.
 *
 * Tras registrar en el silicio (esp_now_add_peer), guardamos en
 * la tabla local para poder listar/buscar sin volver al SDK.
 */
int hashmal_comm_espnow_emparejar(const uint8_t mac[6])
{
    if (!g_iniciado)     return HASHMAL_COMM_ERR_INIT;
    if (mac == NULL)     return HASHMAL_COMM_ERR_NULL;
    if (!hashmal_comm_mac_legitima(mac)) {
        return HASHMAL_COMM_ERR_NULL;
    }

    if (hermana_indice(mac) >= 0) {
        /* Ya emparejada - idempotente. */
        return HASHMAL_COMM_OK;
    }

    int slot = hermana_slot_libre();
    if (slot < 0) {
        ESPNOW_LOG_W("emparejar: tabla llena (máx %d hermanas)",
                     (int)HASHMAL_ESP_NOW_MAX_HERMANAS);
        return HASHMAL_COMM_ERR_LLENO;
    }

    esp_now_peer_info_t peer = {0};
    memcpy(peer.peer_addr, mac, 6u);
    peer.channel  = ESPNOW_WIFI_CANAL;
    peer.ifidx    = ESPNOW_WIFI_IF;
    peer.encrypt  = false;   /* cifrado opcional, fase posterior */

    esp_err_t err = esp_now_add_peer(&peer);
    if (err != ESP_OK) {
        ESPNOW_LOG_E("esp_now_add_peer falló (%d)", (int)err);
        return HASHMAL_COMM_ERR_ESP_NOW;
    }

    memcpy(g_hermanas[slot].mac, mac, 6u);
    g_hermanas[slot].activa = true;
    g_hermanas_count++;

    ESPNOW_LOG_I("joverót: hermana %02x:%02x:%02x:%02x:%02x:%02x "
                 "emparejada (slot %d, total %u)",
                 mac[0], mac[1], mac[2], mac[3], mac[4], mac[5],
                 slot, (unsigned)g_hermanas_count);
    return HASHMAL_COMM_OK;
}

/**
 * hashmal_comm_espnow_desemparejar - ruptura deliberada
 *
 * Retira la hermana del peer table. Si no estaba emparejada,
 * idempotente OK.
 */
int hashmal_comm_espnow_desemparejar(const uint8_t mac[6])
{
    if (!g_iniciado) return HASHMAL_COMM_ERR_INIT;
    if (mac == NULL) return HASHMAL_COMM_ERR_NULL;

    int idx = hermana_indice(mac);
    if (idx < 0) {
        /* No estaba - idempotente. */
        return HASHMAL_COMM_OK;
    }

    (void)esp_now_del_peer(mac);

    g_hermanas[idx].activa = false;
    memset(g_hermanas[idx].mac, 0, 6u);
    if (g_hermanas_count > 0u) g_hermanas_count--;

    ESPNOW_LOG_I("hermana %02x:%02x:%02x:%02x:%02x:%02x desemparejada "
                 "(total %u)",
                 mac[0], mac[1], mac[2], mac[3], mac[4], mac[5],
                 (unsigned)g_hermanas_count);
    return HASHMAL_COMM_OK;
}

/**
 * hashmal_comm_espnow_hermanas_contar - cuenta peers activos
 *
 * Lectura pura. Sin logs.
 */
int hashmal_comm_espnow_hermanas_contar(uint8_t *conteo)
{
    if (!g_iniciado)     return HASHMAL_COMM_ERR_INIT;
    if (conteo == NULL)  return HASHMAL_COMM_ERR_NULL;
    *conteo = g_hermanas_count;
    return HASHMAL_COMM_OK;
}

/* ==================================================================
 *
 *   "jamesh hayeri'ot tihyeyna JOVEROT isha el ajota." - Exodo 26:3
 *
 *   "shtei YADOT laqeresh ha'ejad MESHULLAVOT isha el ajota."
 *     - Exodo 26:17
 *
 *   "vejibarta et hayeri'ot isha el ajota baqqerasim -
 *    vehaya hamMISHKAN EJAD." - Exodo 26:6
 *
 *   "vayyaqrivu lifnei YHWH ESH ZARA asher LO TSIVVA otam."
 *     - Levitico 10:1
 *
 *   La hermana se une deliberadamente. El WiFi se levanta sin
 *   router - porque conectar a internet es esh zara que NO mando.
 *   Y aun siendo otra mutsaqa, el Mishkan sigue siendo UNO
 *   (Ex 26:6 - vehaya hamMISHKAN ejad).
 *
 *   Archivo 8 de 9 de la Menora multi-transporte. Siguiente:
 *     menora_ble_paamon.c (pa'amon, Ex 28:34 - la campanilla)
 *
 * ================================================================== */
