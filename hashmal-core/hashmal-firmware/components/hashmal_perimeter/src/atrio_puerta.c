/**
 * ===================================================================
 * PIEZA 07 - EL ATRIO (JATSER) - atrio_puerta.c
 * Sub-pieza: SHA'AR HEJATSER - la puerta UNICA del atrio
 * Exodo 27:16 + Exodo 40:8,33 + Salmo 118:19-20 + Juan 10:7-10
 *                 + Salmo 100:4
 * ===================================================================
 *
 * "ulesha'ar hejatser masaj 'ESRIM AMMA tejelet ve'argaman vetola'at
 *  shani veshesh mashzar ma'aseh ROQEM 'ammudeihem ARBA'A ve'adneihem
 *  ARBA'A" - Exodo 27:16
 * "Para la PUERTA del atrio: cortina de VEINTE codos, azul, purpura,
 *  carmesi y lino torcido, obra de BORDADOR; sus columnas CUATRO y
 *  sus bases CUATRO."
 *
 * "vayyittEN et masaj sha'ar hejatser" - Exodo 40:33
 * "PUSO la cortina de la puerta del atrio."
 * (Segundo acto - despues de vayyaqem.)
 *
 * "PITJU li sha'arei TSEDEQ 'avo vam 'ode Yah" - Salmo 118:19
 * "ABRIDME las puertas de JUSTICIA; entrare por ellas, alabare a Yah."
 *
 * "ZE hasha'ar laYHWH TSADDIQIM yavo'u vo" - Salmo 118:20
 * "ESTE es el portal de YHWH; los JUSTOS entraran por el."
 *
 * "ego eimi he THYRA ton probaton... di' emou ean tis EISELTHE
 *  SOTHESETAI kai eiseleusetai kai exeleusetai kai NOMEN heuresei"
 *   - Juan 10:7,9
 * "Yo soy la PUERTA de las ovejas... el que por Mi entrare, sera SALVO,
 *  y entrara y saldra y hallara PASTO."
 *
 * "ho KLEPTES ouk erchetai ei me hina klepse kai thyse kai apolese"
 *   - Juan 10:10
 * "El LADRON no viene sino para hurtar, degollar y destruir."
 *
 * --- VOCABULARIO ---
 *
 *   sha'ar       (Sh'R, H8179)     PUERTA. UNA sola. Al ESTE.
 *   masaj        (MSK, H4539)     CORTINA (!= parojet interior).
 *   roqem        (RQM, H7551)      BORDADOR. != josev (tejedor de
 *                                     querubines). La puerta es mas
 *                                     simple que el velo.
 *   pitju        (PTChW, H6605)   ABRID (imperativo). La puerta
 *                                     debe ABRIRSE - no lo esta por
 *                                     defecto.
 *   tsedeq       (TsDQ, H6664)      JUSTICIA. La puerta SE LLAMA
 *                                     "de justicia".
 *   tsaddiqim    (TsDYQYM, H6662) JUSTOS. Sal 118:20 - ellos
 *                                     entraran.
 *   thyra        (thura, G2374)         PUERTA (griego). Jesus es LA
 *                                     puerta (articulo definido).
 *   eiserchomai  (eiserchomai, G1525)  ENTRAR - Jn 10:9.
 *   sozo         (sozo, G4982)        SALVAR - Jn 10:9 sothesetai.
 *   nome         (nomh, G3542)        PASTO - Jn 10:9.
 *   kleptes      (klepths, G2812)    LADRON - Jn 10:10. El que NO
 *                                     usa la puerta.
 *   lestes       (lhsths, G3027)     SALTEADOR. Violento.
 *
 * --- 3 PROMESAS DE LA PUERTA (Jn 10:9) ---
 *
 *   sothesetai      SERA SALVO         -> acceso legitimo
 *   eisel./exel.   entrara y SALDRA    -> libertad bidireccional
 *   nomen heuresei hallara PASTO       -> sustento / recursos
 *
 * --- TEOLOGIA DE LA PUERTA ---
 *
 *   1) UNA SOLA PUERTA. Sin backdoors. Ex 27:16 nombra UNA.
 *      Jn 10:9 "he thyra" con articulo definido = LA puerta.
 *
 *   2) AL ESTE (qedma mizraja, Ex 27:13). Direccion del sol
 *      naciente - esperanza. La primera luz toca la puerta.
 *
 *   3) 4 COLUMNAS de validacion (Ex 27:16 arba'a). Cada
 *      solicitud se sella por UNA de las 4 - redundancia de
 *      firmas de entrada.
 *
 *   4) BORDADA (roqem), no tejida con querubines. Acceso
 *      exterior = mas simple que proteccion interior.
 *
 *   5) ABRIR ES ACTO (pitju). La puerta no abre sola. Sal 118:19
 *      manda abrirla. Operacion explicita, no defecto.
 *
 *   6) LADRON vs PUERTA (Jn 10:8-10). Si no entras por la
 *      puerta, eres kleptes y lestes. No hay zona gris.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   puerta_abrir       Sal 118:19 pitju li - abrir
 *   puerta_cerrar      cerrar acceso
 *   puerta_estado      lectura pura sin logs
 *   puerta_verificar   Jn 10:9 eiselthe - decidir admision
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_perimeter.h      API publica (incluye verificar_intencion)
 *   hashmal_perim_internal.h atrio_mut, get_atrio_interno, ahora_ms
 *   esp_log.h (ORO EGIPTO)   PUERTA_LOG_*
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

#include "hashmal_perimeter.h"
#include "hashmal_perim_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *PUERTA_TAG = "hashmal.perim.puerta";

#define PUERTA_LOG_I(fmt, ...)  ESP_LOGI(PUERTA_TAG, fmt, ##__VA_ARGS__)
#define PUERTA_LOG_W(fmt, ...)  ESP_LOGW(PUERTA_TAG, fmt, ##__VA_ARGS__)
#define PUERTA_LOG_E(fmt, ...)  ESP_LOGE(PUERTA_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * HELPER ESTATICO - escoger columna validadora (Ex 27:16 arba'a)
 *
 * Las 4 columnas de la puerta viven en el array global con lado =
 * LADO_PUERTA. Escogemos la PRIMERA ACTIVA entre ellas y devolvemos
 * su posicion local (0..3) - no la id global. Si ninguna activa,
 * retorna false.
 *
 * El orden de recorrido es el orden de los slots globales (0..59).
 * Ese orden lo define la secuencia de `columna_registrar` - politica
 * externa, no de este archivo.
 * ================================================================== */
static bool escoger_validadora(const hashmal_perimeter_t *a,
                               uint8_t *out_local_idx)
{
    uint8_t pos = 0u;

    for (uint8_t i = 0; i < HASHMAL_PERIM_COLUMNAS; ++i) {
        if (a->columnas[i].lado != HASHMAL_PERIM_LADO_PUERTA) {
            continue;
        }
        /* Es una columna de la puerta (lado == PUERTA). Posicion
         * local `pos` dentro del conjunto de puerta. */
        if (a->columnas[i].activa) {
            *out_local_idx = pos;
            return true;
        }
        ++pos;
    }
    return false;
}

/* ==================================================================
 * API PUBLICA - puerta_abrir (Sal 118:19 pitju li)
 * ================================================================== */

/**
 * hashmal_perimeter_puerta_abrir - pitju li sha'arei tsedeq
 *
 * Abre la puerta del atrio. Requiere:
 *   - atrio levantado                  (ERR_INIT)
 *   - atrio en ACTIVO (60 columnas)    (ERR_DESMONTADO si solo LEVANTADO)
 *
 * Idempotente sobre ABIERTA.
 *
 * 2 testigos:
 *   Sal 118:19 - pitju li (imperativo: abrid)
 *   Ex 40:33   - vayyitten et masaj (Moises puso la cortina)
 */
int hashmal_perimeter_puerta_abrir(void)
{
    hashmal_perimeter_t *a = hashmal_perim_atrio_mut();
    if (a == NULL) {
        PUERTA_LOG_E("puerta_abrir: atrio no levantado");
        return HASHMAL_PERIM_ERR_INIT;
    }

    /* Ex 27:17 - "kol 'ammudei hejatser saviv": el atrio debe
     * estar ACTIVO (60 columnas registradas) para abrir la puerta.
     * En LEVANTADO faltan columnas - la puerta careceria de
     * validadoras. */
    if (a->estado != HASHMAL_PERIM_ACTIVO) {
        PUERTA_LOG_E("puerta_abrir: atrio no ACTIVO (estado=%d) — "
                     "faltan columnas (Éx 27:17 kol ʻammudéi)",
                     (int)a->estado);
        return HASHMAL_PERIM_ERR_DESMONTADO;
    }

    if (a->puerta_estado == HASHMAL_PERIM_PUERTA_ABIERTA) {
        PUERTA_LOG_I("puerta_abrir: ya ABIERTA (idempotente, no-op)");
        return HASHMAL_PERIM_OK;
    }

    a->puerta_estado = HASHMAL_PERIM_PUERTA_ABIERTA;

    PUERTA_LOG_I("═══ PUERTA ABIERTA ═══ (Sal 118:19 pitjú li shaʻaréi tsédeq)");
    PUERTA_LOG_I("  zé hasháʻar laYHWH — tsaddiqím yavóʼu vo (Sal 118:20)");
    PUERTA_LOG_I("  ʻesrím ammá, 4 ʻammudím (Éx 27:16) — validadores en pie");
    return HASHMAL_PERIM_OK;
}

/* ==================================================================
 * API PUBLICA - puerta_cerrar
 * ================================================================== */

/**
 * hashmal_perimeter_puerta_cerrar - cerrar la puerta
 *
 * Neh 13:19 - "vayhi kaasher tsalelu sha'arei Yerushalayim lifnei
 *  hashabbat... vayyisageru hadelatot": "cuando las puertas de
 *  Jerusalen hicieron sombra antes del shabat... cerraron las
 *  puertas." La puerta se cierra en ciertos tiempos (mantenimiento,
 *  fuera del mo'ed, riesgo).
 *
 * Idempotente sobre CERRADA.
 *
 * 2 testigos:
 *   Neh 13:19 - vayyisageru hadelatot (cerraron puertas)
 *   Sal 118:20 - tsaddiqim yavo'u vo (cerrar protege el paso legitimo)
 */
int hashmal_perimeter_puerta_cerrar(void)
{
    hashmal_perimeter_t *a = hashmal_perim_atrio_mut();
    if (a == NULL) {
        PUERTA_LOG_E("puerta_cerrar: atrio no levantado");
        return HASHMAL_PERIM_ERR_INIT;
    }

    if (a->puerta_estado == HASHMAL_PERIM_PUERTA_CERRADA) {
        PUERTA_LOG_I("puerta_cerrar: ya CERRADA (idempotente)");
        return HASHMAL_PERIM_OK;
    }

    a->puerta_estado = HASHMAL_PERIM_PUERTA_CERRADA;

    PUERTA_LOG_I("puerta CERRADA (Neh 13:19 vayyisagerú hadelatót)");
    return HASHMAL_PERIM_OK;
}

/* ==================================================================
 * API PUBLICA - puerta_estado (lectura pura)
 * ================================================================== */

/**
 * hashmal_perimeter_puerta_estado - ?la puerta esta abierta?
 *
 * Lectura pura, sin logs. Patron consistente con los otros
 * *_estado del proyecto.
 *
 * Retornos:
 *   OK           si ABIERTA
 *   ERR_PUERTA   si CERRADA o VERIFICANDO
 *   ERR_INIT     si atrio no levantado
 *
 * 2 testigos:
 *   Ex 27:16  - masaj sha'ar (estado de la cortina)
 *   Sal 118:20 - ze hasha'ar (identidad de LA puerta)
 */
int hashmal_perimeter_puerta_estado(void)
{
    const hashmal_perimeter_t *a = hashmal_perim_get_atrio_interno();
    if (a == NULL) return HASHMAL_PERIM_ERR_INIT;

    if (a->puerta_estado == HASHMAL_PERIM_PUERTA_ABIERTA) {
        return HASHMAL_PERIM_OK;
    }
    return HASHMAL_PERIM_ERR_PUERTA;
}

/* ==================================================================
 * API PUBLICA - puerta_verificar (Jn 10:9 eiselthe)
 * ================================================================== */

/**
 * hashmal_perimeter_puerta_verificar - "di' emou ean tis eiselthe"
 *
 * Verifica una solicitud de entrada. Delega la comprobacion de
 * intencion (toda + tehilla + tsedeq) a atrio_intencion.c. Si la
 * intencion es COMPLETA y hay una columna validadora activa, admite;
 * si no, rechaza.
 *
 * --- FLUJO ---
 *
 *   1) sol != NULL, res != NULL    (ERR_NULL)
 *   2) atrio levantado             (ERR_INIT)
 *   3) puerta == ABIERTA           (ERR_PUERTA)
 *   4) res inicializado a cero + timestamp
 *   5) puerta -> VERIFICANDO (transitorio)
 *   6) verificar_intencion (delegacion)
 *   7a) si intencion incompleta:
 *         res->admitido = false
 *         rechazos_totales++
 *         puerta -> ABIERTA (restaurar)
 *         LOG_W kleptes / lestes (Jn 10:10)
 *         return ERR_INTENCION
 *   7b) si intencion completa:
 *         escoger columna validadora (4 columnas de la puerta)
 *         si ninguna activa -> rechazar (ERR_PUERTA)
 *         else:
 *           res->admitido = true
 *           res->columna_validadora = idx local (0..3)
 *           res->intencion_verificada = sol->intencion_flags
 *           entradas_totales++ + ultimo_acceso_ms
 *           puerta -> ABIERTA (restaurar)
 *           LOG_I eiselthe (Jn 10:9)
 *           return OK
 *
 * 2 testigos:
 *   Jn 10:9     - eiselthe sothesetai (entrara y sera salvo)
 *   Sal 118:20  - tsaddiqim yavo'u vo (los justos entraran)
 */
int hashmal_perimeter_puerta_verificar(const hashmal_perimeter_solicitud_t *sol,
                                       hashmal_perimeter_resultado_t *res)
{
    if (sol == NULL || res == NULL) {
        PUERTA_LOG_E("puerta_verificar: sol o res NULL");
        return HASHMAL_PERIM_ERR_NULL;
    }

    hashmal_perimeter_t *a = hashmal_perim_atrio_mut();
    if (a == NULL) {
        PUERTA_LOG_E("puerta_verificar: atrio no levantado");
        return HASHMAL_PERIM_ERR_INIT;
    }

    if (a->puerta_estado != HASHMAL_PERIM_PUERTA_ABIERTA) {
        PUERTA_LOG_W("puerta_verificar: puerta no ABIERTA (estado=%d) — "
                     "pitjú li pendiente (Sal 118:19)",
                     (int)a->puerta_estado);
        return HASHMAL_PERIM_ERR_PUERTA;
    }

    /* Preparar resultado limpio. */
    memset(res, 0, sizeof(*res));
    res->timestamp_ms = hashmal_perim_ahora_ms();

    /* Transicion transitoria a VERIFICANDO. Restaurar en TODOS los
     * caminos de retorno para no dejar la puerta encerrada. */
    a->puerta_estado = HASHMAL_PERIM_PUERTA_VERIFICANDO;

    /* -- Delegacion a atrio_intencion.c -- */
    const int rc_intencion = hashmal_perimeter_verificar_intencion(sol);

    if (rc_intencion != HASHMAL_PERIM_OK) {
        /* Añadidura 05 - miqlat fallback. Si este nodo es ciudad de
         * refugio (Nm 35:11), la puerta acepta al que huye aunque
         * no traiga las 3 intenciones. Nm 35:15 leKOL... velagger:
         * acceso universal. El juicio lo hara la 'edah despues
         * (Jos 20:6), no la puerta ahora. */
        if (hashmal_perimeter_miqlat_designado()) {
            int rc_ref = hashmal_perimeter_miqlat_refugiar(
                sol->nodo_id, HASHMAL_MIQLAT_MOTIVO_DESCONOCIDO);
            if (rc_ref == HASHMAL_PERIM_OK) {
                res->admitido             = true;
                res->intencion_verificada = sol->intencion_flags;
                a->puerta_estado          = HASHMAL_PERIM_PUERTA_ABIERTA;
                PUERTA_LOG_I("miqlat refugio (Nm 35:15 leKOL Yisrael "
                             "veLAGER) — nodo %u aceptado sin intención "
                             "completa; pendiente juicio de la 'edah",
                             (unsigned)sol->nodo_id);
                return HASHMAL_PERIM_OK;
            }
            /* refugiar fallo (tabla llena o ya MALICIOSO) - cae a
             * rechazo normal. No conto como rechazo del miqlat, si
             * como rechazo de la puerta regular. */
        }

        /* Jn 10:10 - el que no entra por la puerta es kleptes. */
        res->admitido            = false;
        res->intencion_verificada = sol->intencion_flags;   /* que trajo */
        a->rechazos_totales     += 1ull;
        a->puerta_estado         = HASHMAL_PERIM_PUERTA_ABIERTA;

        PUERTA_LOG_W("kléptēs kaí lēstḗs (Jn 10:10) — acceso denegado, "
                     "intención incompleta (rc_int=%d, flags=0x%02X); "
                     "rechazos=%" PRIu64,
                     rc_intencion,
                     (unsigned)sol->intencion_flags,
                     a->rechazos_totales);
        return HASHMAL_PERIM_ERR_INTENCION;
    }

    /* Intencion completa. Escoger columna validadora. */
    uint8_t local_idx = 0u;
    if (!escoger_validadora(a, &local_idx)) {
        /* 4 columnas de la puerta inactivas - no hay quien selle. */
        res->admitido            = false;
        res->intencion_verificada = sol->intencion_flags;
        a->rechazos_totales     += 1ull;
        a->puerta_estado         = HASHMAL_PERIM_PUERTA_ABIERTA;

        PUERTA_LOG_E("puerta_verificar: NINGUNA de las 4 columnas de la "
                     "puerta está activa — no hay validadora (Éx 27:16); "
                     "rechazos=%" PRIu64,
                     a->rechazos_totales);
        return HASHMAL_PERIM_ERR_PUERTA;
    }

    /* Admitido - los 3 regalos de Jn 10:9: salvacion, libertad,
     * pasto. En firmware: el nodo ahora esta dentro del atrio, puede
     * circular, tiene recursos disponibles. */
    res->admitido             = true;
    res->columna_validadora   = local_idx;
    res->intencion_verificada = sol->intencion_flags;

    a->entradas_totales  += 1ull;
    a->ultimo_acceso_ms   = res->timestamp_ms;
    a->puerta_estado      = HASHMAL_PERIM_PUERTA_ABIERTA;

    PUERTA_LOG_I("eisélthē (Jn 10:9) — nodo %u ADMITIDO por columna "
                 "puerta[%u]; entradas=%" PRIu64,
                 (unsigned)sol->nodo_id,
                 (unsigned)local_idx,
                 a->entradas_totales);
    PUERTA_LOG_I("  sōthḗsetai + eisel./exel. + nomḗn — tsédeq "
                 "confirmada (Sal 118:20)");
    return HASHMAL_PERIM_OK;
}

/* ==================================================================
 *
 *   "PITJU li sha'arei TSEDEQ 'avo vam 'ode Yah." - Salmo 118:19
 *   "Abridme las puertas de JUSTICIA; entrare por ellas, alabare
 *    a Yah."
 *
 *   "ZE hasha'ar laYHWH TSADDIQIM yavo'u vo." - Salmo 118:20
 *   "ESTE es el portal de YHWH; los JUSTOS entraran por el."
 *
 *   "ego eimi he thyra; di' emou ean tis EISELTHE SOTHESETAI kai
 *    eiseleusetai kai exeleusetai kai NOMEN heuresei." - Juan 10:9
 *   "Yo soy la puerta; el que por Mi entre, sera SALVO, y ENTRARA
 *    y SALDRA, y hallara PASTO."
 *
 *   UNA puerta. UNA entrada. Sin backdoors. El que entra por ella
 *   es salvo, entra y sale libre, halla pasto. El que no -
 *   kleptes kai lestes (Jn 10:10).
 *
 *   3 de 4 archivos del Atrio - atrio_puerta.c listo. Queda:
 *     atrio_intencion.c   Sal 100:4 + Sal 118:19 - toda+tehilla+tsedeq
 *
 * ================================================================== */
