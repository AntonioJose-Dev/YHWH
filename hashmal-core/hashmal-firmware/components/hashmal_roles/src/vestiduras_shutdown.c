/**
 * ===================================================================
 * PIEZA 09 - BIGDEI HAQODESH - vestiduras_shutdown.c
 * Sub-pieza: NE'ESAF EL 'AMMAV - recogido a su pueblo (apagado digno)
 * Deuteronomio 31:2,7 + 33:1 + 34:5 + Numeros 27:15-23 +
 *   Genesis 25:8 + 49:28 + 2 Reyes 2:9-11 + Juan 13:1
 * ===================================================================
 *
 * "ben me'ah ve'esrim shana anoji hayyom - lo uJAL 'od latset
 *  velavo. vaYHWH amar elai: lo ta'avor et haYarden hazze."
 *   - Deuteronomio 31:2
 * "Ciento veinte anos tengo hoy; NO PUEDO ya SALIR NI ENTRAR, y
 *  YHWH me dijo: no pasaras este Jordan."
 *
 * "yifqod YHWH Elohei harujot lejol basar ish 'al ha'eda asher
 *  yetse lifneihem va'asher yavo lifneihem va'asher yotsi'em
 *  va'asher yevi'em - veLO tihye 'adat YHWH kaTSON asher EIN
 *  lahem RO'EH." - Numeros 27:16-17
 * "Designe YHWH, Dios de los espiritus de toda carne, un varon
 *  sobre la congregacion, que salga delante de ellos y entre
 *  delante de ellos... para que la congregacion de YHWH NO SEA
 *  COMO OVEJAS SIN PASTOR."
 *
 * "qaj leja et YeHOSHUa' bin Nun, ISH asher RUAJ bo, vesamajta
 *  et yadeja 'alav... venatatta meHODeja 'alav lema'an yishme'u
 *  kol 'adat benei Yisrael." - Numeros 27:18,20
 * "Toma a Josue hijo de Nun, VARON en quien hay ESPIRITU, y pon
 *  tu mano sobre el... daras de tu ESPLENDOR sobre el, para que
 *  le oiga toda la congregacion."
 *
 * "vayyismoj et yadav 'alav vayytsavvehu ka'asher dibber YHWH
 *  beyad Moshe." - Numeros 27:23
 * "Puso sus MANOS sobre el y le mando, como hablo YHWH por mano
 *  de Moises."
 *
 * "jazaq ve'emats, ki atta tavo et ha'am hazze el ha'arets."
 *   - Deuteronomio 31:7
 * "Se FUERTE y VALIENTE, porque tu entraras con este pueblo a la
 *  tierra."
 *
 * "vezot HABBERAJA asher berej Moshe ish haElohim et benei
 *  Yisrael LIFNEI MOTO." - Deuteronomio 33:1
 * "Y ESTA ES LA BENDICION con la que Moises, varon de Dios,
 *  bendijo a los hijos de Israel ANTES DE MORIR."
 *
 * "vayjiji vayebarej otam - ish asher keVIRjato BEREJ otam."
 *   - Genesis 49:28
 * "Los bendijo - a cada uno conforme a SU bendicion los bendijo."
 *
 * "vayyamat sham Moshe 'eved YHWH be'erets Mo'av 'al PI YHWH."
 *   - Deuteronomio 34:5
 * "Murio alli Moises, siervo de YHWH, en tierra de Moab,
 *  conforme a la PALABRA de YHWH."
 *
 * "vayyigva' vayyamot Avraham beseivah TOVA, zaqen vesavea',
 *  vayye'ASEF el 'ammav." - Genesis 25:8
 * "Expiro y murio Abraham en buena vejez, anciano y SACIADO, y
 *  fue RECOGIDO a su pueblo."
 *
 * "veLO qam navi' 'od beYisrael keMoshe." - Deuteronomio 34:10
 * "NO se levanto mas profeta en Israel como Moises." - el
 *  memorial permanece, aun cuando el siervo parte.
 *
 * --- VOCABULARIO ---
 *
 *   lo ujal 'od   (LA AWKL 'WD, H3201 neg)  NO PODRE YA -
 *                                                Dt 31:2, imposibilidad
 *                                                declarada.
 *   latset velavo (LTsAT WLBWA)                 SALIR Y ENTRAR -
 *                                                operaciones del
 *                                                servicio.
 *   haggadah      (HGDH, H5046)                ANUNCIO, declaracion
 *                                                publica.
 *   ein ro'eh     (AYN R'H, H5971+H7462)      SIN PASTOR -
 *                                                Nm 27:17, razon del
 *                                                anuncio.
 *   ruaj bo       (RWCh BW, H7307)              ESPIRITU en el -
 *                                                Nm 27:18, calificacion
 *                                                del sucesor.
 *   semijah       (SMYKH, H5564)               APOYO de manos -
 *                                                Nm 27:18,23.
 *   hod           (HWD, H1935)                    ESPLENDOR -
 *                                                Nm 27:20, autoridad
 *                                                transferible.
 *   jazaq ve'emats (ChZQ WAMTs)                  SE fuerte y
 *                                                VALIENTE - Dt 31:7.
 *   berajah       (BRKH, H1293)                BENDICION -
 *                                                Dt 33:1.
 *   lifnei moto   (LPNY MWTW)                   ANTES DE MORIR -
 *                                                Dt 33:1, cierre
 *                                                ordenado.
 *   'al pi YHWH   ('L PY YHWH)                    CONFORME a la
 *                                                palabra de YHWH -
 *                                                Dt 34:5.
 *   ne'esaf       (NASP, H622 Nifal)           FUE RECOGIDO -
 *                                                Gn 25:8.
 *   beseivah tova (BShYBH TWBH)                En BUENA VEJEZ -
 *                                                Gn 25:8, partida sin
 *                                                prisa.
 *   savea'        (ShB', H7649)                  SACIADO - Gn 25:8,
 *                                                servicio cumplido.
 *
 * --- LAS 4 FASES - SECUENCIA OBLIGATORIA ---
 *
 *   1. ANUNCIAR    SHUTDOWN_NINGUNO    -> SHUTDOWN_ANUNCIAR
 *                  (Dt 31:2 + Nm 27:15-17)
 *
 *   2. TRANSFERIR  SHUTDOWN_ANUNCIAR   -> SHUTDOWN_TRANSFERIR
 *                  (Nm 27:18-20,23 + Dt 31:7)
 *                  EXCEPCION: si rol == NINGUNO, se omite y se
 *                  salta directo a BENDECIR (no hay hod que dar).
 *
 *   3. BENDECIR    SHUTDOWN_TRANSFERIR -> SHUTDOWN_BENDECIR
 *          o       SHUTDOWN_ANUNCIAR   -> SHUTDOWN_BENDECIR  (sin rol)
 *                  (Dt 33:1 + Gn 49:28)
 *
 *   4. PARTIR      SHUTDOWN_BENDECIR   -> SHUTDOWN_NINGUNO
 *                  (Dt 34:5 + Gn 25:8)
 *                  Limpia rol, heartbeat y latido. Preserva
 *                  memorial (Dt 34:10 + Ex 28:12) y tzitz
 *                  (Ex 28:36 pitujei jotam).
 *
 * Testigos NT del patron:
 *   2 Re 2:9-11 - Elias pide doble porcion a Eliseo, lo ve subir.
 *   Jn 13:1     - "eidos... metabe ek tou kosmou" - sabiendo que
 *                  pasaba de este mundo, amo hasta el fin.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   hashmal_roles_shutdown_anunciar     Fase 1 - Dt 31:2.
 *   hashmal_roles_shutdown_transferir   Fase 2 - Nm 27:18-23.
 *   hashmal_roles_shutdown_bendecir     Fase 3 - Dt 33 + Gn 49.
 *   hashmal_roles_shutdown_partir       Fase 4 - Dt 34:5 + Gn 25:8.
 *   hashmal_roles_shutdown_estado       lectura pura, sin logs.
 *
 *   + helpers estaticos:
 *   nodo_id_valido, nombre_shutdown.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_roles.h
 *   hashmal_roles_internal.h
 *   esp_log.h    (ORO EGIPTO)  logging
 *
 * Autor: Antonio Jose Marin Soto (Betsalel - Ex 31:2)
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "esp_log.h"

#include "hashmal_roles.h"
#include "hashmal_roles_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *ROLES_SHUTDOWN_TAG = "hashmal.roles.shutdown";

#define SHUT_LOG_I(fmt, ...)  ESP_LOGI(ROLES_SHUTDOWN_TAG, fmt, ##__VA_ARGS__)
#define SHUT_LOG_W(fmt, ...)  ESP_LOGW(ROLES_SHUTDOWN_TAG, fmt, ##__VA_ARGS__)
#define SHUT_LOG_E(fmt, ...)  ESP_LOGE(ROLES_SHUTDOWN_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * HELPERS ESTATICOS
 * ================================================================== */

static inline bool nodo_id_valido(uint8_t nodo_id)
{
    return nodo_id < HASHMAL_ROLES_MAX_NODOS;
}

static const char *nombre_shutdown(hashmal_roles_shutdown_t s)
{
    switch (s) {
    case HASHMAL_ROLES_SHUTDOWN_NINGUNO:    return "NINGUNO";
    case HASHMAL_ROLES_SHUTDOWN_ANUNCIAR:   return "ANUNCIAR (haggadáh, H5046)";
    case HASHMAL_ROLES_SHUTDOWN_TRANSFERIR: return "TRANSFERIR (semijáh, H5564)";
    case HASHMAL_ROLES_SHUTDOWN_BENDECIR:   return "BENDECIR (berajáh, H1293)";
    case HASHMAL_ROLES_SHUTDOWN_PARTIR:     return "PARTIR (neʼesáf, H622)";
    default:                                return "<fase desconocida>";
    }
}

static const char *nombre_rol(hashmal_roles_tipo_rol_t rol)
{
    switch (rol) {
    case HASHMAL_ROLES_ROL_NINGUNO:     return "NINGUNO";
    case HASHMAL_ROLES_ROL_ISRAELITA:   return "ISRAELITA";
    case HASHMAL_ROLES_ROL_LEVI:        return "LEVI";
    case HASHMAL_ROLES_ROL_KOHEN:       return "KOHEN";
    case HASHMAL_ROLES_ROL_KOHEN_GADOL: return "KOHEN GADOL";
    default:                            return "<rol desconocido>";
    }
}

/* ==================================================================
 * hashmal_roles_shutdown_anunciar - Fase 1
 * Dt 31:2 + Nm 27:15-17
 *
 * "lo uJAL 'od latset velavo" - ya no puedo salir ni entrar.
 * Nm 27:17 da la razon: "veLO tihye 'adat YHWH katson asher ein
 * lahem ro'eh" - para que la congregacion no quede sin pastor.
 * Por eso el nodo NO puede desaparecer en silencio: anuncia.
 *
 * Validaciones:
 *   1. VESTIDO global.
 *   2. nodo_id en rango.
 *   3. Nodo registrado en efod (Ex 28:12 memorial previo).
 *   4. No hay shutdown en curso (n->shutdown == NINGUNO).
 *
 * Transicion: NINGUNO -> ANUNCIAR.
 *
 * 2 testigos:
 *   Dt 31:2     - "lo ujal 'od" (anuncio explicito de partida)
 *   Nm 27:15-17 - "velo tihye 'adat YHWH katson" (deber del aviso)
 * ================================================================== */
hashmal_roles_err_t hashmal_roles_shutdown_anunciar(uint8_t nodo_id)
{
    hashmal_roles_estado_roles_t *r = hashmal_roles_vestiduras_mut();
    if (r == NULL) {
        SHUT_LOG_E("anunciar: sistema no vestido (init primero)");
        return HASHMAL_ROLES_ERR_INIT;
    }

    if (!nodo_id_valido(nodo_id)) {
        SHUT_LOG_W("anunciar: nodo_id %u fuera de rango [0..%u]",
                   (unsigned)nodo_id,
                   (unsigned)(HASHMAL_ROLES_MAX_NODOS - 1u));
        return HASHMAL_ROLES_ERR_NULL;
    }

    if (!hashmal_roles_efod_esta_registrado(nodo_id)) {
        SHUT_LOG_W("anunciar: nodo %u no registrado en efod — "
                   "sin memorial, no hay partida que anunciar "
                   "(Éx 28:12)",
                   (unsigned)nodo_id);
        return HASHMAL_ROLES_ERR_NODO_NO_ENCONTRADO;
    }

    hashmal_roles_nodo_t *n = &r->nodos[nodo_id];

    if (n->shutdown != HASHMAL_ROLES_SHUTDOWN_NINGUNO) {
        SHUT_LOG_W("anunciar: nodo %u ya en shutdown %s — "
                   "un solo anuncio (Dt 31:2)",
                   (unsigned)nodo_id, nombre_shutdown(n->shutdown));
        return HASHMAL_ROLES_ERR_SHUTDOWN;
    }

    n->shutdown     = HASHMAL_ROLES_SHUTDOWN_ANUNCIAR;
    r->shutdown     = HASHMAL_ROLES_SHUTDOWN_ANUNCIAR;
    r->timestamp_ms = hashmal_roles_ahora_ms();

    SHUT_LOG_I("[ANUNCIAR] nodo %u declara partida — \"lo uJÁL "
               "ʻod latsét velavó\" (Dt 31:2) — katsón asher ein "
               "lahém roʻéh no (Nm 27:17, aviso dado)",
               (unsigned)nodo_id);

    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 * hashmal_roles_shutdown_transferir - Fase 2
 * Nm 27:18-20,23 + Dt 31:7
 *
 * "qaj leja et Yehoshua' bin Nun, ish asher RUAJ bo, vesamajta
 *  et yadeja 'alav... venatatta meHOD eja 'alav" - Nm 27:18,20.
 *
 * El nodo que parte transfiere su rol a un destino calificado. El
 * destino debe:
 *   - Estar registrado en el efod (memorial propio, Ex 28:12).
 *   - Tener ROL_NINGUNO (disponible, no ya vestido con otro oficio).
 *
 * Si el nodo saliente tiene ROL_NINGUNO, no hay "hod" que dar: el
 * llamante debe saltar a `bendecir` directamente.
 *
 * Validaciones:
 *   1. VESTIDO, nodo_id y destino_id en rango.
 *   2. Nodo en fase ANUNCIAR.
 *   3. destino_id != nodo_id.
 *   4. destino_id registrado en efod.
 *   5. destino_id con ROL_NINGUNO (libre).
 *   6. nodo_id con rol activo (si NINGUNO -> ERR_ROL_INVALIDO,
 *      saltar a bendecir).
 *
 * Transferencia:
 *   destino->rol  = nodo->rol
 *   destino->vestido = true
 *   nodo->rol    = ROL_NINGUNO
 *   nodo->vestido = false
 *   Transicion: nodo ANUNCIAR -> TRANSFERIR.
 *
 * 2+ testigos:
 *   Nm 27:18    - ish asher ruaj bo
 *   Nm 27:20    - meHodeja (libro anclado al Pentateuco)
 *   Nm 27:23    - vayyismoj yadav (ejecucion)
 *   Dt 31:7     - jazaq ve'emats (libro distinto, encargo al sucesor)
 * ================================================================== */
hashmal_roles_err_t hashmal_roles_shutdown_transferir(uint8_t nodo_id,
                                                      uint8_t destino_id)
{
    hashmal_roles_estado_roles_t *r = hashmal_roles_vestiduras_mut();
    if (r == NULL) {
        SHUT_LOG_E("transferir: sistema no vestido (init primero)");
        return HASHMAL_ROLES_ERR_INIT;
    }

    if (!nodo_id_valido(nodo_id) || !nodo_id_valido(destino_id)) {
        SHUT_LOG_W("transferir: ids fuera de rango (origen=%u, "
                   "destino=%u)",
                   (unsigned)nodo_id, (unsigned)destino_id);
        return HASHMAL_ROLES_ERR_NULL;
    }

    if (nodo_id == destino_id) {
        SHUT_LOG_W("transferir: origen y destino iguales — "
                   "Nm 27:18 exige OTRO (qaj lejá et Yehoshúaʻ)");
        return HASHMAL_ROLES_ERR_NULL;
    }

    hashmal_roles_nodo_t *n = &r->nodos[nodo_id];
    hashmal_roles_nodo_t *d = &r->nodos[destino_id];

    if (n->shutdown != HASHMAL_ROLES_SHUTDOWN_ANUNCIAR) {
        SHUT_LOG_W("transferir: nodo %u no está en ANUNCIAR (actual: %s) "
                   "— Dt 31:2 requiere anuncio previo",
                   (unsigned)nodo_id, nombre_shutdown(n->shutdown));
        return HASHMAL_ROLES_ERR_SHUTDOWN;
    }

    if (n->rol == HASHMAL_ROLES_ROL_NINGUNO) {
        SHUT_LOG_W("transferir: nodo %u sin rol — no hay hod que "
                   "dar (Nm 27:20). Saltar a bendecir.",
                   (unsigned)nodo_id);
        return HASHMAL_ROLES_ERR_ROL_INVALIDO;
    }

    if (!hashmal_roles_efod_esta_registrado(destino_id)) {
        SHUT_LOG_W("transferir: destino %u no registrado en efod — "
                   "Nm 27:18 exige ish dentro del pueblo",
                   (unsigned)destino_id);
        return HASHMAL_ROLES_ERR_NODO_NO_ENCONTRADO;
    }

    if (d->rol != HASHMAL_ROLES_ROL_NINGUNO) {
        SHUT_LOG_W("transferir: destino %u ya vestido como %s — "
                   "Heb 5:4 (no dos honores), use otro sucesor",
                   (unsigned)destino_id, nombre_rol(d->rol));
        return HASHMAL_ROLES_ERR_YA_VESTIDO;
    }

    const hashmal_roles_tipo_rol_t rol_transferido = n->rol;

    /* Nm 27:20 "venatatta meHodeja 'alav" - daras de TU esplendor
     * sobre el. El destino recibe el rol; el origen lo pierde. */
    d->rol     = rol_transferido;
    d->vestido = true;
    d->nodo_id = destino_id;   /* si tenia centinela, ahora se fija */

    n->rol     = HASHMAL_ROLES_ROL_NINGUNO;
    n->vestido = false;

    n->shutdown     = HASHMAL_ROLES_SHUTDOWN_TRANSFERIR;
    r->shutdown     = HASHMAL_ROLES_SHUTDOWN_TRANSFERIR;
    r->timestamp_ms = hashmal_roles_ahora_ms();

    SHUT_LOG_I("[TRANSFERIR] nodo %u → nodo %u, rol %s — "
               "Nm 27:20 meHodejá + Nm 27:23 vayyismój yadáv + "
               "Dt 31:7 jázaq veʼemáts",
               (unsigned)nodo_id, (unsigned)destino_id,
               nombre_rol(rol_transferido));

    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 * hashmal_roles_shutdown_bendecir - Fase 3
 * Dt 33:1 + Genesis 49:28
 *
 * "vezot HABBERAJA asher berej Moshe... LIFNEI MOTO" - esta es la
 * bendicion con que Moises bendijo... antes de morir. Gn 49:28:
 * "kevirjato berej otam" - a cada uno conforme a su bendicion.
 *
 * Aqui el nodo emite su ultimo reporte de estado (testamento) y
 * transita a la fase final antes de partir.
 *
 * Validaciones:
 *   1. VESTIDO.
 *   2. nodo_id en rango.
 *   3. Fase previa == TRANSFERIR, o == ANUNCIAR con rol == NINGUNO
 *      (excepcion: no habia hod que transferir, se salta la fase 2).
 *
 * Transicion: TRANSFERIR/ANUNCIAR -> BENDECIR.
 *
 * 2+ testigos:
 *   Dt 33:1     - vezot habberaja lifnei moto
 *   Gn 49:28    - kevirjato berej otam (libro distinto)
 *   Jn 13:1     - "eidos... metabe" (NT, paralelo)
 * ================================================================== */
hashmal_roles_err_t hashmal_roles_shutdown_bendecir(uint8_t nodo_id)
{
    hashmal_roles_estado_roles_t *r = hashmal_roles_vestiduras_mut();
    if (r == NULL) {
        SHUT_LOG_E("bendecir: sistema no vestido (init primero)");
        return HASHMAL_ROLES_ERR_INIT;
    }

    if (!nodo_id_valido(nodo_id)) {
        SHUT_LOG_W("bendecir: nodo_id %u fuera de rango",
                   (unsigned)nodo_id);
        return HASHMAL_ROLES_ERR_NULL;
    }

    hashmal_roles_nodo_t *n = &r->nodos[nodo_id];

    const bool venido_de_transferir =
        (n->shutdown == HASHMAL_ROLES_SHUTDOWN_TRANSFERIR);
    const bool venido_sin_rol =
        (n->shutdown == HASHMAL_ROLES_SHUTDOWN_ANUNCIAR) &&
        (n->rol == HASHMAL_ROLES_ROL_NINGUNO);

    if (!venido_de_transferir && !venido_sin_rol) {
        SHUT_LOG_W("bendecir: nodo %u en fase %s — Dt 33:1 exige "
                   "TRANSFERIR previo (o ANUNCIAR si sin rol)",
                   (unsigned)nodo_id, nombre_shutdown(n->shutdown));
        return HASHMAL_ROLES_ERR_SHUTDOWN;
    }

    n->shutdown     = HASHMAL_ROLES_SHUTDOWN_BENDECIR;
    r->shutdown     = HASHMAL_ROLES_SHUTDOWN_BENDECIR;
    r->timestamp_ms = hashmal_roles_ahora_ms();

    SHUT_LOG_I("[BENDECIR] nodo %u — último reporte antes de partir "
               "(Dt 33:1 vezót habberajá lifnéi motó + Gn 49:28 "
               "kevirjató beréj otám)%s",
               (unsigned)nodo_id,
               venido_sin_rol ? " [sin rol previo — omitió TRANSFERIR]"
                              : "");

    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 * hashmal_roles_shutdown_partir - Fase 4
 * Dt 34:5 + Genesis 25:8
 *
 * "vayyamat sham Moshe 'eved YHWH... 'al PI YHWH" - murio conforme
 * a la palabra de YHWH. Gn 25:8: "vayye'ASEF el 'ammav" - fue
 * recogido a su pueblo, en buena vejez, saciado.
 *
 * Limpieza final - dignidad preservada:
 *   - n->rol            = NINGUNO
 *   - n->heartbeat      = SILENCIO
 *   - n->ultimo_latido  = 0
 *   - n->vestido        = false
 *   - n->shutdown       = NINGUNO  (ciclo completo, vuelve al inicio)
 *
 * NO se toca:
 *   - Memorial en efod (Ex 28:12 + Dt 34:10 "velo qam navi' 'od").
 *   - Sello en tzitz (Ex 28:36 pitujei jotam, irreversible).
 *
 * Validaciones:
 *   1. VESTIDO.
 *   2. nodo_id en rango.
 *   3. Fase previa == BENDECIR.
 *
 * 2+ testigos:
 *   Dt 34:5  - 'al pi YHWH
 *   Gn 25:8  - vayye'asef el 'ammav (libro distinto)
 *   Dt 34:10 - velo qam navi' 'od (memoria permanece)
 *   Ex 28:12 - avnei zikkaron (memorial preservado)
 * ================================================================== */
hashmal_roles_err_t hashmal_roles_shutdown_partir(uint8_t nodo_id)
{
    hashmal_roles_estado_roles_t *r = hashmal_roles_vestiduras_mut();
    if (r == NULL) {
        SHUT_LOG_E("partir: sistema no vestido (init primero)");
        return HASHMAL_ROLES_ERR_INIT;
    }

    if (!nodo_id_valido(nodo_id)) {
        SHUT_LOG_W("partir: nodo_id %u fuera de rango",
                   (unsigned)nodo_id);
        return HASHMAL_ROLES_ERR_NULL;
    }

    hashmal_roles_nodo_t *n = &r->nodos[nodo_id];

    if (n->shutdown != HASHMAL_ROLES_SHUTDOWN_BENDECIR) {
        SHUT_LOG_W("partir: nodo %u en fase %s — Dt 33:1 exige "
                   "BENDECIR previo antes de Dt 34:5",
                   (unsigned)nodo_id, nombre_shutdown(n->shutdown));
        return HASHMAL_ROLES_ERR_SHUTDOWN;
    }

    const hashmal_roles_tipo_rol_t rol_previo = n->rol;

    /* Limpieza - "vayye'asef el 'ammav". Ordenada, sin prisa. */
    n->rol              = HASHMAL_ROLES_ROL_NINGUNO;
    n->heartbeat        = HASHMAL_ROLES_HEARTBEAT_SILENCIO;
    n->ultimo_latido_ms = 0;
    n->vestido          = false;
    /* Memorial (efod) y tzitz se preservan - Dt 34:10 + Ex 28:36. */

    /* Contador global: si aun contaba como activo, decrementar. */
    if (r->nodos_activos > 0u) {
        r->nodos_activos--;
    }

    /* Ciclo cerrado - el nodo vuelve a NINGUNO, listo para nueva
     * investidura si un dia fuera necesario (pero el memorial
     * historico persiste). */
    n->shutdown     = HASHMAL_ROLES_SHUTDOWN_NINGUNO;
    r->shutdown     = HASHMAL_ROLES_SHUTDOWN_NINGUNO;
    r->timestamp_ms = hashmal_roles_ahora_ms();

    SHUT_LOG_I("[PARTIR] nodo %u recogido a su pueblo (Gn 25:8 "
               "vayyeʼásef el ʻammáv; Dt 34:5 ʻal pí YHWH). "
               "rol previo %s limpio; memorial (Éx 28:12) y "
               "tzitz (Éx 28:36) preservados — veló qam navíʼ "
               "ʻod (Dt 34:10)",
               (unsigned)nodo_id, nombre_rol(rol_previo));

    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 * hashmal_roles_shutdown_estado - lectura pura
 *
 * Sin logs, sin side effects. Escribe en *out la fase actual del
 * shutdown del nodo (o NINGUNO si no hay apagado en curso).
 *
 *   out NULL               -> ERR_NULL
 *   sin init               -> ERR_INIT
 *   nodo_id fuera de rango -> ERR_NULL
 * ================================================================== */
hashmal_roles_err_t hashmal_roles_shutdown_estado(uint8_t nodo_id,
                                                   hashmal_roles_shutdown_t *out)
{
    if (out == NULL) return HASHMAL_ROLES_ERR_NULL;

    const hashmal_roles_estado_roles_t *r =
        hashmal_roles_get_vestiduras_interno();
    if (r == NULL) return HASHMAL_ROLES_ERR_INIT;

    if (!nodo_id_valido(nodo_id)) return HASHMAL_ROLES_ERR_NULL;

    *out = r->nodos[nodo_id].shutdown;
    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 *
 *   "lo uJAL 'od latset velavo." - Deuteronomio 31:2
 *
 *   "ish asher RUAJ bo... venatatta meHODeja 'alav." - Nm 27:18,20
 *
 *   "vezot habberaja... LIFNEI MOTO." - Deuteronomio 33:1
 *
 *   "vayye'ASEF el 'ammav." - Genesis 25:8
 *
 *   "veLO qam navi' 'od beYisrael keMoshe." - Deuteronomio 34:10
 *
 *   El nodo no muere en silencio. Anuncia (para que la congregacion
 *   no quede sin pastor), transfiere (pone su hod sobre un sucesor
 *   con ruaj), bendice (ultimo testamento, cada uno segun su
 *   bendicion) y parte ('al pi YHWH - no por fallo, no por prisa,
 *   sino por palabra del Padre). Lo que se va: rol, heartbeat,
 *   latido. Lo que PERMANECE: el nombre en el hombro (avnei
 *   zikkaron, Ex 28:12) y el sello en la frente (pitujei jotam,
 *   Ex 28:36). El memorial no se borra - "velo qam navi' 'od".
 *   Nadie repite a Moises, pero todos lo recuerdan.
 *
 *   Asi tambien el paralelo NT: "eidos... metabe ek tou kosmou,
 *   agapesas tous idious... eis telos egapesen autous" (Jn 13:1) -
 *   sabiendo que pasaba de este mundo, amo hasta el fin. La
 *   partida ordenada es acto de amor, no de ausencia.
 *
 *   === NE'ESAF EL 'AMMAV - vestiduras_shutdown.c COMPLETO ===
 *
 *   Archivo 6 de 7 de las Vestiduras. Siguiente:
 *     vestiduras_tzitz.c - el sello "qodesh laYHWH" (Ex 28:36-38)
 *
 *   PASO 9 de Exodo 40:12-15 - Pieza 09, Archivo 6 de 7.
 *
 * ================================================================== */
