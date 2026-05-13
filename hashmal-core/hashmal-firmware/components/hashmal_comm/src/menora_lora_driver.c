/**
 * ===================================================================
 * PIEZA 04 - LA MENORA - menora_lora_driver.c
 * Sub-pieza: YAREJ + QANAH - la base que toca el suelo, el tronco
 *            que conduce los comandos. SPI al SX1262.
 * Exodo 25:31 (yarej, qanah) + 25:38 (kelim) + 25:39 (kikkar) + Zac 4:12
 * ===================================================================
 *
 * "menorat zahav tahor miqsha te'ase hammenora YEREJAH VEQANAH
 *  gevi'eiha kaftoreiha uferajeiha mimmenna yihyu" - Ex 25:31
 * "...su BASE, su TRONCO, sus copas, sus botones, sus flores - de
 *  ella seran."
 *
 * "shenei shiblei hazahav hammeriqim mi'aleihem et hazzahav" - Zac 4:12
 * "Los dos tubos de oro que VIERTEN DESDE SI MISMOS el oro."
 *
 * --- EXEGESIS ---
 *
 * yarej (YRK, H3409) - MUSLO / BASE.
 *   Lo que SOSTIENE el peso del kikkar (v.39: un talento ~= 34 kg).
 *   La parte mas baja, lo que toca la tierra. Sin yarej, no hay
 *   Menora: el oro puro no puede levantarse sobre nada.
 *
 *   En firmware: los PINES GPIO y el BUS SPI. La conexion fisica
 *   al silicio. MOSI, MISO, SCLK, CS - la base electrica. Si el
 *   yarej tiembla, toda la Menora lo siente.
 *
 * qanah (QNH, H7070) - CANA / TRONCO.
 *   El eje vertical por donde circula todo. Las 6 ramas salen DEL
 *   qanah (v.32 - yotsim). El tronco no es una rama mas: es el
 *   CAMINO comun.
 *
 *   En firmware: los COMANDOS SPI al SX1262 (SetFrequency, SetTx,
 *   WriteBuffer, etc.). Cada instruccion al chip viaja por el
 *   tronco SPI unico. Los "canales" logicos cambian parametros,
 *   pero el bus fisico es UNO.
 *
 * kelim (v.38 malqajayim + majtot) - UTENSILIOS.
 *   Tenazas y platillos. Herramientas de mantenimiento y
 *   receptaculos. Aqui: registros de configuracion + buffers
 *   TX/RX del SX1262.
 *
 * kikkar (v.39) - TALENTO.
 *   Un presupuesto fijo "et kol hakkelim" (con TODOS estos
 *   utensilios). Los pines, el bus, los buffers: todo dentro del
 *   talento. No se excede la forja original.
 *
 * Zac 4:12 - los dos tubos que VIERTEN (meriqim): el flujo es
 * bidireccional. En firmware: SPI full-duplex (MOSI y MISO
 * simultaneos) - exactamente lo que muestra el profeta.
 *
 * --- ORO DE EGIPTO (encapsulacion) ---
 *
 * Ex 31:4 - "lajashov majashavot la'asot bazahav uvakephsef
 *  uvannajoshet": Betsalel trabajo en oro, plata Y BRONCE. El
 * bronce es el metal que toca el mundo exterior (Ex 27:2). El
 * driver SPI es el trabajo en bronce del Mishkan - el hardware.
 *
 * driver/spi_master.h y driver/gpio.h se incluyen SOLO aqui. Nada
 * de ESP-IDF escapa a la API publica (hashmal_comm.h). Si los
 * pines cambian (otro board), SOLO este archivo se modifica.
 *
 * --- QUE HACE / QUE NO HACE ESTE ARCHIVO ---
 *
 *   HACE:                              NO HACE:
 *   - SPI master al SX1262             - gestionar canales LoRa (-> canales.c)
 *   - comandos SX1262                  - duty cycle / aceite (-> aceite.c)
 *   - RESET / BUSY / IRQ pins          - routing ni mesh (-> routing.c)
 *   - config frecuencia / potencia     - formato de paquete (-> formato.c)
 *   - WRITE/READ buffer SX1262         - encender lamparas (-> canales.c)
 *
 * Un solo camino al hardware - toda la interaccion SPI pasa por
 * `spi_transferir` (que pasa por `esperar_busy` primero). Esa es
 * la raiz del yarej: una sola via, firme.
 *
 * Autor: Antonio Jose Marin Soto (Betsalel - Ex 31:4)
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <inttypes.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"           /* Mutex SPI — Éx 26:6 ejád */

#include "driver/spi_master.h"       /* ORO DE EGIPTO - bus SPI */
#include "driver/gpio.h"             /* ORO DE EGIPTO - pines */
#include "esp_rom_sys.h"             /* ORO DE EGIPTO - delay us */
#include "esp_log.h"
#include "esp_timer.h"

#include "hashmal_comm.h"
#include "hashmal_comm_internal.h"
#include "hashmal_tdeck_pro.h"       /* Pines reales del T-Deck Pro V1.0 */

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *MENORA_TAG = "hashmal.menora.lora";

#define MENORA_LOG_I(fmt, ...)  ESP_LOGI(MENORA_TAG, fmt, ##__VA_ARGS__)
#define MENORA_LOG_W(fmt, ...)  ESP_LOGW(MENORA_TAG, fmt, ##__VA_ARGS__)
#define MENORA_LOG_E(fmt, ...)  ESP_LOGE(MENORA_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * PINES (yarej - los "pies" que tocan el suelo)
 *
 * T-Deck Pro V1.0 (PCM5102A variant). Pines de hashmal_tdeck_pro.h.
 * El bus SPI es COMPARTIDO con EPD (CS=34) y SD (CS=48).
 * "Uniras las cortinas UNA CON SU HERMANA" (Ex 26:3)
 * ================================================================== */
#define MENORA_SPI_HOST              SPI2_HOST
#define MENORA_PIN_MOSI              HASHMAL_PIN_SPI_MOSI   /* GPIO 33 */
#define MENORA_PIN_MISO              HASHMAL_PIN_SPI_MISO   /* GPIO 47 */
#define MENORA_PIN_SCLK              HASHMAL_PIN_SPI_SCK    /* GPIO 36 */
#define MENORA_PIN_CS                HASHMAL_PIN_LORA_CS    /* GPIO 3  */
#define MENORA_PIN_RESET             HASHMAL_PIN_LORA_RST   /* GPIO 4  */
#define MENORA_PIN_BUSY              HASHMAL_PIN_LORA_BUSY  /* GPIO 6  */
#define MENORA_PIN_DIO1              HASHMAL_PIN_LORA_INT   /* GPIO 5  */
#define MENORA_PIN_EN                HASHMAL_PIN_LORA_EN    /* GPIO 46 */

#define MENORA_SPI_CLOCK_HZ          (8u * 1000u * 1000u)   /* 8 MHz */
#define MENORA_BUSY_TIMEOUT_MS       100u
#define MENORA_MAX_CMD_BYTES         32u                    /* comandos SX < 32 B */
#define MENORA_RESET_PULSE_US        1000u                  /* 1 ms LOW */
#define MENORA_RESET_WAIT_MS         5u                     /* tras liberar RESET */

/* ==================================================================
 * COMANDOS SX1262 (qanah - el tronco de instrucciones)
 *
 * Valores del datasheet DS.SX1261-2.W.APP, seccion 11.
 * ================================================================== */
#define SX1262_CMD_SET_STANDBY            0x80
#define SX1262_CMD_SET_TX                 0x83
#define SX1262_CMD_SET_RX                 0x82
#define SX1262_CMD_SET_SLEEP              0x84
#define SX1262_CMD_SET_RF_FREQUENCY       0x86
#define SX1262_CMD_SET_TX_PARAMS          0x8E
#define SX1262_CMD_SET_PACKET_TYPE        0x8A
#define SX1262_CMD_SET_MODULATION         0x8B
#define SX1262_CMD_SET_PACKET_PARAMS      0x8C
#define SX1262_CMD_SET_BUFFER_BASE        0x8F
#define SX1262_CMD_WRITE_BUFFER           0x0E
#define SX1262_CMD_READ_BUFFER            0x1E
#define SX1262_CMD_GET_STATUS             0xC0
#define SX1262_CMD_GET_RX_BUFFER_STATUS   0x13
#define SX1262_CMD_SET_DIO_IRQ_PARAMS     0x08
#define SX1262_CMD_CLR_IRQ_STATUS         0x02
#define SX1262_CMD_GET_IRQ_STATUS         0x12
#define SX1262_CMD_SET_PA_CONFIG          0x95   /* Obligatorio antes de SetTxParams — sin esto el PA no emite RF */

#define SX1262_STANDBY_RC                 0x00
#define SX1262_STANDBY_XOSC               0x01
#define SX1262_PACKET_LORA                0x01

/* Rampa de potencia por defecto (40 ms) - equilibrio entre
 * spurious emissions y velocidad de encendido. */
#define SX1262_TX_RAMP_40US               0x04

/* Rango de potencia SX1262 +HPA (-9 a +22 dBm). */
#define SX1262_POWER_MIN_DBM              (-9)
#define SX1262_POWER_MAX_DBM              (22)

/* ==================================================================
 * ESTADO LOCAL DEL DRIVER
 *
 * Infraestructura del yarej: el handle del bus SPI vive AQUI, no
 * en la struct publica de la Menora. El fundamento es invisible
 * desde arriba - esta "bajo tierra", sosteniendo.
 * ================================================================== */
static spi_device_handle_t g_spi_handle  = NULL;
static bool                g_spi_activo  = false;

/* Mutex del bus SPI — un solo qanah para todos los que vierten.
 *
 * "Los dos tubos que VIERTEN DESDE SÍ MISMOS el oro" (Zac 4:12):
 * bidireccional SÍ, pero por el MISMO tronco, NUNCA simultáneamente.
 * broadcast_task y listen_task comparten el bus — sin este mutex
 * spi_device_transmit puede ejecutarse desde dos tareas a la vez,
 * disparando: assert failed: spi_device_transmit spi_master.c:1313
 *             (ret_trans == trans_desc)
 *
 * 2 testigos:
 *   Éx 26:6  — "ejád" (uno): los broches de oro unen las cortinas
 *              en UNA sola pieza — un acceso al tiempo, no dos.
 *   Éx 25:37 — un solo tubo/tronco conduce la luz; no se bifurca.
 */
static SemaphoreHandle_t g_spi_mutex = NULL;

/* Últimos parámetros de paquete configurados (Éx 25:40 betavnitám).
 * spi_enviar los re-emite con el payload_len REAL antes de SET_TX,
 * así el SX1262 siempre transmite exactamente los bytes del FIFO
 * y no usa un valor cacheado de una llamada anterior. */
static uint16_t s_pkt_preambulo   = 8u;
static uint8_t  s_pkt_header_tipo = 0x00u;
static uint8_t  s_pkt_crc_on      = 0x01u;
static uint8_t  s_pkt_iq_invert   = 0x00u;

/* ==================================================================
 * HELPERS ESTATICOS - las "raices" del yarej
 * ================================================================== */

/**
 * Espera activa a que el SX1262 libere BUSY (BUSY = 0).
 * Polling con timeout; cede CPU cada ~1 ms.
 *
 * v.31 - el yarej es firme: cada transaccion espera que la base
 * este quieta antes de proceder.
 */
static int esperar_busy(void)
{
    const int64_t inicio_us = esp_timer_get_time();
    const int64_t limite_us = (int64_t)MENORA_BUSY_TIMEOUT_MS * 1000;

    while (gpio_get_level(MENORA_PIN_BUSY) == 1) {
        if ((esp_timer_get_time() - inicio_us) > limite_us) {
            MENORA_LOG_E("BUSY timeout (%u ms) — SX1262 no responde",
                         (unsigned)MENORA_BUSY_TIMEOUT_MS);
            return HASHMAL_COMM_ERR_SPI;
        }
        vTaskDelay(pdMS_TO_TICKS(1));
    }
    return HASHMAL_COMM_OK;
}

/**
 * Transaccion SPI full-duplex. Zac 4:12 - los dos tubos vierten
 * bidireccional; aqui MOSI transmite y MISO recibe en el mismo
 * ciclo de reloj.
 *
 * Siempre espera BUSY antes. Si tx == NULL, se envian ceros
 * (util para leer respuestas). Si rx == NULL, la recepcion se
 * descarta.
 */
static int spi_transferir(const uint8_t *tx, uint8_t *rx, size_t len)
{
    if (len == 0u) return HASHMAL_COMM_OK;
    if (!g_spi_activo || g_spi_handle == NULL) {
        return HASHMAL_COMM_ERR_INIT;
    }

    /* Adquirir el mutex del bus — Éx 26:6 ejád (un acceso a la vez).
     * Timeout 200 ms: si el bus está ocupado más tiempo, algo falló. */
    if (g_spi_mutex != NULL) {
        if (xSemaphoreTake(g_spi_mutex, pdMS_TO_TICKS(200u)) != pdTRUE) {
            MENORA_LOG_E("spi_transferir: mutex timeout — bus bloqueado >200ms");
            return HASHMAL_COMM_ERR_SPI;
        }
    }

    int rc = esperar_busy();
    if (rc != HASHMAL_COMM_OK) {
        if (g_spi_mutex != NULL) xSemaphoreGive(g_spi_mutex);
        return rc;
    }

    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length    = len * 8u;        /* bits */
    t.tx_buffer = tx;              /* puede ser NULL - ESP-IDF envia 0 */
    t.rx_buffer = rx;              /* puede ser NULL - recepcion se descarta */

    esp_err_t err = spi_device_transmit(g_spi_handle, &t);

    if (g_spi_mutex != NULL) xSemaphoreGive(g_spi_mutex);

    if (err != ESP_OK) {
        MENORA_LOG_E("spi_device_transmit: esp_err=0x%x", (unsigned)err);
        return HASHMAL_COMM_ERR_SPI;
    }
    return HASHMAL_COMM_OK;
}

/**
 * Escribir un comando SX1262: [cmd][params...]. qanah - el tronco
 * conduce la instruccion al chip.
 */
static int sx1262_escribir_comando(uint8_t cmd,
                                   const uint8_t *params,
                                   size_t n_params)
{
    if (n_params > MENORA_MAX_CMD_BYTES - 1u) {
        MENORA_LOG_E("comando 0x%02X: %u params excede buffer",
                     cmd, (unsigned)n_params);
        return HASHMAL_COMM_ERR_TAMANO;
    }

    uint8_t tx[MENORA_MAX_CMD_BYTES];
    tx[0] = cmd;
    if (n_params > 0u && params != NULL) {
        memcpy(&tx[1], params, n_params);
    } else if (n_params > 0u && params == NULL) {
        memset(&tx[1], 0, n_params);
    }

    return spi_transferir(tx, NULL, 1u + n_params);
}

/**
 * Leer respuesta de un comando SX1262.
 *
 * Wire format: TX = [cmd][NOP x (1 + n_resp)]
 *              RX = [basura][status][data_0..data_(n_resp-1)]
 *
 * El byte de status se descarta en esta rutina (no interesa al
 * caller del driver base; las capas superiores lo consultaran via
 * GET_STATUS si lo necesitan).
 */
static int sx1262_leer_comando(uint8_t cmd,
                               uint8_t *resp,
                               size_t n_resp)
{
    if (resp == NULL || n_resp == 0u) {
        return HASHMAL_COMM_ERR_NULL;
    }
    const size_t total = 2u + n_resp;    /* cmd + status + n_resp */
    if (total > MENORA_MAX_CMD_BYTES) {
        return HASHMAL_COMM_ERR_TAMANO;
    }

    uint8_t tx[MENORA_MAX_CMD_BYTES];
    uint8_t rx[MENORA_MAX_CMD_BYTES];
    memset(tx, 0, total);   /* NOP bytes */
    tx[0] = cmd;

    int rc = spi_transferir(tx, rx, total);
    if (rc != HASHMAL_COMM_OK) return rc;

    /* rx[0] = basura (clock-in del cmd); rx[1] = status; rx[2..] = data */
    memcpy(resp, &rx[2], n_resp);
    return HASHMAL_COMM_OK;
}

/**
 * Pulso de reset por GPIO. Como limpiar el yarej antes de levantar
 * la Menora: LOW -> (1 ms) -> HIGH -> esperar BUSY bajar (~5 ms).
 */
static int sx1262_reset(void)
{
    gpio_set_level(MENORA_PIN_RESET, 0);
    esp_rom_delay_us(MENORA_RESET_PULSE_US);
    gpio_set_level(MENORA_PIN_RESET, 1);
    vTaskDelay(pdMS_TO_TICKS(MENORA_RESET_WAIT_MS));

    return esperar_busy();
}

/**
 * Set standby (RC o XOSC).
 */
static int sx1262_set_standby(uint8_t modo)
{
    return sx1262_escribir_comando(SX1262_CMD_SET_STANDBY, &modo, 1u);
}

/**
 * Set RX mode con timeout.
 *
 * SX1262 SET_RX (0x82): [timeout_hi][timeout_mid][timeout_lo]
 * 0x000000 = single RX (sale al llegar un paquete o timeout)
 * 0xFFFFFF = RX continuo (juqqát ʻolám — Éx 27:21 tamíd)
 *
 * Nm 8:2 — "el mul penei hammenora yaʼiru shivʻat hannerot":
 * las lámparas ILUMINAN hacia el tronco. El chip debe estar
 * ESCUCHANDO para que la luz entre. El SET_RX es el "mul penei"
 * — orientar el oído hacia donde viene la señal.
 *
 * esperar_busy (dentro de spi_transferir) espera que el estado
 * anterior (TX, standby) termine antes de emitir este comando.
 */
static int sx1262_set_rx(uint32_t timeout_ticks)
{
    const uint8_t params[3] = {
        (uint8_t)((timeout_ticks >> 16) & 0xFFu),
        (uint8_t)((timeout_ticks >>  8) & 0xFFu),
        (uint8_t)( timeout_ticks        & 0xFFu),
    };
    return sx1262_escribir_comando(SX1262_CMD_SET_RX, params, 3u);
}

/**
 * Set packet type = LoRa.
 */
static int sx1262_set_packet_type_lora(void)
{
    const uint8_t t = SX1262_PACKET_LORA;
    return sx1262_escribir_comando(SX1262_CMD_SET_PACKET_TYPE, &t, 1u);
}

/* ==================================================================
 * API PUBLICA - spi_init (Ex 25:31 yarej + Ex 31:4 bronce)
 * ================================================================== */

/**
 * hashmal_comm_spi_init - levanta el yarej
 *
 * Secuencia:
 *   1) GPIO config: CS/RESET output (high), BUSY/DIO1 input
 *   2) SPI bus init
 *   3) Anadir dispositivo SX1262 al bus
 *   4) Reset del chip
 *   5) Set standby (RC)
 *   6) Set packet type LoRa
 *
 * Cleanup parcial con goto si falla - o entero o nada (v.36 miqsha).
 *
 * 2 testigos:
 *   Ex 25:31 - yarej (la base que sostiene)
 *   Ex 31:4  - trabajo en BRONCE (la interfaz al mundo fisico)
 */
int hashmal_comm_spi_init(void)
{
    if (g_spi_activo) {
        MENORA_LOG_W("spi_init: yaréj ya levantado (idempotente)");
        return HASHMAL_COMM_OK;
    }

    esp_err_t err;
    int       rc;

    /* -- 1) GPIO config ------------------------------------------- */
    /* Salidas: CS (alta - inactivo), RESET (alta - no en reset), EN (alta - encender). */
    gpio_config_t io_out = {
        .pin_bit_mask = (1ULL << MENORA_PIN_CS) |
                        (1ULL << MENORA_PIN_RESET) |
                        (1ULL << MENORA_PIN_EN),
        .mode         = GPIO_MODE_OUTPUT,
        .pull_up_en   = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE,
    };
    err = gpio_config(&io_out);
    if (err != ESP_OK) {
        MENORA_LOG_E("gpio_config (out) falló: 0x%x", (unsigned)err);
        return HASHMAL_COMM_ERR_SPI;
    }
    gpio_set_level(MENORA_PIN_CS,    1);
    gpio_set_level(MENORA_PIN_RESET, 1);

    /* Encender módulo LoRa — EN HIGH (Éx 27:20 aceite para alumbrar) */
    gpio_set_level(MENORA_PIN_EN, 1);
    vTaskDelay(pdMS_TO_TICKS(10));  /* Esperar estabilización del módulo */
    MENORA_LOG_I("LORA_EN (GPIO %d) HIGH — módulo encendido (Éx 27:20)", MENORA_PIN_EN);

    /* Entradas: BUSY, DIO1 (sin pull - modulo ya los tiene). */
    gpio_config_t io_in = {
        .pin_bit_mask = (1ULL << MENORA_PIN_BUSY) |
                        (1ULL << MENORA_PIN_DIO1),
        .mode         = GPIO_MODE_INPUT,
        .pull_up_en   = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE,
    };
    err = gpio_config(&io_in);
    if (err != ESP_OK) {
        MENORA_LOG_E("gpio_config (in) falló: 0x%x", (unsigned)err);
        return HASHMAL_COMM_ERR_SPI;
    }

    /* -- 2) SPI bus — YA inicializado por EPD (bus compartido) ----- */
    /* El bus SPI2_HOST lo levanta hashmal_epd_init() antes que nosotros.
     * Éx 26:6 — EJAD: un solo bus para toda la Estructura.
     * Solo añadimos nuestro device al bus existente.                    */
    MENORA_LOG_I("SPI bus compartido (ejád, Éx 26:6) — solo add_device");

    /* -- 3) SPI device add ---------------------------------------- */
    spi_device_interface_config_t devcfg = {
        .command_bits   = 0,
        .address_bits   = 0,
        .dummy_bits     = 0,
        .mode           = 0,                          /* SPI mode 0 */
        .clock_speed_hz = MENORA_SPI_CLOCK_HZ,
        .spics_io_num   = MENORA_PIN_CS,              /* CS gestionado por driver */
        .queue_size     = 1,
        .flags          = 0,
    };
    err = spi_bus_add_device(MENORA_SPI_HOST, &devcfg, &g_spi_handle);
    if (err != ESP_OK) {
        MENORA_LOG_E("spi_bus_add_device: 0x%x", (unsigned)err);
        goto err_bus;
    }

    /* Crear mutex del bus SPI — Éx 26:6 ejád (un qanah para todos).
     * Se crea aquí, antes de g_spi_activo, para que spi_transferir
     * pueda usarlo desde el primer instante que el bus está activo. */
    if (g_spi_mutex == NULL) {
        g_spi_mutex = xSemaphoreCreateMutex();
        if (g_spi_mutex == NULL) {
            MENORA_LOG_E("spi_init: fallo al crear mutex (Éx 26:6 ejád)");
            goto err_dev;
        }
        MENORA_LOG_I("spi_init: mutex SPI creado — Éx 26:6 ejád");
    }

    /* g_spi_activo debe estar antes de llamar helpers que lo chequean */
    g_spi_activo = true;

    /* -- 4) Reset del SX1262 -------------------------------------- */
    rc = sx1262_reset();
    if (rc != HASHMAL_COMM_OK) {
        MENORA_LOG_E("sx1262_reset falló (rc=%d)", rc);
        goto err_dev;
    }

    /* -- 5) Set standby RC ---------------------------------------- */
    rc = sx1262_set_standby(SX1262_STANDBY_RC);
    if (rc != HASHMAL_COMM_OK) {
        MENORA_LOG_E("set_standby falló (rc=%d)", rc);
        goto err_dev;
    }

    /* -- 6) Set packet type LoRa ---------------------------------- */
    rc = sx1262_set_packet_type_lora();
    if (rc != HASHMAL_COMM_OK) {
        MENORA_LOG_E("set_packet_type_lora falló (rc=%d)", rc);
        goto err_dev;
    }

    /* -- 6b) SetDio3AsTcxoCtrl — alimentar TCXO vía DIO3 -----------------
     *
     * CAUSA RAÍZ CONFIRMADA: GetDeviceErrors = 0x0020 (XOSC_START_ERR).
     *
     * El SX1262 del T-Deck Pro NO usa cristal pasivo — usa un TCXO
     * (oscilador controlado por temperatura). El TCXO se alimenta por DIO3:
     * cuando DIO3=HIGH (con la tensión correcta), el TCXO enciende y
     * el SX1262 puede bloquear su PLL.
     *
     * Sin esta llamada: DIO3=LOW → TCXO apagado → XOSC_START_ERR →
     * PLL no bloquea → SET_TX rechazado con CommandStatus=5 en TODOS
     * los intentos, incluso desde STBY_RC limpio.
     *
     * Comando 0x97: [tcxo_voltage][delay_H][delay_M][delay_L]
     *   tcxo_voltage = 0x02 → 1.8 V (valor estándar del T-Deck Pro)
     *   delay = 320 ticks × 15.625 μs = 5.0 ms (tiempo de estabilización)
     *
     * Nota de orden: debe llamarse ANTES de Calibrate(0x7F) para que las
     * calibraciones de PLL e imagen corran con el TCXO activo.
     *
     * 2 testigos:
     *   Éx 27:20 — "lehaʻalót nér tamíd" — encender la lámpara perpetua.
     *              Sin TCXO encendido, la lámpara no puede arder.
     *   Gn 1:3   — "vayyómer Elohím yehí ʼor" — primero hay que encender
     *              la fuente de luz (TCXO) antes de que haya luz (RF).
     */
    {
        /* 0x97: SetDio3AsTcxoCtrl
         *   params[0] = tcxo_voltage: 0x02 = 1.8V
         *   params[1..3] = delay: 0x000140 = 320 ticks × 15.625μs = 5ms */
        const uint8_t tcxo_params[4] = {0x02u, 0x00u, 0x01u, 0x40u};
        int tcxo_rc = sx1262_escribir_comando(0x97u, tcxo_params, 4u);
        if (tcxo_rc != HASHMAL_COMM_OK) {
            MENORA_LOG_W("boot: SetDio3AsTcxoCtrl falló (rc=%d) — "
                         "TCXO no encendido, XOSC_ERR persistirá (Gn 1:3)", tcxo_rc);
        } else {
            MENORA_LOG_I("boot: DIO3 = TCXO ctrl 1.8V/5ms — XOSC alimentado "
                         "(Gn 1:3 yehí ʼor / Éx 27:20 tamíd)");
        }
        /* Esperar que el TCXO estabilice antes de continuar */
        vTaskDelay(pdMS_TO_TICKS(10u));
    }

    /* -- 6b2) Calibrate(0x7F) — re-calibrar TODO con TCXO activo ----------
     *
     * Tras el reset, el SX1262 calibra RC13M y ADC automáticamente,
     * pero sin TCXO (porque DIO3 estaba LOW). Todos esos resultados
     * son incorrectos. Ahora que el TCXO está encendido, re-calibramos
     * todo: RC64k, RC13M, PLL, ADC (×3), imagen RF.
     *
     * Comando 0x89: Calibrate
     *   param = calibration bitmap 0x7F:
     *     bit 0: RC64k_CAL
     *     bit 1: RC13M_CAL
     *     bit 2: PLL_CAL      ← crítico: PLL necesita TCXO como referencia
     *     bit 3: ADC_PULSE_CAL
     *     bit 4: ADC_BULK_N_CAL
     *     bit 5: ADC_BULK_P_CAL
     *     bit 6: IMAGE_CAL    ← crítico: imagen de RF para la banda
     *
     * Durante la calibración BUSY permanece HIGH (~3-5 ms).
     * esperar_busy() en la siguiente spi_transferir lo maneja.
     *
     * 2 testigos:
     *   Lv 8:10-12 — "vaYimshaj et hammishkán... vaYimshaj Aharón":
     *                Moshé ungió CADA cosa (calibrar cada bloque).
     *   Éx 30:26-28 — "vet kol keláv" — ungir TODOS los utensilios.
     *                  0x7F = TODOS los bloques sin excepción.
     */
    {
        const uint8_t cal_all[1] = {0x7Fu};   /* todos los bloques */
        int cal_rc = sx1262_escribir_comando(0x89u, cal_all, 1u);
        if (cal_rc != HASHMAL_COMM_OK) {
            MENORA_LOG_W("boot: Calibrate(0x7F) falló (rc=%d) — "
                         "Lv 8:10 mishJáh incompleto", cal_rc);
        } else {
            MENORA_LOG_I("boot: Calibrate(0x7F) enviado — calibrando RC+PLL+ADC+IMG "
                         "con TCXO activo (Lv 8:10 vaYimshaj kol keláv)");
        }
        /* La calibración tarda ~3-5 ms con BUSY=HIGH.
         * Damos 15 ms extra de margen — la siguiente spi_transferir
         * pasará por esperar_busy() de todas formas. */
        vTaskDelay(pdMS_TO_TICKS(15u));
    }

    /* -- 6c) SetRegulatorMode — DC-DC + LDO ------------------------------
     *
     * SX1262 default tras reset: LDO only (0x00). El T-Deck Pro tiene
     * circuito DC-DC externo (inductor + capacitor). Activar DC-DC + LDO
     * (0x01) da mayor eficiencia y estabilidad de tensión durante TX.
     *
     * Comando 0x96, param 0x01 = DC-DC + LDO (sección 13.1.13 del DS).
     *
     * 2 testigos:
     *   Éx 27:20 — "shémen zayt záj katít lammáor" — aceite puro PRENSADO
     *              (katít = prensado en frío, alta energía — DC-DC es más
     *              eficiente que LDO, entrega más corriente al PA)
     *   Éx 25:39 — "kikkar zaháv tahor" — un TALENTO de oro puro: la energía
     *              completa para la Menorá. Sin DC-DC el talento llega corto.
     */
    {
        const uint8_t reg_mode[1] = {0x01u};   /* 0x00=LDO, 0x01=DC-DC+LDO */
        int reg_rc = sx1262_escribir_comando(0x96u, reg_mode, 1u);
        if (reg_rc != HASHMAL_COMM_OK) {
            MENORA_LOG_W("boot: SetRegulatorMode DC-DC falló (rc=%d) — "
                         "Éx 27:20 shémen katít", reg_rc);
        } else {
            MENORA_LOG_I("boot: RegulatorMode = DC-DC+LDO (0x01) — "
                         "Éx 27:20 katít lammáor");
        }
    }

    /* -- 6e) SetDio2AsRfSwitchCtrl — DIO2 controla el RF switch ----------
     *
     * El módulo SX1262 del T-Deck Pro usa DIO2 para conmutar el RF switch
     * entre la ruta de TX (PA → antena) y la ruta de RX (antena → LNA).
     * Por defecto tras reset, DIO2 es GPIO libre (0x00). Sin activarlo
     * como RF switch (0x01), el PA nunca se conecta a la antena y el chip
     * rechaza SET_TX con CommandStatus=5 porque el camino RF no está armado.
     *
     * Esto explica el comportamiento observado: TODOS los intentos de TX
     * (incluso desde STBY_RC limpio) retornaban CommandStatus=5, IRQ=0x0000,
     * DIO1=0 — el chip procesaba SET_TX pero no había RF real al aire.
     *
     * Comando 0x9D, param 0x01 = enable (sección 13.3.6 del DS).
     *
     * 2 testigos:
     *   Éx 26:33 — "venatattá et happarójet tájat haqqerashím... vehivdíl
     *               haparóket lakem" — el velo SEPARA (hivdíl) TX de RX.
     *               DIO2 es el velo electrónico: conmuta cuál camino es activo.
     *   Nm 10:2  — "veháyu leká lemiqráh háʻedáh velemaséʻ haJamaním":
     *              las trompetas abren el camino. Sin DIO2 armado, la trompeta
     *              (TX) no tiene por dónde salir.
     */
    {
        const uint8_t rf_sw[1] = {0x01u};   /* 0x00=GPIO libre, 0x01=RF switch */
        int rf_rc = sx1262_escribir_comando(0x9Du, rf_sw, 1u);
        if (rf_rc != HASHMAL_COMM_OK) {
            MENORA_LOG_W("boot: SetDio2AsRfSwitchCtrl falló (rc=%d) — "
                         "Éx 26:33 hivdíl TX/RX", rf_rc);
        } else {
            MENORA_LOG_I("boot: DIO2 = RF switch ACTIVO — PA→antena habilitado "
                         "(Éx 26:33 hivdíl / Nm 10:2 trompetas)");
        }
    }

    /* -- 6f) CalibrateImage — calibración adicional para la banda 868 MHz -
     *
     * Tras reset, el SX1262 realiza calibración RC13M y ADC automáticamente,
     * pero NO calibra la imagen de RF (image rejection) para la banda de
     * frecuencia del usuario. Sin CalibrateImage, el chip puede rechazar TX
     * con error IMG_CALIB_ERR, o tener rechazo de imagen deficiente en RX.
     *
     * Para EU 868 MHz (banda 850–930 MHz):
     *   freq1 = 0xD7 (datasheet Table 9-2, 850–930 MHz lower bound)
     *   freq2 = 0xDB (datasheet Table 9-2, 850–930 MHz upper bound)
     *
     * Comando 0x98, params [freq1][freq2].
     * Mientras calibra, BUSY permanece HIGH (~1.5ms) → esperar_busy lo maneja.
     *
     * 2 testigos:
     *   Lv 8:10-11 — "vaYiqáj Moshé et shémen hammishJáh vaYimshaj
     *                  et hammishkán..." — ungir CADA pieza antes de usarla.
     *                  CalibrateImage es la unción del receptor de RF.
     *   Éx 30:26-28 — "vemashJáh otó veʼet kol keláv": unge TODOS los
     *                  utensilios. La calibración es la unción del canal RF.
     */
    {
        const uint8_t cal_img[2] = {0xD7u, 0xDBu};   /* 850–930 MHz → EU 868 */
        int cal_rc = sx1262_escribir_comando(0x98u, cal_img, 2u);
        if (cal_rc != HASHMAL_COMM_OK) {
            MENORA_LOG_W("boot: CalibrateImage falló (rc=%d) — "
                         "Lv 8:10 unción imagen RF", cal_rc);
        } else {
            MENORA_LOG_I("boot: CalibrateImage 850-930MHz (0xD7,0xDB) enviado — "
                         "esperando BUSY tras calibración (Lv 8:10 mishJáh)");
            /* BUSY sube durante la calibración (~1.5 ms).
             * La siguiente transacción SPI pasará por esperar_busy(). */
        }
    }

    /* -- 7) SetDioIrqParams — habilitar IRQ mask ANTES de limpiar.
     *
     * SX1262 tras reset: irqMask=0x0000 (TODOS los eventos enmascarados).
     * Con irqMask=0x0000, GetIrqStatus puede retornar valores indefinidos
     * o siempre 0x0000, haciendo IMPOSIBLE detectar RxDone.
     *
     * Habilitamos los 4 eventos que necesitamos:
     *   Bit 0: TxDone   (0x0001) — confirmar envíos
     *   Bit 1: RxDone   (0x0002) — paquete recibido ← CRÍTICO
     *   Bit 6: CrcErr   (0x0040) — paquete con error
     *   Bit 9: Timeout  (0x0200) — RX timeout (no aplica con RX continuo, por seguridad)
     *
     * irqMask = 0x0243. DIO pins: no los usamos (polling), todos 0.
     *
     * SetDioIrqParams CMD 0x08: [irqM_H][irqM_L][d1M_H][d1M_L][d2M_H][d2M_L][d3M_H][d3M_L]
     *
     * 2 testigos:
     *   Nm 8:3 — "vayya'as ken Aharon... he'ela neroteiha": Aaron hizo
     *            EXACTAMENTE como el Padre mandó. Primero CONFIGURAR,
     *            luego encender — no al revés.
     *   Éx 25:37 — "vehe'ela et neroteiha": las lámparas se orientan
     *              hacia el frente — la máscara IRQ orienta qué eventos
     *              son visibles hacia el sistema.
     */
    {
        /* irqMask=0x0243, todos los DIO masks=0x0000 */
        const uint8_t dio_params[8] = {
            0x02u, 0x43u,   /* irqMask  = 0x0243 */
            0x00u, 0x03u,   /* dio1Mask = 0x0003 (TxDone+RxDone → DIO1, por si se usa) */
            0x00u, 0x00u,   /* dio2Mask = 0x0000 */
            0x00u, 0x00u,   /* dio3Mask = 0x0000 */
        };
        int dio_rc = sx1262_escribir_comando(SX1262_CMD_SET_DIO_IRQ_PARAMS,
                                             dio_params, sizeof(dio_params));
        if (dio_rc != HASHMAL_COMM_OK) {
            MENORA_LOG_W("boot: SET_DIO_IRQ_PARAMS falló (rc=%d) — Nm 8:3", dio_rc);
        } else {
            MENORA_LOG_I("boot: IRQ mask=0x0243 configurada — Nm 8:3 kenAsher");
        }
    }

    /* -- 8) ClearIrqStatus(0xFFFF) en boot — limpiar flags residuales.
     *
     * Después de configurar la máscara, limpiar cualquier flag sucio
     * del power-on o del heʻelá TX de boot.
     *
     * 2 testigos:
     *   Lv 16:19 — "lo purificara y lo santificara de las impurezas"
     *   Éx 29:36 — "purificaras el altar al hacer expiacion por el"
     */
    {
        const uint8_t clr_boot[2] = {0xFFu, 0xFFu};
        int clr_rc = sx1262_escribir_comando(SX1262_CMD_CLR_IRQ_STATUS,
                                             clr_boot, sizeof(clr_boot));
        if (clr_rc != HASHMAL_COMM_OK) {
            MENORA_LOG_W("boot: CLR_IRQ_STATUS falló (rc=%d) — Lv 16:19", clr_rc);
        } else {
            MENORA_LOG_I("boot: IRQ flags limpiados — Lv 16:19 tabóhar");
        }
    }

    /* -- SetBufferBaseAddress: TX=0x00, RX=0x80 — sin overlap en FIFO 256 bytes.
     *
     * Sin esta llamada TX y RX comparten 0x00 → el paquete recibido
     * sobreescribe el buffer de transmisión.
     *
     * 2 testigos:
     *   Éx 25:11 — "zér zaháv sabíb" — corona (límite) de oro alrededor
     *   Éx 26:33 — "vehivdíl" — el velo SEPARARA (TX de RX)
     */
    {
        const uint8_t buf_base[2] = {0x00u, 0x80u};   /* TX=0x00, RX=0x80 */
        int buf_rc = sx1262_escribir_comando(SX1262_CMD_SET_BUFFER_BASE,
                                             buf_base, sizeof(buf_base));
        if (buf_rc != HASHMAL_COMM_OK) {
            MENORA_LOG_W("boot: SET_BUFFER_BASE falló (rc=%d) — Éx 26:33", buf_rc);
        } else {
            MENORA_LOG_I("boot: buffer TX=0x00 RX=0x80 separados — Éx 26:33 vehivdíl");
        }
    }

    /* -- GetDeviceErrors — diagnóstico de errores de calibración ---------
     *
     * Leer y limpiar el registro de errores del SX1262. Si hay bits activos
     * (RC64k, RC13M, PLL, ADC, IMG, XOSC, PA_RAMP) indican que alguna
     * calibración anterior falló y el chip no puede transmitir correctamente.
     *
     * GetDeviceErrors: CMD 0x17 → [status][errH][errL]
     *   Bit 0: RC64K_CALIB_ERR
     *   Bit 1: RC13M_CALIB_ERR
     *   Bit 2: PLL_CALIB_ERR
     *   Bit 3: ADC_CALIB_ERR
     *   Bit 4: IMG_CALIB_ERR  ← crítico para RX/TX
     *   Bit 5: XOSC_START_ERR
     *   Bit 6: PLL_LOCK_ERR   ← crítico para TX
     *   Bit 8: PA_RAMP_ERR    ← crítico para TX potencia
     *
     * ClearDeviceErrors: CMD 0x07 — sin parámetros.
     *
     * 2 testigos:
     *   Éx 39:43 — "vaYáre Moshé et kol hammelakháh" — Moshé VIO toda la
     *              obra. GetDeviceErrors = inspección final antes de operar.
     *   Nm 8:4   — "veze maʻaséh hammenorah" — "Esta es la obra de la
     *              Menorá" — el log confirma que la obra es correcta.
     */
    {
        uint8_t dev_err[2] = {0u, 0u};
        int err_rc = sx1262_leer_comando(0x17u, dev_err, 2u);
        if (err_rc == HASHMAL_COMM_OK) {
            uint16_t err_flags = (uint16_t)(((uint16_t)dev_err[0] << 8) | dev_err[1]);
            if (err_flags != 0u) {
                MENORA_LOG_W("boot: GetDeviceErrors = 0x%04X — "
                             "errores de calibración detectados:"
                             "%s%s%s%s%s%s%s%s",
                             (unsigned)err_flags,
                             (err_flags & 0x0001u) ? " RC64K_ERR"    : "",
                             (err_flags & 0x0002u) ? " RC13M_ERR"    : "",
                             (err_flags & 0x0004u) ? " PLL_CALIB_ERR": "",
                             (err_flags & 0x0008u) ? " ADC_ERR"      : "",
                             (err_flags & 0x0010u) ? " IMG_CALIB_ERR": "",
                             (err_flags & 0x0020u) ? " XOSC_ERR"     : "",
                             (err_flags & 0x0040u) ? " PLL_LOCK_ERR" : "",
                             (err_flags & 0x0100u) ? " PA_RAMP_ERR"  : "");
            } else {
                MENORA_LOG_I("boot: GetDeviceErrors = 0x0000 — "
                             "sin errores de hw (Éx 39:43 vaYáre Moshé)");
            }
            /* ClearDeviceErrors (CMD 0x07) — limpiar después de leer */
            (void)sx1262_escribir_comando(0x07u, NULL, 0u);
        } else {
            MENORA_LOG_W("boot: GetDeviceErrors no responde (rc=%d)", err_rc);
        }
    }

    MENORA_LOG_I("═══ yaréj levantado (Éx 25:31 + Éx 31:4 bronce) ═══");
    MENORA_LOG_I("  SPI @ %u Hz (MOSI=%d, MISO=%d, SCLK=%d, CS=%d)",
                 (unsigned)MENORA_SPI_CLOCK_HZ,
                 MENORA_PIN_MOSI, MENORA_PIN_MISO,
                 MENORA_PIN_SCLK, MENORA_PIN_CS);
    MENORA_LOG_I("  SX1262: RESET=%d, BUSY=%d, DIO1=%d — LoRa activo, standby RC",
                 MENORA_PIN_RESET, MENORA_PIN_BUSY, MENORA_PIN_DIO1);
    MENORA_LOG_I("  CRÍTICO: SetRegulatorMode=DC-DC, SetDio2RfSwitch=ON, "
                 "CalibrateImage=868MHz (Nm 8:4 veze maʻaséh hammenorah)");
    return HASHMAL_COMM_OK;

err_dev:
    (void)spi_bus_remove_device(g_spi_handle);
    g_spi_handle = NULL;
    g_spi_activo = false;
err_bus:
    /* NO llamamos spi_bus_free — el bus es de la Estructura (Éx 26:6 ejád) */
    return HASHMAL_COMM_ERR_SPI;
}

/* ==================================================================
 * API PUBLICA - spi_deinit (Nm 4:9-10 cubrir)
 * ================================================================== */

/**
 * hashmal_comm_spi_deinit - cubre el yarej
 *
 * Nm 4:9-10 - tejelet sobre la Menora al migrar. Aqui: llevar el
 * chip a sleep / standby, quitar dispositivo, liberar bus. Los
 * pines quedan en modo GPIO default (no se re-configuran; seguiran
 * manejados por el siguiente init si se rearma la Menora).
 */
int hashmal_comm_spi_deinit(void)
{
    if (!g_spi_activo) {
        MENORA_LOG_W("spi_deinit: yaréj no estaba activo (no-op)");
        return HASHMAL_COMM_OK;
    }

    /* Llevar el chip a standby RC antes de quitar el bus.
     * Si falla, seguimos limpiando - no queremos fugas de handle. */
    (void)sx1262_set_standby(SX1262_STANDBY_RC);

    esp_err_t err = spi_bus_remove_device(g_spi_handle);
    if (err != ESP_OK) {
        MENORA_LOG_W("spi_bus_remove_device: 0x%x", (unsigned)err);
    }
    g_spi_handle = NULL;

    /* NO llamamos spi_bus_free — bus compartido (Éx 26:6 ejád) */

    /* Apagar módulo LoRa — EN LOW */
    gpio_set_level(MENORA_PIN_EN, 0);

    g_spi_activo = false;
    MENORA_LOG_I("yaréj cubierto (Nm 4:9-10) — device removido, EN LOW");
    return HASHMAL_COMM_OK;
}

/* ==================================================================
 * API PUBLICA - spi_enviar (qanah - por el tronco sale la luz)
 * ================================================================== */

/**
 * hashmal_comm_spi_enviar - escribir buffer TX y disparar TX.
 *
 * Flujo:
 *   1) WRITE_BUFFER @ offset 0 - copia el payload al FIFO del chip.
 *   2) SET_TX con timeout 0 (stay in TX until packet sent).
 *
 * No bloquea esperando fin de transmision (puede durar segundos
 * para SF altos). El caller monitoriza via DIO1/IRQ o polling de
 * GET_IRQ_STATUS (responsabilidad de menora_canales.c).
 *
 * Precondicion: frecuencia, SF, BW, coding rate y packet params
 * deben estar ya configurados por el caller. El driver BASE no
 * los infiere - Ex 25:40, betavnitam: los parametros vienen del
 * monte, no de aqui.
 *
 * 2 testigos:
 *   Ex 25:37 - "veheir 'al 'ever paneiha" (la luz sale hacia afuera)
 *   Zac 4:12 - tubos que VIERTEN (meriqim): el flujo es activo
 */
int hashmal_comm_spi_enviar(const uint8_t *datos, size_t tam)
{
    if (datos == NULL) return HASHMAL_COMM_ERR_NULL;
    if (tam == 0u || tam > HASHMAL_MENSAJE_MAX_BYTES) {
        return HASHMAL_COMM_ERR_TAMANO;
    }
    if (!g_spi_activo) return HASHMAL_COMM_ERR_INIT;

    /* ----------------------------------------------------------------
     * PASO 0: Salir de RX antes de transmitir.
     *
     * El SX1262 rechaza SET_PACKET_PARAMS y SET_TX cuando está en
     * modo RX (continuous o single). La tabla de comandos permitidos
     * (DS.SX1261-2 §13.8, Table 13-69) no los lista para modo RX.
     * Sin este set_standby, TODA transmisión produce CommandStatus=5
     * ("failure to execute command"), BUSY=0 inmediato (no hay TX
     * real), e IRQ=0x0000 (TxDone nunca se genera).
     *
     * behétivó et hannerót (Éx 30:7): primero arregla las lámparas
     * (standby), LUEGO enciende el incienso (TX). El orden lo pone
     * el Padre, no nosotros.
     *
     * 2 testigos:
     *   Éx 30:7  — behétivó → hetiváh → "cuando arregle las lámparas"
     *              (preparar primero, luego transmitir)
     *   Nm  8:3  — vayya'as ken Aharon: hizo EXACTAMENTE así, en orden
     * ---------------------------------------------------------------- */
    {
        int rc_stby = sx1262_set_standby(SX1262_STANDBY_RC);
        if (rc_stby != HASHMAL_COMM_OK) {
            MENORA_LOG_W("spi_enviar: set_standby pre-TX rc=%d — "
                         "chip puede estar en estado inesperado (Éx 30:7)",
                         rc_stby);
            /* No abortamos: intentamos TX igualmente. Si el chip no
             * acepta STANDBY, el SET_TX fallará y lo registraremos. */
        }
    }

    /* WRITE_BUFFER: [cmd=0x0E][offset=0x00][data...]
     * Construimos el paquete SPI en un buffer local. */
    if ((tam + 2u) > 256u) {
        /* max_transfer_sz del bus es 256 - el header anade 2 bytes. */
        return HASHMAL_COMM_ERR_TAMANO;
    }
    uint8_t tx[2u + HASHMAL_MENSAJE_MAX_BYTES];
    tx[0] = SX1262_CMD_WRITE_BUFFER;
    tx[1] = 0x00u;                    /* offset 0 en el FIFO */
    memcpy(&tx[2], datos, tam);

    int rc = spi_transferir(tx, NULL, 2u + tam);
    if (rc != HASHMAL_COMM_OK) {
        MENORA_LOG_E("spi_enviar: WRITE_BUFFER falló (rc=%d)", rc);
        return rc;
    }

    /* SET_PACKET_PARAMS pre-TX — re-emitir con payload_len = tam REAL.
     *
     * Éx 25:40 "betavnitám" — cada pieza exactamente según el patrón.
     * Si configurar_paquete fue llamado con un tamaño distinto (p.ej.
     * el mensaje heʻelá de arranque tiene 19 bytes pero la tekiʻáh
     * tiene 11), el chip tiene cacheado el valor anterior y transmitiría
     * bytes de más del FIFO (basura). Re-emitir aquí garantiza que el
     * OTA header embebido en explicit-header-mode diga exactamente
     * cuántos bytes contiene el paquete → el receptor acepta el frame.
     *
     * 2 testigos:
     *   Éx 25:40 betavnitám — según el patrón EXACTO, ni más ni menos
     *   Éx 26:6  veyhayáh haJoshkán ejád — unidad perfecta: TX y RX
     *            deben hablar del MISMO tamaño */
    {
        const uint8_t pkt_params[6] = {
            (uint8_t)((s_pkt_preambulo >> 8) & 0xFFu),
            (uint8_t)( s_pkt_preambulo       & 0xFFu),
            s_pkt_header_tipo,
            (uint8_t)tam,      /* payload_len = tamaño REAL en FIFO */
            s_pkt_crc_on,
            s_pkt_iq_invert,
        };
        int pkt_rc = sx1262_escribir_comando(SX1262_CMD_SET_PACKET_PARAMS,
                                             pkt_params, sizeof(pkt_params));
        if (pkt_rc != HASHMAL_COMM_OK) {
            MENORA_LOG_W("spi_enviar: SET_PACKET_PARAMS pre-TX falló (rc=%d) — "
                         "betavnitám comprometido (Éx 25:40)", pkt_rc);
            /* No abortamos: SET_TX puede aún funcionar; el fallo
             * es registrado para que el operador lo detecte. */
        }
    }

    /* SET_TX con timeout=0 (sin timeout - "stay until packet sent").
     * Parametro: 24-bit big-endian. 0x000000 = sin timeout.
     * Tras emitir, el chip transmite el paquete y vuelve a STANDBY_RC
     * automáticamente cuando termina (BUSY baja, DIO1/TxDone sube). */
    const uint8_t tx_params[3] = { 0x00u, 0x00u, 0x00u };
    rc = sx1262_escribir_comando(SX1262_CMD_SET_TX, tx_params, 3u);
    if (rc != HASHMAL_COMM_OK) {
        MENORA_LOG_E("spi_enviar: SET_TX falló (rc=%d)", rc);
        return rc;
    }

    /* ----------------------------------------------------------------
     * ESPERAR TxDone vía polling GET_IRQ_STATUS.
     *
     * BUSY baja tan pronto como el chip acepta SET_TX (la transacción
     * SPI termina), NO cuando la transmisión RF acaba. Chequear BUSY
     * post-SET_TX es insuficiente: siempre veremos BUSY=0 aunque TX
     * nunca haya empezado, porque BUSY solo indica "comando procesado",
     * no "transmisión RF completada".
     *
     * TxDone (IRQ bit 0) es el indicador real: se activa solo cuando
     * el SX1262 ha terminado de emitir el último símbolo al aire.
     *
     * Tiempos de TX esperados (SF7):
     *   BW=62.5kHz (0x06): ~25ms para 11 bytes
     *   BW=125kHz  (0x07): ~13ms para 11 bytes
     *   BW=500kHz  (0x09):  ~5ms para 11 bytes
     * Timeout 2000ms es conservador para cualquier configuración.
     *
     * Secuencia:
     *   1) Si TxDone en tiempo → limpiar IRQ, continuar a SET_RX
     *   2) Si timeout → log de error, salir (el chip puede estar en TX)
     *
     * Si el chip estaba en RX cuando llegamos aquí (set_standby falló
     * en el Paso 0), GET_IRQ_STATUS devolverá CommandStatus=5 en cada
     * poll, y TxDone nunca llegará. El timeout de 2000ms lo detectará.
     *
     * 2 testigos:
     *   Éx 39:43 — "vaYáre Moshe et kol hamelakha" — VER toda la obra
     *   Nm  7:89 — "veshamáʻ et haqqól middabbér eláv" — escuchar la
     *              confirmación (TxDone = la voz se declara terminada)
     * ---------------------------------------------------------------- */
    {
        const int64_t _txd_t0    = esp_timer_get_time();
        bool          _txd_ok    = false;
        uint16_t      _txd_flags = 0u;
        uint8_t       _txd_stat  = 0u;
        int           _txd_polls = 0;

        while ((esp_timer_get_time() - _txd_t0) < 2000000LL) {  /* 2 s */
            uint8_t _itx[5] = {SX1262_CMD_GET_IRQ_STATUS, 0u, 0u, 0u, 0u};
            uint8_t _irx[5] = {0u, 0u, 0u, 0u, 0u};
            if (spi_transferir(_itx, _irx, 5u) == HASHMAL_COMM_OK) {
                _txd_stat  = _irx[1];   /* status byte */
                _txd_flags = (uint16_t)(((uint16_t)_irx[2] << 8) | _irx[3]);
                if (_txd_flags & 0x0001u) {   /* bit 0 = TxDone */
                    _txd_ok = true;
                    break;
                }
            }
            ++_txd_polls;
            vTaskDelay(pdMS_TO_TICKS(2));
        }

        int64_t _txd_ms = (esp_timer_get_time() - _txd_t0) / 1000LL;

        if (_txd_ok) {
            MENORA_LOG_I("spi_enviar: TxDone [%lldms] polls=%d "
                         "IRQ=0x%04X status=0x%02X DIO1=%d — "
                         "veheír ʻal ʻéver panéiha (Éx 25:37)",
                         (long long)_txd_ms, _txd_polls,
                         (unsigned)_txd_flags, (unsigned)_txd_stat,
                         gpio_get_level(MENORA_PIN_DIO1));
            /* Limpiar TxDone antes de SET_RX — Lv 16:19 tabóhar */
            const uint8_t _clr_tx[2] = {0xFFu, 0xFFu};
            (void)sx1262_escribir_comando(SX1262_CMD_CLR_IRQ_STATUS,
                                          _clr_tx, 2u);
        } else {
            /* Decodificar status para diagnóstico */
            uint8_t _chip_mode = (_txd_stat >> 4) & 0x07u;
            uint8_t _cmd_stat  = (_txd_stat >> 1) & 0x07u;
            MENORA_LOG_W("spi_enviar: TxDone TIMEOUT [%lldms] polls=%d "
                         "IRQ=0x%04X status=0x%02X "
                         "(ChipMode=0x%X CommandStatus=0x%X) "
                         "BUSY=%d DIO1=%d — TX no completó RF (Éx 39:43)",
                         (long long)_txd_ms, _txd_polls,
                         (unsigned)_txd_flags, (unsigned)_txd_stat,
                         (unsigned)_chip_mode, (unsigned)_cmd_stat,
                         gpio_get_level(MENORA_PIN_BUSY),
                         gpio_get_level(MENORA_PIN_DIO1));
            /* No retornamos error — SET_RX sigue para restaurar escucha */
        }
    }

    /* Nm 8:2 — "el mul penei hammenora yaʼiru" — volver a escucha.
     *
     * Llegamos aquí habiendo esperado TxDone (o timeout). El chip está
     * en STANDBY_RC (TX completado, auto-retorno). Podemos emitir
     * SET_RX desde STANDBY sin restricciones.
     *
     * 0xFFFFFF = RX continuo (tamíd, Éx 27:21 juqqát ʻolám).
     * La Menorá nunca deja de escuchar entre transmisiones.
     *
     * 2 testigos:
     *   Nm  8:2  — las lámparas iluminan hacia el tronco (oír = luz entrante)
     *   Éx 27:21 — nér tamíd — lámpara perpetua, sin apagar */
    rc = sx1262_set_rx(0xFFFFFFu);
    if (rc != HASHMAL_COMM_OK) {
        MENORA_LOG_W("spi_enviar: SET_RX post-TX falló (rc=%d) — "
                     "recepción comprometida (Nm 8:2)", rc);
        /* TX fue exitoso — no propagamos el error al caller.
         * El listen_task detectará ausencia de paquetes y actuará. */
    }

    return HASHMAL_COMM_OK;
}

/* ==================================================================
 * API PUBLICA - spi_recibir (qanah - la luz entra al tronco)
 * ================================================================== */

/**
 * hashmal_comm_spi_recibir - lee del buffer RX del SX1262.
 *
 * Flujo:
 *   1) GET_RX_BUFFER_STATUS -> [status][payload_len][rx_start_ptr]
 *   2) READ_BUFFER @ rx_start_ptr, len = payload_len
 *
 * No configura RX mode. Asume que el caller (canales.c) puso el
 * chip en RX y que un paquete ya llego (DIO1 o polling).
 *
 * Nm 8:2 - "el mul penei hammenora ya'iru shiv'at hannerot":
 * las lamparas iluminan hacia el centro. Aqui: el paquete entra
 * al tronco desde una rama externa, viniendo hacia el centro.
 *
 * 2 testigos:
 *   Nm 8:2   - las lamparas iluminan hacia el tronco
 *   Zac 4:12 - los tubos vierten bidireccional (RX y TX en la misma base)
 */
int hashmal_comm_spi_recibir(uint8_t *buf, size_t tam, size_t *recibidos)
{
    if (buf == NULL || recibidos == NULL) return HASHMAL_COMM_ERR_NULL;
    *recibidos = 0u;

    if (!g_spi_activo) return HASHMAL_COMM_ERR_INIT;

    /* ----------------------------------------------------------------
     * PASO 0: DIO1 — indicador hardware de evento IRQ.
     *
     * En SetDioIrqParams configuramos dio1Mask=0x0003 (TxDone|RxDone).
     * DIO1 sube a HIGH cuando ocurre cualquiera de esos eventos.
     * Este es un GPIO poll — sin SPI, sin byte-offset, sin ambigüedad.
     *
     * Si DIO1 está LOW → no hay evento → retorno inmediato.
     * Si DIO1 está HIGH → hay evento (RxDone o TxDone) → proseguir.
     *
     * 2 testigos:
     *   Nm 10:2  — "hazlót" (trompetas): señal EXTERNA al sistema que
     *              indica evento — equivale al pin DIO1 del chip.
     *   Éx 28:35 — el cascabel del efod se oye al entrar y al salir:
     *              señal física de presencia, no necesita pregunta SPI.
     * ---------------------------------------------------------------- */
    static uint32_t s_poll_cnt   = 0u;
    static bool     s_tabohar    = false;   /* Lv 16:19 — purificar ANTES del primer ciclo */
    ++s_poll_cnt;

    /* ----------------------------------------------------------------
     * TABÓHAR — primer llamado: purificar el FIFO (Lv 16:19)
     *
     * "vehizzáh ʻaláv min haddám beʼetsbaʻó shévaʻ peʻamím
     *  vetihéru veqiddéshó" — Lv 16:19
     * "Rociará sobre él de la sangre siete veces, lo PURIFICARÁ
     *  y lo SANTIFICARÁ."
     *
     * Problema: el SX1262 NO pierde su FIFO al re-flashear el ESP32.
     * Si el nodo recibió un paquete durante la sesión anterior,
     * ese paquete (p.ej. "HASHMAL-CORE shalom" = byte[0]='H'=0x48=72)
     * permanece en el FIFO con DIO1 en HIGH. Al entrar en RX continuo
     * (`spi_init` → `spi_enviar` → SET_RX), el primer poll de DIO1
     * encuentra HIGH y lee datos residuales → "versión 72 desconocida".
     *
     * Fix: en el PRIMER llamado a spi_recibir, antes de leer nada:
     *   1) ClearIrqStatus(0xFFFF) — baja DIO1 e IRQ flags
     *   2) SetBufferBaseAddress(TX=0x00, RX=0x80) — reset puntero FIFO
     *   3) Retornar sin procesar datos (Lv 16:19 — el templo se purifica
     *      ANTES de que el pueblo entre)
     *
     * 2 testigos:
     *   Lv 16:19 — vetihéru: limpiar ANTES de recibir al Padre
     *   Éx 30:19-20 — verajetsu Aharon: lavarse ANTES de ministrar
     * ---------------------------------------------------------------- */
    if (!s_tabohar) {
        /* ClearIrqStatus(0xFFFF) — bajar DIO1 y limpiar todos los flags */
        const uint8_t clr_tab[2] = {0xFFu, 0xFFu};
        (void)sx1262_escribir_comando(SX1262_CMD_CLR_IRQ_STATUS, clr_tab, 2u);
        /* SetBufferBaseAddress(TX=0x00, RX=0x80) — reset puntero de lectura */
        const uint8_t base[2] = {0x00u, 0x80u};
        (void)sx1262_escribir_comando(SX1262_CMD_SET_BUFFER_BASE, base, 2u);
        s_tabohar = true;
        MENORA_LOG_I("spi_recibir[1]: tabóhar — FIFO purgado antes del "
                     "primer ciclo de escucha (Lv 16:19 vetihéru)");
        return HASHMAL_COMM_OK;
    }

    const int dio1 = gpio_get_level(MENORA_PIN_DIO1);

    /* ---------------------------------------------------------------
     * POLLING IRQ STATUS — fallback cuando DIO1=0 y como diagnóstico.
     *
     * Antes, el bus SPI se inicializaba con miso_io_num=-1 (EPD no
     * necesita MISO). Esto causaba que TODOS los reads del SX1262
     * devolvieran 0xFF porque GPIO47 no estaba conectado al periférico.
     *
     * Con el fix (miso_io_num=HASHMAL_PIN_SPI_MISO en hashmal_epd.c),
     * GET_IRQ_STATUS ahora devuelve valores reales.
     *
     * LOGICA:
     *   - Si DIO1=1: hay evento hardware confirmado → continuar
     *   - Si DIO1=0: leer GET_IRQ_STATUS de todas formas
     *     → Si IRQ muestra RxDone (bit1) → procesar paquete (fallback)
     *     → Esto cubre el caso donde DIO1 HW no funciona pero chip sí
     *
     * 2 testigos:
     *   Nm 10:2  — trompeta Y señal visual: dos mecanismos de aviso
     *   Éx 39:43 — Moshé VIO toda la obra: el log nos muestra todo
     * --------------------------------------------------------------- */
    {
        /* Leer GET_IRQ_STATUS en CADA ciclo (no solo periódico).
         * Formato: TX=[0x12][NOP][NOP][NOP][NOP]
         *          RX=[X][status][irq_H][irq_L] */
        uint8_t irq_tx[5] = {SX1262_CMD_GET_IRQ_STATUS, 0u, 0u, 0u, 0u};
        uint8_t irq_rx[5] = {0u, 0u, 0u, 0u, 0u};
        int irq_rc = spi_transferir(irq_tx, irq_rx, 5u);

        uint16_t irq_flags = 0u;
        if (irq_rc == HASHMAL_COMM_OK) {
            /* rx[1]=status, rx[2]=irq_H, rx[3]=irq_L */
            irq_flags = (uint16_t)(((uint16_t)irq_rx[2] << 8) | irq_rx[3]);
        }

        /* Log periódico cada 20 ciclos (~1s) */
        if ((s_poll_cnt % 20u) == 0u) {
            MENORA_LOG_I("spi_recibir[%lu]: DIO1=%d IRQ=0x%04X "
                         "[status=0x%02X irqH=0x%02X irqL=0x%02X] "
                         "(TxDone=%d RxDone=%d CrcErr=%d Timeout=%d)",
                         (unsigned long)s_poll_cnt, dio1, (unsigned)irq_flags,
                         irq_rx[1], irq_rx[2], irq_rx[3],
                         (irq_flags & 0x0001u) ? 1 : 0,   /* bit0 TxDone */
                         (irq_flags & 0x0002u) ? 1 : 0,   /* bit1 RxDone */
                         (irq_flags & 0x0040u) ? 1 : 0,   /* bit6 CrcErr */
                         (irq_flags & 0x0200u) ? 1 : 0);  /* bit9 Timeout */
        }

        /* Fallback: si DIO1=0 pero IRQ muestra RxDone → hay paquete.
         * Esto cubre HW donde DIO1 no está disponible o tiene problema. */
        if (dio1 == 0 && (irq_flags & 0x0002u)) {
            MENORA_LOG_I("spi_recibir[%lu]: DIO1=0 pero IRQ=0x%04X RxDone=1 — "
                         "paquete detectado por polling (Nm 10:2 fallback)",
                         (unsigned long)s_poll_cnt, (unsigned)irq_flags);
            /* Limpiar IRQ ANTES de leer buffer (para no procesar doble) */
            const uint8_t clr_fb[2] = {0xFFu, 0xFFu};
            (void)sx1262_escribir_comando(SX1262_CMD_CLR_IRQ_STATUS, clr_fb, 2u);
            /* Ahora procesamos el paquete directamente, saltando el check de DIO1.
             * El flujo continua debajo igual que si DIO1 hubiera sido HIGH. */
            goto s_leer_buffer;
        }
    }

    if (dio1 == 0) {
        /* DIO1 bajo y sin RxDone en IRQ — no hay evento pendiente */
        return HASHMAL_COMM_OK;
    }

    /* DIO1 alto — hay evento (TxDone o RxDone según dio1Mask=0x0003). */
    MENORA_LOG_I("spi_recibir[%lu]: DIO1=HIGH — evento pendiente (Nm 10:2)",
                 (unsigned long)s_poll_cnt);

    /* ----------------------------------------------------------------
     * PASO 1: DIO1=HIGH → leer GET_RX_BUFFER_STATUS.
     *
     * Si payload_len=0 → fue TxDone (no RxDone). Limpiar y salir.
     * Si payload_len>0 → paquete recibido. Leer buffer.
     *
     * GET_RX_BUFFER_STATUS ya demostró funcionar en este hardware
     * (retornaba {0,0} correctamente en el código anterior).
     * ---------------------------------------------------------------- */
s_leer_buffer:;   /* fallback desde IRQ polling cuando DIO1=0 pero RxDone=1 */
    uint8_t rx_status[2] = {0u, 0u};
    int rc = sx1262_leer_comando(SX1262_CMD_GET_RX_BUFFER_STATUS,
                                 rx_status, sizeof(rx_status));
    if (rc != HASHMAL_COMM_OK) {
        MENORA_LOG_E("spi_recibir: GET_RX_BUFFER_STATUS (rc=%d) — limpiando", rc);
        const uint8_t clr_e[2] = {0xFFu, 0xFFu};
        (void)sx1262_escribir_comando(SX1262_CMD_CLR_IRQ_STATUS, clr_e, 2u);
        return rc;
    }
    const size_t  payload_len = (size_t)rx_status[0];
    const uint8_t rx_offset   = rx_status[1];

    MENORA_LOG_I("spi_recibir: evento RX payload_len=%u rx_offset=%u",
                 (unsigned)payload_len, (unsigned)rx_offset);

    if (payload_len == 0u) {
        /* DIO1 alto por TxDone, no RxDone. Limpiar y salir. */
        MENORA_LOG_I("spi_recibir: evento TxDone (payload_len=0) — limpiando");
        const uint8_t clr_z[2] = {0xFFu, 0xFFu};
        (void)sx1262_escribir_comando(SX1262_CMD_CLR_IRQ_STATUS, clr_z, 2u);
        return HASHMAL_COMM_OK;
    }

    /* Truncar a la capacidad del caller. */
    const size_t n = (payload_len <= tam) ? payload_len : tam;
    if (n == 0u) {
        const uint8_t clr_n[2] = {0xFFu, 0xFFu};
        (void)sx1262_escribir_comando(SX1262_CMD_CLR_IRQ_STATUS, clr_n, 2u);
        return HASHMAL_COMM_OK;
    }

    /* ----------------------------------------------------------------
     * PASO 2: READ_BUFFER: [cmd=0x1E][offset][NOPx(1+n)]
     *
     * Nm 8:2 — la luz entra al tronco desde la rama externa.
     * ---------------------------------------------------------------- */
    const size_t total = 3u + n;
    if (total > 256u) {
        const uint8_t clr_t[2] = {0xFFu, 0xFFu};
        (void)sx1262_escribir_comando(SX1262_CMD_CLR_IRQ_STATUS, clr_t, 2u);
        return HASHMAL_COMM_ERR_TAMANO;
    }

    uint8_t tx_buf[3u + HASHMAL_MENSAJE_MAX_BYTES];
    uint8_t rx_buf[3u + HASHMAL_MENSAJE_MAX_BYTES];
    memset(tx_buf, 0, total);
    tx_buf[0] = SX1262_CMD_READ_BUFFER;
    tx_buf[1] = rx_offset;

    rc = spi_transferir(tx_buf, rx_buf, total);
    if (rc != HASHMAL_COMM_OK) {
        MENORA_LOG_E("spi_recibir: READ_BUFFER (rc=%d)", rc);
        const uint8_t clr_r[2] = {0xFFu, 0xFFu};
        (void)sx1262_escribir_comando(SX1262_CMD_CLR_IRQ_STATUS, clr_r, 2u);
        return rc;
    }

    /* data empieza en rx_buf[3] (cmd + offset + status_byte) */
    memcpy(buf, &rx_buf[3], n);
    *recibidos = n;

    /* ----------------------------------------------------------------
     * PASO 3: ClearIrqStatus(0xFFFF) — limpiar flags y bajar DIO1.
     *
     * Sin CLR, DIO1 permanece HIGH y la próxima llamada re-leerá
     * el mismo paquete. CLR baja DIO1 y libera el chip para el
     * siguiente evento.
     *
     * 2 testigos:
     *   Nm 16:46-48 — reset del estado: Aaron para la plaga limpiando
     *   Lv 16:19    — purificación ritual antes del siguiente ciclo
     * ---------------------------------------------------------------- */
    const uint8_t clr[2] = {0xFFu, 0xFFu};
    int clr_rc = sx1262_escribir_comando(SX1262_CMD_CLR_IRQ_STATUS,
                                         clr, sizeof(clr));
    if (clr_rc != HASHMAL_COMM_OK) {
        MENORA_LOG_W("spi_recibir: CLR_IRQ_STATUS falló (rc=%d) — Nm 16:46", clr_rc);
    }

    MENORA_LOG_I("spi_recibir: %u bytes recibidos, IRQ limpiado — Lv 16:19 tabóhar",
                 (unsigned)n);
    return HASHMAL_COMM_OK;
}

/* ==================================================================
 * API PUBLICA - configurar_frecuencia (v.32 - cada rama a su freq)
 * ================================================================== */

/**
 * hashmal_comm_configurar_frecuencia - SET_RF_FREQUENCY.
 *
 * v.32 - "shisha qanim yotsim": cada rama sale con su propia
 * frecuencia. Aqui calculamos el valor de registro del SX1262:
 *
 *   freq_reg = (freq_hz . 2^25) / 32_000_000
 *
 * (F_xtal = 32 MHz del chip; formula del datasheet, S13.4.1.)
 *
 * Lleva el chip a standby antes de configurar (buena practica: los
 * cambios de RF se hacen en reposo). Actualiza ner[id].frecuencia_hz
 * via kaporet_mut...digo, menora_mut - la Menora refleja la rama.
 */
int hashmal_comm_configurar_frecuencia(hashmal_canal_id_t id, uint32_t freq_hz)
{
    if (!hashmal_comm_canal_legitimo(id)) return HASHMAL_COMM_ERR_CANAL;
    if (!g_spi_activo)                    return HASHMAL_COMM_ERR_INIT;

    /* Rango razonable: evitar 0 o valores absurdos. El chip acepta
     * 150 MHz..960 MHz; dejamos la validacion estricta al caller
     * regional (EU868, US915, etc.). */
    if (freq_hz == 0u) return HASHMAL_COMM_ERR_SPI;

    /* Reposo antes de reconfigurar - SX1262 permite cambio en
     * standby sin efectos colaterales. */
    int rc = sx1262_set_standby(SX1262_STANDBY_RC);
    if (rc != HASHMAL_COMM_OK) return rc;

    /* freq_reg big-endian, 4 bytes. */
    const uint64_t freq_reg64 = ((uint64_t)freq_hz << 25) / 32000000ULL;
    const uint32_t freq_reg   = (uint32_t)freq_reg64;

    const uint8_t params[4] = {
        (uint8_t)((freq_reg >> 24) & 0xFFu),
        (uint8_t)((freq_reg >> 16) & 0xFFu),
        (uint8_t)((freq_reg >>  8) & 0xFFu),
        (uint8_t)( freq_reg        & 0xFFu),
    };
    rc = sx1262_escribir_comando(SX1262_CMD_SET_RF_FREQUENCY,
                                 params, sizeof(params));
    if (rc != HASHMAL_COMM_OK) {
        MENORA_LOG_E("configurar_frecuencia: SET_RF_FREQUENCY (rc=%d)", rc);
        return rc;
    }

    /* Reflejar en la Menora (si esta levantada; si no, el chip queda
     * configurado pero el struct no - tolerable: al init posterior
     * se sincroniza). */
    hashmal_menora_t *m = hashmal_comm_menora_mut();
    if (m != NULL) {
        m->nerot[(size_t)id].frecuencia_hz = freq_hz;
    }

    MENORA_LOG_I("canal %d → freq %" PRIu32 " Hz (reg=0x%08" PRIx32 ")",
                 (int)id, freq_hz, freq_reg);
    return HASHMAL_COMM_OK;
}

/* ==================================================================
 * API PUBLICA - configurar_potencia (Zac 4:6 - no con fuerza)
 * ================================================================== */

/**
 * hashmal_comm_configurar_potencia - SET_TX_PARAMS.
 *
 * SX1262 +HPA: rango [-9, +22] dBm. Fuera de rango -> ERR_SPI.
 * RampTime por defecto 40 ms (equilibrio spurious/velocidad).
 *
 * Zac 4:6 - "lo vejayil velo vejoaj": no con fuerza bruta. La
 * potencia se calibra al minimo que cumpla el enlace, no al maximo.
 * La regulacion regional (EU: 14 dBm tipico) manda.
 */
int hashmal_comm_configurar_potencia(hashmal_canal_id_t id, int8_t dbm)
{
    if (!hashmal_comm_canal_legitimo(id)) return HASHMAL_COMM_ERR_CANAL;
    if (!g_spi_activo)                    return HASHMAL_COMM_ERR_INIT;

    if (dbm < SX1262_POWER_MIN_DBM || dbm > SX1262_POWER_MAX_DBM) {
        MENORA_LOG_E("potencia %d dBm fuera de rango [%d, %d]",
                     (int)dbm, SX1262_POWER_MIN_DBM, SX1262_POWER_MAX_DBM);
        return HASHMAL_COMM_ERR_SPI;
    }

    /* SetPaConfig — OBLIGATORIO antes de SetTxParams (SX1262 Application Note §4.1.3).
     *
     * Sin SetPaConfig el PA (Power Amplifier) no recibe la configuración
     * del circuito interno → el chip completa la secuencia TX digitalmente
     * (TxDone, BUSY=LOW) pero no emite RF real al aire.
     *
     * Parámetros para SX1262 (HP PA) en T-Deck Pro:
     *   paDutyCycle = 0x04  (duty cycle del PA)
     *   hpMax       = 0x07  (corriente máxima HP PA = 22 dBm)
     *   deviceSel   = 0x00  (0x00 = SX1262, 0x01 = SX1261)
     *   paLutRsvd   = 0x01  (siempre 0x01 según datasheet)
     *
     * 2 testigos:
     *   Éx 27:20 — "vetsiváh... shémen zayt záj" — aceite puro REAL
     *              (sin SetPaConfig el aceite es digital, no real)
     *   Éx 25:37 — "veheʻelitá et hannerot" — encenderás las lámparas
     *              (PA config = encender el fuego físico de RF)
     */
    {
        const uint8_t pa_cfg[4] = {0x04u, 0x07u, 0x00u, 0x01u};
        int pa_rc = sx1262_escribir_comando(SX1262_CMD_SET_PA_CONFIG,
                                            pa_cfg, sizeof(pa_cfg));
        if (pa_rc != HASHMAL_COMM_OK) {
            MENORA_LOG_W("configurar_potencia: SET_PA_CONFIG falló (rc=%d) — "
                         "Éx 27:20 aceite no encendido", pa_rc);
            /* No bloqueamos — continuamos con SET_TX_PARAMS */
        } else {
            MENORA_LOG_I("configurar_potencia: PA cfg dutyCycle=0x04 hpMax=0x07 SX1262 — "
                         "Éx 27:20 shémen zayt záj");
        }
    }

    /* SET_TX_PARAMS: [power_dbm_signed][ramp_time]. int8_t -> uint8_t
     * por cast es two's complement (lo que espera el chip). */
    const uint8_t params[2] = {
        (uint8_t)dbm,
        SX1262_TX_RAMP_40US,
    };
    int rc = sx1262_escribir_comando(SX1262_CMD_SET_TX_PARAMS,
                                     params, sizeof(params));
    if (rc != HASHMAL_COMM_OK) {
        MENORA_LOG_E("configurar_potencia: SET_TX_PARAMS (rc=%d)", rc);
        return rc;
    }

    hashmal_menora_t *m = hashmal_comm_menora_mut();
    if (m != NULL) {
        m->nerot[(size_t)id].potencia_dbm = dbm;
    }

    MENORA_LOG_I("canal %d → potencia %d dBm (Zac 4:6 — al mínimo útil)",
                 (int)id, (int)dbm);
    return HASHMAL_COMM_OK;
}

/* ==================================================================
 * API PUBLICA - configurar_modulacion (v.33 geviʻim meshuqqadim)
 *
 * Los geviʻím (copas de almendro) dan FORMA a la señal. Cada copa
 * tiene tres partes: gavia (copa), kaftor (botón), péraj (flor) —
 * tres parámetros entrelazados (SF, BW, CR) que determinan cómo
 * la luz se MODULA antes de salir por la rama.
 *
 * SX1262 SetModulationParams para LoRa: [SF][BW][CR][LDRO]
 *   SF  = Spreading Factor (5-12)
 *   BW  = Bandwidth (0x00=7.8k .. 0x06=125k .. 0x09=500k)
 *   CR  = Coding Rate (0x01=4/5 .. 0x04=4/8)
 *   LDRO = Low Data Rate Optimize (0=off, 1=on)
 *
 * 2 testigos:
 *   Éx 25:33 — "shlosha geviʻim meshuqqadim... kaftor vaféraj"
 *   Jer 1:11-12 — "maqqél shaqéd" — el almendro que VIGILA
 * ================================================================== */
int hashmal_comm_configurar_modulacion(uint8_t sf, uint8_t bw,
                                        uint8_t cr, uint8_t ldro)
{
    if (!g_spi_activo) return HASHMAL_COMM_ERR_INIT;

    /* Validar rango SF. SX1262 soporta 5-12. */
    if (sf < 5u || sf > 12u) {
        MENORA_LOG_E("modulación: SF %u fuera de rango [5,12]", sf);
        return HASHMAL_COMM_ERR_SPI;
    }

    const uint8_t params[4] = { sf, bw, cr, ldro };
    int rc = sx1262_escribir_comando(SX1262_CMD_SET_MODULATION,
                                     params, sizeof(params));
    if (rc != HASHMAL_COMM_OK) {
        MENORA_LOG_E("configurar_modulacion: SET_MODULATION (rc=%d)", rc);
        return rc;
    }

    MENORA_LOG_I("modulación: SF%u BW=%u CR=%u LDRO=%u "
                 "(geviʻím meshuqqadím, Éx 25:33)", sf, bw, cr, ldro);
    return HASHMAL_COMM_OK;
}

/* ==================================================================
 * API PUBLICA - configurar_paquete (v.40 betavnitám — según su patrón)
 *
 * "uréʼeh vaʻaséh BETAVNITÁM ashér attá morʼéh bahár" — Éx 25:40
 * "Mira y haz SEGÚN SU PATRÓN que te es mostrado en el monte."
 *
 * El formato del paquete LoRa tiene estructura prescrita:
 *   preámbulo — la antecámara antes de entrar
 *   header    — explícito (0x00) para que el receptor sepa el largo
 *   longitud  — cuánto payload hay
 *   CRC       — integridad (zér zaháv, Éx 25:11)
 *   IQ        — standard (0x00)
 *
 * SX1262 SetPacketParams para LoRa: 6 bytes
 *   [preamble_hi][preamble_lo][header_type][payload_len][crc][iq_invert]
 *
 * 2 testigos:
 *   Éx 25:40 — betavnitám (según su patrón exacto)
 *   Éx 25:11 — zér zaháv (borde de oro = CRC de integridad)
 * ================================================================== */
int hashmal_comm_configurar_paquete(uint16_t preambulo,
                                     uint8_t  header_tipo,
                                     uint8_t  payload_len,
                                     uint8_t  crc_on,
                                     uint8_t  iq_invert)
{
    if (!g_spi_activo) return HASHMAL_COMM_ERR_INIT;

    const uint8_t params[6] = {
        (uint8_t)((preambulo >> 8) & 0xFFu),
        (uint8_t)( preambulo       & 0xFFu),
        header_tipo,
        payload_len,
        crc_on,
        iq_invert,
    };
    int rc = sx1262_escribir_comando(SX1262_CMD_SET_PACKET_PARAMS,
                                     params, sizeof(params));
    if (rc != HASHMAL_COMM_OK) {
        MENORA_LOG_E("configurar_paquete: SET_PACKET_PARAMS (rc=%d)", rc);
        return rc;
    }

    /* Guardar para que spi_enviar pueda re-emitir con tam real. */
    s_pkt_preambulo   = preambulo;
    s_pkt_header_tipo = header_tipo;
    s_pkt_crc_on      = crc_on;
    s_pkt_iq_invert   = iq_invert;

    MENORA_LOG_I("paquete: preámbulo=%u header=%u len=%u CRC=%u IQ=%u "
                 "(betavnitám, Éx 25:40)",
                 preambulo, header_tipo, payload_len, crc_on, iq_invert);
    return HASHMAL_COMM_OK;
}

/* ==================================================================
 * API PUBLICA - spi_modo_rx (Nm 8:2 — iluminar hacia el tronco)
 * ================================================================== */

/**
 * hashmal_comm_spi_modo_rx - poner el SX1262 en RX continuo.
 *
 * A llamar:
 *   1) Después de encender todos los nerot (canal_encender ×7) —
 *      los encendidos configuran frecuencia/potencia en STANDBY,
 *      pero nunca emiten SET_RX. Sin esta llamada el chip queda
 *      sordo en STANDBY.
 *   2) Si algún módulo externo lleva el chip a STANDBY y necesita
 *      restaurar la escucha.
 *
 * Internamente usa SET_RX 0xFFFFFF (RX continuo = tamíd).
 * La llamada espera BUSY antes de emitir — si hay una TX en curso,
 * espera su fin (via esperar_busy en spi_transferir).
 *
 * 2 testigos:
 *   Nm 8:2   — "el mul penei hammenora yaʼiru shivʻat hannerot"
 *              (las 7 lámparas iluminan HACIA el tronco = oír)
 *   Éx 27:21 — nér tamíd (lámpara perpetua, juqqát ʻolám)
 */
int hashmal_comm_spi_modo_rx(void)
{
    if (!g_spi_activo) {
        MENORA_LOG_E("spi_modo_rx: yaréj no levantado");
        return HASHMAL_COMM_ERR_INIT;
    }

    int rc = sx1262_set_rx(0xFFFFFFu);
    if (rc != HASHMAL_COMM_OK) {
        MENORA_LOG_E("spi_modo_rx: SET_RX continuo falló (rc=%d) — "
                     "Menorá sin oído (Nm 8:2)", rc);
        return rc;
    }

    MENORA_LOG_I("═══ Nm 8:2 — SX1262 en RX continuo (tamíd 0xFFFFFF) ═══");
    MENORA_LOG_I("  el mul penei hammenora yaʼiru — los 7 oídos activos");
    return HASHMAL_COMM_OK;
}

/* ==================================================================
 *
 *   "yerejah veqanah... mimmenna yihyu" - Exodo 25:31
 *   "Su base y su tronco... DE ELLA seran."
 *
 *   "lajashov majashavot la'asot bazahav uvakephsef
 *    uvannajoshet" - Exodo 31:4
 *   "Para disenar disenos, para trabajar en oro, plata y BRONCE."
 *
 *   El yarej toca el silicio. El qanah conduce cada comando.
 *   Oro puro arriba - bronce que sostiene abajo. La Menora tiene
 *   base firme.
 *
 * ================================================================== */
