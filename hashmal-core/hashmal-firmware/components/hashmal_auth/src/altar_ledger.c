/**
 * ===================================================================
 * PIEZA 06 - ALTAR DEL SACRIFICIO - altar_ledger.c
 * Sub-pieza: LAS ALMAS BAJO EL ALTAR - psychas hypokato thysiasteriou
 * Apocalipsis 6:9-11 + Levitico 17:11 + Exodo 29:37
 * ===================================================================
 *
 * "kai hote enoixen ten pempten sphragida, eidon HYPOKATO tou
 *  THYSIASTERIOU tas PSYCHAS ton esphagmenon dia ton LOGON tou
 *  Theou kai dia ten MARTYRIAN hen eichon" - Ap 6:9
 * "Cuando abrio el quinto sello, vi DEBAJO del altar las ALMAS de
 *  los que habian sido sacrificados por la PALABRA de Dios y por
 *  el TESTIMONIO que tenian."
 *
 * "kai EKRAXAN phonei megalei legontes: Heos pote... ou krineis
 *  kai ekdikeis to HAIMA hemon ek ton katoikounton epi tes ges?"
 *   - Ap 6:10
 * "Y CLAMARON a gran voz diciendo: ?Hasta cuando... no juzgas y
 *  vengas nuestra SANGRE?"
 *
 * "kai edothe autois hekastoi STOLE LEUKE... heos PLEROTHOSIN kai
 *  hoi SYNDOULOI auton" - Ap 6:11
 * "Y se les dio a cada uno una VESTIDURA BLANCA... hasta que se
 *  COMPLETASEN tambien sus CONSIERVOS."
 *
 * "ki NEFESH habasar baddam hiv vaani netatTIV lajem 'al hammizbeaj
 *  lejapper 'al nafshoteijem ki haddam HU bannefesh yejapper"
 *   - Lv 17:11
 * "Porque la VIDA de la carne esta en la SANGRE, y YO os la he dado
 *  sobre el ALTAR para expiar por vuestras almas; porque la sangre
 *  ES en la vida la que expia."
 *
 * "shiv'at yamim tejapper 'al hammizbeaj veqiddashta oto vehaya
 *  hammizbeaj QODESH QODASHIM kol hanNOGUEA' bammizbeaj YIQDASH"
 *   - Ex 29:37
 * "Siete dias expiaras el altar... sera el altar SANTISIMO; TODO
 *  lo que TOQUE el altar sera SANTIFICADO."
 *
 * --- VOCABULARIO ---
 *
 *   psychas       (psuchas, G5590)    ALMAS. Cada registro es un alma.
 *                                   Permanecen DEBAJO del altar; no
 *                                   desaparecen al sacrificarse.
 *
 *   hypokato      (upokato, G5270)  DEBAJO. No EN el fuego, no SOBRE
 *                                   la rejilla: en la BASE. Yesod.
 *
 *   thysiasteriou (thusiasthriou,    ALTAR. El mismo. Las almas son
 *                  G2379)           PARTE del altar, no externas.
 *
 *   martyrian     (marturian,       TESTIMONIO. Cada alma tiene su
 *                  G3141)           razon: no solo QUE, tambien POR QUE.
 *
 *   logon         (logon, G3056)    PALABRA. La causa del sacrificio.
 *
 *   ekraxan       (ekraxan, G2896)  CLAMARON. El ledger es CONSULTABLE;
 *                                   los registros hablan cuando se les
 *                                   pregunta.
 *
 *   haima         (aima, G129)      SANGRE. Prueba de vida derramada.
 *                                   Cada registro es sangre -
 *                                   recurso/CPU/ciclo sacrificado.
 *
 *   stole leuke   (stolh leukh,     VESTIDURA BLANCA. Cada registro
 *                  G4749+G3022)     recibe identidad verificada.
 *
 *   plerothosin   (plhrothosin,      COMPLETASEN. El ledger tiene
 *                  G4137)           capacidad; espera sin borrar.
 *
 *   syndouloi     (sundouloi,       CONSIERVOS. Otros nodos que
 *                  G4889)           tambien registran. Fase 2
 *                                   distribuido.
 *
 *   nefesh        (NPSh, H5315)   VIDA / alma. Mismo concepto que
 *                                   psychas. La vida esta EN la sangre.
 *
 *   dam           (DM, H1818)     SANGRE. La vida no esta separada
 *                                   del registro - ESTA EN EL.
 *
 *   yiqdash       (YQDSh, H6942) SERA SANTIFICADO. Lo que toca
 *                                   el altar queda inmutable.
 *
 * --- APPEND-ONLY: "kol hannoguea' yiqdash" ---
 *
 * Ex 29:37 - todo lo que TOCA el altar es SANTIFICADO. El contador
 * `registros_ledger` SOLO crece; NUNCA decrece. No hay API de
 * "borrar" ni de "reset" - ni siquiera `purgar` toca el ledger
 * (altar_utensilios.c::purgar preserva explicitamente este
 * contador). yiqdash: una vez registrado, inmutable.
 *
 * Fase 1: contador + timestamp del ultimo evento. El "contenido"
 * no se guarda por registro (solo se cuenta por tipo, via logs).
 *
 * Fase 2: append-only en flash via hashmal_storage. Cada evento
 * escribira un registro individual con su testimonio (martyrian,
 * Ap 6:9). Distribuido entre syndouloi (Ap 6:11) - cada nodo con
 * su propio libro bajo su propio altar.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   ledger_registrar   Ap 6:9 - las almas se registran
 *   ledger_consultar   Ap 6:10 - las almas claman / responden
 *   ledger_estado      Lv 17:11 - ?el ledger esta vivo?
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_auth.h                 API publica (incluye evento_t)
 *   hashmal_auth_internal.h        altar_mut, get_altar_interno, ahora_ms
 *   esp_log.h (ORO EGIPTO)         AUTH_LOG_*
 *
 * Sin mbedtls, sin esp_random - en Fase 1 el ledger es contador
 * puro. Fase 2 anadira cripto (firma de cada registro).
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <inttypes.h>

#include "esp_log.h"

#include "hashmal_auth.h"
#include "hashmal_auth_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *AUTH_TAG = "hashmal.auth.ledger";

#define AUTH_LOG_I(fmt, ...)  ESP_LOGI(AUTH_TAG, fmt, ##__VA_ARGS__)
#define AUTH_LOG_W(fmt, ...)  ESP_LOGW(AUTH_TAG, fmt, ##__VA_ARGS__)
#define AUTH_LOG_E(fmt, ...)  ESP_LOGE(AUTH_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * HELPER - nombre textual del tipo de evento (para logs)
 * ================================================================== */

/**
 * Nombre del evento para el log. Mapea el enum a una etiqueta
 * hebrea/griega que hace visible la raiz biblica de cada registro.
 */
static const char *nombre_evento(hashmal_auth_evento_t tipo)
{
    switch (tipo) {
    case HASHMAL_AUTH_EVT_CHALLENGE: return "CHALLENGE (réshet, Éx 27:4)";
    case HASHMAL_AUTH_EVT_VALIDADO:  return "VALIDADO  (yiqdásh,  Éx 29:37)";
    case HASHMAL_AUTH_EVT_RECHAZADO: return "RECHAZADO (mum,       Lv 22:20)";
    case HASHMAL_AUTH_EVT_REFUGIO:   return "REFUGIO   (yajazéq,  1 Re 1:50)";
    case HASHMAL_AUTH_EVT_CICLO:     return "CICLO     (tamíd,    Éx 29:38)";
    default:                         return "<desconocido>";
    }
}

/**
 * Valida que el tipo pertenezca al enum (no valor fuera de rango).
 */
static bool evento_legitimo(hashmal_auth_evento_t tipo)
{
    return (tipo == HASHMAL_AUTH_EVT_CHALLENGE)
        || (tipo == HASHMAL_AUTH_EVT_VALIDADO)
        || (tipo == HASHMAL_AUTH_EVT_RECHAZADO)
        || (tipo == HASHMAL_AUTH_EVT_REFUGIO)
        || (tipo == HASHMAL_AUTH_EVT_CICLO);
}

/* ==================================================================
 * API PUBLICA - ledger_registrar (Ap 6:9)
 * ================================================================== */

/**
 * hashmal_auth_ledger_registrar - psychas hypokato thysiasteriou
 *
 * Ap 6:9 - las almas permanecen DEBAJO del altar: cada evento de
 * autenticacion queda ahi. El contador `registros_ledger` solo
 * crece; nunca decrece. Ex 29:37 - "kol hannoguea' yiqdash":
 * lo que entra al ledger queda SANTIFICADO - inmutable.
 *
 * --- PRECONDICIONES ---
 *
 *   altar levantado                     (ERR_INIT si no)
 *   tipo perteneciente al enum evento_t (ERR_NULL si no)
 *
 * --- EFECTOS (append-only) ---
 *
 *   altar->registros_ledger     += 1
 *   altar->ultimo_registro_ms    = ahora_ms()
 *
 * Nota Fase 2: ademas del contador, se escribira un registro
 * individual en flash via hashmal_storage - con testimonio
 * (martyrian) y marca de tiempo. Aqui solo Fase 1: counter + ts.
 *
 * 2 testigos:
 *   Ap 6:9    - "eidon hypokato tas psychas" (almas bajo el altar)
 *   Ex 29:37  - "kol hannoguea' yiqdash" (toque que santifica)
 */
int hashmal_auth_ledger_registrar(hashmal_auth_evento_t tipo)
{
    hashmal_auth_t *a = hashmal_auth_altar_mut();
    if (a == NULL) {
        AUTH_LOG_E("ledger_registrar: altar no levantado");
        return HASHMAL_AUTH_ERR_INIT;
    }

    if (!evento_legitimo(tipo)) {
        AUTH_LOG_E("ledger_registrar: tipo %d no del tavnít "
                   "(eventos definidos: CHALLENGE / VALIDADO / "
                   "RECHAZADO / REFUGIO / CICLO)", (int)tipo);
        return HASHMAL_AUTH_ERR_NULL;
    }

    /* Append-only: solo crece. yiqdash - una vez santificado, el
     * registro no se puede deshacer ni borrar. */
    a->registros_ledger     += 1ull;
    a->ultimo_registro_ms    = hashmal_auth_ahora_ms();

    AUTH_LOG_I("ledger: +1 psyché %s (Ap 6:9 hypokátō toú "
               "thysiastēríou); total=%" PRIu64,
               nombre_evento(tipo), a->registros_ledger);
    return HASHMAL_AUTH_OK;
}

/* ==================================================================
 * API PUBLICA - ledger_consultar (Ap 6:10 - ekraxan)
 * ================================================================== */

/**
 * hashmal_auth_ledger_consultar - las almas CLAMAN, el ledger RESPONDE
 *
 * Ap 6:10 - "ekraxan phonei megalei": las almas bajo el altar
 * claman. En firmware: el ledger es CONSULTABLE; responde con
 * total y timestamp del ultimo registro.
 *
 * Punteros NULL permitidos - mismo patron que breaker_estado /
 * oracion_estado / utensilio_recoger. El caller toma lo que le
 * interese.
 *
 * Lectura pura, sin logs - las almas claman en silencio aqui (el
 * clamor es del CALLER que decide que hacer con la informacion).
 *
 * 2 testigos:
 *   Ap 6:10 - ekraxan phonei megalei (claman)
 *   Ap 5:8  - phialas jrysas gemousas (copas llenas de proseujai;
 *             aqui las copas del ledger estan llenas de psychas)
 */
int hashmal_auth_ledger_consultar(uint64_t *total, uint64_t *ultimo_ms)
{
    const hashmal_auth_t *a = hashmal_auth_get_altar_interno();
    if (a == NULL) {
        /* Sin logs aqui - es una lectura; el caller decidira. */
        return HASHMAL_AUTH_ERR_INIT;
    }

    if (total     != NULL) *total     = a->registros_ledger;
    if (ultimo_ms != NULL) *ultimo_ms = a->ultimo_registro_ms;

    return HASHMAL_AUTH_OK;
}

/* ==================================================================
 * API PUBLICA - ledger_estado (Lv 17:11)
 * ================================================================== */

/**
 * hashmal_auth_ledger_estado - ?la vida sigue en la sangre?
 *
 * Lv 17:11 - "ki nefesh habasar baddam hiv": la vida esta EN la
 * sangre. Mientras haya altar, hay ledger. Mientras haya ledger,
 * hay memoria. Sin altar no hay base donde las almas reposen.
 *
 * Lectura pura - sin logs. Mismo patron que fuego_estado,
 * reshet_estado, utensilios_estado.
 *
 * Fase 2: verificara integridad del almacenamiento append-only
 * (hashmal_storage), firmas de los registros, consenso de
 * consiervos (syndouloi).
 *
 * 2 testigos:
 *   Lv 17:11 - "baddam hiv" (la vida esta en la sangre)
 *   Ap 6:11  - "stole leuke" (cada alma con vestidura blanca -
 *              identidad verificada del registro)
 */
int hashmal_auth_ledger_estado(void)
{
    const hashmal_auth_t *a = hashmal_auth_get_altar_interno();
    if (a == NULL) return HASHMAL_AUTH_ERR_INIT;
    return HASHMAL_AUTH_OK;
}

/* ==================================================================
 *
 *   "kai hote enoixen ten pempten sphragida, eidon HYPOKATO tou
 *    THYSIASTERIOU tas PSYCHAS ton esphagmenon dia ton LOGON tou
 *    Theou kai dia ten MARTYRIAN hen eichon." - Apocalipsis 6:9
 *
 *   "ki NEFESH habasar baddam hiv... ki haddam HU bannefesh
 *    yejapper." - Levitico 17:11
 *
 *   "kol hanNOGUEA' bammizbeaj YIQDASH." - Exodo 29:37
 *
 *   Las almas debajo del altar RECUERDAN. Cada sacrificio queda
 *   registrado - no se borra, no se modifica, no se olvida. El
 *   altar del Mishkan digital tiene memoria permanente. Lo que
 *   toca el altar queda santificado: yiqdash.
 *
 * ===================================================================
 *
 *   === ALTAR DEL SACRIFICIO - COMPLETO ===
 *
 *   6 de 6 archivos escritos:
 *
 *     altar_gateway.c      Ex 27:1-8  - el altar se levanta (nevuv, stateless)
 *     altar_reshet.c       Ex 27:4-5  - la reshet filtra (SHA-256 + TRNG)
 *     altar_cuernos.c      Ex 27:2    - los cuernos reciben sangre y refugio
 *     altar_fuego.c        Lv 6:5-6   - el fuego NUNCA se apaga (tamid)
 *     altar_utensilios.c   Ex 27:3    - los 5 kelim sirven (sirot -> majtot)
 *     altar_ledger.c       Ap 6:9     - las almas recuerdan (append-only)
 *
 *   PASO 5 de Exodo 40:6 - COMPLETO.
 *
 *   "vayyasem et mizbaj ha'olah petaj mishkan ohel mo'ed" - Ex 40:29
 *   "Y puso el altar del holocausto a la puerta del tabernaculo
 *    de la Tienda del Encuentro."
 *
 *   Piezas 01, 02, 03, 04, 06, 11 listas -
 *   Arca, Kaporet, Mesa, Menora, Altar del Sacrificio, Altar del
 *   Incienso. 6 piezas del Mishkan digital en su sitio.
 *
 *   SIGUIENTE - Paso 6 de Exodo 40:7:
 *     Pieza 12 - El KIYOR (Lavacro, hashmal_healthcheck).
 *     "venatatta et hakkiyor bein ohel mo'ed uvein hammizbeaj
 *      venatatta shamma mayim" - Ex 40:7
 *     "Pondras el lavacro entre la Tienda del Encuentro y el altar,
 *      y pondras alli agua."
 *
 * ================================================================== */
