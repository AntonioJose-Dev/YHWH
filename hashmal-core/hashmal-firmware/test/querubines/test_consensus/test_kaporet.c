/*
 * test_kaporet.c
 * Querubines Test 1 for hashmal_consensus (Kaporet core).
 *
 * Compile and run (from the hashmal-firmware directory):
 *
 *   gcc -Wall -Wextra \
 *       -I test/querubines/ \
 *       -I test/querubines/test_consensus/ \
 *       -I components/hashmal_consensus/include/ \
 *       -I components/hashmal_consensus/src/ \
 *       -I components/hashmal_storage/include/ \
 *       test/querubines/test_consensus/test_kaporet.c \
 *       -o test/querubines/test_consensus/test_kaporet
 *   ./test/querubines/test_consensus/test_kaporet
 *
 * Strategy
 * --------
 * The mocks are included first so their symbols and include-guards
 * are already in place before any firmware header is read. The real
 * kaporet_core.c is then pulled in as source (via #include) so the
 * whole test is a single translation unit with no external linkage
 * dependency on its sibling files (kaporet_rendezvous.c, etc.).
 */

/* 1) Mocks first so macros/types are defined AND include guards of
 *    the real ESP-IDF / storage headers are pre-set. */
#include "mock_esp_log.h"
#include "mock_hashmal_storage.h"

/* 2) Public + internal API headers of the component under test. */
#include "hashmal_consensus.h"
#include "hashmal_consensus_internal.h"

/* 3) The real source, compiled inline. All ESP_LOGx calls route to
 *    printf via the mock; hashmal_storage_* calls route to the mock. */
#include "kaporet_core.c"

/* 4) Test framework (QR_ASSERT and friends). */
#include "querubines_framework.h"


int main(void)
{
    QR_TEST_BEGIN("Consensus Core (Kaporet)");

    /* ================================================================
     * Design-time properties: compile-time constants from the header.
     * If any of these diverges, the contract has drifted.
     * ================================================================ */
    QR_SECTION("Propiedades de diseno");

    QR_ASSERT_TRUE(HASHMAL_MIQSHA_ATOMICO,
                   "Consenso es atomico (miqsha = una sola pieza)");
    QR_ASSERT_EQ(HASHMAL_QUERUBINES_TOTAL, 2,
                 "Exactamente 2 validadores (querubines)");
    QR_ASSERT_EQ(HASHMAL_VERIFICACIONES_SHEVA, 7,
                 "7 verificaciones requeridas (sheva)");
    QR_ASSERT_EQ(HASHMAL_OJOS_MINIMOS, 8,
                 "Minimo 8 puntos de auditoria (ojos)");

    /* ================================================================
     * Runtime properties: init builds a valid singleton, and both
     * validators come out symmetrically forged.
     * ================================================================ */
    QR_SECTION("Ciclo de vida");

    int rc = hashmal_consensus_init();
    QR_ASSERT(rc == HASHMAL_KAPORET_OK, "Init del Kaporet exitoso");

    const hashmal_kaporet_t *kap = hashmal_consensus_get_kaporet();
    QR_ASSERT_NOT_NULL(kap, "Kaporet accesible tras init");
    QR_ASSERT_FALSE(kap->sobre_el_arca,
                    "Kaporet NO esta sobre el Arca al inicio");

    /* Check both validators fulfil every invariant in one combined
     * boolean. If any flag flips, the whole assertion fails and the
     * individual flag is easy to find in kaporet_core.c. */
    bool todo_ok_v1 =
        kap->querubin[0].alas_extendidas_arriba &&
        kap->querubin[0].alas_cubren            &&
        kap->querubin[0].mira_al_hermano        &&
        kap->querubin[0].mira_al_kaporet        &&
        kap->querubin[0].miqsha_con_kaporet     &&
        kap->querubin[0].guardian_no_abridor    &&
        (kap->querubin[0].ojos_total >= HASHMAL_OJOS_MINIMOS);

    bool todo_ok_v2 =
        kap->querubin[1].alas_extendidas_arriba &&
        kap->querubin[1].alas_cubren            &&
        kap->querubin[1].mira_al_hermano        &&
        kap->querubin[1].mira_al_kaporet        &&
        kap->querubin[1].miqsha_con_kaporet     &&
        kap->querubin[1].guardian_no_abridor    &&
        (kap->querubin[1].ojos_total >= HASHMAL_OJOS_MINIMOS);

    QR_ASSERT(todo_ok_v1, "Querubin V1 correctamente forjado");
    QR_ASSERT(todo_ok_v2, "Querubin V2 correctamente forjado");

    /* ================================================================
     * Negative path: if the Arca is not available, cubrir_arca must
     * refuse with the right error and NOT mark sobre_el_arca = true.
     * ================================================================ */
    QR_SECTION("Prueba negativa");

    /* Clean slate before the negative scenario. */
    hashmal_consensus_deinit();

    /* Make hashmal_storage_get_arca() return NULL from this point. */
    mock_arca_disponible = false;

    /* Init itself does not depend on the Arca, so it should still
     * succeed even with the Arca unavailable. */
    rc = hashmal_consensus_init();
    QR_ASSERT(rc == HASHMAL_KAPORET_OK,
              "Init funciona aunque el Arca no este disponible");

    int rc_cubrir = hashmal_consensus_cubrir_arca();
    QR_ASSERT(rc_cubrir == HASHMAL_KAPORET_ERR_SIN_ARCA,
              "Sin Arca, cubrir_arca falla correctamente");

    /* Restore for any further tests and tear down cleanly. */
    mock_arca_disponible = true;
    hashmal_consensus_deinit();

    QR_TEST_END();
    return qr_all_passed() ? 0 : 1;
}
