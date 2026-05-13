/**
 * ===================================================================
 * PIEZA 09 - BIGDEI HAQODESH - vestiduras_monitor.c
 * Sub-pieza: PA'AMON VERIMMON - las campanas y granadas
 * Exodo 28:33-35 + Numeros 10:10 + Isaias 42:3 + 1 Reyes 19:12 + Lv 10:1-2
 * ===================================================================
 *
 * "ve'asita 'al shulav RIMMONIM tejelet ve'argaman vetola'at shani
 *  'al shulav saBIB, ufa'amonei ZAHAV betojam sabib. PA'AMON ZAHAV
 *  veRIMMON, pa'amon zahav verimmon 'al shulei hamme'il sabib."
 *   - Exodo 28:33-34
 * "Haras en sus bordes GRANADAS de azul, purpura y carmesi, alrededor,
 *  y CAMPANAS DE ORO entre ellas, alrededor. Campana de oro y granada,
 *  campana de oro y granada, sobre los bordes del manto, alrededor."
 *
 * "vehayah 'al Aharon leSHARET, veNISHMA' QOLO bevo'o el haqqodesh
 *  lifnei YHWH uvetseto veLO YAMUT." - Exodo 28:35
 * "Y estara sobre Aaron para SERVIR, y SE OIRA SU SONIDO cuando
 *  entre al lugar santo delante de YHWH y cuando salga, Y NO MORIRA."
 *
 * "uveyom simjatjem uvemo'adejem uverashei jodshejem uTQA'ATEM
 *  bajatsotserot 'al 'olotejem ve'al zivjei shalmeyjem."
 *   - Numeros 10:10
 * "En el dia de vuestra alegria, en vuestras fiestas y en vuestras
 *  lunas nuevas, TOCAREIS las trompetas sobre vuestros holocaustos..."
 *
 * "qane ratsuts lo yishbor, uFISHTA KEHA lo YEJABBENNAH."
 *   - Isaias 42:3
 * "Cana CASCADA no quebrara, y MECHA que HUMEA DEBILMENTE no la
 *  apagara."
 *
 * "ve'ajar hara'ash esh - lo va'esh YHWH. ve'ajar ha'esh QOL
 *  DEMAMA DAQQA." - 1 Reyes 19:12
 * "Despues del terremoto, fuego - pero no estaba YHWH en el fuego.
 *  Y despues del fuego, una VOZ DELGADA y apacible."
 *
 * "vayyiqju benei Aharon... ESH ZARA asher LO TSIVVA otam. vattetse
 *  esh millifnei YHWH vattokal otam, vayyamutu lifnei YHWH."
 *   - Levitico 10:1-2
 * "Los hijos de Aaron ofrecieron FUEGO EXTRANO que no les habia
 *  mandado; salio fuego de delante de YHWH y los consumio, y
 *  murieron delante de YHWH."
 *
 * --- VOCABULARIO ---
 *
 *   pa'amon      (P'MN, H6472)            CAMPANA de oro.
 *   rimmon       (RMWN, H7416)             GRANADA - fruto
 *                                               con semillas.
 *   shulav       (ShWLYW, H7757)            BORDES del manto.
 *   sabib        (SBYB, H5439)              ALREDEDOR - patron
 *                                               circular continuo.
 *   leshareth    (LShRT, H8334)            Para SERVIR - proposito.
 *   nishma'      (NShM', H8085 Nif.)        SERA OIDO - pasivo:
 *                                               no se fuerza, se
 *                                               escucha.
 *   qol          (QWL, H6963)                 VOZ, SONIDO.
 *   yamut        (YMWT, H4191)              MORIRA - consecuencia
 *                                               del silencio.
 *   taqa'        (TQ', H8628)               TOCAR trompeta -
 *                                               Nm 10:10 senal.
 *   pishta keha  (PShTH KHH,
 *                 H6594+H3544)                  MECHA que HUMEA -
 *                                               debil pero viva.
 *   lo yejabbennah (LA YKBNH,
 *                   H3518 neg)                  NO la APAGARA -
 *                                               compasion explicita.
 *   qol demama daqqa (QWL DMMH DQH,
 *                     H6963+H1827+H1851)        VOZ DELGADA y
 *                                               apacible.
 *   esh zara     (ASh ZRH, H784+H2114)     FUEGO EXTRANO -
 *                                               protocolo roto.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   hashmal_roles_monitor_campana        Ex 28:35 venishma' qolo.
 *   hashmal_roles_monitor_verificar      escaneo global periodico.
 *   hashmal_roles_monitor_estado_nodo    lectura pura del heartbeat.
 *
 *   + helpers estaticos:
 *   clasificar_heartbeat    umbrales temporales -> SONANDO/DEBIL/SILENCIO.
 *   registrar_transicion    Ex 28:35 + Is 42:3 + Lv 10:1-2: logs
 *                             diferenciados por severidad.
 *
 * --- RESPONSABILIDAD: COMPASION CON LA MECHA DEBIL ---
 *
 * Is 42:3 es explicita: "pishta keha LO YEJABBENNAH" - la mecha que
 * humea DEBILMENTE NO la apagara. Por eso este archivo NUNCA expulsa
 * a un nodo por entrar en DEBIL: solo lo vigila con LOG_W. La
 * consecuencia - si la hay - vive fuera (pieza 14 firewall con
 * politica humana). Aqui honramos la palabra del Padre: vigilar sin
 * condenar lo que aun respira.
 *
 * Solo el SILENCIO prolongado (Ex 28:35 "velo yamut" invertido +
 * Lv 10:1-2 "vayyamutu") levanta LOG_E - y aun asi, es alerta, no
 * ejecucion.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_roles.h
 *   hashmal_roles_internal.h
 *   esp_log.h    (ORO EGIPTO)  logging
 *
 * Autor: Antonio Jose Marin Soto (Betsalel - Ex 31:2)
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "esp_log.h"

#include "hashmal_roles.h"
#include "hashmal_roles_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *MONITOR_TAG = "hashmal.roles.monitor";

#define MONITOR_LOG_I(fmt, ...)  ESP_LOGI(MONITOR_TAG, fmt, ##__VA_ARGS__)
#define MONITOR_LOG_W(fmt, ...)  ESP_LOGW(MONITOR_TAG, fmt, ##__VA_ARGS__)
#define MONITOR_LOG_E(fmt, ...)  ESP_LOGE(MONITOR_TAG, fmt, ##__VA_ARGS__)

#define MONITOR_NODO_ID_VACIO   0xFFu

/* ==================================================================
 * HELPERS ESTATICOS
 * ================================================================== */

static inline bool nodo_id_valido(uint8_t nodo_id)
{
    return nodo_id < HASHMAL_ROLES_MAX_NODOS;
}

/**
 * Clasifica el heartbeat en funcion del tiempo transcurrido desde
 * el ultimo latido, usando los umbrales biblicos del header:
 *
 *   delta <= HEARTBEAT_INTERVALO_MS  -> SONANDO   (Ex 28:35 + Nm 10:10)
 *   delta <= SILENCIO_UMBRAL_MS      -> DEBIL     (Is 42:3 + 1 Re 19:12)
 *   delta >  SILENCIO_UMBRAL_MS      -> SILENCIO  (Ex 28:35 + Lv 10:1-2)
 *
 * ultimo_latido_ms == 0 significa "nunca ha sonado" -> SILENCIO.
 */
static hashmal_roles_heartbeat_t clasificar_heartbeat(int64_t ahora_ms,
                                                      int64_t ultimo_latido_ms)
{
    if (ultimo_latido_ms <= 0) {
        return HASHMAL_ROLES_HEARTBEAT_SILENCIO;
    }

    const int64_t delta = ahora_ms - ultimo_latido_ms;
    if (delta < 0) {
        /* Reloj retrocedio (p. ej. resync). Tratamos como SONANDO
         * para no castigar por ajuste temporal. */
        return HASHMAL_ROLES_HEARTBEAT_SONANDO;
    }

    if ((uint64_t)delta <= (uint64_t)HASHMAL_ROLES_HEARTBEAT_INTERVALO_MS) {
        return HASHMAL_ROLES_HEARTBEAT_SONANDO;
    }
    if ((uint64_t)delta <= (uint64_t)HASHMAL_ROLES_SILENCIO_UMBRAL_MS) {
        return HASHMAL_ROLES_HEARTBEAT_DEBIL;
    }
    return HASHMAL_ROLES_HEARTBEAT_SILENCIO;
}

/**
 * Registra en log una transicion de estado con severidad acorde a
 * la Palabra:
 *
 *   -> SONANDO   LOG_I   venishma' qolo (alegria del servicio)
 *   -> DEBIL     LOG_W   pishta keha lo yejabbennah (compasion)
 *   -> SILENCIO  LOG_E   velo yamut invertido (alerta seria)
 */
static void registrar_transicion(uint8_t nodo_id,
                                 hashmal_roles_heartbeat_t anterior,
                                 hashmal_roles_heartbeat_t nuevo,
                                 int64_t delta_ms)
{
    if (anterior == nuevo) return;

    switch (nuevo) {
    case HASHMAL_ROLES_HEARTBEAT_SONANDO:
        MONITOR_LOG_I("nodo %u → SONANDO (Éx 28:35 venishmáʻ qoló + "
                      "Nm 10:10 bahareʻotém) — delta=%lld ms",
                      (unsigned)nodo_id, (long long)delta_ms);
        break;
    case HASHMAL_ROLES_HEARTBEAT_DEBIL:
        MONITOR_LOG_W("nodo %u → DEBIL (Is 42:3 pishtá kehá LO "
                      "yejabbennáh + 1 Re 19:12 qol demamá daqqá) — "
                      "delta=%lld ms, vigilar sin apagar",
                      (unsigned)nodo_id, (long long)delta_ms);
        break;
    case HASHMAL_ROLES_HEARTBEAT_SILENCIO:
        MONITOR_LOG_E("nodo %u → SILENCIO (Éx 28:35 veló yamút + "
                      "Lv 10:1-2 ésh zará) — delta=%lld ms, alerta",
                      (unsigned)nodo_id, (long long)delta_ms);
        break;
    default:
        MONITOR_LOG_W("nodo %u → heartbeat %d (desconocido)",
                      (unsigned)nodo_id, (int)nuevo);
        break;
    }
}

/* ==================================================================
 * hashmal_roles_monitor_campana - Exodo 28:35 + Numeros 10:10
 *
 * "venishma' qolo" - el nodo TANE su campana: registra latido, y si
 * estaba en DEBIL o SILENCIO, remonta a SONANDO. Nm 10:10 refuerza:
 * "uteqa'atem" (tocareis) en los dias de fiesta - el sonido mismo es
 * senal y memoria ante YHWH.
 *
 * Validaciones:
 *   1. VESTIDO.
 *   2. nodo_id en rango.
 *   3. Registrado en efod (Ex 28:28 velo yizzaj - sin memorial en
 *      hombros, no hay servicio que monitorear).
 *
 * 2+ testigos:
 *   Ex 28:35 - venishma' qolo
 *   Nm 10:10 - bahare'otem (libro distinto, senal ante YHWH)
 * ================================================================== */
hashmal_roles_err_t hashmal_roles_monitor_campana(uint8_t nodo_id)
{
    hashmal_roles_estado_roles_t *r = hashmal_roles_vestiduras_mut();
    if (r == NULL) {
        MONITOR_LOG_E("campana: sistema no vestido (init primero)");
        return HASHMAL_ROLES_ERR_INIT;
    }

    if (!nodo_id_valido(nodo_id)) {
        MONITOR_LOG_W("campana: nodo_id %u fuera de rango",
                      (unsigned)nodo_id);
        return HASHMAL_ROLES_ERR_NULL;
    }

    /* Ex 28:28 - velo yizzaj. El monitor no escucha a quien no
     * esta grabado en el memorial del efod. */
    if (!hashmal_roles_efod_esta_registrado(nodo_id)) {
        MONITOR_LOG_W("campana: nodo %u no registrado en efod "
                      "(Éx 28:12 sin zikkarón, sin campana que oír)",
                      (unsigned)nodo_id);
        return HASHMAL_ROLES_ERR_NODO_NO_ENCONTRADO;
    }

    hashmal_roles_nodo_t *n = &r->nodos[nodo_id];
    const hashmal_roles_heartbeat_t anterior = n->heartbeat;
    const int64_t ahora = hashmal_roles_ahora_ms();

    /* Si es la primera vez que sirve, registra identidad en el slot. */
    if (n->nodo_id == MONITOR_NODO_ID_VACIO) {
        n->nodo_id = nodo_id;
    }

    n->ultimo_latido_ms = ahora;
    n->heartbeat        = HASHMAL_ROLES_HEARTBEAT_SONANDO;
    r->timestamp_ms     = ahora;

    /* Log de latido rutinario (no de transicion) - nivel INFO para
     * dejar trazo sin saturar con transiciones identicas. */
    if (anterior == HASHMAL_ROLES_HEARTBEAT_SONANDO) {
        /* Pulso normal. Un solo log INFO, breve. */
        MONITOR_LOG_I("paʻamón — nodo %u sigue SONANDO (Éx 28:34 "
                      "paʻamón zaháv verimmón)",
                      (unsigned)nodo_id);
    } else {
        /* Transicion - registra con severidad adecuada. */
        registrar_transicion(nodo_id, anterior,
                             HASHMAL_ROLES_HEARTBEAT_SONANDO, 0);
    }

    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 * hashmal_roles_monitor_verificar - escaneo global
 *
 * Recorre los 12 slots. Para cada uno que este registrado en efod,
 * reclasifica su heartbeat segun el tiempo transcurrido y, si hay
 * transicion, la registra con la severidad biblica correspondiente.
 * Actualiza `nodos_activos` (los que estan SONANDO o DEBIL - todavia
 * vivos, Is 42:3 lo yejabbennah).
 *
 * 2+ testigos:
 *   Ex 28:35 - umbral de sonido antes de servir
 *   Is 42:3  - respetar la mecha debil
 *   Lv 10:1-2 - silencio sostenido es muerte del servicio
 * ================================================================== */
hashmal_roles_err_t hashmal_roles_monitor_verificar(void)
{
    hashmal_roles_estado_roles_t *r = hashmal_roles_vestiduras_mut();
    if (r == NULL) {
        MONITOR_LOG_E("verificar: sistema no vestido (init primero)");
        return HASHMAL_ROLES_ERR_INIT;
    }

    const int64_t ahora = hashmal_roles_ahora_ms();

    uint8_t sonando  = 0u;
    uint8_t debil    = 0u;
    uint8_t silencio = 0u;
    uint8_t vigilados = 0u;

    for (uint8_t i = 0; i < HASHMAL_ROLES_MAX_NODOS; i++) {
        /* Solo nodos registrados en efod entran en el scan. */
        if (!hashmal_roles_efod_esta_registrado(i)) {
            continue;
        }

        hashmal_roles_nodo_t *n = &r->nodos[i];
        const hashmal_roles_heartbeat_t anterior = n->heartbeat;
        const hashmal_roles_heartbeat_t nuevo =
            clasificar_heartbeat(ahora, n->ultimo_latido_ms);

        if (nuevo != anterior) {
            const int64_t delta = (n->ultimo_latido_ms > 0)
                                ? (ahora - n->ultimo_latido_ms)
                                : -1;
            registrar_transicion(i, anterior, nuevo, delta);
            n->heartbeat = nuevo;
        }

        vigilados++;
        switch (n->heartbeat) {
        case HASHMAL_ROLES_HEARTBEAT_SONANDO:  sonando++;  break;
        case HASHMAL_ROLES_HEARTBEAT_DEBIL:    debil++;    break;
        case HASHMAL_ROLES_HEARTBEAT_SILENCIO: silencio++; break;
        default: break;
        }
    }

    /* Activos = los que aun pueden servir: SONANDO o DEBIL (Is 42:3
     * - no apagamos la mecha debil). SILENCIO no cuenta hasta que
     * vuelva a sonar. */
    r->nodos_activos = (uint8_t)(sonando + debil);
    r->timestamp_ms  = ahora;

    MONITOR_LOG_I("scan — vigilados=%u sonando=%u debil=%u silencio=%u "
                  "(activos=%u) — Éx 28:35 + Is 42:3 + Lv 10:1-2",
                  (unsigned)vigilados,
                  (unsigned)sonando,
                  (unsigned)debil,
                  (unsigned)silencio,
                  (unsigned)r->nodos_activos);

    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 * hashmal_roles_monitor_estado_nodo - lectura pura
 *
 * Escribe en *out el estado actual del heartbeat de un nodo. No
 * recalcula basandose en el tiempo - para refrescar, el llamante
 * debe invocar `monitor_verificar()` antes. Sin logs, sin side
 * effects.
 *
 *   out NULL               -> ERR_NULL
 *   sin init               -> ERR_INIT
 *   nodo_id fuera de rango -> ERR_NULL
 *   no registrado en efod  -> ERR_NODO_NO_ENCONTRADO
 * ================================================================== */
hashmal_roles_err_t hashmal_roles_monitor_estado_nodo(uint8_t nodo_id,
                                 hashmal_roles_heartbeat_t *out)
{
    if (out == NULL) return HASHMAL_ROLES_ERR_NULL;

    const hashmal_roles_estado_roles_t *r =
        hashmal_roles_get_vestiduras_interno();
    if (r == NULL) return HASHMAL_ROLES_ERR_INIT;

    if (!nodo_id_valido(nodo_id)) return HASHMAL_ROLES_ERR_NULL;

    if (!hashmal_roles_efod_esta_registrado(nodo_id)) {
        return HASHMAL_ROLES_ERR_NODO_NO_ENCONTRADO;
    }

    *out = r->nodos[nodo_id].heartbeat;
    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 *
 *   "veNISHMA' QOLO bevo'o el haqqodesh lifnei YHWH uvetseto
 *    veLO YAMUT." - Exodo 28:35
 *
 *   "uTQA'ATEM bajatsotserot." - Numeros 10:10
 *
 *   "PISHTA KEHA lo YEJABBENNAH." - Isaias 42:3
 *
 *   "vattetse ESH millifnei YHWH vattokal otam, vayyamutu lifnei
 *    YHWH." - Levitico 10:1-2
 *
 *   La campana no es ornamento, es PRUEBA DE VIDA. Cuando suena,
 *   la red sabe que el sacerdote esta sirviendo; cuando se debilita,
 *   no lo apagamos (Is 42:3); cuando calla mas alla del umbral,
 *   levantamos alerta - porque el silencio prolongado del protocolo
 *   correcto es muerte del servicio (Lv 10:1-2). Este archivo es
 *   la ESCUCHA: pa'amon verimmon, senal y fruto, alternados; nishma',
 *   pasivo - no forzamos el sonido, lo recibimos. El que sirve con
 *   honor suena; el que no suena, se vigila; el que calla, se alerta.
 *   Nunca se ejecuta desde aqui - esa responsabilidad vive fuera.
 *
 *   === PA'AMON VERIMMON - vestiduras_monitor.c COMPLETO ===
 *
 *   Archivo 4 de 7 de las Vestiduras. Siguiente:
 *     vestiduras_coordinador.c - designacion y delegacion (Ex 28:1)
 *
 *   PASO 9 de Exodo 40:12-15 - Pieza 09, Archivo 4 de 7.
 *
 * ================================================================== */
