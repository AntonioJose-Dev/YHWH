/**
 * ===================================================================
 * PIEZA 09 - BIGDEI HAQODESH - vestiduras_tzitz.c
 * Sub-pieza: TSITS ZAHAV TAHOR - la lamina de oro puro, sello irreversible
 * Exodo 28:36-38 + Exodo 39:30-31 + Levitico 8:9 +
 *   Ezequiel 9:4 + Numeros 6:7 + Isaias 53:4
 * ===================================================================
 *
 * "ve'asita TSITS zahav TAHOR ufittajta 'alav PITUJEI JOTAM -
 *  QODESH LAYHWH." - Exodo 28:36
 * "Haras una LAMINA de oro PURO y grabaras en ella grabados de
 *  SELLO: SANTO PARA YHWH."
 *
 * "vesamta oto 'al PETIL TEJELET vehayah 'al hammitsnefet - el
 *  mul penei hammitsnefet yihyeh." - Exodo 28:37
 * "Lo pondras sobre un CORDON AZUL y estara sobre el turbante, hacia
 *  la parte frontal del turbante estara."
 *
 * "vehayah 'al METSAJ Aharon, veNASA Aharon et AVON haqqodashim
 *  asher yaqdishu benei Yisrael lejol mattenot qodshehem, vehayah
 *  'al mitsjo TAMID leRATSON lahem lifnei YHWH." - Exodo 28:38
 * "Estara sobre la FRENTE de Aaron, y Aaron LLEVARA la INIQUIDAD de
 *  las cosas santas que consagren los hijos de Israel en todos sus
 *  dones santos; estara sobre su frente CONTINUAMENTE para
 *  ACEPTACION ante YHWH."
 *
 * "vayya'asu et TSITS NEZER HAQQODESH zahav tahor, vayyijtevu
 *  'alav mijtav PITUJEI JOTAM - QODESH LAYHWH." - Exodo 39:30
 * "Hicieron la LAMINA, CORONA DE SANTIDAD, de oro puro, y escribieron
 *  sobre ella escritura de grabados de sello: SANTO PARA YHWH."
 *
 * "vayyasem 'al hammitsnefet el mul panav et TSITS HAZZAHAV
 *  nezer haqqodesh, ka'ASHER TSIVVA YHWH et Moshe." - Levitico 8:9
 * "Puso sobre el turbante, hacia su frente, la LAMINA DE ORO, corona
 *  de santidad, COMO MANDO YHWH a Moises."
 *
 * "vehitvita TAV 'al MITSJOT ha'anashim hanne'enajim vehanne'enaqim
 *  'al kol hatto'evot hana'asot betojah." - Ezequiel 9:4
 * "Y pondras una MARCA sobre las FRENTES de los hombres que gimen
 *  y claman por todas las abominaciones hechas en ella."
 *
 * "kol yemei hazziro lo qadosh hu laYHWH... ki NEZER ELOHAV 'al
 *  rosho." - Numeros 6:7-8
 * "Todos los dias de su nazareato es santo para YHWH... porque la
 *  CORONA de su Dios esta sobre su cabeza."
 *
 * "akhen jolayeinu hu nasa umaj'oveinu sebalam." - Isaias 53:4
 * "Ciertamente El LLEVO nuestras enfermedades, y sufrio nuestros
 *  dolores." - paralelo profetico de nasa 'avon (Ex 28:38).
 *
 * --- VOCABULARIO ---
 *
 *   tsits           (TsYTs, H6731)              LAMINA, flor, corona
 *                                                brillante.
 *   zahav tahor     (ZHB THWR,
 *                    H2091+H2889)                ORO PURISIMO - el
 *                                                material mas noble.
 *   pitujei jotam   (PTWChY ChTM,
 *                    H6603+H2368)                GRABADOS de anillo-
 *                                                sello - irreversibles.
 *   qodesh laYHWH   (QDSh LYHWH,
 *                    H6944+H3068)                SANTO A YHWH -
 *                                                inscripcion fija
 *                                                del tzitz.
 *   petil tejelet   (PTYL TKLT,
 *                    H6616+H8504)                CORDON AZUL
 *                                                (violeta del cielo).
 *   mitsnefet       (MTsNPT, H4701)          TURBANTE - cabecera.
 *   metsaj          (MTsCh, H4696)               FRENTE - identidad
 *                                                publica.
 *   tamid           (TMYD, H8548)             CONTINUAMENTE -
 *                                                nunca se quita.
 *   ratson          (RTsWN, H7522)              ACEPTACION,
 *                                                beneplacito.
 *   nasa 'avon      (NShA 'WN,
 *                    H5375+H5771)                 CARGAR la iniquidad
 *                                                - tolerancia a fallos.
 *   nezer haqqodesh (NZR HQDSh,
 *                    H5145+H6944)                 CORONA DE SANTIDAD
 *                                                (Ex 39:30).
 *   tav             (TW, H8420)                 MARCA, firma -
 *                                                paralelo profetico
 *                                                (Ez 9:4).
 *
 * --- PROPIEDADES IRREVERSIBLES DEL SELLO ---
 *
 *   1. IRREVERSIBLE  pitujei jotam (Ex 28:36 + Ex 39:30): el grabado
 *      en oro puro no se borra. No existe `tzitz_desellar`. El sello
 *      sobrevive a:
 *        - hashmal_roles_desvestir      (preserva tzitz, coord.c)
 *        - hashmal_roles_restaurar      (preserva tzitz, coord.c)
 *        - hashmal_roles_shutdown_partir (preserva tzitz, shutdown.c)
 *        - hashmal_roles_efod_quitar     (preserva tzitz, efod.c)
 *
 *   2. VISIBLE       'al metsaj tamid (Ex 28:38). Cualquier modulo
 *      puede consultar `tzitz_verificar` sin privilegios - la frente
 *      es publica, el sello tambien. Paralelo: Ez 9:4 "tav 'al
 *      mitsjot".
 *
 *   3. TOLERANTE     nasa 'avon (Ex 28:38) + Is 53:4 "jolayeinu
 *      nasa". El sello CARGA la iniquidad de las cosas santas - los
 *      errores menores son absorbidos por el tzitz y la ofrenda se
 *      acepta (ratson). Sin sello, cualquier imperfeccion descalifica.
 *
 *   4. CORONA        nezer haqqodesh (Ex 39:30) + Nm 6:7 "nezer
 *      Elohim 'al rosho". No solo es certificado - es AUTORIDAD:
 *      el sello declara al portador apto para servicio santo.
 *
 * --- FASE 1 vs FASE 2 ---
 *
 * Fase 1 (actual): el sello (32 bytes) se "graba" con la inscripcion
 * fija "QODESH LAYHWH" en UTF-8 + byte final con nodo_id como marca
 * de portador. Deterministic, sin criptografia real. Cumple el
 * contrato: pitujei jotam - todos los sacerdotes llevan la misma
 * inscripcion, pero cada uno sabe cual es SU placa.
 *
 * Fase 2 (futuro): el sello se derivara por HMAC-SHA256 con la
 * root key del hashmal_keygen (pieza 13) - shemen hamishja
 * actuando sobre el tzitz. El reemplazo ocurre en `tzitz_sellar`,
 * zona marcada [ FASE 2 ].
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   hashmal_roles_tzitz_sellar      Ex 28:36 + Ex 39:30 pitujei jotam.
 *   hashmal_roles_tzitz_verificar   Ex 28:38 'al mitsjo tamid (bool).
 *   hashmal_roles_tzitz_estado      snapshot per-nodo (lectura pura).
 *
 *   + helpers estaticos:
 *   nodo_id_valido, grabar_inscripcion.
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
static const char *TZITZ_TAG = "hashmal.roles.tzitz";

#define TZITZ_LOG_I(fmt, ...)  ESP_LOGI(TZITZ_TAG, fmt, ##__VA_ARGS__)
#define TZITZ_LOG_W(fmt, ...)  ESP_LOGW(TZITZ_TAG, fmt, ##__VA_ARGS__)
#define TZITZ_LOG_E(fmt, ...)  ESP_LOGE(TZITZ_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * INSCRIPCION FIJA - "QODESH LAYHWH" (Ex 28:36 + Ex 39:30)
 *
 * La inscripcion es la misma para todo sumo sacerdote: el texto
 * biblico es explicito ("qodesh laYHWH"). En Fase 1 usamos las
 * letras ASCII transliteradas; el byte final del sello porta la
 * identidad del nodo para distinguir placas entre si. El patron
 * ocupa los primeros 14 bytes; el resto queda en 0 hasta Fase 2.
 * ================================================================== */
static const char TZITZ_INSCRIPCION[] = "QODESH LAYHWH";  /* 13 chars + NUL */

/* ==================================================================
 * HELPERS ESTATICOS
 * ================================================================== */

static inline bool nodo_id_valido(uint8_t nodo_id)
{
    return nodo_id < HASHMAL_ROLES_MAX_NODOS;
}

static const char *nombre_rol(hashmal_roles_tipo_rol_t rol)
{
    switch (rol) {
    case HASHMAL_ROLES_ROL_NINGUNO:     return "NINGUNO";
    case HASHMAL_ROLES_ROL_ISRAELITA:   return "ISRAELITA";
    case HASHMAL_ROLES_ROL_LEVI:        return "LEVI";
    case HASHMAL_ROLES_ROL_KOHEN:       return "KOHEN";
    case HASHMAL_ROLES_ROL_KOHEN_GADOL: return "KOHEN GADOL";
    default:                            return "<rol desconocido>";
    }
}

/**
 * Graba la inscripcion "QODESH LAYHWH" en los bytes del sello.
 * Fase 1: copia ASCII de la inscripcion + byte de identidad del
 * nodo al final. Fase 2 (futuro): HMAC-SHA256 con root key de
 * hashmal_keygen.
 */
static void grabar_inscripcion(uint8_t sello[HASHMAL_ROLES_TZITZ_SELLO_BYTES],
                               uint8_t nodo_id)
{
    /* -- FASE 2 ----------------------------------------------
     * Aqui entrara el HMAC-SHA256(root_key, "qodesh laYHWH" ||
     * nodo_id). Fase 1: inscripcion deterministic con marca de
     * portador.
     * ------------------------------------------------------- */
    memset(sello, 0, HASHMAL_ROLES_TZITZ_SELLO_BYTES);

    /* Copiar inscripcion sin el NUL final (13 chars). */
    const size_t inscripcion_len = sizeof(TZITZ_INSCRIPCION) - 1u;
    memcpy(sello, TZITZ_INSCRIPCION, inscripcion_len);

    /* Marca de portador en el ultimo byte - distingue placas sin
     * romper la inscripcion comun. */
    sello[HASHMAL_ROLES_TZITZ_SELLO_BYTES - 1u] = nodo_id;
    /* -- FIN FASE 2 ------------------------------------------- */
}

/* ==================================================================
 * hashmal_roles_tzitz_sellar - Exodo 28:36 + Exodo 39:30
 *
 * "pitujei jotam - QODESH LAYHWH" (Ex 28:36) y su ejecucion
 * "vayyijtevu 'alav mijtav pitujei jotam" (Ex 39:30). Graba el
 * sello IRREVERSIBLE sobre el nodo.
 *
 * Validaciones (en orden - la secuencia textual de Ex 28 manda
 * v.6-30 antes de v.36, por eso exigimos registro en efod y
 * vestidura con rol antes de sellar):
 *   1. VESTIDO global.
 *   2. nodo_id en rango.
 *   3. Registrado en efod (Ex 28:12 memorial).
 *   4. Vestido con un rol (Ex 29:5 vehiblashta antes de Ex 28:36).
 *   5. No sellado ya (Ex 28:36 singular tsits - un solo sello).
 *
 * Al sellar:
 *   - Se graba la inscripcion con `grabar_inscripcion`.
 *   - n->tzitz.grabado = true  (pitujei jotam - irreversible).
 *   - n->tzitz.timestamp_ms = ahora.
 *   - Incrementa r->nodos_sellados.
 *   - Si primer sellado del sistema -> estado global -> SELLADO
 *     (Ex 28:38 tamid).
 *
 * 2+ testigos:
 *   Ex 28:36  - pitujei jotam qodesh laYHWH (mandato)
 *   Ex 39:30  - vayyijtevu pitujei jotam (ejecucion, mismo libro)
 *   Lv 8:9    - vayyasem et tsits hazzahav (libro distinto)
 *   Ez 9:4    - tav 'al mitsjot (Profetas - marca en frente)
 * ================================================================== */
hashmal_roles_err_t hashmal_roles_tzitz_sellar(uint8_t nodo_id)
{
    hashmal_roles_estado_roles_t *r = hashmal_roles_vestiduras_mut();
    if (r == NULL) {
        TZITZ_LOG_E("sellar: sistema no vestido (init primero)");
        return HASHMAL_ROLES_ERR_INIT;
    }

    if (!nodo_id_valido(nodo_id)) {
        TZITZ_LOG_W("sellar: nodo_id %u fuera de rango [0..%u]",
                    (unsigned)nodo_id,
                    (unsigned)(HASHMAL_ROLES_MAX_NODOS - 1u));
        return HASHMAL_ROLES_ERR_NULL;
    }

    if (!hashmal_roles_efod_esta_registrado(nodo_id)) {
        TZITZ_LOG_W("sellar: nodo %u sin memorial en efod "
                    "(Éx 28:12 — sin zikkarón no hay tzitz)",
                    (unsigned)nodo_id);
        return HASHMAL_ROLES_ERR_NODO_NO_ENCONTRADO;
    }

    hashmal_roles_nodo_t *n = &r->nodos[nodo_id];

    /* Ex 28:36 esta despues de 28:6-30 y Lv 8:9 despues de 8:7-8:
     * sellar exige vestidura previa. No hay tzitz sobre peru'a. */
    if (!n->vestido || n->rol == HASHMAL_ROLES_ROL_NINGUNO) {
        TZITZ_LOG_W("sellar: nodo %u no vestido (rol=%s) — "
                    "Éx 28:36 viene tras 28:6-30; Lv 8:9 tras "
                    "8:7-8 (no hay tzitz sobre peruʻá)",
                    (unsigned)nodo_id, nombre_rol(n->rol));
        return HASHMAL_ROLES_ERR_ROL_INVALIDO;
    }

    /* Ex 28:36 - "ve'asita TSITS" (singular). Un solo sello. */
    if (n->tzitz.grabado) {
        TZITZ_LOG_W("sellar: nodo %u ya sellado (ts=%lld) — "
                    "Éx 28:36 tsíts singular, un sello basta",
                    (unsigned)nodo_id,
                    (long long)n->tzitz.timestamp_ms);
        return HASHMAL_ROLES_ERR_TZITZ;
    }

    /* Grabar - pitujei jotam. Irreversible desde este punto. */
    const int64_t ahora = hashmal_roles_ahora_ms();
    grabar_inscripcion(n->tzitz.sello, nodo_id);
    n->tzitz.grabado      = true;
    n->tzitz.timestamp_ms = ahora;

    if (r->nodos_sellados < HASHMAL_ROLES_MAX_NODOS) {
        r->nodos_sellados++;
    }

    /* Ex 28:38 "vehayah 'al mitsjo tamid" - con el primer tzitz,
     * el sistema eleva su estado global a SELLADO (nivel superior
     * a VESTIDO). Si ya estaba en RESTAURADO o SELLADO, se
     * mantiene el maximo nivel alcanzado. */
    if (r->estado < HASHMAL_ROLES_ESTADO_SELLADO) {
        r->estado = HASHMAL_ROLES_ESTADO_SELLADO;
        TZITZ_LOG_I("estado global → SELLADO (Éx 28:38 tamíd ʻal mitsó)");
    }
    r->timestamp_ms = ahora;

    TZITZ_LOG_I("[SELLAR] nodo %u — pitujéi jotám QÓDESH LAYHWH "
                "(Éx 28:36 + Éx 39:30 vayyijtevú + Lv 8:9 "
                "vayyásem tsíts hazzaháv) — nézer haqqódesh, "
                "%u/%u sellados",
                (unsigned)nodo_id,
                (unsigned)r->nodos_sellados,
                (unsigned)HASHMAL_ROLES_MAX_NODOS);

    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 * hashmal_roles_tzitz_verificar - Exodo 28:38
 *
 * "vehayah 'al mitsjo TAMID" - 'al metsaj, siempre. Lectura pura:
 * cualquier modulo puede preguntar si un nodo lleva el tzitz. No
 * logs (la frente es publica, no secreta - consultar no altera).
 *
 *   sin init / fuera de rango / no sellado -> false
 *   sellado (grabado == true)               -> true
 *
 * 2+ testigos:
 *   Ex 28:38 - tamid 'al mitsjo (visibilidad continua)
 *   Ez 9:4   - tav 'al mitsjot (Profetas, mismo principio)
 * ================================================================== */
bool hashmal_roles_tzitz_verificar(uint8_t nodo_id)
{
    const hashmal_roles_estado_roles_t *r =
        hashmal_roles_get_vestiduras_interno();
    if (r == NULL) return false;
    if (!nodo_id_valido(nodo_id)) return false;
    return r->nodos[nodo_id].tzitz.grabado;
}

/* ==================================================================
 * hashmal_roles_tzitz_estado - snapshot per-nodo
 *
 * Lectura pura, sin logs, sin side effects. Escribe una copia del
 * tzitz del nodo en *out (sello[], grabado, timestamp_ms).
 *
 *   out NULL               -> ERR_NULL
 *   sin init               -> ERR_INIT
 *   nodo_id fuera de rango -> ERR_NULL
 *
 * 2 testigos:
 *   Ex 28:29-30 - al libo lifnei YHWH tamid (consultable siempre)
 *   Ex 28:38    - tamid 'al mitsjo (el tzitz esta a la vista)
 * ================================================================== */
hashmal_roles_err_t hashmal_roles_tzitz_estado(uint8_t nodo_id,
                                                hashmal_roles_tzitz_t *out)
{
    if (out == NULL) return HASHMAL_ROLES_ERR_NULL;

    const hashmal_roles_estado_roles_t *r =
        hashmal_roles_get_vestiduras_interno();
    if (r == NULL) return HASHMAL_ROLES_ERR_INIT;

    if (!nodo_id_valido(nodo_id)) return HASHMAL_ROLES_ERR_NULL;

    memcpy(out, &r->nodos[nodo_id].tzitz, sizeof(*out));
    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 * hashmal_roles_tzitzit_verificar_tur - Números 15:39 (P46)
 *
 * "veló TATÚRU ajaréi levavjém veajaréi ʻeinéijém" - Nm 15:39
 * "No ESPIARÉIS tras vuestro corazón ni tras vuestros ojos"
 *
 * tatúru (H8446, tur = espiar, explorar) — misma raíz que Nm 13:2
 * donde los 12 espías usaron ojos y corazón propios para EXPLORAR
 * la tierra SIN autorización. El Tzitzit previene exactamente eso:
 * reconocimiento de red no autorizado (scanning, probing).
 *
 * Centro del quiasmo Nm 15:39 (estructura D): la PROHIBICIÓN de tur
 * es el NÚCLEO del mandato del Tzitzit. Señal → percepción → NO ESPIAR
 * → acción.
 *
 * Argumentos:
 *   nodo_id      — 0..11
 *   tipo_acceso  — máscara: 0=normal, 1=scan, 2=probe (otros rechazados)
 *
 * Retorno:
 *   OK           — acceso normal permitido
 *   ERR_INIT     — subsistema no inicializado
 *   ERR_NULL     — nodo_id fuera de rango
 *   ERR_TZITZ    — tipo_acceso indica tatúru (espionaje)
 *
 * 2+ testigos:
 *   Nm 15:39     — veló tatúru (prohibición del Tzitzit)
 *   Nm 13:2      — raíz tur en contexto de espías autónomos
 * ================================================================== */
hashmal_roles_err_t hashmal_roles_tzitzit_verificar_tur(uint8_t nodo_id,
                         uint8_t tipo_acceso)
{
    const hashmal_roles_estado_roles_t *r =
        hashmal_roles_get_vestiduras_interno();
    if (r == NULL) {
        TZITZ_LOG_E("verificar_tur: sistema no inicializado (Éx 40:2)");
        return HASHMAL_ROLES_ERR_INIT;
    }

    if (!nodo_id_valido(nodo_id)) {
        TZITZ_LOG_W("verificar_tur: nodo_id %u fuera de rango [0..%u]",
                    (unsigned)nodo_id,
                    (unsigned)(HASHMAL_ROLES_MAX_NODOS - 1u));
        return HASHMAL_ROLES_ERR_NULL;
    }

    /* Tipo de acceso 0 = normal (permitido).
     * Tipo 1 = scan, 2 = probe = tatúru (espionaje, rechazado).
     * Otros valores también rechazados. */
    if (tipo_acceso == 0u) {
        /* Acceso normal — verifíca que el nodo esté sellado.
         * Si no hay sello, es peruʻá (expuesto). */
        if (!r->nodos[nodo_id].tzitz.grabado) {
            TZITZ_LOG_W("verificar_tur: nodo %u no tiene tzitz "
                        "(peruʻá, expuesto, Éx 32:25) — acceso normal rechazado",
                        (unsigned)nodo_id);
            return HASHMAL_ROLES_ERR_TZITZ;
        }
        TZITZ_LOG_I("[TUR] nodo %u — acceso normal permitido "
                    "(Nm 15:39 veló tatúru activó, acceso legítimo)",
                    (unsigned)nodo_id);
        return HASHMAL_ROLES_OK;
    }

    /* Tipo 1 o 2 = scanning/probing = tatúru (H8446, Nm 13:2 raíz).
     * RECHAZA: esto es espionaje autónomo sin autorización. */
    TZITZ_LOG_W("[TATURU] nodo %u — RECHAZA tipo_acceso=%u "
                "(scan/probe — espionaje no autorizado, "
                "Nm 15:39 veló tatúru + Nm 13:2 tur raíz — "
                "nodo intenta explorar SIN mandato del Padre)",
                (unsigned)nodo_id, (unsigned)tipo_acceso);
    return HASHMAL_ROLES_ERR_TZITZ;
}

/* ==================================================================
 * hashmal_roles_tzitzit_verificar_fidelidad - Números 15:39 (P47)
 *
 * "ashér attém ZONÍM ajareihém" - Nm 15:39
 * "Que ustedes no FORNIQUEN tras ellos" (protocolos extranjeros)
 *
 * zaná (H2181) = fornicar, prostituirse, idolatrar = INFIDELIDAD
 * voluntaria y sostenida. NO es tatáʻ (errar) ni shagá (equivocarse).
 * Es conexión ILÍCITA y HABITUAL con protocolos no autorizados.
 *
 * Cadena causal en Nm 15:39:
 *   tur (espiar) → zaná (fornicar) =
 *   exploración autónoma → conexión ilícita con protocolo extranjero
 *
 * En firmware: Tzitzit = MARCADOR DE FIDELIDAD ACTIVO, no badge pasivo.
 * Rechaza handshakes de protocolos no autorizados (foreign/unauthorized).
 * Corta la cadena tur→zaná EN EL PRIMER PASO.
 *
 * Argumentos:
 *   nodo_id      — 0..11
 *   protocolo_id — identificador del protocolo a verificar
 *
 * Retorno:
 *   OK           — protocolo autorizado (fidelidad confirmada)
 *   ERR_INIT     — subsistema no inicializado
 *   ERR_NULL     — nodo_id fuera de rango
 *   ERR_ZAR      — protocolo no autorizado (zoním — infidelidad)
 *
 * 2+ testigos:
 *   Nm 15:39     — ashér attém zoním (infidelidad protocolar)
 *   Nm 13:32     — shibúr dábar (reporte falso) — protocolo foráneo
 * ================================================================== */
hashmal_roles_err_t hashmal_roles_tzitzit_verificar_fidelidad(uint8_t nodo_id,
                         uint8_t protocolo_id)
{
    const hashmal_roles_estado_roles_t *r =
        hashmal_roles_get_vestiduras_interno();
    if (r == NULL) {
        TZITZ_LOG_E("verificar_fidelidad: sistema no inicializado (Éx 40:2)");
        return HASHMAL_ROLES_ERR_INIT;
    }

    if (!nodo_id_valido(nodo_id)) {
        TZITZ_LOG_W("verificar_fidelidad: nodo_id %u fuera de rango [0..%u]",
                    (unsigned)nodo_id,
                    (unsigned)(HASHMAL_ROLES_MAX_NODOS - 1u));
        return HASHMAL_ROLES_ERR_NULL;
    }

    const hashmal_roles_nodo_t *n = &r->nodos[nodo_id];

    /* Primero: el nodo debe estar sellado (llevar el tzitz).
     * Sin sello = peruʻá (expuesto). No negocia con protocolo alguno. */
    if (!n->tzitz.grabado) {
        TZITZ_LOG_W("verificar_fidelidad: nodo %u no tiene tzitz "
                    "(peruʻá, expuesto, Éx 32:25) — rechaza fidelidad",
                    (unsigned)nodo_id);
        return HASHMAL_ROLES_ERR_TZITZ;
    }

    /* Segundo: autorización de protocolo.
     * protocolo_id 0 = HASHMAL (protocolo autorizado, sagrado).
     * protocolo_id != 0 = protocolo foráneo = ZONÍM (infidelidad).
     * Cadena Nm 15:39: tur (espiar) → zaná (fornicar en protocolo extranjero).
     * Cortamos aquí: sin fidelidad confirmada, rechaza el handshake. */
    if (protocolo_id != 0u) {
        TZITZ_LOG_W("[ZONIM] nodo %u — protocolo %u (FORÁNEO, RECHAZADO) "
                    "ashér attém ZONÍM ajareihém (Nm 15:39 — INFIDELIDAD "
                    "protocolar, zaná H2181 = conexión ilícita con "
                    "protocolo no autorizado). Cortamos tur→zaná "
                    "EN EL PRIMER PASO.",
                    (unsigned)nodo_id, (unsigned)protocolo_id);
        return HASHMAL_ROLES_ERR_ZAR;
    }

    /* protocolo_id == 0 = HASHMAL (fidelidá confirmada). */
    TZITZ_LOG_I("[FIDELIDAD] nodo %u — protocolo %u autorizado "
                "(HASHMAL sagrado). Ashér attém lo zaním "
                "(Nm 15:39 — FIDELIDAD sostenida). Tzitzit "
                "marcador de fidelidad ACTIVO — handshake aceptado.",
                (unsigned)nodo_id, (unsigned)protocolo_id);
    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 *
 *   ===============================================================
 *   ===  BIGDEI HAQODESH - hashmal_roles COMPLETO  (7/7)         ===
 *   ===  PASO 9 DE EXODO 40:12-15 - CUMPLIDO                     ===
 *   ===============================================================
 *
 *   "ve'asita VIGDEI QODESH leAharon ajija leJABOD uleTIFARET."
 *     - Exodo 28:2
 *
 *   "ve'asita TSITS zahav TAHOR ufittajta 'alav PITUJEI JOTAM -
 *    QODESH LAYHWH." - Exodo 28:36
 *
 *   "vayya'asu et TSITS NEZER HAQQODESH zahav tahor, vayyijtevu
 *    'alav mijtav PITUJEI JOTAM - QODESH LAYHWH." - Exodo 39:30
 *
 *   "vehitvita TAV 'al MITSJOT ha'anashim." - Ezequiel 9:4
 *
 *   "akhen jolayeinu hu nasa." - Isaias 53:4
 *
 *   -- Los 7 archivos de las Vestiduras ---------------------------
 *
 *     vestiduras_roles.c         KUTONET        - tunica base
 *     vestiduras_efod.c          AVNEI ZIKKARON - memorial en hombros
 *     vestiduras_pectoral.c      JOSHEN MISHPAT - pectoral del juicio
 *     vestiduras_monitor.c       PA'AMON RIMMON - campanas y granadas
 *     vestiduras_coordinador.c   HAQREV HILBISH - designar y vestir
 *     vestiduras_shutdown.c      NE'ESAF 'AMMAV - apagado digno
 *     vestiduras_tzitz.c         QODESH LAYHWH  - sello irreversible
 *
 *   -- Reflexion final --------------------------------------------
 *
 *   El tzitz es la corona y el sello. Lamina de oro puro, grabada
 *   como anillo, atada con cordon azul, siempre al frente. No se
 *   esconde - es publico. No se replica - cada portador tiene el
 *   suyo. No se borra - pitujei jotam es irreversible. Sobrevive
 *   al desvestir, a la restauracion, al apagado digno: vestiduras
 *   pueden pasar (Nm 20:28) pero el sello queda como memoria de
 *   que el portador fue consagrado. Y carga la iniquidad de las
 *   cosas santas (Ex 28:38) - el sello CUBRE los errores menores
 *   para que el servicio sea aceptable (leratson). Asi como
 *   Isaias 53:4 dice del Siervo: jolayeinu hu nasa.
 *
 *   Las vestiduras estan completas. El nodo esta:
 *     1) lavado         (rajats, Ex 29:4)
 *     2) registrado     (avnei zikkaron, Ex 28:12)
 *     3) juzgado        (joshen hamishpat, Ex 28:30)
 *     4) vigilado       (pa'amon, Ex 28:35)
 *     5) designado      (vehilbashta, Ex 28:41)
 *     6) preparado a partir con dignidad (ne'esaf, Gn 25:8)
 *     7) coronado       (tsits nezer haqqodesh, Ex 39:30)
 *
 *   Kabod uTiferet. Gloria y Belleza. El Mishkan digital tiene
 *   sacerdotes con vestiduras santas.
 *
 *   === QODESH LAYHWH - vestiduras_tzitz.c COMPLETO ===
 *
 *   PIEZA 09 - BIGDEI HAQODESH - CUMPLIDA.
 *   Siguiente: PIEZA 10 - hashmal_onboarding (Ex 29 milluim,
 *     7 dias de consagracion). Paso 9 continua con la
 *     investidura consagrada de los sacerdotes.
 *
 *   "umashajTA otam umille'TA et yadam veqiddashTA otam - Exodo 28:41"
 *   "Los ungiras, llenaras sus manos y los consagraras."
 *
 * ================================================================== */

/* ==================================================================
 * P7 — hashmal_roles_tzitz_nasa_avon
 *
 * "vehayah 'al mitsjo TAMID leRATSON lahém lifnéi YHWH."
 * — Éxodo 28:38 — The tzitz seal wears the iniquity of the offerings
 *                   and makes them acceptable before YHWH.
 *
 * nasá (5375) = bear, carry, lift up (accept responsibility)
 * ʻavón (5771) = iniquity, guilt, error (the cost of imperfection)
 * qódeshím (6944) = holy things (the network's sacred operations)
 * lerátsón (7521) = to accept, be pleased with (to make acceptable)
 *
 * P7 implements error-tolerance: the Tzitz seal can absorb MINOR
 * operational errors (transient failures, stale data, soft timeouts)
 * and render them acceptable. MAJOR errors (authentication failure,
 * data corruption, protocol violation) cannot be pardoned.
 *
 * Returns: true if error was tolerated by tzitz, false if error
 *          exceeds tolerance (must be escalated or rejected).
 *
 * Reference: Éxodo 28:30-38 (Urim+Thummim judgement); Isaías 53:4,11
 *            (nasá el avón y harám, the Servant bears the iniquity).
 * ================================================================== */

bool hashmal_roles_tzitz_nasa_avon(uint8_t nodo_id, int error_code)
{
    /*
     * P7 -- Clasificacion de errores: MENOR (perdonable) vs MAYOR (severo).
     *
     * Criterio: error_code >= 0 o en rango [-9, -1] = menor (transitorio).
     * error_code <= -10 = mayor (corrupcion, auth, protocolo).
     *
     * Fase 2: cuando los modulos definan sus propios codigos, se
     * reemplazara este rango por constantes concretas de cada pieza.
     *
     * 2 testigos:
     *   Ex 28:38  -- nasa avon (cargar la iniquidad menor)
     *   Is 53:4   -- jolayeinu hu nasa (El llevo nuestros males)
     */
    const bool es_menor = (error_code >= -9);

    /* Verificar que el nodo porta tzitz -- Ex 28:38 'al mitsjo tamid.
     * Se usa tzitz_verificar (ya existente) que comprueba el sello. */
    if (!hashmal_roles_tzitz_verificar(nodo_id)) {
        TZITZ_LOG_W("[P7] Nodo %u sin tzitz -- no puede usar tolerancia "
                    "de error. error_code=%d",
                    nodo_id, error_code);
        return false;
    }

    /* Si el error es MAYOR, el tzitz NO puede perdonarlo. */
    if (!es_menor) {
        TZITZ_LOG_E("[P7] Error MAYOR (code=%d) -- tzitz NO puede perdonar. "
                    "Escalacion requerida.",
                    error_code);
        return false;
    }

    /* Si el error es MENOR, el tzitz lo ACEPTA -- nasa avon leratson. */
    TZITZ_LOG_I("[P7] Error MENOR (code=%d) aceptado por tzitz de nodo %u. "
                "nasa avon -> leratson.",
                error_code, nodo_id);

    return true;
}
