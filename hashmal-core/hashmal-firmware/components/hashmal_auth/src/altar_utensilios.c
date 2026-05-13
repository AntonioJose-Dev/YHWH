/**
 * ===================================================================
 * PIEZA 06 - ALTAR DEL SACRIFICIO - altar_utensilios.c
 * Sub-pieza: LOS 5 KELIM - herramientas de servicio sacerdotal
 * Exodo 27:3 + Exodo 38:3 + Numeros 4:14
 * ===================================================================
 *
 * "ve'asita SIROTAV ledashno veYA'AV uMIZRAQOTAV uMIZLEGOTAV
 *  uMAJTOTAV lejol kelav ta'ase NEJOSHET" - Ex 27:3
 * "Y haras sus CALDEROS para limpiar ceniza, sus PALAS, sus
 *  TAZONES, sus GARFIOS y sus BRASEROS - haras de BRONCE todos
 *  sus utensilios."
 *
 * "vayya'as et kol kelei hammizbeaj... KOL kelav 'asa NEJOSHET"
 *   - Ex 38:3
 * "E hizo todos los utensilios del altar... TODOS sus utensilios
 *  los hizo de BRONCE."
 *
 * "venatanu 'alav et kol kelav asher YESHARETU 'alav bahem... KOL
 *  kele hammizbeaj ufarsu 'alav KESUI 'or TAJASH vesamu baddav"
 *   - Nm 4:14
 * "Pondran sobre el todos los utensilios con que SERVIRAN... todos
 *  los utensilios del altar, cubriran con piel de tajash y pondran
 *  sus varas."
 *
 * --- VOCABULARIO HEBREO ---
 *
 *   kelim       (KLYM, H3627)   UTENSILIOS. Herramientas de servicio.
 *                                  "kol kelav" - TODOS. El altar sin
 *                                  kelim no se puede servir.
 *
 *   sirot       (SYRWT, H5518) CALDEROS. Para ledashno = limpiar
 *                                  ceniza (deshen). Retiran lo ya
 *                                  consumido para que el fuego respire.
 *
 *   deshen      (DShN, H1880)  CENIZA. Residuo post-sacrificio.
 *                                  Limpia periodica.
 *
 *   ya'im       (Y'YM, H3257)  PALAS. Reposicionan brasas sin
 *                                  apagar el fuego.
 *
 *   mizraqot    (MZRQWT, H4219) TAZONES / aspersores. De zaraq
 *                                  (H2236) = esparcir. Recogen y
 *                                  rocian.
 *
 *   mizlegot    (MZLGWT, H4207) GARFIOS / trinches. Voltean la
 *                                  ofrenda para que el fuego toque
 *                                  cada parte. INSPECCION.
 *
 *   majtot      (MChTWT, H4289) BRASEROS. Transportan fuego/brasas.
 *                                  Mismo utensilio que en la Menora
 *                                  (Ex 25:38) y en el rescate de
 *                                  Nm 16:46-48.
 *
 *   nejoshet    (NChShT, H5178) BRONCE. TODOS los kelim son de
 *                                  bronce: mismo material que el
 *                                  altar. Coherencia total.
 *
 *   yesharetu   (YShRTW, H8334) SERVIRAN / ministraran. Los kelim
 *                                  son herramientas de SERVICIO
 *                                  (sherath), no decoracion. El
 *                                  sacerdote los usa cada dia.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   utensilio_purgar               sirot - limpiar ceniza
 *   utensilio_resetear             ya'im - reposicionar brasas
 *   utensilio_recoger              mizraqot - recoger stats
 *   utensilio_inspeccionar         mizlegot - voltear y examinar
 *   utensilio_preparar_transporte  majtot - portar fuego (Nm 4:14)
 *   utensilio_estado               query per-keli (del header)
 *   utensilios_estado              global (?todos disponibles?)
 *
 * --- LOS KELIM SON PARTE DEL ALTAR ---
 *
 * Ex 38:3 lo dice sin ambiguedad: "vayya'as et KOL kelei
 * hammizbeaj". Betsalel NO construyo el altar primero y los kelim
 * despues como accesorios - los hizo JUNTOS, como una obra
 * completa. Nm 4:14 confirma: "kol kele hammizbeaj" viajan CON el
 * altar, sobre el, bajo la misma cubierta de tajash.
 *
 * En firmware: estos kelim NO tienen estado propio. Todo vive en
 * el singleton del altar. Los kelim son LAS FUNCIONES - las
 * operaciones del servicio sacerdotal diario.
 *
 * --- ORDEN DE LAS FUNCIONES ---
 *
 * Las 5 primeras siguen exactamente el orden de Ex 27:3:
 *   sirot -> ya'im -> mizraqot -> mizlegot -> majtot
 *
 * Observacion: Nm 4:14 invierte parcialmente este orden
 * (majtot -> mizlegot -> ya'im -> mizraqot) - porque el
 * TRANSPORTE requiere los braseros primero. Los sirot no
 * aparecen en Nm 4:14 porque la ceniza ya se limpio ANTES de
 * cubrir (Nm 4:13 "vedishhenu" - limpiaron). Pureza y orden.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_auth.h                 API publica
 *   hashmal_auth_internal.h        altar_mut, get_altar_interno, ahora_ms
 *   esp_log.h (ORO EGIPTO)         AUTH_LOG_*
 *
 * Sin mbedtls ni esp_random - los utensilios no hacen cripto.
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
static const char *AUTH_TAG = "hashmal.auth.utensilios";

#define AUTH_LOG_I(fmt, ...)  ESP_LOGI(AUTH_TAG, fmt, ##__VA_ARGS__)
#define AUTH_LOG_W(fmt, ...)  ESP_LOGW(AUTH_TAG, fmt, ##__VA_ARGS__)
#define AUTH_LOG_E(fmt, ...)  ESP_LOGE(AUTH_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * API PUBLICA - utensilio_purgar (SIROT, Ex 27:3)
 * ================================================================== */

/**
 * hashmal_auth_utensilio_purgar - sirot ledashno
 *
 * "Calderos para limpiar la ceniza (deshen)". Tras sacrificios, las
 * cenizas se acumulan y ahogan el fuego si no se retiran. El
 * sacerdote las recoge con sirot y las lleva fuera del campamento
 * (Lv 6:3-4). El fuego respira.
 *
 * En firmware: purga los contadores operativos (challenges, pruebas
 * validadas/rechazadas, timestamp del ultimo challenge). NO toca
 * la estructura (cuernos, reshet, fuego, estado) - como el sacerdote
 * no desmonta el altar para limpiar cenizas.
 *
 * 2 testigos:
 *   Ex 27:3  - sirotav ledashno (calderos para la ceniza)
 *   Lv 6:3   - velabash hakkohen middo (el sacerdote se viste y retira
 *              la ceniza cada dia)
 */
int hashmal_auth_utensilio_purgar(void)
{
    hashmal_auth_t *a = hashmal_auth_altar_mut();
    if (a == NULL) {
        AUTH_LOG_E("purgar: altar no levantado");
        return HASHMAL_AUTH_ERR_INIT;
    }

    /* Retirar cenizas - contadores operativos a cero. No tocamos
     * sesiones_emitidas ni registros_ledger: son HISTORIA (Ap 6:9
     * ledger es append-only); solo purga de metricas volatiles. */
    a->challenges_emitidos      = 0ull;
    a->pruebas_validadas        = 0ull;
    a->pruebas_rechazadas       = 0ull;
    a->ultimo_challenge_ms      = 0ull;

    AUTH_LOG_W("purga — sirot ledashnó (Éx 27:3); contadores volátiles "
               "a cero (ledger y sesiones preservados)");
    return HASHMAL_AUTH_OK;
}

/* ==================================================================
 * API PUBLICA - utensilio_resetear (YA'IM, Ex 27:3)
 * ================================================================== */

/**
 * hashmal_auth_utensilio_resetear - ya'im, mover brasas
 *
 * "Palas para reposicionar". Las ya'im mueven brasas sin apagar el
 * fuego - recomponen el lecho para que el proximo sacrificio se
 * apoye bien. NO es apagar y reencender: es reacomodar.
 *
 * En firmware: combinacion de purga (contadores) + reset del ciclo
 * (ultimo_ciclo_ms = ahora). El fuego sigue ardiendo (tamid, reshet_
 * activa, cuernos) - solo se reubican las metricas operativas.
 *
 * 2 testigos:
 *   Ex 27:3  - ya'av (palas como utensilio propio)
 *   Lv 6:5   - uvi'er 'aleha hakkohen 'etsim babboqer (el sacerdote
 *              anade lena y reorganiza el fuego cada manana)
 */
int hashmal_auth_utensilio_resetear(void)
{
    hashmal_auth_t *a = hashmal_auth_altar_mut();
    if (a == NULL) {
        AUTH_LOG_E("resetear: altar no levantado");
        return HASHMAL_AUTH_ERR_INIT;
    }

    /* Reposicionar: limpiar metricas volatiles + marcar el momento
     * como nuevo punto de partida del ciclo. */
    a->challenges_emitidos      = 0ull;
    a->pruebas_validadas        = 0ull;
    a->pruebas_rechazadas       = 0ull;
    a->ultimo_challenge_ms      = 0ull;
    a->ultimo_ciclo_ms          = hashmal_auth_ahora_ms();

    AUTH_LOG_W("reposición — yaʻím (Éx 27:3); brasas reubicadas, fuego "
               "sigue ardiendo (tamíd)");
    return HASHMAL_AUTH_OK;
}

/* ==================================================================
 * API PUBLICA - utensilio_recoger (MIZRAQOT, Ex 27:3)
 * ================================================================== */

/**
 * hashmal_auth_utensilio_recoger - mizraqot, recoger y esparcir
 *
 * "Tazones" - del verbo zaraq (H2236) = ESPARCIR. Se usaban para
 * recoger la sangre de la victima y ESPARCIRLA sobre el altar. En
 * firmware: recoleccion de estadisticas. El caller recibe los
 * contadores sin modificar el altar.
 *
 * Cualquier puntero puede ser NULL - solo se escribe en los
 * no-NULL. Mismo patron que breaker_estado / oracion_estado. Ap 5:8
 * "phialas jrysas gemousas thymiamaton" (copas llenas) - aqui el
 * caller recibe las copas llenas de metricas.
 *
 * `fuego_encendido` = (tamid && reshet_activa) - si ambos, el fuego
 * esta ardiendo segun Lv 6:6.
 *
 * 2 testigos:
 *   Ex 27:3  - mizraqotav (tazones son utensilio propio)
 *   Ex 24:6  - "vayyiqaj Moshe JATSI haddam vayyasem ba-agganot":
 *              Moises tomo la mitad de la sangre y la puso en
 *              TAZONES (agganot, sinonimo funcional) para rociarla.
 */
int hashmal_auth_utensilio_recoger(uint64_t *challenges,
                                   uint64_t *validadas,
                                   uint64_t *rechazadas,
                                   uint8_t  *cuernos_activos,
                                   bool     *fuego_encendido,
                                   uint64_t *ultimo_challenge_ms,
                                   uint64_t *ultimo_ciclo_ms)
{
    const hashmal_auth_t *a = hashmal_auth_get_altar_interno();
    if (a == NULL) {
        AUTH_LOG_E("recoger: altar no levantado");
        return HASHMAL_AUTH_ERR_INIT;
    }

    if (challenges           != NULL) *challenges           = a->challenges_emitidos;
    if (validadas            != NULL) *validadas            = a->pruebas_validadas;
    if (rechazadas           != NULL) *rechazadas           = a->pruebas_rechazadas;
    if (cuernos_activos      != NULL) *cuernos_activos      = a->cuernos_activos;
    if (fuego_encendido      != NULL) *fuego_encendido      = a->tamid && a->reshet_activa;
    if (ultimo_challenge_ms  != NULL) *ultimo_challenge_ms  = a->ultimo_challenge_ms;
    if (ultimo_ciclo_ms      != NULL) *ultimo_ciclo_ms      = a->ultimo_ciclo_ms;

    AUTH_LOG_I("recolección — mizraqót (Éx 27:3); estadísticas "
               "recogidas");
    return HASHMAL_AUTH_OK;
}

/* ==================================================================
 * API PUBLICA - utensilio_inspeccionar (MIZLEGOT, Ex 27:3)
 * ================================================================== */

/**
 * hashmal_auth_utensilio_inspeccionar - mizlegot, voltear y ver
 *
 * "Garfios" - voltean la ofrenda sobre las brasas para que el
 * fuego toque cada cara. No cambian la ofrenda: la EXAMINAN desde
 * todos los angulos.
 *
 * En firmware: verificacion de consistencia interna del singleton.
 * Revisa:
 *   a) cuernos_activos coincide con el conteo real de cuernos activos
 *   b) tamid y reshet_activa son coherentes entre si
 *   c) estado == ENCENDIDO (altar operativo)
 *   d) conforme_al_tavnit (Ex 27:8 kaasher her'a bahar)
 *
 * Inconsistencias se REPORTAN via LOG_W pero NO se corrigen - el
 * garfio voltea, no modifica. Si algo esta mal, el operador
 * decide la accion (posiblemente purgar o resetear).
 *
 * 2 testigos:
 *   Ex 27:3  - mizlegotav (garfios son utensilio propio)
 *   1 Sa 2:13-14 - el precedente NEGATIVO: los hijos de Eli abusaron
 *                  del garfio para TOMAR; el uso legitimo es VOLTEAR
 *                  para examinar, no quitar.
 */
int hashmal_auth_utensilio_inspeccionar(void)
{
    const hashmal_auth_t *a = hashmal_auth_get_altar_interno();
    if (a == NULL) {
        AUTH_LOG_E("inspeccionar: altar no levantado");
        return HASHMAL_AUTH_ERR_INIT;
    }

    uint32_t inconsistencias = 0u;

    /* (a) Conteo real de cuernos activos vs el contador almacenado. */
    uint8_t reales = 0u;
    for (uint8_t i = 0; i < HASHMAL_AUTH_CUERNOS; ++i) {
        if (a->cuernos[i].activo) ++reales;
    }
    if (reales != a->cuernos_activos) {
        AUTH_LOG_W("inspeccionar: cuernos_activos=%u ≠ reales=%u "
                   "(incoherencia en contador)",
                   (unsigned)a->cuernos_activos, (unsigned)reales);
        ++inconsistencias;
    }

    /* (b) tamid y reshet_activa deben ir juntos: ambos true (fuego
     * encendido) o ambos false (apagado). Un true / un false es
     * inconsistente. */
    if (a->tamid != a->reshet_activa) {
        /* Caso legitimo: tras init y antes de fuego_encender,
         * tamid=true y reshet_activa=false. No es inconsistencia -
         * es "altar forjado, fuego aun no descendio". Solo
         * alertamos si tras un ciclo operativo aparece el patron. */
        AUTH_LOG_W("inspeccionar: tamíd=%d ≠ reshet_activa=%d "
                   "(legítimo pre-fuego_encender; revisar si es post-)",
                   (int)a->tamid, (int)a->reshet_activa);
        /* No contamos como inconsistencia dura. */
    }

    /* (c) Estado operativo. */
    if (a->estado != HASHMAL_AUTH_ENCENDIDO) {
        AUTH_LOG_W("inspeccionar: estado=%d ≠ ENCENDIDO",
                   (int)a->estado);
        ++inconsistencias;
    }

    /* (d) Conformidad al tavnit (Ex 27:8). */
    if (!a->conforme_al_tavnit) {
        AUTH_LOG_W("inspeccionar: conforme_al_tavnit=false "
                   "(Éx 27:8 kaashér herʼá bahár violado)");
        ++inconsistencias;
    }

    if (inconsistencias == 0u) {
        AUTH_LOG_I("inspección — mizlegót (Éx 27:3); altar consistente");
    } else {
        AUTH_LOG_W("inspección — mizlegót (Éx 27:3); %u inconsistencia%s "
                   "reportada%s (no corregida%s — el garfio voltea, no modifica)",
                   (unsigned)inconsistencias,
                   (inconsistencias == 1u) ? "" : "s",
                   (inconsistencias == 1u) ? "" : "s",
                   (inconsistencias == 1u) ? "" : "s");
    }
    /* Inspeccionar no falla por inconsistencias del dato - solo
     * por el altar no estar levantado. Reporta y retorna OK. */
    return HASHMAL_AUTH_OK;
}

/* ==================================================================
 * API PUBLICA - utensilio_preparar_transporte (MAJTOT, Ex 27:3 + Nm 4:14)
 * ================================================================== */

/**
 * hashmal_auth_utensilio_preparar_transporte - majtot, portar fuego
 *
 * "Braseros" - transportan brasas encendidas. Nm 4:14: al migrar, los
 * kelim (incluyendo los majtot) se colocan sobre el altar y se cubren
 * con piel de tajash antes de moverlo. El fuego no se apaga para
 * viajar - se recoge en braseros y se mueve vivo.
 *
 * Fase 1: solo registra la intencion de preparacion. Fase 2
 * desactivara cuernos, pausara reshet, snapshot del estado para
 * migrar a otro nodo, y simulara el beged argaman (pano purpura,
 * Nm 4:13).
 *
 * 2 testigos:
 *   Ex 27:3  - majtotav (braseros son utensilio propio)
 *   Nm 4:14  - "vesamu baddav" (pusieron sus varas) - preparacion
 *              completa para el traslado, incluyendo varas.
 */
int hashmal_auth_utensilio_preparar_transporte(void)
{
    hashmal_auth_t *a = hashmal_auth_altar_mut();
    if (a == NULL) {
        AUTH_LOG_E("preparar_transporte: altar no levantado");
        return HASHMAL_AUTH_ERR_INIT;
    }

    /* Fase 1: solo registro de intencion. No mutamos estado operativo
     * (eso seria Fase 2). El ahora se registra como "ultima actividad
     * administrativa" - util para el operador. */
    a->ultimo_ciclo_ms = hashmal_auth_ahora_ms();

    AUTH_LOG_I("transporte — majtót (Éx 27:3, Nm 4:14); altar marcado "
               "para preparación de migración (Fase 1: intención)");
    AUTH_LOG_I("  Fase 2 pendiente: béged argamán + desactivación de "
               "cuernos + snapshot");
    return HASHMAL_AUTH_OK;
}

/* ==================================================================
 * API PUBLICA - utensilio_estado(tipo) - query per-keli
 * ================================================================== */

/**
 * hashmal_auth_utensilio_estado - ?un keli especifico esta disponible?
 *
 * Los kelim son PARTE del altar (Ex 38:3). Si el altar esta
 * levantado, TODOS los kelim existen (Ex 27:3 "kol kelav ta'ase
 * nejoshet" - todos juntos, hechos a la vez).
 *
 * Valida que el tipo pertenezca al enum y que el altar este
 * levantado. Si ambas -> OK.
 *
 * 2 testigos:
 *   Ex 27:3  - los 5 kelim nombrados explicitamente
 *   Ex 38:3  - "kol kelav" - todos ejecutados juntos
 */
int hashmal_auth_utensilio_estado(hashmal_auth_utensilio_t tipo)
{
    if (tipo != HASHMAL_AUTH_SIROT &&
        tipo != HASHMAL_AUTH_YAIM &&
        tipo != HASHMAL_AUTH_MIZRAQOT &&
        tipo != HASHMAL_AUTH_MIZLEGOT &&
        tipo != HASHMAL_AUTH_MAJTOT) {
        AUTH_LOG_E("utensilio_estado: tipo %d fuera del tavnít (Éx 27:3)",
                   (int)tipo);
        return HASHMAL_AUTH_ERR_NULL;
    }

    const hashmal_auth_t *a = hashmal_auth_get_altar_interno();
    if (a == NULL) return HASHMAL_AUTH_ERR_INIT;

    return HASHMAL_AUTH_OK;
}

/* ==================================================================
 * API PUBLICA - utensilios_estado - global, lectura pura
 * ================================================================== */

/**
 * hashmal_auth_utensilios_estado - ?los kelim estan disponibles?
 *
 * Query global. Altar levantado -> OK (los 5 existen). Sin logs
 * (lectura limpia como cuerno_activo, reshet_estado, fuego_estado).
 *
 * 2 testigos:
 *   Ex 38:3  - "kol kelav" hechos juntos con el altar
 *   Nm 4:14  - "kol kele hammizbeaj" viajan juntos bajo tajash
 */
int hashmal_auth_utensilios_estado(void)
{
    const hashmal_auth_t *a = hashmal_auth_get_altar_interno();
    if (a == NULL) return HASHMAL_AUTH_ERR_INIT;
    return HASHMAL_AUTH_OK;
}

/* ==================================================================
 * P8 (Verificado #11) - API CANONICA CORTA (5 raices distintas)
 *
 * Ex 27:3 nombra 5 utensilios con 5 raices hebreas. Cada funcion
 * canonica corta lleva la raiz como nombre y delega o extiende a
 * los wrappers legacy (utensilio_*).
 * ================================================================== */

/* nejóshet — interfaz bronce, trust Atrio (Éx 27:2) */
int hashmal_auth_sirot(void)
{
    /* sirotav H5518 - recolectar deshen (Ex 27:3 ledashno). */
    return hashmal_auth_utensilio_purgar();
}

/* nejóshet — interfaz bronce, trust Atrio (Éx 27:2) */
int hashmal_auth_yaim(void)
{
    /* ya'av H3257 - limpiar espacio (Ex 27:3 palas reposicionan). */
    return hashmal_auth_utensilio_resetear();
}

/* nejóshet — interfaz bronce, trust Atrio (Éx 27:2) */
int hashmal_auth_mizraqot(uint64_t *out_validadas,
                          uint64_t *out_rechazadas)
{
    /* mizreqotav H4219 (raiz zaraq H2236 esparcir) - DISTRIBUIR
     * resultado: el sacerdote recogia sangre Y la esparcia. Aqui:
     * snapshot del estado de verificacion (validadas/rechazadas)
     * distribuido al caller. Wrapper sobre utensilio_recoger pero
     * limitado a las 2 estadisticas que el destinatario necesita
     * para reaccionar al resultado. */
    const hashmal_auth_t *a = hashmal_auth_get_altar_interno();
    if (a == NULL) return HASHMAL_AUTH_ERR_INIT;

    if (out_validadas  != NULL) *out_validadas  = a->pruebas_validadas;
    if (out_rechazadas != NULL) *out_rechazadas = a->pruebas_rechazadas;
    return HASHMAL_AUTH_OK;
}

/* nejóshet — interfaz bronce, trust Atrio (Éx 27:2) */
int hashmal_auth_mizlegot(uint8_t *entrada, size_t *tam_inout)
{
    /* mizlegotav H4207 - tridentes MANIPULAN la entrada para que
     * el fuego toque cada parte. En firmware: normalizacion in-place
     * - quita bytes nulos finales (mum descartado, Lv 22:20). NO
     * altera contenido legitimo; solo prepara la ofrenda. */
    if (entrada == NULL || tam_inout == NULL) {
        AUTH_LOG_E("mizlegot: entrada o tam_inout NULL");
        return HASHMAL_AUTH_ERR_NULL;
    }
    const hashmal_auth_t *a = hashmal_auth_get_altar_interno();
    if (a == NULL) return HASHMAL_AUTH_ERR_INIT;

    /* Recortar bytes nulos finales (relleno = mum). */
    size_t n = *tam_inout;
    while (n > 0u && entrada[n - 1u] == 0u) {
        --n;
    }
    if (n != *tam_inout) {
        AUTH_LOG_I("mizlegot: %u → %u bytes (Éx 27:3 trinche voltea)",
                   (unsigned)*tam_inout, (unsigned)n);
        *tam_inout = n;
    }
    return HASHMAL_AUTH_OK;
}

/* nejóshet — interfaz bronce, trust Atrio (Éx 27:2) */
int hashmal_auth_majtot(uint8_t modulo_destino)
{
    /* majtotav H4289 (raiz jatáh H2846 = tomar fuego) - TRANSPORTAR
     * fuego a otros modulos. Nm 16:46 - Aaron con majta entre los
     * vivos y los muertos. Fase 1: registra intencion + log. Fase 2
     * conectara con keygen / consensus para transferir auth tokens. */
    if (modulo_destino == 0u) {
        AUTH_LOG_E("majtot: modulo_destino=0 inválido");
        return HASHMAL_AUTH_ERR_NULL;
    }
    hashmal_auth_t *a = hashmal_auth_altar_mut();
    if (a == NULL) return HASHMAL_AUTH_ERR_INIT;
    if (!a->tamid || !a->reshet_activa) {
        AUTH_LOG_E("majtot: fuego no encendido — no hay brasas que portar "
                   "(Lv 6:6 lo tijbé)");
        return HASHMAL_AUTH_ERR_FUEGO;
    }
    a->ultimo_ciclo_ms = hashmal_auth_ahora_ms();
    AUTH_LOG_I("majtót → módulo %u (Éx 27:3 jatáh; Nm 16:46) — "
               "Fase 1: intención, wiring real en Fase 2",
               (unsigned)modulo_destino);
    return HASHMAL_AUTH_OK;
}

/* ==================================================================
 *
 *   "ve'asita SIROTAV ledashno veYA'AV uMIZRAQOTAV uMIZLEGOTAV
 *    uMAJTOTAV lejol kelav ta'ase NEJOSHET." - Exodo 27:3
 *
 *   "vayya'as et kol kelei hammizbeaj... KOL kelav 'asa nejoshet."
 *     - Exodo 38:3
 *
 *   "kol kele hammizbeaj... ufarsu 'alav kesui 'or TAJASH vesamu
 *    baddav." - Numeros 4:14
 *
 *   Los kelim son herramientas de SERVICIO (sherath, H8334),
 *   no decoracion. El sacerdote necesita CADA uno para operar.
 *
 *   En firmware: mantenimiento (sirot), reubicacion (ya'im),
 *   metricas (mizraqot), inspeccion (mizlegot), migracion
 *   (majtot). 5 kelim, 5 operaciones, bronce total.
 *
 *   Observacion: Nm 4:14 invierte el orden para el transporte -
 *   majtot primero porque la ceniza ya se limpio (Nm 4:13
 *   vedishhenu). Pureza antes del viaje.
 *
 *   5 de 6 archivos del Altar del Sacrificio -
 *   altar_utensilios.c listo. Queda:
 *     altar_ledger.c       Ap 6:9 - almas bajo el altar
 *
 * ================================================================== */
