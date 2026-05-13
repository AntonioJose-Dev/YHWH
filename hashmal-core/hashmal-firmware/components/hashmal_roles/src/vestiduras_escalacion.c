/*
 * ===================================================================
 * PIEZA 09 - VESTIDURAS - vestiduras_escalacion.c
 * Añadidura 09: LOS TRES NIVELES DE ESCALACION
 * Mateo 18:15-17 + Deuteronomio 19:15 + Proverbios 25:9
 *                 + Levitico 19:17-18
 * ===================================================================
 *
 * "ean de hamartese eis se ho adelfos sou, hypage ELEGXON auton
 *  METAXY sou kai autou MONOU. ean sou akouse, EKERDESAS ton
 *  adelfon sou." - Mateo 18:15
 * "Si tu hermano peca contra ti, ve y REPRENDELO ENTRE ti y el
 *  SOLO. Si te oye, has GANADO a tu hermano."
 *
 * "ean de me akouse, paralabe meta sou eti ENA e DUO, hina epi
 *  stomatos DUO martyron e TRION stathe pan rhema." - Mateo 18:16
 * "Si no oyere, lleva contigo UNO o DOS mas, para que por boca de
 *  DOS o TRES testigos se establezca toda palabra." (cita Dt 19:15)
 *
 * "ean de parakouse auton, eipe te EKKLESIA; ean de kai tes
 *  ekklesias parakouse, esto soi hosper ho ETHNIKOS kai ho
 *  telones." - Mateo 18:17
 * "Si no los oyere, dilo a la CONGREGACION; y si tampoco a la
 *  congregacion oyere, sea para ti como el GENTIL y el publicano."
 *
 * "'al pi shenayim 'edim o 'al pi shloshah 'edim YAQUM DAVAR."
 *   - Deuteronomio 19:15
 * "Por boca de DOS o TRES testigos se confirmara el asunto."
 *
 * "rivkha riv et re'ekha veSOD aher al tegal." - Proverbios 25:9
 * "Debate con tu projimo, pero NO descubras el secreto de otro."
 *
 * "lo tisna et achikha bilvavekha: HOKHEACH TOKHIACH et 'amitekha
 *  velo tissa 'alav jet." - Levitico 19:17
 * "No aborreceras a tu hermano en tu corazon: REPRENDERAS a tu
 *  projimo, y no cargaras pecado por el."
 *
 * --- EXEGESIS OPERATIVA ---
 *
 *   metaxy monou (G3342+G3441)  "ENTRE SOLO". Mt 18:15 exige
 *                                privacidad inicial. Prov 25:9 lo
 *                                refuerza: el sod (secreto) no se
 *                                publica antes de tiempo.
 *
 *   duo e trion (G1417+G5140)   DOS O TRES. Escala minima de
 *                                verificacion mutua. Dt 19:15 hace
 *                                del numero una REGLA: un testigo
 *                                solo no basta (Nm 35:30 tambien).
 *
 *   ekklesia (G1577)            CONGREGACION. En LXX traduce qahal,
 *                                la asamblea reunida. Ultimo paso
 *                                antes del corte.
 *
 *   ethnikos kai telones (G1482+G5057) GENTIL Y RECAUDADOR -
 *                                exiliados del pacto de Yisrael.
 *                                Equivalente operativo: nikrat
 *                                (H3772, cortado), ya usado en Lv
 *                                17:4 y Nm 15:30.
 *
 * --- MONOTONIA ---
 *
 * Mt 18 establece un orden. No puedes empezar por ekklesia y bajar
 * a solo; el secreto ya se rompio. Tampoco puedes "bajar" de
 * testigos a solo; los testigos ya escucharon. Por eso la tabla
 * solo permite AVANZAR. La unica "salida" es RESUELTO (desde
 * cualquier nivel pre-NIKRAT) o NIKRAT (terminal).
 *
 * --- DIFERENCIA CON JATSOTSROT ---
 *
 *   Jatsotsrot (Añadidura 08) es el CANAL (tipo de toque). Aqui es
 *   el PROTOCOLO (secuencia de pasos). Una disputa escalada a
 *   EKKLESIA USA jatsotsrot TEQIAH_SHETAYIM como vehiculo de
 *   broadcast. Canal distinto de protocolo.
 *
 * --- NIKRAT SIN FIREWALL WIRING (fase 1) ---
 *
 * Spec pidio que nikrat sea "marca estado" en fase 1. La
 * ejecucion real del corte (desautorizar al nodo en Pieza 14
 * firewall) requiere una API de "expel peer" que el firewall
 * actual no expone. Fase 2: wire firewall.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_roles.h            API publica + jatsotsrot + rol estado
 *   hashmal_roles_internal.h   ahora_ms
 *   esp_log.h (ORO EGIPTO)     ESC_LOG_*
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <inttypes.h>

#include "esp_log.h"

#include "hashmal_roles.h"
#include "hashmal_roles_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *ESC_TAG = "hashmal.roles.escalacion";

#define ESC_LOG_I(fmt, ...)  ESP_LOGI(ESC_TAG, fmt, ##__VA_ARGS__)
#define ESC_LOG_W(fmt, ...)  ESP_LOGW(ESC_TAG, fmt, ##__VA_ARGS__)
#define ESC_LOG_E(fmt, ...)  ESP_LOGE(ESC_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * Tabla de disputas
 *
 * Slot libre: estado == VACIO. Otros estados ocupan el slot (incl.
 * RESUELTO - permite re-abrir con el mismo id para transparencia
 * de audit fase 2). NIKRAT es permanente: el slot no se reutiliza
 * hasta que un operador llame a un futuro _purgar.
 * ================================================================== */
static hashmal_escalacion_disputa_t g_disputas[HASHMAL_ESCALACION_DISPUTAS_MAX];

/* ==================================================================
 * Helpers estaticos
 * ================================================================== */

static int buscar_por_id(uint32_t disputa_id)
{
    for (uint8_t i = 0; i < HASHMAL_ESCALACION_DISPUTAS_MAX; ++i) {
        if (g_disputas[i].estado == HASHMAL_ESCALACION_ESTADO_VACIO) continue;
        if (g_disputas[i].disputa_id == disputa_id) return (int)i;
    }
    return -1;
}

static int buscar_slot_libre(void)
{
    /* Prefiere VACIO (nunca usado) sobre RESUELTO (reusable) para
     * preservar el snapshot resuelto un poco mas. Si no hay VACIO,
     * busca RESUELTO. NIKRAT nunca se reutiliza. */
    for (uint8_t i = 0; i < HASHMAL_ESCALACION_DISPUTAS_MAX; ++i) {
        if (g_disputas[i].estado == HASHMAL_ESCALACION_ESTADO_VACIO) return (int)i;
    }
    for (uint8_t i = 0; i < HASHMAL_ESCALACION_DISPUTAS_MAX; ++i) {
        if (g_disputas[i].estado == HASHMAL_ESCALACION_ESTADO_RESUELTO) return (int)i;
    }
    return -1;
}

static const char *nombre_nivel(hashmal_escalacion_nivel_t n)
{
    switch (n) {
    case HASHMAL_ESCALACION_NIVEL_SOLO:     return "SOLO (Mt 18:15 metaxy monou)";
    case HASHMAL_ESCALACION_NIVEL_TESTIGOS: return "TESTIGOS (Mt 18:16 duo e trion)";
    case HASHMAL_ESCALACION_NIVEL_EKKLESIA: return "EKKLESIA (Mt 18:17a)";
    case HASHMAL_ESCALACION_NIVEL_NIKRAT:   return "NIKRAT (Mt 18:17b ho ethnikos)";
    default:                                 return "<nivel desconocido>";
    }
}

/* Nivel 1 (SOLO): simulado en fase 1 - log del mensaje directo. Prov
 * 25:9: el contenido exacto no se publica; solo se registra que
 * existio una reprension privada. Fase 2 wireara unicast por MAC. */
static void simular_mensaje_solo(const hashmal_escalacion_disputa_t *d)
{
    ESC_LOG_I("SOLO simulado (Mt 18:15 metaxy monou) - disputa %" PRIu32
              " acusador=%u acusado=%u - contenido privado, no publicado "
              "(Prov 25:9 vesod aher al tegal)",
              d->disputa_id,
              (unsigned)d->acusador_nodo_id,
              (unsigned)d->acusado_nodo_id);
}

/* Nivel 2 (TESTIGOS): simulado en fase 1 - log del grupo. Fase 2
 * wireara multicast a 2-3 nodos vecinos. */
static void simular_mensaje_testigos(const hashmal_escalacion_disputa_t *d)
{
    ESC_LOG_I("TESTIGOS simulado (Mt 18:16 duo e trion, Dt 19:15) - "
              "disputa %" PRIu32 " promovida; testigos escuchan "
              "ambas versiones (fase 2: multicast)",
              d->disputa_id);
}

/* Nivel 3 (EKKLESIA): dispara jatsotsrot TEQIAH_SHETAYIM real.
 * Payload minimo: disputa_id en 4 bytes big-endian. Retorna el rc
 * de jatsotsrot (OK si broadcast salio). */
static hashmal_roles_err_t
disparar_ekklesia(const hashmal_escalacion_disputa_t *d)
{
    uint8_t payload[4];
    payload[0] = (uint8_t)((d->disputa_id >> 24) & 0xFFu);
    payload[1] = (uint8_t)((d->disputa_id >> 16) & 0xFFu);
    payload[2] = (uint8_t)((d->disputa_id >>  8) & 0xFFu);
    payload[3] = (uint8_t)((d->disputa_id      ) & 0xFFu);

    hashmal_roles_err_t rc = hashmal_jatsotsrot_tocar(
        HASHMAL_JATSOTSROT_TEQIAH_SHETAYIM,
        d->acusador_nodo_id,
        payload, sizeof(payload));

    if (rc == HASHMAL_ROLES_OK) {
        ESC_LOG_I("EKKLESIA - jatsotsrot TEQIAH_SHETAYIM disparada "
                  "(Mt 18:17a eipe te ekklesia) para disputa %" PRIu32,
                  d->disputa_id);
    } else {
        ESC_LOG_W("EKKLESIA - jatsotsrot fallo (rc=%d): acusador %u "
                  "quiza sin autoridad Kohen+ (Nm 10:8)",
                  (int)rc, (unsigned)d->acusador_nodo_id);
    }
    return rc;
}

/* ==================================================================
 * API - hashmal_escalacion_abrir (Mt 18:15)
 * ================================================================== */
hashmal_roles_err_t hashmal_escalacion_abrir(uint32_t disputa_id,
                                              uint8_t acusador_nodo_id,
                                              uint8_t acusado_nodo_id)
{
    if (acusador_nodo_id >= HASHMAL_ROLES_MAX_NODOS
     || acusado_nodo_id  >= HASHMAL_ROLES_MAX_NODOS) {
        ESC_LOG_E("abrir: nodo_id fuera de [0, %u)",
                  (unsigned)HASHMAL_ROLES_MAX_NODOS);
        return HASHMAL_ROLES_ERR_NODO_NO_ENCONTRADO;
    }
    if (acusador_nodo_id == acusado_nodo_id) {
        ESC_LOG_E("abrir: acusador y acusado iguales (%u) - "
                  "Mt 18:15 requiere hermano distinto",
                  (unsigned)acusador_nodo_id);
        return HASHMAL_ROLES_ERR_ESCALACION;
    }

    /* Lv 19:17 - reprehension es mandato al acusador. Verificamos que
     * el id no esta ya abierto para evitar duplicados - cada disputa
     * tiene un solo slot activo. */
    int existente = buscar_por_id(disputa_id);
    if (existente >= 0) {
        hashmal_escalacion_estado_t est = g_disputas[existente].estado;
        if (est == HASHMAL_ESCALACION_ESTADO_ACTIVO
         || est == HASHMAL_ESCALACION_ESTADO_NIKRAT) {
            ESC_LOG_E("abrir: disputa %" PRIu32 " ya existe (estado=%d)",
                      disputa_id, (int)est);
            return HASHMAL_ROLES_ERR_ESCALACION;
        }
        /* RESUELTO - re-abrimos en el mismo slot. */
    }

    int slot = (existente >= 0) ? existente : buscar_slot_libre();
    if (slot < 0) {
        ESC_LOG_E("abrir: tabla llena (%u slots; NIKRAT no se reutilizan)",
                  (unsigned)HASHMAL_ESCALACION_DISPUTAS_MAX);
        return HASHMAL_ROLES_ERR_ESCALACION;
    }

    int64_t ahora = hashmal_roles_ahora_ms();
    g_disputas[slot].disputa_id                       = disputa_id;
    g_disputas[slot].acusador_nodo_id                 = acusador_nodo_id;
    g_disputas[slot].acusado_nodo_id                  = acusado_nodo_id;
    g_disputas[slot].nivel                            = HASHMAL_ESCALACION_NIVEL_SOLO;
    g_disputas[slot].estado                           = HASHMAL_ESCALACION_ESTADO_ACTIVO;
    g_disputas[slot].timestamp_abierto_ms             = ahora;
    g_disputas[slot].timestamp_ultima_escalacion_ms   = ahora;

    simular_mensaje_solo(&g_disputas[slot]);
    ESC_LOG_I("abrir: disputa %" PRIu32 " slot=%d nivel=%s "
              "(Lv 19:17 hokheach tokhiach)",
              disputa_id, slot, nombre_nivel(HASHMAL_ESCALACION_NIVEL_SOLO));
    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 * API - hashmal_escalacion_escalar (Mt 18:15b-17)
 * ================================================================== */
hashmal_roles_err_t hashmal_escalacion_escalar(uint32_t disputa_id)
{
    int idx = buscar_por_id(disputa_id);
    if (idx < 0) {
        ESC_LOG_E("escalar: disputa %" PRIu32 " no encontrada", disputa_id);
        return HASHMAL_ROLES_ERR_ESCALACION;
    }

    hashmal_escalacion_disputa_t *d = &g_disputas[idx];

    if (d->estado == HASHMAL_ESCALACION_ESTADO_NIKRAT) {
        ESC_LOG_W("escalar: disputa %" PRIu32 " ya NIKRAT (terminal)",
                  disputa_id);
        return HASHMAL_ROLES_ERR_ESCALACION;
    }
    if (d->estado == HASHMAL_ESCALACION_ESTADO_RESUELTO) {
        ESC_LOG_W("escalar: disputa %" PRIu32 " ya RESUELTA - re-abrir "
                  "con _abrir si se necesita otra disputa", disputa_id);
        return HASHMAL_ROLES_ERR_ESCALACION;
    }

    /* Monotonico: SOLO -> TESTIGOS -> EKKLESIA -> NIKRAT. */
    hashmal_escalacion_nivel_t nivel_previo = d->nivel;
    hashmal_roles_err_t rc_dispatch = HASHMAL_ROLES_OK;

    switch (d->nivel) {
    case HASHMAL_ESCALACION_NIVEL_SOLO:
        d->nivel = HASHMAL_ESCALACION_NIVEL_TESTIGOS;
        simular_mensaje_testigos(d);
        break;

    case HASHMAL_ESCALACION_NIVEL_TESTIGOS:
        d->nivel = HASHMAL_ESCALACION_NIVEL_EKKLESIA;
        /* Dispatch real: jatsotsrot TEQIAH_SHETAYIM. El estado
         * avanza AUNQUE el dispatch falle - la escalacion como
         * acto ya ocurrio; el caller vera rc != OK si el broadcast
         * no salio. */
        rc_dispatch = disparar_ekklesia(d);
        break;

    case HASHMAL_ESCALACION_NIVEL_EKKLESIA:
        /* Mt 18:17b "esto soi hosper ho ETHNIKOS" - el corte. */
        d->nivel  = HASHMAL_ESCALACION_NIVEL_NIKRAT;
        d->estado = HASHMAL_ESCALACION_ESTADO_NIKRAT;
        ESC_LOG_E("NIKRAT - disputa %" PRIu32 " acusado=%u cortado "
                  "(Mt 18:17b hosper ho ethnikos); fase 1 solo marca "
                  "estado - Pieza 14 firewall wiring en PR futuro",
                  disputa_id, (unsigned)d->acusado_nodo_id);
        break;

    case HASHMAL_ESCALACION_NIVEL_NIKRAT:
    default:
        /* Ya manejado arriba - defensivo. */
        return HASHMAL_ROLES_ERR_ESCALACION;
    }

    d->timestamp_ultima_escalacion_ms = hashmal_roles_ahora_ms();

    ESC_LOG_I("escalar: disputa %" PRIu32 " %s -> %s",
              disputa_id, nombre_nivel(nivel_previo),
              nombre_nivel(d->nivel));

    return rc_dispatch;
}

/* ==================================================================
 * API - hashmal_escalacion_resolver (Mt 18:15b ekerdesas ton adelfon)
 * ================================================================== */
hashmal_roles_err_t hashmal_escalacion_resolver(uint32_t disputa_id)
{
    int idx = buscar_por_id(disputa_id);
    if (idx < 0) {
        return HASHMAL_ROLES_ERR_ESCALACION;
    }

    hashmal_escalacion_disputa_t *d = &g_disputas[idx];

    if (d->estado == HASHMAL_ESCALACION_ESTADO_NIKRAT) {
        /* Mt 18:17b es terminal - el nikrat no se revoca. */
        ESC_LOG_E("resolver: disputa %" PRIu32 " esta NIKRAT - terminal, "
                  "no se puede resolver (Mt 18:17b)", disputa_id);
        return HASHMAL_ROLES_ERR_ESCALACION;
    }
    if (d->estado == HASHMAL_ESCALACION_ESTADO_RESUELTO) {
        /* Idempotente. */
        return HASHMAL_ROLES_OK;
    }

    d->estado = HASHMAL_ESCALACION_ESTADO_RESUELTO;

    ESC_LOG_I("resolver: disputa %" PRIu32 " RESUELTA en nivel %s "
              "(Mt 18:15b ekerdesas ton adelfon)",
              disputa_id, nombre_nivel(d->nivel));
    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 * API - lecturas puras
 * ================================================================== */
hashmal_roles_err_t hashmal_escalacion_nivel(uint32_t disputa_id,
                                              hashmal_escalacion_nivel_t *out)
{
    if (out == NULL) return HASHMAL_ROLES_ERR_NULL;

    int idx = buscar_por_id(disputa_id);
    if (idx < 0) return HASHMAL_ROLES_ERR_ESCALACION;

    *out = g_disputas[idx].nivel;
    return HASHMAL_ROLES_OK;
}

hashmal_roles_err_t
hashmal_escalacion_disputa_estado(uint32_t disputa_id,
                                   hashmal_escalacion_disputa_t *out)
{
    if (out == NULL) return HASHMAL_ROLES_ERR_NULL;

    int idx = buscar_por_id(disputa_id);
    if (idx < 0) return HASHMAL_ROLES_ERR_ESCALACION;

    memcpy(out, &g_disputas[idx], sizeof(*out));
    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 *
 *   "METAXY sou kai autou MONOU... ean sou akouse, EKERDESAS ton
 *    adelfon sou." - Mateo 18:15
 *
 *   "DUO e TRION martyron." - Mateo 18:16 (cita Dt 19:15)
 *
 *   "eipe te EKKLESIA." - Mateo 18:17a
 *
 *   "esto soi hosper ho ETHNIKOS." - Mateo 18:17b
 *
 *   El protocolo de Yeshua - privado primero, testigos segundo,
 *   congregacion tercero, corte si no oye. Monotonico, no reversible.
 *   Prov 25:9 refuerza el orden: "no descubras el secreto de otro"
 *   - el SOD no se publica antes de tiempo. Y Lv 19:17 es el que
 *   inicia todo: "HOKHEACH TOKHIACH" - reprender es mandato, no
 *   opcion.
 *
 *   La mayor parte se resuelve en Nivel 1. El que sube a ekklesia
 *   ya fracaso en el tete-a-tete. El que alcanza nikrat se corto
 *   solo - tres veces oyo, tres veces no escucho.
 *
 * ================================================================== */
