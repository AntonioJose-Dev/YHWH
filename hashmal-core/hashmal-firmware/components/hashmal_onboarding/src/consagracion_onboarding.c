/**
 * ===================================================================
 * PIEZA 10 - MILLUIM - consagracion_onboarding.c
 * Sub-pieza: PIPELINE - CERO -> EN_PROCESO -> ACTIVO
 * Exodo 29:1-4 + Levitico 8:1-6 + Levitico 9:1 + Numeros 3:3
 * ===================================================================
 *
 * "vezeh haddavar asher ta'aseh LAHEM leQADDESH otam leJAHEN li:
 *  laqaj par ejad ben baqar ve'eilim shnayim TEMIMIM."
 *   - Exodo 29:1
 * "Y ESTO es lo que HARAS con ellos para CONSAGRARLOS para que me
 *  sirvan como sacerdotes: toma un becerro hijo de ganado y dos
 *  carneros SIN DEFECTO."
 *
 * "vehiqravta et Aharon ve'et banav el PETAJ ohel mo'ed veraJATSTA
 *  otam BAMMAYIM." - Exodo 29:4
 * "Haras ACERCAR a Aaron y a sus hijos a la PUERTA de la Tienda
 *  del Encuentro y los LAVARAS con AGUA."
 *
 * "vayyomer YHWH el Moshe lemor: qaj et Aharon ve'et banav itto
 *  ve'et habbegadim ve'et shemen hammishja... vehaqhel et kol ha'eda
 *  el petaj ohel mo'ed." - Levitico 8:1-3
 * "Y hablo YHWH a Moises diciendo: Toma a Aaron y a sus hijos con
 *  el, las vestiduras y el aceite de la uncion... y congrega a toda
 *  la congregacion a la puerta de la Tienda del Encuentro."
 *
 * "vayyaqreb Moshe et Aharon ve'et banav vayyirJATS otam
 *  BAMMAYIM." - Levitico 8:6
 * "Y Moises hizo ACERCAR a Aaron y a sus hijos y los LAVO con AGUA."
 *
 * "vayhi bayyom HASHEMINI qara Moshe leAharon ulvanav ulziqnei
 *  Yisrael." - Levitico 9:1
 * "Y fue AL DIA OCTAVO: Moises llamo a Aaron y a sus hijos y a
 *  los ancianos de Israel."
 *
 * "eleh shemot benei Aharon hakkohanim HAMMESHUJIM asher mille'
 *  yadam leJAHEN." - Numeros 3:3
 * "Estos son los nombres de los hijos de Aaron, sacerdotes UNGIDOS,
 *  cuyas MANOS LLENO para ejercer el sacerdocio."
 *
 * --- VOCABULARIO ---
 *
 *   milluim    (MLAYM, H4394)         LLENADO - consagracion es
 *                                          llenar de autoridad.
 *   qiddesh    (QDSh, H6942 Piel)      SANTIFICAR, separar.
 *   rajats     (RChTs, H7364)              LAVAR - primer acto
 *                                          (Ex 29:4 + Lv 8:6).
 *   taqrib     (TQRYB, H7126 Hif)     HACER ACERCAR - traer al
 *                                          candidato a la puerta.
 *   petaj ohel mo'ed                       PUERTA de la Tienda -
 *                                          umbral del proceso.
 *   shemini    (ShMYNY, H8066)          OCTAVO - dia de lanzamiento
 *                                          (Lv 9:1 + Lv 14:10).
 *   hammeshujim (HMShChYM, H4886 pas) LOS UNGIDOS (Nm 3:3).
 *   tamid      (TMYD, H8548)             CONTINUO - servicio
 *                                          perpetuo post-lanzamiento
 *                                          (Ex 29:38-39).
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   hashmal_onboarding_init      rajats - lavar el subsistema
 *                                 (Ex 29:4 + Lv 8:6).
 *   hashmal_onboarding_deinit    desmantelar con dignidad
 *                                 (Nm 4:5-6 + Nm 10:17).
 *   hashmal_onboarding_estado    lectura pura per-nodo.
 *   hashmal_onboarding_activo    bool puro per-nodo.
 *   hashmal_onboarding_iniciar   CERO -> EN_PROCESO (taqrib,
 *                                 Ex 29:4 + Lv 8:6).
 *   hashmal_onboarding_lanzar    EN_PROCESO -> ACTIVO (dia 8,
 *                                 Lv 9:1 + Nm 3:3).
 *   hashmal_onboarding_get       getter publico const.
 *
 *   + helpers internos (hashmal_onboarding_internal.h):
 *   consagracion_mut             mutable para hermanos.
 *   get_consagracion_interno     const interno.
 *   ahora_ms                     monotonic clock ms.
 *
 * --- DEPENDENCIA CON hashmal_roles ---
 *
 * Ex 29:5 "velaqajta et habegadim vehiblashta" - TOMA las
 * vestiduras y VISTE a Aaron. La consagracion presupone la
 * vestidura: Lv 8:7 ejecuta el vestir ANTES del ungir (v.12).
 * Por eso `iniciar` exige que el nodo ya este vestido en pieza 09
 * (hashmal_roles). Consulta via API publica (hashmal_roles_get_estado)
 * - NO se incluye hashmal_roles_internal.h.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_onboarding.h
 *   hashmal_onboarding_internal.h
 *   hashmal_roles.h              verificar vestidura previa
 *   esp_log.h    (ORO EGIPTO)   logging
 *   esp_timer.h  (ORO EGIPTO)   ahora_ms
 *
 * Autor: Antonio Jose Marin Soto (Betsalel - Ex 31:2)
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "esp_log.h"
#include "esp_timer.h"

#include "hashmal_onboarding.h"
#include "hashmal_onboarding_internal.h"
#include "hashmal_roles.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *ONB_TAG = "hashmal.onboarding.pipeline";

#define ONB_LOG_I(fmt, ...)  ESP_LOGI(ONB_TAG, fmt, ##__VA_ARGS__)
#define ONB_LOG_W(fmt, ...)  ESP_LOGW(ONB_TAG, fmt, ##__VA_ARGS__)
#define ONB_LOG_E(fmt, ...)  ESP_LOGE(ONB_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * SINGLETON SAGRADO
 *
 * UN subsistema de consagracion por nodo - Ex 29:1 "vezeh haddavar
 * asher ta'aseh lahem" (singular). Un proceso, una puerta, una
 * tabla de 12 candidatos.
 * ================================================================== */
static hashmal_onboarding_t g_consagracion;
static bool                 g_iniciado = false;

/* ==================================================================
 * API INTERNA - helpers compartidos (hashmal_onboarding_internal.h)
 * ================================================================== */

hashmal_onboarding_t *hashmal_onboarding_consagracion_mut(void)
{
    if (!g_iniciado) return NULL;
    return &g_consagracion;
}

const hashmal_onboarding_t *hashmal_onboarding_get_consagracion_interno(void)
{
    if (!g_iniciado) return NULL;
    return &g_consagracion;
}

int64_t hashmal_onboarding_ahora_ms(void)
{
    return (int64_t)(esp_timer_get_time() / 1000);
}

/* ==================================================================
 * HELPERS LOCALES (autocontencion kelim)
 * ================================================================== */

static inline bool nodo_id_valido(uint8_t nodo_id)
{
    return nodo_id < HASHMAL_ONBOARDING_MAX_NODOS;
}

static const char *nombre_estado(hashmal_onboarding_estado_t e)
{
    switch (e) {
    case HASHMAL_ONBOARDING_CERO:       return "CERO (antes de qiddésh, H6942)";
    case HASHMAL_ONBOARDING_EN_PROCESO: return "EN_PROCESO (milluím, H4394)";
    case HASHMAL_ONBOARDING_ACTIVO:     return "ACTIVO (sheminí, H8066, Lv 9:1)";
    default:                            return "<estado desconocido>";
    }
}

/**
 * Verifica que el nodo este vestido en pieza 09 (hashmal_roles).
 * Ex 29:5 dicta el orden: PRIMERO vestir, DESPUES consagrar. Usamos
 * solo la API publica de roles - get_estado() + consulta del flag
 * vestido del nodo. Sin acceso a internals ajenos.
 *
 * 2 testigos:
 *   Ex 29:5  - velaqajta et habegadim vehiblashta (mandato)
 *   Lv 8:7   - vayyitten 'alav et hakuttonet (ejecucion antes de v.12 ungir)
 */
static bool nodo_esta_vestido_en_roles(uint8_t nodo_id)
{
    const hashmal_roles_estado_roles_t *r = hashmal_roles_get_estado();
    if (r == NULL) return false;
    if (nodo_id >= HASHMAL_ROLES_MAX_NODOS) return false;
    return r->nodos[nodo_id].vestido;
}

/* ==================================================================
 * hashmal_onboarding_init - Exodo 29:4 + Levitico 8:6
 *
 * "veraJATSTA otam BAMMAYIM" (Ex 29:4 mandato) + "vayyirJATS otam
 * BAMMAYIM" (Lv 8:6 ejecucion). El primer acto de la consagracion
 * es LAVAR: en firmware, memset del singleton borra cualquier rastro
 * previo. Partimos de "puerta limpia" - petaj ohel mo'ed sin restos.
 *
 * Idempotencia: ya iniciado -> ERR_YA_INIT (contrato del header).
 *
 * 2 testigos:
 *   Ex 29:4 - rajats (mandato)
 *   Lv 8:6  - vayyirjats (ejecucion, libro distinto)
 * ================================================================== */
int hashmal_onboarding_init(void)
{
    if (g_iniciado) {
        ONB_LOG_W("init: subsistema ya iniciado (idempotente)");
        return HASHMAL_ONBOARDING_ERR_YA_INIT;
    }

    /* Rajats - lavado ritual. Estado post-memset: todos los slots
     * en CERO (enum value 0), sellos false, ciclos 0, contadores 0. */
    memset(&g_consagracion, 0, sizeof(g_consagracion));

    const int64_t ahora = hashmal_onboarding_ahora_ms();

    /* Inicializacion explicita por slot - redundante tras memset,
     * pero documenta el contrato de cada campo. */
    for (uint8_t i = 0; i < HASHMAL_ONBOARDING_MAX_NODOS; i++) {
        hashmal_onboarding_nodo_t *n = &g_consagracion.nodos[i];
        n->estado             = HASHMAL_ONBOARDING_CERO;
        n->fase_actual        = HASHMAL_ONBOARDING_FASE_RAJATS;
        n->ciclo_actual       = 0u;
        n->config_firmada     = false;
        n->lockdown           = false;
        n->cuarentena         = false;
        n->timestamp_inicio_ms = 0;
        n->timestamp_fase_ms   = 0;
        for (uint8_t s = 0; s < HASHMAL_ONBOARDING_SELLOS_TOTAL; s++) {
            n->sellos[s] = false;
        }
        for (uint8_t f = 0; f < HASHMAL_ONBOARDING_FASES_TOTAL; f++) {
            n->fases_completadas[f] = false;
        }
    }

    g_consagracion.nodos_en_proceso = 0u;
    g_consagracion.nodos_activos    = 0u;
    g_consagracion.timestamp_ms     = ahora;
    g_consagracion.iniciado         = true;

    g_iniciado = true;

    ONB_LOG_I("══════ MILLUÍM — subsistema iniciado (Éx 29:1) ══════");
    ONB_LOG_I("  rajáts — subsistema lavado (Éx 29:4 + Lv 8:6)");
    ONB_LOG_I("  %u slots en CERO, esperando taqríb el pétaj óhel moʻéd",
              (unsigned)HASHMAL_ONBOARDING_MAX_NODOS);
    ONB_LOG_I("  7 fases dispuestas (Éx 29:1-28 + Lv 8:6-29)");
    ONB_LOG_I("  shivʻát yamím pendientes (Éx 29:35 + Lv 8:33)");

    return HASHMAL_ONBOARDING_OK;
}

/* ==================================================================
 * hashmal_onboarding_deinit - Numeros 4:5-6 + 10:17
 *
 * "uva Aharon uvanav binsoa' hammajaneh vehoridu et parojet...
 *  vejissu 'alav kesui 'or tajash." - Nm 4:5-6
 * Al desmontar el campamento, los sagrados se CUBREN (no se
 * destruyen). Nm 10:17 "vehurad hammishkan" - fue desmontado
 * ordenadamente. Aqui: memset (cubrir) y flag bajado.
 *
 * Idempotente silencioso si no iniciado.
 *
 * 2 testigos:
 *   Nm 4:5-6  - vejissu 'alav (cubrir con pieles)
 *   Nm 10:17  - vehurad hammishkan (desmontar el Mishkan)
 * ================================================================== */
void hashmal_onboarding_deinit(void)
{
    if (!g_iniciado) {
        /* Idempotente silencioso - ya cubierto. */
        return;
    }

    ONB_LOG_I("subsistema cubierto (Nm 4:5-6 + Nm 10:17) — "
              "milluím en reposo");

    memset(&g_consagracion, 0, sizeof(g_consagracion));
    g_iniciado = false;
}

/* ==================================================================
 * hashmal_onboarding_estado - lectura pura
 *
 * Sin logs, sin side effects. Escribe en *out el estado del nodo.
 *
 *   out NULL               -> ERR_NULL
 *   sin init               -> ERR_NO_INIT
 *   nodo_id fuera de rango -> ERR_NODO_INVALIDO
 * ================================================================== */
int hashmal_onboarding_estado(uint8_t nodo_id,
                               hashmal_onboarding_estado_t *out)
{
    if (out == NULL) return HASHMAL_ONBOARDING_ERR_NULL;
    if (!g_iniciado) return HASHMAL_ONBOARDING_ERR_NO_INIT;
    if (!nodo_id_valido(nodo_id)) {
        return HASHMAL_ONBOARDING_ERR_NODO_INVALIDO;
    }

    *out = (hashmal_onboarding_estado_t)g_consagracion.nodos[nodo_id].estado;
    return HASHMAL_ONBOARDING_OK;
}

/* ==================================================================
 * hashmal_onboarding_activo - bool puro
 *
 * true si estado == ACTIVO. false para cualquier otro caso
 * (sin init, fuera de rango, CERO, EN_PROCESO). Sin logs.
 * ================================================================== */
bool hashmal_onboarding_activo(uint8_t nodo_id)
{
    if (!g_iniciado) return false;
    if (!nodo_id_valido(nodo_id)) return false;
    return g_consagracion.nodos[nodo_id].estado ==
           HASHMAL_ONBOARDING_ACTIVO;
}

/* ==================================================================
 * hashmal_onboarding_iniciar - Exodo 29:4 + Levitico 8:6
 *
 * "vehiqravta et Aharon... el petaj ohel mo'ed" (Ex 29:4 taqrib)
 * + "vayyaqreb Moshe et Aharon" (Lv 8:6 ejecucion). El candidato
 * se hace ACERCAR a la puerta. En firmware: transicion CERO ->
 * EN_PROCESO, fase RAJATS activa, timestamps de arranque fijados,
 * contador incrementado.
 *
 * Prerrequisito (Ex 29:5 + Lv 8:7): el nodo debe estar VESTIDO
 * en pieza 09. Sin vestiduras -> ERR_NO_VESTIDO.
 *
 * Validaciones en orden:
 *   1. Subsistema iniciado.
 *   2. nodo_id en rango.
 *   3. Estado == CERO (no iniciar ya iniciado).
 *   4. Vestido en roles (API publica).
 *
 * 2+ testigos:
 *   Ex 29:4    - taqrib (mandato)
 *   Lv 8:6     - vayyaqreb (ejecucion, libro distinto)
 *   Ex 29:5    - vehiblashta (vestir antes de consagrar)
 *   Lv 8:7     - vayyitten 'alav hakuttonet (ejecucion del orden)
 * ================================================================== */
int hashmal_onboarding_iniciar(uint8_t nodo_id)
{
    if (!g_iniciado) {
        ONB_LOG_E("iniciar: subsistema no iniciado (init primero)");
        return HASHMAL_ONBOARDING_ERR_NO_INIT;
    }

    if (!nodo_id_valido(nodo_id)) {
        ONB_LOG_W("iniciar: nodo_id %u fuera de rango [0..%u]",
                  (unsigned)nodo_id,
                  (unsigned)(HASHMAL_ONBOARDING_MAX_NODOS - 1u));
        return HASHMAL_ONBOARDING_ERR_NODO_INVALIDO;
    }

    hashmal_onboarding_nodo_t *n = &g_consagracion.nodos[nodo_id];

    if (n->estado != HASHMAL_ONBOARDING_CERO) {
        ONB_LOG_W("iniciar: nodo %u ya en %s — se esperaba CERO "
                  "(Éx 29:1 vezéh haddavár — proceso desde el principio)",
                  (unsigned)nodo_id,
                  nombre_estado((hashmal_onboarding_estado_t)n->estado));
        return HASHMAL_ONBOARDING_ERR_FASE_INCORRECTA;
    }

    /* Ex 29:5 + Lv 8:7 - vestir precede consagrar. */
    if (!nodo_esta_vestido_en_roles(nodo_id)) {
        ONB_LOG_W("iniciar: nodo %u no vestido en pieza 09 — "
                  "Éx 29:5 vehibláshta antes de consagrar "
                  "(Lv 8:7 vayyittén ʻaláv hakuttónet)",
                  (unsigned)nodo_id);
        return HASHMAL_ONBOARDING_ERR_NO_VESTIDO;
    }

    const int64_t ahora = hashmal_onboarding_ahora_ms();

    n->estado             = HASHMAL_ONBOARDING_EN_PROCESO;
    n->fase_actual        = HASHMAL_ONBOARDING_FASE_RAJATS;
    n->ciclo_actual       = 0u;
    n->timestamp_inicio_ms = ahora;
    n->timestamp_fase_ms   = ahora;

    /* Sellos, fases, config, lockdown y cuarentena permanecen false
     * - los establecen los archivos hermanos a lo largo del proceso. */

    /* P9 — herencia: inicialmente sin predecesor. */
    n->nodo_predecesor_id = HASHMAL_ONBOARDING_CENTINELA;

    if (g_consagracion.nodos_en_proceso < HASHMAL_ONBOARDING_MAX_NODOS) {
        g_consagracion.nodos_en_proceso++;
    }
    g_consagracion.timestamp_ms = ahora;

    ONB_LOG_I("nodo %u: CERO → EN_PROCESO (taqríb el pétaj óhel "
              "moʻéd — Éx 29:4 + Lv 8:6) — fase RAJATS activa",
              (unsigned)nodo_id);

    return HASHMAL_ONBOARDING_OK;
}

/* ==================================================================
 * hashmal_onboarding_lanzar - Levitico 9:1 + Numeros 3:3
 *
 * "vayhi bayyom HASHEMINI qara Moshe leAharon ulvanav" (Lv 9:1)
 * + "hakkohanim HAMMESHUJIM asher mille' yadam leJAHEN" (Nm 3:3).
 * El dia 8: el sacerdote SALE de la puerta y comienza a servir.
 * Antes de transitar a ACTIVO, verificamos TODO:
 *   1. estado == EN_PROCESO.
 *   2. fase_actual == SHIVAT (se llego a la ultima fase).
 *   3. fases_completadas[0..6] TODAS true (no hay hueco).
 *   4. ciclo_actual >= CICLOS_PRUEBA (7 dias).
 *   5. sellos[0..2] TODOS true (Ex 29:20).
 *   6. config_firmada == true (Ex 29:21).
 *   7. lockdown == true (Ex 29:33-34).
 *
 * Todo esto es "dia 8": no se sale antes de terminar los 7 (Lv 8:33
 * "lo tetse'u shiv'at yamim"). Si algo falta -> error especifico.
 *
 * 2+ testigos:
 *   Lv 9:1  - bayyom hashemini
 *   Nm 3:3  - hakkohanim hammeshujim asher mille' yadam (libro distinto)
 *   Lv 8:33 - lo tetse'u shiv'at yamim (confirmacion: no antes)
 * ================================================================== */
int hashmal_onboarding_lanzar(uint8_t nodo_id)
{
    if (!g_iniciado) {
        ONB_LOG_E("lanzar: subsistema no iniciado (init primero)");
        return HASHMAL_ONBOARDING_ERR_NO_INIT;
    }

    if (!nodo_id_valido(nodo_id)) {
        ONB_LOG_W("lanzar: nodo_id %u fuera de rango",
                  (unsigned)nodo_id);
        return HASHMAL_ONBOARDING_ERR_NODO_INVALIDO;
    }

    hashmal_onboarding_nodo_t *n = &g_consagracion.nodos[nodo_id];

    /* 1. Estado debe ser EN_PROCESO. */
    if (n->estado == HASHMAL_ONBOARDING_ACTIVO) {
        ONB_LOG_W("lanzar: nodo %u ya ACTIVO — no se relanza",
                  (unsigned)nodo_id);
        return HASHMAL_ONBOARDING_ERR_YA_ACTIVO;
    }
    if (n->estado != HASHMAL_ONBOARDING_EN_PROCESO) {
        ONB_LOG_W("lanzar: nodo %u en %s — se esperaba EN_PROCESO "
                  "(Éx 29:35 shivʻát yamím antes de sheminí)",
                  (unsigned)nodo_id,
                  nombre_estado((hashmal_onboarding_estado_t)n->estado));
        return HASHMAL_ONBOARDING_ERR_FASE_INCORRECTA;
    }

    /* 2. Debe estar en fase SHIVAT (ultima). */
    if (n->fase_actual != HASHMAL_ONBOARDING_FASE_SHIVAT) {
        ONB_LOG_W("lanzar: nodo %u en fase %u — se esperaba SHIVAT "
                  "(Éx 29:35 shivʻát yamím, fase 6)",
                  (unsigned)nodo_id, (unsigned)n->fase_actual);
        return HASHMAL_ONBOARDING_ERR_FASE_INCORRECTA;
    }

    /* 3. Todas las 7 fases completadas. */
    for (uint8_t f = 0; f < HASHMAL_ONBOARDING_FASES_TOTAL; f++) {
        if (!n->fases_completadas[f]) {
            ONB_LOG_W("lanzar: nodo %u fase %u incompleta "
                      "(Éx 29:1-28 — las 7 fases son obligatorias)",
                      (unsigned)nodo_id, (unsigned)f);
            return HASHMAL_ONBOARDING_ERR_FASE_INCORRECTA;
        }
    }

    /* 4. Ciclos completos. Lv 8:33 - 7 dias ANTES de salir. */
    if (n->ciclo_actual < HASHMAL_ONBOARDING_CICLOS_PRUEBA) {
        ONB_LOG_W("lanzar: nodo %u ciclos=%u/%u — Lv 8:33 "
                  "'lo tétseʼú shivʻát yamím'",
                  (unsigned)nodo_id,
                  (unsigned)n->ciclo_actual,
                  (unsigned)HASHMAL_ONBOARDING_CICLOS_PRUEBA);
        return HASHMAL_ONBOARDING_ERR_CICLOS_INCOMPLETOS;
    }

    /* 5. Los 3 sellos aplicados (Ex 29:20). */
    for (uint8_t s = 0; s < HASHMAL_ONBOARDING_SELLOS_TOTAL; s++) {
        if (!n->sellos[s]) {
            ONB_LOG_W("lanzar: nodo %u sello %u faltante "
                      "(Éx 29:20 ózen + bóhen yad + bóhen régel)",
                      (unsigned)nodo_id, (unsigned)s);
            return HASHMAL_ONBOARDING_ERR_FASE_INCORRECTA;
        }
    }

    /* 6. Config firmada (Ex 29:21 sangre+aceite en vestiduras). */
    if (!n->config_firmada) {
        ONB_LOG_W("lanzar: nodo %u config no firmada — Éx 29:21 "
                  "rociar sangre+shémen hamishjá sobre vestiduras",
                  (unsigned)nodo_id);
        return HASHMAL_ONBOARDING_ERR_CONFIG_NO_FIRMADA;
    }

    /* 7. Lockdown activo (Ex 29:33 vezar lo yokhal). */
    if (!n->lockdown) {
        ONB_LOG_W("lanzar: nodo %u sin lockdown — Éx 29:33 "
                  "recursos dedicados, vezár lo yokhál",
                  (unsigned)nodo_id);
        return HASHMAL_ONBOARDING_ERR_LOCKDOWN;
    }

    /* Todo verificado - dia 8. */
    const int64_t ahora = hashmal_onboarding_ahora_ms();
    n->estado           = HASHMAL_ONBOARDING_ACTIVO;
    n->timestamp_fase_ms = ahora;

    if (g_consagracion.nodos_en_proceso > 0u) {
        g_consagracion.nodos_en_proceso--;
    }
    if (g_consagracion.nodos_activos < HASHMAL_ONBOARDING_MAX_NODOS) {
        g_consagracion.nodos_activos++;
    }
    g_consagracion.timestamp_ms = ahora;

    ONB_LOG_I("══════ DÍA 8 — nodo %u LANZADO ══════",
              (unsigned)nodo_id);
    ONB_LOG_I("  vayhí bayyóm hasheminí (Lv 9:1)");
    ONB_LOG_I("  hakkohaním hammeshujím ashér milléʼ yadám (Nm 3:3)");
    ONB_LOG_I("  EN_PROCESO → ACTIVO; activos=%u/%u",
              (unsigned)g_consagracion.nodos_activos,
              (unsigned)HASHMAL_ONBOARDING_MAX_NODOS);

    return HASHMAL_ONBOARDING_OK;
}

/* ==================================================================
 * hashmal_onboarding_get - getter publico inmutable
 *
 * Lectura pura, sin logs, sin side effects. Retorna NULL si el
 * subsistema no esta inicializado; de lo contrario, puntero const
 * al singleton.
 * ================================================================== */
const hashmal_onboarding_t *hashmal_onboarding_get(void)
{
    if (!g_iniciado) return NULL;
    return &g_consagracion;
}

/* ==================================================================
 * P9 — hashmal_onboarding_heredar (Éx 29:29-30)
 *
 * "bigdéi haqqódesh yihyú levanáv ajaráv" — las vestiduras
 * sagradas SERÁN (permanencia constitutiva) para sus hijos DESPUÉS
 * DE ÉL. Éx 29:30: "shivʻát yamím yilbasham hakohén tajtáv
 * mibbanaáv" — 7 días las vestirá el sacerdote que VENGA EN SU LUGAR.
 *
 * El nodo sucesor hereda credenciales del predecesor retirado,
 * re-consagrándolas sobre sí mismo. No es inicio virgen: es
 * continuidad de autoridad.
 *
 * 2 testigos:
 *   Éx 29:29 - bigdéi haqqódesh yihyú levanáv ajaráv
 *   Éx 29:30 - shivʻát yamím yilbasham hakohén tajtáv
 * ================================================================== */
int hashmal_onboarding_heredar(uint8_t nodo_id, uint8_t predecesor_id)
{
    if (!g_iniciado) {
        ONB_LOG_E("heredar: subsistema no iniciado (init primero)");
        return HASHMAL_ONBOARDING_ERR_NO_INIT;
    }

    if (!nodo_id_valido(nodo_id)) {
        ONB_LOG_W("heredar: nodo_id %u fuera de rango [0..%u]",
                  (unsigned)nodo_id,
                  (unsigned)(HASHMAL_ONBOARDING_MAX_NODOS - 1u));
        return HASHMAL_ONBOARDING_ERR_NODO_INVALIDO;
    }

    /* predecesor_id puede ser 0xFF (sin predecesor) o un ID válido. */
    if (predecesor_id != HASHMAL_ONBOARDING_CENTINELA &&
        !nodo_id_valido(predecesor_id)) {
        ONB_LOG_W("heredar: predecesor_id %u inválido (esperado [0..%u] o 0xFF)",
                  (unsigned)predecesor_id,
                  (unsigned)(HASHMAL_ONBOARDING_MAX_NODOS - 1u));
        return HASHMAL_ONBOARDING_ERR_NODO_INVALIDO;
    }

    /* No se puede heredar de sí mismo. */
    if (predecesor_id != HASHMAL_ONBOARDING_CENTINELA &&
        nodo_id == predecesor_id) {
        ONB_LOG_W("heredar: nodo %u intenta heredar de sí mismo",
                  (unsigned)nodo_id);
        return HASHMAL_ONBOARDING_ERR_NODO_INVALIDO;
    }

    hashmal_onboarding_nodo_t *n = &g_consagracion.nodos[nodo_id];

    /* El nodo sucesor debe estar en CERO — virgen, listo para herencia. */
    if (n->estado != HASHMAL_ONBOARDING_CERO) {
        ONB_LOG_W("heredar: nodo %u ya en %s — se esperaba CERO "
                  "(Éx 29:29 para herencia limpia)",
                  (unsigned)nodo_id,
                  nombre_estado((hashmal_onboarding_estado_t)n->estado));
        return HASHMAL_ONBOARDING_ERR_YA_ACTIVO;
    }

    /* Si hay predecesor, verificar que sus credenciales sean válidas
     * (Éx 29:30 confirmación de continuidad). */
    if (predecesor_id != HASHMAL_ONBOARDING_CENTINELA) {
        hashmal_onboarding_nodo_t *pred = &g_consagracion.nodos[predecesor_id];

        /* Predecesor debe tener al menos ALGUNAS credenciales (fase avanzada). */
        bool pred_tiene_credenciales = false;
        for (uint8_t f = HASHMAL_ONBOARDING_FASE_HILBISH;
             f < HASHMAL_ONBOARDING_FASES_TOTAL; ++f) {
            if (pred->fases_completadas[f]) {
                pred_tiene_credenciales = true;
                break;
            }
        }

        if (!pred_tiene_credenciales) {
            ONB_LOG_W("heredar: predecesor %u sin credenciales válidas "
                      "(ninguna fase completada) — Éx 29:30 requiere "
                      "continuidad de autoridad",
                      (unsigned)predecesor_id);
            return HASHMAL_ONBOARDING_ERR_NO_VESTIDO;
        }
    }

    /* === HERENCIA APROBADA === */
    const int64_t ahora = hashmal_onboarding_ahora_ms();

    n->nodo_predecesor_id = predecesor_id;
    n->estado             = HASHMAL_ONBOARDING_EN_PROCESO;
    /* Fase HILBISH: ya "vestido" por herencia, no necesita rajats. */
    n->fase_actual        = HASHMAL_ONBOARDING_FASE_HILBISH;
    n->ciclo_actual       = 0u;
    n->timestamp_inicio_ms = ahora;
    n->timestamp_fase_ms   = ahora;

    /* Marcar RAJATS como completada (el predecesor ya pasó por ahí). */
    n->fases_completadas[HASHMAL_ONBOARDING_FASE_RAJATS] = true;
    /* HILBISH entra como fase actual — se completará cuando avance. */

    if (g_consagracion.nodos_en_proceso < HASHMAL_ONBOARDING_MAX_NODOS) {
        g_consagracion.nodos_en_proceso++;
    }
    g_consagracion.timestamp_ms = ahora;

    ONB_LOG_I("nodo %u: HERENCIA de nodo %u (Éx 29:29 bigdéi haqqódesh) "
              "— CERO → EN_PROCESO (fase HILBISH directa)",
              (unsigned)nodo_id,
              (unsigned)predecesor_id);
    ONB_LOG_I("  shivʻát yamím yilbasham hakohén tajtáv (Éx 29:30) "
              "— continuidad de credenciales");

    return HASHMAL_ONBOARDING_OK;
}

/* ==================================================================
 * hashmal_onboarding_predecesor_leer - lectura pura de herencia
 *
 * Retorna el ID del predecesor sin side-effects.
 *
 *   out NULL               -> ERR_NULL
 *   sin init               -> ERR_NO_INIT
 *   nodo_id fuera de rango -> ERR_NODO_INVALIDO
 * ================================================================== */
int hashmal_onboarding_predecesor_leer(uint8_t nodo_id, uint8_t *out)
{
    if (out == NULL) return HASHMAL_ONBOARDING_ERR_NULL;
    if (!g_iniciado) return HASHMAL_ONBOARDING_ERR_NO_INIT;
    if (!nodo_id_valido(nodo_id)) {
        return HASHMAL_ONBOARDING_ERR_NODO_INVALIDO;
    }

    *out = g_consagracion.nodos[nodo_id].nodo_predecesor_id;
    return HASHMAL_ONBOARDING_OK;
}

/* ==================================================================
 *
 *   "vezeh haddavar asher ta'aseh LAHEM leQADDESH otam leJAHEN li."
 *     - Exodo 29:1
 *
 *   "vayyirJATS otam BAMMAYIM." - Levitico 8:6
 *
 *   "vayhi bayyom HASHEMINI qara Moshe leAharon ulvanav." - Lv 9:1
 *
 *   "eleh shemot benei Aharon hakkohanim HAMMESHUJIM asher mille'
 *    yadam leJAHEN." - Numeros 3:3
 *
 *   La consagracion no es ceremonia vacia - es LLENADO de manos.
 *   Quien entra esta CERO: no tiene oficio propio, solo vestiduras
 *   puestas (pieza 09). El proceso lo toma desde la puerta (taqrib),
 *   lo lava (rajats), recorre siete fases ordenadas, permanece
 *   siete dias en el umbral, y al octavo sale a servir con manos
 *   llenas. Nada se salta; nada se improvisa. Si falla algun
 *   requisito al intentar lanzar, la puerta se mantiene cerrada
 *   (Lv 8:33 lo tetse'u), con la razon concreta registrada en log.
 *
 *   === PIPELINE - consagracion_onboarding.c COMPLETO ===
 *
 *   Los 5 archivos hermanos de la consagracion:
 *
 *     consagracion_onboarding.c   PIPELINE       - CERO -> ACTIVO
 *     consagracion_fases.c        7 FASES        - Ex 29:1-28
 *     consagracion_sellos.c       3 SELLOS       - Ex 29:20
 *     consagracion_lockdown.c     VEZAR LO YOKHAL - Ex 29:33-34
 *     consagracion_cuarentena.c   MIJUTS LAMMAJANEH - Ex 29:14
 *
 *   Archivo 1 de 5 de la Consagracion. Siguiente:
 *     consagracion_fases.c - avanzar las 7 fases del milluim
 *
 *   PASO 9 de Exodo 40:12-15 - Pieza 10, Archivo 1 de 5.
 *
 * ================================================================== */
