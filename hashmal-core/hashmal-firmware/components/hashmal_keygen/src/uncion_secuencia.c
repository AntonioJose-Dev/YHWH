/**
 * ===================================================================
 * PIEZA 13 - EL ACEITE DE LA UNCION - uncion_secuencia.c
 * Sub-pieza: MASHAJ - el orden de uncion de las 8 piezas
 * Exodo 30:26-29 + Exodo 40:9-11 + Salmo 133:2
 * ===================================================================
 *
 * "UMASHAJTA bo et OHEL MO'ED ve'et ARON HA'EDUT. ve'et HASHULJAN
 *  ve'et kol kelav ve'et HAMMENORA ve'et keleha ve'et MIZBAJ
 *  HAQQETORET. ve'et MIZBAJ HA'OLA ve'et kol kelav ve'et HAKKIYOR
 *  ve'et KANNO. veqiddashta otam vehayu QODESH QODASHIM - kol
 *  hanogea' bahem YIQDASH." - Exodo 30:26-29
 *
 * "Y UNGIRAS con el la TIENDA DEL ENCUENTRO y el ARCA DEL TESTIMONIO;
 *  la MESA y todos sus utensilios, la MENORA y sus utensilios, el
 *  ALTAR DEL INCIENSO; el ALTAR DEL HOLOCAUSTO y todos sus utensilios,
 *  el LAVACRO y su BASE. Los consagraras y seran SANTIDAD DE
 *  SANTIDADES - todo el que los toque se santificara."
 *
 * "velaqajta et shemen hammishja UMASHAJTA et hammishkan ve'et kol
 *  asher bo veqiddashta oto ve'et kol kelav" - Exodo 40:9
 *
 * "kashemen hattov 'al harosh YORED 'al hazzaqan zeqan Aharon
 *  SHEYORED 'al pi middotav" - Salmo 133:2
 *
 * --- VOCABULARIO ---
 *
 *   umashajta       (WMShChT, H4886)   Y UNGIRAS. Waw
 *                                              consecutivo perfecto -
 *                                              orden que se ejecuta.
 *   mashaj          (MShCh, H4886)            UNGIR. Verbo raiz -
 *                                              aplicar el aceite.
 *   ohel mo'ed      (AHL MW'D)              Tienda del Encuentro
 *                                              (pieza 05 framework).
 *   aron ha'edut    (ARWN H'DT)            Arca del Testimonio
 *                                              (pieza 01 storage).
 *   hashuljan       (HShLChN, H7979)       La MESA
 *                                              (pieza 03 cache).
 *   hammenora       (HMNRH, H4501)         La MENORA
 *                                              (pieza 04 comm).
 *   mizbaj haqqetoret (MZBCh HQTRT)      Altar del INCIENSO
 *                                              (pieza 11 intercession).
 *   mizbaj ha'ola   (MZBCh H'LH)          Altar del SACRIFICIO
 *                                              (pieza 06 auth).
 *   hakkiyor        (HKYWR, H3595)         LAVACRO
 *                                              (pieza 12 healthcheck).
 *   kanno           (KNW, H3653)             Su BASE - soporte
 *                                              del kiyor.
 *   qodesh qodashim (QDSh QDShYM)          SANTIDAD DE SANTIDADES
 *                                              - maximo nivel.
 *   kol hanogea'    (KL HNG')              Todo el que TOQUE.
 *   yiqdash         (YQDSh, H6942)          se SANTIFICARA -
 *                                              transferencia.
 *   yored           (YRD, H3381)               DESCIENDE - Sal 133:2,
 *                                              flujo top-down.
 *
 * --- SECUENCIA BIBLICA (Ex 30:26-28) ---
 *
 *   pos  pieza_id  Hebreo                     Componente
 *   ---  --------  ------------------------   ---------------------
 *    0   05        ohel mo'ed                 hashmal_framework
 *    1   01        aron ha'edut               hashmal_storage
 *    2   03        hashuljan                  hashmal_cache
 *    3   04        hammenora                  hashmal_comm
 *    4   11        mizbaj haqqetoret          hashmal_intercession
 *    5   06        mizbaj ha'ola              hashmal_auth
 *    6   12        hakkiyor                   hashmal_healthcheck
 *    7   12        kanno                      hashmal_healthcheck (base)
 *
 *   Total: 8 posiciones, 7 pieza_id distintos (kiyor y kanno
 *   comparten id=12 porque son UNA pieza fisica con dos elementos
 *   declarados en v.28 "ve'et hakkiyor ve'et kanno"). Politica:
 *   al ungir id=12 por primera vez, se ocupan las posiciones 6 y 7
 *   a la vez - una sola llamada, una sola accion (ma'ase ejad).
 *   Por eso el usuario hace 7 llamadas efectivas para completar
 *   las 8 posiciones.
 *
 * --- FLUJO DE LA UNCION (Sal 133:2 yored) ---
 *
 *   El aceite DESCIENDE, dice el salmo: de la cabeza a la barba y
 *   de ahi al borde de las vestiduras. En el Tavnit la uncion fluye
 *   ADENTRO -> AFUERA: primero la Tienda (continente), luego el Arca
 *   (Qodesh haQodashim), luego Mesa/Menora (Qodesh), luego los
 *   altares, finalmente el Kiyor (Jatser). Saltar pasos rompe el
 *   "yored" - el aceite no puede descender desde un lugar que aun
 *   no fue tocado.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   hashmal_keygen_ungir               Ex 30:26-29 mashaj una pieza.
 *   hashmal_keygen_esta_ungido         consulta silente.
 *   hashmal_keygen_secuencia_estado    lectura pura del progreso.
 *
 *   + helpers estaticos:
 *   SECUENCIA_UNCION[8]                la tabla sagrada.
 *   nombre_pieza                       nombre legible para logs.
 *   pieza_en_secuencia                 ?id pertenece a los 8?
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_keygen.h
 *   hashmal_keygen_internal.h
 *   esp_log.h   (ORO EGIPTO)  logging
 *
 * Autor: Antonio Jose Marin Soto (Betsalel - Ex 31:2)
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "esp_log.h"

#include "hashmal_keygen.h"
#include "hashmal_keygen_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *SEC_TAG = "hashmal.keygen.secuencia";

#define SEC_LOG_I(fmt, ...)  ESP_LOGI(SEC_TAG, fmt, ##__VA_ARGS__)
#define SEC_LOG_W(fmt, ...)  ESP_LOGW(SEC_TAG, fmt, ##__VA_ARGS__)
#define SEC_LOG_E(fmt, ...)  ESP_LOGE(SEC_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * SECUENCIA_UNCION - Exodo 30:26-28
 *
 * Tabla inmutable (const) que codifica el orden exacto dictado por
 * el texto. Ningun otro orden es aceptable: cualquier desviacion
 * devuelve ERR_SECUENCIA.
 * ================================================================== */
static const uint8_t SECUENCIA_UNCION[HASHMAL_KEYGEN_MAX_UNGIDOS] = {
    5u,   /* pos 0 - ohel mo'ed           (Ex 30:26, framework) */
    1u,   /* pos 1 - aron ha'edut         (Ex 30:26, storage) */
    3u,   /* pos 2 - hashuljan             (Ex 30:27, cache) */
    4u,   /* pos 3 - hammenora             (Ex 30:27, comm) */
    11u,  /* pos 4 - mizbaj haqqetoret    (Ex 30:27, intercession) */
    6u,   /* pos 5 - mizbaj ha'ola        (Ex 30:28, auth) */
    12u,  /* pos 6 - hakkiyor              (Ex 30:28, healthcheck) */
    12u,  /* pos 7 - kanno                 (Ex 30:28, base del kiyor) */
};

/* ==================================================================
 * HELPERS ESTATICOS
 * ================================================================== */

static const char *nombre_pieza(uint8_t pieza_id)
{
    switch (pieza_id) {
    case 5u:  return "óhel moʻéd (framework)";
    case 1u:  return "arón haʻédut (storage)";
    case 3u:  return "hashulján (cache)";
    case 4u:  return "hammenorá (comm)";
    case 11u: return "mizbáj haqqetóret (intercession)";
    case 6u:  return "mizbáj haʻolá (auth)";
    case 12u: return "hakkiyór veʼét kannó (healthcheck)";
    default:  return "<pieza fuera de la secuencia>";
    }
}

static bool pieza_en_secuencia(uint8_t pieza_id)
{
    for (uint8_t i = 0; i < HASHMAL_KEYGEN_MAX_UNGIDOS; i++) {
        if (SECUENCIA_UNCION[i] == pieza_id) return true;
    }
    return false;
}

/**
 * Devuelve la primera posicion NO ungida en ungidos[], o
 * HASHMAL_KEYGEN_MAX_UNGIDOS si todas estan llenas.
 */
static uint8_t proxima_posicion(const hashmal_keygen_estado_uncion_t *u)
{
    for (uint8_t i = 0; i < HASHMAL_KEYGEN_MAX_UNGIDOS; i++) {
        if (!u->ungidos[i].ungido) return i;
    }
    return HASHMAL_KEYGEN_MAX_UNGIDOS;
}

/**
 * ?Alguna posicion de ungidos[] ya registra esta pieza como ungida?
 * Usado para devolver ERR_YA_UNGIDO en llamadas repetidas.
 */
static bool ya_ungido(const hashmal_keygen_estado_uncion_t *u,
                      uint8_t pieza_id)
{
    for (uint8_t i = 0; i < HASHMAL_KEYGEN_MAX_UNGIDOS; i++) {
        if (u->ungidos[i].ungido && u->ungidos[i].pieza_id == pieza_id) {
            return true;
        }
    }
    return false;
}

/* ==================================================================
 * hashmal_keygen_ungir - Exodo 30:26-29  mashaj
 *
 * "umashajta" - unge UN componente, en el orden exacto del texto.
 *
 * Validaciones (en este orden):
 *   1. Subsistema iniciado (uncion_mut disponible).
 *   2. pieza_id pertenece a la secuencia de los 8 -> si no,
 *      ERR_BESAR_ADAM (Ex 30:32 "'al besar adam lo yisaj").
 *   3. pieza_id no fue ungida antes -> si si, ERR_YA_UNGIDO.
 *   4. La secuencia coincide con la proxima posicion -> si no,
 *      ERR_SECUENCIA (Sal 133:2 yored - la uncion desciende en orden).
 *
 * Uncion (side effects):
 *   - ungidos[next_pos] se marca con pieza_id, ungido=true, timestamp.
 *   - ungidos_count++
 *   - Caso especial kiyor+kanno (v.28 "ve'et hakkiyor ve'et kanno"):
 *     al ungir id=12 en pos 6, se unge tambien pos 7 en el mismo acto
 *     (ma'ase ejad). ungidos_count sube 2 esta vez.
 *   - Transiciones:
 *       PREPARADO -> UNGIENDO  (primera uncion)
 *       UNGIENDO  -> ACTIVO    (ungidos_count == MAX_UNGIDOS)
 *
 * 2 testigos:
 *   Ex 30:26-28 - lista exacta de 8 elementos
 *   Ex 40:9     - umashajta et hammishkan ve'et kol asher bo
 * ================================================================== */
int hashmal_keygen_ungir(uint8_t pieza_id)
{
    hashmal_keygen_estado_uncion_t *u = hashmal_keygen_uncion_mut();
    if (u == NULL) {
        SEC_LOG_E("ungir: unción no preparada (init primero)");
        return HASHMAL_KEYGEN_ERR_INIT;
    }

    /* 2. ?pieza_id pertenece a la secuencia? */
    if (!pieza_en_secuencia(pieza_id)) {
        SEC_LOG_W("ungir: pieza %u fuera de la secuencia — "
                  "ʻal besar adam lo yisáj (Éx 30:32)",
                  (unsigned)pieza_id);
        return HASHMAL_KEYGEN_ERR_BESAR_ADAM;
    }

    /* 3. ?ya ungida? */
    if (ya_ungido(u, pieza_id)) {
        SEC_LOG_W("ungir: %s ya ungida (idempotente informativo)",
                  nombre_pieza(pieza_id));
        return HASHMAL_KEYGEN_ERR_YA_UNGIDO;
    }

    /* 4. Secuencia - debe coincidir con la proxima posicion. */
    const uint8_t next_pos = proxima_posicion(u);
    if (next_pos >= HASHMAL_KEYGEN_MAX_UNGIDOS) {
        /* Todas llenas pero ya_ungido dijo no - corrupcion: aborta. */
        SEC_LOG_E("ungir: estructura inconsistente — unciones completas");
        return HASHMAL_KEYGEN_ERR_SECUENCIA;
    }
    if (SECUENCIA_UNCION[next_pos] != pieza_id) {
        SEC_LOG_W("ungir: secuencia rota — se esperaba %s (pos %u), "
                  "se recibió %s (Sal 133:2 yoréd)",
                  nombre_pieza(SECUENCIA_UNCION[next_pos]),
                  (unsigned)next_pos,
                  nombre_pieza(pieza_id));
        return HASHMAL_KEYGEN_ERR_SECUENCIA;
    }

    /* -- Uncion ------------------------------------------------ */
    const int64_t ahora = hashmal_keygen_ahora_ms();

    u->ungidos[next_pos].pieza_id     = pieza_id;
    u->ungidos[next_pos].ungido       = true;
    u->ungidos[next_pos].timestamp_ms = ahora;
    u->ungidos_count++;

    SEC_LOG_I("mashaj pieza %u (%s) — posición %u/%u (Éx 30:26-28)",
              (unsigned)pieza_id, nombre_pieza(pieza_id),
              (unsigned)(next_pos + 1u),
              (unsigned)HASHMAL_KEYGEN_MAX_UNGIDOS);

    /* Caso especial: kiyor+kanno (v.28) - la siguiente posicion de
     * la secuencia tambien es id=12; se unge junta en el mismo acto. */
    if (pieza_id == 12u &&
        (next_pos + 1u) < HASHMAL_KEYGEN_MAX_UNGIDOS &&
        SECUENCIA_UNCION[next_pos + 1u] == 12u &&
        !u->ungidos[next_pos + 1u].ungido) {
        u->ungidos[next_pos + 1u].pieza_id     = pieza_id;
        u->ungidos[next_pos + 1u].ungido       = true;
        u->ungidos[next_pos + 1u].timestamp_ms = ahora;
        u->ungidos_count++;

        SEC_LOG_I("  kannó (base) ungida junto con hakkiyór (Éx 30:28 "
                  "veʼét hakkiyór veʼét kannó — maʻasé ejád)");
    }

    /* -- Transiciones de estado ------------------------------- */
    if (u->estado == HASHMAL_KEYGEN_ESTADO_PREPARADO) {
        u->estado = HASHMAL_KEYGEN_ESTADO_UNGIENDO;
        SEC_LOG_I("estado: PREPARADO → UNGIENDO (primera unción)");
    }

    if (u->ungidos_count >= HASHMAL_KEYGEN_MAX_UNGIDOS) {
        u->estado = HASHMAL_KEYGEN_ESTADO_ACTIVO;
        SEC_LOG_I("═══ QÓDESH QODASHÍM ═══ (Éx 30:29)");
        SEC_LOG_I("  veqiddáshta otám veháyú qódesh qodashím");
        SEC_LOG_I("  kol hanogéaʻ bahém yiqdásh — TODO ungido (%u/%u)",
                  (unsigned)u->ungidos_count,
                  (unsigned)HASHMAL_KEYGEN_MAX_UNGIDOS);
        SEC_LOG_I("  estado: UNGIENDO → ACTIVO");
    }

    u->timestamp_ms = ahora;
    return HASHMAL_KEYGEN_OK;
}

/* ==================================================================
 * hashmal_keygen_esta_ungido - lectura pura
 *
 * Ex 30:29 "kol hanogea' bahem yiqdash" - otros componentes
 * necesitan preguntar antes de actuar. Sin logs, sin side effects.
 *
 *   pieza ungida  -> OK
 *   no ungida     -> ERR_INIT (seguir el convenio de estado no listo)
 *   no iniciado   -> ERR_INIT
 * ================================================================== */
int hashmal_keygen_esta_ungido(uint8_t pieza_id)
{
    const hashmal_keygen_estado_uncion_t *u =
        hashmal_keygen_get_uncion_interno();
    if (u == NULL) return HASHMAL_KEYGEN_ERR_INIT;

    for (uint8_t i = 0; i < HASHMAL_KEYGEN_MAX_UNGIDOS; i++) {
        if (u->ungidos[i].ungido && u->ungidos[i].pieza_id == pieza_id) {
            return HASHMAL_KEYGEN_OK;
        }
    }
    return HASHMAL_KEYGEN_ERR_INIT;
}

/* ==================================================================
 * hashmal_keygen_secuencia_estado - lectura pura del progreso
 *
 * OK si la formula ya esta PREPARADA (o mas avanzada - UNGIENDO,
 * ACTIVO). El llamante inspecciona ungidos_count / estado a traves
 * de sus propios canales. Sin logs.
 * ================================================================== */
int hashmal_keygen_secuencia_estado(void)
{
    const hashmal_keygen_estado_uncion_t *u =
        hashmal_keygen_get_uncion_interno();
    if (u == NULL) return HASHMAL_KEYGEN_ERR_INIT;

    /* estado en {PREPARADO, UNGIENDO, ACTIVO}. SELLADO no deberia
     * ocurrir si sellado_roto=true (init lo saca de SELLADO). */
    if (u->estado < HASHMAL_KEYGEN_ESTADO_PREPARADO) {
        return HASHMAL_KEYGEN_ERR_INIT;
    }
    return HASHMAL_KEYGEN_OK;
}

/* ==================================================================
 *
 *   "veqiddashta otam vehayu QODESH QODASHIM - kol hanogea' bahem
 *    YIQDASH." - Exodo 30:29
 *
 *   "kashemen hattov 'al harosh YORED 'al hazzaqan zeqan Aharon
 *    sheyored 'al pi middotav." - Salmo 133:2
 *
 *   La uncion no salta niveles. Desciende: cabeza, barba,
 *   vestidura. Tienda, Arca, Mesa, Menora, Incienso, Sacrificio,
 *   Lavacro. Ocho posiciones, siete contactos - el kiyor y su
 *   base son una sola accion, ma'ase ejad. Intentar ungir la
 *   Mesa sin haber ungido antes la Tienda y el Arca rompe el
 *   yored; el aceite no fluye hacia arriba. Intentar ungir algo
 *   fuera de la lista es "'al besar adam lo yisaj" (v.32) -
 *   error antes de empezar.
 *
 *   === MASHAJ - uncion_secuencia.c COMPLETO ===
 *
 *   Archivo 2 de 4 del Aceite de la Uncion. Siguiente:
 *     uncion_prohibicion.c - karat (Ex 30:32-33)
 *
 *   PASO 8 de Exodo 40:9-11 - Pieza 13, Archivo 2 de 4.
 *
 * ================================================================== */
