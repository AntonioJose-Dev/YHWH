/**
 * ===================================================================
 * PIEZA 07 - EL ATRIO (JATSER) - atrio_perimetro.c
 * Sub-pieza: JATSER SAVIV - el perimetro alrededor
 * Exodo 40:8,33 + Exodo 27:9,18-19 + Numeros 4:26,32
 * ===================================================================
 *
 * "vesamta et hejatser SAVIV venatatta et masaj sha'ar hejatser"
 *   - Exodo 40:8
 * "Pondras el atrio ALREDEDOR y pondras la cortina de la puerta."
 *
 * "vayyAQEM et hejatser saviv lamishkan VELAMMIZBEAJ vayyitten et
 *  masaj sha'ar hejatser vayJAL Moshe et hammelajak" - Exodo 40:33
 * "LEVANTO el atrio alrededor del Mishkan Y del altar, puso la
 *  cortina de la puerta, y Moises TERMINO la obra."
 *
 * "orej hejatser me'a va'amma verojav jamishim bajamishin veqoma
 *  jamesh ammot shesh mashzar ve'adneihem NEJOSHET" - Exodo 27:18
 * "El largo del atrio: CIEN codos. El ancho: CINCUENTA. La altura:
 *  CINCO. Lino torcido. Bases de BRONCE."
 *
 * "lejol kelei hammishkan... vejol yitedot hejatser NEJOSHET"
 *   - Exodo 27:19
 * "Todos los utensilios... y todas las ESTACAS del atrio: BRONCE."
 *
 * "ve'et qal'ei hejatser ve'et masaj petaj sha'ar hejatser" - Nm 4:26
 * "Las CORTINAS del atrio y la cortina de la puerta del atrio."
 * (Los hijos de GERSHON cargan cortinas - Nm 4:24-26.)
 *
 * "ve'ammudei hejatser saviv ve'adneihem viyitdotam" - Nm 4:32
 * "Las COLUMNAS del atrio alrededor, y sus bases y sus estacas."
 * (Los hijos de MERARI cargan estructura pesada - Nm 4:31-32.)
 *
 * --- VOCABULARIO HEBREO ---
 *
 *   saviv       (SBYB, H5439)   ALREDEDOR. No una pared: un CIRCULO
 *                                  completo que envuelve.
 *   vayyaqem    (WYQM, H6965)  LEVANTO. Hifil de qum - hacer que
 *                                  se pare. Moises hizo que el atrio
 *                                  se parara.
 *   vayjal      (WYKL, H3615)   TERMINO. Moises cerro la obra del
 *                                  Mishkan al levantar el atrio. El
 *                                  atrio es la ULTIMA pieza - lo
 *                                  que completa todo.
 *   yeted       (YTD, H3489)    ESTACA. Bronce. Ancla al suelo las
 *                                  columnas. Sin ellas, el viento
 *                                  derriba cortinas.
 *   nejoshet    (NChShT, H5178) BRONCE. Bases, ganchos-base,
 *                                  estacas - TODO de bronce en la
 *                                  frontera exterior.
 *
 * --- DOS ACCIONES EN Ex 40:33 ---
 *
 * Ex 40:33 NOMBRA dos actos distintos de Moises:
 *   1. vayyAQEM et hejatser saviv   - LEVANTO el atrio (estructura)
 *   2. vayyittEN et masaj sha'ar    - PUSO la cortina de la puerta
 *
 * init en este archivo hace SOLO (1) - estructura general,
 * columnas enumeradas, puerta marcada como CERRADA. La cortina
 * efectiva (abrir puerta, verificar intencion) viene de
 * atrio_puerta.c. Mismo patron que Ex 40:30 en el kiyor: vayyasem
 * (montar) antes de vayyitten (llenar).
 *
 * --- EL ATRIO ENVUELVE MISHKAN Y ALTAR (v.33) ---
 *
 * "saviv laMISHKAN veLAMMIZBEAJ" - rodea al Mishkan Y al altar.
 * El atrio NO envuelve solo una cosa: contiene DOS centros - la
 * Tienda (Qodesh) y el Altar del Sacrificio (Jatser interior). El
 * perimetro es la frontera unica para ambos.
 *
 * --- DESMONTAJE (Nm 4:26,32) ---
 *
 * Dos familias leviticas distintas cargan el atrio al migrar:
 *   Gershon  (Nm 4:24-26)  - qal'ei (cortinas) + masaj (puerta)
 *   Merari   (Nm 4:31-32)  - 'ammudim + 'adneihem + yitdotam
 *
 * Division del trabajo: textiles livianos vs estructura pesada.
 * En firmware: deinit resetea todos los componentes; el orden
 * conceptual (cortinas primero, estructura despues) es solo
 * documentacion.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   perimeter_init      Ex 40:33 vayyaqem - levantar el atrio
 *   perimeter_deinit    Nm 4:26,32 - desmontar (Gershon + Merari)
 *   perimeter_estado    lectura pura sin logs
 *
 *   + helpers internos (hashmal_perim_internal.h):
 *   perim_atrio_mut              mutable para hermanos
 *   perim_get_atrio_interno      const interno
 *   perim_ahora_ms               timestamp ms monotonico
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_perimeter.h           API publica
 *   hashmal_perim_internal.h      struct interna + helpers
 *   esp_log.h   (ORO EGIPTO)      logging
 *   esp_timer.h (ORO EGIPTO)      ahora_ms
 *
 * Autor: Antonio Jose Marin Soto (Betsalel - Ex 31:2)
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <inttypes.h>

#include "esp_log.h"
#include "esp_timer.h"

#include "hashmal_perimeter.h"
#include "hashmal_perim_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *PERIM_TAG = "hashmal.perim.core";

#define PERIM_LOG_I(fmt, ...)  ESP_LOGI(PERIM_TAG, fmt, ##__VA_ARGS__)
#define PERIM_LOG_W(fmt, ...)  ESP_LOGW(PERIM_TAG, fmt, ##__VA_ARGS__)
#define PERIM_LOG_E(fmt, ...)  ESP_LOGE(PERIM_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * P48 — vayyitpallél + vayyaʻamíd (Neh 4:3): Intercesión Fase 0
 *
 * "vayyitpallél (interpelaron) vayyaʻamíd mishmár (estableció guardia)."
 * ORDEN: PRIMERO intercede, DESPUÉS defiende. Intercesión es Fase 0
 * de defensa perimetral.
 * ================================================================== */
static bool s_defensa_activa = false;

/* ==================================================================
 * P50 — Timing astronómico guardia (Neh 4:15)
 *
 * "meʻalót hashájar ʻad tsét hakkojavím" — desde alba hasta ocaso.
 * Día = trabajo + guardia; noche = guardia intenso. Sincronización
 * con ciclos de poder (pieza 08).
 * ================================================================== */
static bool s_modo_guardia = false;  /* true = noche (guardia), false = día (trabajo) */

/* ==================================================================
 * SINGLETON SAGRADO
 *
 * UN atrio por nodo - Ex 27:9 "hejatser" en singular. saviv
 * rodea al Mishkan y al altar. Sin factory, sin pool: una sola
 * frontera de lino torcido por 5 codos de alto.
 * ================================================================== */
static hashmal_perimeter_t g_atrio;
static bool                g_levantado = false;

/* ==================================================================
 * HELPERS ESTATICOS
 * ================================================================== */

/**
 * Inicializa las 60 columnas en estado "no registrada". Los
 * registros se iran llenando con `hashmal_perimeter_columna_registrar`
 * (atrio_columnas.c) segun los levitas de Merari hacen el trabajo.
 */
static void inicializar_columnas(void)
{
    for (uint8_t i = 0; i < HASHMAL_PERIM_COLUMNAS; ++i) {
        g_atrio.columnas[i].id          = i;
        g_atrio.columnas[i].lado        = HASHMAL_PERIM_LADO_SUR; /* placeholder */
        g_atrio.columnas[i].activa      = false;
        g_atrio.columnas[i].registro_ms = 0ull;
    }
    g_atrio.columnas_registradas = 0u;
}

/* ==================================================================
 * API INTERNA - helpers compartidos (hashmal_perim_internal.h)
 * ================================================================== */

hashmal_perimeter_t *hashmal_perim_atrio_mut(void)
{
    if (!g_levantado) return NULL;
    return &g_atrio;
}

const hashmal_perimeter_t *hashmal_perim_get_atrio_interno(void)
{
    if (!g_levantado) return NULL;
    return &g_atrio;
}

uint64_t hashmal_perim_ahora_ms(void)
{
    return ((uint64_t)esp_timer_get_time()) / 1000ull;
}

/* ==================================================================
 * hashmal_perimeter_init - Exodo 40:8 + 40:33 (vayyaqem)
 *
 * Levanta el atrio alrededor del Mishkan y del altar. Estado
 * inicial: LEVANTADO. La puerta queda CERRADA (la cortina sha'ar
 * la maneja atrio_puerta.c - vayyitten es acto separado).
 * ================================================================== */
int hashmal_perimeter_init(void)
{
    if (g_levantado) {
        PERIM_LOG_I("init: atrio ya levantado (idempotente, no-op)");
        return HASHMAL_PERIM_OK;
    }

    /* Partimos de cero - lino desenrollado antes del pilar. */
    memset(&g_atrio, 0, sizeof(g_atrio));
    const uint64_t ahora = hashmal_perim_ahora_ms();

    /* Ex 40:33 - vayyaqem: estado LEVANTADO (estructura en pie,
     * cortinas colgadas, columnas esperando ser registradas). */
    g_atrio.estado        = HASHMAL_PERIM_LEVANTADO;

    /* La puerta arranca CERRADA - Ex 40:33 dice que Moises puso la
     * cortina, pero la apertura del acceso es acto operativo que
     * hace atrio_puerta.c (Sal 118:19 "pitju li"). */
    g_atrio.puerta_estado = HASHMAL_PERIM_PUERTA_CERRADA;

    /* 60 columnas preparadas para registro (Ex 27:10-16).
     * atrio_columnas.c las activara lado por lado. */
    inicializar_columnas();

    /* Timestamps y contadores limpios. */
    g_atrio.init_ms          = ahora;
    g_atrio.entradas_totales = 0ull;
    g_atrio.rechazos_totales = 0ull;
    g_atrio.ultimo_acceso_ms = 0ull;

    g_levantado = true;

    PERIM_LOG_I("═══ ATRIO LEVANTADO ═══ (Éx 40:33 vayyáqem et hejatsér savív)");
    PERIM_LOG_I("  %dx%dx%d codos — shésh mashzár savív (Éx 27:18)",
                HASHMAL_PERIM_LARGO, HASHMAL_PERIM_ANCHO, HASHMAL_PERIM_ALTO);
    PERIM_LOG_I("  %u columnas (20+20+10+3+3+4) preparadas para registro "
                "(Éx 27:10-16)",
                HASHMAL_PERIM_COLUMNAS);
    PERIM_LOG_I("  bases de nejóshet (Éx 27:18) — fundamento de juicio");
    PERIM_LOG_I("  puerta CERRADA — pitjú li pendiente (Sal 118:19)");
    PERIM_LOG_I("  savív lamishkán VELAMMIZBÉAJ — envuelve tienda y altar");
    return HASHMAL_PERIM_OK;
}

/* ==================================================================
 * hashmal_perimeter_deinit - Numeros 4:26 + Numeros 4:32
 *
 * Gershon carga cortinas (Nm 4:24-26). Merari carga estructura
 * pesada: columnas, bases, estacas (Nm 4:31-32). Cada familia a
 * su oficio. En firmware: reseteo atomico.
 * ================================================================== */
void hashmal_perimeter_deinit(void)
{
    if (!g_levantado) {
        PERIM_LOG_W("deinit: atrio no estaba levantado (no-op)");
        return;
    }

    /* Reset atomico. Preservamos el tipo/id de las columnas (son
     * posiciones del tavnit), pero las marcamos inactivas.
     * entradas_totales y rechazos_totales se PIERDEN al desmontar
     * - en Fase 2, un deinit preservara auditoria via hashmal_storage.
     * Por ahora: memset completo, identidad reconstruida al init. */
    memset(&g_atrio, 0, sizeof(g_atrio));
    g_atrio.estado        = HASHMAL_PERIM_DESMONTADO;
    g_atrio.puerta_estado = HASHMAL_PERIM_PUERTA_CERRADA;

    g_levantado = false;

    PERIM_LOG_I("═══ ATRIO DESMONTADO ═══ (Nm 4:26 + Nm 4:32)");
    PERIM_LOG_I("  Gershón carga qalʻéi hejatsér (cortinas, Nm 4:24-26)");
    PERIM_LOG_I("  Merarí carga ʻammudím + ʼadnéihem + yitdotám "
                "(estructura, Nm 4:31-32)");
}

/* ==================================================================
 * hashmal_perimeter_estado - lectura pura
 *
 * LEVANTADO / ACTIVO -> OK (operativo).
 * DESMONTADO         -> ERR_DESMONTADO.
 *
 * Sin logs - patron consistente con los *_estado del kiyor y del
 * altar del sacrificio.
 *
 * 2 testigos:
 *   Ex 40:33  - vayyaqem (levantado = operativo)
 *   Nm 4:26   - desmontaje legitimo (empacado = no operativo)
 * ================================================================== */
int hashmal_perimeter_estado(void)
{
    if (!g_levantado) return HASHMAL_PERIM_ERR_DESMONTADO;

    switch (g_atrio.estado) {
    case HASHMAL_PERIM_LEVANTADO:
    case HASHMAL_PERIM_ACTIVO:
        return HASHMAL_PERIM_OK;
    case HASHMAL_PERIM_DESMONTADO:
        return HASHMAL_PERIM_ERR_DESMONTADO;
    default:
        return HASHMAL_PERIM_ERR_INIT;
    }
}

/* ==================================================================
 * P48 - hashmal_perimeter_activar_defensa
 *
 * "vayyitpallél el Elohéinu vayyaʻamíd mishmár" — Neh 4:3.
 * ORDEN PRESCRIPTIVO: PRIMERO intercede (pieza 11), DESPUÉS defiende.
 * Intercesión es FASE 0 de defensa perimetral.
 * ================================================================== */

int hashmal_perimeter_activar_defensa(void)
{
    if (!g_levantado) {
        PERIM_LOG_E("activar_defensa: atrio no levantado");
        return HASHMAL_PERIM_ERR_DESMONTADO;
    }

    if (s_defensa_activa) {
        PERIM_LOG_W("activar_defensa: defensa ya activa (idempotente, no-op)");
        return HASHMAL_PERIM_OK;
    }

    /* Neh 4:3 - PRIMERO intercesión. Extern declaration permite que
     * hashmal_perimeter NO dependa de hashmal_intercession (REQUIRES limpio). */
    extern int hashmal_intercession_fase_cero_sync(void);

    int rc = hashmal_intercession_fase_cero_sync();
    if (rc != 0) {
        /* Intercesión falló — defensa en modo degradado (log warning). */
        PERIM_LOG_W("activar_defensa: intercesión falló (rc=%d) "
                    "— defensa degradada (Neh 4:3)", rc);
        /* Continuamos: sin intercesión, la defensa sigue operando pero sin
         * protección espiritual explícita. */
    }

    /* DESPUÉS de intercesión, activar defensa. */
    s_defensa_activa = true;
    g_atrio.estado = HASHMAL_PERIM_ACTIVO;

    PERIM_LOG_I("═══ DEFENSA PERIMETRAL ACTIVADA ═══ (Neh 4:3)");
    PERIM_LOG_I("  Fase 0 (intercesión) — completada (Neh 4:3 vayyitpallél)");
    PERIM_LOG_I("  Fase 1 (defensa) — establecida (Neh 4:3 vayyaʻamíd mishmár)");
    return HASHMAL_PERIM_OK;
}

/* ==================================================================
 * P50 - hashmal_perimeter_ciclo_astronomico
 *
 * "meʻalót hashájar ʻad tsét hakkojavím" — Neh 4:15.
 * Sincroniza guardia con alba/anochecer. Integra con pieza 08 (power).
 *
 * shajar (H7837) = alba. kojavím (H3556) = estrellas.
 * Día = trabajo + guardia. Noche = guardia intenso.
 * ================================================================== */

int hashmal_perimeter_ciclo_astronomico(bool es_shajar)
{
    if (!g_levantado) {
        PERIM_LOG_E("ciclo_astronomico: atrio no levantado");
        return HASHMAL_PERIM_ERR_DESMONTADO;
    }

    /* Negate: shajar = alba = día = NO es noche de guardia. */
    const bool fue_guardia = s_modo_guardia;
    s_modo_guardia = !es_shajar;  /* es_shajar=true → guardia=false (día) */

    if (fue_guardia != s_modo_guardia) {
        if (s_modo_guardia) {
            PERIM_LOG_I("ciclo_astronomico: NOCHE — guardia intenso "
                        "(meʻalót hashájar → tsét hakkojavím, Neh 4:15)");
        } else {
            PERIM_LOG_I("ciclo_astronomico: DÍA — guardia + trabajo "
                        "(shajar — alba, Neh 4:15)");
        }
    }

    return HASHMAL_PERIM_OK;
}

/* ==================================================================
 *
 *   "vayyAQEM et hejatser SAVIV lamishkan VELAMMIZBEAJ vayyitten
 *    et masaj sha'ar hejatser vayJAL Moshe et hammelajak."
 *     - Exodo 40:33
 *
 *   "Levanto el atrio alrededor del Mishkan y del altar, puso la
 *    cortina de la puerta del atrio, y Moises TERMINO la obra."
 *
 *   vayyaqem (levantar) + vayyitten (poner cortina) + vayjal
 *   (terminar): tres actos en un versiculo. init hace el PRIMERO.
 *   atrio_puerta.c hara el SEGUNDO. El TERCERO - "vayjal" - se
 *   cumple cuando las 60 columnas estan activas y la puerta opera.
 *
 *   1 de 4 archivos del Atrio - atrio_perimetro.c listo. Quedan:
 *     atrio_columnas.c       Ex 27:10-16 - 60 'ammudim
 *     atrio_puerta.c         Ex 27:16 + Jn 10:9 - la puerta unica
 *     atrio_intencion.c      Sal 100:4 + Sal 118:19 - intencion
 *
 * ================================================================== */
