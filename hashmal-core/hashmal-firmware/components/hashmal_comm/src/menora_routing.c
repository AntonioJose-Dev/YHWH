/**
 * ===================================================================
 * PIEZA 04 - LA MENORA - menora_routing.c
 * Sub-pieza: GEVI'IM MESHUQQADIM - los 22 vigias almendro
 * Exodo 25:33-34,35,37 + Jeremias 1:11-12 + Numeros 8:2-3 +
 *   Zacarias 4:2,6 (gulla + beruji - entrega multi-mutsaqa)
 * ===================================================================
 *
 * "shlosha GEVI'IM MESHUQQADIM baqqane ha'ejad kaftor vaferaj...
 *  ken LESHESHET haqqanim hayyotsim min hammenora" - Ex 25:33
 * "TRES COPAS en forma de ALMENDRO en una rama, con boton y flor;
 *  asi para las SEIS ramas que salen de la Menora."
 *
 * "uvammenora ARBA'A gevi'im meshuqqadim kaftoreiha uferajeiha"
 *   - Ex 25:34
 * "Y en la Menora (tronco) CUATRO copas en forma de almendro."
 *
 * "vejaftor TAJAT shenei haqqanim mimmenna... lesheshet haqqanim"
 *   - Ex 25:35
 * "Y un boton DEBAJO de cada PAR de ramas... para las seis ramas."
 *
 * "vehe'ela et neroteiha VEHE'IR 'al 'ever paneiha" - Ex 25:37
 * "Y encendera sus lamparas, Y LA LUZ LLEGARA hacia el frente."
 *
 * "menorat zahav kullah veGULLAH 'al rosha... shiv'a veshiv'a
 *  MUTSAQOT lannerot." - Zacarias 4:2
 * "Menora toda de oro, con un DEPOSITO sobre su cabeza... siete
 *  y siete CONDUCTOS para las lamparas."
 *
 * "lo vejayil velo vejoaj ki im beRUJI." - Zacarias 4:6
 * "No por ejercito ni por fuerza, sino por Mi Espiritu."
 *
 * --- EXEGESIS ---
 *
 *   gevi'im     (GB'YM H1375)  copas / calices - receptaculos de
 *                                informacion que pasa por el mesh.
 *   meshuqqadim (MShQDYM H8246) Pual de shaqad: "en forma de
 *                                vigilante" / "hechos para velar".
 *
 * Jeremias 1:11-12 confirma el juego de palabras del Padre:
 *   "mah atta ro'e Yirmeyahu? maqqel SHAQED ani ro'e.
 *    vayyomer YHWH: hetavta lir'ot ki SHOQED ani 'al devari."
 *   "?Que ves? Una vara de ALMENDRO veo.
 *    Bien has visto, porque YO VELO sobre Mi Palabra."
 *
 * shaqed (almendro, el primer arbol que florece en primavera) <->
 * shoqed (el que vela, el que no duerme). Los nodos intermedios
 * del mesh son almendros: VIGIAS que no duermen sobre los paquetes.
 *
 * --- 22 GEVI'IM (topologia) ---
 *
 *   3 gevi'im x 6 ramas  = 18 vigias en ramas (v.33)
 *   4 gevi'im en tronco  =  4 vigias centrales (v.34)
 *   TOTAL                = 22 nodos vigilantes (HASHMAL_MENORA_GEVIIM_TOTAL)
 *
 * Los kaftorim (v.35) son las JUNCIONES donde los pares simetricos
 * (1<->4, 2<->5, 3<->6) convergen al tronco. Tres kaftorim = tres puntos
 * de encuentro en el mesh.
 *
 * --- FASE 1: SINGLE-HOP ---
 *
 * Esta version es SINGLE-HOP: cada `enviar` va directo al destino
 * por un canal especifico. Los 22 gevi'im estan modelados
 * conceptualmente pero el reenvio entre nodos vendra con la
 * escala. 1 Re 7:49 describe DIEZ menoras en el Templo de
 * Salomon - el multi-hop real llegara cuando haya N nodos y se
 * necesite. Por ahora los almendros miran; en la fase siguiente
 * propagaran.
 *
 * --- QUE HACE / QUE NO HACE ---
 *
 *   HACE:                         NO HACE:
 *   - enviar por canal            - multi-hop forwarding (fase 2)
 *   - pasar por la gulla          - tabla de vecinos real (fase 2)
 *     (Zac 4:2 -> selector)
 *   - recibir multi-mutsaqa       - tocar SPI directo (va via gulla)
 *   - broadcast a encendidos      - formatear paquetes (-> formato.c)
 *   - contar vecinos (simple)     - decidir estados (-> canales.c)
 *   - mantener contadores RX/TX   - elegir mutsaqa (-> mutsaqot.c)
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_comm.h               -> API publica
 *   hashmal_comm_internal.h      -> menora_mut, get_menora_interno,
 *                                  canal_legitimo
 *   esp_log.h (ORO DE EGIPTO)    -> MENORA_LOG_*
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
static const char *MENORA_TAG = "hashmal.menora.routing";

#define MENORA_LOG_I(fmt, ...)  ESP_LOGI(MENORA_TAG, fmt, ##__VA_ARGS__)
#define MENORA_LOG_W(fmt, ...)  ESP_LOGW(MENORA_TAG, fmt, ##__VA_ARGS__)
#define MENORA_LOG_E(fmt, ...)  ESP_LOGE(MENORA_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * P9 (Verificado #10) - MESHUQQADIM (Ex 25:33, shaqad H8245)
 *
 * "shlosha gevi'im MESHUQQADIM baqqane ha'ejad" - 3 copas en forma
 * de almendro por rama. Pual ptc. de shaqad (vigilar). Jer 1:11-12:
 * shaqed (almendro) <-> shoqed (el que vela). Cada hop debe ejecutar
 * inspeccion sobre el trafico que retransmite. 22 puntos de vigilia
 * (3*6 + 4 = HASHMAL_MENORA_GEVIIM_TOTAL).
 *
 * Slot UNICO de vigia (un solo shoqed por nodo, Jer 1:12 sin
 * pluralizar). Inversion de control: el firewall / healthcheck /
 * intercession se registra aqui. Sin vigia: log de aviso, paquete
 * pasa sin inspeccion (modo degradado, no silencioso).
 * ================================================================== */
static hashmal_comm_meshuqqad_fn s_meshuqqad_fn = NULL;

/* ==================================================================
 * P30 — noʻad callback (Nm 10:3, Éx 25:22)
 *
 * "vetaqeʻú bahén veNOʻADÚ eléija kol haʻédah" — Nm 10:3
 * "venoʻADTÍ lejá shám" — Éx 25:22
 * MISMO VERBO (H3259, Nifal): yʻd = rendezvous. Alarma (broadcast)
 * y consenso comparten semántica raíz. Cuando alarm dispara, invoca
 * este callback para triggear rendezvous en el consenso.
 * ================================================================== */
static hashmal_comm_noad_fn s_noad_fn = NULL;

/* ==================================================================
 * P27 — 22 geviim vigilancia distribuida (Éx 25:33-34)
 *
 * "shloshá geviʻím meshuqqadím baqqané haʼejád... ken leshishét
 *  haqqaním" — Éx 25:33-34. 22 vigías (copas-almendro) distribuidas:
 * 3 por rama × 6 ramas + 4 en el tronco = 22. Cada gevia puede tener
 * su propio vigilante registrado.
 * ================================================================== */
static hashmal_comm_gevia_fn s_geviim[HASHMAL_MENORA_GEVIIM_TOTAL] = {NULL};

int hashmal_comm_meshuqqad_subscribir(hashmal_comm_meshuqqad_fn fn)
{
    if (fn == NULL) {
        if (s_meshuqqad_fn != NULL) {
            MENORA_LOG_W("meshuqqad_subscribir: vigia LIBERADO "
                         "(almendro dormido — Jer 1:11-12 invertido)");
        }
        s_meshuqqad_fn = NULL;
        return HASHMAL_COMM_OK;
    }
    if (s_meshuqqad_fn != NULL && s_meshuqqad_fn != fn) {
        MENORA_LOG_W("meshuqqad_subscribir: sobrescribiendo vigia previo "
                     "(idempotencia legítima)");
    }
    s_meshuqqad_fn = fn;
    MENORA_LOG_I("meshuqqad SUSCRITO (Éx 25:33 — shoqéd ʻal devarí, Jer 1:12)");
    return HASHMAL_COMM_OK;
}

bool hashmal_comm_meshuqqad_activo(void)
{
    return s_meshuqqad_fn != NULL;
}

/* ==================================================================
 * P27 - hashmal_comm_geviim_subscribir
 *
 * "shloshá geviʻím meshuqqadím baqqané haʼejád" — Éx 25:33
 * Cada copa (gevia) 0..21 puede vigilar su sección. Registro separado
 * por gevia permite especialización: cada nodo intermedio puede
 * implementar inspección diferenciada (firewall por zona, etc).
 * ================================================================== */
int hashmal_comm_geviim_subscribir(uint8_t gevia_id, hashmal_comm_gevia_fn fn)
{
    if (gevia_id >= HASHMAL_MENORA_GEVIIM_TOTAL) {
        MENORA_LOG_E("geviim_subscribir: gevia_id %u fuera de [0, %d)",
                     (unsigned)gevia_id, HASHMAL_MENORA_GEVIIM_TOTAL);
        return HASHMAL_COMM_ERR_CANAL;
    }

    if (fn == NULL) {
        if (s_geviim[gevia_id] != NULL) {
            MENORA_LOG_W("geviim_subscribir: gevia %u LIBERADA (Jer 1:11-12)",
                         (unsigned)gevia_id);
        }
        s_geviim[gevia_id] = NULL;
        return HASHMAL_COMM_OK;
    }

    if (s_geviim[gevia_id] != NULL && s_geviim[gevia_id] != fn) {
        MENORA_LOG_W("geviim_subscribir: gevia %u vigia sobrescrito",
                     (unsigned)gevia_id);
    }

    s_geviim[gevia_id] = fn;
    MENORA_LOG_I("geviim %u SUSCRITO (Éx 25:33-34 — 22 vigías meshuqqadím)",
                 (unsigned)gevia_id);
    return HASHMAL_COMM_OK;
}

/* ==================================================================
 * P30 - hashmal_comm_noad_subscribir
 *
 * "vetaqeʻú bahén veNOʻADÚ eléija kol haʻédah" — Nm 10:3
 * Cuando una alarma broadcast dispara, invoca este callback para
 * despertar el rendezvous en el consenso. Alarma → Rendezvous.
 * MISMO VERBO, semántica compartida.
 * ================================================================== */
int hashmal_comm_noad_subscribir(hashmal_comm_noad_fn fn)
{
    if (fn == NULL) {
        if (s_noad_fn != NULL) {
            MENORA_LOG_W("noad_subscribir: callback LIBERADO "
                         "(alarma no dispara rendezvous — Nm 10:3)");
        }
        s_noad_fn = NULL;
        return HASHMAL_COMM_OK;
    }

    if (s_noad_fn != NULL && s_noad_fn != fn) {
        MENORA_LOG_W("noad_subscribir: callback sobrescrito");
    }

    s_noad_fn = fn;
    MENORA_LOG_I("noʻad callback SUSCRITO (Nm 10:3 — vetaqeʻú veNOʻADÚ)");
    return HASHMAL_COMM_OK;
}

/**
 * P9 - inspecciona el paquete por el vigia registrado. Si no hay
 * vigia, log de aviso (no bloquea - el mesh sigue funcionando en
 * modo degradado). Si el vigia rechaza, propagamos el rc al caller.
 */
static int meshuqqad_inspeccionar(hashmal_canal_id_t canal,
                                  const uint8_t *msg, size_t tam)
{
    if (s_meshuqqad_fn == NULL) {
        /* almendro dormido - log de aviso, paso libre. El healthcheck
         * (pieza 12) tiene meshuqqad_activo() para detectar este
         * estado degradado y reportarlo arriba. */
        MENORA_LOG_W("meshuqqad: sin vigía registrado en hop canal=%d "
                     "(estado degradado — Jer 1:11-12 vigilancia ausente)",
                     (int)canal);
        return HASHMAL_COMM_OK;
    }
    const int rc = s_meshuqqad_fn(canal, msg, tam);
    if (rc != HASHMAL_COMM_OK) {
        MENORA_LOG_W("meshuqqad canal=%d: vigía RECHAZÓ paquete "
                     "(rc=%d) — shoqéd vio fuego extraño",
                     (int)canal, rc);
    }
    return rc;
}

/* ==================================================================
 * HELPERS ESTATICOS - dispatch por mutsaqa (Zac 4:2 gulla)
 *
 * La gulla (Zac 4:2) recibe el aceite y lo vierte por la mutsaqa
 * que el selector eligio (Zac 4:6 beruji). Este routing no toca
 * SPI/WiFi/BLE directamente: entrega a la gulla y la gulla baja
 * el aceite al ner correcto por el conducto elegido.
 * ================================================================== */

/**
 * Despacha la entrega por la mutsaqa seleccionada. El nodo_destino
 * se propaga por consistencia aunque algunos transportes aun lo
 * descarten (la API `hashmal_comm_enviar` single-arg no carga MAC;
 * fase posterior crecera el contrato).
 */
static int entregar_por_mutsaqa(hashmal_mutsaqa_id_t mut,
                                uint8_t nodo_destino,
                                const uint8_t *msg, size_t tam)
{
    (void)nodo_destino;

    switch (mut) {
    case HASHMAL_MUTSAQA_LORA:
        /* geza' (Ex 25:31) - el tronco. Tamid (Ex 27:21), siempre
         * disponible como columna vertebral. */
        return hashmal_comm_spi_enviar(msg, tam);

    case HASHMAL_MUTSAQA_ESP_NOW:
        /* ESP-NOW requiere MAC del destino (isha el ajota, unicast).
         * La API actual `hashmal_comm_enviar(canal, msg, tam)` no
         * lleva MAC — cuando la fase 2 resuelva nodo_destino→MAC
         * via tabla de vecinos real, llamaremos a espnow_enviar()
         * con la MAC correcta. Por ahora, fallback tamid a LoRa
         * (Ex 27:21 juqqat 'olam). */
        MENORA_LOG_W("entregar: ESP_NOW sin nodo_destino→MAC; "
                     "fallback LoRa tamíd (Éx 27:21, fase 2 pendiente)");
        return hashmal_comm_spi_enviar(msg, tam);

    case HASHMAL_MUTSAQA_BLE: {
        /* Ex 28:35 venishma' qolo — el pa'amon lleva el mensaje
         * embebido en su advertising. Todos los vecinos que escanean
         * lo reciben pasivamente: broadcast natural por diseño bíblico.
         *
         * Solo aplica para tam <= HASHMAL_BLE_MENSAJE_MAX (12 bytes).
         * Para mensajes mayores, el pa'amon es pequeño por diseño
         * (Zac 4:10 yom qetannot); fallback tamid a LoRa. */
        int rc_ble = hashmal_comm_ble_paamon_emitir(msg, tam);
        if (rc_ble == HASHMAL_COMM_OK) {
            MENORA_LOG_I("entregar: BLE paʻamón emitió %u B "
                         "(Éx 28:35 venishmáʻ qoló — berují Zac 4:6)",
                         (unsigned)tam);
            return HASHMAL_COMM_OK;
        }
        if (rc_ble == HASHMAL_COMM_ERR_TAMANO) {
            /* Mensaje supera capacidad del pa'amon — esperado para
             * mensajes grandes. No es error del sistema. */
            MENORA_LOG_I("entregar: BLE paʻamón tam=%u > max; "
                         "tronco LoRa tamíd (Éx 27:21)",
                         (unsigned)tam);
        } else {
            /* Error real del driver BLE — caemos al tronco con aviso. */
            MENORA_LOG_W("entregar: BLE paʻamón rc=%d; "
                         "fallback LoRa tamíd (Éx 27:21)",
                         rc_ble);
        }
        return hashmal_comm_spi_enviar(msg, tam);
    }

    default:
        MENORA_LOG_E("entregar: mutsaqá %d desconocida", (int)mut);
        return HASHMAL_COMM_ERR_SIN_MUTSAQA;
    }
}

/**
 * Recibe por cualquier mutsaqa disponible. Zac 4:2 - la gulla
 * recibe aceite de multiples fuentes; el routing no se ata a una.
 * Prueba en orden: LoRa (tamid), ESP-NOW (isha el ajota). BLE es
 * scan/advertise, no unicast messaging - se consulta vecinos por
 * otra via.
 *
 * @return OK si se recibio algo (de cualquier mutsaqa) o no habia
 * nada pendiente. Error solo si TODAS las mutsaqot fallaron con
 * rc != OK y rc != ERR_TIMEOUT.
 */
static int recibir_cualquier_mutsaqa(uint8_t *buf, size_t tam,
                                     size_t *recibidos)
{
    *recibidos = 0u;

    /* 1) LoRa tamid - Ex 27:21, la llama perpetua. */
    int rc = hashmal_comm_spi_recibir(buf, tam, recibidos);
    if (rc == HASHMAL_COMM_OK && *recibidos > 0u) {
        return HASHMAL_COMM_OK;
    }
    /* Si rc != OK, seguimos intentando otras mutsaqot - el Espiritu
     * (Zac 4:6) permite alternativas si una via falla. */

    /* 2) ESP-NOW - isha el ajota. Descartamos mac_origen porque
     *    esta API (recibir por canal) no lo expone. */
    uint8_t mac_origen[6] = {0};
    size_t  esp_recibidos = 0u;
    int rc_esp = hashmal_comm_espnow_recibir(mac_origen, buf, tam,
                                              &esp_recibidos);
    if (rc_esp == HASHMAL_COMM_OK && esp_recibidos > 0u) {
        *recibidos = esp_recibidos;
        return HASHMAL_COMM_OK;
    }

    /* 3) BLE: pa'amon es descubrimiento, no mensajeria unicast.
     *    Se consulta con hashmal_comm_ble_vecinos_* (otra API). */

    /* Nada pendiente - no es error. */
    *recibidos = 0u;
    return HASHMAL_COMM_OK;
}

/* ==================================================================
 * API PUBLICA - enviar (v.37 veheir - la luz sale)
 * ================================================================== */

/**
 * hashmal_comm_enviar - TX por un canal especifico
 *
 * --- ESTADOS PERMITIDOS ---
 *
 *   ENCENDIDO  -> OK (he'ela pleno, v.37 - luz brillante sale)
 *   ATENUADO   -> OK con LOG_W ("ojo debil transmitiendo", Ex 27:20)
 *   APAGADO    -> ERR_CANAL (no se transmite desde lampara apagada)
 *   REMOVIDO   -> ERR_REMOVIDO (Ap 2:5 - el candelabro fue quitado)
 *
 * --- CONTADORES ---
 *
 *   paquetes_tx += 1  en exito
 *   errores     += 1  en fallo
 *
 * --- PRESUPUESTO DE CONFIGURACION ---
 *
 * No reconfigura el SX1262. Se asume que el caller ha hecho
 * canal_encender(id) antes de enviar - el chip quedo programado
 * para este canal en esa llamada. Si multiples canales estan
 * ENCENDIDOS, el ultimo encendido es el activo en el silicio. El
 * caller orquesta ese secuenciamiento (routing.c maneja estados,
 * canales.c programa el chip). Esta separacion de responsabilidades
 * honra Ex 25:40 (betavnitam): cada capa en su sitio.
 *
 * 2 testigos:
 *   Ex 25:37 - veheir 'al 'ever paneiha (la luz sale hacia afuera)
 *   Jer 1:12 - SHOQED ani 'al devari (YHWH vigila la entrega)
 */
int hashmal_comm_enviar(hashmal_canal_id_t id,
                        const uint8_t *msg, size_t tam)
{
    if (msg == NULL) {
        MENORA_LOG_E("enviar: msg NULL");
        return HASHMAL_COMM_ERR_NULL;
    }
    if (tam == 0u || tam > HASHMAL_MENSAJE_MAX_BYTES) {
        MENORA_LOG_E("enviar: tam %u fuera de (0, %d]",
                     (unsigned)tam, HASHMAL_MENSAJE_MAX_BYTES);
        return HASHMAL_COMM_ERR_TAMANO;
    }
    if (!hashmal_comm_canal_legitimo(id)) {
        MENORA_LOG_E("enviar: canal %d ilegítimo", (int)id);
        return HASHMAL_COMM_ERR_CANAL;
    }

    /* Ez 33:2-3 tsofeh — si el atalaya vigila, TX esta gated. El
     * unico camino permitido es hashmal_comm_shofar_tocar, que
     * bypasea este gate temporalmente. Se chequea ANTES de tocar
     * estado del ner: el atalaya no transmite por ningun canal,
     * indep. del estado. */
    if (hashmal_comm_tsofeh_lockdown_get()) {
        MENORA_LOG_W("enviar canal %d: TSOFEH lockdown — TX gated "
                     "(Ez 33:2-3), solo shofar permitido",
                     (int)id);
        return HASHMAL_COMM_ERR_TSOFEH;
    }

    hashmal_menora_t *m = hashmal_comm_menora_mut();
    if (m == NULL) return HASHMAL_COMM_ERR_INIT;

    hashmal_ner_t *n = &m->nerot[(size_t)id];

    switch (n->estado) {
    case HASHMAL_NER_ENCENDIDO:
        break;  /* OK pleno */
    case HASHMAL_NER_ATENUADO:
        MENORA_LOG_W("enviar canal %d: ATENUADO — ojo débil transmitiendo "
                     "(Éx 27:20 shémen bajo)", (int)id);
        break;
    case HASHMAL_NER_APAGADO:
        MENORA_LOG_E("enviar canal %d: APAGADO — sin luz que enviar",
                     (int)id);
        return HASHMAL_COMM_ERR_CANAL;
    case HASHMAL_NER_REMOVIDO:
        MENORA_LOG_E("enviar canal %d: REMOVIDO (Ap 2:5) — re-init requerido",
                     (int)id);
        return HASHMAL_COMM_ERR_REMOVIDO;
    default:
        return HASHMAL_COMM_ERR_CANAL;
    }

    /* P9 (#10) - Ex 25:33 meshuqqadim: cada hop inspecciona el trafico.
     * Antes de elegir mutsaqa o entregar al silicio, el vigia registrado
     * (firewall/healthcheck/intercession) decide si el paquete sigue.
     * Sin vigia: log degradado, paso libre. Con vigia rechazando:
     * incremento errores y propaga rc. */
    int rc_vigia = meshuqqad_inspeccionar(id, msg, tam);
    if (rc_vigia != HASHMAL_COMM_OK) {
        n->errores += 1ull;
        return rc_vigia;
    }

    /* Zac 4:2 - pasar por la GULLA: el deposito sobre la cabeza
     * decide por que mutsaqa baja el aceite al ner. Zac 4:6 -
     * beruji: no por jayil/koaj, sino por adecuacion (tamano,
     * disponibilidad, consumo). */
    hashmal_mutsaqa_id_t mut_elegida = HASHMAL_MUTSAQA_LORA;
    const uint8_t nodo_destino_auto = 0xFFu;   /* API single-arg - sin dest */
    int rc_sel = hashmal_comm_mutsaqa_seleccionar(id, nodo_destino_auto,
                                                   tam, &mut_elegida);
    if (rc_sel != HASHMAL_COMM_OK) {
        n->errores += 1ull;
        MENORA_LOG_E("enviar canal %d: gullá sin flujo (rc=%d) — "
                     "Zac 4:2 ningún mutsaqá apto — errores=%" PRIu64,
                     (int)id, rc_sel, n->errores);
        return rc_sel;
    }

    /* Éx 25:37 — "vehéʼelá et neroteiha vehéir ʻal ʻéver panéiha":
     * cada ner ilumina hacia SU propio frente. Antes de TX por LoRa,
     * resintonizar el SX1262 a la frecuencia de ESTE canal.
     *
     * Sin este paso los 7 "envíos" del broadcast irían todos en la
     * misma frecuencia (la del último canal_encender durante init),
     * lo que viola Éx 25:37 — cada lámpara tiene su propia frecuencia.
     * "Cada ner ilumina hacia SU frente, no hacia el mismo frente."
     *
     * 2 testigos:
     *   Éx 25:37 — "shivʻá nerot" (7 lámparas = 7 frecuencias distintas)
     *   Nm 8:2   — "el mul penéi hammenorá yaʼiru shivʻát hannerot"
     *              (los 7 nerot iluminan hacia sus propios frentes) */
    if (mut_elegida == HASHMAL_MUTSAQA_LORA) {
        (void)hashmal_comm_configurar_frecuencia(id, n->frecuencia_hz);
    }

    /* Delegar al driver de la mutsaqa elegida. qanah - por el
     * tronco sale la luz (Ex 25:31 geza'), aunque hoy otros
     * conductos puedan compartir el oficio cuando el diseno los
     * elija (Zac 4:6 beruji). */
    int rc = entregar_por_mutsaqa(mut_elegida, nodo_destino_auto, msg, tam);
    if (rc != HASHMAL_COMM_OK) {
        n->errores += 1ull;
        MENORA_LOG_E("enviar canal %d: entrega por mutsaqá falló "
                     "(rc=%d) — errores=%" PRIu64,
                     (int)id, rc, n->errores);
        return rc;
    }

    n->paquetes_tx += 1ull;
    MENORA_LOG_I("enviar canal %d: %u B vía mutsaqá %d "
                 "(tx=%" PRIu64 ", Éx 25:37 vehéir + Zac 4:6 berují)",
                 (int)id, (unsigned)tam, (int)mut_elegida, n->paquetes_tx);
    return HASHMAL_COMM_OK;
}

/* ==================================================================
 * API PUBLICA - recibir (Nm 8:2 - la luz llega al centro)
 * ================================================================== */

/**
 * hashmal_comm_recibir - RX de un canal especifico
 *
 * Nm 8:2 - "el mul penei hammenora ya'iru shiv'at hannerot":
 * las lamparas iluminan HACIA el tronco. El paquete viene de fuera,
 * entra por la rama, y llega al centro.
 *
 * --- ESTADOS PERMITIDOS ---
 *
 *   ENCENDIDO  -> OK (oido pleno)
 *   ATENUADO   -> OK (el oido sigue abierto aunque la voz sea debil)
 *   APAGADO    -> ERR_CANAL (no hay oido activo)
 *   REMOVIDO   -> ERR_REMOVIDO
 *
 * Nota de fase 1: no configura el chip en RX mode. Asume que
 * canal_encender + politicas superiores pusieron el SX1262 en
 * recepcion para este canal. Si no hay paquete, *recibidos = 0 y
 * retorno OK (no es error - solo no hay nada que leer).
 *
 * 2 testigos:
 *   Nm 8:2   - "el mul penei hammenora" (hacia el tronco central)
 *   Ex 25:37 - paneiha ya'iru (la cara de la Menora recibe la luz)
 */
int hashmal_comm_recibir(hashmal_canal_id_t id,
                         uint8_t *buf, size_t tam, size_t *recibidos)
{
    if (buf == NULL || recibidos == NULL) {
        MENORA_LOG_E("recibir: buf o recibidos NULL");
        return HASHMAL_COMM_ERR_NULL;
    }
    *recibidos = 0u;

    if (!hashmal_comm_canal_legitimo(id)) {
        MENORA_LOG_E("recibir: canal %d ilegítimo", (int)id);
        return HASHMAL_COMM_ERR_CANAL;
    }

    hashmal_menora_t *m = hashmal_comm_menora_mut();
    if (m == NULL) return HASHMAL_COMM_ERR_INIT;

    hashmal_ner_t *n = &m->nerot[(size_t)id];

    switch (n->estado) {
    case HASHMAL_NER_ENCENDIDO:
        break;
    case HASHMAL_NER_ATENUADO:
        /* Oido debil pero abierto - no logueamos WARN en RX cada
         * vez (demasiado ruido si el canal esta en ATENUADO por
         * un tiempo largo). El diagnostico lo da aceite_verificar. */
        break;
    case HASHMAL_NER_APAGADO:
        MENORA_LOG_E("recibir canal %d: APAGADO — sin oído activo",
                     (int)id);
        return HASHMAL_COMM_ERR_CANAL;
    case HASHMAL_NER_REMOVIDO:
        MENORA_LOG_E("recibir canal %d: REMOVIDO (Ap 2:5)", (int)id);
        return HASHMAL_COMM_ERR_REMOVIDO;
    default:
        return HASHMAL_COMM_ERR_CANAL;
    }

    /* Zac 4:2 - la gulla recibe aceite de multiples fuentes.
     * El helper prueba LoRa (tamid) y ESP-NOW en orden; BLE es
     * descubrimiento y se consulta por otra API. Si ninguna
     * mutsaqa tiene algo, *recibidos = 0 y OK (no es error -
     * solo silencio). */
    int rc = recibir_cualquier_mutsaqa(buf, tam, recibidos);
    if (rc != HASHMAL_COMM_OK) {
        n->errores += 1ull;
        MENORA_LOG_E("recibir canal %d: multi-mutsaqá falló (rc=%d) — "
                     "errores=%" PRIu64,
                     (int)id, rc, n->errores);
        return rc;
    }

    if (*recibidos > 0u) {
        n->paquetes_rx += 1ull;
        MENORA_LOG_I("recibir canal %d: %u B (rx=%" PRIu64 ", "
                     "Nm 8:2 + Zac 4:2 gullá)",
                     (int)id, (unsigned)*recibidos, n->paquetes_rx);
    }
    return HASHMAL_COMM_OK;
}

/* ==================================================================
 * API PUBLICA - broadcast (v.37 - ilumina TODO el frente)
 * ================================================================== */

/**
 * hashmal_comm_broadcast - enviar a todos los nerot activos
 *
 * v.37 "veheir 'al 'ever paneiha" - la luz ilumina TODO el frente.
 * Zac 4:10 "meshotetim bejol haarets" - los 7 ojos recorren TODA
 * la tierra. Broadcast = iluminacion total.
 *
 * Recorre los 7 nerot. Para cada ENCENDIDO o ATENUADO, llama a
 * `enviar(i, msg, tam)`. APAGADO y REMOVIDO se saltan (APAGADO
 * silencioso; REMOVIDO con log informativo - merece testigo).
 *
 * Retorna:
 *   OK                          si al menos UN ner envio con exito
 *   ERR_CANAL                   si ningun ner esta operativo
 *                               (todos APAGADO/REMOVIDO)
 *   ultimo codigo de fallo      si todos los operativos fallaron
 *
 * 2 testigos:
 *   Ex 25:37 - veheir 'al 'ever paneiha (iluminar el frente entero)
 *   Zac 4:10 - shiv'a 'einei YHWH meshotetim (7 ojos recorren todo)
 */
int hashmal_comm_broadcast(const uint8_t *msg, size_t tam)
{
    if (msg == NULL) return HASHMAL_COMM_ERR_NULL;
    if (tam == 0u || tam > HASHMAL_MENSAJE_MAX_BYTES) {
        return HASHMAL_COMM_ERR_TAMANO;
    }

    const hashmal_menora_t *m = hashmal_comm_get_menora_interno();
    if (m == NULL) return HASHMAL_COMM_ERR_INIT;

    uint8_t n_exitos     = 0u;
    uint8_t n_fallos     = 0u;
    uint8_t n_operativos = 0u;
    int     ultimo_err   = HASHMAL_COMM_ERR_CANAL;   /* default si 0 operativos */

    for (uint8_t i = 0; i < HASHMAL_MENORA_CANALES; ++i) {
        const hashmal_ner_estado_t estado = m->nerot[i].estado;

        if (estado != HASHMAL_NER_ENCENDIDO &&
            estado != HASHMAL_NER_ATENUADO) {
            /* APAGADO o REMOVIDO: skip. */
            continue;
        }

        ++n_operativos;
        int rc = hashmal_comm_enviar((hashmal_canal_id_t)i, msg, tam);
        if (rc == HASHMAL_COMM_OK) {
            ++n_exitos;
        } else {
            ++n_fallos;
            ultimo_err = rc;
        }
    }

    if (n_operativos == 0u) {
        MENORA_LOG_E("broadcast: NINGÚN ner operativo — no hay luz que enviar");
        return HASHMAL_COMM_ERR_CANAL;
    }

    MENORA_LOG_I("broadcast: %u/%u éxitos (fallos=%u) — Éx 25:37 vehéir",
                 (unsigned)n_exitos, (unsigned)n_operativos,
                 (unsigned)n_fallos);

    /* Nm 8:2 + Éx 27:21 — mul penei hammenora / tamíd:
     * Tras iluminar todos los frentes, el tronco (GEZA, canal 0)
     * vuelve a escuchar. Resintonizar al qanah central y activar
     * RX continuo para captar el clamor del hermano.
     *
     * "ken ʻasáh Aharon" (Nm 8:3) — así lo hizo Aarón: encendió
     * y volvió a su puesto de escucha frente al tronco.
     *
     * 2 testigos:
     *   Nm 8:2   — "el mul penéi hammenorá" (frente = canal GEZA)
     *   Éx 27:21 — "mimérev ʻad bóqer tamíd" (siempre escuchando) */
    {
        uint32_t freq_geza = m->nerot[HASHMAL_CANAL_GEZA].frecuencia_hz;
        if (freq_geza > 0u) {
            (void)hashmal_comm_configurar_frecuencia(HASHMAL_CANAL_GEZA,
                                                      freq_geza);
            (void)hashmal_comm_spi_modo_rx();
        }
    }

    if (n_exitos > 0u) return HASHMAL_COMM_OK;
    return ultimo_err;    /* todos los operativos fallaron */
}

/* ==================================================================
 * API PUBLICA - vecinos_contar (fase 1: single-hop)
 * ================================================================== */

/**
 * hashmal_comm_vecinos_contar - ?cuantos vecinos alcanzables?
 *
 * FASE 1 (single-hop): el numero de vecinos es simplemente la
 * cantidad de canales ENCENDIDOS - cada canal activo representa
 * UN vecino potencial alcanzable directamente.
 *
 * FASE 2 (multi-hop, futura): los 22 gevi'im del mesh mantendran
 * una tabla real de vecinos descubiertos (via HELLO/beacon). Los
 * almendros vigilaran quien esta despierto en el vecindario. 1 Re
 * 7:49 describe DIEZ menoras en el Templo de Salomon - esa escala
 * hara inevitable el multi-hop.
 *
 * Por ahora, la equivalencia es exacta para topologia plana:
 * cuantos canales puedo usar = cuantos vecinos puedo tocar.
 */
int hashmal_comm_vecinos_contar(uint8_t *conteo)
{
    if (conteo == NULL) {
        MENORA_LOG_E("vecinos_contar: conteo NULL");
        return HASHMAL_COMM_ERR_NULL;
    }

    const hashmal_menora_t *m = hashmal_comm_get_menora_interno();
    if (m == NULL) {
        *conteo = 0u;
        return HASHMAL_COMM_ERR_INIT;
    }

    /* Fase 1: single-hop = nerot_encendidos. */
    *conteo = m->nerot_encendidos;

    MENORA_LOG_I("vecinos_contar: %u vecinos alcanzables (fase 1 single-hop; "
                 "fase 2 usará los 22 geviʻím — 1 Re 7:49)",
                 (unsigned)*conteo);
    return HASHMAL_COMM_OK;
}

/* ==================================================================
 *
 *   "shlosha gevi'im meshuqqadim baqqane ha'ejad... ken lesheshet
 *    haqqanim hayyotsim min hammenora" - Exodo 25:33
 *   "uvammenora arba'a gevi'im meshuqqadim" - Exodo 25:34
 *
 *   22 copas en forma de almendro. 22 vigias que no duermen.
 *
 *   "hetavta lir'ot ki SHOQED ani 'al devari la'asoto"
 *     - Jeremias 1:12
 *   "Bien has visto, porque YO VELO sobre Mi Palabra para cumplirla."
 *
 *   Los almendros del mesh no duermen sobre los paquetes. Cada
 *   copa meshuqqadet es un ojo despierto - el Padre vela sobre
 *   Su Palabra, y estos vigias vigilan el camino por donde ella
 *   viaja.
 *
 *   v.35 - los kaftorim son las junciones donde los pares
 *   simetricos (1<->4, 2<->5, 3<->6) convergen al tronco. Cuando
 *   llegue el multi-hop, los kaftorim seran los puntos de
 *   reenvio natural - el tavnit ya los marco.
 *
 * ================================================================== */
