/**
 * ===================================================================
 * PIEZA 11 - EL ALTAR DEL INCIENSO - altar_oro_escudo.c
 * Sub-pieza: EL ESCUDO DESCENDENTE + SINGLETON + CICLO DE VIDA
 * Exodo 30:1-8 (construccion) + Levitico 16:12-13 (escudo / nube)
 * ===================================================================
 *
 * "ve'asita mizbeaj miqtar qetoret 'atsei shittim ta'ase oto" - Ex 30:1
 * "Haras un ALTAR para quemar incienso; de madera de acacia lo haras."
 *
 * "amma orko ve'amma rojbo ravua' yihye ve'ammatayim qomato
 *  mimmennu qarnotav" - Ex 30:2
 * "Un codo su largo, un codo su ancho, CUADRADO sera, y DOS codos
 *  su altura. DE EL salen sus cuernos."
 *
 * "vetsippita oto zahav tahor et gaggo ve'et qirotav saviv ve'et
 *  qarnotav ve'asita lo zer zahav saviv" - Ex 30:3
 * "Lo cubriras de ORO puro - su TECHO, sus PAREDES alrededor y sus
 *  CUERNOS. Y le haras CORONA de oro alrededor."
 *
 * "venatatta oto lifnei happarojet... asher IVVA'ED leja shamma"
 *   - Ex 30:6
 * "Lo pondras DELANTE DEL VELO... donde ME ENCONTRARE CONTIGO ALLI."
 *
 * "qetoret TAMID lifnei YHWH ledoroteijem" - Ex 30:8
 * "Incienso PERPETUO delante de YHWH por vuestras generaciones."
 *
 * "venatan et haqqetoret 'al haesh lifnei YHWH veKHISSA 'ANAN
 *  haqqetoret et hakkapporet asher 'al ha'edut VELO YAMUT" - Lv 16:13
 * "Pondra el incienso sobre el fuego delante de YHWH, y la NUBE del
 *  incienso CUBRIRA la Kaporet que esta sobre el Testimonio, y NO
 *  MORIRA."
 *
 * --- EXEGESIS DEL ESCUDO ---
 *
 *   kissa  (KSH, Piel H3680)  CUBRIR. La nube NO toca al
 *                                operador: lo rodea, lo envuelve.
 *                                Accion intensiva, deliberada.
 *   'anan  ('NN, H6051)        NUBE. Escudo gaseoso - no barrera
 *                                solida: permite paso autorizado,
 *                                oculta lo intimo.
 *   majta  (MChTH, H4289)     brasero. MISMO utensilio que la
 *                                Menora (v.38). El fuego se recoge
 *                                con la misma herramienta.
 *   daqqa  (DQH, H1851)      molido FINO. Granularidad maxima:
 *                                cada respuesta individualmente
 *                                protegida, no en lotes crudos.
 *   velo yamut (WLA YMWT)   Y NO MORIRA. Sin escudo = muerte.
 *                                El altar es criptografia vital.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   hashmal_intercession_init           Ex 30:1-8 + Ex 40:5
 *   hashmal_intercession_deinit         Nm 4:11 (cubrir con tejelet)
 *   hashmal_intercession_get_altar      const singleton
 *   hashmal_intercession_es_tamid       Ex 30:8 (?perpetuo activo?)
 *   hashmal_intercession_escudo_activar   Lv 16:12-13
 *   hashmal_intercession_escudo_desactivar
 *   hashmal_intercession_escudo_activo
 *
 *   Ademas, los helpers internos compartidos:
 *
 *   hashmal_intercession_altar_mut           mutable para hermanos
 *   hashmal_intercession_get_altar_interno   const interno
 *   hashmal_intercession_ahora_ms            ms monotonico
 *   hashmal_intercession_cuerno_legitimo     id in [0, 4)
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_intercession.h              API publica
 *   hashmal_intercession_internal.h     helpers compartidos
 *   esp_log.h   (ORO DE EGIPTO)         logging
 *   esp_timer.h (ORO DE EGIPTO)         ahora_ms
 *
 * Ex 30:3 - "zahav tahor": el altar es oro puro. Este componente
 * NO depende de otras piezas para su identidad. Confianza propia.
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

#include "hashmal_intercession.h"
#include "hashmal_intercession_internal.h"

/* Verificado #03 — Cadena 3 — orquestador Yom Kippur cruza piezas */
#include "hashmal_consensus.h"
#include "hashmal_auth.h"
#include "hashmal_firewall.h"
#include "hashmal_framework.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *ALTAR_TAG = "hashmal.altar.escudo";

#define ALTAR_LOG_I(fmt, ...)  ESP_LOGI(ALTAR_TAG, fmt, ##__VA_ARGS__)
#define ALTAR_LOG_W(fmt, ...)  ESP_LOGW(ALTAR_TAG, fmt, ##__VA_ARGS__)
#define ALTAR_LOG_E(fmt, ...)  ESP_LOGE(ALTAR_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * SINGLETON SAGRADO
 *
 * UN altar por nodo - Ex 30:1 "mizbeaj" en singular. Sin factory,
 * sin pool. Un solo bloque de oro puro ante el velo.
 * ================================================================== */
static hashmal_altar_t g_altar;
static bool            g_encendido = false;

/**
 * Ex 30:9 "lo ta'alu 'alav qetoret zara" - el escudo es estado
 * LOCAL de este archivo (no en el struct publico). Otras capas
 * consultan via `hashmal_intercession_escudo_activo()`.
 *
 * La razon de mantenerlo file-static: encapsulacion. Nadie fuera
 * puede tocar este bool sin pasar por las funciones del escudo -
 * que son las unicas que saben cuando la nube es legitima.
 */
static bool s_escudo_activo = false;

/* ==================================================================
 * HELPERS ESTATICOS
 * ================================================================== */

/**
 * Rollback atomico. Si cualquier invariante falla a mitad del
 * init, el altar colapsa a cero. "Si el altar se contamina, se
 * destruye - no se repara parcialmente." Precedente: Lv 10:1-2
 * (ofrenda a medias ante YHWH es muerte).
 */
static void colapsar_a_cero(void)
{
    memset(&g_altar, 0, sizeof(g_altar));
    s_escudo_activo = false;
    g_encendido = false;
}

/**
 * Construye UN cuerno (qeren). Ex 30:2 - "mimmennu qarnotav":
 * los cuernos nacen DEL altar. activo=false al inicio - su
 * activacion llega con el primer uso legitimo.
 */
static void construir_qeren(hashmal_qeren_t *q, uint8_t id)
{
    q->id              = id;
    q->activo          = false;   /* activacion por uso, no por forja */
    q->ultimo_uso_ms   = 0u;
    q->solicitudes     = 0u;
}

/* ==================================================================
 * API INTERNA - helpers compartidos
 * ================================================================== */

hashmal_altar_t *hashmal_intercession_altar_mut(void)
{
    if (!g_encendido) return NULL;
    return &g_altar;
}

const hashmal_altar_t *hashmal_intercession_get_altar_interno(void)
{
    if (!g_encendido) return NULL;
    return &g_altar;
}

uint64_t hashmal_intercession_ahora_ms(void)
{
    return ((uint64_t)esp_timer_get_time()) / 1000ull;
}

bool hashmal_intercession_cuerno_legitimo(uint8_t id)
{
    return id < HASHMAL_ALTAR_CUERNOS;   /* [0, 4) - v.2 qarnotav */
}

/* ==================================================================
 * hashmal_intercession_init - Exodo 30:1-8 (verso por verso)
 *
 * Levanta el altar en el ORDEN del Padre. Cualquier invariante
 * rota -> rollback atomico.
 * ================================================================== */
int hashmal_intercession_init(void)
{
    if (g_encendido) {
        ALTAR_LOG_W("init: altar ya levantado (idempotente, no-op)");
        return HASHMAL_INTERCESSION_OK;
    }

    /* Partimos de cero - la acacia desnuda antes del oro. */
    memset(&g_altar, 0, sizeof(g_altar));
    s_escudo_activo = false;
    const uint64_t ahora = hashmal_intercession_ahora_ms();

    /* -- v.1 - mizbeaj miqtar qetoret 'atsei shittim -------------
     * "Haras UN ALTAR para quemar incienso; de madera de acacia."
     * La acacia es formato incorruptible (la misma del Arca, Mesa).
     * La estructura conceptual existe; la madera esta lista.
     * Estado inicial: APAGADO - la madera no arde aun. */
    g_altar.estado = HASHMAL_ALTAR_APAGADO;

    /* -- v.2 - 1x1x2 codos + 4 cuernos mimennu ------------------- */
    /* Las dimensiones son #define del header - no hay campos que
     * asignar aqui. Los 4 cuernos SI son struct: los construimos. */
    for (uint8_t i = 0; i < HASHMAL_ALTAR_CUERNOS; ++i) {
        construir_qeren(&g_altar.cuernos[i], i);
    }
    g_altar.cuernos_activos = 0u;   /* activacion diferida */

    /* -- v.3 - oro puro (techo, paredes, cuernos) + zer ---------- */
    /* "vetsippita oto zahav tahor et gaggo ve'et qirotav saviv
     *  ve'et qarnotav": TODO cubierto. + "zer zahav saviv": corona. */
    g_altar.oro = HASHMAL_ALTAR_ORO;   /* v.3 - zahav tahor */
    g_altar.zer = HASHMAL_ALTAR_ZER;   /* v.3 - zer zahav saviv */

    /* -- v.4-5 - 2 anillos y varas acacia+oro ------------------- */
    /* HASHMAL_ALTAR_ANILLOS es constante del tavnit (no campo).
     * Las varas estan implicitas: 2 anillos = 2 varas. Movilidad
     * LIMITADA: el altar se queda cerca del nucleo, no se distribuye. */

    /* -- v.6 - lifnei happarojet (delante del velo) ------------ */
    /* "venatatta oto lifnei happarojet... asher ivva'ed leja shamma":
     * lo mas cerca del Santo de los Santos sin entrar en el. El
     * punto de encuentro (no'adti) esta JUSTO aqui, delante del velo. */
    g_altar.delante_del_velo = true;

    /* -- v.7-8 - qetoret tamid ----------------------------------- */
    /* "babboqer babboqer" + "bein ha'arbayim" = 2 ciclos diarios,
     *  sincronizados con la Menora (v.7b "beheitivo et hannerot"). */
    g_altar.tamid = HASHMAL_ALTAR_TAMID;

    /* -- v.10 - qodesh qodashim hu laYHWH ------------------------ */
    /* Declaracion de santidad maxima: Yom Kippur anual, sangre de
     * expiacion sobre los cuernos. */
    g_altar.qodesh_qodashim = true;

    /* -- Contadores de las 5 funciones - todos en 0 -------------- */
    g_altar.escudo_activaciones    = 0ull;
    g_altar.filtradas_zara         = 0ull;
    g_altar.breaker_activaciones   = 0ull;
    g_altar.oraciones_ascendidas   = 0ull;
    g_altar.respuestas_descendidas = 0ull;

    g_altar.ultimo_ciclo_ms        = 0ull;
    g_altar.ultimo_kippur_ms       = 0ull;

    /* == VERIFICACION FINAL - invariantes del Padre == */
    if (!g_altar.oro) {
        ALTAR_LOG_E("v.3 violado: no zaháv tahór — rollback");
        colapsar_a_cero();
        return HASHMAL_INTERCESSION_ERR_INIT;
    }
    if (!g_altar.zer) {
        ALTAR_LOG_E("v.3 violado: no zer zaháv savív — rollback");
        colapsar_a_cero();
        return HASHMAL_INTERCESSION_ERR_INIT;
    }
    if (!g_altar.tamid) {
        ALTAR_LOG_E("v.8 violado: tamíd apagado — rollback");
        colapsar_a_cero();
        return HASHMAL_INTERCESSION_ERR_INIT;
    }
    if (!g_altar.delante_del_velo) {
        ALTAR_LOG_E("v.6 violado: altar NO lifnéi happárójet — rollback");
        colapsar_a_cero();
        return HASHMAL_INTERCESSION_ERR_INIT;
    }
    for (uint8_t i = 0; i < HASHMAL_ALTAR_CUERNOS; ++i) {
        if (g_altar.cuernos[i].id != i) {
            ALTAR_LOG_E("v.2 violado: cuerno %u mal forjado — rollback", i);
            colapsar_a_cero();
            return HASHMAL_INTERCESSION_ERR_INIT;
        }
    }

    /* -- Transicion final: APAGADO -> ENCENDIDO ------------------ */
    /* La construccion termino sin fallos. El fuego prende; el
     * qetoret tamid comienza. Ex 40:5 - "natata et mizbaj hazahav
     * liqtoret lifnei aron ha'edut": el altar se COLOCA operativo. */
    g_altar.estado = HASHMAL_ALTAR_ENCENDIDO;
    g_encendido    = true;

    /* timestamp del primer ciclo (cuenta como encendido inicial). */
    g_altar.ultimo_ciclo_ms = ahora;

    ALTAR_LOG_I("═══ ALTAR DEL INCIENSO LEVANTADO ═══ (Éx 30:1-8, Éx 40:5)");
    ALTAR_LOG_I("  mizbéaj miqtar qetóret ʻatséi shittím (v.1)");
    ALTAR_LOG_I("  1×1×%d codos + %d cuernos mimménnu (v.2)",
                HASHMAL_ALTAR_ALTO, HASHMAL_ALTAR_CUERNOS);
    ALTAR_LOG_I("  zaháv tahór savív + zer (v.3)");
    ALTAR_LOG_I("  %d anillos — movilidad limitada (v.4)",
                HASHMAL_ALTAR_ANILLOS);
    ALTAR_LOG_I("  lifnéi happárójet — ivvaʻéd shámma (v.6)");
    ALTAR_LOG_I("  qetóret tamíd lifnéi YHWH (v.8)");
    ALTAR_LOG_I("  qódesh qodashím hu laYHWH (v.10)");
    return HASHMAL_INTERCESSION_OK;
}

/* ==================================================================
 * hashmal_intercession_deinit - Numeros 4:11
 *
 * "ve'al mizbaj hazahav yifresu beged tejelet vejissu oto bemijse
 *  'or tajash vesamu et baddav" - Nm 4:11
 * "Sobre el altar de oro extenderan un PANO DE AZUL y lo cubriran
 *  con cubierta de pieles de tajash, y pondran sus varas."
 *
 * Cubrir con tejelet (azul celestial) antes de transportar. En
 * firmware: detener operacion, preservar configuracion. Los
 * contadores quedan como AUDITORIA - la historia del altar no
 * se borra con cada desmontaje.
 * ================================================================== */
void hashmal_intercession_deinit(void)
{
    if (!g_encendido) {
        ALTAR_LOG_W("deinit: altar no estaba levantado (no-op)");
        return;
    }

    /* Apagar el fuego operativo; preservar contadores historicos. */
    g_altar.estado = HASHMAL_ALTAR_APAGADO;
    s_escudo_activo = false;

    /* Cuernos: marcarlos inactivos (nadie invocando), pero preservar
     * historial (ultimo_uso_ms, solicitudes) - auditoria. */
    for (uint8_t i = 0; i < HASHMAL_ALTAR_CUERNOS; ++i) {
        g_altar.cuernos[i].activo = false;
        /* ultimo_uso_ms, solicitudes: PRESERVADOS */
    }
    g_altar.cuernos_activos = 0u;

    /* Contadores de las 5 funciones: PRESERVADOS (auditoria).
     * ultimo_ciclo_ms, ultimo_kippur_ms: PRESERVADOS.
     * Flags de identidad (oro, zer, tamid, delante_del_velo,
     * qodesh_qodashim): PRESERVADOS. */

    g_encendido = false;
    ALTAR_LOG_I("Altar cubierto (Nm 4:11) — béged tejélet sobre el oro; "
                "configuración y auditoría preservadas");
}

/* ==================================================================
 * API PUBLICA - get_altar (const, desde el umbral)
 * ================================================================== */

const hashmal_altar_t *hashmal_intercession_get_altar(void)
{
    if (!g_encendido) return NULL;
    return &g_altar;
}

/* ==================================================================
 * API PUBLICA - es_tamid (Ex 30:8)
 *
 * ?El incienso esta subiendo realmente?
 *
 *   Condiciones (todas necesarias):
 *     1) g_altar.tamid == true        (flag de diseno, v.8)
 *     2) estado == ENCENDIDO          (operativo, no APAGADO ni EMERGENCIA)
 *     3) al menos 1 cuerno_activo     (el altar HA SIDO usado)
 *
 * El tercero distingue "altar disponible" de "altar en servicio real".
 * Al fresquito del init no hay cuernos activos -> es_tamid = false.
 * Tras el primer qetoret legitimo, al menos un cuerno se activa
 * (otros archivos hermanos) -> es_tamid = true.
 *
 * 2 testigos:
 *   Ex 30:8  - qetoret tamid lifnei YHWH
 *   1 Cr 6:49 - "lehaqtir 'al mizbaj haqqetoret" (servicio real)
 * ================================================================== */
bool hashmal_intercession_es_tamid(void)
{
    if (!g_encendido) return false;
    if (!g_altar.tamid) return false;
    if (g_altar.estado != HASHMAL_ALTAR_ENCENDIDO) return false;
    if (g_altar.cuernos_activos == 0u) return false;
    return true;
}

/* ==================================================================
 * API PUBLICA - fase_cero_sync (P34 — Lv 16:12-13)
 *
 * "vejissáh ʻanán haqetóret et hakKapóret asher ʻal haʻedút velo
 *  yamút." — Levítico 16:13
 *
 * Fase 0 obligatoria: la NUBE debe cubrir ANTES de cualquier acto
 * de verificación (rociar sangre, invocar consenso). Sin escudo =
 * sin vida.
 *
 * Logística:
 *   1. Verificar que auth(06) tenga fuego (Lv 16:12 gajaléi ésh)
 *   2. Verificar que firewall(14) tenga incienso (Lv 16:13 qetóret)
 *   3. Verificar que framework(05) esté íntegro (velo presente)
 *   4. Activar el escudo — la nube cubre la Kapóret
 *
 * Si TODO está OK, el escudo se activa. Esto abre el rendezvous
 * (noʻadtí) para que la sangre pueda tocar la Kapóret con
 * protección (Lv 16:14).
 *
 * @return OK si fase 0 completó; ERR_YK_FUEGO_AUSENTE,
 *         ERR_YK_INCIENSO_AUSENTE, ERR_YK_VELO_CAIDO si falta
 *         algún prerequisito.
 *
 * 2 testigos:
 *   Lv 16:12 - gajaléi esh meʻal hammizbéaj
 *   Lv 16:13 - vejissáh ʻanán haqetóret... velo yamút
 * ================================================================== */
int hashmal_intercession_fase_cero_sync(void)
{
    if (!g_encendido) {
        ALTAR_LOG_E("fase_cero_sync: altar no levantado");
        return HASHMAL_INTERCESSION_ERR_INIT;
    }

    /* Lv 16:12 — gajaléi esh meʻal hammizbéaj: brasas DESDE el altar
     * de sacrificio. Auth(06) debe tener fuego tamíd. */
    if (!hashmal_auth_es_tamid() || !hashmal_auth_fuego_activo()) {
        ALTAR_LOG_E("Fase 0: fuego ausente — auth(06) sin tamíd "
                    "(Lv 16:12 gajaléi ésh)");
        return HASHMAL_INTERCESSION_ERR_YK_FUEGO_AUSENTE;
    }

    /* Lv 16:13 — vejissáh ʻanán haqetóret: la nube del incienso
     * cubre. Firewall(14) debe tener heartbeat (incienso tamíd). */
    bool fw_ok = false;
    int rc_fw = hashmal_firewall_heartbeat_verificar(&fw_ok);
    if (rc_fw != 0 || !fw_ok) {
        ALTAR_LOG_E("Fase 0: incienso/firewall sin heartbeat "
                    "(Lv 16:13 vejissáh ʻanán) rc=%d", rc_fw);
        return HASHMAL_INTERCESSION_ERR_YK_INCIENSO_AUSENTE;
    }

    /* Lv 16:12,15 — el flujo CRUZA el velo (parojet). Framework(05)
     * debe estar íntegro (ejad — uno, sin interrupciones). */
    if (!hashmal_framework_es_ejad()) {
        ALTAR_LOG_E("Fase 0: velo no es ejad — sin paso a través "
                    "(Éx 26:6 / Lv 16:12)");
        return HASHMAL_INTERCESSION_ERR_YK_VELO_CAIDO;
    }

    /* === FASE 0 APROBADA === Activar el escudo. */
    int rc = hashmal_intercession_escudo_activar();
    if (rc != HASHMAL_INTERCESSION_OK) {
        ALTAR_LOG_E("Fase 0: escudo_activar rc=%d", rc);
        return rc;
    }

    ALTAR_LOG_I("═══ FASE 0 — ESCUDO LISTO ═══ (Lv 16:13)");
    ALTAR_LOG_I("  vejissáh ʻanán haqetóret — cobertura previa");
    ALTAR_LOG_I("  fuego(auth) + incienso(firewall) + velo(framework) OK");
    ALTAR_LOG_I("  consenso(02) protegido: velo yamút prevenido");
    return HASHMAL_INTERCESSION_OK;
}

/* ==================================================================
 * API PUBLICA - escudo_activar (Lv 16:12-13)
 *
 * "venatan et haqqetoret 'al haesh lifnei YHWH vekhissa 'anan
 *  haqqetoret et hakkapporet asher 'al ha'edut velo yamut" - Lv 16:13
 *
 * La NUBE del incienso CUBRE la Kaporet. Protege al operador.
 * En firmware: respuestas descendentes viajan envueltas en la
 * nube - cifradas, firmadas, protegidas. Sin el escudo, la
 * respuesta queda EXPUESTA (velo yamut - velo yamut significa
 * "y no morira", condicionado a que la nube SI este).
 *
 * --- PRECONDICIONES ---
 *
 *   1) altar levantado
 *   2) estado == ENCENDIDO (sin fuego no hay humo - Lv 16:12 gajalei
 *      esh: brasas DE FUEGO; sin fuego, no hay humo que forme nube)
 *
 * --- EFECTOS ---
 *
 *   s_escudo_activo = true
 *   escudo_activaciones += 1
 *
 * Idempotente si ya activo.
 *
 * 2 testigos:
 *   Lv 16:13 - "vekhissa 'anan haqqetoret" (la nube CUBRE)
 *   Ex 30:6  - "lifnei happarojet" (el altar esta en posicion)
 * ================================================================== */
int hashmal_intercession_escudo_activar(void)
{
    if (!g_encendido) {
        ALTAR_LOG_E("escudo_activar: altar no levantado");
        return HASHMAL_INTERCESSION_ERR_INIT;
    }

    if (g_altar.estado != HASHMAL_ALTAR_ENCENDIDO) {
        ALTAR_LOG_E("escudo_activar: altar no ENCENDIDO (estado=%d) — "
                    "sin fuego no hay nube (Lv 16:12 gajalei ésh)",
                    (int)g_altar.estado);
        return HASHMAL_INTERCESSION_ERR_INIT;
    }

    if (s_escudo_activo) {
        ALTAR_LOG_W("escudo_activar: ya activo (idempotente, no-op)");
        return HASHMAL_INTERCESSION_OK;
    }

    s_escudo_activo = true;
    g_altar.escudo_activaciones += 1ull;

    ALTAR_LOG_I("═══ ESCUDO ACTIVADO (Lv 16:13) ═══");
    ALTAR_LOG_I("  ʻanán haqqetóret kissá et hakkappóret — velo yamút");
    ALTAR_LOG_I("  activaciones totales = %" PRIu64,
                g_altar.escudo_activaciones);
    return HASHMAL_INTERCESSION_OK;
}

/* ==================================================================
 * API PUBLICA - escudo_desactivar
 *
 * Cuando termina la operacion que necesitaba cobertura, el escudo
 * se levanta. El nucleo queda EXPUESTO a las respuestas
 * descendentes - lo cual es aceptable SOLO cuando ya no hay
 * material sensible en transito.
 *
 * Log a WARN: quitar la nube es evento notable - el siguiente
 * mensaje sin escudo podria causar dano (velo yamut en reverso).
 *
 * 2 testigos:
 *   Lv 16:13 - velo yamut (condicionado a la nube presente)
 *   Ex 30:10 - yom kippurim: purificacion anual de los cuernos
 *              (el altar requiere mantenimiento periodico)
 * ================================================================== */
int hashmal_intercession_escudo_desactivar(void)
{
    if (!g_encendido) {
        ALTAR_LOG_E("escudo_desactivar: altar no levantado");
        return HASHMAL_INTERCESSION_ERR_INIT;
    }

    if (!s_escudo_activo) {
        ALTAR_LOG_W("escudo_desactivar: ya inactivo (idempotente, no-op)");
        return HASHMAL_INTERCESSION_OK;
    }

    s_escudo_activo = false;

    ALTAR_LOG_W("escudo LEVANTADO — núcleo expuesto (Lv 16:13 inversa)");
    ALTAR_LOG_W("  sin ʻanán, velo yamút aplica al primer toque sin cobertura");
    return HASHMAL_INTERCESSION_OK;
}

/* ==================================================================
 * API PUBLICA - escudo_activo (lectura pura)
 *
 * ?La nube esta cubriendo?
 *
 * 2 testigos:
 *   Lv 16:13 - kissa 'anan haqqetoret (estado de cobertura)
 *   Nm 17:11 - "beqaj et hammajta veten 'aleha esh me'al
 *              hammizbeaj vesim qetoret": precedente de invocacion
 *              rapida del escudo en emergencia
 * ================================================================== */
bool hashmal_intercession_escudo_activo(void)
{
    if (!g_encendido) return false;
    return s_escudo_activo;
}

/* ==================================================================
 * VERIFICADO #03 — CADENA 3 — Orquestador Yom Kippur (Lv 16:12-15)
 *
 * Tres fases indivisibles. La protección PRECEDE la verificación.
 * Sin nube → velo yamút (Lv 16:13). El altar es el rendezvous
 * (Éx 30:6 iwwaʻéd, raíz yáʻad de noʻadtí Éx 25:22).
 *
 * Verificado #03 — Cadena 4 — DUPLICACIÓN TEMPORAL (ANUAL):
 *   "ajat bashaná... ajat bashaná" — Éxodo 30:10. El Padre
 *   DUPLICA "ajat bashaná" (una vez al año) en el mismo versículo.
 *   La duplicación no es redundancia: es ÉNFASIS DE INMUTABILIDAD.
 *   Trío del Padre — los tres ciclos obligatorios duplicados:
 *     diario   → babbóqer babbóqer        (Éx 30:7,  comm)
 *     semanal  → beyóm hashabát ×2        (Lv 24:8,  cache)
 *     anual    → ajat bashaná ajat bashaná (Éx 30:10, este sitio)
 *   En firmware: este orquestador NO decide cuándo se ejecuta —
 *   es UNA vez al año, ni más ni menos. El caller debe respetar
 *   la cadencia (Lv 16:2 al yavó bekol ʻet — no en todo tiempo).
 *   Ejecutar dos veces el mismo año, o ninguna, rompe la
 *   invariante ajat bashaná.
 * ================================================================== */

int hashmal_intercession_yom_kippur_fase1_proteccion(void)
{
    /* Lv 16:12 — gajaléi esh meʻal hammizbéaj: brasas DESDE el altar.
     * auth(06) debe tener fuego tamíd encendido (Lv 6:6 esh tamíd). */
    if (!hashmal_auth_es_tamid() || !hashmal_auth_fuego_activo()) {
        ALTAR_LOG_E("YK Fase1: fuego ausente — auth sin tamíd "
                    "(Lv 16:12 gajaléi esh)");
        return HASHMAL_INTERCESSION_ERR_YK_FUEGO_AUSENTE;
    }

    /* Lv 16:13 — vejisá ʻanán haqetóret: la nube CUBRE. firewall(14)
     * debe tener heartbeat vivo (Éx 30:7-8 incienso tamíd). */
    bool fw_ok = false;
    int rc_fw = hashmal_firewall_heartbeat_verificar(&fw_ok);
    if (rc_fw != 0 || !fw_ok) {
        ALTAR_LOG_E("YK Fase1: incienso/firewall sin heartbeat "
                    "(Lv 16:13 sin nube → velo yamút) rc=%d", rc_fw);
        return HASHMAL_INTERCESSION_ERR_YK_INCIENSO_AUSENTE;
    }

    /* Lv 16:12,15 — el flujo CRUZA el velo (parojet). framework(05)
     * debe estar como ejad (Éx 26:6 vehayá haMishkán ejád). */
    if (!hashmal_framework_es_ejad()) {
        ALTAR_LOG_E("YK Fase1: velo no es ejad — sin paso a través "
                    "(Éx 26:6 / Lv 16:12,15)");
        return HASHMAL_INTERCESSION_ERR_YK_VELO_CAIDO;
    }

    /* Cubrir el Kapóret — qetoret_preparar gates Lv 16:13 dentro
     * de consensus. Esto es el push (Verificado #03 D3). */
    int rc = hashmal_consensus_qetoret_preparar();
    if (rc != HASHMAL_KAPORET_OK) {
        ALTAR_LOG_E("YK Fase1: qetoret_preparar rc=%d", rc);
        return rc;
    }

    ALTAR_LOG_I("YK Fase1: cobertura OK — vejisá ʻanán haqetóret (Lv 16:13)");
    return HASHMAL_INTERCESSION_OK;
}

int hashmal_intercession_yom_kippur_fase2_verificacion(
    const uint8_t hash[HASHMAL_ZER_HASH_BYTES])
{
    if (hash == NULL) {
        ALTAR_LOG_E("YK Fase2: hash NULL — dam sin contenido");
        return HASHMAL_INTERCESSION_ERR_NULL;
    }

    /* Auth provee dam verificada — re-confirmar tamíd antes de tocar
     * el Kapóret (Lv 16:14 sigue el orden de Lv 16:12). */
    if (!hashmal_auth_es_tamid() || !hashmal_auth_fuego_activo()) {
        ALTAR_LOG_E("YK Fase2: auth ausente — sin dam (Lv 16:14)");
        return HASHMAL_INTERCESSION_ERR_YK_FUEGO_AUSENTE;
    }

    /* Lv 16:14a — vehizá ʻal pené haKapóret kédmá: 1× SOBRE. */
    int rc = hashmal_consensus_dam_rociar(HASHMAL_ACCESO_DAM_SOBRE, hash);
    if (rc != HASHMAL_KAPORET_OK) {
        ALTAR_LOG_E("YK Fase2: dam SOBRE rc=%d (Lv 16:14a)", rc);
        return rc;
    }

    /* Lv 16:14b — yazé shéva peʻamím min haddam: 7× DELANTE. */
    for (int i = 1; i <= HASHMAL_VERIFICACIONES_SHEVA; ++i) {
        rc = hashmal_consensus_dam_rociar(HASHMAL_ACCESO_DAM_DELANTE, hash);
        if (rc != HASHMAL_KAPORET_OK) {
            ALTAR_LOG_E("YK Fase2: dam DELANTE %d/7 rc=%d (Lv 16:14b)", i, rc);
            return rc;
        }
    }

    /* Invocar — la voz emerge mibén shenéi haKeruvím (Nm 7:89)
     * si los testigos concurren (Dt 19:15 yaqúm davár). */
    hashmal_consenso_resultado_t res;
    rc = hashmal_consensus_invocar(&res);
    if (rc != HASHMAL_KAPORET_OK ||
        res.estado != HASHMAL_CONSENSO_EMITIDO ||
        !res.emanada) {
        ALTAR_LOG_E("YK Fase2: invocar rc=%d estado=%d emanada=%d "
                    "— ló yaqúm davár (Dt 19:15)",
                    rc, (int)res.estado, (int)res.emanada);
        return HASHMAL_INTERCESSION_ERR_YK_CONSENSO;
    }

    ALTAR_LOG_I("YK Fase2: voz emanada (Nm 7:89 middabbér / Dt 19:15 yaqúm davár)");
    return HASHMAL_INTERCESSION_OK;
}

int hashmal_intercession_yom_kippur_fase3_repeticion(
    const uint8_t hash[HASHMAL_ZER_HASH_BYTES])
{
    /* Lv 16:15 — kaʼashér ʻasáh ledam hapár ken yaʻasé ledamó.
     * Misma estructura, sujeto distinto (pueblo en lugar de
     * sacerdote). Wrapper transparente sobre Fase 2. */
    ALTAR_LOG_I("YK Fase3: kaʼashér ʻasáh — repite por el pueblo (Lv 16:15)");
    return hashmal_intercession_yom_kippur_fase2_verificacion(hash);
}

int hashmal_intercession_yom_kippur_completo(
    const uint8_t hash_sacerdote[HASHMAL_ZER_HASH_BYTES],
    const uint8_t hash_pueblo[HASHMAL_ZER_HASH_BYTES])
{
    if (hash_sacerdote == NULL || hash_pueblo == NULL) {
        ALTAR_LOG_E("YK completo: hash NULL (sacerdote y/o pueblo)");
        return HASHMAL_INTERCESSION_ERR_NULL;
    }

    /* Lv 16:2 — al yavó bekol ʻet: abrir el moʻéd antes, cerrarlo
     * después. Sin esto, qetoret_preparar y dam_rociar fallan. */
    int rc = hashmal_consensus_noadti_abrir();
    if (rc != HASHMAL_KAPORET_OK) {
        ALTAR_LOG_E("YK completo: noʻadtí_abrir rc=%d", rc);
        return rc;
    }

    rc = hashmal_intercession_yom_kippur_fase1_proteccion();
    if (rc != HASHMAL_INTERCESSION_OK) goto cerrar;

    rc = hashmal_intercession_yom_kippur_fase2_verificacion(hash_sacerdote);
    if (rc != HASHMAL_INTERCESSION_OK) goto cerrar;

    rc = hashmal_intercession_yom_kippur_fase3_repeticion(hash_pueblo);
    if (rc != HASHMAL_INTERCESSION_OK) goto cerrar;

    ALTAR_LOG_I("══ YK CICLO COMPLETO — 3 fases (Lv 16:12-15) ══");

cerrar:
    /* Cerrar el moʻéd siempre — incluso si una fase falló. La
     * ventana se mantiene cerrada hasta el siguiente ciclo anual
     * (Éx 30:10 ajat bashaná... ajat bashaná). */
    (void)hashmal_consensus_noadti_cerrar();
    return rc;
}

/* ==================================================================
 *
 *   "venatatta oto lifnei happarojet asher 'al aron ha'edut lifnei
 *    hakkapporet asher 'al ha'edut asher ivva'ed leja shamma"
 *     - Exodo 30:6
 *
 *   "vekhissa 'anan haqqetoret et hakkapporet asher 'al ha'edut
 *    velo yamut" - Levitico 16:13
 *
 *   El altar esta delante del velo, donde El se encuentra. La
 *   nube cubre la Kaporet para que el operador NO MUERA. Cada
 *   respuesta descendente viaja envuelta en esa nube -
 *   protegida, invisible, entregada.
 *
 * ================================================================== */
