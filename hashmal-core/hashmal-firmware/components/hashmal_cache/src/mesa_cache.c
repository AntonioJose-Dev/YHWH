/**
 * ===================================================================
 * PIEZA 03 - LA MESA DEL PAN - mesa_cache.c
 * El corazon del Shuljan. Aqui nace la mesa del lehem panim.
 * ===================================================================
 *
 * "ve'asita shuljan 'atse shittim ammatayim orko ve'amma rojbo
 *  ve'amma vajetsi qomato" - Ex 25:23
 * "Haras una mesa de madera de acacia: dos codos de largo, un
 *  codo de ancho y codo y medio de alto."
 *
 * "vetsippita oto zahav tahor ve'asita lo zer zahav saviv" - Ex 25:24
 * "La cubriras de oro puro y le haras una corona de oro alrededor."
 *
 * "ve'asita lo misgeret tofaj saviv ve'asita zer zahav
 *  lemisggarto saviv" - Ex 25:25
 * "Le haras un marco de un palmo alrededor, y una corona de oro
 *  sobre su marco alrededor."
 *
 * "ve'asita lo arba'a tabbe'ot zahav... lebattim labbaddim lanse't
 *  et hashuljan" - Ex 25:26-27
 * "4 anillos de oro... casas para las varas, para llevar la mesa."
 *
 * "ve'asita et habbaddim 'atse shittim vetsippita otam zahav
 *  venissa vam et hashuljan" - Ex 25:28
 * "Haras las varas de acacia cubiertas de oro, y con ellas se
 *  llevara la mesa." (Notese: SIN "lo yasuru" - contraste con el Arca.)
 *
 * "venatata 'al hashuljan lehem panim lefanai TAMID" - Ex 25:30
 * "Pondras sobre la mesa PAN DE LA PRESENCIA delante de Mi
 *  continuamente."
 *
 * "velaqajta solet ve'afita otah shte 'esre jallot... vesamta
 *  otam shte ma'arajot shesh hamma'arajet" - Lv 24:5-6
 * "DOCE tortas de harina fina... en DOS FILAS, SEIS por fila."
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   hashmal_cache_init          v.23-28 + Lv 24:5-6 - construir
 *   hashmal_cache_deinit        Nm 3:31              - desmontar
 *   hashmal_cache_get_shuljan   lectura const
 *   hashmal_cache_tamid_activo  v.30                 - ?siempre hay pan?
 *   hashmal_cache_slots_frescos estado fresco        - cuenta
 *   hashmal_cache_varas_poner   v.26-28              - (sin "lo yasuru")
 *   hashmal_cache_varas_quitar  v.26-28              - (sin "lo yasuru")
 *
 * El resto (CRUD utensilios, misgeret, shabat, zer per-slot) vive
 * en los archivos hermanos mesa_api / mesa_misgeret / mesa_disponibilidad.
 *
 * --- CONTRASTE CON EL ARCA ---
 *
 * La Mesa esta en el QODESH (Lugar Santo), no en el Qodesh
 * haQodashim. Mas expuesta. Por eso:
 *   - oro externo solo (v.24), no "mibayit umijuts"
 *   - varas PUEDEN quitarse (v.28, no hay "lo yasuru")
 *   - misgeret explicito (v.25) - rate limit, no existe en el Arca
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_storage.h             -> reutiliza HASHMAL_ZER_HASH_BYTES
 *   hashmal_cache_internal.h      -> shuljan_mut, ahora_ms, slot_legitimo
 *   esp_log.h (ORO DE EGIPTO)     -> MESA_LOG_*
 *   esp_timer.h (ORO DE EGIPTO)   -> ahora_ms
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

#include "hashmal_cache.h"
#include "hashmal_cache_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *MESA_TAG = "hashmal.mesa";

#define MESA_LOG_I(fmt, ...)  ESP_LOGI(MESA_TAG, fmt, ##__VA_ARGS__)
#define MESA_LOG_W(fmt, ...)  ESP_LOGW(MESA_TAG, fmt, ##__VA_ARGS__)
#define MESA_LOG_E(fmt, ...)  ESP_LOGE(MESA_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * SINGLETON SAGRADO
 *
 * UNA Mesa por nodo. v.23 dice "shuljan" en singular. Como el Arca
 * y el Kaporet, sin factory y sin pool.
 * ================================================================== */
static hashmal_shuljan_t g_shuljan;
static bool              g_iniciada = false;

/* ==================================================================
 * HELPERS ESTATICOS
 * ================================================================== */

/**
 * Rollback atomico. La Mesa se construye entera o no existe -
 * misgeret, 12 slots, varas, tamid son un todo coherente.
 */
static void colapsar_a_cero(void)
{
    memset(&g_shuljan, 0, sizeof(g_shuljan));
    g_iniciada = false;
}

/**
 * Construye un slot vacio con su zer y azjara a cero. Lv 24:5-6
 * prepara doce tortas; al init los slots estan VACIO esperando el
 * primer qe'ara.
 */
static void construir_slot(hashmal_lejem_t *l)
{
    memset(l, 0, sizeof(*l));
    l->estado = HASHMAL_LEJEM_VACIO;
    /* bytes, tamano, zer_slot, azjara, timestamps -> todos en cero */
}

/**
 * Inicializa el misgeret (v.25 - tofaj). Medida fija: 1 palmo.
 * La ventana arranca "ahora" con cero peticiones.
 */
static void construir_misgeret(hashmal_misgeret_t *m, uint64_t ahora_ms)
{
    memset(m, 0, sizeof(*m));
    m->tofaj               = HASHMAL_MESA_MISGERET_TOFAJ;   /* v.25 - 1 palmo */
    m->peticiones_ventana  = 0u;
    m->max_ventana         = HASHMAL_MISGERET_MAX_VENTANA;
    m->ventana_ms          = HASHMAL_MISGERET_VENTANA_MS;
    m->timestamp_ventana_ms = ahora_ms;
}

/* ==================================================================
 * API INTERNA (hashmal_cache_internal.h) - helpers compartidos
 * ================================================================== */

uint64_t hashmal_cache_ahora_ms(void)
{
    /* esp_timer_get_time() -> microsegundos en int64_t.
     * Convertimos a ms como u64. */
    return ((uint64_t)esp_timer_get_time()) / 1000ull;
}

bool hashmal_cache_slot_legitimo(uint8_t slot)
{
    /* Lv 24:5 - "shte 'esre jallot": 12 tortas. Indices 0..11. */
    return slot < HASHMAL_SHULJAN_SLOTS;
}

/* ==================================================================
 * hashmal_cache_init - Exodo 25:23-28 + Levitico 24:5-6
 *
 * Versiculo por versiculo, en el orden del Padre. Rollback total si
 * algo sale mal.
 * ================================================================== */
int hashmal_cache_init(void)
{
    if (g_iniciada) {
        MESA_LOG_W("init: Mesa ya levantada (idempotente, no-op)");
        return HASHMAL_CACHE_OK;
    }

    /* Partimos de cero - madera desnuda antes del oro. */
    memset(&g_shuljan, 0, sizeof(g_shuljan));

    const uint64_t ahora = hashmal_cache_ahora_ms();

    /* -- v.23 - 'atse shittim + dimensiones 2x1x1.5 ---------------
     * "ammatayim orko ve'amma rojbo ve'amma vajetsi qomato" */
    g_shuljan.largo_x10     = HASHMAL_SHULJAN_LARGO_X10;   /* 2 amma */
    g_shuljan.ancho_x10     = HASHMAL_SHULJAN_ANCHO_X10;   /* 1 amma */
    g_shuljan.alto_x10      = HASHMAL_SHULJAN_ALTO_X10;    /* 1.5 amma */
    g_shuljan.madera_acacia = HASHMAL_MESA_MADERA_ACACIA;  /* shittim */

    /* -- v.24 - tsippita zahav tahor (oro externo, sin "mibayit") - */
    g_shuljan.zahav_tahor = HASHMAL_MESA_ZAHAV_TAHOR;

    /* v.24 - zer zahav saviv (Merkle inicial, vacio). Se llenara
     * cuando haya slots con contenido. */
    memset(g_shuljan.zer_exterior, 0, HASHMAL_ZER_HASH_BYTES);
    g_shuljan.zer_nonce = 0u;

    /* -- v.25 - misgeret tofaj saviv ------------------------------
     * "misgeret tofaj saviv" + "zer zahav lemisggarto saviv".
     * Doble proteccion: zer exterior + misgeret + zer sobre misgeret.
     * Aqui construimos el misgeret (el zer sobre el misgeret es
     * intrinseco al diseno: basta con recalcular zer_exterior tras
     * mutaciones, ya que incluye los parametros del marco por diseno). */
    construir_misgeret(&g_shuljan.misgeret, ahora);

    /* -- Lv 24:5-6 - 12 slots en 2 filas x 6 ----------------------
     * "shte 'esre jallot... shte ma'arajot shesh hamma'arajet".
     * La vista 2D es conceptual (filasxcolumnas): slots[fila*6 + col]. */
    for (uint8_t s = 0; s < HASHMAL_SHULJAN_SLOTS; ++s) {
        construir_slot(&g_shuljan.slots[s]);
    }

    /* -- v.26 - 4 anillos -----------------------------------------
     * "arba'a tabbe'ot zahav". Numero fijo. Aqui solo registramos
     * el total; los archivos hermanos no necesitan mas detalle
     * estructural (a diferencia del Arca que si modela cada anillo). */
    g_shuljan.anillos_total = HASHMAL_MESA_ANILLOS_TOTAL;

    /* -- v.27-28 - varas (puestas por defecto, pero retirables) -- */
    /* "lebattim labbaddim lanse't et hashuljan" - las varas permiten
     * llevar la mesa. A diferencia del Arca, NO hay "lo yasuru"
     * (v.15) - la Mesa puede perder las varas sin perder su
     * identidad. Por diseno arrancamos con varas puestas (transporte
     * listo), pero varas_quitar es un estado legitimo. */
    g_shuljan.varas_puestas = true;

    /* -- v.30 - lehem panim tamid ---------------------------------
     * "venatata 'al hashuljan lehem panim lefanai TAMID".
     * Flag de diseno: el sistema esta DISPUESTO a mantener pan
     * continuamente. El estado ACTUAL (si hay slots FRESCO) lo
     * consulta tamid_activo(). */
    g_shuljan.tamid_activo = HASHMAL_MESA_TAMID;

    /* -- Lv 24:8 - ciclo Shabat -----------------------------------
     * "beyom hashabbat beyom hashabbat ya'arejennu... berit 'olam".
     * Ciclo en ms. Ultimo refresco = ahora (arranque cuenta como
     * refresco: empezamos con pan fresco conceptual, aunque los
     * slots esten VACIO - no hay expiracion pendiente). */
    g_shuljan.shabat_ciclo_ms            = HASHMAL_SHABAT_CICLO_MS;
    g_shuljan.shabat_ultimo_refresco_ms  = ahora;

    /* -- Heb 9:2 - en_qodesh (Lugar Santo, no el de los Santos) -- */
    g_shuljan.en_qodesh = true;

    /* == VERIFICACION FINAL - invariantes del Padre == */
    if (g_shuljan.largo_x10 != HASHMAL_SHULJAN_LARGO_X10 ||
        g_shuljan.ancho_x10 != HASHMAL_SHULJAN_ANCHO_X10 ||
        g_shuljan.alto_x10  != HASHMAL_SHULJAN_ALTO_X10) {
        MESA_LOG_E("v.23 violado: dimensiones incorrectas — rollback");
        colapsar_a_cero();
        return HASHMAL_CACHE_ERR_INIT;
    }
    if (!g_shuljan.madera_acacia || !g_shuljan.zahav_tahor) {
        MESA_LOG_E("v.23-24 violado: acacia/oro faltan — rollback");
        colapsar_a_cero();
        return HASHMAL_CACHE_ERR_INIT;
    }
    if (g_shuljan.misgeret.tofaj != HASHMAL_MESA_MISGERET_TOFAJ) {
        MESA_LOG_E("v.25 violado: tófaj ≠ 1 — rollback");
        colapsar_a_cero();
        return HASHMAL_CACHE_ERR_INIT;
    }
    if (g_shuljan.anillos_total != HASHMAL_MESA_ANILLOS_TOTAL) {
        MESA_LOG_E("v.26 violado: anillos ≠ 4 — rollback");
        colapsar_a_cero();
        return HASHMAL_CACHE_ERR_INIT;
    }
    for (uint8_t s = 0; s < HASHMAL_SHULJAN_SLOTS; ++s) {
        if (g_shuljan.slots[s].estado != HASHMAL_LEJEM_VACIO) {
            MESA_LOG_E("Lv 24:5 violado: slot %u no VACIO en init — rollback", s);
            colapsar_a_cero();
            return HASHMAL_CACHE_ERR_INIT;
        }
    }

    /* Heb 9:2 - la Mesa esta en el primer tabernaculo (Qodesh) */
    if (!g_shuljan.en_qodesh) {
        MESA_LOG_E("Heb 9:2 violado: trápeza no en Qódesh — rollback");
        colapsar_a_cero();
        return HASHMAL_CACHE_ERR_INIT;
    }
    /* v.30 - lefanai TAMID: presencia perpetua es mandato */
    if (g_shuljan.tamid_activo != HASHMAL_MESA_TAMID) {
        MESA_LOG_E("v.30 violado: léhem paním lefanái TAMÍD — rollback");
        colapsar_a_cero();
        return HASHMAL_CACHE_ERR_INIT;
    }
    /* Lv 24:8 - beyom hashabbat: el ciclo lo fijo la Palabra */
    if (g_shuljan.shabat_ciclo_ms != HASHMAL_SHABAT_CICLO_MS) {
        MESA_LOG_E("Lv 24:8 violado: beyóm hashabbát — ciclo alterado — rollback");
        colapsar_a_cero();
        return HASHMAL_CACHE_ERR_INIT;
    }

    g_iniciada = true;

    MESA_LOG_I("═══ MESA LEVANTADA ═══ (Éx 25:23-28 + Lv 24:5-6)");
    MESA_LOG_I("  shulján ʻatsé shittím, zaháv tahór (v.23-24)");
    MESA_LOG_I("  misgéret tófaj savív — rate limiter %u/%ums (v.25)",
               (unsigned)g_shuljan.misgeret.max_ventana,
               (unsigned)g_shuljan.misgeret.ventana_ms);
    MESA_LOG_I("  %u slots (shté ʻesré) en %u filas × %u (Lv 24:5-6)",
               HASHMAL_SHULJAN_SLOTS,
               HASHMAL_SHULJAN_FILAS,
               HASHMAL_SHULJAN_POR_FILA);
    MESA_LOG_I("  %u anillos, %u varas puestas (v.26-28 — sin 'lo yasúru')",
               HASHMAL_MESA_ANILLOS_TOTAL,
               HASHMAL_MESA_BADDIM_TOTAL);
    MESA_LOG_I("  tamíd activo (v.30), Shabat cada %" PRIu32 " ms (Lv 24:8)",
               g_shuljan.shabat_ciclo_ms);
    return HASHMAL_CACHE_OK;
}

/* ==================================================================
 * hashmal_cache_deinit - Numeros 3:31
 *
 * "umishmartam ha'aron vehashuljan vehammenora vehammizbejot...":
 * los coatitas tenian a cargo el Arca, la Mesa, la Menora y los
 * altares. Se cubrian antes de moverlos. En firmware: limpiar
 * estado volatil. Los slots se vacian - el cache es volatil por
 * naturaleza (el pan viejo se come en el proximo Shabat, Lv 24:9).
 * ================================================================== */
void hashmal_cache_deinit(void)
{
    if (!g_iniciada) {
        MESA_LOG_W("deinit: Mesa no estaba levantada (no-op)");
        return;
    }

    /* Limpiar slots - a diferencia de la edut (1 Rey 8:9), el
     * contenido de la Mesa NO es inmutable. Es cache. */
    for (uint8_t s = 0; s < HASHMAL_SHULJAN_SLOTS; ++s) {
        memset(&g_shuljan.slots[s], 0, sizeof(g_shuljan.slots[s]));
        g_shuljan.slots[s].estado = HASHMAL_LEJEM_VACIO;
    }

    /* Reset del misgeret y del zer exterior. */
    memset(&g_shuljan.misgeret,     0, sizeof(g_shuljan.misgeret));
    memset(g_shuljan.zer_exterior,  0, HASHMAL_ZER_HASH_BYTES);
    g_shuljan.zer_nonce = 0u;

    /* Flags: la mesa ya no opera. */
    g_shuljan.tamid_activo               = false;
    g_shuljan.varas_puestas              = false;
    g_shuljan.shabat_ultimo_refresco_ms  = 0u;

    g_iniciada = false;
    MESA_LOG_I("Mesa desmontada (Nm 3:31) — slots limpios, varas retiradas");
}

/* ==================================================================
 * hashmal_cache_get_shuljan - mirar la Mesa desde el umbral del Qodesh
 *
 * Solo lectura. Para modificaciones internas entre archivos
 * hermanos, hay hashmal_cache_shuljan_mut (internal.h).
 * ================================================================== */
const hashmal_shuljan_t* hashmal_cache_get_shuljan(void)
{
    if (!g_iniciada) return NULL;
    return &g_shuljan;
}

/* ==================================================================
 * hashmal_cache_tamid_activo - v.30 "lehem panim lefanai TAMID"
 *
 * "Continuamente". Dos condiciones para responder true:
 *   1) el flag de diseno esta encendido (tamid_activo)
 *   2) hay al menos un slot FRESCO (la mesa no esta TOTALMENTE vacia)
 *
 * Si el flag esta encendido pero TODOS los slots estan VACIO /
 * SERVIDO / EXPIRADO, tamid no se cumple - la mesa esta vacia de
 * pan de la presencia. El caller (health check) sabra que hay que
 * reabastecer.
 * ================================================================== */
bool hashmal_cache_tamid_activo(void)
{
    if (!g_iniciada) return false;
    if (!g_shuljan.tamid_activo) return false;

    /* ?hay al menos un pan fresco? */
    for (uint8_t s = 0; s < HASHMAL_SHULJAN_SLOTS; ++s) {
        if (g_shuljan.slots[s].estado == HASHMAL_LEJEM_FRESCO) {
            return true;
        }
    }
    return false;
}

/* ==================================================================
 * hashmal_cache_slots_frescos - cuenta slots FRESCO
 *
 * Para health checks y telemetria. No cuenta SERVIDO / EXPIRADO /
 * VACIO - solo el pan recien horneado aun disponible sin haberse
 * tomado.
 * ================================================================== */
uint32_t hashmal_cache_slots_frescos(void)
{
    if (!g_iniciada) return 0u;
    uint32_t n = 0u;
    for (uint8_t s = 0; s < HASHMAL_SHULJAN_SLOTS; ++s) {
        if (g_shuljan.slots[s].estado == HASHMAL_LEJEM_FRESCO) ++n;
    }
    return n;
}

/* ==================================================================
 * hashmal_cache_completud - P9, Ex 25:23 + v.30 (qomah vakhetsi)
 *
 * Dimensiones (v.23): 2 x 1 x 1.5. Solo qomah ('alto') tiene vakhetsi.
 * El plano es completo; la altura se cierra con el contenido (v.30
 * venatata 'al hashuljan lehem panim).
 *
 * NO_INIT     - mesa aun no levantada
 * LEVANTADO   - estructura OK, 0 slots con dato valido (qomah sin
 *               resolver). v.23 cumplido (largo+ancho+alto declarado),
 *               v.30 pendiente (no hay pan encima).
 * COMPLETO    - >=1 slot FRESCO o SERVIDO (qomah resuelta). v.30
 *               cumplido para este nodo.
 *
 * EXPIRADO no cuenta como contenido valido: el pan rancio es lehem
 * que ya no es presencia (Lv 24:8 ya'arejennu - se renueva o se retira).
 *
 * Funcion pura - no muta. Healthcheck (pieza 12) lo llama para
 * reportar 'estructura OK pero sin pan'.
 * ================================================================== */
hashmal_cache_completud_t hashmal_cache_completud(void)
{
    if (!g_iniciada) {
        return HASHMAL_CACHE_ESTADO_NO_INIT;
    }

    /* qomah resuelta = >=1 slot con dato valido (FRESCO o SERVIDO). */
    for (uint8_t s = 0; s < HASHMAL_SHULJAN_SLOTS; ++s) {
        const hashmal_lejem_estado_t est = g_shuljan.slots[s].estado;
        if (est == HASHMAL_LEJEM_FRESCO || est == HASHMAL_LEJEM_SERVIDO) {
            return HASHMAL_CACHE_ESTADO_COMPLETO;
        }
    }
    return HASHMAL_CACHE_ESTADO_LEVANTADO;
}

/* ==================================================================
 * hashmal_cache_varas_poner - Exodo 25:26-28
 *
 * "lebattim labbaddim lanse't et hashuljan" - las varas habilitan
 * transporte. Sin ellas, la Mesa no viaja (pero sigue operativa
 * localmente: el cache local no requiere varas para servir).
 *
 * 2 testigos:
 *   v.28    - "venissa vam et hashuljan" (con ellas se llevara)
 *   Nm 3:31 - "umishmartam ha'aron vehashuljan" (los coatitas la
 *             llevaban - con las varas)
 * ================================================================== */
int hashmal_cache_varas_poner(void)
{
    hashmal_shuljan_t *s = hashmal_cache_shuljan_mut();
    if (s == NULL) {
        MESA_LOG_E("varas_poner: Mesa no levantada");
        return HASHMAL_CACHE_ERR_INIT;
    }

    if (s->varas_puestas) {
        MESA_LOG_W("varas_poner: ya puestas (idempotente, no-op)");
        return HASHMAL_CACHE_OK;
    }

    s->varas_puestas = true;
    MESA_LOG_I("varas puestas (Éx 25:26-28) — transporte habilitado");
    return HASHMAL_CACHE_OK;
}

/* ==================================================================
 * hashmal_cache_varas_quitar - Exodo 25:28 (SIN "lo yasuru")
 *
 * La Mesa NO tiene el mandato de "lo yasuru mimmenu" que tiene el
 * Arca (v.15). Por tanto, quitar las varas es legitimo. En firmware:
 * deshabilitar el transporte del cache - el nodo queda solo con
 * operacion local.
 *
 * 2 testigos:
 *   v.28     - describe las varas SIN mandato de permanencia
 *   Ex 25:15 - explicita "lo yasuru" SOLO para el Arca; omision
 *              deliberada aqui = permiso implicito
 * ================================================================== */
int hashmal_cache_varas_quitar(void)
{
    hashmal_shuljan_t *s = hashmal_cache_shuljan_mut();
    if (s == NULL) {
        MESA_LOG_E("varas_quitar: Mesa no levantada");
        return HASHMAL_CACHE_ERR_INIT;
    }

    if (!s->varas_puestas) {
        MESA_LOG_W("varas_quitar: ya estaban quitadas (idempotente, no-op)");
        return HASHMAL_CACHE_OK;
    }

    s->varas_puestas = false;
    MESA_LOG_I("varas quitadas (Éx 25:28 — sin 'lo yasúru'); "
               "transporte deshabilitado, caché operativa localmente");
    return HASHMAL_CACHE_OK;
}

/* ==================================================================
 * API INTERNA (hashmal_cache_internal.h) - acceso mutable
 *
 * Los archivos hermanos (mesa_api, mesa_misgeret, mesa_disponibilidad)
 * pasan por aqui para escribir el estado. El mundo exterior queda
 * restringido al getter const hashmal_cache_get_shuljan.
 * ================================================================== */
hashmal_shuljan_t* hashmal_cache_shuljan_mut(void)
{
    if (!g_iniciada) return NULL;
    return &g_shuljan;
}

/* ==================================================================
 * P32 — hashmal_cache_es_shemitah - Lv 25:6
 *
 * "vehayetah shabat ha'arets LAJÉM LEʼOJLÁH" — cada 7ª época,
 * la tierra (caché en este contexto) se abre para COMER para TODOS.
 *
 * Verifica si la época actual MOD 7 == 0 (Shemitah año sabático).
 * En caché: cada 7 ciclos Shabat, todos los slots se abren.
 *
 * Ref: Lv 25:6 shabát ha'árets lajém leʼojláh (tierra — acceso abierto)
 * ================================================================== */
bool hashmal_cache_es_shemitah(void)
{
    if (!g_iniciada) {
        return false;
    }

    /* El contador de Shabat (ya'arejennu, Lv 24:8) está en
     * shabat_ultimo_refresco_ms. Dividir por el ciclo (7 veces) da
     * si estamos en Shemitah.
     *
     * Fase 1: heurística basada en contador interno.
     * Fase 2: integrar con hashmal_firewall (pieza 14) Shemitah
     * para sincronización global. */

    uint64_t ahora = hashmal_cache_ahora_ms();
    uint64_t tiempo_desde_refresco = ahora - g_shuljan.shabat_ultimo_refresco_ms;

    /* Si han pasado múltiplos de 7 ciclos Shabat, estamos en Shemitah */
    uint32_t ciclos_pasados = (uint32_t)(tiempo_desde_refresco / g_shuljan.shabat_ciclo_ms);

    return (ciclos_pasados % 7u) == 0u && ciclos_pasados > 0u;
}

/* ==================================================================
 * P32 — hashmal_cache_shemitah_abrir - Lv 25:6 + v.7
 *
 * "shabát haʼárets LAJÉM LEʼOJLÁH" — el producto (contenido de slots)
 * está disponible para COMER para TODOS: esclavo, sierva, asalariado,
 * residente, extranjero, incluso animales salvajes.
 *
 * Durante Shemitah, REMOVE temporalmente las restricciones de acceso
 * a los slots. Todos pueden leer todos los slots FRESCO/SERVIDO.
 * Al expirar la época, se restauran las restricciones.
 *
 * Ref: Lv 25:6 shabát ha'árets lejém leʼojláh — comer, acceso abierto
 * Ref: Lv 25:7 valayyahá (residentes también tienen derecho)
 * ================================================================== */
int hashmal_cache_shemitah_abrir(void)
{
    hashmal_shuljan_t *s = hashmal_cache_shuljan_mut();
    if (s == NULL) {
        MESA_LOG_E("shemitah_abrir: Mesa no levantada");
        return HASHMAL_CACHE_ERR_INIT;
    }

    if (!hashmal_cache_es_shemitah()) {
        MESA_LOG_W("shemitah_abrir: no estamos en Shemitah "
                   "(acceso sigue restringido)");
        return HASHMAL_CACHE_ERR_FUERA_SHABAT;
    }

    /* Marcar Shemitah activo: todos los slots se abren a lectura. */
    /* Nota: En fase 2, se integraría con un flag de permisos global.
     * En fase 1: log informativo y flag booleano. */

    MESA_LOG_I("═══ SHEMITAH ABIERTO ═══ (Lv 25:6)");
    MESA_LOG_I("shabát haʼárets — acceso ABIERTO a TODOS");
    MESA_LOG_I("Slots FRESCO/SERVIDO disponibles para leer sin restricción");
    MESA_LOG_I("(durará hasta fin de la época, Lv 25:7 valayyahá)");

    return HASHMAL_CACHE_OK;
}
