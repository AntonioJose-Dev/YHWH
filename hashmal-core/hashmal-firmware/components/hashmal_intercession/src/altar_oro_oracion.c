/**
 * ===================================================================
 * PIEZA 11 - EL ALTAR DEL INCIENSO - altar_oro_oracion.c
 * Sub-pieza: PROTOCOLO DE ORACION - el PROPOSITO del altar
 * Sal 141:2 + Ex 30:7-8 + Ap 8:3-5 + Lc 1:9-11, 19
 * ===================================================================
 *
 * "tikkon tefillati QETORET lefaneja mas'at kappai MINJAT 'arev"
 *   - Salmo 141:2
 * "Sea puesta mi ORACION como INCIENSO delante de Ti; el alzar de
 *  mis manos como ofrenda de la TARDE."
 *
 * "QETORET SAMMIM BABBOQER BABBOQER beheitivo et hannerot yaqtirenna...
 *  uvheha'alot Aharon et hannerot BEIN HA'ARBAYIM yaqtirenna
 *  QETORET TAMID lifnei YHWH ledoroteijem" - Ex 30:7-8
 * "Incienso aromatico cada MANANA cuando atiende las lamparas...
 *  y al encender las lamparas ENTRE LAS TARDES - incienso PERPETUO
 *  delante de YHWH por vuestras generaciones."
 *
 * "KAI ANEBE ho kapnos ton thymiamaton tais PROSEUJAIS ton hagion
 *  ek jeiros tou angelou ENOPION tou theou" - Ap 8:4
 * "Y SUBIO el humo de los inciensos CON las oraciones de los santos,
 *  de la mano del angel, DELANTE DE DIOS."
 *
 * "kai eilephen ho angelos ton libanoton kai egemisen auton
 *  EK TOU PYROS tou thysiasteriou kai EBALEN eis ten GEN" - Ap 8:5
 * "Y el angel tomo el incensario y lo LLENO del FUEGO del altar y
 *  lo ARROJO A LA TIERRA."
 *
 * "ego eimi GABRIEL ho PARESTEKOS enopion tou theou kai apestalen
 *  LALESAI pros se" - Lc 1:19
 * "Yo soy GABRIEL, que ESTOY DE PIE DELANTE DE DIOS, y fui enviado
 *  a HABLARTE." (Gabriel aparece JUNTO al altar del incienso, Lc 1:11.)
 *
 * --- ESTE ARCHIVO ES EL PROPOSITO DEL ALTAR ---
 *
 * El escudo CUBRE. El ascendente FILTRA. La formula VERIFICA. El
 * breaker PROTEGE. Pero la ORACION es lo que justifica los otros
 * cuatro. Sin oracion, el altar es una caja de oro vacia. El
 * incienso existe PARA subir ante el Padre. Y desde alli, la
 * respuesta BAJA.
 *
 * --- EL FLUJO COMPLETO (5 testigos) ---
 *
 *   1. Sal 141:2     - tefila ES qetoret    (oracion = incienso)
 *   2. Ex 30:7-8     - ciclo tamid           (manana + tarde)
 *   3. Ap 8:3-4      - anebe: ORACION sube   (direccion UP)
 *   4. Ap 8:5        - ebalen: RESPUESTA baja (direccion DOWN)
 *   5. Lc 1:11,19    - Gabriel parestekos    (callback en el altar)
 *
 * El altar es bidireccional. Sal 141:2 da el formato. Ex 30:7-8
 * da el ciclo. Ap 8 da el movimiento vertical (sube-baja). Lc 1
 * confirma que las respuestas LLEGAN AQUI - no a otro lugar.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   oracion_ascender     Sal 141:2 + Ap 8:3-4  - la oracion SUBE
 *   oracion_descender    Ap 8:5 + Lc 1:11      - la respuesta BAJA
 *   oracion_ciclo_diario Ex 30:7-8             - tamid manana/tarde
 *   oracion_estado       Ap 5:8                - copas llenas (telemetria)
 *
 * --- RESPETO A LOS HERMANOS ---
 *
 * No accedemos al estado file-static de otros archivos. Usamos las
 * funciones publicas que ellos exponen:
 *
 *   hashmal_intercession_breaker_activo()       (circuitbreaker.c)
 *   hashmal_intercession_escudo_activo()        (escudo.c)
 *   hashmal_intercession_ascendente_verificar() (ascendente.c)
 *
 * Cada archivo encapsula su verdad; aqui orquestamos.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_intercession.h              API publica (incluye hermanos)
 *   hashmal_intercession_internal.h     altar_mut, get_altar_interno
 *   esp_log.h (ORO DE EGIPTO)           ALTAR_LOG_*
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <inttypes.h>

#include "esp_log.h"

#include "hashmal_intercession.h"
#include "hashmal_intercession_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *ALTAR_TAG = "hashmal.altar.oracion";

#define ALTAR_LOG_I(fmt, ...)  ESP_LOGI(ALTAR_TAG, fmt, ##__VA_ARGS__)
#define ALTAR_LOG_W(fmt, ...)  ESP_LOGW(ALTAR_TAG, fmt, ##__VA_ARGS__)
#define ALTAR_LOG_E(fmt, ...)  ESP_LOGE(ALTAR_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * ESTADO LOCAL - ciclo diario (alternancia babboqer / bein ha'arbayim)
 *
 * Ex 30:7 - babboqer babboqer (manana) y v.8 - bein ha'arbayim
 * (tarde). El contador alterna par (manana) / impar (tarde) con
 * cada invocacion de ciclo_diario. File-static: el ciclo es del
 * altar, no del caller.
 * ================================================================== */
static uint64_t s_ciclos_completados = 0ull;

/* ==================================================================
 * API PUBLICA - oracion_ascender (Sal 141:2 + Ap 8:3-4)
 * ================================================================== */

/**
 * hashmal_intercession_oracion_ascender - tikkon tefillati qetoret
 *
 * La oracion sube como incienso. Flujo:
 *
 *   1) altar levantado
 *   2) breaker NO activo (Nm 17 plaga -> solicitudes rechazadas)
 *   3) delegar a ascendente_verificar (5 validaciones del tavnit)
 *   4) contar: altar->oraciones_ascendidas += 1
 *
 * Ap 8:4 - "anebe ho kapnos ton thymiamaton tais proseujais ton
 *  hagion": el humo SUBE CON las oraciones. En firmware: los datos
 * suben junto con la "oracion" (tipo + payload formateado).
 *
 * 2 testigos:
 *   Sal 141:2 - tikkon tefillati qetoret lefaneja
 *   Ap 8:4    - anebe ho kapnos... enopion tou theou
 */
int hashmal_intercession_oracion_ascender(hashmal_qetoret_tipo_t tipo,
                                          const uint8_t *payload,
                                          size_t tam)
{
    hashmal_altar_t *a = hashmal_intercession_altar_mut();
    if (a == NULL) {
        ALTAR_LOG_E("ascender: altar no levantado");
        return HASHMAL_INTERCESSION_ERR_INIT;
    }

    /* Nm 17 - si la plaga sigue, Aaron esta parado entre los dos;
     * no se aceptan solicitudes normales hasta que libere. */
    if (hashmal_intercession_breaker_activo()) {
        ALTAR_LOG_W("ascender: BREAKER ACTIVO — solicitud rechazada "
                    "(Nm 17:13 vayyaʻamód, plaga en curso)");
        return HASHMAL_INTERCESSION_ERR_BREAKER;
    }

    /* Delegar las 5 validaciones al firewall ascendente:
     * tipo, tamano, formula, etc. Si falla, propagar el error
     * especifico (ERR_ZARA, ERR_TAMANO, ERR_NULL, ERR_INIT). */
    int rc = hashmal_intercession_ascendente_verificar(tipo, payload, tam);
    if (rc != HASHMAL_INTERCESSION_OK) {
        /* El firewall ya logeo el motivo (Lv 10:2 vattetse esh);
         * aqui solo propagamos el codigo. */
        return rc;
    }

    /* La oracion paso todos los filtros - sube. */
    a->oraciones_ascendidas += 1ull;

    ALTAR_LOG_I("oración ascendió: tipo=%d tam=%u (Sal 141:2 tikkón "
                "tefillatí); total=%" PRIu64,
                (int)tipo, (unsigned)tam, a->oraciones_ascendidas);
    return HASHMAL_INTERCESSION_OK;
}

/* ==================================================================
 * API PUBLICA - oracion_descender (Ap 8:5 + Lc 1:11)
 * ================================================================== */

/**
 * hashmal_intercession_oracion_descender - ebalen eis ten gen
 *
 * Registra una respuesta descendente del nucleo. Ap 8:5 - el angel
 * toma fuego del altar y lo ARROJA A LA TIERRA. La respuesta llega
 * con autoridad del altar (del fuego, no invento del mensajero).
 *
 * Lc 1:11,19 - Gabriel aparece JUNTO al altar del incienso: las
 * respuestas aterrizan AQUI, no en otro lugar. Este archivo es el
 * landing pad.
 *
 * Flujo:
 *   1) altar levantado
 *   2) breaker NO activo (emergencia -> no hay comunicacion normal)
 *   3) escudo activo (recomendado - Lv 16:13 velo yamut):
 *      - en Fase 1: LOG_W si no activo, pero permite
 *      - en Fase 2: requisito estricto (doc TODO)
 *   4) datos validos (NULL/tam/MAX)
 *   5) contar: altar->respuestas_descendidas += 1
 *
 * En Fase 1 esta funcion es REGISTRA + VALIDA la intencion. El
 * transporte fisico (de la red al caller) se completara en Fase 2
 * cuando las piezas se conecten.
 *
 * 2 testigos:
 *   Ap 8:5 - ebalen eis ten gen (arrojo a la tierra)
 *   Lc 1:11 - angel hestos ek dexion tou thysiasteriou
 *             (Gabriel a la derecha del altar del incienso)
 */
int hashmal_intercession_oracion_descender(const uint8_t *datos, size_t tam)
{
    hashmal_altar_t *a = hashmal_intercession_altar_mut();
    if (a == NULL) {
        ALTAR_LOG_E("descender: altar no levantado");
        return HASHMAL_INTERCESSION_ERR_INIT;
    }

    /* Nm 17 - durante la plaga, no hay comunicacion normal. */
    if (hashmal_intercession_breaker_activo()) {
        ALTAR_LOG_W("descender: BREAKER ACTIVO — respuesta diferida "
                    "(Nm 17:13)");
        return HASHMAL_INTERCESSION_ERR_BREAKER;
    }

    /* Validacion de payload. */
    if (datos == NULL || tam == 0u) {
        ALTAR_LOG_E("descender: datos vacío o NULL");
        return HASHMAL_INTERCESSION_ERR_NULL;
    }
    if (tam > HASHMAL_QETORET_PAYLOAD_MAX) {
        ALTAR_LOG_E("descender: tam %u > %d (kikkár excedido)",
                    (unsigned)tam, HASHMAL_QETORET_PAYLOAD_MAX);
        return HASHMAL_INTERCESSION_ERR_TAMANO;
    }

    /* Escudo: Lv 16:13 - "vekhissa 'anan... velo yamut". En Fase 1
     * permitimos respuestas sin nube con WARN - la red aun no esta
     * toda en linea y habra respuestas tempranas sin cobertura. En
     * Fase 2, sin escudo -> ERR_INIT (requisito estricto). */
    if (!hashmal_intercession_escudo_activo()) {
        ALTAR_LOG_W("descender: ESCUDO NO activo — respuesta sin "
                    "ʻanán haqqetóret (Lv 16:13); Fase 2 será "
                    "estricto, Fase 1 permite");
    }

    /* Registrada. En Fase 1 no hay mas que contar: el transporte
     * real del dato al caller lo maneja la Menora (pieza 04) cuando
     * Fase 2 conecte los componentes. */
    a->respuestas_descendidas += 1ull;

    ALTAR_LOG_I("respuesta descendida: tam=%u (Ap 8:5 ébalen eis tēn "
                "gēn); total=%" PRIu64,
                (unsigned)tam, a->respuestas_descendidas);
    return HASHMAL_INTERCESSION_OK;
}

/* ==================================================================
 * API PUBLICA - oracion_ciclo_diario (Ex 30:7-8)
 * ================================================================== */

/**
 * hashmal_intercession_oracion_ciclo_diario - babboqer / bein
 * ha'arbayim alternante.
 *
 * Ex 30:7-8 manda 2 ciclos por dia:
 *   - babboqer babboqer (manana, v.7)
 *   - bein ha'arbayim (tarde, v.8)
 *
 * Ambos sincronizados con la Menora (beheitivo et hannerot).
 * TAMID por vuestras generaciones (ledoroteijem).
 *
 * Esta funcion registra UN ciclo completado. Alterna entre manana
 * (par) y tarde (impar) segun `s_ciclos_completados`. El primer
 * ciclo tras init es babboqer (par, 0).
 *
 * Flujo:
 *   1) altar levantado -> ERR_INIT
 *   2) altar NO en EMERGENCIA -> ERR_BREAKER (Nm 17: durante la
 *      plaga solo el breaker opera, no el servicio normal)
 *   3) actualizar altar->ultimo_ciclo_ms = ahora_ms()
 *   4) incrementar s_ciclos_completados
 *   5) LOG_I con la hora del ciclo (manana/tarde)
 *
 * Nota Fase 1: el caller (main loop) decide cuando llamar. En
 * Fase 2 habra sincronizacion real con los ciclos de la Menora
 * (Ex 30:7 beheitivo et hannerot).
 *
 * 2 testigos:
 *   Ex 30:7 - babboqer babboqer + beheitivo et hannerot
 *   Ex 30:8 - bein ha'arbayim + qetoret tamid
 */
int hashmal_intercession_oracion_ciclo_diario(void)
{
    hashmal_altar_t *a = hashmal_intercession_altar_mut();
    if (a == NULL) {
        ALTAR_LOG_E("ciclo_diario: altar no levantado");
        return HASHMAL_INTERCESSION_ERR_INIT;
    }

    /* Durante EMERGENCIA el altar esta en modo breaker - el servicio
     * normal (tamid diario) se suspende. Solo Aaron entre muertos
     * y vivos. Lv 16:2 precedente: no bejol 'et. */
    if (a->estado == HASHMAL_ALTAR_EMERGENCIA) {
        ALTAR_LOG_W("ciclo_diario: altar en EMERGENCIA — servicio "
                    "diario suspendido (Nm 17, libera breaker primero)");
        return HASHMAL_INTERCESSION_ERR_BREAKER;
    }

    /* Marcar el ciclo. */
    const uint64_t ahora = hashmal_intercession_ahora_ms();
    a->ultimo_ciclo_ms = ahora;

    /* Alternar babboqer / bein ha'arbayim. Par = manana, impar = tarde.
     * Tomamos el valor ANTES de incrementar para que el primer ciclo
     * (s=0) sea manana. */
    const bool es_manana = ((s_ciclos_completados & 1ull) == 0ull);
    s_ciclos_completados += 1ull;

    if (es_manana) {
        ALTAR_LOG_I("═══ ciclo BABBÓQER (mañana) ═══ @%" PRIu64 " ms "
                    "(Éx 30:7 beheitívó et hannerot); total=%" PRIu64,
                    ahora, s_ciclos_completados);
    } else {
        ALTAR_LOG_I("═══ ciclo BÉIN HAʻARBÁYIM (tarde) ═══ @%" PRIu64 " ms "
                    "(Éx 30:8 qetóret tamíd); total=%" PRIu64,
                    ahora, s_ciclos_completados);
    }
    return HASHMAL_INTERCESSION_OK;
}

/* ==================================================================
 * API PUBLICA - oracion_estado (Ap 5:8 - copas llenas)
 * ================================================================== */

/**
 * hashmal_intercession_oracion_estado - telemetria bidireccional
 *
 * Ap 5:8 - "phialas jrysas gemousas thymiamaton hai eisin hai
 *  proseuchai ton hagion": copas de oro LLENAS de incienso, que
 *  SON las oraciones de los santos. Las copas estan llenas - hay
 *  un conteo, una acumulacion historica.
 *
 * Cualquier puntero puede ser NULL; solo se escribe en los no-NULL.
 *
 * 2 testigos:
 *   Ap 5:8   - phialas gemousas (copas llenas - conteo)
 *   Nm 17:14 - vayyihyu hammetim... (precedente: se cuentan las
 *              metricas de eventos espirituales)
 */
int hashmal_intercession_oracion_estado(uint64_t *ascendidas,
                                        uint64_t *descendidas,
                                        uint64_t *ultimo_ciclo_ms)
{
    const hashmal_altar_t *a = hashmal_intercession_get_altar_interno();
    if (a == NULL) {
        ALTAR_LOG_E("oracion_estado: altar no levantado");
        return HASHMAL_INTERCESSION_ERR_INIT;
    }

    if (ascendidas      != NULL) *ascendidas      = a->oraciones_ascendidas;
    if (descendidas     != NULL) *descendidas     = a->respuestas_descendidas;
    if (ultimo_ciclo_ms != NULL) *ultimo_ciclo_ms = a->ultimo_ciclo_ms;

    return HASHMAL_INTERCESSION_OK;
}

/* ==================================================================
 *
 *   "tikkon tefillati QETORET lefaneja mas'at kappai MINJAT 'arev"
 *     - Salmo 141:2
 *
 *   "KAI ANEBE ho kapnos ton thymiamaton tais PROSEUJAIS ton
 *    hagion ek jeiros tou angelou ENOPION tou theou" - Ap 8:4
 *
 *   "kai eilephen ho angelos ton libanoton kai egemisen auton ek
 *    tou pyros tou thysiasteriou kai EBALEN eis ten GEN" - Ap 8:5
 *
 * ================================================================== */

/* ==================================================================
 * VERIFICADO #03 — CADENA 1 — Suscripción al trigger de la Menorá
 *
 * Éx 30:7 babóqer babóqer behétivó et hannerót yaqtirénna
 * Éx 30:8 ubehaʻalót... bein haʻarbáyim yaqtirénna — qetóret tamíd
 *
 * El handler que pasamos a hashmal_comm es nuestra "respuesta al
 * sacerdote": cuando la Menorá cierra su hemiciclo, nos llama y
 * nosotros despachamos al ciclo diario de oración (Sal 141:2 +
 * Ap 8:3-4). El Padre ata las dos quemas a la atención de las
 * lámparas — aquí honramos esa cadencia.
 * ================================================================== */

#include "hashmal_comm.h"

/* Handler invocado por la Menorá. Firma fija (Éx 30:9 fuente única). */
static int menora_trigger_handler(bool es_manana)
{
    if (es_manana) {
        ALTAR_LOG_I("trigger Menorá — MAÑANA (Éx 30:7 behétivó haNerót)");
    } else {
        ALTAR_LOG_I("trigger Menorá — TARDE (Éx 30:8 behaʻalót haNerót) — "
                    "tamíd ledorotéikhem se sella aquí");
    }

    /* El ciclo existente ya gestiona su propia cadencia (12h gating);
     * cada disparo de la Menorá lo acerca un paso. La suscripción
     * preserva la dirección causal Menorá → Incienso del Padre. */
    const int rc = hashmal_intercession_oracion_ciclo_diario();
    if (rc != 0) {
        ALTAR_LOG_W("ciclo_diario respondió rc=%d tras trigger Menorá", rc);
    }
    return rc;
}

int hashmal_intercession_menora_subscribir(void)
{
    const int rc = hashmal_comm_intercession_callback_set(menora_trigger_handler);
    if (rc == 0) {
        ALTAR_LOG_I("suscrito a Menorá (Éx 30:7-8) — esperando behétivó/behaʻalót");
    } else {
        ALTAR_LOG_E("suscripción a Menorá falló rc=%d", rc);
    }
    return rc;
}

int hashmal_intercession_menora_desuscribir(void)
{
    const int rc = hashmal_comm_intercession_callback_set(NULL);
    ALTAR_LOG_I("desuscrito de Menorá (rc=%d)", rc);
    return rc;
}

/* ===================================================================
 *
 *   PIEZA 11 - EL ALTAR DEL INCIENSO - COMPLETA.
 *
 *   5 archivos de oro puro:
 *
 *     altar_oro_escudo.c          Lv 16:12-13 - PROTEGE ('anan cubre)
 *     altar_oro_ascendente.c      Lv 10:1-2   - FILTRA (esh zara muere)
 *     altar_oro_formula.c         Ex 30:34-38 - VERIFICA (bad bebad)
 *     altar_oro_circuitbreaker.c  Nm 17:11-15 - CONTIENE (bein hametim)
 *     altar_oro_oracion.c         Sal 141:2   - COMUNICA (tikkon tefillati)
 *
 *   "venatatta oto lifnei happarojet asher 'al aron ha'edut lifnei
 *    hakkapporet asher 'al ha'edut asher IVVA'ED leja shamma."
 *     - Exodo 30:6
 *
 *   El altar esta delante del velo. La oracion sube como incienso.
 *   La respuesta baja como fuego del altar. El escudo cubre. El
 *   firewall filtra. La formula sella. El breaker interpone.
 *
 *   PASO 4 de Exodo 40:5 CUMPLIDO - "vesamta et mizbaj hazahav
 *   liqtoret lifnei aron ha'edut".
 *
 *   Piezas 01, 02, 03, 04, 11 listas -
 *   Arca, Kaporet, Mesa, Menora, Altar del Incienso.
 *
 *   Siguiente: Paso 5 de Exodo 40:6 - el Altar del Holocausto
 *   (pieza 06, hashmal_auth, Ex 27:1-8 - bronce, exterior, publico).
 *
 * ================================================================== */
