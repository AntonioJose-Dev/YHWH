/*
 * ===================================================================
 * PIEZA 07 - EL ATRIO - atrio_orchim.c
 * Añadidura 21: HACHNASAT ORCHIM - Hospitalidad
 * Genesis 18:1-15 + Levitico 19:33-34 + Hebreos 13:2 + 1 Pedro 4:9
 * ===================================================================
 *
 * "vayyera elav YHWH be'eilonei Mamre vehu yoshev petaj haohel
 *  kejom hayyom; vayyissa 'einav VAYYAR vehinneh shloshah anashim
 *  nitsavim 'alav VAYYAR VAYYARATZ liqratam mippetaj haohel
 *  vayyishtaju artsah." - Genesis 18:1-2
 * "Se le aparecio YHWH en los terebintos de Mamre, mientras Avraham
 *  estaba sentado a la entrada de la tienda en el calor del dia.
 *  Alzo sus ojos y VIO a tres varones de pie junto a el; al verlos
 *  CORRIO a recibirlos desde la entrada de la tienda y se inclino
 *  a tierra."
 *
 * "vayyomar Adonai im na matsati jen be'eineikha al na ta'avor
 *  me'al 'avdekha. yuqqaj na ME'AT MAYIM verajatsu ragleikhem
 *  vehishsha'anu tajat ha'ets. veeqejah PAT LEJEM vesa'adu libbekhem
 *  AJAR TA'AVORU ki 'al ken 'avartem 'al 'avdekhem." - Genesis 18:3-5
 * "Senor, si he hallado gracia en tus ojos, te ruego que no pases
 *  de tu siervo. Tomen un POCO DE AGUA y laven sus pies, descansen
 *  bajo el arbol; tomare un BOCADO DE PAN y sustentaran su corazon,
 *  DESPUES SEGUIRAN su camino."
 *
 * "tes PHILOXENIAS me epilanthanesthe; dia tautes gar elathon tines
 *  XENISANTES AGGELOUS." - Hebreos 13:2
 * "No olvideis la HOSPITALIDAD; por ella algunos hospedaron ANGELES
 *  sin saberlo." (referencia explicita a Avraham y Sodoma)
 *
 * "vekhi yagur itkha GER be'artsekhem LO TONU oto. ke'EZRAJ mikkem
 *  yiheyeh lakhem ha'GER hagger itkhem ve'AHAVTA LO KAMOKHA ki
 *  gerim heyitem be'erets Mitsrayim." - Levitico 19:33-34
 * "Cuando el EXTRANJERO more con vosotros NO LO OPRIMAIS; como a
 *  un NATIVO sera el extranjero que mora con vosotros, y lo
 *  AMARAS COMO A TI MISMO, porque extranjeros fuisteis en Egipto."
 *
 * "PHILOXENOI eis allelous ANEU GOGGYSMOU." - 1 Pedro 4:9
 * "Sed HOSPITALARIOS los unos a los otros SIN MURMURACIONES."
 *
 * --- EXEGESIS OPERATIVA ---
 *
 *   vayyar vayyaratz (H7200+H7323) "VIO Y CORRIO." Inmediatez
 *                             absoluta. Avraham no espero al
 *                             tercer signo - vio y respondio.
 *
 *   me'at mayim, pat lejem (H4592+H4325, H6595+H3899)
 *                             "POCO DE AGUA, BOCADO DE PAN." Lo
 *                             minimo necesario. Avraham PROMETIO
 *                             poco; DIO mucho (Gn 18:6-8: tres
 *                             medidas, becerro, manteca, leche).
 *                             En el firmware: el contrato es
 *                             minimo, la actitud generosa.
 *
 *   ajar ta'avoru (H309+H5674) "DESPUES SEGUIREIS." La hospitalidad
 *                             es TEMPORAL. No casa para siempre,
 *                             no compromiso indefinido. Bocado +
 *                             descanso + camino.
 *
 *   philoxenia (G5381)        "AMOR AL EXTRANJERO." Lit. philos
 *                             (amigo) + xenos (extrano). El
 *                             extrano se TRATA como amigo, sin
 *                             requerir que primero deje de serlo.
 *
 *   xenisantes aggelous (G3579+G32) "HOSPEDARON ANGELES." Heb 13:2
 *                             advierte: el visitante puede ser mas
 *                             de lo que parece. No rechazar por
 *                             desconocido.
 *
 *   ger / ezraj (H1616 / H249) EXTRANJERO / NATIVO. Lv 19:34
 *                             ordena tratar al ger como ezraj. La
 *                             igualdad operativa es mandato, no
 *                             concesion.
 *
 *   aneu goggysmou (G427+G1112) "SIN MURMURACIONES." 1 P 4:9 -
 *                             la hospitalidad sin queja interior.
 *                             El servicio se da con corazon.
 *
 * --- COEXISTENCIA CON MIQLAT (Añadidura 05) ---
 *
 *   Miqlat (Nm 35):  refugio para nodo en peligro que conoce la red.
 *                    Historia conocida, peligro presente.
 *
 *   Orchim (Gn 18):  primer contacto con desconocido que aparece.
 *                    Sin historia, sin peligro declarado, solo
 *                    presencia nueva.
 *
 * Ambos viven en Pieza 07 (perimeter) porque ambos son lo PRIMERO
 * que cruza la frontera del nodo. Pero responden a condiciones
 * distintas y siguen caminos distintos.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_perimeter.h        API publica
 *   hashmal_perim_internal.h   ahora_ms
 *   esp_log.h (ORO EGIPTO)     ORCH_LOG_*
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

#include "hashmal_perimeter.h"
#include "hashmal_perim_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *ORCH_TAG = "hashmal.atrio.orchim";

#define ORCH_LOG_I(fmt, ...)  ESP_LOGI(ORCH_TAG, fmt, ##__VA_ARGS__)
#define ORCH_LOG_W(fmt, ...)  ESP_LOGW(ORCH_TAG, fmt, ##__VA_ARGS__)
#define ORCH_LOG_E(fmt, ...)  ESP_LOGE(ORCH_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * Estado interno - tabla de visitantes + contadores historicos
 * ================================================================== */
static hashmal_orchim_visitante_t g_orchim[HASHMAL_ORCHIM_MAX];
static uint32_t                   g_total_recibidos  = 0;
static uint32_t                   g_total_unidos     = 0;
static uint32_t                   g_total_idos       = 0;
static uint32_t                   g_total_expirados  = 0;

/* ==================================================================
 * Helpers estaticos
 * ================================================================== */

static const char *nombre_estado(hashmal_orchim_estado_t e)
{
    switch (e) {
    case HASHMAL_ORCHIM_DETECTADO:  return "DETECTADO (Gn 18:2 vayyar)";
    case HASHMAL_ORCHIM_BIENVENIDO: return "BIENVENIDO (Gn 18:2 vayyaratz)";
    case HASHMAL_ORCHIM_SERVIDO:    return "SERVIDO (Gn 18:4-5 me'at mayim)";
    case HASHMAL_ORCHIM_DECIDIDO:   return "DECIDIDO (Gn 18:5 ajar ta'avoru)";
    case HASHMAL_ORCHIM_EXPIRADO:   return "EXPIRADO";
    case HASHMAL_ORCHIM_VACIO:
    default:                         return "VACIO";
    }
}

static const char *nombre_decision(hashmal_orchim_decision_t d)
{
    switch (d) {
    case HASHMAL_ORCHIM_DECISION_UNIRSE: return "UNIRSE (consagracion)";
    case HASHMAL_ORCHIM_DECISION_IRSE:   return "IRSE (ajar ta'avoru)";
    default:                              return "<decision desconocida>";
    }
}

static bool slot_legitimo(uint8_t slot)
{
    return slot < HASHMAL_ORCHIM_MAX;
}

/* Slot libre = VACIO o terminal (DECIDIDO/EXPIRADO). Prefiere VACIO. */
static int buscar_slot_libre(void)
{
    for (uint8_t i = 0; i < HASHMAL_ORCHIM_MAX; ++i) {
        if (g_orchim[i].estado == HASHMAL_ORCHIM_VACIO) return (int)i;
    }
    for (uint8_t i = 0; i < HASHMAL_ORCHIM_MAX; ++i) {
        hashmal_orchim_estado_t e = g_orchim[i].estado;
        if (e == HASHMAL_ORCHIM_DECIDIDO || e == HASHMAL_ORCHIM_EXPIRADO) {
            return (int)i;
        }
    }
    return -1;
}

static bool estado_es_activo(hashmal_orchim_estado_t e)
{
    return e == HASHMAL_ORCHIM_DETECTADO
        || e == HASHMAL_ORCHIM_BIENVENIDO
        || e == HASHMAL_ORCHIM_SERVIDO;
}

/* ==================================================================
 * API - hashmal_perimeter_orchim_detectar (Gn 18:2)
 * ================================================================== */
int hashmal_perimeter_orchim_detectar(uint8_t *out_slot)
{
    int slot = buscar_slot_libre();
    if (slot < 0) {
        ORCH_LOG_E("detectar: tabla llena (%u slots, terminales no "
                   "purgados)", (unsigned)HASHMAL_ORCHIM_MAX);
        return HASHMAL_PERIM_ERR_ORCHIM;
    }

    int64_t ahora = hashmal_perim_ahora_ms();
    /* Gn 18:2 - "vayyar vayyaratz": vio Y corrio, sin demora.
     * Marcamos BIENVENIDO en el mismo acto que DETECTAMOS. */
    g_orchim[slot].estado               = HASHMAL_ORCHIM_BIENVENIDO;
    g_orchim[slot].timestamp_llegada_ms = ahora;
    g_orchim[slot].ciclos_restantes     = HASHMAL_ORCHIM_CICLOS_DEFAULT;
    g_orchim[slot].decision             = HASHMAL_ORCHIM_DECISION_IRSE;  /* default seguro */
    g_total_recibidos                  += 1u;

    if (out_slot != NULL) *out_slot = (uint8_t)slot;

    ORCH_LOG_I("detectar: nuevo visitante slot=%d -> BIENVENIDO "
               "(Gn 18:2 vayyar vayyaratz); %u ciclos de hospitalidad",
               slot, (unsigned)HASHMAL_ORCHIM_CICLOS_DEFAULT);
    return HASHMAL_PERIM_OK;
}

/* ==================================================================
 * API - hashmal_perimeter_orchim_servir (Gn 18:4-5)
 * ================================================================== */
int hashmal_perimeter_orchim_servir(uint8_t slot)
{
    if (!slot_legitimo(slot)) return HASHMAL_PERIM_ERR_ORCHIM;

    if (g_orchim[slot].estado != HASHMAL_ORCHIM_BIENVENIDO) {
        ORCH_LOG_W("servir: slot %u en estado %s (esperaba BIENVENIDO)",
                   (unsigned)slot, nombre_estado(g_orchim[slot].estado));
        return HASHMAL_PERIM_ERR_ORCHIM;
    }

    g_orchim[slot].estado = HASHMAL_ORCHIM_SERVIDO;

    ORCH_LOG_I("servir: slot %u -> SERVIDO (Gn 18:4 me'at mayim, "
               "Gn 18:5 pat lejem); acceso minimo otorgado",
               (unsigned)slot);
    return HASHMAL_PERIM_OK;
}

/* ==================================================================
 * API - hashmal_perimeter_orchim_decidir (Gn 18:5 ajar ta'avoru)
 * ================================================================== */
int hashmal_perimeter_orchim_decidir(uint8_t slot,
                                      hashmal_orchim_decision_t decision)
{
    if (!slot_legitimo(slot)) return HASHMAL_PERIM_ERR_ORCHIM;

    if (decision != HASHMAL_ORCHIM_DECISION_UNIRSE
     && decision != HASHMAL_ORCHIM_DECISION_IRSE) {
        return HASHMAL_PERIM_ERR_ORCHIM;
    }

    hashmal_orchim_estado_t prev = g_orchim[slot].estado;
    if (prev != HASHMAL_ORCHIM_SERVIDO && prev != HASHMAL_ORCHIM_BIENVENIDO) {
        ORCH_LOG_W("decidir: slot %u en estado %s (esperaba SERVIDO o "
                   "BIENVENIDO)", (unsigned)slot, nombre_estado(prev));
        return HASHMAL_PERIM_ERR_ORCHIM;
    }

    g_orchim[slot].estado   = HASHMAL_ORCHIM_DECIDIDO;
    g_orchim[slot].decision = decision;

    if (decision == HASHMAL_ORCHIM_DECISION_UNIRSE) {
        g_total_unidos += 1u;
        ORCH_LOG_I("decidir: slot %u -> %s; visitante inicia "
                   "consagracion (fase 2 wirea Pieza 10 onboarding)",
                   (unsigned)slot, nombre_decision(decision));
    } else {
        g_total_idos += 1u;
        ORCH_LOG_I("decidir: slot %u -> %s; visitante sigue su camino "
                   "(Gn 18:5 ajar ta'avoru, sin rencor)",
                   (unsigned)slot, nombre_decision(decision));
    }
    return HASHMAL_PERIM_OK;
}

/* ==================================================================
 * API - hashmal_perimeter_orchim_expirar (Gn 18:5 - temporal)
 * ================================================================== */
int hashmal_perimeter_orchim_expirar(uint8_t slot)
{
    if (!slot_legitimo(slot)) return HASHMAL_PERIM_ERR_ORCHIM;

    hashmal_orchim_estado_t prev = g_orchim[slot].estado;
    if (!estado_es_activo(prev)) {
        ORCH_LOG_W("expirar: slot %u en estado %s (no activo)",
                   (unsigned)slot, nombre_estado(prev));
        return HASHMAL_PERIM_ERR_ORCHIM;
    }

    g_orchim[slot].estado = HASHMAL_ORCHIM_EXPIRADO;
    g_total_expirados   += 1u;

    ORCH_LOG_I("expirar: slot %u -> EXPIRADO (hospitalidad TEMPORAL, "
               "Gn 18:5 ajar ta'avoru); acceso cerrado",
               (unsigned)slot);
    return HASHMAL_PERIM_OK;
}

/* ==================================================================
 * API - hashmal_perimeter_orchim_es_sospechoso (fase 1: stub)
 * ================================================================== */
bool hashmal_perimeter_orchim_es_sospechoso(uint8_t slot)
{
    /* Fase 1: sin metricas de comportamiento, todos pasan. Fase 2
     * wireara con Tsofeh (Añadidura 02) para vigilancia activa
     * durante la estancia. Heb 13:2 nos recuerda no rechazar por
     * desconocido - "algunos hospedaron angeles sin saberlo." */
    (void)slot;
    return false;
}

/* ==================================================================
 * API - hashmal_perimeter_orchim_info (lectura pura)
 * ================================================================== */
int hashmal_perimeter_orchim_info(hashmal_orchim_info_t *out)
{
    if (out == NULL) return HASHMAL_PERIM_ERR_NULL;

    uint8_t activos   = 0;
    uint8_t decididos = 0;
    for (uint8_t i = 0; i < HASHMAL_ORCHIM_MAX; ++i) {
        switch (g_orchim[i].estado) {
        case HASHMAL_ORCHIM_DETECTADO:
        case HASHMAL_ORCHIM_BIENVENIDO:
        case HASHMAL_ORCHIM_SERVIDO:
            ++activos;
            break;
        case HASHMAL_ORCHIM_DECIDIDO:
            ++decididos;
            break;
        default:
            break;
        }
    }

    out->visitantes_activos    = activos;
    out->visitantes_decididos  = decididos;
    out->total_recibidos       = g_total_recibidos;
    out->total_unidos          = g_total_unidos;
    out->total_idos            = g_total_idos;
    out->total_expirados       = g_total_expirados;
    return HASHMAL_PERIM_OK;
}

/* ==================================================================
 *
 *   "VAYYAR VAYYARATZ liqratam mippetaj haohel." - Genesis 18:2
 *
 *   "yuqqaj na ME'AT MAYIM... veeqejah PAT LEJEM." - Genesis 18:4-5
 *
 *   "AJAR TA'AVORU." - Genesis 18:5
 *
 *   "PHILOXENIAS me epilanthanesthe; XENISANTES AGGELOUS." - Heb 13:2
 *
 *   "ke'EZRAJ mikkem yiheyeh lakhem ha'GER... ve'AHAVTA LO KAMOKHA."
 *     - Levitico 19:33-34
 *
 *   "PHILOXENOI eis allelous ANEU GOGGYSMOU." - 1 Pedro 4:9
 *
 *   Avraham es el modelo. Vio y CORRIO - sin demora, sin
 *   verificar primero quien era el visitante. Ofrecio MENOS de
 *   lo que dio: "un poco de agua, un bocado de pan" - y termino
 *   sirviendo becerro tierno con manteca y leche. La hospitalidad
 *   biblica promete poco y da mucho.
 *
 *   Pero tambien es TEMPORAL: "ajar ta'avoru" - despues seguireis.
 *   No casa para siempre. La generosidad del momento no obliga
 *   permanencia. Y por eso el firmware respeta dos verbos: SERVIR
 *   (acceso minimo, suficiente) y EXPIRAR (cuando el tiempo
 *   acaba, el visitante no se queda atrapado en limbo).
 *
 *   Heb 13:2 cierra: el visitante puede ser angel sin que tu lo
 *   sepas. No rechazar por desconocido es la regla. Y 1 P 4:9
 *   completa: sin murmuraciones - el servicio con corazon
 *   limpio.
 *
 * ================================================================== */
