/**
 * ===================================================================
 * PIEZA 06 - ALTAR DEL SACRIFICIO - altar_cuernos.c
 * Sub-pieza: LOS 4 CUERNOS - qarnotav 'al arba' pinnotav
 * Exodo 27:2 + 1 Reyes 1:50 + 1 Reyes 2:28-34 + Levitico 4:25,30
 * ===================================================================
 *
 * "ve'asita QARNOTAV 'al ARBA' PINNOTAV MIMMENNU tihyena qarnotav
 *  vetsippita oto NEJOSHET" - Ex 27:2
 * "Haras sus CUERNOS en sus cuatro ESQUINAS; DE EL seran sus
 *  cuernos, y lo cubriras de BRONCE."
 *
 * "vaAdoniyahu YARE mippenei Shelomo vayyaqom vayyelej
 *  vaYYAJAZEQ beqarnot hammizbeaj" - 1 Re 1:50
 * "Y Adonias TEMIO delante de Salomon, se levanto, fue y SE
 *  AGARRO de los cuernos del altar."
 *
 * "veYoav NAS el ohel YHWH vaYYAJAZEQ beqarnot hammizbeaj" - 1 Re 2:28
 * "Y Joab HUYO al tabernaculo de YHWH y se AGARRO de los cuernos."
 *
 * "velaqaj hakkohen middam hajattat beetsba'o venatan 'al
 *  QARNOT mizbaj ha'olah" - Lv 4:25
 * "El sacerdote tomara de la SANGRE de la expiacion con su dedo y
 *  pondra sobre los CUERNOS del altar del holocausto." (cf. Lv 4:30)
 *
 * --- VOCABULARIO HEBREO ---
 *
 *   qarnotav      (QRNTYW, H7161)  CUERNOS. Puntos de fuerza y
 *                                      acceso que SOBRESALEN del altar.
 *                                      En el animal: defensa (Dt 33:17).
 *                                      En el altar: endpoints publicos.
 *
 *   arba' pinnotav (ARB' PNTYW  H702+H6438)  4 ESQUINAS.
 *                                      No 3, no 5 - cuatro EXACTAMENTE.
 *                                      Simetricos, perifericos.
 *
 *   mimmennu      (MMNW)           DE EL. Los cuernos NO son
 *                                      piezas separadas adosadas al
 *                                      altar - SALEN del altar mismo.
 *                                      Son extension organica del
 *                                      singleton, como los querubines
 *                                      brotan miqsha de la Kaporet.
 *
 *   nejoshet      (NChShT, H5178)    BRONCE. Nivel de confianza
 *                                      exterior; accesibles al publico
 *                                      autorizado.
 *
 *   yare          (YRA, H3372)       TEMIO. Adonias llega en
 *                                      emergencia buscando asilo -
 *                                      modelo de la invocacion de
 *                                      refugio.
 *
 *   yajazeq       (YChZQ, H2388)     SE AGARRO. jazaq = asir con
 *                                      fuerza. No toque casual -
 *                                      aferrarse PARA LA VIDA. El
 *                                      cuerno es punto de refugio.
 *
 *   dam           (DM, H1818)        SANGRE. En Lv 4:25,30 la sangre
 *                                      se pone SOBRE los cuernos: es
 *                                      el acto que los ACTIVA. Sin
 *                                      sangre, existen pero no sirven.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   cuerno_activar     Lv 4:25 - sangre sobre el qeren lo activa
 *   cuerno_desactivar  Nm 4:13 - desmonte / preparacion para mover
 *   cuerno_activo      lectura pura - ?este cuerno tiene sangre?
 *   cuerno_refugio     1 Re 1:50 - yajazeq para asilo
 *
 * --- MIMMENNU - principio de diseno ---
 *
 * "mimmennu tihyena qarnotav" (Ex 27:2) - DE EL seran sus cuernos.
 * Los cuernos NO son parche externo: salen del altar. En firmware:
 * este archivo NO mantiene estado propio. Todo vive en el singleton
 * (g_altar.cuernos[i]). Cada funcion opera DENTRO del altar - no
 * sobre el. Como los cuernos del texto, las funciones son extension
 * del altar, no elementos adosados.
 *
 * --- REFUGIO: NO ES IMPUNIDAD ---
 *
 * 1 Re 1:50 - Adonias se aferra y Salomon le perdona la vida
 * (condicional: "si fuere hombre virtuoso", v.52).
 * 1 Re 2:28-34 - Joab se aferra, pero es EJECUTADO alli mismo
 * porque su maldad era conocida.
 *
 * El cuerno ofrece VENTANA DE GRACIA, no impunidad absoluta. En
 * Fase 1 registramos la solicitud (solicitudes++); en Fase 2 el
 * refugio dara un TIMEOUT de proteccion temporal - como Salomon
 * decide, el sistema resolvera durante esa ventana.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_auth.h                 API publica
 *   hashmal_auth_internal.h        altar_mut, get_altar_interno,
 *                                  cuerno_legitimo
 *   esp_log.h (ORO EGIPTO)         AUTH_LOG_*
 *
 * Sin mbedtls, sin esp_random. Este archivo no hace cripto - las
 * funciones son gestion de estado de los endpoints (activar /
 * desactivar / leer / contar).
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <inttypes.h>

#include "esp_log.h"

#include "hashmal_auth.h"
#include "hashmal_auth_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *AUTH_TAG = "hashmal.auth.cuernos";

#define AUTH_LOG_I(fmt, ...)  ESP_LOGI(AUTH_TAG, fmt, ##__VA_ARGS__)
#define AUTH_LOG_W(fmt, ...)  ESP_LOGW(AUTH_TAG, fmt, ##__VA_ARGS__)
#define AUTH_LOG_E(fmt, ...)  ESP_LOGE(AUTH_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * API PUBLICA - cuerno_activar (Lv 4:25 - sangre en el qeren)
 * ================================================================== */

/**
 * hashmal_auth_cuerno_activar - sangre sobre el cuerno
 *
 * Lv 4:25,30 - la sangre se pone SOBRE los cuernos. Es el acto
 * que los pone operativos. Sin sangre el cuerno existe pero no
 * recibe ofrendas.
 *
 * En firmware: marca `activo = true`. Idempotente: la sangre no
 * dana un cuerno que ya la tiene (un segundo rociado no re-consagra;
 * se contabiliza como confirmacion silenciosa).
 *
 * --- EFECTOS ---
 *
 *   cuerno.activo = true
 *   si transicion false->true -> altar->cuernos_activos += 1
 *
 * 2 testigos:
 *   Lv 4:25 - "venatan 'al qarnot mizbaj ha'olah"
 *   Lv 4:30 - confirmacion del patron de activacion por sangre
 */
int hashmal_auth_cuerno_activar(uint8_t id)
{
    hashmal_auth_t *a = hashmal_auth_altar_mut();
    if (a == NULL) {
        AUTH_LOG_E("cuerno_activar: altar no levantado");
        return HASHMAL_AUTH_ERR_INIT;
    }

    if (!hashmal_auth_cuerno_legitimo(id)) {
        AUTH_LOG_E("cuerno_activar: id %u fuera de [0, %d) — Éx 27:2 "
                   "arbaʻ pinnotáv",
                   (unsigned)id, HASHMAL_AUTH_CUERNOS);
        return HASHMAL_AUTH_ERR_CUERNO;
    }

    hashmal_qeren_auth_t *q = &a->cuernos[id];

    /* Idempotencia - segundo rociado sobre cuerno ya consagrado
     * no cambia nada; log silencioso. */
    if (q->activo) {
        AUTH_LOG_I("cuerno_activar: cuerno %u ya activo (idempotente)",
                   (unsigned)id);
        return HASHMAL_AUTH_OK;
    }

    /* Transicion false -> true. */
    q->activo = true;
    a->cuernos_activos += 1u;

    AUTH_LOG_I("cuerno %u activado — Lv 4:25 dám ʻal qarnót; "
               "cuernos_activos=%u",
               (unsigned)id, (unsigned)a->cuernos_activos);
    return HASHMAL_AUTH_OK;
}

/* ==================================================================
 * API PUBLICA - cuerno_desactivar (Nm 4:13 - desmonte)
 * ================================================================== */

/**
 * hashmal_auth_cuerno_desactivar - retirar la sangre operativa
 *
 * Nm 4:13-14 - al preparar el altar para migrar, se limpia ("ve-
 * dishhenu et hammizbeaj") y se cubre con beged argaman. Los
 * cuernos dejan de recibir ofrendas durante el traslado.
 *
 * En firmware: marca `activo = false`. Idempotente: volver a
 * desactivar lo que ya esta frio es no-op silencioso.
 *
 * --- EFECTOS ---
 *
 *   cuerno.activo = false
 *   si transicion true->false -> altar->cuernos_activos -= 1
 *
 * 2 testigos:
 *   Nm 4:13   - "vedishhenu et hammizbeaj" (limpiar el altar)
 *   Nm 4:14   - "ufresu 'alav beged argaman" (cubrir con purpura)
 */
int hashmal_auth_cuerno_desactivar(uint8_t id)
{
    hashmal_auth_t *a = hashmal_auth_altar_mut();
    if (a == NULL) {
        AUTH_LOG_E("cuerno_desactivar: altar no levantado");
        return HASHMAL_AUTH_ERR_INIT;
    }

    if (!hashmal_auth_cuerno_legitimo(id)) {
        AUTH_LOG_E("cuerno_desactivar: id %u fuera de [0, %d)",
                   (unsigned)id, HASHMAL_AUTH_CUERNOS);
        return HASHMAL_AUTH_ERR_CUERNO;
    }

    hashmal_qeren_auth_t *q = &a->cuernos[id];

    /* Idempotencia. */
    if (!q->activo) {
        AUTH_LOG_I("cuerno_desactivar: cuerno %u ya inactivo "
                   "(idempotente)", (unsigned)id);
        return HASHMAL_AUTH_OK;
    }

    /* Transicion true -> false. Defensa contra underflow del contador
     * (no deberia ocurrir si el estado es coherente, pero si ocurre
     * preferimos un log en vez de wraparound silencioso a UINT8_MAX). */
    q->activo = false;
    if (a->cuernos_activos > 0u) {
        a->cuernos_activos -= 1u;
    } else {
        AUTH_LOG_W("cuerno_desactivar: cuernos_activos ya era 0 — "
                   "estado inconsistente corregido");
    }

    AUTH_LOG_W("cuerno %u desactivado — Nm 4:13 desmonte; "
               "cuernos_activos=%u",
               (unsigned)id, (unsigned)a->cuernos_activos);
    return HASHMAL_AUTH_OK;
}

/* ==================================================================
 * API PUBLICA - cuerno_activo (lectura pura)
 * ================================================================== */

/**
 * hashmal_auth_cuerno_activo - ?este cuerno tiene sangre?
 *
 * Lectura SIN logs (evita ruido durante polling). Retorna false en
 * cualquier condicion anomala (altar no levantado o id invalido) -
 * el caller distingue el motivo si necesita consultando otras APIs
 * (get_altar para el estado global, cuerno_legitimo indirectamente
 * por documentacion).
 *
 * 2 testigos:
 *   Ex 27:2 - arba' pinnotav (4 cuernos existen)
 *   Lv 4:25 - sangre distingue activo de inactivo
 */
bool hashmal_auth_cuerno_activo(uint8_t id)
{
    const hashmal_auth_t *a = hashmal_auth_get_altar_interno();
    if (a == NULL) return false;
    if (!hashmal_auth_cuerno_legitimo(id)) return false;
    return a->cuernos[id].activo;
}

/* ==================================================================
 * API PUBLICA - cuerno_refugio (1 Re 1:50 - yajazeq)
 * ================================================================== */

/**
 * hashmal_auth_cuerno_refugio - "vaYYAJAZEQ beqarnot hammizbeaj"
 *
 * 1 Re 1:50 - Adonias temio y SE AGARRO de los cuernos del altar.
 * En firmware: un nodo en emergencia solicita asilo en un cuerno.
 * El altar REGISTRA la solicitud (contador), pero no garantiza
 * impunidad (Joab fue ejecutado alli mismo - 1 Re 2:34: el refugio
 * protege al inocente, no al culpable probado).
 *
 * --- PRECONDICIONES ---
 *
 *   altar levantado
 *   id legitimo [0, 4)
 *   cuerno ACTIVO (sin sangre no hay refugio - Lv 4:25 condicion previa)
 *
 * --- EFECTOS ---
 *
 *   cuerno.solicitudes += 1
 *   NO cambia activo (el refugio usa al cuerno, no lo muta)
 *
 * --- FASE 2 ---
 *
 * El refugio concedera una VENTANA DE GRACIA (timeout de
 * proteccion temporal) durante la cual el sistema decidira si
 * confirma asilo o ejecuta. Como Salomon: el rey decide. Fase 1
 * solo registra; la politica de resolucion viene cuando la red
 * tenga juez (piezas futuras).
 *
 * 2 testigos:
 *   1 Re 1:50 - vaYAJAZEQ beqarnot (agarrarse por asilo)
 *   1 Re 2:28 - segundo testigo del mismo mecanismo (Joab)
 */
int hashmal_auth_cuerno_refugio(uint8_t id)
{
    hashmal_auth_t *a = hashmal_auth_altar_mut();
    if (a == NULL) {
        AUTH_LOG_E("cuerno_refugio: altar no levantado");
        return HASHMAL_AUTH_ERR_INIT;
    }

    if (!hashmal_auth_cuerno_legitimo(id)) {
        AUTH_LOG_E("cuerno_refugio: id %u fuera de [0, %d)",
                   (unsigned)id, HASHMAL_AUTH_CUERNOS);
        return HASHMAL_AUTH_ERR_CUERNO;
    }

    hashmal_qeren_auth_t *q = &a->cuernos[id];

    /* Un cuerno sin sangre (inactivo) NO ofrece asilo. Lv 4:25
     * es precondicion - sin la sangre, el cuerno es piedra inerte. */
    if (!q->activo) {
        AUTH_LOG_W("cuerno_refugio: cuerno %u INACTIVO — sin dám no "
                   "hay asilo (Lv 4:25)", (unsigned)id);
        return HASHMAL_AUTH_ERR_CUERNO;
    }

    /* Registrar la solicitud. No mutamos `activo` - el refugio USA
     * al cuerno, no lo transforma. */
    q->solicitudes += 1ull;

    AUTH_LOG_I("refugio en cuerno %u — 1 Re 1:50 vayyajazéq; "
               "solicitudes=%" PRIu64,
               (unsigned)id, q->solicitudes);
    return HASHMAL_AUTH_OK;
}

/* ==================================================================
 *
 *   "ve'asita QARNOTAV 'al ARBA' PINNOTAV MIMMENNU tihyena
 *    qarnotav vetsippita oto NEJOSHET." - Exodo 27:2
 *
 *   "vaYYAJAZEQ beqarnot hammizbeaj." - 1 Reyes 1:50
 *
 *   mimmennu - DE EL. Los cuernos no se inventan.
 *   Salen del altar. Son parte del altar. Son el altar.
 *
 *   3 de 6 archivos del Altar del Sacrificio -
 *   altar_cuernos.c listo. Quedan:
 *     altar_fuego.c        Lv 6:5-6 - esh tamid, encender y ciclar
 *     altar_utensilios.c   v.3     - los 5 kelim
 *     altar_ledger.c       Ap 6:9  - almas bajo el altar
 *
 * ================================================================== */
