/**
 * ===================================================================
 * HASHMAL-FIRMWARE — PANTALLA DE ESTADO
 * hashmal_menu.c  (v3 — estado simple, sin menú)
 * ===================================================================
 *
 * "vayyimallé haKavod YHWH et haMishkán"
 *   — Éxodo 40:34
 * "La gloria de YHWH llenó el Mishkán."
 *
 * Pantalla única de estado:
 *   - Nombre del proyecto y autor
 *   - Estado KAVOD (14/14 piezas)
 *   - Batería actual
 *   - Nodos LoRa conectados
 *   - Toque en pantalla → refresca los datos
 *
 * Autor: Antonio José Marín Soto
 * Proyecto: HASHMAL-CORE
 * ===================================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "esp_log.h"
#include "driver/i2c.h"
#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"

#include "hashmal_menu.h"
#include "hashmal_epd.h"
#include "hashmal_tdeck_pro.h"
#include "hashmal_comm.h"
#include "hashmal_power.h"   /* Pieza 08 — Aceite Perpetuo (Éx 27:20) */

/* Conteo real de peers ESP-NOW (hermanas descubiertas).
 * Nm 1:2 — lifrót: contad los que están presentes de verdad.
 * 2 testigos:
 *   Nm 1:2  — lifrót ot-ám: contar por nombre, no por canal
 *   Éx 26:3 — jabéret ishá el ajotá: las tablas se unen entre sí */
extern int hashmal_comm_espnow_hermanas_contar(uint8_t *conteo);

/* ==================================================================
 * LOGS
 * ================================================================== */
static const char *MENU_TAG = "hashmal.menu";
#define MENU_LOG_I(fmt, ...)  ESP_LOGI(MENU_TAG, fmt, ##__VA_ARGS__)
#define MENU_LOG_W(fmt, ...)  ESP_LOGW(MENU_TAG, fmt, ##__VA_ARGS__)
#define MENU_LOG_E(fmt, ...)  ESP_LOGE(MENU_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * CST328 — TOUCH (I2C 0x1A, INT=GPIO12, RST=GPIO45)
 * "veʻasitá masakh lefétaj haʼohel" — Éx 26:36
 * ================================================================== */
#define CST328_ADDR    HASHMAL_I2C_ADDR_TOUCH   /* 0x1A */

/* ==================================================================
 * TCA8418 — TECLADO (I2C 0x34) — inicialización básica
 * ================================================================== */
#define TCA8418_ADDR        0x34
#define TCA8418_REG_CFG     0x01
#define TCA8418_CFG_KE_IEN  0x01

/* ==================================================================
 * BATERÍA — lectura ADC directa
 * GPIO4 = ADC1_CH3, divisor de tensión 1:1 (R=100K/100K).
 * Rango LiPo: 3.0 V (0%) → 4.2 V (100%).
 * Con divisor 1:2 en ADC: 1.5 V → 2.1 V.
 *
 * "shémen záyit zaj katít lamaor" — Éx 27:20
 * El aceite puro mide su nivel con exactitud.
 *
 * 2 testigos:
 *   Éx 27:20 — aceite siempre visible en la Menorá
 *   Zac 4:12 — "dos tubos de oro que vierten el aceite":
 *               dos mitades del divisor que miden la carga ================================================================== */
#define BAT_ADC_UNIT     ADC_UNIT_1
#define BAT_ADC_CHANNEL  ADC_CHANNEL_3   /* GPIO4 — T-Deck Pro */
#define BAT_ADC_ATTEN    ADC_ATTEN_DB_12 /* rango 0–3.1 V */
/* Valores RAW calibrados (12-bit, sin calibración fina):
 *   3.0 V / 2 = 1.50 V → raw ≈ 1980  (0%)
 *   4.2 V / 2 = 2.10 V → raw ≈ 2770  (100%) */
#define BAT_RAW_MIN   1980
#define BAT_RAW_MAX   2770

static adc_oneshot_unit_handle_t s_adc_handle = NULL;

static void bat_adc_init(void)
{
    adc_oneshot_unit_init_cfg_t cfg = {
        .unit_id  = BAT_ADC_UNIT,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    if (adc_oneshot_new_unit(&cfg, &s_adc_handle) != ESP_OK) {
        MENU_LOG_W("ADC bat init fallo");
        s_adc_handle = NULL;
        return;
    }
    adc_oneshot_chan_cfg_t ch = {
        .bitwidth = ADC_BITWIDTH_12,
        .atten    = BAT_ADC_ATTEN,
    };
    adc_oneshot_config_channel(s_adc_handle, BAT_ADC_CHANNEL, &ch);
    MENU_LOG_I("  [✓] BAT ADC GPIO4 (Éx 27:20 shemen)");
}

static uint8_t bat_leer_pct(void)
{
    /* Zac 4:12 shibbolei hazzahav — los tubos de oro vierten desde
     * el componente de energia (pieza 08, Ex 27:20). El ADC lo
     * administra hashmal_power; el menu solo consume la lectura
     * por la API publica — no duplicamos handles. */
    hashmal_power_lectura_t lectura;
    if (hashmal_power_leer_olivo(HASHMAL_POWER_OLIVO_SMOL, &lectura) == HASHMAL_POWER_OK) {
        return lectura.porcentaje;
    }
    /* Fallback silencioso si el componente no esta listo todavia */
    return 0;
}

/* ==================================================================
 * ESTADO INTERNO
 * ================================================================== */
static SemaphoreHandle_t s_mutex       = NULL;
static QueueHandle_t     s_touch_queue = NULL;
static bool              s_initialized = false;

/* ==================================================================
 * I2C HELPERS
 * "Oro de Egipto" — toda llamada I2C encapsulada aquí.
 * ================================================================== */
static void i2c_bus_init(void)
{
    i2c_config_t conf = {
        .mode             = I2C_MODE_MASTER,
        .sda_io_num       = HASHMAL_PIN_I2C_SDA,
        .scl_io_num       = HASHMAL_PIN_I2C_SCL,
        .sda_pullup_en    = GPIO_PULLUP_ENABLE,
        .scl_pullup_en    = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 400000,
    };
    i2c_param_config(I2C_NUM_0, &conf);
    esp_err_t ret = i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);
    if (ret != ESP_OK && ret != ESP_ERR_INVALID_STATE) {
        MENU_LOG_E("I2C init error: %s", esp_err_to_name(ret));
    }
    /* TCA8418 — habilitar interrupciones de teclado */
    uint8_t buf[2] = { TCA8418_REG_CFG, TCA8418_CFG_KE_IEN };
    i2c_master_write_to_device(I2C_NUM_0, TCA8418_ADDR, buf, 2, pdMS_TO_TICKS(20));
}

/* ==================================================================
 * CST328 — ISR + GPIO
 * Un toque = un flanco NEGEDGE = un evento en la cola.
 * Éx 28:34 — paʻamón: la campana suena UNA vez al toque.
 * ================================================================== */
static void IRAM_ATTR cst328_isr_handler(void *arg)
{
    uint32_t ev = 1;
    xQueueSendFromISR(s_touch_queue, &ev, NULL);
}

static void cst328_init(void)
{
    /* RST: salida */
    gpio_config_t cfg = {
        .mode         = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << HASHMAL_PIN_TOUCH_RST),
        .pull_up_en   = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE,
    };
    gpio_config(&cfg);

    /* INT: entrada pull-up, NEGEDGE */
    cfg.mode         = GPIO_MODE_INPUT;
    cfg.pull_up_en   = GPIO_PULLUP_ENABLE;
    cfg.pin_bit_mask = (1ULL << HASHMAL_PIN_TOUCH_INT);
    cfg.intr_type    = GPIO_INTR_NEGEDGE;
    gpio_config(&cfg);

    /* Reset físico */
    gpio_set_level(HASHMAL_PIN_TOUCH_RST, 0);
    vTaskDelay(pdMS_TO_TICKS(20));
    gpio_set_level(HASHMAL_PIN_TOUCH_RST, 1);
    vTaskDelay(pdMS_TO_TICKS(100));

    MENU_LOG_I("  [✓] CST328 (Éx 26:36)");
}

/* Lee coordenadas con protocolo 0xD000 + ACK obligatorio.
 * Sin ACK el INT permanece LOW → ghost touches infinitos.
 * 2 testigos:
 *   Lv 16:14 — la lectura requiere el gesto de cierre (ACK)
 *   Nm 8:3   — kenAsher: seguir el protocolo del fabricante */
static bool cst328_leer_evento(uint16_t *px, uint16_t *py)
{
    static bool s_espera_subida = false;
    int int_level = gpio_get_level(HASHMAL_PIN_TOUCH_INT);

    if (s_espera_subida) {
        if (int_level != 0) s_espera_subida = false;
        return false;
    }
    if (int_level != 0) return false;

    uint8_t reg[2] = {0xD0, 0x00};
    uint8_t buf[7] = {0};
    esp_err_t ret = i2c_master_write_read_device(
        I2C_NUM_0, CST328_ADDR, reg, 2, buf, 7, pdMS_TO_TICKS(20));

    /* ACK SIEMPRE — libera el INT pin */
    uint8_t ack[3] = {0xD0, 0x00, 0xAB};
    i2c_master_write_to_device(I2C_NUM_0, CST328_ADDR, ack, 3, pdMS_TO_TICKS(10));

    if (ret != ESP_OK) return false;
    if (buf[6] != 0xAB || buf[0] == 0xAB) return false;
    if ((buf[5] & 0x7F) == 0) return false;

    uint16_t x = ((uint16_t)buf[1] << 4) | ((buf[3] >> 4) & 0x0F);
    uint16_t y = ((uint16_t)buf[2] << 4) | (buf[3] & 0x0F);

    /* FLIP_Y: panel Y=0 abajo, EPD Y=0 arriba */
    y = (uint16_t)(HASHMAL_EPD_HEIGHT - 1 - y);

    if (x >= HASHMAL_EPD_WIDTH)  x = (uint16_t)(HASHMAL_EPD_WIDTH  - 1);
    if (y >= HASHMAL_EPD_HEIGHT) y = (uint16_t)(HASHMAL_EPD_HEIGHT - 1);

    s_espera_subida = true;
    if (px) *px = x;
    if (py) *py = y;
    return true;
}

/* ==================================================================
 * PANTALLA DE ESTADO — lo único que se muestra
 *
 * "vayyimallé haKavod YHWH et haMishkán" — Éx 40:34
 * La gloria del Padre llena el Mishkán: pantalla limpia con todo
 * lo que el operador necesita saber de un vistazo.
 *
 * 2 testigos para mostrar batería y conexión:
 *   Éx 27:20 — shémen záyit zaj katít: el aceite (energía) visible
 *   Nm 1:2   — lifrót: contad los conectados por sus nombres
 * ================================================================== */
static void dibujar_estado(void)
{
    hashmal_epd_clear(0xFF);

    int y = 2;

    /* ── Cabecera ──────────────────────────────────────── */
    hashmal_epd_draw_string(2, y, "HASHMAL-CORE", 2, true);
    y += 22;
    hashmal_epd_draw_string(2, y, "Red Soberana segun", 1, true);
    y += 12;
    hashmal_epd_draw_string(2, y, "el Tavnit de YHWH", 1, true);
    y += 16;
    hashmal_epd_draw_hline(0, y, HASHMAL_EPD_WIDTH, true);
    y += 6;

    /* ── Estado KAVOD ──────────────────────────────────── */
    hashmal_epd_draw_string(2, y, "Estado: KAVOD", 2, true);
    y += 22;
    hashmal_epd_draw_string(2, y, "Gloria lleno el Mishkan", 1, true);
    y += 12;
    hashmal_epd_draw_string(2, y, "Exodo 40:34", 1, true);
    y += 14;
    hashmal_epd_draw_string(2, y, "14/14 piezas operativas", 1, true);
    y += 16;
    hashmal_epd_draw_hline(0, y, HASHMAL_EPD_WIDTH, true);
    y += 6;

    /* ── Hardware ──────────────────────────────────────── */
    hashmal_epd_draw_string(2, y, "SX1262 LoRa 868.1MHz", 1, true);
    y += 12;
    hashmal_epd_draw_string(2, y, "EPD UC8253 activo", 1, true);
    y += 12;
    hashmal_epd_draw_string(2, y, "SPI bus ejad (Ex 26:6)", 1, true);
    y += 16;
    hashmal_epd_draw_string(2, y, "ESP32-S3 T-Deck Pro V1", 1, true);
    y += 16;
    hashmal_epd_draw_hline(0, y, HASHMAL_EPD_WIDTH, true);
    y += 6;

    /* ── Batería — lectura ADC real GPIO4 — Éx 27:20 ──── */
    {
        char bat_str[32];
        uint8_t pct = bat_leer_pct();
        snprintf(bat_str, sizeof(bat_str), "Bateria: %u%%", (unsigned)pct);
        hashmal_epd_draw_string(2, y, bat_str, 2, true);
        y += 22;
    }

    /* ── Nodos reales — ESP-NOW hermanas — Nm 1:2 ──────── */
    {
        char nodos_str[32];
        uint8_t hermanas = 0;
        int rc = hashmal_comm_espnow_hermanas_contar(&hermanas);
        if (rc != 0 || hermanas == 0) {
            snprintf(nodos_str, sizeof(nodos_str), "Red: buscando...");
        } else {
            snprintf(nodos_str, sizeof(nodos_str), "Nodos: %u conectado%s",
                     (unsigned)hermanas, hermanas == 1 ? "" : "s");
        }
        hashmal_epd_draw_string(2, y, nodos_str, 2, true);
        y += 22;
    }

    hashmal_epd_draw_hline(0, y, HASHMAL_EPD_WIDTH, true);
    y += 6;

    /* ── Autor ─────────────────────────────────────────── */
    hashmal_epd_draw_string(2, y, "Antonio Jose Marin Soto", 1, true);

    hashmal_epd_refresh();
    MENU_LOG_I("Estado dibujado (Éx 40:34)");
}

/* ==================================================================
 * TASK — refresca la pantalla al toque o cada 30 segundos
 * "tamíd" — perpetuo, siempre actualizado.
 * ================================================================== */
static void hashmal_menu_task(void *pv)
{
    (void)pv;
    vTaskDelay(pdMS_TO_TICKS(2000));

    if (xSemaphoreTake(s_mutex, pdMS_TO_TICKS(500)) == pdTRUE) {
        dibujar_estado();
        xSemaphoreGive(s_mutex);
    }

    for (;;) {
        uint32_t ev;

        /* Esperar evento de toque o timeout de 60 s.
         * El E-Paper tarda 2-4 s en refrescar (parpadeo físico inevitable).
         * Para no parpadear en cada toque, el toque SOLO hace ACK al CST328
         * (libera el INT pin) pero NO redibuja la pantalla.
         * La pantalla se actualiza sola cada 60 segundos.
         *
         * 2 testigos:
         *   Éx 40:2  — "el primer día del mes levantarás": el Mishkán
         *               se levanta en su tiempo, no cada vez que alguien
         *               pasa la mano por la cortina
         *   Nm 28:3  — tamíd: el sacrificio perpetuo tiene su hora fija */
        if (xQueueReceive(s_touch_queue, &ev, pdMS_TO_TICKS(60000)) == pdTRUE) {
            /* Toque recibido — ACK al CST328 (libera INT), NO redibujar */
            uint16_t tx = 0, ty = 0;
            cst328_leer_evento(&tx, &ty);
            /* Vaciar la cola por si hubo más toques durante el procesamiento */
            while (xQueueReceive(s_touch_queue, &ev, 0) == pdTRUE) {
                cst328_leer_evento(&tx, &ty);
            }
            continue;   /* sin refresco — sin parpadeo */
        }

        /* Timeout de 60 s: actualizar batería y nodos */
        if (xSemaphoreTake(s_mutex, pdMS_TO_TICKS(500)) == pdTRUE) {
            dibujar_estado();
            xSemaphoreGive(s_mutex);
        }

        /* Vaciar toques acumulados durante el refresco (~3 s de EPD) */
        {
            uint16_t tx = 0, ty = 0;
            while (xQueueReceive(s_touch_queue, &ev, 0) == pdTRUE) {
                cst328_leer_evento(&tx, &ty);
            }
        }
    }
}

/* ==================================================================
 * API PÚBLICA
 * ================================================================== */
void hashmal_menu_init(void)
{
    if (s_initialized) {
        MENU_LOG_W("menu ya inicializado");
        return;
    }

    MENU_LOG_I("═══ KAVOD — Éx 40:34 ═══");

    s_mutex = xSemaphoreCreateMutex();
    if (!s_mutex) { MENU_LOG_E("fallo mutex"); return; }

    s_touch_queue = xQueueCreate(4, sizeof(uint32_t));
    if (!s_touch_queue) { MENU_LOG_E("fallo queue"); return; }

    esp_err_t rc = hashmal_epd_init();
    if (rc != ESP_OK) { MENU_LOG_E("EPD init falló"); return; }
    MENU_LOG_I("  [✓] EPD");

    bat_adc_init();

    i2c_bus_init();
    MENU_LOG_I("  [✓] I2C + TCA8418");

    cst328_init();

    esp_err_t isr_ret = gpio_install_isr_service(0);
    if (isr_ret != ESP_OK && isr_ret != ESP_ERR_INVALID_STATE) {
        MENU_LOG_W("gpio isr service: %s", esp_err_to_name(isr_ret));
    }
    gpio_isr_handler_add(HASHMAL_PIN_TOUCH_INT,
                         cst328_isr_handler,
                         (void *)(uintptr_t)HASHMAL_PIN_TOUCH_INT);
    MENU_LOG_I("  [✓] CST328 ISR NEGEDGE GPIO%d", HASHMAL_PIN_TOUCH_INT);

    xTaskCreate(hashmal_menu_task, "kavod", 4096, NULL, 3, NULL);
    MENU_LOG_I("  [✓] Task kavod");

    s_initialized = true;
    MENU_LOG_I("═══ KAVOD levantado ═══");
}

void hashmal_menu_refrescar(void)
{
    if (!s_initialized || !s_mutex) return;
    if (xSemaphoreTake(s_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        dibujar_estado();
        xSemaphoreGive(s_mutex);
    }
}
