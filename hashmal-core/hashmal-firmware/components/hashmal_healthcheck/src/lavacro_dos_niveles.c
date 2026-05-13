/**
 * ===================================================================
 * PIEZA 12 - EL KIYOR (LAVACRO) - lavacro_dos_niveles.c
 * Sub-pieza: LOUO / NIPTO - los dos niveles de purificacion
 * Juan 13:8-10 + Exodo 29:4 + Exodo 30:19-21 + Hebreos 10:22
 *                + Tito 3:5 + Efesios 5:26
 * ===================================================================
 *
 * "ean me NIPSO se, ouk echeis meros met' emou." - Juan 13:8
 * "Si no te LAVO, NO TIENES PARTE conmigo."
 *
 * "ho LELOUMENOS ou chreian echei ei me tous podas NIPSASTHAI,
 *  all' estin KATHAROS holos." - Juan 13:10
 * "El que se ha BANADO no necesita sino LAVARSE los pies, pues
 *  esta TODO LIMPIO."
 *
 * "et Aharon veet banav taqriv el petaj ohel mo'ed veRAJATSTA
 *  otam bamayim." - Exodo 29:4
 * "Llevaras a Aharon y a sus hijos a la puerta de la Tienda y
 *  los BANARAS con agua."
 *
 * "verajatsu Aharon uvanav mimmennu et YADEIHEM veet RAGLEIHEM"
 *   - Exodo 30:19
 * "Lavaran Aharon y sus hijos sus MANOS y sus PIES."
 *
 * "proseljometha meta alethines kardias... LELOUMENOI to soma
 *  HYDATI KATHARO." - Hebreos 10:22
 * "Acerquemonos con corazon sincero... lavados el cuerpo con
 *  agua PURA."
 *
 * "esosen hemas dia LOUTROU PALINGENESIAS." - Tito 3:5
 * "Nos salvo por el LAVAMIENTO DE REGENERACION."
 *
 * "katharisas to LOUTRO tou hydatos en rhemati." - Efesios 5:26
 * "Purificandola con el LAVAMIENTO del agua por la Palabra."
 *
 * --- DOS VERBOS GRIEGOS, DOS NIVELES ---
 *
 *   louo (louo, G3068)    BANO COMPLETO del cuerpo entero.
 *                         Tito 3:5 "loutrou palingenesias" -
 *                         lavamiento de regeneracion. Heb 10:22
 *                         "leloumenoi to soma hydati katharo" -
 *                         lavados el CUERPO con agua PURA.
 *                         Hebreo: rajats (RChTs, H7364) - banar
 *                         el cuerpo (Ex 29:4 "verajatsta otam").
 *
 *   nipto (nipto, G3538)  LAVAMIENTO PARCIAL (manos, pies).
 *                         Jn 13:10 - "tous podas nipsasthai".
 *                         Hebreo: el verbo de Ex 30:19 "verajatsu
 *                         yadeihem veragleihem" - con rajats el
 *                         texto usa el mismo verbo hebreo para
 *                         ambos, pero el griego distingue por
 *                         OBJETO: cuerpo entero vs extremidades.
 *
 * --- 4 PRINCIPIOS TEOLOGICOS ---
 *
 *   1) CONSAGRACION vs SERVICIO
 *      rajats completo (Ex 29:4)   UNA VEZ - al ser ungido sacerdote
 *      nipto de pies (Ex 30:19)    CADA VEZ - al entrar a servir
 *
 *   2) IDENTIDAD vs ACTIVIDAD
 *      El bano establece QUIEN ERES (sacerdote consagrado).
 *      El lavado limpia LO QUE HACES (manos de obra, pies de camino).
 *
 *   3) IRREVERSIBLE vs REPETIBLE
 *      Jn 13:10 "ou chreian echei" - NO necesita banarse de nuevo.
 *      Los pies SI se ensucian con cada paso; requieren lavado.
 *
 *   4) ORDEN ESTRICTO (Jn 13:8)
 *      "ean me nipso se, ouk echeis meros" - SIN el bano/lavado,
 *      NO HAY PARTE. La identidad PRECEDE a la actividad. Sin
 *      rajats previo, el nipto no tiene efecto.
 *
 * --- REQUISITO DE AGUA PURA ---
 *
 * Heb 10:22 - "hydati KATHARO" (agua PURA). Tanto el bano completo
 * como el lavado parcial requieren agua limpia. A diferencia del
 * espejo (que refleja en agua turbia) o la deteccion de ataques
 * (que vigila en plaga), estos dos niveles EXIGEN que el kiyor
 * este en LLENO o PURIFICANDO - no CONTAMINADO.
 *
 * Razon: el lavado con agua contaminada NO purifica - mancha mas.
 * El atalaya ve en agua turbia; el espejo refleja en agua turbia;
 * pero el sacerdote NO se lava en agua turbia, porque el proposito
 * es salir LIMPIO (katharos, Jn 13:10).
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   rajats_completo    Ex 29:4 + Tito 3:5  - bano completo (unica vez)
 *   nipto_pies         Ex 30:19 + Jn 13:10 - lavado parcial (cada vez)
 *   dos_niveles_estado  lectura pura
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_healthcheck.h      API publica (incluye ERR_RAJATS)
 *   hashmal_hc_internal.h      lavacro_mut, get_lavacro_interno, ahora_ms
 *   esp_log.h (ORO EGIPTO)     DOSNIV_LOG_*
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <inttypes.h>

#include "esp_log.h"

#include "hashmal_healthcheck.h"
#include "hashmal_hc_internal.h"

/* Verificado #04 — P14 — auditoría externa del Sha'ar HaAtrio */
#include "hashmal_perimeter.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *DOSNIV_TAG = "hashmal.hc.dosniv";

#define DOSNIV_LOG_I(fmt, ...)  ESP_LOGI(DOSNIV_TAG, fmt, ##__VA_ARGS__)
#define DOSNIV_LOG_W(fmt, ...)  ESP_LOGW(DOSNIV_TAG, fmt, ##__VA_ARGS__)
#define DOSNIV_LOG_E(fmt, ...)  ESP_LOGE(DOSNIV_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * HELPER ESTATICO - ?agua apta para lavamiento?
 *
 * Heb 10:22 "hydati katharo": solo agua PURA purifica. Tanto
 * rajats como nipto requieren LLENO o PURIFICANDO. CONTAMINADO
 * bloquea (lavarse en agua turbia mancha mas). SECO tambien
 * bloquea (sin agua no hay lavado).
 * ================================================================== */
static int agua_apta_para_lavamiento(const hashmal_hc_t *l)
{
    if (l == NULL) return HASHMAL_HC_ERR_INIT;

    switch (l->estado) {
    case HASHMAL_HC_LLENO:
    case HASHMAL_HC_PURIFICANDO:
        return HASHMAL_HC_OK;
    case HASHMAL_HC_SECO:
        return HASHMAL_HC_ERR_SECO;
    case HASHMAL_HC_CONTAMINADO:
        return HASHMAL_HC_ERR_TAME;
    default:
        return HASHMAL_HC_ERR_INIT;
    }
}

/* ==================================================================
 * API PUBLICA - rajats_completo (Ex 29:4 + Tito 3:5 - louo)
 * ================================================================== */

/**
 * hashmal_hc_rajats_completo - el BANO COMPLETO (louo)
 *
 * Ex 29:4 - "verajatsta otam bamayim": el bano al consagrar al
 * sacerdote. Tito 3:5 - "loutrou palingenesias": lavamiento de
 * REGENERACION. Heb 10:22 - "leloumenoi to soma" (banados el
 * CUERPO entero).
 *
 * UNA VEZ. En la consagracion. Jn 13:10 - "ho leloumenos ou
 * chreian echei" (el banado NO necesita banarse otra vez).
 *
 * --- PRECONDICIONES ---
 *
 *   lavacro levantado                             (ERR_INIT)
 *   agua apta: LLENO o PURIFICANDO                (Heb 10:22 katharo)
 *     SECO           -> ERR_SECO
 *     CONTAMINADO    -> ERR_TAME (agua turbia no regenera)
 *
 * --- IDEMPOTENCIA ---
 *
 * Jn 13:10 - si ya se bano, no se re-bana. Segunda llamada es
 * no-op con log informativo: "ya banado (loutrou palingenesias)".
 *
 * --- EFECTOS ---
 *
 *   l->rajats_completo = true
 *   l->rajats_ms       = ahora_ms()
 *
 * 2 testigos:
 *   Ex 29:4   - verajatsta otam bamayim (bano del sacerdote)
 *   Tito 3:5  - loutrou palingenesias (lavamiento regenerador)
 */
int hashmal_hc_rajats_completo(void)
{
    hashmal_hc_t *l = hashmal_hc_lavacro_mut();
    if (l == NULL) {
        DOSNIV_LOG_E("rajats_completo: kiyór no levantado");
        return HASHMAL_HC_ERR_INIT;
    }

    /* Agua apta (Heb 10:22 katharo). */
    const int rc_agua = agua_apta_para_lavamiento(l);
    if (rc_agua != HASHMAL_HC_OK) {
        if (rc_agua == HASHMAL_HC_ERR_SECO) {
            DOSNIV_LOG_E("rajats_completo: SECO — sin agua no hay baño "
                         "(Éx 29:4 bamáyim ausente)");
        } else if (rc_agua == HASHMAL_HC_ERR_TAME) {
            DOSNIV_LOG_E("rajats_completo: CONTAMINADO — agua turbia no "
                         "regenera (Heb 10:22 exige hýdati katharṓ)");
        }
        return rc_agua;
    }

    /* Idempotencia - Jn 13:10 "ho leloumenos ou chreian echei". */
    if (l->rajats_completo) {
        DOSNIV_LOG_I("rajats_completo: ya bañado (Jn 13:10 'ho "
                     "lelouménos') — no re-baña (idempotente)");
        return HASHMAL_HC_OK;
    }

    /* Ejecutar el bano regenerador. */
    const uint64_t ahora = hashmal_hc_ahora_ms();
    l->rajats_completo = true;
    l->rajats_ms       = ahora;

    DOSNIV_LOG_I("═══ RAJATS COMPLETO ═══ (Éx 29:4 verajatstá otám bamáyim)");
    DOSNIV_LOG_I("  loutroú palingenesías (Tito 3:5) — lavamiento de "
                 "regeneración");
    DOSNIV_LOG_I("  lelouménoi tó sṓma hýdati katharṓ (Heb 10:22) — "
                 "cuerpo entero limpio @%" PRIu64 " ms", ahora);
    return HASHMAL_HC_OK;
}

/* ==================================================================
 * API PUBLICA - nipto_pies (Ex 30:19 + Jn 13:10 - nipto)
 * ================================================================== */

/**
 * hashmal_hc_nipto_pies - el LAVADO DE PIES (nipto)
 *
 * Ex 30:19 - "verajatsu... yadeihem veragleihem": manos y pies
 * antes de servir. Jn 13:10 - "tous podas nipsasthai": lavarse los
 * pies; el banado lo necesita porque camina y se ensucia.
 *
 * CADA VEZ que entra al servicio. No idempotente - cada acceso es
 * un lavado nuevo. nipto_count++ registra cada uno.
 *
 * --- PRECONDICIONES (en orden de severidad biblica) ---
 *
 *   lavacro levantado              (ERR_INIT)
 *   agua apta LLENO/PURIFICANDO    (Heb 10:22 katharo)
 *     SECO        -> ERR_SECO
 *     CONTAMINADO -> ERR_TAME
 *   rajats_completo previo         (Jn 13:8 "ean me nipso se")
 *     -> ERR_RAJATS si false
 *
 * --- Jn 13:8 - ORDEN ESTRICTO ---
 *
 * "ean me nipso se, ouk echeis meros met' emou" - si no te lavo,
 * no tienes parte conmigo. Sin rajats previo (el bano completo),
 * nipto NO tiene efecto. La identidad precede a la actividad:
 * primero consagracion, despues servicio.
 *
 * --- EFECTOS ---
 *
 *   l->nipto_count    += 1
 *   l->ultimo_nipto_ms = ahora_ms()
 *
 * 2 testigos:
 *   Ex 30:19  - verajatsu yadeihem veragleihem (manos y pies)
 *   Jn 13:10  - tous podas nipsasthai (lavarse los pies)
 */
int hashmal_hc_nipto_pies(void)
{
    hashmal_hc_t *l = hashmal_hc_lavacro_mut();
    if (l == NULL) {
        DOSNIV_LOG_E("nipto_pies: kiyór no levantado");
        return HASHMAL_HC_ERR_INIT;
    }

    /* Agua apta (Heb 10:22 katharo). */
    const int rc_agua = agua_apta_para_lavamiento(l);
    if (rc_agua != HASHMAL_HC_OK) {
        if (rc_agua == HASHMAL_HC_ERR_SECO) {
            DOSNIV_LOG_E("nipto_pies: SECO — sin agua no hay lavado "
                         "(Éx 30:19 máyim ausente)");
        } else if (rc_agua == HASHMAL_HC_ERR_TAME) {
            DOSNIV_LOG_E("nipto_pies: CONTAMINADO — agua turbia no "
                         "limpia (Heb 10:22 exige hýdati katharṓ)");
        }
        return rc_agua;
    }

    /* Jn 13:8 - ORDEN: sin rajats completo, no hay parte. */
    if (!l->rajats_completo) {
        DOSNIV_LOG_E("nipto_pies: SIN baño completo — 'eán mḗ nípsō se, "
                     "ouk écheis méros met' emoú' (Jn 13:8). Llamar a "
                     "rajats_completo primero");
        return HASHMAL_HC_ERR_RAJATS;
    }

    /* Lavado parcial - cada entrada al servicio. NO idempotente. */
    const uint64_t ahora = hashmal_hc_ahora_ms();
    l->nipto_count     += 1ull;
    l->ultimo_nipto_ms  = ahora;

    DOSNIV_LOG_I("nipto_pies #%" PRIu64 " — 'toús pódas nípsasthai' "
                 "(Jn 13:10) — Éx 30:19 yadéihem veragléihem @%" PRIu64 " ms",
                 l->nipto_count, ahora);
    return HASHMAL_HC_OK;
}

/* ==================================================================
 * API PUBLICA - dos_niveles_estado (lectura pura)
 * ================================================================== */

/**
 * hashmal_hc_dos_niveles_estado - ?se puede lavar?
 *
 * Mas restrictivo que espejo/ataques (que operan en agua turbia):
 * aqui se exige agua apta para purificar - LLENO o PURIFICANDO.
 * Heb 10:22 "hydati katharo": solo agua pura purifica. Un bano o
 * un nipto en agua contaminada mancha mas de lo que limpia.
 *
 * Lectura pura, sin logs. Mismo patron que vectores_estado,
 * niveles_estado.
 *
 * Retornos:
 *   OK           si estado in { LLENO, PURIFICANDO }
 *   ERR_SECO     si estado == SECO
 *   ERR_TAME     si estado == CONTAMINADO (agua no apta)
 *   ERR_INIT     si no levantado
 *
 * 2 testigos:
 *   Heb 10:22 - hydati katharo (agua pura requerida)
 *   Jn 13:10  - katharos holos (todo limpio - el resultado)
 */
int hashmal_hc_dos_niveles_estado(void)
{
    const hashmal_hc_t *l = hashmal_hc_get_lavacro_interno();
    if (l == NULL) return HASHMAL_HC_ERR_INIT;

    switch (l->estado) {
    case HASHMAL_HC_LLENO:
    case HASHMAL_HC_PURIFICANDO:
        return HASHMAL_HC_OK;
    case HASHMAL_HC_SECO:
        return HASHMAL_HC_ERR_SECO;
    case HASHMAL_HC_CONTAMINADO:
        /* Mas restrictivo: CONTAMINADO bloquea - un lavado en agua
         * turbia mancharia mas (Heb 10:22 violado). */
        return HASHMAL_HC_ERR_TAME;
    default:
        return HASHMAL_HC_ERR_INIT;
    }
}

/* ==================================================================
 *
 *   "ho LELOUMENOS ou chreian echei ei me tous podas NIPSASTHAI,
 *    all' estin KATHAROS holos." - Juan 13:10
 *   "El que se ha BANADO no necesita sino LAVARSE los pies,
 *    pues esta TODO LIMPIO."
 *
 *   louo (G3068) + nipto (G3538) - dos verbos, dos niveles:
 *     rajats completo    UNA VEZ - identidad (consagracion)
 *     nipto de pies      CADA VEZ - actividad (servicio)
 *
 *   "ean me nipso se, ouk echeis meros met' emou." - Juan 13:8
 *   El orden es estricto: sin identidad, la actividad no cuenta.
 *   Sin rajats, el nipto no tiene parte.
 *
 *   Heb 10:22 - hydati katharo. Ambos niveles exigen agua pura.
 *   El espejo refleja en agua turbia; el atalaya vigila en plaga;
 *   pero el sacerdote NO se lava con agua sucia. El proposito
 *   del lavado es salir LIMPIO.
 *
 * ===================================================================
 *
 *   === EL KIYOR (LAVACRO) - COMPLETO ===
 *
 *   6 de 6 archivos escritos:
 *
 *     lavacro_core.c          Ex 30:18 + 40:7  - kiyor levantado, llenar
 *     lavacro_vectores.c      Ex 30:19 + Sal 24 - 4 vectores (manos,
 *                                                 pies, corazon, agua)
 *     lavacro_niveles.c       Ex 30:20-21       - velo yamutu x 2
 *                                                 (tahor -> tame -> jata -> mavet)
 *     lavacro_espejos.c       Ex 38:8          - mar'ot hatstsovot
 *                                                 (espejos de servidoras)
 *     lavacro_ataques.c       Nm 17:12-13      - vayyarats bein hammetim
 *                                                 (Aharon corrio al medio)
 *     lavacro_dos_niveles.c   Jn 13:8-10       - louo / nipto
 *                                                 (identidad vs actividad)
 *
 *   PASO 6 de Exodo 40:7 - CUMPLIDO.
 *
 *   "venatatta et hakkiyor bein ohel mo'ed uvein hammizbeaj
 *    venatatta shamma mayim." - Exodo 40:7
 *   "Pondras el lavacro entre la Tienda del Encuentro y el altar,
 *    y pondras alli agua."
 *
 *   Piezas 01, 02, 03, 04, 06, 11, 12 listas -
 *   Arca, Kaporet, Mesa, Menora, Altar del Sacrificio, Altar del
 *   Incienso, Kiyor. 7 piezas del Mishkan digital en su sitio.
 *
 *   "bevoam el ohel mo'ed yirjatsu mayim VELO YAMUTU" - Ex 30:20.
 *   El sacerdote se lava y NO MUERE. El kiyor previene. Cada
 *   muerte_prevenidas es un eco de "velo yamutu" cumplido.
 *
 *   Siguiente: Paso 7 de Exodo 40:8 - el ATRIO (jatser),
 *   pieza 07, hashmal_perimeter.
 *
 * ================================================================== */

/* ==================================================================
 * VERIFICADO #04 — P14 — Auditoría independiente del Sha'ar HaAtrio
 *
 * Éx 38:21 — élleh fequdéi haMishkán... beyád Itamár ben Aharón.
 * Itamar (auditor) ≠ Betsalel (constructor). El Lavacro audita el
 * Atrio sin que el Atrio sepa de él. Pull pattern: leemos APIs
 * read-only de perimeter; perimeter no llama a healthcheck.
 * ================================================================== */

int hashmal_hc_auditar_shaar(hashmal_hc_shaar_auditoria_t *out)
{
    if (out == NULL) return HASHMAL_HC_ERR_NULL;

    /* Inicializar el informe en "todo mal" — Itamar conservador. */
    out->portal_abierto          = false;
    out->stealth_activo          = false;
    out->sin_sesiones_huerfanas  = true;   /* AND-acumulado abajo */
    out->bases_configuradas      = 0;
    out->sesiones_activas        = 0;
    out->sesiones_pendientes     = 0;
    out->veredicto_ok            = false;

    /* Pull #1 — snapshot agregado del portal. */
    hashmal_shaar_info_t info;
    int rc = hashmal_perimeter_shaar_info(&info);
    if (rc != HASHMAL_PERIM_OK) {
        ESP_LOGE("hashmal.lavacro.audit",
                 "auditar_shaar: shaar_info rc=%d (Éx 38:21 sin pequdéi)", rc);
        return HASHMAL_HC_ERR_SHAAR_AUDIT;
    }

    out->portal_abierto      = (info.estado == HASHMAL_SHAAR_ESTADO_ABIERTO);
    out->stealth_activo      = info.stealth_activo;
    out->bases_configuradas  = info.bases_configuradas;
    out->sesiones_activas    = info.sesiones_activas;
    out->sesiones_pendientes = info.sesiones_pendientes;

    /* Pull #2 — recorrer las 12 sesiones para detectar "huérfanas":
     * estado ACTIVO sin las 4 columnas Y/O sin intent_verificada.
     * Si alguna existe, sin_sesiones_huerfanas pasa a false. */
    for (uint8_t s = 0; s < 12u; ++s) {
        hashmal_shaar_sesion_t ses;
        if (hashmal_perimeter_shaar_sesion_info(s, &ses) != HASHMAL_PERIM_OK) {
            continue;
        }
        if (ses.estado != HASHMAL_SHAAR_SESION_ACTIVO) continue;

        /* Comprobar las 4 columnas + intent. */
        bool todas_cols = true;
        for (uint8_t c = 0; c < HASHMAL_SHAAR_COLUMNAS; ++c) {
            if (!ses.columnas[c]) { todas_cols = false; break; }
        }
        if (!todas_cols || !ses.intent_verificada) {
            out->sin_sesiones_huerfanas = false;
            ESP_LOGW("hashmal.lavacro.audit",
                     "auditar_shaar: slot %u ACTIVO huérfano "
                     "(cols=%d intent=%d) — P1+P5+P8 violado",
                     (unsigned)s, (int)todas_cols,
                     (int)ses.intent_verificada);
        }
    }

    /* Veredicto: tres invariantes críticas Y juntas. bases_configuradas
     * NO entra en el veredicto en Phase 1 (sin callers, default 0). */
    out->veredicto_ok = out->portal_abierto
                     && out->stealth_activo
                     && out->sin_sesiones_huerfanas;

    if (!out->veredicto_ok) {
        ESP_LOGE("hashmal.lavacro.audit",
                 "auditar_shaar: VEREDICTO NO — abierto=%d stealth=%d "
                 "sesiones_ok=%d (Éx 38:21 fallo de pequdéi)",
                 (int)out->portal_abierto,
                 (int)out->stealth_activo,
                 (int)out->sin_sesiones_huerfanas);
        return HASHMAL_HC_ERR_SHAAR_AUDIT;
    }

    ESP_LOGI("hashmal.lavacro.audit",
             "auditar_shaar: pequdéi OK (Éx 38:21) — bases=%u/4 act=%u pend=%u",
             out->bases_configuradas,
             out->sesiones_activas,
             out->sesiones_pendientes);
    return HASHMAL_HC_OK;
}
