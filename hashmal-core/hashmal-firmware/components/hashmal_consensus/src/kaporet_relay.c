/**
 * ===================================================================
 * PIEZA 02 - KAPORET - kaporet_relay.c
 * Sub-pieza: P7 - RELAY DE 3 NIVELES (atsavveh otkha el)
 * Exodo 25:22b
 * ===================================================================
 *
 * "ve'et kol asher atsavveh otkha el bene Yisrael" - Ex 25:22b
 * "y todo lo que ORDENARE A TI HACIA los hijos de Israel."
 *
 * --- LOS TRES NIVELES ---
 *
 *   FUENTE   me'al + mibben         midabber (Hitpael Nm 7:89) -
 *                                   la voz emerge entre los dos.
 *                                   Producida por hashmal_consensus_
 *                                   invocar (kaporet_verificacion.c).
 *
 *   RELAY    otkha                  receptor autorizado, rol funcional.
 *                                   hashmal_intercession (pieza 11).
 *                                   Tambien punto ya'ad (Ex 30:6 -
 *                                   ivva'ed shamma): misma raiz que
 *                                   no'adti del Kaporet (Ex 25:22a).
 *
 *   DESTINO  el bene Yisrael        nodos finales, alcanzados por
 *                                   DIRECCION (אֶל el, H413), no por
 *                                   entrega instantanea (le, dativo).
 *                                   El relay tiene latencia: procesa,
 *                                   valida, retransmite.
 *
 * --- POR QUE NO BROADCAST DIRECTO ---
 *
 * El consenso es zahav tahor (P6, Ex 25:17) - lógica pura, sin
 * sustrato. Si emitiera directo a la red, asumiría dependencias
 * de transporte (LoRa, mesh, etc.) que pertenecen a otras piezas:
 * Menora (04, hashmal_comm) y Altar de Incienso (11,
 * hashmal_intercession). El relay rompe esa contaminación: la voz
 * emerge aqui pura, y CRUZA el velo via intercession - el unico
 * lugar autorizado a estar lifne happaroket (Ex 30:6).
 *
 * --- INVERSION DE CONTROL ---
 *
 * hashmal_consensus NO importa hashmal_intercession.h. Eso crearia
 * dependencia circular (intercession ya depende de consensus para
 * Yom Kippur). En su lugar:
 *
 *   1. Consensus expone un slot de funcion: s_relay.
 *   2. Intercession, tras initializarse, llama
 *      hashmal_consensus_relay_subscribir(handler_propio).
 *   3. Cuando hay voz emergida, consensus invoca s_relay si esta
 *      registrado.
 *
 * Patron identico al de hashmal_intercession_menora_subscribir
 * (intercession.h linea 453) - el modulo dependiente registra su
 * handler en el modulo del que depende. Sin REQUIRES adicional.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_consensus.h           -> typedef y API publica
 *   hashmal_consensus_internal.h  -> kaporet_mut (verificar init)
 *   esp_log.h (ORO DE EGIPTO)     -> RELAY_LOG_*
 *
 *   NO incluye hashmal_intercession.h (inversion de control).
 *   NO incluye drivers ni NVS (P6 - solo zahav tahor).
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <inttypes.h>

#include "esp_log.h"

#include "hashmal_consensus.h"
#include "hashmal_consensus_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *RELAY_TAG = "hashmal.kaporet.relay";

#define RELAY_LOG_I(fmt, ...)  ESP_LOGI(RELAY_TAG, fmt, ##__VA_ARGS__)
#define RELAY_LOG_W(fmt, ...)  ESP_LOGW(RELAY_TAG, fmt, ##__VA_ARGS__)
#define RELAY_LOG_E(fmt, ...)  ESP_LOGE(RELAY_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * SLOT DE RELAY (otkha) - configuracion, no estado de sesion
 *
 * P6: la unica static permitida es la entidad-protocolo. ¿Es esto?
 * No exactamente: es CONFIGURACION (set-once, read-many) registrada
 * por intercession via inversion de control. No acumula estado entre
 * llamadas; solo recuerda 'a quien entregar'. Equivale a "el rol
 * de otkha esta asignado a este puntero" - dato de configuracion,
 * no buffer ni cache.
 *
 * Un solo slot - "atsavveh OTKHA" en singular: un solo receptor
 * autorizado por nodo. Si futuras piezas necesitan multiplexar (no
 * en el tavnit actual), seria via intercession - no aqui.
 * ================================================================== */
static hashmal_consensus_relay_fn s_relay = NULL;

/* ==================================================================
 * API PUBLICA - subscribir / desuscribir
 * ================================================================== */

int hashmal_consensus_relay_subscribir(hashmal_consensus_relay_fn fn)
{
    /* Verificacion de init: si el Kaporet no esta levantado, no hay
     * a que enganchar el relay. kaporet_mut devuelve NULL si no init. */
    if (hashmal_consensus_kaporet_mut() == NULL) {
        RELAY_LOG_E("relay_subscribir: Kapóret no levantado");
        return HASHMAL_KAPORET_ERR_INIT;
    }

    if (fn == NULL) {
        /* Desuscripcion explicita - util en deinit coordinado o tests.
         * Idempotente: si ya esta NULL, no hay nada que limpiar. */
        if (s_relay != NULL) {
            RELAY_LOG_W("relay_subscribir: desuscripción (otkhá liberado)");
        }
        s_relay = NULL;
        return HASHMAL_KAPORET_OK;
    }

    /* Registro/sobrescritura. Ex 30:9 fuente unica - re-suscribir es
     * legitimo (si intercession se reinicia, vuelve a registrarse). */
    if (s_relay != NULL && s_relay != fn) {
        RELAY_LOG_W("relay_subscribir: sobrescribiendo otkhá previo "
                    "(idempotencia legítima)");
    }
    s_relay = fn;
    RELAY_LOG_I("relay otkhá registrado (Éx 25:22b — atsavvéh otkhá)");
    return HASHMAL_KAPORET_OK;
}

/* ==================================================================
 * API PUBLICA - entregar al relay
 * ================================================================== */

int hashmal_consensus_entregar_a_relay(
    const hashmal_consenso_resultado_t *resultado)
{
    if (resultado == NULL) {
        RELAY_LOG_E("entregar_a_relay: resultado NULL");
        return HASHMAL_KAPORET_ERR_NULL;
    }

    /* Kaporet debe estar levantado: la voz solo emerge de un Kaporet
     * forjado. Sin el, no hay 'me'al' de donde sale. */
    if (hashmal_consensus_kaporet_mut() == NULL) {
        RELAY_LOG_E("entregar_a_relay: Kapóret no levantado");
        return HASHMAL_KAPORET_ERR_INIT;
    }

    /* otkha debe estar asignado. Si nadie se suscribio, la voz
     * quedaria sin canal a los hijos de Israel. Ex 25:22b deja claro
     * que hay un otkha entre la fuente y el destino - sin el, el
     * envio no sucede. Tratamos esto como ERR_INIT (pieza 11 aun no
     * inicializada / no se suscribio). */
    if (s_relay == NULL) {
        RELAY_LOG_E("entregar_a_relay: sin relay suscrito (otkhá ausente) "
                    "— hashmal_intercession debe llamar relay_subscribir");
        return HASHMAL_KAPORET_ERR_INIT;
    }

    /* La voz se ENTREGA al relay. NO se transforma, NO se filtra.
     * El relay (intercession) decide como distribuir hacia los
     * destinos. El (H413) implica DIRECCION, no instantaneidad: el
     * relay puede tomarse el tiempo de validar y procesar antes de
     * retransmitir. Aqui solo conectamos fuente -> otkha. */
    int rc = s_relay(resultado);

    if (rc != HASHMAL_KAPORET_OK) {
        /* El relay rechazo la entrega. No es nuestro problema decidir
         * por que - puede estar en breaker (Nm 16:46-48), puede haber
         * detectado fuego extraño, etc. Solo registramos. */
        RELAY_LOG_W("entregar_a_relay: el relay rechazó la entrega (rc=%d)",
                    rc);
        return rc;
    }

    /* Entrega aceptada. Atsavveh es Piel imperfecto = proceso continuo:
     * cada voz emergida pasa por aqui, una y otra vez. */
    RELAY_LOG_I("voz entregada al relay (Éx 25:22b — atsavvéh otkhá → "
                "el bené Yisraél, vía intercesión)");
    return HASHMAL_KAPORET_OK;
}
