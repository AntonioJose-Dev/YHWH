/**
 * ===================================================================
 * PIEZA 07 — EL ATRIO — atrio_wifi_ap.c
 * Añadidura 22: SHA'AR HA'ATRIO — Punto de Acceso WiFi
 * Éxodo 27:16 + Isaías 60:11 + Génesis 18:1-5 (Añadidura 21)
 * ===================================================================
 *
 * "ufittjau SHE'ARAYIKH TAMID yomam valaylah lo yissageru"
 *   — Isaías 60:11
 * "Tus puertas estarán ABIERTAS CONTINUAMENTE, día y noche no
 *  se cerrarán."
 *
 * El T-Deck crea una red WiFi local llamada "HASHMAL".
 * No tiene internet — es una red CERRADA, soberana.
 * La contraseña es la barrera (columna 2 del sha'ar).
 * Sin contraseña: nadie entra. Con contraseña: el portal se abre.
 *
 * "veyyarA elav YHWH be'elon Mamre vehu yoshev petaj ha'OHEL
 *  kejom hayyom." — Génesis 18:1
 * "YHWH se le apareció junto a los terebintos de Mamre,
 *  mientras él estaba sentado a la PUERTA (petaj) de la tienda."
 * Abraham vio y CORRIÓ al encuentro. La puerta estaba ABIERTA.
 * La hospitalidad (Añadidura 21) empieza por la puerta abierta.
 *
 * --- DISEÑO ---
 *
 * Modo: WiFi AP (Access Point puro).
 *   SSID:     "HASHMAL"         (configurable via sdkconfig)
 *   Password: "yisrael1"        (mínimo 8 chars WPA2)
 *   Canal RF: 6                 (ISM 2.4GHz, menos saturado)
 *   Max STA:  4                 (Éx 27:16 — 4 columnas del sha'ar)
 *   IP AP:    192.168.4.1       (gateway y servidor DNS)
 *
 * Al conectarse un teléfono → WIFI_EVENT_AP_STACONNECTED →
 *   hashmal_perimeter_shaar_conectar() registra la sesión.
 * Al desconectarse → WIFI_EVENT_AP_STADISCONNECTED →
 *   hashmal_perimeter_shaar_desconectar() cierra la sesión.
 *
 * 2 testigos:
 *   Is 60:11 — sha'arayikh tamid: puertas abiertas continuamente
 *   Gn 18:1  — petaj ha'ohel: sentado a la puerta, listo para recibir
 *
 * Autor: Antonio José Marín Soto — Hashmal-Core
 */

#include <string.h>
#include <stdbool.h>

#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "nvs_flash.h"

#include "hashmal_perimeter.h"

/* ===================================================================
 * ENCAPSULACIÓN ESP-IDF
 * =================================================================== */
static const char *WIFIAP_TAG = "hashmal.perimeter.wifi_ap";
#define WIFIAP_LOG_I(fmt, ...)  ESP_LOGI(WIFIAP_TAG, fmt, ##__VA_ARGS__)
#define WIFIAP_LOG_W(fmt, ...)  ESP_LOGW(WIFIAP_TAG, fmt, ##__VA_ARGS__)
#define WIFIAP_LOG_E(fmt, ...)  ESP_LOGE(WIFIAP_TAG, fmt, ##__VA_ARGS__)

/* ===================================================================
 * CONFIGURACIÓN — 4 columnas del sha'ar (Éx 27:16)
 * =================================================================== */

#define SHAAR_WIFI_SSID        "HASHMAL"     /* Nombre de la red */
#define SHAAR_WIFI_PASS        "yisrael1"    /* Contraseña WPA2 */
#define SHAAR_WIFI_CANAL_RF    6             /* Canal 2.4GHz */
#define SHAAR_WIFI_MAX_CONN    4u            /* 4 columnas = 4 conexiones */

/* ===================================================================
 * ESTADO INTERNO
 * =================================================================== */
static bool              g_wifi_init = false;
static esp_netif_t      *g_netif_ap  = NULL;

/* ===================================================================
 * HANDLER DE EVENTOS
 * =================================================================== */

/**
 * Manejador de eventos WiFi del AP.
 *
 * "vetaqeʻú vetiqeʻú kol ha'edah 'el petaj 'ohel mo'ed" — Nm 10:3.
 * "Tocaréis y SE CONGREGARÁ toda la congregación a la puerta."
 * Al conectarse un cliente: se abre una sesión de sha'ar.
 * Al desconectarse: la sesión se cierra y el slot queda libre.
 *
 * 2 testigos:
 *   Nm 10:3 — congregar a la puerta: el evento WiFi = la trompeta
 *   Éx 27:16 — ammudeihem arba'ah: máximo 4 conexiones simultáneas
 */
static void wifi_ap_event_handler(void         *arg,
                                   esp_event_base_t event_base,
                                   int32_t          event_id,
                                   void            *event_data)
{
    if (event_base != WIFI_EVENT) return;

    if (event_id == WIFI_EVENT_AP_STACONNECTED) {
        wifi_event_ap_staconnected_t *ev =
            (wifi_event_ap_staconnected_t *)event_data;
        WIFIAP_LOG_I("cliente conectado: MAC=%02X:%02X:%02X:%02X:%02X:%02X "
                     "aid=%d — Nm 10:3 congregación",
                     ev->mac[0], ev->mac[1], ev->mac[2],
                     ev->mac[3], ev->mac[4], ev->mac[5],
                     (int)ev->aid);

        /* Abrir sesión en el sha'ar — columna 1 (identidad MAC) */
        uint8_t slot = 0u;
        int rc = hashmal_perimeter_shaar_conectar(&slot);
        if (rc == 0) {
            WIFIAP_LOG_I("sha'ar: slot %u asignado (Éx 27:16 sha'ar abierto)",
                         (unsigned)slot);
        } else {
            WIFIAP_LOG_W("sha'ar: no hay slot libre (rc=%d) — atrio lleno", rc);
        }

    } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
        wifi_event_ap_stadisconnected_t *ev =
            (wifi_event_ap_stadisconnected_t *)event_data;
        WIFIAP_LOG_I("cliente desconectado: MAC=%02X:%02X:%02X:%02X:%02X:%02X "
                     "reason=%d — salió del atrio",
                     ev->mac[0], ev->mac[1], ev->mac[2],
                     ev->mac[3], ev->mac[4], ev->mac[5],
                     (int)ev->reason);
        /* En Fase 2 simplificada: el slot se libera al timeout.
         * Fase 3: rastrear por MAC → slot para desconexión exacta. */

    } else if (event_id == WIFI_EVENT_AP_START) {
        WIFIAP_LOG_I("AP arrancado — Is 60:11 sha'arayikh tamid");

    } else if (event_id == WIFI_EVENT_AP_STOP) {
        WIFIAP_LOG_W("AP detenido — la puerta se cerró (Nm 4:9 beged tejelet)");
    }
}

/* ===================================================================
 * API PÚBLICA
 * =================================================================== */

/**
 * hashmal_perimeter_wifi_ap_init — levanta el punto de acceso WiFi.
 *
 * Secuencia:
 *   1) esp_netif_init (si no fue inicializado por otro componente)
 *   2) esp_event_loop_create_default (idempotente)
 *   3) esp_netif_create_default_wifi_ap
 *   4) esp_wifi_init + esp_wifi_set_mode(AP)
 *   5) Configurar SSID / pass / canal / max_conn
 *   6) esp_wifi_start
 *
 * "vayyásem et masakh sha'ar hejatsér" — Éxodo 40:33.
 * Moisés PUSO la cortina de la puerta. Aquí: el AP es la cortina.
 *
 * 2 testigos:
 *   Éx 40:33 — poner la cortina del atrio: el AP crea la "cortina"
 *   Is 60:11 — sha'arayikh tamid: las puertas siempre abiertas
 */
int hashmal_perimeter_wifi_ap_init(void)
{
    if (g_wifi_init) {
        WIFIAP_LOG_W("wifi_ap_init: ya inicializado (idempotente)");
        return 0;
    }

    /* ── Paso 1: netif e event loop (idempotentes) ──────────────────
     * hashmal_comm ya pudo haberlos inicializado para ESP-NOW.
     * ESP_ERR_INVALID_STATE = ya inicializado → OK, continuamos. */
    esp_err_t ret = esp_netif_init();
    if (ret != ESP_OK && ret != ESP_ERR_INVALID_STATE) {
        WIFIAP_LOG_W("esp_netif_init rc=0x%x", (unsigned)ret);
    }
    ret = esp_event_loop_create_default();
    if (ret != ESP_OK && ret != ESP_ERR_INVALID_STATE) {
        WIFIAP_LOG_W("event_loop_create rc=0x%x", (unsigned)ret);
    }

    /* ── Paso 2: crear interfaz AP ──────────────────────────────────
     * esp_netif_create_default_wifi_ap() crea UNA nueva instancia AP.
     * Si ya existe (llamada doble), devuelve NULL → guardia g_wifi_init
     * lo previene. */
    g_netif_ap = esp_netif_create_default_wifi_ap();
    if (g_netif_ap == NULL) {
        WIFIAP_LOG_E("wifi_ap_init: no se pudo crear netif AP");
        return -1;
    }

    /* ── Paso 3: NVS — cimiento antes del WiFi driver ──────────────
     *
     * El driver WiFi de ESP-IDF requiere NVS inicializado para guardar
     * calibración de RF y configuración. Si NVS no está listo, WiFi
     * devuelve ESP_ERR_NVS_NOT_INITIALIZED (0x1101).
     *
     * hashmal_comm inicializa NVS antes que nosotros, pero si wifi_ap_init
     * se llama ANTES de hashmal_comm (o standalone), necesitamos hacerlo
     * nosotros mismos — idempotente: si ya está init, regresa ESP_OK o
     * ESP_ERR_NVS_NO_FREE_PAGES (en cuyo caso borramos y reiniciamos).
     *
     * "הוּסְדָה אֶרֶץ" — Is 48:13: "Mi mano fundó la tierra" (yasad).
     * NVS es el cimiento (yasad) sobre el que reposa el WiFi.
     *
     * 2 testigos:
     *   Is 48:13 — yasad (H3245): fundar primero el cimiento
     *   Éx 26:19 — yadót hakkesef: los pies de plata (NVS = base) */
    {
        esp_err_t nvs_ret = nvs_flash_init();
        if (nvs_ret == ESP_ERR_NVS_NO_FREE_PAGES ||
            nvs_ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
            WIFIAP_LOG_W("NVS: borrar y reinit (rc=0x%x)", (unsigned)nvs_ret);
            nvs_flash_erase();
            nvs_ret = nvs_flash_init();
        }
        if (nvs_ret == ESP_OK) {
            WIFIAP_LOG_I("NVS: cimiento listo (Is 48:13 yasad)");
        } else {
            /* Ya estaba inicializado por hashmal_comm — OK, continuamos */
            WIFIAP_LOG_I("NVS: ya init (rc=0x%x) — continuando", (unsigned)nvs_ret);
        }
    }

    /* ── Paso 4: esp_wifi_init — UNA sola vez en todo el sistema ────
     *
     * "kullá miqshá ajat" — Éx 25:36: toda la Menorá ES UNA SOLA pieza.
     * hashmal_comm inicializa WiFi para ESP-NOW ANTES que nosotros.
     * esp_wifi_init() devuelve ESP_ERR_INVALID_STATE si ya fue llamado.
     * En ese caso NO es un error — WiFi ya está listo, solo agregamos AP.
     *
     * 2 testigos:
     *   Éx 25:36 — miqshá ajat: una sola pieza (un solo esp_wifi_init)
     *   Nm 9:14  — juqqá ajat: una sola ley para todos */
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ret = esp_wifi_init(&cfg);
    bool wifi_ya_init = (ret == ESP_ERR_INVALID_STATE);
    if (ret != ESP_OK && !wifi_ya_init) {
        WIFIAP_LOG_E("esp_wifi_init: 0x%x — error real", (unsigned)ret);
        return -1;
    }
    if (wifi_ya_init) {
        WIFIAP_LOG_I("wifi_ap_init: WiFi ya init por hashmal_comm — "
                     "agregando AP (miqshá ajat, Éx 25:36)");
    }

    /* ── Paso 5: registrar manejador de eventos ─────────────────────
     * Siempre registrar aunque WiFi ya esté init — el evento AP
     * STACONNECTED lo necesita para abrir sesiones de sha'ar. */
    ret = esp_event_handler_instance_register(
              WIFI_EVENT, ESP_EVENT_ANY_ID,
              &wifi_ap_event_handler, NULL, NULL);
    if (ret != ESP_OK) {
        WIFIAP_LOG_W("event_handler_register rc=0x%x", (unsigned)ret);
    }

    /* ── Paso 6: modo APSTA si WiFi ya corre en STA, AP si no ───────
     *
     * hashmal_comm usa ESP-NOW que puede requerir modo STA internamente.
     * Si ya está en STA → subir a APSTA (Éx 26:3 ishá el ajotá: unión).
     * Si está en NULL/AP → poner AP.
     *
     * Parar WiFi → cambiar modo → reconfigurar → reiniciar.
     *
     * 2 testigos:
     *   Éx 26:3 — ishá el ajotá: unión de dos (STA+AP = APSTA)
     *   Is 60:11 — sha'arayikh tamid: el AP debe quedar activo */
    wifi_mode_t modo_actual = WIFI_MODE_NULL;
    esp_wifi_get_mode(&modo_actual);
    WIFIAP_LOG_I("wifi_ap_init: modo actual = %d", (int)modo_actual);

    wifi_mode_t modo_nuevo;
    if (modo_actual == WIFI_MODE_STA || modo_actual == WIFI_MODE_APSTA) {
        modo_nuevo = WIFI_MODE_APSTA;   /* Éx 26:3 — unir STA + AP */
        WIFIAP_LOG_I("wifi_ap_init: STA detectado → modo APSTA (Éx 26:3 ishá el ajotá)");
    } else {
        modo_nuevo = WIFI_MODE_AP;      /* Is 60:11 — AP puro */
    }

    /* Detener WiFi, cambiar modo, reconfigurar, reiniciar */
    esp_wifi_stop();   /* idempotente si no corre */

    ret = esp_wifi_set_mode(modo_nuevo);
    if (ret != ESP_OK) {
        WIFIAP_LOG_E("set_mode %d: 0x%x", (int)modo_nuevo, (unsigned)ret);
        return -1;
    }

    /* ── Paso 7: configurar el AP ───────────────────────────────────
     * "ammudeihem arba'ah" — Éx 27:16: 4 columnas = 4 conexiones.
     * "ma'aseh roqem" — Éx 38:18: WPA2 (obra de bordador, no abierto).
     * Beacon 100ms = tamíd. */
    wifi_config_t wifi_cfg = { 0 };
    wifi_cfg.ap.ssid_len        = (uint8_t)sizeof(SHAAR_WIFI_SSID) - 1u;
    wifi_cfg.ap.channel         = SHAAR_WIFI_CANAL_RF;
    wifi_cfg.ap.max_connection  = SHAAR_WIFI_MAX_CONN;
    wifi_cfg.ap.authmode        = WIFI_AUTH_WPA2_PSK;
    wifi_cfg.ap.pmf_cfg.required = false;
    wifi_cfg.ap.beacon_interval = 100u;
    memcpy(wifi_cfg.ap.ssid,     SHAAR_WIFI_SSID, sizeof(SHAAR_WIFI_SSID));
    memcpy(wifi_cfg.ap.password, SHAAR_WIFI_PASS, sizeof(SHAAR_WIFI_PASS));

    ret = esp_wifi_set_config(WIFI_IF_AP, &wifi_cfg);
    if (ret != ESP_OK) {
        WIFIAP_LOG_E("set_config AP: 0x%x", (unsigned)ret);
        return -1;
    }

    /* ── Paso 8: arrancar WiFi — Is 60:11 sha'arayikh tamid ─────── */
    ret = esp_wifi_start();
    if (ret != ESP_OK) {
        WIFIAP_LOG_E("esp_wifi_start: 0x%x", (unsigned)ret);
        return -1;
    }

    g_wifi_init = true;
    WIFIAP_LOG_I("═══ SHA'AR WIFI LEVANTADO ═══ (Is 60:11 sha'arayikh tamid)");
    WIFIAP_LOG_I("  SSID:     \"%s\"", SHAAR_WIFI_SSID);
    WIFIAP_LOG_I("  Modo:     %s", (modo_nuevo == WIFI_MODE_APSTA) ? "APSTA" : "AP");
    WIFIAP_LOG_I("  Canal RF: %d (2.4GHz)", SHAAR_WIFI_CANAL_RF);
    WIFIAP_LOG_I("  Max STA:  %u (Éx 27:16 ammudeihem arba'ah)", SHAAR_WIFI_MAX_CONN);
    WIFIAP_LOG_I("  IP AP:    192.168.4.1 — aquí el portal responde");
    WIFIAP_LOG_I("  Auth:     WPA2 — ma'aseh roqem (Éx 38:18)");
    return 0;
}

/**
 * hashmal_perimeter_wifi_ap_deinit — apaga el AP (Nm 4:9 beged tejelet).
 *
 * Solo para desmontaje ordenado. En operación normal el AP corre
 * tamíd — Is 60:11 "lo yissageru" (no se cerrarán).
 */
int hashmal_perimeter_wifi_ap_deinit(void)
{
    if (!g_wifi_init) return 0;
    esp_wifi_stop();
    esp_wifi_deinit();
    g_wifi_init = false;
    WIFIAP_LOG_I("AP detenido — Nm 4:9 beged tejelet (desmontaje)");
    return 0;
}
