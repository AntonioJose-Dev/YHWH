/**
 * ===================================================================
 * HASHMAL-FIRMWARE — DRIVER E-PAPER DISPLAY
 * hashmal_epd.c
 * ===================================================================
 *
 * "veʻasitá MASAKH lefetaj haʼohel" -- Exodo 26:36
 * "Haras una CORTINA para la entrada de la tienda."
 *
 * Driver SPI para el display e-paper GDEQ031T10 (UC8253).
 * El display es la cortina visible del Mishkan -- muestra
 * el estado al mundo sin revelar el interior sagrado.
 *
 * Hardware: GDEQ031T10, 3.1", 240x320, B/W, UC8253
 * SPI: Mode 0, max 20MHz (usamos 10MHz por seguridad)
 * Bus compartido con LoRa (CS=3) y SD (CS=48)
 *
 * Oro de Egipto: toda llamada a ESP-IDF esta encapsulada aqui.
 * La logica de negocio solo ve hashmal_epd_*().
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 * ===================================================================
 */

#include <string.h>
#include <stdlib.h>

#include "esp_log.h"
#include "esp_err.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "hashmal_epd.h"
#include "hashmal_tdeck_pro.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF (Oro de Egipto)
 * ================================================================== */
static const char *EPD_TAG = "hashmal.epd";

#define EPD_LOG_I(fmt, ...)  ESP_LOGI(EPD_TAG, fmt, ##__VA_ARGS__)
#define EPD_LOG_W(fmt, ...)  ESP_LOGW(EPD_TAG, fmt, ##__VA_ARGS__)
#define EPD_LOG_E(fmt, ...)  ESP_LOGE(EPD_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * ESTADO INTERNO
 * ================================================================== */
static spi_device_handle_t s_epd_spi = NULL;
static uint8_t            *s_framebuffer = NULL;
static bool                s_initialized = false;

/* ==================================================================
 * UC8253 COMMAND TABLE
 *
 * Fuente: UC8253 datasheet (UltraChip/Good Display)
 * Los comandos siguen la familia UC81xx/UC82xx.
 * ================================================================== */
#define UC8253_PSR    0x00   /* Panel Setting */
#define UC8253_PWR    0x01   /* Power Setting */
#define UC8253_POF    0x02   /* Power OFF */
#define UC8253_PFS    0x03   /* Power OFF Sequence */
#define UC8253_PON    0x04   /* Power ON */
#define UC8253_BTST   0x06   /* Booster Soft Start */
#define UC8253_DSLP   0x07   /* Deep Sleep */
#define UC8253_DTM1   0x10   /* Data Start Transmission 1 (old) */
#define UC8253_DSP    0x11   /* Data Stop */
#define UC8253_DRF    0x12   /* Display Refresh */
#define UC8253_DTM2   0x13   /* Data Start Transmission 2 (new) */
#define UC8253_CDI    0x50   /* VCOM and Data Interval Setting */
#define UC8253_TCON   0x60   /* TCON Setting */
#define UC8253_TRES   0x61   /* Resolution Setting */
#define UC8253_DAM    0x65   /* SPI Flash Control */
#define UC8253_FLG    0x71   /* Get Status */

/* Timeout para BUSY (ms) */
#define EPD_BUSY_TIMEOUT_MS  30000

/* ==================================================================
 * SPI LOW-LEVEL (Oro de Egipto -- encapsulacion)
 * ================================================================== */

/**
 * Esperar a que BUSY baje (display listo).
 * UC8253: BUSY=LOW cuando ocupado, HIGH cuando listo.
 */
static esp_err_t epd_wait_busy(void)
{
    int timeout = EPD_BUSY_TIMEOUT_MS;
    while (gpio_get_level(HASHMAL_PIN_EPD_BUSY) == 0) {
        vTaskDelay(pdMS_TO_TICKS(10));
        timeout -= 10;
        if (timeout <= 0) {
            EPD_LOG_E("BUSY timeout -- display no responde");
            return ESP_ERR_TIMEOUT;
        }
    }
    return ESP_OK;
}

/**
 * Enviar COMANDO al UC8253 (DC=LOW).
 */
static void epd_send_cmd(uint8_t cmd)
{
    gpio_set_level(HASHMAL_PIN_EPD_DC, 0);  /* DC=0 -> command */

    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = 8;
    t.tx_buffer = &cmd;
    spi_device_polling_transmit(s_epd_spi, &t);
}

/**
 * Enviar DATOS al UC8253 (DC=HIGH).
 *
 * IMPORTANTE: enviar en UNA SOLA transaccion SPI.
 * GxEPD2 usa _startTransfer() + loop + _endTransfer()
 * que mantiene CS LOW durante TODA la transmision.
 * Si partimos en trozos, CS sube entre trozos y el
 * UC8253 pierde la cuenta de bytes.
 * DMA soporta hasta max_transfer_sz (~9664 bytes).
 */
static void epd_send_data(const uint8_t *data, size_t len)
{
    if (len == 0) return;

    gpio_set_level(HASHMAL_PIN_EPD_DC, 1);  /* DC=1 -> data */

    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = len * 8;
    t.tx_buffer = data;
    spi_device_polling_transmit(s_epd_spi, &t);
}

/**
 * Enviar un byte de datos.
 */
static void epd_send_data_byte(uint8_t val)
{
    epd_send_data(&val, 1);
}

/* ==================================================================
 * UC8253 INIT SEQUENCE
 *
 * Fuente EXACTA: GxEPD2_310_GDEQ031T10.cpp por Jean-Marc Zingg
 * (ZinggJM/GxEPD2 en GitHub -- driver verificado funcional)
 *
 * La secuencia real del UC8253 para GDEQ031T10 es MINIMALISTA:
 *   1. Soft reset via PSR (0x00) con [0x1E, 0x0D]
 *   2. Re-configurar PSR con [0x1F, 0x0D]
 *
 * El Power ON (0x04) y CDI (0x50) van ANTES del refresh,
 * NO durante el init. Esto fue el error principal.
 * ================================================================== */
static esp_err_t epd_init_display(void)
{
    EPD_LOG_I("Inicializando UC8253 (GDEQ031T10 240x320)...");
    EPD_LOG_I("Secuencia de GxEPD2_310_GDEQ031T10 (verificada)");

    /* Espera breve para que el UC8253 este listo tras power-up
     * (GxEPD2 usa init(115200, true, 2, false) que incluye delay) */
    vTaskDelay(pdMS_TO_TICKS(100));

    /* --- Paso 1: Soft Reset via Panel Setting ---
     * GxEPD2: _writeCommand(0x00); _writeData(0x1e); _writeData(0x0d);
     * Byte1 0x1E = soft reset + BW mode + scan config
     * Byte2 0x0D = panel-specific config (boost, gate) */
    epd_send_cmd(UC8253_PSR);
    epd_send_data_byte(0x1E);
    epd_send_data_byte(0x0D);

    /* Breve espera para que el soft reset surta efecto */
    vTaskDelay(pdMS_TO_TICKS(10));

    /* --- Paso 2: Panel Setting (configuracion final) ---
     * GxEPD2: _writeCommand(0x00); _writeData(0x1f); _writeData(0x0d);
     * 0x1F = KW-BW mode, scan up, shift left, DC-DC on, no-reset
     * 0x0D = panel boost/gate config */
    epd_send_cmd(UC8253_PSR);
    epd_send_data_byte(0x1F);
    epd_send_data_byte(0x0D);

    EPD_LOG_I("UC8253 inicializado OK (solo PSR, minimalista)");
    return ESP_OK;
}

/* ==================================================================
 * FONT 5x7 — ASCII 32-126
 *
 * Fuente bitmap embebida. Cada caracter es 5 columnas x 7 filas,
 * codificado como 5 bytes (1 byte por columna, bits 0-6 = filas).
 *
 * "veshinnanTAM levanéja" -- Deuteronomio 6:7
 * "Las GRABARAS a tus hijos" -- shinnan (H8150) = grabar, afilar.
 * Las letras se graban bit a bit en la pantalla, como la Palabra
 * se graba linea a linea en el corazon.
 * ================================================================== */
static const uint8_t s_font_5x7[95][5] = {
    /* 32  ' ' */ {0x00, 0x00, 0x00, 0x00, 0x00},
    /* 33  '!' */ {0x00, 0x00, 0x5F, 0x00, 0x00},
    /* 34  '"' */ {0x00, 0x07, 0x00, 0x07, 0x00},
    /* 35  '#' */ {0x14, 0x7F, 0x14, 0x7F, 0x14},
    /* 36  '$' */ {0x24, 0x2A, 0x7F, 0x2A, 0x12},
    /* 37  '%' */ {0x23, 0x13, 0x08, 0x64, 0x62},
    /* 38  '&' */ {0x36, 0x49, 0x55, 0x22, 0x50},
    /* 39  '\''*/ {0x00, 0x05, 0x03, 0x00, 0x00},
    /* 40  '(' */ {0x00, 0x1C, 0x22, 0x41, 0x00},
    /* 41  ')' */ {0x00, 0x41, 0x22, 0x1C, 0x00},
    /* 42  '*' */ {0x14, 0x08, 0x3E, 0x08, 0x14},
    /* 43  '+' */ {0x08, 0x08, 0x3E, 0x08, 0x08},
    /* 44  ',' */ {0x00, 0x50, 0x30, 0x00, 0x00},
    /* 45  '-' */ {0x08, 0x08, 0x08, 0x08, 0x08},
    /* 46  '.' */ {0x00, 0x60, 0x60, 0x00, 0x00},
    /* 47  '/' */ {0x20, 0x10, 0x08, 0x04, 0x02},
    /* 48  '0' */ {0x3E, 0x51, 0x49, 0x45, 0x3E},
    /* 49  '1' */ {0x00, 0x42, 0x7F, 0x40, 0x00},
    /* 50  '2' */ {0x42, 0x61, 0x51, 0x49, 0x46},
    /* 51  '3' */ {0x21, 0x41, 0x45, 0x4B, 0x31},
    /* 52  '4' */ {0x18, 0x14, 0x12, 0x7F, 0x10},
    /* 53  '5' */ {0x27, 0x45, 0x45, 0x45, 0x39},
    /* 54  '6' */ {0x3C, 0x4A, 0x49, 0x49, 0x30},
    /* 55  '7' */ {0x01, 0x71, 0x09, 0x05, 0x03},
    /* 56  '8' */ {0x36, 0x49, 0x49, 0x49, 0x36},
    /* 57  '9' */ {0x06, 0x49, 0x49, 0x29, 0x1E},
    /* 58  ':' */ {0x00, 0x36, 0x36, 0x00, 0x00},
    /* 59  ';' */ {0x00, 0x56, 0x36, 0x00, 0x00},
    /* 60  '<' */ {0x08, 0x14, 0x22, 0x41, 0x00},
    /* 61  '=' */ {0x14, 0x14, 0x14, 0x14, 0x14},
    /* 62  '>' */ {0x00, 0x41, 0x22, 0x14, 0x08},
    /* 63  '?' */ {0x02, 0x01, 0x51, 0x09, 0x06},
    /* 64  '@' */ {0x32, 0x49, 0x79, 0x41, 0x3E},
    /* 65  'A' */ {0x7E, 0x11, 0x11, 0x11, 0x7E},
    /* 66  'B' */ {0x7F, 0x49, 0x49, 0x49, 0x36},
    /* 67  'C' */ {0x3E, 0x41, 0x41, 0x41, 0x22},
    /* 68  'D' */ {0x7F, 0x41, 0x41, 0x22, 0x1C},
    /* 69  'E' */ {0x7F, 0x49, 0x49, 0x49, 0x41},
    /* 70  'F' */ {0x7F, 0x09, 0x09, 0x09, 0x01},
    /* 71  'G' */ {0x3E, 0x41, 0x49, 0x49, 0x7A},
    /* 72  'H' */ {0x7F, 0x08, 0x08, 0x08, 0x7F},
    /* 73  'I' */ {0x00, 0x41, 0x7F, 0x41, 0x00},
    /* 74  'J' */ {0x20, 0x40, 0x41, 0x3F, 0x01},
    /* 75  'K' */ {0x7F, 0x08, 0x14, 0x22, 0x41},
    /* 76  'L' */ {0x7F, 0x40, 0x40, 0x40, 0x40},
    /* 77  'M' */ {0x7F, 0x02, 0x0C, 0x02, 0x7F},
    /* 78  'N' */ {0x7F, 0x04, 0x08, 0x10, 0x7F},
    /* 79  'O' */ {0x3E, 0x41, 0x41, 0x41, 0x3E},
    /* 80  'P' */ {0x7F, 0x09, 0x09, 0x09, 0x06},
    /* 81  'Q' */ {0x3E, 0x41, 0x51, 0x21, 0x5E},
    /* 82  'R' */ {0x7F, 0x09, 0x19, 0x29, 0x46},
    /* 83  'S' */ {0x46, 0x49, 0x49, 0x49, 0x31},
    /* 84  'T' */ {0x01, 0x01, 0x7F, 0x01, 0x01},
    /* 85  'U' */ {0x3F, 0x40, 0x40, 0x40, 0x3F},
    /* 86  'V' */ {0x1F, 0x20, 0x40, 0x20, 0x1F},
    /* 87  'W' */ {0x3F, 0x40, 0x38, 0x40, 0x3F},
    /* 88  'X' */ {0x63, 0x14, 0x08, 0x14, 0x63},
    /* 89  'Y' */ {0x07, 0x08, 0x70, 0x08, 0x07},
    /* 90  'Z' */ {0x61, 0x51, 0x49, 0x45, 0x43},
    /* 91  '[' */ {0x00, 0x7F, 0x41, 0x41, 0x00},
    /* 92  '\\'*/ {0x02, 0x04, 0x08, 0x10, 0x20},
    /* 93  ']' */ {0x00, 0x41, 0x41, 0x7F, 0x00},
    /* 94  '^' */ {0x04, 0x02, 0x01, 0x02, 0x04},
    /* 95  '_' */ {0x40, 0x40, 0x40, 0x40, 0x40},
    /* 96  '`' */ {0x00, 0x01, 0x02, 0x04, 0x00},
    /* 97  'a' */ {0x20, 0x54, 0x54, 0x54, 0x78},
    /* 98  'b' */ {0x7F, 0x48, 0x44, 0x44, 0x38},
    /* 99  'c' */ {0x38, 0x44, 0x44, 0x44, 0x20},
    /* 100 'd' */ {0x38, 0x44, 0x44, 0x48, 0x7F},
    /* 101 'e' */ {0x38, 0x54, 0x54, 0x54, 0x18},
    /* 102 'f' */ {0x08, 0x7E, 0x09, 0x01, 0x02},
    /* 103 'g' */ {0x0C, 0x52, 0x52, 0x52, 0x3E},
    /* 104 'h' */ {0x7F, 0x08, 0x04, 0x04, 0x78},
    /* 105 'i' */ {0x00, 0x44, 0x7D, 0x40, 0x00},
    /* 106 'j' */ {0x20, 0x40, 0x44, 0x3D, 0x00},
    /* 107 'k' */ {0x7F, 0x10, 0x28, 0x44, 0x00},
    /* 108 'l' */ {0x00, 0x41, 0x7F, 0x40, 0x00},
    /* 109 'm' */ {0x7C, 0x04, 0x18, 0x04, 0x78},
    /* 110 'n' */ {0x7C, 0x08, 0x04, 0x04, 0x78},
    /* 111 'o' */ {0x38, 0x44, 0x44, 0x44, 0x38},
    /* 112 'p' */ {0x7C, 0x14, 0x14, 0x14, 0x08},
    /* 113 'q' */ {0x08, 0x14, 0x14, 0x18, 0x7C},
    /* 114 'r' */ {0x7C, 0x08, 0x04, 0x04, 0x08},
    /* 115 's' */ {0x48, 0x54, 0x54, 0x54, 0x20},
    /* 116 't' */ {0x04, 0x3F, 0x44, 0x40, 0x20},
    /* 117 'u' */ {0x3C, 0x40, 0x40, 0x20, 0x7C},
    /* 118 'v' */ {0x1C, 0x20, 0x40, 0x20, 0x1C},
    /* 119 'w' */ {0x3C, 0x40, 0x30, 0x40, 0x3C},
    /* 120 'x' */ {0x44, 0x28, 0x10, 0x28, 0x44},
    /* 121 'y' */ {0x0C, 0x50, 0x50, 0x50, 0x3C},
    /* 122 'z' */ {0x44, 0x64, 0x54, 0x4C, 0x44},
    /* 123 '{' */ {0x00, 0x08, 0x36, 0x41, 0x00},
    /* 124 '|' */ {0x00, 0x00, 0x7F, 0x00, 0x00},
    /* 125 '}' */ {0x00, 0x41, 0x36, 0x08, 0x00},
    /* 126 '~' */ {0x10, 0x08, 0x08, 0x10, 0x08},
};

/* ==================================================================
 * PIXEL SET/CLEAR en framebuffer
 *
 * El framebuffer es 1bpp: 1=blanco, 0=negro.
 * Organizacion: byte[y * (width/8) + x/8], bit (7 - x%8)
 * MSB first (bit 7 = pixel mas a la izquierda del byte).
 * ================================================================== */
static inline void fb_set_pixel(int x, int y, bool black)
{
    if (x < 0 || x >= HASHMAL_EPD_WIDTH) return;
    if (y < 0 || y >= HASHMAL_EPD_HEIGHT) return;

    int byte_idx = y * (HASHMAL_EPD_WIDTH / 8) + (x / 8);
    uint8_t bit_mask = 0x80 >> (x % 8);

    if (black) {
        s_framebuffer[byte_idx] &= ~bit_mask;  /* 0 = negro */
    } else {
        s_framebuffer[byte_idx] |= bit_mask;    /* 1 = blanco */
    }
}

/* ==================================================================
 * API PUBLICA
 * ================================================================== */

esp_err_t hashmal_epd_init(void)
{
    esp_err_t ret;

    if (s_initialized) {
        EPD_LOG_W("EPD ya inicializado");
        return ESP_OK;
    }

    /* --- Allocar framebuffer --- */
    s_framebuffer = (uint8_t *)malloc(HASHMAL_EPD_BUF_SIZE);
    if (s_framebuffer == NULL) {
        EPD_LOG_E("No hay memoria para framebuffer (%d bytes)",
                  HASHMAL_EPD_BUF_SIZE);
        return ESP_ERR_NO_MEM;
    }
    /* Iniciar en blanco (todos los bits = 1) */
    memset(s_framebuffer, 0xFF, HASHMAL_EPD_BUF_SIZE);

    /* --- Configurar GPIO --- */
    gpio_config_t io_conf = {
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    /* DC pin */
    io_conf.pin_bit_mask = (1ULL << HASHMAL_PIN_EPD_DC);
    gpio_config(&io_conf);

    /* CS pin (manual, no automatico por SPI driver por compatibilidad) */
    io_conf.pin_bit_mask = (1ULL << HASHMAL_PIN_EPD_CS);
    gpio_config(&io_conf);
    gpio_set_level(HASHMAL_PIN_EPD_CS, 1);  /* CS alto = deseleccionado */

    /* BUSY pin (input) */
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    io_conf.pin_bit_mask = (1ULL << HASHMAL_PIN_EPD_BUSY);
    gpio_config(&io_conf);

    /* Enable LoRa module pin (keep disabled during EPD use) */
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    io_conf.pin_bit_mask = (1ULL << HASHMAL_PIN_LORA_EN);
    gpio_config(&io_conf);
    gpio_set_level(HASHMAL_PIN_LORA_EN, 0);  /* LoRa OFF por ahora */

    /* --- Pull HIGH all other CS pins on shared SPI bus ---
     * GxEPD2 test_EPD.ino: ALL CS pins HIGH before SPI init.
     * "Uniras las cortinas UNA CON SU HERMANA" (Ex 26:3)
     * -- cada hermana tiene su broche (CS) bien cerrado. */
    gpio_config_t cs_conf = {
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
        .pin_bit_mask = (1ULL << HASHMAL_PIN_LORA_CS) |
                        (1ULL << HASHMAL_PIN_SD_CS),
    };
    gpio_config(&cs_conf);
    gpio_set_level(HASHMAL_PIN_LORA_CS, 1);  /* LoRa CS HIGH */
    gpio_set_level(HASHMAL_PIN_SD_CS, 1);    /* SD CS HIGH */

    /* --- Inicializar SPI bus ---
     * MISO = GPIO47: el EPD no usa MISO (solo escribe), pero el bus
     * es compartido con el SX1262 LoRa que SÍ necesita MISO para
     * leer IRQ status, buffer RX, etc.
     *
     * ADVERTENCIA: miso_io_num = -1 fue el bug raíz que causaba que
     * todos los reads del SX1262 devolvieran 0xFF (pin flotante).
     * El periférico SPI no puede leer MISO si no fue configurado aquí.
     * spi_bus_add_device() no configura pines del bus — solo el CS.
     *
     * 2 testigos:
     *   Éx 26:6 — "veyhayáh haJoshkán ejád" — el Mishkán es UNO.
     *              Un solo bus SPI con TODAS sus líneas activas.
     *   Zac 4:12 — los dos tubos de oro VIERTEN (bidireccional):
     *              MOSI envía, MISO recibe — ambos son necesarios.
     */
    spi_bus_config_t bus_cfg = {
        .mosi_io_num = HASHMAL_PIN_SPI_MOSI,
        .miso_io_num = HASHMAL_PIN_SPI_MISO,   /* GPIO47 — compartido EPD+LoRa+SD */
        .sclk_io_num = HASHMAL_PIN_SPI_SCK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = HASHMAL_EPD_BUF_SIZE + 64,
    };

    ret = spi_bus_initialize(SPI2_HOST, &bus_cfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK) {
        EPD_LOG_E("SPI bus init fallo: %s", esp_err_to_name(ret));
        free(s_framebuffer);
        s_framebuffer = NULL;
        return ret;
    }

    /* --- Añadir dispositivo EPD al bus SPI --- */
    spi_device_interface_config_t dev_cfg = {
        .clock_speed_hz = 10 * 1000 * 1000,  /* 10 MHz */
        .mode = 0,                             /* SPI Mode 0 (CPOL=0, CPHA=0) */
        .spics_io_num = HASHMAL_PIN_EPD_CS,
        .queue_size = 1,
    };

    ret = spi_bus_add_device(SPI2_HOST, &dev_cfg, &s_epd_spi);
    if (ret != ESP_OK) {
        EPD_LOG_E("SPI add device fallo: %s", esp_err_to_name(ret));
        spi_bus_free(SPI2_HOST);
        free(s_framebuffer);
        s_framebuffer = NULL;
        return ret;
    }

    /* --- Inicializar display UC8253 --- */
    ret = epd_init_display();
    if (ret != ESP_OK) {
        EPD_LOG_E("UC8253 init fallo");
        return ret;
    }

    s_initialized = true;
    EPD_LOG_I("masakh -- cortina e-paper lista (Ex 26:36)");
    return ESP_OK;
}

void hashmal_epd_clear(uint8_t color)
{
    if (s_framebuffer == NULL) return;
    memset(s_framebuffer, color, HASHMAL_EPD_BUF_SIZE);
}

void hashmal_epd_draw_char(int x, int y, char c, uint8_t scale, bool black)
{
    if (s_framebuffer == NULL) return;
    if (c < 32 || c > 126) c = '?';
    if (scale == 0) scale = 1;

    int idx = c - 32;
    const uint8_t *glyph = s_font_5x7[idx];

    for (int col = 0; col < 5; col++) {
        uint8_t col_data = glyph[col];
        for (int row = 0; row < 7; row++) {
            if (col_data & (1 << row)) {
                /* Pixel encendido -- escalar */
                for (int sy = 0; sy < (int)scale; sy++) {
                    for (int sx = 0; sx < (int)scale; sx++) {
                        fb_set_pixel(x + col * (int)scale + sx,
                                     y + row * (int)scale + sy,
                                     black);
                    }
                }
            }
        }
    }
}

void hashmal_epd_draw_string(int x, int y, const char *str,
                              uint8_t scale, bool black)
{
    if (str == NULL || s_framebuffer == NULL) return;
    if (scale == 0) scale = 1;

    int char_width = 6 * (int)scale;  /* 5 pixels + 1 spacing */
    int cx = x;

    while (*str) {
        if (*str == '\n') {
            cx = x;
            y += 8 * (int)scale;  /* 7 pixels + 1 spacing */
        } else {
            hashmal_epd_draw_char(cx, y, *str, scale, black);
            cx += char_width;
        }
        str++;
    }
}

/* ==================================================================
 * PRIMITIVAS GRAFICAS — Exodo 31:4 (jalashévet majashot)
 * "para pensar diseños artísticos en oro, plata y bronce"
 * ================================================================== */

void hashmal_epd_draw_pixel(int x, int y, bool black)
{
    /* Ex 26:15 — "tablas puestas EN PIE": cada pixel en su lugar exacto */
    if (s_framebuffer) fb_set_pixel(x, y, black);
}

void hashmal_epd_fill_rect(int x, int y, int w, int h, bool black)
{
    /* Ex 31:4 — jalashévet: rellenar con diseño artístico deliberado */
    if (!s_framebuffer || w <= 0 || h <= 0) return;
    for (int row = y; row < y + h; row++)
        for (int col = x; col < x + w; col++)
            fb_set_pixel(col, row, black);
}

void hashmal_epd_draw_hline(int x, int y, int len, bool black)
{
    /* Ex 26:1 — "hilo de lino torcido": linea horizontal */
    if (!s_framebuffer || len <= 0) return;
    for (int i = 0; i < len; i++) fb_set_pixel(x + i, y, black);
}

void hashmal_epd_draw_vline(int x, int y, int len, bool black)
{
    /* Ex 26:15 — tablas puestas en pie: linea vertical */
    if (!s_framebuffer || len <= 0) return;
    for (int i = 0; i < len; i++) fb_set_pixel(x, y + i, black);
}

void hashmal_epd_draw_rect(int x, int y, int w, int h, bool black)
{
    /* Ex 26:16 — "un codo y medio el ancho de cada tabla" — 4 lados exactos
     * 2 testigos:
     *   Ex 26:16 — medidas precisas de la tabla del Mishkán
     *   Ex 27:1  — "cinco codos de largo y cinco de ancho" (cuadrado) */
    if (w <= 0 || h <= 0) return;
    hashmal_epd_draw_hline(x, y, w, black);
    hashmal_epd_draw_hline(x, y + h - 1, w, black);
    hashmal_epd_draw_vline(x, y, h, black);
    hashmal_epd_draw_vline(x + w - 1, y, h, black);
}

void hashmal_epd_refresh(void)
{
    if (!s_initialized || s_framebuffer == NULL) return;

    EPD_LOG_I("Enviando framebuffer al display...");

    /* ============================================================
     * SECUENCIA EXACTA de GxEPD2 clearScreen():
     *   1. _writeScreenBuffer(0x10, value)  -- "previous" buffer
     *   2. _writeScreenBuffer(0x13, value)  -- "current" buffer
     *   3. _Update_Full()
     *
     * _Update_Full() hace:
     *   - CDI (0x50) con 0x97
     *   - PowerOn (0x04) + wait BUSY
     *   - DRF (0x12) + wait BUSY
     * ============================================================ */

    /* --- Buffer "previous" (DTM1 = 0x10) ---
     * GxEPD2 envia el mismo contenido a ambos buffers en clearScreen.
     * Para full refresh, enviamos nuestro framebuffer a AMBOS. */
    epd_send_cmd(UC8253_DTM1);
    epd_send_data(s_framebuffer, HASHMAL_EPD_BUF_SIZE);

    /* --- Buffer "current" (DTM2 = 0x13) --- */
    epd_send_cmd(UC8253_DTM2);
    epd_send_data(s_framebuffer, HASHMAL_EPD_BUF_SIZE);

    /* --- _Update_Full: CDI + PowerOn + Refresh --- */

    /* CDI (0x50): VCOM and Data Interval -- borde blanco */
    epd_send_cmd(UC8253_CDI);
    epd_send_data_byte(0x97);

    /* PON (0x04): Power ON -- enciende charge pump */
    epd_send_cmd(UC8253_PON);
    EPD_LOG_I("Power ON -- esperando charge pump...");
    epd_wait_busy();

    /* DRF (0x12): Display Refresh -- actualiza la pantalla */
    epd_send_cmd(UC8253_DRF);
    EPD_LOG_I("Refresh enviado -- esperando e-paper (2-4s)...");
    epd_wait_busy();

    EPD_LOG_I("Refresco completo -- masakh actualizado");
}

void hashmal_epd_sleep(void)
{
    if (!s_initialized) return;

    /* POF (0x02): Power OFF -- apagar charge pump */
    epd_send_cmd(UC8253_POF);
    epd_wait_busy();

    /* DSLP (0x07): Deep Sleep con check code 0xA5
     * GxEPD2: _writeCommand(0x07); _writeData(0xA5); */
    epd_send_cmd(UC8253_DSLP);
    epd_send_data_byte(0xA5);

    EPD_LOG_I("EPD en deep sleep -- masakh descansa (Shabat)");
}
