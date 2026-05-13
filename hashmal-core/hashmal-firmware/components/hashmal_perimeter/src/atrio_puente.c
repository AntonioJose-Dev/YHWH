/**
 * ===================================================================
 * PIEZA 07 — EL ATRIO — atrio_puente.c
 * Añadidura 22: SHA'AR HA'ATRIO — Puente WiFi↔LoRa
 * Éxodo 27:16 + Zacarías 4:12 + Números 10:2 + Juan 10:9
 * ===================================================================
 *
 * "uleSHA'AR HEJATSER MASAKH 'ESRIM AMMAH" — Éxodo 27:16
 * La puerta es el PUNTO DE CRUCE: lo que entra por WiFi sale por
 * LoRa, y lo que llega por LoRa sale por WiFi al teléfono.
 *
 * "shenei shibbolei hazzahav HAMMERIQIM me'alehem et hazzahav"
 *   — Zacarías 4:12
 * "Las dos espigas de oro QUE VIERTEN de sí mismas el aceite dorado."
 * Dos canales, dos flujos, una sola puerta. El puente NO mezcla:
 * cada flujo mantiene su dirección (WiFi→LoRa, LoRa→WiFi).
 *
 * "hazlót... vetaqeʻú vetiqeʻú kol ha'edah" — Números 10:2
 * Las trompetas de plata sirven para CONGREGAR y para MOVERSE.
 * Aquí: el puente congrega mensajes LoRa (ring buffer) y los
 * emite hacia los teléfonos cuando se congregan en el portal.
 *
 * --- ARQUITECTURA DEL PUENTE ---
 *
 *  LoRa RX                WiFi (teléfono)
 *  ────────                ───────────────
 *  listen_task             HTTP GET /api/mensajes
 *      │                        ↑
 *      ↓                        │
 *  puente_rx_push()  ──→  g_rx_buf[] (ring)
 *
 *  HTTP POST /api/mensajes
 *      │
 *      ↓
 *  puente_tx_send()  ──→  hashmal_comm_enviar() ──→ LoRa TX
 *
 * --- RING BUFFER ---
 *
 *  g_rx_buf[PUENTE_RX_CAP]: circular, FIFO, mutex-protegido.
 *  Cada entrada = {timestamp_ms, origen, tam, datos[]}.
 *  Cuando lleno: sobrescribe el más antiguo (Lv 19:9 leket —
 *  las espigas caídas son para el siguiente que pase).
 *
 * 2 testigos:
 *   Zac 4:12 — hammeriqim: dos flujos VIERTEN (bidireccional)
 *   Nm  10:2 — trompetas: congregar (RX buffer) y mover (TX send)
 *
 * Autor: Antonio José Marín Soto — Hashmal-Core
 */

#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "esp_timer.h"

#include "hashmal_perimeter.h"
#include "hashmal_comm.h"

/* ===================================================================
 * ENCAPSULACIÓN ESP-IDF
 * =================================================================== */
static const char *PUENTE_TAG = "hashmal.perimeter.puente";
#define PUENTE_LOG_I(fmt, ...)  ESP_LOGI(PUENTE_TAG, fmt, ##__VA_ARGS__)
#define PUENTE_LOG_W(fmt, ...)  ESP_LOGW(PUENTE_TAG, fmt, ##__VA_ARGS__)
#define PUENTE_LOG_E(fmt, ...)  ESP_LOGE(PUENTE_TAG, fmt, ##__VA_ARGS__)

/* ===================================================================
 * CONSTANTES
 * =================================================================== */

/* Capacidad del ring buffer — 32 mensajes máximo en RAM.
 * Lv 19:9 leket: las espigas olvidadas quedan para el que pasa.
 * Cuando lleno, el mensaje más antiguo da paso al nuevo. */
#define PUENTE_RX_CAP      32u

/* Tamaño máximo de payload en cada mensaje del buffer */
#define PUENTE_MSG_MAX     HASHMAL_MENSAJE_MAX_BYTES

/* ===================================================================
 * ESTRUCTURAS INTERNAS
 * =================================================================== */

/** Entrada del ring buffer RX */
typedef struct {
    int64_t   timestamp_ms;              /* esp_timer_get_time() / 1000 */
    uint8_t   origen;                    /* nodo origen (0-6 canal, 0xFF broadcast) */
    uint8_t   tipo;                      /* tipo HASHMAL (0x01=msg, 0x02=beacon...) */
    uint8_t   datos[PUENTE_MSG_MAX];     /* payload desempaquetado */
    size_t    tam;                       /* bytes válidos en datos[] */
    bool      valida;                    /* slot ocupado */
} puente_msg_t;

/* ===================================================================
 * ESTADO SINGLETON
 * =================================================================== */

static puente_msg_t      g_rx_buf[PUENTE_RX_CAP];
static uint32_t          g_rx_head  = 0u;   /* próximo slot a escribir */
static uint32_t          g_rx_total = 0u;   /* mensajes recibidos (acumulado) */
static uint32_t          g_tx_total = 0u;   /* mensajes enviados (acumulado) */
static SemaphoreHandle_t g_rx_mutex = NULL;
static bool              g_init     = false;

/* ===================================================================
 * INICIALIZACIÓN
 * =================================================================== */

/**
 * hashmal_perimeter_puente_init — crea el mutex y limpia el buffer.
 *
 * "uleSHA'AR HEJATSER MASAKH 'ESRIM AMMAH" — Éx 27:16.
 * Antes de que alguien cruce la puerta, la cortina debe estar puesta.
 * Esta función pone la cortina: el buffer está listo para recibir.
 *
 * 2 testigos:
 *   Éx 40:33 — vayyitén et masakh sha'ar hejatser: poner la cortina
 *   Nm 10:2  — hazlót keseph: preparar las trompetas primero
 */
int hashmal_perimeter_puente_init(void)
{
    if (g_init) {
        PUENTE_LOG_W("puente_init: ya inicializado (idempotente)");
        return 0;
    }

    memset(g_rx_buf, 0, sizeof(g_rx_buf));
    g_rx_head  = 0u;
    g_rx_total = 0u;
    g_tx_total = 0u;

    g_rx_mutex = xSemaphoreCreateMutex();
    if (g_rx_mutex == NULL) {
        PUENTE_LOG_E("puente_init: no se pudo crear mutex — sin puente");
        return -1;
    }

    g_init = true;
    PUENTE_LOG_I("═══ puente WiFi↔LoRa listo (Zac 4:12 hammeriqim) ═══");
    PUENTE_LOG_I("  ring buffer: %u slots × %u bytes", PUENTE_RX_CAP, PUENTE_MSG_MAX);
    return 0;
}

/* ===================================================================
 * API RX: LoRa → buffer → WiFi
 * =================================================================== */

/**
 * hashmal_perimeter_puente_rx_push — llamado por listen_task al recibir.
 *
 * "el mul penei hammenora ya'iru shiv'at hannerot" — Nm 8:2.
 * La luz que ENTRA por las ramas (LoRa) se deposita en el tronco
 * central (este buffer), disponible para el portal WiFi.
 *
 * Thread-safe: protegido por mutex.
 *
 * 2 testigos:
 *   Nm 8:2  — las 7 lámparas iluminan hacia el centro (depósito central)
 *   Lv 19:9 — leket: las espigas caídas quedan para quien pase
 */
int hashmal_perimeter_puente_rx_push(uint8_t  tipo,
                                      uint8_t  origen,
                                      const uint8_t *datos,
                                      size_t   tam)
{
    if (!g_init || datos == NULL || tam == 0u) return -1;
    if (tam > PUENTE_MSG_MAX) tam = PUENTE_MSG_MAX;

    if (xSemaphoreTake(g_rx_mutex, pdMS_TO_TICKS(20)) != pdTRUE) {
        PUENTE_LOG_W("rx_push: mutex timeout — mensaje descartado");
        return -1;
    }

    puente_msg_t *slot = &g_rx_buf[g_rx_head % PUENTE_RX_CAP];
    slot->timestamp_ms = esp_timer_get_time() / 1000LL;
    slot->origen       = origen;
    slot->tipo         = tipo;
    slot->tam          = tam;
    slot->valida       = true;
    memcpy(slot->datos, datos, tam);

    g_rx_head = (g_rx_head + 1u) % PUENTE_RX_CAP;
    ++g_rx_total;

    xSemaphoreGive(g_rx_mutex);

    PUENTE_LOG_I("rx_push: tipo=0x%02X orig=%u tam=%u total=%" PRIu32
                 " (Nm 8:2 luz→tronco)",
                 (unsigned)tipo, (unsigned)origen,
                 (unsigned)tam, g_rx_total);
    return 0;
}

/**
 * hashmal_perimeter_puente_rx_json — serializa buffer como JSON.
 *
 * Retorna el número de bytes escritos en out_buf (sin '\0').
 * El JSON tiene forma: {"msgs":[{"t":1234,"o":0,"d":"hola"},...]}
 *
 * "kol ha'edah" (toda la congregación) ve los mensajes juntos.
 *
 * 2 testigos:
 *   Éx 25:30 — lechem hapaním SIEMPRE delante: disponibilidad total
 *   Sal 100:4 — entrad por sus puertas: la información espera al que llega
 */
int hashmal_perimeter_puente_rx_json(char *out_buf, size_t cap)
{
    if (!g_init || out_buf == NULL || cap < 16u) return -1;

    if (xSemaphoreTake(g_rx_mutex, pdMS_TO_TICKS(50)) != pdTRUE) {
        /* Si no podemos adquirir el mutex, devolvemos array vacío */
        int n = snprintf(out_buf, cap, "{\"msgs\":[]}");
        return (n > 0) ? n : -1;
    }

    int written = 0;
    written += snprintf(out_buf + written, cap - (size_t)written,
                        "{\"msgs\":[");

    bool first = true;
    /* Recorremos en orden cronológico (de más antiguo a más nuevo).
     * Filtramos beacons (tipo 0x02) del tab Mensajes — son útiles
     * para el mapa de red pero no para el chat.
     * "Escucha a Israel" — Dt 6:4: distinguir la VOZ entre el ruido. */
    for (uint32_t i = 0u; i < PUENTE_RX_CAP; ++i) {
        const puente_msg_t *m = &g_rx_buf[i];
        if (!m->valida) continue;
        /* Beacon (tipo 0x02) — silenciar en el chat */
        if (m->tipo == 0x02u) continue;

        /* Convertir datos a hex string para JSON */
        char hex[PUENTE_MSG_MAX * 2u + 1u];
        for (size_t b = 0u; b < m->tam && b < PUENTE_MSG_MAX; ++b) {
            snprintf(&hex[b * 2], 3, "%02X", (unsigned)m->datos[b]);
        }
        hex[m->tam * 2u] = '\0';

        /* Intentar interpretar como texto imprimible.
         * texto debe tener espacio para el peor caso: "[hex]"
         * donde hex tiene PUENTE_MSG_MAX*2 bytes + 2 corchetes + '\0'.
         * Lv 19:9 leket — no desperdiciar ni un byte: tamaño exacto. */
        char texto[PUENTE_MSG_MAX * 2u + 4u];   /* "[XX...XX]\0" — peor caso */
        /* es_texto: acepta ASCII imprimible + UTF-8 multi-byte (0x80-0xFF).
         * Solo rechaza control chars < 0x20 (Dt 6:4 — distinguir la VOZ).
         * Antes rechazaba > 0x7E, bloqueando tildes (é, ó, á, ú, ñ, etc.). */
        bool es_texto = true;
        for (size_t b = 0u; b < m->tam; ++b) {
            if (m->datos[b] < 0x20u) { es_texto = false; break; }
        }
        if (es_texto) {
            /* Copiar con escape JSON de " y \ — bytes UTF-8 pasan tal cual */
            size_t out = 0u;
            for (size_t b = 0u; b < m->tam && out < sizeof(texto) - 3u; ++b) {
                uint8_t c = m->datos[b];
                if (c == '"' || c == '\\') { texto[out++] = '\\'; }
                texto[out++] = (char)c;
            }
            texto[out] = '\0';
        } else {
            snprintf(texto, sizeof(texto), "[%s]", hex);
        }

        int needed = snprintf(out_buf + written,
                              cap - (size_t)written,
                              "%s{\"t\":%" PRId64 ",\"o\":%u,"
                              "\"tipo\":%u,\"d\":\"%s\"}",
                              first ? "" : ",",
                              m->timestamp_ms,
                              (unsigned)m->origen,
                              (unsigned)m->tipo,
                              texto);
        if (needed > 0 && (written + needed) < (int)cap - 2) {
            written += needed;
            first = false;
        } else {
            break; /* buffer lleno */
        }
    }

    int tail = snprintf(out_buf + written, cap - (size_t)written,
                        "],\"rx\":%" PRIu32 ",\"tx\":%" PRIu32 "}",
                        g_rx_total, g_tx_total);
    if (tail > 0) written += tail;

    xSemaphoreGive(g_rx_mutex);
    return written;
}

/* ===================================================================
 * API TX: WiFi → LoRa
 * =================================================================== */

/**
 * hashmal_perimeter_puente_tx_send — envía mensaje desde portal→LoRa.
 *
 * "veheír 'al 'éver panéiha" — Éx 25:37.
 * La luz sale HACIA AFUERA por las ramas. Lo que el teléfono escribe
 * sale por LoRa como si lo hubiera escrito el nodo.
 *
 * 2 testigos:
 *   Éx 25:37 — veheír: iluminar hacia el exterior (TX hacia la red)
 *   Zac 4:12 — hammeriqim: los tubos vierten hacia afuera
 */
int hashmal_perimeter_puente_tx_send(const uint8_t *datos, size_t tam)
{
    if (!g_init || datos == NULL || tam == 0u) return -1;

    /* Empaquetar como mensaje HASHMAL tipo 0x01 (mensaje directo) */
    uint8_t frame[HASHMAL_MENSAJE_MAX_BYTES];
    /* Origen = canal GEZA (0), destino = broadcast 0xFF */
    int rc = hashmal_comm_formato_empaquetar(
                 0x01u,                       /* tipo: mensaje */
                 HASHMAL_CANAL_GEZA,          /* origen: tronco central */
                 0xFFu,                       /* destino: toda la red */
                 datos, tam,
                 frame, sizeof(frame));
    if (rc <= 0) {
        PUENTE_LOG_E("tx_send: empaquetar falló (rc=%d)", rc);
        return -1;
    }
    size_t frame_tam = (size_t)rc;

    rc = hashmal_comm_enviar(HASHMAL_CANAL_GEZA, frame, frame_tam);
    if (rc != HASHMAL_COMM_OK) {
        PUENTE_LOG_E("tx_send: enviar falló (rc=%d)", rc);
        return -1;
    }

    ++g_tx_total;
    PUENTE_LOG_I("tx_send: %u bytes → LoRa (total=%" PRIu32
                 ") — veheír Éx 25:37",
                 (unsigned)tam, g_tx_total);

    /* Echo local: guardar el mensaje enviado en el ring buffer con
     * tipo 0xF0 (TX echo) para que el portal lo muestre como "enviado".
     *
     * "veheír 'al 'éver panéiha" — Éx 25:37: la Menorá ilumina HACIA
     * el frente DE LA LÁMPARA — la luz ilumina también al que envió.
     * El teléfono ve confirmación de que su mensaje salió.
     *
     * 2 testigos:
     *   Éx 25:37 — veheír: iluminar el origen también (feedback)
     *   Sal 19:5 — vekol ha'árets yatsá: la voz salió → confirmación */
    hashmal_perimeter_puente_rx_push(0xF0u,              /* tipo TX echo */
                                      HASHMAL_CANAL_GEZA, /* origen: este nodo */
                                      datos, tam);
    return 0;
}

/**
 * hashmal_perimeter_puente_estado_json — JSON de estado de la red.
 *
 * Útil para la barra de estado del portal: nodos, aceite, señal.
 *
 * 2 testigos:
 *   Zac 4:10 — shiv'ah 'einei YHWH: los 7 ojos recorren la tierra
 *   Éx 39:43 — vaYáre Moshé: el constructor VE el estado completo
 */
int hashmal_perimeter_puente_estado_json(char *out_buf, size_t cap)
{
    if (out_buf == NULL || cap < 32u) return -1;

    /* Obtener nivel de aceite (energía) del singleton de power */
    const char *nivel_str = "dai";    /* Fase 2: placeholder */
    uint8_t     aceite    = 50u;      /* Fase 2: placeholder */

    int n = snprintf(out_buf, cap,
        "{\"rx\":%" PRIu32 ",\"tx\":%" PRIu32
        ",\"aceite\":%u,\"nivel\":\"%s\""
        ",\"shaar\":\"abierto\",\"tamid\":true}",
        g_rx_total, g_tx_total, (unsigned)aceite, nivel_str);
    return (n > 0) ? n : -1;
}
