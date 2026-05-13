/**
 * ===================================================================
 * PIEZA 12 - EL KIYOR (LAVACRO) - lavacro_espejos.c
 * Sub-pieza: MAR'OT HATSTSOVOT - espejos de auto-diagnostico
 * Exodo 38:8 + Santiago 1:23-24 + 1 Corintios 13:12 + 2 Corintios 3:18
 * ===================================================================
 *
 * "vayya'as et hakkiyor nejoshet veet kanno nejoshet beMAR'OT
 *  HATSTSOVOT asher tsav'u PETAJ ohel mo'ed" - Exodo 38:8
 * "Hizo el lavacro de bronce y su base de bronce, DE LOS ESPEJOS
 *  de las mujeres que SERVIAN a la PUERTA de la Tienda."
 *
 * "ei tis akroates logou kai ou poietes, houtos eoiken andri
 *  katanoounti to prosopon tes geneseos autou en ESOPTRO"
 *   - Santiago 1:23
 * "Si alguno es oidor y no hacedor, es como el hombre que mira
 *  su rostro natural EN UN ESPEJO."
 *
 * "blepomen gar arti di' ESOPTROU en AINIGMATI" - 1 Co 13:12
 * "Ahora vemos por ESPEJO, OSCURAMENTE."
 *
 * "hemeis de pantes anakekalymmeno prosopo ten doxan kyriou
 *  KATOPTRIZOMENOI ten auten eikona METAMORPHOUMETHA" - 2 Co 3:18
 * "Nosotros todos, mirando a cara descubierta como en un ESPEJO
 *  la gloria del Senor, somos TRANSFORMADOS."
 *
 * --- VOCABULARIO ---
 *
 *   mar'ot       (MRAT, H4759)     ESPEJOS / VISIONES. Plural de
 *                                      mare'. Raiz ra'a (H7200) = VER.
 *                                      El espejo muestra REALIDAD,
 *                                      no deseo.
 *
 *   hatstsovot   (HTsBAT, H6633)  Las que SERVIAN. Raiz tsava =
 *                                      servir en masa, militar. No
 *                                      espectadoras - servidoras
 *                                      activas de la Tienda.
 *
 *   petaj        (PTCh, H6607)      PUERTA / abertura. El punto de
 *                                      acceso. Las mujeres servian
 *                                      EN la frontera.
 *
 *   esoptron     (esoptron, G2072)    Espejo. Usado en Stg 1:23 y
 *                                      1 Co 13:12. Ver sin cambiar.
 *
 *   katoptrizo   (katoptrizo, G2734)  Mirar como en espejo (reflejar).
 *                                      Voz media: el reflejado SE
 *                                      transforma al mirar.
 *
 * --- TEOLOGIA DEL ESPEJO ---
 *
 * 1) PERSONAL -> COLECTIVO. Las servidoras dieron sus espejos
 *    personales, y Betsalel los fundio en el bronce del kiyor. Lo
 *    que era auto-examinacion PRIVADA se transformo en instrumento
 *    de purificacion COLECTIVA. El espejo paso de mostrar vanidad
 *    (uso privado) a mostrar verdad (servicio comun).
 *
 * 2) MUESTRA, NO CAMBIA (Stg 1:23-24). El espejo REVELA lo que
 *    eres; el cambio viene del HACER. Fase 1: el espejo reporta.
 *    Fase 2 anadira transformacion activa.
 *
 * 3) PARCIAL AHORA (1 Co 13:12). "Di' esoptrou en ainigmati" -
 *    por espejo, oscuramente. La vision actual es parcial. Fase 1
 *    ve lo que puede - la vision plena es Fase 2+.
 *
 * 4) TRANSFORMADOR A LARGO PLAZO (2 Co 3:18). "Katoptrizomenoi...
 *    metamorphoumetha": mirando somos transformados. El uso
 *    repetido del espejo CAMBIA al que se mira. Cada reflexion
 *    (el contador) aporta a la metamorfosis del sistema.
 *
 * --- EL ESPEJO FUNCIONA EN AGUA CONTAMINADA ---
 *
 * A diferencia de los vectores (que requieren agua operativa), el
 * espejo funciona incluso con agua CONTAMINADA. Stg 1:23 - el
 * hombre mira su rostro SIN condicion sobre la limpieza del agua.
 * El diagnostico es ESPECIALMENTE necesario cuando hay
 * contaminacion: hay que ver cuan grave es el dano.
 *
 * Solo SECO bloquea: sin agua no hay superficie donde reflejar.
 *
 * --- SNAPSHOT COMPLETO ---
 *
 * Las mar'ot mostraban la CARA ENTERA - no un ojo, no una mejilla:
 * el rostro completo (prosopon, Stg 1:23). Asi el espejo del
 * kiyor captura el nodo COMPLETO: estado, 4 vectores, contadores,
 * ataques, dos niveles. Una foto integra.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   espejo_examinar(reporte)     Ex 38:8 - snapshot completo
 *   espejo_estado()              lectura pura sin logs
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_healthcheck.h      API publica (incluye espejo_t)
 *   hashmal_hc_internal.h      lavacro_mut, get_lavacro_interno, ahora_ms
 *   esp_log.h (ORO EGIPTO)     ESPEJO_LOG_*
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

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 *
 * NO usamos ESP_TAG (colisiona con macros de ESP-IDF). ESPEJO_TAG
 * evita ambiguedad y preserva convencion local (HC_TAG, VEC_TAG,
 * NIV_TAG en archivos hermanos).
 * ================================================================== */
static const char *ESPEJO_TAG = "hashmal.hc.espejos";

#define ESPEJO_LOG_I(fmt, ...)  ESP_LOGI(ESPEJO_TAG, fmt, ##__VA_ARGS__)
#define ESPEJO_LOG_W(fmt, ...)  ESP_LOGW(ESPEJO_TAG, fmt, ##__VA_ARGS__)
#define ESPEJO_LOG_E(fmt, ...)  ESP_LOGE(ESPEJO_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * API PUBLICA - espejo_examinar (Ex 38:8 mar'ot hatstsovot)
 * ================================================================== */

/**
 * hashmal_hc_espejo_examinar - snapshot completo del kiyor
 *
 * Toma una foto del estado entero del lavacro y la escribe en el
 * reporte del caller. Como las mar'ot mostraban la cara completa
 * (Stg 1:23 prosopon), esta funcion captura el nodo completo:
 * estado + 4 vectores + contadores + ataques + dos niveles.
 *
 * --- PRECONDICIONES ---
 *
 *   reporte != NULL               (ERR_NULL si falta superficie)
 *   lavacro levantado             (ERR_INIT)
 *   estado != SECO                (ERR_SECO: sin agua no hay espejo)
 *
 * --- EFECTOS EN EL SINGLETON ---
 *
 * Minimos y defensivos: solo se tocan los campos del espejo mismo
 * (Stg 1:23-24 - el espejo muestra, no modifica lo examinado).
 *
 *   l->espejo_activo      = true
 *   l->reflexiones       += 1
 *   l->ultimo_espejo_ms   = ahora_ms()
 *
 * NO se tocan: niveles de vectores, contadores globales del servicio
 * (purificaciones, contaminaciones, muertes_prevenidas), etc. El
 * espejo es LECTURA para el reporte y ESCRITURA solo de su propia
 * meta-informacion.
 *
 * --- AGUA CONTAMINADA ---
 *
 * Si estado == CONTAMINADO, el espejo AUN opera - Stg 1:23 no
 * condiciona la funcion del espejo a la pureza del agua. Solo
 * marcamos LOG_W para que el operador sepa que el diagnostico es
 * urgente.
 *
 * 2 testigos:
 *   Ex 38:8   - bemar'ot hatstsovot (espejos de las servidoras)
 *   Stg 1:23  - katanoounti to prosopon en esoptro (mirando el rostro)
 */
int hashmal_hc_espejo_examinar(hashmal_hc_espejo_t *reporte)
{
    if (reporte == NULL) {
        ESPEJO_LOG_E("espejo_examinar: sin superficie — reporte NULL");
        return HASHMAL_HC_ERR_NULL;
    }

    hashmal_hc_t *l = hashmal_hc_lavacro_mut();
    if (l == NULL) {
        ESPEJO_LOG_E("espejo_examinar: kiyór no levantado");
        return HASHMAL_HC_ERR_INIT;
    }

    /* SECO bloquea: sin agua no hay superficie reflectante. */
    if (l->estado == HASHMAL_HC_SECO) {
        ESPEJO_LOG_E("espejo_examinar: estado SECO — sin máyim no hay "
                     "reflejo (Éx 30:18 venatattá ausente)");
        return HASHMAL_HC_ERR_SECO;
    }

    const uint64_t ahora = hashmal_hc_ahora_ms();

    /* -- Partimos de cero en el reporte: sin basura del caller. */
    memset(reporte, 0, sizeof(*reporte));

    /* -- Meta --------------------------------------------------- */
    reporte->timestamp_ms = ahora;
    reporte->estado       = l->estado;
    reporte->nivel_global = l->nivel_global;

    /* -- Per-vector (4 arrays paralelos) ------------------------ */
    for (uint8_t i = 0; i < HASHMAL_HC_VECTORES; ++i) {
        reporte->nivel[i]           = l->vectores[i].nivel;
        reporte->checks_pasados[i]  = l->vectores[i].checks_pasados;
        reporte->checks_fallidos[i] = l->vectores[i].checks_fallidos;
    }

    /* -- Contadores globales de servicio ------------------------ */
    reporte->purificaciones     = l->purificaciones;
    reporte->contaminaciones    = l->contaminaciones;
    reporte->muertes_prevenidas = l->muertes_prevenidas;

    /* -- Ataques + dos niveles (Jn 13:10) ----------------------- */
    reporte->ataques_detectados = l->ataques_detectados;
    reporte->rajats_completo    = l->rajats_completo;
    reporte->nipto_count        = l->nipto_count;

    /* -- Actualizar meta del espejo en el singleton ------------ */
    l->espejo_activo    = true;
    l->reflexiones     += 1ull;
    l->ultimo_espejo_ms = ahora;

    /* El reporte incluye la reflexion AFTER del incremento - asi
     * el caller ve el numero actualizado de su propia reflexion. */
    reporte->reflexiones = l->reflexiones;

    /* -- Log segun pureza del agua ------------------------------ */
    if (l->estado == HASHMAL_HC_CONTAMINADO) {
        ESPEJO_LOG_W("═══ espejo en AGUA CONTAMINADA ═══ "
                     "(Stg 1:23 muestra siempre); diagnóstico URGENTE");
        ESPEJO_LOG_W("  reflexión #%" PRIu64 " @%" PRIu64 " ms — "
                     "nivel_global=%d, muertes_prev=%" PRIu64,
                     reporte->reflexiones, ahora,
                     (int)reporte->nivel_global,
                     reporte->muertes_prevenidas);
    } else {
        ESPEJO_LOG_I("mar'ót hatstsovót (Éx 38:8) — reflexión #%" PRIu64
                     " completada",
                     reporte->reflexiones);
        ESPEJO_LOG_I("  estado=%d, nivel_global=%d, purificaciones=%" PRIu64
                     ", contaminaciones=%" PRIu64
                     ", muertes_prev=%" PRIu64,
                     (int)reporte->estado,
                     (int)reporte->nivel_global,
                     reporte->purificaciones,
                     reporte->contaminaciones,
                     reporte->muertes_prevenidas);
    }
    return HASHMAL_HC_OK;
}

/* ==================================================================
 * API PUBLICA - espejo_estado (lectura pura)
 * ================================================================== */

/**
 * hashmal_hc_espejo_estado - ?el espejo puede reflejar?
 *
 * Opera en LLENO, PURIFICANDO y CONTAMINADO (Stg 1:23 - el espejo
 * muestra siempre). Solo SECO bloquea (sin agua no hay reflejo).
 *
 * Lectura pura - sin logs. Mismo patron que vectores_estado,
 * niveles_estado, fuego_estado del altar.
 *
 * 2 testigos:
 *   Ex 38:8   - mar'ot hatstsovot (existen mientras haya kiyor)
 *   1 Co 13:12 - di' esoptrou (por espejo - incluso en lo parcial)
 */
int hashmal_hc_espejo_estado(void)
{
    const hashmal_hc_t *l = hashmal_hc_get_lavacro_interno();
    if (l == NULL) return HASHMAL_HC_ERR_INIT;

    switch (l->estado) {
    case HASHMAL_HC_LLENO:
    case HASHMAL_HC_PURIFICANDO:
    case HASHMAL_HC_CONTAMINADO:
        /* El espejo refleja aunque el agua este turbia. */
        return HASHMAL_HC_OK;
    case HASHMAL_HC_SECO:
        return HASHMAL_HC_ERR_SECO;
    default:
        return HASHMAL_HC_ERR_INIT;
    }
}

/* ==================================================================
 *
 *   "vayya'as et hakkiyor nejoshet... beMAR'OT HATSTSOVOT asher
 *    tsav'u PETAJ ohel mo'ed." - Exodo 38:8
 *
 *   "katanoounti to prosopon tes geneseos autou en ESOPTRO."
 *     - Santiago 1:23
 *
 *   "blepomen gar arti di' ESOPTROU en AINIGMATI." - 1 Corintios 13:12
 *
 *   "KATOPTRIZOMENOI... METAMORPHOUMETHA." - 2 Corintios 3:18
 *
 *   Lo que era espejo personal se hizo instrumento colectivo.
 *   Lo que mostraba vanidad ahora muestra verdad. Las servidoras
 *   (tsovot - ejercito que sirve) dieron lo mas intimo - su
 *   capacidad de verse - para que otros se purificaran.
 *
 *   El espejo muestra siempre - limpio o turbio. En Fase 1 reporta.
 *   En Fase 2 transformara (2 Co 3:18 - metamorphoumetha).
 *
 *   4 de 6 archivos del Kiyor - lavacro_espejos.c listo. Quedan:
 *     lavacro_ataques.c        Ex 30:20 - prevencion de muerte
 *     lavacro_dos_niveles.c    Jn 13:10 - louo / nipto
 *
 * ================================================================== */
