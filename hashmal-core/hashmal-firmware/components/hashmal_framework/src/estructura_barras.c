/**
 * ===================================================================
 * PIEZA 05 - LA ESTRUCTURA - estructura_barras.c
 * Sub-pieza E - Las barras (berijim) - Exodo 26:26-29
 * ===================================================================
 *
 *   v.26 - "ve'asita berijim 'atse shittim jamisha leqarse tsela'
 *           hamishkan ha'ejad"
 *          Haras 5 berijim de acacia para un lado del Mishkan.
 *
 *   v.27 - "vajamisha berijim leqarse tsela' hamishkan hasheni
 *           vajamisha berijim leqarse tsela' hamishkan layark'tayim yamah"
 *          5 mas para el segundo lado, 5 mas para el oeste.
 *          -> 3 lados x 5 barras = 15 berijim.
 *
 *   v.28 - "vehabriaj hatijon betoj haqerashim mabriaj
 *           min haqatse el haqatse"
 *          LA BARRA DEL CENTRO (hatijon), EN MEDIO (betoj) de los
 *          tablones, atravesando de EXTREMO A EXTREMO.
 *          -> Invisible desde fuera: pasa POR DENTRO de los qerashim.
 *          -> UNA SOLA pieza que cruza todo. Las otras 4 pueden
 *            segmentarse; esta no.
 *          -> BACKBONE. Sin ella los qerashim se caen.
 *
 *   v.29 - "ve'et haqerashim tetsape zahav ve'et tabbe'otehem
 *           ta'ase zahav battim labberijim vetsipita et habberijim zahav"
 *          Tablones cubiertos de oro. Anillos (tabbe'ot) de oro
 *          como CASAS (battim) para las barras. Barras cubiertas
 *          de oro. (Los anillos viven en el qeresh, no en la barra
 *          - la barra pasa POR ellos.)
 *
 * PATRON DE COLOCACION (convencion local, 5 posiciones por lado):
 *
 *     pos 0 -+                    pos 1 -+
 *            |    fila superior           |
 *     pos 2 -+-------- hatijon ----------- --- (v.28, backbone)
 *            |    fila inferior           |
 *     pos 3 -+                    pos 4 -+
 *
 *   - pos 2 = hatijon -> es_backbone = true -> span extremo a extremo
 *   - pos 0, 1, 3, 4 = segmentadas (v.28 enfatiza SOLO la central;
 *     la segmentacion de las otras 4 es lectura comun, no afirmada
 *     explicitamente en el texto).
 *
 * IDs GLOBALES:
 *     sur   -> 0..4
 *     norte -> 5..9
 *     oeste -> 10..14
 *
 * "kemishpato asher horeta bahar" - Ex 26:30
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "esp_log.h"           /* ORO DE EGIPTO - encapsulado solo aqui (Ex 12:35-36) */

#include "hashmal_framework.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *BARRAS_TAG = "hashmal.barras";

#define BARRAS_LOG_I(fmt, ...)  ESP_LOGI(BARRAS_TAG, fmt, ##__VA_ARGS__)
#define BARRAS_LOG_W(fmt, ...)  ESP_LOGW(BARRAS_TAG, fmt, ##__VA_ARGS__)
#define BARRAS_LOG_E(fmt, ...)  ESP_LOGE(BARRAS_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * POSICIONES DENTRO DEL LADO (convencion local, 0..4)
 * ================================================================== */
#define BARRA_POS_SUP_A      0u   /* segmentada - fila superior, mitad A */
#define BARRA_POS_SUP_B      1u   /* segmentada - fila superior, mitad B */
#define BARRA_POS_HATIJON    2u   /* v.28 - backbone: centro, extremo a extremo */
#define BARRA_POS_INF_A      3u   /* segmentada - fila inferior, mitad A */
#define BARRA_POS_INF_B      4u   /* segmentada - fila inferior, mitad B */

/* ==================================================================
 * API INTERNA DEL MODULO
 * (Se promovera a hashmal_framework_internal.h con el orquestador.)
 * ================================================================== */

/**
 * Levanta las 15 berijim (3 lados x 5 barras) conforme a Ex 26:26-29.
 * @return 0 en exito, -1 si algun puntero es NULL.
 */
int hashmal_barras_init(
    hashmal_beriaj_t sur  [HASHMAL_BARRAS_POR_LADO],
    hashmal_beriaj_t norte[HASHMAL_BARRAS_POR_LADO],
    hashmal_beriaj_t oeste[HASHMAL_BARRAS_POR_LADO]);

/**
 * Verifica invariantes biblicas:
 *   v.26 - cada lado tiene exactamente 5 berijim
 *   v.27 - 3 lados: sur, norte, oeste (este abierto, sin barras)
 *   v.28 - exactamente 1 hatijon por lado, spanning min->max del lado
 *   v.28 - el hatijon es unico: ninguna otra barra lleva es_backbone=true
 *   total - 15 barras, 3 backbones
 *   rangos - desde_tabla <= hasta_tabla, y dentro del rango del lado
 * @return true si el conjunto respeta el patron del monte (v.30).
 */
bool hashmal_barras_verificar(
    const hashmal_beriaj_t sur  [HASHMAL_BARRAS_POR_LADO],
    const hashmal_beriaj_t norte[HASHMAL_BARRAS_POR_LADO],
    const hashmal_beriaj_t oeste[HASHMAL_BARRAS_POR_LADO]);

/**
 * Cuenta backbones totales (debe ser HASHMAL_BARRAS_LADOS = 3,
 * uno por cada lado cerrado: sur, norte, oeste).
 */
uint8_t hashmal_barras_backbones_total(
    const hashmal_beriaj_t sur  [HASHMAL_BARRAS_POR_LADO],
    const hashmal_beriaj_t norte[HASHMAL_BARRAS_POR_LADO],
    const hashmal_beriaj_t oeste[HASHMAL_BARRAS_POR_LADO]);

/* ==================================================================
 * HELPERS
 * ================================================================== */

/**
 * Rango de IDs de qerashim que cubre cada lado.
 * Derivado explicitamente del layout usado en estructura_tablas.c:
 *   sur   -> [0                       , TABLAS_SUR - 1]
 *   norte -> [TABLAS_SUR              , SUR+NORTE - 1]
 *   oeste -> [SUR+NORTE               , SUR+NORTE+OESTE - 1]
 * @return true si el lado es valido y se completaron min/max.
 */
static bool rango_lado(hashmal_lado_t lado, uint8_t *min_id, uint8_t *max_id)
{
    if (min_id == NULL || max_id == NULL) return false;
    switch (lado) {
        case HASHMAL_LADO_SUR:
            *min_id = 0u;
            *max_id = (uint8_t)(HASHMAL_TABLAS_SUR - 1u);
            return true;
        case HASHMAL_LADO_NORTE:
            *min_id = (uint8_t)(HASHMAL_TABLAS_SUR);
            *max_id = (uint8_t)(HASHMAL_TABLAS_SUR + HASHMAL_TABLAS_NORTE - 1u);
            return true;
        case HASHMAL_LADO_OESTE:
            *min_id = (uint8_t)(HASHMAL_TABLAS_SUR + HASHMAL_TABLAS_NORTE);
            *max_id = (uint8_t)(HASHMAL_TABLAS_SUR + HASHMAL_TABLAS_NORTE + HASHMAL_TABLAS_OESTE - 1u);
            return true;
        default:
            return false;
    }
}

/**
 * ID global de una barra a partir de lado + posicion (0..4).
 *   sur   -> 0..4
 *   norte -> 5..9
 *   oeste -> 10..14
 */
static inline uint8_t id_barra(hashmal_lado_t lado, uint8_t pos)
{
    uint8_t base = 0u;
    switch (lado) {
        case HASHMAL_LADO_SUR:   base = 0u;                                break;
        case HASHMAL_LADO_NORTE: base = (uint8_t)(HASHMAL_BARRAS_POR_LADO);       break;
        case HASHMAL_LADO_OESTE: base = (uint8_t)(2u * HASHMAL_BARRAS_POR_LADO);  break;
        default:                 base = 0u;                                break;
    }
    return (uint8_t)(base + pos);
}

/* ==================================================================
 * CONSTRUCTOR POR LADO
 * ================================================================== */

/**
 * Construye las 5 berijim de un lado. Aplica el patron segmentado
 * documentado en la cabecera de este archivo:
 *
 *   pos 0,1 -> fila superior, dos mitades del rango [min, max]
 *   pos 2   -> hatijon (v.28), rango completo [min, max]
 *   pos 3,4 -> fila inferior, dos mitades del rango [min, max]
 *
 * La particion en "mitades" se hace por (min+max)/2. Es inferencia
 * estructural - v.28 SOLO afirma extremo-a-extremo para el hatijon;
 * no dicta la segmentacion de las otras cuatro.
 */
static void construir_lado(hashmal_beriaj_t barras[HASHMAL_BARRAS_POR_LADO],
                           hashmal_lado_t lado)
{
    uint8_t min_id = 0u, max_id = 0u;
    (void)rango_lado(lado, &min_id, &max_id);

    const uint8_t medio = (uint8_t)((min_id + max_id) / 2u);

    /* Inicializacion base comun */
    for (uint8_t p = 0; p < HASHMAL_BARRAS_POR_LADO; ++p) {
        memset(&barras[p], 0, sizeof(barras[p]));
        barras[p].id          = id_barra(lado, p);
        barras[p].lado        = lado;                     /* v.26-27 */
        barras[p].es_backbone = false;
        barras[p].nivel       = HASHMAL_CONFIANZA_ORO;    /* v.29 - zahav (oro) */
    }

    /* Fila superior (segmentadas) - v.26-27 */
    barras[BARRA_POS_SUP_A].desde_tabla = min_id;
    barras[BARRA_POS_SUP_A].hasta_tabla = medio;

    barras[BARRA_POS_SUP_B].desde_tabla = (uint8_t)(medio + 1u);
    barras[BARRA_POS_SUP_B].hasta_tabla = max_id;

    /* Hatijon - v.28: "betoj haqerashim... min haqatse el haqatse" */
    barras[BARRA_POS_HATIJON].es_backbone = true;
    barras[BARRA_POS_HATIJON].desde_tabla = min_id;
    barras[BARRA_POS_HATIJON].hasta_tabla = max_id;

    /* Fila inferior (segmentadas) - v.26-27 */
    barras[BARRA_POS_INF_A].desde_tabla = min_id;
    barras[BARRA_POS_INF_A].hasta_tabla = medio;

    barras[BARRA_POS_INF_B].desde_tabla = (uint8_t)(medio + 1u);
    barras[BARRA_POS_INF_B].hasta_tabla = max_id;
}

/* ==================================================================
 * API INTERNA - IMPLEMENTACION
 * ================================================================== */

int hashmal_barras_init(
    hashmal_beriaj_t sur  [HASHMAL_BARRAS_POR_LADO],
    hashmal_beriaj_t norte[HASHMAL_BARRAS_POR_LADO],
    hashmal_beriaj_t oeste[HASHMAL_BARRAS_POR_LADO])
{
    if (sur == NULL || norte == NULL || oeste == NULL) {
        BARRAS_LOG_E("init: puntero NULL (sur=%p, norte=%p, oeste=%p)",
                     (const void*)sur, (const void*)norte, (const void*)oeste);
        return -1;
    }

    construir_lado(sur,   HASHMAL_LADO_SUR);    /* v.26       */
    construir_lado(norte, HASHMAL_LADO_NORTE);  /* v.27a      */
    construir_lado(oeste, HASHMAL_LADO_OESTE);  /* v.27b      */

    BARRAS_LOG_I("15 berijím levantadas (Ex 26:26-27): 5 sur + 5 norte + 5 oeste");
    BARRAS_LOG_I("v.28 — habríaj hatijón (×3): betój haqerashím, min haqatsé el haqatsé");
    BARRAS_LOG_I("v.29 — acacia + oro; tabbeʻót zaháv en los qerashím (casas para las barras)");
    return 0;
}

/* Verificador de un lado. Devuelve true si el grupo de 5 barras
 * cumple v.26, v.28 y consistencia interna. */
static bool verificar_lado(const hashmal_beriaj_t barras[HASHMAL_BARRAS_POR_LADO],
                           hashmal_lado_t lado_esperado)
{
    uint8_t min_id = 0u, max_id = 0u;
    if (!rango_lado(lado_esperado, &min_id, &max_id)) {
        BARRAS_LOG_E("lado inválido: %d", (int)lado_esperado);
        return false;
    }

    uint8_t backbones = 0u;

    for (uint8_t p = 0; p < HASHMAL_BARRAS_POR_LADO; ++p) {
        const hashmal_beriaj_t *b = &barras[p];

        /* id coherente con pos+lado */
        if (b->id != id_barra(lado_esperado, p)) {
            BARRAS_LOG_E("id(%u)!=esperado(%u) en pos %u",
                         b->id, id_barra(lado_esperado, p), p);
            return false;
        }

        /* lado consistente */
        if (b->lado != lado_esperado) {
            BARRAS_LOG_E("lado(%d)!=esperado(%d) en pos %u",
                         (int)b->lado, (int)lado_esperado, p);
            return false;
        }

        /* v.29 - vetsipita et habberijim zahav: todas las barras son
         * oro. El texto no hace excepcion. */
        if (b->nivel != HASHMAL_CONFIANZA_ORO) {
            BARRAS_LOG_E("v.29 violado: barra %u no es oro", b->id);
            return false;
        }

        /* Rango valido */
        if (b->desde_tabla > b->hasta_tabla) {
            BARRAS_LOG_E("rango invertido en pos %u: %u..%u",
                         p, b->desde_tabla, b->hasta_tabla);
            return false;
        }
        if (b->desde_tabla < min_id || b->hasta_tabla > max_id) {
            BARRAS_LOG_E("rango fuera de lado en pos %u: %u..%u (lado=%u..%u)",
                         p, b->desde_tabla, b->hasta_tabla, min_id, max_id);
            return false;
        }

        if (b->es_backbone) {
            backbones++;
            /* v.28 - el hatijon DEBE cubrir extremo a extremo */
            if (b->desde_tabla != min_id || b->hasta_tabla != max_id) {
                BARRAS_LOG_E("v.28 violado: hatijón no va extremo-a-extremo "
                             "(%u..%u, esperado %u..%u)",
                             b->desde_tabla, b->hasta_tabla, min_id, max_id);
                return false;
            }
        }
    }

    /* v.28 - exactamente UN hatijon por lado */
    if (backbones != HASHMAL_BACKBONE) {
        BARRAS_LOG_E("v.28 violado: hatijones=%u (esperado %d)",
                     backbones, HASHMAL_BACKBONE);
        return false;
    }

    return true;
}

bool hashmal_barras_verificar(
    const hashmal_beriaj_t sur  [HASHMAL_BARRAS_POR_LADO],
    const hashmal_beriaj_t norte[HASHMAL_BARRAS_POR_LADO],
    const hashmal_beriaj_t oeste[HASHMAL_BARRAS_POR_LADO])
{
    if (sur == NULL || norte == NULL || oeste == NULL) {
        BARRAS_LOG_E("verificar: puntero NULL");
        return false;
    }

    if (!verificar_lado(sur,   HASHMAL_LADO_SUR))   return false;
    if (!verificar_lado(norte, HASHMAL_LADO_NORTE)) return false;
    if (!verificar_lado(oeste, HASHMAL_LADO_OESTE)) return false;

    /* Total: v.26 (5) x v.27 (3 lados) = 15 */
    /* (implicito: 3 x verificar_lado = HASHMAL_BARRAS_TOTAL) */

    uint8_t total_bb = hashmal_barras_backbones_total(sur, norte, oeste);
    if (total_bb != HASHMAL_BARRAS_LADOS) {
        BARRAS_LOG_E("v.28 total violado: backbones=%u (esperado %d)",
                     total_bb, HASHMAL_BARRAS_LADOS);
        return false;
    }

    BARRAS_LOG_I("15 berijím verificadas (v.26-28): 5 por lado × 3 lados");
    BARRAS_LOG_I("3 hatijones — cada uno betój haqerashím, extremo a extremo (v.28)");
    return true;
}

uint8_t hashmal_barras_backbones_total(
    const hashmal_beriaj_t sur  [HASHMAL_BARRAS_POR_LADO],
    const hashmal_beriaj_t norte[HASHMAL_BARRAS_POR_LADO],
    const hashmal_beriaj_t oeste[HASHMAL_BARRAS_POR_LADO])
{
    if (sur == NULL || norte == NULL || oeste == NULL) return 0u;

    uint8_t total = 0u;
    for (uint8_t p = 0; p < HASHMAL_BARRAS_POR_LADO; ++p) {
        if (sur  [p].es_backbone) total++;
        if (norte[p].es_backbone) total++;
        if (oeste[p].es_backbone) total++;
    }
    return total;
}
