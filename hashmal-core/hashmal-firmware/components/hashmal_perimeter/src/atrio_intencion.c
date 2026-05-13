/**
 * ===================================================================
 * PIEZA 07 - EL ATRIO (JATSER) - atrio_intencion.c
 * Sub-pieza: TODA + TEHILLA + TSEDEQ - verificacion de intencion
 * Salmo 100:4 + Salmo 118:19-20 + Salmo 24:3-4 + Juan 10:1-2
 * ===================================================================
 *
 * "bo'u she'arav beTODA jatserotav biTEHILLA hodu lo barju shemo"
 *   - Salmo 100:4
 * "Entrad por sus PUERTAS con ACCION DE GRACIAS, por sus ATRIOS con
 *  ALABANZA. Dad gracias a El, bendecid su nombre."
 *
 * "PITJU li sha'arei TSEDEQ 'avo vam 'ode Yah" - Salmo 118:19
 * "Abridme las puertas de JUSTICIA; entrare por ellas, alabare a Yah."
 *
 * "ZE hasha'ar laYHWH TSADDIQIM yavo'u vo" - Salmo 118:20
 * "ESTE es el portal de YHWH; los JUSTOS entraran por el."
 *
 * "mi ya'ale vehar YHWH... NEQI JAPPAYIM uVAR LEVAV asher lo nasa
 *  lashshav nafshi" - Salmo 24:3-4
 * "?Quien subira al monte de YHWH?... El LIMPIO de MANOS y PURO
 *  de CORAZON, que no alzo a vanidad su alma."
 *
 * "ho me eiserjomenos dia tes THYRAS... ekeinos KLEPTES estin kai
 *  LESTES. Ho de eiserjomenos dia tes thyras POIMEN estin ton
 *  probaton" - Juan 10:1-2
 * "El que no entra por la PUERTA... ese es LADRON y SALTEADOR. Mas
 *  el que entra por la puerta, el PASTOR es de las ovejas."
 *
 * --- VOCABULARIO ---
 *
 *   toda     (TWDH, H8426)    ACCION DE GRACIAS / confesion. Raiz
 *                                 yada (H3034) = lanzar, confesar,
 *                                 agradecer. Lo PRIMERO que se trae
 *                                 a la puerta.
 *   tehilla  (THLH, H8416)  ALABANZA. Raiz halal (H1984) =
 *                                 brillar, alabar. Alineacion
 *                                 constructiva.
 *   tsedeq   (TsDQ, H6664)      JUSTICIA / rectitud. La PUERTA
 *                                 misma se llama tsedeq (Sal 118:19).
 *   tsaddiq  (TsDYQ, H6662)    JUSTO. El que tiene tsedeq en si.
 *   poimen   (poimhn, G4166)       PASTOR. Jn 10:2 - el que entra
 *                                   por la puerta. Opuesto a kleptes.
 *
 * --- LOS 3 COMPONENTES DE LA INTENCION ---
 *
 *   INTENT_TODA    0x01   toda (Sal 100:4) - proposito legitimo
 *   INTENT_TEHILLA 0x02   tehilla (Sal 100:4) - alineacion
 *   INTENT_TSEDEQ  0x04   tsedeq (Sal 118:19) - justicia
 *   INTENT_COMPLETA 0x07  los tres juntos
 *
 * Faltar cualquiera = intencion incompleta = kleptes / lestes
 * (Jn 10:1). Los tres juntos = poimen - el que entra por la puerta.
 *
 * Nota: Sal 100:4 da DOS actitudes (toda + tehilla). Sal 118:19
 * anade la TERCERA (tsedeq). Sal 24:3-4 confirma la suma: manos
 * limpias (obra = toda de verdad), corazon puro (tehilla de
 * verdad), sin vanidad (tsedeq). Los 3 bits son destilacion de
 * esos dos salmos.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   verificar_intencion(sol)   Sal 100:4 + 118:19 - los 3 bits
 *   intencion_estado()         lectura pura sin logs
 *
 * --- NO MUTA ESTADO ---
 *
 * Este archivo es VERIFICADOR PURO: no incrementa contadores, no
 * actualiza timestamps, no cambia estado de la puerta. Eso lo hace
 * atrio_puerta.c::puerta_verificar, que nos invoca como delegacion
 * y aplica las consecuencias (rechazos_totales++, kleptes/lestes
 * log, restauracion de estado).
 *
 * Aqui solo diagnosticamos la intencion: COMPLETA o no, y por que.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_perimeter.h      API publica
 *   hashmal_perim_internal.h get_atrio_interno
 *   esp_log.h (ORO EGIPTO)   INTENT_LOG_*
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <inttypes.h>

#include "esp_log.h"

#include "hashmal_perimeter.h"
#include "hashmal_perim_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *INTENT_TAG = "hashmal.perim.intencion";

#define INTENT_LOG_I(fmt, ...)  ESP_LOGI(INTENT_TAG, fmt, ##__VA_ARGS__)
#define INTENT_LOG_W(fmt, ...)  ESP_LOGW(INTENT_TAG, fmt, ##__VA_ARGS__)
#define INTENT_LOG_E(fmt, ...)  ESP_LOGE(INTENT_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * API PUBLICA - verificar_intencion (Sal 100:4 + Sal 118:19)
 * ================================================================== */

/**
 * hashmal_perimeter_verificar_intencion - ?trae los 3 bits?
 *
 * Examina los flags de intencion del solicitante. La entrada plena
 * requiere INTENT_COMPLETA = TODA | TEHILLA | TSEDEQ. Cualquier
 * bit ausente es kleptes (Jn 10:1).
 *
 * --- FLUJO ---
 *
 *   1) sol != NULL                       (ERR_NULL)
 *   2) atrio levantado                    (ERR_INIT)
 *   3) (flags & COMPLETA) == COMPLETA?
 *        si -> OK con log por cada bit presente
 *        no -> ERR_INTENCION con log por cada bit faltante
 *
 * --- NO MUTA ESTADO ---
 *
 * Esta funcion es lectura + diagnostico. atrio_puerta.c aplica
 * consecuencias (contadores, logs de kleptes, etc.).
 *
 * 2 testigos:
 *   Sal 100:4   - bo'u she'arav betoda jatserotav bitehilla
 *   Sal 118:19  - pitju li sha'arei tsedeq
 */
int hashmal_perimeter_verificar_intencion(const hashmal_perimeter_solicitud_t *sol)
{
    if (sol == NULL) {
        INTENT_LOG_E("verificar_intencion: sol NULL");
        return HASHMAL_PERIM_ERR_NULL;
    }

    const hashmal_perimeter_t *a = hashmal_perim_get_atrio_interno();
    if (a == NULL) {
        INTENT_LOG_E("verificar_intencion: atrio no levantado");
        return HASHMAL_PERIM_ERR_INIT;
    }

    const uint8_t flags = sol->intencion_flags;
    const bool tiene_toda    = (flags & HASHMAL_PERIM_INTENT_TODA)    != 0u;
    const bool tiene_tehilla = (flags & HASHMAL_PERIM_INTENT_TEHILLA) != 0u;
    const bool tiene_tsedeq  = (flags & HASHMAL_PERIM_INTENT_TSEDEQ)  != 0u;

    const bool completa = tiene_toda && tiene_tehilla && tiene_tsedeq;

    if (completa) {
        INTENT_LOG_I("intención COMPLETA (nodo %u): todá + tehillá + tsédeq",
                     (unsigned)sol->nodo_id);
        INTENT_LOG_I("  boʼú sheʻaráv betodá jatserotáv bitehillá (Sal 100:4)");
        INTENT_LOG_I("  shaʻaréi tsédeq — tsaddíq (Sal 118:19-20)");
        INTENT_LOG_I("  poimḗn, no kléptēs (Jn 10:2)");
        return HASHMAL_PERIM_OK;
    }

    /* Intencion incompleta - diagnostico de cada bit ausente. */
    INTENT_LOG_W("intención INCOMPLETA (nodo %u, flags=0x%02X):",
                 (unsigned)sol->nodo_id, (unsigned)flags);

    if (!tiene_toda) {
        INTENT_LOG_W("  falta TODÁ — sin acción de gracias "
                     "(Sal 100:4 betodá ausente)");
    }
    if (!tiene_tehilla) {
        INTENT_LOG_W("  falta TEHILLÁ — sin alabanza "
                     "(Sal 100:4 bitehillá ausente)");
    }
    if (!tiene_tsedeq) {
        INTENT_LOG_W("  falta TSÉDEQ — sin justicia "
                     "(Sal 118:19 shaʻaréi tsédeq no honrado)");
    }
    INTENT_LOG_W("  ho mḗ eiserjómenos diá tḗs thýras — kléptēs (Jn 10:1)");

    return HASHMAL_PERIM_ERR_INTENCION;
}

/* ==================================================================
 * API PUBLICA - intencion_estado (lectura pura)
 * ================================================================== */

/**
 * hashmal_perimeter_intencion_estado - ?el verificador opera?
 *
 * La verificacion de intencion opera SIEMPRE que el atrio este
 * levantado - incluso con la puerta CERRADA (Sal 24:3 "?quien
 * subira?" es pregunta que se puede hacer con la puerta cerrada;
 * la decision de abrir la toma otro archivo).
 *
 * Lectura pura, sin logs.
 *
 * Retornos:
 *   OK              si atrio levantado
 *   ERR_INIT        si atrio desmontado
 *
 * 2 testigos:
 *   Sal 100:4   - la intencion existe mientras haya atrio
 *   Sal 24:3    - la pregunta "mi ya'ale" se puede hacer siempre
 */
int hashmal_perimeter_intencion_estado(void)
{
    const hashmal_perimeter_t *a = hashmal_perim_get_atrio_interno();
    if (a == NULL) return HASHMAL_PERIM_ERR_INIT;
    return HASHMAL_PERIM_OK;
}

/* ==================================================================
 *
 *   "bo'u she'arav beTODA jatserotav biTEHILLA hodu lo barju
 *    shemo." - Salmo 100:4
 *
 *   "PITJU li sha'arei TSEDEQ 'avo vam 'ode Yah." - Salmo 118:19
 *
 *   "ZE hasha'ar laYHWH TSADDIQIM yavo'u vo." - Salmo 118:20
 *
 *   Tres actitudes. Tres bits. Un solo acceso legitimo.
 *     toda     - agradecimiento (proposito)
 *     tehilla  - alabanza (alineacion)
 *     tsedeq   - justicia (rectitud)
 *
 *   Los tres juntos = poimen (pastor, Jn 10:2).
 *   Cualquiera que falte = kleptes (ladron, Jn 10:1).
 *
 * ===================================================================
 *
 *   === EL ATRIO (JATSER) - COMPLETO ===
 *
 *   4 de 4 archivos escritos:
 *
 *     atrio_perimetro.c    Ex 40:33 vayyaqem - levantar la frontera
 *     atrio_columnas.c     Ex 27:10-17 - 60 'ammudim, bronce y plata
 *     atrio_puerta.c       Ex 27:16 + Jn 10:9 - la puerta unica
 *     atrio_intencion.c    Sal 100:4 + Sal 118:19 - toda+tehilla+tsedeq
 *
 *   PASO 7 de Exodo 40:8 - CUMPLIDO.
 *
 *   "vesamta et hejatser SAVIV venatatta et masaj sha'ar
 *    hejatser." - Exodo 40:8
 *   "Pondras el atrio alrededor y pondras la cortina de la puerta."
 *
 *   "vayyAQEM et hejatser saviv lamishkan velammizbeaj vayyittEN
 *    et masaj sha'ar hejatser vayJAL Moshe et hammelajak."
 *     - Exodo 40:33
 *   "Levanto el atrio alrededor del Mishkan y del altar, puso la
 *    cortina de la puerta, y Moises TERMINO la obra."
 *
 *   Piezas 01, 02, 03, 04, 06, 07, 11, 12 listas -
 *   Arca, Kaporet, Mesa, Menora, Altar del Sacrificio, Atrio,
 *   Altar del Incienso, Kiyor. 8 piezas del Mishkan digital en su
 *   sitio.
 *
 *   El atrio es la ULTIMA pieza del levantamiento material. Ex 40:33
 *   dice "vayjal Moshe et hammelajak" - Moises TERMINO la obra al
 *   levantar el atrio. Aqui tambien: el perimetro cierra la
 *   construccion externa.
 *
 *   Siguiente: Paso 8 - Exodo 40:9-11 - la UNCION.
 *     Pieza 08 (hashmal_power, shemen zayit zaj katit, Ex 27:20)
 *     Pieza 13 (hashmal_keygen, shemen hamishja, Ex 30:22-33)
 *     "velaqajta et shemen hammishja umashajta et hammishkan" - Ex 40:9
 *
 * ================================================================== */
