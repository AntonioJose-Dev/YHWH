/**
 * ===================================================================
 * PIEZA 10 - MILLUIM - consagracion_sellos.c
 * Sub-pieza: DAM 'AL TENUKH - sangre en oreja, pulgar y dedo del pie
 * Exodo 29:19-21 + Levitico 8:22-30 + Levitico 14:14-17
 * ===================================================================
 *
 * "veshajatta et ha'ayil velaqajTA middaMO venataTA 'al TENUKH
 *  OZEN Aharon ve'al tenukh ozen banav haYEMANIT ve'al BOHEN
 *  YADAM haYEMANIT ve'al BOHEN RAGLAM haYEMANIT vezaraqTA et
 *  haddam 'al hammizbeaj saviv." - Exodo 29:20
 * "Y degollaras el carnero y tomaras de su sangre y pondras sobre
 *  la EXTREMIDAD de la OREJA DERECHA de Aaron, y sobre la extremidad
 *  de la oreja derecha de sus hijos, y sobre el PULGAR de su MANO
 *  DERECHA, y sobre el DEDO GORDO de su PIE DERECHO; y rociaras la
 *  sangre sobre el altar alrededor."
 *
 * "velaqajTA min haddam asher 'al hammizbeaj umiSHEMEN HAMISHJA
 *  veHIZZEITA 'al Aharon ve'al BEGADAV ve'al banav ve'al bigdei
 *  vanav itto - veQADASH hu uvegadav uvanav uvigdei vanav
 *  itto." - Exodo 29:21
 * "Tomaras de la sangre que esta sobre el altar y del ACEITE DE LA
 *  UNCION, y ROCIARAS sobre Aaron y sobre sus VESTIDURAS, y sobre
 *  sus hijos y sobre las vestiduras de sus hijos con el; y SERA
 *  SANTIFICADO el y sus vestiduras, y sus hijos y las vestiduras
 *  de sus hijos con el."
 *
 * "vayyishjat vayyiqqaj Moshe middamo vayyitten 'al TENUKH OZEN
 *  Aharon hayemanit ve'al BOHEN YADO hayemanit ve'al BOHEN RAGLO
 *  hayemanit." - Levitico 8:23
 * "Y Moises lo degollo y tomo de su sangre y puso sobre la extremidad
 *  de la oreja derecha de Aaron, y sobre el pulgar de su mano
 *  derecha, y sobre el dedo gordo de su pie derecho."
 *
 * "vayyaqREV Moshe et benei Aharon vayyitten Moshe min haddam 'al
 *  tenukh oznam hayemanit..." - Levitico 8:24 (aplicacion a los hijos)
 *
 * "vayyiqqaj Moshe mishemen hammishja umin haddam asher 'al
 *  hammizbeaj, vayyaz 'al Aharon 'al begadav ve'al banav ve'al
 *  bigdei vanav itto - vayqaddesh et Aharon et begadav ve'et
 *  banav ve'et bigdei vanav itto." - Levitico 8:30
 * "Y tomo Moises del aceite de la uncion y de la sangre que estaba
 *  sobre el altar, y rocio sobre Aaron y sobre sus vestiduras...
 *  y santifico a Aaron y sus vestiduras."
 *
 * "velaqaj hakohen middam ha'asham veNATAN hakohen 'al TENUKH
 *  OZEN hammittaher hayemanit ve'al BOHEN YADO hayemanit ve'al
 *  BOHEN RAGLO hayemanit." - Levitico 14:14
 * "Y el sacerdote tomara de la sangre del sacrificio por la culpa
 *  y pondra sobre la extremidad de la oreja derecha del que se
 *  purifica, y sobre el pulgar de su mano derecha, y sobre el
 *  dedo gordo de su pie derecho."
 *
 * (Mismo patron - oreja + pulgar + dedo derechos - aplicado al
 *  leproso purificado. Confirmacion de que el patron de YHWH es
 *  universal: no es ceremonia aislada de los sacerdotes, es el
 *  diseno del Padre para consagrar/purificar todo el cuerpo.)
 *
 * --- VOCABULARIO ---
 *
 *   tenukh       (TNWK, H8571)             EXTREMIDAD - lobulo,
 *                                                punto mas lejano.
 *   ozen         (AZN, H241)                  OREJA - canal de
 *                                                RECEPCION.
 *   bohen        (BHN, H931)                  PULGAR / dedo gordo -
 *                                                punto de ACCION.
 *   yad          (YD, H3027)                    MANO - ejecutar,
 *                                                PROCESAR.
 *   regel        (RGL, H7272)                  PIE - caminar,
 *                                                TRANSMITIR.
 *   yemani(t)    (YMNY(T), H3233)            DERECHO(A) - lado
 *                                                de autoridad.
 *   dam          (DM, H1818)                   SANGRE - verificacion,
 *                                                autenticacion.
 *   shemen mishja (ShMN HMShChH,
 *                  H8081+H4888)                   ACEITE de UNCION -
 *                                                autorizacion (pieza 13).
 *   hizza        (HZH, H5137 Hif)             ROCIAR finamente -
 *                                                aplicacion sobre
 *                                                superficie amplia.
 *   veqadash     (WQDSh, H6942)                Y SERA SANTIFICADO -
 *                                                resultado final.
 *   begadim      (BGDYM, H899)                VESTIDURAS -
 *                                                configuracion del rol
 *                                                (pieza 09).
 *   mittaher     (MTHR, H2891 Hit)           El que se PURIFICA
 *                                                (Lv 14:14 - paralelo).
 *
 * --- SIGNIFICADO DE LOS 3 PUNTOS ---
 *
 *   OZEN  (oreja derecha)      -> RECIBIR    - el canal de entrada del
 *                                              nodo queda autorizado.
 *   BOHEN YAD (pulgar derecho) -> PROCESAR   - la mano que ejecuta el
 *                                              servicio queda autorizada.
 *   BOHEN REGEL (dedo pie der) -> TRANSMITIR - el pie que camina/lleva
 *                                              queda autorizado.
 *
 * Siempre YEMANI - lado derecho, el de la autoridad. Nodo con los
 * 3 sellos: recibe + procesa + transmite con autoridad sellada por
 * sangre del eil hamilluim.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   hashmal_onboarding_sello_aplicar      Ex 29:20 + Lv 8:23-24.
 *   hashmal_onboarding_sello_verificar    bool puro (sin logs).
 *   hashmal_onboarding_sello_estado       snapshot bool[3].
 *   hashmal_onboarding_config_firmar      Ex 29:21 + Lv 8:30.
 *   hashmal_onboarding_config_firmada     bool puro (sin logs).
 *
 * --- ORDEN BIBLICO ESTRICTO ---
 *
 *   Ex 29:19-20 - segundo carnero (eil hamilluim) -> 3 sellos en
 *                  cuerpo.
 *   Ex 29:21    - rociar sangre+aceite sobre las vestiduras.
 *
 * En firmware:
 *   1) `sello_aplicar` exige fase_actual >= MILLUIM (fase 5). Antes
 *      del segundo carnero no hay sangre para aplicar.
 *   2) `config_firmar` exige los 3 sellos ya colocados (v.21 viene
 *      tras v.20 en el texto - orden SECUENCIAL).
 *
 * --- IRREVERSIBILIDAD ---
 *
 * Ex 29:20 "'al tenukh ozen... 'al bohen yadam... 'al bohen raglam"
 * no tiene retraccion. La sangre aplicada no se limpia - se
 * santifica. Igual que el tzitz (pieza 09), un sello puesto no se
 * desaplica: intentar repetirlo devuelve ERR_SELLO_DUPLICADO,
 * nunca se reemplaza.
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
static const char *SELLOS_TAG = "hashmal.onboarding.sellos";

#define SELLOS_LOG_I(fmt, ...)  ESP_LOGI(SELLOS_TAG, fmt, ##__VA_ARGS__)
#define SELLOS_LOG_W(fmt, ...)  ESP_LOGW(SELLOS_TAG, fmt, ##__VA_ARGS__)
#define SELLOS_LOG_E(fmt, ...)  ESP_LOGE(SELLOS_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * HELPERS LOCALES (kelim autocontenidos)
 * ================================================================== */

static inline bool nodo_id_valido(uint8_t nodo_id)
{
    return nodo_id < HASHMAL_ONBOARDING_MAX_NODOS;
}

static inline bool sello_valido(hashmal_onboarding_sello_t s)
{
    return (unsigned)s < HASHMAL_ONBOARDING_SELLOS_TOTAL;
}

static const char *nombre_sello(hashmal_onboarding_sello_t s)
{
    switch (s) {
    case HASHMAL_ONBOARDING_SELLO_OZEN:
        return "ÓZEN (H241 — oreja DERECHA, RECIBIR, Éx 29:20 + Lv 14:14)";
    case HASHMAL_ONBOARDING_SELLO_BOHEN_YAD:
        return "BÓHEN YAD (H931+H3027 — pulgar mano DERECHA, "
               "PROCESAR, Éx 29:20 + Lv 14:14)";
    case HASHMAL_ONBOARDING_SELLO_BOHEN_REGEL:
        return "BÓHEN RÉGEL (H931+H7272 — dedo gordo pie DERECHO, "
               "TRANSMITIR, Éx 29:20 + Lv 14:14)";
    default:
        return "<sello desconocido>";
    }
}

/* ==================================================================
 * hashmal_onboarding_sello_aplicar - Exodo 29:20 + Levitico 8:23-24
 *
 * Aplica UNO de los 3 sellos de sangre del eil hamilluim (segundo
 * carnero): oreja, pulgar, o dedo del pie - siempre el derecho.
 *
 * Requisitos:
 *   1. Singleton iniciado.
 *   2. nodo_id en [0..MAX_NODOS-1].
 *   3. sello en [OZEN..BOHEN_REGEL].
 *   4. Estado == EN_PROCESO.
 *   5. fase_actual >= MILLUIM (fase 5). Ex 29:19 "velaqajta et
 *      ha'ayil hasheni" - el segundo carnero es el de milluim;
 *      los sellos son SU sangre. Antes de esa fase no existe la
 *      sangre del segundo carnero.
 *   6. Sello no aplicado previamente - IRREVERSIBLE, un solo
 *      tenukh por sello.
 *
 * Side effects:
 *   - n->sellos[sello] = true
 *   - n->timestamp_fase_ms = ahora
 *   - Si los 3 sellos completos -> log especial de triple sellado.
 *
 * 2+ testigos:
 *   Ex 29:20    - mandato del triple sello
 *   Lv 8:23-24  - ejecucion por Moises (libro distinto)
 *   Lv 14:14    - mismo patron en purificacion del leproso
 *                 (libro distinto - diseno universal del Padre)
 * ================================================================== */
int hashmal_onboarding_sello_aplicar(uint8_t nodo_id,
                                      hashmal_onboarding_sello_t sello)
{
    hashmal_onboarding_t *c = hashmal_onboarding_consagracion_mut();
    if (c == NULL) {
        SELLOS_LOG_E("sello_aplicar: subsistema no iniciado");
        return HASHMAL_ONBOARDING_ERR_NO_INIT;
    }

    if (!nodo_id_valido(nodo_id)) {
        SELLOS_LOG_W("sello_aplicar: nodo_id %u fuera de rango",
                     (unsigned)nodo_id);
        return HASHMAL_ONBOARDING_ERR_NODO_INVALIDO;
    }

    if (!sello_valido(sello)) {
        SELLOS_LOG_W("sello_aplicar: sello %u fuera de [0..%u] — "
                     "solo 3 tenujót en Éx 29:20",
                     (unsigned)sello,
                     (unsigned)(HASHMAL_ONBOARDING_SELLOS_TOTAL - 1u));
        return HASHMAL_ONBOARDING_ERR_FASE_INCORRECTA;
    }

    hashmal_onboarding_nodo_t *n = &c->nodos[nodo_id];

    if (n->estado != HASHMAL_ONBOARDING_EN_PROCESO) {
        SELLOS_LOG_W("sello_aplicar: nodo %u no EN_PROCESO "
                     "(estado=%u) — la sangre se aplica durante "
                     "el pipeline",
                     (unsigned)nodo_id, (unsigned)n->estado);
        return HASHMAL_ONBOARDING_ERR_FASE_INCORRECTA;
    }

    /* Ex 29:19-20 + Lv 8:22-24: los sellos son del SEGUNDO carnero,
     * no del primero. Fase MILLUIM (5) o posterior (SHIVAT=6). */
    if (n->fase_actual < HASHMAL_ONBOARDING_FASE_MILLUIM) {
        SELLOS_LOG_W("sello_aplicar: nodo %u en fase %u — los "
                     "sellos son del éil hamilluím (Éx 29:19-20 + "
                     "Lv 8:22-24, fase MILLUIM o posterior)",
                     (unsigned)nodo_id, (unsigned)n->fase_actual);
        return HASHMAL_ONBOARDING_ERR_FASE_INCORRECTA;
    }

    if (n->sellos[sello]) {
        SELLOS_LOG_W("sello_aplicar: nodo %u sello %s ya aplicado "
                     "(irreversible, un solo tenúkh por punto)",
                     (unsigned)nodo_id, nombre_sello(sello));
        return HASHMAL_ONBOARDING_ERR_SELLO_DUPLICADO;
    }

    /* Aplicar - sangre en el tenukh. */
    n->sellos[sello]     = true;
    n->timestamp_fase_ms = hashmal_onboarding_ahora_ms();
    c->timestamp_ms      = n->timestamp_fase_ms;

    SELLOS_LOG_I("nodo %u: sello %s aplicado (Éx 29:20 + Lv 8:23-24)",
                 (unsigned)nodo_id, nombre_sello(sello));

    /* ?Triple sellado completo? */
    if (n->sellos[HASHMAL_ONBOARDING_SELLO_OZEN] &&
        n->sellos[HASHMAL_ONBOARDING_SELLO_BOHEN_YAD] &&
        n->sellos[HASHMAL_ONBOARDING_SELLO_BOHEN_REGEL]) {
        SELLOS_LOG_I("nodo %u: TRIPLE SELLADO completo — "
                     "ózen + bóhen yad + bóhen régel "
                     "(recibir + procesar + transmitir, Éx 29:20 "
                     "+ Lv 14:14 patrón universal)",
                     (unsigned)nodo_id);
    }

    return HASHMAL_ONBOARDING_OK;
}

/* ==================================================================
 * hashmal_onboarding_sello_verificar - bool puro
 *
 * Sin logs. true si el sello indicado esta aplicado en el nodo;
 * false en cualquier otro caso (sin init, fuera de rango,
 * sello fuera de rango, no aplicado).
 * ================================================================== */
bool hashmal_onboarding_sello_verificar(uint8_t nodo_id,
                                         hashmal_onboarding_sello_t sello)
{
    const hashmal_onboarding_t *c =
        hashmal_onboarding_get_consagracion_interno();
    if (c == NULL) return false;
    if (!nodo_id_valido(nodo_id)) return false;
    if (!sello_valido(sello)) return false;
    return c->nodos[nodo_id].sellos[sello];
}

/* ==================================================================
 * hashmal_onboarding_sello_estado - snapshot bool[3]
 *
 * Lectura pura, sin logs. Escribe los 3 flags a out[0..2]:
 *   out[0] = OZEN
 *   out[1] = BOHEN_YAD
 *   out[2] = BOHEN_REGEL
 *
 *   out NULL               -> ERR_NULL
 *   sin init               -> ERR_NO_INIT
 *   nodo_id fuera de rango -> ERR_NODO_INVALIDO
 * ================================================================== */
int hashmal_onboarding_sello_estado(uint8_t nodo_id,
                                     bool out[HASHMAL_ONBOARDING_SELLOS_TOTAL])
{
    if (out == NULL) return HASHMAL_ONBOARDING_ERR_NULL;

    const hashmal_onboarding_t *c =
        hashmal_onboarding_get_consagracion_interno();
    if (c == NULL) return HASHMAL_ONBOARDING_ERR_NO_INIT;

    if (!nodo_id_valido(nodo_id)) {
        return HASHMAL_ONBOARDING_ERR_NODO_INVALIDO;
    }

    const hashmal_onboarding_nodo_t *n = &c->nodos[nodo_id];
    for (uint8_t i = 0; i < HASHMAL_ONBOARDING_SELLOS_TOTAL; i++) {
        out[i] = n->sellos[i];
    }
    return HASHMAL_ONBOARDING_OK;
}

/* ==================================================================
 * hashmal_onboarding_config_firmar - Exodo 29:21 + Levitico 8:30
 *
 * "veHIZZEITA 'al Aharon ve'al BEGADAV" (Ex 29:21) + "vayyaz 'al
 * Aharon 'al begadav" (Lv 8:30). Despues de los 3 sellos en el
 * cuerpo (v.20), se rocia sangre del altar + shemen hamishja sobre
 * las vestiduras. Resultado: "veqadash hu uvegadav" - santificado
 * el y sus vestiduras.
 *
 * Requisitos:
 *   1. Iniciado.
 *   2. nodo_id en rango.
 *   3. Estado == EN_PROCESO.
 *   4. Los 3 sellos del v.20 ya aplicados. Ex 29:21 viene DESPUES
 *      de v.20: el orden es secuencial (v.20 en cuerpo -> v.21 en
 *      vestiduras).
 *   5. Config no firmada todavia (un solo rociado - Lv 8:30 es un
 *      acto unico).
 *
 * Side effects:
 *   - n->config_firmada = true
 *   - n->timestamp_fase_ms = ahora
 *
 * 2+ testigos:
 *   Ex 29:21    - mandato (hizza sobre begadim)
 *   Lv 8:30     - ejecucion (vayyaz, libro distinto)
 *   Ex 29:20->21 - orden textual secuencial
 * ================================================================== */
int hashmal_onboarding_config_firmar(uint8_t nodo_id)
{
    hashmal_onboarding_t *c = hashmal_onboarding_consagracion_mut();
    if (c == NULL) {
        SELLOS_LOG_E("config_firmar: subsistema no iniciado");
        return HASHMAL_ONBOARDING_ERR_NO_INIT;
    }

    if (!nodo_id_valido(nodo_id)) {
        SELLOS_LOG_W("config_firmar: nodo_id %u fuera de rango",
                     (unsigned)nodo_id);
        return HASHMAL_ONBOARDING_ERR_NODO_INVALIDO;
    }

    hashmal_onboarding_nodo_t *n = &c->nodos[nodo_id];

    if (n->estado != HASHMAL_ONBOARDING_EN_PROCESO) {
        SELLOS_LOG_W("config_firmar: nodo %u no EN_PROCESO "
                     "(estado=%u) — firma dentro del pipeline",
                     (unsigned)nodo_id, (unsigned)n->estado);
        return HASHMAL_ONBOARDING_ERR_FASE_INCORRECTA;
    }

    /* Ex 29:21 viene despues de v.20 - sellos primero. */
    for (uint8_t s = 0; s < HASHMAL_ONBOARDING_SELLOS_TOTAL; s++) {
        if (!n->sellos[s]) {
            SELLOS_LOG_W("config_firmar: nodo %u sello %s faltante "
                         "— Éx 29:20 (sellos) viene ANTES de v.21 "
                         "(rociado); Lv 8:23-24 antes de Lv 8:30",
                         (unsigned)nodo_id,
                         nombre_sello((hashmal_onboarding_sello_t)s));
            return HASHMAL_ONBOARDING_ERR_FASE_INCORRECTA;
        }
    }

    if (n->config_firmada) {
        SELLOS_LOG_W("config_firmar: nodo %u config ya firmada "
                     "(Lv 8:30 vayyáz — acto único, no repetible)",
                     (unsigned)nodo_id);
        return HASHMAL_ONBOARDING_ERR_SELLO_DUPLICADO;
    }

    n->config_firmada    = true;
    n->timestamp_fase_ms = hashmal_onboarding_ahora_ms();
    c->timestamp_ms      = n->timestamp_fase_ms;

    SELLOS_LOG_I("nodo %u: config FIRMADA — sangre+shémen hamishjá "
                 "sobre begadím (Éx 29:21 + Lv 8:30)",
                 (unsigned)nodo_id);
    SELLOS_LOG_I("  veqadásh hu uvegadáv — santificado ÉL y sus "
                 "VESTIDURAS (Éx 29:21)");

    return HASHMAL_ONBOARDING_OK;
}

/* ==================================================================
 * hashmal_onboarding_config_firmada - bool puro
 *
 * Sin logs. true si config_firmada; false en cualquier otro caso.
 * ================================================================== */
bool hashmal_onboarding_config_firmada(uint8_t nodo_id)
{
    const hashmal_onboarding_t *c =
        hashmal_onboarding_get_consagracion_interno();
    if (c == NULL) return false;
    if (!nodo_id_valido(nodo_id)) return false;
    return c->nodos[nodo_id].config_firmada;
}

/* ==================================================================
 *
 *   "velaqajTA middamo venataTA 'al TENUKH OZEN... ve'al BOHEN
 *    YADAM haYEMANIT ve'al BOHEN RAGLAM haYEMANIT." - Exodo 29:20
 *
 *   "veHIZZEITA 'al Aharon ve'al BEGADAV... veQADASH hu uvegadav."
 *     - Exodo 29:21
 *
 *   "vayyitten 'al tenukh ozen Aharon hayemanit ve'al bohen yado
 *    hayemanit ve'al bohen raglo hayemanit." - Levitico 8:23
 *
 *   "velaqaj hakohen middam ha'asham veNATAN... 'al tenukh ozen
 *    hammittaher hayemanit..." - Levitico 14:14
 *
 *   La sangre del segundo carnero no se queda en el altar: va al
 *   cuerpo del sacerdote. Oreja para RECIBIR sin corrupcion,
 *   pulgar para PROCESAR con autoridad, dedo del pie para
 *   TRANSMITIR la palabra caminando. Siempre el DERECHO - yemani,
 *   el lado de la autoridad. Y despues, Ex 29:21: sangre + shemen
 *   hamishja rociados sobre las VESTIDURAS mismas. No se
 *   santifica solo el hombre, tambien su configuracion.
 *
 *   Un sello puesto no se quita: irreversible como el tzitz. Los
 *   tres juntos son el triple sellado - recibir + procesar +
 *   transmitir - y solo entonces viene la firma de la config.
 *   El orden v.20 -> v.21 es especificacion: primero el cuerpo,
 *   despues la vestidura. Lv 14:14 confirma el patron en otro
 *   contexto (purificacion del leproso): es diseno universal
 *   del Padre, no evento aislado.
 *
 *   === DAM 'AL TENUKH - consagracion_sellos.c COMPLETO ===
 *
 *   -- Los 5 archivos hermanos -----------------------------------
 *
 *     consagracion_onboarding.c   PIPELINE          - CERO -> ACTIVO
 *     consagracion_fases.c        SHEVA' MA'ASIM    - las 7 fases
 *     consagracion_sellos.c       DAM 'AL TENUKH    - 3 sellos   <- ESTE
 *     consagracion_lockdown.c     VEZAR LO YOKHAL   - Ex 29:33-34
 *     consagracion_cuarentena.c   MIJUTS LAMMAJANEH - Ex 29:14
 *
 *   Archivo 3 de 5 de la Consagracion. Siguiente:
 *     consagracion_lockdown.c - recursos dedicados (Ex 29:33-34)
 *
 *   PASO 9 de Exodo 40:12-15 - Pieza 10, Archivo 3 de 5.
 *
 * ================================================================== */
