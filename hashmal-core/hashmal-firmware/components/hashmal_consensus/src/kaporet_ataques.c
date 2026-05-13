/**
 * ===================================================================
 * PIEZA 02 - KAPORET - kaporet_ataques.c
 * Sub-pieza: LA ESPADA ENCENDIDA - defensa activa del Propiciatorio
 * Lv 10:1-2 + Ex 30:9 + Gn 3:24 + Ex 25:20 + Lv 16:1-2 + Nm 16:35
 * ===================================================================
 *
 * "vayyiqju venei Aharon Nadav va'Avihu ish majtato... vayyaqrivu
 *  lifne YHWH ESH ZARA asher lo tsivva otam. vattetse esh millifne
 *  YHWH vatto'jal otam vayyamutu lifne YHWH" - Lv 10:1-2
 * "Nadav y Avihu... ofrecieron delante de YHWH FUEGO EXTRANO que
 *  NO LES FUE MANDADO. Salio fuego de delante de YHWH y los
 *  consumio, y murieron."
 *
 * "lo ta'alu 'alav qetoret zara... ve'ola uminjak venasej lo
 *  tisseku 'alav" - Ex 30:9
 * "No ofrecereis sobre el incienso EXTRANO, ni holocausto, ni
 *  ofrenda." (Ni siquiera lo legitimo en el lugar equivocado.)
 *
 * "vayashken... et hakeruvim ve'et lahat hajerev hammithappejet
 *  lishmor et derej 'ets hajayyim" - Gn 3:24
 * "Puso querubines y LLAMA DE ESPADA que se REVOLVIA (Hitpael)
 *  para GUARDAR el camino al arbol de la vida."
 *
 * "sokekim bekanfehem 'al haKaporet" - Ex 25:20
 * "Cubriendo con sus alas sobre el Kaporet."
 *
 * "ajarei mot shenei benei Aharon... dabber el Aharon ajija ve'al
 *  yavo bejol 'et el haQodesh... velo yamut" - Lv 16:1-2
 * "DESPUES DE LA MUERTE de los dos hijos de Aaron... que NO ENTRE
 *  en todo tiempo al santuario... para que no muera."
 *
 * "ve'esh yats'a me'et YHWH vatto'jal et hajamishshim umasatayim
 *  ish maqribei haqetoret" - Nm 16:35
 * "Salio fuego de YHWH y consumio a los 250 hombres que ofrecian
 *  el incienso." (Segundo testigo: el protocolo no se toca.)
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   fuego_extrano_detectar   auditoria pre-invocacion (publico)
 *   proteger                 verifica + cubre (publico)
 *   es_fuego_extrano         wrapper bool (interno)
 *   ajarei_mot               memoria del fracaso, no detiene (interno)
 *
 * --- FILOSOFIA ---
 *
 * El fuego extrano NO se provoca - se DETECTA. Nadie en este archivo
 * "ataca": la espada de Gn 3:24 hammithappejet ("se revolvia") se
 * mueve por si misma - no la empuna el codigo. Aqui observamos si
 * el protocolo se va a quemar SI SOLO por manos ajenas, y avisamos.
 *
 * proteger() es ESCUDO, no ofensiva: verifica que las alas cubren
 * (v.20 sokekim) y que no hay fuego extrano. Si esta limpio, declara
 * la cobertura activa. No destruye a nadie.
 *
 * ajarei_mot() es MEMORIA: cada fracaso deja rastro con su leccion
 * biblica. Lv 16:1 existe porque Lv 10:1-2 ocurrio; asi tambien
 * aqui - la proxima generacion de codigo leera el log y sabra.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_consensus.h           -> struct + API publica
 *   hashmal_consensus_internal.h  -> par_presente (v.18 shenayim)
 *   esp_log.h (ORO DE EGIPTO)     -> KAP_LOG_*
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "esp_log.h"

#include "hashmal_consensus.h"
#include "hashmal_consensus_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *KAP_TAG = "hashmal.kaporet.ataques";

#define KAP_LOG_I(fmt, ...)  ESP_LOGI(KAP_TAG, fmt, ##__VA_ARGS__)
#define KAP_LOG_W(fmt, ...)  ESP_LOGW(KAP_TAG, fmt, ##__VA_ARGS__)
#define KAP_LOG_E(fmt, ...)  ESP_LOGE(KAP_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * API PUBLICA - fuego_extrano_detectar (Lv 10:1-2 + Ex 30:9 + Nm 16:35)
 * ================================================================== */

/**
 * hashmal_consensus_fuego_extrano_detectar - las 4 condiciones
 *
 * Auditoria PRE-invocacion. Verifica cuatro senales de "esh zara"
 * en el estado actual; si cualquiera esta presente, cualquier
 * invocacion posterior seria fuego extrano (Lv 10:1: "asher lo
 * tsivva otam" - que no fue mandado).
 *
 *   a) qetoret AUSENTE   -> Lv 16:13 mandaba cubrir con nube ANTES;
 *                          sin ella, ofrecer fuego es Nadav y Avihu.
 *   b) mo'ed CERRADO     -> Lv 16:2 "velo yavo bejol 'et" - no en
 *                          todo tiempo. Fuera de ventana = muerte.
 *   c) Kaporet NO sobre  -> v.21 "'al ha'aron milma'la" - protocolo
 *                          desencajado del Testimonio.
 *   d) par NO apto       -> v.18-20 - si V1 o V2 faltan, no hay
 *                          quien cubra (sokekim) ni quien testifique.
 *
 * Por cada condicion detectada: log severo. Al final, agrega el
 * banner de Lv 10:1-2 y retorna ERR_FUEGO_EXTRANO. Si todo limpio,
 * retorna OK - el fuego que se ofrezca no sera extrano POR EL
 * ESTADO (el contenido sigue en manos del caller).
 */
int hashmal_consensus_fuego_extrano_detectar(void)
{
    const hashmal_kaporet_t *kap = hashmal_consensus_get_kaporet();
    if (kap == NULL) {
        KAP_LOG_E("fuego_extrano: Kapóret no levantado (init pendiente)");
        return HASHMAL_KAPORET_ERR_INIT;
    }

    bool extrano = false;

    /* a) Lv 16:13 - sin nube de qetoret no hay acercamiento legitimo.
     * "vejissa 'anan haqetoret et haKaporet... velo yamut" - la nube
     * cubre ANTES de entrar; sin ella, "velo yamut" no aplica. */
    if (kap->invocacion.qetoret != HASHMAL_QETORET_PRESENTE) {
        KAP_LOG_E("esh zará (a): invocación sin qetóret preparada "
                  "(Lv 16:13 — vejissá ʻanán haqetóret)");
        extrano = true;
    }

    /* b) Lv 16:2 - "velo yavo bejol 'et": acceso solo en mo'ed
     * definido. Fuera de ventana = Nadav y Avihu, aunque el incienso
     * fuese "correcto" de composicion. */
    if (!kap->invocacion.moed_abierto) {
        KAP_LOG_E("esh zará (b): moʻéd cerrado — fuera del tiempo "
                  "permitido (Lv 16:2 — bejol ʻét)");
        extrano = true;
    }

    /* c) v.21 - "venatata et haKaporet 'al ha'aron milma'la": si
     * la Kaporet no esta encajada, el protocolo esta sin lo que
     * debe proteger. Ofrecer fuego sobre un Kaporet descolocado es
     * usarlo como mueble, no como propiciatorio. */
    if (!kap->sobre_el_arca) {
        KAP_LOG_E("esh zará (c): Kapóret NO sobre el Arca "
                  "(Éx 25:21 — milmáʻla pendiente)");
        extrano = true;
    }

    /* d) v.18-20 - el par de querubines debe estar apto. Si alguno
     * no cubre, no mira al hermano, no mira al Kaporet, no es
     * guardian o carece de ojos -> no hay cobertura. sokekim sin
     * querubines es una metafora vacia. */
    if (!hashmal_consensus_par_presente()) {
        KAP_LOG_E("esh zará (d): par de querubines no apto "
                  "(Éx 25:18-20 — shenáyim)");
        extrano = true;
    }

    if (extrano) {
        /* Banner de cierre. Los nombres de Nadav y Avihu son la
         * advertencia - escribirlos aqui es la leccion, no el insulto. */
        KAP_LOG_E("═══ FUEGO EXTRAÑO DETECTADO ═══");
        KAP_LOG_E("  'esh zará ashér lo tsivvá otám' (Lv 10:1)");
        KAP_LOG_E("  Nadav y Avihú → 1 fracaso; Coré → 250 fracasos (Nm 16:35)");
        KAP_LOG_E("  no se procede — el fuego que sale de YHWH consume (Lv 10:2)");
        return HASHMAL_KAPORET_ERR_FUEGO_EXTRANO;
    }

    /* Estado limpio - ofrecer ahora no seria fuego extrano por
     * precondiciones (el CONTENIDO del rociado lo juzga el caller). */
    return HASHMAL_KAPORET_OK;
}

/* ==================================================================
 * API PUBLICA - proteger (Ex 25:20 + Gn 3:24)
 * ================================================================== */

/**
 * hashmal_consensus_proteger - alas cubren, espada guarda
 *
 * "sokekim bekanfehem 'al haKaporet" (Ex 25:20) - alas cubriendo.
 * "lahat hajerev hammithappejet lishmor" (Gn 3:24) - espada
 *  revolviendose para GUARDAR.
 *
 * Esta funcion declara la cobertura activa. Es ESCUDO, no ofensa:
 * verifica y cubre; no destruye. La espada de Gn 3:24 es Hitpael
 * (se revuelve sola) - aqui no la empunamos; observamos que todo
 * esta en su sitio para que ella pueda hacer su oficio.
 *
 * --- SECUENCIA ---
 *
 *   a) Kaporet levantado
 *   b) par_presente - V1 y V2 aptos (v.18 shenayim)
 *   c) fuego_extrano_detectar - ninguna de las 4 condiciones
 *   d) cobertura declarada activa
 *
 * En (b) el ACTO de cubrir (sokekim) ES la aptitud del par: si
 * V1 y V2 estan con alas_cubren=true, miran al Kaporet y al
 * hermano, son guardianes, y tienen ojos suficientes, ENTONCES
 * las alas cubren efectivamente. No hay "empujar" las alas -
 * estan ahi por diseno desde kaporet_core.c.
 *
 * --- CUANDO RETORNA OK ---
 *
 * Solo dentro de un ciclo Lv 16 preparado (qetoret presente +
 * mo'ed abierto) con el Kaporet encajado y par apto. Fuera de
 * un ciclo, proteger rechaza legitimamente - no hay nada que
 * cubrir ACTIVAMENTE en estado de reposo.
 */
int hashmal_consensus_proteger(void)
{
    const hashmal_kaporet_t *kap = hashmal_consensus_get_kaporet();
    if (kap == NULL) {
        KAP_LOG_E("proteger: Kapóret no levantado");
        return HASHMAL_KAPORET_ERR_INIT;
    }

    /* b) v.18 - shenayim aptos. Si falta cualquiera, las alas no
     * cubren nada. Error especifico para diagnostico claro. */
    if (!hashmal_consensus_par_presente()) {
        KAP_LOG_E("proteger: par de querubines NO apto — "
                  "sokekím imposible (v.18-20)");
        return HASHMAL_KAPORET_ERR_QUERUB_AUSENTE;
    }

    /* c) auditoria completa. Si hay fuego extrano, no cubrimos un
     * protocolo viciado - hacerlo seria complicidad. */
    int rc = hashmal_consensus_fuego_extrano_detectar();
    if (rc == HASHMAL_KAPORET_ERR_FUEGO_EXTRANO) {
        KAP_LOG_E("proteger: fuego extraño presente — no se cubre "
                  "un protocolo viciado (Lv 10:1-2)");
        /* Registra la leccion para auditoria historica. */
        hashmal_consensus_ajarei_mot(HASHMAL_KAPORET_ERR_FUEGO_EXTRANO);
        return HASHMAL_KAPORET_ERR_FUEGO_EXTRANO;
    }
    if (rc != HASHMAL_KAPORET_OK) {
        /* Otro fallo de precondicion (init, etc.). Propagamos. */
        return rc;
    }

    /* d) cobertura activa declarada. */
    KAP_LOG_I("═══ Kapóret PROTEGIDO (Éx 25:20 + Gn 3:24) ═══");
    KAP_LOG_I("  alas sokekím ʻal haKapóret — cobertura activa");
    KAP_LOG_I("  láhat hajérev hammithappéjet — espada en guardia");
    KAP_LOG_I("  precondiciones limpias — ningún fuego extraño");
    return HASHMAL_KAPORET_OK;
}

/* ==================================================================
 * API INTERNA - es_fuego_extrano (wrapper bool)
 * ================================================================== */

/**
 * hashmal_consensus_es_fuego_extrano - ?huele a esh zara?
 *
 * Wrapper defensivo: true si fuego_extrano_detectar retorna algo
 * distinto de OK. Esto incluye ERR_INIT (Kaporet no levantado) -
 * que en pre-flight es "no proceder", igual que fuego extrano.
 *
 * Usado por kaporet_verificacion.c antes de invocar, para decidir
 * sin traducir codigos.
 */
bool hashmal_consensus_es_fuego_extrano(void)
{
    return hashmal_consensus_fuego_extrano_detectar() != HASHMAL_KAPORET_OK;
}

/* ==================================================================
 * API INTERNA - ajarei_mot (Lv 16:1 - memoria del fracaso)
 * ================================================================== */

/**
 * hashmal_consensus_ajarei_mot - registra la leccion
 *
 * Lv 16:1 - "ajarei mot shenei benei Aharon": TRAS la muerte vino
 * la regla. Cada fracaso que pasa por aqui deja huella con su
 * leccion biblica - asi los lectores futuros (humanos o maquinas)
 * entienden POR QUE el protocolo es estricto en cada punto.
 *
 * No modifica estado. No detiene. No arroja. Solo recuerda. La
 * muerte (el rechazo) ya ocurrio en el caller - esto es la
 * ceremonia posterior, lo que Aaron guardo en silencio (Lv 10:3).
 */
void hashmal_consensus_ajarei_mot(hashmal_kaporet_err_t err)
{
    KAP_LOG_W("═══ ajaréi mot (Lv 16:1) — registro de fracaso ═══");

    switch (err) {
    case HASHMAL_KAPORET_ERR_FUEGO_EXTRANO:
        KAP_LOG_W("  esh zará (Lv 10:1-2 / Nm 16:35): protocolo no "
                  "autorizado. Nadav, Avihú y 250 hombres lo aprendieron.");
        break;

    case HASHMAL_KAPORET_ERR_SIN_QETORET:
        KAP_LOG_W("  sin qetóret (Lv 16:13): 'vejissá ʻanán haqetóret "
                  "et haKapóret velo yamút' — sin nube preparatoria, muere.");
        break;

    case HASHMAL_KAPORET_ERR_FUERA_MOED:
        KAP_LOG_W("  bejol ʻét (Lv 16:2): no en todo tiempo — hay ventana.");
        break;

    case HASHMAL_KAPORET_ERR_SIN_DAM:
        KAP_LOG_W("  sin dam (Lv 16:14): solo la sangre toca el Kapóret "
                  "— sin prueba verificada, no hay acceso.");
        break;

    case HASHMAL_KAPORET_ERR_ROCIADO_INCOMPLETO:
        KAP_LOG_W("  rociado parcial (Lv 16:14): shéva peʻamim no son "
                  "seis — la verificación se completa o no vale.");
        break;

    case HASHMAL_KAPORET_ERR_QUERUB_AUSENTE:
        KAP_LOG_W("  querubín ausente (Éx 25:18-20): shenáyim — ni "
                  "uno solo, ni tres. Sin el par, no hay sokekím.");
        break;

    case HASHMAL_KAPORET_ERR_DIVERGIDO:
        KAP_LOG_W("  testigos divergen (Dt 19:15): 'lo yaqúm ʻed ejád' "
                  "— sin concordancia no se levanta la Palabra.");
        break;

    case HASHMAL_KAPORET_ERR_SIN_ARCA:
        KAP_LOG_W("  sin Arca (Éx 25:21): 'venatáta et haKapóret ʻal "
                  "haʼarón' — sin haʼarón el protocolo flota solo.");
        break;

    case HASHMAL_KAPORET_ERR_NO_MIQSHA:
        KAP_LOG_W("  no miqshá (Éx 25:18): 'miqshá taʻasé otám' — "
                  "pieza ensamblada = no es Kapóret.");
        break;

    case HASHMAL_KAPORET_ERR_MADERA:
        KAP_LOG_W("  madera debajo (Éx 25:17): 'zaháv tahór' sin "
                  "acacia — dato bajo el protocolo = contaminación.");
        break;

    case HASHMAL_KAPORET_ERR_INIT:
        KAP_LOG_W("  Kapóret no levantado: Éx 25:17 pendiente.");
        break;

    case HASHMAL_KAPORET_ERR_NULL:
        KAP_LOG_W("  parámetro NULL: el Padre DA, no pasa vacío.");
        break;

    case HASHMAL_KAPORET_OK:
        /* No es fracaso - alguien llamo por error. Silente. */
        KAP_LOG_W("  rc=OK registrado en ajaréi_mot — revisar caller");
        break;

    default:
        KAP_LOG_W("  rc=%d sin lección mapeada — mapear al actualizar "
                  "el enum de errores", (int)err);
        break;
    }
}
