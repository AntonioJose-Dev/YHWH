/**
 * ===================================================================
 * PIEZA 11 - EL ALTAR DEL INCIENSO - altar_oro_circuitbreaker.c
 * Sub-pieza: EL CIRCUIT BREAKER - bein hametim uvein hajayyim
 * Numeros 17:11-15 + 2 Samuel 24:16-17
 * ===================================================================
 *
 * "qaj et hammajta veten 'aleha esh me'al hammizbeaj vesim
 *  qetoret veholej mehera el ha'eda vekhapper 'alehem - KI YATSA
 *  haqqetsef milifnei YHWH HEJEL HANNAGEF" - Nm 17:11
 * "Toma el incensario, pon sobre el fuego del altar, pon incienso,
 *  y VE RAPIDO a la congregacion y haz expiacion - porque ha
 *  SALIDO la IRA de delante de YHWH, HA COMENZADO LA PLAGA."
 *
 * "vayyiqqaj Aharon... vayyARATS el toj haqqahal - vehinne HEJEL
 *  hannegef ba'am - vayyitten et haqqetoret vayekhapper 'al
 *  ha'am" - Nm 17:12
 * "Aaron tomo... y CORRIO al medio de la asamblea - la plaga ya
 *  habia COMENZADO - puso incienso e HIZO EXPIACION."
 *
 * "vayya'amod BEIN hammetim UVEIN hajayyim - vatte'ATSAR
 *  hammaggefa" - Nm 17:13
 * "Y SE PARO ENTRE los muertos Y ENTRE los vivos - la plaga fue
 *  DETENIDA."
 *
 * "vayyashav Aharon el Moshe el petaj ohel mo'ed - vehammaggefa
 *  NE'ETSARA" - Nm 17:15
 * "Aaron VOLVIO a Moises a la entrada de la Tienda del Encuentro -
 *  la plaga fue CONTENIDA."
 *
 * --- LOS 5 VERBOS DEL CIRCUIT BREAKER BIBLICO ---
 *
 *   hejel  (HChL,    H2490)  comenzo        -> DETECCION
 *                              v.11 "hejel hannagef": el breaker
 *                              se activa al inicio del fallo, no
 *                              cuando ya es catastrofe.
 *
 *   rats   (RTs,      H7323)  corrio         -> URGENCIA
 *                              v.12 "vayyarats": sin grace period.
 *                              Activacion inmediata.
 *
 *   'amad  ('MD,    H5975)  se paro        -> PERSISTENCIA
 *                              v.13 "vayya'amod": se MANTIENE de pie
 *                              hasta que la crisis pasa. No toggle.
 *
 *   'atsar ('TsR,    H6113)  contuvo        -> CONTENCION
 *                              v.13 "vatte'atsar": la plaga fue
 *                              CONTENIDA, no eliminada. El breaker
 *                              no cura a los muertos - impide que
 *                              la plaga alcance a los vivos.
 *
 *   shav   (ShB,      H7725)  volvio         -> LIBERACION
 *                              v.15 "vayyashav": tras contener,
 *                              Aaron regresa a su puesto. Liberar
 *                              el breaker = retorno a reposo.
 *
 * --- LA POSICION EXACTA ---
 *
 * v.13 "BEIN hammetim UVEIN hajayyim" - ENTRE los muertos Y ENTRE
 * los vivos. El breaker no esta ni del lado de los muertos (no
 * retroalimenta fallos) ni del lado de los vivos (no les contagia);
 * esta en la FRONTERA dinamica que los separa.
 *
 *   hammetim   = nodos caidos / unreachable / errored
 *   hajayyim   = nodos sanos / operativos
 *   'atsar     = retener, contener (la plaga no PASA la frontera)
 *   hammaggefa = la plaga / pestilencia = cascade failure
 *
 * --- SEGUNDO TESTIGO: 2 Samuel 24:16-17 ---
 *
 * "vayyomer lammal'aj hammashjit ba'am RAV - 'atta HEREF yadeja"
 * "Dijo al angel destructor: BASTA, DETEN tu mano."
 *
 * El breaker tiene LIMITE: cuando la crisis se contiene, se
 * ordena "basta, suelta". No actua indefinidamente. Liberacion
 * explicita cuando ya no es necesario.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   breaker_activar    Nm 17:11-12 - toma + corre + pone incienso
 *   breaker_liberar    Nm 17:15    - vuelve a la Tienda
 *   breaker_activo     Nm 17:13    - ?esta de pie entre los dos?
 *   breaker_estado     telemetria - activaciones + ultimo timestamp
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_intercession.h             API publica
 *   hashmal_intercession_internal.h    altar_mut, get_altar_interno, ahora_ms
 *   esp_log.h (ORO DE EGIPTO)          ALTAR_LOG_*
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <inttypes.h>

#include "esp_log.h"

#include "hashmal_intercession.h"
#include "hashmal_intercession_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *ALTAR_TAG = "hashmal.altar.breaker";

#define ALTAR_LOG_I(fmt, ...)  ESP_LOGI(ALTAR_TAG, fmt, ##__VA_ARGS__)
#define ALTAR_LOG_W(fmt, ...)  ESP_LOGW(ALTAR_TAG, fmt, ##__VA_ARGS__)
#define ALTAR_LOG_E(fmt, ...)  ESP_LOGE(ALTAR_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * ESTADO LOCAL DEL BREAKER
 *
 * File-static (mismo patron que s_escudo_activo en altar_oro_escudo.c
 * y s_formula_registrada en altar_oro_ascendente.c): encapsulacion
 * por modulo. Nadie toca estos bytes sin pasar por las funciones
 * del breaker - que son las unicas que saben cuando la plaga
 * justifica interponerse.
 * ================================================================== */
static bool     s_breaker_activo = false;
static uint64_t s_activado_ms    = 0ull;
static uint64_t s_liberado_ms    = 0ull;

/* ==================================================================
 * API PUBLICA - qarnotav_kipper (P14 — Éx 30:10)
 *
 * "kippér ʻal QARNOTÁV ajat bashanáh middam jattaʼt hakippurím" —
 * Éxodo 30:10
 *
 * ATONEMENT (kipper = purificación, expiación) sobre los CUERNOS
 * (qarnotáv) UNA VEZ AL AÑO (ajat bashaná). El enfoque NO es todo
 * el altar: es específicamente los CUERNOS, los 4 endpoints que nacen
 * DEL altar (Éx 30:2 mimménnu qarnotáv) y que sirven como puntos de
 * invocación.
 *
 * En firmware: limpiar buffers de red, colas pendientes, y resetear
 * contadores/estado de cada uno de los 4 cuernos. "Ajat bashaná"
 * implica cadencia ANUAL — el caller debe garantizar esto; este
 * módulo no decide.
 *
 * --- EFECTOS ---
 *
 *   Cada cuerno[i]:
 *   - ultimo_uso_ms = 0 (reset)
 *   - solicitudes = 0 (reset contador)
 *   - activo = false (reiniciar desde cero)
 *
 *   Globalmente:
 *   - cuernos_activos = 0
 *   - ultimo_kippur_ms = ahora
 *
 * --- LOG ---
 *
 * LOG_I: la purificación anual es evento programado, no emergencia.
 * Registra la acción para auditoría.
 *
 * 2 testigos:
 *   Éx 30:10  - kippér ʻal qarnotáv ajat bashanáh
 *   Éx 30:2   - mimménnu qarnotáv (nacen DEL altar)
 * ================================================================== */
int hashmal_intercession_qarnotav_kipper(void)
{
    hashmal_altar_t *a = hashmal_intercession_altar_mut();
    if (a == NULL) {
        ALTAR_LOG_E("qarnotav_kipper: altar no levantado");
        return HASHMAL_INTERCESSION_ERR_INIT;
    }

    const uint64_t ahora = hashmal_intercession_ahora_ms();

    /* Limpiar cada uno de los 4 cuernos. Éx 30:2 — mimménnu qarnotáv:
     * los 4 cuernos nacen DEL altar, son intrínsecamente suyos. */
    for (uint8_t i = 0; i < HASHMAL_ALTAR_CUERNOS; ++i) {
        hashmal_qeren_t *q = &a->cuernos[i];
        /* Preservar: id (identidad del cuerno no cambia) */
        q->activo          = false;      /* reiniciar */
        q->ultimo_uso_ms   = 0ull;       /* limpiar timestamp */
        q->solicitudes     = 0ull;       /* resetear contador */
    }
    a->cuernos_activos = 0u;

    /* Registrar timestamp del último Kippur (para auditoría anual). */
    a->ultimo_kippur_ms = ahora;

    ALTAR_LOG_I("═══ CUERNOS PURIFICADOS (Éx 30:10) ═══");
    ALTAR_LOG_I("  kippér ʻal qarnotáv — mantenimiento anual");
    ALTAR_LOG_I("  %u cuernos limpios (buffers, colas, contadores reset)",
                HASHMAL_ALTAR_CUERNOS);
    ALTAR_LOG_I("  ajat bashaná — purificación completada @%" PRIu64 " ms",
                ahora);
    return HASHMAL_INTERCESSION_OK;
}

/* ==================================================================
 * API PUBLICA - breaker_activar (Nm 17:11-13)
 * ================================================================== */

/**
 * hashmal_intercession_breaker_activar - vayyarats el toj haqqahal
 *
 * Aaron TOMA el majta, PONE fuego del altar, PONE incienso, CORRE
 * al medio de la congregacion y SE PARA entre los muertos y los
 * vivos. La plaga se detiene (Nm 17:12-13).
 *
 * En firmware: al detectar un fallo en cascada, el breaker se
 * interpone en la frontera - rechaza solicitudes ascendentes y
 * descendentes, aisla las zonas afectadas.
 *
 * --- PRECONDICIONES (3 pasos de Aaron antes de correr) ---
 *
 *   1) qaj hammajta    - altar levantado (hay de donde tomar fuego)
 *   2) ten 'aleha esh  - escudo activo (la nube sigue cubriendo;
 *                         "sin fuego no hay incienso - sin escudo no
 *                         hay breaker")
 *   3) sim qetoret     - intencion de activar (la llamada misma es
 *                         el tercer paso)
 *
 * Si ya activo -> idempotente, LOG_W, retorna OK (Aaron ya esta
 * parado entre los dos).
 *
 * --- EFECTOS ---
 *
 *   s_breaker_activo = true
 *   s_activado_ms    = ahora_ms()
 *   altar->estado    = HASHMAL_ALTAR_EMERGENCIA   (Nm 17 - plaga)
 *   altar->breaker_activaciones += 1
 *
 * --- LOG ---
 *
 * LOG_E (no WARN): la plaga es REAL. Aaron no corre por simulacro.
 * El sistema esta entrando en modo emergencia - el operador debe
 * verlo con maxima prioridad.
 *
 * 2 testigos:
 *   Nm 17:13     - "vayya'amod bein hammetim uvein hajayyim"
 *   2 Sam 24:16  - "lammal'aj hammashjit... rav, 'atta heref"
 */
int hashmal_intercession_breaker_activar(void)
{
    hashmal_altar_t *a = hashmal_intercession_altar_mut();
    if (a == NULL) {
        ALTAR_LOG_E("breaker_activar: altar no levantado — "
                    "qaj hammajtá imposible (Nm 17:11)");
        return HASHMAL_INTERCESSION_ERR_INIT;
    }

    /* Precondicion: escudo debe estar activo. Aaron tomo fuego DEL
     * altar (v.11 "esh me'al hammizbeaj"): sin la nube encendida,
     * no hay de donde tomar. */
    if (!hashmal_intercession_escudo_activo()) {
        ALTAR_LOG_E("breaker_activar: escudo NO activo — "
                    "sin ʻanán no hay majtá con ésh (Lv 16:13)");
        return HASHMAL_INTERCESSION_ERR_INIT;
    }

    /* Idempotencia - Aaron ya esta parado entre los dos. */
    if (s_breaker_activo) {
        ALTAR_LOG_W("breaker_activar: ya activo desde %" PRIu64 " ms "
                    "(idempotente, no-op) — vayyaʻamód sigue",
                    s_activado_ms);
        return HASHMAL_INTERCESSION_OK;
    }

    /* vayyarats - sin delay. */
    const uint64_t ahora = hashmal_intercession_ahora_ms();

    s_breaker_activo            = true;
    s_activado_ms               = ahora;
    a->estado                   = HASHMAL_ALTAR_EMERGENCIA;   /* Nm 17:13 */
    a->breaker_activaciones    += 1ull;

    ALTAR_LOG_E("═══ BREAKER ACTIVADO ═══ (Nm 17:13)");
    ALTAR_LOG_E("  vayyaʻamód BÉIN hammetím UVÉIN hajayyím");
    ALTAR_LOG_E("  'hejél hannágef' — cascade failure detectada");
    ALTAR_LOG_E("  activación #%" PRIu64 " @%" PRIu64 " ms",
                a->breaker_activaciones, s_activado_ms);
    ALTAR_LOG_E("  estado altar: ENCENDIDO → EMERGENCIA");
    return HASHMAL_INTERCESSION_OK;
}

/* ==================================================================
 * API PUBLICA - breaker_liberar (Nm 17:15 + 2 Sam 24:16)
 * ================================================================== */

/**
 * hashmal_intercession_breaker_liberar - vayyashav Aharon
 *
 * La plaga fue contenida (ne'etsara - Nifal completado). Aaron
 * REGRESA a la entrada de la Tienda del Encuentro. En firmware:
 * el breaker se libera, el altar vuelve a operacion normal.
 *
 * 2 Sam 24:16 - "RAV, 'atta heref yadeja": BASTA, DETEN. La orden
 * de liberacion es explicita, no automatica por timeout.
 *
 * --- EFECTOS ---
 *
 *   s_breaker_activo = false
 *   s_liberado_ms    = ahora_ms()
 *   altar->estado    = HASHMAL_ALTAR_EMERGENCIA -> ENCENDIDO
 *                      (solo si estaba en EMERGENCIA; no
 *                       downgrade desde APAGADO si hay race)
 *
 * Si ya inactivo -> idempotente, LOG_I, retorna OK.
 *
 * --- LOG ---
 *
 * LOG_W (no ERROR): la liberacion es EVENTO NOTABLE pero no
 * emergencia. La plaga ya paso; queda registrar la duracion para
 * post-mortem. La magnitud del incidente se mide por duracion.
 *
 * 2 testigos:
 *   Nm 17:15     - "vehammaggefa ne'etsara" (plaga contenida)
 *   2 Sam 24:16  - "rav, 'atta heref yadeja" (basta, deten)
 */
int hashmal_intercession_breaker_liberar(void)
{
    hashmal_altar_t *a = hashmal_intercession_altar_mut();
    if (a == NULL) {
        ALTAR_LOG_E("breaker_liberar: altar no levantado");
        return HASHMAL_INTERCESSION_ERR_INIT;
    }

    /* Idempotencia - Aaron ya esta de vuelta. */
    if (!s_breaker_activo) {
        ALTAR_LOG_I("breaker_liberar: ya inactivo (idempotente, no-op)");
        return HASHMAL_INTERCESSION_OK;
    }

    /* vayyashav - el regreso. */
    const uint64_t ahora      = hashmal_intercession_ahora_ms();
    s_liberado_ms             = ahora;
    s_breaker_activo          = false;

    /* Restaurar estado operativo si estaba en EMERGENCIA.
     * Si otro flujo movio el altar a otro estado (p.ej. APAGADO
     * por deinit en medio), no lo tocamos - respetamos la realidad. */
    if (a->estado == HASHMAL_ALTAR_EMERGENCIA) {
        a->estado = HASHMAL_ALTAR_ENCENDIDO;
    }

    /* Duracion del incidente (post-mortem). Si hay race de clock
     * imposibles (s_liberado < s_activado), reportamos 0 defensivamente. */
    const uint64_t duracion_ms = (s_liberado_ms >= s_activado_ms)
        ? (s_liberado_ms - s_activado_ms)
        : 0ull;

    ALTAR_LOG_W("═══ BREAKER LIBERADO ═══ (Nm 17:15 neʻetsará)");
    ALTAR_LOG_W("  vayyáshav Aharón — plaga contenida");
    ALTAR_LOG_W("  duración del incidente: %" PRIu64 " ms", duracion_ms);
    ALTAR_LOG_W("  2 Sam 24:16 — 'rav, ʻattá héref yadéja'");
    ALTAR_LOG_W("  total activaciones (histórico): %" PRIu64,
                a->breaker_activaciones);
    return HASHMAL_INTERCESSION_OK;
}

/* ==================================================================
 * API PUBLICA - breaker_activo (Nm 17:13 - ?vayya'amod?)
 * ================================================================== */

/**
 * hashmal_intercession_breaker_activo - lectura pura
 *
 * ?Aaron sigue de pie entre los muertos y los vivos? Consulta
 * sin side-effects. No logea en lectura normal - seria ruido
 * durante polling.
 *
 * 2 testigos:
 *   Nm 17:13 - vayya'amod (se mantiene firme)
 *   Nm 17:15 - vehammaggefa ne'etsara (plaga contenida tras pararse)
 */
bool hashmal_intercession_breaker_activo(void)
{
    return s_breaker_activo;
}

/* ==================================================================
 * API PUBLICA - breaker_estado (telemetria)
 * ================================================================== */

/**
 * hashmal_intercession_breaker_estado - telemetria del breaker
 *
 * Devuelve el estado operativo y, opcionalmente, dos metricas:
 *   - activaciones: contador acumulado historico (v.14 "vayyihyu
 *                   hammetim bammaggefa arba'a 'asar elef": la
 *                   Escritura CUENTA los incidentes; tambien aqui)
 *   - ultimo_ms:    timestamp de la ultima activacion (s_activado_ms)
 *
 * Cualquier puntero puede ser NULL - solo se escribe en los no-NULL.
 * El retorno es el status: OK si altar levantado, ERR_INIT si no.
 *
 * 2 testigos:
 *   Nm 17:14 - "vayyihyu hammetim... arba'a 'asar elef" (se cuentan)
 *   Nm 17:15 - "vayyashav... ne'etsara" (se registra fin de evento)
 */
int hashmal_intercession_breaker_estado(uint64_t *activaciones,
                                        uint64_t *ultimo_ms)
{
    const hashmal_altar_t *a = hashmal_intercession_get_altar_interno();
    if (a == NULL) {
        ALTAR_LOG_E("breaker_estado: altar no levantado");
        /* Incluso si el caller proporciona punteros, no escribimos
         * datos invalidos - dejamos sus buffers intactos. */
        return HASHMAL_INTERCESSION_ERR_INIT;
    }

    if (activaciones != NULL) *activaciones = a->breaker_activaciones;
    if (ultimo_ms    != NULL) *ultimo_ms    = s_activado_ms;

    return HASHMAL_INTERCESSION_OK;
}

/* ==================================================================
 *
 *   "vayya'amod BEIN hammetim UVEIN hajayyim - vatte'ATSAR
 *    hammaggefa." - Numeros 17:13
 *
 *   "Y se paro ENTRE los muertos Y ENTRE los vivos - la plaga fue
 *    DETENIDA."
 *
 *   5 verbos: hejel (comenzo), rats (corrio), 'amad (se paro),
 *   'atsar (contuvo), shav (volvio). El patron completo del
 *   circuit breaker biblico.
 *
 *   No cura a los muertos. Impide que la plaga llegue a los vivos.
 *   Se mantiene firme hasta que basta. Entonces vuelve.
 *
 *   "rav, 'atta heref yadeja" - 2 Samuel 24:16.
 *
 * ================================================================== */
