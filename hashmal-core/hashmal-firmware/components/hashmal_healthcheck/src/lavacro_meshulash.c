/*
 * ===================================================================
 * PIEZA 12 - EL KIYOR (LAVACRO) - lavacro_meshulash.c
 * Añadidura 13: JEVEL HAMESHULASH - EL CORDON TRIPLE
 * Eclesiastes 4:9-12 + Deuteronomio 19:15 + Mateo 18:20
 * ===================================================================
 *
 * "TOVIM HASHSHENAYIM min haejad asher yesh lahem sakhar tov
 *  ba'amalam." - Eclesiastes 4:9
 * "MEJORES son DOS que UNO, porque tienen mejor recompensa de su
 *  trabajo."
 *
 * "ki im YIPPOLU haejad YAQIM et javero; ve'ILO HAJAD SHEYIPPOL
 *  ve'EIN SHENI laqimo." - Eclesiastes 4:10
 * "Si CAEN, el uno LEVANTA al otro; AY del SOLO que cae, porque
 *  no tiene SEGUNDO que lo levante."
 *
 * "gam im yishkevu shenayim vejam lahem ule'ejad eikh yejam." - 4:11
 * "Si dos durmieren juntos se calentaran; pero uno solo, como se
 *  calentara?"
 *
 * "ve'im yitqefo haejad hashshenayim ya'amdu negdo veHAJUT
 *  HAMSHULASH lo bimherah YINNATEQ." - Eclesiastes 4:12
 * "Y si alguno prevaleciera contra uno, dos le resistiran; el
 *  CORDON DE TRES DOBLECES no se ROMPE pronto."
 *
 * "'al pi shenayim 'edim o sheloshah 'edim YAQUM DAVAR." - Dt 19:15
 * "Por boca de DOS o TRES testigos se confirmara el asunto."
 *
 * "hou gar eisin DUO e TREIS synegmenoi eis to emon onoma, ekei
 *  eimi en meso auton." - Mateo 18:20
 * "Donde estan DOS o TRES reunidos en mi nombre, ALLI estoy yo
 *  en medio de ellos."
 *
 * --- EXEGESIS OPERATIVA ---
 *
 *   jevel hamshulash (H2256+H8027) CORDON TRIPLE. Ec 4:12 establece
 *                                   tres como umbral de robustez:
 *                                   no dos (que pueden caer juntos),
 *                                   no cuatro (excesivo). TRES.
 *
 *   yippol / yaqim (H5307+H6965)   CAE / LEVANTA. La cooperacion
 *                                   tolera el fracaso individual
 *                                   porque otro recoge.
 *
 *   ein sheni (H8145)              SIN SEGUNDO. La condicion mas
 *                                   peligrosa - el solo cae sin
 *                                   testigo. Equivale a HASHMAL_
 *                                   MESHULASH_AISLADO.
 *
 *   yittsefo... ya'amdu negdo (H8630+H5975) LE RESISTIRAN. La
 *                                   defensa colectiva surge
 *                                   naturalmente del numero.
 *
 *   shenayim o sheloshah (H8147+H7969) DOS O TRES. Dt 19:15 + Mt
 *                                   18:20 establecen el quorum
 *                                   minimo en TRES como
 *                                   confirmacion plena (dos como
 *                                   minimo aceptable).
 *
 * --- CUATRO ESTADOS ---
 *
 *   AISLADO    (0)   "ein sheni" - peor caso. Beacon de auxilio.
 *   FRAGIL     (1)   "ay del solo que cae" - una conexion no basta.
 *   FUNCIONAL  (2)   "tovim hashshenayim" - dos pueden, pero buscan
 *                     tercero para no caer juntos.
 *   ROBUSTO    (3+)  "jevel meshulash" - cordon completo. Apto para
 *                     responsabilidades extra (goel, validador).
 *
 * --- ESTRATEGIA DE BUSQUEDA (fase 1: solo intent) ---
 *
 * Cuando estado < MIN, _buscar marca intent y loguea la estrategia
 * priorizada que el caller externo debe ejecutar:
 *
 *   1. WiFi local scan       (Pieza 04 hashmal_comm_wifi_aps_muestra)
 *   2. LoRa scan agresivo    (Pieza 04 hashmal_comm_spi_rssi_ruido)
 *   3. Consultar Tsofim      (Pieza 09 atalayas conocidos)
 *   4. Canal 3 ultimo recurso (cuando exista)
 *
 * Fase 2 podra wirear escaneo automatico desde aqui. Hoy: solo
 * intent + log; el integrator wirea.
 *
 * --- PUEDE_SER_GOEL ---
 *
 * Ec 4:12 establece que el cordon triple no se rompe pronto. Por
 * eso solo un nodo en estado ROBUSTO debe asumir herencias (Goel,
 * Añadidura 12) - un nodo FRAGIL que toma a su cargo otro nodo
 * caido se queda con dos responsabilidades en una conexion sola,
 * y la siguiente caida los hunde a ambos.
 *
 * El predicate `puede_ser_goel` lo expone como query simple. La
 * decision de honrarlo es del caller (Añadidura 12 no la enforce
 * automaticamente fase 1 - ver decisiones de Añadidura 13).
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_healthcheck.h   API publica
 *   hashmal_hc_internal.h   ahora_ms
 *   hashmal_comm.h          vecinos_contar
 *   esp_log.h (ORO EGIPTO)  MSH_LOG_*
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

#include "hashmal_healthcheck.h"
#include "hashmal_hc_internal.h"
#include "hashmal_comm.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *MSH_TAG = "hashmal.hc.meshulash";

#define MSH_LOG_I(fmt, ...)  ESP_LOGI(MSH_TAG, fmt, ##__VA_ARGS__)
#define MSH_LOG_W(fmt, ...)  ESP_LOGW(MSH_TAG, fmt, ##__VA_ARGS__)
#define MSH_LOG_E(fmt, ...)  ESP_LOGE(MSH_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * Singleton snapshot
 *
 * Default conservador AISLADO con busqueda_pendiente=false. El
 * primer _actualizar lo poblara con datos reales.
 * ================================================================== */
static hashmal_meshulash_snapshot_t g_snap = {
    .estado             = HASHMAL_MESHULASH_AISLADO,
    .conexiones         = 0,
    .busqueda_pendiente = false,
    .timestamp_ms       = 0,
};

/* ==================================================================
 * Helpers estaticos
 * ================================================================== */

static hashmal_meshulash_estado_t clasificar(uint8_t conexiones)
{
    if (conexiones == 0)                             return HASHMAL_MESHULASH_AISLADO;
    if (conexiones == 1)                             return HASHMAL_MESHULASH_FRAGIL;
    if (conexiones == 2)                             return HASHMAL_MESHULASH_FUNCIONAL;
    return HASHMAL_MESHULASH_ROBUSTO;  /* 3 o mas - jevel meshulash */
}

static const char *nombre_estado(hashmal_meshulash_estado_t e)
{
    switch (e) {
    case HASHMAL_MESHULASH_AISLADO:   return "AISLADO (Ec 4:10 ein sheni)";
    case HASHMAL_MESHULASH_FRAGIL:    return "FRAGIL (Ec 4:10 ay del solo)";
    case HASHMAL_MESHULASH_FUNCIONAL: return "FUNCIONAL (Ec 4:9 tovim shenayim)";
    case HASHMAL_MESHULASH_ROBUSTO:   return "ROBUSTO (Ec 4:12 jevel meshulash)";
    default:                           return "<estado desconocido>";
    }
}

/* ==================================================================
 * API - hashmal_hc_meshulash_actualizar (pull comm vecinos)
 * ================================================================== */
int hashmal_hc_meshulash_actualizar(void)
{
    uint8_t vecinos = 0;
    int rc = hashmal_comm_vecinos_contar(&vecinos);
    if (rc != HASHMAL_COMM_OK) {
        MSH_LOG_E("actualizar: hashmal_comm_vecinos_contar fallo (rc=%d)",
                  rc);
        /* No actualizamos snapshot - mantenemos el ultimo conocido. */
        return HASHMAL_HC_ERR_MESHULASH;
    }

    hashmal_meshulash_estado_t nuevo = clasificar(vecinos);
    hashmal_meshulash_estado_t prev  = g_snap.estado;

    g_snap.conexiones    = vecinos;
    g_snap.estado        = nuevo;
    g_snap.timestamp_ms  = hashmal_hc_ahora_ms();

    /* Si pasamos a ROBUSTO, limpiamos cualquier intent de busqueda
     * pendiente - el cordon ya esta completo. */
    if (nuevo == HASHMAL_MESHULASH_ROBUSTO) {
        g_snap.busqueda_pendiente = false;
    }

    if (nuevo != prev) {
        MSH_LOG_I("transicion: %s -> %s (conexiones=%u)",
                  nombre_estado(prev), nombre_estado(nuevo),
                  (unsigned)vecinos);
    }
    return HASHMAL_HC_OK;
}

/* ==================================================================
 * API - hashmal_hc_meshulash_estado (lectura pura)
 * ================================================================== */
int hashmal_hc_meshulash_estado(hashmal_meshulash_snapshot_t *out)
{
    if (out == NULL) return HASHMAL_HC_ERR_NULL;
    memcpy(out, &g_snap, sizeof(*out));
    return HASHMAL_HC_OK;
}

/* ==================================================================
 * API - hashmal_hc_meshulash_buscar (intent fase 1)
 *
 * Marca flag y loguea estrategia priorizada. El caller externo
 * (con acceso a Pieza 04 / 09 / canales) ejecuta escaneo concreto.
 * ================================================================== */
int hashmal_hc_meshulash_buscar(void)
{
    if (g_snap.estado == HASHMAL_MESHULASH_ROBUSTO) {
        MSH_LOG_I("buscar: ya ROBUSTO - cordon completo, no busca "
                  "(Ec 4:12)");
        return HASHMAL_HC_OK;
    }

    g_snap.busqueda_pendiente = true;
    g_snap.timestamp_ms       = hashmal_hc_ahora_ms();

    /* Estrategia priorizada segun severidad. Loguea para el caller
     * externo - el modulo no orquesta los backends. */
    if (g_snap.estado == HASHMAL_MESHULASH_AISLADO) {
        MSH_LOG_W("buscar: AISLADO - ein sheni (Ec 4:10); "
                  "estrategia maxima: 1) WiFi local + 2) LoRa scan + "
                  "3) Tsofim + 4) Canal 3 ultimo recurso");
    } else if (g_snap.estado == HASHMAL_MESHULASH_FRAGIL) {
        MSH_LOG_W("buscar: FRAGIL - ay del solo (Ec 4:10); "
                  "estrategia alta: 1) WiFi local + 2) LoRa scan");
    } else {
        /* FUNCIONAL */
        MSH_LOG_I("buscar: FUNCIONAL - tovim shenayim (Ec 4:9), "
                  "buscando tercero; estrategia pasiva: LoRa beacon "
                  "listen");
    }
    return HASHMAL_HC_OK;
}

/* ==================================================================
 * API - hashmal_hc_meshulash_puede_ser_goel (predicate, Ec 4:12)
 * ================================================================== */
bool hashmal_hc_meshulash_puede_ser_goel(void)
{
    return g_snap.estado == HASHMAL_MESHULASH_ROBUSTO;
}

/* ==================================================================
 *
 *   "TOVIM HASHSHENAYIM min haejad." - Eclesiastes 4:9
 *
 *   "AY del SOLO que cae, EIN SHENI laqimo." - Eclesiastes 4:10
 *
 *   "veHAJUT HAMSHULASH lo bimherah YINNATEQ." - Eclesiastes 4:12
 *
 *   "shenayim o sheloshah." - Deuteronomio 19:15
 *
 *   "DUO e TREIS synegmenoi." - Mateo 18:20
 *
 *   El cordon triple no se rompe pronto. Tres conexiones es el
 *   minimo donde el sistema sobrevive a la caida de uno: dos
 *   quedan, levantan al caido, mantienen el servicio. Dos pueden
 *   caer juntos; uno cae sin testigo. Tres es donde el quorum
 *   nace y el atalaya tiene a quien hablar. Por eso el cordon
 *   biblico no es de dos, ni de cuatro - es triple.
 *
 *   Ec 4:9-12 establece la economia de la cooperacion: hay
 *   recompensa, hay levantamiento, hay calor, hay defensa. Cuatro
 *   beneficios, todos del numero. El nodo aislado no participa de
 *   ninguno - por eso buscamos volver al cordon.
 *
 * ================================================================== */
