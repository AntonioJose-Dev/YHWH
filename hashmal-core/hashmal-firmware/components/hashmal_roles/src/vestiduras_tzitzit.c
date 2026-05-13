/*
 * ===================================================================
 * PIEZA 09 - VESTIDURAS - vestiduras_tzitzit.c
 * Añadidura 06: EL RECORDATORIO VISUAL
 * Numeros 15:37-41 + Deuteronomio 6:4-9 + Deuteronomio 11:18-21
 * ===================================================================
 *
 * "dabber el benei Yisrael veamarta aleihem ve'asu lahem TZITZIT
 *  'al kanfei vigdeihem ledorotam venatenu 'al TZITZIT hakanaf
 *  PETIL TEKHELET." - Numeros 15:38
 * "Que se hagan FLECOS en los bordes de sus vestidos... y pongan
 *  en el fleco del borde un CORDON DE AZUL."
 *
 * "veayah lakhem leTZITZIT uREITEM oto uZKHARTEM et kol mitzvot
 *  YHWH va'asitem otam." - Numeros 15:39
 * "Y os sera por FLECO, y los VEREIS y RECORDAREIS todos los
 *  mandamientos de YHWH, y los HAREIS."
 *
 * "shema Yisrael YHWH Eloheinu YHWH echad... uqsartam le'ot 'al
 *  yadekha veayu leTOTAFOT ben 'eineikha." - Dt 6:4-8
 * "Escucha Israel... los ataras como SENAL en tu mano y seran
 *  FRONTALES entre tus ojos."
 *
 * "ukhtavtam 'al MEZUZOT beitekha uvish'arekha." - Dt 6:9
 * "Los escribiras en los POSTES de tu casa."
 *
 * --- EXEGESIS OPERATIVA ---
 *
 *   tzitzit (TsYTsYT, H6734)  FLECO. Raiz tzits = flor, brote.
 *                              Algo pequeno pero VISIBLE y vivo.
 *
 *   petil tekhelet (H6616+H8504) HILO AZUL. El azul del cielo -
 *                              color que apunta hacia arriba,
 *                              recordatorio de la fuente.
 *
 *   u-reitem oto (H7200+H853+H2142)  LO VEREIS. La accion es
 *                              visual, no auditiva. El tzitzit no
 *                              suena; esta presente a la vista.
 *
 * --- DISTINCION CON tzitz (vestiduras_tzitz.c) ---
 *
 *   tzitz (TsYTs, H6731)       LA PLACA DE ORO en la frente del
 *                              Sumo Sacerdote (Ex 28:36 "qodesh
 *                              laYHWH"). Unica, oro, frente.
 *   tzitzit (H6734)            FLECOS en los bordes del manto de
 *                              cualquier israelita (Nm 15:38).
 *                              Multiples, hilos, bordes.
 *
 * Parientes etimologicos (misma raiz TsYTs), propositos distintos.
 * tzitz identifica al que sirve; tzitzit recuerda al que recuerda.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   _actualizar(t)   Nm 15:39 ureitem - recomputa el frame (auto-
 *                    rota si paso 1h desde ultima rotacion)
 *   _rotar           avanza modo manualmente (boton T-Deck)
 *   _frame_obtener   lectura pura del frame para driver e-paper
 *
 * 5 modos ciclicos:
 *   VERSICULO - rotacion diaria por doy % 12
 *   PRINCIPIO - "SHEMA YISRAEL" fijo (fase 1; config futuro)
 *   ROL       - nodo_id=0 (self) + rol sacerdotal leido via Pieza 09
 *   MOED      - Añadidura 03 hashmal_moedim_actual
 *   SHAON     - Añadidura 01 hashmal_hc_parokhet_indice
 *
 * --- HILO AZUL (petil tekhelet) ---
 *
 * 1 char prefijo independiente del modo. Refleja nivel de shaon:
 *   DEMAMAH/SHAQAT (ambiente silencioso)  -> '|'
 *   HAMON/SHAON    (agitado)              -> '~'
 *   RAASH          (temblor, 1 Re 19:11)  -> '!'
 *   sin_dato                              -> ' '
 *
 * Driver de e-paper (fase 2) pintara ese char en tekhelet (azul);
 * fase 1 es ASCII visible.
 *
 * --- NO PINTA PIXELES ---
 *
 * Este modulo PUBLICA contenido (struct frame). El driver de
 * pantalla (futuro) lee y renderiza. Separacion de capas: Nm 15:39
 * ureitem es la VISTA; este archivo prepara el contenido que se
 * vera.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_roles.h            API publica (types + prototipos)
 *   hashmal_cache.h            moedim_actual + hashmal_moedim_tiempo_t
 *   hashmal_healthcheck.h      parokhet_indice (hilo azul + modo shaon)
 *   esp_log.h (ORO EGIPTO)     TZTZT_LOG_*
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "esp_log.h"

#include "hashmal_roles.h"
#include "hashmal_cache.h"          /* moedim + hashmal_moedim_tiempo_t */
#include "hashmal_healthcheck.h"    /* parokhet_indice para hilo azul + SHAON */

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *TZTZT_TAG = "hashmal.roles.tzitzit";

#define TZTZT_LOG_I(fmt, ...)  ESP_LOGI(TZTZT_TAG, fmt, ##__VA_ARGS__)
#define TZTZT_LOG_W(fmt, ...)  ESP_LOGW(TZTZT_TAG, fmt, ##__VA_ARGS__)
#define TZTZT_LOG_E(fmt, ...)  ESP_LOGE(TZTZT_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * Estado interno
 * ================================================================== */
static hashmal_tzitzit_frame_t g_frame = {
    .modo                      = HASHMAL_TZITZIT_MODO_VERSICULO,
    .hilo_azul                 = ' ',
    .texto                     = {0},
    .texto_len                 = 0,
    .timestamp_actualizado_ms  = 0,
};

/* Minutos absolutos desde el anio 0 (aproximado - ignora leap-year
 * secular; solo usamos DIFERENCIAS dentro de pocas horas). */
static uint64_t g_last_rotar_min = 0;

/* ==================================================================
 * Tabla de versiculos rotados por doy % N. Un clasico por indice.
 * ASCII sin caracteres especiales - compatible con e-paper basico.
 * ================================================================== */
#define TZITZIT_VERSICULOS_N  12u
static const char *g_versiculos[TZITZIT_VERSICULOS_N] = {
    "Dt 6:4  Shema Yisrael YHWH Eloheinu YHWH echad",
    "Sal 23:1 YHWH ro'i lo echsar",
    "Sal 1:1  Ashrei haish asher lo halakh",
    "Pr 3:5   Betach el YHWH bekhol libbekha",
    "Is 40:31 Veqoei YHWH yajalifu koach",
    "Sal 27:1 YHWH ori veyish'i",
    "Sal 119:105 Ner leragli dvarekha",
    "Dt 30:19 uVacharta bachayim",
    "Miq 6:8  Asot mishpat ve'ahavat chesed",
    "Hab 2:4  Tzaddik be'emunato yichyeh",
    "Is 26:3  Yetzer samukh tirtz'or shalom",
    "Sal 46:10 Harpu ud'u ki anokhi Elohim",
};

/* ==================================================================
 * Helpers de tiempo absoluto (para rotacion)
 * ================================================================== */
static int tzitzit_doy(int y, int m, int d)
{
    static const int acc[] = {0, 31, 59, 90, 120, 151, 181,
                              212, 243, 273, 304, 334};
    int doy = acc[m - 1] + d;
    bool bisiesto = ((y % 4 == 0) && (y % 100 != 0)) || (y % 400 == 0);
    if (bisiesto && m > 2) doy += 1;
    return doy;
}

/* Minutos absolutos aproximados. Ignora ajuste secular fino - solo
 * se usa para comparar diferencias de menos de 1 dia. */
static uint64_t tiempo_a_minutos(const hashmal_moedim_tiempo_t *t)
{
    return (uint64_t)t->year * 525600ull
         + (uint64_t)tzitzit_doy(t->year, t->month, t->day) * 1440ull
         + (uint64_t)t->hour * 60ull
         + (uint64_t)t->minute;
}

/* ==================================================================
 * Hilo azul - petil tekhelet (Nm 15:38)
 *
 * Consulta parokhet_indice y mapea el nivel shaon a 1 char.
 * ================================================================== */
static char computar_hilo_azul(void)
{
    uint8_t                  indice = 0;
    hashmal_hc_shaon_nivel_t nivel  = HASHMAL_HC_SHAON_DEMAMAH;
    int rc = hashmal_hc_parokhet_indice(&indice, &nivel);
    if (rc != HASHMAL_HC_OK) return ' ';  /* sin dato */

    if (nivel == HASHMAL_HC_SHAON_RAASH)    return '!';
    if (nivel >= HASHMAL_HC_SHAON_HAMON)    return '~';
    return '|';   /* DEMAMAH / SHAQAT - silencioso */
}

/* ==================================================================
 * Generadores de contenido por modo (5)
 *
 * Cada uno escribe el texto en el buffer y devuelve la longitud
 * efectiva. No incluyen el hilo azul - el frame lo lleva aparte.
 * ================================================================== */

static int contenido_versiculo(const hashmal_moedim_tiempo_t *t,
                                char *buf, size_t buf_max)
{
    int doy = tzitzit_doy(t->year, t->month, t->day);
    const char *v = g_versiculos[(uint32_t)doy % TZITZIT_VERSICULOS_N];
    int n = snprintf(buf, buf_max, "%s", v);
    return (n < 0) ? 0 : n;
}

static int contenido_principio(char *buf, size_t buf_max)
{
    /* Fase 1: SHEMA como principio fijo. Fase 2 expondra config
     * para que el operador/portador elija su principio. */
    int n = snprintf(buf, buf_max,
                     "SHEMA YISRAEL - Dt 6:4");
    return (n < 0) ? 0 : n;
}

static const char *nombre_rol(hashmal_roles_tipo_rol_t r)
{
    switch (r) {
    case HASHMAL_ROLES_ROL_ISRAELITA:   return "ISRAELITA";
    case HASHMAL_ROLES_ROL_LEVI:        return "LEVI";
    case HASHMAL_ROLES_ROL_KOHEN:       return "KOHEN";
    case HASHMAL_ROLES_ROL_KOHEN_GADOL: return "KOHEN GADOL";
    case HASHMAL_ROLES_ROL_NINGUNO:
    default:                             return "SIN ROL";
    }
}

static int contenido_rol(char *buf, size_t buf_max)
{
    /* Fase 1: portador = nodo_id 0 por convencion (el nodo local
     * se registra en el slot 0 por Pieza 09 existente).  */
    hashmal_roles_estado_roles_t st;
    memset(&st, 0, sizeof(st));
    hashmal_roles_err_t rc = hashmal_roles_estado(&st);
    const char *rol_str = (rc == HASHMAL_ROLES_OK)
                           ? nombre_rol(st.nodos[0].rol)
                           : "SIN DATO";
    int n = snprintf(buf, buf_max, "Portador nodo 0 | %s", rol_str);
    return (n < 0) ? 0 : n;
}

static const char *nombre_moed_corto(hashmal_moed_t m)
{
    switch (m) {
    case HASHMAL_MOED_SHABAT:     return "SHABAT (Lv 23:3)";
    case HASHMAL_MOED_PESAJ:      return "PESAJ (Lv 23:5)";
    case HASHMAL_MOED_SHAVUOT:    return "SHAVUOT (Lv 23:15)";
    case HASHMAL_MOED_YOM_TERUAH: return "YOM TERUAH (Lv 23:24)";
    case HASHMAL_MOED_YOM_KIPPUR: return "YOM KIPPUR (Lv 23:27)";
    case HASHMAL_MOED_SUKKOT:     return "SUKKOT (Lv 23:34)";
    case HASHMAL_MOED_JUBILEO:    return "JUBILEO (Lv 25:9)";
    case HASHMAL_MOED_NINGUNO:
    default:                       return "dia ordinario";
    }
}

static int contenido_moed(char *buf, size_t buf_max)
{
    hashmal_moed_t m = hashmal_moedim_actual();
    int n = snprintf(buf, buf_max, "Moed: %s", nombre_moed_corto(m));
    return (n < 0) ? 0 : n;
}

static const char *nombre_shaon(hashmal_hc_shaon_nivel_t n)
{
    switch (n) {
    case HASHMAL_HC_SHAON_DEMAMAH: return "DEMAMAH (1 Re 19:12)";
    case HASHMAL_HC_SHAON_SHAQAT:  return "SHAQAT (Is 30:15)";
    case HASHMAL_HC_SHAON_HAMON:   return "HAMON";
    case HASHMAL_HC_SHAON_SHAON:   return "SHAON";
    case HASHMAL_HC_SHAON_RAASH:   return "RAASH (1 Re 19:11)";
    default:                        return "desconocido";
    }
}

static int contenido_shaon(char *buf, size_t buf_max)
{
    uint8_t                   indice = 0;
    hashmal_hc_shaon_nivel_t  nivel  = HASHMAL_HC_SHAON_DEMAMAH;
    int rc = hashmal_hc_parokhet_indice(&indice, &nivel);
    if (rc != HASHMAL_HC_OK) {
        int n = snprintf(buf, buf_max, "Shaon: sin dato");
        return (n < 0) ? 0 : n;
    }
    int n = snprintf(buf, buf_max, "Shaon %u/100 - %s",
                     (unsigned)indice, nombre_shaon(nivel));
    return (n < 0) ? 0 : n;
}

/* ==================================================================
 * API - hashmal_tzitzit_rotar
 * ================================================================== */
hashmal_roles_err_t hashmal_tzitzit_rotar(void)
{
    uint32_t siguiente = ((uint32_t)g_frame.modo + 1u) % HASHMAL_TZITZIT_MODOS_TOTAL;
    g_frame.modo       = (hashmal_tzitzit_modo_t)siguiente;
    /* g_last_rotar_min se actualiza en _actualizar con el tiempo que
     * el caller provea - no tenemos clock propio aqui. */
    TZTZT_LOG_I("rotar -> modo %u (Nm 15:39 ureitem oto)",
                (unsigned)g_frame.modo);
    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 * API - hashmal_tzitzit_actualizar (Nm 15:39 ureitem)
 * ================================================================== */
hashmal_roles_err_t
hashmal_tzitzit_actualizar(const struct hashmal_moedim_tiempo *t)
{
    if (t == NULL) {
        TZTZT_LOG_E("actualizar: tiempo NULL");
        return HASHMAL_ROLES_ERR_NULL;
    }

    /* Auto-rotacion: si paso ROTAR_MS (1h) desde el ultimo rotar,
     * avanza modo antes de recomputar. Primer llamado inicializa
     * el timer sin rotar (g_last_rotar_min empieza en 0 -> delta
     * enorme solo en el arranque; lo tratamos como "acaba de
     * rotar" para no saltar en el primer tick). */
    uint64_t ahora_min = tiempo_a_minutos(t);
    if (g_last_rotar_min == 0) {
        g_last_rotar_min = ahora_min;
    } else {
        uint64_t elapsed_min = (ahora_min > g_last_rotar_min)
                               ? (ahora_min - g_last_rotar_min)
                               : 0ull;
        if (elapsed_min * 60000ull >= (uint64_t)HASHMAL_TZITZIT_ROTAR_MS) {
            (void)hashmal_tzitzit_rotar();
            g_last_rotar_min = ahora_min;
        }
    }

    /* Recalcular el hilo azul cada tick - refleja shaon EN VIVO. */
    g_frame.hilo_azul = computar_hilo_azul();

    /* Componer texto segun el modo actual. */
    int n = 0;
    switch (g_frame.modo) {
    case HASHMAL_TZITZIT_MODO_VERSICULO:
        n = contenido_versiculo(t, g_frame.texto, sizeof(g_frame.texto));
        break;
    case HASHMAL_TZITZIT_MODO_PRINCIPIO:
        n = contenido_principio(g_frame.texto, sizeof(g_frame.texto));
        break;
    case HASHMAL_TZITZIT_MODO_ROL:
        n = contenido_rol(g_frame.texto, sizeof(g_frame.texto));
        break;
    case HASHMAL_TZITZIT_MODO_MOED:
        n = contenido_moed(g_frame.texto, sizeof(g_frame.texto));
        break;
    case HASHMAL_TZITZIT_MODO_SHAON:
        n = contenido_shaon(g_frame.texto, sizeof(g_frame.texto));
        break;
    default:
        g_frame.texto[0] = '\0';
        n = 0;
        break;
    }
    if (n < 0) n = 0;
    if ((uint32_t)n >= HASHMAL_TZITZIT_TEXTO_MAX) {
        n = (int)(HASHMAL_TZITZIT_TEXTO_MAX - 1u);
    }
    g_frame.texto_len                = (uint16_t)n;
    g_frame.timestamp_actualizado_ms = ahora_min * 60000ull;

    TZTZT_LOG_I("actualizar modo=%u hilo='%c' texto=\"%.*s\"",
                (unsigned)g_frame.modo, g_frame.hilo_azul,
                (int)n, g_frame.texto);
    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 * API - hashmal_tzitzit_frame_obtener (lectura pura)
 * ================================================================== */
hashmal_roles_err_t hashmal_tzitzit_frame_obtener(hashmal_tzitzit_frame_t *out)
{
    if (out == NULL) return HASHMAL_ROLES_ERR_NULL;
    memcpy(out, &g_frame, sizeof(*out));
    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 *
 *   "ureitem oto uzkhartem et kol mitzvot YHWH." - Numeros 15:39
 *
 *   "uqsartam le'ot 'al yadekha." - Dt 6:8
 *
 *   "ukhtavtam 'al MEZUZOT beitekha." - Dt 6:9
 *
 *   El tzitzit no suena, no vibra, no pide atencion. Simplemente
 *   esta. En la vista. Siempre. Como la mezuza en el poste, como
 *   las totafot entre los ojos: presencia constante del recuerdo.
 *   El e-paper es justo esa superficie - mantiene la imagen sin
 *   consumir energia, como el fleco mantiene el hilo azul sin
 *   pedir nada. "La palabra visible" (Dt 6:9 ukhtavtam) encuentra
 *   su version digital aqui.
 *
 * ================================================================== */
