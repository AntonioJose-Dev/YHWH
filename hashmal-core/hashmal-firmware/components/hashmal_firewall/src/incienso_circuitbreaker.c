/**
 * ===================================================================
 * PIEZA 14 - QETORET HASAMMIM - incienso_circuitbreaker.c
 * Sub-pieza: BEIN HAMMETIM UVEIN HAJJAYYIM - entre los muertos y los vivos
 * Numeros 17:11-15 + Levitico 16:12-13 + 2 Cronicas 26:16-20
 * ===================================================================
 *
 * "vayyomer Mosheh el Aharon: qaj et HAMMAJTA veten 'aleha esh
 *  me'al hammizbeaj vesim QETORET veholej meherah el ha'edah
 *  veJAPPER 'aleihem - ki yatsa' haqqetsef millifnei YHWH, hejel
 *  hanNAGEF." - Numeros 17:11
 * "Moises dijo a Aaron: toma el INCENSARIO, pon fuego del altar y
 *  pon INCIENSO, y ve RAPIDO a la congregacion y HAZ EXPIACION
 *  por ellos - porque salio la IRA de delante de YHWH, comenzo
 *  la PLAGA."
 *
 * "vayyiqqaj Aharon ka'asher dibber Mosheh vayyarots el toj
 *  haqqahal - vehinneh hejel hannegef ba'am - vayyitten et
 *  haqqetoret vayjapper 'al ha'am." - Numeros 17:12
 * "Aaron tomo [el incensario] como dijo Moises, y CORRIO al medio
 *  de la congregacion - y he aqui la plaga ya habia comenzado en
 *  el pueblo - puso el incienso e hizo expiacion por el pueblo."
 *
 * "vayya'aMOD BEIN HAMMETIM UVEIN HAJJAYYIM - vatte'aTSAR
 *  hammaggefa." - Numeros 17:13
 * "Y se puso DE PIE ENTRE LOS MUERTOS Y LOS VIVOS - y la plaga
 *  SE DETUVO."
 *
 * "vayyashov Aharon el Mosheh el petaj ohel mo'ed - vehammaggefa
 *  ne'etsarah." - Numeros 17:15
 * "Aaron regreso a Moises, a la puerta de la Tienda del Encuentro
 *  - y la plaga habia sido detenida."
 *
 * "velaqaj melo' hammajta gajalei esh me'al hammizbeaj milifnei
 *  YHWH umelo' jofnav QETORET sammim DAQQA vehevi' mibeit
 *  lapparojet. venatan et haqqetoret 'al ha'esh lifnei YHWH,
 *  veJISSAH 'anan haqqetoret et HAKKAPORET asher 'al ha'edut
 *  veLO YAMUT." - Levitico 16:12-13
 * "Tomara un incensario lleno de brasas del altar... y la NUBE
 *  del incienso CUBRIRA el propiciatorio que esta sobre el
 *  Testimonio, y NO MORIRA."
 *
 * "uvjetsqato gavah libbo 'ad lehashjit, vayyim'al baYHWH
 *  Elohav vayyavo el heijal YHWH leHAQTIR 'al mizbaj haqqetoret.
 *  ...vehatsara'at zareja bemitsjo." - 2 Cronicas 26:16,19
 * "Y cuando se fortalecio [el rey Uzias], se enaltecio su
 *  corazon hasta corromperse, y transgredio contra YHWH su Dios,
 *  entrando al Templo de YHWH para quemar incienso sobre el altar
 *  del incienso... la lepra broto en su frente."
 *
 * (Solo el autorizado activa el circuit breaker. Uzias lo intento
 *  sin oficio y quedo cortado.)
 *
 * --- VOCABULARIO ---
 *
 *   hammajta       (HMChTH, H4289)          EL INCENSARIO.
 *   qetoret        (QTRT, H7004)              INCIENSO.
 *   japper         (KPR, H3722 Piel)           HACER EXPIACION,
 *                                                   cubrir.
 *   hammaggefa     (HMGPH, H4046)            LA PLAGA -
 *                                                   emergencia catastrofica.
 *   haqqetsef      (HQTsP, H7110)                LA IRA - salida
 *                                                   de YHWH que pide
 *                                                   contencion.
 *   meherah        (MHRH, H4120)                 RAPIDO, con
 *                                                   urgencia.
 *   vayyarots      (WYRTs, H7323 Qal)             Y CORRIO.
 *   vayya'amod     (WY'MD, H5975 Qal)          Y SE PUSO de pie.
 *   bein           (BYN, H996)                     ENTRE.
 *   hammetim       (HMTYM, H4191)                LOS MUERTOS.
 *   hajjayyim      (HChYYM, H2416)                LOS VIVOS.
 *   ne'etsarah     (N'TsRH, H6113 Nifal)         fue DETENIDA
 *                                                   (la plaga).
 *   vayyashov      (WYShB, H7725)                 VOLVIO - tras
 *                                                   la emergencia.
 *   jissa          (KSH, H3680 Piel)             CUBRIR (Lv 16:13
 *                                                   - proteccion del
 *                                                   nucleo).
 *   ma'al          (M'L, H4603)                    TRANSGREDIR
 *                                                   (2 Cr 26:16
 *                                                   - uso sin oficio).
 *
 * --- PATRON DEL CB ---
 *
 *   ACTIVAR      Aaron toma el incensario y CORRE (Nm 17:11-13):
 *                 detiene todo el trafico, se planta "bein
 *                 hammetim uvein hajjayyim" - frontera de emergencia.
 *                 Toda funcion de filtrado devolvera ERR_CB_ACTIVO
 *                 mientras este abierto (lo aplica incienso_firewall.c).
 *
 *   DESACTIVAR   Aaron VUELVE a la puerta (Nm 17:15): la plaga
 *                 ceso. El trafico se reanuda. No es permanente -
 *                 el CB es expiacion de emergencia, no estado normal.
 *
 *   ESTADO       Lectura pura: ?estamos en emergencia o en paz?
 *                 Sin logs, sin side effects.
 *
 * --- QUIEN AUTORIZA ---
 *
 * 2 Cr 26:16-20 advierte: Uzias intento quemar incienso sin ser
 * sacerdote y su atrevimiento le costo la salud. En firmware: solo
 * quien tiene rol adecuado (verificado en otros modulos) deberia
 * invocar `activar` - este archivo no filtra al llamante por
 * simplicidad, pero deja constancia del principio en el colofon.
 * La pieza 09 (hashmal_roles) es la que identifica al llamante.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   hashmal_firewall_circuitbreaker_activar     Nm 17:11-13 -
 *                                                 Aaron corre, se
 *                                                 planta.
 *   hashmal_firewall_circuitbreaker_desactivar  Nm 17:15 -
 *                                                 Aaron vuelve.
 *   hashmal_firewall_circuitbreaker_estado      lectura pura.
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

#include "esp_log.h"

#include "hashmal_firewall.h"
#include "hashmal_firewall_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *CB_TAG = "hashmal.firewall.circuitbreaker";

#define CB_LOG_I(fmt, ...)  ESP_LOGI(CB_TAG, fmt, ##__VA_ARGS__)
#define CB_LOG_W(fmt, ...)  ESP_LOGW(CB_TAG, fmt, ##__VA_ARGS__)
#define CB_LOG_E(fmt, ...)  ESP_LOGE(CB_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * hashmal_firewall_circuitbreaker_activar - Numeros 17:11-13
 *
 * "qaj et hammajta... veholej MEHERAH el ha'edah veJAPPER 'aleihem"
 * (v.11) -> "vayyarots el toj haqqahal... vayyitten et haqqetoret
 * vayjapper 'al ha'am" (v.12) -> "vayya'amod BEIN HAMMETIM UVEIN
 * HAJJAYYIM - vatte'atsar hammaggefa" (v.13).
 *
 * El circuit breaker es Aaron corriendo con el incensario: no se
 * analiza, se actua. Toda funcion de filtrado devolvera ERR_CB_ACTIVO
 * mientras este abierto.
 *
 * Validaciones:
 *   - Subsistema iniciado -> ERR_NO_INIT.
 *   - CB ya abierto       -> ERR_ESTADO (ya esta plantado en la
 *                            frontera; no se corre dos veces).
 *
 * 2+ testigos:
 *   Nm 17:11  - orden de Moises (mandato)
 *   Nm 17:12  - ejecucion por Aaron (ejecucion)
 *   Nm 17:13  - vayya'amod bein hammetim (el acto de frontera)
 *   Lv 16:13  - jissah 'anan haqqetoret (nube protectora - paralelo)
 * ================================================================== */
int hashmal_firewall_circuitbreaker_activar(void)
{
    hashmal_firewall_t *f = hashmal_firewall_incienso_mut();
    if (f == NULL) {
        CB_LOG_E("cb_activar: firewall no iniciado (init primero)");
        return HASHMAL_FIREWALL_ERR_NO_INIT;
    }

    if (f->circuitbreaker == HASHMAL_FIREWALL_CB_ABIERTO) {
        CB_LOG_W("cb_activar: CB ya ABIERTO — Aarón ya está bein "
                 "hammetím uvein hajjayyím (Nm 17:13), no se corre "
                 "dos veces");
        return HASHMAL_FIREWALL_ERR_ESTADO;
    }

    f->circuitbreaker = HASHMAL_FIREWALL_CB_ABIERTO;
    f->timestamp_ms   = hashmal_firewall_ahora_ms();

    CB_LOG_I("══════ CIRCUIT BREAKER ACTIVADO ══════");
    CB_LOG_I("  qaj et hammajtá... veholéj meheráh (Nm 17:11)");
    CB_LOG_I("  vayyárots el toj haqqahál (Nm 17:12) — Aarón corre");
    CB_LOG_I("  vayyaʻamód BEIN HAMMETÍM UVEIN HAJJAYYÍM (Nm 17:13)");
    CB_LOG_I("  todo tráfico DETENIDO — frontera de emergencia activa");

    return HASHMAL_FIREWALL_OK;
}

/* ==================================================================
 * hashmal_firewall_circuitbreaker_desactivar - Numeros 17:15
 *
 * "vayyashov Aharon el Mosheh el petaj ohel mo'ed - vehammaggefa
 *  NE'ETSARAH" (Nm 17:15). La plaga ceso; Aaron vuelve a la puerta.
 *  El circuit breaker no es permanente: se abre en emergencia y se
 *  cierra cuando la amenaza pasa. El llamante debe haber confirmado
 *  que la causa fue resuelta antes de invocar esta funcion.
 *
 * Validaciones:
 *   - Subsistema iniciado -> ERR_NO_INIT.
 *   - CB ya cerrado       -> ERR_ESTADO (no hay plaga activa que
 *                            detener; vayyashov presupone haber
 *                            estado fuera).
 *
 * 2+ testigos:
 *   Nm 17:15  - vayyashov Aharon (regreso del oficio de emergencia)
 *   Nm 17:13  - vatte'atsar hammaggefa (la plaga ceso - condicion
 *                 para el regreso)
 * ================================================================== */
int hashmal_firewall_circuitbreaker_desactivar(void)
{
    hashmal_firewall_t *f = hashmal_firewall_incienso_mut();
    if (f == NULL) {
        CB_LOG_E("cb_desactivar: firewall no iniciado (init primero)");
        return HASHMAL_FIREWALL_ERR_NO_INIT;
    }

    if (f->circuitbreaker == HASHMAL_FIREWALL_CB_CERRADO) {
        CB_LOG_W("cb_desactivar: CB ya CERRADO — no hay plaga "
                 "que detener (vayyáshov presupone haber estado "
                 "fuera, Nm 17:15)");
        return HASHMAL_FIREWALL_ERR_ESTADO;
    }

    f->circuitbreaker = HASHMAL_FIREWALL_CB_CERRADO;
    f->timestamp_ms   = hashmal_firewall_ahora_ms();

    CB_LOG_I("circuit breaker DESACTIVADO — vayyáshov Aharón el "
             "Moshéh (Nm 17:15); vatteʻatsár hammaggefá (Nm 17:13) "
             "— tráfico reanudado");

    return HASHMAL_FIREWALL_OK;
}

/* ==================================================================
 * hashmal_firewall_circuitbreaker_estado - lectura pura
 *
 * Sin logs, sin side effects. Escribe en *out el estado actual
 * del CB (CERRADO o ABIERTO).
 *
 *   out NULL  -> ERR_NULL
 *   sin init  -> ERR_NO_INIT
 * ================================================================== */
int hashmal_firewall_circuitbreaker_estado(hashmal_firewall_cb_t *out)
{
    if (out == NULL) return HASHMAL_FIREWALL_ERR_NULL;

    const hashmal_firewall_t *f = hashmal_firewall_get_incienso_interno();
    if (f == NULL) return HASHMAL_FIREWALL_ERR_NO_INIT;

    *out = f->circuitbreaker;
    return HASHMAL_FIREWALL_OK;
}

/* ==================================================================
 *
 *   "qaj et HAMMAJTA veten 'aleha esh... vesim QETORET veholej
 *    MEHERAH el ha'edah veJAPPER 'aleihem." - Numeros 17:11
 *
 *   "vayyarots el toj haqqahal... vayjapper 'al ha'am." - Nm 17:12
 *
 *   "vayya'amod BEIN HAMMETIM UVEIN HAJJAYYIM - vatte'atsar
 *    hammaggefa." - Numeros 17:13
 *
 *   "vayyashov Aharon el Mosheh el petaj ohel mo'ed - vehammaggefa
 *    ne'etsarah." - Numeros 17:15
 *
 *   El circuit breaker es Aaron corriendo con el incensario. No
 *   analiza - actua. No pregunta - corre. La emergencia exige un
 *   acto rapido y total: toda la frontera se cierra para que la
 *   plaga no cruce. Aaron no se queda afuera para siempre; cuando
 *   la plaga cesa, VUELVE (Nm 17:15). El circuit breaker tampoco
 *   es estado normal - es expiacion de emergencia, un parentesis
 *   de proteccion que se abre y se cierra bajo mandato explicito.
 *
 *   Y una advertencia final: 2 Cr 26:16-20 recuerda que el
 *   incensario no es de cualquiera. Uzias lo tomo sin oficio y
 *   quedo leproso en la frente. Este archivo no valida al llamante
 *   - la pieza 09 (hashmal_roles) es la que identifica quien tiene
 *   autoridad para activar la frontera. Aqui solo ejecutamos la
 *   accion cuando nos la piden, y la ejecutamos bien: rapido,
 *   total, y reversible cuando la causa cesa.
 *
 *   === BEIN HAMMETIM UVEIN HAJJAYYIM -
 *       incienso_circuitbreaker.c COMPLETO ===
 *
 *   -- Los 5 archivos hermanos -----------------------------------
 *
 *     incienso_firewall.c        QETORET HADEQ
 *     incienso_formato.c         MA'ASEH ROQEAJ
 *     incienso_circuitbreaker.c  BEIN HAMMETIM UVEIN HAJJAYYIM <- ESTE
 *     incienso_heartbeat.c       TAMID BABOQER UVEIN HA'ARBAYIM
 *     incienso_penalizacion.c    VENIKRAT ME'AMMAV
 *
 *   Archivo 3 de 5 del Incienso Sagrado. Siguiente:
 *     incienso_heartbeat.c - tamid manana y tarde (Ex 30:7-8)
 *
 *   PASO 10 de Exodo 40:27,34 - Pieza 14, Archivo 3 de 5.
 *
 * ================================================================== */
