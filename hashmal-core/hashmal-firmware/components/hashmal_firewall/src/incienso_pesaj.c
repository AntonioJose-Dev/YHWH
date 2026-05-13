/*
 * ===================================================================
 * PIEZA 14 - INCIENSO SAGRADO - incienso_pesaj.c
 * Añadidura 15: DAM AL HAMEZUZOT - Marcado de Nodos Seguros
 * Exodo 12:1-32 + Ezequiel 9:1-7 + Apocalipsis 7:3 + 1 Pedro 1:19
 * ===================================================================
 *
 * "velaqju min HADDAM venatnu 'al SHTEI HAMMEZUZOT ve'al
 *  HAMMASHQOF 'al habbattim asher yokhlu oto bahem." - Exodo 12:7
 * "Tomaran de la SANGRE y la pondran sobre las DOS JAMBAS y sobre
 *  el DINTEL en las casas donde lo coman."
 *
 * "vehayah HADDAM lakhem leOT 'al habbattim asher attem sham; vera'iti
 *  et HADDAM uPASAJTI 'aleikhem; velo yiheyeh bakhem NEGEF lemashjit
 *  behakkoti be'erets mitsrayim." - Exodo 12:13
 * "La sangre os sera por SENAL en las casas donde esteis; vere la
 *  sangre y PASARE DE LARGO sobre vosotros; no habra en vosotros
 *  PLAGA destructora cuando hiera la tierra de Egipto."
 *
 * "ulqajtem AGUDDAT EZOV utvaltem baddam asher bassaf vehigga'tem
 *  el hammashqof ve'el shtei hammezuzot min haddam asher bassaf;
 *  ve'attem lo tetse'u ish mippetaj beito 'ad boqer." - Exodo 12:22
 * "Tomareis MANOJO DE HISOPO, lo mojareis en la sangre del lebrillo
 *  y untareis el dintel y las dos jambas; ninguno de vosotros
 *  saldra por la puerta de su casa hasta la manana."
 *
 * "ve'avar YHWH lingoph et mitsrayim vera'ah et haddam 'al hammashqof
 *  ve'al shtei hammezuzot uFASAJ YHWH 'al happetaj velo yitten
 *  HAMMASHJIT lavo el batteikhem lingoph." - Exodo 12:23
 * "Pasara YHWH para herir a Egipto y vera la sangre sobre el dintel
 *  y las dos jambas, y pasara YHWH por la puerta y NO PERMITIRA AL
 *  DESTRUCTOR entrar en vuestras casas para herir."
 *
 * "vayyomer YHWH elav: 'avor betokh ha'ir betokh Yerushalayim
 *  veHITVITA TAV 'al mitsjot ha'anashim hanne'enajim vehanne'enaqim
 *  'al kol hatto'evot hanna'asot betokhah." - Ezequiel 9:4
 * "YHWH le dijo: pasa por la ciudad, por en medio de Jerusalen, y
 *  PON UNA MARCA (tav) en la frente de los hombres que GIMEN y se
 *  duelen por todas las abominaciones que se hacen en medio de
 *  ella." (segundo testigo - marca de los justos)
 *
 * "me lutroamhete... timio haimati hos amnou amomou kai aspilou
 *  Christou." - 1 Pedro 1:18-19
 * "Fuisteis rescatados... con la sangre preciosa de Cristo, como
 *  cordero sin defecto y sin mancha." (cumplimiento neotestamentario)
 *
 * --- EXEGESIS OPERATIVA ---
 *
 *   dam (H1818)              SANGRE. Senal visible aplicada al
 *                             exterior del nodo. En el firmware:
 *                             token de 32 bytes que se distribuye
 *                             en emergencia.
 *
 *   shtei mezuzot (H4201)    LAS DOS JAMBAS. Identidad + rol -
 *                             quien eres y que puedes hacer.
 *
 *   mashqof (H4947)          DINTEL. Estado de limpieza - estas
 *                             listo para pasar.
 *
 *   pasaj (H6452)            PASAR DE LARGO. Whitelist activa: el
 *                             destructor ve la sangre y omite la
 *                             casa.
 *
 *   negef (H5063)            PLAGA. Lo que sucede si NO hay marca.
 *                             Daño colateral del juicio.
 *
 *   mashjit (H7843)          DESTRUCTOR. Ex 12:23 explicito: el
 *                             firewall en modo Pesaj ES el mashjit -
 *                             defensa activa que excluye lo no
 *                             marcado.
 *
 *   tav (H8420)              MARCA. Ez 9:4 - segundo testigo. La
 *                             ultima letra del alefato hebreo;
 *                             marca de pertenencia al remanente.
 *
 * --- INVARIANTE INV-NIKRAT-PERMANENTE ---
 *
 * Ex 12:7 dice "al habbattim" (en las CASAS). El nodo NIKRAT
 * (Ex 30:33,38) ya fue cortado de su pueblo - no tiene casa en el
 * campamento. No se inspeccionan dinteles de quien no mora. Por eso
 * `_verificar` chequea NIKRAT ANTES de las 3 marcas: si esta cortado,
 * return false sin mirar mezuzot - el cortado no es candidato a
 * marcar.
 *
 * --- TRES PUNTOS DE VERIFICACION ---
 *
 * Ex 12:7 nombra exactamente TRES superficies: dintel + 2 jambas.
 * Tres es el numero de la confirmacion (Dt 19:15, Mt 18:16). Una
 * sola marca seria insuficiente; dos podrian coincidir falsamente;
 * tres son distintas y compuestas - la falsificacion requeria
 * coordinacion en tres frentes diferentes a la vez.
 *
 * En firmware fase 1 las 3 marcas son booleanas: el caller construye
 * la struct y la pasa. Fase 2 wireara verificacion criptografica:
 *   mezuza_izq -> Pieza 13 keygen verifica firma de identidad
 *   mezuza_der -> Pieza 09 roles verifica autorizacion del rol
 *   mashqof    -> Pieza 12 healthcheck verifica limpieza interna
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_firewall.h          API publica
 *   hashmal_firewall_internal.h get_incienso_interno + ahora_ms
 *                                (mismo componente - acceso pares)
 *   esp_log.h (ORO EGIPTO)      PSJ_LOG_*
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

#include "hashmal_firewall.h"
#include "hashmal_firewall_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *PSJ_TAG = "hashmal.incienso.pesaj";

#define PSJ_LOG_I(fmt, ...)  ESP_LOGI(PSJ_TAG, fmt, ##__VA_ARGS__)
#define PSJ_LOG_W(fmt, ...)  ESP_LOGW(PSJ_TAG, fmt, ##__VA_ARGS__)
#define PSJ_LOG_E(fmt, ...)  ESP_LOGE(PSJ_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * Estado interno
 *
 * El token se guarda crudo en RAM solo durante la emergencia.
 * _expirar lo limpia con memset. Fase 2 puede usar memoria segura
 * de mbedtls; aqui RAM standard basta para el stub.
 * ================================================================== */
static hashmal_pesaj_estado_t g_estado                  = HASHMAL_PESAJ_INACTIVO;
static uint8_t                g_token[HASHMAL_PESAJ_TOKEN_TAM];
static bool                   g_token_presente          = false;
static int64_t                g_timestamp_activacion_ms = 0;
static uint32_t               g_verif_pasadas           = 0;
static uint32_t               g_verif_rechazadas        = 0;

/* ==================================================================
 * Helpers internos
 * ================================================================== */

/* Chequea si el nodo esta NIKRAT consultando la tabla de pares de
 * Pieza 14. Si firewall no esta levantado, asumimos no-NIKRAT
 * (defensivo: en pre-init no se rechaza por NIKRAT desconocido). */
static bool nodo_es_nikrat(uint8_t nodo_id)
{
    if (nodo_id >= HASHMAL_FIREWALL_MAX_PARES) {
        /* Fuera de la tabla - tratamos como "no en el pueblo" arriba
         * en _verificar; aqui solo chequeamos NIKRAT especificamente. */
        return false;
    }
    const hashmal_firewall_t *fw = hashmal_firewall_get_incienso_interno();
    if (fw == NULL) return false;
    return fw->pares[nodo_id].nivel == HASHMAL_FIREWALL_NIVEL_NIKRAT;
}

/* ==================================================================
 * API - hashmal_pesaj_activar (Ex 12:7 velaqju min haddam)
 * ================================================================== */
int hashmal_pesaj_activar(const uint8_t *token, size_t tam)
{
    if (token == NULL) {
        PSJ_LOG_E("activar: token NULL");
        return HASHMAL_FIREWALL_ERR_NULL;
    }
    if (tam != (size_t)HASHMAL_PESAJ_TOKEN_TAM) {
        PSJ_LOG_E("activar: tam=%zu (esperado %u)",
                  tam, (unsigned)HASHMAL_PESAJ_TOKEN_TAM);
        return HASHMAL_FIREWALL_ERR_PESAJ;
    }

    bool reactivacion = (g_estado == HASHMAL_PESAJ_ACTIVO);

    memcpy(g_token, token, HASHMAL_PESAJ_TOKEN_TAM);
    g_token_presente          = true;
    g_estado                  = HASHMAL_PESAJ_ACTIVO;
    g_timestamp_activacion_ms = hashmal_firewall_ahora_ms();

    if (reactivacion) {
        PSJ_LOG_I("Pesaj re-activado - token refrescado "
                  "(Ex 12:7 dam al hammezuzot)");
    } else {
        /* Reset contadores en activacion fresca. */
        g_verif_pasadas    = 0;
        g_verif_rechazadas = 0;
        PSJ_LOG_W("===== PESAJ ACTIVADO ===== dam al hamezuzot "
                  "(Ex 12:7); el mashjit observa los dinteles "
                  "(Ex 12:23)");
    }
    return HASHMAL_FIREWALL_OK;
}

/* ==================================================================
 * API - hashmal_pesaj_verificar (Ex 12:13 ufasajti 'aleikhem)
 * ================================================================== */
bool hashmal_pesaj_verificar(uint8_t nodo_id,
                              const hashmal_pesaj_marca_t *marca)
{
    /* Sin emergencia: todos pasan. */
    if (g_estado != HASHMAL_PESAJ_ACTIVO) {
        return true;
    }

    /* Marca NULL en modo activo: no se puede pasar sin presentarse. */
    if (marca == NULL) {
        PSJ_LOG_W("verificar: nodo %u sin marca (NULL) - rechazado",
                  (unsigned)nodo_id);
        ++g_verif_rechazadas;
        return false;
    }

    /* Ex 28:21 shtem esreh - solo 12 piedras en el Pectoral.
     * Fuera del rango = fuera del pueblo. */
    if (nodo_id >= HASHMAL_FIREWALL_MAX_PARES) {
        PSJ_LOG_W("verificar: nodo %u fuera del Pectoral (max=%u, "
                  "Ex 28:21 shtem esreh) - rechazado",
                  (unsigned)nodo_id,
                  (unsigned)HASHMAL_FIREWALL_MAX_PARES);
        ++g_verif_rechazadas;
        return false;
    }

    /* INV-NIKRAT-PERMANENTE: Ex 30:33,38 - el cortado no tiene casa
     * en el pueblo (Ex 12:7 al habbattim). No se inspeccionan
     * dinteles de quien no mora. Check ANTES de las marcas. */
    if (nodo_es_nikrat(nodo_id)) {
        PSJ_LOG_W("verificar: nodo %u NIKRAT (Ex 30:33) - no recibe "
                  "marca de Pesaj (no tiene casa en el pueblo)",
                  (unsigned)nodo_id);
        ++g_verif_rechazadas;
        return false;
    }

    /* Las tres marcas (Ex 12:7 - dintel + 2 jambas). Las tres
     * deben ser true; si falta una, el dintel esta incompleto. */
    bool ok = marca->mezuza_izq && marca->mezuza_der && marca->mashqof;

    if (ok) {
        ++g_verif_pasadas;
        /* Pasaj silencioso: el atalaya no canta cada paso del
         * destructor; solo loguea las anomalias para no inundar. */
        return true;
    }

    PSJ_LOG_W("verificar: nodo %u SIN marca completa "
              "(izq=%d der=%d mashqof=%d) - mashjit observa "
              "(Ex 12:23 negativo)",
              (unsigned)nodo_id,
              (int)marca->mezuza_izq,
              (int)marca->mezuza_der,
              (int)marca->mashqof);
    ++g_verif_rechazadas;
    return false;
}

/* ==================================================================
 * API - hashmal_pesaj_expirar (la sangre era para esa noche)
 * ================================================================== */
int hashmal_pesaj_expirar(void)
{
    if (g_estado == HASHMAL_PESAJ_INACTIVO) {
        /* Idempotente. */
        return HASHMAL_FIREWALL_OK;
    }

    /* Limpia token con memset (no zero-on-free formal pero suficiente
     * para fase 1; fase 2 puede usar mbedtls_platform_zeroize). */
    memset(g_token, 0, sizeof(g_token));
    g_token_presente = false;
    g_estado         = HASHMAL_PESAJ_EXPIRADO;

    PSJ_LOG_I("Pesaj EXPIRADO - dam limpiado; verificaciones pasadas=%"
              PRIu32 " rechazadas=%" PRIu32,
              g_verif_pasadas, g_verif_rechazadas);

    /* Transicion final a INACTIVO. EXPIRADO es momentaneo - en una
     * implementacion con timer podria persistir un tick para que un
     * caller observador detecte el flanco; aqui pasamos directo. */
    g_estado = HASHMAL_PESAJ_INACTIVO;
    return HASHMAL_FIREWALL_OK;
}

/* ==================================================================
 * API - lecturas puras
 * ================================================================== */
hashmal_pesaj_estado_t hashmal_pesaj_estado_actual(void)
{
    return g_estado;
}

int hashmal_pesaj_info(hashmal_pesaj_info_t *out)
{
    if (out == NULL) return HASHMAL_FIREWALL_ERR_NULL;
    out->estado                    = g_estado;
    out->token_presente            = g_token_presente;
    out->timestamp_activacion_ms   = g_timestamp_activacion_ms;
    out->verificaciones_pasadas    = g_verif_pasadas;
    out->verificaciones_rechazadas = g_verif_rechazadas;
    return HASHMAL_FIREWALL_OK;
}

/* ==================================================================
 *
 *   "velaqju min HADDAM venatnu 'al SHTEI HAMMEZUZOT ve'al
 *    HAMMASHQOF." - Exodo 12:7
 *
 *   "vera'iti et HADDAM uFASAJTI 'aleikhem." - Exodo 12:13
 *
 *   "velo yitten HAMMASHJIT lavo el batteikhem." - Exodo 12:23
 *
 *   "veHITVITA TAV 'al mitsjot ha'anashim." - Ezequiel 9:4
 *
 *   "lutroamhete timio haimati hos amnou amomou." - 1 Pedro 1:18-19
 *
 *   La marca es senal externa pero relacion interna. La sangre en
 *   el dintel no salvaba al de adentro porque la sangre fuera
 *   magia - salvaba porque DECLARABA pertenencia. El destructor
 *   pasaba de largo no porque la sangre fuera carne suya, sino
 *   porque el Padre habia DICHO "esa casa es mia". El firewall
 *   honra el mismo modelo: la marca triple no es seguridad
 *   criptografica primaria - es declaracion publica de pertenencia
 *   verificable en tres frentes. El cortado no se marca porque ya
 *   no pertenece; ese es el peso del karat.
 *
 *   Ez 9:4 amplia: la marca tambien es identificacion de los
 *   justos en medio del juicio. No protege a quien no merece
 *   proteccion, pero exime a quien clama por la corrupcion del
 *   resto. Ambos pasajes convergen: la marca es del que es.
 *
 * ================================================================== */
