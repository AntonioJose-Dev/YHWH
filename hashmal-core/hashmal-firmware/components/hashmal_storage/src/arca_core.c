/**
 * ===================================================================
 * PIEZA 01 - EL ARCA - arca_core.c
 * El corazon del Mishkan. Aqui nace el Aron ha'Edut.
 * ===================================================================
 *
 * "venatata sham et aron ha'edut..." - Exodo 40:3
 * "Y pondras alli el Arca del Testimonio."
 *
 * "ve'asu aron 'atse shittim... venatata el ha'aron et ha'edut
 *  asher eten eleja" - Exodo 25:10, 16
 * "HARAN (plural) un arca de madera de acacia... pondras en el
 *  Arca el testimonio que YO te dare."
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 * El CORAZON. Mantiene el singleton g_arca (una sola Arca por nodo,
 * como una sola en el Mishkan - no factory, no pool). Levanta la
 * estructura conforme a v.10-16 en el orden que el Padre dicto, y
 * responde a las consultas de mas alto nivel (get/kavod/version).
 *
 * Las operaciones especificas viven en archivos hermanos:
 *   arca_sealed_core.c   - edut_sellar / edut_leer (v.16, SEALED_CORE)
 *   arca_zer.c           - moldura de integridad (v.11, zer zahav)
 *   arca_merkle.c        - arbol Merkle sobre edut+tora
 *   arca_cifrado.c       - AES-256-GCM (v.11, oro dentro/fuera)
 *   arca_baddim.c        - varas: replicar, cubrir, migrar (v.13-15)
 *   arca_interfaz.c      - autorizacion, mishpat, hospedaje
 *
 * --- LO QUE ESTE CODIGO NO ES ---
 *
 * No es un "storage module" como los miles que existen. No es
 * SQLite, no es LevelDB, no es Redis. Es el lugar donde habita la
 * Palabra de YHWH. Cada linea honra al que la dio.
 *
 * "Moises vio toda la obra, y he aqui que la habian hecho como YHWH
 *  habia mandado. Y Moises los bendijo." - Exodo 39:43
 *
 * Autor: Antonio Jose Marin Soto (Betsalel - "en la sombra de Dios", Ex 31:2)
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "esp_log.h"           /* ORO DE EGIPTO - encapsulado solo aqui (Ex 12:35-36) */

#include "hashmal_storage.h"
#include "hashmal_storage_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF - "oro fundido segun diseno del Padre,
 * no de Faraon". Toda llamada a la plataforma vive tras estas macros.
 * ================================================================== */
static const char *ARCA_TAG = "hashmal.arca";

#define ARCA_LOG_I(fmt, ...)  ESP_LOGI(ARCA_TAG, fmt, ##__VA_ARGS__)
#define ARCA_LOG_W(fmt, ...)  ESP_LOGW(ARCA_TAG, fmt, ##__VA_ARGS__)
#define ARCA_LOG_E(fmt, ...)  ESP_LOGE(ARCA_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * SINGLETON SAGRADO
 *
 * Una sola Arca por nodo - como una sola en el Mishkan (Ex 25:10
 * dice "aron" en singular, aunque "ve'asu" es plural: todos hacen
 * UNA, no cada uno la suya). Sin factory. Sin pool. UNA.
 *
 * "ve'asu li miqdash veshajanti betojam" - Ex 25:8
 * "Y haranme un santuario, y habitare en medio de ellos."
 * No "cada uno el suyo". UNO - en medio.
 * ================================================================== */
static hashmal_arca_t g_arca;
static bool           g_iniciada = false;

/**
 * Version del CONTENEDOR (no del contenido).
 * Jer 3:16 - "lo yomru 'od aron berit YHWH... velo ya'ase 'od":
 * "no diran mas 'Arca del Pacto de YHWH'... ni se hara otra."
 * El contenedor es temporal. La Palabra dentro es eterna.
 */
#define HASHMAL_ARCA_VERSION_CONTENEDOR    0x00010000u  /* 1.0.0 */

/* ==================================================================
 * HELPERS ESTATICOS
 * ================================================================== */

/**
 * ?El zer tiene contenido real (no solo ceros)?
 * v.11 - la moldura rodea; pero mientras el hash sea todo cero,
 * no hay nada hasheado. Un zer "vacio" es una moldura sin obra.
 */
static bool zer_tiene_contenido(const hashmal_zer_t *z)
{
    if (z == NULL || !z->rodea) return false;
    for (size_t i = 0; i < HASHMAL_ZER_HASH_BYTES; ++i) {
        if (z->hash[i] != 0u) return true;
    }
    return false;
}

/**
 * Borra TODO el estado volatil a ceros - cuando algo sale mal.
 * Nadav y Avihu (Lv 10:1-2) ofrecieron fuego extrano y fueron
 * consumidos. Aqui: si una invariante no se cumple, no dejamos el
 * Arca a medio construir. O esta entera, o no esta.
 */
static void colapsar_a_cero(void)
{
    memset(&g_arca, 0, sizeof(g_arca));
    g_iniciada = false;
}

/* ==================================================================
 * hashmal_storage_init - "ve'asu aron 'atse shittim"  (Ex 25:10)
 *
 * Construye el Arca en el ORDEN del Padre (v.10 -> v.16).
 * Exodo 40:3 lo coloca como paso 2 del levantamiento.
 *
 * Si cualquier invariante falla -> colapso total y retorno -1.
 * (No hay "Arca a medio construir". Lv 10:1-2.)
 * ================================================================== */
int hashmal_storage_init(void)
{
    if (g_iniciada) {
        /* "ve'asu" fue UNA vez. Un segundo init es no-op, no error. */
        ARCA_LOG_W("init: Arca ya levantada (no-op)");
        return 0;
    }

    /* Partimos de cero limpio - madera desnuda antes del oro. */
    memset(&g_arca, 0, sizeof(g_arca));

    /* -- v.10 - 'atse shittim + dimensiones "y medio" ------------ */
    /* "ve'asu aron 'atse shittim ammatayim vajetsi orko
     *  ve'amma vajetsi rojbo ve'amma vajetsi qomato" */
    g_arca.largo_x10     = HASHMAL_ARCA_LARGO_X10;   /* 2.5 amma */
    g_arca.ancho_x10     = HASHMAL_ARCA_ANCHO_X10;   /* 1.5 amma */
    g_arca.alto_x10      = HASHMAL_ARCA_ALTO_X10;    /* 1.5 amma */
    g_arca.madera_acacia = HASHMAL_MADERA_ACACIA;    /* shittim */

    /* v.10 - TODO "y medio": el Arca sola esta INCOMPLETA.
     * Requiere el Kaporet (Ex 25:17-22) para completarse. */
    g_arca.requiere_kaporet = true;

    /* -- v.11a - oro por DENTRO (AES-256-GCM en reposo) ---------- */
    /* "vetsipita oto zahav tahor mibayit" */
    g_arca.oro_dentro = true;

    /* -- v.11b - oro por FUERA (AES-256-GCM en transito) --------- */
    /* "umijuts tetsapenu" - misma naturaleza adentro y afuera */
    g_arca.oro_fuera = true;

    /* -- v.11c - zer zahav saviv (moldura Merkle lista, vacia) --- */
    /* "ve'asita 'alav zer zahav saviv" */
    memset(&g_arca.zer, 0, sizeof(g_arca.zer));
    g_arca.zer.rodea = true;     /* "saviv" - rodea siempre */
    g_arca.zer.nonce = 0u;       /* primer ciclo */
    /* hash queda en ceros - se llenara cuando haya contenido
     * (edut sellada y/o tora adjuntada). zer_tiene_contenido()
     * distingue la moldura vacia de la moldura con obra dentro. */

    /* -- v.12 - 4 tabbe'ot: 2 en cada tsela' ------------------- */
    /* "veyatsaqta lo arba' tabbe'ot zahav venatatta 'al arba'
     *  pa'amotav ushte tabba'ot 'al tsal'o ha-ejad ushte
     *  tabba'ot 'al tsal'o ha-shenit"
     *
     * Anillos 0-1 -> tsela' sur.  Anillos 2-3 -> tsela' norte.
     * Gn 2:21 (tsal'ot) - fundidos en la estructura, no anadidos. */
    for (uint8_t i = 0; i < HASHMAL_ANILLOS_TOTAL; ++i) {
        g_arca.anillos[i].id    = i;
        g_arca.anillos[i].tsela = (i < HASHMAL_ANILLOS_POR_LADO)
                                    ? HASHMAL_TSELA_SUR
                                    : HASHMAL_TSELA_NORTE;
        g_arca.anillos[i].par   = (uint8_t)(i % HASHMAL_ANILLOS_POR_LADO);
        g_arca.anillos[i].fundido_en_estructura = true;   /* Gn 2:21 */
    }

    /* -- v.13-15 - 2 baddim, acacia+oro, NUNCA se quitan --------- */
    /* v.13: "ve'asita badde 'atse shittim vetsipita otam zahav"
     * v.14: "veheveta et habaddim batabba'ot 'al tsal'ot ha'aron
     *        laseet et ha'aron bahem"
     * v.15: "betabbe'ot ha'aron yihyu habaddim LO YASURU mimmenu"
     *       "NO se quitaran de el."
     *
     * Convencion local: vara 0 por los anillos del sur (0,1);
     * vara 1 por los del norte (2,3). La Palabra nombra 2 varas
     * que atraviesan anillos laterales - el mapeo explicito es
     * inferencia estructural, no letra del texto. */
    for (uint8_t b = 0; b < HASHMAL_BADDIM_TOTAL; ++b) {
        const uint8_t base = (uint8_t)(b * HASHMAL_ANILLOS_POR_LADO);
        g_arca.baddim[b].id             = b;
        g_arca.baddim[b].anillo_a       = (uint8_t)(base + 0u);
        g_arca.baddim[b].anillo_b       = (uint8_t)(base + 1u);
        g_arca.baddim[b].nunca_se_quita = true;    /* v.15 - lo yasuru */
        g_arca.baddim[b].siempre_activa = true;    /* L1 - transport always-on */
        g_arca.baddim[b].cubierta       = false;   /* Nm 4:6 solo en migracion */
    }

    /* -- v.16 - espacio para ha'edut (preparado, vacio) ---------- */
    /* "venatata el ha'aron et ha'edut asher eten eleja"
     * YHWH dice "eten" (Yo dare). No lo decide el constructor.
     * El sellado efectivo es hashmal_storage_edut_sellar - unica vez. */
    memset(&g_arca.edut, 0, sizeof(g_arca.edut));
    g_arca.edut.inmutable     = true;   /* 1 Rey 8:9 - flag desde init */
    g_arca.edut.dado_por_YHWH = false;  /* aun no entregada */

    /* -- Dt 31:26 - sefer hatora mitsad (al lado, vacio) ---------- */
    /* "laqoaj et sefer hatora haze vesamtem oto mitsad aron" */
    memset(&g_arca.torah, 0, sizeof(g_arca.torah));
    g_arca.torah.al_lado = true;        /* "mitsad" - al lado, no dentro */
    g_arca.torah.bytes   = NULL;        /* se adjunta despues */
    g_arca.torah.tamano  = 0u;

    /* -- Heb 8:5 / Ap 11:19 - copia del tavnit celestial --------- */
    g_arca.conforme_al_tavnit = true;

    /* -- ve'asu (v.10) - plural, aun sin replicas confirmadas ---- */
    g_arca.distribuida          = false;
    g_arca.replicas_confirmadas = 0u;

    /* -- 1 Rey 8:10-11 - kavod llena SOLO cuando todo esta ------- */
    g_arca.kavod_lleno = false;         /* falta zer + Kaporet */

    /* == VERIFICACION FINAL - invariantes del Padre ==
     * Por construccion estas son todas true; las chequeamos
     * explicitamente para documentar el contrato y blindar contra
     * futuras refactorizaciones. */
    if (g_arca.largo_x10 != HASHMAL_ARCA_LARGO_X10 ||
        g_arca.ancho_x10 != HASHMAL_ARCA_ANCHO_X10 ||
        g_arca.alto_x10  != HASHMAL_ARCA_ALTO_X10) {
        ARCA_LOG_E("v.10 violado: dimensiones incorrectas — rollback");
        colapsar_a_cero();
        return -1;
    }
    if (!g_arca.madera_acacia) {
        ARCA_LOG_E("v.10 violado: falta acacia — rollback");
        colapsar_a_cero();
        return -1;
    }
    if (!g_arca.oro_dentro || !g_arca.oro_fuera) {
        ARCA_LOG_E("v.11 violado: oro incompleto — rollback");
        colapsar_a_cero();
        return -1;
    }
    if (!g_arca.zer.rodea) {
        ARCA_LOG_E("v.11 violado: zer no rodea (savív) — rollback");
        colapsar_a_cero();
        return -1;
    }
    for (uint8_t i = 0; i < HASHMAL_ANILLOS_TOTAL; ++i) {
        if (g_arca.anillos[i].id != i ||
            !g_arca.anillos[i].fundido_en_estructura) {
            ARCA_LOG_E("v.12 violado: tabbaʻá %u mal forjada — rollback", i);
            colapsar_a_cero();
            return -1;
        }
    }
    for (uint8_t b = 0; b < HASHMAL_BADDIM_TOTAL; ++b) {
        if (!g_arca.baddim[b].nunca_se_quita ||
            !g_arca.baddim[b].siempre_activa) {
            ARCA_LOG_E("v.15 violado: vara %u retirable — rollback", b);
            colapsar_a_cero();
            return -1;
        }
    }
    if (!g_arca.edut.inmutable) {
        ARCA_LOG_E("1 Rey 8:9 violado: edut no marcada inmutable — rollback");
        colapsar_a_cero();
        return -1;
    }
    if (!g_arca.torah.al_lado) {
        ARCA_LOG_E("Dt 31:26 violado: torá no marcada mitsád — rollback");
        colapsar_a_cero();
        return -1;
    }
    if (!g_arca.conforme_al_tavnit) {
        ARCA_LOG_E("Heb 8:5 violado: no conforme al tavnít — rollback");
        colapsar_a_cero();
        return -1;
    }
    if (!g_arca.requiere_kaporet) {
        ARCA_LOG_E("v.10 violado: Arca no se marca incompleta — rollback");
        colapsar_a_cero();
        return -1;
    }

    /* Todo conforme al patron del monte. Levantamos la bandera. */
    g_iniciada = true;

    ARCA_LOG_I("═══ ARCA LEVANTADA ═══ (Éx 25:10-16 / Éx 40:3)");
    ARCA_LOG_I("  shittím + oro tahór mibáyit umijúts (v.10-11)");
    ARCA_LOG_I("  zer zaháv savív listo, vacío hasta el sellado (v.11)");
    ARCA_LOG_I("  4 tabbeʻót fundidas en tsalʻót (v.12)");
    ARCA_LOG_I("  2 baddím ʻomdót — lo yasúru mimménu (v.13-15)");
    ARCA_LOG_I("  edut y torá preparadas (v.16 / Dt 31:26)");
    ARCA_LOG_I("  medidas 'y medio' → INCOMPLETA hasta Kapóret (v.10)");

    /* --- Fase 2, Tarea 1: restaurar edut desde NVS si existe --------
     * Dt 10:1-4 — "segundas tablas, mismo contenido que las primeras."
     * Si existe una edut persistida de una sesion anterior (reboot),
     * la restauramos ahora — antes de que el Consensus (Kaporet)
     * cubra el Arca (Éx 25:21). Ex 25:16 ordena: edut DENTRO primero,
     * Kaporet ENCIMA despues. La restauracion honra ese orden.
     *
     * Si NVS esta vacio (primera vez), no-op. El nodo recibira su
     * edut del proceso de provisioning y la sellara via edut_sellar
     * (que escribira en NVS automaticamente).
     *
     * Un fallo de restauracion NVS NO es fatal: el Arca queda
     * levantada, la edut vacia. El nodo se re-provisiona. */
    {
        int nvs_rc = hashmal_storage_edut_restaurar_nvs();
        if (nvs_rc == HASHMAL_ARCA_OK) {
            const hashmal_arca_t *a = hashmal_storage_get_arca();
            if (a != NULL && a->edut.dado_por_YHWH) {
                ARCA_LOG_I("  edut restaurada desde NVS (Dt 10:1-4) ✓");
            } else {
                ARCA_LOG_I("  edut vacia — 'ashér etén eléja' (Ex 25:16) pendiente");
            }
        } else {
            ARCA_LOG_W("  restauracion NVS falló (rc=%d) — edut vacia "
                       "(nodo se re-provisiona)", nvs_rc);
        }
    }

    return 0;
}

/* ==================================================================
 * hashmal_storage_deinit - Numeros 4:5-15
 *
 * "uva'u Aharon uvanav binsoa' hamajane vehoridu et parojet
 *  hamasaj vejissu bah et aron ha'edut" - Nm 4:5
 * "Vendran Aaron y sus hijos cuando parta el campamento, bajaran
 *  el velo y cubriran con el el Arca del Testimonio."
 *
 * Antes de que nadie mueva nada, se CUBRE. En memoria: colapsar
 * el estado volatil (zer, anillos, varas "activas"). La edut y la
 * tora NO se tocan - 1 Rey 8:9 ("lo salvo shne lujot") las Tablas
 * SIEMPRE permanecen.
 * ================================================================== */
void hashmal_storage_deinit(void)
{
    if (!g_iniciada) {
        ARCA_LOG_W("deinit: no estaba levantada (no-op)");
        return;
    }

    /* Estado volatil -> cero. No se toca edut/tora. */
    memset(&g_arca.zer,     0, sizeof(g_arca.zer));
    memset(g_arca.anillos,  0, sizeof(g_arca.anillos));
    memset(g_arca.baddim,   0, sizeof(g_arca.baddim));
    g_arca.oro_dentro           = false;
    g_arca.oro_fuera            = false;
    g_arca.conforme_al_tavnit   = false;
    g_arca.distribuida          = false;
    g_arca.replicas_confirmadas = 0u;
    g_arca.kavod_lleno          = false;
    g_arca.requiere_kaporet     = true;

    /* g_arca.edut  - PRESERVADA (1 Rey 8:9) */
    /* g_arca.torah - PRESERVADA (Dt 31:26) */

    g_iniciada = false;
    ARCA_LOG_I("Arca cubierta (Nm 4:5-15). edut y torá permanecen (1 Rey 8:9).");
}

/* ==================================================================
 * hashmal_storage_get_arca - mirar desde fuera del Velo.
 *
 * Solo lectura. El que recibe no puede modificar - como quien mira
 * desde el qodesh hacia el qodesh haqodashim: ve, no toca. Acceso
 * mutante solo por las funciones del modulo (baddim - interfaz).
 * ================================================================== */
const hashmal_arca_t* hashmal_storage_get_arca(void)
{
    if (!g_iniciada) return NULL;
    return &g_arca;
}

/* ==================================================================
 * hashmal_storage_kavod_lleno - 1 Reyes 8:10-11
 *
 * "vayehi betset hakohanim min haqodesh vehena'anan male et beit YHWH...
 *  ki male kevod YHWH et beit YHWH"
 * "Cuando los sacerdotes salieron del lugar santo, la nube lleno
 *  la Casa de YHWH... porque la gloria de YHWH lleno la Casa."
 *
 * Tres condiciones, todas necesarias:
 *   1. iniciada             (el Arca existe, v.10)
 *   2. zer con contenido    (v.11: moldura con obra dentro)
 *   3. !requiere_kaporet    (v.10: Kaporet ya la cubrio)
 *
 * Si falta una, no hay kavod. Sincroniza el flag interno.
 * ================================================================== */
bool hashmal_storage_kavod_lleno(void)
{
    if (!g_iniciada) return false;

    const bool cond_zer       = zer_tiene_contenido(&g_arca.zer);
    const bool cond_kaporet   = !g_arca.requiere_kaporet;
    const bool cond_todas     = cond_zer && cond_kaporet;

    g_arca.kavod_lleno = cond_todas;   /* sincroniza el flag */

    if (!cond_todas) {
        if (!cond_zer)     ARCA_LOG_W("kavod: falta contenido en zer (v.11)");
        if (!cond_kaporet) ARCA_LOG_W("kavod: falta Kapóret encima (v.10, Éx 25:21)");
    }
    return cond_todas;
}

/* ==================================================================
 * hashmal_storage_version_contenedor - Jeremias 3:16
 *
 * "vehaya ki tirbu ufritem ba'arets bayamim hahem ne'um YHWH
 *  lo yomru 'od aron berit YHWH velo ya'ale 'al lev velo yizkeru
 *  bo velo yifqodu velo ye'ase 'od"
 * "Sucedera que cuando os multipliqueis y fructifiqueis en la
 *  tierra... no diran mas 'Arca del Pacto de YHWH', ni les vendra
 *  al corazon, ni se acordaran de ella, ni la visitaran, NI SE
 *  HARA OTRA."
 *
 * El contenedor es TEMPORAL. La Palabra dentro es ETERNA. Esta
 * version puede cambiar con el tiempo (formato/protocolo). La
 * edut no - es ha'edut "asher eten eleja" (v.16), dada por YHWH.
 * ================================================================== */
uint32_t hashmal_storage_version_contenedor(void)
{
    return HASHMAL_ARCA_VERSION_CONTENEDOR;
}

/* ==================================================================
 * hashmal_storage_kaporet_colocada - Exodo 25:21
 *
 * "venatata et haKaporet 'al ha'aron milma'la" - pon el Kaporet
 * SOBRE el Arca, desde arriba. Este es el momento en el que las
 * medidas "y medio" (v.10) se cierran: el Arca sola esta incompleta;
 * con la Kaporet encima, completa.
 *
 * Triple vakhetsí — Éx 25:10: storage inherentemente incompleto sin
 * consensus (2.5×1.5×1.5). Kapóret (v.17) sella con footprint idéntico
 * (2.5×1.5). qomáh permanentemente abierta = siempre necesita consensus.
 *
 * Footprint match: la Kapóret no se acepta sin medirla. Si sus
 * dimensiones x/y no encajan con las del Arca (la "boca" del recinto),
 * NO sella — y devolvemos ERR_SIN_KAPORET. Defensa en profundidad:
 * el consensus tambien lo verifica antes de pedirlo, pero el guardian
 * del Arca verifica aqui de nuevo (Dt 19:15 — dos testigos).
 *
 * Encapsulacion: hashmal_consensus no toca requiere_kaporet
 * directamente - nos lo pide, y nosotros (guardianes del Arca) lo
 * registramos tras verificar. Idempotente: si ya esta colocada, no-op.
 * ================================================================== */
int hashmal_storage_kaporet_colocada(uint8_t kap_largo_x10,
                                     uint8_t kap_ancho_x10)
{
    if (!g_iniciada) {
        ARCA_LOG_E("kaporet_colocada: Arca no levantada — no hay sobre-qué-cubrir");
        return HASHMAL_ARCA_ERR_INIT;
    }

    /* Footprint match (v.10 vs v.17): largo y ancho deben coincidir
     * EXACTAMENTE. La altura no se compara: Arca qomá=1.5 (v.10),
     * Kapóret altura silente (v.17) — la Palabra no la fija. */
    if (kap_largo_x10 != g_arca.largo_x10 ||
        kap_ancho_x10 != g_arca.ancho_x10) {
        ARCA_LOG_E("kaporet_colocada: footprint NO coincide "
                   "(arca %u×%u vs kapóret %u×%u) — v.10/v.17 no encajan",
                   (unsigned)g_arca.largo_x10, (unsigned)g_arca.ancho_x10,
                   (unsigned)kap_largo_x10,    (unsigned)kap_ancho_x10);
        return HASHMAL_ARCA_ERR_SIN_KAPORET;
    }

    if (!g_arca.requiere_kaporet) {
        ARCA_LOG_W("kaporet_colocada: ya registrada encima (idempotente, no-op)");
        return HASHMAL_ARCA_OK;
    }
    g_arca.requiere_kaporet = false;
    ARCA_LOG_I("Kapóret colocada milmáʻla (Éx 25:21) — footprint %u×%u "
               "encaja, medidas 'y medio' cerradas",
               (unsigned)kap_largo_x10, (unsigned)kap_ancho_x10);
    return HASHMAL_ARCA_OK;
}

/* ==================================================================
 * API INTERNA (hashmal_storage_internal.h) - acceso mutable
 *
 * Los archivos hermanos del componente (arca_zer, arca_cifrado,
 * arca_baddim, arca_sealed_core, arca_interfaz, arca_merkle)
 * pasan por aqui para escribir el estado. El mundo exterior queda
 * restringido al getter const hashmal_storage_get_arca.
 * ================================================================== */
hashmal_arca_t* hashmal_storage_arca_mut(void)
{
    if (!g_iniciada) return NULL;
    return &g_arca;
}
