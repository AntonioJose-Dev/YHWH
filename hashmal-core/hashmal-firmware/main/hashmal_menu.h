/**
 * ===================================================================
 * HASHMAL-FIRMWARE — MENÚ DEL T-DECK PRO
 * hashmal_menu.h
 * ===================================================================
 *
 * "ufitajú sheʻarayikh tamíd — yomam valayilah lo yissgárú"
 *   — Isaías 60:11
 * "Tus PUERTAS estarán abiertas CONTINUAMENTE — de día y de noche
 *  no se cerrarán."
 *
 * El menú es la puerta visible del Mishkán digital. El operador
 * la abre para ver el estado de la red, leer la Palabra del Padre,
 * controlar el WiFi y ajustar el nodo. Nunca se cierra mientras
 * el Mishkán esté en pie.
 *
 * Hardware: GDEQ031T10 (240x320 B/W) + TCA8418 (teclado I2C 0x34)
 *           + CST328 (touch I2C 0x1A, INT=GPIO12, RST=GPIO45)
 * Navegación: toque en icono (táctil) | teclas i/k/ENTER (físico)
 *
 * Autor: Antonio José Marín Soto
 * Proyecto: HASHMAL-CORE
 * ===================================================================
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ==================================================================
 * ÍTEMS DEL MENÚ
 * Is 60:11 — puertas abiertas siempre, de día y de noche
 * ================================================================== */
typedef enum {
    HASHMAL_MENU_MENSAJES  = 0,  /* Mensajes LoRa recibidos         */
    HASHMAL_MENU_DEVAR     = 1,  /* Devár YHWH — la Palabra         */
    HASHMAL_MENU_MOEDIM    = 2,  /* Moedím — tiempos señalados      */
    HASHMAL_MENU_WIFI      = 3,  /* WiFi AP — encender/apagar       */
    HASHMAL_MENU_RED       = 4,  /* Estado de la red LoRa           */
    HASHMAL_MENU_BATERIA   = 5,  /* Batería y energía               */
    HASHMAL_MENU_AJUSTES   = 6,  /* Ajustes del nodo                */
    HASHMAL_MENU_COUNT     = 7,
} hashmal_menu_item_t;

/* ==================================================================
 * API PÚBLICA
 * ================================================================== */

/**
 * Inicializa el menú: EPD + teclado TCA8418.
 * Llama a hashmal_epd_init() internamente si no está ya inicializado.
 * Dibuja la pantalla de inicio y lanza la task de menú.
 *
 * 2 testigos:
 *   Is 60:11 — puertas abiertas continuamente
 *   Éx 26:36 — masakh: la cortina visible del Mishkán
 */
void hashmal_menu_init(void);

/**
 * Redibuja la pantalla actual (útil tras recibir datos nuevos).
 * Se puede llamar desde cualquier task — usa mutex interno.
 */
void hashmal_menu_refrescar(void);

#ifdef __cplusplus
}
#endif
