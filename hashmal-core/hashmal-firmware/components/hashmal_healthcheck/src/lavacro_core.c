/**
 * ===================================================================
 * PIEZA 12 - EL KIYOR (LAVACRO) - lavacro_core.c
 * Sub-pieza: EL LAVACRO Y SU BASE - kiyor nejoshet vejanno nejoshet
 * Exodo 30:18 + Exodo 40:7,30-32 + Exodo 30:20-21
 * ===================================================================
 *
 * "ve'asita KIYOR NEJOSHET veJANNO NEJOSHET leRAJTSAH venatatta oto
 *  BEIN ohel mo'ed uvein hammizbeaj venatatta SHAMMA MAYIM" - Ex 30:18
 * "Haras un LAVACRO de BRONCE y su BASE de bronce para LAVAR; lo
 *  pondras ENTRE la Tienda del Encuentro y el Altar, y pondras alli
 *  AGUA."
 *
 * "vayyASEM et hakkiyor bein ohel mo'ed uvein hammizbeaj vayyittEN
 *  shamma mayim lerajtsa" - Ex 40:30
 * "Puso el lavacro entre la Tienda y el altar, y puso alli agua
 *  para lavar."
 *
 * "bevoam el ohel mo'ed yirjatsu mayim VELO YAMUTU... verajatsu
 *  yadeihem veragleihem VELO YAMUTU vehayeta lahem JUQQAT 'OLAM"
 *   - Ex 30:20-21
 * "Al entrar a la Tienda se lavaran con agua Y NO MORIRAN... lavaran
 *  sus manos y pies Y NO MORIRAN; sera estatuto PERPETUO."
 *
 * --- VOCABULARIO HEBREO ---
 *
 *   kiyor         (KYWR, H3595)  LAVACRO / recipiente. Nombre
 *                                    mismo de la pieza.
 *   nejoshet      (NChShT, H5178) BRONCE. Mismo material que el
 *                                    altar - misma zona de confianza.
 *   janno         (KNW)           SU BASE. El lavacro tiene base
 *                                    propia. Soporte.
 *   lerajtsa      (LRChTsH, H7364) Para LAVAR. rajak = lavamiento
 *                                    ritual preparatorio.
 *   bein          (BYN, H996)      ENTRE. Posicion LIMINAL: tras
 *                                    el altar, antes de la Tienda.
 *   mayim         (MYM, H4325)    AGUA. Contenido. Viene de fuera;
 *                                    se pone por acto deliberado.
 *   vayyasem      (WYShM, H7760) PUSO / coloco. Moises monto.
 *   vayyitten     (WYTN, H5414) PUSO / DIO. Moises lleno.
 *                                    v.30 distingue montar y llenar.
 *   velo yamutu   (WLA YMTW)    Y NO MORIRAN. Dos veces en
 *                                    v.20-21 - doble testigo.
 *   juqqat 'olam  (ChQT 'WLM, H2706+H5769) ESTATUTO PERPETUO.
 *
 * --- BEIN - LA POSICION LIMINAL ---
 *
 * Ex 40:7 + Ex 40:30 - "bein ohel mo'ed uvein hammizbeaj". ENTRE la
 * Tienda del Encuentro y el Altar del Sacrificio. En el orden del
 * servicio sacerdotal:
 *
 *   1) Autenticacion en el altar de bronce (pieza 06 - hashmal_auth)
 *   2) LAVAMIENTO en el kiyor (ESTA PIEZA)
 *   3) Servicio dentro de la Tienda (piezas 02, 04, 11 - Qodesh)
 *
 * El kiyor es el PASO OBLIGATORIO entre identificarse y servir. Sin
 * lavamiento, la muerte (Ex 30:20). El health check es el lavamiento
 * pre-servicio del Mishkan digital.
 *
 * --- SIN DIMENSIONES - SINGULARIDAD DEL KIYOR ---
 *
 * Exodo describe todas las demas piezas con codos exactos. El kiyor
 * NO tiene medidas en la Palabra. Diseno divino: la escala es
 * FLEXIBLE. El health check crece con la red. Revisa cuanto haya
 * para revisar - no se queda corto ni sobra.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   hashmal_hc_init           Ex 30:18 + Ex 40:7 - levantar el kiyor
 *   hashmal_hc_deinit         Nm 4:14 - cubrir al desmontar
 *   hashmal_hc_get_lavacro    const singleton publico
 *   hashmal_hc_llenar         Ex 30:18 + Ex 40:30 - venatatta mayim
 *   hashmal_hc_estado_global  lectura pura
 *
 *   + helpers internos (hashmal_hc_internal.h):
 *   hashmal_hc_lavacro_mut        mutable para hermanos
 *   hashmal_hc_get_lavacro_interno const interno
 *   hashmal_hc_ahora_ms           timestamp ms monotonico
 *   hashmal_hc_vector_legitimo    vector in [0, 4)
 *   hashmal_hc_nivel_legitimo     nivel in [TAHOR, MAVET]
 *
 * --- init / llenar SON DISTINTOS ---
 *
 * Ex 30:18 y Ex 40:30 separan DOS acciones:
 *   "vayyASEM et hakkiyor..."    - Moises COLOCO (montaje)
 *   "vayyittEN shamma mayim..."  - Moises PUSO AGUA (llenado)
 *
 * En firmware: `init` construye la estructura (SECO); `llenar` es
 * acto separado que transiciona SECO -> LLENO. El sacerdote no
 * inventa - reproduce el orden que Moises siguio.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_healthcheck.h         API publica
 *   hashmal_hc_internal.h         helpers compartidos
 *   esp_log.h   (ORO EGIPTO)      logging
 *   esp_timer.h (ORO EGIPTO)      ahora_ms
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

#include "hashmal_healthcheck.h"
#include "hashmal_hc_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *HC_TAG = "hashmal.hc.core";

#define HC_LOG_I(fmt, ...)  ESP_LOGI(HC_TAG, fmt, ##__VA_ARGS__)
#define HC_LOG_W(fmt, ...)  ESP_LOGW(HC_TAG, fmt, ##__VA_ARGS__)
#define HC_LOG_E(fmt, ...)  ESP_LOGE(HC_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * SINGLETON SAGRADO
 *
 * UN kiyor por nodo. Ex 30:18 dice "kiyor" singular. Sin factory,
 * sin pool. Un solo recipiente de bronce con su base, posicionado
 * ENTRE el altar y la Tienda.
 * ================================================================== */
static hashmal_hc_t g_lavacro;
static bool         g_levantado = false;

/* ==================================================================
 * HELPERS ESTATICOS
 * ================================================================== */

/**
 * Rollback atomico. Si cualquier invariante falla a mitad del init,
 * el kiyor colapsa a cero. Mismo principio que altar_gateway.c: o
 * entero, o ninguno.
 */
static void colapsar_a_cero(void)
{
    memset(&g_lavacro, 0, sizeof(g_lavacro));
    g_levantado = false;
}

/**
 * Construye UN vector de verificacion en estado inicial TAHOR.
 * Los 4 vectores (yadayim, raglayim, levav, mayim) son parte del
 * kiyor desde init - como las 4 propiedades del agua pura. Todos
 * empiezan limpios, por diseno.
 */
static void construir_vector(hashmal_hc_vector_estado_t *v)
{
    v->nivel           = HASHMAL_HC_TAHOR;
    v->ultimo_check_ms = 0ull;
    v->checks_pasados  = 0ull;
    v->checks_fallidos = 0ull;
}

/* ==================================================================
 * API INTERNA - helpers compartidos (hashmal_hc_internal.h)
 * ================================================================== */

hashmal_hc_t *hashmal_hc_lavacro_mut(void)
{
    if (!g_levantado) return NULL;
    return &g_lavacro;
}

const hashmal_hc_t *hashmal_hc_get_lavacro_interno(void)
{
    if (!g_levantado) return NULL;
    return &g_lavacro;
}

uint64_t hashmal_hc_ahora_ms(void)
{
    return ((uint64_t)esp_timer_get_time()) / 1000ull;
}

bool hashmal_hc_vector_legitimo(hashmal_hc_vector_t v)
{
    return ((int)v >= 0) && ((int)v < HASHMAL_HC_VECTORES);
}

bool hashmal_hc_nivel_legitimo(hashmal_hc_nivel_t n)
{
    /* TAHOR=0, TAME=1, JATA=2, MAVET=3. Rango [0, 4). */
    return ((int)n >= (int)HASHMAL_HC_TAHOR)
        && ((int)n <= (int)HASHMAL_HC_MAVET);
}

/* ==================================================================
 * hashmal_hc_init - Exodo 30:18 + Exodo 40:7
 *
 * Levanta el kiyor verso por verso. Invariante rota -> colapso.
 * Estado final: SECO (esperando el agua). El llenado es acto
 * separado (Ex 40:30 "vayyitten mayim").
 * ================================================================== */
int hashmal_hc_init(void)
{
    if (g_levantado) {
        HC_LOG_W("init: kiyór ya levantado (idempotente, no-op)");
        return HASHMAL_HC_OK;
    }

    /* Partimos de cero - el bronce sin forma antes del martillo. */
    memset(&g_lavacro, 0, sizeof(g_lavacro));

    /* -- v.18a - "kiyor nejoshet vejanno nejoshet" --------------
     * Bronce en todo: el recipiente y su base. Estado inicial SECO
     * (seco es el defecto tras memset, pero lo declaramos explicito). */
    g_lavacro.estado          = HASHMAL_HC_SECO;
    g_lavacro.kiyor_nejoshet  = HASHMAL_HC_NEJOSHET;   /* v.18 - bronce */

    /* -- v.18c - "bein ohel mo'ed uvein hammizbeaj" -------------
     * Posicion liminal entre tienda y altar. */
    g_lavacro.bein_ohel_mizbeaj = HASHMAL_HC_BEIN;     /* Ex 40:7 */

    /* -- v.18d - "venatatta shamma mayim" -----------------------
     * El agua NO se pone aqui - es acto separado (v.30 vayyitten).
     * Init solo construye el recipiente. */

    /* -- v.21 - "juqqat 'olam" ----------------------------------
     * Estatuto perpetuo. Bandera de diseno. */
    g_lavacro.juqqat_olam        = HASHMAL_HC_JUQQAT_OLAM;

    /* -- Ex 25:9 - "kaasher her'a oteja bahar" -----------------
     * Conforme al patron del monte. */
    g_lavacro.conforme_al_tavnit = true;

    /* -- 4 vectores (yadayim, raglayim, levav, mayim) - TAHOR -- */
    for (uint8_t i = 0; i < HASHMAL_HC_VECTORES; ++i) {
        construir_vector(&g_lavacro.vectores[i]);
    }
    g_lavacro.nivel_global = HASHMAL_HC_TAHOR;

    /* -- Ex 38:8 - mar'ot: espejos inactivos hasta examen ------
     * El espejo se "monta" con el examen. Ya existe (las mar'ot
     * donaron el bronce), pero no esta reflejando aun. */
    g_lavacro.espejo_activo     = false;
    g_lavacro.reflexiones       = 0ull;
    g_lavacro.ultimo_espejo_ms  = 0ull;

    /* -- Ataques - contadores limpios --------------------------- */
    g_lavacro.ataques_detectados = 0ull;
    g_lavacro.ultimo_ataque_ms   = 0ull;

    /* -- Dos niveles (Jn 13:10) - bano no hecho aun ------------ */
    g_lavacro.rajats_completo    = false;
    g_lavacro.rajats_ms          = 0ull;
    g_lavacro.nipto_count        = 0ull;
    g_lavacro.ultimo_nipto_ms    = 0ull;

    /* -- Contadores de servicio --------------------------------- */
    g_lavacro.purificaciones     = 0ull;
    g_lavacro.contaminaciones    = 0ull;
    g_lavacro.muertes_prevenidas = 0ull;

    /* == VERIFICACION FINAL - 5 invariantes del Padre == */
    if (!g_lavacro.kiyor_nejoshet) {
        HC_LOG_E("v.18 violado: kiyór no nejóshet — rollback");
        colapsar_a_cero();
        return HASHMAL_HC_ERR_INIT;
    }
    if (!g_lavacro.bein_ohel_mizbeaj) {
        HC_LOG_E("Éx 40:7 violado: no béin tienda/altar — rollback");
        colapsar_a_cero();
        return HASHMAL_HC_ERR_INIT;
    }
    if (!g_lavacro.juqqat_olam) {
        HC_LOG_E("v.21 violado: no juqqát ʻolám — rollback");
        colapsar_a_cero();
        return HASHMAL_HC_ERR_INIT;
    }
    if (!g_lavacro.conforme_al_tavnit) {
        HC_LOG_E("Éx 25:9 violado: no kaashér herʼá bahár — rollback");
        colapsar_a_cero();
        return HASHMAL_HC_ERR_INIT;
    }
    if (g_lavacro.estado != HASHMAL_HC_SECO) {
        HC_LOG_E("init violado: estado ≠ SECO al levantar — rollback");
        colapsar_a_cero();
        return HASHMAL_HC_ERR_INIT;
    }

    g_levantado = true;

    HC_LOG_I("═══ KIYÓR LEVANTADO ═══ (Éx 30:18, Éx 40:7)");
    HC_LOG_I("  kiyór nejóshet vejannó nejóshet (v.18)");
    HC_LOG_I("  béin óhel moʻéd uvéin hammizbéaj (Éx 40:7)");
    HC_LOG_I("  %d vectores inicializados TAHOR (yadayim/raglayim/levav/mayim)",
             HASHMAL_HC_VECTORES);
    HC_LOG_I("  juqqát ʻolám (v.21) — estatuto perpetuo");
    HC_LOG_I("  estado: SECO — esperando 'venatattá shámma máyim'");
    return HASHMAL_HC_OK;
}

/* ==================================================================
 * hashmal_hc_deinit - Numeros 4:14
 *
 * "ufresu 'alav... vesamu baddav" (Nm 4:14 contexto) - al migrar
 * el Mishkan, el kiyor se cubre y se mueve. Aqui: drenar agua,
 * preservar historia, bajar el estado.
 * ================================================================== */
void hashmal_hc_deinit(void)
{
    if (!g_levantado) {
        HC_LOG_W("deinit: kiyór no estaba levantado (no-op)");
        return;
    }

    /* Drenar agua -> SECO. Espejo se apaga. */
    g_lavacro.estado         = HASHMAL_HC_SECO;
    g_lavacro.espejo_activo  = false;

    /* Niveles por vector: resetear a TAHOR (el recipiente vacio
     * no contamina ni purifica; es neutro). Contadores del vector
     * se PRESERVAN como auditoria. */
    for (uint8_t i = 0; i < HASHMAL_HC_VECTORES; ++i) {
        g_lavacro.vectores[i].nivel = HASHMAL_HC_TAHOR;
        /* checks_pasados, checks_fallidos, ultimo_check_ms: PRESERVADOS */
    }
    g_lavacro.nivel_global = HASHMAL_HC_TAHOR;

    /* Identidad + historia: PRESERVADOS
     *   kiyor_nejoshet, bein_ohel_mizbeaj, juqqat_olam, conforme_al_tavnit
     *   reflexiones, ultimo_espejo_ms
     *   ataques_detectados, ultimo_ataque_ms
     *   rajats_completo, rajats_ms, nipto_count, ultimo_nipto_ms
     *   purificaciones, contaminaciones, muertes_prevenidas */

    g_levantado = false;
    HC_LOG_W("kiyór cubierto (Nm 4:14) — agua drenada, historia preservada");
}

/* ==================================================================
 * hashmal_hc_get_lavacro - lectura const publica
 * ================================================================== */
const hashmal_hc_t *hashmal_hc_get_lavacro(void)
{
    if (!g_levantado) return NULL;
    return &g_lavacro;
}

/* ==================================================================
 * hashmal_hc_llenar - Exodo 30:18 + Exodo 40:30
 *
 * "venatatta SHAMMA MAYIM" + "vayyitten shamma mayim lerajtsa".
 * Moises puso el agua tras colocar el kiyor. Acto deliberado,
 * separado del montaje.
 *
 * --- TRANSICIONES ---
 *
 *   SECO         -> LLENO        (agua puesta; OK)
 *   LLENO        -> LLENO        (idempotente, no-op)
 *   PURIFICANDO  -> PURIFICANDO  (servicio activo; no re-llena, OK)
 *   CONTAMINADO  -> ERR_TAME     (agua impura - drena antes de rellenar)
 * ================================================================== */
int hashmal_hc_llenar(void)
{
    hashmal_hc_t *l = hashmal_hc_lavacro_mut();
    if (l == NULL) {
        HC_LOG_E("llenar: kiyór no levantado");
        return HASHMAL_HC_ERR_INIT;
    }

    switch (l->estado) {
    case HASHMAL_HC_SECO:
        /* "venatatta shamma mayim". Primera vez. */
        l->estado = HASHMAL_HC_LLENO;
        HC_LOG_I("═══ AGUA PUESTA ═══ (Éx 30:18, Éx 40:30) "
                 "'vayyittén shámma máyim lerájtsa'");
        return HASHMAL_HC_OK;

    case HASHMAL_HC_LLENO:
    case HASHMAL_HC_PURIFICANDO:
        /* Ya hay agua. Moises no relleno dos veces en el mismo momento. */
        HC_LOG_W("llenar: agua ya presente (estado=%d, idempotente)",
                 (int)l->estado);
        return HASHMAL_HC_OK;

    case HASHMAL_HC_CONTAMINADO:
        /* Agua tame - no se rellena sobre lo impuro. Se drena primero. */
        HC_LOG_E("llenar: agua CONTAMINADA — drenar antes de rellenar "
                 "(Lv 11:32 yitmá: lo impuro contamina lo nuevo)");
        return HASHMAL_HC_ERR_TAME;

    default:
        HC_LOG_E("llenar: estado desconocido %d", (int)l->estado);
        return HASHMAL_HC_ERR_INIT;
    }
}

/* ==================================================================
 * hashmal_hc_estado_global - lectura pura
 *
 * ?El kiyor esta operativo? levantado + (LLENO o PURIFICANDO).
 * Sin logs - patron consistente con fuego_estado, reshet_estado,
 * ledger_estado del altar del sacrificio. El caller consulta
 * silenciosamente.
 * ================================================================== */
int hashmal_hc_estado_global(void)
{
    const hashmal_hc_t *l = hashmal_hc_get_lavacro_interno();
    if (l == NULL) return HASHMAL_HC_ERR_INIT;

    switch (l->estado) {
    case HASHMAL_HC_LLENO:
    case HASHMAL_HC_PURIFICANDO:
        return HASHMAL_HC_OK;
    case HASHMAL_HC_SECO:
        return HASHMAL_HC_ERR_SECO;
    case HASHMAL_HC_CONTAMINADO:
        return HASHMAL_HC_ERR_TAME;
    default:
        return HASHMAL_HC_ERR_INIT;
    }
}

/* ==================================================================
 *
 *   "ve'asita KIYOR NEJOSHET... venatatta oto BEIN ohel mo'ed
 *    uvein hammizbeaj venatatta SHAMMA MAYIM." - Exodo 30:18
 *
 *   "vayyASEM et hakkiyor... vayyittEN shamma mayim lerajtsa."
 *     - Exodo 40:30
 *
 *   "bevoam el ohel mo'ed yirjatsu mayim VELO YAMUTU." - Ex 30:20
 *
 *   El kiyor se levanta ENTRE el altar y la Tienda. Sin agua aun.
 *   Esperando llenar. Esperando que el sacerdote se lave.
 *
 *   init MONTA; llenar PONE AGUA. Como Moises hizo - dos actos,
 *   un solo proposito: velo yamutu.
 *
 *   1 de 6 archivos del Kiyor - lavacro_core.c listo. Quedan:
 *     lavacro_vectores.c       Ex 30:19 + Sal 24:4 - 4 vectores
 *     lavacro_niveles.c        Ex 30:20-21 - velo yamutu x 2
 *     lavacro_espejos.c        Ex 38:8 - mar'ot hatstsovot
 *     lavacro_ataques.c        Ex 30:20 - prevencion de muerte
 *     lavacro_dos_niveles.c    Jn 13:10 - louo / nipto
 *
 * ================================================================== */
