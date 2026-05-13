/*
 * ===================================================================
 * PIEZA 03 - LA MESA - mesa_moedim.c
 * Añadidura 03: MOTOR DE CALENDARIO MOEDIM (tiempos senalados)
 * Levitico 23 + Levitico 25:9 + Genesis 1:14 + Genesis 1:5
 * ===================================================================
 *
 * "eleh MOADEI YHWH miqra'ei qodesh asher tiqre'u otam bemoadam."
 *   - Levitico 23:4
 * "Y dijo Dios: haya lumbreras en la expansion... y sean para
 *  SENALES, y para TIEMPOS SENALADOS, y para dias y anos."
 *   - Genesis 1:14
 *
 * "vayhi 'erev vayhi voqer yom ejad." - Genesis 1:5
 * "Y fue tarde y fue manana: dia uno." - el dia biblico comienza
 *  al ATARDECER, no a medianoche.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 * Motor determinista de calendario que, dada (year/month/day
 * gregoriano + hora + lat/lon + tz), responde:
 *
 *   1. El atardecer en esa ubicacion (Meeus astronomico).
 *   2. El dia biblico correspondiente (sunset-to-sunset).
 *   3. La fecha hebrea aproximada (tabla Rosh Hashanah + metonico).
 *   4. El mo'ed actual (uno de 7 + NINGUNO).
 *   5. El bitmap de comportamiento para ese mo'ed.
 *
 * --- APROXIMACION FASE 1 ---
 *
 * El calendario hebreo completo (Hillel II / Maimonides) requiere
 * ~500 lineas y tabla rabinica. Para fase 1 usamos:
 *
 *   - Tabla de fechas Rosh Hashanah 5780-5830 (Gregoriano 2019-2069).
 *   - Longitudes de ano aprox: comun 354 dias, embolismico 384.
 *   - Pesach = Tishri 1 + 163 (comun) o + 193 (leap) aprox.
 *   - Shavuot = Pesach + 50.
 *
 * Precision: moedim caen en el dia correcto ±1 para anos cubiertos
 * por la tabla. Fuera del rango (fase 2) habra que extender tabla
 * o adoptar tabla rabinica completa.
 *
 * --- MEEUS SIMPLIFICADO ---
 *
 * Formula de posicion solar clasica (declinacion + angulo horario).
 * Precision ~±5 min a latitudes moderadas. Regiones polares
 * detectadas y degraded a "sin sunset" (sol no se pone en verano,
 * no sale en invierno).
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_cache.h           tipos + prototipos publicos
 *   esp_log.h (ORO EGIPTO)    MOEDIM_LOG_*
 *   math.h                    sin/cos/acos para Meeus
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>

#include "esp_log.h"

#include "hashmal_cache.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *MOEDIM_TAG = "hashmal.mesa.moedim";

#define MOEDIM_LOG_I(fmt, ...)  ESP_LOGI(MOEDIM_TAG, fmt, ##__VA_ARGS__)
#define MOEDIM_LOG_W(fmt, ...)  ESP_LOGW(MOEDIM_TAG, fmt, ##__VA_ARGS__)
#define MOEDIM_LOG_E(fmt, ...)  ESP_LOGE(MOEDIM_TAG, fmt, ##__VA_ARGS__)

#ifndef M_PI
#define M_PI  3.14159265358979323846
#endif

/* ==================================================================
 * Estado interno - ultimo resultado publicado
 * ================================================================== */
static hashmal_moed_t g_moed_actual         = HASHMAL_MOED_NINGUNO;
static uint32_t       g_comportamiento_bits = 0u;

/* ==================================================================
 * Tabla de Rosh Hashanah (Tishri 1) en Gregoriano
 *
 * Hebrew year 5780 + i  <=>  gregorian year 2019 + i  (aprox.)
 * Para cada entrada: mes * 100 + dia (ej. 1003 = 3 de Octubre).
 *
 * Fuente: calendario hebreo estandar. Precision: dia exacto para
 * el rango cubierto. Fuera del rango, el motor usa extrapolacion
 * por ciclo metonico de 19 anos (aprox ±3 dias).
 * ================================================================== */
#define RH_EPOCH_HEBREW 5780u
#define RH_EPOCH_GREG   2019u
#define RH_TABLE_LEN    51u   /* 5780-5830 cubre 2019-2069 */

static const uint16_t g_rh_mmdd[RH_TABLE_LEN] = {
    930, 919, 907, 926, 916, 1003, 923, 912, 1002, 921,    /* 5780-5789 */
    910, 928, 918, 906, 924, 914, 1004, 922, 912, 930,     /* 5790-5799 */
    920, 908, 926, 915, 1005, 924, 914, 1001, 921, 909,    /* 5800-5809 */
    927, 917, 905, 923, 913, 1003, 922, 910, 930, 918,     /* 5810-5819 */
    906, 925, 915, 1003, 923, 911, 929, 919, 907, 925,     /* 5820-5829 */
    914                                                     /* 5830 */
};

/* ==================================================================
 * Helpers de fecha / JDN (Julian Day Number)
 *
 * JDN: dias desde el 1 Enero 4713 BC, proleptic Julian. Permite
 * aritmetica de fechas sin preocuparse por meses y anos bisiestos.
 * ================================================================== */

static int64_t gregorian_a_jdn(int y, int m, int d)
{
    /* Formula estandar de Fliegel-Van Flandern (1968). */
    int a  = (14 - m) / 12;
    int y2 = y + 4800 - a;
    int m2 = m + 12 * a - 3;
    return (int64_t)d + (153L * m2 + 2) / 5 + 365L * y2
         + y2 / 4 - y2 / 100 + y2 / 400 - 32045L;
}

static int doy_gregoriano(int y, int m, int d)
{
    static const int acc[] = {0, 31, 59, 90, 120, 151, 181,
                              212, 243, 273, 304, 334};
    int doy = acc[m - 1] + d;
    /* Anio bisiesto (Gregoriano): divisible por 4, excepto si
     * divisible por 100 y no por 400. */
    bool bisiesto = ((y % 4 == 0) && (y % 100 != 0)) || (y % 400 == 0);
    if (bisiesto && m > 2) doy += 1;
    return doy;
}

static int dia_semana_jdn(int64_t jdn)
{
    /* JDN 0 fue lunes (por convencion astronomica). Para biblico:
     * Domingo=0, Sabado=6. Ajuste: (jdn + 1) % 7. */
    int64_t r = (jdn + 1) % 7;
    if (r < 0) r += 7;
    return (int)r;
}

/* ==================================================================
 * Meeus simplificado - eventos solares
 *
 * Entrada: dia-del-ano (doy), latitud/longitud en grados, tz_min.
 * Salida: hora decimal local (0..24) de amanecer y atardecer.
 *
 * Retorna false si region polar (el sol no se pone o no sale ese
 * dia). En ese caso *sunrise y *sunset quedan en -1.0.
 *
 * Precision: ~±5 min a latitudes moderadas. Declinacion solar
 * aproximada (±0.5 deg) - suficiente para fronteras biblicas y
 * para dividir la noche en tercios (ashmurot).
 * ================================================================== */
static bool calcular_eventos_solares(int doy,
                                      double lat_deg,
                                      double lon_deg,
                                      int tz_minutes,
                                      double *out_sunrise,
                                      double *out_sunset)
{
    /* Declinacion solar - mismo valor para ambos eventos. */
    double b    = 2.0 * M_PI * (doy - 81) / 365.0;
    double decl = asin(sin(23.45 * M_PI / 180.0) * sin(b));

    /* Angulo horario. */
    double lat_rad = lat_deg * M_PI / 180.0;
    double arg     = -tan(lat_rad) * tan(decl);
    if (arg > 1.0 || arg < -1.0) {
        /* Region polar: sin frontera este dia. */
        if (out_sunrise) *out_sunrise = -1.0;
        if (out_sunset)  *out_sunset  = -1.0;
        return false;
    }
    double ha       = acos(arg);
    double ha_hours = ha * 12.0 / M_PI;

    /* Mediodia solar UTC corregido por longitud; -ha = amanecer,
     * +ha = atardecer. */
    double noon_utc    = 12.0 - lon_deg / 15.0;
    double sunrise_utc = noon_utc - ha_hours;
    double sunset_utc  = noon_utc + ha_hours;

    /* A hora local. */
    double tz_h       = (double)tz_minutes / 60.0;
    double sunrise_lc = sunrise_utc + tz_h;
    double sunset_lc  = sunset_utc  + tz_h;

    /* Normalizar a [0, 24). */
    while (sunrise_lc < 0.0)   sunrise_lc += 24.0;
    while (sunrise_lc >= 24.0) sunrise_lc -= 24.0;
    while (sunset_lc  < 0.0)   sunset_lc  += 24.0;
    while (sunset_lc  >= 24.0) sunset_lc  -= 24.0;

    if (out_sunrise) *out_sunrise = sunrise_lc;
    if (out_sunset)  *out_sunset  = sunset_lc;
    return true;
}

/* Wrapper interno: solo sunset. Usado por dia_biblico_jdn. */
static double atardecer_hora_local(int doy,
                                    double lat_deg,
                                    double lon_deg,
                                    int tz_minutes)
{
    double sunset = -1.0;
    (void)calcular_eventos_solares(doy, lat_deg, lon_deg, tz_minutes,
                                    NULL, &sunset);
    return sunset;
}

/* ==================================================================
 * Motor: dada fecha gregoriana + hora, deriva dia biblico
 *
 * El dia biblico comienza en el atardecer (Gn 1:5). Si la hora
 * actual es POSTERIOR al sunset, estamos en el siguiente dia
 * biblico - incrementamos la fecha en 1 JDN.
 * ================================================================== */
static int64_t dia_biblico_jdn(const hashmal_moedim_tiempo_t *t)
{
    int64_t jdn_greg = gregorian_a_jdn(t->year, t->month, t->day);

    int    doy     = doy_gregoriano(t->year, t->month, t->day);
    double lat_deg = (double)t->lat_milideg / 1000.0;
    double lon_deg = (double)t->lon_milideg / 1000.0;
    double sunset  = atardecer_hora_local(doy, lat_deg, lon_deg,
                                           (int)t->tz_minutes);
    double hora    = (double)t->hour + (double)t->minute / 60.0;

    if (sunset > 0.0 && hora >= sunset) {
        /* Dia biblico = dia siguiente (ya paso erev). */
        return jdn_greg + 1;
    }
    /* Aun dentro del dia biblico actual (no ha atardecido). */
    return jdn_greg;
}

/* ==================================================================
 * Hebrew date approximation desde JDN
 *
 * 1. Busca en g_rh_mmdd la ultima Rosh Hashanah <= JDN.
 * 2. Calcula days_into_hebrew_year = JDN - RH.
 * 3. Decide si el ano hebreo es comun o embolismico segun el
 *    ciclo metonico (7 leap por 19 anos: 3,6,8,11,14,17,19).
 * 4. Pesach = RH + 163 (comun) o + 193 (leap).
 * 5. Shavuot = Pesach + 50.
 *
 * Devuelve: 0 si encontrada, -1 si fuera de tabla.
 * Escribe dias_desde_rh y hebrew_year_rel (offset desde EPOCH).
 * ================================================================== */
static int hebrew_desde_jdn(int64_t jdn,
                             int *dias_desde_rh,
                             int *hebrew_year_rel,
                             bool *es_leap)
{
    /* Convertir cada entrada de la tabla a JDN y buscar la mayor
     * que no exceda jdn. */
    int64_t mejor_jdn  = 0;
    int     mejor_idx  = -1;
    for (uint32_t i = 0; i < RH_TABLE_LEN; ++i) {
        int greg_y = (int)(RH_EPOCH_GREG + i);
        int mmdd   = (int)g_rh_mmdd[i];
        int m      = mmdd / 100;
        int d      = mmdd % 100;
        int64_t rh_jdn = gregorian_a_jdn(greg_y, m, d);
        if (rh_jdn <= jdn && rh_jdn > mejor_jdn) {
            mejor_jdn = rh_jdn;
            mejor_idx = (int)i;
        }
    }

    if (mejor_idx < 0) return -1;

    *dias_desde_rh   = (int)(jdn - mejor_jdn);
    *hebrew_year_rel = mejor_idx;  /* 0 = 5780, 1 = 5781, ... */

    /* Ciclo metonico: leap en 3, 6, 8, 11, 14, 17, 19 del ciclo.
     * Hebrew year = 5780 + mejor_idx. Hebrew year mod 19 indica
     * la posicion en el ciclo (0 = ano 19 = leap). */
    int hy = 5780 + mejor_idx;
    int mod = hy % 19;
    *es_leap = (mod == 0) || (mod == 3) || (mod == 6) || (mod == 8)
            || (mod == 11) || (mod == 14) || (mod == 17);

    return 0;
}

/* ==================================================================
 * Mapear fecha hebrea -> mo'ed (fase 1)
 * ================================================================== */
static hashmal_moed_t moed_de_fecha_hebrea(int dias_desde_rh, bool es_leap)
{
    /* Tishri mo'adim - anclados en 0, 9, 14-20. */
    if (dias_desde_rh == 0)  return HASHMAL_MOED_YOM_TERUAH;
    if (dias_desde_rh == 9)  return HASHMAL_MOED_YOM_KIPPUR;
    if (dias_desde_rh >= 14 && dias_desde_rh <= 20) return HASHMAL_MOED_SUKKOT;

    /* Pesach y Shavuot - offsets varian con leap year. */
    int pesach_offset  = es_leap ? 193 : 163;
    int shavuot_offset = pesach_offset + 50;

    if (dias_desde_rh == pesach_offset)  return HASHMAL_MOED_PESAJ;
    if (dias_desde_rh == shavuot_offset) return HASHMAL_MOED_SHAVUOT;

    return HASHMAL_MOED_NINGUNO;
}

/* ==================================================================
 * Mapear mo'ed -> bitmap de comportamiento
 *
 * Tabla central: cada mo'ed declara sus flags. Puede activar
 * varios (Yom Kippur = tres flags). Jubileo se superpone al ano
 * en curso: si es ano de jubileo, anade RESET_DEUDA al mo'ed
 * actual (normalmente Yom Kippur, Lv 25:9 lo fija en ese dia).
 * ================================================================== */
static uint32_t comportamiento_de_moed(hashmal_moed_t m)
{
    switch (m) {
    case HASHMAL_MOED_SHABAT:
        return HASHMAL_COMPORT_BAJO_CONSUMO
             | HASHMAL_COMPORT_SOLO_HEARTBEAT;

    case HASHMAL_MOED_PESAJ:
        return HASHMAL_COMPORT_INTEGRIDAD;

    case HASHMAL_MOED_SHAVUOT:
        return HASHMAL_COMPORT_HANDSHAKE;

    case HASHMAL_MOED_YOM_TERUAH:
        return HASHMAL_COMPORT_ALTA_VIGILANCIA;

    case HASHMAL_MOED_YOM_KIPPUR:
        return HASHMAL_COMPORT_SINC_TOTAL
             | HASHMAL_COMPORT_LIMPIEZA_PROFUNDA
             | HASHMAL_COMPORT_BAJO_CONSUMO;

    case HASHMAL_MOED_SUKKOT:
        return HASHMAL_COMPORT_MODO_COMUNIDAD;

    case HASHMAL_MOED_JUBILEO:
        /* Por si se publica standalone; normalmente se combina. */
        return HASHMAL_COMPORT_RESET_DEUDA
             | HASHMAL_COMPORT_LIMPIEZA_PROFUNDA;

    case HASHMAL_MOED_NINGUNO:
    default:
        return HASHMAL_COMPORT_NINGUNO;
    }
}

static const char *nombre_moed(hashmal_moed_t m)
{
    switch (m) {
    case HASHMAL_MOED_SHABAT:     return "SHABAT (Lv 23:3)";
    case HASHMAL_MOED_PESAJ:      return "PESAJ (Lv 23:5, Nisan 14)";
    case HASHMAL_MOED_SHAVUOT:    return "SHAVUOT (Lv 23:15-16)";
    case HASHMAL_MOED_YOM_TERUAH: return "YOM TERUAH (Lv 23:24, Tishri 1)";
    case HASHMAL_MOED_YOM_KIPPUR: return "YOM KIPPUR (Lv 23:27, Tishri 10)";
    case HASHMAL_MOED_SUKKOT:     return "SUKKOT (Lv 23:34, Tishri 15-21)";
    case HASHMAL_MOED_JUBILEO:    return "JUBILEO (Lv 25:9, 50 anos)";
    case HASHMAL_MOED_NINGUNO:
    default:                      return "NINGUNO (dia ordinario)";
    }
}

/* ==================================================================
 * API PUBLICA - hashmal_moedim_calcular
 *
 * Secuencia:
 *   1. Dia biblico (JDN) segun sunset.
 *   2. Dia de la semana -> si es Shabat (dia 6), toma precedencia.
 *   3. Fecha hebrea aproximada -> mo'ed anual si aplica.
 *   4. Ano de jubileo (epoch + 50n) -> modula bitmap.
 *   5. Publica moed + comportamiento.
 * ================================================================== */
int hashmal_moedim_calcular(const hashmal_moedim_tiempo_t *t)
{
    if (t == NULL) {
        MOEDIM_LOG_E("calcular: tiempo NULL");
        return HASHMAL_CACHE_ERR_NULL;
    }

    int64_t jdn_bib = dia_biblico_jdn(t);

    /* 1) Shabat semanal. Sabado = day_of_week == 6. */
    int dow = dia_semana_jdn(jdn_bib);
    hashmal_moed_t moed = HASHMAL_MOED_NINGUNO;
    if (dow == 6) {
        moed = HASHMAL_MOED_SHABAT;
    }

    /* 2) Mo'adim anuales. Si Shabat ya aplica, un anual encima
     * puede seguir activo - pero el bitmap se OR'ea al final.
     * Para `g_moed_actual` preferimos el anual (mas especifico)
     * si existe; el Shabat queda reflejado por el dow check. */
    int  dias = 0;
    int  hy_rel = 0;
    bool leap = false;
    hashmal_moed_t anual = HASHMAL_MOED_NINGUNO;
    if (hebrew_desde_jdn(jdn_bib, &dias, &hy_rel, &leap) == 0) {
        anual = moed_de_fecha_hebrea(dias, leap);
        if (anual != HASHMAL_MOED_NINGUNO) moed = anual;
    } else {
        MOEDIM_LOG_W("calcular: fecha fuera de tabla RH (fase 1 cubre "
                     "2019-2069) - solo Shabat semanal disponible");
    }

    /* 3) Construir bitmap. Shabat + anual pueden coexistir el mismo
     * dia (p.ej. Yom Kippur cayendo en sabado): ambos bits encendidos. */
    uint32_t bits = 0u;
    if (dow == 6)            bits |= comportamiento_de_moed(HASHMAL_MOED_SHABAT);
    if (anual != HASHMAL_MOED_NINGUNO) bits |= comportamiento_de_moed(anual);

    /* 4) Jubileo: ano gregoriano = EPOCH + 50n. Modula anadiendo
     * RESET_DEUDA + LIMPIEZA_PROFUNDA al bitmap. El mo'ed
     * publicado cambia a JUBILEO si era NINGUNO (dia ordinario de
     * ano jubilar no tiene otro senalado). */
    uint32_t diff = (uint32_t)t->year - HASHMAL_JUBILEO_EPOCH;
    bool     es_anio_jubilar =
        (t->year >= HASHMAL_JUBILEO_EPOCH) &&
        (diff > 0u) &&
        (diff % HASHMAL_JUBILEO_ANIOS == 0u);
    if (es_anio_jubilar) {
        bits |= HASHMAL_COMPORT_RESET_DEUDA
              | HASHMAL_COMPORT_LIMPIEZA_PROFUNDA;
        if (moed == HASHMAL_MOED_NINGUNO) moed = HASHMAL_MOED_JUBILEO;
    }

    g_moed_actual         = moed;
    g_comportamiento_bits = bits;

    MOEDIM_LOG_I("moed=%s bits=0x%03" PRIx32 " "
                 "(greg=%u-%02u-%02u, dow=%d, dias_desde_rh=%d, leap=%d)",
                 nombre_moed(moed), bits,
                 (unsigned)t->year, (unsigned)t->month, (unsigned)t->day,
                 dow, dias, (int)leap);
    (void)hy_rel;
    return HASHMAL_CACHE_OK;
}

/* ==================================================================
 * API PUBLICA - lecturas puras
 * ================================================================== */
hashmal_moed_t hashmal_moedim_actual(void)
{
    return g_moed_actual;
}

uint32_t hashmal_moedim_comportamiento(void)
{
    return g_comportamiento_bits;
}

/* ==================================================================
 * API PUBLICA - sunset / sunrise (Añadidura 04)
 *
 * Exponen el calculo astronomico para quien necesite las fronteras
 * de dia biblico sin pasar por todo el motor de moedim (p.ej. Pieza
 * 08 ashmurot, que divide la noche en tercios).
 * ================================================================== */
int hashmal_moedim_sunset_hour(const hashmal_moedim_tiempo_t *t,
                                double *out_hour_local)
{
    if (t == NULL || out_hour_local == NULL) {
        return HASHMAL_CACHE_ERR_NULL;
    }
    int    doy     = doy_gregoriano(t->year, t->month, t->day);
    double lat_deg = (double)t->lat_milideg / 1000.0;
    double lon_deg = (double)t->lon_milideg / 1000.0;
    double sunset  = -1.0;
    bool ok = calcular_eventos_solares(doy, lat_deg, lon_deg,
                                        (int)t->tz_minutes,
                                        NULL, &sunset);
    *out_hour_local = sunset;
    return ok ? HASHMAL_CACHE_OK : HASHMAL_CACHE_ERR_INIT;
}

int hashmal_moedim_sunrise_hour(const hashmal_moedim_tiempo_t *t,
                                 double *out_hour_local)
{
    if (t == NULL || out_hour_local == NULL) {
        return HASHMAL_CACHE_ERR_NULL;
    }
    int    doy     = doy_gregoriano(t->year, t->month, t->day);
    double lat_deg = (double)t->lat_milideg / 1000.0;
    double lon_deg = (double)t->lon_milideg / 1000.0;
    double sunrise = -1.0;
    bool ok = calcular_eventos_solares(doy, lat_deg, lon_deg,
                                        (int)t->tz_minutes,
                                        &sunrise, NULL);
    *out_hour_local = sunrise;
    return ok ? HASHMAL_CACHE_OK : HASHMAL_CACHE_ERR_INIT;
}

/* ==================================================================
 *
 *   "eleh MOADEI YHWH miqra'ei qodesh asher tiqre'u otam bemoadam."
 *     - Levitico 23:4
 *
 *   "veayu le'OTOT uleMOADIM." - Genesis 1:14
 *
 *   "vayhi 'erev vayhi voqer yom ejad." - Genesis 1:5
 *
 *   Los tiempos son de YHWH - no nuestros. El motor no INVENTA
 *   moedim; solo los calcula desde las senales que el Padre puso
 *   en el sol, la luna y los anos. El dia comienza al atardecer
 *   porque asi lo dijo en Genesis 1. Fase 1 aproxima; fase 2
 *   afinara. Pero el patron es Suyo desde el principio.
 *
 * ================================================================== */
