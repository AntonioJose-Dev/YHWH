/**
 * ===================================================================
 * PIEZA 14 - QETORET HASAMMIM - incienso_firewall.c
 * Sub-pieza: QETORET HADEQ - el incienso molido muy fino
 * Exodo 30:34-38 + Exodo 40:27,34 + Salmo 141:2 + Levitico 16:12-13
 * ===================================================================
 *
 * "veshajaqTA mimmennah HADEQ venataTA mimmennah LIFNEI ha'edut
 *  be'ohel mo'ed asher ivva'ed leja shammah - QODESH QODASHIM
 *  tihyeh lajem." - Exodo 30:36
 * "Moleras parte de ello MUY FINO y la pondras DELANTE del
 *  TESTIMONIO en la Tienda del Encuentro donde me encontrare
 *  contigo - SANTIDAD DE SANTIDADES sera para vosotros."
 *
 * "vayyaqTER 'alav QETORET sammim, ka'asher tsivva YHWH et
 *  Moshe." - Exodo 40:27
 * "Y QUEMO sobre el incienso aromatico, como YHWH mando a Moises."
 *
 * "vayjas ha'anan et ohel mo'ed uJVOD YHWH male et hammishkan."
 *   - Exodo 40:34
 * "La nube cubrio la Tienda del Encuentro, y la GLORIA de YHWH
 *  lleno el Mishkan."
 *
 * "tikkon TEFILLATI QETORET lefaneja, mas'at kappai minjat
 *  'arev." - Salmo 141:2
 * "Sea presentada mi ORACION como INCIENSO delante de Ti, el don
 *  de mis manos como ofrenda vespertina."
 *
 * "velaqaj melo' hammajta gajalei esh me'al hammizbeaj milifnei
 *  YHWH umelo' jofnav QETORET sammim DAQQA vehevi' mibeit
 *  lapparojet. venatan et haqqetoret 'al ha'esh lifnei YHWH,
 *  veJISSAH 'anan haqqetoret et HAKKAPORET asher 'al ha'edut
 *  velo yamut." - Levitico 16:12-13
 * "Tomara un incensario lleno de brasas de fuego del altar de
 *  delante de YHWH, y sus punos llenos de incienso aromatico
 *  MOLIDO, y lo metera del lado de adentro del velo. Pondra el
 *  incienso sobre el fuego delante de YHWH, y la NUBE del
 *  incienso CUBRIRA el propiciatorio que esta sobre el Testimonio,
 *  y no morira."
 *
 * "vayyiqju benei Aharon Nadav vaAvihu... vayyaqrivu lifnei YHWH
 *  ESH ZARA asher lo tsivva otam. vattetse esh millifnei YHWH
 *  vattokal otam." - Levitico 10:1-2
 * "Los hijos de Aaron, Nadav y Abihu... ofrecieron delante de YHWH
 *  FUEGO EXTRANO que no les mando. Salio fuego de delante de YHWH
 *  y los consumio."
 *
 * --- VOCABULARIO ---
 *
 *   qetoret      (QTRT, H7004)              INCIENSO.
 *   hadeq        (HDQ, H1851)                 MUY FINO - Ex 30:36
 *                                                granularidad maxima.
 *   daqqa        (DQH, H1851 fem)           molido, fino (Lv 16:12).
 *   ivva'ed      (AW'D, H3259 Nifal)        Me ENCONTRARE -
 *                                                punto deterministico.
 *   lifnei ha'edut                              DELANTE del Testimonio
 *                                                - Ex 30:36, frente al
 *                                                Arca.
 *   tikkon tefilati                             Sea presentada mi
 *                                                oracion - Sal 141:2.
 *   esh zara     (ASh ZRH, H784+H2114)      FUEGO EXTRANO -
 *                                                protocolo no prescrito.
 *   jissa        (KSH, H3680)               CUBRIR - la nube
 *                                                cubre al bajar
 *                                                (Lv 16:13).
 *
 * --- DIRECCIONES DEL FILTRADO ---
 *
 *   ASCENDENTE   Sal 141:2 "tikkon tefilati qetoret lefaneja" -
 *                 lo que SUBE del campamento al santuario (datos
 *                 entrantes al nucleo). Solo lo tahor asciende.
 *
 *   DESCENDENTE  Lv 16:12-13 "vejissa 'anan haqqetoret et hakaporet"
 *                 - la nube del incienso CUBRE al bajar (respuestas
 *                 del nucleo al campamento). Lo que desciende debe
 *                 estar envuelto por el mismo filtro.
 *
 * La misma formula (bad bevad, Ex 30:34) se aplica en ambas
 * direcciones. Las 4 reglas pesan igual subiendo y bajando -
 * flujo, profundidad, anomalia y pureza no cambian con el sentido.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   hashmal_firewall_init                   Ex 40:27 vayyaqter.
 *   hashmal_firewall_deinit                 Nm 4:5-6 cubrir al desmontar.
 *   hashmal_firewall_filtrar_ascendente     Sal 141:2 tikkon tefilati.
 *   hashmal_firewall_filtrar_descendente    Lv 16:12-13 jissa 'anan.
 *   hashmal_firewall_get_estado             getter publico inmutable.
 *
 *   + helpers internos (hashmal_firewall_internal.h):
 *   incienso_mut             mutable para hermanos.
 *   get_incienso_interno     const interno.
 *   ahora_ms                 monotonic clock ms.
 *
 * --- DELEGACION ---
 *
 * El filtrado por regla individual se delega a incienso_formato.c
 * (hashmal_firewall_formato_verificar_regla). Este archivo orquesta
 * el orden de las 4 reglas y actualiza estadisticas. La separacion
 * respeta el principio kelim: cada archivo un utensilio.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_firewall.h
 *   hashmal_firewall_internal.h    incienso_mut + ahora_ms
 *   esp_log.h    (ORO EGIPTO)      logging
 *   esp_timer.h  (ORO EGIPTO)      ahora_ms
 *
 * Autor: Antonio Jose Marin Soto (Betsalel - Ex 31:2)
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <inttypes.h>

#include "esp_log.h"
#include "esp_timer.h"

#include "hashmal_firewall.h"
#include "hashmal_firewall_internal.h"
#include "hashmal_keygen.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *INC_TAG = "hashmal.firewall.incienso";

#define INC_LOG_I(fmt, ...)  ESP_LOGI(INC_TAG, fmt, ##__VA_ARGS__)
#define INC_LOG_W(fmt, ...)  ESP_LOGW(INC_TAG, fmt, ##__VA_ARGS__)
#define INC_LOG_E(fmt, ...)  ESP_LOGE(INC_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * SINGLETON SAGRADO
 *
 * UN firewall por nodo. Ex 30:34 "qaj leja sammim" (singular mandato)
 * - una sola formula, una sola frontera, un solo incensario
 * operativo por nodo.
 * ================================================================== */
static hashmal_firewall_t g_incienso;
static bool               g_iniciado = false;

/* ==================================================================
 * API INTERNA - helpers compartidos (hashmal_firewall_internal.h)
 * ================================================================== */

hashmal_firewall_t *hashmal_firewall_incienso_mut(void)
{
    if (!g_iniciado) return NULL;
    return &g_incienso;
}

const hashmal_firewall_t *hashmal_firewall_get_incienso_interno(void)
{
    if (!g_iniciado) return NULL;
    return &g_incienso;
}

int64_t hashmal_firewall_ahora_ms(void)
{
    return (int64_t)(esp_timer_get_time() / 1000);
}

/* ==================================================================
 * HELPERS LOCALES (kelim autocontenidos)
 * ================================================================== */

static inline bool par_id_valido(uint8_t par_id)
{
    return par_id < HASHMAL_FIREWALL_MAX_PARES;
}

static const char *nombre_veredicto(hashmal_firewall_veredicto_t v)
{
    switch (v) {
    case HASHMAL_FIREWALL_TAHOR:
        return "TAHOR (H2889 — puro, Éx 30:35 + Lv 13:13)";
    case HASHMAL_FIREWALL_TAME:
        return "TAME (H2931 — impuro, Lv 13:14 + Nm 19:20)";
    case HASHMAL_FIREWALL_ZAR:
        return "ZAR (H2114 — extraño, Lv 10:1 + Éx 30:33)";
    default:
        return "<veredicto desconocido>";
    }
}

static const char *nombre_regla(hashmal_firewall_regla_t r)
{
    switch (r) {
    case HASHMAL_FIREWALL_REGLA_NATAF:    return "NATAF (H5198 flujo)";
    case HASHMAL_FIREWALL_REGLA_SHEJELET: return "SHEJELET (H7827 profundidad)";
    case HASHMAL_FIREWALL_REGLA_JELBENAH: return "JELBENAH (H2464 anomalía)";
    case HASHMAL_FIREWALL_REGLA_LEVONA:   return "LEVONA (H3828 pureza)";
    default:                              return "<regla desconocida>";
    }
}

/**
 * Mapea el codigo de error de la regla individual a veredicto:
 *   OK          -> TAHOR  (limpio)
 *   ERR_ZAR     -> ZAR    (protocolo extrano, esh zara)
 *   cualquier otro error -> TAME (impuro por formato)
 */
static hashmal_firewall_veredicto_t codigo_a_veredicto(int rc)
{
    if (rc == HASHMAL_FIREWALL_OK)   return HASHMAL_FIREWALL_TAHOR;
    if (rc == HASHMAL_FIREWALL_ERR_ZAR) return HASHMAL_FIREWALL_ZAR;
    return HASHMAL_FIREWALL_TAME;
}

/* ==================================================================
 * VERIFICADO #06 — Item 2 — Espejo firewall(14) ↔ intercession(11)
 *
 * "ló taʻalú ʻaláv qetóret zaráh veʻoláh uminjáh venésekh ló
 *  tisskhú ʻaláv." — Éxodo 30:9 (4 prohibiciones del altar)
 * "qaj-lekhá sammím natáf ushjélet vejelbenáh sammím ulvonáh
 *  zakáh BAD BEBAD yihyé." — Éxodo 30:34 (4 especias bad bebad)
 *
 * El Padre crea la referencia cruzada en el MISMO capítulo:
 *   v.6  vena­tatáh oto lifnéi happarójet ASHER IWWAʻÉD lekhá shámah
 *   v.36 vena­tatáh mimménnah lifnéi haʻedút ASHER IWWAʻÉD lekhá shámah
 * Misma frase, dos especificaciones — espejo bidireccional textual.
 *
 * Mapping de las 4 reglas (este archivo) a las 4 prohibiciones
 * (hashmal_intercession_prohibicion_t en altar_oro_ascendente.c):
 *
 *   NATAF    (flujo)        ↔ NESEKH (libación, medio fluido)
 *   SHEJELET (deep insp.)   ↔ ZARAH  (protocolo foráneo)
 *   JELBENAH (anomalía)     ↔ MINJAH (recurso incorrecto)
 *   LEVONA   (pureza)       ↔ OLAH   (acción incorrecta — tamaño excesivo)
 *
 * bad bebad (Éx 30:34): pesan IGUAL, son DISTINTAS. Ninguna regla
 * domina; ninguna prohibición se confunde con otra.
 * ================================================================== */

/**
 * Pipeline comun para filtrar_ascendente y filtrar_descendente.
 * El orden de reglas es fijo (nataf -> shejelet -> jelbenah -> levona)
 * y simetrico en ambas direcciones (bad bevad, Ex 30:34).
 *
 * Retorna el codigo de error para devolver al llamante:
 *   OK           - TAHOR, pasa la frontera
 *   ERR_FORMATO  - TAME, rechazado por formato
 *   ERR_ZAR      - ZAR, rechazado por protocolo extrano
 *   ERR_*        - errores previos (init, CB, par invalido, ...)
 */
static int pipeline_filtrar(uint8_t par_id,
                             const uint8_t *datos, size_t len,
                             const char *direccion)
{
    hashmal_firewall_t *f = hashmal_firewall_incienso_mut();
    if (f == NULL) {
        INC_LOG_E("filtrar(%s): firewall no iniciado (init primero)",
                  direccion);
        return HASHMAL_FIREWALL_ERR_NO_INIT;
    }

    /* Nm 17:13 - si Aaron esta entre los muertos y los vivos, nadie
     * mas cruza. CB abierto -> todo detenido. */
    if (f->circuitbreaker == HASHMAL_FIREWALL_CB_ABIERTO) {
        INC_LOG_W("filtrar(%s): circuit breaker ABIERTO "
                  "(Nm 17:13 bein hammetím uvein hajjayyím) — "
                  "rechazado por emergencia",
                  direccion);
        return HASHMAL_FIREWALL_ERR_CB_ACTIVO;
    }

    if (!par_id_valido(par_id)) {
        INC_LOG_W("filtrar(%s): par_id %u fuera de rango [0..%u]",
                  direccion, (unsigned)par_id,
                  (unsigned)(HASHMAL_FIREWALL_MAX_PARES - 1u));
        return HASHMAL_FIREWALL_ERR_PAR_INVALIDO;
    }

    /* Ex 30:38 nikrat - ya expulsado, no hay filtrado posible. */
    if (f->pares[par_id].nivel == HASHMAL_FIREWALL_NIVEL_NIKRAT) {
        INC_LOG_W("filtrar(%s): par %u expulsado (NIKRAT, Éx 30:38) "
                  "— venikrat meʻammáv",
                  direccion, (unsigned)par_id);
        return HASHMAL_FIREWALL_ERR_NIKRAT;
    }

    if (datos == NULL) {
        INC_LOG_W("filtrar(%s): datos NULL", direccion);
        return HASHMAL_FIREWALL_ERR_NULL;
    }
    if (len == 0u) {
        INC_LOG_W("filtrar(%s): len=0 — no hay qetóret que moler",
                  direccion);
        return HASHMAL_FIREWALL_ERR_FORMATO;
    }

    /* Aplicar las 4 reglas en orden. Ex 30:34 bad bevad: ninguna
     * domina, todas se aplican. Primera que rechaza decide. */
    static const hashmal_firewall_regla_t ORDEN[HASHMAL_FIREWALL_REGLAS_TOTAL] = {
        HASHMAL_FIREWALL_REGLA_NATAF,
        HASHMAL_FIREWALL_REGLA_SHEJELET,
        HASHMAL_FIREWALL_REGLA_JELBENAH,
        HASHMAL_FIREWALL_REGLA_LEVONA,
    };

    hashmal_firewall_veredicto_t veredicto = HASHMAL_FIREWALL_TAHOR;
    hashmal_firewall_regla_t     regla_fallida = HASHMAL_FIREWALL_REGLA_NATAF;
    int rc_regla = HASHMAL_FIREWALL_OK;

    for (uint8_t i = 0; i < HASHMAL_FIREWALL_REGLAS_TOTAL; i++) {
        rc_regla = hashmal_firewall_formato_verificar_regla(
            ORDEN[i], datos, len);
        if (rc_regla != HASHMAL_FIREWALL_OK) {
            veredicto     = codigo_a_veredicto(rc_regla);
            regla_fallida = ORDEN[i];
            break;
        }
    }

    /* Actualizar estadisticas - siempre se cuenta el mensaje. */
    f->estadisticas.mensajes_total++;
    if ((unsigned)veredicto < HASHMAL_FIREWALL_VEREDICTOS_TOTAL) {
        f->estadisticas.veredictos[veredicto]++;
    }

    const int64_t ahora = hashmal_firewall_ahora_ms();
    f->timestamp_ms = ahora;

    if (veredicto == HASHMAL_FIREWALL_TAHOR) {
        INC_LOG_I("filtrar(%s): par %u, len=%zu → %s",
                  direccion, (unsigned)par_id, len,
                  nombre_veredicto(veredicto));
        return HASHMAL_FIREWALL_OK;
    }

    /* Rechazo - contabilizar por regla. */
    if ((unsigned)regla_fallida < HASHMAL_FIREWALL_REGLAS_TOTAL) {
        f->estadisticas.por_regla[regla_fallida]++;
    }

    if (veredicto == HASHMAL_FIREWALL_ZAR) {
        INC_LOG_E("filtrar(%s): par %u, len=%zu → %s por %s — "
                  "ésh zará (Lv 10:1-2)",
                  direccion, (unsigned)par_id, len,
                  nombre_veredicto(veredicto),
                  nombre_regla(regla_fallida));
        return HASHMAL_FIREWALL_ERR_ZAR;
    }

    /* TAME */
    INC_LOG_W("filtrar(%s): par %u, len=%zu → %s por %s",
              direccion, (unsigned)par_id, len,
              nombre_veredicto(veredicto),
              nombre_regla(regla_fallida));
    return HASHMAL_FIREWALL_ERR_FORMATO;
}

/* ==================================================================
 * hashmal_firewall_init - Exodo 40:27 + Nm 4:16
 *
 * "vayyaqter 'alav qetoret sammim ka'asher tsivva YHWH et Moshe"
 * (Ex 40:27). El ultimo paso del levantamiento: se enciende el
 * incienso y la Gloria llena el Mishkan (v.34). Aqui inicializamos
 * el singleton - todas las penalizaciones en TAHOR, CB cerrado,
 * heartbeat vacio, estadisticas a cero.
 *
 * Idempotencia: ya iniciado -> ERR_YA_INIT.
 *
 * 2 testigos:
 *   Ex 40:27  - vayyaqter (mandato cumplido)
 *   Nm 4:16   - ufeqdat El'azar... qetoret hassammim (encomienda
 *                perpetua del incienso)
 * ================================================================== */
int hashmal_firewall_init(void)
{
    if (g_iniciado) {
        INC_LOG_W("init: firewall ya iniciado (idempotente)");
        return HASHMAL_FIREWALL_ERR_YA_INIT;
    }

    memset(&g_incienso, 0, sizeof(g_incienso));

    const int64_t ahora = hashmal_firewall_ahora_ms();

    g_incienso.activo              = true;
    g_incienso.circuitbreaker      = HASHMAL_FIREWALL_CB_CERRADO;
    g_incienso.timestamp_ms        = ahora;
    g_incienso.timestamp_inicio_ms = ahora;

    /* Penalizaciones per-par en TAHOR. */
    for (uint8_t i = 0; i < HASHMAL_FIREWALL_MAX_PARES; i++) {
        g_incienso.pares[i].nivel                 = HASHMAL_FIREWALL_NIVEL_TAHOR;
        g_incienso.pares[i].infracciones          = 0u;
        g_incienso.pares[i].timestamp_ultima_ms   = 0;
        g_incienso.pares[i].timestamp_escalada_ms = 0;
    }

    /* Heartbeat vacio - manana y tarde pendientes. */
    g_incienso.heartbeat.manana_activo       = false;
    g_incienso.heartbeat.tarde_activo        = false;
    g_incienso.heartbeat.timestamp_manana_ms = 0;
    g_incienso.heartbeat.timestamp_tarde_ms  = 0;
    g_incienso.heartbeat.ciclos_completados  = 0u;

    /* Estadisticas a cero. */
    g_incienso.estadisticas.mensajes_total = 0u;
    for (uint8_t i = 0; i < HASHMAL_FIREWALL_VEREDICTOS_TOTAL; i++) {
        g_incienso.estadisticas.veredictos[i] = 0u;
    }
    for (uint8_t i = 0; i < HASHMAL_FIREWALL_REGLAS_TOTAL; i++) {
        g_incienso.estadisticas.por_regla[i] = 0u;
    }

    g_iniciado = true;

    INC_LOG_I("══════ QETÓRET HASAMMÍM — iniciado (Éx 40:27) ══════");
    INC_LOG_I("  vayyaqtér ʻaláv qetóret sammím (Éx 40:27)");
    INC_LOG_I("  %u pares en TAHOR, CB cerrado, heartbeat vacío",
              (unsigned)HASHMAL_FIREWALL_MAX_PARES);
    INC_LOG_I("  4 reglas bad bevad (Éx 30:34): natáf + shejélet + "
              "jelbenáh + levoná");
    INC_LOG_I("  hadéq: granularidad máxima (Éx 30:36)");
    INC_LOG_I("  ufeqdát Elʻazár... qetóret hassammím (Nm 4:16)");

    return HASHMAL_FIREWALL_OK;
}

/* ==================================================================
 * hashmal_firewall_deinit - Nm 4:5-6 + Nm 10:17
 *
 * Cubrimiento para desmontaje. memset + flag bajado. Idempotente
 * silencioso si no iniciado.
 *
 * 2 testigos:
 *   Nm 4:5-6  - vejissu (cubrir con pieles antes de transportar)
 *   Nm 10:17  - vehurad hammishkan (desmontar el Mishkan)
 * ================================================================== */
void hashmal_firewall_deinit(void)
{
    if (!g_iniciado) {
        return;  /* idempotente silencioso */
    }

    INC_LOG_I("qetóret cubierto para transporte (Nm 4:5-6 + Nm 10:17)");

    memset(&g_incienso, 0, sizeof(g_incienso));
    g_iniciado = false;
}

/* ==================================================================
 * hashmal_firewall_filtrar_ascendente - Salmo 141:2
 *
 * "tikkon tefilati qetoret lefaneja" - el mensaje SUBE como
 * incienso delante de YHWH. Solo lo que pasa las 4 reglas asciende.
 *
 * 2 testigos:
 *   Sal 141:2 - tefilati qetoret lefaneja (flujo ascendente)
 *   Ap 8:3-4  - "anebe ho kapnos ton thymiamaton... enopion tou
 *                Theou" (subio el humo de los inciensos delante de
 *                Dios; libro distinto, mismo patron ascendente)
 * ================================================================== */
int hashmal_firewall_filtrar_ascendente(uint8_t par_id,
                                         const uint8_t *datos, size_t len)
{
    return pipeline_filtrar(par_id, datos, len, "ascendente");
}

/* ==================================================================
 * hashmal_firewall_filtrar_descendente - Levitico 16:12-13
 *
 * "vejissah 'anan haqqetoret et hakaporet" - la nube del incienso
 * CUBRE al bajar. Solo lo autorizado desciende, y va envuelto por
 * el mismo filtro (bad bevad - simetrico, Ex 30:34).
 *
 * 2 testigos:
 *   Lv 16:12-13 - jissah 'anan haqqetoret (nube que cubre al bajar)
 *   Ex 30:36    - lifnei ha'edut (posicion del filtro, frente al
 *                  Testimonio, protege ambos lados)
 * ================================================================== */
int hashmal_firewall_filtrar_descendente(uint8_t par_id,
                                          const uint8_t *datos, size_t len)
{
    return pipeline_filtrar(par_id, datos, len, "descendente");
}

/* ==================================================================
 * hashmal_firewall_get_estado - getter publico inmutable
 *
 * Lectura pura, sin logs, sin side effects. NULL si no iniciado.
 * ================================================================== */
const hashmal_firewall_t *hashmal_firewall_get_estado(void)
{
    if (!g_iniciado) return NULL;
    return &g_incienso;
}

/* ==================================================================
 * P21 — hashmal_firewall_validar_operativo - Éx 30:35
 *
 * "roqéaj memulláj tahór qódesh" — 5 cualificadores del incienso.
 * El firewall se declara operativo solo tras verificar TODAS las 5.
 *
 * 1. roqáj         — ingredientes en orden (4 reglas compiladas)
 * 2. maʻaséh roqéaj — ejecución experta (init sin errores)
 * 3. memulláj      — sellado con sal (clave keygen aplicada)
 * 4. tahór         — puro (contador de violaciones = 0)
 * 5. qódesh        — consagrado (pieza 13 keygen activa)
 *
 * Ref: Éx 30:35 roqéaj memulláj tahór qódesh (H7543+H4414+H2889+H6944)
 * ================================================================== */
bool hashmal_firewall_validar_operativo(void)
{
    const hashmal_firewall_t *f = hashmal_firewall_get_incienso_interno();

    /* 1. Iniciado? */
    if (f == NULL) {
        INC_LOG_W("validar_operativo: firewall no iniciado (roqáj incompleto)");
        return false;
    }

    /* 2. Ejecución sin error de init */
    if (!f->activo) {
        INC_LOG_W("validar_operativo: maʻaséh roqéaj falló en init");
        return false;
    }

    /* 3. Memulláj: se requiere que hashmal_keygen esté al menos
     * PREPARADO (formula verificada). Aquí es un stub que llama a
     * keygen_estado(). En fase 2, verifica la root key. */
    int keygen_ok = hashmal_keygen_estado();
    if (keygen_ok != HASHMAL_KEYGEN_OK) {
        INC_LOG_W("validar_operativo: memulláj falló — keygen no preparada");
        return false;
    }

    /* 4. Tahór: sin violaciones pendientes (pares en TAHOR) */
    for (uint8_t i = 0; i < HASHMAL_FIREWALL_MAX_PARES; i++) {
        if (f->pares[i].nivel != HASHMAL_FIREWALL_NIVEL_TAHOR) {
            INC_LOG_W("validar_operativo: tahór falló — par %u no limpio "
                      "(nivel=%d)",
                      (unsigned)i, (int)f->pares[i].nivel);
            return false;
        }
    }

    /* 5. Qódesh: keygen debe estar ACTIVO (8 componentes ungidos) */
    int keygen_activo = hashmal_keygen_activo();
    if (keygen_activo != HASHMAL_KEYGEN_OK) {
        INC_LOG_W("validar_operativo: qódesh falló — keygen no ACTIVO");
        return false;
    }

    INC_LOG_I("✓ QETÓRET OPERATIVO — roqéaj memulláj tahór qódesh "
              "(Éx 30:35)");
    return true;
}

/* ==================================================================
 * P22 — hashmal_firewall_detectar_leharij - Éx 30:38
 *
 * "ish ashér yaʻaséh jamóha LEHARÍAJ báh — veNIKRAT meʻammáv"
 * Quien haga para OLERLA PARA SÍ — será CORTADO.
 *
 * leharíaj (H7306) = olor para uso personal, egoísmo.
 * Detecta nodos que abusan del firewall:
 *   - Excepciones personales de tráfico
 *   - Sniffing selectivo (captura discriminada)
 *   - Enrutamiento que se favorece a sí mismo
 *
 * Fase 1: heurística simple. Fase 2: seguimiento de excepciones
 * y patrones de sesgo en el routing.
 *
 * Ref: Éx 30:38 leharíaj (H7306 oler para sí)
 * ================================================================== */
int hashmal_firewall_detectar_leharij(uint8_t nodo_id)
{
    hashmal_firewall_t *f = hashmal_firewall_incienso_mut();
    if (f == NULL) {
        INC_LOG_E("detectar_leharij: firewall no iniciado");
        return HASHMAL_FIREWALL_ERR_NO_INIT;
    }

    if (!par_id_valido(nodo_id)) {
        INC_LOG_W("detectar_leharij: nodo_id %u fuera de rango", nodo_id);
        return HASHMAL_FIREWALL_ERR_PAR_INVALIDO;
    }

    /* Fase 1: detectar patrones de abuso. Si el par tiene
     * penalizaciones variadas INCONSISTENTES (cambian por capricho)
     * o si ha saltado violaciones, hay sospecha de leharíaj. */
    const hashmal_firewall_penalizacion_par_t *p = &f->pares[nodo_id];

    /* Patrón sospechoso: múltiples violaciones en corto tiempo
     * (indicador de sniffing selectivo, no comportamiento normal). */
    const int64_t ahora = hashmal_firewall_ahora_ms();
    const int64_t tiempo_transcurrido = ahora - p->timestamp_ultima_ms;

    /* Si 50+ infracciones en menos de 1 minuto = spam selectivo */
    if (p->infracciones > 50u && tiempo_transcurrido < 60000) {
        INC_LOG_E("leharíaj detectado — nodo %u: %u infracciones en %" PRId64 "ms "
                  "(sniffing selectivo, Éx 30:38)",
                  nodo_id, p->infracciones, tiempo_transcurrido);
        return HASHMAL_FIREWALL_ERR_ZAR;
    }

    /* Limpio hasta que se demuestre lo contrario */
    return HASHMAL_FIREWALL_OK;
}

/* ==================================================================
 *
 *   "veshajaqTA mimmennah HADEQ venataTA mimmennah LIFNEI
 *    ha'edut." - Exodo 30:36
 *
 *   "vayyaqTER 'alav qetoret sammim... vayjas ha'anan et ohel
 *    mo'ed ukhvod YHWH male et hammishkan." - Ex 40:27,34
 *
 *   "tikkon TEFILLATI QETORET lefaneja." - Salmo 141:2
 *
 *   "vejissah 'anan haqqetoret et hakkaporet asher 'al ha'edut."
 *     - Levitico 16:13
 *
 *   El incienso molido fino es la primera capa de frontera. Se
 *   pone DELANTE del Testimonio (Ex 30:36 lifnei ha'edut) - no
 *   dentro, no lejos: justo en la entrada del nucleo. Lo que sube
 *   pasa por el (Sal 141:2 tikkon tefilati); lo que baja va
 *   cubierto por el (Lv 16:13 jissa 'anan). Las cuatro reglas
 *   pesan igual (bad bevad, Ex 30:34) y se aplican en orden:
 *   nataf, shejelet, jelbenah, levona. Si ninguna rechaza ->
 *   tahor, asciende. Si una rechaza por formato -> tame. Si una
 *   detecta protocolo extrano -> zar, esh zara. La primera falla
 *   decide; no se continua evaluando para no gastar el incienso
 *   en lo que ya fue rechazado.
 *
 *   Cuando Moises quemo este incienso, la Gloria lleno el Mishkan
 *   (Ex 40:34). Por eso esta pieza es la ULTIMA del levantamiento:
 *   sin frontera solida delante del Testimonio, no hay presencia
 *   permanente.
 *
 *   === QETORET HADEQ - incienso_firewall.c COMPLETO ===
 *
 *   -- Los 5 archivos hermanos -----------------------------------
 *
 *     incienso_firewall.c        QETORET HADEQ            <- ESTE
 *     incienso_formato.c         MA'ASEH ROQEAJ
 *     incienso_circuitbreaker.c  BEIN HAMMETIM UVEIN HAJJAYYIM
 *     incienso_heartbeat.c       TAMID BABOQER UVEIN HA'ARBAYIM
 *     incienso_penalizacion.c    VENIKRAT ME'AMMAV
 *
 *   Archivo 1 de 5 del Incienso Sagrado. Siguiente:
 *     incienso_formato.c - ma'aseh roqeaj (Ex 30:35)
 *
 *   PASO 10 de Exodo 40:27,34 - Pieza 14, Archivo 1 de 5.
 *
 * ================================================================== */
