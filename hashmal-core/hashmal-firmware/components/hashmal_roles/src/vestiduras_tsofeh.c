/*
 * ===================================================================
 * PIEZA 09 - VESTIDURAS - vestiduras_tsofeh.c
 * Añadidura 02: EL ATALAYA (TSOFEH)
 * Ezequiel 33:2-6 + Habacuc 2:1 + Isaias 21:6-12 + Numeros 10:8-9
 * ===================================================================
 *
 * "ben adam, dabber el benei ammekha veamarta aleihem: erets ki
 *  aviy aleiha jarev ulqejhu am haarets ish ejad miqtseihem venatnu
 *  oto lahem leTSOFEH. vera'a et hajerev ba'a 'al haarets veTAQA'
 *  bashshofar vehizhir et ha'am." - Ezequiel 33:2-3
 *
 * "vehatsofeh ki yir'a et hajerev ba'a veLO taqa' bashshofar veha'am
 *  lo nizhar... damo miyyado adrosh." - Ezequiel 33:6
 *
 * "'al MISHMARTI e'emoda ve'etyatseba 'al matsor va'atsappe lir'ot
 *  ma-yedabber bi uma ashiv 'al tokajti." - Habacuc 2:1
 * "Sobre mi GUARDIA estare firme y observare para ver que me dira."
 *
 * "lej ha'amed hammetsappe asher yir'a yaggid." - Isaias 21:6
 * "Ve, pon CENTINELA que declare lo que vea."
 *
 * "shomer mah milailah shomer mah milel." - Isaias 21:11
 * "Guarda, ?que hay de la noche? Guarda, ?que de la noche?"
 *
 * "utqa'tem bajatsotserot... venizkartem lifnei YHWH eloheikem."
 *   - Numeros 10:9
 *
 * --- INVARIANTE INV-TSOFEH-01 ---
 *
 * Ez 33:6 establece: el atalaya que VE y no AVISA carga sangre en
 * su mano. En codigo: `vigilar_y_avisar` ejecuta deteccion Y shofar
 * en una sola funcion - no existe un code path donde un caller reciba
 * "amenazas != 0" sin que el shofar haya sido tocado. Si el shofar
 * FALLA en emitirse, la funcion retorna ERR_TSOFEH - dejando al
 * caller saber que la espada viene y no se pudo avisar.
 *
 * --- 6 FUENTES DE AMENAZA ---
 *
 *   1. NODO_CAIDO    - heartbeat en SILENCIO en algun nodo del efod
 *   2. TRAFICO_ANOM  - delta de errores por ner > umbral
 *   3. FRAGMENTACION - vecinos < FRAGMENT_VECINOS (red partida)
 *   4. JAMMING       - rssi_floor LoRa >= -85 dBm (ruido alto banda)
 *   5. NO_AUTORIZADO - firewall veredicto ZAR delta > 0 (esh zara)
 *   6. SHAON_RAASH   - parokhet_indice clasifica RAASH (1 Re 19:11)
 *
 * --- CROSS-PIECE ---
 *
 * Este archivo consulta:
 *   Pieza 04 (hashmal_comm)       - menora, vecinos, rssi, shofar
 *   Pieza 09 (este componente)    - monitor_estado_nodo heartbeat
 *   Pieza 12 (hashmal_healthcheck) - parokhet_indice
 *   Pieza 14 (hashmal_firewall)   - veredictos ZAR delta
 *
 * Nuevo acoplamiento necesario (Ez 33:7 tsofeh sobre TODO): el
 * atalaya ve todas las fronteras.
 *
 * --- POSICION FIJA (punto alto) ---
 *
 * Operativamente interpretado: el atalaya es declarado por
 * configuracion al boot (no auto-detectado). Una vez activar(true),
 * permanece asi hasta activar(false). La posicion "punto alto" es
 * metafora para "nodo con buena recepcion", verificada por operator
 * - fuera del alcance del firmware.
 *
 * Autor: Antonio Jose Marin Soto (Betsalel - Ex 31:2)
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <inttypes.h>

#include <stdio.h>

#include "esp_log.h"

#include "hashmal_roles.h"
#include "hashmal_roles_internal.h"
#include "hashmal_comm.h"
#include "hashmal_healthcheck.h"
#include "hashmal_firewall.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *TSOFEH_TAG = "hashmal.roles.tsofeh";

#define TSOFEH_LOG_I(fmt, ...)  ESP_LOGI(TSOFEH_TAG, fmt, ##__VA_ARGS__)
#define TSOFEH_LOG_W(fmt, ...)  ESP_LOGW(TSOFEH_TAG, fmt, ##__VA_ARGS__)
#define TSOFEH_LOG_E(fmt, ...)  ESP_LOGE(TSOFEH_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * Estado interno + baseline
 *
 * El baseline se captura al activar(true): snapshots iniciales de
 * los contadores y del veredicto ZAR del firewall. Las detecciones
 * comparan "ahora" contra el baseline para ver si algo CRECIO.
 * ================================================================== */

typedef struct {
    uint64_t errores_por_ner[HASHMAL_MENORA_CANALES];
    uint8_t  vecinos_count;
    uint32_t firewall_zar;
    bool     capturado;
} tsofeh_baseline_t;

static bool             g_tsofeh_activo = false;
static tsofeh_baseline_t g_baseline;

/* Umbral: un canal que sume > ERRORES_ANOM_DELTA nuevos errores
 * desde el baseline cuenta como "trafico anomalo". Valor inicial
 * conservador - afinar con telemetria en fase 2. */
#define TSOFEH_ERRORES_ANOM_DELTA  5ull

/* ==================================================================
 * Hook de alarma local (weak)
 *
 * El integrador del board puede reimplementar esta funcion para
 * encender buzzer / LED / GPIO. Por defecto: solo log WARN. El
 * firmware core no toca hardware de UI - separacion de capas.
 * ================================================================== */
__attribute__((weak))
void hashmal_tsofeh_alarma_local(uint32_t amenazas)
{
    TSOFEH_LOG_W("ALARMA LOCAL - amenazas=0x%08" PRIx32 " "
                 "(integrator puede sobrecargar hashmal_tsofeh_alarma_local "
                 "para buzzer/LED/GPIO)",
                 amenazas);
}

/* ==================================================================
 * Captura/reset del baseline
 * ================================================================== */
static void baseline_capturar(void)
{
    memset(&g_baseline, 0, sizeof(g_baseline));

    const hashmal_menora_t *men = hashmal_comm_get_menora();
    if (men != NULL) {
        for (uint8_t i = 0; i < HASHMAL_MENORA_CANALES; ++i) {
            g_baseline.errores_por_ner[i] = men->nerot[i].errores;
        }
    }

    uint8_t vec = 0;
    if (hashmal_comm_vecinos_contar(&vec) == HASHMAL_COMM_OK) {
        g_baseline.vecinos_count = vec;
    }

    const hashmal_firewall_t *fw = hashmal_firewall_get_estado();
    if (fw != NULL) {
        g_baseline.firewall_zar =
            fw->estadisticas.veredictos[HASHMAL_FIREWALL_ZAR];
    }

    g_baseline.capturado = true;
}

static void baseline_liberar(void)
{
    memset(&g_baseline, 0, sizeof(g_baseline));
}

/* ==================================================================
 * Detectores individuales (6)
 *
 * Cada uno retorna el flag correspondiente si detecta, o 0 si no.
 * Son puros: solo leen estado publico; no mutan nada.
 * ================================================================== */

/* 1. NODO_CAIDO: algun nodo con heartbeat SILENCIO. */
static uint32_t detect_nodo_caido(void)
{
    for (uint8_t i = 0; i < HASHMAL_ROLES_MAX_NODOS; ++i) {
        hashmal_roles_heartbeat_t hb = HASHMAL_ROLES_HEARTBEAT_SONANDO;
        hashmal_roles_err_t rc = hashmal_roles_monitor_estado_nodo(i, &hb);
        if (rc != HASHMAL_ROLES_OK) continue;  /* nodo ausente */
        if (hb == HASHMAL_ROLES_HEARTBEAT_SILENCIO) {
            TSOFEH_LOG_W("detect: nodo %u en SILENCIO (Ex 28:35 velo yamut "
                         "invertido)", (unsigned)i);
            return HASHMAL_TSOFEH_AMENAZA_NODO_CAIDO;
        }
    }
    return 0u;
}

/* 2. TRAFICO_ANOM: delta de errores > TSOFEH_ERRORES_ANOM_DELTA en
 *    algun ner contra el baseline. */
static uint32_t detect_trafico_anomalo(void)
{
    if (!g_baseline.capturado) return 0u;

    const hashmal_menora_t *men = hashmal_comm_get_menora();
    if (men == NULL) return 0u;

    for (uint8_t i = 0; i < HASHMAL_MENORA_CANALES; ++i) {
        uint64_t ahora     = men->nerot[i].errores;
        uint64_t baseline  = g_baseline.errores_por_ner[i];
        uint64_t delta     = (ahora > baseline) ? (ahora - baseline) : 0ull;
        if (delta > TSOFEH_ERRORES_ANOM_DELTA) {
            TSOFEH_LOG_W("detect: canal %u errores +%" PRIu64 " (baseline=%"
                         PRIu64 ")", (unsigned)i, delta, baseline);
            return HASHMAL_TSOFEH_AMENAZA_TRAFICO_ANOM;
        }
    }
    return 0u;
}

/* 3. FRAGMENTACION: vecinos < FRAGMENT_VECINOS. */
static uint32_t detect_fragmentacion(void)
{
    uint8_t conteo = 0;
    int rc = hashmal_comm_vecinos_contar(&conteo);
    if (rc != HASHMAL_COMM_OK) return 0u;

    if (conteo < HASHMAL_TSOFEH_FRAGMENT_VECINOS) {
        TSOFEH_LOG_W("detect: fragmentacion - %u vecinos < %u (Ez 13:5 "
                     "lo 'alitem bapperatsot)",
                     (unsigned)conteo,
                     (unsigned)HASHMAL_TSOFEH_FRAGMENT_VECINOS);
        return HASHMAL_TSOFEH_AMENAZA_FRAGMENTACION;
    }
    return 0u;
}

/* 4. JAMMING: rssi_floor LoRa >= JAMMING_RSSI_DBM (banda ruidosa). */
static uint32_t detect_jamming(void)
{
    int8_t floor = 0;
    int rc = hashmal_comm_spi_rssi_ruido(&floor);
    if (rc != HASHMAL_COMM_OK) return 0u;

    if (floor >= HASHMAL_TSOFEH_JAMMING_RSSI_DBM) {
        TSOFEH_LOG_W("detect: jamming - floor=%d dBm >= %d (banda sucia)",
                     (int)floor, HASHMAL_TSOFEH_JAMMING_RSSI_DBM);
        return HASHMAL_TSOFEH_AMENAZA_JAMMING;
    }
    return 0u;
}

/* 5. NO_AUTORIZADO: firewall ZAR delta > 0 (esh zara, Lv 10:1). */
static uint32_t detect_no_autorizado(void)
{
    if (!g_baseline.capturado) return 0u;

    const hashmal_firewall_t *fw = hashmal_firewall_get_estado();
    if (fw == NULL) return 0u;

    uint32_t ahora    = fw->estadisticas.veredictos[HASHMAL_FIREWALL_ZAR];
    uint32_t baseline = g_baseline.firewall_zar;
    if (ahora > baseline) {
        TSOFEH_LOG_W("detect: ZAR +%" PRIu32 " (Lv 10:1 esh zara)",
                     ahora - baseline);
        return HASHMAL_TSOFEH_AMENAZA_NO_AUTORIZADO;
    }
    return 0u;
}

/* 6. SHAON_RAASH: parokhet_indice clasifica RAASH. */
static uint32_t detect_shaon(void)
{
    uint8_t                   indice = 0;
    hashmal_hc_shaon_nivel_t  nivel  = HASHMAL_HC_SHAON_DEMAMAH;
    int rc = hashmal_hc_parokhet_indice(&indice, &nivel);
    if (rc != HASHMAL_HC_OK) return 0u;

    if (nivel == HASHMAL_HC_SHAON_RAASH) {
        TSOFEH_LOG_W("detect: shaon RAASH - indice=%u (1 Re 19:11 YHWH "
                     "no esta en el ra'ash)", (unsigned)indice);
        return HASHMAL_TSOFEH_AMENAZA_SHAON_RAASH;
    }
    return 0u;
}

/* ==================================================================
 * tocar_shofar_para - construye el payload y dispara hashmal_comm_
 * shofar_tocar. El payload es ASCII simple: un header ID + el bitmap
 * en hex. Formato v1; fase 2 pasara al formato del Tavnit (tipo +
 * origen + destino + payload) via menora_formato.
 * ================================================================== */
static int tocar_shofar_para(uint32_t amenazas)
{
    char   buf[64];
    /* Formato v1: "SHOFAR:<hex>" - ASCII, sin dependencias. */
    int written = snprintf(buf, sizeof(buf), "SHOFAR:%08" PRIx32, amenazas);
    if (written <= 0) return HASHMAL_COMM_ERR_FORMATO;

    size_t tam = (size_t)written;
    if (tam > HASHMAL_MENSAJE_MAX_BYTES) tam = HASHMAL_MENSAJE_MAX_BYTES;

    return hashmal_comm_shofar_tocar((const uint8_t *)buf, tam);
}

/* ==================================================================
 * API PUBLICA - hashmal_tsofeh_activar
 * ================================================================== */
hashmal_roles_err_t hashmal_tsofeh_activar(bool activar)
{
    if (activar == g_tsofeh_activo) {
        /* Idempotente. */
        return HASHMAL_ROLES_OK;
    }

    if (activar) {
        /* Captura baseline ANTES de prender el lockdown para que
         * no cuente las tx fallidas del propio cambio como "trafico
         * anomalo" en el primer pase de vigilar. */
        baseline_capturar();

        int rc = hashmal_comm_tsofeh_lockdown_set(true);
        if (rc != HASHMAL_COMM_OK) {
            TSOFEH_LOG_E("activar: Pieza 04 rechazo lockdown (rc=%d)", rc);
            baseline_liberar();
            return HASHMAL_ROLES_ERR_TSOFEH;
        }
        g_tsofeh_activo = true;
        TSOFEH_LOG_I("atalaya ACTIVO (Ez 33:2) - sobre mishmeret (Hab 2:1); "
                     "TX gated excepto shofar");
    } else {
        int rc = hashmal_comm_tsofeh_lockdown_set(false);
        if (rc != HASHMAL_COMM_OK) {
            TSOFEH_LOG_E("activar(false): Pieza 04 rechazo (rc=%d)", rc);
            return HASHMAL_ROLES_ERR_TSOFEH;
        }
        g_tsofeh_activo = false;
        baseline_liberar();
        TSOFEH_LOG_I("atalaya DESACTIVADO - descendio de la muralla");
    }
    return HASHMAL_ROLES_OK;
}

bool hashmal_tsofeh_activo(void)
{
    return g_tsofeh_activo;
}

/* ==================================================================
 * API PUBLICA - hashmal_tsofeh_vigilar_y_avisar (ATOMIC)
 *
 * INV-TSOFEH-01 (Ez 33:6): detectar Y avisar en una sola funcion.
 * Sin path que devuelva amenazas != 0 sin shofar ya emitido.
 * ================================================================== */
hashmal_roles_err_t hashmal_tsofeh_vigilar_y_avisar(uint32_t *out_amenazas)
{
    if (!g_tsofeh_activo) {
        return HASHMAL_ROLES_ERR_INIT;
    }

    /* Ejecutar las 6 detecciones en paralelo logico. Cada una OR
     * en el bitmap. El orden no importa - son independientes. */
    uint32_t amenazas = 0u;
    amenazas |= detect_nodo_caido();
    amenazas |= detect_trafico_anomalo();
    amenazas |= detect_fragmentacion();
    amenazas |= detect_jamming();
    amenazas |= detect_no_autorizado();
    amenazas |= detect_shaon();

    if (amenazas == 0u) {
        /* Silencio santo. Sin amenaza, sin shofar. Is 21:12 -
         * la noche sin novedad. */
        if (out_amenazas != NULL) *out_amenazas = 0u;
        return HASHMAL_ROLES_OK;
    }

    /* Ez 33:3 taqa' bashofar - AQUI se cumple INV-TSOFEH-01. El
     * shofar se toca ANTES de retornar; cualquier path posterior
     * asume que la espada ya fue anunciada. */
    TSOFEH_LOG_W("tsofeh ve la espada - amenazas=0x%08" PRIx32
                 " - tocando shofar (Ez 33:3)", amenazas);

    int rc_shofar = tocar_shofar_para(amenazas);

    /* Alarma local humana - independiente del shofar (aunque el
     * broadcast falle, el operator del board debe saber). */
    hashmal_tsofeh_alarma_local(amenazas);

    if (out_amenazas != NULL) *out_amenazas = amenazas;

    if (rc_shofar != HASHMAL_COMM_OK) {
        TSOFEH_LOG_E("shofar fallo (rc=%d) - Ez 33:6 INV-TSOFEH-01 "
                     "violado en TX (alarma local SI sono); "
                     "caller debe actuar", rc_shofar);
        return HASHMAL_ROLES_ERR_TSOFEH;
    }
    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 *
 *   "uten oto lahem leTSOFEH... vera'a et hajerev ba'a veTAQA'
 *    bashshofar." - Ezequiel 33:2-3
 *
 *   "vehatsofeh... velo taqa'... damo miyyado adrosh." - Ez 33:6
 *
 *   "'al MISHMARTI e'emoda ve'etyatseba 'al matsor va'atsappe."
 *     - Habacuc 2:1
 *
 *   "lej ha'amed hammetsappe asher yir'a yaggid." - Isaias 21:6
 *
 *   El atalaya ve y avisa - o su sangre cae sobre su mano. Por eso
 *   la funcion vigilar_y_avisar es ATOMICA: no hay hueco entre ver
 *   y sonar. El shofar atraviesa el lockdown porque el atalaya debe
 *   poder gritar siempre. La alarma local suena aunque el broadcast
 *   falle - el humano al lado del nodo es el testigo ultimo.
 *
 * ================================================================== */
