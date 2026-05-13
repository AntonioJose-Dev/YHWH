/*
 * ===================================================================
 * PIEZA 07 - EL ATRIO - atrio_shaar.c
 * Añadidura 22: SHA'AR HA'ATRIO - Portal Cautivo (puerta WiFi)
 * Exodo 27:16 + Exodo 26:36 + Exodo 38:18 + Salmo 100:4
 *              + Isaias 60:11 + Juan 10:9
 * ===================================================================
 *
 * "uleSHA'AR HEJATSER MASAKH 'ESRIM AMMAH tekhelet veargaman
 *  vetola'at shani veshesh moshzar ma'aseh ROQEM; 'AMMUDEIHEM
 *  ARBA'AH veadneihem ARBA'AH." - Exodo 27:16
 * "Para la PUERTA del atrio, una CORTINA de 20 codos de azul,
 *  purpura, carmesi y lino torzido, OBRA DE BORDADOR; sus COLUMNAS
 *  CUATRO y sus basas CUATRO."
 *
 * "ve'asita masakh lefetaj ha'OHEL tekhelet veargaman vetola'at
 *  shani veshesh moshzar MA'ASEH ROQEM." - Exodo 26:36
 * "Haras una CORTINA para la entrada de la TIENDA... obra de
 *  BORDADOR." (la entrada NO es tosca - es bordada con arte)
 *
 * "umasakh sha'ar hejatser MA'ASEH ROQEM tekhelet veargaman..."
 *   - Exodo 38:18
 * "La cortina de la puerta del atrio era OBRA DE BORDADOR..."
 *
 * "BO'U SHE'ARAV BETODAH JATSEROTAV BITEHILLAH hodu lo barakhu
 *  shemo." - Salmo 100:4
 * "ENTRAD por sus PUERTAS con accion de gracias, por sus ATRIOS
 *  con alabanza."
 *
 * "ufittjau SHE'ARAYIKH TAMID yomam valaylah lo yissageru lehavi
 *  elayikh jeil goyim." - Isaias 60:11
 * "Tus PUERTAS estaran ABIERTAS CONTINUAMENTE, dia y noche no se
 *  cerraran, para que se traiga a ti la riqueza de las naciones."
 *
 * "ego eimi he THYRA; di emou EAN TIS EISELTHE SOTHESETAI kai
 *  eiseleusetai kai exeleusetai kai nomen heuresei." - Juan 10:9
 * "Yo soy la PUERTA; el que por mi entrare, sera salvo, y entrara
 *  y saldra y hallara pasto."
 *
 * --- EXEGESIS OPERATIVA ---
 *
 *   sha'ar (H8179)           PUERTA. La unica entrada autorizada.
 *                             Ex 27:16 - 20 codos de ancho:
 *                             generosa, no estrecha. Una sola, no
 *                             multiples; 4 columnas, no 3 ni 5.
 *
 *   masakh (H4539)           CORTINA. Es separacion VISIBLE pero
 *                             no rigida. La puerta no es muralla -
 *                             se puede cruzar.
 *
 *   ma'aseh roqem (H4639+H7551) "OBRA DE BORDADOR." La interfaz se
 *                             trabaja con arte. Ex 26:36 + 27:16 +
 *                             38:18 lo repiten 3 veces - no es
 *                             detalle. La PUERTA debe ser hermosa.
 *
 *   ammudeihem arba'ah (H5982+H702) "SUS COLUMNAS CUATRO." La
 *                             cortina se sostiene en 4 puntos -
 *                             ni 3 (insuficiente) ni 5 (excesivo).
 *                             Cuatro columnas = cuatro puntos de
 *                             verificacion en el firmware.
 *
 *   tamid (H8548)            "CONTINUAMENTE." Is 60:11 - el portal
 *                             no se cierra en horario fijo. Solo
 *                             se PAUSA por causa especifica
 *                             (Shabat, emergencia).
 *
 *   bo'u betodah (H8426)     "ENTRAD CON GRATITUD." Sal 100:4 - la
 *                             entrada no es transaccion, es gozo.
 *                             En el firmware: la conexion exitosa
 *                             se loguea positivamente.
 *
 *   he thyra (G2374)         "LA PUERTA." Jn 10:9 - cumplimiento NT.
 *                             Una sola puerta por la cual se entra
 *                             y se sale (eiseleusetai kai
 *                             exeleusetai). No es restriccion - es
 *                             SALVACION (sothesetai).
 *
 * --- 4 COLUMNAS DE VERIFICACION (Ex 27:16) ---
 *
 *   IDENTIDAD     - quien eres (MAC, hostname, sub)
 *   AUTENTICACION - tienes la contrasena (PSK / oauth / etc.)
 *   AUTORIZACION  - rol permite acceso (Pieza 09)
 *   INTEGRIDAD    - sesion no comprometida (TLS / handshake limpio)
 *
 * Las CUATRO deben pasar. Si falta UNA, la cortina no se sostiene -
 * el visitante no entra. Es el patron biblico de la puerta exacta.
 *
 * --- COEXISTENCIA EN PIEZA 07 ---
 *
 *   Miqlat   (Añ 05) - refugio para nodo en peligro
 *   Orchim   (Añ 21) - hospitalidad para desconocido
 *   Sha'ar   (Añ 22) - puerta fisica WiFi
 *
 * Tres mecanismos en la frontera del nodo. Sha'ar es la MATERIA
 * (la puerta misma); Orchim es el PROTOCOLO de quien entra sin
 * cuenta; Miqlat es la EXCEPCION para el que viene huyendo.
 *
 * --- FASE 1 vs FASE 2 ---
 *
 * Fase 1 (este archivo): logica de estado pura. No crea WiFi AP,
 * no sirve HTTP, no hace bridge LoRa. Solo registra sesiones y
 * verifica columnas. Stub fiable para tests y composicion.
 *
 * Fase 2 (cuando flasheemos T-Decks): wirea con esp_wifi (modo AP),
 * esp_http_server (portal cautivo HTML/CSS/JS), bridge a comm
 * (mensajes WiFi <-> LoRa). Esta API actual sera el frontend de
 * estado de toda esa infraestructura.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_perimeter.h        API publica
 *   hashmal_perim_internal.h   ahora_ms
 *   esp_log.h (ORO EGIPTO)     SHR_LOG_*
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
static const char *SHR_TAG = "hashmal.atrio.shaar";

#define SHR_LOG_I(fmt, ...)  ESP_LOGI(SHR_TAG, fmt, ##__VA_ARGS__)
#define SHR_LOG_W(fmt, ...)  ESP_LOGW(SHR_TAG, fmt, ##__VA_ARGS__)
#define SHR_LOG_E(fmt, ...)  ESP_LOGE(SHR_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * Estado global + tabla de sesiones
 *
 * Verificado #04 — añadidos:
 *   g_bases[]    P2 — 4 ʼadaním nejóshet (criterios externos)
 *   g_stealth    P6 — leʻummat (Éx 38:18, default true)
 * ================================================================== */
static hashmal_shaar_estado_t  g_estado            = HASHMAL_SHAAR_ESTADO_CERRADO;
static hashmal_shaar_sesion_t  g_sesiones[HASHMAL_SHAAR_SESIONES_MAX];
static uint32_t                g_conexiones_totales = 0;

static hashmal_shaar_base_t    g_bases[HASHMAL_SHAAR_COLUMNAS]; /* configurada=false */
static bool                    g_stealth            = true;     /* P6 default */

/* ==================================================================
 * Helpers estaticos
 * ================================================================== */

static const char *nombre_estado_global(hashmal_shaar_estado_t e)
{
    switch (e) {
    case HASHMAL_SHAAR_ESTADO_ABIERTO: return "ABIERTO (Is 60:11 tamid)";
    case HASHMAL_SHAAR_ESTADO_PAUSADO: return "PAUSADO (Shabat/emergencia)";
    case HASHMAL_SHAAR_ESTADO_CERRADO:
    default:                            return "CERRADO";
    }
}

static const char *nombre_estado_sesion(hashmal_shaar_sesion_estado_t e)
{
    switch (e) {
    case HASHMAL_SHAAR_SESION_CONECTADO:    return "CONECTADO";
    case HASHMAL_SHAAR_SESION_VERIFICADO:   return "VERIFICADO";
    case HASHMAL_SHAAR_SESION_ACTIVO:       return "ACTIVO (Jn 10:9 sothesetai)";
    case HASHMAL_SHAAR_SESION_DESCONECTADO: return "DESCONECTADO";
    case HASHMAL_SHAAR_SESION_VACIA:
    default:                                 return "VACIA";
    }
}

static const char *nombre_columna(hashmal_shaar_columna_t c)
{
    switch (c) {
    case HASHMAL_SHAAR_COL_IDENTIDAD:     return "IDENTIDAD";
    case HASHMAL_SHAAR_COL_AUTENTICACION: return "AUTENTICACION";
    case HASHMAL_SHAAR_COL_AUTORIZACION:  return "AUTORIZACION";
    case HASHMAL_SHAAR_COL_INTEGRIDAD:    return "INTEGRIDAD";
    default:                               return "<columna desconocida>";
    }
}

static bool slot_legitimo(uint8_t slot)
{
    return slot < HASHMAL_SHAAR_SESIONES_MAX;
}

static bool columna_legitima(hashmal_shaar_columna_t c)
{
    return (unsigned)c < HASHMAL_SHAAR_COLUMNAS;
}

/* Slot libre = VACIA o DESCONECTADO. Prefiere VACIA. */
static int buscar_slot_libre(void)
{
    for (uint8_t i = 0; i < HASHMAL_SHAAR_SESIONES_MAX; ++i) {
        if (g_sesiones[i].estado == HASHMAL_SHAAR_SESION_VACIA) return (int)i;
    }
    for (uint8_t i = 0; i < HASHMAL_SHAAR_SESIONES_MAX; ++i) {
        if (g_sesiones[i].estado == HASHMAL_SHAAR_SESION_DESCONECTADO) return (int)i;
    }
    return -1;
}

/* Cuenta cuantas columnas estan true en la sesion. */
static uint8_t contar_columnas_true(uint8_t slot)
{
    uint8_t c = 0;
    for (uint8_t i = 0; i < HASHMAL_SHAAR_COLUMNAS; ++i) {
        if (g_sesiones[slot].columnas[i]) ++c;
    }
    return c;
}

/* ==================================================================
 * Verificado #04 — helpers para P1 (secuencia) + P5 (3 capas)
 * ================================================================== */

static bool capa_legitima(hashmal_shaar_capa_t c)
{
    return (unsigned)c < HASHMAL_SHAAR_CAPAS_POR_COLUMNA;
}

/* Las 3 capas de la columna están true → la columna está completa. */
static bool columna_completa(uint8_t slot, hashmal_shaar_columna_t col)
{
    for (uint8_t k = 0; k < HASHMAL_SHAAR_CAPAS_POR_COLUMNA; ++k) {
        if (!g_sesiones[slot].capas[(unsigned)col][k]) return false;
    }
    return true;
}

/* P1 — Éx 27:16 ʻammudéihem arbaʻáh: las columnas son secuenciales.
 * No se puede tocar la N hasta que las 0..N-1 estén completas. */
static bool columnas_previas_completas(uint8_t slot,
                                       hashmal_shaar_columna_t col)
{
    for (unsigned i = 0; i < (unsigned)col; ++i) {
        if (!columna_completa(slot, (hashmal_shaar_columna_t)i)) return false;
    }
    return true;
}

/* P9 — Sal 100:4 boʼú sheʻaráv betodáh / jatserótav bitehilláh:
 * tras pasar todas las verificaciones del portal, la sesión se
 * "mueve al jatsér". Phase 1 = stub con TODO Phase 2 — el wiring
 * real (atrio_perimetro / atrio_intencion) se cablea cuando exista
 * caller. La invocación queda registrada en log para auditoría. */
static void mover_a_jatser(uint8_t slot)
{
    SHR_LOG_I("hand-off jatsér: slot %u — boʼú sheʻaráv betodáh / "
              "jatserótav bitehilláh (Sal 100:4) — TODO Phase 2: "
              "registrar visitante en atrio_perimetro", (unsigned)slot);
    /* TODO Phase 2: hashmal_perimeter_visitante_registrar(...)
     * cuando esa API exista. Mientras tanto, el ESTADO ACTIVO ya
     * basta como contrato externo (caller consulta y opera). */
}

/* ==================================================================
 * API - hashmal_perimeter_shaar_abrir (Is 60:11 tamid)
 * ================================================================== */
int hashmal_perimeter_shaar_abrir(void)
{
    if (g_estado == HASHMAL_SHAAR_ESTADO_ABIERTO) {
        return HASHMAL_PERIM_OK;  /* idempotente */
    }
    g_estado = HASHMAL_SHAAR_ESTADO_ABIERTO;
    SHR_LOG_I("sha'ar ABIERTO - upiteju she'arayikh tamid (Is 60:11); "
              "ma'aseh roqem (Ex 27:16)");
    return HASHMAL_PERIM_OK;
}

/* ==================================================================
 * API - hashmal_perimeter_shaar_cerrar
 * ================================================================== */
int hashmal_perimeter_shaar_cerrar(void)
{
    if (g_estado == HASHMAL_SHAAR_ESTADO_CERRADO) {
        return HASHMAL_PERIM_OK;  /* idempotente */
    }

    /* Todas las sesiones activas / pendientes pasan a DESCONECTADO. */
    int64_t ahora = hashmal_perim_ahora_ms();
    uint8_t cerradas = 0;
    for (uint8_t i = 0; i < HASHMAL_SHAAR_SESIONES_MAX; ++i) {
        hashmal_shaar_sesion_estado_t e = g_sesiones[i].estado;
        if (e == HASHMAL_SHAAR_SESION_CONECTADO
         || e == HASHMAL_SHAAR_SESION_VERIFICADO
         || e == HASHMAL_SHAAR_SESION_ACTIVO) {
            g_sesiones[i].estado       = HASHMAL_SHAAR_SESION_DESCONECTADO;
            g_sesiones[i].timestamp_ms = ahora;
            ++cerradas;
        }
    }

    g_estado = HASHMAL_SHAAR_ESTADO_CERRADO;
    SHR_LOG_I("sha'ar CERRADO - %u sesiones desconectadas",
              (unsigned)cerradas);
    return HASHMAL_PERIM_OK;
}

/* ==================================================================
 * API - hashmal_perimeter_shaar_pausar
 * ================================================================== */
int hashmal_perimeter_shaar_pausar(void)
{
    if (g_estado == HASHMAL_SHAAR_ESTADO_PAUSADO) {
        return HASHMAL_PERIM_OK;
    }
    g_estado = HASHMAL_SHAAR_ESTADO_PAUSADO;
    SHR_LOG_I("sha'ar PAUSADO - sesiones existentes se mantienen, "
              "no se aceptan nuevas (Shabat/emergencia)");
    return HASHMAL_PERIM_OK;
}

/* ==================================================================
 * API - hashmal_perimeter_shaar_conectar (Sal 100:4)
 * ================================================================== */
int hashmal_perimeter_shaar_conectar(uint8_t *out_slot)
{
    if (g_estado != HASHMAL_SHAAR_ESTADO_ABIERTO) {
        SHR_LOG_W("conectar: portal no ABIERTO (estado=%s)",
                  nombre_estado_global(g_estado));
        return HASHMAL_PERIM_ERR_SHAAR;
    }

    int slot = buscar_slot_libre();
    if (slot < 0) {
        SHR_LOG_E("conectar: tabla llena (%u slots, terminales no "
                  "purgados)", (unsigned)HASHMAL_SHAAR_SESIONES_MAX);
        return HASHMAL_PERIM_ERR_SHAAR;
    }

    int64_t ahora = hashmal_perim_ahora_ms();
    g_sesiones[slot].estado            = HASHMAL_SHAAR_SESION_CONECTADO;
    g_sesiones[slot].timestamp_ms      = ahora;
    g_sesiones[slot].intent_verificada = false;            /* P8 — sin tsédeq por defecto */
    /* Reset columnas + capas - nada verificado al inicio (P5). */
    for (uint8_t i = 0; i < HASHMAL_SHAAR_COLUMNAS; ++i) {
        g_sesiones[slot].columnas[i] = false;
        for (uint8_t k = 0; k < HASHMAL_SHAAR_CAPAS_POR_COLUMNA; ++k) {
            g_sesiones[slot].capas[i][k] = false;
        }
    }
    g_conexiones_totales += 1u;

    if (out_slot != NULL) *out_slot = (uint8_t)slot;

    SHR_LOG_I("conectar: nueva sesion slot=%d - bo'u she'arav betodah "
              "(Sal 100:4); total %" PRIu32, slot, g_conexiones_totales);
    return HASHMAL_PERIM_OK;
}

/* ==================================================================
 * API - hashmal_perimeter_shaar_verificar_columna (Ex 27:16)
 * ================================================================== */
int hashmal_perimeter_shaar_verificar_columna(uint8_t slot,
                                               hashmal_shaar_columna_t columna)
{
    /* Verificado #04 — convenience wrapper sobre verificar_capa.
     * Marca las 3 capas (P5) en una sola llamada manteniendo el
     * enforcement secuencial (P1). Backward-compatible con la firma
     * pre-Verificado #04. */
    for (uint8_t k = 0; k < HASHMAL_SHAAR_CAPAS_POR_COLUMNA; ++k) {
        int rc = hashmal_perimeter_shaar_verificar_capa(
            slot, columna, (hashmal_shaar_capa_t)k);
        if (rc != HASHMAL_PERIM_OK) return rc;
    }
    return HASHMAL_PERIM_OK;
}

/* ==================================================================
 * Verificado #04 — APIs nuevas (P1, P2, P5, P6, P8)
 * ================================================================== */

int hashmal_perimeter_shaar_verificar_capa(uint8_t slot,
                                           hashmal_shaar_columna_t columna,
                                           hashmal_shaar_capa_t capa)
{
    if (!slot_legitimo(slot))      return HASHMAL_PERIM_ERR_SHAAR;
    if (!columna_legitima(columna)) {
        SHR_LOG_E("verificar_capa: columna %d inválida (Éx 27:16 "
                  "ʻammudéihem arbaʻáh)", (int)columna);
        return HASHMAL_PERIM_ERR_SHAAR;
    }
    if (!capa_legitima(capa)) {
        SHR_LOG_E("verificar_capa: capa %d inválida (Éx 38:17 — 3 niveles)",
                  (int)capa);
        return HASHMAL_PERIM_ERR_SHAAR;
    }

    hashmal_shaar_sesion_estado_t prev = g_sesiones[slot].estado;
    if (prev != HASHMAL_SHAAR_SESION_CONECTADO
     && prev != HASHMAL_SHAAR_SESION_VERIFICADO
     && prev != HASHMAL_SHAAR_SESION_ACTIVO) {
        SHR_LOG_W("verificar_capa: slot %u en %s (no verificable)",
                  (unsigned)slot, nombre_estado_sesion(prev));
        return HASHMAL_PERIM_ERR_SHAAR;
    }

    /* P1 — secuencia: no tocar columna N si 0..N-1 incompletas. */
    if (!columnas_previas_completas(slot, columna)) {
        SHR_LOG_W("verificar_capa: slot %u secuencia rota — columna %s "
                  "antes de completar las previas (P1, Éx 27:16)",
                  (unsigned)slot, nombre_columna(columna));
        return HASHMAL_PERIM_ERR_SHAAR;
    }

    /* Idempotente sobre capa ya marcada. */
    if (g_sesiones[slot].capas[(unsigned)columna][(unsigned)capa]) {
        return HASHMAL_PERIM_OK;
    }

    g_sesiones[slot].capas[(unsigned)columna][(unsigned)capa] = true;

    /* Recompute rollup de la columna. */
    if (columna_completa(slot, columna)) {
        g_sesiones[slot].columnas[(unsigned)columna] = true;
        SHR_LOG_I("verificar_capa: slot %u columna %s COMPLETA "
                  "(3 capas: bronce + plata + plata, Éx 38:17)",
                  (unsigned)slot, nombre_columna(columna));
    }

    /* Transición ACTIVO requiere: 4 columnas + intent_verificada (P8).
     * P9 hand-off (Sal 100:4) ocurre en este punto. */
    if (contar_columnas_true(slot) == HASHMAL_SHAAR_COLUMNAS &&
        g_sesiones[slot].intent_verificada &&
        prev != HASHMAL_SHAAR_SESION_ACTIVO) {
        g_sesiones[slot].estado = HASHMAL_SHAAR_SESION_ACTIVO;
        SHR_LOG_I("slot %u → VERIFICADO → ACTIVO "
                  "(4 columnas + tsédeq intent — Sal 118:19-20; "
                  "Jn 10:9 sothésetai)", (unsigned)slot);
        mover_a_jatser(slot);
    }
    return HASHMAL_PERIM_OK;
}

int hashmal_perimeter_shaar_base_configurar(hashmal_shaar_columna_t columna,
                                            uint32_t id_referencia)
{
    if (!columna_legitima(columna)) return HASHMAL_PERIM_ERR_SHAAR;
    g_bases[(unsigned)columna].configurada   = true;
    g_bases[(unsigned)columna].id_referencia = id_referencia;
    SHR_LOG_I("base configurada: columna %s id_ref=%" PRIu32
              " (Éx 27:16 ʼadan nejóshet)",
              nombre_columna(columna), id_referencia);
    return HASHMAL_PERIM_OK;
}

int hashmal_perimeter_shaar_base_estado(hashmal_shaar_columna_t columna,
                                        hashmal_shaar_base_t *out)
{
    if (out == NULL)                return HASHMAL_PERIM_ERR_NULL;
    if (!columna_legitima(columna)) return HASHMAL_PERIM_ERR_SHAAR;
    *out = g_bases[(unsigned)columna];
    return HASHMAL_PERIM_OK;
}

int hashmal_perimeter_shaar_stealth_set(bool activar)
{
    g_stealth = activar;
    SHR_LOG_I("stealth: %s (Éx 38:18 leʻummat)",
              activar ? "ON — portal indistinguible del muro"
                      : "OFF — portal anunciado (sólo Phase 2/diag)");
    return HASHMAL_PERIM_OK;
}

bool hashmal_perimeter_shaar_stealth_get(void)
{
    return g_stealth;
}

int hashmal_perimeter_shaar_intent_set(uint8_t slot, bool verificada)
{
    if (!slot_legitimo(slot)) return HASHMAL_PERIM_ERR_SHAAR;
    g_sesiones[slot].intent_verificada = verificada;
    SHR_LOG_I("intent_set: slot %u → %s (Sal 118:19-20 shaʻaréi tsédeq)",
              (unsigned)slot, verificada ? "verificada" : "no verificada");
    return HASHMAL_PERIM_OK;
}

int hashmal_perimeter_shaar_sesion_info(uint8_t slot,
                                        hashmal_shaar_sesion_t *out)
{
    if (out == NULL)            return HASHMAL_PERIM_ERR_NULL;
    if (!slot_legitimo(slot))   return HASHMAL_PERIM_ERR_SHAAR;
    *out = g_sesiones[slot];
    return HASHMAL_PERIM_OK;
}

/* ==================================================================
 * API - hashmal_perimeter_shaar_desconectar
 * ================================================================== */
int hashmal_perimeter_shaar_desconectar(uint8_t slot)
{
    if (!slot_legitimo(slot)) return HASHMAL_PERIM_ERR_SHAAR;

    hashmal_shaar_sesion_estado_t prev = g_sesiones[slot].estado;
    if (prev == HASHMAL_SHAAR_SESION_VACIA
     || prev == HASHMAL_SHAAR_SESION_DESCONECTADO) {
        return HASHMAL_PERIM_OK;  /* idempotente */
    }

    g_sesiones[slot].estado            = HASHMAL_SHAAR_SESION_DESCONECTADO;
    g_sesiones[slot].timestamp_ms      = hashmal_perim_ahora_ms();
    g_sesiones[slot].intent_verificada = false;
    /* Limpia columnas + capas para sanidad de audit. */
    for (uint8_t i = 0; i < HASHMAL_SHAAR_COLUMNAS; ++i) {
        g_sesiones[slot].columnas[i] = false;
        for (uint8_t k = 0; k < HASHMAL_SHAAR_CAPAS_POR_COLUMNA; ++k) {
            g_sesiones[slot].capas[i][k] = false;
        }
    }

    SHR_LOG_I("desconectar: slot %u %s -> DESCONECTADO",
              (unsigned)slot, nombre_estado_sesion(prev));
    return HASHMAL_PERIM_OK;
}

/* ==================================================================
 * API - hashmal_perimeter_shaar_info (lectura pura)
 * ================================================================== */
int hashmal_perimeter_shaar_info(hashmal_shaar_info_t *out)
{
    if (out == NULL) return HASHMAL_PERIM_ERR_NULL;

    uint8_t activas    = 0;
    uint8_t pendientes = 0;
    for (uint8_t i = 0; i < HASHMAL_SHAAR_SESIONES_MAX; ++i) {
        switch (g_sesiones[i].estado) {
        case HASHMAL_SHAAR_SESION_ACTIVO:
            ++activas;
            break;
        case HASHMAL_SHAAR_SESION_CONECTADO:
        case HASHMAL_SHAAR_SESION_VERIFICADO:
            ++pendientes;
            break;
        default:
            break;
        }
    }

    uint8_t bases_ok = 0;
    for (uint8_t i = 0; i < HASHMAL_SHAAR_COLUMNAS; ++i) {
        if (g_bases[i].configurada) ++bases_ok;
    }

    out->estado              = g_estado;
    out->sesiones_activas    = activas;
    out->sesiones_pendientes = pendientes;
    out->conexiones_totales  = g_conexiones_totales;
    out->stealth_activo      = g_stealth;          /* P6 */
    out->bases_configuradas  = bases_ok;           /* P2 */
    return HASHMAL_PERIM_OK;
}

/* ==================================================================
 * VERIFICADO #04 — Invariantes ARQUITECTÓNICAS (P3, P4, P7, P10, P11,
 * P12, P13). Sin código en Phase 1: son reglas de diseño que esta
 * pieza HONRA y que cualquier cambio futuro DEBE preservar.
 *
 * P3 — ROQÉM (visible) vs JOSÉV (interno). Éx 27:16 maʻaséh roqém.
 *   La interfaz del portal (esta pieza) es BORDADO — capa visible al
 *   usuario que entra. La seguridad real (cifrado, mediación de zonas)
 *   vive en hashmal_framework (Pieza 05) — TEJIDO interior. Por eso
 *   esta pieza NO realiza criptografía propia: sólo sequencia
 *   verificaciones cuyas BASES (P2) viven en otras piezas.
 *
 * P4 — MASAKH (activo) vs QELAʻÍM (pasivo). Éx 27:16 vs 27:9.
 *   ESTE módulo cambia de estado (CERRADO/ABIERTO/PAUSADO) — único
 *   en el perímetro. Las APIs perimetrales hermanas (atrio_perimetro,
 *   atrio_columnas, atrio_puerta) son MURO PASIVO: enums fijos,
 *   ninguna transición runtime. Mezclar ambos rompe la distinción.
 *
 * P7 — Simetría 3-4-3 katéf. Éx 27:14-15.
 *   Dos hombros de 3 columnas + portal central de 4. El portal
 *   tiene UNA columna más que cada hombro: máxima diferenciación
 *   con mínima asimetría. En firmware: HASHMAL_SHAAR_COLUMNAS=4 (no 3,
 *   no 5). Tocar este número rompe la proporción del Padre.
 *
 * P10 — YETEDÓT infraestructura invisible. Éx 27:19 nejóshet.
 *   En Phase 2, los servicios subyacentes (DHCP del SoftAP, DNS del
 *   captive portal, timers, watchdog) anclan el portal sin ser
 *   vistos. En Phase 1 quedan documentados. Su muerte = portal cae
 *   silenciosamente. Auditoría P14 debería extenderse en Phase 2
 *   para verificar que todas las yetedót están vivas.
 *
 * P11 — Espejo de portales (atrio bronce vs Tienda oro). Éx 27:16
 *   vs 26:36. Si en futuro existe un sha'ar interno entre zonas de
 *   confianza (hashmal_framework velo dorado), debe replicar esta
 *   misma estructura (4 columnas, 3 capas, intent) con criterios
 *   más estrictos (oro). Misma forma, distinto material.
 *
 * P12 — MIZZÉH UMIZZÉH bilateral. Éx 38:15 — misma fórmula que
 *   Éx 25:19 querubines. Las 4 columnas son DOS PARES espejados
 *   alrededor del paso central:
 *     IDENTIDAD ←→ INTEGRIDAD     (extremos: quién entra / quién es ahora)
 *     AUTENTIC. ←→ AUTORIZACIÓN   (centro: tienes clave / qué puedes)
 *   Esta lectura está implícita en el orden secuencial de P1.
 *
 * P13 — Multi-protocolo SÓLO en el sha'ar. Éx 38:16 vs 38:18.
 *   El muro perimetral opera UN tipo de filtrado (accept/reject por
 *   pertenencia). Sólo el portal mezcla 4 tipos (los 4 colores
 *   tekhelet/argamán/toláʻat/shesh = identidad/autenticación/auto-
 *   rización/integridad). NO añadir multi-canal a otras piezas del
 *   perímetro: rompe el monocromo del muro.
 * ================================================================== */

/* ==================================================================
 *
 *   "uleSHA'AR HEJATSER MASAKH 'ESRIM AMMAH... 'AMMUDEIHEM
 *    ARBA'AH veadneihem ARBA'AH." - Exodo 27:16
 *
 *   "ma'aseh ROQEM." - Exodo 26:36 / 27:16 / 38:18
 *
 *   "BO'U SHE'ARAV BETODAH JATSEROTAV BITEHILLAH." - Salmo 100:4
 *
 *   "ufittjau SHE'ARAYIKH TAMID." - Isaias 60:11
 *
 *   "ego eimi he THYRA; di emou ean tis EISELTHE SOTHESETAI." - Jn 10:9
 *
 *   La puerta es UNA - amplia (20 codos, 1/5 del frente entero) y
 *   reposa en CUATRO columnas. No tres ni cinco. Cuatro puntos de
 *   verificacion sostienen la cortina; si falta uno, la cortina se
 *   cae. Ex 26:36/27:16/38:18 repiten tres veces "obra de bordador"
 *   - la puerta debe ser HERMOSA, no funcional minima. La interfaz
 *   es parte del mensaje.
 *
 *   Is 60:11 dice TAMID - dia y noche abierta. La puerta no se
 *   cierra por horario; solo por causa especifica (Shabat,
 *   emergencia). Por eso PAUSADO existe como estado distinto de
 *   CERRADO: pausa temporal != desmonte.
 *
 *   Y Jn 10:9 cierra: la puerta no es restriccion, es SALVACION.
 *   Quien entra por ella halla pasto. El sha'ar bordado de azul,
 *   purpura y carmesi era hermoso porque la entrada al pueblo del
 *   Padre es hermosa. El firmware honra el patron en su pequena
 *   escala: pantalla del portal cautivo (fase 2) sera ma'aseh
 *   roqem - bordada con arte, no formulario tosco.
 *
 * ================================================================== */
