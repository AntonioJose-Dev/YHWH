/*
 * ===================================================================
 * PIEZA 09 - VESTIDURAS - vestiduras_goel.c
 * Añadidura 12: GOEL - HERENCIA DE NODO (el redentor cercano)
 * Rut 3-4 + Levitico 25:23-55 + Numeros 35:19 + Job 19:25
 * ===================================================================
 *
 * "uvva GO'ALO HAQQAROV elav vega'al et mimkar ajiv." - Levitico 25:25
 * "Vendra su REDENTOR el MAS CERCANO a el y rescatara lo que su
 *  hermano hubiere vendido."
 *
 * "she'er besaro mimmishpajto YIG'ALENNU." - Levitico 25:49
 * "Pariente de su CARNE de su FAMILIA lo redimira."
 *
 * "vehinneh HAGGO'EL 'over asher dibber Bo'az shevah-poh peloni
 *  almoni vayyasor vayyeshev." - Rut 4:1
 * "He aqui el GO'EL que Bo'az habia dicho pasaba; le dijo: ven
 *  aca, fulano, y se sento."
 *
 * "vayyiqaj 'ASARAH anashim mizziqney ha'ir vayyomer SHEVU-FOH
 *  vayyeshevu." - Rut 4:2
 * "Tomo a DIEZ ancianos de la ciudad y dijo: sentaos aqui."
 *
 * "im TIG'AL GE'AL ve'im LO YIG'AL haggidah lli ve'eda'ah ki ein
 *  zulatekha ligh'ol ve'anokhi ajareikha." - Rut 4:4
 * "Si redimes, REDIME; si NO, dimelo, porque no hay otro fuera de
 *  ti que pueda redimir, y yo despues de ti."
 *
 * "vayyomer haggo'el LO UKHAL ligh'ol li pen ashjit et najalati;
 *  GE'AL LEKHA ATTAH et ge'ulati ki LO UKHAL ligh'ol." - Rut 4:6
 * "El goel dijo: NO PUEDO redimir, no sea que dane mi heredad;
 *  redime tu por ti la mia, porque YO NO PUEDO."
 *
 * "leHAQIM SHEM hammet 'al najalato velo yikkaret SHEM hammet
 *  me'im ehav umishsha'ar mqomo." - Rut 4:10
 * "Para LEVANTAR el NOMBRE del muerto sobre su heredad, y no se
 *  borre su nombre de entre sus hermanos."
 *
 * "ki anokhi yada'ti GO'ALI JAY ve'ajaron 'al 'afar yaqum." - Job 19:25
 * "Yo se que mi REDENTOR vive, y al final se levantara sobre el polvo."
 *
 * --- EXEGESIS OPERATIVA ---
 *
 *   go'el (H1350)            REDENTOR. Pariente cercano con derecho
 *                             y obligacion de actuar por el caido.
 *                             En Israel: rescata tierra, persona,
 *                             nombre. Vengador de sangre (Nm 35:19).
 *
 *   haqqarov elav (H7138)    EL MAS CERCANO. La proximidad determina
 *                             el orden. No es asignacion arbitraria;
 *                             el linaje fija prioridades.
 *
 *   she'er besaro (H7607)    PARIENTE DE SU CARNE. Linaje biologico
 *                             en Israel; en el firmware: vecindario
 *                             topologico (RSSI / hop count).
 *
 *   ge'al / lo ukhal (H1350+H3201) "REDIME" / "NO PUEDO". El goel
 *                             tiene DERECHO pero tambien puede
 *                             DECLINAR si arruina su propia heredad.
 *                             Rut 4:6: el primero declino; Bo'az
 *                             (segundo) acepto.
 *
 *   leHAQIM SHEM hammet      "LEVANTAR EL NOMBRE DEL MUERTO." En
 *                             Israel: el primer hijo del goel toma
 *                             el nombre del muerto. En el firmware:
 *                             mensajes dirigidos al muerto se
 *                             redirigen al goel - su identidad
 *                             persiste a traves del redentor.
 *
 * --- CADENA DE GOELIM (Rut 4:4-6) ---
 *
 * Bo'az establece el orden: "no hay otro... y yo DESPUES DE TI"
 * (4:4). Cuando el primero declina (4:6), el siguiente toma el
 * lugar. El firmware lo replica: tabla ordenada, _heredar walka
 * en orden, primer disponible es el goel.
 *
 * Fase 1 simplificacion: si la entrada ya esta CONFIRMADA, no
 * re-proponemos. Lo que valdria una "negativa" del primero (que
 * dispararia avance al siguiente) es API de fase 2 - por ahora,
 * el caller debe llamar _heredar despues de un explicito decline
 * via mecanismo no implementado aqui.
 *
 * --- DIFERENCIA CON MIQLAT, ESCALACION, NEHEMIAS ---
 *
 *   Miqlat (Añ. 05):    refugio para el que HUYE - acoge.
 *   Escalacion (Añ. 09):disputa entre hermanos VIVOS - resuelve.
 *   Nehemias (Añ. 10): defensa colectiva ante amenaza - rally.
 *   Goel (Añ. 12):     herencia del que MURIO - sucede.
 *
 * Cuatro respuestas distintas a cuatro condiciones distintas. El
 * sistema no las confunde.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_roles.h          API publica
 *   hashmal_roles_internal.h ahora_ms
 *   esp_log.h (ORO EGIPTO)   GOEL_LOG_*
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
static const char *GOEL_TAG = "hashmal.roles.goel";

#define GOEL_LOG_I(fmt, ...)  ESP_LOGI(GOEL_TAG, fmt, ##__VA_ARGS__)
#define GOEL_LOG_W(fmt, ...)  ESP_LOGW(GOEL_TAG, fmt, ##__VA_ARGS__)
#define GOEL_LOG_E(fmt, ...)  ESP_LOGE(GOEL_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * Estado interno
 *
 * Tabla de goelim ordenada por proximidad (operador-set). Tabla
 * de herencias indexada por muerto_nodo_id (lookup directo).
 * ================================================================== */
static uint8_t g_goelim_orden[HASHMAL_GOEL_TABLA_MAX];
static uint8_t g_num_goelim = 0;

/* Una herencia activa por muerto. Slot vacio: estado == NINGUNA. */
static hashmal_goel_herencia_t g_herencias[HASHMAL_ROLES_MAX_NODOS];

/* ==================================================================
 * Helpers estaticos
 * ================================================================== */

static const char *nombre_estado(hashmal_goel_estado_t e)
{
    switch (e) {
    case HASHMAL_GOEL_ESTADO_PROPUESTA:  return "PROPUESTA";
    case HASHMAL_GOEL_ESTADO_CONFIRMADA: return "CONFIRMADA (Rut 4:2 ancianos)";
    case HASHMAL_GOEL_ESTADO_REVERTIDA:  return "REVERTIDA";
    case HASHMAL_GOEL_ESTADO_NINGUNA:
    default:                              return "NINGUNA";
    }
}

static bool nodo_legitimo(uint8_t nodo_id)
{
    return nodo_id < HASHMAL_ROLES_MAX_NODOS;
}

/* ==================================================================
 * API - hashmal_goel_tabla_set (Lv 25:25 haqarov elav)
 * ================================================================== */
hashmal_roles_err_t hashmal_goel_tabla_set(const uint8_t *goelim_orden,
                                            uint8_t n)
{
    if (goelim_orden == NULL && n > 0) return HASHMAL_ROLES_ERR_NULL;
    if (n > HASHMAL_GOEL_TABLA_MAX) {
        GOEL_LOG_E("tabla_set: n=%u excede TABLA_MAX=%u",
                   (unsigned)n, (unsigned)HASHMAL_GOEL_TABLA_MAX);
        return HASHMAL_ROLES_ERR_GOEL;
    }

    /* Validacion: cada id valido y sin duplicados. */
    for (uint8_t i = 0; i < n; ++i) {
        if (!nodo_legitimo(goelim_orden[i])) {
            GOEL_LOG_E("tabla_set: id %u en slot %u fuera de rango",
                       (unsigned)goelim_orden[i], (unsigned)i);
            return HASHMAL_ROLES_ERR_NODO_NO_ENCONTRADO;
        }
        for (uint8_t j = i + 1; j < n; ++j) {
            if (goelim_orden[i] == goelim_orden[j]) {
                GOEL_LOG_E("tabla_set: id %u duplicado en slots %u y %u",
                           (unsigned)goelim_orden[i],
                           (unsigned)i, (unsigned)j);
                return HASHMAL_ROLES_ERR_GOEL;
            }
        }
    }

    if (n > 0) memcpy(g_goelim_orden, goelim_orden, (size_t)n);
    g_num_goelim = n;

    GOEL_LOG_I("tabla_set: %u goelim ordenados por proximidad "
               "(Lv 25:25 haqarov elav)", (unsigned)n);
    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 * API - hashmal_goel_tabla_get (lectura pura)
 * ================================================================== */
hashmal_roles_err_t hashmal_goel_tabla_get(uint8_t *out_orden,
                                            uint8_t *out_n)
{
    if (out_orden == NULL || out_n == NULL) return HASHMAL_ROLES_ERR_NULL;
    if (g_num_goelim > 0) {
        memcpy(out_orden, g_goelim_orden, (size_t)g_num_goelim);
    }
    *out_n = g_num_goelim;
    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 * API - hashmal_goel_heredar (Rut 4:1-6)
 * ================================================================== */
hashmal_roles_err_t hashmal_goel_heredar(uint8_t muerto_nodo_id,
                                          uint8_t *out_goel_id)
{
    if (out_goel_id == NULL) return HASHMAL_ROLES_ERR_NULL;
    if (!nodo_legitimo(muerto_nodo_id)) {
        return HASHMAL_ROLES_ERR_NODO_NO_ENCONTRADO;
    }

    hashmal_goel_herencia_t *h = &g_herencias[muerto_nodo_id];

    /* Idempotente: si ya hay herencia PROPUESTA o CONFIRMADA, devuelve
     * el goel existente sin re-proponer. REVERTIDA o NINGUNA permite
     * nueva propuesta. */
    if (h->estado == HASHMAL_GOEL_ESTADO_PROPUESTA
     || h->estado == HASHMAL_GOEL_ESTADO_CONFIRMADA) {
        *out_goel_id = h->goel_nodo_id;
        return HASHMAL_ROLES_OK;
    }

    if (g_num_goelim == 0) {
        GOEL_LOG_E("heredar: tabla de goelim vacia - operator debe "
                   "llamar _tabla_set primero (Lv 25:25 sin she'er)");
        return HASHMAL_ROLES_ERR_GOEL;
    }

    /* Walk la tabla: primer goel != muerto. Rut 4:4 "im tig'al ge'al"
     * - se ofrece al primero. Rut 4:6 "lo ukhal" del primero
     * dispararia avance al siguiente, pero esa logica de "decline"
     * es API de fase 2 - aqui solo proponemos al primero apto. */
    int seleccionado = -1;
    for (uint8_t i = 0; i < g_num_goelim; ++i) {
        uint8_t cand = g_goelim_orden[i];
        if (cand == muerto_nodo_id) continue;  /* no se redime a si mismo */
        seleccionado = (int)cand;
        break;
    }

    if (seleccionado < 0) {
        GOEL_LOG_E("heredar: no hay goel apto para muerto %u",
                   (unsigned)muerto_nodo_id);
        return HASHMAL_ROLES_ERR_GOEL;
    }

    int64_t ahora = hashmal_roles_ahora_ms();
    h->muerto_nodo_id            = muerto_nodo_id;
    h->goel_nodo_id              = (uint8_t)seleccionado;
    h->estado                    = HASHMAL_GOEL_ESTADO_PROPUESTA;
    h->votos_recibidos           = 0;
    h->timestamp_propuesta_ms    = ahora;
    h->timestamp_confirmacion_ms = 0;

    *out_goel_id = (uint8_t)seleccionado;

    GOEL_LOG_I("heredar: muerto=%u -> goel=%u PROPUESTA "
               "(Rut 4:1 vehinneh haggo'el 'over); espera quorum %u "
               "(Rut 4:2)",
               (unsigned)muerto_nodo_id, (unsigned)seleccionado,
               (unsigned)HASHMAL_GOEL_QUORUM_MINIMO);
    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 * API - hashmal_goel_verificar (Rut 4:2 ziqney ha'ir)
 * ================================================================== */
hashmal_roles_err_t hashmal_goel_verificar(uint8_t muerto_nodo_id,
                                            uint8_t goel_nodo_id,
                                            uint8_t votos_a_favor)
{
    if (!nodo_legitimo(muerto_nodo_id)) {
        return HASHMAL_ROLES_ERR_NODO_NO_ENCONTRADO;
    }

    hashmal_goel_herencia_t *h = &g_herencias[muerto_nodo_id];

    if (h->estado == HASHMAL_GOEL_ESTADO_NINGUNA
     || h->estado == HASHMAL_GOEL_ESTADO_REVERTIDA) {
        GOEL_LOG_E("verificar: muerto %u sin propuesta activa",
                   (unsigned)muerto_nodo_id);
        return HASHMAL_ROLES_ERR_GOEL;
    }

    if (h->goel_nodo_id != goel_nodo_id) {
        GOEL_LOG_W("verificar: goel propuesto %u, voto a favor de %u "
                   "no coincide", (unsigned)h->goel_nodo_id,
                   (unsigned)goel_nodo_id);
        return HASHMAL_ROLES_ERR_GOEL;
    }

    /* Idempotente: ya CONFIRMADA y se vuelve a llamar -> OK. */
    if (h->estado == HASHMAL_GOEL_ESTADO_CONFIRMADA) {
        return HASHMAL_ROLES_OK;
    }

    h->votos_recibidos = votos_a_favor;

    if (votos_a_favor >= HASHMAL_GOEL_QUORUM_MINIMO) {
        h->estado                    = HASHMAL_GOEL_ESTADO_CONFIRMADA;
        h->timestamp_confirmacion_ms = hashmal_roles_ahora_ms();
        GOEL_LOG_I("verificar: herencia %s - muerto=%u goel=%u con "
                   "%u votos (Rut 4:10 leHAQIM SHEM hammet 'al najalato)",
                   nombre_estado(h->estado),
                   (unsigned)muerto_nodo_id, (unsigned)goel_nodo_id,
                   (unsigned)votos_a_favor);
        return HASHMAL_ROLES_OK;
    }

    GOEL_LOG_W("verificar: votos %u < QUORUM_MINIMO %u - herencia "
               "permanece PROPUESTA (caller puede reintentar con "
               "mas votos)",
               (unsigned)votos_a_favor,
               (unsigned)HASHMAL_GOEL_QUORUM_MINIMO);
    return HASHMAL_ROLES_ERR_GOEL;
}

/* ==================================================================
 * API - hashmal_goel_herencia_consultar (lectura pura)
 * ================================================================== */
hashmal_roles_err_t
hashmal_goel_herencia_consultar(uint8_t muerto_nodo_id,
                                 hashmal_goel_herencia_t *out)
{
    if (out == NULL) return HASHMAL_ROLES_ERR_NULL;
    if (!nodo_legitimo(muerto_nodo_id)) {
        return HASHMAL_ROLES_ERR_NODO_NO_ENCONTRADO;
    }

    const hashmal_goel_herencia_t *h = &g_herencias[muerto_nodo_id];
    if (h->estado == HASHMAL_GOEL_ESTADO_NINGUNA) {
        return HASHMAL_ROLES_ERR_GOEL;
    }
    memcpy(out, h, sizeof(*out));
    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 *
 *   "uvva GO'ALO HAQQAROV elav." - Levitico 25:25
 *
 *   "vehinneh HAGGO'EL 'over." - Rut 4:1
 *
 *   "im TIG'AL GE'AL ve'im LO YIG'AL haggidah lli." - Rut 4:4
 *
 *   "GE'AL LEKHA ATTAH ki LO UKHAL." - Rut 4:6
 *
 *   "leHAQIM SHEM hammet 'al najalato." - Rut 4:10
 *
 *   "yada'ti GO'ALI JAY." - Job 19:25
 *
 *   El cercano tiene primer derecho. Si declina, pasa al siguiente.
 *   Y la congregacion confirma con votos - no es transferencia
 *   silenciosa, es publica. El nombre del que cayo se levanta
 *   sobre su heredad: el firmware redirige los mensajes al goel
 *   para que la identidad del muerto siga sirviendo. La red no
 *   olvida a quien la sirvio - el redentor cercano lo levanta de
 *   nuevo.
 *
 * ================================================================== */
