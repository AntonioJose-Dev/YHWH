/*
 * mock_hashmal_storage.h
 * Mock of hashmal_storage.h for tests of hashmal_consensus (Kaporet).
 * Provides only the types and functions that kaporet_core.c calls
 * at runtime, plus a couple of test-controllable knobs.
 */
#ifndef MOCK_HASHMAL_STORAGE_H
#define MOCK_HASHMAL_STORAGE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Neutralize the real hashmal_storage.h: defining its include guard
 * before it is ever read makes the real body a no-op, so only this
 * mock contributes symbols. */
#ifndef HASHMAL_STORAGE_H
#define HASHMAL_STORAGE_H
#endif

/* Size of the Merkle/SHA-256 hash used by the Arca. Matches the real
 * header value. hashmal_consensus.h uses it for hash_testigo arrays. */
#define HASHMAL_ZER_HASH_BYTES 32

/* Minimal zer (Merkle moldura) — kaporet_verificacion.c reads .hash
 * for the data-check vertical (el haKapóret, Éx 25:20b). */
typedef struct {
    uint8_t  hash[HASHMAL_ZER_HASH_BYTES];
    uint64_t nonce;
    bool     rodea;
} hashmal_zer_t;

/* Minimal Arca type: dimensions for x/y fit (kaporet_core.c) + zer
 * for the data-check vertical (kaporet_verificacion.c). */
typedef struct {
    uint8_t       largo_x10;
    uint8_t       ancho_x10;
    uint8_t       alto_x10;
    hashmal_zer_t zer;
} hashmal_arca_t;

/* Controllable mock state. Tests flip mock_arca_disponible to simulate
 * "Arca not initialized" without tearing anything down. */
static hashmal_arca_t mock_arca = {
    .largo_x10 = 25,
    .ancho_x10 = 15,
    .alto_x10  = 15,
    .zer       = { .hash = {0}, .nonce = 0, .rodea = true }
};
static bool mock_arca_disponible = true;

/* Inline so the symbols live in whatever single translation unit
 * includes this header (the test binary). Prevents duplicate-symbol
 * errors if the header is pulled in more than once. */
static inline const hashmal_arca_t *hashmal_storage_get_arca(void) {
    return mock_arca_disponible ? &mock_arca : NULL;
}

static inline int hashmal_storage_kaporet_colocada(uint8_t kap_largo_x10,
                                                   uint8_t kap_ancho_x10) {
    /* Footprint match against the real Arca dimensions. Tests can
     * tweak mock_arca.largo_x10/ancho_x10 to simulate divergence. */
    if (!mock_arca_disponible) return -2;  /* ERR_INIT */
    if (kap_largo_x10 != mock_arca.largo_x10 ||
        kap_ancho_x10 != mock_arca.ancho_x10) {
        return -3;  /* ERR_SIN_KAPORET — footprint mismatch */
    }
    return 0;  /* OK */
}

static inline bool hashmal_storage_kavod_lleno(void) {
    return false;
}

#endif /* MOCK_HASHMAL_STORAGE_H */
