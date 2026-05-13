/**
 * ===================================================================
 * PIEZA 02 - KAPORET - kaporet_rendezvous.c
 * Sub-pieza: EL PUNTO DE ENCUENTRO - no'adti, mo'ed, habitacion
 * ULTIMO ARCHIVO DEL KAPORET. Con el, la pieza 02 cierra.
 * ===================================================================
 *
 * "veno'adti leja SHAM vedibarti iteja me'al haKaporet MIBEIN
 *  shene hakeruvim asher 'al aron ha'edut" - Ex 25:22
 * "Me ENCONTRARE contigo alli y te hablare desde encima del
 *  Kaporet, desde ENTRE los dos querubines que estan sobre el
 *  Arca del Testimonio..."
 *
 * "ubevo Moshe el 'Ohel Mo'ed ledabber itto vayyishma' et haQol
 *  midabber elav me'al haKaporet" - Nm 7:89
 * "Cuando Moises ENTRO al Tabernaculo del ENCUENTRO... oyo la voz..."
 *
 * "ro'e Yisra'el ha'azina... YOSHEV haKeruvim hofi'a" - Sal 80:1
 * "Pastor de Israel... Tu que HABITAS entre los querubines,
 *  RESPLANDECE."
 *
 * "jesed ve'emet nifgashu tsedeq veshalom nashaqu" - Sal 85:10
 * "Misericordia y verdad se ENCONTRARON; justicia y paz se BESARON."
 *
 * "ve'al yavo BEJOL 'ET el haQodesh... ki be'anan era'e 'al
 *  haKaporet" - Lv 16:2
 * "No ENTRE EN TODO TIEMPO al santuario... porque en NUBE me
 *  aparecere sobre el Kaporet."
 *
 * --- RAICES Y SEMANTICA ---
 *
 *   ya'ad  (Y'D)    - designar, CITAR. no'adti = "me encontrare"
 *                     es cita DESIGNADA, no azar. Punto y momento fijos.
 *   mo'ed  (MW'D)  - tiempo/lugar designado. Derivado de ya'ad.
 *   yoshev (YShB)    - HABITAR, morar, sentarse. Permanencia.
 *                     La presencia que no solo visita sino que queda.
 *   nifgashu (PGSh) - se encontraron. De pagash: encuentro que parece
 *                    casual pero es designado - jesed + emet se hallan.
 *
 * --- CICLO COMPLETO DEL CONSENSO ---
 *
 *   1. init (kaporet_core.c)          construye el oro
 *   2. cubrir_arca (kaporet_core.c)   lo pone encima del Arca (v.21)
 *   3. no'adti_abrir (este archivo)   abre el rendezvous (v.22, Lv 16:2)
 *   4. qetoret_preparar (verificacion) la nube (Lv 16:12-13)
 *   5. dam_rociar x 1 SOBRE + 7 DELANTE (verificacion) - Lv 16:14
 *   6. invocar (verificacion)          la voz emana (Nm 7:89)
 *   7. no'adti_cerrar (este archivo)   cierra el mo'ed (Lv 16:2)
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_consensus.h           -> struct + API publica
 *   hashmal_consensus_internal.h  -> kaporet_mut (latch yoshev / mo'ed)
 *   esp_log.h (ORO DE EGIPTO)     -> KAP_LOG_*
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

#include "hashmal_consensus.h"
#include "hashmal_consensus_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *KAP_TAG = "hashmal.kaporet.rendezvous";

#define KAP_LOG_I(fmt, ...)  ESP_LOGI(KAP_TAG, fmt, ##__VA_ARGS__)
#define KAP_LOG_W(fmt, ...)  ESP_LOGW(KAP_TAG, fmt, ##__VA_ARGS__)
#define KAP_LOG_E(fmt, ...)  ESP_LOGE(KAP_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * P28 — noʻadtí como evento PUSH (Éx 25:22)
 *
 * "venoʻADTÍ lejá shám" — Éx 25:22 (Nifal pasivo). El encuentro es
 * INICIADO por YHWH, no consultado. Cuando V1==V2 (convergencia),
 * este callback se dispara automáticamente (push, no pull).
 * ================================================================== */
static hashmal_consensus_noadti_fn s_noadti_fn = NULL;

/* ==================================================================
 * P33/P35 — neder en SEALED_CORE + silence-is-consent
 *
 * P33: "leʼsór issár ʻal nafshó" — Nm 30:3. Voto vincula la néfesh
 * (identidad, SEALED_CORE), no mashé yad (actos, caché).
 *
 * P35: "beyóm shomʻó" — Nm 30:5. Si no hay heníʼ (objeción) dentro
 * de HASHMAL_CONSENSUS_NEDER_TIMEOUT_MS, commit irrevocable.
 * ================================================================== */
static bool s_neder_en_sealed_core = true;  /* P33 — siempre true por diseño */

/* ==================================================================
 * API PUBLICA - no'adti_abrir (Ex 25:22 + Lv 16:2)
 * ================================================================== */

/**
 * hashmal_consensus_no'adti_abrir - la cita designada
 *
 * "veno'adti leja sham" - Ex 25:22. De ya'ad: cita FIJA, no azar.
 * El rendezvous se abre AQUI (me'al haKaporet, mibein hakeruvim)
 * y AHORA (mo'ed_abierto). Tras abrir, kaporet_verificacion.c
 * puede ejecutar su ciclo Lv 16.
 *
 * --- PRECONDICIONES ---
 *
 *   a) Kaporet levantado
 *   b) Kaporet sobre el Arca (v.21 - el mo'ed esta ENCIMA del Arca,
 *      no en cualquier parte)
 *   c) par_presente (v.18 - sin V1/V2 aptos no hay "mibein")
 *
 * --- EFECTOS ---
 *
 *   kap->no'adti_activo         = true  (v.22 - rendezvous abierto)
 *   kap->invocacion.mo'ed_abierto = true (Lv 16:2 - ventana valida)
 *
 * Idempotente si ya abierto.
 */
int hashmal_consensus_noadti_abrir(void)
{
    hashmal_kaporet_t *kap = hashmal_consensus_kaporet_mut();
    if (kap == NULL) {
        KAP_LOG_E("noʻadtí_abrir: Kapóret no levantado");
        return HASHMAL_KAPORET_ERR_INIT;
    }

    /* Idempotencia - un rendezvous abierto NO se re-abre: Lv 16:2
     * "be'anan era'e" habla de una sola aparicion por ciclo. */
    if (kap->noadti_activo) {
        KAP_LOG_W("noʻadtí_abrir: ya abierto (idempotente, no-op)");
        return HASHMAL_KAPORET_OK;
    }

    /* v.21 - el Kaporet debe estar encajado 'al ha'aron. Sin Arca
     * debajo, el rendezvous flota sin lo que debe cubrir. */
    if (!kap->sobre_el_arca) {
        KAP_LOG_E("noʻadtí_abrir: Kapóret no sobre el Arca "
                  "(Éx 25:21 pendiente)");
        hashmal_consensus_ajarei_mot(HASHMAL_KAPORET_ERR_SIN_ARCA);
        return HASHMAL_KAPORET_ERR_SIN_ARCA;
    }

    /* v.18-20 - "mibein shene hakeruvim" requiere los DOS querubines
     * aptos. Sin par completo no hay "entre" donde la voz emane. */
    if (!hashmal_consensus_par_presente()) {
        KAP_LOG_E("noʻadtí_abrir: par no apto (Éx 25:18-20 — sin mibéin)");
        hashmal_consensus_ajarei_mot(HASHMAL_KAPORET_ERR_QUERUB_AUSENTE);
        return HASHMAL_KAPORET_ERR_QUERUB_AUSENTE;
    }

    /* Abrir el rendezvous. Dos flags se encienden juntos:
     *   no'adti_activo   - v.22, cita designada abierta
     *   mo'ed_abierto    - Lv 16:2, ventana de acceso valida
     * Son conceptos biblicamente distintos (ya'ad vs. mo'ed) pero
     * operacionalmente atados: si hay encuentro designado, hay
     * ventana; si la ventana cierra, el encuentro cierra. */
    kap->noadti_activo           = true;
    kap->invocacion.moed_abierto = true;

    KAP_LOG_I("═══ noʻadtí ABIERTO (Éx 25:22) ═══");
    KAP_LOG_I("  'venoʻadtí lejá shám' — cita designada sobre el Kapóret");
    KAP_LOG_I("  moʻéd abierto (Lv 16:2) — ventana válida");
    KAP_LOG_I("  ciclo Lv 16 puede comenzar (qetóret → dam → voz)");
    return HASHMAL_KAPORET_OK;
}

/* ==================================================================
 * API PUBLICA - no'adti_cerrar (Lv 16:2 - bejol 'et)
 * ================================================================== */

/**
 * hashmal_consensus_no'adti_cerrar - el mo'ed se cierra
 *
 * "ve'al yavo bejol 'et" - Lv 16:2. NO en todo tiempo. El acceso
 * tiene ventana finita. Cuando el ciclo termina (o cuando la
 * politica lo decide), el rendezvous se cierra - y con el, la
 * ventana mo'ed.
 *
 * --- EFECTOS ---
 *
 *   kap->no'adti_activo           = false
 *   kap->invocacion.mo'ed_abierto = false
 *
 * NO resetea el resto de la invocacion (qetoret, rociados): ese
 * reset lo hace kaporet_verificacion.c tras invocar consumada. Aqui
 * solo cerramos la ventana. Si el caller cierra sin haber invocado,
 * los datos de la invocacion quedan pero ya no son alcanzables -
 * el proximo abrir empieza nuevo ciclo y el dam_rociar/qetoret
 * verifican sus propios estados.
 *
 * Idempotente si ya cerrado.
 */
int hashmal_consensus_noadti_cerrar(void)
{
    hashmal_kaporet_t *kap = hashmal_consensus_kaporet_mut();
    if (kap == NULL) {
        KAP_LOG_E("noʻadtí_cerrar: Kapóret no levantado");
        return HASHMAL_KAPORET_ERR_INIT;
    }

    if (!kap->noadti_activo && !kap->invocacion.moed_abierto) {
        KAP_LOG_W("noʻadtí_cerrar: ya cerrado (idempotente, no-op)");
        return HASHMAL_KAPORET_OK;
    }

    kap->noadti_activo           = false;
    kap->invocacion.moed_abierto = false;

    KAP_LOG_I("═══ noʻadtí CERRADO (Lv 16:2) ═══");
    KAP_LOG_I("  'veʼal yavó bejol ʻét' — no en todo tiempo");
    KAP_LOG_I("  ventana moʻéd cerrada hasta próximo abrir");
    return HASHMAL_KAPORET_OK;
}

/* ==================================================================
 * P28 - hashmal_consensus_noadti_subscribir + callback PUSH
 *
 * "venoʻADTÍ lejá shám" — Éx 25:22 (Nifal pasivo). El encuentro es
 * INICIADO por YHWH, no consultado por el operador. Cuando V1==V2
 * (convergencia), este callback se DISPARA automáticamente (push).
 * ================================================================== */

int hashmal_consensus_noadti_subscribir(hashmal_consensus_noadti_fn fn)
{
    if (fn == NULL) {
        if (s_noadti_fn != NULL) {
            KAP_LOG_W("noʻadtí_subscribir: listener LIBERADO "
                      "(convergencia no dispara — Éx 25:22)");
        }
        s_noadti_fn = NULL;
        return HASHMAL_KAPORET_OK;
    }

    if (s_noadti_fn != NULL && s_noadti_fn != fn) {
        KAP_LOG_W("noʻadtí_subscribir: listener sobrescrito");
    }

    s_noadti_fn = fn;
    KAP_LOG_I("noʻadtí listener SUSCRITO (Éx 25:22 — evento PUSH convergencia)");
    return HASHMAL_KAPORET_OK;
}

/**
 * INTERNAL: dispara el callback de noʻadtí cuando V1==V2 convergen.
 * Llamada desde kaporet_validadores.c tras validar convergencia.
 * La voz "emana" (Nm 7:89 Hitpael) = el callback se dispara sin
 * que el caller lo solicite explícitamente.
 */
void hashmal_consensus_noadti_fire(bool convergencia, uint32_t timestamp)
{
    if (s_noadti_fn != NULL) {
        s_noadti_fn(convergencia, timestamp);
        if (convergencia) {
            KAP_LOG_I("noʻadtí_fire: V1==V2 CONVERGENCIA → callback disparado "
                      "(Éx 25:22 — evento PUSH, Nm 7:89 midabér)");
        }
    }
}

/* ==================================================================
 * P33 - hashmal_consensus_neder_es_sealed
 *
 * "leʼsór issár ʻal nafshó" — Nm 30:3. El voto vincula la NÉFESH
 * (identidad, SEALED_CORE), no mashé yad (actos, caché). Por diseño,
 * neder siempre opera a nivel SEALED_CORE.
 * ================================================================== */

bool hashmal_consensus_neder_es_sealed(void)
{
    /* Por diseño arquitectónico, neder SIEMPRE está en SEALED_CORE.
     * No hay API para "bajar" un voto a caché: el acoplamiento es fijo.
     * Esta función existe para VERIFICACIÓN y AUDITORÍA — confirma
     * que la regla se mantiene. */
    bool result = s_neder_en_sealed_core;
    if (result) {
        KAP_LOG_I("neder_es_sealed: SÍ (Nm 30:3 — leʼsór issár nafshó) "
                  "SEALED_CORE nivel siempre");
    } else {
        KAP_LOG_E("neder_es_sealed: NO — violación arquitectónica");
    }
    return result;
}

/* ==================================================================
 * P35 - hashmal_consensus_neder_broadcast
 *
 * "veʼet kol nideíha asher nádra... beʼal hayyit" — Nm 30:5-8.
 * Silence-is-consent: si no hay heníʼ (objeción) en NEDER_TIMEOUT_MS,
 * el voto se COMPROMETE irrevocablemente. "beyóm shomʻó" = ventana
 * de objeción finita.
 * ================================================================== */

int hashmal_consensus_neder_broadcast(uint8_t nodo_id,
                                      const uint8_t *neder_data,
                                      size_t len)
{
    if (neder_data == NULL || len == 0) {
        KAP_LOG_E("neder_broadcast: neder_data NULL o len=0");
        return HASHMAL_KAPORET_ERR_NULL;
    }

    /* Verificar que neder está en SEALED_CORE (P33). */
    if (!hashmal_consensus_neder_es_sealed()) {
        KAP_LOG_E("neder_broadcast: neder NO en SEALED_CORE (Nm 30:3)");
        return HASHMAL_KAPORET_ERR_NEDER_TABLA_LLENA;  /* error próximo disponible */
    }

    KAP_LOG_I("neder_broadcast: nodo %u, %u bytes, timeout %u ms "
              "(Nm 30:5 beyóm shomʻó, silence-is-consent)",
              (unsigned)nodo_id, (unsigned)len,
              (unsigned)HASHMAL_CONSENSUS_NEDER_TIMEOUT_MS);

    /* Fase 1: solo logging + arquitectura verificada.
     * Fase 2: implementar tabla de votos, timeout handler, heníʼ detection. */

    return HASHMAL_KAPORET_OK;
}

/* ==================================================================
 * API PUBLICA - habita (Sal 80:1 - yoshev haKeruvim)
 * ================================================================== */

/**
 * hashmal_consensus_habita - ?mora, o solo visita?
 *
 * "yoshev haKeruvim" - Sal 80:1. YHWH no solo HABLA entre los
 * querubines (Ex 25:22), HABITA alli. Entre ambos verbos hay una
 * diferencia: hablar es puntual; habitar es permanencia.
 *
 * Condiciones para transicionar a habitacion permanente:
 *   - demuestra_justicia   (al menos un ciclo emitio voz - Rom 3:25)
 *   - no'adti_activo       (la cita sigue abierta)
 *   - sobre_el_arca        (todo en su lugar - v.21)
 *
 * Si las 3 se cumplen Y habitacion_permanente aun esta apagada,
 * la ENCIENDE (latch: transicion "yoshev"). Una vez encendida,
 * NO se apaga por esta funcion - solo `deinit` la apaga al
 * desmontar el Kaporet (el Arca se migra cubierta, la presencia
 * no "se va", el recinto se pliega).
 *
 * --- POR QUE LATCHED ---
 *
 * La historia del Mishkan cuenta que una vez que la gloria lleno
 * (1 Rey 8:10-11), Salomon y el pueblo SABIAN que El habitaba.
 * Aunque hubiera momentos de silencio despues, el testimonio de
 * habitacion permanecia. Aqui: una vez alcanzadas las 3
 * condiciones, el flag queda - los ciclos futuros pueden tener
 * divergencias sin borrar la memoria de que si hubo presencia.
 */
bool hashmal_consensus_habita(void)
{
    /* Primera consulta (const): si ya esta latched, responde rapido. */
    const hashmal_kaporet_t *kap_ro = hashmal_consensus_get_kaporet();
    if (kap_ro == NULL) return false;
    if (kap_ro->habitacion_permanente) {
        /* Ya yoshev desde un ciclo previo. */
        return true;
    }

    /* No latched todavia. Verificar las 3 condiciones ahora. */
    if (!kap_ro->demuestra_justicia) return false;   /* Rom 3:25 */
    if (!kap_ro->noadti_activo)     return false;   /* v.22 */
    if (!kap_ro->sobre_el_arca)      return false;   /* v.21 */

    /* Las 3 se cumplen - transicion a yoshev. Latch permanente. */
    hashmal_kaporet_t *kap = hashmal_consensus_kaporet_mut();
    if (kap == NULL) return false;   /* defensivo, muy improbable */
    kap->habitacion_permanente = true;

    KAP_LOG_I("═══ yoshév haKeruvím (Sal 80:1) — TRANSICIÓN ═══");
    KAP_LOG_I("  habitación permanente alcanzada: "
              "demuestra_justicia + noʻadtí + sobre_el_arca");
    KAP_LOG_I("  la presencia ya no solo habla — mora. hofíʻa.");
    return true;
}

/* ==================================================================
 * API PUBLICA - demuestra_justicia (Rom 3:25 - ilastirion)
 * ================================================================== */

/**
 * hashmal_consensus_demuestra_justicia - Rom 3:25
 *
 * "hon proetheto ho theos hilastirion... eis endeixin tes
 *  dikaiosynes autou": a quien Dios puso como propiciatorio...
 * para DEMOSTRACION de su justicia.
 *
 * El Kaporet demuestra justicia CUANDO un ciclo Lv 16 completo
 * emite voz (kaporet_verificacion.c enciende el flag al emitir
 * EMITIDO). Lectura simple del flag - cumulativo: una vez
 * demostrada, queda demostrada.
 */
bool hashmal_consensus_demuestra_justicia(void)
{
    const hashmal_kaporet_t *kap = hashmal_consensus_get_kaporet();
    if (kap == NULL) return false;
    return kap->demuestra_justicia;
}

/* ==================================================================
 * API PUBLICA - universal_abierto (1 Jn 2:2 - hilasmos)
 * ================================================================== */

/**
 * hashmal_consensus_universal_abierto - 1 Jn 2:2
 *
 * "autos hilasmos estin peri ton hamartion hemon, ou peri ton
 *  hemeteron de monon, alla kai peri holou tou kosmou": El es la
 * propiciacion por nuestros pecados, y no solo por los nuestros
 * sino tambien por los de TODO el mundo.
 *
 * El Kaporet es UNIVERSAL por diseno - accesible al que entre
 * correctamente (no exclusivo por pertenencia). Flag de diseno
 * (true desde init en kaporet_core.c). Lectura simple.
 */
bool hashmal_consensus_universal_abierto(void)
{
    const hashmal_kaporet_t *kap = hashmal_consensus_get_kaporet();
    if (kap == NULL) return false;
    return kap->universal;
}

/* ==================================================================
 * API PUBLICA - mishpat_con_hesed (Sal 85:10)
 * ================================================================== */

/**
 * hashmal_consensus_mishpat_con_hesed - Salmo 85:10
 *
 * "jesed ve'emet nifgashu, tsedeq veshalom nashaqu": misericordia
 * y verdad SE ENCONTRARON (nifgashu - raiz pagash); justicia y
 * paz se besaron. El Kaporet es ese lugar.
 *
 * En firmware: el ultimo consenso CUMPLIO AMBAS:
 *   emet  -> validez tecnica: ultimo.estado == EMITIDO (testigos concordaron)
 *   jesed -> apertura: universal == true (accesible al que entre)
 *
 * Si ambas -> hesed ve'emet se encontraron. Si el ultimo fue
 * DIVERGIDO, hubo emet fallido (no se levanto davar) -> false.
 * Si universal es false (diseno cerrado, que no deberia ocurrir) ->
 * jesed ausente -> false.
 */
bool hashmal_consensus_mishpat_con_hesed(void)
{
    const hashmal_kaporet_t *kap = hashmal_consensus_get_kaporet();
    if (kap == NULL) return false;

    const bool emet_cumplida  = (kap->ultimo.estado == HASHMAL_CONSENSO_EMITIDO)
                              && kap->ultimo.emanada;
    const bool jesed_presente = kap->universal;

    return emet_cumplida && jesed_presente;
}

/* ==================================================================
 * API PUBLICA - auditoria_obtener (Ez 10:12 / Ap 4:8 - llenos de ojos)
 * ================================================================== */

/**
 * hashmal_consensus_auditoria_obtener - ojos mirando atras
 *
 * "vejol besaram... mel'im 'enayim saviv" - Ez 10:12.
 * Cada observacion historica es un ojo. La auditoria es el conjunto
 * de ojos mirando hacia atras.
 *
 * --- IMPLEMENTACION v1 ---
 *
 * Esta version inicial devuelve SOLO el ultimo resultado (kap->ultimo)
 * si su timestamp >= desde_timestamp y si efectivamente hubo un
 * ciclo ejecutado (timestamp > 0). Mas ciclos requeriran un ring
 * buffer - TODO cuando pieza 12 (hashmal_healthcheck / Kiyor)
 * gestione el historico general del Mishkan y nos de el buffer
 * compartido.
 *
 * --- CONTRATO ---
 *
 *   - out, out_escritos != NULL
 *   - max_registros >= 1 (sin espacio, no hay donde escribir)
 *   - *out_escritos sera 0 o 1 en esta v1
 *   - out[0] se escribe solo si:
 *       ultimo.timestamp > 0               (alguna vez hubo ciclo)
 *       Y ultimo.timestamp >= desde_ts     (dentro de la ventana)
 *
 * --- NOTA DE UNIDADES ---
 *
 * ultimo.timestamp esta en microsegundos (esp_timer_get_time).
 * El parametro desde_timestamp es uint32_t - puede rebosar a las
 * ~1.2 h de uptime si se usa microsegundos. Unidad del caller es
 * su responsabilidad; aqui comparamos tras promover a uint64_t.
 */
int hashmal_consensus_auditoria_obtener(uint32_t desde_timestamp,
                                        hashmal_consenso_resultado_t *out,
                                        size_t max_registros,
                                        size_t *out_escritos)
{
    if (out == NULL || out_escritos == NULL) {
        KAP_LOG_E("auditoria_obtener: out u out_escritos NULL");
        return HASHMAL_KAPORET_ERR_NULL;
    }
    if (max_registros < 1u) {
        KAP_LOG_E("auditoria_obtener: max_registros=0 — sin espacio");
        return HASHMAL_KAPORET_ERR_NULL;
    }

    const hashmal_kaporet_t *kap = hashmal_consensus_get_kaporet();
    if (kap == NULL) {
        KAP_LOG_E("auditoria_obtener: Kapóret no levantado");
        *out_escritos = 0u;
        return HASHMAL_KAPORET_ERR_INIT;
    }

    const uint64_t desde = (uint64_t)desde_timestamp;
    const bool hubo_ciclo   = (kap->ultimo.timestamp > 0u);
    const bool dentro       = (kap->ultimo.timestamp >= desde);

    if (hubo_ciclo && dentro) {
        out[0]        = kap->ultimo;
        *out_escritos = 1u;
        KAP_LOG_I("auditoria: 1 registro @%" PRIu64 " us (estado=%d, "
                  "emanada=%d)",
                  kap->ultimo.timestamp,
                  (int)kap->ultimo.estado,
                  (int)kap->ultimo.emanada);
    } else {
        *out_escritos = 0u;
        KAP_LOG_I("auditoria: 0 registros (ningún ciclo en ventana)");
    }

    return HASHMAL_KAPORET_OK;
}

/* ==================================================================
 * AÑADIDURA #20 — NEDER (Nm 30:3) — voto inquebrantable
 *
 * "ish kí yiddór NEDER laYHWH... LO YAJÉL devaró; kekol hayyotsé
 *  mippív yaʻasé." — Nm 30:3
 *
 * Tabla de 12 slots (Éx 28:21 — pectoral 12 piedras = INV-TABLA-12).
 * Por afinidad semántica con noʻadtí (cita designada): noʻadtí dice
 * "estaré allí"; neder dice "haré aquello". Misma raíz pragmática —
 * compromiso público a una acción futura.
 *
 * Lazy sweep: la transición COMPROMETIDO → QUEBRANTADO al expirar
 * plazo se aplica cuando alguien lee (obtener/capacidad), sin task
 * de fondo. Razón: este componente es STATELESS por v.17 (no hay
 * "altura" / qomáh) — un task perpetuo añadiría estado externo al
 * Kapóret. La voz se declara sola (middabbér Hitpael, Nm 7:89);
 * análogamente, la quebrantadura se declara sola al consultarse.
 * ================================================================== */

#include "esp_timer.h"

static hashmal_neder_t g_neder_tabla[HASHMAL_NEDER_MAX_ACTIVOS];

static inline uint64_t neder_ahora_us(void)
{
    return (uint64_t)esp_timer_get_time();
}

/* Lazy sweep: marca QUEBRANTADO cualquier COMPROMETIDO con plazo
 * vencido. Idempotente. No emite log por slot — el log se emite
 * al obtener (interfaz pública) para que el caller vea el evento. */
static void neder_barrer_vencidos(uint64_t ahora)
{
    for (uint8_t i = 0; i < HASHMAL_NEDER_MAX_ACTIVOS; ++i) {
        hashmal_neder_t *n = &g_neder_tabla[i];
        if (n->estado == HASHMAL_NEDER_COMPROMETIDO &&
            ahora > (n->timestamp_aceptado_us + n->plazo_us)) {
            n->estado              = HASHMAL_NEDER_QUEBRANTADO;
            n->timestamp_cierre_us = ahora;
        }
    }
}

int hashmal_consensus_neder_aceptar(hashmal_neder_tipo_t tipo,
                                    const uint8_t payload_hash[HASHMAL_ZER_HASH_BYTES],
                                    uint64_t      plazo_us,
                                    bool          capacidad_garantizada,
                                    uint8_t      *out_id)
{
    if (payload_hash == NULL || out_id == NULL) {
        KAP_LOG_E("neder_aceptar: payload_hash u out_id NULL");
        return HASHMAL_KAPORET_ERR_NULL;
    }

    /* Regla 1 (Ec 5:5) — tov ashér ló tiddór mishét-tiddór veló
     * teshalém. Mejor no prometer que prometer y no pagar. Si el
     * caller no garantiza capacidad, RECHAZAR ANTES de comprometer. */
    if (!capacidad_garantizada) {
        KAP_LOG_W("neder_aceptar: capacidad no garantizada — Ec 5:5 "
                  "(tov ashér ló tiddór). Rechazado.");
        return HASHMAL_KAPORET_ERR_NEDER_NO_GARANTIZA;
    }

    /* Kapóret debe estar levantado — un voto sin Kapóret no tiene
     * dónde "salir de la boca" (kayotsé mippív). */
    if (hashmal_consensus_get_kaporet() == NULL) {
        KAP_LOG_E("neder_aceptar: Kapóret no levantado");
        return HASHMAL_KAPORET_ERR_INIT;
    }

    const uint64_t ahora = neder_ahora_us();
    neder_barrer_vencidos(ahora);

    /* Buscar primer LIBRE. Pectoral de 12 — si los 12 están
     * comprometidos no se acepta uno más (Éx 28:21 límite). */
    for (uint8_t i = 0; i < HASHMAL_NEDER_MAX_ACTIVOS; ++i) {
        if (g_neder_tabla[i].estado == HASHMAL_NEDER_LIBRE ||
            g_neder_tabla[i].estado == HASHMAL_NEDER_CUMPLIDO ||
            g_neder_tabla[i].estado == HASHMAL_NEDER_LIBERADO ||
            g_neder_tabla[i].estado == HASHMAL_NEDER_QUEBRANTADO) {
            hashmal_neder_t *n = &g_neder_tabla[i];
            n->estado                = HASHMAL_NEDER_COMPROMETIDO;
            n->tipo                  = tipo;
            memcpy(n->payload_hash, payload_hash, HASHMAL_ZER_HASH_BYTES);
            n->timestamp_aceptado_us = ahora;
            n->timestamp_cierre_us   = 0;
            n->plazo_us              = plazo_us;
            n->causa_liberacion      = (hashmal_neder_causa_liberacion_t)0;
            *out_id = i;
            KAP_LOG_I("neder_aceptar: slot=%u tipo=%d plazo=%" PRIu64 "us "
                      "(Nm 30:3 — yiddór neder)", i, (int)tipo, plazo_us);
            return HASHMAL_KAPORET_OK;
        }
    }

    KAP_LOG_E("neder_aceptar: tabla 12/12 llena (Éx 28:21 — pectoral)");
    return HASHMAL_KAPORET_ERR_NEDER_TABLA_LLENA;
}

int hashmal_consensus_neder_cumplir(uint8_t id)
{
    if (id >= HASHMAL_NEDER_MAX_ACTIVOS) {
        KAP_LOG_E("neder_cumplir: id=%u fuera de rango", id);
        return HASHMAL_KAPORET_ERR_NEDER_ID;
    }

    hashmal_neder_t *n = &g_neder_tabla[id];
    if (n->estado != HASHMAL_NEDER_COMPROMETIDO) {
        KAP_LOG_E("neder_cumplir: id=%u no está COMPROMETIDO (estado=%d)",
                  id, (int)n->estado);
        return HASHMAL_KAPORET_ERR_NEDER_ID;
    }

    const uint64_t ahora    = neder_ahora_us();
    const uint64_t deadline = n->timestamp_aceptado_us + n->plazo_us;

    if (ahora > deadline) {
        /* Regla 3 (Ec 5:4 al teʼajér) violada. lo yajél (Nm 30:3) —
         * el voto se quebrantó antes de cumplirse. Marcar y reportar. */
        n->estado              = HASHMAL_NEDER_QUEBRANTADO;
        n->timestamp_cierre_us = ahora;
        KAP_LOG_E("neder_cumplir: id=%u TARDE (Ec 5:4 al teʼajér violado, "
                  "Nm 30:3 lo yajél). delta=%" PRIu64 "us",
                  id, ahora - deadline);
        return HASHMAL_KAPORET_ERR_NEDER_QUEBRANTADO;
    }

    /* Sal 116:14 — shillem nidró. Cumplido a tiempo. */
    n->estado              = HASHMAL_NEDER_CUMPLIDO;
    n->timestamp_cierre_us = ahora;
    KAP_LOG_I("neder_cumplir: id=%u CUMPLIDO (Sal 116:14 — shillem nidró)", id);
    return HASHMAL_KAPORET_OK;
}

int hashmal_consensus_neder_liberar(uint8_t                          id,
                                    hashmal_neder_causa_liberacion_t causa)
{
    if (id >= HASHMAL_NEDER_MAX_ACTIVOS) {
        KAP_LOG_E("neder_liberar: id=%u fuera de rango", id);
        return HASHMAL_KAPORET_ERR_NEDER_ID;
    }

    /* Whitelist estricta — Nm 30:4-8 sólo permite anular voto por
     * autoridad superior; un nodo no puede invocarse a sí mismo. */
    if (causa != HASHMAL_NEDER_LIB_BATERIA_CRITICA &&
        causa != HASHMAL_NEDER_LIB_RUTA_INNECESARIA &&
        causa != HASHMAL_NEDER_LIB_SHABAT) {
        KAP_LOG_E("neder_liberar: causa=%d ilegítima (Nm 30:4-8)", (int)causa);
        return HASHMAL_KAPORET_ERR_NEDER_LIBERACION_ILEGITIMA;
    }

    hashmal_neder_t *n = &g_neder_tabla[id];
    if (n->estado != HASHMAL_NEDER_COMPROMETIDO) {
        KAP_LOG_E("neder_liberar: id=%u no está COMPROMETIDO (estado=%d)",
                  id, (int)n->estado);
        return HASHMAL_KAPORET_ERR_NEDER_ID;
    }

    n->estado              = HASHMAL_NEDER_LIBERADO;
    n->causa_liberacion    = causa;
    n->timestamp_cierre_us = neder_ahora_us();
    KAP_LOG_I("neder_liberar: id=%u causa=%d (Nm 30:4-8 — autoridad legítima)",
              id, (int)causa);
    return HASHMAL_KAPORET_OK;
}

int hashmal_consensus_neder_obtener(uint8_t id, hashmal_neder_t *out)
{
    if (out == NULL) {
        KAP_LOG_E("neder_obtener: out NULL");
        return HASHMAL_KAPORET_ERR_NULL;
    }
    if (id >= HASHMAL_NEDER_MAX_ACTIVOS) {
        KAP_LOG_E("neder_obtener: id=%u fuera de rango", id);
        return HASHMAL_KAPORET_ERR_NEDER_ID;
    }

    /* Lazy sweep antes de leer — la quebrantadura se declara sola
     * (paralelo middabbér Hitpael de Nm 7:89). */
    neder_barrer_vencidos(neder_ahora_us());

    *out = g_neder_tabla[id];
    return HASHMAL_KAPORET_OK;
}

uint8_t hashmal_consensus_neder_capacidad(void)
{
    neder_barrer_vencidos(neder_ahora_us());

    uint8_t libres = 0;
    for (uint8_t i = 0; i < HASHMAL_NEDER_MAX_ACTIVOS; ++i) {
        const hashmal_neder_estado_t e = g_neder_tabla[i].estado;
        if (e != HASHMAL_NEDER_COMPROMETIDO) {
            ++libres;
        }
    }
    return libres;
}

/* ==================================================================
 *
 *   PIEZA 02 - KAPORET - COMPLETA.
 *
 *   5 archivos, UNA SOLA pieza batida (miqsha - Ex 25:18):
 *
 *     kaporet_core.c          v.17-21 - construir y encajar
 *     kaporet_validadores.c   v.18-20 + Dt 19:15 - querubines operan
 *     kaporet_ataques.c       Lv 10:1-2 + Gn 3:24 - espada guarda
 *     kaporet_verificacion.c  Lv 16 completo - la voz emana
 *     kaporet_rendezvous.c    Ex 25:22 + Sal 80:1 - la cita y la morada
 *
 *   "veno'adti leja sham vedibarti iteja me'al haKaporet mibein
 *    shene hakeruvim" - Exodo 25:22
 *   "Me encontrare contigo alli y te hablare desde encima del
 *    Kaporet, desde ENTRE los dos querubines."
 *
 *   "vayyishma' et haQol midabber elav" - Numeros 7:89
 *   "Oyo la voz que le hablaba."
 *
 *   Ex 40:3 coloca el Arca y el Kaporet en el mismo paso.
 *   Piezas 01 y 02 estan listas. El Mishkan tiene centro.
 *
 * ================================================================== */
