/**
 * ===================================================================
 * PIEZA 08 - EL ACEITE PERPETUO - aceite_energia.c
 * Sub-pieza: TETSAVVE - el mandato de establecer la energia
 * Exodo 27:20-21 + Numeros 4:9-10 + Exodo 40:9
 * ===================================================================
 *
 * "ve'atta TETSAVVE et benei Yisra'el veyiqju eleja SHEMEN ZAYIT
 *  ZAJ KATIT LAMMA'OR leha'alot NER TAMID" - Exodo 27:20
 * "Tu ORDENARAS a los hijos de Israel que te traigan ACEITE DE
 *  OLIVA PURO, PRENSADO, para la luminaria, para hacer ASCENDER
 *  lampara CONTINUAMENTE."
 *
 * "be'ohel mo'ed MIJUTS LAPPAROJET asher 'al ha'edut ya'aroj oto
 *  Aharon uvanav me'EREV 'ad BOQER lifnei YHWH JUQQAT 'OLAM
 *  ledorotam" - Exodo 27:21
 * "En la Tienda, FUERA DEL VELO... Aaron y sus hijos lo dispondran
 *  de TARDE a MANANA - estatuto PERPETUO por sus generaciones."
 *
 * "velaqju beged tejelet vejissu et menorat hamma'or ve'et
 *  neroteha ve'et malqajeha ve'et majtoteha ve'et kol kelei
 *  hashemen asher yesharetu lah bahem" - Numeros 4:9
 * "Tomaran un pano azul y cubriran la menora del alumbrado, sus
 *  lamparas, sus tenazas, sus braseros y todos los vasos del aceite."
 *
 * "velaqajta et shemen hammishja umashajta et hammishkan"
 *   - Exodo 40:9
 * "Tomaras el aceite de la uncion y ungiras el Mishkan."
 * (Paso 8 del levantamiento - aqui se posiciona la pieza 08.)
 *
 * --- VOCABULARIO ---
 *
 *   tetsavve     (TTsWH, H6680 Piel)   ORDENARAS. Mandato
 *                                           imperativo. init no es
 *                                           opcion.
 *   shemen       (ShMN, H8081)           ACEITE - la energia.
 *   zayit        (ZYT, H2132)             OLIVA. Fuente que
 *                                           resiste sequia y revive.
 *   zaj          (ZK, H2134)             PURO, sin sedimento.
 *   katit        (KTYT, H3795)          PRENSADO - maxima pureza
 *                                           bajo presion.
 *   lamma'or     (LMAWR, H3974)        PARA LA LUMINARIA. Un
 *                                           solo proposito.
 *   leha'alot    (LH'LT, H5927 Hifil)  Hacer ASCENDER. La llama
 *                                           sube.
 *   ner tamid    (NR TMYD, H5216+H8548) LAMPARA PERPETUA.
 *                                           Never down.
 *   mijuts       (MChWTs, H2351)           FUERA del velo. Capa de
 *                                           servicio, no nucleo.
 *   juqqat 'olam (H2708+H5769)             ESTATUTO PERPETUO.
 *   beged tejelet (H899+H8504)             PANO AZUL. Nm 4:9 -
 *                                           cubre la menora al migrar.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   hashmal_power_init     Ex 27:20 tetsavve - el mandato
 *   hashmal_power_deinit   Nm 4:9 - cubrir la menora al migrar
 *   hashmal_power_estado   lectura pura sin logs
 *   hashmal_power_activo   ?en tamid? sin logs
 *
 *   + helpers internos (hashmal_power_internal.h):
 *   aceite_mut              mutable para hermanos
 *   get_aceite_interno      const interno
 *   ahora_ms                timestamp ms monotonico (int64_t)
 *
 * --- init vs tamid_iniciar ---
 *
 * Ex 27:20 separa DOS verbos: "tetsavve" (ordenar recursos) y
 * "leha'alot" (hacer ascender la lampara). Analogamente:
 *   init()            establece infraestructura + estado ENCENDIDO
 *   tamid_iniciar()   eleva al tamid perpetuo (aceite_tamid.c)
 *
 * init deja el aceite en JOSHEJ (oscuridad) porque ninguna lectura
 * se ha tomado aun - solo la estructura esta lista. tamid_iniciar
 * arranca el ciclo perpetuo que leera olivos y ajustara niveles.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_power.h          API publica
 *   hashmal_power_internal.h struct + helpers compartidos
 *   esp_log.h   (ORO EGIPTO) logging
 *   esp_timer.h (ORO EGIPTO) ahora_ms
 *
 * Autor: Antonio Jose Marin Soto (Betsalel - Ex 31:2)
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <inttypes.h>

#include "esp_log.h"
#include "esp_timer.h"

#include "hashmal_power.h"
#include "hashmal_power_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *ACEITE_TAG = "hashmal.power.aceite";

#define ACEITE_LOG_I(fmt, ...)  ESP_LOGI(ACEITE_TAG, fmt, ##__VA_ARGS__)
#define ACEITE_LOG_W(fmt, ...)  ESP_LOGW(ACEITE_TAG, fmt, ##__VA_ARGS__)
#define ACEITE_LOG_E(fmt, ...)  ESP_LOGE(ACEITE_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * SINGLETON SAGRADO
 *
 * UN subsistema de energia por nodo. Ex 27:20 "SHEMEN" singular.
 * El aceite viene de fuera ("veyiqju eleja" - lo traen a ti) pero
 * se administra UNO: sin factory, sin pool.
 * ================================================================== */
static hashmal_power_estado_energia_t g_aceite;
static bool                           g_encendido = false;

/* ==================================================================
 * API INTERNA - helpers compartidos (hashmal_power_internal.h)
 * ================================================================== */

hashmal_power_estado_energia_t *hashmal_power_aceite_mut(void)
{
    if (!g_encendido) return NULL;
    return &g_aceite;
}

const hashmal_power_estado_energia_t *hashmal_power_get_aceite_interno(void)
{
    if (!g_encendido) return NULL;
    return &g_aceite;
}

int64_t hashmal_power_ahora_ms(void)
{
    /* esp_timer_get_time() -> microsegundos en int64_t.
     * Convertimos a ms manteniendo el tipo firmado. */
    return esp_timer_get_time() / 1000;
}

/* ==================================================================
 * hashmal_power_init - Exodo 27:20 tetsavve
 *
 * "tetsavve" es imperativo: ordena que el aceite fluya. Aqui
 * establecemos la estructura - el mandato se cumple. El aceite
 * real (lecturas de olivos, ciclo tamid) se eleva despues con
 * tamid_iniciar (aceite_tamid.c).
 *
 * Idempotencia: si ya encendido -> ERR_YA_INIT (contrato del header).
 * A diferencia de otras piezas que retornan OK en idempotente, aqui
 * el header declara ERR_YA_INIT - seguimos el contrato.
 *
 * 2 testigos:
 *   Ex 27:20  - tetsavve (ordena)
 *   Ex 40:9   - umashajta (unges / estableces)
 * ================================================================== */
int hashmal_power_init(void)
{
    if (g_encendido) {
        ACEITE_LOG_W("init: aceite ya encendido (idempotente)");
        return HASHMAL_POWER_ERR_YA_INIT;
    }

    /* Partimos de cero - Nm 4:9 "beged tejelet" inverso: descubrir
     * el aceite para comenzar a administrarlo. */
    memset(&g_aceite, 0, sizeof(g_aceite));

    /* "veyiqju eleja shemen" (Ex 27:20) — conectar los tubos de oro.
     * aceite_adc_init() establece el ADC GPIO4 para olivo SMOL (bateria).
     * Si falla, la lectura continua con fallback seguro (Zac 4:6 beRuji). */
    aceite_adc_init();

    const int64_t ahora = esp_timer_get_time() / 1000;

    /* -- Estado inicial ------------------------------------------ */
    g_aceite.estado               = HASHMAL_POWER_ESTADO_APAGADO;
    /* JOSHEJ (oscuridad) - aun no hay lectura de olivos. Gn 1:2:
     * "vejoshej 'al penei tehom" antes del "yehi 'or". */
    g_aceite.nivel                = HASHMAL_POWER_NIVEL_JOSHEJ;

    /* -- Zac 4:3 - dos olivos. Fase 1: ambos activos por defecto. ---
     *
     * En Fase 1 no hay driver ADC real. Los olivos se simulan con
     * `activo = true` y 50% (DAI — Zac 4:6 "lo bejayil... ki im
     * beRuji": no por potencia bruta sino por provisión del Padre).
     * El 50% simulado es suficiente (DAI): la red opera con plena
     * funcionalidad en hardware de prueba.
     *
     * Fase 2: aceite_supervivencia.c leerá ADC real y actualizará
     * porcentaje/activo en cada llamada a leer_olivo(). El driver
     * ADC reemplazará la simulación (bloque [ FASE 2 ] marcado en
     * aceite_supervivencia.c). Las flags `activo` cambiarán según
     * el voltaje real medido. */
    g_aceite.olivo_yamin.olivo      = HASHMAL_POWER_OLIVO_YAMIN;
    g_aceite.olivo_yamin.porcentaje = 50u;  /* Fase 1: shemen zayit dai */
    g_aceite.olivo_yamin.voltaje_mv = 2100u; /* ~50% LiPo */
    g_aceite.olivo_yamin.activo     = true;  /* Fase 1: yamin activo */

    g_aceite.olivo_smol.olivo       = HASHMAL_POWER_OLIVO_SMOL;
    g_aceite.olivo_smol.porcentaje  = 50u;  /* Fase 1: shemen zayit dai */
    g_aceite.olivo_smol.voltaje_mv  = 2100u; /* ~50% LiPo */
    g_aceite.olivo_smol.activo      = true;  /* Fase 1: smol activo */

    /* -- Agregado: MAX(50, 50) = 50 -> DAI (Zac 4:2 gulla) ------- */
    g_aceite.porcentaje_combinado = 50u;
    g_aceite.nivel                = HASHMAL_POWER_NIVEL_DAI;  /* 50 >= KATIT */
    g_aceite.ciclo_actual_ms      = HASHMAL_POWER_CICLO_TAMID_MS;
    g_aceite.timestamp_ms         = ahora;

    /* -- Transicion APAGADO -> ENCENDIDO -------------------------- */
    g_aceite.estado = HASHMAL_POWER_ESTADO_ENCENDIDO;
    g_encendido     = true;

    ACEITE_LOG_I("═══ ACEITE ESTABLECIDO ═══ (Éx 27:20 tetsavvé)");
    ACEITE_LOG_I("  shémen záyit zaj katít lammaʼór — aceite puro para luminaria");
    ACEITE_LOG_I("  Fase 1: yamín+smol activos 50%% — DAI (Zac 4:6 lo bejayil)");
    ACEITE_LOG_I("  nivel inicial: DAI (Gn 1:3 yehi ʼor) — Fase 2 usará ADC real");
    ACEITE_LOG_I("  estado: ENCENDIDO — aguarda 'lehaʻalót nér tamíd'");
    ACEITE_LOG_I("  juqqát ʻolám (Éx 27:21) — protocolo perpetuo listo");
    return HASHMAL_POWER_OK;
}

/* ==================================================================
 * hashmal_power_deinit - Numeros 4:9-10
 *
 * "velaqju beged tejelet vejissu et menorat hamma'or ve'et
 *  neroteha... ve'et kol kelei hashemen"
 * "Tomaran un pano azul y cubriran la menora... y todos los
 *  vasos del aceite."
 *
 * Al desmontar el Mishkan, la Menora y TODOS los utensilios del
 * aceite se cubren con tejelet (azul celestial) y pieles de tajash.
 * En firmware: reset completo; el aceite queda protegido por
 * ausencia (no hay estado volatil que pueda abusarse).
 * ================================================================== */
void hashmal_power_deinit(void)
{
    if (!g_encendido) {
        /* Idempotente silencioso - ya estaba cubierto. */
        return;
    }

    ACEITE_LOG_I("aceite cubierto (Nm 4:9 béged tejélet sobre los keléi "
                 "hashémen) — estado reseteado");

    /* Liberar los tubos de oro antes de cubrir el aceite. */
    aceite_adc_deinit();

    /* memset -> cubrir completamente. No preservamos lecturas:
     * en Fase 2, si se requiere auditoria persistente, hashmal_storage
     * guardara las metricas ANTES de llegar aqui. */
    memset(&g_aceite, 0, sizeof(g_aceite));
    g_encendido = false;
}

/* ==================================================================
 * hashmal_power_estado - lectura pura del subsistema
 *
 * Juqqat 'olam (Ex 27:21) - el protocolo siempre se puede consultar.
 * Sin logs, sin side effects. Patron consistente con los *_estado
 * de otras piezas.
 *
 *   encendido  -> OK
 *   no init    -> ERR_INIT
 * ================================================================== */
int hashmal_power_estado(void)
{
    if (!g_encendido) return HASHMAL_POWER_ERR_INIT;
    return HASHMAL_POWER_OK;
}

/* ==================================================================
 * hashmal_power_activo - ?ner tamid?
 *
 * Ex 27:20 "leha'alot ner tamid" - hacer ascender lampara
 * perpetuamente. Esta consulta distingue "establecido" de
 * "perpetuamente ascendiendo": solo cuando el ciclo tamid
 * (aceite_tamid.c) esta operando, la lampara SUBE.
 *
 *   TAMID      -> OK (subiendo)
 *   ENCENDIDO  -> ERR_INIT (estructura lista, tamid no arrancado)
 *   APAGADO    -> ERR_INIT
 *   no init    -> ERR_INIT
 *
 * Sin logs.
 * ================================================================== */
int hashmal_power_activo(void)
{
    if (!g_encendido) return HASHMAL_POWER_ERR_INIT;
    if (g_aceite.estado != HASHMAL_POWER_ESTADO_TAMID) {
        return HASHMAL_POWER_ERR_INIT;
    }
    return HASHMAL_POWER_OK;
}

/* ==================================================================
 *
 *   "shemen zayit ZAJ KATIT lamma'or leha'alot NER TAMID."
 *     - Exodo 27:20
 *
 *   "lo vejayil velo vejoaj ki im beRUJI amar YHWH tseva'ot."
 *     - Zacarias 4:6
 *   "No con fuerza ni con poder, sino con MI ESPIRITU."
 *
 *   El aceite no se impone - FLUYE. Tetsavve (orden) es el mandato
 *   del Padre; tamid (perpetuo) es la promesa de Su sustento. Este
 *   archivo solo establece el cimiento. El aceite real - el ascenso
 *   de la llama - lo levanta aceite_tamid.c.
 *
 *   === ACEITE PERPETUO - aceite_energia.c COMPLETO ===
 *
 *   Archivo 1 de 4 del Aceite Perpetuo. Siguiente: aceite_tamid.c
 *     (Ex 27:20-21 - ner tamid, me'erev 'ad boqer)
 *   Quedan:
 *     aceite_tamid.c          ciclo perpetuo + duty cycle
 *     aceite_supervivencia.c  dos olivos (Zac 4:3,11-12)
 *     aceite_mantenimiento.c  Aaron dispone (Lv 24:1-4 ya'aroj)
 *
 *   PASO 8 de Exodo 40:9-11 - en progreso.
 *
 * ================================================================== */
