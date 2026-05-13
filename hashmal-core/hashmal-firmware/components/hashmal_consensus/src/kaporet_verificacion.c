/**
 * ===================================================================
 * PIEZA 02 - KAPORET - kaporet_verificacion.c
 * Sub-pieza: EL CICLO DE LEVITICO 16 - la voz emana entre los dos
 * Lv 16:12-14 + Ex 25:22 + Nm 7:89 + Dt 19:15 + Rom 3:25
 * ===================================================================
 *
 * "velaqaj melo hamajta gajle esh me'al hammizbeaj millifne YHWH
 *  umlo' jofnav qetoret sammim daqqa vehebi mibeit lapparojet.
 *  Venatan et haqetoret 'al ha'esh lifne YHWH VEJISSA 'ANAN
 *  haqetoret et haKaporet asher 'al ha'edut VELO YAMUT" - Lv 16:12-13
 * "...y la NUBE del incienso CUBRIRA el Kaporet que esta sobre el
 *  Testimonio, PARA QUE NO MUERA."
 *
 * "velaqaj middam happar vehiza ve'etsba'o 'AL pene haKaporet
 *  QEDMA veLIFNE haKaporet yaze min haddam be'etsba'o SHEVA
 *  PE'AMIM" - Lv 16:14
 * "...rociara con su dedo SOBRE la faz del Kaporet al lado oriental,
 *  y DELANTE del Kaporet rociara SIETE VECES."
 *
 * "veno'adti leja sham vedibarti iteja me'al haKaporet MIBEIN
 *  shene hakeruvim..." - Ex 25:22
 * "Me encontrare contigo alli y te hablare desde ENCIMA del Kaporet,
 *  desde ENTRE los dos querubines..."
 *
 * "vayyishma' et haQol MIDABBER elav me'al haKaporet asher 'al aron
 *  ha'edut MIBEIN shene hakeruvim vaydabber elav" - Nm 7:89
 * "Oyo la VOZ que le HABLABA (midabber - HITPAEL: se hablaba a si
 *  misma) desde encima del Kaporet, desde ENTRE los dos querubines."
 *
 * "'al pi shenayim 'edim o 'al pi shelosha 'edim YAQUM DAVAR" - Dt 19:15
 * "Por boca de DOS o TRES testigos se LEVANTARA la PALABRA."
 *
 * --- EL CICLO COMPLETO ---
 *
 *   Paso 1 (Lv 16:12-13) - qetoret_preparar:  'anan cubriendo
 *   Paso 2a (Lv 16:14a)  - dam_rociar(SOBRE): 'al pene, 1 vez
 *   Paso 2b (Lv 16:14b)  - dam_rociar(DELANTE): lifne, 7 veces
 *   Paso 3 (Ex 25:22 + Nm 7:89 + Dt 19:15) - invocar: la voz emana
 *
 * Cada paso sin el anterior -> rechazo con su leccion ajarei mot.
 *
 * --- POR QUE LA VOZ "EMANA" ---
 *
 * Nm 7:89 midabber = Hitpael reflexivo. La voz se habla a si misma -
 * no tiene director. En firmware: el resultado de invocar ES la
 * concordancia entre V1 y V2, no una decision tomada por algun
 * arbitro. memcpy() de V1.hash_testigo a out->verdad solo ocurre
 * CUANDO los dos concuerdan; el codigo no EMITE la voz, la
 * DESCUBRE.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_consensus.h           -> struct + API publica
 *   hashmal_consensus_internal.h  -> kaporet_mut, par_presente,
 *                                   testigos_concuerdan,
 *                                   es_fuego_extrano, ajarei_mot
 *   esp_log.h  (ORO DE EGIPTO)    -> KAP_LOG_*
 *   esp_timer.h (ORO DE EGIPTO)   -> timestamp de la voz
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

#include "hashmal_storage.h"   /* el hakkappóret — data-check vertical (v.20) */
#include "hashmal_consensus.h"
#include "hashmal_consensus_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *KAP_TAG = "hashmal.kaporet.verificacion";

#define KAP_LOG_I(fmt, ...)  ESP_LOGI(KAP_TAG, fmt, ##__VA_ARGS__)
#define KAP_LOG_W(fmt, ...)  ESP_LOGW(KAP_TAG, fmt, ##__VA_ARGS__)
#define KAP_LOG_E(fmt, ...)  ESP_LOGE(KAP_TAG, fmt, ##__VA_ARGS__)

/**
 * Timestamp monotonico en microsegundos. Encapsula esp_timer.
 */
static inline uint64_t ahora_us(void)
{
    return (uint64_t)esp_timer_get_time();
}

/* ==================================================================
 * HELPER - reset del ciclo Lv 16 tras consumarse
 *
 * Lv 16:17 - "vejol adam lo yihye be'ohel mo'ed bebo'o lejapper
 * baqodesh 'ad tseto": el sumo sacerdote entra, propicia, y SALE.
 * El ciclo TERMINA. Aqui tambien: tras invocar, limpiamos la
 * invocacion para que el proximo ciclo empiece en reposo.
 *
 * Se llama SOLO despues de una invocacion consumada (EMITIDO o
 * DIVERGIDO - ambas cuentan como ciclo ejecutado). Si la invocacion
 * falla por precondicion, NO se resetea: el ciclo no se consumo.
 * ================================================================== */
static void reset_ciclo(hashmal_kaporet_t *kap)
{
    kap->invocacion.qetoret             = HASHMAL_QETORET_AUSENTE;
    kap->invocacion.rociados_delante    = 0u;
    kap->invocacion.rociado_sobre_hecho = false;
    kap->invocacion.moed_abierto       = false;
    /* Nota: hash_testigo de los querubines NO se limpia aqui. El
     * proximo ciclo los reescribira via querubin_testigo(); si
     * alguien intenta invocar sin rociar antes, rociados_delante=0
     * y rociado_sobre_hecho=false detendran la invocacion. */
}

/* ==================================================================
 * API PUBLICA - qetoret_preparar (Lv 16:12-13 - paso 1)
 * ================================================================== */

/**
 * hashmal_consensus_qetoret_preparar - la nube PRIMERO
 *
 * "vejissa 'anan haqetoret et haKaporet asher 'al ha'edut
 *  velo yamut" - Lv 16:13
 * "La nube del incienso CUBRIRA el Kaporet que esta sobre el
 *  Testimonio, PARA QUE NO MUERA."
 *
 * La nube obligatoria ANTES de todo acercamiento. Sin ella, cualquier
 * intento posterior seria Nadav y Avihu (kaporet_ataques.c lo detecta).
 *
 * --- PRECONDICIONES ---
 *
 *   - Kaporet levantado
 *   - Kaporet sobre el Arca (v.21): sin ha'aron, no hay sobre que
 *     poner la nube
 *   - Par presente (v.18): sin V1/V2 aptos, sokekim es vacio
 *
 * --- EFECTO ---
 *
 *   invocacion.qetoret = HASHMAL_QETORET_PRESENTE
 *
 * Idempotente: si ya esta presente, no-op ruidoso.
 */
int hashmal_consensus_qetoret_preparar(void)
{
    hashmal_kaporet_t *kap = hashmal_consensus_kaporet_mut();
    if (kap == NULL) {
        KAP_LOG_E("qetoret_preparar: Kapóret no levantado");
        return HASHMAL_KAPORET_ERR_INIT;
    }

    /* Idempotencia - la nube ya cubre; no re-encendemos el fuego. */
    if (kap->invocacion.qetoret == HASHMAL_QETORET_PRESENTE) {
        KAP_LOG_W("qetoret_preparar: nube ya presente (idempotente, no-op)");
        return HASHMAL_KAPORET_OK;
    }

    /* v.21 - sin ha'aron el Kaporet no esta en su lugar; poner nube
     * sobre aire no cubre nada. */
    if (!kap->sobre_el_arca) {
        KAP_LOG_E("qetoret_preparar: Kapóret NO sobre el Arca (Éx 25:21)");
        hashmal_consensus_ajarei_mot(HASHMAL_KAPORET_ERR_SIN_ARCA);
        return HASHMAL_KAPORET_ERR_SIN_ARCA;
    }

    /* v.18-20 - sokekim requiere querubines aptos. */
    if (!hashmal_consensus_par_presente()) {
        KAP_LOG_E("qetoret_preparar: par no apto "
                  "(Éx 25:18-20 — sin sokekím)");
        hashmal_consensus_ajarei_mot(HASHMAL_KAPORET_ERR_QUERUB_AUSENTE);
        return HASHMAL_KAPORET_ERR_QUERUB_AUSENTE;
    }

    kap->invocacion.qetoret = HASHMAL_QETORET_PRESENTE;

    KAP_LOG_I("═══ qetóret preparada (Lv 16:12-13) ═══");
    KAP_LOG_I("  'vejissá ʻanán haqetóret et haKapóret'");
    KAP_LOG_I("  'velo yamút' — la secuencia salva la vida");
    return HASHMAL_KAPORET_OK;
}

/* ==================================================================
 * API PUBLICA - dam_rociar (Lv 16:14 - paso 2)
 * ================================================================== */

/**
 * hashmal_consensus_dam_rociar - la sangre sobre y delante
 *
 * "velaqaj middam happar vehiza ve'etsba'o 'AL pene haKaporet
 *  qedma veLIFNE haKaporet yaze min haddam be'etsba'o sheva
 *  pe'amim" - Lv 16:14
 *
 * Cada rocio = un hash (prueba verificada). El SOBRE va a V1; los
 * DELANTE van a V2 (se sobrescribe cada vez; el ultimo DELANTE es
 * el que V2 llevara a la invocacion). Si V1 y V2 reciben el MISMO
 * hash (el mismo dam), testigos_concuerdan -> true.
 *
 * --- ORDEN OBLIGATORIO ---
 *
 *   1o  SOBRE    ('al pene haKaporet qedma) - UNA vez, unica
 *   2o  DELANTE  (lifne haKaporet)          - hasta SHEVA pe'amim
 *
 * Intentar DELANTE antes de SOBRE -> ERR_SIN_DAM (orden violado).
 * Intentar SOBRE dos veces -> idempotente (no-op ruidoso).
 * Intentar mas de 7 DELANTE -> cap + warning (sheva pe'amim cerrado).
 * Modo PROHIBIDO o valor fuera del enum -> ERR_FUEGO_EXTRANO.
 *
 * --- PRECONDICIONES ---
 *
 *   - hash != NULL (el dam no es vacio)
 *   - Kaporet levantado
 *   - qetoret presente (paso 1 hecho)
 *   - par presente
 *
 * --- DELEGACION ---
 *
 * No escribimos hash_testigo directo. Usamos querubin_testigo() -
 * valida aptitud del querubin, incrementa ojos (Ez 10:12), registra
 * timestamp. La contabilidad de ceremonia (SOBRE hecho, cuenta
 * DELANTE) se actualiza aqui.
 */
int hashmal_consensus_dam_rociar(hashmal_acceso_kaporet_t modo,
                                 const uint8_t hash[HASHMAL_ZER_HASH_BYTES])
{
    if (hash == NULL) {
        KAP_LOG_E("dam_rociar: hash NULL — dam sin contenido (Lv 16:14)");
        return HASHMAL_KAPORET_ERR_NULL;
    }

    hashmal_kaporet_t *kap = hashmal_consensus_kaporet_mut();
    if (kap == NULL) {
        KAP_LOG_E("dam_rociar: Kapóret no levantado");
        return HASHMAL_KAPORET_ERR_INIT;
    }

    /* Lv 16:13 - qetoret ANTES. Rociar sin la nube es Nadav y Avihu. */
    if (kap->invocacion.qetoret != HASHMAL_QETORET_PRESENTE) {
        KAP_LOG_E("dam_rociar: sin qetóret preparada — paso 1 pendiente");
        hashmal_consensus_ajarei_mot(HASHMAL_KAPORET_ERR_SIN_QETORET);
        return HASHMAL_KAPORET_ERR_SIN_QETORET;
    }

    /* v.18 - sin par apto no hay a quien rociar. */
    if (!hashmal_consensus_par_presente()) {
        KAP_LOG_E("dam_rociar: par no apto (v.18 shenáyim)");
        hashmal_consensus_ajarei_mot(HASHMAL_KAPORET_ERR_QUERUB_AUSENTE);
        return HASHMAL_KAPORET_ERR_QUERUB_AUSENTE;
    }

    switch (modo) {

    case HASHMAL_ACCESO_DAM_SOBRE: {
        /* Lv 16:14a - 'al pene haKaporet qedma (una sola vez). */
        if (kap->invocacion.rociado_sobre_hecho) {
            KAP_LOG_W("dam_rociar SOBRE: ya hecho en este ciclo "
                      "(idempotente, no-op)");
            return HASHMAL_KAPORET_OK;
        }

        /* El SOBRE queda en V1 (el primer testigo). */
        int rc = hashmal_consensus_querubin_testigo(HASHMAL_QUERUB_V1, hash);
        if (rc != HASHMAL_KAPORET_OK) {
            KAP_LOG_E("dam_rociar SOBRE: V1 rechazó testimonio (rc=%d)", rc);
            hashmal_consensus_ajarei_mot((hashmal_kaporet_err_t)rc);
            return rc;
        }
        kap->invocacion.rociado_sobre_hecho = true;

        KAP_LOG_I("dam rociado SOBRE (Lv 16:14a): ʻal pené haKapóret qédma");
        return HASHMAL_KAPORET_OK;
    }

    case HASHMAL_ACCESO_DAM_DELANTE: {
        /* Orden obligatorio: SOBRE antes que DELANTE. */
        if (!kap->invocacion.rociado_sobre_hecho) {
            KAP_LOG_E("dam_rociar DELANTE: SOBRE va primero (Lv 16:14 orden)");
            hashmal_consensus_ajarei_mot(HASHMAL_KAPORET_ERR_SIN_DAM);
            return HASHMAL_KAPORET_ERR_SIN_DAM;
        }

        /* Lv 16:14b - sheva pe'amim. Cap al septimo. */
        if (kap->invocacion.rociados_delante >= HASHMAL_VERIFICACIONES_SHEVA) {
            KAP_LOG_W("dam_rociar DELANTE: %d/%d completos (shéva peʻamím "
                      "ya cumplidos, no se añade más)",
                      (int)kap->invocacion.rociados_delante,
                      HASHMAL_VERIFICACIONES_SHEVA);
            return HASHMAL_KAPORET_OK;
        }

        /* Cada DELANTE actualiza V2 (ultimo rocio gana). */
        int rc = hashmal_consensus_querubin_testigo(HASHMAL_QUERUB_V2, hash);
        if (rc != HASHMAL_KAPORET_OK) {
            KAP_LOG_E("dam_rociar DELANTE: V2 rechazó testimonio (rc=%d)", rc);
            hashmal_consensus_ajarei_mot((hashmal_kaporet_err_t)rc);
            return rc;
        }
        kap->invocacion.rociados_delante += 1u;

        KAP_LOG_I("dam rociado DELANTE %u/%d (Lv 16:14b lifné haKapóret)",
                  (unsigned)kap->invocacion.rociados_delante,
                  HASHMAL_VERIFICACIONES_SHEVA);
        return HASHMAL_KAPORET_OK;
    }

    case HASHMAL_ACCESO_PROHIBIDO:
    default:
        /* Cualquier modo fuera de SOBRE/DELANTE es esh zara - ningun
         * rociado "creativo" fue mandado. Lv 10:1 "asher lo tsivva". */
        KAP_LOG_E("dam_rociar: modo %d PROHIBIDO — esh zará (Lv 10:1)",
                  (int)modo);
        hashmal_consensus_ajarei_mot(HASHMAL_KAPORET_ERR_FUEGO_EXTRANO);
        return HASHMAL_KAPORET_ERR_FUEGO_EXTRANO;
    }
}

/* ==================================================================
 * API PUBLICA - invocar (Ex 25:22 + Nm 7:89 + Dt 19:15 - paso 3)
 * ================================================================== */

/**
 * hashmal_consensus_invocar - la voz emana entre los dos
 *
 * "veno'adti leja sham vedibarti iteja me'al haKaporet MIBEIN
 *  shene hakeruvim" - Ex 25:22
 * "vayyishma' et haQol midabber elav... mibein shene
 *  hakeruvim" - Nm 7:89
 * "'al pi shenayim 'edim yaqum davar" - Dt 19:15
 *
 * Culminacion del ciclo. Si las 7 precondiciones pasan, se averigua
 * si V1 y V2 concuerdan. Si si -> voz EMITIDA (emanada=true, Hitpael);
 * si no -> DIVERGIDO (davar no se levanto). En AMBOS casos la
 * invocacion se consumo correctamente -> retorna OK y reset del ciclo.
 *
 * Si ALGUNA precondicion falla -> error especifico + ajarei_mot,
 * SIN reset (el ciclo no se consumo - puede retomarse).
 *
 * --- LAS 7 PRECONDICIONES (en orden de chequeo) ---
 *
 *   1) out != NULL                         (parametro)
 *   2) Kaporet levantado                   (init)
 *   3) par_presente                        (v.18)
 *   4) no'adti_activo                      (v.22)
 *   5) qetoret presente                    (Lv 16:13 - paso 1)
 *   6) mo'ed_abierto                       (Lv 16:2 - ventana)
 *   7) rociado_sobre_hecho                 (Lv 16:14a - paso 2a)
 *   8) rociados_delante >= SHEVA           (Lv 16:14b - paso 2b)
 *   9) es_fuego_extrano() == false         (auditoria final)
 *
 * --- CONCORDANCIA (DOBLE VECTOR SIMULTÁNEO — Éx 25:20) ---
 *
 * (a) ish el ajív (peer, horizontal): V1.hash_testigo == V2.hash_testigo
 *     — testigos_concuerdan(). Dt 19:15: dos testigos LEVANTAN davar.
 *
 * (b) el haKapóret (data, vertical): V1.hash_testigo == arca->zer.hash
 *     Y V2.hash_testigo == arca->zer.hash — el dato bajo el Kapóret
 *     ES la fuente. Dos testigos pueden mentir concordes; el dato no.
 *
 * AMBOS deben pasar EN ESTE MISMO CICLO (no en ciclos alternos —
 * Éx 25:20 los enuncia como cláusulas separadas pero simultáneas:
 * "ufnehém ish el ajív, el haKapóret yihyú pené hakeruvím"). Si
 * sólo (a) → dos testigos concordes en una mentira. Si sólo (b) →
 * un solo testigo levantando davar, lo prohibido en Dt 19:15.
 *
 * Si ambos pasan, el hash que se EMITE es el de V1 (equivale al de
 * V2 y al del Arca) — la voz no se construye, se COPIA de lo que
 * los tres vieron. "midabber" (Hitpael) en su forma mas literal:
 * la voz habla desde lo que hay.
 */
int hashmal_consensus_invocar(hashmal_consenso_resultado_t *out)
{
    if (out == NULL) {
        KAP_LOG_E("invocar: out NULL");
        return HASHMAL_KAPORET_ERR_NULL;
    }

    hashmal_kaporet_t *kap = hashmal_consensus_kaporet_mut();
    if (kap == NULL) {
        KAP_LOG_E("invocar: Kapóret no levantado");
        return HASHMAL_KAPORET_ERR_INIT;
    }

    /* 3) v.18 - par apto. Si no lo esta, nadie puede testificar. */
    if (!hashmal_consensus_par_presente()) {
        KAP_LOG_E("invocar: par de querubines no apto (v.18)");
        hashmal_consensus_ajarei_mot(HASHMAL_KAPORET_ERR_QUERUB_AUSENTE);
        return HASHMAL_KAPORET_ERR_QUERUB_AUSENTE;
    }

    /* 4) v.22 - no'adti abierto. Sin rendezvous, no hay lugar de
     * encuentro. kaporet_rendezvous.c controla este flag. */
    if (!kap->noadti_activo) {
        KAP_LOG_E("invocar: noʻadtí NO abierto (Éx 25:22 — sin rendezvous)");
        hashmal_consensus_ajarei_mot(HASHMAL_KAPORET_ERR_FUERA_MOED);
        return HASHMAL_KAPORET_ERR_FUERA_MOED;
    }

    /* 5) Lv 16:13 - qetoret primero. */
    if (kap->invocacion.qetoret != HASHMAL_QETORET_PRESENTE) {
        KAP_LOG_E("invocar: sin qetóret (Lv 16:13 — paso 1 pendiente)");
        hashmal_consensus_ajarei_mot(HASHMAL_KAPORET_ERR_SIN_QETORET);
        return HASHMAL_KAPORET_ERR_SIN_QETORET;
    }

    /* 6) Lv 16:2 - mo'ed abierto. */
    if (!kap->invocacion.moed_abierto) {
        KAP_LOG_E("invocar: moʻed cerrado (Lv 16:2 — bejol ʻét)");
        hashmal_consensus_ajarei_mot(HASHMAL_KAPORET_ERR_FUERA_MOED);
        return HASHMAL_KAPORET_ERR_FUERA_MOED;
    }

    /* 7) Lv 16:14a - SOBRE hecho. */
    if (!kap->invocacion.rociado_sobre_hecho) {
        KAP_LOG_E("invocar: sin rociado SOBRE (Lv 16:14a — paso 2a)");
        hashmal_consensus_ajarei_mot(HASHMAL_KAPORET_ERR_SIN_DAM);
        return HASHMAL_KAPORET_ERR_SIN_DAM;
    }

    /* 8) Lv 16:14b - sheva pe'amim completos. */
    if (kap->invocacion.rociados_delante < HASHMAL_VERIFICACIONES_SHEVA) {
        KAP_LOG_E("invocar: %u/%d rociados DELANTE (Lv 16:14b shéva peʻamím)",
                  (unsigned)kap->invocacion.rociados_delante,
                  HASHMAL_VERIFICACIONES_SHEVA);
        hashmal_consensus_ajarei_mot(HASHMAL_KAPORET_ERR_ROCIADO_INCOMPLETO);
        return HASHMAL_KAPORET_ERR_ROCIADO_INCOMPLETO;
    }

    /* 9) Auditoria final - red de seguridad. Cubre sobre_el_arca
     * (v.21) y redundancia sobre qetoret/mo'ed/par ya verificados.
     * Si llegara aqui con fuego extrano, es un bug en precondiciones
     * previas; mejor detectarlo que emitir una voz viciada. */
    if (hashmal_consensus_es_fuego_extrano()) {
        KAP_LOG_E("invocar: fuego extraño en auditoría final — abortando");
        hashmal_consensus_ajarei_mot(HASHMAL_KAPORET_ERR_FUEGO_EXTRANO);
        return HASHMAL_KAPORET_ERR_FUEGO_EXTRANO;
    }

    /* == Precondiciones limpias. Averiguar si los testigos concuerdan.
     *
     * ísh el ajív + el hakkappóret — Éx 25:20: doble vector simultáneo.
     * Peer-check (horizontal) + data-check (vertical) en cada ciclo.
     * Nunca uno sin el otro. El hebreo separa explícitamente las dos
     * cláusulas:
     *
     *   "ufnehém ish el ajív"               → rostros varón hacia su
     *                                          hermano: peer-check (V1↔V2)
     *   "el haKapóret yihyú pené hakeruvím" → hacia el Kapóret serán
     *                                          los rostros: data-check
     *                                          (V_i ↔ dato del Arca)
     *
     * Dt 19:15 - peer: si concuerdan, yaqum davar. Si no, "lo yaqum 'ed
     * ejad". Pero AUNQUE concuerden entre ellos, si la concordancia no
     * coincide con el dato bajo el Kapóret (arca->zer.hash), la voz
     * NO emerge: dos testigos concordes en una mentira siguen mintiendo. */
    hashmal_consenso_resultado_t r;
    memset(&r, 0, sizeof(r));
    r.timestamp = ahora_us();

    /* (a) Peer-check horizontal — ish el ajív (v.20a) */
    const bool concuerdan = hashmal_consensus_testigos_concuerdan();

    /* (b) Data-check vertical — el hakkappóret (v.20b). Ambos
     * testigos deben ver lo MISMO que el Arca (zer.hash). El Kapóret
     * sobre el Arca (v.21) es lo que los querubines miran; en firmware
     * eso es arca->zer.hash. Sin Arca accesible no hay sobre-qué-
     * mirar — fuego extraño implícito (Lv 10:1 — esh zará). */
    bool concuerdan_con_dato = false;
    const hashmal_arca_t *arca_ref = hashmal_storage_get_arca();
    if (arca_ref != NULL) {
        const hashmal_querub_t *v1 = &kap->querubin[HASHMAL_QUERUB_V1];
        const hashmal_querub_t *v2 = &kap->querubin[HASHMAL_QUERUB_V2];
        const bool v1_ve_dato = (memcmp(v1->hash_testigo,
                                        arca_ref->zer.hash,
                                        HASHMAL_ZER_HASH_BYTES) == 0);
        const bool v2_ve_dato = (memcmp(v2->hash_testigo,
                                        arca_ref->zer.hash,
                                        HASHMAL_ZER_HASH_BYTES) == 0);
        concuerdan_con_dato = v1_ve_dato && v2_ve_dato;
    }

    if (concuerdan && concuerdan_con_dato) {
        /* La voz EMANA. Nm 7:89 midabber - Hitpael: la voz se habla
         * a si misma. memcpy aqui es literalmente "tomar lo que hay"
         * - no se construye una verdad, se copia la concordancia. */
        r.estado  = HASHMAL_CONSENSO_EMITIDO;
        memcpy(r.verdad,
               kap->querubin[HASHMAL_QUERUB_V1].hash_testigo,
               HASHMAL_ZER_HASH_BYTES);
        r.emanada = true;

        /* Rom 3:25 - ilastirion: el Kaporet DEMUESTRA justicia. Un
         * ciclo Lv 16 que emitio voz cumple ese proposito; el flag
         * queda encendido (historico cumulativo). */
        kap->demuestra_justicia = true;

        KAP_LOG_I("═══ VOZ EMERGIDA (Éx 25:22 / Nm 7:89) ═══");
        KAP_LOG_I("  ish el ajív + el haKapóret — doble vector concuerda (v.20)");
        KAP_LOG_I("  mibéin shené hakeruvím — desde ENTRE los dos");
        KAP_LOG_I("  midabbér (Hitpael) — la voz se habla a sí misma");
        KAP_LOG_I("  yaqúm davár (Dt 19:15) — la Palabra se levantó");
        KAP_LOG_I("  emanada @%" PRIu64 " us", r.timestamp);
    } else {
        /* DIVERGIDO - alguno de los dos vectores falló. No se construye
         * una "verdad a medias": verdad queda en ceros, emanada=false.
         * Distinguimos en el log cuál vector falló para diagnóstico. */
        r.estado  = HASHMAL_CONSENSO_DIVERGIDO;
        /* r.verdad ya en ceros por memset inicial */
        r.emanada = false;

        if (!concuerdan && !concuerdan_con_dato) {
            KAP_LOG_W("═══ DIVERGENCIA TOTAL — ambos vectores fallan (Éx 25:20) ═══");
            KAP_LOG_W("  ish el ajív: V1≠V2 (peer)");
            KAP_LOG_W("  el haKapóret: testigo(s) ≠ dato del Arca (data)");
        } else if (!concuerdan) {
            KAP_LOG_W("═══ TESTIGOS DIVERGEN — peer-check falla (Dt 19:15) ═══");
            KAP_LOG_W("  'lo yaqúm ʻed ejád beʼísh' — V1≠V2 (ish el ajív)");
        } else {
            KAP_LOG_W("═══ TESTIGOS CONCUERDAN PERO MIENTEN — data-check falla ═══");
            KAP_LOG_W("  ish el ajív OK, pero el haKapóret no: testigos ≠ "
                      "arca->zer (v.20b)");
            KAP_LOG_W("  dos testigos concordes en una mentira siguen mintiendo");
        }
        KAP_LOG_W("  la invocación se ejecutó, pero no hay voz");
    }

    /* Registrar el resultado en el Kaporet (ultimo) y entregarlo al
     * caller. El ultimo es append-latest: audit externa se apoya
     * en kap->ultimo + querubin.timestamp_testigo para reconstruir
     * la historia (kaporet_rendezvous.c / futuro). */
    kap->ultimo = r;
    *out        = r;

    /* Lv 16:17 - el sumo sacerdote SALE. El ciclo termino, limpiamos.
     * Se limpia en AMBOS casos (EMITIDO o DIVERGIDO): la invocacion
     * CONSUMIO el ciclo, aunque no haya producido voz. */
    reset_ciclo(kap);

    /* OK: la invocacion se ejecuto correctamente. El contenido del
     * resultado (EMITIDO vs DIVERGIDO) lo revisa el caller en *out. */
    return HASHMAL_KAPORET_OK;
}
