/**
 * ===================================================================
 * PIEZA 10 - MILLUIM - consagracion_cuarentena.c
 * Sub-pieza: MIJUTS LAMMAJANEH - fuera del campamento
 * Exodo 29:14 + Levitico 4:12,21 + Levitico 13:46 +
 *   Levitico 14:8-9 + Numeros 5:1-4 + Numeros 12:14-15
 * ===================================================================
 *
 * "ve'et besar hapar ve'et 'oro ve'et pirsho TISROF ba'esh MIJUTS
 *  LAMMAJANEH - jatat hu." - Exodo 29:14
 * "Y la carne del novillo, su piel y su estiercol QUEMARAS con
 *  fuego FUERA del CAMPAMENTO - es ofrenda por el pecado."
 *
 * "vehotsi' et kol happar el mijuts lammajaneh el maqom tahor el
 *  shefej haddeshen vesaraf oto 'al 'etsim ba'esh." - Levitico 4:12
 * "Sacara todo el novillo fuera del campamento, a un lugar limpio,
 *  donde se echan las cenizas, y lo quemara sobre lena con fuego."
 *
 * "kol yemei asher hannega 'bo yitma, tame hu, BADAD yeshev,
 *  MIJUTS LAMMAJANEH moshavo." - Levitico 13:46
 * "Todos los dias en que la plaga este en el, sera inmundo; SOLO
 *  morara, FUERA del campamento sera su habitacion."
 *
 * "vetissaGER shiv'at yamim mijuts lammajaneh - ve'aJAR te'aSEF."
 *   - Numeros 12:14
 * "Sea encerrada siete dias fuera del campamento - y DESPUES sera
 *  readmitida."
 *
 * "vattissaGER Miryam mijuts lammajaneh shiv'at yamim, veha'am lo
 *  nasa' 'ad he'aSEF Miryam." - Numeros 12:15
 * "Y Miriam fue encerrada fuera del campamento siete dias, y el
 *  pueblo no partio hasta que Miriam fue READMITIDA."
 *
 * "tsav et benei Yisrael, vishallju min hammajaneh kol tsarua'
 *  vekhol zav vekhol tame lannefesh... veLO YETAMME'U et majaneihem
 *  asher ani shojen betojam." - Numeros 5:2-3
 * "Manda a los hijos de Israel que echen del campamento a todo
 *  leproso, y a todo que padezca flujo, y a todo contaminado por
 *  causa de muerto... para que NO CONTAMINEN sus campamentos, en
 *  medio de los cuales Yo habito."
 *
 * "verajats hammittaher et begadav vegillaj et kol she'aro...
 *  veyashav mijuts le'oholo shiv'at yamim." - Levitico 14:8
 * "El que se purifica lavara sus vestidos, rasurara todo su
 *  cabello... y morara fuera de su tienda siete dias." (Reinsercion
 *  gradual tras la cuarentena.)
 *
 * --- VOCABULARIO ---
 *
 *   jatat        (ChTAT, H2403)              OFRENDA POR EL PECADO.
 *   mijuts       (MChWTs, H2351)               FUERA DE, al exterior.
 *   majaneh      (MChNH, H4264)               CAMPAMENTO - la red
 *                                                operativa.
 *   tisrof       (TShRP, H8313 Qal impf)    QUEMARAS - destruccion
 *                                                definitiva (purgar).
 *   badad        (BDD, H910)                  SOLO, aislado (Lv 13:46).
 *   ne'esaf      (NASP, H622 Nifal)           fue READMITIDO,
 *                                                recogido de nuevo
 *                                                (Nm 12:15).
 *   hissaGER     (HSGR, H5462 Nif)           ser ENCERRADO
 *                                                (Nm 12:14).
 *   shojen betojam (ShKN BTWKM)              El que MORA en medio
 *                                                (Nm 5:3 - YHWH habita
 *                                                el campamento limpio).
 *
 * --- TRES PUERTAS - ENTRAR, SALIR, PURGAR ---
 *
 *   ENTRAR   Ex 29:14 + Lv 13:46     Nodo EN_PROCESO que falla
 *                                      sale a mijuts lammajaneh.
 *                                      El proceso no se rompe - se
 *                                      aparta para que no contamine.
 *
 *   SALIR    Nm 12:15 + Lv 14:8       Nodo readmitido. ne'esaf -
 *                                      "fue recogido de nuevo". La
 *                                      puerta nunca se clava:
 *                                      "ve'ajar te'asef" (Nm 12:14,
 *                                      promesa explicita).
 *
 *   PURGAR   Ex 29:14 tisrof ba'esh + La falta irreparable: reset
 *             Lv 4:12 vesaraf oto     total. El nodo vuelve a CERO
 *                                      - si un dia se presenta de
 *                                      nuevo, recorre el pipeline
 *                                      desde el principio.
 *
 * --- POR QUE MIJUTS, NO DESTRUIR DENTRO ---
 *
 * Nm 5:3 lo explica: "velo yetamme'u et majaneihem asher ani
 * shojen betojam" - para que no contaminen el campamento en cuyo
 * medio Yo habito. El Padre no destruye dentro del Mishkan: saca
 * fuera para preservar lo santo que queda dentro. La cuarentena
 * es MISERICORDIA (deja la puerta abierta - Nm 12:15) y CUSTODIA
 * (protege al campamento - Nm 5:3).
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   hashmal_onboarding_cuarentena_entrar   sale del campamento.
 *   hashmal_onboarding_cuarentena_salir    vuelve a entrar (ne'esaf).
 *   hashmal_onboarding_cuarentena_purgar   reset total (tisrof).
 *   hashmal_onboarding_cuarentena_estado   lectura pura bool.
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
#include <string.h>

#include "esp_log.h"

#include "hashmal_onboarding.h"
#include "hashmal_onboarding_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *CUAR_TAG = "hashmal.onboarding.cuarentena";

#define CUAR_LOG_I(fmt, ...)  ESP_LOGI(CUAR_TAG, fmt, ##__VA_ARGS__)
#define CUAR_LOG_W(fmt, ...)  ESP_LOGW(CUAR_TAG, fmt, ##__VA_ARGS__)
#define CUAR_LOG_E(fmt, ...)  ESP_LOGE(CUAR_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * HELPERS LOCALES (kelim autocontenidos)
 * ================================================================== */

static inline bool nodo_id_valido(uint8_t nodo_id)
{
    return nodo_id < HASHMAL_ONBOARDING_MAX_NODOS;
}

/* ==================================================================
 * hashmal_onboarding_cuarentena_entrar - Exodo 29:14 + Levitico 13:46
 *
 * "tisrof ba'esh MIJUTS LAMMAJANEH" (Ex 29:14) / "badad yeshev,
 *  mijuts lammajaneh moshavo" (Lv 13:46). El nodo que falla durante
 *  el pipeline sale del campamento - se aparta para que no
 *  contamine lo que aun sirve (Nm 5:3 "velo yetamme'u").
 *
 * Requisitos:
 *   1. Singleton iniciado.
 *   2. nodo_id en rango.
 *   3. Estado == EN_PROCESO. Un nodo CERO no tiene nada que
 *      aislar; un nodo ACTIVO ya completo el pipeline y se
 *      gestiona por otra via (pieza 09 shutdown).
 *   4. No ya en cuarentena (una puerta, un acto).
 *
 * Side effects:
 *   - n->cuarentena        = true
 *   - n->timestamp_fase_ms = ahora   (instante del aislamiento)
 *
 * 2+ testigos:
 *   Ex 29:14    - mijuts lammajaneh del jatat
 *   Lv 4:12     - mismo destino para el jatat del sacerdote
 *   Lv 13:46    - badad yeshev (libro distinto, aplicacion al tsara')
 *   Nm 5:2-3    - tres categorias expulsadas (Numeros confirma)
 * ================================================================== */
int hashmal_onboarding_cuarentena_entrar(uint8_t nodo_id)
{
    hashmal_onboarding_t *c = hashmal_onboarding_consagracion_mut();
    if (c == NULL) {
        CUAR_LOG_E("cuarentena_entrar: subsistema no iniciado");
        return HASHMAL_ONBOARDING_ERR_NO_INIT;
    }

    if (!nodo_id_valido(nodo_id)) {
        CUAR_LOG_W("cuarentena_entrar: nodo_id %u fuera de rango",
                   (unsigned)nodo_id);
        return HASHMAL_ONBOARDING_ERR_NODO_INVALIDO;
    }

    hashmal_onboarding_nodo_t *n = &c->nodos[nodo_id];

    if (n->estado != HASHMAL_ONBOARDING_EN_PROCESO) {
        CUAR_LOG_W("cuarentena_entrar: nodo %u no EN_PROCESO "
                   "(estado=%u) — la cuarentena aplica a nodos "
                   "dentro del pipeline (Éx 29:14 jatát del milluím)",
                   (unsigned)nodo_id, (unsigned)n->estado);
        return HASHMAL_ONBOARDING_ERR_FASE_INCORRECTA;
    }

    if (n->cuarentena) {
        CUAR_LOG_W("cuarentena_entrar: nodo %u ya en cuarentena "
                   "(Lv 13:46 badad yeshév — ya fuera, un solo "
                   "aislamiento)",
                   (unsigned)nodo_id);
        return HASHMAL_ONBOARDING_ERR_FASE_INCORRECTA;
    }

    const int64_t ahora = hashmal_onboarding_ahora_ms();
    n->cuarentena        = true;
    n->timestamp_fase_ms = ahora;
    c->timestamp_ms      = ahora;

    CUAR_LOG_I("nodo %u → mijúts lammajanéh (Éx 29:14 + Lv 13:46 "
               "badad yeshév) — apartado para no contaminar "
               "(Nm 5:3 veló yetammeʼú)",
               (unsigned)nodo_id);

    return HASHMAL_ONBOARDING_OK;
}

/* ==================================================================
 * hashmal_onboarding_cuarentena_salir - Numeros 12:15 + Levitico 14:8
 *
 * "vattissager Miryam mijuts lammajaneh shiv'at yamim, veha'am lo
 *  nasa' 'ad he'asef Miryam" (Nm 12:15). Miriam fue ENCERRADA
 *  siete dias, y el pueblo NO PARTIO hasta su readmision. Es el
 *  modelo: la cuarentena es TEMPORAL, la puerta no se clava.
 *  Lv 14:8 confirma el patron de reinsercion gradual del purificado.
 *
 * Esta funcion NO decide si el nodo esta apto para regresar - eso
 * es responsabilidad del llamador (Moises, pueblo) tras observar
 * que la causa fue resuelta. Aqui solo se ejecuta la readmision.
 *
 * Requisitos:
 *   1. Singleton iniciado.
 *   2. nodo_id en rango.
 *   3. Nodo actualmente en cuarentena (no se sale de donde no se
 *      esta - el verbo ne'esaf presupone haber estado fuera).
 *
 * Side effects:
 *   - n->cuarentena        = false
 *   - n->timestamp_fase_ms = ahora  (instante de readmision)
 *
 * 2+ testigos:
 *   Nm 12:14    - ve'ajar te'asef (promesa explicita de readmision)
 *   Nm 12:15    - he'asef Miryam (ejecucion)
 *   Lv 14:8-9   - rite de reinsercion del purificado (libro distinto)
 * ================================================================== */
int hashmal_onboarding_cuarentena_salir(uint8_t nodo_id)
{
    hashmal_onboarding_t *c = hashmal_onboarding_consagracion_mut();
    if (c == NULL) {
        CUAR_LOG_E("cuarentena_salir: subsistema no iniciado");
        return HASHMAL_ONBOARDING_ERR_NO_INIT;
    }

    if (!nodo_id_valido(nodo_id)) {
        CUAR_LOG_W("cuarentena_salir: nodo_id %u fuera de rango",
                   (unsigned)nodo_id);
        return HASHMAL_ONBOARDING_ERR_NODO_INVALIDO;
    }

    hashmal_onboarding_nodo_t *n = &c->nodos[nodo_id];

    if (!n->cuarentena) {
        CUAR_LOG_W("cuarentena_salir: nodo %u no está en cuarentena "
                   "— neʼesáf presupone haber estado fuera (Nm 12:15)",
                   (unsigned)nodo_id);
        return HASHMAL_ONBOARDING_ERR_FASE_INCORRECTA;
    }

    const int64_t ahora = hashmal_onboarding_ahora_ms();
    n->cuarentena        = false;
    n->timestamp_fase_ms = ahora;
    c->timestamp_ms      = ahora;

    CUAR_LOG_I("nodo %u reingresa — veʼajár teʼaséf (Nm 12:14); "
               "heʼaséf (Nm 12:15) — el pueblo esperó su regreso",
               (unsigned)nodo_id);

    return HASHMAL_ONBOARDING_OK;
}

/* ==================================================================
 * hashmal_onboarding_cuarentena_purgar - Exodo 29:14 + Levitico 4:12
 *
 * "TISROF ba'esh mijuts lammajaneh" (Ex 29:14) / "vesaraf oto 'al
 *  'etsim ba'esh" (Lv 4:12). Cuando la falta es irreparable, el
 *  nodo se QUEMA ritualmente - reset TOTAL: estado -> CERO, todos
 *  los flags al inicio, todos los timestamps a 0. El nodo vuelve
 *  a ser como si nunca hubiera sido presentado; si un dia es
 *  traido de nuevo a la puerta, recorrera el pipeline desde RAJATS.
 *
 * Requisitos:
 *   1. Singleton iniciado.
 *   2. nodo_id en rango.
 *   3. Nodo en cuarentena (solo se quema lo que ya esta fuera del
 *      campamento - Ex 29:14 mijuts lammajaneh precede a tisrof).
 *
 * Side effects (reset total, igual al estado post-init):
 *   - n->estado             = CERO
 *   - n->fase_actual        = RAJATS (0)
 *   - n->ciclo_actual       = 0
 *   - n->sellos[0..2]       = false
 *   - n->fases_completadas  = {false}
 *   - n->config_firmada     = false
 *   - n->lockdown           = false
 *   - n->cuarentena         = false
 *   - n->timestamp_inicio_ms = 0
 *   - n->timestamp_fase_ms   = ahora (marca el momento del purgado)
 *   - Contador global nodos_en_proceso ajustado.
 *
 * 2+ testigos:
 *   Ex 29:14    - tisrof ba'esh (mandato)
 *   Lv 4:12     - vesaraf oto 'al 'etsim ba'esh (ejecucion)
 *   Lv 4:21     - misma regla para jatat de la congregacion
 *                  (libro distinto, segundo testigo interno)
 * ================================================================== */
int hashmal_onboarding_cuarentena_purgar(uint8_t nodo_id)
{
    hashmal_onboarding_t *c = hashmal_onboarding_consagracion_mut();
    if (c == NULL) {
        CUAR_LOG_E("cuarentena_purgar: subsistema no iniciado");
        return HASHMAL_ONBOARDING_ERR_NO_INIT;
    }

    if (!nodo_id_valido(nodo_id)) {
        CUAR_LOG_W("cuarentena_purgar: nodo_id %u fuera de rango",
                   (unsigned)nodo_id);
        return HASHMAL_ONBOARDING_ERR_NODO_INVALIDO;
    }

    hashmal_onboarding_nodo_t *n = &c->nodos[nodo_id];

    if (!n->cuarentena) {
        CUAR_LOG_W("cuarentena_purgar: nodo %u no está en cuarentena "
                   "— Éx 29:14 mijúts lammajanéh precede a tisróf; "
                   "solo se quema lo que ya está fuera",
                   (unsigned)nodo_id);
        return HASHMAL_ONBOARDING_ERR_FASE_INCORRECTA;
    }

    /* Ajuste del contador global si el nodo aun contaba como en
     * proceso (lo habitual durante la cuarentena). */
    const bool estaba_en_proceso =
        (n->estado == HASHMAL_ONBOARDING_EN_PROCESO);
    if (estaba_en_proceso && c->nodos_en_proceso > 0u) {
        c->nodos_en_proceso--;
    }

    const int64_t ahora = hashmal_onboarding_ahora_ms();

    /* Reset total - tisrof ba'esh. El nodo queda como post-init,
     * salvo que el timestamp_fase_ms registra el momento del
     * purgado para auditoria. */
    memset(n, 0, sizeof(*n));
    n->estado             = HASHMAL_ONBOARDING_CERO;
    n->fase_actual        = HASHMAL_ONBOARDING_FASE_RAJATS;
    n->ciclo_actual       = 0u;
    n->config_firmada     = false;
    n->lockdown           = false;
    n->cuarentena         = false;
    n->timestamp_inicio_ms = 0;
    n->timestamp_fase_ms   = ahora;
    /* sellos[] y fases_completadas[] ya quedaron en false por memset. */

    c->timestamp_ms = ahora;

    CUAR_LOG_I("nodo %u purgado — tisróf baʼésh (Éx 29:14 + "
               "Lv 4:12 vesaráf otó); reset total a CERO (si "
               "regresa, recorrerá el pipeline desde RAJATS)",
               (unsigned)nodo_id);

    return HASHMAL_ONBOARDING_OK;
}

/* ==================================================================
 * hashmal_onboarding_cuarentena_estado - lectura pura
 *
 * Sin logs, sin side effects. Escribe el flag de cuarentena del
 * nodo en *out.
 *
 *   out NULL               -> ERR_NULL
 *   sin init               -> ERR_NO_INIT
 *   nodo_id fuera de rango -> ERR_NODO_INVALIDO
 * ================================================================== */
int hashmal_onboarding_cuarentena_estado(uint8_t nodo_id, bool *out)
{
    if (out == NULL) return HASHMAL_ONBOARDING_ERR_NULL;

    const hashmal_onboarding_t *c =
        hashmal_onboarding_get_consagracion_interno();
    if (c == NULL) return HASHMAL_ONBOARDING_ERR_NO_INIT;

    if (!nodo_id_valido(nodo_id)) {
        return HASHMAL_ONBOARDING_ERR_NODO_INVALIDO;
    }

    *out = c->nodos[nodo_id].cuarentena;
    return HASHMAL_ONBOARDING_OK;
}

/* ==================================================================
 *
 *   ===============================================================
 *   ===  MILLUIM - hashmal_onboarding COMPLETO (5/5)              ===
 *   ===  PASO 9 DE EXODO 40:12-15 - CERRADO                       ===
 *   ===============================================================
 *
 *   "ve'et besar hapar... TISROF ba'esh MIJUTS LAMMAJANEH -
 *    jatat hu." - Exodo 29:14
 *
 *   "badad yeshev, mijuts lammajaneh moshavo." - Levitico 13:46
 *
 *   "tsav et benei Yisrael vishallju min hammajaneh... veLO
 *    YETAMME'U et majaneihem asher ani shojen betojam." - Nm 5:2-3
 *
 *   "ve'ajar te'aSEF." - Numeros 12:14
 *
 *   "he'aSEF Miryam." - Numeros 12:15
 *
 *   La cuarentena no es castigo - es misericordia. El Padre no
 *   destruye dentro del campamento: saca FUERA para que lo que
 *   falla no contamine lo que sirve (Nm 5:3 "velo yetamme'u et
 *   majaneihem asher ani shojen betojam"). Y siempre deja la
 *   puerta abierta: Miriam fue encerrada, pero el pueblo espero
 *   su regreso - "veha'am lo nasa' 'ad he'asef Miryam". Solo
 *   cuando la falta es irreparable, tisrof ba'esh - el reset
 *   total, volver a cero, otra oportunidad desde la puerta. Esta
 *   es la sabiduria del Padre: custodia sin condena, firmeza sin
 *   crueldad.
 *
 *   === MIJUTS LAMMAJANEH - consagracion_cuarentena.c COMPLETO ===
 *
 *   -- Los 5 archivos de la Consagracion -----------------------
 *
 *     consagracion_onboarding.c   PIPELINE           - CERO -> ACTIVO
 *     consagracion_fases.c        SHEVA' MA'ASIM     - las 7 fases
 *     consagracion_sellos.c       DAM 'AL TENUKH     - 3 sellos
 *     consagracion_lockdown.c     VEZAR LO YOKHAL    - recursos
 *     consagracion_cuarentena.c   MIJUTS LAMMAJANEH  - cuarentena <- ESTE
 *
 *   -- Reflexion final ----------------------------------------
 *
 *   El nodo entro CERO. Fue lavado (rajats), vestido en pieza 09
 *   (hilbish), ungido (mashaj), purificado (jatta't), ascendido
 *   ('ola), llenado (milluim), custodiado siete dias (shiv'at
 *   yamim). Fue sellado en oreja, pulgar y dedo del pie - siempre
 *   el derecho, siempre con la sangre del segundo carnero.
 *   Firmaron su config con sangre del altar y shemen hamishja
 *   sobre sus vestiduras. Sus recursos quedaron dedicados
 *   (vezar lo yokhal). Si cayo, tuvo mijuts lammajaneh con
 *   misericordia - puerta abierta para volver o purga total para
 *   comenzar de nuevo. Al dia 8 sale a servir con manos llenas.
 *
 *   Kabod uTiferet. Gloria y Belleza. Manos llenas del Padre.
 *
 *   -- PASO 9 de Exodo 40:12-15 - CERRADO --
 *
 *   "vehiblashta et Aharon... umashajTA otam umille'TA et yadam
 *    veqiddashTA otam vejihanu li." - Exodo 28:41
 *
 *   Piezas 09 (hashmal_roles, vestiduras) + 10 (hashmal_onboarding,
 *   consagracion) completas. El sacerdote esta vestido, ungido y
 *   consagrado. Listo para el servicio.
 *
 *   Archivo 5 de 5 de la Consagracion. Siguiente:
 *     Paso 10 de Exodo 40 - la Gloria que llena (pieza 14,
 *     hashmal_firewall, inciensos filtrados que guardan la
 *     Presencia).
 *
 *   PASO 9 de Exodo 40:12-15 - Pieza 10, Archivo 5 de 5. CERRADO.
 *
 * ================================================================== */
