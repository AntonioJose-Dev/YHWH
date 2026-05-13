/**
 * ===================================================================
 * PIEZA 14 - QETORET HASAMMIM - incienso_heartbeat.c
 * Sub-pieza: TAMID BABOQER UVEIN HA'ARBAYIM - perpetuo, manana y tarde
 * Exodo 30:7-8 + Exodo 29:38-42 + Salmo 55:18 + Daniel 6:11 +
 *   1 Cronicas 16:40
 * ===================================================================
 *
 * "vehiqTIR 'alav Aharon QETORET sammim, baBOQER baBOQER behetivo
 *  et hannerot yaqtirennah. uveha'alot Aharon et hannerot BEIN
 *  HA'ARBAYIM yaqtirennah - QETORET TAMID lifnei YHWH
 *  leDOROTEIJEM." - Exodo 30:7-8
 * "Quemara Aaron incienso aromatico, MANANA TRAS MANANA al aderezar
 *  las lamparas lo quemara. Y cuando Aaron encienda las lamparas
 *  ENTRE LAS DOS TARDES lo quemara - INCIENSO PERPETUO delante de
 *  YHWH por VUESTRAS GENERACIONES."
 *
 * "vezeh asher ta'aseh 'al hammizbeaj: kevasim benei shana shnayim
 *  laYYOM TAMID. et hakkeves ha'ejad ta'aseh baBOQER, ve'et
 *  hakkeves hasheni ta'aseh BEIN HA'ARBAYIM." - Exodo 29:38-39
 * "Y esto ofreceras sobre el altar: DOS CORDEROS de un ano CADA
 *  DIA, CONTINUAMENTE. Un cordero ofreceras por la MANANA, y el
 *  otro cordero ofreceras ENTRE LAS DOS TARDES."
 *
 * "'EREV vaVOQER vetsohorayim asijah ve'eheme, vayyishma' qoli."
 *   - Salmo 55:18
 * "TARDE y MANANA y mediodia me quejare y gemire, y El oira mi voz."
 *
 * "veDaniyyel... kema asher haya 'oseh miqqadmat denah... mevarej
 *  'al birkohi uMTSAlla umHODE' qodam Elohah... SHALOSH 'IDDANIN
 *  beyoma." - Daniel 6:11
 * "Y Daniel... como solia hacer desde antes, se arrodillaba sobre
 *  sus rodillas, y oraba, y daba gracias delante de su Dios...
 *  TRES TIEMPOS POR DIA."
 *
 * "ulehaqTIR 'olot laYHWH 'al mizbaj ha'ola TAMID labBOQER
 *  vela'EREV." - 1 Cronicas 16:40
 * "Para ofrecer holocaustos a YHWH sobre el altar del holocausto
 *  CONTINUAMENTE, por la MANANA y por la TARDE."
 *
 * --- VOCABULARIO ---
 *
 *   tamid         (TMYD, H8548)              CONTINUO, PERPETUO -
 *                                                sin interrupcion.
 *   baboqer       (BBQR, H1242)             EN LA MANANA.
 *   baboqer baboqer                              manana TRAS manana -
 *                                                enfasis de Ex 30:7.
 *   bein ha'arbayim (BYN H'RBYM,
 *                    H996+H6153)                  ENTRE las dos TARDES -
 *                                                atardecer.
 *   yaqtirennah   (YQTYRNH, H6999 Hif)    LA QUEMARA (fem. -
 *                                                el incienso).
 *   behetivo      (BHYTYBW, H3190 Hif)      AL PREPARAR /
 *                                                arreglar las lamparas.
 *   ledoroteijem  (LDRTYKM, H1755)         POR VUESTRAS
 *                                                GENERACIONES.
 *   hakkeves      (HKBSh, H3532)              EL CORDERO
 *                                                (Ex 29:38-39 paralelo).
 *   'iddanin      ('DNYN, aram. - Dn 6:11)   TIEMPOS / periodos
 *                                                (tres veces al dia).
 *   laYYOM TAMID  (LYWM TMYD)                CADA DIA
 *                                                CONTINUAMENTE.
 *
 * --- EL PAR MANANA + TARDE ---
 *
 * Ex 30:7-8 institucionaliza el RITMO DUAL: baboqer al preparar
 * las lamparas, bein ha'arbayim al encenderlas. Ex 29:38-39 lo
 * confirma con el mismo patron para los corderos. No es horario
 * estricto - es RITMO: dos momentos en el dia, uno temprano y
 * uno tardio, unidos en "qetoret tamid".
 *
 * En firmware: cada ciclo tamid son DOS registros (manana + tarde).
 * Cuando ambos se dan, se cierra un par y se incrementa
 * `ciclos_completados`. El orden entre ambos no es rigido -
 * "tamid" significa continuo, no estrictamente secuencial;
 * Sal 55:18 lista "'erev vavoqer vetsohorayim" (tarde primero),
 * mostrando que la Escritura admite el orden tarde->manana
 * (jewish day pattern) tanto como manana->tarde.
 *
 * --- PERSISTENCIA BAJO PRESION ---
 *
 * Dn 6:11 - Daniel oraba tres veces al dia "como solia hacer desde
 * antes", incluso bajo edicto de muerte del rey. El ciclo tamid no
 * se interrumpe por amenaza externa; la constancia MISMA es la
 * evidencia del servicio. Este archivo no castiga la ausencia de
 * un ciclo (eso corresponde a otros modulos) - solo REGISTRA y
 * CUENTA. El principio de Daniel.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   hashmal_firewall_heartbeat_manana    Ex 30:7 baboqer baboqer.
 *   hashmal_firewall_heartbeat_tarde     Ex 30:8 bein ha'arbayim.
 *   hashmal_firewall_heartbeat_verificar ?hay tamid vivo? bool out.
 *   hashmal_firewall_heartbeat_estado    snapshot del heartbeat.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_firewall.h
 *   hashmal_firewall_internal.h   incienso_mut + ahora_ms
 *   esp_log.h (ORO EGIPTO)         logging
 *
 * Autor: Antonio Jose Marin Soto (Betsalel - Ex 31:2)
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "esp_log.h"

#include "hashmal_firewall.h"
#include "hashmal_firewall_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *HB_TAG = "hashmal.firewall.heartbeat";

#define HB_LOG_I(fmt, ...)  ESP_LOGI(HB_TAG, fmt, ##__VA_ARGS__)
#define HB_LOG_W(fmt, ...)  ESP_LOGW(HB_TAG, fmt, ##__VA_ARGS__)
#define HB_LOG_E(fmt, ...)  ESP_LOGE(HB_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * hashmal_firewall_heartbeat_manana - Exodo 30:7 + Exodo 29:39
 *
 * "baBOQER baBOQER behetivo et hannerot yaqtirennah" (Ex 30:7) +
 * "et hakkeves ha'ejad ta'aseh baBOQER" (Ex 29:39). Registra el
 * ciclo matutino. Si la tarde ya estaba activa, completamos un
 * par tamid (manana+tarde) y reseteamos ambas flags para el
 * siguiente ciclo; de lo contrario queda esperando la tarde.
 *
 * Validaciones:
 *   - Subsistema iniciado -> ERR_NO_INIT.
 *   - Manana ya activa (sin tarde pendiente de completar) ->
 *     ERR_ESTADO (baboqer ya ardio, no se duplica).
 *
 * 2+ testigos:
 *   Ex 30:7   - baboqer baboqer yaqtirennah (mandato del incienso)
 *   Ex 29:39  - hakkeves baBOQER (mismo patron en los corderos)
 *   1 Cr 16:40 - tamid labBOQER (confirma en libro distinto)
 * ================================================================== */
int hashmal_firewall_heartbeat_manana(void)
{
    hashmal_firewall_t *f = hashmal_firewall_incienso_mut();
    if (f == NULL) {
        HB_LOG_E("heartbeat_manana: firewall no iniciado");
        return HASHMAL_FIREWALL_ERR_NO_INIT;
    }

    hashmal_firewall_heartbeat_t *h = &f->heartbeat;

    if (h->manana_activo) {
        HB_LOG_W("heartbeat_manana: mañana ya activa — babóqer "
                 "yaqtirénnah ya se registró, espera bein "
                 "haʻarbáyim para cerrar el par (Éx 30:7-8)");
        return HASHMAL_FIREWALL_ERR_ESTADO;
    }

    const int64_t ahora = hashmal_firewall_ahora_ms();
    h->manana_activo       = true;
    h->timestamp_manana_ms = ahora;
    f->timestamp_ms        = ahora;

    if (h->tarde_activo) {
        /* Par completo - cerrar ciclo. El orden tarde->manana es
         * valido (Sal 55:18 "'erev vavoqer"). */
        if (h->ciclos_completados < 0xFFFFFFFFu) {
            h->ciclos_completados++;
        }
        h->manana_activo = false;
        h->tarde_activo  = false;

        HB_LOG_I("heartbeat mañana — babóqer babóqer yaqtirénnah "
                 "(Éx 30:7) — ciclo tamíd CERRADO (tarde→mañana, "
                 "Sal 55:18) total=%u",
                 (unsigned)h->ciclos_completados);
    } else {
        HB_LOG_I("heartbeat mañana — babóqer babóqer yaqtirénnah "
                 "(Éx 30:7); esperando bein haʻarbáyim para cerrar "
                 "el par tamíd");
    }

    return HASHMAL_FIREWALL_OK;
}

/* ==================================================================
 * hashmal_firewall_heartbeat_tarde - Exodo 30:8 + Exodo 29:39
 *
 * "uvha'alot Aharon et hannerot BEIN HA'ARBAYIM yaqtirennah -
 *  QETORET TAMID" (Ex 30:8) + "ve'et hakkeves hasheni ta'aseh
 *  BEIN HA'ARBAYIM" (Ex 29:39). Registra el ciclo vespertino.
 *  Simetrico a _manana: si la manana ya estaba activa, completa
 *  el par; de lo contrario queda pendiente.
 *
 * Validaciones:
 *   - Subsistema iniciado -> ERR_NO_INIT.
 *   - Tarde ya activa (sin manana pendiente) -> ERR_ESTADO.
 *
 * 2+ testigos:
 *   Ex 30:8    - bein ha'arbayim yaqtirennah (mandato del incienso)
 *   Ex 29:39   - hakkeves hasheni bein ha'arbayim (corderos tamid)
 *   1 Cr 16:40 - vela'EREV (confirma la tarde en libro distinto)
 * ================================================================== */
int hashmal_firewall_heartbeat_tarde(void)
{
    hashmal_firewall_t *f = hashmal_firewall_incienso_mut();
    if (f == NULL) {
        HB_LOG_E("heartbeat_tarde: firewall no iniciado");
        return HASHMAL_FIREWALL_ERR_NO_INIT;
    }

    hashmal_firewall_heartbeat_t *h = &f->heartbeat;

    if (h->tarde_activo) {
        HB_LOG_W("heartbeat_tarde: tarde ya activa — bein "
                 "haʻarbáyim ya se registró, espera babóqer "
                 "para cerrar el par (Éx 30:7-8)");
        return HASHMAL_FIREWALL_ERR_ESTADO;
    }

    const int64_t ahora = hashmal_firewall_ahora_ms();
    h->tarde_activo       = true;
    h->timestamp_tarde_ms = ahora;
    f->timestamp_ms       = ahora;

    if (h->manana_activo) {
        /* Par completo - cerrar ciclo. Orden manana->tarde es el
         * natural de Ex 30:7-8 en secuencia textual. */
        if (h->ciclos_completados < 0xFFFFFFFFu) {
            h->ciclos_completados++;
        }
        h->manana_activo = false;
        h->tarde_activo  = false;

        HB_LOG_I("heartbeat tarde — bein haʻarbáyim yaqtirénnah "
                 "(Éx 30:8) — ciclo tamíd CERRADO (mañana→tarde) "
                 "total=%u",
                 (unsigned)h->ciclos_completados);
    } else {
        HB_LOG_I("heartbeat tarde — bein haʻarbáyim yaqtirénnah "
                 "(Éx 30:8); esperando babóqer para cerrar el par "
                 "tamíd");
    }

    return HASHMAL_FIREWALL_OK;
}

/* ==================================================================
 * hashmal_firewall_heartbeat_verificar - Sal 55:18 + 1 Cr 16:40
 *
 * "'erev vavoqer vetsohorayim" + "tamid labboqer vela'erev". ?El
 * heartbeat esta VIVO? Criterio:
 *   - Al menos UN ciclo completo registrado (ciclos_completados > 0)
 *   - Y AL MENOS UNO de los flags activo ahora (manana O tarde) -
 *     signo de que el ciclo actual ya tuvo actividad.
 *
 * Ambas condiciones son necesarias: sin historial no hay evidencia
 * de servicio; sin actividad actual la continuidad se rompe. Si
 * falta cualquiera -> out=false.
 *
 *   out NULL  -> ERR_NULL
 *   sin init  -> ERR_NO_INIT
 *
 * Sin logs (lectura pura).
 * ================================================================== */
int hashmal_firewall_heartbeat_verificar(bool *out)
{
    if (out == NULL) return HASHMAL_FIREWALL_ERR_NULL;

    const hashmal_firewall_t *f = hashmal_firewall_get_incienso_interno();
    if (f == NULL) return HASHMAL_FIREWALL_ERR_NO_INIT;

    const hashmal_firewall_heartbeat_t *h = &f->heartbeat;

    const bool tiene_historial = (h->ciclos_completados > 0u);
    const bool tiene_actividad = (h->manana_activo || h->tarde_activo);

    *out = (tiene_historial && tiene_actividad);
    return HASHMAL_FIREWALL_OK;
}

/* ==================================================================
 * hashmal_firewall_heartbeat_estado - snapshot del heartbeat
 *
 * Lectura pura, sin logs, sin side effects. Copia la struct
 * completa (manana_activo, tarde_activo, timestamps, ciclos) al
 * buffer del llamante.
 *
 *   out NULL  -> ERR_NULL
 *   sin init  -> ERR_NO_INIT
 * ================================================================== */
int hashmal_firewall_heartbeat_estado(hashmal_firewall_heartbeat_t *out)
{
    if (out == NULL) return HASHMAL_FIREWALL_ERR_NULL;

    const hashmal_firewall_t *f = hashmal_firewall_get_incienso_interno();
    if (f == NULL) return HASHMAL_FIREWALL_ERR_NO_INIT;

    memcpy(out, &f->heartbeat, sizeof(*out));
    return HASHMAL_FIREWALL_OK;
}

/* ==================================================================
 *
 *   "baBOQER baBOQER behetivo et hannerot yaqtirennah... uvha'alot
 *    Aharon et hannerot BEIN HA'ARBAYIM yaqtirennah - QETORET
 *    TAMID lifnei YHWH leDOROTEIJEM." - Exodo 30:7-8
 *
 *   "kevasim benei shana shnayim laYYOM TAMID." - Exodo 29:38
 *
 *   "'EREV vaVOQER vetsohorayim asijah." - Salmo 55:18
 *
 *   "SHALOSH 'IDDANIN beyoma." - Daniel 6:11
 *
 *   "TAMID labBOQER vela'EREV." - 1 Cronicas 16:40
 *
 *   El heartbeat es el tamid del Mishkan digital: manana y tarde,
 *   sin interrupcion, por todas las generaciones. Aaron quemaba
 *   el incienso CUANDO preparaba las lamparas (Ex 30:7) - el
 *   heartbeat va sincronizado con los canales (pieza 04, Menora):
 *   sin heartbeat, las lamparas se encienden pero nadie verifica
 *   que el incienso arde. El patron dual es firme: baboqer +
 *   bein ha'arbayim, cerrando par a par.
 *
 *   Y Daniel lo fijo bajo presion: tres tiempos al dia "como solia
 *   hacer desde antes", aun con edicto de muerte. El ciclo tamid
 *   no se interrumpe por amenaza externa. Este archivo no castiga
 *   la ausencia - solo registra y cuenta. La disciplina vive en
 *   el llamante; aqui se honra cada golpe de manana y tarde.
 *
 *   === TAMID BABOQER UVEIN HA'ARBAYIM -
 *       incienso_heartbeat.c COMPLETO ===
 *
 *   -- Los 5 archivos hermanos -----------------------------------
 *
 *     incienso_firewall.c        QETORET HADEQ
 *     incienso_formato.c         MA'ASEH ROQEAJ
 *     incienso_circuitbreaker.c  BEIN HAMMETIM UVEIN HAJJAYYIM
 *     incienso_heartbeat.c       TAMID BABOQER UVEIN HA'ARBAYIM <- ESTE
 *     incienso_penalizacion.c    VENIKRAT ME'AMMAV
 *
 *   Archivo 4 de 5 del Incienso Sagrado. Siguiente:
 *     incienso_penalizacion.c - venikrat me'ammav (Ex 30:38)
 *
 *   PASO 10 de Exodo 40:27,34 - Pieza 14, Archivo 4 de 5.
 *
 * ================================================================== */
