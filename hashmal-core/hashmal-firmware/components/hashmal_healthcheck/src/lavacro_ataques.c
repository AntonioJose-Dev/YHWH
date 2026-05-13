/**
 * ===================================================================
 * PIEZA 12 - EL KIYOR (LAVACRO) - lavacro_ataques.c
 * Sub-pieza: VAYYARATS BEIN HAMMETIM - deteccion de ataques
 * Numeros 17:11-14 + Ezequiel 33:6 + Nehemias 4:9
 * ===================================================================
 *
 * "qaj et hammajtta vesim 'aleha esh min hammizbeaj vesim qetoret
 *  veholej MEHERAH el ha'edah vejapper 'aleihem ki yatsa haqqetseph
 *  milliphne YHWH HEJEL HANNAGEPH" - Nm 17:11
 * "Toma el incensario, pon fuego del altar, pon incienso, y ve
 *  RAPIDO a la congregacion y expia; porque la IRA salio de YHWH,
 *  COMENZO LA PLAGA."
 *
 * "vayyiqqaj Aharon kaasher dibber Mosheh vayyARATS el toj haqqahal
 *  vehinneh HEJEL HANNEGEPH ba'am vayyitten et haqqetoret vayjapper
 *  'al ha'am" - Nm 17:12
 * "Y Aharon tomo como Moises dijo, y CORRIO al medio de la
 *  congregacion, y he aqui que la PLAGA YA HABIA COMENZADO en el
 *  pueblo, y puso incienso e hizo expiacion."
 *
 * "vayya'amod BEIN HAMMETIM UVEIN HAJJAYYIM vatte'atsar
 *  hammaggefeh" - Nm 17:13
 * "Y se puso ENTRE LOS MUERTOS Y LOS VIVOS, y la PLAGA fue DETENIDA."
 *
 * "vayyihyu hammetim bammaggefeh ARBA'AH 'ASAR ELEPH USHVA'
 *  MEOT" - Nm 17:14
 * "Los muertos por la plaga fueron 14,700."
 *
 * "vehatstsofe ki yar'e et hajerev ba'ah velo taqa' bashoshofar...
 *  damo miyyad hatstsofe edrosh" - Ez 33:6
 * "Si el ATALAYA ve venir la espada y no toca la trompeta... su
 *  sangre demandare de la mano del atalaya."
 *
 * --- VOCABULARIO HEBREO ---
 *
 *   vayyarats    (WYRTs, H7323)     CORRIO. Aharon no camino -
 *                                       CORRIO. Urgencia maxima.
 *   meherah      (MHRH, H4120)     RAPIDO. Prisa en la respuesta.
 *   hejel        (H2490)               comenzo.
 *   hannageph    (H5063)               la plaga (raiz nagaph =
 *                                       golpear). Ataque que golpea.
 *   bein         (BYN, H996)         ENTRE. Posicion de barrera.
 *   hammetim     (HMTYM, H4191)   los MUERTOS.
 *   hajjayyim    (HChYYM, H2416)    los VIVOS.
 *   vatte'atsar  (WT'TsR, H6113)  fue DETENIDA (raiz 'atsar =
 *                                       retener, contener).
 *   hammaggefeh  (HMGPH, H4046)  la PLAGA / pestilencia.
 *   hatstsofe    (HTsPH, H6822)    el ATALAYA (raiz tsafa = vigilar).
 *
 * --- AHARON EL MODELO ---
 *
 *   1. CORRIO (vayyarats): la deteccion es urgente. Nada de
 *      deliberacion previa.
 *   2. AL MEDIO (el toj haqqahal): se puso DENTRO del problema,
 *      no lo observo desde fuera.
 *   3. ENTRE MUERTOS Y VIVOS (bein hammetim uvein hajjayyim):
 *      se constituyo en BARRERA. Fisicamente en medio.
 *   4. LA PLAGA SE DETUVO (vatte'atsar hammaggefeh): el acto de
 *      ponerse en medio CONTUVO la propagacion.
 *   5. HUBO PERDIDA PREVIA (14,700, Nm 17:14): la deteccion tardia
 *      tiene costo. Cuanto antes se registra, menos victimas.
 *
 * En firmware: `ataque_registrar` es Aharon corriendo al medio. Cada
 * invocacion se pone ENTRE el nodo muerto (vector que se esta
 * corrompiendo) y el nodo vivo (el resto del sistema). La plaga se
 * contiene por el acto de registrar + escalar el nivel.
 *
 * --- OPERA EN AGUA CONTAMINADA ---
 *
 * La vigilancia NO para cuando el sistema se contamina. Aharon
 * corrio a la plaga que YA ESTABA matando. Ez 33:6 - el atalaya
 * que VE y NO alerta es culpable de la sangre. Aqui: registrar es
 * obligacion perpetua mientras haya kiyor levantado con agua
 * (incluso turbia). Solo SECO bloquea.
 *
 * --- DIVISION DE RESPONSABILIDADES ---
 *
 * Este archivo NO decide transiciones de estado (CONTAMINADO,
 * PURIFICANDO). Solo registra el ataque y delega a nivel_subir
 * (`lavacro_niveles.c`). nivel_subir sabe como recalcular
 * nivel_global y cuando contaminar el kiyor. Aharon corre;
 * Moises organiza el ceremonial. Cada archivo su oficio.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   ataque_registrar(vec)    Nm 17:12-13 - Aharon corre, se pone en medio
 *   ataques_estado()         lectura pura
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_healthcheck.h      API publica (incluye nivel_subir)
 *   hashmal_hc_internal.h      lavacro_mut, get_lavacro_interno,
 *                               ahora_ms, vector_legitimo
 *   esp_log.h (ORO EGIPTO)     ATQ_LOG_*
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <inttypes.h>

#include "esp_log.h"

#include "hashmal_healthcheck.h"
#include "hashmal_hc_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *ATQ_TAG = "hashmal.hc.ataques";

#define ATQ_LOG_I(fmt, ...)  ESP_LOGI(ATQ_TAG, fmt, ##__VA_ARGS__)
#define ATQ_LOG_W(fmt, ...)  ESP_LOGW(ATQ_TAG, fmt, ##__VA_ARGS__)
#define ATQ_LOG_E(fmt, ...)  ESP_LOGE(ATQ_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * HELPER ESTATICO - nombre_vector (log legible)
 *
 * Duplicacion deliberada con lavacro_niveles.c (autocontencion por
 * modulo, patron consistente del proyecto). Los cuatro nombres
 * hebreos son del tavnit, no de este archivo - son constantes
 * semanticas, no estado.
 * ================================================================== */
static const char *nombre_vector(hashmal_hc_vector_t v)
{
    switch (v) {
    case HASHMAL_HC_VEC_YADAYIM:   return "yadáyim";
    case HASHMAL_HC_VEC_RAGLAYIM:  return "ragláyim";
    case HASHMAL_HC_VEC_LEVAV:     return "levav";
    case HASHMAL_HC_VEC_MAYIM:     return "máyim";
    default:                       return "<desconocido>";
    }
}

/* ==================================================================
 * API PUBLICA - ataque_registrar (Nm 17:12-13)
 * ================================================================== */

/**
 * hashmal_hc_ataque_registrar - Aharon corre al medio
 *
 * Registra UN ataque contra un vector. La funcion encarna a Aharon
 * en Nm 17:12-13: corre (vayyarats), se pone entre los muertos y
 * los vivos, y la plaga se contiene.
 *
 * --- FLUJO -----------------------------------------------------
 *
 *   1) Validaciones (lavacro levantado, vec legitimo, no SECO)
 *   2) ataques_detectados += 1           (auditoria global)
 *   3) ultimo_ataque_ms = ahora_ms()
 *   4) vectores[vec].checks_fallidos += 1   (un ataque ES fallo)
 *   5) Delegar a nivel_subir(vec)         (escala severidad 1 paso)
 *   6) Log por el nuevo nivel si cambio
 *   7) Log final: "vayya'amod bein hammetim... #N"
 *
 * --- NO PROPAGA ERRORES DE nivel_subir -------------------------
 *
 * El registro del ataque SIEMPRE se completa (pasos 2-4) aunque
 * nivel_subir falle despues. El conteo es auditoria perpetua -
 * como los 14,700 contados en Nm 17:14: la Escritura NO omite
 * las victimas por conveniencia. Registramos y luego delegamos.
 *
 * Si nivel_subir retorna error inesperado, se logea como WARN
 * pero la funcion retorna OK para preservar el acto de registro.
 *
 * --- OPERA EN CONTAMINADO --------------------------------------
 *
 * Ez 33:6 - el atalaya que ve y no alerta paga con su sangre.
 * La vigilancia es obligacion perpetua. Solo SECO bloquea (sin
 * kiyor activo no hay detector).
 *
 * 2 testigos:
 *   Nm 17:12-13  - vayyarats... vayya'amod bein hammetim
 *   Ez 33:6      - hatstsofe... damo miyyad hatstsofe edrosh
 */
int hashmal_hc_ataque_registrar(hashmal_hc_vector_t vec)
{
    hashmal_hc_t *l = hashmal_hc_lavacro_mut();
    if (l == NULL) {
        ATQ_LOG_E("ataque_registrar: kiyór no levantado");
        return HASHMAL_HC_ERR_INIT;
    }
    if (!hashmal_hc_vector_legitimo(vec)) {
        ATQ_LOG_E("ataque_registrar: vec %d fuera de [0, %d)",
                  (int)vec, HASHMAL_HC_VECTORES);
        return HASHMAL_HC_ERR_VECTOR;
    }
    if (l->estado == HASHMAL_HC_SECO) {
        ATQ_LOG_E("ataque_registrar: SECO — sin kiyór no hay atalaya "
                  "(Ez 33:6 hatstsofé ausente)");
        return HASHMAL_HC_ERR_SECO;
    }

    /* -- PASO 1: registrar - SIEMPRE primero (auditoria perpetua) -- */
    const uint64_t ahora = hashmal_hc_ahora_ms();
    l->ataques_detectados += 1ull;
    l->ultimo_ataque_ms    = ahora;
    l->vectores[vec].checks_fallidos += 1ull;

    /* Snapshot del nivel ANTES de escalar (para comparar delta). */
    const hashmal_hc_nivel_t prev = l->vectores[vec].nivel;

    /* -- PASO 2: delegar escalada a lavacro_niveles.c ------------
     * nivel_subir maneja la transicion a CONTAMINADO si el global
     * cae a MAVET. Aqui no duplicamos esa logica. */
    const int rc_subir = hashmal_hc_nivel_subir(vec);
    if (rc_subir != HASHMAL_HC_OK) {
        ATQ_LOG_W("ataque_registrar: nivel_subir rc=%d — registro "
                  "preservado, escalada no aplicada", rc_subir);
        /* No propagamos - el acto de registrar ya esta hecho. */
    }

    /* -- PASO 3: log por el nuevo nivel, si cambio ---------------- */
    const hashmal_hc_nivel_t nuevo = l->vectores[vec].nivel;
    if (nuevo != prev) {
        switch (nuevo) {
        case HASHMAL_HC_TAME:
            ATQ_LOG_W("plaga detectada — vector %s → TAMÉ (Nm 17:11 "
                      "hejél hannágeph)",
                      nombre_vector(vec));
            break;
        case HASHMAL_HC_JATA:
            ATQ_LOG_W("nagáph (H5062) — vector %s bajo ataque → JATÁ",
                      nombre_vector(vec));
            break;
        case HASHMAL_HC_MAVET:
            ATQ_LOG_E("hammaggeféh (Nm 17:14) — vector %s alcanzó MÁVET; "
                      "14,700 cayeron ANTES de que Aharón llegara — "
                      "detección tardía tiene costo",
                      nombre_vector(vec));
            break;
        default:
            break;
        }
    }

    /* -- PASO 4: log final del acto de ponerse en medio ----------- */
    ATQ_LOG_I("vayyaʻamód béin hammetím uvéin hajjayyím (Nm 17:13) — "
              "ataque #%" PRIu64 " registrado contra %s",
              l->ataques_detectados, nombre_vector(vec));

    return HASHMAL_HC_OK;
}

/* ==================================================================
 * API PUBLICA - ataques_estado (lectura pura)
 * ================================================================== */

/**
 * hashmal_hc_ataques_estado - ?el atalaya puede vigilar?
 *
 * Lectura pura, sin logs. Opera en LLENO, PURIFICANDO y CONTAMINADO
 * - la vigilancia no para cuando la plaga ya esta activa. Mismo
 * patron que espejo_estado (ambos son funciones que operan en agua
 * turbia porque su proposito es JUSTAMENTE diagnosticar el dano).
 *
 * Solo SECO bloquea (sin kiyor, no hay atalaya).
 *
 * 2 testigos:
 *   Nm 17:12  - Aharon corrio aunque la plaga ya habia comenzado
 *   Ez 33:6   - el atalaya debe vigilar SIEMPRE (omision = culpa)
 */
int hashmal_hc_ataques_estado(void)
{
    const hashmal_hc_t *l = hashmal_hc_get_lavacro_interno();
    if (l == NULL) return HASHMAL_HC_ERR_INIT;

    switch (l->estado) {
    case HASHMAL_HC_LLENO:
    case HASHMAL_HC_PURIFICANDO:
    case HASHMAL_HC_CONTAMINADO:
        /* El atalaya sigue de pie aunque el agua se haya turbio. */
        return HASHMAL_HC_OK;
    case HASHMAL_HC_SECO:
        return HASHMAL_HC_ERR_SECO;
    default:
        return HASHMAL_HC_ERR_INIT;
    }
}

/* ==================================================================
 *
 *   "vayyARATS el toj haqqahal... vayya'amod BEIN HAMMETIM UVEIN
 *    HAJJAYYIM vatte'atsar hammaggefeh." - Numeros 17:12-13
 *
 *   "vayyihyu hammetim bammaggefeh ARBA'AH 'ASAR ELEPH USHVA'
 *    MEOT." - Numeros 17:14
 *
 *   "hatstsofe ki yar'e et hajerev ba'ah velo taqa' bashoshofar...
 *    damo miyyad hatstsofe edrosh." - Ezequiel 33:6
 *
 *   Cada ataque registrado es Aharon corriendo al medio. Cada
 *   contador incrementado es un acto de ponerse entre el muerto
 *   y el vivo. Cada muertes_prevenidas (lavacro_niveles.c) es la
 *   plaga detenida.
 *
 *   El atalaya que no alerta paga con su sangre (Ez 33:6). El
 *   sistema NUNCA deja de vigilar - ni siquiera cuando el agua
 *   ya esta turbia. La deteccion es obligacion perpetua.
 *
 *   14,700 cayeron ANTES de que Aharon llegara. La Escritura los
 *   CUENTA. El ledger del kiyor tambien cuenta: auditoria perpetua
 *   para que los siguientes aprendan.
 *
 *   5 de 6 archivos del Kiyor - lavacro_ataques.c listo. Queda:
 *     lavacro_dos_niveles.c    Jn 13:10 - louo / nipto
 *
 * ================================================================== */
