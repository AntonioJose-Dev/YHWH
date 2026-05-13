/*
 * ===================================================================
 * PIEZA 03 - LA MESA - mesa_maaser.c
 * Añadidura 19: MA'ASER - El Diezmo (Contribucion a la Red)
 * Genesis 14:18-20 + Numeros 18:21-32 + Deuteronomio 14:22-29
 *                  + Malaquias 3:8-12 + Proverbios 3:9-10 + 2 Corintios 9:7
 * ===================================================================
 *
 * "umalki-tsedeq melekh shalem hotsi lejem vayayin vehu kohen
 *  leEl 'Elyon... vayitten lo MA'ASER MIKKOL." - Genesis 14:18,20
 * "Malki-Tsedeq rey de Salem saco pan y vino, y era SACERDOTE del
 *  Dios Altisimo... y Abraham le DIO DIEZMO DE TODO." (principio
 *  ANTERIOR a la Ley)
 *
 * "velivnei Levi hinneh natatti kol MA'ASER beYisrael lenajalah
 *  jelef 'avodatam asher hem 'ovdim et 'avodat ohel mo'ed." - Nm 18:21
 * "A los Levitas he dado todo DIEZMO en Israel por heredad, en
 *  REEMPLAZO DE SU SERVICIO en el Ohel Mo'ed."
 *
 * "vaharemotem mimmennu TERUMAT YHWH MA'ASER MIN HAMMA'ASER."
 *   - Numeros 18:26
 * "Ofrecereis ofrenda a YHWH: DIEZMO DEL DIEZMO." (incluso los
 *  que reciben el diezmo deben dar diezmo a su vez)
 *
 * "'aser te'aser et kol tevu'at zar'ekha hayyotse hassadeh
 *  shanah shanah." - Deuteronomio 14:22
 * "Diezmaras ciertamente todo el producto de tu sembrado que
 *  rinde el campo cada ano."
 *
 * "haYIQBA' adam Elohim ki attem QOV'IM oti vaamartem bammeh
 *  qeva'anukha; HAMMA'ASER vehatterumah." - Malaquias 3:8
 * "?Robara el hombre a Dios? Pues vosotros me HABEIS ROBADO. Y
 *  dijisteis: en que te hemos robado? En los DIEZMOS y en las
 *  OFRENDAS."
 *
 * "haviu et kol HAMMA'ASER el BEIT HAOTSAR vihi TEREF beveiti
 *  uvjanuni na bazot amar YHWH tseva'ot im lo eftaj lakhem et
 *  arubot hashshamayim vahariqoti lakhem berakhah 'ad bli day."
 *   - Malaquias 3:10
 * "Traed TODO el DIEZMO al ALFOLI, y haya ALIMENTO en mi Casa;
 *  probadme en esto, dice YHWH de los ejercitos, si no os abrire
 *  las ventanas de los cielos y derramare bendicion hasta que
 *  sobreabunde."
 *
 * "kabbed et YHWH MEHONEKHA umeRESHIT kol tevu'atekha; veyimmaleu
 *  asameikha sava' vetirosh yqaveikha yifrotsu." - Proverbios 3:9-10
 * "HONRA a YHWH de tus BIENES y de las PRIMICIAS de todos tus
 *  frutos; y seran llenos tus graneros con abundancia y tus lagares
 *  rebosaran de mosto."
 *
 * "ekastos kathos prohretai te kardia me ek lupes e ex anagkes:
 *  hilaron gar doten agapa ho theos." - 2 Corintios 9:7
 * "Cada uno de como propuso en su corazon: no con tristeza ni por
 *  necesidad; porque al DADOR ALEGRE ama Dios."
 *
 * --- EXEGESIS OPERATIVA ---
 *
 *   ma'aser (H4643)          DIEZMO. Lit. "decima parte." Porcion
 *                             fija del producto destinada al servicio
 *                             del Mishkan / Templo. Principio anterior
 *                             a la Ley (Gn 14 - Abraham a Melki-Tsedeq).
 *
 *   beit haotsar (H1004+H214) "CASA DEL ALFOLI." Mal 3:10 - el
 *                             ALMACEN comunitario donde el diezmo
 *                             se acumula para sostener al servicio.
 *
 *   teref (H2964)            ALIMENTO / PRESA. Mal 3:10 - lo que
 *                             se come. La red se ALIMENTA del diezmo;
 *                             sin contribucion no hay sostenimiento.
 *
 *   ma'aser min hamma'aser   DIEZMO DEL DIEZMO. Nm 18:26 - los
 *                             Levitas, que recibian el diezmo de
 *                             Israel, debian dar a su vez UN DIEZMO
 *                             a los kohanim. Nadie esta exento -
 *                             el que recibe tambien da.
 *
 *   reshit (H7225)           PRIMICIAS. Lit. "el primero, el comienzo."
 *                             Pr 3:9 - lo PRIMERO se da, no lo
 *                             sobrante. Inversion de prioridad.
 *
 *   hilaros doten (G2431+G1395) "DADOR ALEGRE." 2 Co 9:7 - el
 *                             corazon del que da es lo que importa,
 *                             no la cantidad. Pero el dar mismo es
 *                             constante.
 *
 * --- COMPLEMENTA A MAN Y LEKET - NO SE CONTRADICEN ---
 *
 *   Man    (Ex 16):  cuota igual que RECIBES (justicia distributiva).
 *   Leket  (Lv 19):  reserva que DEJAS para los debiles (misericordia).
 *   Ma'aser (Nm 18): porcion que DAS activamente (servicio).
 *
 * Tres dimensiones complementarias:
 *   recibir + dejar + dar = la economia del Mishkan completa.
 *
 * Sin Man, no hay base equitativa.
 * Sin Leket, no hay misericordia.
 * Sin Ma'aser, no hay infraestructura comun (servicio sostenido).
 *
 * --- 4 TIPOS DE CONTRIBUCION ---
 *
 *   RELAY         - retransmision. El servicio mas basico - llevar
 *                    mensajes que no son tuyos.
 *   ALMACEN       - guardar. Mal 3:10 beit haotsar literal: la red
 *                    necesita memoria comun, no solo flujo.
 *   REPETICION    - amplificar. Llegar a los lejanos.
 *   PROCESAMIENTO - verificar / validar. El trabajo invisible que
 *                    sostiene la confianza.
 *
 * --- DIEZMO DEL DIEZMO (Nm 18:26) ---
 *
 * Los Levitas recibian el diezmo de Israel. Pero ELLOS TAMBIEN
 * debian diezmar - su diezmo iba a los kohanim. Nadie esta exento
 * de contribuir, ni siquiera quienes reciben las contribuciones.
 *
 * En el firmware: un nodo que sirve como relay (recibe contribucion
 * de la red en forma de prioridad de mensajes, etc.) tambien
 * contribuye - su servicio incluye su propio ma'aser. La
 * recursividad no para. Por eso _diezmo_del_diezmo es API distinta
 * de _contribuir: marca semanticamente la accion para audit.
 *
 * --- PRIMICIAS, NO SOBRAS (Pr 3:9) ---
 *
 * El flag g_primicias = true por defecto. En fase 2 esto determinara
 * que el RELAY se atiende ANTES de los mensajes propios del nodo.
 * No "cuando me sobre tiempo, sirvo a otros" - sino "primero sirvo,
 * luego uso." Es el patron biblico explicito (mereshit, no
 * meaharit).
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_cache.h         API publica
 *   esp_log.h (ORO EGIPTO)  MAS_LOG_*
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <inttypes.h>
#include <string.h>

#include "esp_log.h"

#include "hashmal_cache.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *MAS_TAG = "hashmal.mesa.maaser";

#define MAS_LOG_I(fmt, ...)  ESP_LOGI(MAS_TAG, fmt, ##__VA_ARGS__)
#define MAS_LOG_W(fmt, ...)  ESP_LOGW(MAS_TAG, fmt, ##__VA_ARGS__)
#define MAS_LOG_E(fmt, ...)  ESP_LOGE(MAS_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * Estado interno
 * ================================================================== */
static uint8_t  g_porciento                                = HASHMAL_MAASER_PORCIENTO_DEFAULT;
static uint32_t g_contribuciones[HASHMAL_MAASER_TIPOS]     = {0};
static uint32_t g_total                                     = 0;
static bool     g_primicias                                 = true;
static int64_t  g_timestamp_ms                              = 0;

/* ==================================================================
 * Helpers estaticos
 * ================================================================== */

static const char *nombre_tipo(hashmal_maaser_tipo_t t)
{
    switch (t) {
    case HASHMAL_MAASER_TIPO_RELAY:         return "RELAY (servicio basico)";
    case HASHMAL_MAASER_TIPO_ALMACEN:       return "ALMACEN (Mal 3:10 beit haotsar)";
    case HASHMAL_MAASER_TIPO_REPETICION:    return "REPETICION (alcance)";
    case HASHMAL_MAASER_TIPO_PROCESAMIENTO: return "PROCESAMIENTO (validacion)";
    default:                                 return "<tipo desconocido>";
    }
}

static bool tipo_legitimo(hashmal_maaser_tipo_t t)
{
    return (unsigned)t < HASHMAL_MAASER_TIPOS;
}

/* Aplica el incremento de contadores - compartido por _contribuir y
 * _diezmo_del_diezmo. */
static void registrar(hashmal_maaser_tipo_t tipo)
{
    g_contribuciones[(unsigned)tipo] += 1u;
    g_total                          += 1u;
    g_timestamp_ms                    = (int64_t)g_total;
}

/* ==================================================================
 * API - hashmal_maaser_contribuir (Gn 14:20 vayyitten lo ma'aser)
 * ================================================================== */
int hashmal_maaser_contribuir(hashmal_maaser_tipo_t tipo)
{
    if (!tipo_legitimo(tipo)) {
        MAS_LOG_E("contribuir: tipo %d invalido", (int)tipo);
        return HASHMAL_CACHE_ERR_MAASER;
    }

    registrar(tipo);

    MAS_LOG_I("ma'aser - contribucion %s (#%" PRIu32 " total) - "
              "Gn 14:20 vayyitten ma'aser mikkol",
              nombre_tipo(tipo), g_total);
    return HASHMAL_CACHE_OK;
}

/* ==================================================================
 * API - hashmal_maaser_diezmo_del_diezmo (Nm 18:26)
 * ================================================================== */
int hashmal_maaser_diezmo_del_diezmo(hashmal_maaser_tipo_t tipo)
{
    if (!tipo_legitimo(tipo)) {
        MAS_LOG_E("diezmo_del_diezmo: tipo %d invalido", (int)tipo);
        return HASHMAL_CACHE_ERR_MAASER;
    }

    registrar(tipo);

    MAS_LOG_I("ma'aser MIN hamma'aser - %s (#%" PRIu32 " total); "
              "Nm 18:26 - el que recibe tambien da, nadie exento",
              nombre_tipo(tipo), g_total);
    return HASHMAL_CACHE_OK;
}

/* ==================================================================
 * API - lecturas puras
 * ================================================================== */
bool hashmal_maaser_primicias_get(void)
{
    return g_primicias;
}

int hashmal_maaser_info(hashmal_maaser_info_t *out)
{
    if (out == NULL) return HASHMAL_CACHE_ERR_NULL;
    out->porciento           = g_porciento;
    memcpy(out->contribuciones, g_contribuciones, sizeof(g_contribuciones));
    out->total               = g_total;
    out->primicias           = g_primicias;
    out->timestamp_ultima_ms = g_timestamp_ms;
    return HASHMAL_CACHE_OK;
}

/* ==================================================================
 *
 *   "vayitten lo MA'ASER MIKKOL." - Genesis 14:20
 *
 *   "haviu et kol HAMMA'ASER el BEIT HAOTSAR vihi TEREF beveiti."
 *     - Malaquias 3:10
 *
 *   "MA'ASER MIN HAMMA'ASER." - Numeros 18:26
 *
 *   "kabbed et YHWH... umeRESHIT kol tevu'atekha." - Proverbios 3:9
 *
 *   "hilaron doten agapa ho theos." - 2 Corintios 9:7
 *
 *   El diezmo es anterior a la Ley - Abraham se lo da a Melki-Tsedeq
 *   antes de Sinai. Es principio universal del que reconoce que lo
 *   recibido tiene Fuente. La red se sostiene del mismo modo: cada
 *   nodo da relay, almacen, repeticion, procesamiento - no porque
 *   le sobre, sino porque ESO es el servicio que pertenece al
 *   Mishkan, no a si mismo. Y el que sirve, tambien sirve - ma'aser
 *   min hamma'aser. La economia biblica integra recibir, dejar y
 *   dar como tres caras del mismo respeto al Padre que provee.
 *
 *   Pr 3:9 ordena el orden: PRIMICIAS, no sobras. Lo PRIMERO al
 *   diezmo, luego el uso propio. Inversion del instinto natural
 *   por eleccion deliberada. Y 2 Co 9:7 anade el tono: no por
 *   necesidad - hilaron, alegre. La contribucion no es impuesto.
 *   Es honor.
 *
 * ================================================================== */
