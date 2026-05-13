/*
 * ===================================================================
 * PIEZA 14 - INCIENSO SAGRADO - incienso_shemitah.c
 * Añadidura 14: SHEMITAH Y YOVEL - Reset Periodico
 * Levitico 25:1-55 + Deuteronomio 15:1-11 + Isaias 61:1-2
 * ===================================================================
 *
 * "vaydabber YHWH el Moshe behar Sinai lemor:... ki tavou el
 *  ha'arets asher ani noten lakhem veshavetah ha'arets SHABBAT
 *  laYHWH. SHESH SHANIM tizra' sadekha veshesh shanim tizmor
 *  karmekha veasafta et tevuatah." - Levitico 25:1-3
 * "Hablo YHWH a Moshe en el monte Sinai: cuando entrareis a la
 *  tierra que yo os doy, la tierra reposara SHABAT. SEIS ANOS
 *  sembraras tu campo y SEIS podaras tu vina."
 *
 * "uvashshanah hashshvi'it SHABBAT SHABBATON yiheyeh la'arets
 *  SHABBAT laYHWH; sadekha LO TIZRA' vekharmkha LO TIZMOR."
 *   - Levitico 25:4
 * "Y el SEPTIMO ano sera SHABAT SHABBATON para la tierra: NO
 *  SEMBRARAS, NO PODARAS."
 *
 * "vesafarta lekha SHEVA shabbatot shanim sheva shanim SHEVA
 *  PE'AMIM... vehayu lekha yemei sheva shabbatot hashshanim TESHA
 *  VE'ARBA'IM SHANAH." - Levitico 25:8
 * "Contaras siete shabados de anos siete anos siete veces, y los
 *  dias de los siete shabados de anos seran CUARENTA Y NUEVE."
 *
 * "uqratem DEROR ba'arets lekhol yoshveiha YOVEL hi tiheyeh lakhem
 *  veshavtem ish el AJUZATO." - Levitico 25:10
 * "PROCLAMAREIS LIBERTAD en la tierra a TODOS sus moradores;
 *  JUBILEO sera, y volvereis cada uno a su POSESION."
 *
 * "miqqets sheva shanim ta'aseh SHEMITAH; vezeh devar
 *  hashshemittah: SHAMOT KOL BA'AL MASHEH YADO." - Deuteronomio 15:1-2
 * "Cada SIETE anos haras SHEMITAH; el asunto es: SOLTARA TODO
 *  ACREEDOR su mano (cancela toda deuda)."
 *
 * "ruaj Adonai YHWH 'alai ya'an mashaj YHWH oti levasser 'anavim
 *  shelajani lajavosh lenishberei lev liqro lishvuyim DEROR
 *  velaasurim peqaj-qoaj." - Isaias 61:1
 * "El Espiritu del Senor YHWH esta sobre mi... me envio a
 *  proclamar LIBERTAD a los cautivos."
 *
 * --- INVARIANTE INV-NIKRAT-PERMANENTE ---
 *
 * Ex 30:33,38: el karat (NIKRAT) es CORTE PERMANENTE. Lv 25:10
 * proclama deror para "todos los moradores", pero el que fue
 * NIKRAT ya no es morador del campamento - fue cortado de su
 * pueblo. Por eso el reset jubilar libera AZHARAH/MASAK/HEREM
 * pero NO toca NIKRAT. La libertad del Yovel es para los que
 * estan adentro; el karat es la frontera de lo que esta afuera.
 *
 * --- DOS CONTADORES, DOS RESETS ---
 *
 *   contador_shemitah (0..6)
 *     en 7 -> ejecuta Shemitah, vuelve a 0
 *     incrementa contador_yovel
 *
 *   contador_yovel (0..6)
 *     cuenta cuantas Shemitot transcurrieron
 *     en 7 (= 49 epocas) -> ejecuta Yovel, ambos a 0
 *
 * Cascada: cada 7 epocas = Shemitah; cada 7 Shemitot = Yovel.
 * Una llamada a _epoca_avanzar puede disparar AMBOS si toca el
 * cierre de los 49 (esto es lo correcto biblicamente: el ano del
 * Yovel es TAMBIEN un ano de Shemitah, Lv 25:11-12).
 *
 * --- SIMPLIFICACION FASE 1 (acordada con el operador) ---
 *
 * Spec original pedia "THROTTLE >= 7 epocas -> liberar". Sin
 * historial de epoca por par en la struct existente, fase 1 hace
 * limpieza incondicional para AZHARAH/MASAK/HEREM (espiritu Dt
 * 15:2 shamot KOL ba'al masheh - "soltara TODO acreedor"). NIKRAT
 * NUNCA. Fase 2 podria extender la struct con epoca_de_escalada
 * para enforce el >=7.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_firewall.h          API publica
 *   hashmal_firewall_internal.h incienso_mut + ahora_ms (mismo componente)
 *   esp_log.h (ORO EGIPTO)      SHM_LOG_*
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <inttypes.h>

#include "esp_log.h"

#include "hashmal_firewall.h"
#include "hashmal_firewall_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *SHM_TAG = "hashmal.incienso.shemitah";

#define SHM_LOG_I(fmt, ...)  ESP_LOGI(SHM_TAG, fmt, ##__VA_ARGS__)
#define SHM_LOG_W(fmt, ...)  ESP_LOGW(SHM_TAG, fmt, ##__VA_ARGS__)
#define SHM_LOG_E(fmt, ...)  ESP_LOGE(SHM_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * Estado interno
 * ================================================================== */
static uint8_t                  g_contador_shemitah     = 0;
static uint8_t                  g_contador_yovel        = 0;
static hashmal_shemitah_ciclo_t g_ciclo_actual          = HASHMAL_SHEMITAH_NORMAL;
static uint32_t                 g_shemitot_ejecutadas   = 0;
static uint32_t                 g_yovelim_ejecutados    = 0;
static int64_t                  g_timestamp_ultima_ms   = 0;

/* ==================================================================
 * Helpers internos - liberar pares respetando NIKRAT
 *
 * Recorre los HASHMAL_FIREWALL_MAX_PARES slots y baja a TAHOR los
 * que esten en AZHARAH / MASAK / HEREM. NIKRAT permanece (Ex
 * 30:33,38 - el karat es permanente).
 * ================================================================== */
static uint8_t liberar_pares_no_nikrat(hashmal_firewall_t *fw)
{
    uint8_t liberados = 0;
    if (fw == NULL) return 0;

    for (uint8_t i = 0; i < HASHMAL_FIREWALL_MAX_PARES; ++i) {
        hashmal_firewall_penalizacion_par_t *par = &fw->pares[i];

        switch (par->nivel) {
        case HASHMAL_FIREWALL_NIVEL_AZHARAH:
        case HASHMAL_FIREWALL_NIVEL_MASAK:
        case HASHMAL_FIREWALL_NIVEL_HEREM:
            /* Dt 15:2 shamot kol ba'al masheh - libera. */
            par->nivel                 = HASHMAL_FIREWALL_NIVEL_TAHOR;
            par->infracciones          = 0;
            par->timestamp_escalada_ms = 0;
            ++liberados;
            break;

        case HASHMAL_FIREWALL_NIVEL_NIKRAT:
            /* Ex 30:33,38 - INV-NIKRAT-PERMANENTE. No tocar. */
            break;

        case HASHMAL_FIREWALL_NIVEL_TAHOR:
        default:
            /* Ya limpio o desconocido - no-op. */
            break;
        }
    }
    return liberados;
}

/* ==================================================================
 * _shemitah_ejecutar - reset de septimo ano (Lv 25:4 + Dt 15:2)
 * ================================================================== */
static void shemitah_ejecutar(void)
{
    hashmal_firewall_t *fw = hashmal_firewall_incienso_mut();
    if (fw == NULL) {
        SHM_LOG_W("shemitah_ejecutar: firewall no levantado - skip");
        return;
    }

    uint8_t liberados = liberar_pares_no_nikrat(fw);
    g_shemitot_ejecutadas += 1;

    SHM_LOG_I("SHEMITAH ejecutada (Lv 25:4 sadekha lo tizra') - "
              "%u pares liberados (Dt 15:2 shamot kol ba'al masheh); "
              "NIKRAT intocable (Ex 30:33,38). Total Shemitot: %"
              PRIu32, (unsigned)liberados, g_shemitot_ejecutadas);
}

/* ==================================================================
 * _yovel_ejecutar - reset jubilar (Lv 25:10 deror)
 *
 * Mismo barrido que Shemitah pero con semantica adicional: el ano
 * del Yovel ES TAMBIEN un ano de Shemitah (Lv 25:11-12), por eso
 * la cascada lo dispara DESPUES de la Shemitah del 49o ano.
 * ================================================================== */
static void yovel_ejecutar(void)
{
    hashmal_firewall_t *fw = hashmal_firewall_incienso_mut();
    if (fw == NULL) {
        SHM_LOG_W("yovel_ejecutar: firewall no levantado - skip");
        return;
    }

    /* En este punto Shemitah ya barrio el ciclo. Re-barremos por
     * consistencia (idempotente sobre TAHOR). El log distingue
     * el evento jubilar. */
    uint8_t liberados = liberar_pares_no_nikrat(fw);
    g_yovelim_ejecutados += 1;

    SHM_LOG_I("===== YOVEL ejecutado (Lv 25:10 uqratem DEROR) ===== "
              "deror proclamado a TODOS los moradores; %u pares en "
              "estado liberado; NIKRAT NO se libera (Ex 30:33,38 - "
              "el karat es frontera permanente). Total Yovelim: %"
              PRIu32, (unsigned)liberados, g_yovelim_ejecutados);
    SHM_LOG_I("Lv 25:13 ish el AJUZATO - cada uno vuelve a su "
              "posesion original");
}

/* ==================================================================
 * API - hashmal_shemitah_epoca_avanzar (Lv 25:3-4 cascade)
 * ================================================================== */
int hashmal_shemitah_epoca_avanzar(void)
{
    /* Verificacion ligera: si firewall no esta levantado, no
     * podemos limpiar nada. El contador podria avanzar de todas
     * formas (estado interno solo), pero sin efecto - mejor abortar
     * para que el caller sepa. */
    const hashmal_firewall_t *fw = hashmal_firewall_get_incienso_interno();
    if (fw == NULL) {
        SHM_LOG_E("epoca_avanzar: firewall no levantado");
        return HASHMAL_FIREWALL_ERR_SHEMITAH;
    }

    g_contador_shemitah    += 1u;
    g_timestamp_ultima_ms   = hashmal_firewall_ahora_ms();
    g_ciclo_actual          = HASHMAL_SHEMITAH_NORMAL;

    if (g_contador_shemitah >= HASHMAL_SHEMITAH_CICLO) {
        /* Disparar Shemitah. */
        shemitah_ejecutar();
        g_contador_shemitah = 0;
        g_contador_yovel   += 1u;
        g_ciclo_actual      = HASHMAL_SHEMITAH_SHEMITAH;

        if (g_contador_yovel >= HASHMAL_YOVEL_CICLO) {
            /* Cascada al Yovel - el 49o ano es Shemitah Y Yovel
             * (Lv 25:11-12). Ya ejecutamos shemitah arriba; ahora
             * el deror jubilar adicional. */
            yovel_ejecutar();
            g_contador_yovel = 0;
            g_ciclo_actual   = HASHMAL_SHEMITAH_YOVEL;
        }
    }

    return HASHMAL_FIREWALL_OK;
}

/* ==================================================================
 * API - lecturas puras
 * ================================================================== */
hashmal_shemitah_ciclo_t hashmal_shemitah_ciclo_actual(void)
{
    return g_ciclo_actual;
}

int hashmal_shemitah_info(hashmal_shemitah_info_t *out)
{
    if (out == NULL) return HASHMAL_FIREWALL_ERR_NULL;
    out->contador_shemitah        = g_contador_shemitah;
    out->contador_yovel           = g_contador_yovel;
    out->ciclo                    = g_ciclo_actual;
    out->shemitot_ejecutadas      = g_shemitot_ejecutadas;
    out->yovelim_ejecutados       = g_yovelim_ejecutados;
    out->timestamp_ultima_epoca_ms = g_timestamp_ultima_ms;
    return HASHMAL_FIREWALL_OK;
}

/* ==================================================================
 *
 *   "SHESH SHANIM tizra'... uvashshanah hashshvi'it SHABBAT
 *    SHABBATON." - Levitico 25:3-4
 *
 *   "vesafarta lekha SHEVA SHABBATOT SHANIM sheva shanim SHEVA
 *    PE'AMIM" - Levitico 25:8
 *
 *   "uqratem DEROR ba'arets lekhol yoshveiha." - Levitico 25:10
 *
 *   "shamot KOL ba'al masheh yado." - Deuteronomio 15:2
 *
 *   "qara liShVUYIM DEROR." - Isaias 61:1
 *
 *   La tierra reposa cada siete; las deudas se cancelan; cada
 *   cincuenta se proclama LIBERTAD. La economia biblica tiene
 *   resets periodicos por diseno - lo que se acumula no se
 *   acumula para siempre. Los warnings, throttles, herems
 *   acumulados a lo largo del tiempo se sueltan: el atalaya
 *   recuerda con misericordia.
 *
 *   Pero el karat (NIKRAT) NO se libera. Lv 25:10 dice "para
 *   todos sus moradores" - el cortado dejo de ser morador. La
 *   libertad jubilar es para quien esta adentro; el karat marca
 *   la frontera. No es crueldad; es coherencia: el sistema
 *   sostiene la libertad de los presentes precisamente porque
 *   marco lo que no podia entrar.
 *
 * ================================================================== */
