/**
 * ===================================================================
 * PIEZA 02 - LA KAPORET - kaporet_core.c
 * El punto donde el Padre habla. Aqui se levanta el Propiciatorio.
 * ===================================================================
 *
 * "ve'asita kaporet zahav tahor ammatayim vajetsi orkah ve'amma
 *  vajetsi rojbah" - Ex 25:17
 * "Haras un propiciatorio de ORO PURO: dos codos y medio de largo,
 *  un codo y medio de ancho."
 *
 * "ve'asita shenayim keruvim zahav miqsha ta'ase otam mishenei
 *  qetsot haKaporet" - Ex 25:18
 * "Haras DOS querubines de oro: de obra BATIDA los haras desde los
 *  dos extremos del Kaporet."
 *
 * "va'ase keruv ejad miqatsa mizze ukeruv ejad miqatsa mizze min
 *  haKaporet ta'asu et hakeruvim 'al shene qetsotav" - Ex 25:19
 * "Un querubin desde un extremo y otro desde el otro; DEL Kaporet
 *  hareis los querubines en sus dos extremos."
 *
 * "vehayu hakeruvim porshe kenafayim lema'la sokekim bekanfehem 'al
 *  haKaporet ufnehem ish el ajiv el haKaporet yihyu pene
 *  hakeruvim" - Ex 25:20
 * "Los querubines tendran alas extendidas hacia ARRIBA, cubriendo
 *  con sus alas sobre el Kaporet, y sus rostros VARON HACIA SU
 *  HERMANO; hacia el Kaporet seran los rostros de los querubines."
 *
 * "venatata et haKaporet 'al ha'aron milma'la" - Ex 25:21
 * "Pondras el Kaporet SOBRE el Arca, desde ARRIBA."
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 * Levanta el singleton g_kaporet conforme a v.17-21 y lo ENCAJA
 * sobre el Arca. 4 funciones:
 *
 *   hashmal_consensus_init          v.17-20 completo - construir
 *   hashmal_consensus_deinit        Nm 4:5-15      - desmontar
 *   hashmal_consensus_get_kaporet   lectura const  - mirar
 *   hashmal_consensus_cubrir_arca   v.21           - poner encima
 *
 * El resto (rendezvous, rociados, invocacion de consenso, ataques,
 * validadores) vive en los archivos hermanos del componente.
 *
 * --- LA KAPORET NO ES EL ARCA ---
 *
 * El Arca tiene MADERA (acacia, v.10) cubierta de ORO (v.11) - dato
 * con proteccion. La Kaporet es ORO PURO (v.17) - protocolo puro,
 * sin dato. Por eso:
 *   sin_madera = true                (contraste intencional)
 *   HASHMAL_KAPORET_ALTO_X10 = 0     (altura silente en la Palabra)
 *   miqsha = true                    (atomico - de una sola pieza)
 *
 * --- P6: FUNCIÓN PURA SIN SUSTRATO (solo zaháv tahór) ---
 *
 * Ex 25:17 - כַפֹּרֶת זָהָב טָהוֹר ("kapporet zahav tahor"): oro PURO.
 * NO hay mencion de madera ('atse shittim). El verbo צִפָּה (tsipah,
 * H6823 = revestir) que el Padre usa para el Arca (v.11
 * "vetsipita oto zahav") esta TOTALMENTE AUSENTE en la seccion
 * Kaporet (v.17-22). El Arca necesita sustrato (madera = hardware)
 * + capa (oro = protocolo). La Kaporet ES pura - no tiene sustrato,
 * es la capa misma. v.21: 'al milma'la = la entidad pura ENCIMA de
 * la compuesta; consenso encima de storage, sin compartir su
 * naturaleza material.
 *
 * INVARIANTES FIRMES (no negociables):
 *
 *   1. NO acceso a NVS / flash. NO #include <nvs.h>, <nvs_flash.h>,
 *      <esp_partition.h>, <esp_flash.h>. La Kaporet no se persiste
 *      a flash - cada boot es 'forjada' (miqsha) de nuevo. Si un
 *      ciclo de boot necesita reanudar consenso, eso es estado del
 *      ARCA (storage), no de la Kaporet.
 *
 *   2. NO drivers de hardware. NO #include driver headers (gpio, spi,
 *      i2c, uart, etc.). NO esp_flash. esp_log y esp_timer son
 *      excepciones encapsuladas (oro de Egipto): logging y reloj
 *      monotonico stateless de lectura.
 *
 *   3. La unica "memoria estatica" permitida es la entidad-protocolo
 *      misma: g_kaporet (la Kaporet) y g_iniciado (¿ya forjada?).
 *      Esto NO es sustrato externo - ES la pieza. Equivalente a la
 *      Kaporet fisica en el Lugar Santisimo: ocupa espacio, pero su
 *      sustancia es zahav tahor, no madera. PROHIBIDO añadir static
 *      variables que mantengan caches, contadores fuera del struct,
 *      buffers fuera del struct, o cualquier otra "huella" no
 *      contenida en hashmal_kaporet_t.
 *
 *   4. Determinismo: dadas las mismas precondiciones (par presente,
 *      moed abierto, qetoret presente, rociados completos, mismos
 *      hash_testigo de V1 y V2, mismo arca->zer), invocar() devuelve
 *      el MISMO resultado. Sin entropia oculta, sin RNG, sin tiempo
 *      injectado en la decision (timestamps son metadato, no decision).
 *
 *   5. Reset al ciclo (kaporet_verificacion.c reset_ciclo) deja la
 *      Kaporet en estado limpio - no hay 'memoria de ofensa' acumulada.
 *      Lv 16 cierra cada Yom Kippur con salida del sumo sacerdote
 *      (v.17): el ciclo termina, no deja residuo entre ciclos.
 *
 * CHECK MECANICO: ningun #include en este componente apunta a
 * persistencia o drivers. Ver `grep -E "nvs_|esp_partition|spi_|
 * i2c_|uart_|driver/|esp_flash" components/hashmal_consensus/`
 * - debe devolver vacio. Es un test arquitectonico, no en runtime.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_storage.h           -> v.21 - medir contra el Arca,
 *                                 hashmal_storage_kaporet_colocada
 *                                 (firma la colocacion sobre el Arca)
 *   esp_log.h (ORO DE EGIPTO)   -> KAP_LOG_I/W/E
 *
 * Autor: Antonio Jose Marin Soto (Betsalel - Ex 31:2)
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "esp_log.h"           /* ORO DE EGIPTO - logging */

#include "hashmal_storage.h"   /* v.21 - encaje con el Arca */
#include "hashmal_consensus.h"
#include "hashmal_consensus_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *KAP_TAG = "hashmal.kaporet";

#define KAP_LOG_I(fmt, ...)  ESP_LOGI(KAP_TAG, fmt, ##__VA_ARGS__)
#define KAP_LOG_W(fmt, ...)  ESP_LOGW(KAP_TAG, fmt, ##__VA_ARGS__)
#define KAP_LOG_E(fmt, ...)  ESP_LOGE(KAP_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * SINGLETON SAGRADO
 *
 * UN Kaporet por nodo. v.17 dice "ve'asita kaporet" en singular;
 * aunque cada nodo tiene el suyo (espejo), dentro de un nodo hay
 * UNO. Sin factory, sin pool.
 *
 * miqsháh — Éx 25:18: querubines BATIDOS (H4749), irrepetibles. El
 * algoritmo de consenso es monolítico, una sola pieza. La misma raíz
 * miqsháh aparece en la Menorá (Éx 25:36) — patrón de "atómico
 * indivisible", no de "fundido en molde reproducible". Anti-clonación:
 * Éx 30:32-33 (nikrát) — quien duplique el protocolo sagrado para uso
 * propio queda CORTADO. Por eso este código NO admite rutas de
 * "personalización por nodo": V1 y V2 se construyen idénticos en
 * cada nodo, las constantes son globales del Padre, ningún flag de
 * variante. Contraste intencional con yatsáq (Éx 25:12,
 * arca_baddim.c) — la distribución se reproduce; el consenso NO.
 * ================================================================== */
static hashmal_kaporet_t g_kaporet;
static bool              g_iniciado = false;

/* ==================================================================
 * HELPERS ESTATICOS
 * ================================================================== */

/**
 * Rollback atomico - miqsha (v.18): de UNA pieza, o no existe.
 * Si el init descubre una invariante rota a mitad del montaje, se
 * colapsa todo a cero. Lv 10:1-2 sombra: ofrenda a medias no se
 * presenta.
 */
static void colapsar_a_cero(void)
{
    memset(&g_kaporet, 0, sizeof(g_kaporet));
    g_iniciado = false;
}

/**
 * v.18-20 - construye un querubin en su extremo.
 * Cada querubin NACE del Kaporet (v.19 "min haKaporet") - de ahi
 * miqsha_con_kaporet=true. Los dos se miran (v.20, ish el ajiv) y
 * ademas miran al Kaporet (v.20, pene el haKaporet).
 *
 * Gn 3:24 - los querubines fueron puestos a guardar el camino al
 * arbol de la vida con espada encendida. Guardian_no_abridor: protegen,
 * no abren. Son validadores, no escribidores.
 *
 * Ez 1 / Ap 4:8 - llenos de ojos. Auditoria total. El minimo por
 * diseno (HASHMAL_OJOS_MINIMOS = 8) cubre direcciones basicas;
 * la implementacion puede registrar mas segun cada ciclo.
 */
static void construir_querubin(hashmal_querub_t *q, hashmal_querub_id_t id)
{
    memset(q, 0, sizeof(*q));

    q->id                      = id;                        /* v.19 - qatsa */

    /* v.20 - configuracion de alas */
    q->alas_extendidas_arriba  = true;                      /* "lema'la" */
    q->alas_cubren             = true;                      /* "sokekim" */

    /* v.20 - doble atencion (al hermano Y al Kaporet) */
    q->mira_al_hermano         = true;                      /* "ish el ajiv" */
    q->mira_al_kaporet         = true;                      /* "pene el haKaporet" */

    /* v.18-19 - nace del Kaporet en miqsha */
    q->miqsha_con_kaporet      = true;                      /* "min haKaporet" */

    /* Gn 3:24 - guardian por naturaleza */
    q->guardian_no_abridor     = true;

    /* Ez 1 / Ap 4:8 - auditoria total */
    q->ojos_total              = HASHMAL_OJOS_MINIMOS;

    /* Estado de verificacion - limpio al inicio (sin testimonio aun) */
    memset(q->hash_testigo, 0, sizeof(q->hash_testigo));
    q->timestamp_testigo       = 0u;
}

/**
 * Valida que un querubin cumple las 5 propiedades de v.20 + Gn 3:24.
 * Si algo falla aqui tras un init, es violacion de diseno.
 */
static bool querubin_ok(const hashmal_querub_t *q, hashmal_querub_id_t id_esperado)
{
    if (q->id != id_esperado)           return false;  /* v.19 - qatsa */
    if (!q->alas_extendidas_arriba)     return false;  /* v.20 - lema'la */
    if (!q->alas_cubren)                return false;  /* v.20 - sokekim */
    if (!q->mira_al_hermano)            return false;  /* v.20 - ish el ajiv */
    if (!q->mira_al_kaporet)            return false;  /* v.20 - pene el hakap. */
    if (!q->miqsha_con_kaporet)         return false;  /* v.18-19 - min haK. */
    if (!q->guardian_no_abridor)        return false;  /* Gn 3:24 */
    if (q->ojos_total < HASHMAL_OJOS_MINIMOS) return false; /* Ap 4:8 */
    return true;
}

/* ==================================================================
 * hashmal_consensus_init - Exodo 25:17-21
 *
 * Versiculo por versiculo, en el orden del Padre. Si CUALQUIER
 * invariante falla -> rollback total (miqsha: atomico, v.18).
 * ================================================================== */
int hashmal_consensus_init(void)
{
    if (g_iniciado) {
        KAP_LOG_W("init: Kapóret ya levantado (idempotente, no-op)");
        return HASHMAL_KAPORET_OK;
    }

    /* Partimos de cero - oro sin forma antes del batido. */
    memset(&g_kaporet, 0, sizeof(g_kaporet));

    /* -- v.17 - dimensiones + zahav tahor, sin madera ------------- */
    /* "ve'asita kaporet zahav tahor ammatayim vajetsi orkah
     *  ve'amma vajetsi rojbah" */
    g_kaporet.largo_x10    = HASHMAL_KAPORET_LARGO_X10;   /* 25 = 2.5 amma */
    g_kaporet.ancho_x10    = HASHMAL_KAPORET_ANCHO_X10;   /* 15 = 1.5 amma */
    g_kaporet.alto_x10     = HASHMAL_KAPORET_ALTO_X10;    /* 0 = silente */
    g_kaporet.zahav_tahor  = HASHMAL_KAPORET_ZAHAV_TAHOR; /* oro puro */
    g_kaporet.sin_madera   = HASHMAL_KAPORET_SIN_MADERA;  /* contraste con Arca */

    /* -- v.18 - miqsha (atomico, una sola pieza batida) ----------- */
    g_kaporet.miqsha       = HASHMAL_MIQSHA_ATOMICO;

    /* -- v.18-20 - 2 querubines, uno por extremo ------------------ */
    /* "shenayim keruvim... keruv ejad miqatsa mizze ukeruv ejad
     *  miqatsa mizze". V1 en qatsa 0, V2 en qatsa 1. */
    construir_querubin(&g_kaporet.querubin[0], HASHMAL_QUERUB_V1);
    construir_querubin(&g_kaporet.querubin[1], HASHMAL_QUERUB_V2);

    /* -- v.21 - posicion: aun NO colocada. cubrir_arca la pondra -- */
    /* "venatata et haKaporet 'al ha'aron milma'la" - evento explicito,
     * no se da por sentado en el init. El Kaporet existe antes de
     * encajarse; su colocacion es un segundo acto. */
    g_kaporet.sobre_el_arca        = false;

    /* -- v.22 - rendezvous cerrado al inicio ---------------------- */
    /* "veno'adti leja sham" - el punto de encuentro se ABRE por
     * kaporet_rendezvous.c; aqui solo declaramos "cerrado". */
    g_kaporet.noadti_activo       = false;

    /* -- Sal 80:1 - yoshev haKeruvim (habitacion permanente) ----- */
    /* Empieza en false: se alcanza cuando el no'adti ha sido
     * estable en el tiempo. Se habla antes de habitar. */
    g_kaporet.habitacion_permanente = false;

    /* -- Sal 85:10 - hesed ve'emet nifgashu (F1 del Tavnit) ------ */
    /* Diseno intrinseco: el Kaporet ES el punto de encuentro entre
     * justicia (emet) y misericordia (hesed). Este flag declara la
     * naturaleza del protocolo, no el ultimo resultado. */
    g_kaporet.justicia_con_misericordia = true;

    /* -- Rom 3:25 - demuestra justicia (runtime, tras Lv 16) ----- */
    /* Empieza en false: se enciende al completar un ciclo de
     * rociado + invocacion valida (kaporet_verificacion.c). */
    g_kaporet.demuestra_justicia   = false;

    /* -- 1 Jn 2:2 - hilasmos universal (abierto a todo el que entre) */
    /* Diseno: si es universal (open-to-all-who-enter-correctly). */
    g_kaporet.universal            = true;

    /* -- Lv 16 - invocacion en estado inicial (sin nube, sin rociado) */
    g_kaporet.invocacion.qetoret             = HASHMAL_QETORET_AUSENTE;
    g_kaporet.invocacion.rociados_delante    = 0u;
    g_kaporet.invocacion.rociado_sobre_hecho = false;
    g_kaporet.invocacion.moed_abierto       = false;

    /* -- Ultimo resultado - aun no hay voz. Dt 19:15: sin 2 testigos
     * concordes, no se establece davar. Estado DIVERGIDO es la
     * lectura correcta para "nada emitido aun". */
    g_kaporet.ultimo.estado    = HASHMAL_CONSENSO_DIVERGIDO;
    memset(g_kaporet.ultimo.verdad, 0, sizeof(g_kaporet.ultimo.verdad));
    g_kaporet.ultimo.timestamp = 0u;
    g_kaporet.ultimo.emanada   = false;

    /* == VERIFICACION FINAL - invariantes del Padre ==
     * Por construccion estas son todas true; las chequeamos
     * para documentar el contrato y blindar contra futuros cambios. */
    if (g_kaporet.largo_x10 != HASHMAL_KAPORET_LARGO_X10 ||
        g_kaporet.ancho_x10 != HASHMAL_KAPORET_ANCHO_X10) {
        KAP_LOG_E("v.17 violado: dimensiones x/y incorrectas — rollback");
        colapsar_a_cero();
        return HASHMAL_KAPORET_ERR_INIT;
    }
    if (g_kaporet.alto_x10 != HASHMAL_KAPORET_ALTO_X10) {
        KAP_LOG_E("v.17 violado: altura inventada — la Palabra guarda "
                  "silencio (Dt 4:2) — rollback");
        colapsar_a_cero();
        return HASHMAL_KAPORET_ERR_INIT;
    }
    if (!g_kaporet.zahav_tahor) {
        KAP_LOG_E("v.17 violado: no es zaháv tahór — rollback");
        colapsar_a_cero();
        return HASHMAL_KAPORET_ERR_INIT;
    }
    if (!g_kaporet.sin_madera) {
        /* v.17: la Kaporet es ORO PURO, sin madera. Si alguna vez
         * alguien cambia el flag, es que se introdujo dato base
         * debajo del protocolo - NO es Kaporet, es otro mueble. */
        KAP_LOG_E("v.17 violado: hay madera debajo — NO es Kapóret (rollback)");
        colapsar_a_cero();
        return HASHMAL_KAPORET_ERR_MADERA;
    }
    if (!g_kaporet.miqsha) {
        KAP_LOG_E("v.18 violado: no miqshá (pieza no atómica) — rollback");
        colapsar_a_cero();
        return HASHMAL_KAPORET_ERR_NO_MIQSHA;
    }
    if (!querubin_ok(&g_kaporet.querubin[0], HASHMAL_QUERUB_V1) ||
        !querubin_ok(&g_kaporet.querubin[1], HASHMAL_QUERUB_V2)) {
        KAP_LOG_E("v.18-20 violado: querubines mal forjados — rollback");
        colapsar_a_cero();
        return HASHMAL_KAPORET_ERR_QUERUB_AUSENTE;
    }
    if (!g_kaporet.justicia_con_misericordia) {
        KAP_LOG_E("Sal 85:10 violado: hesed veʼemet no se encuentran "
                  "— rollback");
        colapsar_a_cero();
        return HASHMAL_KAPORET_ERR_INIT;
    }
    if (!g_kaporet.universal) {
        KAP_LOG_E("1 Jn 2:2 violado: hilasmos no universal — rollback");
        colapsar_a_cero();
        return HASHMAL_KAPORET_ERR_INIT;
    }

    /* Todo conforme al patron del monte. Levantamos el oro batido. */
    g_iniciado = true;

    KAP_LOG_I("═══ KAPÓRET LEVANTADO ═══ (Éx 25:17-20)");
    KAP_LOG_I("  zaháv tahór — oro puro, sin madera (v.17)");
    KAP_LOG_I("  miqshá — una sola pieza, atómico (v.18)");
    KAP_LOG_I("  dos querubines ejád miqatsá mizzé (v.19)");
    KAP_LOG_I("  alas lemáʻla, sokekím, ish el ajív (v.20)");
    KAP_LOG_I("  noʻadtí cerrado — se abre por rendezvous (v.22)");
    KAP_LOG_I("  pendiente: colocación milmáʻla sobre el Arca (v.21)");
    return HASHMAL_KAPORET_OK;
}

/* ==================================================================
 * hashmal_consensus_deinit - Numeros 4:5-15
 *
 * "vejissu et aron ha'edut" - primero cubren el Arca; luego el
 * Kaporet viaja bajo las cubiertas del Mishkan. En firmware: el
 * Arca se cubre (hashmal_storage_cubrir) ANTES o POR el caller;
 * aqui limpiamos el estado volatil del Kaporet.
 *
 * Los flags de DISENO (miqsha, zahav tahor, sin_madera, etc.) no
 * se "borran" logicamente - la Kaporet sigue siendo Kaporet hasta
 * que deje de existir. Lo que limpiamos es estado VOLATIL (no'adti,
 * testigos, invocacion, ultimo resultado).
 * ================================================================== */
void hashmal_consensus_deinit(void)
{
    if (!g_iniciado) {
        KAP_LOG_W("deinit: Kapóret no estaba levantado (no-op)");
        return;
    }

    /* Alas dejan de cubrir - el no'adti se cierra por desmontaje. */
    g_kaporet.noadti_activo        = false;
    g_kaporet.habitacion_permanente = false;
    g_kaporet.demuestra_justicia    = false;
    g_kaporet.sobre_el_arca         = false;

    /* Testigos: olvido volatil (el pacto inmutable esta en el Arca, no aqui). */
    for (uint8_t i = 0; i < HASHMAL_QUERUBINES_TOTAL; ++i) {
        memset(g_kaporet.querubin[i].hash_testigo, 0,
               sizeof(g_kaporet.querubin[i].hash_testigo));
        g_kaporet.querubin[i].timestamp_testigo = 0u;
    }

    /* Invocacion: sin nube, sin rociados, sin mo'ed. */
    g_kaporet.invocacion.qetoret             = HASHMAL_QETORET_AUSENTE;
    g_kaporet.invocacion.rociados_delante    = 0u;
    g_kaporet.invocacion.rociado_sobre_hecho = false;
    g_kaporet.invocacion.moed_abierto       = false;

    /* Ultimo resultado: limpieza. */
    g_kaporet.ultimo.estado    = HASHMAL_CONSENSO_DIVERGIDO;
    memset(g_kaporet.ultimo.verdad, 0, sizeof(g_kaporet.ultimo.verdad));
    g_kaporet.ultimo.timestamp = 0u;
    g_kaporet.ultimo.emanada   = false;

    g_iniciado = false;
    KAP_LOG_I("Kapóret desmontado (Nm 4:5-15) — alas dejan de cubrir");
}

/* ==================================================================
 * hashmal_consensus_get_kaporet - lectura const
 *
 * Solo lectura. Para modificaciones internas entre archivos hermanos,
 * hay hashmal_consensus_kaporet_mut (internal.h).
 * ================================================================== */
const hashmal_kaporet_t* hashmal_consensus_get_kaporet(void)
{
    if (!g_iniciado) return NULL;
    return &g_kaporet;
}

/* ==================================================================
 * hashmal_consensus_cubrir_arca - Exodo 25:21
 *
 * "venatata et haKaporet 'al ha'aron milma'la"
 * "Pondras el Kaporet SOBRE el Arca, desde ARRIBA."
 *
 * Este es el acto que cierra las medidas "y medio" del Arca (v.10).
 * Precondiciones:
 *   - Kaporet levantado (init ejecutado)
 *   - Arca levantada (hashmal_storage_init ejecutado)
 *   - dimensiones x/y coinciden (misma base 2.5 x 1.5)
 *
 * Efectos:
 *   - g_kaporet.sobre_el_arca = true
 *   - el Arca registra que ya tiene Kaporet encima
 *     (hashmal_storage_kaporet_colocada - requiere_kaporet -> false)
 *
 * Telemetria:
 *   - consulta 1 Rey 8:10-11: si todas las condiciones del kavod
 *     estan, la gloria puede llenar. Solo informa; no decide aqui.
 *
 * 2 testigos:
 *   Ex 25:21    - venatata 'al ha'aron milma'la
 *   1 Rey 8:10-11 - "male kevod YHWH et beit YHWH" (cuando todo
 *                   esta en su lugar, la gloria llena)
 * ================================================================== */
int hashmal_consensus_cubrir_arca(void)
{
    /* a) Kaporet debe existir - no se coloca lo que no existe. */
    if (!g_iniciado) {
        KAP_LOG_E("cubrir_arca: Kapóret no levantado (init pendiente)");
        return HASHMAL_KAPORET_ERR_INIT;
    }

    /* b) Arca debe existir - v.21 dice "'al ha'aron", necesita ha'aron. */
    const hashmal_arca_t *arca = hashmal_storage_get_arca();
    if (arca == NULL) {
        KAP_LOG_E("cubrir_arca: Arca no levantada (v.21 sin sobre-qué-cubrir)");
        return HASHMAL_KAPORET_ERR_SIN_ARCA;
    }

    /* c) Dimensiones x/y deben coincidir. El Padre diseno ambos con
     * la MISMA base 2.5 x 1.5 precisamente para que encaje milma'la. */
    if (arca->largo_x10 != g_kaporet.largo_x10 ||
        arca->ancho_x10 != g_kaporet.ancho_x10) {
        KAP_LOG_E("cubrir_arca: dimensiones no encajan "
                  "(arca %u×%u vs kapóret %u×%u) — v.21 no se cumple",
                  (unsigned)arca->largo_x10, (unsigned)arca->ancho_x10,
                  (unsigned)g_kaporet.largo_x10, (unsigned)g_kaporet.ancho_x10);
        return HASHMAL_KAPORET_ERR_INIT;
    }

    /* d) Idempotencia. Si ya estaba encima, no-op ruidoso. */
    if (g_kaporet.sobre_el_arca) {
        KAP_LOG_W("cubrir_arca: Kapóret ya colocado milmáʻla (idempotente)");
        return HASHMAL_KAPORET_OK;
    }

    /* e) Registrar la colocacion en el Arca. Encapsulacion: NO
     * tocamos el struct del Arca directamente - le pedimos al
     * guardian del Arca que lo anote. Pasamos nuestras dimensiones
     * x/y para que el storage verifique el footprint match (defensa
     * en profundidad: ya validamos arriba, pero el guardian del Arca
     * mide tambien — Dt 19:15, dos testigos). */
    int rc = hashmal_storage_kaporet_colocada(g_kaporet.largo_x10,
                                              g_kaporet.ancho_x10);
    if (rc != 0) {
        /* Si el Arca no puede registrarlo (p.ej. se desinicializo
         * entre lineas - race teorica), no mentimos marcando
         * sobre_el_arca. Fallar antes de afirmar. */
        KAP_LOG_E("cubrir_arca: el Arca rechazó el registro (rc=%d)", rc);
        return HASHMAL_KAPORET_ERR_SIN_ARCA;
    }

    /* f) Marcar en nuestro lado. */
    g_kaporet.sobre_el_arca = true;

    KAP_LOG_I("═══ 'venatáta et haKapóret ʻAL haʼarón MILMÁʻLA' (Éx 25:21) ═══");
    KAP_LOG_I("  base 2.5×1.5 encaja — medidas 'y medio' del Arca cerradas");

    /* g) Telemetria del kavod (1 Rey 8:10-11). No decidimos; consultamos. */
    if (hashmal_storage_kavod_lleno()) {
        KAP_LOG_I("  kavod YHWH llena el recinto (1 Rey 8:10-11)");
    } else {
        KAP_LOG_W("  kavod aún no llena — faltan condiciones "
                  "(edut sellada + zer con contenido)");
    }

    return HASHMAL_KAPORET_OK;
}

/* ==================================================================
 * API INTERNA (hashmal_consensus_internal.h) - acceso mutable
 *
 * Los archivos hermanos (kaporet_validadores, kaporet_rendezvous,
 * kaporet_ataques, kaporet_verificacion) pasan por aqui para
 * escribir estado. El mundo exterior queda restringido al getter
 * const hashmal_consensus_get_kaporet.
 * ================================================================== */
hashmal_kaporet_t* hashmal_consensus_kaporet_mut(void)
{
    if (!g_iniciado) return NULL;
    return &g_kaporet;
}

/* ==================================================================
 * CONEXION POR SANGRE - Lv 16:14 - recibe desde Altar de Sacrificio
 *
 * "velaqaj middam happar vehizza ve'etsba'o 'al penei haKKAPORET
 *  qedmah, velifnei hakkaporet yazzeh SHEVA pe'amim min haddam
 *  be'etsba'o." - Levitico 16:14
 * "Tomara de la sangre del becerro y ROCIARA con su dedo sobre la
 *  faz del KAPORET hacia el oriente, y DELANTE del kaporet rociara
 *  SIETE VECES de la sangre con su dedo."
 *
 * Segundo testigo - Hebreos 9:7:
 * "ou choris haimatos ho prosferei hyper heautou kai ton tou laou
 *  agnoematon."
 * "No SIN SANGRE, la cual ofrece por si y por los pecados del
 *  pueblo."
 *
 * Tavnit pieza 15 (CONEXIONES): "Altar de Sacrificio (06) -> Kaporet
 * (02): la sangre del Yom Kippur va del altar al kaporet". El
 * kaporet no se activa sobre nodos cuya sangre no haya llegado
 * desde el altar - sin sangre no hay acceso al Qodesh haQodashim.
 * ================================================================== */

/* TODO - Phase 2: hashmal_consensus recibe el token del altar.
 *
 * Cuando Phase 2 incluya los headers cruzados, este modulo
 * consultara el token de verificacion emitido por hashmal_auth
 * (pieza 06). Solo nodos con verificacion completa - SHEVA
 * PE'AMIM (7 puntos, Lv 16:14) - participan en el consenso dual
 * de los querubines (V1 <-> V2).
 *
 * La sangre llega "'al penei hakkaporet QEDMAH" - sobre la FAZ
 * del kaporet hacia el ORIENTE. Significado operativo: la
 * verificacion se presenta ANTE el punto de consenso, no DENTRO
 * de el. El kaporet no valida identidad - recibe sangre ya
 * validada por el altar y la aplica al encuentro (no'adti,
 * Ex 25:22). La verificacion identitaria queda fuera; el consenso
 * permanece puro (miqsha, Ex 25:18).
 *
 * Heb 9:7 refuerza en libro distinto: "ou choris haimatos" - no
 * sin sangre. Un nodo sin token del altar no puede ser admitido,
 * aunque su mensaje parezca valido. La regla viene del Padre, no
 * del protocolo.
 * ================================================================== */
