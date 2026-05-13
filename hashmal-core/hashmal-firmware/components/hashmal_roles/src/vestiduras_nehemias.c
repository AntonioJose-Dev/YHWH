/*
 * ===================================================================
 * PIEZA 09 - VESTIDURAS - vestiduras_nehemias.c
 * Añadidura 10: DEFENSA DISTRIBUIDA CON PUNTO DE RALLY
 * Nehemias 4:13-20 + Jueces 7:18-20 + 1 Samuel 17:47
 * ===================================================================
 *
 * "vaa'amid mittajtiyot lammaqom mei'aharei lajomah... vaa'amid
 *  et ha'am LEMISHPAJOT 'im jarvoteihem rmajeihem ve qashtotam."
 *   - Nehemias 4:13
 * "Puse al pueblo en los lugares mas bajos detras del muro... lo
 *  puse por FAMILIAS con sus espadas, lanzas y arcos."
 *
 * "al tir'u mippeneihem; et ADONAI haggadol vehannora zekhoru,
 *  veHILLAJAMU 'al ajeikhem." - Nehemias 4:14
 * "No temais delante de ellos; acordaos del Senor grande y
 *  temible, y PELEAD por vuestros hermanos."
 *
 * "kullanu el HAJOMAH ISH EL MELA'KHTO." - Nehemias 4:15
 * "Cada uno volvio a SU OBRA en el muro."
 *
 * "JATSI ne'aray 'osim bammela'khah veJATSI majaziqim
 *  baromakhim hammaginnim vehaqshatot vehashshiryyonim." - Nehemias 4:16
 * "MITAD de mis criados trabajaba en la obra, MITAD sostenia
 *  lanzas, escudos, arcos y corazas."
 *
 * "be'AJAT yado 'oseh bammela'khah veAJAT majazeqet hashshalaj."
 *   - Nehemias 4:17
 * "Con UNA mano hacian la obra, con la OTRA sostenian el arma."
 *
 * "vaomar el hajorim ve'el hassganim ve'el yeter ha'am: hammela'khah
 *  harbeh urjavah va'ananu nefaradim 'al hajomah rejoqim ish meajiv.
 *  biMQOM asher tishme'u et qol hashshofar SHAMMAH tiqavtsu eleinu;
 *  Eloheinu yilajem lanu." - Nehemias 4:19-20
 * "Dije a los nobles, oficiales y al pueblo: la obra es grande y
 *  extensa, estamos esparcidos lejos uno del otro. En el LUGAR
 *  donde oyereis la voz del SHOFAR, ALLI os reunireis con nosotros;
 *  nuestro Dios peleara por nosotros."
 *
 * "lo bajerev uvajanit yehoshia' YHWH ki la-YHWH hammiljamah."
 *   - 1 Samuel 17:47
 * "No con espada y lanza salva YHWH; pues de YHWH es la batalla."
 *
 * --- EXEGESIS OPERATIVA ---
 *
 *   mishpajot (H4940)         FAMILIAS - agrupacion natural por
 *                              afinidad. En firmware: nodos vecinos
 *                              forman cluster temporal alrededor del
 *                              anchor.
 *
 *   ish el mela'khto (H4399)  CADA UNO A SU OBRA. Default = trabajo
 *                              productivo. La defensa interrumpe; al
 *                              terminar, vuelta.
 *
 *   jatsi (H2677)             MITAD. Neh 4:16 establece la
 *                              proporcion: 50/50 trabajo/vigilancia.
 *                              No es "todos defienden" - el trabajo
 *                              continua siempre.
 *
 *   shofar (H7782)            CUERNO. La senal acustica. En el
 *                              firmware: ya existe via Tsofeh
 *                              (Añadidura 02 hashmal_comm_shofar_tocar).
 *                              Aqui solo recibimos el efecto.
 *
 *   tiqavtsu (H6908 nifal)    OS REUNIREIS. Verbo reflexivo - los
 *                              nodos se autoorganizan; el modulo
 *                              registra esa autoorganizacion.
 *
 * --- ASIGNACION DETERMINISTA SIN ESTADO POR-NODO ---
 *
 * En vez de almacenar el rol de cada participante, lo derivamos:
 *
 *   indice_en_lista = posicion del nodo en participantes[]
 *   rol = ((indice_en_lista + rotaciones) par) ? TRABAJO : VIGILIA
 *
 * Asi un solo contador `rotaciones` invierte todos los roles de
 * golpe (Neh 4:16 jatsi/jatsi swap completo). Sin estado per-node
 * adicional. _rotar incrementa el contador.
 *
 * --- DIFERENCIA CON TSOFEH (Añadidura 02) Y ESCALACION (09) ---
 *
 *   Tsofeh:     DETECCION + AVISO (toca el shofar). Ez 33.
 *   Nehemias:   RECEPCION + CLUSTER. Neh 4. Despues del shofar.
 *   Escalacion: DISPUTA INTERNA entre hermanos. Mt 18.
 *
 * Nehemias asume que el shofar ya sono (otra capa lo emite). Su
 * funcion es ORGANIZAR la respuesta colectiva. Los tres son
 * complementarios.
 *
 * --- PIEZA 14 FIREWALL WIRING (fase 1: documentado, no wireado) ---
 *
 * Spec menciona "defensa activa en la zona atacada". En fase 1,
 * _activar solo registra el rally. La activacion de reglas
 * defensivas en firewall (incrementar penalizaciones, cerrar
 * pares hacia el anchor, etc.) queda para PR futuro - similar
 * patron a NIKRAT en Escalacion (Añadidura 09).
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_roles.h            API publica
 *   hashmal_roles_internal.h   ahora_ms
 *   esp_log.h (ORO EGIPTO)     NEH_LOG_*
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
static const char *NEH_TAG = "hashmal.roles.nehemias";

#define NEH_LOG_I(fmt, ...)  ESP_LOGI(NEH_TAG, fmt, ##__VA_ARGS__)
#define NEH_LOG_W(fmt, ...)  ESP_LOGW(NEH_TAG, fmt, ##__VA_ARGS__)
#define NEH_LOG_E(fmt, ...)  ESP_LOGE(NEH_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * Singleton del rally
 *
 * Un rally activo a la vez (fase 1). Estado inicial INACTIVO -
 * antes del primer _activar todo es cero, num_participantes = 0.
 * ================================================================== */
static hashmal_rally_t g_rally = {
    .rally_id                     = 0,
    .anchor_nodo_id               = 0,
    .participantes                = {0},
    .num_participantes            = 0,
    .estado                       = HASHMAL_RALLY_ESTADO_INACTIVO,
    .timestamp_activado_ms        = 0,
    .timestamp_ultima_rotacion_ms = 0,
    .rotaciones                   = 0,
};

/* ==================================================================
 * Helpers estaticos
 * ================================================================== */

/* Devuelve indice del nodo en participantes[] o -1 si no encontrado. */
static int buscar_participante(uint8_t nodo_id)
{
    for (uint8_t i = 0; i < g_rally.num_participantes; ++i) {
        if (g_rally.participantes[i] == nodo_id) return (int)i;
    }
    return -1;
}

static const char *nombre_estado(hashmal_rally_estado_t e)
{
    switch (e) {
    case HASHMAL_RALLY_ESTADO_ACTIVO:   return "ACTIVO";
    case HASHMAL_RALLY_ESTADO_DISUELTO: return "DISUELTO (Neh 4:15)";
    case HASHMAL_RALLY_ESTADO_INACTIVO:
    default:                             return "INACTIVO";
    }
}

static const char *nombre_rol(hashmal_rally_rol_t r)
{
    switch (r) {
    case HASHMAL_RALLY_ROL_TRABAJO: return "TRABAJO (Neh 4:17 yado 'oseh)";
    case HASHMAL_RALLY_ROL_VIGILIA: return "VIGILIA (Neh 4:17 majazeqet hashshalaj)";
    case HASHMAL_RALLY_ROL_NINGUNO:
    default:                         return "NINGUNO";
    }
}

/* Asignacion determinista derivada (no almacenada por nodo). */
static hashmal_rally_rol_t rol_para_indice(int idx)
{
    if (idx < 0) return HASHMAL_RALLY_ROL_NINGUNO;
    /* Neh 4:16 - jatsi/jatsi alternado. Rotaciones invierten. */
    return (((uint32_t)idx + g_rally.rotaciones) & 1u)
           ? HASHMAL_RALLY_ROL_VIGILIA
           : HASHMAL_RALLY_ROL_TRABAJO;
}

/* ==================================================================
 * API - hashmal_rally_activar (Neh 4:19-20)
 * ================================================================== */
hashmal_roles_err_t hashmal_rally_activar(uint32_t rally_id,
                                           uint8_t anchor_nodo_id)
{
    if (anchor_nodo_id >= HASHMAL_ROLES_MAX_NODOS) {
        NEH_LOG_E("activar: anchor=%u fuera de [0, %u)",
                  (unsigned)anchor_nodo_id,
                  (unsigned)HASHMAL_ROLES_MAX_NODOS);
        return HASHMAL_ROLES_ERR_NODO_NO_ENCONTRADO;
    }

    int64_t ahora = hashmal_roles_ahora_ms();

    if (g_rally.estado == HASHMAL_RALLY_ESTADO_ACTIVO) {
        if (g_rally.rally_id == rally_id) {
            /* Idempotente: refresca anchor y timestamp; conserva
             * participantes ya unidos. */
            g_rally.anchor_nodo_id        = anchor_nodo_id;
            g_rally.timestamp_activado_ms = ahora;
            NEH_LOG_I("activar: rally %" PRIu32 " ya activo - anchor "
                      "refrescado a %u (idempotente)",
                      rally_id, (unsigned)anchor_nodo_id);
            return HASHMAL_ROLES_OK;
        }
        /* Distinto rally_id - fusion fase 1 (multi-rally es fase 2). */
        NEH_LOG_W("activar: rally previo %" PRIu32 " activo; "
                  "FUSIONANDO con %" PRIu32 " - participantes previos "
                  "descartados (fase 1: singleton)",
                  g_rally.rally_id, rally_id);
    }

    /* Inicializa singleton para nuevo rally. */
    memset(&g_rally, 0, sizeof(g_rally));
    g_rally.rally_id                     = rally_id;
    g_rally.anchor_nodo_id               = anchor_nodo_id;
    g_rally.estado                       = HASHMAL_RALLY_ESTADO_ACTIVO;
    g_rally.timestamp_activado_ms        = ahora;
    g_rally.timestamp_ultima_rotacion_ms = ahora;

    NEH_LOG_I("activar: rally %" PRIu32 " ACTIVO en anchor=%u "
              "(Neh 4:19-20 shamah tiqavtsu)",
              rally_id, (unsigned)anchor_nodo_id);
    /* TODO fase 2: llamar a Pieza 14 firewall para defensa activa
     * en zona del anchor (incrementar penalizaciones, cerrar pares
     * sospechosos). */
    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 * API - hashmal_rally_unirse (Neh 4:13)
 * ================================================================== */
hashmal_roles_err_t hashmal_rally_unirse(uint32_t rally_id, uint8_t nodo_id)
{
    if (nodo_id >= HASHMAL_ROLES_MAX_NODOS) {
        return HASHMAL_ROLES_ERR_NODO_NO_ENCONTRADO;
    }
    if (g_rally.estado != HASHMAL_RALLY_ESTADO_ACTIVO) {
        NEH_LOG_W("unirse: no hay rally activo");
        return HASHMAL_ROLES_ERR_RALLY;
    }
    if (g_rally.rally_id != rally_id) {
        NEH_LOG_W("unirse: rally_id %" PRIu32 " no coincide con activo %"
                  PRIu32, rally_id, g_rally.rally_id);
        return HASHMAL_ROLES_ERR_RALLY;
    }

    /* Idempotente: si ya unido, no-op. */
    if (buscar_participante(nodo_id) >= 0) {
        return HASHMAL_ROLES_OK;
    }

    if (g_rally.num_participantes >= HASHMAL_RALLY_PARTICIPANTES_MAX) {
        NEH_LOG_W("unirse: tabla llena (%u participantes)",
                  (unsigned)HASHMAL_RALLY_PARTICIPANTES_MAX);
        return HASHMAL_ROLES_ERR_RALLY;
    }

    g_rally.participantes[g_rally.num_participantes++] = nodo_id;

    /* El rol asignado al nuevo participante es derivado por su
     * posicion - ver rol_para_indice. Lo logamos para audit. */
    int idx = (int)g_rally.num_participantes - 1;
    NEH_LOG_I("unirse: nodo %u -> rally %" PRIu32 " idx=%d rol=%s "
              "(Neh 4:13 mishpajot 'im jarvoteihem)",
              (unsigned)nodo_id, rally_id, idx,
              nombre_rol(rol_para_indice(idx)));
    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 * API - hashmal_rally_rotar (Neh 4:16 jatsi/jatsi swap)
 * ================================================================== */
hashmal_roles_err_t hashmal_rally_rotar(uint32_t rally_id)
{
    if (g_rally.estado != HASHMAL_RALLY_ESTADO_ACTIVO) {
        return HASHMAL_ROLES_ERR_RALLY;
    }
    if (g_rally.rally_id != rally_id) {
        return HASHMAL_ROLES_ERR_RALLY;
    }

    g_rally.rotaciones                  += 1u;
    g_rally.timestamp_ultima_rotacion_ms = hashmal_roles_ahora_ms();

    NEH_LOG_I("rotar: rally %" PRIu32 " - rotacion #%" PRIu32 " "
              "(Neh 4:16 jatsi swap)",
              rally_id, g_rally.rotaciones);
    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 * API - hashmal_rally_rol_nodo (lectura pura)
 * ================================================================== */
hashmal_roles_err_t hashmal_rally_rol_nodo(uint8_t nodo_id,
                                            hashmal_rally_rol_t *out)
{
    if (out == NULL) return HASHMAL_ROLES_ERR_NULL;

    if (g_rally.estado != HASHMAL_RALLY_ESTADO_ACTIVO) {
        *out = HASHMAL_RALLY_ROL_NINGUNO;
        return HASHMAL_ROLES_OK;
    }

    int idx = buscar_participante(nodo_id);
    *out = rol_para_indice(idx);
    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 * API - hashmal_rally_punto (lectura pura)
 * ================================================================== */
hashmal_roles_err_t hashmal_rally_punto(hashmal_rally_t *out)
{
    if (out == NULL) return HASHMAL_ROLES_ERR_NULL;
    memcpy(out, &g_rally, sizeof(*out));
    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 * API - hashmal_rally_disolver (Neh 4:15 ish el mela'khto)
 * ================================================================== */
hashmal_roles_err_t hashmal_rally_disolver(uint32_t rally_id)
{
    if (g_rally.estado != HASHMAL_RALLY_ESTADO_ACTIVO) {
        if (g_rally.estado == HASHMAL_RALLY_ESTADO_DISUELTO
         && g_rally.rally_id == rally_id) {
            /* Idempotente. */
            return HASHMAL_ROLES_OK;
        }
        NEH_LOG_W("disolver: no hay rally activo");
        return HASHMAL_ROLES_ERR_RALLY;
    }
    if (g_rally.rally_id != rally_id) {
        NEH_LOG_W("disolver: rally_id %" PRIu32 " no coincide con "
                  "activo %" PRIu32, rally_id, g_rally.rally_id);
        return HASHMAL_ROLES_ERR_RALLY;
    }

    g_rally.estado = HASHMAL_RALLY_ESTADO_DISUELTO;

    NEH_LOG_I("disolver: rally %" PRIu32 " %s (Neh 4:15 ish el "
              "mela'khto - vuelta a la obra); %u participantes "
              "regresan; %" PRIu32 " rotaciones realizadas",
              rally_id, nombre_estado(g_rally.estado),
              (unsigned)g_rally.num_participantes,
              g_rally.rotaciones);
    /* TODO fase 2: notificar a Pieza 14 firewall para desactivar
     * defensa activa de la zona del anchor. */
    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 *
 *   "vaa'amid 'et ha'am LEMISHPAJOT 'im jarvoteihem." - Neh 4:13
 *
 *   "kullanu el HAJOMAH ISH EL MELA'KHTO." - Nehemias 4:15
 *
 *   "JATSI 'osim bammela'khah veJATSI majaziqim." - Nehemias 4:16
 *
 *   "be'AJAT yado 'oseh veAJAT majazeqet hashshalaj." - Nehemias 4:17
 *
 *   "biMQOM asher tishme'u et qol hashshofar SHAMMAH tiqavtsu."
 *     - Nehemias 4:19-20
 *
 *   El rally es dinamico - va donde suena el shofar. Y la mitad
 *   sigue trabajando: la red NUNCA se detiene completamente. La
 *   defensa interrumpe por momentos, no por dias. Cuando la
 *   amenaza pasa, todos vuelven al muro - cada uno a su obra.
 *   Eloheinu yilajem lanu (Neh 4:20): la batalla es del Padre,
 *   no del firmware. El firmware solo organiza la respuesta.
 *
 * ================================================================== */
