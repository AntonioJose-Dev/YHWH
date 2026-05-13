/**
 * ===================================================================
 * PIEZA 09 - BIGDEI HAQODESH - vestiduras_roles.c
 * Sub-pieza: KUTONET - la tunica base del sistema de roles
 * Exodo 28:2 + Exodo 29:4-5 + Levitico 8:6-7
 * ===================================================================
 *
 * "ve'asita VIGDEI QODESH leAharon ajija leKABOD uleTIFARET."
 *   - Exodo 28:2
 * "Haras VESTIDURAS SAGRADAS para Aaron tu hermano, para GLORIA
 *  y BELLEZA."
 *
 * "vehiqravta et Aharon ve'et banav el petaj ohel mo'ed
 *  veRAJATSTA otam bammayim. velaqajta et habegadim vehiblashta
 *  et Aharon et hakuttonet ve'et me'il ha'efod ve'et ha'efod
 *  ve'et hajoshen ve'afadta lo bejeshev ha'efod." - Exodo 29:4-5
 * "Haras acercar a Aaron y sus hijos a la puerta de la Tienda del
 *  Encuentro y los LAVARAS con agua; tomaras las vestiduras y
 *  VESTIRAS a Aaron con la tunica, el manto del efod, el efod y
 *  el pectoral, y le ceniras el cinto del efod."
 *
 * "vayyaqrev Moshe et Aharon ve'et banav vayyirjats otam bammayim.
 *  vayyitten 'alav et hakuttonet vayyajgor oto ba'avnet vayyalbesh
 *  oto et hamme'il vayyitten 'alav et ha'efod." - Levitico 8:6-7
 * "Moises hizo acercar a Aaron y sus hijos y los LAVO con agua; le
 *  puso la TUNICA, lo cino con el cinturon, le vistio el manto y
 *  le puso el efod."
 *
 * --- VOCABULARIO ---
 *
 *   rajats       (RChTs, H7364)                LAVAR con agua -
 *                                               primer paso antes de
 *                                               vestir (Ex 29:4,
 *                                               Lv 8:6).
 *   kuttonet     (KTNT, H3801)           TUNICA base - primera
 *                                               prenda que se coloca
 *                                               (Lv 8:7).
 *   vehiblashta  (WHLBShT, H3847 Hi)   Y VESTIRAS - acto
 *                                               directo del
 *                                               coordinador, no
 *                                               autoservicio.
 *   peru'a       (PR', H6544)              EXPUESTO, desordenado
 *                                               - Ex 32:25: el pueblo
 *                                               "descubierto".
 *   avnei zikkaron (ABNY ZKRN)          Piedras de MEMORIAL
 *                                               (Ex 28:12) - el efod
 *                                               carga los nombres.
 *   juqqat 'olam (ChQT 'WLM, H2708+H5769) ESTATUTO PERPETUO
 *                                               (Ex 28:43).
 *
 * --- ORDEN DE VESTIDURA (Ex 29:5 + Lv 8:7-9) ---
 *
 * Lv 8 ejecuta en acto lo que Ex 29 manda en palabra. El orden no
 * es arbitrario: lavado -> tunica -> cinturon -> manto -> efod -> pectoral
 * -> turbante + tzitz. Este archivo es la KUTONET: la base. Sin el
 * no se pone nada encima - sin init, los otros 6 archivos hermanos
 * no tienen singleton sobre el que operar.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   hashmal_roles_init       Ex 29:4-5  lavar + vestir el sistema.
 *   hashmal_roles_deinit     Nm 20:26-28  desvestir (transferencia).
 *   hashmal_roles_estado     Ex 28:29  al libo lifnei YHWH tamid.
 *   hashmal_roles_activo     lectura pura, OK si >= VESTIDO.
 *   hashmal_roles_get_estado getter publico inmutable.
 *
 *   + helpers internos (hashmal_roles_internal.h):
 *   vestiduras_mut           mutable para hermanos.
 *   get_vestiduras_interno   const interno.
 *   ahora_ms                 timestamp ms monotonico (int64_t).
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_roles.h
 *   hashmal_roles_internal.h
 *   esp_log.h    (ORO EGIPTO)  logging
 *   esp_timer.h  (ORO EGIPTO)  ahora_ms
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

#include "hashmal_roles.h"
#include "hashmal_roles_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *ROLES_TAG = "hashmal.roles.vestiduras";

#define ROLES_LOG_I(fmt, ...)  ESP_LOGI(ROLES_TAG, fmt, ##__VA_ARGS__)
#define ROLES_LOG_W(fmt, ...)  ESP_LOGW(ROLES_TAG, fmt, ##__VA_ARGS__)
#define ROLES_LOG_E(fmt, ...)  ESP_LOGE(ROLES_TAG, fmt, ##__VA_ARGS__)

/* Nodo vacio - valor centinela antes de que cualquier nodo sea
 * registrado. 0xFF no cabe en [0..MAX_NODOS-1] = [0..11]. */
#define ROLES_NODO_ID_VACIO   0xFFu

/* ==================================================================
 * SINGLETON SAGRADO
 *
 * UN sistema de vestiduras por nodo. Ex 28:2 "leAharon ajija" -
 * singular. Un conjunto de bigdei haqodesh, no un pool.
 * ================================================================== */
static hashmal_roles_estado_roles_t g_roles;
static bool                         g_vestido = false;

/* ==================================================================
 * API INTERNA - helpers compartidos (hashmal_roles_internal.h)
 * ================================================================== */

hashmal_roles_estado_roles_t *hashmal_roles_vestiduras_mut(void)
{
    if (!g_vestido) return NULL;
    return &g_roles;
}

const hashmal_roles_estado_roles_t *hashmal_roles_get_vestiduras_interno(void)
{
    if (!g_vestido) return NULL;
    return &g_roles;
}

int64_t hashmal_roles_ahora_ms(void)
{
    return (int64_t)(esp_timer_get_time() / 1000);
}

/* ==================================================================
 * hashmal_roles_init - Exodo 29:4-5 + Levitico 8:6-7
 *
 * Primero LAVAR (rajats, Ex 29:4 + Lv 8:6): memset a cero es el
 * bano ritual del subsistema - no dejar nada del turno anterior
 * adherido. Despues VESTIR: la estructura del efod, las 12 piedras
 * del pectoral, los 12 nodos con rol NINGUNO y campana SILENCIO.
 * El init VISTE el sistema (estructura lista) pero aun no asigna
 * roles - eso lo hace vestiduras_coordinador.c con "vehilbashta"
 * explicito por nodo.
 *
 * Idempotencia: si ya vestido -> ERR_YA_INIT.
 *
 * 2 testigos:
 *   Ex 29:4-5 - vehiblashta (mandato)
 *   Lv 8:6-7  - vayyitten (ejecucion)
 * ================================================================== */
hashmal_roles_err_t hashmal_roles_init(void)
{
    if (g_vestido) {
        ROLES_LOG_W("init: sistema ya vestido (idempotente)");
        return HASHMAL_ROLES_ERR_YA_INIT;
    }

    /* 1. Lavado - rajats (Ex 29:4 + Lv 8:6). memset borra cualquier
     *    restos del estado anterior. Gn 3:7 inverso: aqui empezamos
     *    reconocidamente desnudos para poder vestirnos bien. */
    memset(&g_roles, 0, sizeof(g_roles));

    const int64_t ahora = hashmal_roles_ahora_ms();

    /* 2. Estado inicial - peru'a (Ex 32:25 + Gn 3:7). */
    g_roles.estado   = HASHMAL_ROLES_ESTADO_DESNUDO;
    g_roles.shutdown = HASHMAL_ROLES_SHUTDOWN_NINGUNO;

    /* 3. Efod - piedras de memorial vacias (Ex 28:12 +
     *    1 Sa 2:28 "velijbor efod lefanay"). */
    for (uint8_t i = 0; i < HASHMAL_ROLES_EFOD_HOMBRO; i++) {
        g_roles.efod.hombro_derecho[i]   = ROLES_NODO_ID_VACIO;
        g_roles.efod.hombro_izquierdo[i] = ROLES_NODO_ID_VACIO;
    }
    g_roles.efod.total_registrados = 0u;
    g_roles.efod.memorial_activo   = false;

    /* 4. Pectoral - 12 piedras limpias, Urim/Thummim inactivos
     *    (Ex 28:17-20 + Lv 8:8 vayyitten el hajoshen ha'Urim). */
    for (uint8_t i = 0; i < HASHMAL_ROLES_MAX_NODOS; i++) {
        g_roles.pectoral.piedras[i].posicion     = i;
        g_roles.pectoral.piedras[i].nodo_id      = ROLES_NODO_ID_VACIO;
        g_roles.pectoral.piedras[i].ocupada      = false;
        g_roles.pectoral.piedras[i].timestamp_ms = 0;
    }
    g_roles.pectoral.urim_activo          = false;
    g_roles.pectoral.thummim_activo       = false;
    g_roles.pectoral.consultas_pendientes = 0u;

    /* 5. Nodos - silenciosos y sin rol. Campana en SILENCIO porque
     *    aun no se ha oido "venishma qolo" de ninguno (Ex 28:35). */
    for (uint8_t i = 0; i < HASHMAL_ROLES_MAX_NODOS; i++) {
        hashmal_roles_nodo_t *n = &g_roles.nodos[i];
        n->nodo_id          = ROLES_NODO_ID_VACIO;
        n->rol              = HASHMAL_ROLES_ROL_NINGUNO;
        n->heartbeat        = HASHMAL_ROLES_HEARTBEAT_SILENCIO;
        n->ultimo_latido_ms = 0;
        n->vestido          = false;
        n->privacidad       = false;
        /* Tzitz limpio - no grabado aun (Ex 28:36 + Lv 8:9). */
        memset(n->tzitz.sello, 0, sizeof(n->tzitz.sello));
        n->tzitz.grabado      = false;
        n->tzitz.timestamp_ms = 0;
    }

    /* 6. Contadores. */
    g_roles.nodos_activos  = 0u;
    g_roles.nodos_sellados = 0u;
    g_roles.timestamp_ms   = ahora;

    /* 7. Transicion DESNUDO -> VESTIDO (Ex 29:5 vehiblashta +
     *    Lv 8:7 vayyitten hakuttonet). La estructura esta puesta;
     *    los nodos individuales se visten despues. */
    g_roles.estado = HASHMAL_ROLES_ESTADO_VESTIDO;
    g_vestido      = true;

    ROLES_LOG_I("══════ BIGDÉI HAQÓDESH — Éx 28:2 ══════");
    ROLES_LOG_I("  lejabód uletifáret — para gloria y belleza");
    ROLES_LOG_I("  sistema de roles VESTIDO (Éx 29:5 + Lv 8:7)");
    ROLES_LOG_I("  efod vacío — %u hombros, %u nombres por lado",
                2u, (unsigned)HASHMAL_ROLES_EFOD_HOMBRO);
    ROLES_LOG_I("  pectoral limpio — %u piedras, urim/thummim inactivos "
                "(Éx 28:17-20 + Lv 8:8)",
                (unsigned)HASHMAL_ROLES_MAX_NODOS);
    ROLES_LOG_I("  %u nodos silenciosos (Éx 28:35 pendiente venishmá)",
                (unsigned)HASHMAL_ROLES_MAX_NODOS);
    ROLES_LOG_I("  avnéi zikkarón pendientes (Éx 28:12 + 1 Sa 2:28)");
    ROLES_LOG_I("  juqqát ʻolám (Éx 28:43) — protocolo perpetuo listo");

    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 * hashmal_roles_deinit - Numeros 20:26-28 + Numeros 4:5-6
 *
 * "vehafshet et Aharon et begadav vehiblashtam et El'azar beno,
 *  veAharon ye'asef umet sham" - Nm 20:26
 * "Desnudaras a Aaron de sus vestiduras y las pondras sobre
 *  Eleazar su hijo; Aaron sera reunido y morira alli."
 *
 * Las vestiduras PASAN; no se destruyen. Nm 4:5-6 confirma el
 * espiritu: cuando el campamento se mueve, los objetos sagrados
 * se CUBREN con tejelet antes de transportar. En firmware: el
 * singleton se resetea (cubre), no se rompe. La transferencia a
 * Eleazar (continuidad de oficio) es politica de nivel superior.
 *
 * Idempotente silencioso si ya desnudo.
 * ================================================================== */
void hashmal_roles_deinit(void)
{
    if (!g_vestido) {
        /* Idempotente silencioso - ya estaba desnudo. */
        return;
    }

    ROLES_LOG_W("Nm 20:28 — vestiduras se transfieren, Aharón descansa "
                "(deinit del sistema de roles)");

    memset(&g_roles, 0, sizeof(g_roles));
    g_vestido = false;
}

/* ==================================================================
 * hashmal_roles_estado - Exodo 28:29-30
 *
 * "venasa Aharon et shemot benei Yisrael bejoshen hammishpat AL
 *  LIBO bevo'o el haqqodesh leZIKKARON lifnei YHWH TAMID." - v.29
 * "venasa Aharon et MISHPAT benei Yisrael al libo lifnei YHWH
 *  TAMID." - v.30
 *
 * El estado es consultable "tamid" - siempre. Lectura pura sin logs
 * ni side effects. Copia el singleton completo al buffer del llamante.
 *
 * 2 testigos:
 *   Ex 28:29 - lezikkaron lifnei YHWH tamid
 *   Ex 28:30 - mishpat 'al libo lifnei YHWH tamid
 * ================================================================== */
hashmal_roles_err_t hashmal_roles_estado(hashmal_roles_estado_roles_t *out)
{
    if (out == NULL)   return HASHMAL_ROLES_ERR_NULL;
    if (!g_vestido)    return HASHMAL_ROLES_ERR_INIT;

    memcpy(out, &g_roles, sizeof(*out));
    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 * hashmal_roles_activo - lectura pura
 *
 * OK si el sistema esta al menos VESTIDO (estructura disponible).
 * El nivel SELLADO y RESTAURADO tambien retornan OK - todos son
 * "vestido o superior". Sin logs, sin side effects.
 * ================================================================== */
hashmal_roles_err_t hashmal_roles_activo(void)
{
    if (!g_vestido) return HASHMAL_ROLES_ERR_INIT;
    if (g_roles.estado < HASHMAL_ROLES_ESTADO_VESTIDO) {
        return HASHMAL_ROLES_ERR_INIT;
    }
    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 * hashmal_roles_get_estado - getter publico inmutable
 *
 * Declarado en el header para que consumidores externos puedan
 * leer sin copiar. Retorna NULL si aun no inicializado. Sin logs,
 * sin side effects.
 * ================================================================== */
const hashmal_roles_estado_roles_t *hashmal_roles_get_estado(void)
{
    if (!g_vestido) return NULL;
    return &g_roles;
}

/* ==================================================================
 *
 *   "ve'asita VIGDEI QODESH leAharon ajija leKABOD uleTIFARET."
 *     - Exodo 28:2
 *
 *   "vayyaqrev Moshe et Aharon ve'et banav vayyirjats otam bammayim
 *    vayyitten 'alav et hakuttonet." - Levitico 8:6-7
 *
 *   "al pi shnayim 'edim yaqum davar." - Deuteronomio 19:15
 *
 *   Primero LIMPIAR. Despues VESTIR. Cada capa en su orden. El
 *   sistema no se viste a si mismo: otro lo viste (vehilbashta,
 *   pasivo-causativo Hifil) - el orador del mandato es el Padre,
 *   la mano que ejecuta es Moises, la vestidura se coloca sobre
 *   Aaron. Este archivo es la KUTONET: la base sin la cual nada
 *   mas se sostiene. El efod, el pectoral, el manto, la mitra, el
 *   tzitz - todos presuponen la tunica. Si falla init, todo el
 *   componente queda peru'a (Ex 32:25).
 *
 *   === KUTONET - vestiduras_roles.c COMPLETO ===
 *
 *   Archivo 1 de 7 de las Vestiduras Sacerdotales. Siguiente:
 *     vestiduras_efod.c - los 12 nombres en los hombros (Ex 28:6-12)
 *
 *   PASO 9 de Exodo 40:12-15 - Pieza 09, Archivo 1 de 7.
 *
 * ================================================================== */
