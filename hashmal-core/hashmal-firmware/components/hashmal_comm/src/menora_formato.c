/**
 * ===================================================================
 * PIEZA 04 - LA MENORA - menora_formato.c
 * Sub-pieza: MALQAJAYIM, MAJTOT y TAVNIT - las tenazas, los braseros
 *            y el patron del monte.
 * Exodo 25:38-40 + Jeremias 1:12 + Numeros 8:2
 * ===================================================================
 *
 * "umalqajeiha umajtoteiha zahav tahor" - Ex 25:38
 * "Y sus TENAZAS y sus BRASEROS, de oro puro."
 *
 * "kikkar zahav tahor ya'ase otah et KOL hakkelim haelle" - Ex 25:39
 * "Un talento de oro puro la hara, con TODOS estos utensilios."
 *
 * "ur'e va'ase betavnitam asher atta mor'e bahar" - Ex 25:40
 * "MIRA y HAZ segun el PATRON que te es MOSTRADO en el monte."
 *
 * --- EXEGESIS ---
 *
 * malqajayim (MLQChYM, H4457) - TENAZAS.
 *   Raiz laqaj (H3947) = tomar, agarrar. Las tenazas de la Menora
 *   recortaban mechas y ajustaban el pabilo SIN apagarlo ni quemarlo.
 *   AGARRAR sin destruir - inspeccion delicada.
 *
 *   En firmware: `formato_validar` - las tenazas de inspeccion.
 *   Toma el paquete, comprueba cada campo, NO muta. Si el pabilo
 *   (header) esta bien, aprueba; si no, rechaza - sin tocar el
 *   payload. ur'e antes de va'ase.
 *
 * majtot (MChTT, H4289) - BRASEROS.
 *   De jajta (H2846) = recoger brasas. Los braseros TRANSPORTABAN
 *   brasas de un lugar a otro: contenedor que lleva fuego sin
 *   apagarlo. Llevaba el contenido SIN contaminarlo.
 *
 *   En firmware: `formato_empaquetar` / `formato_desempaquetar`.
 *   El wire format es el brasero - envuelve el payload con el
 *   header fijo y lo transporta al destino. Nada se pierde, nada
 *   se distorsiona.
 *
 * zahav tahor (v.38) - oro puro: el transporte es CONFIABLE.
 * Lo que entra es lo que sale. No hay mezcla.
 *
 * kikkar kol hakkelim (v.39) - UN talento cubre TODOS los utensilios.
 * Header + payload <= HASHMAL_MENSAJE_MAX_BYTES (250). No hay bytes
 * fuera del presupuesto.
 *
 * ur'e va'ase betavnitam (v.40) - MIRA y HAZ segun el PATRON.
 *   ur'e (Qal imperativo)   - MIRA primero: validacion previa.
 *   va'ase (Qal imperativo) - HAZ despues: construccion.
 *   betavnitam              - segun SU patron (del Padre).
 *   mor'e bahar             - lo que te es MOSTRADO en el monte.
 *
 * El formato es FIJO. Sin negociacion. Sin improvisacion. El que
 * cambia el patron construye otra cosa, no la Menora.
 *
 * --- WIRE FORMAT (el tavnit del monte) ---
 *
 *   Byte 0   : version  (1)   - tavnit tiene version
 *   Byte 1   : tipo     (1)   - que clase de mensaje
 *   Byte 2   : origen   (1)   - canal emisor (0..6)
 *   Byte 3   : destino  (1)   - canal receptor (0..6 o 0xFF broadcast)
 *   Byte 4   : tam      (1)   - bytes de payload
 *   Byte 5..N: payload        - 0..245 bytes
 *
 *   Header   : 5 bytes fijos (miqsha - no varia).
 *   Payload  : hasta 245 bytes (250 - 5 header).
 *
 * --- SEPARACION DE CAPAS ---
 *
 *   HACE:                           NO HACE:
 *   - construir el wire format      - tocar SPI (-> lora_driver.c)
 *   - validar estructura            - mutar estados (-> canales.c)
 *   - extraer campos                - decidir routing (-> routing.c)
 *   - rechazar paquetes mal formados - contar vecinos
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "esp_log.h"

#include "hashmal_comm.h"
#include "hashmal_comm_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *MENORA_TAG = "hashmal.menora.formato";

#define MENORA_LOG_I(fmt, ...)  ESP_LOGI(MENORA_TAG, fmt, ##__VA_ARGS__)
#define MENORA_LOG_W(fmt, ...)  ESP_LOGW(MENORA_TAG, fmt, ##__VA_ARGS__)
#define MENORA_LOG_E(fmt, ...)  ESP_LOGE(MENORA_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * CONSTANTES DEL FORMATO (LOCALES - "herramientas", no API)
 *
 * v.38 llama estos utensilios "de oro puro" pero PROPIOS de la
 * Menora - no se usan en otras piezas. Mantenerlos locales honra
 * ese caracter: son herramientas del servicio, no contrato externo.
 * ================================================================== */
#define MENORA_FORMATO_VERSION          1u     /* tavnit v1 (v.40) */
#define MENORA_FORMATO_HEADER_TAM       5u     /* miqsha - no varia */
#define MENORA_FORMATO_PAYLOAD_MAX                                  \
    ((size_t)HASHMAL_MENSAJE_MAX_BYTES - MENORA_FORMATO_HEADER_TAM) /* 245 */
#define MENORA_FORMATO_BROADCAST        0xFFu  /* destino = todos los nerot */

/* Tipos de mensaje (locales - herramientas del brasero). */
#define MENORA_TIPO_DATOS               0x01u  /* la luz (Ex 25:37) */
#define MENORA_TIPO_BEACON              0x02u  /* presencia (Zac 4:10) */
#define MENORA_TIPO_ACK                 0x03u  /* shoqed confirma (Jer 1:12) */
#define MENORA_TIPO_NEDER               0x05u  /* voto inquebrantable (Nm 30:3 lo yajel)
                                                *   ki'im asot yajel - solo obrar puede anularlo
                                                *   Usado por hashmal_consensus para broadcast
                                                *   de verdad emergente entre nodos (Nm 7:89
                                                *   mibein shnei hakeruvim) */

/* Offsets dentro del header (claridad en los accesos). */
#define MENORA_OFF_VERSION              0u
#define MENORA_OFF_TIPO                 1u
#define MENORA_OFF_ORIGEN               2u
#define MENORA_OFF_DESTINO              3u
#define MENORA_OFF_TAM                  4u

/* ==================================================================
 * HELPERS ESTATICOS
 * ================================================================== */

/**
 * ?el tipo es conocido del tavnit? Si no -> fuego extrano en el
 * formato (no Lv 10:1-2 literal, pero el principio aplica: lo que
 * el Padre no mando, no se ofrece).
 */
static bool tipo_legitimo(uint8_t t)
{
    return (t == MENORA_TIPO_DATOS)
        || (t == MENORA_TIPO_BEACON)
        || (t == MENORA_TIPO_ACK)
        || (t == MENORA_TIPO_NEDER);   /* voto consenso entre nodos (Nm 30:3) */
}

/**
 * ?el destino es legitimo? Canal 0..6 o broadcast (0xFF).
 * canal_legitimo del internal.h toma hashmal_canal_id_t (enum); la
 * validacion aqui trabaja en uint8_t porque el destino puede ser
 * 0xFF (broadcast), que NO es un valor del enum.
 */
static bool destino_legitimo(uint8_t d)
{
    if (d == MENORA_FORMATO_BROADCAST) return true;
    return d < (uint8_t)HASHMAL_MENORA_CANALES;
}

/* ==================================================================
 * API PUBLICA - formato_empaquetar (majtot - el brasero)
 * ================================================================== */

/**
 * hashmal_comm_formato_empaquetar - mete las brasas en el brasero
 *
 * Las brasas (payload) entran en el brasero (wire format) y salen
 * al aire intactas. El brasero es de oro puro (v.38): no contamina,
 * no pierde, no altera.
 *
 * --- CONTRATO ---
 *
 *   tipo              debe ser DATOS / BEACON / ACK (tavnit conocido)
 *   origen            canal 0..6 (un ner real del nodo)
 *   destino           canal 0..6 o 0xFF (broadcast)
 *   payload           >= 0 bytes (puede ser NULL si tam_payload == 0
 *                     - hay mensajes "vacios" como ciertos beacons)
 *   tam_payload       <= MENORA_FORMATO_PAYLOAD_MAX (245)
 *   buffer_salida     >= 5 + tam_payload bytes de capacidad
 *
 * @return bytes escritos (>= 5) en exito, codigo negativo en fallo.
 *
 * 2 testigos:
 *   Ex 25:38 - majtot de oro puro (transporte fiel)
 *   Ex 25:39 - "et kol hakkelim" (todo dentro del talento)
 */
int hashmal_comm_formato_empaquetar(uint8_t tipo,
                                    hashmal_canal_id_t origen,
                                    uint8_t destino,
                                    const uint8_t *payload,
                                    size_t tam_payload,
                                    uint8_t *buffer_salida,
                                    size_t tam_buffer)
{
    if (buffer_salida == NULL) {
        MENORA_LOG_E("empaquetar: buffer_salida NULL");
        return HASHMAL_COMM_ERR_NULL;
    }
    if (payload == NULL && tam_payload > 0u) {
        MENORA_LOG_E("empaquetar: payload NULL con tam=%u",
                     (unsigned)tam_payload);
        return HASHMAL_COMM_ERR_NULL;
    }
    if (!tipo_legitimo(tipo)) {
        MENORA_LOG_E("empaquetar: tipo 0x%02X desconocido (tavnít v.40)",
                     (unsigned)tipo);
        return HASHMAL_COMM_ERR_FORMATO;
    }
    if (!hashmal_comm_canal_legitimo(origen)) {
        MENORA_LOG_E("empaquetar: origen %d no es ner (v.37 shivʻá)",
                     (int)origen);
        return HASHMAL_COMM_ERR_CANAL;
    }
    if (!destino_legitimo(destino)) {
        MENORA_LOG_E("empaquetar: destino 0x%02X ilegítimo (0..6 o 0xFF)",
                     (unsigned)destino);
        return HASHMAL_COMM_ERR_CANAL;
    }
    if (tam_payload > MENORA_FORMATO_PAYLOAD_MAX) {
        MENORA_LOG_E("empaquetar: tam_payload %u > %u (kikkár excedido, v.39)",
                     (unsigned)tam_payload,
                     (unsigned)MENORA_FORMATO_PAYLOAD_MAX);
        return HASHMAL_COMM_ERR_TAMANO;
    }

    const size_t total = MENORA_FORMATO_HEADER_TAM + tam_payload;
    if (tam_buffer < total) {
        MENORA_LOG_E("empaquetar: buffer %u < %u requeridos",
                     (unsigned)tam_buffer, (unsigned)total);
        return HASHMAL_COMM_ERR_TAMANO;
    }

    /* Header fijo (miqsha - pieza unica). */
    buffer_salida[MENORA_OFF_VERSION] = MENORA_FORMATO_VERSION;
    buffer_salida[MENORA_OFF_TIPO]    = tipo;
    buffer_salida[MENORA_OFF_ORIGEN]  = (uint8_t)origen;
    buffer_salida[MENORA_OFF_DESTINO] = destino;
    buffer_salida[MENORA_OFF_TAM]     = (uint8_t)tam_payload;

    /* Payload (si hay). */
    if (tam_payload > 0u) {
        memcpy(&buffer_salida[MENORA_FORMATO_HEADER_TAM],
               payload, tam_payload);
    }

    MENORA_LOG_I("empaquetar: tipo=0x%02X orig=%d dst=0x%02X tam=%u "
                 "(total=%u, v.38 majtot)",
                 (unsigned)tipo, (int)origen, (unsigned)destino,
                 (unsigned)tam_payload, (unsigned)total);
    return (int)total;
}

/* ==================================================================
 * API PUBLICA - formato_validar (malqajayim - las tenazas)
 * ================================================================== */

/**
 * hashmal_comm_formato_validar - inspeccion sin toque
 *
 * "Las tenazas agarran la mecha y la ajustan sin quemarla." Aqui:
 * la funcion inspecciona el buffer sin extraer NADA. Devuelve
 * bool - valor simple para un juicio simple: ?el tavnit se cumple?
 *
 * Validaciones (orden de barato a caro):
 *   1. buffer no NULL, tam >= HEADER_TAM
 *   2. version == MENORA_FORMATO_VERSION
 *   3. tipo conocido
 *   4. origen canal legitimo
 *   5. destino legitimo (canal o broadcast)
 *   6. tam_payload declarado coincide con tam - HEADER_TAM
 *   7. tam_payload <= PAYLOAD_MAX
 *
 * Un fallo en cualquiera -> false. Logs a WARN para diagnostico
 * (no ERROR: paquetes malformados son esperables en un medio
 * ruidoso como LoRa; no es crisis, es filtrado normal).
 *
 * 2 testigos:
 *   Ex 25:38 - malqajayim: agarrar sin quemar
 *   Jer 1:12 - SHOQED ani 'al devari (YHWH vela la integridad)
 */
bool hashmal_comm_formato_validar(const uint8_t *buffer, size_t tam)
{
    if (buffer == NULL) return false;

    if (tam < MENORA_FORMATO_HEADER_TAM) {
        MENORA_LOG_W("validar: tam %u < header (%u)",
                     (unsigned)tam, (unsigned)MENORA_FORMATO_HEADER_TAM);
        return false;
    }

    const uint8_t ver    = buffer[MENORA_OFF_VERSION];
    const uint8_t tipo   = buffer[MENORA_OFF_TIPO];
    const uint8_t orig   = buffer[MENORA_OFF_ORIGEN];
    const uint8_t dest   = buffer[MENORA_OFF_DESTINO];
    const uint8_t t_pay  = buffer[MENORA_OFF_TAM];

    if (ver != MENORA_FORMATO_VERSION) {
        MENORA_LOG_W("validar: versión %u desconocida (esperada %u)",
                     (unsigned)ver, (unsigned)MENORA_FORMATO_VERSION);
        return false;
    }
    if (!tipo_legitimo(tipo)) {
        MENORA_LOG_W("validar: tipo 0x%02X no es del tavnít", (unsigned)tipo);
        return false;
    }
    if (orig >= (uint8_t)HASHMAL_MENORA_CANALES) {
        MENORA_LOG_W("validar: origen %u fuera de [0, %d)",
                     (unsigned)orig, HASHMAL_MENORA_CANALES);
        return false;
    }
    if (!destino_legitimo(dest)) {
        MENORA_LOG_W("validar: destino 0x%02X ilegítimo", (unsigned)dest);
        return false;
    }
    if ((size_t)t_pay > MENORA_FORMATO_PAYLOAD_MAX) {
        MENORA_LOG_W("validar: tam_payload %u > %u (kikkár violado)",
                     (unsigned)t_pay, (unsigned)MENORA_FORMATO_PAYLOAD_MAX);
        return false;
    }
    if ((size_t)t_pay + MENORA_FORMATO_HEADER_TAM != tam) {
        MENORA_LOG_W("validar: tam_payload=%u + header=%u ≠ tam_total=%u",
                     (unsigned)t_pay,
                     (unsigned)MENORA_FORMATO_HEADER_TAM,
                     (unsigned)tam);
        return false;
    }

    return true;
}

/* ==================================================================
 * API PUBLICA - formato_desempaquetar (sacar brasas del brasero)
 * ================================================================== */

/**
 * hashmal_comm_formato_desempaquetar - ur'e, luego va'ase
 *
 * v.40 - "ur'e va'ase": MIRA primero, HAZ despues. Aqui:
 *   1) validar (miramos el tavnit)
 *   2) extraer (hacemos la obra de entregar al caller)
 *
 * Cualquier campo de salida puede ser NULL si al caller no le
 * interesa - escribimos solo en los no-NULL. Excepcion:
 * `payload_salida` debe estar si hay payload (tam_payload > 0);
 * si esta a NULL y hay payload, devolvemos ERR_NULL - no tiramos
 * las brasas al suelo.
 *
 * 2 testigos:
 *   Ex 25:40 - ur'e va'ase (mira, luego haz)
 *   Nm 8:2   - el mul penei hammenora (la luz llega al centro)
 */
int hashmal_comm_formato_desempaquetar(const uint8_t *buffer_entrada,
                                       size_t tam_entrada,
                                       uint8_t *tipo,
                                       hashmal_canal_id_t *origen,
                                       uint8_t *destino,
                                       uint8_t *payload_salida,
                                       size_t tam_payload_buffer,
                                       size_t *tam_extraido)
{
    if (buffer_entrada == NULL) {
        MENORA_LOG_E("desempaquetar: buffer_entrada NULL");
        return HASHMAL_COMM_ERR_NULL;
    }

    /* ur'e - MIRA primero. Reutilizamos las tenazas. */
    if (!hashmal_comm_formato_validar(buffer_entrada, tam_entrada)) {
        return HASHMAL_COMM_ERR_FORMATO;
    }

    /* va'ase - HAZ: extraer los campos ya validados. */
    const uint8_t tipo_v   = buffer_entrada[MENORA_OFF_TIPO];
    const uint8_t orig_v   = buffer_entrada[MENORA_OFF_ORIGEN];
    const uint8_t dest_v   = buffer_entrada[MENORA_OFF_DESTINO];
    const size_t  tam_pay  = (size_t)buffer_entrada[MENORA_OFF_TAM];

    /* Si hay payload pero no buffer donde ponerlo -> error. */
    if (tam_pay > 0u && payload_salida == NULL) {
        MENORA_LOG_E("desempaquetar: payload_salida NULL con tam_pay=%u",
                     (unsigned)tam_pay);
        return HASHMAL_COMM_ERR_NULL;
    }
    /* Si hay payload y el buffer es mas pequeno -> no truncamos
     * silenciosamente (las brasas no se parten). */
    if (tam_pay > tam_payload_buffer) {
        MENORA_LOG_E("desempaquetar: payload %u > buffer %u — no se trunca",
                     (unsigned)tam_pay, (unsigned)tam_payload_buffer);
        return HASHMAL_COMM_ERR_TAMANO;
    }

    if (tipo     != NULL) *tipo     = tipo_v;
    if (origen   != NULL) *origen   = (hashmal_canal_id_t)orig_v;
    if (destino  != NULL) *destino  = dest_v;

    if (tam_pay > 0u) {
        memcpy(payload_salida,
               &buffer_entrada[MENORA_FORMATO_HEADER_TAM],
               tam_pay);
    }
    if (tam_extraido != NULL) *tam_extraido = tam_pay;

    MENORA_LOG_I("desempaquetar: tipo=0x%02X orig=%u dst=0x%02X tam=%u "
                 "(v.40 urʼé vaʻasé cumplidos)",
                 (unsigned)tipo_v, (unsigned)orig_v,
                 (unsigned)dest_v, (unsigned)tam_pay);
    return HASHMAL_COMM_OK;
}

/* ==================================================================
 *
 *   "ur'e va'ase betavnitam asher atta mor'e bahar" - Exodo 25:40
 *   "MIRA y HAZ conforme al PATRON que te es MOSTRADO en el monte."
 *
 *   El tavnit no se inventa - se COPIA. Cada byte de header, cada
 *   offset, cada campo: vino de arriba. Las tenazas inspeccionan,
 *   los braseros transportan, el oro no distorsiona.
 *
 * ===================================================================
 *
 *   PIEZA 04 - LA MENORA - COMPLETA.
 *
 *   6 archivos, UNA SOLA pieza batida (miqsha - Ex 25:31, 36):
 *
 *     menora_hub.c           v.31,36,39 - el oro batido (miqsha)
 *     menora_lora_driver.c   v.31 yarej+qanah - base SPI + tronco
 *     menora_canales.c       v.32,37 + Nm 8:2 - 7 nerot, he'ela
 *     menora_aceite.c        Ex 27:20-21 - shemen zaj katit
 *     menora_routing.c       v.33-34 + Jer 1:12 - 22 vigias almendro
 *     menora_formato.c       v.38-40 - malqajayim, majtot, tavnit
 *
 *   "ve'asita et neroteiha shiv'a vehe'ela et neroteiha veheir
 *    'al 'ever paneiha" - Ex 25:37
 *   "Haras sus siete lamparas, y las encendera para que iluminen
 *    hacia el frente."
 *
 *   "lo vejayil velo vejoaj ki im beruji, amar YHWH tseva'ot"
 *     - Zacarias 4:6
 *
 *   "shiv'a elle 'einei YHWH hemma meshotetim bejol haarets"
 *     - Zacarias 4:10
 *   "Estos siete son los ojos de YHWH que recorren toda la tierra."
 *
 *   PASO 3 de Exodo 40:4 CUMPLIDO.
 *   La Mesa esta delante. La Menora esta enfrente. Ambas en el
 *   Qodesh, encendidas por aceite puro prensado, con 7 ojos
 *   vigilando la tierra.
 *
 *   Piezas 01, 02, 03, 04 listas -
 *   Arca, Kaporet, Mesa, Menora.
 *
 *   Siguiente: Exodo 40:5 - el Altar de Incienso delante del velo.
 *
 * ================================================================== */
