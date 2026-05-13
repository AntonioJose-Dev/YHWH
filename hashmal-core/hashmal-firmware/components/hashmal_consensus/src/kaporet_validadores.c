/**
 * ===================================================================
 * PIEZA 02 - KAPORET - kaporet_validadores.c
 * Sub-pieza: LOS QUERUBINES OPERAN - verificacion, testimonio, mirada
 * Exodo 25:18-20 + Genesis 3:24 + Ezequiel 10:12 + Dt 19:15
 * ===================================================================
 *
 * "ve'asita shenayim keruvim zahav miqsha ta'ase otam" - Ex 25:18
 * "Haras DOS querubines de oro, de obra BATIDA los haras."
 *
 * "va'ase keruv ejad miqatsa mizze ukeruv ejad miqatsa mizze min
 *  haKaporet ta'asu et hakeruvim 'al shene qetsotav" - Ex 25:19
 * "Uno en un extremo, otro en el otro; DEL Kaporet haras los
 *  querubines en sus dos extremos."
 *
 * "vehayu hakeruvim porshe kenafayim lema'la sokekim bekanfehem 'al
 *  haKaporet ufnehem ish el ajiv el haKaporet yihyu pene
 *  hakeruvim" - Ex 25:20
 * "Alas extendidas hacia ARRIBA, cubriendo; rostros VARON HACIA SU
 *  HERMANO; hacia el Kaporet seran los rostros."
 *
 * "vayashken miqqedem leGan 'Eden et hakeruvim ve'et lahat hajerev
 *  hammithappejet lishmor et derej 'ets hajayyim" - Gn 3:24
 * "Puso querubines al oriente del Eden y ESPADA encendida que se
 *  revolvia para GUARDAR el camino al arbol de la vida."
 *
 * "vejol besaram vegabbehem vidhehem vejanfehem veha'ofannim mel'im
 *  'enayim saviv" - Ez 10:12
 * "Todo su cuerpo... LLENOS de ojos alrededor." (cf. Ap 4:8)
 *
 * "lo yaqum 'ed ejad be'ish... 'al pi shenayim 'edim o 'al pi
 *  shelosha 'edim yaqum davar" - Dt 19:15
 * "Un solo testigo NO se levantara... por boca de DOS o TRES
 *  testigos se establecera la PALABRA."
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 * Los querubines OPERAN. kaporet_core.c los forjo; aqui los hacen
 * su trabajo:
 *
 *   querubin_verificar     v.18-20 + Gn 3:24 + Ez 10:12
 *                          (?cumple este querubin las 8 propiedades?)
 *   querubin_testigo       v.20 (ish el ajiv) + Dt 19:15
 *                          (registrar lo que un querubin vio)
 *   ojos_total             Ez 10:12 / Ap 4:8
 *                          (suma de auditoria total)
 *   par_presente           v.18 shenayim
 *                          (wrapper: V1 y V2 ambos aptos) - interno
 *   testigos_concuerdan    Dt 19:15 - davar
 *                          (?sus testimonios levantan la Palabra?) - interno
 *
 * --- INDEPENDENCIA ESPACIAL ---
 *
 * v.19 "ejad miqatsa mizze ukeruv ejad miqatsa mizze" - cada uno en
 * SU extremo. Cross-validation no significa sincronizacion: V1
 * observa independiente en qatsa 0, V2 observa independiente en
 * qatsa 1. La concordancia es PRUEBA, no consecuencia.
 *
 * --- P5: VALIDADORES EMERGEN DEL PROTOCOLO (min haKapóret) ---
 *
 * Ex 25:19 - מִן הַכַּפֹּרֶת תַּעֲשׂוּ אֶת הַכְּרֻבִים
 * "min hakKaporet ta'asu et hakeruvim" - "DESDE la Kaporet hareis
 * los querubines." La preposicion min (H4480) indica ORIGEN: los
 * validadores son extensiones EMERGENTES del mismo bloque de oro
 * (miqsha), NO piezas externas colocadas sobre ('al, H5921) ella.
 * Su sustancia ES el protocolo.
 *
 * Par C/C' del quiasmo confirma circuito cerrado:
 *   v.19 emergen DE   (min) la Kapóret
 *   v.20 miran HACIA  (el)  la Kapóret
 *
 * INVARIANTE FIRME (no negociable):
 *   1. Toda la lógica de V1/V2 vive DENTRO del componente
 *      hashmal_consensus. NO se importa de otros componentes.
 *   2. NO hay function pointers en la API publica/interna que
 *      permitan a otro modulo "inyectar" un validador. La
 *      construccion (`construir_querubin` en kaporet_core.c) es la
 *      UNICA fuente de existencia; aqui se les hace operar.
 *   3. NO hay tabla virtual, NO hay registry, NO hay plugin. Si
 *      un futuro componente intenta sobrescribir V1/V2, esta
 *      ausencia de hooks le obliga a parchear estos archivos -
 *      lo que se detecta en code review (espada encendida, Gn 3:24).
 *   4. construir_querubin es identica para V1 y V2 (solo difiere
 *      el id) - ningun nodo "personaliza" sus validadores. Misma
 *      sustancia, mismo comportamiento. miqsha (Ex 25:18, H4749)
 *      = una sola pieza batida; nikrat (Ex 30:32-33) anti-clonacion.
 *
 * CHECK MECANICO: los includes de este archivo se limitan a la
 * libc estandar + esp-idf encapsulado (oro de Egipto) + cabeceras
 * propias del componente. Cualquier #include "hashmal_<otro>.h"
 * (excepto storage para data-check vertical en kaporet_verificacion.c)
 * rompe el invariante.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_consensus.h           -> struct + API publica
 *   hashmal_consensus_internal.h  -> kaporet_mut (singleton mutable)
 *   esp_log.h  (ORO DE EGIPTO)    -> KAP_LOG_*
 *   esp_timer.h (ORO DE EGIPTO)   -> timestamp monotonico (microsegundos)
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
#include "esp_timer.h"

#include "hashmal_consensus.h"
#include "hashmal_consensus_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *KAP_TAG = "hashmal.kaporet.validadores";

#define KAP_LOG_I(fmt, ...)  ESP_LOGI(KAP_TAG, fmt, ##__VA_ARGS__)
#define KAP_LOG_W(fmt, ...)  ESP_LOGW(KAP_TAG, fmt, ##__VA_ARGS__)
#define KAP_LOG_E(fmt, ...)  ESP_LOGE(KAP_TAG, fmt, ##__VA_ARGS__)

/**
 * esp_timer_get_time() encapsulado - timestamp monotonico en
 * microsegundos desde boot. Se trata como u64 (ignoramos el signo
 * del int64_t de origen: no esperamos ~292.000 anos de uptime).
 */
static inline uint64_t ahora_us(void)
{
    return (uint64_t)esp_timer_get_time();
}

/* ==================================================================
 * HELPERS ESTATICOS - indices y validacion de id
 * ================================================================== */

/**
 * v.18 - shenayim (exactamente 2): V1=0 o V2=1. Cualquier otro
 * valor es querubin inventado.
 */
static bool id_legitimo(hashmal_querub_id_t id)
{
    return (id == HASHMAL_QUERUB_V1) || (id == HASHMAL_QUERUB_V2);
}

/**
 * v.19 - "ejad miqatsa mizze ukeruv ejad miqatsa mizze":
 * cada querubin nace EN SU extremo. El indice en el array coincide
 * con el id por construccion en kaporet_core.c. Verificamos la
 * invariante aqui antes de toda operacion.
 */
static bool indice_de_id(hashmal_querub_id_t id, uint8_t *out_idx)
{
    if (!id_legitimo(id)) return false;
    *out_idx = (uint8_t)id;   /* V1=0, V2=1 */
    return true;
}

/* ==================================================================
 * API PUBLICA - querubin_verificar (v.18-20 + Gn 3:24 + Ez 10:12)
 * ================================================================== */

/**
 * hashmal_consensus_querubin_verificar - las 8 propiedades
 *
 * 1. id correcto (V1 o V2)                        - v.19 (qatsa)
 * 2. alas_extendidas_arriba                       - v.20 lema'la
 * 3. alas_cubren                                  - v.20 sokekim
 * 4. mira_al_hermano                              - v.20 ish el ajiv
 * 5. mira_al_kaporet                              - v.20 pene el haK.
 * 6. miqsha_con_kaporet                           - v.18-19 min haK.
 * 7. guardian_no_abridor                          - Gn 3:24 lishmor
 * 8. ojos_total >= HASHMAL_OJOS_MINIMOS           - Ez 10:12 / Ap 4:8
 *
 * Una sola falta -> ERR_QUERUB_AUSENTE. "Ausente" aqui no es
 * necesariamente FISICAMENTE ausente: es ausente DE DISENO -
 * no cumple lo que el Padre especifico, por tanto no cuenta como
 * querubin apto para el Kaporet.
 */
int hashmal_consensus_querubin_verificar(hashmal_querub_id_t id)
{
    const hashmal_kaporet_t *kap = hashmal_consensus_get_kaporet();
    if (kap == NULL) {
        KAP_LOG_E("querubin_verificar: Kapóret no levantado");
        return HASHMAL_KAPORET_ERR_INIT;
    }

    uint8_t idx;
    if (!indice_de_id(id, &idx)) {
        KAP_LOG_E("querubin_verificar: id %d ilegítimo (v.18 shenáyim)",
                  (int)id);
        return HASHMAL_KAPORET_ERR_QUERUB_AUSENTE;
    }

    const hashmal_querub_t *q = &kap->querubin[idx];

    /* 1 - v.19: el id almacenado debe casar con el extremo. */
    if (q->id != id) {
        KAP_LOG_E("querub %d: id interno %d no coincide (v.19 qatsá)",
                  (int)id, (int)q->id);
        return HASHMAL_KAPORET_ERR_QUERUB_AUSENTE;
    }

    /* 2-3 - v.20: alas arriba cubriendo. lema'la + sokekim. */
    if (!q->alas_extendidas_arriba) {
        KAP_LOG_E("querub %d: alas NO lemáʻla (v.20)", (int)id);
        return HASHMAL_KAPORET_ERR_QUERUB_AUSENTE;
    }
    if (!q->alas_cubren) {
        KAP_LOG_E("querub %d: alas NO sokekím (v.20)", (int)id);
        return HASHMAL_KAPORET_ERR_QUERUB_AUSENTE;
    }

    /* 4-5 - v.20: doble atencion (hermano + Kaporet). */
    if (!q->mira_al_hermano) {
        KAP_LOG_E("querub %d: no mira al hermano "
                  "(v.20 ish el ajív)", (int)id);
        return HASHMAL_KAPORET_ERR_QUERUB_AUSENTE;
    }
    if (!q->mira_al_kaporet) {
        KAP_LOG_E("querub %d: no mira al Kapóret "
                  "(v.20 pené el haKapóret)", (int)id);
        return HASHMAL_KAPORET_ERR_QUERUB_AUSENTE;
    }

    /* 6 - v.18-19: nace miqsha con el Kaporet. */
    if (!q->miqsha_con_kaporet) {
        KAP_LOG_E("querub %d: no miqshá con Kapóret "
                  "(v.18-19 min haKapóret)", (int)id);
        return HASHMAL_KAPORET_ERR_QUERUB_AUSENTE;
    }

    /* 7 - Gn 3:24: guardian (lishmor), no abridor. */
    if (!q->guardian_no_abridor) {
        KAP_LOG_E("querub %d: no guardian (Gn 3:24 lishmór)", (int)id);
        return HASHMAL_KAPORET_ERR_QUERUB_AUSENTE;
    }

    /* 8 - Ez 10:12 / Ap 4:8: auditoria total (minimo). */
    if (q->ojos_total < HASHMAL_OJOS_MINIMOS) {
        KAP_LOG_E("querub %d: ojos insuficientes %u (<%u requerido, "
                  "Ez 10:12 / Ap 4:8)",
                  (int)id, (unsigned)q->ojos_total,
                  (unsigned)HASHMAL_OJOS_MINIMOS);
        return HASHMAL_KAPORET_ERR_QUERUB_AUSENTE;
    }

    return HASHMAL_KAPORET_OK;
}

/* ==================================================================
 * API PUBLICA - querubin_testigo (Dt 19:15 + v.20)
 * ================================================================== */

/**
 * hashmal_consensus_querubin_testigo - registrar una observacion
 *
 * Dt 19:15 - "lo yaqum 'ed ejad be'ish... 'al pi shenayim 'edim yaqum
 *  davar": un solo testigo no levanta; dos SI. Aqui cada querubin
 * declara INDIVIDUALMENTE lo que vio; la concordancia la establece
 * `testigos_concuerdan` - funcion SEPARADA porque es juicio distinto
 * del testimonio (primero se testifica, despues se coteja).
 *
 * v.19 "ejad miqatsa mizze" - cada uno en SU extremo. Que V1 y V2
 * se llamen a testificar en secuencia no los acopla: cada uno ve
 * independiente, y el `hash` que entrega proviene de su propia
 * observacion (cada extremo tiene su propia cadena de evidencia;
 * eso lo aseguran los callers - kaporet_verificacion.c).
 *
 * Ez 10:12 - mel'im 'enayim saviv: cada observacion ANADE un ojo.
 * Incrementamos ojos_total del querubin testificante (con saturacion
 * a UINT16_MAX para no envolver). Los ojos crecen con el tiempo:
 * mas ciclos vistos -> mas ojos (historia acumulada).
 *
 * Efectos (via kaporet_mut):
 *   querubin[id].hash_testigo     <- copia de `hash` (32 B)
 *   querubin[id].timestamp_testigo <- ahora_us()
 *   querubin[id].ojos_total       <- +1 (saturando a UINT16_MAX)
 */
int hashmal_consensus_querubin_testigo(hashmal_querub_id_t id,
                                       const uint8_t hash[HASHMAL_ZER_HASH_BYTES])
{
    if (hash == NULL) {
        KAP_LOG_E("querubin_testigo: hash NULL — el testigo declara "
                  "ALGO, no vacío (Dt 19:15)");
        return HASHMAL_KAPORET_ERR_NULL;
    }

    hashmal_kaporet_t *kap = hashmal_consensus_kaporet_mut();
    if (kap == NULL) {
        KAP_LOG_E("querubin_testigo: Kapóret no levantado");
        return HASHMAL_KAPORET_ERR_INIT;
    }

    uint8_t idx;
    if (!indice_de_id(id, &idx)) {
        KAP_LOG_E("querubin_testigo: id %d ilegítimo (v.18 shenáyim)",
                  (int)id);
        return HASHMAL_KAPORET_ERR_QUERUB_AUSENTE;
    }

    /* El querubin que testifica debe estar apto: alas cubriendo,
     * guardian, ojos minimos, etc. Si no esta apto, su testimonio
     * no vale (Dt 19:15 habla de testigos VALIDOS; un testigo
     * descalificado no levanta davar). */
    int rc = hashmal_consensus_querubin_verificar(id);
    if (rc != HASHMAL_KAPORET_OK) {
        KAP_LOG_E("querubin_testigo: querub %d no apto (rc=%d)",
                  (int)id, rc);
        return rc;
    }

    hashmal_querub_t *q = &kap->querubin[idx];

    /* Registrar el testimonio. Sobrescribe el anterior por diseno:
     * cada ciclo nuevo reemplaza el testimonio previo. La historia
     * queda en el log de auditoria (kaporet_verificacion.c, futuro);
     * aqui solo guardamos el testimonio VIGENTE. */
    memcpy(q->hash_testigo, hash, HASHMAL_ZER_HASH_BYTES);
    q->timestamp_testigo = ahora_us();

    /* Ez 10:12 - cada observacion anade un ojo. Saturacion explicita
     * para no envolver uint16_t tras muchos ciclos. */
    if (q->ojos_total < UINT16_MAX) {
        q->ojos_total = (uint16_t)(q->ojos_total + 1u);
    }

    KAP_LOG_I("testigo V%d registrado @%" PRIu64 " us — ojos=%u "
              "(Dt 19:15 / Ez 10:12)",
              (int)id + 1,
              q->timestamp_testigo,
              (unsigned)q->ojos_total);
    return HASHMAL_KAPORET_OK;
}

/* ==================================================================
 * API PUBLICA - ojos_total (Ez 10:12 / Ap 4:8)
 * ================================================================== */

/**
 * hashmal_consensus_ojos_total - suma auditoria de ambos querubines
 *
 * Ez 10:12 - "mel'im 'enayim saviv": llenos de ojos alrededor.
 * Cada querubin acumula sus ojos; el total es la auditoria del
 * Kaporet como conjunto. Cuantos mas ojos, mas observacion historica
 * respalda el sistema.
 *
 * Sin acoplamiento al resultado: un Kaporet con V1 apto y V2 ausente
 * igual suma lo que haya. Es un contador, no un veredicto - para el
 * veredicto esta `par_presente` + `testigos_concuerdan`.
 *
 * @return suma uint32_t (2xuint16_t no desborda) o 0 si no init.
 */
uint32_t hashmal_consensus_ojos_total(void)
{
    const hashmal_kaporet_t *kap = hashmal_consensus_get_kaporet();
    if (kap == NULL) return 0u;

    return (uint32_t)kap->querubin[HASHMAL_QUERUB_V1].ojos_total
         + (uint32_t)kap->querubin[HASHMAL_QUERUB_V2].ojos_total;
}

/* ==================================================================
 * API INTERNA - par_presente (v.18 shenayim)
 * ================================================================== */

/**
 * hashmal_consensus_par_presente - ambos aptos
 *
 * v.18 "shenayim keruvim": no UNO, no TRES. DOS. Y ambos aptos
 * (v.18-20, Gn 3:24, Ez 10:12). Si falta cualquiera, el Kaporet
 * queda sin quien cubra - no se puede invocar voz.
 *
 * Silente: retorna bool sin logear (el caller decide la severidad).
 * Los logs severos los emite querubin_verificar cuando falla.
 */
bool hashmal_consensus_par_presente(void)
{
    if (hashmal_consensus_get_kaporet() == NULL) return false;

    const int v1 = hashmal_consensus_querubin_verificar(HASHMAL_QUERUB_V1);
    const int v2 = hashmal_consensus_querubin_verificar(HASHMAL_QUERUB_V2);
    return (v1 == HASHMAL_KAPORET_OK) && (v2 == HASHMAL_KAPORET_OK);
}

/* ==================================================================
 * API INTERNA - testigos_concuerdan (Dt 19:15 - davar)
 * ================================================================== */

/**
 * hashmal_consensus_testigos_concuerdan - ?yaqum davar?
 *
 * Dt 19:15 - "'al pi shenayim 'edim o 'al pi shelosha 'edim yaqum
 *  davar". Aqui, dos querubines: si ambos han declarado Y sus hashes
 * coinciden (32 B byte a byte), la Palabra SE LEVANTA.
 *
 * NO logea (defensivo, silencioso). El veredicto final + emision lo
 * hace kaporet_verificacion.c; aqui solo se responde al juicio.
 */
bool hashmal_consensus_testigos_concuerdan(void)
{
    const hashmal_kaporet_t *kap = hashmal_consensus_get_kaporet();
    if (kap == NULL) return false;

    const hashmal_querub_t *v1 = &kap->querubin[HASHMAL_QUERUB_V1];
    const hashmal_querub_t *v2 = &kap->querubin[HASHMAL_QUERUB_V2];

    /* "lo yaqum 'ed ejad" - un solo testigo NO levanta. Si alguno
     * aun no declaro (timestamp 0), la Palabra no se establece. */
    if (v1->timestamp_testigo == 0u) return false;
    if (v2->timestamp_testigo == 0u) return false;

    /* Ambos declararon - ?concuerdan? */
    return memcmp(v1->hash_testigo,
                  v2->hash_testigo,
                  HASHMAL_ZER_HASH_BYTES) == 0;
}
