/**
 * ===================================================================
 * PIEZA 04 - LA MENORA - menora_aceite.c
 * Sub-pieza: EL ACEITE - shemen zayit zaj katit, duty cycle
 * Exodo 27:20-21 + Zacarias 4:2-3, 11-12 + Zacarias 4:6
 * ===================================================================
 *
 * "ve'atta tetsavve et benei Yisra'el veyiqju eleija SHEMEN ZAYIT
 *  ZAJ KATIT LAMMA'OR leha'alot NER TAMID" - Ex 27:20
 * "Y tu mandaras a los hijos de Israel que te traigan ACEITE DE
 *  OLIVA PURO, PRENSADO, para la luminaria, para hacer SUBIR lampara
 *  CONTINUAMENTE."
 *
 * "be'ohel mo'ed mijuts lapparojet... ya'aroj oto Aharon uvanav
 *  me'EREV 'ad BOQER lifne YHWH juqqat 'olam ledorotam" - Ex 27:21
 * "En la Tienda de Reunion... Aaron y sus hijos lo dispondran desde
 *  la TARDE hasta la MANANA - estatuto PERPETUO por sus generaciones."
 *
 * "menorat zahav kulla vegulla 'al roshah veshiv'a neroteiha 'aleiha
 *  shiv'a veshiv'a MUTSAQOT lannerot" - Zac 4:2
 * "Una Menora toda de oro, con un deposito en su cima, siete
 *  lamparas, y SIETE TUBOS para las lamparas."
 *
 * "ushenayim zetim 'aleiha ejad min hayamin hagulla ve'ejad 'al
 *  smolah" - Zac 4:3
 * "Y DOS OLIVOS sobre ella: uno a la DERECHA y uno a la IZQUIERDA."
 *
 * --- EXEGESIS DE 4 PALABRAS ---
 *
 *   shemen (H8081)  ACEITE - la fuente de energia que alimenta la llama.
 *   zayit  (H2132)  OLIVO - organico, de la tierra, no fabricado.
 *   zaj    (H2134)  PURO, claro. Sin impurezas, sin mezcla.
 *   katit  (H3795)  PRENSADO. Paso por presion PARA ser puro.
 *
 * La energia debe ser VERIFICADA (zaj) y PROBADA (katit). No
 * cualquier corriente: solo la que supera el filtro.
 *
 * --- me'EREV 'ad BOQER - duty cycle ---
 *
 * La Menora ardia de tarde a manana (Ex 27:21), no 24h sin parar.
 * Es un CICLO con renovacion. En firmware: duty cycle - periodos
 * activos con renovacion de aceite entre medias. juqqat 'olam: el
 * ciclo no se rompe; si un ciclo se agota, viene el siguiente.
 *
 * --- Zac 4:2-3 DOS OLIVOS, SIETE TUBOS ---
 *
 * gulla (deposito central) + mutsaqot (tubos dedicados por ner) +
 * dos olivos a los lados = modelo de alimentacion con redundancia.
 * La pieza 08 (hashmal_power - Aceite Perpetuo) gestionara el
 * hardware (bateria + solar, dos olivos). Este archivo es la
 * INTERFAZ: como los canales leen y reaccionan al nivel de aceite.
 *
 * --- QUE HACE / QUE NO HACE ---
 *
 *   HACE:                             NO HACE:
 *   - verifica nivel por ner          - gestiona bateria/solar (-> pieza 08)
 *   - reporta suficiencia             - toca SPI / silicio
 *   - aplica duty cycle (ATENUADO)    - modifica energia real
 *   - restituye ENCENDIDO si fluye    - decide estrategia energetica
 *
 * El contrato con pieza 08 (cuando exista): ella ESCRIBE
 * aceite_nivel en cada ner (rellenandolo desde la realidad
 * electrica); nosotros LEEMOS y reaccionamos. Aqui no se toca el
 * hardware de energia - solo los flags que la Menora guarda.
 *
 * --- UNA EXCEPCION DE MUTACION ---
 *
 * aceite_verificar, aceite_nivel, aceite_suficiente NO mutan.
 * SOLO duty_cycle_aplicar muta estado del ner (ENCENDIDO<->ATENUADO):
 * es la razon de ser del aceite - si se agota, la llama desciende;
 * si fluye, sube. Esta funcion es la "voz del aceite" sobre la luz.
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
static const char *MENORA_TAG = "hashmal.menora.aceite";

#define MENORA_LOG_I(fmt, ...)  ESP_LOGI(MENORA_TAG, fmt, ##__VA_ARGS__)
#define MENORA_LOG_W(fmt, ...)  ESP_LOGW(MENORA_TAG, fmt, ##__VA_ARGS__)
#define MENORA_LOG_E(fmt, ...)  ESP_LOGE(MENORA_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * UMBRALES LOCALES (en "unidades de aceite" 0..100)
 *
 * Zac 4 no fija numeros concretos; los elegimos para reflejar las
 * tres zonas naturales: critico / operativo / optimo. El aceite es
 * continuo, la decision no: por eso umbrales discretos.
 * ================================================================== */
#define MENORA_ACEITE_CRITICO    25u   /* <= critico -> atenuar */
#define MENORA_ACEITE_OPTIMO     75u   /* >= optimo  -> plenitud */
#define MENORA_ACEITE_LLENO     100u   /* tope */

/* ==================================================================
 * HELPER ESTATICO - recuento de nerot encendidos
 *
 * Duplicacion deliberada con menora_canales.c (alli tambien existe):
 * cada archivo mantiene su propia version para no crear una
 * dependencia transitiva. El contador es barato; la autocontencion
 * por modulo vale mas que la DRYness estricta.
 * ================================================================== */
static void recontar_encendidos(hashmal_menora_t *m)
{
    uint8_t n = 0u;
    for (uint8_t i = 0; i < HASHMAL_MENORA_CANALES; ++i) {
        if (m->nerot[i].estado == HASHMAL_NER_ENCENDIDO) ++n;
    }
    m->nerot_encendidos = n;
}

/* ==================================================================
 * API PUBLICA - aceite_verificar (Ex 27:20 - zaj katit)
 * ================================================================== */

/**
 * hashmal_comm_aceite_verificar - ?el aceite es zaj?
 *
 * Comprobacion del nivel de aceite de UN canal. Tres zonas:
 *
 *   nivel == 0                 -> ERR_ACEITE (no hay shemen)
 *   0 < nivel <= CRITICO       -> OK con LOG_W (aceite bajo)
 *   CRITICO < nivel <= LLENO   -> OK silencioso (flujo normal)
 *
 * No muta. La accion correctiva (atenuar el ner) la hace
 * `duty_cycle_aplicar`; aqui solo se observa y se emite el aviso.
 *
 * 2 testigos:
 *   Ex 27:20 - "shemen zayit zaj katit" (el aceite debe ser puro)
 *   Zac 4:2  - mutsaqot: cada ner con su propio flujo verificable
 */
int hashmal_comm_aceite_verificar(hashmal_canal_id_t id)
{
    if (!hashmal_comm_canal_legitimo(id)) {
        MENORA_LOG_E("aceite_verificar: canal %d ilegítimo", (int)id);
        return HASHMAL_COMM_ERR_CANAL;
    }

    const hashmal_menora_t *m = hashmal_comm_get_menora_interno();
    if (m == NULL) return HASHMAL_COMM_ERR_INIT;

    const uint8_t nivel = m->nerot[(size_t)id].aceite_nivel;

    if (nivel == 0u) {
        MENORA_LOG_W("ner %d: sin aceite (nivel=0) — Éx 27:20 exige shémen",
                     (int)id);
        return HASHMAL_COMM_ERR_ACEITE;
    }
    if (nivel <= MENORA_ACEITE_CRITICO) {
        MENORA_LOG_W("ner %d: aceite BAJO (%u/100) — considera atenuar "
                     "(Éx 27:20 katít)", (int)id, (unsigned)nivel);
        return HASHMAL_COMM_OK;
    }
    /* nivel > CRITICO: flujo normal, sin log (ruido evitable). */
    return HASHMAL_COMM_OK;
}

/* ==================================================================
 * API PUBLICA - aceite_nivel (lectura pura)
 * ================================================================== */

/**
 * hashmal_comm_aceite_nivel - inspeccion sin toque
 *
 * Nivel 0..100 del canal. No muta. Cuando pieza 08 exista, ella
 * alimentara este numero; por ahora devuelve lo que haya (0 tras
 * init - coherente: no hay aceite real sin Aceite Perpetuo).
 */
int hashmal_comm_aceite_nivel(hashmal_canal_id_t id, uint8_t *nivel)
{
    if (nivel == NULL) {
        MENORA_LOG_E("aceite_nivel: nivel NULL");
        return HASHMAL_COMM_ERR_NULL;
    }
    if (!hashmal_comm_canal_legitimo(id)) {
        MENORA_LOG_E("aceite_nivel: canal %d ilegítimo", (int)id);
        return HASHMAL_COMM_ERR_CANAL;
    }

    const hashmal_menora_t *m = hashmal_comm_get_menora_interno();
    if (m == NULL) return HASHMAL_COMM_ERR_INIT;

    *nivel = m->nerot[(size_t)id].aceite_nivel;
    return HASHMAL_COMM_OK;
}

/* ==================================================================
 * API PUBLICA - aceite_suficiente (bool)
 * ================================================================== */

/**
 * hashmal_comm_aceite_suficiente - ?hay shemen aunque sea poco?
 *
 * Ex 27:20 pide aceite puro. Esta funcion solo pregunta si HAY
 * aceite (> 0). El juicio de "poco" vs "suficiente" lo hace el
 * caller contra sus umbrales (o duty_cycle con CRITICO).
 *
 * Mientras pieza 08 no exista, retorna false para todos los nerot
 * (init los pone en 0). Es correcto: sin Aceite Perpetuo no hay
 * aceite real.
 */
bool hashmal_comm_aceite_suficiente(hashmal_canal_id_t id)
{
    if (!hashmal_comm_canal_legitimo(id)) return false;

    const hashmal_menora_t *m = hashmal_comm_get_menora_interno();
    if (m == NULL) return false;

    return m->nerot[(size_t)id].aceite_nivel > 0u;
}

/* ==================================================================
 * API PUBLICA - duty_cycle_aplicar (Ex 27:21 me'erev 'ad boqer)
 * ================================================================== */

/**
 * hashmal_comm_duty_cycle_aplicar - la voz del aceite sobre la luz
 *
 * "ya'aroj oto Aharon uvanav me'erev 'ad boqer" (Ex 27:21) -
 * dispuesto de tarde a manana. Este ciclo se llama periodicamente
 * (por pieza 08 cuando exista, o por un timer del sistema) y
 * ajusta el estado de los nerot segun el nivel de aceite:
 *
 *   ENCENDIDO, aceite <= CRITICO  -> ATENUADO  (la llama baja)
 *   ATENUADO,  aceite > CRITICO  -> ENCENDIDO (la llama sube de nuevo)
 *   REMOVIDO                     -> skip (Ap 2:5 - ya no existe)
 *   APAGADO                      -> skip (no consume aceite)
 *
 * Esta es la UNICA funcion de aceite.c que muta estado. Justificada:
 * la RAZON DE SER del aceite es alimentar la luz. Cuando el aceite
 * dicta, la luz responde.
 *
 * Transiciones hechas DIRECTAS (sin pasar por canal_apagar/encender)
 * porque son CONDICIONADAS por el aceite, no intencionales. canales.c
 * gestiona la voluntad; aceite.c gestiona la provision.
 *
 * 2 testigos:
 *   Ex 27:21 - me'erev 'ad boqer (ciclo, no continuidad sin pausa)
 *   Zac 4:12 - shiblei hazahav meriqim (los tubos vierten flujo)
 */
int hashmal_comm_duty_cycle_aplicar(void)
{
    hashmal_menora_t *m = hashmal_comm_menora_mut();
    if (m == NULL) {
        MENORA_LOG_E("duty_cycle: Menorá no levantada");
        return HASHMAL_COMM_ERR_INIT;
    }

    uint8_t n_atenuados   = 0u;
    uint8_t n_restituidos = 0u;
    uint8_t n_apagados    = 0u;
    uint8_t n_removidos   = 0u;

    for (uint8_t i = 0; i < HASHMAL_MENORA_CANALES; ++i) {
        hashmal_ner_t *ner = &m->nerot[i];

        switch (ner->estado) {

        case HASHMAL_NER_REMOVIDO:
            ++n_removidos;                 /* Ap 2:5 - skip */
            break;

        case HASHMAL_NER_APAGADO:
            ++n_apagados;                  /* no consume - skip */
            break;

        case HASHMAL_NER_ENCENDIDO:
            if (ner->aceite_nivel <= MENORA_ACEITE_CRITICO) {
                /* La llama DESCIENDE: sin aceite (0) o critico. Va a
                 * ATENUADO para signalizar degradacion al resto del
                 * sistema - ATENUADO cuenta como "presente pero
                 * debil"; APAGADO seria olvido. */
                ner->estado = HASHMAL_NER_ATENUADO;
                ++n_atenuados;
                if (ner->aceite_nivel == 0u) {
                    MENORA_LOG_W("ner %u: ENCENDIDO → ATENUADO (aceite=0) "
                                 "— Éx 27:20", (unsigned)i);
                } else {
                    MENORA_LOG_W("ner %u: ENCENDIDO → ATENUADO "
                                 "(aceite=%u ≤ %u crítico)",
                                 (unsigned)i,
                                 (unsigned)ner->aceite_nivel,
                                 MENORA_ACEITE_CRITICO);
                }
            }
            break;

        case HASHMAL_NER_ATENUADO:
            if (ner->aceite_nivel > MENORA_ACEITE_CRITICO) {
                /* Aceite restituido. La llama sube de nuevo. Este
                 * es he'ela por gracia del aceite - como Aaron
                 * encendiendo por la manana tras la noche.
                 * timestamp_encendido_ms se renueva: la lampara
                 * es "nueva" desde ahora. */
                ner->estado                  = HASHMAL_NER_ENCENDIDO;
                ner->timestamp_encendido_ms  = hashmal_comm_ahora_ms();
                ++n_restituidos;
                MENORA_LOG_I("ner %u: ATENUADO → ENCENDIDO "
                             "(aceite=%u restituido) — Éx 27:21",
                             (unsigned)i, (unsigned)ner->aceite_nivel);
            }
            break;

        default:
            /* estado desconocido - no tocar; defensivo. */
            break;
        }
    }

    /* Recontar encendidos si hubo transiciones que afecten conteo. */
    if (n_atenuados > 0u || n_restituidos > 0u) {
        recontar_encendidos(m);
    }

    MENORA_LOG_I("duty cycle aplicado (Éx 27:21 meʻérev ʻad bóqer): "
                 "atenuados=%u, restituidos=%u, apagados=%u, "
                 "removidos=%u — encendidos ahora=%u/%d",
                 (unsigned)n_atenuados,
                 (unsigned)n_restituidos,
                 (unsigned)n_apagados,
                 (unsigned)n_removidos,
                 (unsigned)m->nerot_encendidos,
                 HASHMAL_MENORA_CANALES);
    return HASHMAL_COMM_OK;
}

/* ==================================================================
 * API PUBLICA - aceite_nivel_set (Zac 4:12 hammeriqim)
 * ================================================================== */

/**
 * hashmal_comm_aceite_nivel_set - los tubos de oro VIERTEN
 *
 * "shenei shibbolei hazzahav HAMMERIQIM me'alehem et hazzahav"
 *   - Zacarias 4:12
 * "Las dos espigas de oro QUE VIERTEN de si mismas el aceite dorado."
 *
 * Solo pieza 08 (hashmal_power) debe llamar esta funcion — ella
 * posee la gulla (deposito, Zac 4:2) y vierte el aceite en cada
 * ner. El codigo de menora solo REACCIONA al nivel; pieza 08 lo
 * ESTABLECE via esta puerta.
 *
 * nivel: 0..100. Se clampea a 100 si excede.
 *
 * 2 testigos:
 *   Zac 4:12 - hammeriqim (los tubos que vierten aceite dorado)
 *   Ex 27:20 -> Ex 25:37 - shemen -> ner (aceite fluye -> lampara)
 */
int hashmal_comm_aceite_nivel_set(hashmal_canal_id_t id, uint8_t nivel)
{
    if (!hashmal_comm_canal_legitimo(id)) {
        MENORA_LOG_E("aceite_nivel_set: canal %d ilegítimo", (int)id);
        return HASHMAL_COMM_ERR_CANAL;
    }

    /* Clampeo defensivo: nivel no puede exceder 100. */
    if (nivel > MENORA_ACEITE_LLENO) {
        nivel = MENORA_ACEITE_LLENO;
    }

    hashmal_menora_t *m = hashmal_comm_menora_mut();
    if (m == NULL) {
        /* W, no E: en el arranque (Éx 40:9 paso 8) el Aceite se establece
         * ANTES que la Menorá (Éx 40:4 paso 3). La primera llamada a
         * hashmal_power_mantenimiento() es necesariamente temprana;
         * una segunda llamada post-comm_init (Zac 4:12 hammeriqím) vierte
         * el aceite correctamente. Esta condición es transitoria y esperada.
         * 2 testigos: Éx 40:4+9 (orden del Padre) — Zac 4:12 (segunda vez) */
        MENORA_LOG_W("aceite_nivel_set: Menorá aún no levantada "
                     "(esperado en boot, Éx 40:4+9 — segunda vez en Zac 4:12)");
        return HASHMAL_COMM_ERR_INIT;
    }

    m->nerot[(size_t)id].aceite_nivel = nivel;

    /* Log solo si informativo (no REMOVIDO). Sin ruido en cada ciclo. */
    if (m->nerot[(size_t)id].estado != HASHMAL_NER_REMOVIDO) {
        MENORA_LOG_I("hammeriqím: ner %d ← %u/100 (Zac 4:12 — Éx 27:20)",
                     (int)id, (unsigned)nivel);
    }

    return HASHMAL_COMM_OK;
}

/* ==================================================================
 *
 *   "shemen zayit zaj katit lamma'or leha'alot ner tamid"
 *     - Exodo 27:20
 *   "Aceite de oliva PURO, PRENSADO, para la luminaria, para hacer
 *    subir lampara continuamente."
 *
 *   "lo vejayil velo vejoaj ki im beruji, amar YHWH tseva'ot"
 *     - Zacarias 4:6
 *   "No con fuerza ni con poder, sino con MI ESPIRITU, dice YHWH
 *    de los ejercitos."
 *
 *   El aceite no se impone - FLUYE. La Menora no se enciende con
 *   fuerza: se enciende con aceite puro, que fue prensado. La
 *   pureza paso por presion. La luz sube porque el aceite sube.
 *
 *   "ushenayim zetim 'aleiha" - Zacarias 4:3. Dos olivos
 *   sostienen el deposito. Redundancia desde el diseno. Pieza 08
 *   los gestionara con hardware; aqui se honra el principio.
 *
 * ================================================================== */
