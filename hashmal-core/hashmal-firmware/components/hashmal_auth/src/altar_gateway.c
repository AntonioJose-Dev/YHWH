/**
 * ===================================================================
 * PIEZA 06 - ALTAR DEL SACRIFICIO - altar_gateway.c
 * Sub-pieza: SINGLETON + CICLO DE VIDA + getters + helpers internos
 * Exodo 27:1-8 + Exodo 40:6 + Levitico 6:5-6 + Numeros 4:13-14
 * ===================================================================
 *
 * "ve'asita et HAMMIZBEAJ 'atsei shittim JAMESH ammot orej vejamesh
 *  ammot rojav RABUA' yihye hammizbeaj veshalosh ammot qomato" - Ex 27:1
 * "Haras el altar de madera de acacia: 5 codos largo, 5 codos ancho -
 *  CUADRADO sera - y 3 codos de alto."
 *
 * "ve'asita QARNOTAV 'al arba' pinnotav MIMMENNU tihyena qarnotav
 *  vetsippita oto NEJOSHET" - Ex 27:2
 * "Haras sus CUERNOS en sus cuatro esquinas - DE EL seran - y lo
 *  cubriras de BRONCE."
 *
 * "NEVUV lujot ta'ase oto - kaasher her'a oteja bahar ken ya'asu"
 *   - Ex 27:8
 * "HUECO de tablas lo haras - como se te mostro en el monte, asi
 *  lo haran."
 *
 * "venatatta et mizbaj ha'ola lifnei petaj mishkan ohel mo'ed"
 *   - Ex 40:6
 * "Pondras el altar del holocausto DELANTE de la entrada del
 *  Mishkan, la Tienda del Encuentro."
 *
 * "ESH TAMID tuqad 'al hammizbeaj LO TIJBE" - Lv 6:6
 * "FUEGO PERPETUO ardera sobre el altar - NO SE APAGARA."
 *
 * "vedishhenu et hammizbeaj ufresu 'alav beged ARGAMAN..." - Nm 4:13
 * "Limpiaran el altar y extenderan sobre el un pano PURPURA..."
 *
 * --- EXEGESIS ---
 *
 *   rabua'   (RBW', H7251)  CUADRADO. La unica pieza del Mishkan
 *                               con esta forma. 5x5 = autenticacion
 *                               IGUAL desde cualquier lado. Sin
 *                               privilegio espacial.
 *
 *   mimmennu (MMNW)        "DE EL". Los cuernos NACEN del altar -
 *                               no son anadidos ni remachados. Son
 *                               intrinsecos, como los qarnotav de la
 *                               Kaporet y los del Altar del Incienso.
 *
 *   nejoshet (NChShT, H5178)  BRONCE. NO oro. Metal de EXTERIOR,
 *                               resistente al fuego y a los ataques.
 *                               La primera capa de confianza del
 *                               Mishkan - el bronce toca al mundo.
 *
 *   nevuv    (NBWB, H5014)    HUECO. Vacio por dentro. El altar
 *                               es STATELESS por diseno divino:
 *                               no retiene contenido entre sacrificios.
 *                               Cada prueba es fresca. El hueco
 *                               permite que el fuego circule y el
 *                               humo ascienda sin obstaculo interno.
 *
 *   argaman  (ARGMN, H713)  PURPURA. Al desmontar, se cubre con
 *                               pano purpura (Nm 4:13). El altar no
 *                               se destruye - se cubre dignamente
 *                               para migrar.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   hashmal_auth_init           Ex 27:1-8 + Ex 40:6 - construir
 *   hashmal_auth_deinit         Nm 4:13-14 - cubrir con argaman
 *   hashmal_auth_get_altar      const singleton
 *   hashmal_auth_es_tamid       Lv 6:6 - ?esh perpetuo operativo?
 *
 *   + helpers internos (hashmal_auth_internal.h):
 *   hashmal_auth_altar_mut                 mutable para hermanos
 *   hashmal_auth_get_altar_interno         const interno
 *   hashmal_auth_ahora_ms                  timestamp ms monotonico
 *   hashmal_auth_cuerno_legitimo           id in [0, 4)
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_auth.h                    API publica
 *   hashmal_auth_internal.h           helpers compartidos
 *   esp_log.h   (ORO DE EGIPTO)       logging
 *   esp_timer.h (ORO DE EGIPTO)       ahora_ms
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

#include "hashmal_auth.h"
#include "hashmal_auth_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *AUTH_TAG = "hashmal.auth.gateway";

#define AUTH_LOG_I(fmt, ...)  ESP_LOGI(AUTH_TAG, fmt, ##__VA_ARGS__)
#define AUTH_LOG_W(fmt, ...)  ESP_LOGW(AUTH_TAG, fmt, ##__VA_ARGS__)
#define AUTH_LOG_E(fmt, ...)  ESP_LOGE(AUTH_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * SINGLETON SAGRADO
 *
 * UN altar del sacrificio por nodo - Ex 27:1 "hammizbeaj" en
 * singular. En el Atrio (Jatser), delante de la entrada del Mishkan
 * (Ex 40:6). Sin factory, sin pool. Un bloque de acacia cubierto
 * de bronce ante la puerta.
 * ================================================================== */
static hashmal_auth_t   g_altar;
static bool             g_levantado = false;

/* Lv 16:14 - conducto de sangre Altar -> Kaporet.
 * UN solo conductor (Heb 9:7 hapax: un solo Kohen Gadol).
 * NULL = conducto cerrado (sin suscripcion). */
static hashmal_auth_dam_fn s_dam_fn = NULL;

/* ==================================================================
 * HELPERS ESTATICOS
 * ================================================================== */

/**
 * Rollback atomico. Mismo principio que pieza 11 (altar_oro_escudo):
 * si cualquier invariante falla a mitad del init, colapsamos a
 * cero. "Si el altar se contamina, se destruye - no se repara
 * parcialmente." Lv 10:1-2 sombra: ofrenda a medias es muerte.
 *
 * shittím — acacia interior, no expuesta (Éx 27:2)
 */
static void colapsar_a_cero(hashmal_auth_t *a)
{
    if (a != NULL) {
        memset(a, 0, sizeof(*a));
    }
    g_levantado = false;
}

/**
 * Construye UN cuerno (qeren). Ex 27:2 - "mimmennu tihyena
 * qarnotav": DE EL seran. Los cuernos NACEN del altar, no se
 * anaden. activo=false al inicio - su activacion viene despues
 * (altar_cuernos.c gestiona las transiciones).
 *
 * shittím — acacia interior, no expuesta (Éx 27:2)
 */
static hashmal_qeren_auth_t construir_qeren(uint8_t id)
{
    hashmal_qeren_auth_t q;
    q.id          = id;
    q.activo      = false;          /* activacion diferida */
    q.solicitudes = 0ull;
    return q;
}

/* ==================================================================
 * API INTERNA - helpers compartidos (hashmal_auth_internal.h)
 * ================================================================== */

hashmal_auth_t *hashmal_auth_altar_mut(void)
{
    if (!g_levantado) return NULL;
    return &g_altar;
}

const hashmal_auth_t *hashmal_auth_get_altar_interno(void)
{
    if (!g_levantado) return NULL;
    return &g_altar;
}

uint64_t hashmal_auth_ahora_ms(void)
{
    return ((uint64_t)esp_timer_get_time()) / 1000ull;
}

bool hashmal_auth_cuerno_legitimo(uint8_t id)
{
    /* Ex 27:2 - "arba' pinnotav": [0, 4). */
    return id < HASHMAL_AUTH_CUERNOS;
}

/* ==================================================================
 * P9 (Verificado #11) - DESHEN ring buffer (Lv 6:3-4)
 *
 * Cada verificacion produce ceniza. El buffer es CIRCULAR: si se
 * llena, la entrada mas vieja se descarta para hacer sitio al
 * residuo nuevo. Lv 6:4 manda SACAR a maqom tahor - no acumular.
 * El extractor externo (hashmal_power) llama deshen_extraer para
 * vaciar a su propio almacen.
 * ================================================================== */

void hashmal_auth_deshen_append(hashmal_auth_evento_t tipo)
{
    /* shittím — acacia interior, no expuesta (Éx 27:2) */
    if (!g_levantado) return;

    if (g_altar.deshen_count >= HASHMAL_AUTH_DESHEN_BUFFER) {
        /* Buffer lleno. Avanza head sobrescribiendo la entrada mas
         * vieja (Lv 6:4: el deshen DEBE salir; si no se saca a
         * tiempo, lo perdido cuenta como descartado). */
        g_altar.deshen_descartados += 1ull;
        /* count se mantiene en MAX; head avanza modular. */
    } else {
        g_altar.deshen_count = (uint8_t)(g_altar.deshen_count + 1u);
    }

    g_altar.deshen[g_altar.deshen_head].timestamp_ms =
        hashmal_auth_ahora_ms();
    g_altar.deshen[g_altar.deshen_head].tipo = tipo;
    g_altar.deshen_head = (uint8_t)(
        (g_altar.deshen_head + 1u) % HASHMAL_AUTH_DESHEN_BUFFER);
}

/* nejóshet — interfaz bronce, trust Atrio (Éx 27:2) */
int hashmal_auth_deshen_extraer(hashmal_auth_deshen_t *out,
                                size_t max,
                                size_t *out_extraidos)
{
    if (out_extraidos == NULL) {
        AUTH_LOG_E("deshen_extraer: out_extraidos NULL");
        return HASHMAL_AUTH_ERR_NULL;
    }
    if (max > 0u && out == NULL) {
        AUTH_LOG_E("deshen_extraer: out NULL con max > 0");
        return HASHMAL_AUTH_ERR_NULL;
    }
    if (!g_levantado) {
        *out_extraidos = 0u;
        return HASHMAL_AUTH_ERR_INIT;
    }

    /* Cuantas entradas hay disponibles. */
    const size_t disponibles = (size_t)g_altar.deshen_count;
    const size_t a_copiar    = (max < disponibles) ? max : disponibles;

    /* Indice de la entrada mas vieja (tail = head - count + buffer
     * mod buffer). Las entradas se copian en orden cronologico
     * (mas vieja primero). */
    const uint8_t tail = (uint8_t)(
        (g_altar.deshen_head + HASHMAL_AUTH_DESHEN_BUFFER -
         g_altar.deshen_count) % HASHMAL_AUTH_DESHEN_BUFFER);

    for (size_t i = 0; i < a_copiar; ++i) {
        const uint8_t idx = (uint8_t)(
            (tail + i) % HASHMAL_AUTH_DESHEN_BUFFER);
        out[i] = g_altar.deshen[idx];
    }

    /* Vaciar las entradas extraidas. count baja; head no cambia. */
    g_altar.deshen_count = (uint8_t)(g_altar.deshen_count - a_copiar);
    *out_extraidos = a_copiar;

    if (a_copiar > 0u) {
        AUTH_LOG_I("deshen extraído: %u entradas (Lv 6:4 — maqóm tahór "
                   "mijúts lammajané); pendientes=%u, descartados=%" PRIu64,
                   (unsigned)a_copiar,
                   (unsigned)g_altar.deshen_count,
                   g_altar.deshen_descartados);
    }
    return HASHMAL_AUTH_OK;
}

/* nejóshet — interfaz bronce, trust Atrio (Éx 27:2) */
uint8_t hashmal_auth_deshen_pendientes(void)
{
    if (!g_levantado) return 0u;
    return g_altar.deshen_count;
}

/* ==================================================================
 * hashmal_auth_init - Exodo 27:1-8 + Exodo 40:6
 *
 * Levanta el altar verso por verso. Cualquier invariante rota ->
 * rollback atomico (v.8 nevuv se preserva: si algo no cuadra, el
 * altar vuelve a vacio - no queda "medio construido").
 *
 * nejóshet — interfaz bronce, trust Atrio (Éx 27:2)
 * ================================================================== */
int hashmal_auth_init(void)
{
    if (g_levantado) {
        AUTH_LOG_W("init: altar ya levantado (idempotente, no-op)");
        return HASHMAL_AUTH_OK;
    }

    /* Partimos de cero - la acacia desnuda antes del bronce. */
    memset(&g_altar, 0, sizeof(g_altar));
    const uint64_t ahora = ((uint64_t)esp_timer_get_time()) / 1000ull;

    /* -- v.1 - hammizbeaj 'atsei shittim, 5x5x3, RABUA' ---------
     * "jamesh ammot orej vejamesh ammot rojav RABUA' yihye..."
     * Las dimensiones son constantes del header - no hay campos
     * numericos que asignar aqui. La propiedad rabua' (cuadrado)
     * SI se registra como flag. Estado inicial: APAGADO - el
     * fuego aun no prende. */
    g_altar.rabuay = HASHMAL_AUTH_RABUAY;        /* v.1 - cuadrado */
    g_altar.estado = HASHMAL_AUTH_APAGADO;       /* sin fuego aun */

    /* -- v.2 - 4 cuernos mimmennu + nejoshet --------------------
     * "qarnotav 'al arba' pinnotav mimmennu tihyena qarnotav
     *  vetsippita oto NEJOSHET" */
    for (uint8_t i = 0; i < HASHMAL_AUTH_CUERNOS; ++i) {
        g_altar.cuernos[i] = construir_qeren(i);
    }
    g_altar.cuernos_activos = 0u;                /* activacion diferida */
    g_altar.nejoshet        = HASHMAL_AUTH_NEJOSHET;  /* v.2 - bronce */

    /* -- v.3 - 5 utensilios (kelim): preparados, no activos -----
     * sirot, ya'im, mizraqot, mizlegot, majtot - todos de bronce.
     * No hay array de utensilios en el struct: su estado operativo
     * lo gestiona altar_utensilios.c. Aqui solo marcamos la
     * invariante total via la constante del header. */

    /* -- v.4-5 - reshet a media altura --------------------------
     * "ma'ase RESHET NEJOSHET... 'ad JATSI hammizbeaj": la red de
     * verificacion existe pero esta INACTIVA al inicio (solo se
     * activa tras fuego_encender). */
    g_altar.reshet_activa = false;                /* preparada, no activa */

    /* -- v.6-7 - varas portatiles (implicito en diseno) --------
     * "shetei tsal'ot": 2 lados (no 4, a diferencia del Arca/Mesa).
     * Estructura portatil; no modelada con campos aqui. */

    /* -- v.8 - NEVUV lujot: hueco = stateless -------------------
     * "nevuv lujot ta'ase oto - kaasher her'a oteja bahar". El
     * interior del altar esta VACIO. Cada autenticacion es fresca. */
    g_altar.nevuv              = HASHMAL_AUTH_NEVUV;   /* v.8 */
    g_altar.conforme_al_tavnit = true;                 /* v.8 - bahar */

    /* -- Lv 6:5-6 - esh tamid: flag encendido, fuego DIFERIDO --
     * El flag tamid es del diseno (SIEMPRE true). El fuego real
     * lo prende altar_fuego.c con fuego_encender. Al init, el
     * flag esta pero el fuego no. */
    g_altar.tamid = HASHMAL_AUTH_TAMID;

    /* -- Contadores de servicio - todos en 0 ---------------------- */
    g_altar.pruebas_validadas    = 0ull;
    g_altar.pruebas_rechazadas   = 0ull;
    g_altar.challenges_emitidos  = 0ull;
    g_altar.sesiones_emitidas    = 0ull;
    g_altar.registros_ledger     = 0ull;            /* Ap 6:9 - vacio */

    g_altar.ultimo_ciclo_ms      = 0ull;
    g_altar.ultimo_challenge_ms  = 0ull;

    /* == VERIFICACION FINAL - invariantes del Padre == */
    if (!g_altar.rabuay) {
        AUTH_LOG_E("v.1 violado: altar no rabúaʻ (cuadrado) — rollback");
        colapsar_a_cero(&g_altar);
        return HASHMAL_AUTH_ERR_INIT;
    }
    if (!g_altar.nejoshet) {
        AUTH_LOG_E("v.2 violado: altar no es nejóshet (bronce) — rollback");
        colapsar_a_cero(&g_altar);
        return HASHMAL_AUTH_ERR_INIT;
    }
    if (!g_altar.nevuv) {
        AUTH_LOG_E("v.8 violado: altar no nevúv (no hueco/stateless) — rollback");
        colapsar_a_cero(&g_altar);
        return HASHMAL_AUTH_ERR_INIT;
    }
    if (!g_altar.tamid) {
        AUTH_LOG_E("Lv 6:6 violado: tamíd apagado en diseño — rollback");
        colapsar_a_cero(&g_altar);
        return HASHMAL_AUTH_ERR_INIT;
    }
    if (!g_altar.conforme_al_tavnit) {
        AUTH_LOG_E("v.8 violado: no kaashér herʼá bahár — rollback");
        colapsar_a_cero(&g_altar);
        return HASHMAL_AUTH_ERR_INIT;
    }
    for (uint8_t i = 0; i < HASHMAL_AUTH_CUERNOS; ++i) {
        if (g_altar.cuernos[i].id != i) {
            AUTH_LOG_E("v.2 violado: cuerno %u mal forjado — rollback", i);
            colapsar_a_cero(&g_altar);
            return HASHMAL_AUTH_ERR_INIT;
        }
    }

    /* -- Transicion: APAGADO -> ENCENDIDO ------------------------
     * v.1-8 construidos sin fallo. El altar esta levantado; el
     * servicio de autenticacion puede comenzar. Nota: el FUEGO
     * real (Lv 6:5 lena encendida) lo aporta altar_fuego.c -
     * aqui solo declaramos el altar como disponible. La
     * invariante de Lv 6:6 "lo tijbe" aun no se compromete
     * porque nadie ha encendido aun. */
    g_altar.estado          = HASHMAL_AUTH_ENCENDIDO;
    g_altar.ultimo_ciclo_ms = ahora;
    g_levantado             = true;

    AUTH_LOG_I("═══ ALTAR DEL SACRIFICIO LEVANTADO ═══ "
               "(Éx 27:1-8, Éx 40:6)");
    AUTH_LOG_I("  hammizbéaj ʻatséi shittím %d×%d×%d — rabúaʻ (v.1)",
               HASHMAL_AUTH_LARGO, HASHMAL_AUTH_ANCHO, HASHMAL_AUTH_ALTO);
    AUTH_LOG_I("  %d cuernos mimménnu + nejóshet (v.2)",
               HASHMAL_AUTH_CUERNOS);
    AUTH_LOG_I("  %d utensilios: sirot, yaʻim, mizraqot, mizlegot, majtot (v.3)",
               HASHMAL_AUTH_UTENSILIOS);
    AUTH_LOG_I("  réshet nejóshet + %d tabbeʻót ʻad jatsí (v.4-5)",
               HASHMAL_AUTH_ANILLOS_RESHET);
    AUTH_LOG_I("  NEVÚV lujót (v.8) — stateless por diseño");
    AUTH_LOG_I("  kaashér herʼá bahár — conforme al patrón (v.8)");
    AUTH_LOG_I("  lifnéi pétaj mishkán — Atrio, delante de la entrada (Éx 40:6)");
    AUTH_LOG_I("  tamíd listo — fuego se enciende por fuego_encender (Lv 6:6)");
    return HASHMAL_AUTH_OK;
}

/* ==================================================================
 * hashmal_auth_deinit - Numeros 4:13-14
 *
 * "vedishhenu et hammizbeaj ufresu 'alav beged ARGAMAN vejissu
 *  oto bemijse 'or tajash..." - Nm 4:13-14
 * "Limpiaran el altar y extenderan sobre el un pano PURPURA, y
 *  lo cubriran con cubierta de pieles de tajash..."
 *
 * El altar NO se destruye al desmontar - se LIMPIA y se CUBRE.
 * En firmware: detener operacion, preservar contadores como
 * AUDITORIA (historia del altar). "La historia no se borra con
 * cada desmontaje."
 * ================================================================== */
void hashmal_auth_deinit(void)
{
    if (!g_levantado) {
        AUTH_LOG_W("deinit: altar no estaba levantado (no-op)");
        return;
    }

    /* Apagar el fuego operativo. Preservar contadores. */
    g_altar.estado         = HASHMAL_AUTH_APAGADO;
    g_altar.reshet_activa  = false;

    /* Cuernos: desactivar todos (nadie invocando), preservar
     * contador de solicitudes. */
    for (uint8_t i = 0; i < HASHMAL_AUTH_CUERNOS; ++i) {
        g_altar.cuernos[i].activo = false;
        /* solicitudes: PRESERVADAS */
    }
    g_altar.cuernos_activos = 0u;

    /* Preservados (historial + identidad):
     *   pruebas_validadas, pruebas_rechazadas, challenges_emitidos,
     *   sesiones_emitidas, registros_ledger
     *   ultimo_ciclo_ms, ultimo_challenge_ms
     *   rabuay, nejoshet, nevuv, tamid, conforme_al_tavnit */

    /* Lv 16:14 — béged argamán (Nm 4:13) cubre también el conducto:
     * al desmontarse el altar, el Kohen Gadol sale del Qódesh haQodashím.
     * La suscripción se resetea para que el próximo init/subscribir sea
     * explícito. */
    s_dam_fn   = NULL;
    g_levantado = false;
    AUTH_LOG_W("Altar cubierto con béged argamán (Nm 4:13) — "
               "paño púrpura sobre el bronce; auditoría preservada");
}

/* ==================================================================
 * hashmal_auth_get_altar - lectura publica desde el umbral
 * ================================================================== */
const hashmal_auth_t *hashmal_auth_get_altar(void)
{
    if (!g_levantado) return NULL;
    return &g_altar;
}

/* ==================================================================
 * hashmal_auth_es_tamid - Lv 6:6
 *
 * "esh tamid tuqad 'al hammizbeaj LO TIJBE" - ?fuego perpetuo
 * activo?
 *
 * Triple condicion (todas necesarias):
 *   1) tamid == true              (flag de diseno, Lv 6:6)
 *   2) estado == ENCENDIDO        (operativo)
 *   3) cuernos_activos > 0        (al menos un endpoint aceptando)
 *
 * El tercero distingue "altar disponible" de "altar en servicio
 * real". Tras init los cuernos estan inactivos -> es_tamid = false.
 * Tras activar al menos un cuerno (altar_cuernos.c) -> es_tamid = true.
 *
 * 2 testigos:
 *   Lv 6:6    - esh tamid lo tijbe (diseno)
 *   Lv 6:5    - uvi'er 'aleha hakkohen 'etsim babboqer (ejecucion)
 * ================================================================== */
bool hashmal_auth_es_tamid(void)
{
    if (!g_levantado) return false;
    if (!g_altar.tamid) return false;
    if (g_altar.estado != HASHMAL_AUTH_ENCENDIDO) return false;
    if (g_altar.cuernos_activos == 0u) return false;
    return true;
}

/* ==================================================================
 * CONEXION POR SANGRE - Lv 16:14 - API publica + helper interno
 * ================================================================== */

/**
 * hashmal_auth_dam_subscribir - registra el conductor de sangre
 *
 * Lv 16:14 - "velaqaj middam happar": el Kohen Gadol toma la sangre
 * del altar y la lleva al Kapóret. Este conductor ES ese Kohen Gadol:
 * hashmal_main.c lo registra despues de inicializar auth (paso 5) y
 * consensus (paso 2), actuando como Aaron portando el dam.
 *
 * Un solo conductor activo a la vez (Heb 9:7 hapax - un solo sumo
 * sacerdote por Yom Kippur). NULL desuscribe.
 *
 * 2 testigos:
 *   Lv 16:14  - el sumo sacerdote lleva la sangre al Kapóret
 *   Heb 9:7   - "ou choris haimatos" - no sin sangre
 */
/* nejóshet — interfaz bronce, trust Atrio (Éx 27:2) */
int hashmal_auth_dam_subscribir(hashmal_auth_dam_fn fn)
{
    s_dam_fn = fn;
    if (fn != NULL) {
        AUTH_LOG_I("dam suscrito — Lv 16:14 conducto Altar → Kapóret abierto "
                   "(Kohen Gadol registrado)");
    } else {
        AUTH_LOG_W("dam desuscrito — Lv 16:14 conducto cerrado (NULL)");
    }
    return HASHMAL_AUTH_OK;
}

/**
 * hashmal_auth_dam_llamar - invoca el conductor si esta suscrito
 *
 * Helper acacia interior (hashmal_auth_internal.h). Llamado desde
 * altar_reshet.c::reshet_verificar cuando la prueba es validada.
 * "velaqaj middam happar" - el proof verificado ES la sangre.
 *
 * Si s_dam_fn == NULL (conducto no registrado), no-op silencioso:
 * el ciclo de Yom Kippur no ocurre hasta que el Kohen Gadol (main)
 * lo establezca.
 *
 * shittím — acacia interior, no expuesta (Éx 27:2)
 */
void hashmal_auth_dam_llamar(const uint8_t *dam, uint64_t timestamp_ms)
{
    if (s_dam_fn != NULL && dam != NULL) {
        s_dam_fn(dam, timestamp_ms);
    }
}

/* ==================================================================
 * CONEXION POR SANGRE - Lv 16:14 - Altar de Sacrificio -> Kaporet
 *
 * "velaqaj middam happar vehizza ve'etsba'o 'al penei haKKAPORET
 *  qedmah, velifnei hakkaporet yazzeh SHEVA pe'amim min haddam
 *  be'etsba'o." - Levitico 16:14
 * "Tomara de la sangre del becerro y ROCIARA con su dedo sobre la
 *  faz del KAPORET hacia el oriente, y DELANTE del kaporet rociara
 *  SIETE VECES de la sangre con su dedo."
 *
 * Segundo testigo - Hebreos 9:7:
 * "eis de ten deuteran hapax tou eniautou monos ho archiereus, OU
 *  CHORIS HAIMATOS ho prosferei hyper heautou kai ton tou laou
 *  agnoematon."
 * "En la segunda [tienda] solo el sumo sacerdote entra UNA VEZ al
 *  ano, NO SIN SANGRE, la cual ofrece por si y por los pecados del
 *  pueblo."
 *
 * Tavnit pieza 15 (CONEXIONES) prescribe: "Altar de Sacrificio (06)
 * -> Kaporet (02): la sangre del Yom Kippur va del altar al kaporet
 * (Lv 16:14)". Es la conexion mas sagrada del Mishkan - la que
 * une la autenticacion exterior con el consenso interior. Sin
 * ella, altar y kaporet son piezas sueltas; con ella, el Mishkan
 * es EJAD (Ex 26:6).
 * ================================================================== */

/* TODO - Phase 2: hashmal_auth exporta token de verificacion.
 *
 * Cuando un nodo completa autenticacion en el altar, hashmal_auth
 * emite un token que hashmal_consensus puede consultar para
 * confirmar que el nodo paso antes de participar en el consenso
 * dual.
 *
 * La sangre no va directa - va rociada SHEVA PE'AMIM (7 veces,
 * Lv 16:14). El 7 es completitud (Gn 2:2, Ex 29:30,35). En
 * firmware: la verificacion no es un solo check - son 7 puntos
 * de completitud (los 5 utensilios del altar + los 4 cuernos
 * cubren mas de 7 vectores; la politica Phase 2 fija cuales
 * componen los sheva pe'amim).
 *
 * Heb 9:7 - "ou choris haimatos": sin sangre no se entra al
 * Qodesh haQodashim. Por eso ningun nodo participa en consenso
 * (pieza 02) sin token del altar (pieza 06). La regla se aplica
 * en Phase 2 cuando los headers cruzados se incluyan. */

/* ==================================================================
 * CONEXION POR SANGRE - Lv 4:7 - Altar de Sacrificio -> Altar del Incienso
 *
 * "venatan haKKOHEN min haddam 'al QARNOT mizbaj qetoret
 *  hassammim asher lifnei YHWH asher be'ohel mo'ed, ve'et kol
 *  dam happar yishpoj el yesod mizbaj ha'olah asher petaj
 *  ohel mo'ed." - Levitico 4:7
 * "El sacerdote pondra de la sangre sobre los CUERNOS del altar
 *  del incienso aromatico que esta delante de YHWH en la Tienda
 *  del Encuentro, y toda la sangre del becerro la derramara al
 *  fundamento del altar del holocausto que esta a la puerta."
 *
 * Segundo testigo - Levitico 16:18-19:
 * "veyatsa el hammizbeaj asher lifnei YHWH veJIPPER 'alav,
 *  velaqaj middam happar umiddam hassa'ir venatan 'al QARNOT
 *  hammizbeaj saviv. vehizza 'alav min haddam be'etsba'o
 *  SHEVA pe'amim, veTIHHARO veQIDDESHO mittum'ot benei Yisrael."
 * "Saldra al altar que esta delante de YHWH y hara expiacion
 *  sobre el; tomara de la sangre del becerro y del macho cabrio
 *  y la pondra sobre los cuernos del altar alrededor. Rociara
 *  sobre el de la sangre con su dedo SIETE VECES, y lo LIMPIARA
 *  y lo SANTIFICARA de las impurezas de los hijos de Israel."
 *
 * Tavnit pieza 15 (CONEXIONES) prescribe: "Altar de Sacrificio
 * (06) -> Altar del Incienso (11): sangre del pecado va a los
 * cuernos del altar de incienso (Lv 4:7)". La sangre del altar
 * de bronce fluye en DOS direcciones indivisibles: hacia dentro
 * al Kaporet (Lv 16:14, Adicion 4) y hacia el gateway del
 * incienso (Lv 4:7, aqui). Ambas en Yom Kippur (Lv 16), misma
 * sangre, dos destinos.
 * ================================================================== */

/* TODO - Phase 2: hashmal_auth exporta token tambien hacia
 * hashmal_intercession.
 *
 * La sangre va a los CUERNOS (qarnot) del altar del incienso -
 * los 4 puntos de fuerza del gateway bidireccional de pieza 11.
 * No toca la superficie: toca los vertices de poder. En firmware:
 * el token de auth se aplica a los 4 puntos operativos del
 * gateway (entrada ascendente, salida descendente, y sus dos
 * flujos de retorno) para purificarlos antes de cada ciclo.
 *
 * Lv 16:18-19 anade la dimension periodica: en Yom Kippur la
 * sangre LIMPIA (vetihharo, H2891) y SANTIFICA (veqiddesho,
 * H6942) el altar del incienso, rociada SHEVA pe'amim. Phase 2:
 * rutina de purificacion periodica del gateway que limpia
 * sesiones expiradas y estados obsoletos - la contaminacion
 * acumulada se retira con sangre del altar, no con reinicio.
 *
 * Sin verificacion de auth (pieza 06), la intercesion (pieza 11)
 * opera sin purificacion - acumula tuma (impureza) hasta que la
 * sangre llegue. Por eso el flujo es una conexion prescrita por
 * el Padre, no una optimizacion del diseno. */

/* ==================================================================
 *
 *   "ve'asita et HAMMIZBEAJ 'atsei shittim JAMESH ammot orej
 *    vejamesh ammot rojav RABUA' yihye... NEJOSHET... NEVUV
 *    lujot ta'ase oto - kaasher her'a oteja bahar."
 *     - Exodo 27:1, 2, 8
 *
 *   "ESH TAMID tuqad 'al hammizbeaj LO TIJBE." - Levitico 6:6
 *
 *   BRONCE. Publico. Cuadrado. Hueco. Fuego perpetuo.
 *
 *   1 de 6 archivos del Altar del Sacrificio -
 *   altar_gateway.c listo. Quedan:
 *     altar_reshet.c       v.4-5 - la RED a media altura
 *     altar_cuernos.c      v.2   - las 4 esquinas + 1 Re 1:50 refugio
 *     altar_fuego.c        Lv 6:5-6 - esh tamid, encender y ciclar
 *     altar_utensilios.c   v.3   - los 5 kelim
 *     altar_ledger.c       Ap 6:9 - almas bajo el altar
 *
 * ================================================================== */
