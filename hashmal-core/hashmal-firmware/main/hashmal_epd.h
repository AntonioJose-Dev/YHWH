/**
 * ===================================================================
 * HASHMAL-FIRMWARE — DRIVER E-PAPER DISPLAY
 * hashmal_epd.h
 * ===================================================================
 *
 * "veʻasitá MASAKH lefetaj haʼohel tejélet veʼargamán vetholaʻat
 *  shaní veshésh mashezár maʻaséh roqém." -- Exodo 26:36
 * "Haras una CORTINA para la entrada de la tienda, de azul,
 *  purpura, carmesi y lino torcido, obra de bordador."
 *
 * La pantalla e-paper es el MASAKH (cortina) del Mishkan digital:
 * la cara visible que muestra al mundo el estado de la red sin
 * revelar lo que hay detras del velo.
 *
 * Hardware: GDEQ031T10 (Good Display), 3.1", 240x320, B/W
 * Controlador: UC8253 (UltraChip)
 * Interfaz: SPI (4-wire) + BUSY pin
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 * ===================================================================
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

/**
 * hashmal_epd_init -- Inicializar SPI bus + display e-paper
 *
 * Configura GPIO (CS, DC, BUSY), inicializa SPI master,
 * envia secuencia de inicio al UC8253.
 *
 * @return ESP_OK si todo bien, ESP_FAIL si error de SPI o BUSY timeout
 */
esp_err_t hashmal_epd_init(void);

/**
 * hashmal_epd_clear -- Limpiar pantalla completa
 *
 * @param color  0x00 = negro, 0xFF = blanco
 */
void hashmal_epd_clear(uint8_t color);

/**
 * hashmal_epd_draw_char -- Dibujar un caracter en el framebuffer
 *
 * @param x      Coordenada X (pixeles, 0-239)
 * @param y      Coordenada Y (pixeles, 0-319)
 * @param c      Caracter ASCII (32-126)
 * @param scale  Factor de escala (1=5x7, 2=10x14, 3=15x21, etc.)
 * @param black  true=negro sobre blanco, false=blanco sobre negro
 */
void hashmal_epd_draw_char(int x, int y, char c, uint8_t scale, bool black);

/**
 * hashmal_epd_draw_string -- Dibujar cadena de texto en el framebuffer
 *
 * @param x      Coordenada X inicial
 * @param y      Coordenada Y inicial
 * @param str    Cadena de texto null-terminated
 * @param scale  Factor de escala
 * @param black  true=negro sobre blanco
 */
void hashmal_epd_draw_string(int x, int y, const char *str,
                              uint8_t scale, bool black);

/**
 * hashmal_epd_refresh -- Enviar framebuffer al display y refrescar
 *
 * Envia los datos del framebuffer interno al UC8253 y ejecuta
 * un ciclo completo de refresco. La pantalla e-paper tarda
 * ~2-4 segundos en actualizar. Bloquea hasta que BUSY baje.
 */
void hashmal_epd_refresh(void);

/**
 * hashmal_epd_sleep -- Poner display en deep sleep
 *
 * Reduce consumo al minimo. Requiere hashmal_epd_init() para
 * despertar de nuevo.
 */
void hashmal_epd_sleep(void);

/* ==================================================================
 * PRIMITIVAS GRAFICAS — Exodo 31:4
 *
 * "lemajshevet majashot labadah uvazahav uvajneshet"
 *   -- Exodo 31:4
 * "para pensar DISENOS ARTISTICOS en oro, plata y bronce"
 *
 * El Padre llenó a Betsalél de jalashévet (H2803) para grabar
 * formas. Aquí: cada pixel es una tabla puesta en pie (Ex 26:15).
 *
 * 2 testigos:
 *   Ex 31:4 — jalashévet majashot: diseño artístico deliberado
 *   Ex 26:1 — querubines TEJIDOS en la tela: gráficos integrados
 * ================================================================== */

/** Dibujar un pixel en el framebuffer. */
void hashmal_epd_draw_pixel(int x, int y, bool black);

/** Rellenar rectangulo solido (Ex 31:4 — relleno como obra de artista). */
void hashmal_epd_fill_rect(int x, int y, int w, int h, bool black);

/** Dibujar solo el contorno de un rectangulo (Ex 26:16 — medidas exactas). */
void hashmal_epd_draw_rect(int x, int y, int w, int h, bool black);

/** Linea horizontal (Ex 26:1 — hilo tendido de lado a lado). */
void hashmal_epd_draw_hline(int x, int y, int len, bool black);

/** Linea vertical (Ex 26:15 — tablas puestas EN PIE). */
void hashmal_epd_draw_vline(int x, int y, int len, bool black);
