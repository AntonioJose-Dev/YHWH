/**
 * ===================================================================
 * PIEZA 10 - MILLUIM - consagracion_fases.c
 * Sub-pieza: SHEVA' MA'ASIM - 7 actos secuenciales de Exodo 29
 * Exodo 29:1-35 + Levitico 8:6-35 + Levitico 9:1
 * ===================================================================
 *
 * "ve'asita leAharon ulvanav KAJAH kejol asher tsivviti otajah -
 *  SHIV'AT YAMIM temalle yadam." - Exodo 29:35
 * "Y haras a Aaron y a sus hijos ASI, conforme a TODO lo que te he
 *  mandado - durante SIETE DIAS llenaras sus manos."
 *
 * "kaasher 'asa bayyom hazzeh tsivva YHWH la'asot leJAPPER
 *  'aleijem." - Levitico 8:34
 * "Como se ha hecho en este dia, mando YHWH hacer para HACER EXPIACION
 *  por vosotros."
 *
 * "umiPETAJ ohel mo'ed lo tetse'u SHIV'AT YAMIM, 'ad yom melo't
 *  yemei millu'ejem, ki shiv'at yamim yemalle et yedjem." - Lv 8:33
 * "De la puerta de la Tienda del Encuentro NO SALDREIS durante SIETE
 *  DIAS, hasta que se cumpla el dia de vuestros milluim, porque por
 *  siete dias llenara vuestras manos."
 *
 * --- LAS 7 FASES DE EXODO 29 ---
 *
 *   0. RAJATS    Ex 29:4     + Lv 8:6       lavar con agua
 *   1. HILBISH   Ex 29:5-6   + Lv 8:7-9     vestir con begadim
 *   2. MASHAJ    Ex 29:7     + Lv 8:12      ungir con shemen
 *   3. JATTAT    Ex 29:10-14 + Lv 8:14-17   novillo de pecado
 *   4. OLA       Ex 29:15-18 + Lv 8:18-21   primer carnero (ascension)
 *   5. MILLUIM   Ex 29:19-28 + Lv 8:22-29   segundo carnero (llenado)
 *   6. SHIVAT    Ex 29:35    + Lv 8:33-35   7 dias en la puerta
 *
 *   Dia 8 (Lv 9:1) - vayhi bayyom hashemini - el sacerdote SALE a
 *   servir con manos llenas. Lo maneja consagracion_onboarding.c
 *   (hashmal_onboarding_lanzar).
 *
 * --- VOCABULARIO ---
 *
 *   rajats       (RChTs, H7364)               LAVAR con agua.
 *   hilbish      (HLBYSh, H3847 Hifil)    VESTIR.
 *   mashaj       (MShCh, H4886)               UNGIR con aceite.
 *   jatta't      (ChTAT, H2403)              PURIFICACION, ofrenda
 *                                               por pecado.
 *   'ola         ('LH, H5930)                ASCENSION, holocausto
 *                                               entero.
 *   milluim      (MLAYM, H4394)            LLENADO, consagracion.
 *   shiv'at      (ShB'T, H7651)              SIETE - siete dias.
 *   semija       (SMYKH, H5564)             IMPOSICION de manos -
 *                                               Ex 29:10,15,19.
 *   reaj nijoaj  (RYCh NYChWCh,
 *                  H7381+H5207)                  AROMA de REPOSO -
 *                                               senal de aceptacion
 *                                               (Ex 29:18,25,41).
 *   mijuts       (MChWTs, H2351)                FUERA - lo de la jatta't
 *                                               se quema fuera del
 *                                               campamento (Ex 29:14).
 *   yemei millu'ejem (YMY MLAYKM)        DIAS de VUESTROS
 *                                               milluim (Lv 8:33).
 *   layyom       (LYWM, H3117)                POR DIA - Ex 29:36
 *                                               'ar jatta't layyom
 *                                               (un novillo por dia
 *                                               de los 7).
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   hashmal_onboarding_fase_avanzar      0 -> 1 -> 2 -> 3 -> 4 -> 5 -> 6
 *   hashmal_onboarding_fase_estado       lectura pura.
 *   hashmal_onboarding_fase_completada   ?fase X ya paso? bool puro.
 *   hashmal_onboarding_ciclo_registrar   1 de los 7 dias (en SHIVAT).
 *   hashmal_onboarding_ciclo_estado      lectura pura del ciclo.
 *
 * --- SEPARACION DE RESPONSABILIDADES ---
 *
 *   Este archivo MARCA la fase completada y AVANZA a la siguiente.
 *   NO aplica sellos (Ex 29:20) - eso vive en consagracion_sellos.c.
 *   NO firma config (Ex 29:21) - idem.
 *   NO activa lockdown (Ex 29:33-34) - eso vive en consagracion_lockdown.c.
 *
 *   La razon es cultual: cada archivo es un keli (utensilio) propio,
 *   con funcion limitada. Igual que en el Mishkan el altar no cumple
 *   la funcion del kiyor - cada uno hace su parte.
 *
 * --- FASE SHIVAT NO SE AVANZA ---
 *
 * Las fases 0..5 se completan con `fase_avanzar`. La fase 6 (SHIVAT)
 * se completa SOLO cuando `ciclo_registrar` alcanza 7 ciclos. Intentar
 * avanzar en SHIVAT devuelve ERR_FASE_INCORRECTA con log de explicacion
 * - Ex 29:35 + Lv 8:33 exigen los 7 dias completos, no saltable.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_onboarding.h
 *   hashmal_onboarding_internal.h   consagracion_mut + ahora_ms
 *   hashmal_roles.h                  verificar vestidura (HILBISH)
 *   esp_log.h (ORO EGIPTO)           logging
 *
 * Autor: Antonio Jose Marin Soto (Betsalel - Ex 31:2)
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "esp_log.h"

#include "hashmal_onboarding.h"
#include "hashmal_onboarding_internal.h"
#include "hashmal_roles.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *FASES_TAG = "hashmal.onboarding.fases";

#define FASES_LOG_I(fmt, ...)  ESP_LOGI(FASES_TAG, fmt, ##__VA_ARGS__)
#define FASES_LOG_W(fmt, ...)  ESP_LOGW(FASES_TAG, fmt, ##__VA_ARGS__)
#define FASES_LOG_E(fmt, ...)  ESP_LOGE(FASES_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * HELPERS LOCALES (kelim autocontenidos)
 * ================================================================== */

static inline bool nodo_id_valido(uint8_t nodo_id)
{
    return nodo_id < HASHMAL_ONBOARDING_MAX_NODOS;
}

static const char *nombre_fase(hashmal_onboarding_fase_t f)
{
    switch (f) {
    case HASHMAL_ONBOARDING_FASE_RAJATS:
        return "RAJATS (H7364 — lavar, Éx 29:4 + Lv 8:6)";
    case HASHMAL_ONBOARDING_FASE_HILBISH:
        return "HILBISH (H3847 — vestir, Éx 29:5 + Lv 8:7)";
    case HASHMAL_ONBOARDING_FASE_MASHAJ:
        return "MASHAJ (H4886 — ungir, Éx 29:7 + Lv 8:12)";
    case HASHMAL_ONBOARDING_FASE_JATTAT:
        return "JATTAT (H2403 — purificar, Éx 29:10-14 + Lv 8:14-17)";
    case HASHMAL_ONBOARDING_FASE_OLA:
        return "OLA (H5930 — ascender, Éx 29:15-18 + Lv 8:18-21)";
    case HASHMAL_ONBOARDING_FASE_MILLUIM:
        return "MILLUIM (H4394 — llenar, Éx 29:19-28 + Lv 8:22-29)";
    case HASHMAL_ONBOARDING_FASE_SHIVAT:
        return "SHIVAT (H7651 — siete días, Éx 29:35 + Lv 8:33)";
    default:
        return "<fase desconocida>";
    }
}

/**
 * Verifica que el nodo siga vestido en pieza 09 (consistencia de
 * HILBISH a lo largo del pipeline). Ex 29:5 + Lv 8:7 - vestir
 * antes, y la vestidura permanece durante toda la consagracion
 * (Lv 8:33 "lo tetse'u" - ni siquiera sale de la puerta).
 */
static bool nodo_esta_vestido_en_roles(uint8_t nodo_id)
{
    const hashmal_roles_estado_roles_t *r = hashmal_roles_get_estado();
    if (r == NULL) return false;
    if (nodo_id >= HASHMAL_ROLES_MAX_NODOS) return false;
    return r->nodos[nodo_id].vestido;
}

/* ==================================================================
 * hashmal_onboarding_fase_avanzar - Exodo 29:1-28 + Levitico 8:6-29
 *
 * Progresa al siguiente acto del milluim. Ex 29 es SECUENCIAL - no
 * se permite saltar, repetir ni retroceder. Lv 8 confirma el orden
 * ejecutandolo paso a paso: v.6 (rajats) -> v.7-9 (hilbish) -> v.12
 * (mashaj) -> v.14-17 (jatta't) -> v.18-21 ('ola) -> v.22-29 (milluim)
 * -> v.33-35 (shiv'at).
 *
 * Validaciones (en orden):
 *   1. Subsistema iniciado.
 *   2. nodo_id en rango.
 *   3. Estado == EN_PROCESO (sin pipeline activo, no hay fase
 *      que avanzar).
 *   4. fase_actual dentro de [RAJATS..MILLUIM]. SHIVAT exige
 *      `ciclo_registrar` - avanzar en SHIVAT se rechaza con
 *      mensaje explicativo.
 *
 * Side effects:
 *   - fases_completadas[fase_actual] = true
 *   - fase_actual = siguiente (excepto al salir de MILLUIM, que
 *     pasa a SHIVAT y la completitud de 6 se hara por ciclos)
 *   - timestamp_fase_ms = ahora
 *
 * Validacion especial HILBISH: comprueba que el nodo siga vestido
 * en pieza 09 (consistencia con Ex 29:5 + Lv 8:7).
 *
 * 2+ testigos por fase (ver nombre_fase): mandato en Ex 29 +
 * ejecucion en Lv 8, siempre libros distintos.
 * ================================================================== */
int hashmal_onboarding_fase_avanzar(uint8_t nodo_id)
{
    hashmal_onboarding_t *c = hashmal_onboarding_consagracion_mut();
    if (c == NULL) {
        FASES_LOG_E("avanzar: subsistema no iniciado (init primero)");
        return HASHMAL_ONBOARDING_ERR_NO_INIT;
    }

    if (!nodo_id_valido(nodo_id)) {
        FASES_LOG_W("avanzar: nodo_id %u fuera de rango [0..%u]",
                    (unsigned)nodo_id,
                    (unsigned)(HASHMAL_ONBOARDING_MAX_NODOS - 1u));
        return HASHMAL_ONBOARDING_ERR_NODO_INVALIDO;
    }

    hashmal_onboarding_nodo_t *n = &c->nodos[nodo_id];

    if (n->estado != HASHMAL_ONBOARDING_EN_PROCESO) {
        FASES_LOG_W("avanzar: nodo %u no EN_PROCESO (estado=%u) — "
                    "Éx 29:1 vezéh haddavár requiere pipeline activo",
                    (unsigned)nodo_id, (unsigned)n->estado);
        return HASHMAL_ONBOARDING_ERR_FASE_INCORRECTA;
    }

    const hashmal_onboarding_fase_t actual =
        (hashmal_onboarding_fase_t)n->fase_actual;

    if (actual > HASHMAL_ONBOARDING_FASE_SHIVAT) {
        FASES_LOG_W("avanzar: nodo %u fase %u fuera del enum "
                    "(corrupción)", (unsigned)nodo_id, (unsigned)actual);
        return HASHMAL_ONBOARDING_ERR_FASE_INCORRECTA;
    }

    /* Fase 6 (SHIVAT) no se avanza - se completa con ciclo_registrar.
     * Ex 29:35 + Lv 8:33: siete dias completos, no atajo. */
    if (actual == HASHMAL_ONBOARDING_FASE_SHIVAT) {
        FASES_LOG_W("avanzar: nodo %u en SHIVAT — 'shivʻát yamím "
                    "yemallé et yedjém' (Lv 8:33); usar "
                    "ciclo_registrar() hasta completar 7/7",
                    (unsigned)nodo_id);
        return HASHMAL_ONBOARDING_ERR_FASE_INCORRECTA;
    }

    hashmal_onboarding_fase_t siguiente;
    const char *versiculo_accion = "";

    switch (actual) {
    case HASHMAL_ONBOARDING_FASE_RAJATS:
        /* Ex 29:4 "verajatsta otam bammayim" + Lv 8:6 "vayyirjats
         * otam bammayim". El lavado ya ocurrio en `iniciar()`
         * (memset del pipeline). Aqui confirmamos la fase. */
        siguiente        = HASHMAL_ONBOARDING_FASE_HILBISH;
        versiculo_accion = "verajátstá bammáyim (Éx 29:4 + Lv 8:6)";
        break;

    case HASHMAL_ONBOARDING_FASE_HILBISH:
        /* Ex 29:5 "velaqajta et habegadim vehiblashta" + Lv 8:7
         * "vayyitten 'alav et hakuttonet". Verificamos que el
         * nodo siga vestido en pieza 09 - si alguien lo desvistio
         * durante el pipeline, la fase no puede avanzar. */
        if (!nodo_esta_vestido_en_roles(nodo_id)) {
            FASES_LOG_W("avanzar: nodo %u perdió la vestidura — "
                        "Éx 29:5 vehibláshta debe permanecer durante "
                        "todo milluím (Lv 8:33 no salir)",
                        (unsigned)nodo_id);
            return HASHMAL_ONBOARDING_ERR_NO_VESTIDO;
        }
        siguiente        = HASHMAL_ONBOARDING_FASE_MASHAJ;
        versiculo_accion = "vehibláshta (Éx 29:5 + Lv 8:7)";
        break;

    case HASHMAL_ONBOARDING_FASE_MASHAJ:
        /* Ex 29:7 "veyatsaqta 'al rosho umashajta oto" + Lv 8:12
         * "vayyitsoq mishemen hammishja 'al rosh Aharon". Aceite
         * sobre la cabeza del sumo sacerdote. */
        siguiente        = HASHMAL_ONBOARDING_FASE_JATTAT;
        versiculo_accion = "veyatsáqta ʻal roshó (Éx 29:7 + Lv 8:12)";
        break;

    case HASHMAL_ONBOARDING_FASE_JATTAT:
        /* Ex 29:10-14 "vehiqravta et happar... vesamakh Aharon uvanav
         * et yedehem 'al rosh happar" + Lv 8:14-17. Semija (1ra).
         * Residuos queman MIJUTS lammajaneh (Ex 29:14). */
        siguiente        = HASHMAL_ONBOARDING_FASE_OLA;
        versiculo_accion = "par jattáʼt, mijúts lammajanéh "
                           "(Éx 29:10-14 + Lv 8:14-17)";
        break;

    case HASHMAL_ONBOARDING_FASE_OLA:
        /* Ex 29:15-18 "ve'et ha'ayil ha'ejad tiqqaj... reaj nijoaj
         * laYHWH" + Lv 8:18-21 "reaj nijoaj ishe laYHWH". Primer
         * carnero QUEMADO COMPLETO - ascension. Semija (2da). */
        siguiente        = HASHMAL_ONBOARDING_FASE_MILLUIM;
        versiculo_accion = "éil ʻolá, réaj nijóaj "
                           "(Éx 29:18 + Lv 8:21)";
        break;

    case HASHMAL_ONBOARDING_FASE_MILLUIM:
        /* Ex 29:19-28 "velaqajta et ha'ayil hasheni... eil milluim"
         * + Lv 8:22-29. Segundo carnero, el del llenado. Semija
         * (3ra). Sangre en oreja/pulgar/dedo (v.20) la aplica
         * consagracion_sellos.c - aqui solo marcamos la fase. */
        siguiente        = HASHMAL_ONBOARDING_FASE_SHIVAT;
        versiculo_accion = "éil hamilluím (Éx 29:22 + Lv 8:28)";
        break;

    default:
        /* Inalcanzable - SHIVAT ya filtrado arriba, enum cerrado. */
        FASES_LOG_E("avanzar: fase actual %u inalcanzable",
                    (unsigned)actual);
        return HASHMAL_ONBOARDING_ERR_FASE_INCORRECTA;
    }

    /* Aplicar cambios. */
    n->fases_completadas[actual] = true;
    n->fase_actual               = (uint8_t)siguiente;
    n->timestamp_fase_ms         = hashmal_onboarding_ahora_ms();
    c->timestamp_ms              = n->timestamp_fase_ms;

    FASES_LOG_I("nodo %u: %s completada — %s → %s",
                (unsigned)nodo_id, nombre_fase(actual),
                versiculo_accion, nombre_fase(siguiente));

    return HASHMAL_ONBOARDING_OK;
}

/* ==================================================================
 * hashmal_onboarding_fase_estado - lectura pura
 *
 * Sin logs, sin side effects.
 *
 *   out NULL               -> ERR_NULL
 *   sin init               -> ERR_NO_INIT
 *   nodo_id fuera de rango -> ERR_NODO_INVALIDO
 * ================================================================== */
int hashmal_onboarding_fase_estado(uint8_t nodo_id,
                                    hashmal_onboarding_fase_t *out)
{
    if (out == NULL) return HASHMAL_ONBOARDING_ERR_NULL;

    const hashmal_onboarding_t *c =
        hashmal_onboarding_get_consagracion_interno();
    if (c == NULL) return HASHMAL_ONBOARDING_ERR_NO_INIT;

    if (!nodo_id_valido(nodo_id)) {
        return HASHMAL_ONBOARDING_ERR_NODO_INVALIDO;
    }

    *out = (hashmal_onboarding_fase_t)c->nodos[nodo_id].fase_actual;
    return HASHMAL_ONBOARDING_OK;
}

/* ==================================================================
 * hashmal_onboarding_fase_completada - bool puro
 *
 * Sin logs. Devuelve true si la fase indicada fue marcada como
 * completada para el nodo. Cualquier condicion invalida -> false.
 * ================================================================== */
bool hashmal_onboarding_fase_completada(uint8_t nodo_id,
                                         hashmal_onboarding_fase_t fase)
{
    const hashmal_onboarding_t *c =
        hashmal_onboarding_get_consagracion_interno();
    if (c == NULL) return false;
    if (!nodo_id_valido(nodo_id)) return false;
    if ((unsigned)fase >= HASHMAL_ONBOARDING_FASES_TOTAL) return false;
    return c->nodos[nodo_id].fases_completadas[fase];
}

/* ==================================================================
 * hashmal_onboarding_ciclo_registrar - Exodo 29:35 + Levitico 8:33
 *
 * "shiv'at yamim temalle yadam" (Ex 29:35) / "shiv'at yamim yemalle
 *  et yedjem" (Lv 8:33). Siete dias de consagracion, un ciclo por
 *  dia. Cada llamada registra UNO. Al alcanzar 7, la fase SHIVAT
 *  queda marcada completada y el nodo puede ser lanzado en dia 8.
 *
 * Validaciones:
 *   1. Iniciado.
 *   2. nodo_id en rango.
 *   3. Estado == EN_PROCESO.
 *   4. fase_actual == SHIVAT.
 *   5. ciclo_actual < CICLOS_PRUEBA (aun quedan dias por contar).
 *
 * Ex 29:36 refuerza: "upar jatta't ta'aseh layyom" - un novillo
 * POR CADA DIA de los siete, no acumulativo de una sola vez.
 *
 * 2+ testigos:
 *   Ex 29:35  - shiv'at yamim temalle yadam
 *   Lv 8:33   - lo tetse'u shiv'at yamim (libro distinto)
 *   Ex 29:36  - upar jatta't layyom (uno por dia)
 * ================================================================== */
int hashmal_onboarding_ciclo_registrar(uint8_t nodo_id)
{
    hashmal_onboarding_t *c = hashmal_onboarding_consagracion_mut();
    if (c == NULL) {
        FASES_LOG_E("ciclo_registrar: subsistema no iniciado");
        return HASHMAL_ONBOARDING_ERR_NO_INIT;
    }

    if (!nodo_id_valido(nodo_id)) {
        FASES_LOG_W("ciclo_registrar: nodo_id %u fuera de rango",
                    (unsigned)nodo_id);
        return HASHMAL_ONBOARDING_ERR_NODO_INVALIDO;
    }

    hashmal_onboarding_nodo_t *n = &c->nodos[nodo_id];

    if (n->estado != HASHMAL_ONBOARDING_EN_PROCESO) {
        FASES_LOG_W("ciclo_registrar: nodo %u no EN_PROCESO — "
                    "los 7 días son dentro del pipeline "
                    "(Lv 8:33 lo tétseʼú)",
                    (unsigned)nodo_id);
        return HASHMAL_ONBOARDING_ERR_FASE_INCORRECTA;
    }

    if (n->fase_actual != HASHMAL_ONBOARDING_FASE_SHIVAT) {
        FASES_LOG_W("ciclo_registrar: nodo %u en %s, no SHIVAT — "
                    "los ciclos solo cuentan tras milluím "
                    "(Éx 29:35 viene después de v.19-28)",
                    (unsigned)nodo_id,
                    nombre_fase(
                        (hashmal_onboarding_fase_t)n->fase_actual));
        return HASHMAL_ONBOARDING_ERR_FASE_INCORRECTA;
    }

    if (n->ciclo_actual >= HASHMAL_ONBOARDING_CICLOS_PRUEBA) {
        FASES_LOG_W("ciclo_registrar: nodo %u ya completó los 7 "
                    "días (%u/%u) — Éx 29:35 shivʻát yamím no más",
                    (unsigned)nodo_id,
                    (unsigned)n->ciclo_actual,
                    (unsigned)HASHMAL_ONBOARDING_CICLOS_PRUEBA);
        return HASHMAL_ONBOARDING_ERR_FASE_INCORRECTA;
    }

    /* Registrar dia. Ex 29:36 "upar jatta't ta'aseh layyom" - uno
     * por dia. */
    n->ciclo_actual++;
    n->timestamp_fase_ms = hashmal_onboarding_ahora_ms();
    c->timestamp_ms      = n->timestamp_fase_ms;

    if (n->ciclo_actual >= HASHMAL_ONBOARDING_CICLOS_PRUEBA) {
        /* SHIVAT completada al septimo dia. El lanzamiento ocurre
         * al dia 8 (Lv 9:1), gestionado por consagracion_onboarding.c. */
        n->fases_completadas[HASHMAL_ONBOARDING_FASE_SHIVAT] = true;
        FASES_LOG_I("nodo %u: shivʻát yamím COMPLETOS — %u/%u "
                    "(Éx 29:35 + Lv 8:33) — listo para sheminí (Lv 9:1)",
                    (unsigned)nodo_id,
                    (unsigned)n->ciclo_actual,
                    (unsigned)HASHMAL_ONBOARDING_CICLOS_PRUEBA);
    } else {
        FASES_LOG_I("nodo %u: ciclo %u/%u registrado "
                    "(Éx 29:36 upar jattáʼt layyóm)",
                    (unsigned)nodo_id,
                    (unsigned)n->ciclo_actual,
                    (unsigned)HASHMAL_ONBOARDING_CICLOS_PRUEBA);
    }

    return HASHMAL_ONBOARDING_OK;
}

/* ==================================================================
 * hashmal_onboarding_ciclo_estado - lectura pura
 *
 * Sin logs, sin side effects. Escribe el contador de ciclos [0..7]
 * en *out.
 *
 *   out NULL               -> ERR_NULL
 *   sin init               -> ERR_NO_INIT
 *   nodo_id fuera de rango -> ERR_NODO_INVALIDO
 * ================================================================== */
int hashmal_onboarding_ciclo_estado(uint8_t nodo_id, uint8_t *out)
{
    if (out == NULL) return HASHMAL_ONBOARDING_ERR_NULL;

    const hashmal_onboarding_t *c =
        hashmal_onboarding_get_consagracion_interno();
    if (c == NULL) return HASHMAL_ONBOARDING_ERR_NO_INIT;

    if (!nodo_id_valido(nodo_id)) {
        return HASHMAL_ONBOARDING_ERR_NODO_INVALIDO;
    }

    *out = c->nodos[nodo_id].ciclo_actual;
    return HASHMAL_ONBOARDING_OK;
}

/* ==================================================================
 *
 *   "vezeh haddavar asher ta'aseh LAHEM leQADDESH otam." - Exodo 29:1
 *
 *   "SHIV'AT YAMIM temalle yadam." - Exodo 29:35
 *
 *   "shiv'at yamim yemalle et yedjem." - Levitico 8:33
 *
 *   Las 7 fases del milluim no son casillas que marcar: son actos
 *   que el Padre ordeno para LLENAR las manos del que servira.
 *   Lavar, vestir, ungir, purificar, ascender, llenar, esperar
 *   siete dias. Cada uno en su sitio. Cada uno con su versiculo.
 *   Ninguno se salta - Lv 8:33 lo repite: "no saldreis de la
 *   puerta siete dias". Quien quiere lanzar al octavo, camina los
 *   siete primeros. Este archivo protege ese orden.
 *
 *   -- Las 7 fases cerradas por este archivo ---------------------
 *
 *     0. RAJATS    verajatsta bammayim           (Ex 29:4 + Lv 8:6)
 *     1. HILBISH   vehiblashta et habegadim      (Ex 29:5 + Lv 8:7)
 *     2. MASHAJ    veyatsaqta 'al rosho          (Ex 29:7 + Lv 8:12)
 *     3. JATTAT    par jatta't, mijuts lammajaneh (Ex 29:10-14 + Lv 8:14-17)
 *     4. OLA       eil 'ola, reaj nijoaj         (Ex 29:18 + Lv 8:21)
 *     5. MILLUIM   eil hamilluim                 (Ex 29:22 + Lv 8:28)
 *     6. SHIVAT    shiv'at yamim yemalle yadam   (Ex 29:35 + Lv 8:33)
 *
 *   -- Los 5 archivos hermanos -----------------------------------
 *
 *     consagracion_onboarding.c   PIPELINE          - CERO -> ACTIVO
 *     consagracion_fases.c        SHEVA' MA'ASIM    - las 7 fases  <- ESTE
 *     consagracion_sellos.c       OZEN+BOHEN+REGEL  - Ex 29:20
 *     consagracion_lockdown.c     VEZAR LO YOKHAL   - Ex 29:33-34
 *     consagracion_cuarentena.c   MIJUTS LAMMAJANEH - Ex 29:14
 *
 *   Archivo 2 de 5 de la Consagracion. Siguiente:
 *     consagracion_sellos.c - los 3 sellos + firma de config
 *
 *   PASO 9 de Exodo 40:12-15 - Pieza 10, Archivo 2 de 5.
 *
 * ================================================================== */
