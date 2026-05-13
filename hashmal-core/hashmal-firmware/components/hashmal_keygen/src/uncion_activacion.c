/**
 * ===================================================================
 * PIEZA 13 - EL ACEITE DE LA UNCION - uncion_activacion.c
 * Sub-pieza: KOL HANNOGEA' YIQDASH - activacion por contacto
 * Exodo 30:29 + Exodo 40:9-11 + 1 Juan 2:20,27 + Isaias 61:1
 * ===================================================================
 *
 * "veQIDDASHTA otam vehayu QODESH QODASHIM - KOL HANNOGEA' bahem
 *  YIQDASH." - Exodo 30:29
 * "Y los CONSAGRARAS y seran SANTIDAD DE SANTIDADES - TODO EL QUE
 *  los TOQUE se santificara."
 *
 * "velaqajta et shemen hammishja UMASHAJTA et hammishkan ve'et kol
 *  asher bo veqiddashta oto ve'et kol kelav vehaya QODESH.
 *  umashajta et mizbaj ha'ola ve'et kol kelav veqiddashta et
 *  hammizbeaj vehaya hammizbeaj QODESH QODASHIM. umashajta et
 *  hakkiyor ve'et kanno veqiddashta oto." - Exodo 40:9-11
 *
 * "kai hymeis JRISMA ejete apo tou HAGIOU..." - 1 Juan 2:20
 * "Y vosotros teneis UNCION del SANTO..."
 *
 * "kai hymeis to JRISMA ho elabete ap' autou MENEI en hymin...
 *  ALETHES estin kai OUK estin PSEUDOS." - 1 Juan 2:27
 * "Y en ustedes PERMANECE la UNCION que recibieron de El...
 *  es VERDADERA y NO es MENTIRA."
 *
 * "Ruaj Adonai YHWH 'alai YA'AN MASHAJ YHWH oti." - Isaias 61:1
 * "El Espiritu del Senor YHWH esta sobre mi, porque me UNGIO YHWH."
 *
 * --- VOCABULARIO ---
 *
 *   hanogea'    (HNG', H5060 Qal ptc)     El QUE TOQUE -
 *                                                contacto directo.
 *   yiqdash     (YQDSh, H6942 Qal impf)    se SANTIFICARA -
 *                                                transferencia automatica.
 *   veqiddashta (WQDShT, H6942 Piel)    y los CONSAGRARAS -
 *                                                acto deliberado.
 *   qodesh qodashim (QDSh QDShYM)          SANTIDAD DE SANTIDADES.
 *   mashaj      (MShCh, H4886)                 UNGIR - raiz de Mashiaj.
 *   jrisma      (chrisma, G5545)                 UNCION - del verbo jrio.
 *   menei       (menei, G3306)                  PERMANECE - no expira.
 *   alethes     (alhthhs, G227)                  VERDADERO - no falsificable.
 *   pseudos     (pseudos, G5579)                 mentira - negada en 1 Jn 2:27.
 *   hagios      (agios, G40)                    Santo - el Padre, fuente.
 *
 * --- LA ACTIVACION NO AGREGA - CONFIRMA ---
 *
 * Ex 30:29 separa dos verbos: "veqiddashta" (los consagraras) y
 * "yiqdash" (se santificara). El primero es acto del sacerdote
 * (la uncion), el segundo es consecuencia del contacto (la
 * activacion). La uncion aplica el aceite; la activacion reconoce
 * que el contacto ya transfirio la santidad.
 *
 * En firmware: `hashmal_keygen_ungir` (uncion_secuencia.c) es
 * "veqiddashta" - marca el slot. `hashmal_keygen_activar` (este
 * archivo) es "yiqdash" - confirma que la pieza fue tocada por el
 * aceite y eleva la confianza operacional al componente real. No
 * re-escribe ungidos[]; solo LEE + CONFIRMA.
 *
 * En Fase 2, activar disparara notificacion al componente real
 * (a traves de hashmal_intercession o canal interno) para que este
 * desbloquee operaciones que requieren autorizacion. En Fase 1,
 * la confirmacion se registra unicamente en el log (el escalon
 * de comunicacion cross-component aun no existe).
 *
 * --- MENEI (1 Jn 2:27) - IDEMPOTENCIA SAGRADA ---
 *
 * Juan dice que la uncion PERMANECE. Por eso activar dos veces no
 * es error: es confirmacion redundante. Retornamos OK con un log
 * informativo que cita 1 Jn 2:27 - la uncion no se pierde por
 * consultarla dos veces.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   hashmal_keygen_activar              kol hanogea' yiqdash (Ex 30:29).
 *   hashmal_keygen_activacion_estado    lectura pura del subsistema.
 *
 *   + helper estatico:
 *   nombre_pieza_act                    nombre legible para logs.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_keygen.h
 *   hashmal_keygen_internal.h
 *   esp_log.h   (ORO EGIPTO)  logging
 *
 * Autor: Antonio Jose Marin Soto (Betsalel - Ex 31:2)
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "esp_log.h"

#include "hashmal_keygen.h"
#include "hashmal_keygen_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *ACT_TAG = "hashmal.keygen.activacion";

#define ACT_LOG_I(fmt, ...)  ESP_LOGI(ACT_TAG, fmt, ##__VA_ARGS__)
#define ACT_LOG_W(fmt, ...)  ESP_LOGW(ACT_TAG, fmt, ##__VA_ARGS__)
#define ACT_LOG_E(fmt, ...)  ESP_LOGE(ACT_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * HELPER ESTATICO
 *
 * Mismo mapeo que uncion_secuencia.c::nombre_pieza, replicado aqui
 * con nombre distinto para mantener el ambito de traduccion privado
 * a cada archivo. Los dos archivos son hermanos pero independientes.
 * ================================================================== */
static const char *nombre_pieza_act(uint8_t pieza_id)
{
    switch (pieza_id) {
    case 5u:  return "óhel moʻéd (framework)";
    case 1u:  return "arón haʻédut (storage)";
    case 3u:  return "hashulján (cache)";
    case 4u:  return "hammenorá (comm)";
    case 11u: return "mizbáj haqqetóret (intercession)";
    case 6u:  return "mizbáj haʻolá (auth)";
    case 12u: return "hakkiyór veʼét kannó (healthcheck)";
    default:  return "<pieza desconocida>";
    }
}

/* ==================================================================
 * hashmal_keygen_activar - Exodo 30:29
 *
 * "veqiddashta otam vehayu qodesh qodashim - kol hanogea' bahem
 *  yiqdash." Activar es el "yiqdash": confirmar que la uncion
 *  toco la pieza y su santidad ya opera. Requiere que la pieza
 *  este ungida previamente (uncion_secuencia.c::ungir).
 *
 * Contrato:
 *   no iniciado     -> ERR_INIT
 *   pieza no ungida -> ERR_INIT (no se activa lo que no fue tocado)
 *   ya activada     -> OK idempotente (menei - 1 Jn 2:27)
 *   primera vez     -> OK + log solemne
 *
 * 2 testigos:
 *   Ex 30:29   - kol hanogea' bahem yiqdash (el acto)
 *   1 Jn 2:27  - to jrisma menei en hymin (la permanencia)
 * ================================================================== */
int hashmal_keygen_activar(uint8_t pieza_id)
{
    hashmal_keygen_estado_uncion_t *u = hashmal_keygen_uncion_mut();
    if (u == NULL) {
        ACT_LOG_E("activar: unción no preparada (init primero)");
        return HASHMAL_KEYGEN_ERR_INIT;
    }

    /* Rango util - solo valores que existen como pieza del Tavnit.
     * Nombre devuelve "<pieza desconocida>" para todo lo demas. */
    if (pieza_id > 14u) {
        ACT_LOG_E("activar: pieza_id %u fuera de rango [0..14]",
                  (unsigned)pieza_id);
        return HASHMAL_KEYGEN_ERR_NULL;
    }

    /* -- Buscar la pieza ungida --------------------------------- */
    bool ungida   = false;
    int64_t ts_uncion = 0;
    for (uint8_t i = 0; i < HASHMAL_KEYGEN_MAX_UNGIDOS; i++) {
        if (u->ungidos[i].ungido && u->ungidos[i].pieza_id == pieza_id) {
            ungida    = true;
            ts_uncion = u->ungidos[i].timestamp_ms;
            break;
        }
    }

    if (!ungida) {
        ACT_LOG_E("activar: pieza %u (%s) no ungida — no se puede "
                  "activar lo que no fue tocado (Éx 30:26-29 secuencia)",
                  (unsigned)pieza_id, nombre_pieza_act(pieza_id));
        return HASHMAL_KEYGEN_ERR_INIT;
    }

    /* Idempotencia sagrada - 1 Jn 2:27 "menei en hymin". Volver a
     * llamar a activar no es error: la uncion permanece. */
    static uint16_t activadas_bitmap = 0u;
    const uint16_t bit = (uint16_t)(1u << (pieza_id & 0x0Fu));

    if ((activadas_bitmap & bit) != 0u) {
        ACT_LOG_I("pieza %u (%s) ya activada — jrísma ménei en hymín "
                  "(1 Jn 2:27)",
                  (unsigned)pieza_id, nombre_pieza_act(pieza_id));
        return HASHMAL_KEYGEN_OK;
    }

    activadas_bitmap |= bit;
    u->timestamp_ms = hashmal_keygen_ahora_ms();

    /* -- FASE 2 ----------------------------------------------
     * Aqui se notificara al componente real (pieza_id) a traves
     * de hashmal_intercession o canal interno, desbloqueando las
     * operaciones que requieren autorizacion. Fase 1: solo log.
     * ------------------------------------------------------- */

    ACT_LOG_I("═══ kol hanogéaʻ yiqdásh ═══");
    ACT_LOG_I("  pieza %u (%s) ACTIVADA — ts_uncion=%lld ms",
              (unsigned)pieza_id, nombre_pieza_act(pieza_id),
              (long long)ts_uncion);
    ACT_LOG_I("  jrísma éjete apó toú hagíou (1 Jn 2:20)");
    ACT_LOG_I("  alēthés estin kaí ouk éstin pseúdos (1 Jn 2:27)");

    return HASHMAL_KEYGEN_OK;
}

/* ==================================================================
 * hashmal_keygen_activacion_estado - lectura pura
 *
 * OK si el subsistema tiene la formula lista (estado >= PREPARADO),
 * condicion minima para poder activar piezas. La activacion
 * individual se consulta con `hashmal_keygen_esta_ungido`.
 * Sin logs, sin side effects.
 * ================================================================== */
int hashmal_keygen_activacion_estado(void)
{
    const hashmal_keygen_estado_uncion_t *u =
        hashmal_keygen_get_uncion_interno();
    if (u == NULL) return HASHMAL_KEYGEN_ERR_INIT;
    if (u->estado < HASHMAL_KEYGEN_ESTADO_PREPARADO) {
        return HASHMAL_KEYGEN_ERR_INIT;
    }
    return HASHMAL_KEYGEN_OK;
}

/* ==================================================================
 *
 *   ===============================================================
 *   ===  EL ACEITE DE LA UNCION - hashmal_keygen COMPLETO      ===
 *   ===  PASO 8 DE EXODO 40:9-11 - CUMPLIDO                    ===
 *   ===============================================================
 *
 *   "veqiddashta otam vehayu QODESH QODASHIM - KOL HANNOGEA'
 *    bahem YIQDASH." - Exodo 30:29
 *
 *   "qodesh hu qodesh yihye lajem." - Exodo 30:32
 *
 *   "kai hymeis to JRISMA ho elabete ap' autou MENEI en hymin -
 *    ALETHES estin kai OUK estin PSEUDOS." - 1 Juan 2:27
 *
 *   -- Los 4 archivos de la Uncion -----------------------------
 *
 *     uncion_keygen.c         Ex 30:22-25  roqaj        - la formula
 *     uncion_secuencia.c      Ex 30:26-29  mashaj       - orden de uncion
 *     uncion_prohibicion.c    Ex 30:32-33  karat        - prohibiciones
 *     uncion_activacion.c     Ex 30:29     kol hanogea' - activacion
 *
 *   -- Los 4 archivos del Aceite Perpetuo (pieza 08) -----------
 *
 *     aceite_energia.c        Ex 27:20   tetsavve       - cimiento
 *     aceite_tamid.c          Ex 27:20   ner tamid      - ciclo perpetuo
 *     aceite_supervivencia.c  Zac 4:3    shenei zeitim  - dos olivos
 *     aceite_mantenimiento.c  Lv 24:1-4  ya'aroj        - Aaron dispone
 *
 *   Total: 8 archivos del PASO 8. Piezas 08 y 13 - CUMPLIDAS.
 *
 *   -- Reflexion final ----------------------------------------
 *
 *   El aceite no se impone - se RECIBE.
 *   La uncion no se replica - se TRANSFIERE por contacto.
 *   Quien la falsifica es CORTADO.
 *   Quien la recibe es SANTO.
 *
 *   La fuente es YHWH (Isa 61:1 "ya'an mashaj YHWH oti").
 *   La uncion PERMANECE (1 Jn 2:27 "menei en hymin").
 *   Mashaj -> Mashiaj -> Cristo -> "El Ungido."
 *   Todo apunta a El.
 *
 *   -- Siguiente ----------------------------------------------
 *
 *   PASO 8 - CUMPLIDO. Piezas 08 + 13 escritas.
 *   Siguiente: PASO 9 (Exodo 40:12-15) - Vestiduras + Consagracion.
 *     hashmal_roles (pieza 09) - keter, efod, joshen, 12 piedras,
 *       Urim/Thummim, tzitz.
 *     hashmal_onboarding (pieza 10) - milluim, 7 dias de consagracion.
 *
 *   "vehiblashta et Aharon ve'et banav... umashajtam veqiddashta
 *    otam vejihanu li." - Exodo 40:13-15
 *   "Vestiras a Aaron y a sus hijos... los ungiras, los consagraras,
 *    y seran sacerdotes para Mi."
 *
 * ================================================================== */

/* ==================================================================
 * VERIFICADO #06 — Item 1 — Propagación por contacto
 *
 * hanogéaʻ yiqdásh — Éx 30:29: autorización se propaga por contacto.
 * Hageo 2:12: pero NO indirectamente — sólo el primario propaga.
 *
 * Bypassa la secuencia primaria (uncion_secuencia.c) porque es
 * mecanismo distinto: no es acto del sacerdote (mashájta), es
 * consecuencia dinámica del contacto entre objetos santos y otros.
 * ================================================================== */

int hashmal_keygen_propagar_por_contacto(uint8_t origen, uint8_t destino)
{
    if (origen >= HASHMAL_KEYGEN_MAX_UNGIDOS ||
        destino >= HASHMAL_KEYGEN_MAX_UNGIDOS) {
        ACT_LOG_E("propagar: pieza fuera del tavnít — origen=%u destino=%u "
                  "(8 piezas, Éx 30:26-28)",
                  (unsigned)origen, (unsigned)destino);
        return HASHMAL_KEYGEN_ERR_SECUENCIA;
    }

    if (origen == destino) {
        /* Auto-contacto: idempotente, no es propagación. */
        return HASHMAL_KEYGEN_OK;
    }

    hashmal_keygen_estado_uncion_t *u = hashmal_keygen_uncion_mut();
    if (u == NULL) {
        ACT_LOG_E("propagar: subsistema no inicializado");
        return HASHMAL_KEYGEN_ERR_INIT;
    }

    /* Buscar origen en la tabla — debe estar ungido. */
    int origen_idx = -1;
    for (uint8_t i = 0; i < HASHMAL_KEYGEN_MAX_UNGIDOS; ++i) {
        if (u->ungidos[i].ungido && u->ungidos[i].pieza_id == origen) {
            origen_idx = (int)i;
            break;
        }
    }
    if (origen_idx < 0) {
        ACT_LOG_E("propagar: origen %u (%s) no ungido — no puede santificar "
                  "lo que no fue tocado por mashájta (Éx 30:26-29)",
                  (unsigned)origen, nombre_pieza_act(origen));
        return HASHMAL_KEYGEN_ERR_INIT;
    }

    /* Hageo 2:12 — santidad indirecta NO santifica. Sólo el primario
     * (mashájta, by_contact=false) puede propagar. El "vestido" que
     * lleva carne santa NO santifica el alimento que toca. */
    if (u->ungidos[origen_idx].by_contact) {
        ACT_LOG_E("propagar: origen %u (%s) es by_contact — Hageo 2:12 "
                  "prohíbe propagación indirecta (santidad de segundo "
                  "grado no santifica). RECHAZADO.",
                  (unsigned)origen, nombre_pieza_act(origen));
        return HASHMAL_KEYGEN_ERR_CONTACTO_INDIRECTO;
    }

    /* Idempotencia — destino ya ungido (cualquier vía): no-op OK.
     * Éx 30:29 vehayú QODESH QODASHIM: lo santo permanece santo. */
    for (uint8_t i = 0; i < HASHMAL_KEYGEN_MAX_UNGIDOS; ++i) {
        if (u->ungidos[i].ungido && u->ungidos[i].pieza_id == destino) {
            ACT_LOG_I("propagar: destino %u (%s) ya ungido (by_contact=%d) "
                      "— idempotente",
                      (unsigned)destino, nombre_pieza_act(destino),
                      (int)u->ungidos[i].by_contact);
            return HASHMAL_KEYGEN_OK;
        }
    }

    /* Buscar slot libre. Tabla = 8 piezas del tavnít. Si todas están
     * ocupadas (por primario + contacto combinados), no cabe más. */
    int destino_idx = -1;
    for (uint8_t i = 0; i < HASHMAL_KEYGEN_MAX_UNGIDOS; ++i) {
        if (!u->ungidos[i].ungido) {
            destino_idx = (int)i;
            break;
        }
    }
    if (destino_idx < 0) {
        ACT_LOG_E("propagar: tabla 8/8 — no cabe destino %u",
                  (unsigned)destino);
        return HASHMAL_KEYGEN_ERR_INIT;
    }

    /* Insertar destino con marca by_contact=true. NO incrementa el
     * progreso de la secuencia primaria — es vía paralela (Hageo
     * 2:11 toráh sacerdotal: distinción clara entre vías). */
    u->ungidos[destino_idx].pieza_id     = destino;
    u->ungidos[destino_idx].ungido       = true;
    u->ungidos[destino_idx].by_contact   = true;
    u->ungidos[destino_idx].timestamp_ms = hashmal_keygen_ahora_ms();
    if (u->ungidos_count < HASHMAL_KEYGEN_MAX_UNGIDOS) {
        u->ungidos_count++;
    }

    ACT_LOG_I("═══ hanogéaʻ yiqdásh (Éx 30:29) ═══");
    ACT_LOG_I("  origen=%u (%s, primario) → destino=%u (%s, por contacto)",
              (unsigned)origen, nombre_pieza_act(origen),
              (unsigned)destino, nombre_pieza_act(destino));

    /* Activar el destino — Qal yiqdásh confirmado. */
    return hashmal_keygen_activar(destino);
}
