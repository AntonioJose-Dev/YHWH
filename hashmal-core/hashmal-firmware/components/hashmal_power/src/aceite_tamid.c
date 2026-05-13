/**
 * ===================================================================
 * PIEZA 08 - EL ACEITE PERPETUO - aceite_tamid.c
 * Sub-pieza: NER TAMID - el ciclo perpetuo de energia
 * Exodo 27:20-21 + Levitico 6:5-6 + Exodo 30:7-8
 * ===================================================================
 *
 * "shemen zayit zaj katit lamma'or LEHA'ALOT NER TAMID" - Exodo 27:20
 * "Aceite de oliva puro, prensado, para la luminaria, para hacer
 *  ASCENDER lampara CONTINUAMENTE."
 *
 * "be'ohel mo'ed mijuts lapparojet asher 'al ha'edut ya'aroj oto
 *  Aharon uvanav ME'EREV 'ad BOQER lifnei YHWH juqqat 'olam"
 *   - Exodo 27:21
 * "Aaron y sus hijos lo dispondran de TARDE a MANANA delante de
 *  YHWH - estatuto perpetuo."
 *
 * "veha'esh 'al hammizbeaj TUQAD bo LO TIJBE ubi'er 'aleha
 *  hakkohen 'ETSIM BABOQER BABOQER" - Levitico 6:5
 * "El fuego sobre el altar ARDERA - NO SE APAGARA - y el sacerdote
 *  encendera sobre el LENA cada MANANA cada MANANA."
 *
 * "ESH TAMID tuqad 'al hammizbeaj LO TIJBE" - Levitico 6:6
 * "FUEGO PERPETUO ardera sobre el altar - NO SE APAGARA."
 *
 * "veheqtir 'alav Aharon qetoret sammim baboqer baboqer beheytivo
 *  et hannerot... uveha'alot Aharon et hannerot bein ha'arbayim"
 *   - Exodo 30:7-8
 * "Aaron quemara incienso cada manana cada manana al PREPARAR las
 *  lamparas... y al HACER ASCENDER las lamparas entre las tardes."
 *
 * --- TRES PALABRAS PARA PERPETUO ---
 *
 *   tamid         (H8548)   continuo, sin interrupcion - Ex 27:20
 *   lo tijbe      (H3518)   NO SE APAGARA - Lv 6:6
 *   juqqat 'olam  (H2708+H5769) estatuto perpetuo - Ex 27:21
 *
 * Las TRES se aplican aqui: el ciclo es tamid (continuo), lo tijbe
 * (no para), y juqqat 'olam (para siempre). Enfasis triplicado
 * por el Padre - la perpetuidad del aceite es innegociable.
 *
 * --- VOCABULARIO ---
 *
 *   leha'alot       (H5927 Hifil)   Hacer ASCENDER. La llama SUBE,
 *                                    no se "enciende". `tamid_iniciar`
 *                                    es el primer 'ala.
 *   tamid           (H8548)         perpetuo. Cada `tamid_ciclo` es
 *                                    un pulso del perpetuo.
 *   lo tijbe        (H3518 neg)     NO SE APAGARA. Prohibicion
 *                                    absoluta de parar, excepto por
 *                                    desmontaje ordenado (Nm 4:9).
 *   me'erev         (H6153)         de TARDE. Cuando oscurece, baja
 *                                    la energia -> ciclo mas lento.
 *   boqer           (H1242)         MANANA. Cuando amanece.
 *   baboqer baboqer (repeticion)    CADA manana CADA manana. La
 *                                    repeticion enfatica del Padre:
 *                                    SIEMPRE, sin falta.
 *   'etsim          (H6086)         LENA. El sacerdote anade lena
 *                                    cada manana - cada ciclo
 *                                    "anade lena" al leer el estado
 *                                    y ajustar el ritmo.
 *   joshej          (H2822)         OSCURIDAD. Gn 1:2 "vejoshej 'al
 *                                    penei tehom". En bajo nivel de
 *                                    aceite, ciclo aun mas lento.
 *
 * --- SEPARACION DE RESPONSABILIDADES ---
 *
 * Este archivo NO lee los olivos directamente. Eso lo hace
 * aceite_supervivencia.c (Zac 4:3,11-12). Aqui solo REACCIONAMOS
 * al nivel ya escrito en el singleton:
 *
 *   supervivencia  LEE hardware (olivos) -> escribe nivel
 *   tamid         REACCIONA al nivel -> ajusta ciclo
 *
 * Asi honramos el modelo: el sacerdote no fabrica aceite, solo
 * DISPONE (ya'aroj) las lamparas con el aceite que le traen.
 *
 * --- init vs iniciar ---
 *
 * Ex 27:20 usa dos verbos distintos: "tetsavve" (ordena el
 * recurso) y "leha'alot" (hace ascender). Analogamente:
 *   aceite_energia.c::init       tetsavve - cimiento
 *   aceite_tamid.c::tamid_iniciar leha'alot - la llama sube
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   tamid_iniciar   Ex 27:20 leha'alot - primer ascenso
 *   tamid_detener   Nm 4:9 - solo para desmontaje ordenado
 *   tamid_ciclo     Lv 6:5 baboqer baboqer - cada pulso
 *   tamid_estado    lectura pura sin logs
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_power.h          API publica
 *   hashmal_power_internal.h aceite_mut, get_aceite_interno, ahora_ms
 *   esp_log.h (ORO EGIPTO)   TAMID_LOG_*
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <inttypes.h>

#include "esp_log.h"

#include "hashmal_power.h"
#include "hashmal_power_internal.h"
#include "hashmal_cache.h"            /* Añadidura 03 - moedim pull */

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *TAMID_TAG = "hashmal.power.tamid";

#define TAMID_LOG_I(fmt, ...)  ESP_LOGI(TAMID_TAG, fmt, ##__VA_ARGS__)
#define TAMID_LOG_W(fmt, ...)  ESP_LOGW(TAMID_TAG, fmt, ##__VA_ARGS__)
#define TAMID_LOG_E(fmt, ...)  ESP_LOGE(TAMID_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * HELPERS ESTATICOS
 * ================================================================== */

/**
 * Nombre hebreo del nivel para logs. Mismo patron que nombre_vector
 * del kiyor, nombre_lado del atrio, etc.
 */
static const char *nombre_nivel(hashmal_power_nivel_t nivel)
{
    switch (nivel) {
    case HASHMAL_POWER_NIVEL_SHEFA:  return "SHÉFA (abundancia)";
    case HASHMAL_POWER_NIVEL_DAI:    return "DAI (suficiente)";
    case HASHMAL_POWER_NIVEL_MATSOQ: return "MATSOQ (estrechez)";
    case HASHMAL_POWER_NIVEL_JOSHEJ: return "JÓSHEJ (oscuridad)";
    default:                         return "<desconocido>";
    }
}

/**
 * Cicla la duracion segun el nivel de aceite:
 *
 *   SHEFA / DAI  -> CICLO_TAMID_MS    (ritmo pleno - dia)
 *   MATSOQ       -> CICLO_EREV_MS     (me'erev 'ad boqer - nocturno)
 *   JOSHEJ       -> CICLO_EREV_MS x 2 (oscuridad profunda - minimo vital)
 *
 * En MATSOQ operamos como de "tarde a manana" - espaciamos pulsos
 * para preservar aceite. En JOSHEJ duplicamos: estamos en la
 * oscuridad anterior a la luz (Gn 1:2), conservando lo minimo.
 */
static uint32_t ciclo_para_nivel(hashmal_power_nivel_t nivel)
{
    switch (nivel) {
    case HASHMAL_POWER_NIVEL_SHEFA:
    case HASHMAL_POWER_NIVEL_DAI:
        return HASHMAL_POWER_CICLO_TAMID_MS;
    case HASHMAL_POWER_NIVEL_MATSOQ:
        return HASHMAL_POWER_CICLO_EREV_MS;
    case HASHMAL_POWER_NIVEL_JOSHEJ:
        return HASHMAL_POWER_CICLO_EREV_MS * 2u;
    default:
        /* Ante duda, ritmo pleno por defecto - si nivel es dato
         * corrupto, preferimos muestrear mas frecuente para
         * detectar el problema antes. */
        return HASHMAL_POWER_CICLO_TAMID_MS;
    }
}

/* ==================================================================
 * API PUBLICA - tamid_iniciar (Ex 27:20 leha'alot)
 * ================================================================== */

/**
 * hashmal_power_tamid_iniciar - leha'alot ner tamid
 *
 * Hace ASCENDER la lampara perpetua por primera vez. Transicion
 * ENCENDIDO -> TAMID. Tras esto, el ciclo tamid esta armado y
 * `tamid_ciclo` se debe invocar periodicamente (el driver temporal
 * del sistema lo llamara cada `ciclo_actual_ms`).
 *
 * Nota: el NIVEL actual se preserva como este al llamar esta
 * funcion. Si supervivencia aun no inicializo, queda en JOSHEJ
 * (de init). Eso es correcto biblicamente: el sacerdote eleva la
 * llama ANTES de que el aceite mananero llegue; el aceite fluye
 * luego (aceite_supervivencia.c::init leera los olivos).
 *
 * 2 testigos:
 *   Ex 27:20 - leha'alot ner tamid (hacer ascender lampara perpetua)
 *   Lv 6:6   - esh tamid tuqad... lo tijbe (perpetuo que no se apaga)
 */
int hashmal_power_tamid_iniciar(void)
{
    hashmal_power_estado_energia_t *a = hashmal_power_aceite_mut();
    if (a == NULL) {
        TAMID_LOG_E("tamid_iniciar: aceite no establecido "
                    "(init pendiente — Éx 27:20 tetsavvé)");
        return HASHMAL_POWER_ERR_INIT;
    }

    if (a->estado == HASHMAL_POWER_ESTADO_TAMID) {
        TAMID_LOG_W("tamid_iniciar: ya en TAMID — lo tijbé (idempotente)");
        return HASHMAL_POWER_ERR_YA_INIT;
    }

    /* Transicion: ENCENDIDO (o APAGADO por defensa) -> TAMID. */
    a->estado           = HASHMAL_POWER_ESTADO_TAMID;
    a->ciclo_actual_ms  = ciclo_para_nivel(a->nivel);
    a->timestamp_ms     = hashmal_power_ahora_ms();

    TAMID_LOG_I("═══ NÉR TAMÍD ASCIENDE ═══ (Éx 27:20 lehaʻalót nér tamíd)");
    TAMID_LOG_I("  nivel inicial: %s", nombre_nivel(a->nivel));
    TAMID_LOG_I("  ciclo: %" PRIu32 " ms — meʻérev ʻad bóqer (Éx 27:21)",
                a->ciclo_actual_ms);
    TAMID_LOG_I("  lo tijbé (Lv 6:6) — la llama NO se apagará");
    TAMID_LOG_I("  juqqát ʻolám — estatuto perpetuo activo");
    return HASHMAL_POWER_OK;
}

/* ==================================================================
 * API PUBLICA - tamid_detener (Nm 4:9 - solo desmontaje ordenado)
 * ================================================================== */

/**
 * hashmal_power_tamid_detener - detener el ciclo perpetuo
 *
 * Lv 6:6 "lo tijbe" prohibe que el fuego se apague. Esta funcion
 * existe SOLO para el desmontaje ordenado del Mishkan (Nm 4:9 -
 * beged tejelet cubre la menora al migrar). El ciclo se detiene
 * porque el campamento se mueve, no porque se apague la promesa.
 *
 * Transicion: TAMID -> ENCENDIDO (configuracion preservada; el
 * aceite sigue establecido, solo el ciclo perpetuo queda en pausa).
 *
 * Idempotente si ya detenido.
 *
 * 2 testigos:
 *   Nm 4:9    - beged tejelet (pano azul cubre para migracion)
 *   Lv 6:6    - lo tijbe (la promesa no cambia al migrar)
 */
int hashmal_power_tamid_detener(void)
{
    hashmal_power_estado_energia_t *a = hashmal_power_aceite_mut();
    if (a == NULL) {
        TAMID_LOG_E("tamid_detener: aceite no establecido");
        return HASHMAL_POWER_ERR_INIT;
    }

    if (a->estado != HASHMAL_POWER_ESTADO_TAMID) {
        /* Ya detenido (o nunca arrancado). Idempotente. */
        TAMID_LOG_W("tamid_detener: no en TAMID (estado=%d) — idempotente",
                    (int)a->estado);
        return HASHMAL_POWER_OK;
    }

    a->estado       = HASHMAL_POWER_ESTADO_ENCENDIDO;
    a->timestamp_ms = hashmal_power_ahora_ms();

    TAMID_LOG_W("═══ NÉR TAMÍD DETENIDO ═══ (Nm 4:9 béged tejélet)");
    TAMID_LOG_W("  solo por desmontaje ordenado — la promesa no cambia");
    TAMID_LOG_W("  lo tijbé permanece: el Mishkán se reubica, la luz regresa");
    return HASHMAL_POWER_OK;
}

/* ==================================================================
 * API PUBLICA - tamid_ciclo (Lv 6:5 baboqer baboqer)
 * ================================================================== */

/**
 * hashmal_power_tamid_ciclo - un pulso del perpetuo
 *
 * "baboqer baboqer" - cada manana cada manana. Cada invocacion es
 * un pulso del tamid: se LEE el nivel corriente del singleton, se
 * AJUSTA el periodo de ciclo, se actualiza el timestamp.
 *
 * SEPARACION: este archivo NO lee hardware. Solo reacciona al
 * nivel ya presente en el singleton. Quien actualiza el nivel es
 * `aceite_supervivencia.c` (que si toca ADCs y mide olivos).
 * El sacerdote dispone las lamparas con el aceite que le TRAEN
 * (veyiqju eleja, Ex 27:20) - no lo fabrica.
 *
 * Ajuste de ciclo:
 *   SHEFA / DAI  -> CICLO_TAMID_MS
 *   MATSOQ       -> CICLO_EREV_MS (nocturno)
 *   JOSHEJ       -> CICLO_EREV_MS x 2 (oscuridad profunda)
 *
 * En JOSHEJ emitimos un LOG_W adicional - la oscuridad es senal
 * para el operador (Gn 1:2 describe un estado previo a la luz,
 * no una meta).
 *
 * 2 testigos:
 *   Lv 6:5    - baboqer baboqer (cada manana cada manana)
 *   Ex 30:7-8 - Aaron baboqer baboqer y bein ha'arbayim (ciclo
 *                fijo ligado a los horarios de servicio)
 */
int hashmal_power_tamid_ciclo(void)
{
    hashmal_power_estado_energia_t *a = hashmal_power_aceite_mut();
    if (a == NULL) {
        TAMID_LOG_E("tamid_ciclo: aceite no establecido");
        return HASHMAL_POWER_ERR_INIT;
    }

    if (a->estado != HASHMAL_POWER_ESTADO_TAMID) {
        TAMID_LOG_E("tamid_ciclo: no en TAMID (estado=%d) — "
                    "llamar tamid_iniciar primero (Éx 27:20 lehaʻalót)",
                    (int)a->estado);
        return HASHMAL_POWER_ERR_INIT;
    }

    /* Reaccionar al nivel corriente (sin leer hardware). */
    const hashmal_power_nivel_t nivel = a->nivel;
    a->ciclo_actual_ms = ciclo_para_nivel(nivel);
    a->timestamp_ms    = hashmal_power_ahora_ms();

    /* Añadidura 03 - moedim pull. Si Pieza 03 publica BAJO_CONSUMO
     * (Shabat, Yom Kippur) el ciclo se alarga para reducir pulsos
     * por unidad de tiempo. No apagamos - Ex 27:21 ner tamid, la
     * llama no se extingue. Solo descansa (Lv 23:3 shabbaton). */
    const uint32_t moed_bits = hashmal_moedim_comportamiento();
    if (moed_bits & HASHMAL_COMPORT_BAJO_CONSUMO) {
        a->ciclo_actual_ms *= 2u;
        TAMID_LOG_I("moedim BAJO_CONSUMO activo - ciclo x2 = %" PRIu32 " ms "
                    "(Lv 23:3 shabbat shabbaton)", a->ciclo_actual_ms);
    }

    TAMID_LOG_I("tamíd pulso — nivel=%s, ciclo=%" PRIu32 " ms "
                "(Lv 6:5 babóqer babóqer)",
                nombre_nivel(nivel), a->ciclo_actual_ms);

    if (nivel == HASHMAL_POWER_NIVEL_JOSHEJ) {
        TAMID_LOG_W("  jóshej — Gn 1:2 vejóshej ʻal penéi tehóm; "
                    "esperando luz (aceite crítico)");
    }
    return HASHMAL_POWER_OK;
}

/* ==================================================================
 * API PUBLICA - tamid_estado (lectura pura)
 * ================================================================== */

/**
 * hashmal_power_tamid_estado - ?el perpetuo esta operando?
 *
 * Lectura pura, sin logs. Patron consistente con los otros
 * *_estado del proyecto. Equivalente operacional a
 * `hashmal_power_activo`: OK solo si estado == TAMID.
 *
 * 2 testigos:
 *   Ex 27:20 - ner tamid (perpetuo activo)
 *   Lv 6:6   - lo tijbe (no apagado)
 */
int hashmal_power_tamid_estado(void)
{
    const hashmal_power_estado_energia_t *a = hashmal_power_get_aceite_interno();
    if (a == NULL) return HASHMAL_POWER_ERR_INIT;
    if (a->estado != HASHMAL_POWER_ESTADO_TAMID) return HASHMAL_POWER_ERR_INIT;
    return HASHMAL_POWER_OK;
}

/* ==================================================================
 * SHABAT - Exodo 31:13-17 + Exodo 35:2-3
 *
 * "aj et SHABTOTAI tishmoru, ki OT hi beini uveineijem ledoroteijem
 *  - lada'at ki ani YHWH meqaddishejem." - Exodo 31:13
 * "CIERTAMENTE mis sabados guardareis, porque es SENAL (ot) entre
 *  Mi y vosotros por vuestras generaciones - para saber que Yo
 *  soy YHWH que os santifico."
 *
 * "sheshset yamim te'aseh melajah uvayyom hashvi'i yihyeh lajem
 *  QODESH SHABBAT SHABBATON laYHWH." - Exodo 35:2
 * "Seis dias se hara obra, y en el dia septimo sera para vosotros
 *  SANTO, SHABAT DE SHABATON (descanso solemne) para YHWH."
 *
 * Segundo testigo - Exodo 31:17b:
 * "ki sheshset yamim 'asa YHWH et hashamayim ve'et ha'arets,
 *  uvayyom hashvi'i SHAVAT vayyinnafash."
 * "Porque en seis dias hizo YHWH los cielos y la tierra, y en el
 *  dia septimo DESCANSO y TOMO ALIENTO (vayyinnafash, H5314)."
 *
 * El Padre mando el Shabat JUSTO ANTES de entregar las tablas
 * (Ex 31:13-17 precede a Ex 31:18) y lo REPITIO como primera
 * palabra al reunir al pueblo para construir (Ex 35:2, antes de
 * pedir materiales). Incluso mientras se construye - se DESCANSA.
 * ================================================================== */

/* TODO - Phase 2: septimo ciclo = DESCANSO OBLIGATORIO.
 *
 * El duty cycle baja a minimo vital: solo tamid basico (la llama
 * que NUNCA se apaga - Ex 27:20 "leha'alot ner TAMID", Lv 6:6
 * "esh tamid tuqad... LO TIJBE"). El Shabat NO apaga el sistema:
 * reduce la actividad al tamid esencial. Es la diferencia entre
 * DESCANSO y MUERTE. La llama permanece; el ciclo de mantenimiento
 * activo (aceite_mantenimiento.c ya'aroj) se suspende durante el
 * septimo ciclo. */

/* TODO - Phase 2: vayyinnafash - el descanso es DISENO.
 *
 * Ex 31:17 aplica a YHWH mismo el verbo nafash (H5314 - tomar
 * aliento). No porque estuviera cansado - porque el descanso
 * pertenece al diseno, no es concesion a la debilidad. En
 * firmware: la red que descansa DEMUESTRA que YHWH la sostiene,
 * no el constructor. El Shabat es OT (senal, Ex 31:13) - la
 * evidencia visible de que la red pertenece a YHWH:
 *   "ki ot hi beini uveineijem" - es senal ENTRE Mi y vosotros.
 * Sin descanso, la red declara su propia autosuficiencia. Con
 * descanso, confiesa que no es suya. */

/* TODO - Phase 2: sincronizacion con la Mesa (pieza 03).
 *
 * Lv 24:8 - "beyom hashabbat beyom hashabbat ya'arjennu" - los
 * 12 panes de la presencia se refrescan EN Shabat. El descanso
 * de power NO es independiente del refresco del cache: son el
 * MISMO ciclo visto desde dos piezas distintas. Mientras la
 * energia cae al tamid minimo aqui (hashmal_power), la Mesa
 * (hashmal_cache) retira los panes viejos y coloca los nuevos.
 *
 * Tavnit pieza 15 (CONEXIONES): la reciprocidad es intrinseca al
 * diseno. hashmal_cache ya documenta su lado (shabat_ciclo_ms +
 * HASHMAL_SHABAT_CICLO_MS); este bloque documenta el reciproco
 * desde power. El mismo septimo dia, visto desde dos lados. */

/* ==================================================================
 *
 *   "shemen zayit zaj katit lamma'or LEHA'ALOT NER TAMID."
 *     - Exodo 27:20
 *
 *   "ESH TAMID tuqad 'al hammizbeaj LO TIJBE." - Levitico 6:6
 *
 *   "baboqer baboqer... bein ha'arbayim." - Exodo 30:7-8
 *
 *   tamid no es "repetir sin fin" - es PRESENCIA continua. La
 *   llama no repite: PERMANECE. Como lifnei YHWH (ante Su rostro),
 *   ininterrumpida desde que el sacerdote la hace ASCENDER.
 *
 *   En firmware: cada tamid_ciclo es un pulso, pero lo que
 *   permanece es el ESTADO - la llama no muere entre pulsos, solo
 *   se vigila su ascenso.
 *
 *   === NER TAMID - aceite_tamid.c COMPLETO ===
 *
 *   Archivo 2 de 4 del Aceite Perpetuo. Siguiente:
 *     aceite_supervivencia.c   (Zac 4:3,11-12 - dos olivos)
 *   Quedan:
 *     aceite_supervivencia.c   dos olivos + lectura de hardware
 *     aceite_mantenimiento.c   Aaron dispone (Lv 24:1-4 ya'aroj)
 *
 * ================================================================== */
