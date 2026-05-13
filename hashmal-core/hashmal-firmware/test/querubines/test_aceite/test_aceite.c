/*
 * test_aceite.c
 * Querubines Test 3 for hashmal_comm - oil management (menora_aceite.c).
 *
 * Compile and run (from hashmal-firmware/):
 *
 *   gcc -Wall -Wextra \
 *       -I test/querubines/ \
 *       -I test/querubines/test_aceite/ \
 *       -I test/querubines/test_consensus/ \
 *       -I components/hashmal_comm/include/ \
 *       -I components/hashmal_comm/src/ \
 *       test/querubines/test_aceite/test_aceite.c \
 *       -o test/querubines/test_aceite/test_aceite
 *   ./test/querubines/test_aceite/test_aceite
 *
 * The esp_log mock is reused from the test_consensus tree; its stub
 * esp_log.h lives on the include path so menora_aceite.c's
 * `#include "esp_log.h"` resolves to no-op-body-plus-mock-macros.
 */

/* 1) esp_log mock (reused from test_consensus). Pre-sets the ESP-IDF
 *    include guard so any later esp_log.h include is a no-op body. */
#include "../test_consensus/mock_esp_log.h"

/* 2) Internal-API mock for menora_aceite.c. Drags in the real
 *    hashmal_comm.h and pre-sets HASHMAL_COMM_INTERNAL_H, supplying
 *    get_menora_interno / menora_mut / canal_legitimo / ahora_ms. */
#include "mock_aceite_deps.h"

/* 3) The real source under test, pulled in inline. All ESP_LOGx
 *    route through the mock to printf; all hashmal_comm_* internal
 *    calls resolve to the mock inline functions in this TU. */
#include "menora_aceite.c"

/* 4) Test framework. */
#include "querubines_framework.h"


int main(void)
{
    QR_TEST_BEGIN("Oil Management (Aceite)");

    /* ================================================================
     * Design thresholds: compile-time constants exposed by the source.
     * They're defined inside menora_aceite.c but visible here because
     * the .c was #include'd above.
     * ================================================================ */
    QR_SECTION("Umbrales de diseno");

    QR_ASSERT_EQ(MENORA_ACEITE_CRITICO, 25,
                 "Umbral critico = 25");
    QR_ASSERT_EQ(MENORA_ACEITE_OPTIMO, 75,
                 "Umbral optimo = 75");
    QR_ASSERT_EQ(MENORA_ACEITE_LLENO, 100,
                 "Tope lleno = 100");

    /* ================================================================
     * aceite_verificar: pure observation across the three oil zones.
     * ================================================================ */
    QR_SECTION("Verificacion de aceite (Ex 27:20 zaj)");

    int rc;

    /* TEST 4 - normal oil (80): verifier returns OK silently. */
    setup_menora_aceite();
    rc = hashmal_comm_aceite_verificar(HASHMAL_CANAL_GEZA);
    QR_ASSERT(rc == HASHMAL_COMM_OK,
              "Aceite nivel 80 pasa verificacion");

    /* TEST 5 - zero oil: verifier refuses with ERR_ACEITE. */
    setup_menora_aceite();
    mock_menora.nerot[0].aceite_nivel = 0;
    rc = hashmal_comm_aceite_verificar(HASHMAL_CANAL_GEZA);
    QR_ASSERT(rc == HASHMAL_COMM_ERR_ACEITE,
              "Aceite nivel 0 devuelve error");

    /* TEST 6 - low but nonzero (20, <= CRITICO): passes with warn. */
    setup_menora_aceite();
    mock_menora.nerot[0].aceite_nivel = 20;
    rc = hashmal_comm_aceite_verificar(HASHMAL_CANAL_GEZA);
    QR_ASSERT(rc == HASHMAL_COMM_OK,
              "Aceite bajo (20) pasa pero con aviso");

    /* ================================================================
     * aceite_nivel: readout and null-rejection.
     * ================================================================ */
    QR_SECTION("Lectura de nivel");

    /* TEST 7 - readout returns the stored value. */
    setup_menora_aceite();
    mock_menora.nerot[0].aceite_nivel = 55;
    uint8_t nivel = 0;
    rc = hashmal_comm_aceite_nivel(HASHMAL_CANAL_GEZA, &nivel);
    QR_ASSERT(rc == HASHMAL_COMM_OK && nivel == 55,
              "Lectura de nivel devuelve 55");

    /* TEST 8 - null output pointer is rejected up front. */
    rc = hashmal_comm_aceite_nivel(HASHMAL_CANAL_GEZA, NULL);
    QR_ASSERT(rc == HASHMAL_COMM_ERR_NULL,
              "Nivel con NULL rechazado");

    /* ================================================================
     * aceite_suficiente: bool predicate (has oil at all?).
     * ================================================================ */
    QR_SECTION("Suficiencia");

    /* TEST 9 - any positive oil counts as sufficient. */
    setup_menora_aceite();
    QR_ASSERT_TRUE(hashmal_comm_aceite_suficiente(HASHMAL_CANAL_GEZA),
                   "Aceite 80 es suficiente");

    /* TEST 10 - zero is not sufficient. Note: piggybacks on the
     * state left by TEST 9 (setup_menora_aceite already ran); we
     * just flip the geza ner's oil to zero. */
    mock_menora.nerot[0].aceite_nivel = 0;
    QR_ASSERT_FALSE(hashmal_comm_aceite_suficiente(HASHMAL_CANAL_GEZA),
                    "Aceite 0 no es suficiente");

    /* ================================================================
     * duty_cycle_aplicar: the only mutator - oil dictates the flame.
     *   ENCENDIDO + oil <= CRITICO  -> ATENUADO
     *   ATENUADO  + oil >  CRITICO  -> ENCENDIDO
     *   REMOVIDO                    -> untouched (Ap 2:5)
     * ================================================================ */
    QR_SECTION("Duty cycle (Ex 27:21 erev ad boqer)");

    /* TEST 11 - ENCENDIDO with critical oil (exactly 25, <= CRITICO)
     * steps down to ATENUADO. */
    setup_menora_aceite();
    mock_menora.nerot[0].estado        = HASHMAL_NER_ENCENDIDO;
    mock_menora.nerot[0].aceite_nivel  = 25;
    rc = hashmal_comm_duty_cycle_aplicar();
    QR_ASSERT(rc == HASHMAL_COMM_OK,
              "Duty cycle se aplica OK");
    QR_ASSERT(mock_menora.nerot[0].estado == HASHMAL_NER_ATENUADO,
              "ENCENDIDO + aceite critico -> ATENUADO");

    /* TEST 12 - ATENUADO with restored oil (80 > CRITICO) climbs
     * back up to ENCENDIDO. */
    setup_menora_aceite();
    mock_menora.nerot[0].estado        = HASHMAL_NER_ATENUADO;
    mock_menora.nerot[0].aceite_nivel  = 80;
    rc = hashmal_comm_duty_cycle_aplicar();
    QR_ASSERT(rc == HASHMAL_COMM_OK,
              "Duty cycle restitucion OK");
    QR_ASSERT(mock_menora.nerot[0].estado == HASHMAL_NER_ENCENDIDO,
              "ATENUADO + aceite restituido -> ENCENDIDO");

    /* TEST 13 - REMOVIDO is permanent: duty_cycle does not revive it
     * even with oil available. */
    setup_menora_aceite();
    mock_menora.nerot[0].estado        = HASHMAL_NER_REMOVIDO;
    mock_menora.nerot[0].aceite_nivel  = 80;
    rc = hashmal_comm_duty_cycle_aplicar();
    (void)rc;  /* the rc here is not the assertion; state is. */
    QR_ASSERT(mock_menora.nerot[0].estado == HASHMAL_NER_REMOVIDO,
              "REMOVIDO permanece intocable (Ap 2:5)");

    QR_TEST_END();
    return qr_all_passed() ? 0 : 1;
}
