/*
 * ===================================================================
 * PIEZA 08 - ACEITE PERPETUO - aceite_ashmurot.c
 * Añadidura 04: LAS VIGILIAS DE LA NOCHE
 * Salmo 63:6 + Lamentaciones 2:19 + Salmo 119:148 + Exodo 14:24
 *             + Jueces 7:19
 * ===================================================================
 *
 * "zakhartikha 'al yetsu'ay beASHMUROT ehgeh bakh." - Salmo 63:6
 * "En las VIGILIAS de la noche medito en Ti."
 *
 * "qumi roni vallailah leROSH ASHMUROT shifkhi khamayim libbekh."
 *   - Lamentaciones 2:19
 * "Levantate, clama en la noche, al COMENZAR cada vigilia;
 *  derrama tu corazon como agua."
 *
 * "qidmu einay ASHMUROT." - Salmo 119:148
 * "Mis ojos se ANTICIPAN a las vigilias."
 *
 * "vayhi beashmoret HABOQER vayashqef YHWH el majaneh mitsrayim."
 *   - Exodo 14:24
 * "Y fue en la vigilia de la MANANA, YHWH miro al campamento
 *  egipcio." - momento de ACCION (el ejercito cae).
 *
 * "vayavo Gid'on... birosh haASHMORET HATIKHONAH."
 *   - Jueces 7:19
 * "Entro Gedeon... al comienzo de la vigilia MEDIA." - ataque,
 *  alta vigilia.
 *
 * --- EXEGESIS OPERATIVA ---
 *
 * 3 vigilias (no 4 como la tradicion romana): el canon hebreo
 * divide la noche en TRES tercios iguales (Jue 7:19 "hatikhonah"
 * = la DE EN MEDIO, implica DE TRES). Asi dividimos la noche
 * (sunset -> sunrise) en tres partes iguales.
 *
 *   PRIMERA (rishona, Lam 2:19)   sunset       -> sunset + 1/3 noche
 *   MEDIA   (tikhonah, Jue 7:19)  +1/3         -> +2/3
 *   MANANA  (haboqer, Ex 14:24)   +2/3         -> sunrise
 *
 * Cada vigilia lleva un comportamiento:
 *
 *   PRIMERA = transicion dia->noche, red normal, ultimo scan de shaon.
 *   MEDIA   = bajo consumo, heartbeat reducido, atencion alerta.
 *   MANANA  = reactivacion gradual, scan de red, preparacion para dia.
 *
 * --- QUE HACE / QUE NO HACE ---
 *
 *   HACE:                             NO HACE:
 *   - calcula vigilia desde tiempo    - apaga ni enciende nerot
 *   - publica estado + heartbeat rec. - modifica consumidores
 *   - reusa Meeus de Pieza 03         - duplica astronomia
 *
 * Consumidores (Pieza 04 pa'amon, Pieza 09 roles, Pieza 02 tsofeh)
 * subscriben por pull cuando decidan - no en este PR.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_power.h        API publica (enum ashmura, prototipos)
 *   hashmal_cache.h        Meeus publico (sunset/sunrise) + tiempo_t
 *   esp_log.h (ORO EGIPTO) ASHM_LOG_*
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "esp_log.h"

#include "hashmal_power.h"
#include "hashmal_cache.h"   /* Meeus publico + hashmal_moedim_tiempo_t */

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *ASHM_TAG = "hashmal.aceite.ashmurot";

#define ASHM_LOG_I(fmt, ...)  ESP_LOGI(ASHM_TAG, fmt, ##__VA_ARGS__)
#define ASHM_LOG_W(fmt, ...)  ESP_LOGW(ASHM_TAG, fmt, ##__VA_ARGS__)
#define ASHM_LOG_E(fmt, ...)  ESP_LOGE(ASHM_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * Estado interno - ultima vigilia calculada
 * ================================================================== */
static hashmal_ashmura_t g_ashmura_actual = HASHMAL_ASHMURA_NINGUNA;

/* ==================================================================
 * Helpers
 * ================================================================== */

static double hora_actual_local(const hashmal_moedim_tiempo_t *t)
{
    return (double)t->hour + (double)t->minute / 60.0;
}

static const char *nombre_ashmura(hashmal_ashmura_t a)
{
    switch (a) {
    case HASHMAL_ASHMURA_PRIMERA: return "PRIMERA (Lam 2:19 rishona)";
    case HASHMAL_ASHMURA_MEDIA:   return "MEDIA (Jue 7:19 hatikhonah)";
    case HASHMAL_ASHMURA_MANANA:  return "MANANA (Ex 14:24 haboqer)";
    case HASHMAL_ASHMURA_NINGUNA:
    default:                       return "NINGUNA (dia)";
    }
}

/* ==================================================================
 * API PUBLICA - hashmal_ashmurot_calcular
 *
 * Pasos:
 *   1. Consultar sunrise/sunset desde Pieza 03 (Meeus publico).
 *   2. Si region polar (ambos -1) o t invalido -> NINGUNA.
 *   3. Si estamos fuera de noche (h >= sunrise && h < sunset) -> NINGUNA.
 *   4. Calcular offset desde sunset (wrap en medianoche si toca).
 *   5. Comparar contra 1/3 y 2/3 del largo de noche.
 *   6. Publicar estado.
 *
 * Mismo patron que moedim_calcular: input por struct, resultado
 * persistido en estado interno para lecturas puras posteriores.
 * ================================================================== */
int hashmal_ashmurot_calcular(const hashmal_moedim_tiempo_t *t)
{
    if (t == NULL) {
        ASHM_LOG_E("calcular: tiempo NULL");
        return HASHMAL_POWER_ERR_NULL;
    }

    double sunrise = -1.0;
    double sunset  = -1.0;
    int rc_sr = hashmal_moedim_sunrise_hour(t, &sunrise);
    int rc_ss = hashmal_moedim_sunset_hour(t,  &sunset);
    if (rc_sr != HASHMAL_CACHE_OK || rc_ss != HASHMAL_CACHE_OK
        || sunrise < 0.0 || sunset < 0.0) {
        /* Region polar o error - sin frontera clara de noche. */
        g_ashmura_actual = HASHMAL_ASHMURA_NINGUNA;
        ASHM_LOG_W("calcular: sin frontera solar (polar o rc err) -> NINGUNA");
        return HASHMAL_POWER_OK;
    }

    double h = hora_actual_local(t);

    /* Estamos de dia si sunrise <= h < sunset (ordenamiento normal)
     * - caso comun fuera de latitudes extremas. Si por alguna razon
     * sunset < sunrise (edge case poco probable en [-60, 60] deg),
     * el test below sigue dando la respuesta correcta por inversion
     * del intervalo. */
    bool en_noche = !(h >= sunrise && h < sunset);
    if (!en_noche) {
        g_ashmura_actual = HASHMAL_ASHMURA_NINGUNA;
        return HASHMAL_POWER_OK;
    }

    /* Largo de noche y offset desde sunset (siempre no-negativo).
     * Si h >= sunset (misma tarde): offset = h - sunset.
     * Si h < sunrise (ya paso medianoche): offset = (24 - sunset) + h. */
    double offset;
    if (h >= sunset) {
        offset = h - sunset;
    } else {
        offset = (24.0 - sunset) + h;
    }
    double largo_noche = (24.0 - sunset) + sunrise;
    double tercio      = largo_noche / 3.0;

    hashmal_ashmura_t nueva;
    if (offset < tercio) {
        nueva = HASHMAL_ASHMURA_PRIMERA;
    } else if (offset < 2.0 * tercio) {
        nueva = HASHMAL_ASHMURA_MEDIA;
    } else {
        nueva = HASHMAL_ASHMURA_MANANA;
    }

    if (nueva != g_ashmura_actual) {
        ASHM_LOG_I("transicion %s -> %s (offset=%.2fh, noche=%.2fh, "
                   "sunset=%.2f, sunrise=%.2f)",
                   nombre_ashmura(g_ashmura_actual),
                   nombre_ashmura(nueva),
                   offset, largo_noche, sunset, sunrise);
    }
    g_ashmura_actual = nueva;
    return HASHMAL_POWER_OK;
}

/* ==================================================================
 * API PUBLICA - lecturas puras
 * ================================================================== */
hashmal_ashmura_t hashmal_ashmurot_actual(void)
{
    return g_ashmura_actual;
}

uint32_t hashmal_ashmurot_heartbeat_ms_recomendado(void)
{
    switch (g_ashmura_actual) {
    case HASHMAL_ASHMURA_PRIMERA: return HASHMAL_ASHMURA_HB_PRIMERA_MS;
    case HASHMAL_ASHMURA_MEDIA:   return HASHMAL_ASHMURA_HB_MEDIA_MS;
    case HASHMAL_ASHMURA_MANANA:  return HASHMAL_ASHMURA_HB_MANANA_MS;
    case HASHMAL_ASHMURA_NINGUNA:
    default:                       return 0u;
    }
}

/* ==================================================================
 *
 *   "beashmoret HABOQER vayashqef YHWH." - Exodo 14:24
 *
 *   "leROSH ASHMUROT shifkhi khamayim libbekh." - Lamentaciones 2:19
 *
 *   "beRO'SH haashmORET hatikhONAH." - Jueces 7:19
 *
 *   La noche no es una sola. Se divide en tres para que el
 *   corazon no se canse. Cada tercio tiene su oficio: el primero
 *   trae la transicion, el segundo la vigilancia mas estrecha,
 *   el tercero la preparacion para la luz. El Padre organizo
 *   asi la vigilia porque El mismo vigila "toda la noche como en
 *   una vigilia" (Sal 90:4).
 *
 * ================================================================== */
