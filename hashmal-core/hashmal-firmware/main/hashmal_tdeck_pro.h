/**
 * ===================================================================
 * HASHMAL-FIRMWARE — MAPA DE PINES T-DECK PRO V1.0
 * hashmal_tdeck_pro.h
 * ===================================================================
 *
 * "veʻasitá et haqqerashím lammishkán ʻatsé shittím ʻomedím."
 *   -- Exodo 26:15
 * "Haras las TABLAS para el Mishkan de madera de acacia, puestas
 *  EN PIE." -- Cada tabla tiene su POSICION FIJA. Cada GPIO tiene
 *  su asignacion FIJA en el hardware. No se mueven.
 *
 * Este archivo mapea los GPIO del LilyGO T-Deck Pro V1.0
 * (ESP32-S3 + SX1262 + GDEQ031T10 e-paper + TCA8418 keyboard)
 * a constantes del firmware HASHMAL-CORE.
 *
 * Fuente: Xinyuan-LilyGO/T-Deck-Pro utilities.h (ShallowGreen123)
 * Hardware: T-Deck Pro V1.0 (PCM5102A variant)
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 * ===================================================================
 */

#pragma once

/* ==================================================================
 * BUS SPI COMPARTIDO (HSPI)
 *
 * "vehibbartá et hayeriʻot ishshá el ajotáh" -- Exodo 26:3
 * "Uniras las cortinas UNA CON SU HERMANA" -- Un solo bus SPI
 * conecta tres perifericos (EPD, LoRa, SD) como hermanas unidas.
 * Cada una tiene su propio CS (broche de oro) que la identifica.
 * ================================================================== */
#define HASHMAL_PIN_SPI_SCK    36
#define HASHMAL_PIN_SPI_MOSI   33
#define HASHMAL_PIN_SPI_MISO   47

/* ==================================================================
 * E-PAPER DISPLAY — GDEQ031T10 (UC8253), 240x320
 *
 * "veʻasitá MASAKH lefetaj haʼohel" -- Exodo 26:36
 * "Haras una CORTINA para la entrada de la tienda" -- La pantalla
 * e-paper es la cortina visible del Mishkan: lo que el mundo ve
 * desde afuera. Muestra el estado sin revelar el interior.
 * ================================================================== */
#define HASHMAL_PIN_EPD_CS     34
#define HASHMAL_PIN_EPD_DC     35
#define HASHMAL_PIN_EPD_BUSY   37
#define HASHMAL_PIN_EPD_RST    (-1)   /* Sin reset HW -- solo SW reset */

#define HASHMAL_EPD_WIDTH      240
#define HASHMAL_EPD_HEIGHT     320
#define HASHMAL_EPD_BUF_SIZE   (HASHMAL_EPD_WIDTH / 8 * HASHMAL_EPD_HEIGHT)

/* ==================================================================
 * LORA SX1262 — Comunicacion de largo alcance
 *
 * "veʻasitá menorát zahav tahor miqshah teʻaseh hammenurah"
 *   -- Exodo 25:31
 * "Haras un candelabro de oro puro, labrado a martillo" -- La
 * Menora (pieza 04) ilumina a distancia. El SX1262 es el
 * hardware que lleva esa luz (LoRa) a donde el ojo no alcanza.
 * ================================================================== */
#define HASHMAL_PIN_LORA_CS    3
#define HASHMAL_PIN_LORA_BUSY  6
#define HASHMAL_PIN_LORA_RST   4
#define HASHMAL_PIN_LORA_INT   5
#define HASHMAL_PIN_LORA_EN    46     /* Enable del modulo LoRa */

/* ==================================================================
 * I2C BUS
 *
 * Compartido por: Keyboard, Touch, Gyroscope, Light sensor,
 * BQ27220 (fuel gauge), BQ25896 (charger).
 * ================================================================== */
#define HASHMAL_PIN_I2C_SDA    13
#define HASHMAL_PIN_I2C_SCL    14

/* ==================================================================
 * KEYBOARD — TCA8418 (I2C 0x34)
 *
 * "ufitju sheʻarayikh tamid" -- Isaias 60:11
 * "Tus PUERTAS estaran abiertas CONTINUAMENTE" -- El teclado es
 * la puerta de entrada del operador al Mishkan.
 * ================================================================== */
#define HASHMAL_I2C_ADDR_KEYBOARD  0x34
#define HASHMAL_PIN_KEYBOARD_INT   15
#define HASHMAL_PIN_KEYBOARD_LED   42

/* ==================================================================
 * TOUCH — CST328 (I2C 0x1A)
 * ================================================================== */
#define HASHMAL_I2C_ADDR_TOUCH     0x1A
#define HASHMAL_PIN_TOUCH_INT      12
#define HASHMAL_PIN_TOUCH_RST      45

/* ==================================================================
 * SD CARD
 * ================================================================== */
#define HASHMAL_PIN_SD_CS      48

/* ==================================================================
 * GPS
 * ================================================================== */
#define HASHMAL_PIN_GPS_RXD    44
#define HASHMAL_PIN_GPS_TXD    43
#define HASHMAL_PIN_GPS_PPS    1

/* ==================================================================
 * ENABLE PINS — Control de alimentacion por modulo
 *
 * "veheʻelitá et hannerot" -- Exodo 25:37
 * "Encenderas las lamparas" -- Cada modulo tiene su propio
 * interruptor de encendido. No se encienden todos a la vez.
 * ================================================================== */
#define HASHMAL_PIN_GPS_EN     39     /* Enable GPS */
#define HASHMAL_PIN_1V8_EN     38     /* Enable gyroscope */
#define HASHMAL_PIN_6609_EN    41     /* Enable 7682 modem */
/* HASHMAL_PIN_LORA_EN ya definido arriba como 46 */

/* ==================================================================
 * OTROS
 * ================================================================== */
#define HASHMAL_PIN_BOOT       0
#define HASHMAL_PIN_MOTOR      2

/* I2S (Audio PCM5102A) */
#define HASHMAL_PIN_I2S_BCLK   7
#define HASHMAL_PIN_I2S_DOUT   8
#define HASHMAL_PIN_I2S_LRC    9

/* Light sensor LTR-553ALS */
#define HASHMAL_I2C_ADDR_LTR553  0x23
#define HASHMAL_PIN_ALS_INT      16

/* Gyroscope BHI260AP */
#define HASHMAL_I2C_ADDR_GYRO    0x28
#define HASHMAL_PIN_GYRO_INT     21

/* Mic */
#define HASHMAL_PIN_MIC_DATA     17
#define HASHMAL_PIN_MIC_CLOCK    18
