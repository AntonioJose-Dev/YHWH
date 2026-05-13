/*
 * mock_hashmal_comm.h
 * Mock of hashmal_comm_internal.h for host-side tests of
 * menora_mutsaqot.c. Pulls in the REAL hashmal_comm.h (types/enums/
 * constants) and replaces the internal.h contract with a
 * test-controllable singleton + the four primitives that
 * menora_mutsaqot.c calls at runtime:
 *
 *   hashmal_comm_get_menora_interno()
 *   hashmal_comm_canal_legitimo()
 *   hashmal_comm_mutsaqa_legitima()
 *   hashmal_comm_mutsaqa_mut()
 *
 * The real menora_hub.c / menora_canales.c etc. are NOT linked; this
 * header provides just enough of the internal API to compile the
 * selector alone.
 */
#ifndef MOCK_HASHMAL_COMM_H
#define MOCK_HASHMAL_COMM_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* Public API (types, enums, constants) comes from the real header. */
#include "hashmal_comm.h"

/* Prevent the REAL internal.h from contributing anything. Declared
 * here before the test source includes menora_mutsaqot.c. The real
 * hashmal_comm_internal.h checks this guard and its body gets
 * skipped; this mock supplies the functions it would have declared. */
#ifndef HASHMAL_COMM_INTERNAL_H
#define HASHMAL_COMM_INTERNAL_H
#endif


/* ==================================================================
 * Test-controllable state
 * ================================================================== */

/* Singleton the mock hands out. Tests populate it via setup_*
 * helpers and/or tweak individual fields directly before calling
 * the selector under test. */
static hashmal_menora_t mock_menora;

/* Toggle to simulate "Menora not initialized":
 *   true  -> hashmal_comm_get_menora_interno() returns &mock_menora
 *   false -> hashmal_comm_get_menora_interno() returns NULL */
static bool mock_menora_disponible = true;


/* ==================================================================
 * Internal-API primitives (minimal reimplementation)
 * ================================================================== */

static inline const hashmal_menora_t *hashmal_comm_get_menora_interno(void)
{
    return mock_menora_disponible ? &mock_menora : NULL;
}

static inline bool hashmal_comm_canal_legitimo(hashmal_canal_id_t id)
{
    /* Cast to unsigned once: negative enum values wrap past the
     * upper bound, so a single check covers both ends. */
    return (unsigned)id < (unsigned)HASHMAL_MENORA_CANALES;
}

static inline bool hashmal_comm_mutsaqa_legitima(hashmal_mutsaqa_id_t id)
{
    return (unsigned)id < (unsigned)HASHMAL_MUTSAQA_TOTAL;
}

static inline hashmal_mutsaqa_t *hashmal_comm_mutsaqa_mut(
        hashmal_canal_id_t canal,
        hashmal_mutsaqa_id_t mutsaqa)
{
    if (!mock_menora_disponible)               return NULL;
    if (!hashmal_comm_canal_legitimo(canal))   return NULL;
    if (!hashmal_comm_mutsaqa_legitima(mutsaqa)) return NULL;
    return &mock_menora.nerot[canal].mutsaqot[mutsaqa];
}


/* ==================================================================
 * Setup helpers
 *
 * Kept `static` so each TU including this header gets its own copy
 * (there is only one such TU: the test binary). The intent is to
 * give the test a one-line "healthy Menora" baseline that each TC
 * can then tweak before exercising the selector.
 * ================================================================== */

/* Fill one ner (lamp) with sane defaults: ENCENDIDO, high oil, all
 * three mutsaqot available and active with representative
 * consumo_mw values (BLE lowest, ESP-NOW middle, LoRa highest). */
static void setup_ner_basico(hashmal_ner_t *ner, int canal_id)
{
    memset(ner, 0, sizeof(*ner));

    ner->id            = (hashmal_canal_id_t)canal_id;
    ner->estado        = HASHMAL_NER_ENCENDIDO;
    ner->aceite_nivel  = 80;  /* well above every mutsaqa's aceite_min */

    /* LoRa - the geza (always-present spine). Highest consumption
     * in this mock: the point is that beruji normally prefers BLE. */
    ner->mutsaqot[0].id         = HASHMAL_MUTSAQA_LORA;
    ner->mutsaqot[0].disponible = true;
    ner->mutsaqot[0].activa     = true;
    ner->mutsaqot[0].consumo_mw = 120;

    /* ESP-NOW - isha el ajota (peer-to-peer). Mid consumption. */
    ner->mutsaqot[1].id         = HASHMAL_MUTSAQA_ESP_NOW;
    ner->mutsaqot[1].disponible = true;
    ner->mutsaqot[1].activa     = true;
    ner->mutsaqot[1].consumo_mw = 80;

    /* BLE - pa'amon (campanilla). Lowest consumption: beruji winner
     * when the message fits and oil is sufficient. */
    ner->mutsaqot[2].id         = HASHMAL_MUTSAQA_BLE;
    ner->mutsaqot[2].disponible = true;
    ner->mutsaqot[2].activa     = true;
    ner->mutsaqot[2].consumo_mw = 15;
}

/* Full healthy Menora: all seven nerot set up, miqsha + tamid flags
 * raised, and the singleton marked available. Every test that wants
 * to test a variant calls this first, then mutates what it needs. */
static void setup_menora_completa(void)
{
    memset(&mock_menora, 0, sizeof(mock_menora));

    for (int i = 0; i < HASHMAL_MENORA_CANALES; i++) {
        setup_ner_basico(&mock_menora.nerot[i], i);
    }

    mock_menora.miqsha      = true;
    mock_menora.tamid       = true;
    mock_menora_disponible  = true;
}

#endif /* MOCK_HASHMAL_COMM_H */
