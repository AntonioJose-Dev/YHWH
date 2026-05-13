/*
 * test_beruji.c
 * Querubines Test 2 for hashmal_comm - transport selector (beruji).
 * Tests menora_mutsaqot.c in isolation: the selector that chooses
 * which of the three mutsaqot (LoRa / ESP-NOW / BLE) carries each
 * message, by adequacy rather than raw power (Zac 4:6).
 *
 * Compile and run (from hashmal-firmware/):
 *
 *   gcc -Wall -Wextra \
 *       -I test/querubines/ \
 *       -I test/querubines/test_beruji/ \
 *       -I test/querubines/test_consensus/ \
 *       -I components/hashmal_comm/include/ \
 *       -I components/hashmal_comm/src/ \
 *       test/querubines/test_beruji/test_beruji.c \
 *       -o test/querubines/test_beruji/test_beruji
 *   ./test/querubines/test_beruji/test_beruji
 *
 * The esp_log.h mock is reused from the test_consensus tree: the
 * stub esp_log.h created there already lives on the include path
 * (-I test/querubines/test_consensus/), and mock_esp_log.h is pulled
 * in explicitly here below.
 */

/* 1) esp_log mock - reuse the one from test_consensus. This also
 *    pre-sets the ESP-IDF include-guard so any later
 *    #include "esp_log.h" becomes a no-op at the body level. */
#include "../test_consensus/mock_esp_log.h"

/* 2) Our hashmal_comm mock. It sets HASHMAL_COMM_INTERNAL_H before
 *    the real internal.h can contribute anything, and it defines the
 *    four internal-API functions that menora_mutsaqot.c needs. It
 *    also drags in the real hashmal_comm.h for enums/types/macros. */
#include "mock_hashmal_comm.h"

/* 3) The real source under test, pulled in inline. All ESP_LOGx go
 *    through the mock to printf; all hashmal_comm_* internal calls
 *    go through the mock inline functions. No linker dependency on
 *    sibling sources (menora_hub.c, menora_canales.c, etc.). */
#include "menora_mutsaqot.c"

/* 4) Test framework. */
#include "querubines_framework.h"


int main(void)
{
    QR_TEST_BEGIN("Transport Selector (Beruji)");

    /* ================================================================
     * Compile-time constants that encode the design.
     * ================================================================ */
    QR_SECTION("Constantes de diseno");

    QR_ASSERT_EQ(HASHMAL_MUTSAQA_TOTAL, 3,
                 "Exactamente 3 conductos por canal");
    QR_ASSERT_EQ(HASHMAL_MENORA_CANALES, 7,
                 "7 canales (shiva nerot)");

    QR_ASSERT(MUTS_BLE_ACEITE_MIN < MUTS_LORA_ACEITE_MIN,
              "BLE requiere menos aceite que LoRa");
    QR_ASSERT(MUTS_BLE_ACEITE_MIN < MUTS_ESPNOW_ACEITE_MIN,
              "BLE requiere menos aceite que ESP-NOW");

    /* ================================================================
     * Positive selection paths: the selector picks the lowest-
     * consumption viable mutsaqa for the given size.
     * ================================================================ */
    QR_SECTION("Seleccion beruji (menor consumo)");

    hashmal_mutsaqa_id_t elegida;
    int rc;

    /* TEST 4 - all three available, message small: BLE wins (15 mW). */
    setup_menora_completa();
    rc = hashmal_comm_mutsaqa_seleccionar(HASHMAL_CANAL_GEZA, 1, 100, &elegida);
    QR_ASSERT(rc == HASHMAL_COMM_OK,
              "Seleccion exitosa con todo disponible");
    QR_ASSERT(elegida == HASHMAL_MUTSAQA_BLE,
              "Elige BLE (menor consumo) - beruji");

    /* TEST 5 - message > BLE max (200): BLE drops out, ESP-NOW wins
     * (80 mW < LoRa 120 mW). */
    setup_menora_completa();
    rc = hashmal_comm_mutsaqa_seleccionar(HASHMAL_CANAL_GEZA, 1, 201, &elegida);
    QR_ASSERT(rc == HASHMAL_COMM_OK,
              "Seleccion exitosa con mensaje grande");
    QR_ASSERT(elegida == HASHMAL_MUTSAQA_ESP_NOW,
              "Mensaje grande salta BLE, elige ESP-NOW");

    /* TEST 6 - oil so low that NONE of the three passes the strict
     * beruji filter (aceite < every MIN: < 10 BLE, < 20 LoRa, < 40
     * ESP-NOW). Fallback tamid then picks LoRa because it is the
     * geza / spine (Ex 27:21 - juqqat olam).
     *
     * NOTE: the task prompt suggested aceite_nivel = 15, but with
     * MUTS_BLE_ACEITE_MIN = 10 that value still lets BLE pass and
     * no fallback fires. Using aceite_nivel = 5 so the scenario
     * actually matches the assertion ("only LoRa reaches", i.e.
     * via the tamid fallback). */
    setup_menora_completa();
    for (int i = 0; i < HASHMAL_MENORA_CANALES; i++) {
        mock_menora.nerot[i].aceite_nivel = 5;
    }
    rc = hashmal_comm_mutsaqa_seleccionar(HASHMAL_CANAL_GEZA, 1, 100, &elegida);
    QR_ASSERT(rc == HASHMAL_COMM_OK,
              "Con aceite bajo, fallback LoRa tamid funciona");
    QR_ASSERT(elegida == HASHMAL_MUTSAQA_LORA,
              "Fallback a LoRa (tamid) con aceite bajo");

    /* ================================================================
     * Negative paths: malformed inputs / no viable transport.
     * ================================================================ */
    QR_SECTION("Pruebas negativas");

    /* TEST 7 - every mutsaqa of every ner unavailable: even LoRa
     * fallback cannot fire. Selector must refuse cleanly. */
    setup_menora_completa();
    for (int i = 0; i < HASHMAL_MENORA_CANALES; i++) {
        for (int j = 0; j < HASHMAL_MUTSAQA_TOTAL; j++) {
            mock_menora.nerot[i].mutsaqot[j].disponible = false;
        }
    }
    rc = hashmal_comm_mutsaqa_seleccionar(HASHMAL_CANAL_GEZA, 1, 100, &elegida);
    QR_ASSERT(rc == HASHMAL_COMM_ERR_SIN_MUTSAQA,
              "Sin mutsaqot disponibles, falla correctamente");

    /* TEST 8 - NULL output pointer is rejected before anything else. */
    rc = hashmal_comm_mutsaqa_seleccionar(HASHMAL_CANAL_GEZA, 1, 100, NULL);
    QR_ASSERT(rc == HASHMAL_COMM_ERR_NULL,
              "Parametro NULL rechazado");

    /* TEST 9 - tam_mensaje = 0 is a zero-byte message, refused. */
    setup_menora_completa();
    rc = hashmal_comm_mutsaqa_seleccionar(HASHMAL_CANAL_GEZA, 1, 0, &elegida);
    QR_ASSERT(rc == HASHMAL_COMM_ERR_TAMANO,
              "Mensaje vacio rechazado");

    QR_TEST_END();
    return qr_all_passed() ? 0 : 1;
}
