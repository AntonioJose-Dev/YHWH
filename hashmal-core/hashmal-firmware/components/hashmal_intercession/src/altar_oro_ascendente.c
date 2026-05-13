/**
 * ===================================================================
 * PIEZA 11 - EL ALTAR DEL INCIENSO - altar_oro_ascendente.c
 * Sub-pieza: EL FIREWALL ASCENDENTE - lo que sube se filtra
 * Levitico 10:1-3 + Exodo 30:9 + 2 Cronicas 26:16-21 + Exodo 30:37-38
 * ===================================================================
 *
 * "vayyiqju venei Aharon Nadav vaAvihu ISH MAJTATO vayyittenu vahen
 *  ESH vayyasimu 'aleiha qetoret vayyaqrivu lifnei YHWH ESH ZARA
 *  asher LO TSIVVA otam" - Lv 10:1
 * "Nadav y Avihu tomaron CADA UNO su brasero, pusieron FUEGO,
 *  pusieron sobre el incienso, y ofrecieron delante de YHWH FUEGO
 *  EXTRANO que EL NO LES MANDO."
 *
 * "vattetse ESH millifnei YHWH vattojal otam vayyamutu lifnei
 *  YHWH" - Lv 10:2
 * "Y SALIO fuego de delante de YHWH y los CONSUMIO, y MURIERON
 *  delante de YHWH."
 *
 * "biqrovai EQQADESH ve'al penei jol ha'am ekaved vayyidom
 *  Aharon" - Lv 10:3
 * "En los que se ACERCAN a Mi sere SANTIFICADO... y Aaron CALLO."
 *
 * "lo ta'alu 'alav qetoret ZARA ve'ola uminja venesej lo tissku
 *  'alav" - Ex 30:9
 * "NO ofrecereis sobre el incienso EXTRANO, ni holocausto, ni
 *  ofrenda, ni libacion."
 *
 * "ubematjunta LO TA'ASU LAJEM qodesh tihye leja laYHWH" - Ex 30:37
 * "En su proporcion NO HAREIS PARA VOSOTROS - santa sera para YHWH."
 *
 * "ish asher ya'ase jamoha lehaariaj bah VENIKRAT me'ammav" - Ex 30:38
 * "El varon que haga semejante para olerla sera CORTADO de su pueblo."
 *
 * --- EXEGESIS DEL FUEGO EXTRANO ---
 *
 *   esh zara (ASh ZRH)      fuego FORANEO. De zur (H2114) = ser
 *                              extranjero, apartarse. No del tavnit.
 *   lo tsivva (LA TsWH)     NO les mando. No es que estuviera
 *                              prohibido - simplemente NO MANDADO.
 *                              La diferencia es crucial: una sola
 *                              formula autorizada; todo lo demas,
 *                              por omision, es zara.
 *   vattetse esh millifnei     SALIO el fuego desde la misma
 *                              presencia. La respuesta es INMEDIATA.
 *                              No advertencia, no throttle, no retry.
 *                              Ejecucion directa al primer paquete
 *                              infractor.
 *   biqrovai eqqadesh          "En los cercanos me santifico."
 *                              Cuanto MAS CERCA del nucleo, MAS
 *                              ESTRICTO el filtro. Los operadores
 *                              autorizados estan bajo escrutinio
 *                              mayor, no menor.
 *   vayyidom Aharon            "Y Aaron CALLO." Sin apelacion.
 *                              Sin excepcion. Sin negociacion.
 *
 * --- UZZIAS (2 Cr 26:16-21) - SEGUNDO TESTIGO ---
 *
 * El rey Uzzias, fortalecido, se ensoberbecio y entro al templo a
 * quemar incienso sin ser sacerdote. 80 sacerdotes lo confrontaron.
 * La LEPRA aparecio en su frente AL INSTANTE (v.19). Lo expulsaron
 * apresuradamente. Uso no autorizado = penalizacion inmediata +
 * expulsion. Es el modelo de rechazo del firewall ascendente.
 *
 * --- 4 PROHIBICIONES EXPLICITAS (Ex 30:9) ---
 *
 *   1. qetoret zara   - incienso extrano (tipo no prescrito)
 *   2. 'ola           - holocausto (funcion incorrecta)
 *   3. minja          - ofrenda de grano (tipo de dato incorrecto)
 *   4. nesej          - libacion (canal equivocado)
 *
 * En firmware: 4 formas de rechazo del firewall. Cada una recibe
 * el mismo tratamiento: rechazo inmediato, log ERROR, incremento
 * de contador `filtradas_zara`.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   hashmal_intercession_ascendente_verificar
 *     Filtro principal: tipo + tamano + (opcionalmente) firma.
 *
 *   hashmal_intercession_ascendente_registrar_formula
 *     Registra la firma de referencia (UNA sola vez).
 *
 * --- LA FORMULA SE REGISTRA UNA VEZ ---
 *
 * Ex 30:37 - "ubematjunta LO TA'ASU LAJEM": en su PROPORCION no
 * hareis para vosotros. Una sola formula existe; registrarla DOS
 * veces equivale a "hacer otra" - zara. Por eso
 * `registrar_formula` devuelve ERR_FORMULA si ya hay registro.
 *
 * Ex 30:38 - "venikrat me'ammav": cortado de su pueblo. La
 * replicacion es muerte social. En firmware: el segundo registro
 * no sucede - el sistema rechaza, no por castigo, sino por diseno.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_intercession.h               API publica + formula_verificar
 *   hashmal_intercession_internal.h      altar_mut
 *   esp_log.h (ORO DE EGIPTO)            ALTAR_LOG_*
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

#include "hashmal_intercession.h"
#include "hashmal_intercession_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *ALTAR_TAG = "hashmal.altar.ascendente";

#define ALTAR_LOG_I(fmt, ...)  ESP_LOGI(ALTAR_TAG, fmt, ##__VA_ARGS__)
#define ALTAR_LOG_W(fmt, ...)  ESP_LOGW(ALTAR_TAG, fmt, ##__VA_ARGS__)
#define ALTAR_LOG_E(fmt, ...)  ESP_LOGE(ALTAR_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * ESTADO LOCAL - la firma de la formula
 *
 * Ex 30:37 - "ubematjunta lo ta'asu lajem": una SOLA formula. El
 * sistema guarda su huella (SHA-256, 32 bytes) aqui y la usa para
 * validar todo paquete ascendente. Solo se registra UNA VEZ.
 *
 * Hasta que pieza 13 (Shemen haMishja / hashmal_keygen) entregue la
 * clave definitiva, el sistema arranca SIN firma registrada. En ese
 * estado, el firewall PERMITE con WARN - la verificacion de formula
 * solo es estricta una vez la clave esta presente.
 * ================================================================== */
static bool    s_formula_registrada = false;
static uint8_t s_firma[HASHMAL_FORMULA_FIRMA_BYTES];

/* ==================================================================
 * HELPERS ESTATICOS
 * ================================================================== */

/**
 * Valida que el tipo pertenezca al tavnit. Cualquier otro valor es
 * "qetoret zara" - fuego extrano (Ex 30:9).
 */
static bool tipo_legitimo(hashmal_qetoret_tipo_t t)
{
    return (t == HASHMAL_QETORET_ORACION)
        || (t == HASHMAL_QETORET_REPORTE)
        || (t == HASHMAL_QETORET_EMERGENCIA);
}

/* ==================================================================
 * VERIFICADO #06 — Item 2 — Espejo intercession(11) ↔ firewall(14)
 *
 * "ló taʻalú ʻaláv qetóret zaráh veʻoláh uminjáh venésekh ló
 *  tisskhú ʻaláv." — Éxodo 30:9 (4 prohibiciones, este altar)
 * "qaj-lekhá sammím natáf ushjélet vejelbenáh sammím ulvonáh
 *  zakáh BAD BEBAD yihyé." — Éxodo 30:34 (4 especias firewall(14))
 *
 * Espejo bidireccional creado por el Padre en el mismo capítulo:
 *   v.6  ASHER IWWAʻÉD lekhá shámah (altar = lugar del encuentro)
 *   v.36 ASHER IWWAʻÉD lekhá shámah (incienso = lugar del encuentro)
 * Misma frase, dos especificaciones distintas — el cross-ref está
 * en el texto mismo.
 *
 * Mapping de las 4 prohibiciones (este archivo) a las 4 reglas
 * (hashmal_firewall_regla_t en incienso_firewall.c):
 *
 *   ZARAH  (protocolo foráneo)   ↔ SHEJELET (deep inspection)
 *   OLAH   (acción incorrecta)   ↔ LEVONA   (pureza/integridad)
 *   MINJAH (recurso incorrecto)  ↔ JELBENAH (anomalía)
 *   NESEKH (medio incorrecto)    ↔ NATAF    (flujo/medio)
 *
 * bad bebad: pesan igual, son distintas. El Padre nombra cada una
 * individualmente — colapsarlas borra nombres dados.
 * ================================================================== */

/**
 * Verificado #06 — Item 2 — contadores discriminados por las 4
 * prohibiciones de Éx 30:9. El contador global filtradas_zara
 * sigue acumulando todo (compatibilidad); además registramos por
 * prohibición individual (bad bebad — pesan igual, son distintas).
 */
static uint64_t s_filtradas_por_prohibicion[HASHMAL_QETORET_PROHIBICIONES];

static const char *nombre_prohibicion(hashmal_qetoret_prohibicion_t p)
{
    switch (p) {
    case HASHMAL_QETORET_PROHIB_ZARAH:  return "ZARÁH (protocolo foráneo)";
    case HASHMAL_QETORET_PROHIB_OLAH:   return "ʻOLÁH (acción/tamaño incorrecto)";
    case HASHMAL_QETORET_PROHIB_MINJAH: return "MINJÁH (recurso incorrecto)";
    case HASHMAL_QETORET_PROHIB_NESEKH: return "NÉSEKH (medio incorrecto)";
    default:                             return "<prohibición desconocida>";
    }
}

/**
 * Registra una deteccion de fuego extrano: incrementa contador +
 * log ERROR con referencia biblica precisa. Lv 10:2 - "vattetse
 * esh": la respuesta al fuego extrano es INMEDIATA.
 *
 * Verificado #06 — discrimina por prohibición individual (Éx 30:9).
 */
static void zara_detectado(const char *razon,
                           hashmal_qetoret_prohibicion_t prohibicion)
{
    hashmal_altar_t *a = hashmal_intercession_altar_mut();
    if (a != NULL) {
        a->filtradas_zara += 1ull;   /* contador global (compat) */
    }
    if ((unsigned)prohibicion < HASHMAL_QETORET_PROHIBICIONES) {
        s_filtradas_por_prohibicion[(unsigned)prohibicion] += 1ull;
    }
    ALTAR_LOG_E("ésh zará RECHAZADO [%s] (%s) — vattétsé ésh millifnéi "
                "YHWH (Lv 10:2); total_zara=%" PRIu64,
                nombre_prohibicion(prohibicion), razon,
                (uint64_t)((a != NULL) ? a->filtradas_zara : 0u));
}

uint64_t hashmal_intercession_prohibicion_contar(
    hashmal_qetoret_prohibicion_t prohibicion)
{
    if ((unsigned)prohibicion >= HASHMAL_QETORET_PROHIBICIONES) return 0ull;
    return s_filtradas_por_prohibicion[(unsigned)prohibicion];
}

/* ==================================================================
 * API PUBLICA - ascendente_verificar (Lv 10:1-2 + Ex 30:9)
 * ================================================================== */

/**
 * hashmal_intercession_ascendente_verificar - el filtro principal
 *
 * Todo lo que asciende pasa por aqui. La secuencia de chequeos
 * honra el orden del tavnit:
 *
 *   1) altar levantado              (no hay altar = no hay ascenso)
 *   2) payload no vacio             (Aaron no ofrecio con manos vacias)
 *   3) tipo del tavnit              (Ex 30:9 - tipos autorizados)
 *   4) tamano dentro del kikkar     (Ex 25:39 trasladado: sin exceso)
 *   5) firma de formula (si hay)    (Ex 30:37-38 - ubematjunta)
 *
 * Cada rechazo:
 *   - incrementa filtradas_zara
 *   - logea ERROR con cita del versiculo
 *   - retorna codigo especifico (ERR_ZARA, ERR_TAMANO, ERR_NULL, ERR_INIT)
 *
 * La respuesta es INMEDIATA. No hay throttle, no hay reintento.
 * Lv 10:2 - "vattetse esh millifnei YHWH": el fuego SALE al
 * instante. No advertencia. No apelacion. vayyidom Aharon.
 *
 * 2 testigos:
 *   Lv 10:1  - esh zara asher lo tsivva otam
 *   Ex 30:9  - lo ta'alu 'alav qetoret zara (las 4 prohibiciones)
 */
int hashmal_intercession_ascendente_verificar(hashmal_qetoret_tipo_t tipo,
                                              const uint8_t *datos,
                                              size_t tam)
{
    /* 1) altar levantado */
    hashmal_altar_t *a = hashmal_intercession_altar_mut();
    if (a == NULL) {
        ALTAR_LOG_E("ascendente: altar no levantado");
        return HASHMAL_INTERCESSION_ERR_INIT;
    }

    /* 2) payload presente (sin offering vacio).
     *    Biblicamente: Aaron llenaba sus punos (melo jofnav, Lv 16:12).
     *    Nadie se presenta ante YHWH con las manos vacias. */
    if (datos == NULL || tam == 0u) {
        ALTAR_LOG_E("ascendente: payload vacío — no se ofrece con manos vacías");
        return HASHMAL_INTERCESSION_ERR_NULL;
    }

    /* 3) tipo del tavnit - Ex 30:9 las 4 prohibiciones explicitas.
     *    Solo ORACION / REPORTE / EMERGENCIA son autorizados.
     *    Todo lo demas - qetoret zara, 'ola, minja o nesej - es
     *    rechazo inmediato. */
    if (!tipo_legitimo(tipo)) {
        zara_detectado("tipo no del tavnít — Éx 30:9 qetóret zará",
                       HASHMAL_QETORET_PROHIB_ZARAH);
        return HASHMAL_INTERCESSION_ERR_ZARA;
    }

    /* 4) tamano dentro del kikkar. Ex 30:9 - "ve'ola": ni
     *    holocausto (funcion/tamano que excede el altar). Payload
     *    que rebasa el presupuesto es ofrenda inapropiada. */
    if (tam > HASHMAL_QETORET_PAYLOAD_MAX) {
        zara_detectado("tam > kikkár — Éx 30:9 ʻolá (tamaño excesivo)",
                       HASHMAL_QETORET_PROHIB_OLAH);
        return HASHMAL_INTERCESSION_ERR_TAMANO;
    }

    /* 5) firma de formula.
     *
     *    FASE TEMPRANA (sin formula registrada):
     *      El altar funciona antes de que pieza 13 entregue la clave.
     *      Logueamos WARN para que el operador sepa que el filtro
     *      criptografico aun no esta activo, pero permitimos pasar.
     *      Ex 30:34-38 describe la formula - pero Ex 30:7-8 manda
     *      el qetoret tamid ANTES del capitulo de la formula: el
     *      servicio precede al sello exacto, por unos versos.
     *
     *    FASE OPERATIVA (formula registrada):
     *      Delegamos a altar_oro_formula.c. Si la verificacion
     *      falla, es fuego extrano - rechazo inmediato.
     */
    if (!s_formula_registrada) {
        ALTAR_LOG_W("ascendente: fórmula no registrada — filtro "
                    "criptográfico inactivo (fase temprana, pieza 13 pendiente)");
    } else {
        const bool firma_ok = hashmal_intercession_formula_verificar(
            datos, tam, s_firma, HASHMAL_FORMULA_FIRMA_BYTES);
        if (!firma_ok) {
            zara_detectado("firma NO valida — Lv 10:1 ésh zará ashér lo tsivvá",
                           HASHMAL_QETORET_PROHIB_ZARAH);
            return HASHMAL_INTERCESSION_ERR_ZARA;
        }
    }

    /* Todos los chequeos pasaron. El paquete puede ascender. */
    ALTAR_LOG_I("ascendente: verificado tipo=%d tam=%u "
                "(biqrovái eqqadésh, Lv 10:3)",
                (int)tipo, (unsigned)tam);
    return HASHMAL_INTERCESSION_OK;
}

/* ==================================================================
 * API PUBLICA - ascendente_registrar_formula (Ex 30:37-38)
 * ================================================================== */

/**
 * hashmal_intercession_ascendente_registrar_formula - ubematjunta
 *
 * Registra la firma de la formula contra la que se verificaran los
 * paquetes ascendentes. Llamada esperada desde pieza 13
 * (hashmal_keygen) tras generar la clave derivada del tavnit.
 *
 * --- REGLA UNICA: UNA SOLA REGISTRACION ---
 *
 * Ex 30:37 - "ubematjunta lo ta'asu lajem qodesh tihye leja
 * laYHWH": en su PROPORCION no hareis para vosotros - santa es
 * para YHWH. Registrar dos veces seria "hacer otra" formula - zara.
 *
 * Ex 30:38 - "venikrat me'ammav": el que replique es cortado. En
 * firmware: el segundo registro NO sucede. No por castigo - por
 * diseno. Quien llame dos veces recibe ERR_FORMULA.
 *
 * --- VALIDACIONES ---
 *
 *   - altar levantado
 *   - firma != NULL
 *   - tam_firma == HASHMAL_FORMULA_FIRMA_BYTES (32)
 *   - !s_formula_registrada (primera vez)
 *
 * 2 testigos:
 *   Ex 30:37 - ubematjunta lo ta'asu lajem (una sola formula)
 *   Ex 30:38 - asher ya'ase jamoha... venikrat (clonar = expulsion)
 */
int hashmal_intercession_ascendente_registrar_formula(const uint8_t *firma,
                                                      size_t tam_firma)
{
    if (hashmal_intercession_altar_mut() == NULL) {
        ALTAR_LOG_E("registrar_formula: altar no levantado");
        return HASHMAL_INTERCESSION_ERR_INIT;
    }

    if (firma == NULL) {
        ALTAR_LOG_E("registrar_formula: firma NULL");
        return HASHMAL_INTERCESSION_ERR_NULL;
    }

    if (tam_firma != HASHMAL_FORMULA_FIRMA_BYTES) {
        ALTAR_LOG_E("registrar_formula: tam_firma=%u ≠ %d (SHA-256)",
                    (unsigned)tam_firma, HASHMAL_FORMULA_FIRMA_BYTES);
        return HASHMAL_INTERCESSION_ERR_FORMULA;
    }

    if (s_formula_registrada) {
        /* Ex 30:37-38 - una sola formula. El segundo registro es
         * replicacion, y la replicacion es nikrat. */
        ALTAR_LOG_E("registrar_formula: fórmula YA registrada — "
                    "'ubematjuntá lo taʻasú lajém' (Éx 30:37)");
        ALTAR_LOG_E("  venikrát meʻammáv — replicación prohibida (Éx 30:38)");
        return HASHMAL_INTERCESSION_ERR_FORMULA;
    }

    memcpy(s_firma, firma, HASHMAL_FORMULA_FIRMA_BYTES);
    s_formula_registrada = true;

    ALTAR_LOG_I("═══ fórmula registrada ═══ (Éx 30:37)");
    ALTAR_LOG_I("  qódesh tihyé lejá laYHWH — firma ÚNICA activa");
    ALTAR_LOG_I("  filtro criptográfico: ACTIVO desde este momento");
    return HASHMAL_INTERCESSION_OK;
}

/* ==================================================================
 * CONEXION POR SANGRE - Lv 4:7 + Lv 16:18-19 - recibe desde Altar de Sacrificio
 *
 * "venatan haKKOHEN min haddam 'al QARNOT mizbaj qetoret hassammim
 *  asher lifnei YHWH asher be'ohel mo'ed." - Levitico 4:7
 * "El sacerdote pondra de la sangre sobre los CUERNOS del altar
 *  del incienso aromatico que esta delante de YHWH en la Tienda."
 *
 * Segundo testigo - Levitico 16:18-19:
 * "velaqaj middam happar umiddam hassa'ir venatan 'al QARNOT
 *  hammizbeaj saviv. vehizza 'alav min haddam be'etsba'o SHEVA
 *  pe'amim, veTIHHARO veQIDDESHO mittum'ot benei Yisrael."
 * "Tomara de la sangre del becerro y del macho cabrio y la pondra
 *  sobre los cuernos del altar alrededor. Rociara sobre el de la
 *  sangre con su dedo SIETE VECES, y lo LIMPIARA y lo SANTIFICARA
 *  de las impurezas de los hijos de Israel."
 *
 * Tavnit pieza 15 (CONEXIONES): "Altar de Sacrificio (06) -> Altar
 * del Incienso (11): sangre del pecado en los cuernos del altar
 * del incienso (Lv 4:7)". Este gateway recibe sangre del altar
 * de bronce - sin ella opera sin purificacion.
 * ================================================================== */

/* TODO - Phase 2: hashmal_intercession recibe token del altar.
 *
 * Cuando Phase 2 incluya los headers cruzados, este modulo
 * consultara el token de verificacion emitido por hashmal_auth
 * (pieza 06). La sangre se pone en los QARNOT (cuernos) del altar
 * del incienso - los 4 puntos de fuerza del gateway bidireccional.
 * No toca la superficie: toca los vertices donde se decide que
 * asciende y que desciende.
 *
 * Lv 16:18-19 establece la rutina periodica: en Yom Kippur la
 * sangre LIMPIA (vetihharo, H2891) y SANTIFICA (veqiddesho,
 * H6942) este altar de la TUMA acumulada, rociada SHEVA pe'amim.
 * Phase 2 aqui: rutina de purificacion periodica del gateway que
 * limpia sesiones expiradas y estados obsoletos. La contaminacion
 * no se "limpia" con reinicio - se retira con sangre del altar.
 * La diferencia es doctrinal: reinicio borra; sangre expia.
 *
 * Sin esta conexion el gateway funciona, pero sin la purificacion
 * que el Padre prescribe (Lv 4:7 + Lv 16:18-19). La conexion es
 * del Tavnit, no opcional - aunque no sea un check de autorizacion
 * como el de pieza 02, es un mandato ritual: el altar del incienso
 * se limpia con sangre del altar de bronce, siempre.
 * ================================================================== */

/* ==================================================================
 *
 *   "vayyaqrivu lifnei YHWH ESH ZARA asher LO TSIVVA otam.
 *    vattetse ESH millifnei YHWH vattojal otam vayyamutu lifnei
 *    YHWH." - Levitico 10:1-2
 *
 *   "biqrovai EQQADESH... vayyidom Aharon." - Levitico 10:3
 *
 *   "lo ta'alu 'alav qetoret ZARA..." - Exodo 30:9
 *
 *   El filtro no negocia. Lo que no viene del tavnit no sube.
 *   La respuesta es inmediata. El rechazo es total.
 *
 *   Aaron callo. Y el altar quedo mas santo por el silencio.
 *
 * ================================================================== */
