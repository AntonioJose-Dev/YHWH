/**
 * ===================================================================
 * PIEZA 10 - MILLUIM - consagracion_lockdown.c
 * Sub-pieza: VEZAR LO YOKHAL - recursos dedicados, ningun extrano come
 * Exodo 29:18,31-34 + Levitico 8:31-36 + Levitico 22:10-13
 * ===================================================================
 *
 * "ve'et kol ha'ayil taqtir hammizbejah - 'olat YHWH hu, REAJ
 *  NIJOAJ ishe laYHWH." - Exodo 29:18
 * "Quemaras TODO el carnero sobre el altar - es HOLOCAUSTO a YHWH,
 *  AROMA de REPOSO, ofrenda encendida a YHWH."
 *
 * "ve'akhelu otam asher KUPPAR bahem leMALLE et yadam leQADDESH
 *  otam - veZAR LO YOKHAL ki QODESH hem." - Exodo 29:33
 * "Comeran aquello con lo cual se hizo expiacion para llenar sus
 *  manos y consagrarlos - y el EXTRANO NO COMERA, porque son
 *  cosas SANTAS."
 *
 * "ve'im yivvater mibbesar hamilluim umin hallejem 'ad habboqer -
 *  vesarafTA et hannotar ba'esh, lo ye'akhel ki QODESH hu."
 *   - Exodo 29:34
 * "Y si SOBRARE de la carne de los milluim y del pan hasta la
 *  manana, QUEMARAS lo que sobre con fuego; NO se comera, porque
 *  es SANTO."
 *
 * "ufetaj ohel mo'ed teshvu yomam valaylah SHIV'AT YAMIM
 *  ushmarTEM et MISHMERET YHWH velo tamutu - ki khen tsuvveiti."
 *   - Levitico 8:35
 * "A la puerta de la Tienda del Encuentro estareis dia y noche
 *  siete dias, y GUARDAREIS la ORDENANZA de YHWH para que no murais
 *  - porque ASI me fue mandado."
 *
 * "vekhol ZAR lo yokhal QODESH; toshav kohen vesakhir lo yokhal
 *  qodesh." - Levitico 22:10
 * "Ningun EXTRANO comera cosa SANTA; ni el huesped del sacerdote
 *  ni su jornalero comeran cosa santa."
 *
 * --- VOCABULARIO ---
 *
 *   zar          (ZR, H2114)                 EXTRANO - forastero,
 *                                               no autorizado.
 *   yokhal       (YAKL, H398)                COMERA - consumir,
 *                                               tener acceso.
 *   qodesh       (QDSh, H6944)               SANTO - separado,
 *                                               dedicado.
 *   kuppar       (KPR, H3722 Pual)         fue EXPIADO - cubierto,
 *                                               limpiado.
 *   saraf        (ShRP, H8313)               QUEMAR - destruir lo
 *                                               sobrante.
 *   notar        (NWTR, H3498 Nif)           lo que SOBRA -
 *                                               remanente a destruir.
 *   'ola         ('LH, H5930)                 HOLOCAUSTO - todo
 *                                               asciende, cero residuo.
 *   taqtir       (TQTYR, H6999 Hif)         QUEMARAS - convertir
 *                                               en humo por fuego.
 *   reaj nijoaj  (RYCh NYChWCh,
 *                  H7381+H5207)                  AROMA de REPOSO -
 *                                               senal de aceptacion.
 *   mishmeret    (MShMRT, H4931)            GUARDIA, ORDENANZA -
 *                                               lo que se custodia.
 *   toshav       (TWShB, H8453)               RESIDENTE - no hereda
 *                                               aunque viva cerca.
 *   sakhir       (ShKYR, H7916)                ASALARIADO - no come
 *                                               aunque trabaje alli.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   hashmal_onboarding_lockdown_activar    Ex 29:18 + Ex 29:33 -
 *                                           dedicar recursos ('ola).
 *   hashmal_onboarding_lockdown_verificar  Ex 29:33 + Lv 8:35 -
 *                                           integridad del mishmeret.
 *   hashmal_onboarding_lockdown_estado     lectura pura, sin logs.
 *
 * --- PRINCIPIOS DEL LOCKDOWN ---
 *
 *   1. ZAR NO COMPARTE (Ex 29:33 + Lv 22:10): lo consagrado no
 *      se comparte con el no autorizado. Confirmado en libro
 *      distinto (Levitico) con ejemplos concretos (ni toshav ni
 *      sakhir).
 *
 *   2. 'OLA - ENTREGA TOTAL (Ex 29:18): "ve'et KOL ha'ayil taqtir".
 *      TODO el carnero se quema. No se reserva una pata, no se
 *      guarda la piel. Cero residuo profano. En firmware: los
 *      recursos del nodo en consagracion estan integramente
 *      dedicados; ninguno queda disponible para otro uso.
 *
 *   3. LO SOBRANTE SE QUEMA (Ex 29:34): "vesarafta et hannotar
 *      ba'esh". Lo que no se consumio en el acto se destruye, no
 *      se guarda para otro dia. No hay "uso parcial" - o es
 *      consagrado y se usa integro, o se destruye.
 *
 *   4. MISHMERET (Lv 8:35): "ushmartem et mishmeret YHWH velo
 *      tamutu". Custodiar la ordenanza es condicion de vida. El
 *      `verificar` de este archivo es esa custodia: comprobar
 *      que el lockdown se mantiene.
 *
 * --- FASE REQUERIDA: OLA O POSTERIOR ---
 *
 * El lockdown se activa a partir de la fase OLA (fase 4) -
 * justo cuando Ex 29:18 manda "TODO el carnero se quema", la
 * entrega total de recursos cobra sentido. Antes de la 'ola, el
 * candidato aun no ha entregado totalmente: lavar, vestir, ungir
 * y purificar (jatta't) son actos sobre su persona; la 'ola es
 * la primera ENTREGA TOTAL. Desde ahi, el lockdown se sostiene
 * hasta el dia 8.
 *
 * --- IRREVERSIBILIDAD ---
 *
 * El lockdown, una vez activo, no se desactiva manualmente
 * durante el pipeline. Solo el paso natural CERO (via
 * cuarentena_purgar o deinit) libera recursos. Esto refleja
 * Ex 29:34: lo dedicado no se recicla - o se completa el ciclo
 * o se quema lo sobrante.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_onboarding.h
 *   hashmal_onboarding_internal.h   consagracion_mut + ahora_ms
 *   esp_log.h (ORO EGIPTO)           logging
 *
 * Autor: Antonio Jose Marin Soto (Betsalel - Ex 31:2)
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "esp_log.h"

#include "hashmal_onboarding.h"
#include "hashmal_onboarding_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *LOCK_TAG = "hashmal.onboarding.lockdown";

#define LOCK_LOG_I(fmt, ...)  ESP_LOGI(LOCK_TAG, fmt, ##__VA_ARGS__)
#define LOCK_LOG_W(fmt, ...)  ESP_LOGW(LOCK_TAG, fmt, ##__VA_ARGS__)
#define LOCK_LOG_E(fmt, ...)  ESP_LOGE(LOCK_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * HELPERS LOCALES (kelim autocontenidos)
 * ================================================================== */

static inline bool nodo_id_valido(uint8_t nodo_id)
{
    return nodo_id < HASHMAL_ONBOARDING_MAX_NODOS;
}

/* ==================================================================
 * hashmal_onboarding_lockdown_activar - Exodo 29:18 + Exodo 29:33
 *
 * Dedica los recursos del nodo al proceso de consagracion: a partir
 * de la 'ola (entrega total), el nodo no comparte. Ex 29:18 "kol
 * ha'ayil taqtir" - TODO el carnero se quema - manda la entrega
 * integra. Ex 29:33 "vezar lo yokhal" - el extrano no come - sella
 * el principio de no-compartir.
 *
 * Validaciones (en orden):
 *   1. Singleton iniciado.
 *   2. nodo_id en rango.
 *   3. Estado == EN_PROCESO.
 *   4. fase_actual >= OLA (fase 4). El lockdown acompana la
 *      entrega total del holocausto; antes no hay que lockear.
 *   5. Lockdown no ya activo (un solo acto - Ex 29:18 taqtir una
 *      vez por animal; no se requema lo ya quemado).
 *
 * Side effects:
 *   - n->lockdown         = true
 *   - n->timestamp_fase_ms = ahora
 *
 * 2+ testigos:
 *   Ex 29:18  - 'ola, entrega total (cero residuo)
 *   Ex 29:33  - vezar lo yokhal (recursos exclusivos)
 *   Ex 29:34  - lo sobrante se quema (nada profano queda)
 *   Lv 22:10  - libro distinto: ningun zar come cosa santa
 * ================================================================== */
int hashmal_onboarding_lockdown_activar(uint8_t nodo_id)
{
    hashmal_onboarding_t *c = hashmal_onboarding_consagracion_mut();
    if (c == NULL) {
        LOCK_LOG_E("lockdown_activar: subsistema no iniciado");
        return HASHMAL_ONBOARDING_ERR_NO_INIT;
    }

    if (!nodo_id_valido(nodo_id)) {
        LOCK_LOG_W("lockdown_activar: nodo_id %u fuera de rango",
                   (unsigned)nodo_id);
        return HASHMAL_ONBOARDING_ERR_NODO_INVALIDO;
    }

    hashmal_onboarding_nodo_t *n = &c->nodos[nodo_id];

    if (n->estado != HASHMAL_ONBOARDING_EN_PROCESO) {
        LOCK_LOG_W("lockdown_activar: nodo %u no EN_PROCESO "
                   "(estado=%u) — el lockdown es del pipeline "
                   "activo (Lv 8:35 mishméret durante shivʻát yamím)",
                   (unsigned)nodo_id, (unsigned)n->estado);
        return HASHMAL_ONBOARDING_ERR_FASE_INCORRECTA;
    }

    /* La 'ola (fase OLA=4) es la primera entrega TOTAL: Ex 29:18
     * "kol ha'ayil taqtir". Antes de esta fase no hay entrega
     * integra sobre la cual construir lockdown. */
    if (n->fase_actual < HASHMAL_ONBOARDING_FASE_OLA) {
        LOCK_LOG_W("lockdown_activar: nodo %u en fase %u — lockdown "
                   "requiere fase >= OLA (Éx 29:18 'kol haʼáyil "
                   "taqtír', entrega total); vezár lo yokhál "
                   "(Éx 29:33)",
                   (unsigned)nodo_id, (unsigned)n->fase_actual);
        return HASHMAL_ONBOARDING_ERR_FASE_INCORRECTA;
    }

    if (n->lockdown) {
        LOCK_LOG_W("lockdown_activar: nodo %u ya en lockdown — "
                   "Éx 29:18 taqtír es un solo acto, no se "
                   "requema lo ya quemado",
                   (unsigned)nodo_id);
        return HASHMAL_ONBOARDING_ERR_LOCKDOWN;
    }

    n->lockdown          = true;
    n->timestamp_fase_ms = hashmal_onboarding_ahora_ms();
    c->timestamp_ms      = n->timestamp_fase_ms;

    LOCK_LOG_I("nodo %u: LOCKDOWN activado — recursos DEDICADOS",
               (unsigned)nodo_id);
    LOCK_LOG_I("  ʻolá: TODO asciende, cero residuo profano "
               "(Éx 29:18 kol haʼáyil taqtír)");
    LOCK_LOG_I("  vezár lo yokhál kí qódesh hém (Éx 29:33 + Lv 22:10)");
    LOCK_LOG_I("  notár yisaréf (Éx 29:34) — lo sobrante se quema, "
               "nada queda para uso profano");

    return HASHMAL_ONBOARDING_OK;
}

/* ==================================================================
 * hashmal_onboarding_lockdown_verificar - Ex 29:33 + Lv 8:35
 *
 * Custodia del mishmeret (Lv 8:35 "ushmartem et mishmeret YHWH").
 * Comprueba que el nodo mantiene el lockdown. Verificacion EXITOSA
 * es silenciosa - solo registra cuando el lockdown esta ausente
 * (violacion de la ordenanza).
 *
 *   sin init                -> ERR_NO_INIT
 *   nodo_id fuera de rango  -> ERR_NODO_INVALIDO
 *   lockdown == false       -> ERR_LOCKDOWN + LOG_W
 *   lockdown == true        -> OK (silente)
 *
 * 2+ testigos:
 *   Ex 29:33  - lo santo no se comparte (base del lockdown)
 *   Lv 8:35   - mishmeret YHWH (libro distinto - el guardar)
 *   Ex 29:34  - notar yisaref (nada para otro uso)
 * ================================================================== */
int hashmal_onboarding_lockdown_verificar(uint8_t nodo_id)
{
    const hashmal_onboarding_t *c =
        hashmal_onboarding_get_consagracion_interno();
    if (c == NULL) {
        LOCK_LOG_E("lockdown_verificar: subsistema no iniciado");
        return HASHMAL_ONBOARDING_ERR_NO_INIT;
    }

    if (!nodo_id_valido(nodo_id)) {
        LOCK_LOG_W("lockdown_verificar: nodo_id %u fuera de rango",
                   (unsigned)nodo_id);
        return HASHMAL_ONBOARDING_ERR_NODO_INVALIDO;
    }

    const hashmal_onboarding_nodo_t *n = &c->nodos[nodo_id];

    if (!n->lockdown) {
        LOCK_LOG_W("lockdown_verificar: nodo %u sin lockdown — "
                   "violación del mishméret (Lv 8:35 ushmartém "
                   "et mishméret YHWH); vezár lo yokhál (Éx 29:33)",
                   (unsigned)nodo_id);
        return HASHMAL_ONBOARDING_ERR_LOCKDOWN;
    }

    /* Verificacion exitosa - silenciosa (Ex 29:34 lo sobrante se
     * quema: no hace falta celebrar la integridad, solo registrar
     * cuando falla). */
    return HASHMAL_ONBOARDING_OK;
}

/* ==================================================================
 * hashmal_onboarding_lockdown_estado - lectura pura
 *
 * Sin logs, sin side effects.
 *
 *   out NULL               -> ERR_NULL
 *   sin init               -> ERR_NO_INIT
 *   nodo_id fuera de rango -> ERR_NODO_INVALIDO
 * ================================================================== */
int hashmal_onboarding_lockdown_estado(uint8_t nodo_id, bool *out)
{
    if (out == NULL) return HASHMAL_ONBOARDING_ERR_NULL;

    const hashmal_onboarding_t *c =
        hashmal_onboarding_get_consagracion_interno();
    if (c == NULL) return HASHMAL_ONBOARDING_ERR_NO_INIT;

    if (!nodo_id_valido(nodo_id)) {
        return HASHMAL_ONBOARDING_ERR_NODO_INVALIDO;
    }

    *out = c->nodos[nodo_id].lockdown;
    return HASHMAL_ONBOARDING_OK;
}

/* ==================================================================
 *
 *   "ve'et KOL ha'ayil taqtir hammizbejah - 'olat YHWH hu, reaj
 *    nijoaj ishe laYHWH." - Exodo 29:18
 *
 *   "veZAR LO YOKHAL ki QODESH hem." - Exodo 29:33
 *
 *   "vesarafta et hannotar ba'esh - lo ye'akhel ki qodesh hu."
 *     - Exodo 29:34
 *
 *   "ushmartem et MISHMERET YHWH velo tamutu." - Levitico 8:35
 *
 *   "vekhol ZAR lo yokhal qodesh; toshav kohen vesakhir lo
 *    yokhal qodesh." - Levitico 22:10
 *
 *   El lockdown no es celos: es santidad. Lo dedicado a YHWH no
 *   se comparte - no porque se proteja del que sirve, sino porque
 *   ES santo. La 'ola marca el principio: TODO el carnero
 *   asciende, ni una brizna queda en la cocina para despues.
 *   Ex 29:34 extiende: si sobra, se quema. No hay "uso menor"
 *   de lo consagrado. Y Lv 22:10 confirma en libro distinto: ni
 *   el huesped del sacerdote (toshav) ni el asalariado (sakhir)
 *   comen de lo santo - el criterio no es proximidad, es
 *   consagracion. En firmware: los recursos del nodo en
 *   consagracion estan DEDICADOS. Ni proceso ajeno, ni consulta
 *   externa, ni "uso parcial". Durante el pipeline, el nodo no
 *   comparte - solo sirve al que lo esta formando.
 *
 *   === VEZAR LO YOKHAL - consagracion_lockdown.c COMPLETO ===
 *
 *   -- Los 5 archivos hermanos -----------------------------------
 *
 *     consagracion_onboarding.c   PIPELINE           - CERO -> ACTIVO
 *     consagracion_fases.c        SHEVA' MA'ASIM     - las 7 fases
 *     consagracion_sellos.c       DAM 'AL TENUKH     - 3 sellos
 *     consagracion_lockdown.c     VEZAR LO YOKHAL    - recursos   <- ESTE
 *     consagracion_cuarentena.c   MIJUTS LAMMAJANEH  - Ex 29:14
 *
 *   Archivo 4 de 5 de la Consagracion. Siguiente:
 *     consagracion_cuarentena.c - mijuts lammajaneh (Ex 29:14)
 *
 *   PASO 9 de Exodo 40:12-15 - Pieza 10, Archivo 4 de 5.
 *
 * ================================================================== */
