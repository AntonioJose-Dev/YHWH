/*
 * test_routing.c
 * Querubines Test 4 - menora_routing.c (single-hop routing / broadcast
 * / neighbor count). Exercises the routing layer in isolation by
 * stubbing out the internal primitives, the LoRa / ESP-NOW driver
 * entry points, and the mutsaqa selector.
 *
 * Compile and run (from hashmal-firmware/):
 *
 *   gcc -Wall -Wextra \
 *       -I test/querubines/ \
 *       -I test/querubines/test_routing/ \
 *       -I test/querubines/test_consensus/ \
 *       -I components/hashmal_comm/include/ \
 *       -I components/hashmal_comm/src/ \
 *       test/querubines/test_routing/test_routing.c \
 *       -o test/querubines/test_routing/test_routing
 *   ./test/querubines/test_routing/test_routing
 */

/* 1) esp_log mock (reused from test_consensus). Pre-sets ESP-IDF
 *    include guards so esp_log.h becomes a no-op body. */
#include "../test_consensus/mock_esp_log.h"

/* 2) Routing dependency mock: public header + internal.h-guard pre-set
 *    + selector/driver stubs + setup helper. */
#include "mock_routing_deps.h"

/* 3) The real source under test, pulled in inline. */
#include "menora_routing.c"

/* 4) Test framework. */
#include "querubines_framework.h"


int main(void)
{
    QR_TEST_BEGIN("Routing (Geviim Meshuqqadim)");

    /* ================================================================
     * Topology constants: the 22 almond-shaped watchers distribution.
     * ================================================================ */
    QR_SECTION("Constantes de topologia");

    QR_ASSERT_EQ(HASHMAL_MENORA_GEVIIM_TOTAL, 22,
                 "22 vigias almendro en total");
    QR_ASSERT_EQ(HASHMAL_MENORA_GEVIIM_RAMA, 3,
                 "3 vigias por rama");
    QR_ASSERT_EQ(HASHMAL_MENORA_GEVIIM_GEZA, 4,
                 "4 vigias en tronco");
    QR_ASSERT_EQ(HASHMAL_MENORA_GEVIIM_RAMA * HASHMAL_MENORA_RAMAS
                    + HASHMAL_MENORA_GEVIIM_GEZA,
                 22,
                 "3*6+4 = 22 vigias coherente");

    /* ================================================================
     * enviar: the four ner states behave as documented, and success
     * bumps paquetes_tx on the target ner.
     * ================================================================ */
    QR_SECTION("Enviar (Ex 25:37 veheir)");

    uint8_t msg[] = { 0xAA, 0xBB, 0xCC };
    int rc;

    /* TEST 5 - ENCENDIDO: straight success, counter goes up. */
    setup_menora_routing();
    rc = hashmal_comm_enviar(HASHMAL_CANAL_GEZA, msg, 3);
    QR_ASSERT(rc == HASHMAL_COMM_OK,
              "Enviar con ENCENDIDO exitoso");
    QR_ASSERT(mock_menora.nerot[0].paquetes_tx == 1,
              "Contador TX incrementado");

    /* TEST 6 - ATENUADO: still transmits ("ojo debil"), with a
     * WARN log from the source but the return is OK. */
    setup_menora_routing();
    mock_menora.nerot[0].estado = HASHMAL_NER_ATENUADO;
    rc = hashmal_comm_enviar(HASHMAL_CANAL_GEZA, msg, 3);
    QR_ASSERT(rc == HASHMAL_COMM_OK,
              "Enviar con ATENUADO exitoso (ojo debil)");

    /* TEST 7 - APAGADO: rejected with ERR_CANAL. */
    setup_menora_routing();
    mock_menora.nerot[0].estado = HASHMAL_NER_APAGADO;
    rc = hashmal_comm_enviar(HASHMAL_CANAL_GEZA, msg, 3);
    QR_ASSERT(rc == HASHMAL_COMM_ERR_CANAL,
              "Enviar con APAGADO rechazado");

    /* TEST 8 - REMOVIDO: rejected with ERR_REMOVIDO (Ap 2:5). */
    setup_menora_routing();
    mock_menora.nerot[0].estado = HASHMAL_NER_REMOVIDO;
    rc = hashmal_comm_enviar(HASHMAL_CANAL_GEZA, msg, 3);
    QR_ASSERT(rc == HASHMAL_COMM_ERR_REMOVIDO,
              "Enviar con REMOVIDO rechazado (Ap 2:5)");

    /* ================================================================
     * Input validation: NULL message, zero-length, over-length.
     * These checks happen BEFORE any state lookup, so the setup left
     * by the previous test doesn't affect the outcome.
     * ================================================================ */
    QR_SECTION("Validaciones de entrada");

    /* TEST 9 - NULL msg pointer. */
    rc = hashmal_comm_enviar(HASHMAL_CANAL_GEZA, NULL, 3);
    QR_ASSERT(rc == HASHMAL_COMM_ERR_NULL,
              "MSG NULL rechazado");

    /* TEST 10 - zero-byte message. */
    setup_menora_routing();
    rc = hashmal_comm_enviar(HASHMAL_CANAL_GEZA, msg, 0);
    QR_ASSERT(rc == HASHMAL_COMM_ERR_TAMANO,
              "Tam=0 rechazado");

    /* TEST 11 - over the 250-byte LoRa payload ceiling. */
    setup_menora_routing();
    rc = hashmal_comm_enviar(HASHMAL_CANAL_GEZA, msg, 251);
    QR_ASSERT(rc == HASHMAL_COMM_ERR_TAMANO,
              "Tam>250 rechazado");

    /* ================================================================
     * broadcast: loops over the seven nerot, dispatches via enviar,
     * returns OK when at least one ner succeeds, ERR_CANAL when no
     * ner is operative at all.
     * ================================================================ */
    QR_SECTION("Broadcast (Ex 25:37 iluminacion total)");

    /* TEST 12 - all seven ENCENDIDO: broadcast succeeds. */
    setup_menora_routing();
    rc = hashmal_comm_broadcast(msg, 3);
    QR_ASSERT(rc == HASHMAL_COMM_OK,
              "Broadcast exitoso con 7 nerot");

    /* TEST 13 - every ner APAGADO: no operative lamp -> ERR_CANAL. */
    setup_menora_routing();
    for (int i = 0; i < HASHMAL_MENORA_CANALES; i++) {
        mock_menora.nerot[i].estado = HASHMAL_NER_APAGADO;
    }
    rc = hashmal_comm_broadcast(msg, 3);
    QR_ASSERT(rc == HASHMAL_COMM_ERR_CANAL,
              "Broadcast sin nerot operativos falla");

    /* ================================================================
     * vecinos_contar: phase 1 equivalence - neighbors == nerot
     * encendidos. Multi-hop meshing is phase 2.
     * ================================================================ */
    QR_SECTION("Vecinos (fase 1 single-hop)");

    /* TEST 14 - neighbor count mirrors nerot_encendidos. */
    setup_menora_routing();
    mock_menora.nerot_encendidos = 5;
    uint8_t conteo = 0;
    rc = hashmal_comm_vecinos_contar(&conteo);
    QR_ASSERT(rc == HASHMAL_COMM_OK && conteo == 5,
              "Vecinos = nerot_encendidos (fase 1)");

    /* TEST 15 - NULL output pointer rejected up front. */
    rc = hashmal_comm_vecinos_contar(NULL);
    QR_ASSERT(rc == HASHMAL_COMM_ERR_NULL,
              "Conteo NULL rechazado");

    QR_TEST_END();
    return qr_all_passed() ? 0 : 1;
}
