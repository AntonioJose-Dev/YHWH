/*
 * ===================================================================
 * PIEZA 09 - VESTIDURAS - vestiduras_neder.c
 * Añadidura 20: NEDER - El Voto (Compromiso Inquebrantable)
 * Numeros 30:1-16 + Deuteronomio 23:21-23 + Eclesiastes 5:4-6
 *                 + Salmo 116:14 + Mateo 5:33-37 + Santiago 5:12
 * ===================================================================
 *
 * "vaydabber Moshe el rashei hammattot livnei Yisrael lemor: zeh
 *  haddavar asher tsivvah YHWH. ish ki YIDDOR NEDER laYHWH o
 *  hishshava' shevu'ah lessor issar 'al nafsho LO YAJEL DEVARO;
 *  KEKHOL HAYYOTSE MIPPIV YA'ASEH." - Numeros 30:2-3
 * "Hablo Moshe a las cabezas de las tribus: esto es lo que mando
 *  YHWH. Si un hombre HACE VOTO a YHWH o hace juramento ligando
 *  obligacion sobre su alma, NO QUEBRANTARA su palabra; conforme
 *  a TODO LO QUE SALIO DE SU BOCA, HARA."
 *
 * "im havi'ah el bet ishah nedareiha 'aleiha o mivta sefateiha
 *  asher asrah 'al nafshah veshama' ishah... vehefer YHWH lah
 *  veYHWH yislaj lah." - Numeros 30:11-13
 * "Si dentro de la casa de su marido hizo voto... y su marido lo
 *  ESCUCHO Y LO ANULO, entonces YHWH la PERDONARA." (autoridad
 *  legitima puede liberar el voto, no romperlo)
 *
 * "kaasher tiddor neder le'Elohim AL TE'AJER leshallemo ki ein
 *  jefets bakkesilim; et asher tiddor SHALLEM. tov asher LO TIDDOR
 *  mishshettiddor velo teshallem." - Eclesiastes 5:4-5
 * "Cuando hagas voto a Dios, NO TARDES en pagarlo, porque no se
 *  agrada en los necios; lo que prometes, PAGA. MEJOR es que NO
 *  HAGAS VOTO, que hacerlo y no pagar."
 *
 * "ki tiddor neder laYHWH Eloheikha LO TE'AJER leshallemo ki
 *  darosh yidresheinu YHWH Eloheikha me'immakh vehayah vekha
 *  jet." - Deuteronomio 23:21
 * "Cuando hagas voto a YHWH... NO TARDES en pagarlo, porque YHWH
 *  ciertamente lo demandara de ti, y seria pecado en ti."
 *
 * "vekhi tejdal lindor LO YIHEYEH BEKHA JET." - Deuteronomio 23:22
 * "Si TE ABSTIENES de votar, no habra pecado en ti." (no es
 *  obligatorio)
 *
 * "ashallem nedaray laYHWH negda na lekhol 'ammo." - Salmo 116:14
 * "PAGARE mis VOTOS a YHWH delante de todo su pueblo."
 *
 * "palin ekousate hoti errethe tois arkhaios: ouk epiorkeseis...
 *  ego de lego hymin: me omosai holos... esto de ho logos hymon
 *  NAI NAI, OU OU; to de perisson touton ek tou ponerou estin."
 *   - Mateo 5:33-37
 * "Oisteis: no perjuraras... yo os digo, no jureis nada... sea
 *  vuestra palabra SI SI, NO NO; lo que es mas de esto, del
 *  malo procede."
 *
 * "pro panton de, adelphoi mou, me omnyete... eto de hymon to
 *  NAI nai, kai to OU ou; hina me hypo krisin pesete." - Stg 5:12
 * "Sobre todo, hermanos mios, NO JUREIS... mas bien sea vuestro
 *  SI, si; vuestro NO, no; para que no caigais en juicio."
 *
 * --- EXEGESIS OPERATIVA ---
 *
 *   neder (H5088)            VOTO. Compromiso voluntario y publico.
 *                             No es obligacion del Padre - es
 *                             ofrenda del que promete.
 *
 *   lo yajel (H2490)         "NO QUEBRANTARA / NO PROFANARA." Raiz
 *                             jalal: profanar lo santo. Romper voto
 *                             es PROFANACION del nombre invocado.
 *
 *   kekhol hayyotse mippiv   "CONFORME A TODO LO QUE SALIO DE SU
 *                             BOCA." El voto es COMPLETO - no se
 *                             cumple parcialmente.
 *
 *   ya'aseh (H6213)          "HARA." No "intentara" o "querra" -
 *                             ejecucion concreta.
 *
 *   al te'ajer (H309)        "NO TARDES." Ec 5:4 + Dt 23:21. La
 *                             tardanza es forma de incumplimiento.
 *
 *   tov asher LO TIDDOR      "MEJOR NO VOTAR." Ec 5:5 - inversion
 *                             prudencial. La sabiduria reconoce los
 *                             limites antes de prometer.
 *
 *   nai nai, ou ou (G3483)   "SI SI, NO NO." Mt 5:37 + Stg 5:12.
 *                             Cumplimiento NT: la palabra de un
 *                             discipulo deberia ser tan firme que
 *                             no necesite jurar para ser creida.
 *
 * --- COEXISTENCIA CON OTRAS AÑADIDURAS DE PIEZA 09 ---
 *
 *   Tsofeh (Añ. 02):    DETECCION + AVISO de amenaza. Ez 33.
 *   Tzitzit (Añ. 06):   recordatorio visual constante. Nm 15:38.
 *   Jatsotsrot (Añ. 08): broadcast tipificado. Nm 10.
 *   Escalacion (Añ. 09): disputa interna. Mt 18:15-17.
 *   Nehemias (Añ. 10):  rally defensivo. Neh 4.
 *   Goel (Añ. 12):      herencia del muerto. Lv 25 + Rut 4.
 *   NEDER (Añ. 20):     compromiso de servicio. Nm 30.
 *
 * Estas siete ahora cubren las seis dimensiones del rol del
 * operador en la red: detectar, recordar, comunicar, dirimir,
 * defender, suceder, y prometer. Cada una distinta, ninguna
 * redundante.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_roles.h          API publica
 *   hashmal_roles_internal.h ahora_ms
 *   esp_log.h (ORO EGIPTO)   NDR_LOG_*
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
static const char *NDR_TAG = "hashmal.roles.neder";

#define NDR_LOG_I(fmt, ...)  ESP_LOGI(NDR_TAG, fmt, ##__VA_ARGS__)
#define NDR_LOG_W(fmt, ...)  ESP_LOGW(NDR_TAG, fmt, ##__VA_ARGS__)
#define NDR_LOG_E(fmt, ...)  ESP_LOGE(NDR_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * Estado interno - tabla de votos + contadores historicos
 * ================================================================== */
static hashmal_neder_voto_t g_nedarim[HASHMAL_NEDER_MAX];
static uint32_t              g_cumplidos_total = 0;
static uint32_t              g_liberados_total = 0;
static uint32_t              g_violados_total  = 0;

/* Limite para _puede_prometer: maximo de votos activos concurrentes
 * por nodo. 4 = "alguno de cada tipo" - simple heuristica fase 1. */
#define NEDER_MAX_ACTIVOS_POR_NODO  4u

/* ==================================================================
 * Helpers estaticos
 * ================================================================== */

static const char *nombre_tipo(hashmal_neder_tipo_t t)
{
    switch (t) {
    case HASHMAL_NEDER_TIPO_RUTA:     return "RUTA";
    case HASHMAL_NEDER_TIPO_SERVICIO: return "SERVICIO";
    case HASHMAL_NEDER_TIPO_ALMACEN:  return "ALMACEN";
    case HASHMAL_NEDER_TIPO_QUORUM:   return "QUORUM";
    default:                           return "<tipo desconocido>";
    }
}

static const char *nombre_estado(hashmal_neder_estado_t e)
{
    switch (e) {
    case HASHMAL_NEDER_ESTADO_PENDIENTE: return "PENDIENTE";
    case HASHMAL_NEDER_ESTADO_ACTIVO:    return "ACTIVO";
    case HASHMAL_NEDER_ESTADO_CUMPLIDO:  return "CUMPLIDO (Sal 116:14 ashallem)";
    case HASHMAL_NEDER_ESTADO_LIBERADO:  return "LIBERADO (Nm 30:5 hefer)";
    case HASHMAL_NEDER_ESTADO_VIOLADO:   return "VIOLADO (Ec 5:5 jet)";
    case HASHMAL_NEDER_ESTADO_VACIO:
    default:                              return "VACIO";
    }
}

static bool tipo_legitimo(hashmal_neder_tipo_t t)
{
    return (unsigned)t < HASHMAL_NEDER_TIPOS;
}

static bool nodo_legitimo(uint8_t nodo_id)
{
    return nodo_id < HASHMAL_ROLES_MAX_NODOS;
}

/* Busca un voto por (nodo, tipo) en estado activo (PENDIENTE o
 * ACTIVO). Devuelve indice o -1 si no encuentra. */
static int buscar_voto_activo(uint8_t nodo_id, hashmal_neder_tipo_t tipo)
{
    for (uint8_t i = 0; i < HASHMAL_NEDER_MAX; ++i) {
        const hashmal_neder_voto_t *v = &g_nedarim[i];
        if (v->estado != HASHMAL_NEDER_ESTADO_PENDIENTE
         && v->estado != HASHMAL_NEDER_ESTADO_ACTIVO) {
            continue;
        }
        if (v->nodo_id == nodo_id && v->tipo == tipo) return (int)i;
    }
    return -1;
}

/* Slot libre = VACIO o estado terminal (CUMPLIDO/LIBERADO/VIOLADO).
 * Prefiere VACIO; si no hay, reusa terminal mas viejo (heuristica
 * simple: primero encontrado). */
static int buscar_slot_libre(void)
{
    for (uint8_t i = 0; i < HASHMAL_NEDER_MAX; ++i) {
        if (g_nedarim[i].estado == HASHMAL_NEDER_ESTADO_VACIO) return (int)i;
    }
    for (uint8_t i = 0; i < HASHMAL_NEDER_MAX; ++i) {
        hashmal_neder_estado_t e = g_nedarim[i].estado;
        if (e == HASHMAL_NEDER_ESTADO_CUMPLIDO
         || e == HASHMAL_NEDER_ESTADO_LIBERADO
         || e == HASHMAL_NEDER_ESTADO_VIOLADO) {
            return (int)i;
        }
    }
    return -1;
}

/* Cuenta votos activos (PENDIENTE/ACTIVO) de un nodo. */
static uint8_t contar_activos_de(uint8_t nodo_id)
{
    uint8_t c = 0;
    for (uint8_t i = 0; i < HASHMAL_NEDER_MAX; ++i) {
        const hashmal_neder_voto_t *v = &g_nedarim[i];
        if ((v->estado == HASHMAL_NEDER_ESTADO_PENDIENTE
          || v->estado == HASHMAL_NEDER_ESTADO_ACTIVO)
         && v->nodo_id == nodo_id) {
            ++c;
        }
    }
    return c;
}

/* ==================================================================
 * API - hashmal_neder_registrar (Nm 30:3 ki yiddor neder)
 * ================================================================== */
hashmal_roles_err_t hashmal_neder_registrar(uint8_t nodo_id,
                                             hashmal_neder_tipo_t tipo)
{
    if (!nodo_legitimo(nodo_id))    return HASHMAL_ROLES_ERR_NODO_NO_ENCONTRADO;
    if (!tipo_legitimo(tipo)) {
        NDR_LOG_E("registrar: tipo %d invalido", (int)tipo);
        return HASHMAL_ROLES_ERR_NEDER;
    }

    /* Anti-duplicado: un solo voto activo del mismo tipo por nodo. */
    if (buscar_voto_activo(nodo_id, tipo) >= 0) {
        NDR_LOG_W("registrar: nodo %u ya tiene voto %s activo - duplicado",
                  (unsigned)nodo_id, nombre_tipo(tipo));
        return HASHMAL_ROLES_ERR_NEDER;
    }

    int slot = buscar_slot_libre();
    if (slot < 0) {
        NDR_LOG_E("registrar: tabla llena (%u slots, terminales no "
                  "purgados)", (unsigned)HASHMAL_NEDER_MAX);
        return HASHMAL_ROLES_ERR_NEDER;
    }

    int64_t ahora = hashmal_roles_ahora_ms();
    g_nedarim[slot].nodo_id               = nodo_id;
    g_nedarim[slot].tipo                  = tipo;
    /* Ec 5:4 al te'ajer - de PENDIENTE pasa a ACTIVO en el mismo
     * acto. La fase PENDIENTE existe semanticamente pero es de
     * duracion cero salvo que el caller la fuerce con otra API. */
    g_nedarim[slot].estado                = HASHMAL_NEDER_ESTADO_ACTIVO;
    g_nedarim[slot].timestamp_inicio_ms   = ahora;
    g_nedarim[slot].timestamp_terminal_ms = 0;

    NDR_LOG_I("registrar: nodo %u tipo %s slot %d - lo yajel devaro "
              "(Nm 30:3 kekhol hayyotse mippiv ya'aseh)",
              (unsigned)nodo_id, nombre_tipo(tipo), slot);
    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 * API - hashmal_neder_cumplir (Sal 116:14 ashallem nedaray)
 * ================================================================== */
hashmal_roles_err_t hashmal_neder_cumplir(uint8_t nodo_id,
                                           hashmal_neder_tipo_t tipo)
{
    if (!nodo_legitimo(nodo_id) || !tipo_legitimo(tipo)) {
        return HASHMAL_ROLES_ERR_NEDER;
    }

    int idx = buscar_voto_activo(nodo_id, tipo);
    if (idx < 0) {
        NDR_LOG_W("cumplir: nodo %u tipo %s sin voto activo",
                  (unsigned)nodo_id, nombre_tipo(tipo));
        return HASHMAL_ROLES_ERR_NEDER;
    }

    g_nedarim[idx].estado                = HASHMAL_NEDER_ESTADO_CUMPLIDO;
    g_nedarim[idx].timestamp_terminal_ms = hashmal_roles_ahora_ms();
    g_cumplidos_total                   += 1u;

    NDR_LOG_I("cumplir: nodo %u tipo %s -> %s (#%" PRIu32 " cumplidos "
              "totales)",
              (unsigned)nodo_id, nombre_tipo(tipo),
              nombre_estado(HASHMAL_NEDER_ESTADO_CUMPLIDO),
              g_cumplidos_total);
    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 * API - hashmal_neder_liberar (Nm 30:5 hefer)
 * ================================================================== */
hashmal_roles_err_t hashmal_neder_liberar(uint8_t nodo_id,
                                           hashmal_neder_tipo_t tipo)
{
    if (!nodo_legitimo(nodo_id) || !tipo_legitimo(tipo)) {
        return HASHMAL_ROLES_ERR_NEDER;
    }

    int idx = buscar_voto_activo(nodo_id, tipo);
    if (idx < 0) {
        NDR_LOG_W("liberar: nodo %u tipo %s sin voto activo",
                  (unsigned)nodo_id, nombre_tipo(tipo));
        return HASHMAL_ROLES_ERR_NEDER;
    }

    g_nedarim[idx].estado                = HASHMAL_NEDER_ESTADO_LIBERADO;
    g_nedarim[idx].timestamp_terminal_ms = hashmal_roles_ahora_ms();
    g_liberados_total                   += 1u;

    NDR_LOG_I("liberar: nodo %u tipo %s -> LIBERADO por autoridad "
              "legitima (Nm 30:5 hefer; #%" PRIu32 " liberados)",
              (unsigned)nodo_id, nombre_tipo(tipo), g_liberados_total);
    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 * API - hashmal_neder_violar (Ec 5:5 tov asher lo tiddor)
 * ================================================================== */
hashmal_roles_err_t hashmal_neder_violar(uint8_t nodo_id,
                                          hashmal_neder_tipo_t tipo)
{
    if (!nodo_legitimo(nodo_id) || !tipo_legitimo(tipo)) {
        return HASHMAL_ROLES_ERR_NEDER;
    }

    int idx = buscar_voto_activo(nodo_id, tipo);
    if (idx < 0) {
        NDR_LOG_W("violar: nodo %u tipo %s sin voto activo",
                  (unsigned)nodo_id, nombre_tipo(tipo));
        return HASHMAL_ROLES_ERR_NEDER;
    }

    g_nedarim[idx].estado                = HASHMAL_NEDER_ESTADO_VIOLADO;
    g_nedarim[idx].timestamp_terminal_ms = hashmal_roles_ahora_ms();
    g_violados_total                    += 1u;

    NDR_LOG_E("VIOLADO: nodo %u tipo %s rompio voto sin autorizacion "
              "(Ec 5:5 tov asher lo tiddor; #%" PRIu32 " violados); "
              "fase 2 -> firewall penalizacion",
              (unsigned)nodo_id, nombre_tipo(tipo), g_violados_total);
    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 * API - hashmal_neder_puede_prometer (Ec 5:5 - prudencia)
 * ================================================================== */
bool hashmal_neder_puede_prometer(uint8_t nodo_id)
{
    if (!nodo_legitimo(nodo_id)) return false;
    return contar_activos_de(nodo_id) < NEDER_MAX_ACTIVOS_POR_NODO;
}

/* ==================================================================
 * API - hashmal_neder_info (lectura pura)
 * ================================================================== */
hashmal_roles_err_t hashmal_neder_info(hashmal_neder_info_t *out)
{
    if (out == NULL) return HASHMAL_ROLES_ERR_NULL;

    uint8_t activos    = 0;
    uint8_t pendientes = 0;
    for (uint8_t i = 0; i < HASHMAL_NEDER_MAX; ++i) {
        switch (g_nedarim[i].estado) {
        case HASHMAL_NEDER_ESTADO_ACTIVO:    ++activos;    break;
        case HASHMAL_NEDER_ESTADO_PENDIENTE: ++pendientes; break;
        default: break;
        }
    }

    out->votos_activos    = activos;
    out->votos_pendientes = pendientes;
    out->cumplidos_total  = g_cumplidos_total;
    out->liberados_total  = g_liberados_total;
    out->violados_total   = g_violados_total;
    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 *
 *   "ish ki YIDDOR NEDER laYHWH... LO YAJEL DEVARO; KEKHOL
 *    HAYYOTSE MIPPIV YA'ASEH." - Numeros 30:3
 *
 *   "AL TE'AJER leshallemo." - Eclesiastes 5:4 / Deuteronomio 23:21
 *
 *   "TOV ASHER LO TIDDOR mishshettiddor velo teshallem." - Ec 5:5
 *
 *   "ASHALLEM NEDARAY laYHWH negda na lekhol 'ammo." - Sal 116:14
 *
 *   "NAI NAI, OU OU." - Mateo 5:37 / Santiago 5:12
 *
 *   El voto es voluntario - nadie obliga a prometer. Pero una vez
 *   pronunciado, es palabra del que invoca al Padre. Romperlo no
 *   es solo ineficiencia operativa - es PROFANACION (lo yajel,
 *   raiz jalal). Por eso la economia biblica del compromiso tiene
 *   tres puertas de salida: cumplir (Sal 116:14), liberar por
 *   autoridad (Nm 30:5), o violar (Ec 5:5 jet). La cuarta, la
 *   imposible, seria romper en silencio - eso no existe. Toda
 *   ruptura se registra; toda promesa se honra o se cuenta.
 *
 *   Mt 5:37 cierra el arco: la madurez del discipulo es no
 *   necesitar jurar. SI SI, NO NO. La palabra firme hace
 *   innecesario el voto. El firmware honra ese ideal aceptando
 *   votos voluntarios pero distinguiendo claramente quien cumple
 *   de quien no - sin votos vagos, sin "promesas ambiguas." En la
 *   red biblica, la palabra cuenta porque se contabiliza.
 *
 * ================================================================== */
