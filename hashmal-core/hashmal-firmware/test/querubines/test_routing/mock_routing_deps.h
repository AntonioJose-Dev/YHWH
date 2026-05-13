/*
 * mock_routing_deps.h
 * Host-side mocks for testing menora_routing.c in isolation.
 *
 * menora_routing.c depends on three layers:
 *   (a) internal.h primitives    (canal_legitimo, menora_mut, ...)
 *   (b) driver entry points      (hashmal_comm_spi_enviar, ...)
 *   (c) the selector             (hashmal_comm_mutsaqa_seleccionar)
 *
 * (a) is replaced with `static inline` functions because those names
 *     are declared only in hashmal_comm_internal.h, whose body is
 *     skipped by pre-setting the include guard below.
 *
 * (b) and (c) are declared in the PUBLIC header (hashmal_comm.h) with
 *     external linkage. We therefore define them without `static` so
 *     their mock definitions match that declaration. The single-
 *     include guard on this header keeps them as one definition.
 *
 * This header is only included from test_routing.c (one TU), so no
 * ODR violation arises from putting definitions in a header.
 */
#ifndef MOCK_ROUTING_DEPS_H
#define MOCK_ROUTING_DEPS_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* Real public API (types, enums, constants, external declarations). */
#include "hashmal_comm.h"

/* Pre-set the internal.h guard: when menora_routing.c drags in
 * hashmal_comm_internal.h, its body is skipped and this mock supplies
 * the functions it would have declared. */
#ifndef HASHMAL_COMM_INTERNAL_H
#define HASHMAL_COMM_INTERNAL_H
#endif


/* ==================================================================
 * Singleton state
 * ================================================================== */

static hashmal_menora_t mock_menora;
static bool             mock_menora_disponible = true;


/* ==================================================================
 * Driver stubs - test-controllable outcomes
 *
 * Regular (non-static) linkage: these names are declared extern in
 * hashmal_comm.h, so a `static` definition would trigger
 * "static declaration follows non-static declaration". External
 * linkage keeps the declarations consistent.
 * ================================================================== */

/* SPI / LoRa send stub: the test sets the rc; we also capture the
 * last size that menora_routing.c asked us to transmit, for
 * assertion if needed. */
static int    mock_spi_enviar_rc  = 0;
static size_t mock_spi_enviar_tam = 0;

int hashmal_comm_spi_enviar(const uint8_t *datos, size_t tam)
{
    (void)datos;
    mock_spi_enviar_tam = tam;
    return mock_spi_enviar_rc;
}

/* SPI / LoRa receive stub: simulates a pending LoRa frame when
 * mock_spi_recibir_bytes > 0 (data copied from mock_spi_recibir_data),
 * else reports "nothing pending" with *recibidos = 0. */
static int     mock_spi_recibir_rc    = 0;
static size_t  mock_spi_recibir_bytes = 0;
static uint8_t mock_spi_recibir_data[256];

int hashmal_comm_spi_recibir(uint8_t *buf, size_t tam, size_t *recibidos)
{
    if (buf == NULL || recibidos == NULL) return HASHMAL_COMM_ERR_NULL;

    if (mock_spi_recibir_bytes == 0u) {
        *recibidos = 0u;
        return mock_spi_recibir_rc;
    }

    size_t n = (tam < mock_spi_recibir_bytes) ? tam : mock_spi_recibir_bytes;
    memcpy(buf, mock_spi_recibir_data, n);
    *recibidos = n;
    return mock_spi_recibir_rc;
}

/* ESP-NOW receive stub: always reports "nothing pending". The
 * routing recibir path falls through to this only if LoRa reported
 * zero bytes; here we simply confirm the call chain without adding
 * data. */
int hashmal_comm_espnow_recibir(uint8_t mac_origen[6],
                                uint8_t *buf,
                                size_t tam,
                                size_t *recibidos)
{
    (void)mac_origen;
    (void)buf;
    (void)tam;
    if (recibidos != NULL) *recibidos = 0u;
    return HASHMAL_COMM_OK;
}

/* Selector stub: always returns OK and writes the pre-set mutsaqa
 * into *out. Routing tests aren't about selection logic; we isolate
 * routing by pinning the selector's answer. */
static hashmal_mutsaqa_id_t mock_mutsaqa_elegida = HASHMAL_MUTSAQA_LORA;

int hashmal_comm_mutsaqa_seleccionar(hashmal_canal_id_t canal,
                                      uint8_t nodo_destino,
                                      size_t tam_mensaje,
                                      hashmal_mutsaqa_id_t *mutsaqa_elegida)
{
    (void)canal;
    (void)nodo_destino;
    (void)tam_mensaje;
    if (mutsaqa_elegida == NULL) return HASHMAL_COMM_ERR_NULL;
    *mutsaqa_elegida = mock_mutsaqa_elegida;
    return HASHMAL_COMM_OK;
}

/* Tsofeh lockdown stub (Añadidura 02): the routing source now
 * consults this at the top of hashmal_comm_enviar. Default to
 * "not locked" so existing routing tests are unaffected. */
bool hashmal_comm_tsofeh_lockdown_get(void)
{
    return false;
}


/* ==================================================================
 * Internal-API primitives (static inline - not in public header)
 * ================================================================== */

static inline const hashmal_menora_t *hashmal_comm_get_menora_interno(void)
{
    return mock_menora_disponible ? &mock_menora : NULL;
}

static inline hashmal_menora_t *hashmal_comm_menora_mut(void)
{
    return mock_menora_disponible ? &mock_menora : NULL;
}

static inline bool hashmal_comm_canal_legitimo(hashmal_canal_id_t id)
{
    /* Single cast to unsigned covers both ends. */
    return (unsigned)id < (unsigned)HASHMAL_MENORA_CANALES;
}


/* ==================================================================
 * Setup helper
 *
 * Baseline: seven nerot ENCENDIDO with oil at 80 and LoRa mutsaqa
 * available+active on every ner, and all stubbed driver knobs at
 * "success, nothing pending". Tests call this first and tweak only
 * what their scenario requires.
 * ================================================================== */

static void setup_menora_routing(void)
{
    memset(&mock_menora, 0, sizeof(mock_menora));

    for (int i = 0; i < HASHMAL_MENORA_CANALES; i++) {
        mock_menora.nerot[i].id           = (hashmal_canal_id_t)i;
        mock_menora.nerot[i].estado       = HASHMAL_NER_ENCENDIDO;
        mock_menora.nerot[i].aceite_nivel = 80;

        /* LoRa mutsaqa (index 0): the spine (geza') always present.
         * Routing stubs the selector and delivers via LoRa in these
         * tests - make sure the ner's LoRa slot is plausible. */
        mock_menora.nerot[i].mutsaqot[0].id         = HASHMAL_MUTSAQA_LORA;
        mock_menora.nerot[i].mutsaqot[0].disponible = true;
        mock_menora.nerot[i].mutsaqot[0].activa     = true;
        mock_menora.nerot[i].mutsaqot[0].consumo_mw = 120;
    }

    mock_menora.miqsha            = true;
    mock_menora.tamid             = true;
    mock_menora.nerot_encendidos  = HASHMAL_MENORA_CANALES;

    /* Driver / selector stub knobs back to defaults. */
    mock_spi_enviar_rc     = 0;
    mock_spi_enviar_tam    = 0;
    mock_spi_recibir_rc    = 0;
    mock_spi_recibir_bytes = 0;
    mock_mutsaqa_elegida   = HASHMAL_MUTSAQA_LORA;

    mock_menora_disponible = true;
}

#endif /* MOCK_ROUTING_DEPS_H */
