/**
 * ===================================================================
 * PIEZA 07 — EL ATRIO — atrio_dns_cautivo.c
 * Añadidura 22: SHA'AR HA'ATRIO — DNS Cautivo (redirige al portal)
 * Éxodo 27:16 + Juan 10:9 + Números 9:17
 * ===================================================================
 *
 * "ego eimi he THYRA; di' emou EAN TIS EISELTHE SOTHESETAI"
 *   — Juan 10:9
 * "Yo soy la PUERTA; el que por Mí ENTRARE, será salvo."
 * UNA sola puerta. El DNS cautivo asegura que CUALQUIER petición
 * de internet que haga el teléfono, llegue a esta puerta.
 *
 * --- CÓMO FUNCIONA EL PORTAL CAUTIVO ---
 *
 * Cuando el teléfono se conecta al WiFi "HASHMAL" (sin internet),
 * el sistema operativo detecta que no hay internet y automáticamente
 * pregunta: "¿Hay un portal cautivo?" Para saberlo, hace peticiones
 * DNS a dominios conocidos (google.com, apple.com, msn.com, etc.)
 *
 * Este servidor DNS responde a TODAS las consultas con 192.168.4.1
 * (la IP del T-Deck). Así el teléfono se redirige al portal.
 *
 * En Android: conecttest.txt, generate_204
 * En iOS:     hotspot-detect.html, success.html
 * En Windows: ncsi.txt, connecttest.txt
 * En Linux:   captive.txt, generate_204
 *
 * "ubimqom asher YISHKÁN sham he'anán sham YAJANU bnei Yisrael"
 *   — Números 9:17
 * "En el lugar donde la nube HABITABA, allí ACAMPABAN."
 * La nube (el AP) indica dónde está el campamento. El DNS
 * redirige a todos al lugar donde la nube mora.
 *
 * --- IMPLEMENTACIÓN ---
 *
 * Servidor UDP en puerto 53.
 * Tarea FreeRTOS de prioridad baja (la puerta no consume recursos).
 * Parsea suficiente del paquete DNS para responder con A-record 192.168.4.1.
 *
 * 2 testigos:
 *   Jn 10:9 — una sola puerta: toda consulta DNS lleva al portal
 *   Nm 9:17 — donde mora la nube, allí se acampa: 192.168.4.1
 *
 * Autor: Antonio José Marín Soto — Hashmal-Core
 */

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"

#include "hashmal_perimeter.h"

/* ===================================================================
 * ENCAPSULACIÓN ESP-IDF
 * =================================================================== */
static const char *DNS_TAG = "hashmal.perimeter.dns";
#define DNS_LOG_I(fmt, ...)  ESP_LOGI(DNS_TAG, fmt, ##__VA_ARGS__)
#define DNS_LOG_W(fmt, ...)  ESP_LOGW(DNS_TAG, fmt, ##__VA_ARGS__)
#define DNS_LOG_E(fmt, ...)  ESP_LOGE(DNS_TAG, fmt, ##__VA_ARGS__)

/* ===================================================================
 * CONSTANTES
 * =================================================================== */
#define DNS_PORT          53
#define DNS_BUF_SIZE      512u
#define DNS_PORTAL_IP     { 192u, 168u, 4u, 1u }   /* IP del T-Deck */
#define DNS_TASK_STACK    4096u
#define DNS_TASK_PRIO     3u    /* prioridad baja — la puerta espera sin urgencia */

/* ===================================================================
 * FORMATO DNS (mínimo para A-record response)
 *
 * Estructura básica DNS (RFC 1035):
 *   Header: ID(2) + Flags(2) + QDCount(2) + ANCount(2) + NSCount(2) + ARCount(2)
 *   Question: QNAME(var) + QTYPE(2) + QCLASS(2)
 *   Answer: NAME(2) + TYPE(2) + CLASS(2) + TTL(4) + RDLENGTH(2) + RDATA(4)
 * =================================================================== */

/** Cabecera DNS — 12 bytes fijos */
typedef struct __attribute__((packed)) {
    uint16_t id;        /* identificador de consulta */
    uint16_t flags;     /* QR(1) OPCODE(4) AA(1) TC(1) RD(1) RA(1) Z(3) RCODE(4) */
    uint16_t qdcount;   /* número de preguntas */
    uint16_t ancount;   /* número de respuestas */
    uint16_t nscount;   /* authority RRs */
    uint16_t arcount;   /* additional RRs */
} dns_header_t;

/* ===================================================================
 * FUNCIONES INTERNAS
 * =================================================================== */

/**
 * Construye una respuesta DNS que apunta al portal (192.168.4.1).
 *
 * "Yo soy la puerta" — Juan 10:9. Toda pregunta DNS recibe la
 * misma respuesta: "el portal está en 192.168.4.1".
 *
 * Retorna el tamaño de la respuesta construida en out_buf.
 */
static int dns_construir_respuesta(const uint8_t *req,   size_t  req_len,
                                   uint8_t       *resp,  size_t  resp_cap)
{
    if (req_len < sizeof(dns_header_t) || resp_cap < (req_len + 16u)) {
        return -1;
    }

    /* Copiar cabecera de la pregunta */
    memcpy(resp, req, req_len);

    dns_header_t *hdr = (dns_header_t *)resp;

    /* Marcar como respuesta (QR=1), respuesta autoritativa (AA=1),
     * sin truncar (TC=0), recursión disponible (RA=1), RCODE=0 (OK).
     *
     * Flags en network byte order:
     *   QR=1, OPCODE=0, AA=1, TC=0, RD=1, RA=1, Z=0, RCODE=0
     *   = 1000 0001 1000 0000 = 0x8180 */
    hdr->flags   = htons(0x8180u);
    hdr->ancount = htons(1u);   /* 1 respuesta */
    hdr->nscount = htons(0u);
    hdr->arcount = htons(0u);

    /* Añadir el A-record al final del mensaje.
     * NAME: puntero de compresión → offset 12 (inicio de la pregunta).
     *   0xC0 0x0C = puntero a offset 12 (después de la cabecera) */
    uint8_t *ans = resp + req_len;
    size_t  space = resp_cap - req_len;
    if (space < 16u) return -1;

    const uint8_t portal_ip[4] = DNS_PORTAL_IP;

    ans[0]  = 0xC0u;    /* puntero de compresión — NAME */
    ans[1]  = 0x0Cu;    /* apunta a offset 12 (la pregunta) */
    ans[2]  = 0x00u;    /* TYPE: A (1) */
    ans[3]  = 0x01u;
    ans[4]  = 0x00u;    /* CLASS: IN (1) */
    ans[5]  = 0x01u;
    ans[6]  = 0x00u;    /* TTL: 60 segundos — breve para no cachear */
    ans[7]  = 0x00u;
    ans[8]  = 0x00u;
    ans[9]  = 60u;
    ans[10] = 0x00u;    /* RDLENGTH: 4 bytes (IPv4) */
    ans[11] = 0x04u;
    ans[12] = portal_ip[0];  /* RDATA: 192.168.4.1 */
    ans[13] = portal_ip[1];
    ans[14] = portal_ip[2];
    ans[15] = portal_ip[3];

    return (int)(req_len + 16u);
}

/* ===================================================================
 * TAREA FreeRTOS — DNS server loop
 * =================================================================== */

/**
 * Tarea DNS cautivo — escucha UDP:53 y responde redirigiendo al portal.
 *
 * "ló yamísh 'ammud heʻanán yomam veʻammud haʼésh layelah" — Éx 13:22.
 * La columna NUNCA se ausenta. Este servidor DNS tampoco — es perpetuo.
 *
 * 2 testigos:
 *   Éx 13:22 — ló yamísh: la tarea nunca se detiene (tamíd)
 *   Jn 10:9  — una sola puerta: toda consulta lleva al mismo lugar
 */
static void dns_cautivo_task(void *pv)
{
    (void)pv;
    DNS_LOG_I("tarea DNS cautivo arrancando (Jn 10:9 — una sola puerta)");

    /* Crear socket UDP */
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        DNS_LOG_E("socket: %d — DNS no disponible", sock);
        vTaskDelete(NULL);
        return;
    }

    /* Bind a puerto 53, todas las interfaces */
    struct sockaddr_in addr = {
        .sin_family      = AF_INET,
        .sin_port        = htons(DNS_PORT),
        .sin_addr.s_addr = htonl(INADDR_ANY),
    };
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        DNS_LOG_E("bind puerto 53 falló — ¿otro servidor DNS activo?");
        close(sock);
        vTaskDelete(NULL);
        return;
    }

    DNS_LOG_I("DNS cautivo escuchando en UDP:53 — redirige → 192.168.4.1");

    uint8_t req_buf[DNS_BUF_SIZE];
    uint8_t rsp_buf[DNS_BUF_SIZE + 16u];
    uint32_t consultas = 0u;

    for (;;) {
        struct sockaddr_in src;
        socklen_t src_len = sizeof(src);

        int req_len = recvfrom(sock,
                               req_buf, sizeof(req_buf),
                               0,
                               (struct sockaddr *)&src, &src_len);
        if (req_len < (int)sizeof(dns_header_t)) {
            /* Paquete demasiado corto o error */
            vTaskDelay(pdMS_TO_TICKS(10));
            continue;
        }

        ++consultas;

        /* Construir y enviar respuesta */
        int rsp_len = dns_construir_respuesta(req_buf, (size_t)req_len,
                                               rsp_buf, sizeof(rsp_buf));
        if (rsp_len > 0) {
            sendto(sock, rsp_buf, (size_t)rsp_len, 0,
                   (struct sockaddr *)&src, src_len);
        }

        /* Log cada 20 consultas para no saturar */
        if ((consultas % 20u) == 1u) {
            DNS_LOG_I("DNS: %" PRIu32 " consultas → 192.168.4.1 (Jn 10:9)",
                      consultas);
        }
    }

    close(sock);   /* nunca llega aquí — ló yamísh */
}

/* ===================================================================
 * API PÚBLICA
 * =================================================================== */

/**
 * hashmal_perimeter_dns_cautivo_init — arranca el servidor DNS.
 *
 * "vetiqeʻú... vetiqeʻú... vetiqeʻú" — Números 10:5-7.
 * La señal de trompeta llama a reunión. El DNS es la trompeta
 * que dirige a todos hacia el portal.
 *
 * 2 testigos:
 *   Nm 10:5  — tiqeʻú teruʻáh: la señal de movimiento (toque de alerta)
 *   Jn 10:9  — una sola puerta: el DNS garantiza que todos usen la misma
 */
int hashmal_perimeter_dns_cautivo_init(void)
{
    BaseType_t rc = xTaskCreate(
        dns_cautivo_task,
        "dns_cautivo",
        DNS_TASK_STACK,
        NULL,
        DNS_TASK_PRIO,
        NULL
    );

    if (rc != pdPASS) {
        DNS_LOG_E("dns_cautivo_init: no se pudo crear tarea");
        return -1;
    }

    DNS_LOG_I("DNS cautivo iniciado — tarea spawneada (Jn 10:9 thyra)");
    return 0;
}
