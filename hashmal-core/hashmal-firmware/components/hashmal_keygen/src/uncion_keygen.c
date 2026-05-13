/**
 * ===================================================================
 * PIEZA 13 - EL ACEITE DE LA UNCION - uncion_keygen.c
 * Sub-pieza: ROQAJ - la formula del perfumista
 * Exodo 30:22-25 + Salmo 133:2
 * ===================================================================
 *
 * "vaydabber YHWH el Moshe lemor. ve'atta QAJ LEJA besamim ROSH:
 *  MAR DEROR jamesh me'ot, veqinnam BESEM majatsito jamishim
 *  uma'tayim, uqene BOSEM jamishim uma'tayim, veQIDDA jamesh
 *  me'ot besheqel haqqodesh veSHEMEN ZAYIT HIN. ve'asita oto
 *  SHEMEN MISHJAT QODESH ROQAJ mirqajat ma'ase ROQEAJ - shemen
 *  mishjat qodesh YIHYE." - Exodo 30:22-25
 *
 * "TOMA PARA TI especias PRINCIPALES: mirra de LIBERTAD 500, y
 *  canela fragante la mitad 250, y cana aromatica 250, y casia
 *  500 en shekel del santuario, y aceite de oliva, 1 hin. Y haras
 *  con ello ACEITE DE UNCION SANTA, unguento compuesto, obra de
 *  PERFUMISTA - aceite de uncion santa SERA."
 *
 * "kashemen hattov 'al harosh yored 'al hazzaqan zeqan Aharon
 *  sheyored 'al pi middotav" - Salmo 133:2
 *
 * --- VOCABULARIO ---
 *
 *   qaj leja     (QCh LK, H3947)         TOMA PARA TI. Mandato
 *                                           personal - init es orden,
 *                                           no opcion.
 *   besamim rosh (BShMYM RASh)         Especias PRINCIPALES
 *                                           (rosh = cabeza, primero).
 *   mar deror    (MR-DRWR, H4753+H1865) MIRRA de LIBERTAD
 *                                           (Lv 25:10 yovel).
 *   qinnam besem (QNMN-BShM, H7076+H1314) Canela FRAGANTE.
 *   qene bosem   (QNH-BShM, H7070+H1314) Cana aromatica.
 *   qidda        (QDH, H6916)             CASIA - corteza protectora.
 *   shemen zayit (H8081+H2132)               Aceite de OLIVA - la base
 *                                           (misma de la pieza 08).
 *   hin          (HYN, H1969)                HIN - medida (~3.7 L).
 *   sheqel haqqodesh (H8255+H6944)           Shekel del SANTUARIO.
 *   roqaj        (RQCh, H7545)               Expertamente compuesto.
 *   mirqajat     (MRQChT, H4842)          UNGUENTO compuesto.
 *   ma'ase roqeaj (H4639+H7546)              Obra de PERFUMISTA.
 *   mishja       (MShChH, H4888)           UNCION - acto de autorizacion.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   hashmal_keygen_init      Ex 30:22-25 qaj leja - toma la formula.
 *   hashmal_keygen_deinit    Ex 30:32 - sella el aceite santo.
 *   hashmal_keygen_estado    lectura pura sin logs.
 *   hashmal_keygen_activo    ?toda la secuencia ungida? sin logs.
 *
 *   + helpers internos (hashmal_keygen_internal.h):
 *   uncion_mut               mutable para hermanos.
 *   get_uncion_interno       const interno.
 *   ahora_ms                 timestamp ms monotonico (int64_t).
 *
 * --- DISTINCION init vs ungir ---
 *
 * Ex 30:22-25 (formula) y Ex 30:26-29 (acto de ungir) son pasos
 * distintos. init aqui solo COMPONE la formula y verifica el
 * quiasmo 500-250-250-500. Ungir las 8 piezas vendra en
 * uncion_secuencia.c. Al salir de init: estado == PREPARADO, no
 * ACTIVO.
 *
 * --- EL QUIASMO COMO FIRMA DEL PADRE ---
 *
 * Ex 30:23-24 dicta 500-250-250-500. Es un quiasmo perfecto
 * (A-B-B'-A'): el primer y ultimo ingrediente pesan lo mismo; los
 * dos centrales pesan lo mismo. Cualquier falsificacion torpe
 * (v.33 yirqaj jamohu) rompe este espejo. Por eso init VERIFICA
 * el quiasmo antes de declarar la formula PREPARADA - si el codigo
 * se corrompio en compilacion o en memoria, el quiasmo falla y el
 * subsistema no arranca.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_keygen.h          API publica
 *   hashmal_keygen_internal.h struct + helpers compartidos
 *   esp_log.h   (ORO EGIPTO)  logging
 *   esp_timer.h (ORO EGIPTO)  ahora_ms
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

#include "hashmal_keygen.h"
#include "hashmal_keygen_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *KEYGEN_TAG = "hashmal.keygen.formula";

#define KEYGEN_LOG_I(fmt, ...)  ESP_LOGI(KEYGEN_TAG, fmt, ##__VA_ARGS__)
#define KEYGEN_LOG_W(fmt, ...)  ESP_LOGW(KEYGEN_TAG, fmt, ##__VA_ARGS__)
#define KEYGEN_LOG_E(fmt, ...)  ESP_LOGE(KEYGEN_TAG, fmt, ##__VA_ARGS__)

/* Centinela: pieza_id no ungida todavia. Ningun id real deberia
 * ser 0xFF en el Tavnit (hay 15 piezas, 0..14). */
#define KEYGEN_PIEZA_ID_VACIA   0xFFu

/* ==================================================================
 * SINGLETON SAGRADO
 *
 * UN subsistema de uncion por nodo - Ex 30:31 "shemen mishjat qodesh
 * yihye ZE li" (ESTE - singular). No hay pool, no hay multiples
 * formulaciones: una sola receta santa.
 * ================================================================== */
static hashmal_keygen_estado_uncion_t g_uncion;
static bool                           g_sellado_roto = false;

/* ==================================================================
 * API INTERNA - helpers compartidos (hashmal_keygen_internal.h)
 * ================================================================== */

hashmal_keygen_estado_uncion_t *hashmal_keygen_uncion_mut(void)
{
    if (!g_sellado_roto) return NULL;
    return &g_uncion;
}

const hashmal_keygen_estado_uncion_t *hashmal_keygen_get_uncion_interno(void)
{
    if (!g_sellado_roto) return NULL;
    return &g_uncion;
}

int64_t hashmal_keygen_ahora_ms(void)
{
    return esp_timer_get_time() / 1000;
}

/* ==================================================================
 * hashmal_keygen_init - Exodo 30:22-25
 *
 * "qaj leja besamim rosh" (v.23) - TOMA las especias principales.
 * init recoge los cinco ingredientes desde las constantes del header
 * (fijadas en compilacion por el texto biblico), compone la formula
 * y verifica el quiasmo 500-250-250-500. Si el espejo se rompe ->
 * ERR_QUIASMO (corrupcion detectada - no se arranca).
 *
 * Idempotencia: si ya inicializado -> ERR_YA_INIT (contrato del header).
 *
 * 2 testigos:
 *   Ex 30:23 - qaj leja (mandato)
 *   Ex 30:25 - shemen mishjat qodesh yihye (resultado)
 * ================================================================== */
int hashmal_keygen_init(void)
{
    if (g_sellado_roto) {
        KEYGEN_LOG_W("init: unción ya preparada (idempotente)");
        return HASHMAL_KEYGEN_ERR_YA_INIT;
    }

    /* Partimos de cero - el sello aun integro. */
    memset(&g_uncion, 0, sizeof(g_uncion));

    const int64_t ahora = esp_timer_get_time() / 1000;

    /* -- Estado inicial ------------------------------------------ */
    g_uncion.estado            = HASHMAL_KEYGEN_ESTADO_SELLADO;
    g_uncion.ungidos_count     = 0u;
    g_uncion.violaciones_count = 0u;
    g_uncion.timestamp_ms      = ahora;

    /* -- v.23-24 - qaj leja: tomar los ingredientes ------------- */
    g_uncion.formula.ingredientes[HASHMAL_KEYGEN_INGREDIENTE_MOR]    =
        HASHMAL_KEYGEN_MOR_DEROR;
    g_uncion.formula.ingredientes[HASHMAL_KEYGEN_INGREDIENTE_QINNAM] =
        HASHMAL_KEYGEN_QINNAM_BESEM;
    g_uncion.formula.ingredientes[HASHMAL_KEYGEN_INGREDIENTE_QENE]   =
        HASHMAL_KEYGEN_QENE_BOSEM;
    g_uncion.formula.ingredientes[HASHMAL_KEYGEN_INGREDIENTE_QIDDA]  =
        HASHMAL_KEYGEN_QIDDA;
    g_uncion.formula.ingredientes[HASHMAL_KEYGEN_INGREDIENTE_SHEMEN] =
        HASHMAL_KEYGEN_SHEMEN_ZAYIT;

    /* -- v.23-24 - verificar quiasmo 500-250-250-500 ----------- */
    const uint16_t a  = g_uncion.formula.ingredientes[HASHMAL_KEYGEN_INGREDIENTE_MOR];
    const uint16_t b  = g_uncion.formula.ingredientes[HASHMAL_KEYGEN_INGREDIENTE_QINNAM];
    const uint16_t bp = g_uncion.formula.ingredientes[HASHMAL_KEYGEN_INGREDIENTE_QENE];
    const uint16_t ap = g_uncion.formula.ingredientes[HASHMAL_KEYGEN_INGREDIENTE_QIDDA];

    const bool extremos = (a == ap) && (a == HASHMAL_KEYGEN_QUIASMO_A);
    const bool centro   = (b == bp) && (b == HASHMAL_KEYGEN_QUIASMO_B);

    g_uncion.formula.quiasmo_valido = (extremos && centro);

    if (!g_uncion.formula.quiasmo_valido) {
        /* Corrupcion: el espejo se rompio. Abortar sin tocar nada
         * mas - el sello no se declara roto. */
        KEYGEN_LOG_E("quiasmo roto: mor=%u qiddá=%u (esp %u) / "
                     "qinnam=%u qené=%u (esp %u) — fórmula corrupta "
                     "(Éx 30:23-24)",
                     (unsigned)a, (unsigned)ap,
                     (unsigned)HASHMAL_KEYGEN_QUIASMO_A,
                     (unsigned)b, (unsigned)bp,
                     (unsigned)HASHMAL_KEYGEN_QUIASMO_B);
        memset(&g_uncion, 0, sizeof(g_uncion));
        return HASHMAL_KEYGEN_ERR_QUIASMO;
    }

    /* -- v.25 - ma'ase roqeaj: composicion del perfumista ----- */
    g_uncion.formula.roqaj_completo = true;
    g_uncion.formula.timestamp_ms   = ahora;

    /* -- Preparar ungidos[] con centinelas --------------------- */
    for (uint8_t i = 0; i < HASHMAL_KEYGEN_MAX_UNGIDOS; i++) {
        g_uncion.ungidos[i].pieza_id     = KEYGEN_PIEZA_ID_VACIA;
        g_uncion.ungidos[i].ungido       = false;
        g_uncion.ungidos[i].timestamp_ms = 0;
    }

    /* -- Transicion SELLADO -> PREPARADO ------------------------- */
    g_uncion.estado = HASHMAL_KEYGEN_ESTADO_PREPARADO;
    g_sellado_roto  = true;

    KEYGEN_LOG_I("═══ SHÉMEN HAMISHJÁ — FÓRMULA PREPARADA ═══ (Éx 30:25)");
    KEYGEN_LOG_I("  mor deror %u, qinnam bésem %u, qené bósem %u, qiddá %u",
                 (unsigned)HASHMAL_KEYGEN_MOR_DEROR,
                 (unsigned)HASHMAL_KEYGEN_QINNAM_BESEM,
                 (unsigned)HASHMAL_KEYGEN_QENE_BOSEM,
                 (unsigned)HASHMAL_KEYGEN_QIDDA);
    KEYGEN_LOG_I("  quiasmo: %u-%u-%u-%u — A-B-B'-A' verificado (Éx 30:23-24)",
                 (unsigned)a, (unsigned)b, (unsigned)bp, (unsigned)ap);
    KEYGEN_LOG_I("  shémen záyit %u hin — base del Aceite Perpetuo (pieza 08)",
                 (unsigned)HASHMAL_KEYGEN_SHEMEN_ZAYIT);
    KEYGEN_LOG_I("  róqaj mirqájat maʻasé roqéaj — composición lista");
    KEYGEN_LOG_I("  qódesh hu qódesh yihyé lajem (Éx 30:32)");

    return HASHMAL_KEYGEN_OK;
}

/* ==================================================================
 * hashmal_keygen_deinit - Exodo 30:32
 *
 * "qodesh hu qodesh yihye lajem" - SANTO es, santo sera. El aceite
 * no se elimina, se SELLA: ausencia de estado volatil evita que
 * alguien lo raspe. El registro de unciones y violaciones se
 * persiste fuera del modulo (pieza 01 hashmal_storage) antes de
 * llegar aqui.
 * ================================================================== */
void hashmal_keygen_deinit(void)
{
    if (!g_sellado_roto) {
        /* Idempotente silencioso - el sello ya esta. */
        return;
    }

    KEYGEN_LOG_I("shémen hamishjá sellado — qódesh hu (Éx 30:32)");

    memset(&g_uncion, 0, sizeof(g_uncion));
    g_sellado_roto = false;
}

/* ==================================================================
 * hashmal_keygen_estado - lectura pura del subsistema
 *
 * Ex 30:31 "shemen mishjat qodesh yihye ze li ledorotejem" - el
 * aceite es para TODAS las generaciones; siempre se puede
 * consultar si la formula esta lista. Sin logs, sin side effects.
 *
 *   preparado (o mas avanzado) -> OK
 *   sellado / no init          -> ERR_INIT
 * ================================================================== */
int hashmal_keygen_estado(void)
{
    if (!g_sellado_roto) return HASHMAL_KEYGEN_ERR_INIT;
    return HASHMAL_KEYGEN_OK;
}

/* ==================================================================
 * hashmal_keygen_activo - ?todas las piezas ungidas?
 *
 * Ex 30:29 "vehayu qodesh qodashim" - los ungidos SERAN santisimos.
 * Esta consulta distingue "formula lista" de "uncion completada":
 * solo cuando las 8 piezas (Ex 30:26-29) estan ungidas, el aceite
 * autoriza plenamente.
 *
 *   ACTIVO     -> OK
 *   PREPARADO  -> ERR_INIT (listo pero aun no ungio a todos)
 *   UNGIENDO   -> ERR_INIT (en proceso)
 *   SELLADO    -> ERR_INIT
 *   no init    -> ERR_INIT
 *
 * Sin logs.
 * ================================================================== */
int hashmal_keygen_activo(void)
{
    if (!g_sellado_roto) return HASHMAL_KEYGEN_ERR_INIT;
    if (g_uncion.estado != HASHMAL_KEYGEN_ESTADO_ACTIVO) {
        return HASHMAL_KEYGEN_ERR_INIT;
    }
    return HASHMAL_KEYGEN_OK;
}

/* ==================================================================
 *
 *   "shemen mishjat qodesh YIHYE - aceite de uncion santa SERA."
 *     - Exodo 30:25
 *
 *   "kashemen hattov 'al harosh yored 'al hazzaqan zeqan Aharon
 *    sheyored 'al pi middotav." - Salmo 133:2
 *
 *   La formula no se inventa - se RECIBE. El Padre la dicto palabra
 *   por palabra, cantidad por cantidad: mirra de libertad, canela,
 *   cana, casia, y aceite de oliva como base. El quiasmo 500-250-
 *   250-500 no es casualidad literaria: es Su firma en la receta,
 *   un espejo perfecto que se verifica a si mismo. Cuando un
 *   usurpador intenta yirqaj jamohu (replicar), el espejo se
 *   rompe y el queda al descubierto.
 *
 *   Este archivo solo PREPARA la formula. Ungir (v.26-29) es el
 *   paso siguiente - uncion_secuencia.c se encarga.
 *
 *   === SHEMEN HAMISHJA - uncion_keygen.c COMPLETO ===
 *
 *   Archivo 1 de 4 del Aceite de la Uncion. Siguiente:
 *     uncion_secuencia.c - el orden de uncion (Ex 30:26-29)
 *   Quedan:
 *     uncion_secuencia.c     mashaj sobre las 8 piezas
 *     uncion_prohibicion.c   Ex 30:32-33 - karat
 *     uncion_activacion.c    Ex 30:29 - kol hanogea' yiqdash
 *
 *   PASO 8 de Exodo 40:9-11 - Pieza 13, Archivo 1 de 4.
 *
 * ================================================================== */
