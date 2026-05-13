/*
 * mock_aceite_deps.h
 * Internal-API mock for host-side tests of menora_aceite.c.
 *
 * Pulls in the REAL hashmal_comm.h for types/enums/constants and
 * replaces hashmal_comm_internal.h with just the four primitives
 * menora_aceite.c calls:
 *
 *   hashmal_comm_get_menora_interno()  (const read)
 *   hashmal_comm_menora_mut()          (mutable write, for duty_cycle)
 *   hashmal_comm_canal_legitimo()      (range check)
 *   hashmal_comm_ahora_ms()            (monotonic clock, stubbed)
 *
 * The real menora_hub.c is NOT linked; this header supplies the
 * minimum needed to exercise the aceite module alone.
 */
#ifndef MOCK_ACEITE_DEPS_H
#define MOCK_ACEITE_DEPS_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* Real public types / enums / constants. */
#include "hashmal_comm.h"

/* Pre-set the internal.h include guard so its real body is skipped
 * when menora_aceite.c drags it in. This mock supplies the
 * functions the body would have declared. */
#ifndef HASHMAL_COMM_INTERNAL_H
#define HASHMAL_COMM_INTERNAL_H
#endif


/* ==================================================================
 * Test-controllable state
 * ================================================================== */

/* Singleton the mock exposes. Tests populate via setup_menora_aceite()
 * and/or poke individual fields before calling the function under
 * test. */
static hashmal_menora_t mock_menora;

/* Toggle to simulate "Menora not initialized":
 *   true  -> get_menora_interno / menora_mut return &mock_menora
 *   false -> both return NULL */
static bool mock_menora_disponible = true;


/* ==================================================================
 * Internal-API primitives
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
    /* Single cast to unsigned covers both ends: a negative enum wraps
     * past the upper bound. */
    return (unsigned)id < (unsigned)HASHMAL_MENORA_CANALES;
}

/* Fixed timestamp is enough for these tests: duty_cycle only reads
 * it when restoring ATENUADO -> ENCENDIDO, to stamp the new
 * encendido time. A constant value avoids flakiness and keeps the
 * test deterministic. */
static inline uint64_t hashmal_comm_ahora_ms(void)
{
    return 1000u;
}


/* ==================================================================
 * Setup helper
 * ================================================================== */

/* Full healthy Menora for aceite tests: seven lamps ENCENDIDO with
 * plenty of oil (80 on a 0..100 scale), miqsha + tamid flags raised,
 * singleton marked available, nerot_encendidos = 7. Tests call this
 * first and then mutate whatever the scenario requires. */
static void setup_menora_aceite(void)
{
    memset(&mock_menora, 0, sizeof(mock_menora));

    for (int i = 0; i < HASHMAL_MENORA_CANALES; i++) {
        mock_menora.nerot[i].id            = (hashmal_canal_id_t)i;
        mock_menora.nerot[i].estado        = HASHMAL_NER_ENCENDIDO;
        mock_menora.nerot[i].aceite_nivel  = 80;
    }

    mock_menora.miqsha            = true;
    mock_menora.tamid             = true;
    mock_menora.nerot_encendidos  = HASHMAL_MENORA_CANALES;

    mock_menora_disponible = true;
}

#endif /* MOCK_ACEITE_DEPS_H */
