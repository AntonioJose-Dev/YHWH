/**
 * ===================================================================
 * PIEZA 12 - EL KIYOR (LAVACRO) - lavacro_joq_olam.c
 * Sub-pieza: JUQQAT OLAM - ESTATUTO PERPETUO SIN BYPASS
 * Exodo 30:21 + Exodo 39:43 + Numeros 10:8
 * ===================================================================
 *
 * "verajatsu yadeihem veragleihem VELO YAMUTU vehayeta lahem
 *  JUQQAT 'OLAM lo ulezar'o ledorotam" - Exodo 30:21
 * "Lavaran sus manos y pies Y NO MORIRAN; sera para ellos estatuto
 *  PERPETUO, para el y para su simiente por sus generaciones."
 *
 * "vehayeta HUQQAT 'OLAM lekem ledoroteikem" - Numeros 10:8
 * "Y sera ESTATUTO PERPETUO para vosotros por vuestras generaciones."
 *
 * --- JUQQAT 'OLAM - DECRETO CONSTITUCIONAL ---
 *
 * juqqah (CHQH, H2706)  = estatuto, decretos, ordenanza
 * olam   ('WLM, H5769) = perpetuo, eterno, sin fin
 *
 * Juntos: un mandato que NO CAMBIA, NO SE REVOCA, NO SE BYPASSEA.
 * Es la constitucion misma del Mishkan - ni Moises puede anularla.
 *
 * Ex 39:43 - "velo halal davar": Moises vio que nada faltaba en el
 * patrón dado (tavnit). El patrón PERFECTO. El lavamiento es parte
 * de ese patrón perfecto - no inventado, no opcional, no suprimible.
 *
 * --- CRONOLOGIA BIBLICA ---
 *
 * Ex 30:21 - LO PRIMERO: durante la CONSAGRACION, el mandato se
 * declara PERPETUO. "vehayeta lahem juqqat olam" - para ELLOS sera
 * perpetuo. Ni siquiera en el futuro hay exception.
 *
 * Nm 10:8 - REPETICION (segundo testigo): cuando los sacerdotes
 * USAN la trompeta, es bajo "huqqat olam" - la misma palabra. El
 * decreto que vale para el lavamiento vale para la trompeta, para
 * el incienso, para TODO. Nadie puede cambiar la constitucion.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 * Una sola funcion: hashmal_hc_es_joq_olam()
 *
 * Retorna: bool, SIEMPRE true.
 * Implementacion: la funcion cuerpo es trivial.
 * Significado: profundo - el codigo REFLEJA la verdad teologica
 *             "no hay bypass, no hay disable, no hay excepcion".
 *
 * Si en algun momento anterior habia "hc_enabled" flag o bypass
 * mecanismo en otra parte del codigo, ESTE ARCHIVO lo invalida.
 * El sistema RECHAZA tal pensamiento - por diseño.
 *
 * --- COMPILE-TIME ASSERTION ---
 *
 * En algunos compiladores, se puede poner
 *   _Static_assert(HASHMAL_HC_JUQQAT_OLAM, "...")
 * en el init para verificar en tiempo de compilacion que la
 * constante es true. Aca no la anadimos porque sdkconfig ya la
 * establece, y la ley la proclama. Pero la intention permanece:
 * "ni siquiera el compilador puede ignorar el decreto".
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_healthcheck.h     API publica, constantes
 *   hashmal_hc_internal.h     no necesario aca (trivial)
 *   esp_log.h (ORO EGIPTO)    logging
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>

#include "esp_log.h"

#include "hashmal_healthcheck.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *JOQ_TAG = "hashmal.hc.joq_olam";

#define JOQ_LOG_I(fmt, ...)  ESP_LOGI(JOQ_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * VERIFICACION COMPILE-TIME
 *
 * Si HASHMAL_HC_JUQQAT_OLAM no es true, la compilacion FALLA.
 * La constitucion es no-negotiable.
 * ================================================================== */
#if !HASHMAL_HC_JUQQAT_OLAM
#error "HASHMAL_HC_JUQQAT_OLAM DEBE ser true. El lavamiento es perpetuo."
#endif

/* ==================================================================
 * hashmal_hc_es_joq_olam - Exodo 30:21 vehayeta lahem juqqat olam
 *
 * "Y sera para ellos ESTATUTO PERPETUO."
 *
 * La funcion responde a la pregunta: "?puede el healthcheck ser
 * deshabilitado?" La respuesta es NO - es juqqat olam, decreto
 * constitucional que vincula generaciones ("lo ulezar'o ledorotam").
 *
 * Implementacion: trivial - una funcion que retorna true.
 * Significado: profundo - el codigo ENCARNA la verdad de que la
 * integridad NO es opcional.
 *
 * 2 testigos:
 *   Ex 30:21    - "vehayeta lahem juqqat olam" (estatuto perpetuo)
 *   Nm 10:8     - "huqqat olam" (repeticion de decreto eterno)
 * ================================================================== */
bool hashmal_hc_es_joq_olam(void)
{
    /* No hay alternativa. No hay bypass. No hay disable.
     * Ex 30:21 -- "juqqat olam" es mas fuerte que cualquier flag.
     * Nm 16:48 -- Aharon se puso entre los muertos y los vivos.
     * El healthcheck SIEMPRE esta ahi, entre el nodo vivo y la muerte. */
    JOQ_LOG_I("juqqat olam -- Ex 30:21: estatuto perpetuo verificado");
    return true;
}

/* ==================================================================
 * FIN DE ARCHIVO - INICIO EN LA INIT
 *
 * Al inicializar el kiyor (hashmal_hc_init en lavacro_core.c),
 * se verifica IMPLICITAMENTE que juqqat_olam = true. Si algo
 * intentara romper esto, el init colapsaria (colapsar_a_cero).
 *
 * "Conforme a TODO lo que Yo te muestro — el patron del tabernaculo
 *  y el patron de todos sus utensilios — ASI lo haras."
 *   - Exodo 25:9
 *
 * El patron NO tiene "opcion". El Padre no dijo "o lo haras asi O
 * podras omitirlo segun circunstancia". El lavamiento es SIEMPRE.
 *
 * ================================================================== */
