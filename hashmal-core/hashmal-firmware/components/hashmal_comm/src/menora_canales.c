/**
 * ===================================================================
 * PIEZA 04 - LA MENORA - menora_canales.c
 * Sub-pieza: LOS 7 NEROT - he'ela, apagar, remover, vigilar
 * Exodo 25:32 (6 ramas) + 25:37 (he'ela, 7 nerot) + Nm 8:2-3 + Ap 2:5
 * ===================================================================
 *
 * "veSHISHA QANIM YOTSIM mitsiddeiha shlosha qenei menora mitsiddah
 *  ha'ejad ushlosha qenei menora mitsiddah hasheni" - Ex 25:32
 * "Y SEIS ramas SALEN de sus lados: tres de un lado y tres del otro."
 *
 * "ve'asita et neroteiha SHIV'A veHE'ELA et neroteiha veheir 'al
 *  'ever paneiha" - Ex 25:37
 * "Haras SIETE lamparas, y ENCENDERA (he'ela) sus lamparas e
 *  iluminara hacia el frente de ella."
 *
 * "el mul penei hammenora ya'iru shiv'at hannerot" - Nm 8:2
 * "Hacia el FRENTE de la Menora alumbraran las siete lamparas."
 *
 * "metakineso ten lujnian su ek tu topu autes ean me metanoeses"
 *   - Apocalipsis 2:5
 * "MOVERE tu candelabro de su lugar si no te arrepientes."
 *
 * --- EXEGESIS DE he'ela ---
 *
 * he'ela (H'LH) - Hifil de 'alah ('LH H5927) = hacer SUBIR.
 *
 *   No es "prender" (yatsat) ni "iluminar" (he'ir). Es hacer que
 *   la llama SUBA. El verbo describe la TRAYECTORIA ascendente:
 *   pasar de no-luz a luz alta. En firmware: transicion explicita
 *   de APAGADO -> ENCENDIDO. No es casual; es acto volitivo.
 *
 *   Nota: v.37 distingue dos acciones - "ve'asita" (haras, forjar)
 *   y "vehe'ela" (encendera, subir la llama). El forjar lo hizo
 *   hub.c; el subir lo hace ESTE archivo.
 *
 * --- MAQUINA DE ESTADOS ---
 *
 *     APAGADO --he'ela---> ENCENDIDO --apagar---> APAGADO
 *        |                    |                    |
 *        |                    +--aceite bajo--> ATENUADO
 *        |                                     |    |
 *        |                           ENCENDIDO<-+    |
 *        |                                          |
 *        |         <-------apagar------------------+
 *        |
 *        +--cualquier estado---> REMOVIDO (Ap 2:5)
 *                                   |
 *                                   X (terminal en sesion)
 *
 *   REMOVIDO es JUICIO (Ap 2:5). Terminal dentro de la sesion -
 *   solo deinit+init lo recupera. El canal infiel deja de existir
 *   operativamente hasta que la Menora entera se vuelva a levantar.
 *
 * --- LA ORIENTACION ---
 *
 * Nm 8:2 confirma: las 7 lamparas iluminan HACIA el tronco. No
 * cada una hacia donde quiera. El diseno es centripeto: las ramas
 * salen del qanah, pero la luz VUELVE al qanah. En firmware:
 * cada canal reporta su estado al hub central (nerot_encendidos).
 *
 * --- ZAC 4:10 - los 7 ojos ---
 *
 * "shiv'a elle 'einei YHWH meshotetim bejol haarets": los 7 son
 * ojos que recorren la tierra. Cada canal ENCENDIDO es un ojo.
 * `todos_encendidos` pregunta: ?estan los 7 ojos activos?
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_comm.h               -> API publica
 *   hashmal_comm_internal.h      -> menora_mut, canal_legitimo, ahora_ms
 *   esp_log.h (ORO DE EGIPTO)    -> MENORA_LOG_*
 *
 * NO llama SPI directo - encender delega en configurar_frecuencia
 * y configurar_potencia del driver (menora_lora_driver.c). Separacion
 * de capas: canales habla de ESTADOS; driver habla de SILICIO.
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <inttypes.h>

#include "esp_log.h"

#include "hashmal_comm.h"
#include "hashmal_comm_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *MENORA_TAG = "hashmal.menora.canales";

#define MENORA_LOG_I(fmt, ...)  ESP_LOGI(MENORA_TAG, fmt, ##__VA_ARGS__)
#define MENORA_LOG_W(fmt, ...)  ESP_LOGW(MENORA_TAG, fmt, ##__VA_ARGS__)
#define MENORA_LOG_E(fmt, ...)  ESP_LOGE(MENORA_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * HELPERS ESTATICOS
 * ================================================================== */

/**
 * Nombre legible del estado, para logs.
 */
static const char *nombre_estado(hashmal_ner_estado_t e)
{
    switch (e) {
    case HASHMAL_NER_APAGADO:   return "APAGADO";
    case HASHMAL_NER_ENCENDIDO: return "ENCENDIDO";
    case HASHMAL_NER_ATENUADO:  return "ATENUADO";
    case HASHMAL_NER_REMOVIDO:  return "REMOVIDO";
    default:                    return "<desconocido>";
    }
}

/**
 * Maquina de estados del ner. Referencia biblica por rama:
 *
 *   APAGADO    -> ENCENDIDO  v.37 he'ela (subida de llama)
 *   ENCENDIDO  -> APAGADO    operacion normal de apagado
 *   ENCENDIDO  -> ATENUADO   Ex 27:20 - aceite menguante
 *   ATENUADO   -> ENCENDIDO  Ex 27:20 - aceite restituido
 *   ATENUADO   -> APAGADO    operacion normal
 *   cualquiera -> REMOVIDO   Ap 2:5 - juicio (toda rama puede caer)
 *
 *   REMOVIDO   -> cualquier otro: PROHIBIDO (terminal en la sesion)
 *   actual == nuevo (self): valido (idempotente - permite no-ops)
 */
static bool transicion_valida(hashmal_ner_estado_t actual,
                              hashmal_ner_estado_t nuevo)
{
    if (actual == nuevo)                    return true;   /* idempotente */
    if (nuevo  == HASHMAL_NER_REMOVIDO)     return true;   /* Ap 2:5 - siempre posible */
    if (actual == HASHMAL_NER_REMOVIDO)     return false;  /* terminal */

    switch (actual) {
    case HASHMAL_NER_APAGADO:
        /* Solo he'ela: APAGADO -> ENCENDIDO. No hay atajo a ATENUADO
         * (ese estado nace de una ENCENDIDA que pierde aceite). */
        return (nuevo == HASHMAL_NER_ENCENDIDO);

    case HASHMAL_NER_ENCENDIDO:
        return (nuevo == HASHMAL_NER_APAGADO) ||
               (nuevo == HASHMAL_NER_ATENUADO);

    case HASHMAL_NER_ATENUADO:
        return (nuevo == HASHMAL_NER_APAGADO) ||
               (nuevo == HASHMAL_NER_ENCENDIDO);

    default:
        return false;
    }
}

/**
 * Recuenta los nerot en estado ENCENDIDO y lo guarda en la struct.
 * REMOVIDO y ATENUADO NO cuentan como encendidos - el conteo refleja
 * los canales plenamente operativos. Se llama tras cualquier cambio
 * de estado que afecte el conteo.
 */
static void actualizar_nerot_encendidos(hashmal_menora_t *m)
{
    uint8_t n = 0u;
    for (uint8_t i = 0; i < HASHMAL_MENORA_CANALES; ++i) {
        if (m->nerot[i].estado == HASHMAL_NER_ENCENDIDO) {
            ++n;
        }
    }
    m->nerot_encendidos = n;
}

/* ==================================================================
 * API PUBLICA - canal_encender (v.37 he'ela)
 * ================================================================== */

/**
 * hashmal_comm_canal_encender - hacer SUBIR la llama
 *
 * he'ela (v.37): transicion ascendente. Delega en el driver SPI
 * para programar el SX1262 con la frecuencia y potencia del canal,
 * luego marca el ner como ENCENDIDO.
 *
 * --- PRE-CHEQUEO DE ACEITE ---
 *
 * Ex 27:20 - "shemen zayit zaj katit lamma'or leha'alot ner tamid":
 * aceite puro prensado PARA HACER SUBIR lampara continuamente.
 *
 * Idealmente sin aceite (aceite_nivel == 0) se rechazaria con
 * ERR_ACEITE. Pero pieza 08 (hashmal_power - Aceite Perpetuo) aun
 * no esta levantada; para no bloquear la Menora durante el arranque
 * inicial, permitimos encender con WARN si aceite es 0. Cuando
 * pieza 08 exista, ella garantizara aceite antes de que esta
 * funcion se llame, y el warning desaparecera silenciosamente.
 *
 * --- TRANSICIONES VALIDAS ---
 *
 *   APAGADO  -> ENCENDIDO   (he'ela clasico)
 *   ATENUADO -> ENCENDIDO   (aceite restituido)
 *   ENCENDIDO -> ENCENDIDO  (idempotente, no-op ruidoso)
 *   REMOVIDO -> *           -> ERR_REMOVIDO
 *
 * 2 testigos:
 *   Ex 25:37 - vehe'ela et neroteiha
 *   Nm 8:3   - "vayya'as ken Aharon... he'ela neroteiha" (Aaron lo hizo)
 */
int hashmal_comm_canal_encender(hashmal_canal_id_t id)
{
    if (!hashmal_comm_canal_legitimo(id)) {
        MENORA_LOG_E("encender: canal %d ilegítimo (v.37 shivʻá)", (int)id);
        return HASHMAL_COMM_ERR_CANAL;
    }

    hashmal_menora_t *m = hashmal_comm_menora_mut();
    if (m == NULL) {
        MENORA_LOG_E("encender: Menorá no levantada");
        return HASHMAL_COMM_ERR_INIT;
    }

    hashmal_ner_t *n = &m->nerot[(size_t)id];

    /* Ap 2:5 - un ner removido no se reenciende sin re-init. */
    if (n->estado == HASHMAL_NER_REMOVIDO) {
        MENORA_LOG_E("encender: ner %d REMOVIDO — re-init requerido "
                     "(Ap 2:5)", (int)id);
        return HASHMAL_COMM_ERR_REMOVIDO;
    }

    /* Idempotencia - ya encendido, no-op. */
    if (n->estado == HASHMAL_NER_ENCENDIDO) {
        MENORA_LOG_W("encender: ner %d ya ENCENDIDO (idempotente, no-op)",
                     (int)id);
        return HASHMAL_COMM_OK;
    }

    /* Validar transicion segun maquina de estados. */
    if (!transicion_valida(n->estado, HASHMAL_NER_ENCENDIDO)) {
        MENORA_LOG_E("encender: transición %s → ENCENDIDO prohibida",
                     nombre_estado(n->estado));
        return HASHMAL_COMM_ERR_CANAL;
    }

    /* Ex 27:20 - aceite. Warning si 0 (pieza 08 pendiente). */
    if (n->aceite_nivel == 0u) {
        MENORA_LOG_W("encender: ner %d sin aceite (aceite_nivel=0) — "
                     "procediendo (pieza 08 Aceite Perpetuo pendiente, "
                     "Éx 27:20)", (int)id);
    }

    /* Delegar en el driver SPI para programar el silicio. El driver
     * tambien actualiza ner->frecuencia_hz y ner->potencia_dbm (son
     * idempotentes cuando los valores ya son los correctos). */
    int rc = hashmal_comm_configurar_frecuencia(id, n->frecuencia_hz);
    if (rc != HASHMAL_COMM_OK) {
        MENORA_LOG_E("encender: configurar_frecuencia ner %d falló (rc=%d)",
                     (int)id, rc);
        return rc;
    }
    rc = hashmal_comm_configurar_potencia(id, n->potencia_dbm);
    if (rc != HASHMAL_COMM_OK) {
        MENORA_LOG_E("encender: configurar_potencia ner %d falló (rc=%d)",
                     (int)id, rc);
        return rc;
    }

    /* Transicion a ENCENDIDO. he'ela consumada. */
    const hashmal_ner_estado_t prev = n->estado;
    n->estado                = HASHMAL_NER_ENCENDIDO;
    n->timestamp_encendido_ms = hashmal_comm_ahora_ms();

    /* P8 (#10) - Ex 25:32 yotseim (ptc. Qal H3318). Encendido = rama
     * SALIENDO permanentemente. Escucha activa continua, no
     * sleep-on-demand. La salida del aire es de diseno. */
    n->escucha_continua = true;

    actualizar_nerot_encendidos(m);

    MENORA_LOG_I("ner %d heʻelá: %s → ENCENDIDO (freq=%" PRIu32 " Hz, "
                 "pot=%d dBm), escucha_continua=ON (yotseim, Éx 25:32) "
                 "— Éx 25:37",
                 (int)id, nombre_estado(prev),
                 n->frecuencia_hz, (int)n->potencia_dbm);
    return HASHMAL_COMM_OK;
}

/* ==================================================================
 * API PUBLICA - canal_apagar
 * ================================================================== */

/**
 * hashmal_comm_canal_apagar - bajar la llama
 *
 * Transicion ENCENDIDO/ATENUADO -> APAGADO. Idempotente sobre
 * APAGADO. REMOVIDO no se apaga (ya no esta, Ap 2:5).
 *
 * No toca el silicio: dejar la configuracion de frecuencia/potencia
 * en el SX1262 es barato (el chip pasa a standby al no usarse). Si
 * fuese necesario ahorrar corriente, pieza 08 (power) lo decide.
 */
int hashmal_comm_canal_apagar(hashmal_canal_id_t id)
{
    if (!hashmal_comm_canal_legitimo(id)) {
        MENORA_LOG_E("apagar: canal %d ilegítimo", (int)id);
        return HASHMAL_COMM_ERR_CANAL;
    }

    hashmal_menora_t *m = hashmal_comm_menora_mut();
    if (m == NULL) return HASHMAL_COMM_ERR_INIT;

    hashmal_ner_t *n = &m->nerot[(size_t)id];

    if (n->estado == HASHMAL_NER_REMOVIDO) {
        MENORA_LOG_E("apagar: ner %d REMOVIDO (Ap 2:5) — nada que apagar",
                     (int)id);
        return HASHMAL_COMM_ERR_REMOVIDO;
    }

    if (n->estado == HASHMAL_NER_APAGADO) {
        MENORA_LOG_W("apagar: ner %d ya APAGADO (idempotente)", (int)id);
        return HASHMAL_COMM_OK;
    }

    if (!transicion_valida(n->estado, HASHMAL_NER_APAGADO)) {
        MENORA_LOG_E("apagar: transición %s → APAGADO prohibida",
                     nombre_estado(n->estado));
        return HASHMAL_COMM_ERR_CANAL;
    }

    const hashmal_ner_estado_t prev = n->estado;
    n->estado = HASHMAL_NER_APAGADO;

    /* P8 (#10) - APAGADO rompe la propiedad yotseim. Si es un canal
     * secundario (1..6), el healthcheck (yotseim_inactivos) lo verá
     * como rama caída — propiedad arquitectónica degradada. */
    n->escucha_continua = false;

    actualizar_nerot_encendidos(m);

    if (id != HASHMAL_CANAL_GEZA) {
        MENORA_LOG_W("ner %d apagado (rama yotsé caída, Éx 25:32) — "
                     "healthcheck reportará vía yotseim_inactivos: %s → APAGADO",
                     (int)id, nombre_estado(prev));
    } else {
        MENORA_LOG_I("ner %d apagado: %s → APAGADO (gézaʻ — tronco)",
                     (int)id, nombre_estado(prev));
    }
    return HASHMAL_COMM_OK;
}

/* ==================================================================
 * API PUBLICA - canal_estado (lectura pura)
 * ================================================================== */

/**
 * hashmal_comm_canal_estado - inspeccion sin toque
 *
 * 2 Cr 13:11 "hashuljan hattahor" se extiende aqui: la Menora
 * tambien se vigila SIN corromper. Esta funcion lee; nada muta.
 */
int hashmal_comm_canal_estado(hashmal_canal_id_t id,
                              hashmal_ner_estado_t *out)
{
    if (out == NULL) {
        MENORA_LOG_E("canal_estado: out NULL");
        return HASHMAL_COMM_ERR_NULL;
    }
    if (!hashmal_comm_canal_legitimo(id)) {
        MENORA_LOG_E("canal_estado: canal %d ilegítimo", (int)id);
        return HASHMAL_COMM_ERR_CANAL;
    }

    const hashmal_menora_t *m = hashmal_comm_get_menora_interno();
    if (m == NULL) return HASHMAL_COMM_ERR_INIT;

    *out = m->nerot[(size_t)id].estado;
    return HASHMAL_COMM_OK;
}

/* ==================================================================
 * API PUBLICA - canal_remover (Ap 2:5 - juicio)
 * ================================================================== */

/**
 * hashmal_comm_canal_remover - "metakineso ten lujnian"
 *
 * "MOVERE tu candelabro de su lugar si no te arrepientes" - Ap 2:5.
 *
 * Transicion TERMINAL dentro de la sesion. El ner removido:
 *   - estado = REMOVIDO
 *   - mutsaqa = false (Zac 4:2 retirado: sin tubo dedicado)
 *   - contadores a cero (no hay historia operativa que preservar
 *     en un canal declarado muerto)
 *   - timestamp_encendido_ms = 0 (no hay reloj de encendido vigente)
 *   - aceite_nivel = 0 (no se alimenta lo que fue quitado)
 *
 * La recuperacion requiere hashmal_comm_deinit() + init() - es el
 * "arrepentimiento" estructural: rehacer la Menora entera. Ap 2:5
 * es juicio que exige TESHUVA (volver); en firmware equivale a
 * reconstruir.
 *
 * Idempotente sobre REMOVIDO.
 *
 * 2 testigos:
 *   Ap 2:5   - metakineso ten lujnian (movere tu candelabro)
 *   Zac 4:2  - mutsaq dedicado: al quitarlo, el ner no alimenta
 */
int hashmal_comm_canal_remover(hashmal_canal_id_t id)
{
    if (!hashmal_comm_canal_legitimo(id)) {
        MENORA_LOG_E("remover: canal %d ilegítimo", (int)id);
        return HASHMAL_COMM_ERR_CANAL;
    }

    hashmal_menora_t *m = hashmal_comm_menora_mut();
    if (m == NULL) return HASHMAL_COMM_ERR_INIT;

    hashmal_ner_t *n = &m->nerot[(size_t)id];

    if (n->estado == HASHMAL_NER_REMOVIDO) {
        MENORA_LOG_W("remover: ner %d ya REMOVIDO (idempotente, no-op)",
                     (int)id);
        return HASHMAL_COMM_OK;
    }

    const hashmal_ner_estado_t prev = n->estado;

    /* Transicion terminal. */
    n->estado                  = HASHMAL_NER_REMOVIDO;
    n->mutsaqa                 = false;        /* Zac 4:2 - tubo cortado */
    n->paquetes_tx             = 0ull;
    n->paquetes_rx             = 0ull;
    n->errores                 = 0ull;
    n->timestamp_encendido_ms  = 0ull;
    n->aceite_nivel            = 0u;
    n->escucha_continua        = false;        /* P8 (#10) - rama amputada */
    /* frecuencia, potencia, SF, BW: se preservan como dato de auditoria.
     * El canal fue QUITADO, no reconfigurado. */

    actualizar_nerot_encendidos(m);

    MENORA_LOG_W("═══ ner %d REMOVIDO ═══ (%s → REMOVIDO)",
                 (int)id, nombre_estado(prev));
    MENORA_LOG_W("  'metakinéso tén lujnían' (Ap 2:5) — juicio");
    MENORA_LOG_W("  mutsaqá cortada (Zac 4:2) — recuperación = deinit + init");
    return HASHMAL_COMM_OK;
}

/* ==================================================================
 * API PUBLICA - todos_encendidos (Zac 4:10 - los 7 ojos)
 * ================================================================== */

/**
 * hashmal_comm_todos_encendidos - ?brillan los 7?
 *
 * Zac 4:10 - "shiv'a elle 'einei YHWH meshotetim bejol haarets":
 * "estos siete son los ojos de YHWH que recorren toda la tierra."
 *
 * La plenitud operativa se alcanza SOLO cuando los 7 estan
 * ENCENDIDO. ATENUADO (aceite bajo) NO basta - el ojo esta
 * debil. REMOVIDO tampoco - el ojo ya no mira.
 */
bool hashmal_comm_todos_encendidos(void)
{
    const hashmal_menora_t *m = hashmal_comm_get_menora_interno();
    if (m == NULL) return false;

    for (uint8_t i = 0; i < HASHMAL_MENORA_CANALES; ++i) {
        if (m->nerot[i].estado != HASHMAL_NER_ENCENDIDO) {
            return false;
        }
    }
    return true;
}

/* ==================================================================
 * API PUBLICA - yotseim_inactivos (P8 Verificado #10, Ex 25:32)
 *
 * "veSHISHA QANIM YOTSEIM mitsiddeiha" - 6 ramas SALIENDO
 * permanentemente. Participio = estado continuo. Cada canal 1..6
 * que pierda la escucha activa (APAGADO o REMOVIDO) rompe la
 * propiedad arquitectónica. ATENUADO mantiene el ojo abierto
 * (Zac 4:10), por eso cuenta como yotse activo.
 *
 * El gezá' (canal 0) NO se cuenta - es el tronco, no rama saliendo.
 * Su salud va por hashmal_comm_tamid_activo y nerot_encendidos.
 * ================================================================== */
uint8_t hashmal_comm_yotseim_inactivos(void)
{
    const hashmal_menora_t *m = hashmal_comm_get_menora_interno();
    if (m == NULL) return HASHMAL_MENORA_RAMAS;  /* sin menorá: 6 caídos */

    uint8_t caidos = 0u;
    /* Contar SOLO canales secundarios 1..6 (las 6 ramas yotseim). */
    for (uint8_t i = 1; i < HASHMAL_MENORA_CANALES; ++i) {
        const hashmal_ner_t *n = &m->nerot[i];
        const bool en_escucha =
            (n->estado == HASHMAL_NER_ENCENDIDO ||
             n->estado == HASHMAL_NER_ATENUADO) &&
            n->escucha_continua;
        if (!en_escucha) {
            ++caidos;
        }
    }
    return caidos;
}

/* ==================================================================
 * P52 — malqajáyim + majtót (Éx 25:38): mantenimiento nativo
 *
 * "et malqajéiha veʼet majtotéiha zaháv tahór" — Éx 25:38.
 * malqajáyim (H4457) = tenazas para recortar mechas = calibración.
 * majtót (H4289) = platillos para recoger ceniza = garbage collection.
 * Ambos son NATIVOS (zaháv tahór, mismo material que la Menorá).
 * ================================================================== */

/**
 * hashmal_comm_canal_calibrar - recalibra parámetros del canal.
 *
 * "et malqajéiha" — Éx 25:38. Tenazas = recorte fino de parámetros.
 * Recalibra RSSI thresholds, timing, SNR targets del SX1262 sin
 * resetear contadores operativos. Estado se preserva.
 */
int hashmal_comm_canal_calibrar(hashmal_canal_id_t id)
{
    if (!hashmal_comm_canal_legitimo(id)) {
        MENORA_LOG_E("calibrar: canal %d ilegítimo", (int)id);
        return HASHMAL_COMM_ERR_CANAL;
    }

    hashmal_menora_t *m = hashmal_comm_menora_mut();
    if (m == NULL) return HASHMAL_COMM_ERR_INIT;

    hashmal_ner_t *n = &m->nerot[(size_t)id];

    /* Validar estado — REMOVIDO no se calibra (terminal). */
    if (n->estado == HASHMAL_NER_REMOVIDO) {
        MENORA_LOG_E("calibrar: ner %d REMOVIDO (Ap 2:5) — nada que calibrar",
                     (int)id);
        return HASHMAL_COMM_ERR_REMOVIDO;
    }

    /* Llamada placeholder — drivers específicos del SX1262 irán aquí.
     * Fase 1: solo log de auditoría. Fase 2: reconfiguración real. */
    MENORA_LOG_I("calibrar ner %d: recalibración RSSI/timing "
                 "(malqajáyim — Éx 25:38)",
                 (int)id);
    return HASHMAL_COMM_OK;
}

/**
 * hashmal_comm_canal_limpiar - garbage collection del canal.
 *
 * "et majtotéiha" — Éx 25:38. Platillos = recolecta de ceniza.
 * Reset contadores de error, purga buffers RX/TX, limpia estado
 * de retransmisión sin tocar configuración de radio.
 */
int hashmal_comm_canal_limpiar(hashmal_canal_id_t id)
{
    if (!hashmal_comm_canal_legitimo(id)) {
        MENORA_LOG_E("limpiar: canal %d ilegítimo", (int)id);
        return HASHMAL_COMM_ERR_CANAL;
    }

    hashmal_menora_t *m = hashmal_comm_menora_mut();
    if (m == NULL) return HASHMAL_COMM_ERR_INIT;

    hashmal_ner_t *n = &m->nerot[(size_t)id];

    if (n->estado == HASHMAL_NER_REMOVIDO) {
        MENORA_LOG_E("limpiar: ner %d REMOVIDO — nada que limpiar",
                     (int)id);
        return HASHMAL_COMM_ERR_REMOVIDO;
    }

    /* Limpieza nativa (zaháv tahór). */
    n->errores = 0ull;  /* Pizarra limpia. */
    /* Buffers RX/TX se limpian en drivers; aquí solo reset de state. */

    MENORA_LOG_I("limpiar ner %d: contadores reset, buffers purgados "
                 "(majtót — Éx 25:38)",
                 (int)id);
    return HASHMAL_COMM_OK;
}

/* ==================================================================
 * P53 — kikkár zaháv tahór (Éx 25:39): presupuesto FIJO
 *
 * "kikkár zaháv tahór yaʻaséh otáh ÉT kol hakkelím haʼélle."
 * "UN talento de oro puro la hará, CON TODOS estos utensilios."
 *
 * kikkár = talento, ~30 kg = medida FIJA, no expansible.
 * El presupuesto de la Menorá es SELLADO. No se expande.
 * ================================================================== */

int hashmal_comm_kikkar_uso(uint32_t *usado, uint32_t *total)
{
    if (usado == NULL || total == NULL) {
        MENORA_LOG_E("kikkar_uso: parámetros NULL");
        return HASHMAL_COMM_ERR_NULL;
    }

    const hashmal_menora_t *m = hashmal_comm_get_menora();
    if (m == NULL) {
        *usado = 0u;
        *total = HASHMAL_COMM_KIKKAR_BYTES;
        return HASHMAL_COMM_ERR_INIT;
    }

    *usado = (uint32_t)(m->kikkar_usado);
    *total = (uint32_t)HASHMAL_COMM_KIKKAR_BYTES;

    MENORA_LOG_I("kikkar_uso: %" PRIu32 " / %" PRIu32 " bytes "
                 "(kikkár — Éx 25:39)",
                 *usado, *total);
    return HASHMAL_COMM_OK;
}

bool hashmal_comm_kikkar_disponible(uint32_t bytes_needed)
{
    const hashmal_menora_t *m = hashmal_comm_get_menora();
    if (m == NULL) {
        /* Menorá no inicializada — sin presupuesto. */
        return false;
    }

    uint32_t usado = (uint32_t)(m->kikkar_usado);
    uint32_t total = (uint32_t)HASHMAL_COMM_KIKKAR_BYTES;

    bool hay = (usado + bytes_needed <= total);
    if (!hay) {
        MENORA_LOG_W("kikkar_disponible: %u bytes NO hay (usado=%u, "
                     "total=%u, necesario=%u) — kikkár SELLADO (Éx 25:39)",
                     (unsigned)(bytes_needed + usado - total),
                     (unsigned)usado, (unsigned)total,
                     (unsigned)bytes_needed);
    }
    return hay;
}

/* ==================================================================
 *
 *   "vehe'ela et neroteiha veheir 'al 'ever paneiha" - Ex 25:37
 *
 *   "vayya'as ken Aharon el mul penei hammenora he'ela neroteiha
 *    ka'asher tsivva YHWH et Moshe" - Nm 8:3
 *   "Y Aaron lo hizo asi: hacia el frente de la Menora encendio
 *    sus lamparas, como YHWH mando a Moises."
 *
 *   La llama sube o no sube. No hay medio. he'ela o apagado -
 *   ATENUADO es aviso, REMOVIDO es juicio.
 *
 *   "shiv'a elle 'einei YHWH" - Zacarias 4:10. Cuando los siete
 *   estan ENCENDIDO, la Menora vigila toda la tierra.
 *
 * ================================================================== */
