/*
 * ===================================================================
 * PIEZA 08 - ACEITE PERPETUO - aceite_anan_esh.c
 * Añadidura 07: ANAN VE'ESH - NUBE Y FUEGO
 * Exodo 13:21-22 + Numeros 9:15-23 + Exodo 40:34-38
 * ===================================================================
 *
 * "vaYHWH holekh lifneihem yomam be'AMMUD ANAN lanjotam hadderekh
 *  velaylah be'AMMUD ESH leair lahem lalekhet yomam valaylah."
 *   - Exodo 13:21
 * "YHWH iba delante de ellos: de dia en columna de NUBE para
 *  guiarlos por el camino, y de noche en columna de FUEGO para
 *  alumbrarles - para marchar de dia y de noche."
 *
 * "LO YAMISH ammud he'anan yomam ve'ammud ha'esh laylah lifney
 *  ha'am." - Exodo 13:22
 * "NO SE APARTABA la columna de nube de dia ni la columna de
 *  fuego de noche, delante del pueblo."
 *
 * "beyom HE'ALOT he'anan me'al hammishkan YIS'U benei Yisrael...
 *  uvmiqom asher YISHKON sham he'anan sham YAJANU benei Yisrael."
 *   - Numeros 9:17
 * "Cuando la nube se ALZABA sobre el Mishkan, los hijos de Israel
 *  MARCHABAN; en el lugar donde la nube se POSABA alli ACAMPABAN."
 *
 * "'al pi YHWH yajanu ve'al pi YHWH yis'au." - Numeros 9:23
 * "Por mandato de YHWH acampaban y por mandato de YHWH marchaban."
 *
 * --- EXEGESIS OPERATIVA ---
 *
 *   anan (H6051)   NUBE. Cubierta que protege y oculta a la vez.
 *                   De dia, bloquea el sol - protege del calor.
 *                   Senal visible por encima del campamento.
 *
 *   esh (H784)     FUEGO. Luz propia cuando no hay sol. Senal
 *                   visible en la oscuridad.
 *
 *   lo yamish (H4185) "NO SE APARTABA". La guia no se toma
 *                   vacaciones - continua.
 *
 *   yis'u / yajanu (H5265 / H2583)  MARCHAR / ACAMPAR. Los dos
 *                   estados binarios del campamento. Nunca ambos.
 *
 * --- MODELO ---
 *
 * Dos ejes ortogonales:
 *
 *   MODO (dia/noche):  viene de Ashmurot (Añadidura 04)
 *                      - DIA   = hashmal_ashmurot_actual() == NINGUNA
 *                      - NOCHE = cualquier vigilia (PRIMERA/MEDIA/MANANA)
 *
 *   MOVIMIENTO (posa/alza):  viene de amenaza de red
 *                      - ALZA si shaon nivel >= HAMON (Añadidura 01)
 *                              o vecinos_mesh == 0 (aislado)
 *                      - POSA en cualquier otro caso
 *
 * Lo yamish (Ex 13:22): SIEMPRE hay un modo Y un movimiento
 * publicados. Si alguna consulta falla, default conservador
 * (POSA + VERDE + BREATH).
 *
 * --- HARDWARE HINTS ---
 *
 * El firmware no pinta LEDs ni vibra - solo publica intencion:
 *
 *   POSA diurna:   color VERDE    + patron BREATH
 *   POSA nocturna: color VERDE    + patron STATIC  (fuego steady)
 *   ALZA diurna:   color AMBAR    + patron VIBRATE_UP
 *   ALZA nocturna: color ROJO     + patron VIBRATE_UP
 *
 * Driver LED / haptic (fase 2) consume estos hints.
 *
 * --- MIQLAT DIRECTIONAL (placeholder fase 1) ---
 *
 * Spec pedia "direccion de nodos refugio cercanos". Sin broadcast
 * de posicion (Pieza 04 fase 2) no hay bearings entre nodos. Fase
 * 1 expone:
 *   - miqlat_local_activo: este mismo nodo es ciudad de refugio
 *   - miqlat_refugiados:   cuantos hospedamos aqui
 *
 * Futura fase wireara hashmal_comm_broadcast con posicion + contara
 * miqlatim vecinos.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_power.h              API publica (types + prototipos)
 *   hashmal_power_internal.h     ahora_ms (timestamp)
 *   hashmal_cache.h              hashmal_moedim_tiempo_t
 *   hashmal_comm.h               vecinos_contar
 *   hashmal_healthcheck.h        parokhet_indice
 *   hashmal_perimeter.h          miqlat_designado + refugiados_contar
 *   esp_log.h (ORO EGIPTO)       ANAN_LOG_*
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <inttypes.h>

#include "esp_log.h"

#include "hashmal_power.h"
#include "hashmal_power_internal.h"
#include "hashmal_cache.h"
#include "hashmal_comm.h"
#include "hashmal_healthcheck.h"
#include "hashmal_perimeter.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *ANAN_TAG = "hashmal.aceite.anan_esh";

#define ANAN_LOG_I(fmt, ...)  ESP_LOGI(ANAN_TAG, fmt, ##__VA_ARGS__)
#define ANAN_LOG_W(fmt, ...)  ESP_LOGW(ANAN_TAG, fmt, ##__VA_ARGS__)
#define ANAN_LOG_E(fmt, ...)  ESP_LOGE(ANAN_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * Estado interno - la senal publicada actual.
 *
 * Default conservador: POSA + VERDE + BREATH. Asi "lo yamish" se
 * cumple desde el arranque: siempre hay senal valida publicada.
 * ================================================================== */
static hashmal_anan_senal_t g_senal = {
    .modo                = HASHMAL_ANAN_MODO_DIA,
    .movimiento          = HASHMAL_ANAN_MOV_POSA,
    .color_hint          = HASHMAL_ANAN_COLOR_VERDE,
    .patron_hint         = HASHMAL_ANAN_PATRON_BREATH,
    .vecinos_mesh        = 0,
    .shaon_indice        = 0,
    .miqlat_local_activo = false,
    .miqlat_refugiados   = 0,
    .timestamp_ms        = 0,
};

/* ==================================================================
 * Helpers estaticos
 * ================================================================== */

/* Decide modo diurno/nocturno a partir de Ashmurot. Ashmurot
 * NINGUNA = dia; cualquier vigilia = noche. Misma fuente que el
 * resto del firmware. */
static hashmal_anan_modo_t decidir_modo(void)
{
    hashmal_ashmura_t a = hashmal_ashmurot_actual();
    return (a == HASHMAL_ASHMURA_NINGUNA)
           ? HASHMAL_ANAN_MODO_DIA
           : HASHMAL_ANAN_MODO_NOCHE;
}

/* Decide movimiento POSA/ALZA con criterio binario fase 1:
 *   ALZA si shaon >= HAMON o vecinos == 0.
 *   POSA en cualquier otro caso.
 * Devuelve tambien los valores leidos para la struct publicada. */
static hashmal_anan_movimiento_t decidir_movimiento(uint8_t vecinos,
                                                     uint8_t indice_shaon,
                                                     hashmal_hc_shaon_nivel_t nivel)
{
    bool amenaza = false;

    /* Shaon agitado (>= HAMON) es señal de que la zona no esta calma. */
    if (nivel == HASHMAL_HC_SHAON_HAMON
     || nivel == HASHMAL_HC_SHAON_SHAON
     || nivel == HASHMAL_HC_SHAON_RAASH) {
        amenaza = true;
    }

    /* Red fragmentada: nadie alrededor. */
    if (vecinos == 0u) amenaza = true;

    (void)indice_shaon;  /* reservado para logica mas fina fase 2 */

    return amenaza ? HASHMAL_ANAN_MOV_ALZA : HASHMAL_ANAN_MOV_POSA;
}

/* Mapea (modo, movimiento) -> (color, patron). Tabla central. */
static void mapear_hints(hashmal_anan_modo_t modo,
                         hashmal_anan_movimiento_t mov,
                         hashmal_anan_color_t *out_color,
                         hashmal_anan_patron_t *out_patron)
{
    if (mov == HASHMAL_ANAN_MOV_POSA) {
        *out_color  = HASHMAL_ANAN_COLOR_VERDE;
        /* De dia: BREATH (respiracion suave). De noche: STATIC
         * (fuego firme, Ex 13:21 "leair lahem"). */
        *out_patron = (modo == HASHMAL_ANAN_MODO_DIA)
                      ? HASHMAL_ANAN_PATRON_BREATH
                      : HASHMAL_ANAN_PATRON_STATIC;
    } else {
        /* ALZA - yis'u. De dia ambar (advertencia visible); de
         * noche rojo (alerta nocturna). Patron vibracion
         * ascendente en ambos (Nm 9:17 he'alot = subida). */
        *out_color  = (modo == HASHMAL_ANAN_MODO_DIA)
                      ? HASHMAL_ANAN_COLOR_AMBAR
                      : HASHMAL_ANAN_COLOR_ROJO;
        *out_patron = HASHMAL_ANAN_PATRON_VIBRATE_UP;
    }
}

static const char *nombre_modo(hashmal_anan_modo_t m)
{
    return (m == HASHMAL_ANAN_MODO_DIA) ? "DIA (anan)" : "NOCHE (esh)";
}

static const char *nombre_mov(hashmal_anan_movimiento_t m)
{
    return (m == HASHMAL_ANAN_MOV_POSA) ? "POSA (yajanu)" : "ALZA (yis'u)";
}

/* ==================================================================
 * API - hashmal_anan_actualizar
 *
 * Consulta cuatro fuentes y compone la senal. "Lo yamish" (Ex 13:22)
 * garantizado: aunque una fuente falle, siempre quedan valores
 * razonables.
 * ================================================================== */
int hashmal_anan_actualizar(const hashmal_moedim_tiempo_t *t)
{
    if (t == NULL) {
        ANAN_LOG_E("actualizar: tiempo NULL");
        return HASHMAL_POWER_ERR_NULL;
    }

    /* 1) Modo: Ashmurot local (no puede fallar - siempre devuelve
     *    NINGUNA o una vigilia). */
    hashmal_anan_modo_t modo = decidir_modo();

    /* 2) Shaon: Parokhet (Pieza 12). Si rc != OK, usamos
     *    conservador (indice=0, nivel=DEMAMAH). */
    uint8_t                   indice = 0;
    hashmal_hc_shaon_nivel_t  nivel  = HASHMAL_HC_SHAON_DEMAMAH;
    int rc_pk = hashmal_hc_parokhet_indice(&indice, &nivel);
    if (rc_pk != HASHMAL_HC_OK) {
        indice = 0;
        nivel  = HASHMAL_HC_SHAON_DEMAMAH;
    }

    /* 3) Vecinos: Menora (Pieza 04). Si rc != OK, asumimos 0
     *    (conservador - fuerza ALZA si realmente no sabemos). */
    uint8_t vecinos = 0;
    (void)hashmal_comm_vecinos_contar(&vecinos);

    /* 4) Miqlat local (Pieza 07). */
    bool    miqlat_local = hashmal_perimeter_miqlat_designado();
    uint8_t refugiados   = 0;
    (void)hashmal_perimeter_miqlat_refugiados_contar(&refugiados);

    /* 5) Movimiento + hints. */
    hashmal_anan_movimiento_t mov   = decidir_movimiento(vecinos, indice, nivel);
    hashmal_anan_color_t      color = HASHMAL_ANAN_COLOR_VERDE;
    hashmal_anan_patron_t     patron = HASHMAL_ANAN_PATRON_BREATH;
    mapear_hints(modo, mov, &color, &patron);

    /* 6) Publicar struct atomica. Lo yamish: desde aqui en adelante
     *    el getter siempre encuentra una senal valida. */
    g_senal.modo                = modo;
    g_senal.movimiento          = mov;
    g_senal.color_hint          = color;
    g_senal.patron_hint         = patron;
    g_senal.vecinos_mesh        = vecinos;
    g_senal.shaon_indice        = indice;
    g_senal.miqlat_local_activo = miqlat_local;
    g_senal.miqlat_refugiados   = refugiados;
    g_senal.timestamp_ms        = hashmal_power_ahora_ms();

    /* Nm 9:23 'al pi YHWH - mandato publicado, no ejecutado aqui. */
    ANAN_LOG_I("anan/esh: %s + %s -> color=%d patron=%d "
               "(vecinos=%u shaon=%u miqlat_local=%d refugiados=%u)",
               nombre_modo(modo), nombre_mov(mov),
               (int)color, (int)patron,
               (unsigned)vecinos, (unsigned)indice,
               (int)miqlat_local, (unsigned)refugiados);
    return HASHMAL_POWER_OK;
}

/* ==================================================================
 * API - hashmal_anan_senal_obtener
 * ================================================================== */
int hashmal_anan_senal_obtener(hashmal_anan_senal_t *out)
{
    if (out == NULL) return HASHMAL_POWER_ERR_NULL;
    memcpy(out, &g_senal, sizeof(*out));
    return HASHMAL_POWER_OK;
}

/* ==================================================================
 *
 *   "YHWH holekh lifneihem... 'ammud ANAN yomam ve'ammud ESH laylah."
 *     - Exodo 13:21
 *
 *   "LO YAMISH ammud he'anan yomam ve'ammud ha'esh laylah." - Ex 13:22
 *
 *   "'al pi YHWH YAJANU ve'al pi YHWH YIS'AU." - Numeros 9:23
 *
 *   La columna nunca se ausenta. De dia nube que guia; de noche
 *   fuego que alumbra. Ni un solo instante el pueblo estuvo sin
 *   senal - ni en la travesia del mar, ni en los 40 anos del
 *   desierto. El firmware copia el patron: el default conservador
 *   (POSA + VERDE + BREATH) garantiza que al arrancar ya hay
 *   columna; cada actualizar refina lo publicado. Lo yamish.
 *
 * ================================================================== */
