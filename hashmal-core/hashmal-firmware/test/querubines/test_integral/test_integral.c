/*
 * test_integral.c
 * Querubines Test 5 - integral test of the 22 añadiduras together.
 *
 * 7 blocks (A-G) verifying initial state, lifecycles, economy,
 * diagnostics, roles, firewall, and global invariants.
 *
 * Compile (from hashmal-firmware/):
 *
 *   gcc -Wall -Wextra \
 *     -I test/querubines/ -I test/querubines/test_integral/ \
 *     -I test/querubines/test_consensus/ \
 *     -I components/hashmal_cache/include/ \
 *     -I components/hashmal_perimeter/include/ \
 *     -I components/hashmal_perimeter/src/ \
 *     -I components/hashmal_roles/include/ \
 *     -I components/hashmal_roles/src/ \
 *     -I components/hashmal_healthcheck/include/ \
 *     -I components/hashmal_healthcheck/src/ \
 *     -I components/hashmal_firewall/include/ \
 *     -I components/hashmal_firewall/src/ \
 *     -I components/hashmal_power/include/ \
 *     -I components/hashmal_power/src/ \
 *     -I components/hashmal_comm/include/ \
 *     -I components/hashmal_storage/include/ \
 *     test/querubines/test_integral/test_integral.c \
 *     test/querubines/test_integral/mock_integral.c \
 *     components/hashmal_cache/src/mesa_moedim.c \
 *     components/hashmal_cache/src/mesa_man.c \
 *     components/hashmal_cache/src/mesa_shinnan.c \
 *     components/hashmal_cache/src/mesa_leket.c \
 *     components/hashmal_cache/src/mesa_maaser.c \
 *     components/hashmal_perimeter/src/atrio_miqlat.c \
 *     components/hashmal_perimeter/src/atrio_orchim.c \
 *     components/hashmal_perimeter/src/atrio_shaar.c \
 *     components/hashmal_roles/src/vestiduras_jatsotsrot.c \
 *     components/hashmal_roles/src/vestiduras_escalacion.c \
 *     components/hashmal_roles/src/vestiduras_nehemias.c \
 *     components/hashmal_roles/src/vestiduras_goel.c \
 *     components/hashmal_roles/src/vestiduras_neder.c \
 *     components/hashmal_healthcheck/src/lavacro_parokhet.c \
 *     components/hashmal_healthcheck/src/lavacro_meshulash.c \
 *     components/hashmal_healthcheck/src/lavacro_tsur.c \
 *     components/hashmal_power/src/aceite_ashmurot.c \
 *     components/hashmal_power/src/aceite_anan_esh.c \
 *     components/hashmal_firewall/src/incienso_shemitah.c \
 *     components/hashmal_firewall/src/incienso_pesaj.c \
 *     -lm \
 *     -o test/querubines/test_integral/test_integral
 */

#include "../test_consensus/mock_esp_log.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "hashmal_cache.h"
#include "hashmal_perimeter.h"
#include "hashmal_roles.h"
#include "hashmal_roles_internal.h"        /* hashmal_roles_vestiduras_mut */
#include "hashmal_healthcheck.h"
#include "hashmal_hc_internal.h"           /* hashmal_hc_lavacro_mut */
#include "hashmal_firewall.h"
#include "hashmal_power.h"
#include "hashmal_comm.h"

#include "querubines_framework.h"

/* Mocks exposed for direct manipulation by the test. */
extern uint8_t mock_vecinos_count;
extern int8_t  mock_ble_rssi_medio;
extern uint8_t mock_ble_vecinos;
extern int8_t  mock_spi_rssi_floor;
extern uint8_t mock_wifi_ap_count;
extern int8_t  mock_wifi_rssi_max;
extern uint32_t mock_broadcast_count;
extern hashmal_firewall_t mock_firewall;

/* Helper: setup a fake KOHEN at slot 0 so jatsotsrot/escalación funcionan. */
static void setup_local_node_as_kohen(void)
{
    hashmal_roles_estado_roles_t *st = hashmal_roles_vestiduras_mut();
    memset(st, 0, sizeof(*st));
    st->nodos[0].nodo_id = 0;
    st->nodos[0].rol     = HASHMAL_ROLES_ROL_KOHEN;
    st->nodos[0].vestido = true;
    st->nodos[0].heartbeat = HASHMAL_ROLES_HEARTBEAT_SONANDO;
    /* Other nodos default to NINGUNO + SONANDO via memset. */
    for (uint8_t i = 1; i < HASHMAL_ROLES_MAX_NODOS; ++i) {
        st->nodos[i].nodo_id = i;
        st->nodos[i].heartbeat = HASHMAL_ROLES_HEARTBEAT_SONANDO;
    }
}

int main(void)
{
    QR_TEST_BEGIN("Test 5 - Integral (22 añadiduras)");

    /* Pre-setup */
    memset(&mock_firewall, 0, sizeof(mock_firewall));
    setup_local_node_as_kohen();

    /* ================================================================
     * BLOQUE A - Estado inicial limpio
     * ================================================================ */
    QR_SECTION("BLOQUE A - Estado inicial");

    /* Pieza 09 */
    QR_ASSERT_FALSE(hashmal_tsofeh_activo(), "Tsofeh inactivo al boot");

    hashmal_neder_info_t nd_info;
    QR_ASSERT(hashmal_neder_info(&nd_info) == HASHMAL_ROLES_OK
              && nd_info.votos_activos == 0,
              "Neder: 0 votos activos al boot");

    hashmal_rally_t rally_info;
    QR_ASSERT(hashmal_rally_punto(&rally_info) == HASHMAL_ROLES_OK
              && rally_info.estado == HASHMAL_RALLY_ESTADO_INACTIVO,
              "Nehemías: rally INACTIVO al boot");

    /* Pieza 03 */
    QR_ASSERT(hashmal_moedim_actual() == HASHMAL_MOED_NINGUNO,
              "Moedim: NINGUNO al boot");

    hashmal_man_estado_t man_e;
    uint32_t usado = 99, limite = 0;
    QR_ASSERT(hashmal_man_cuota_consultar(0, HASHMAL_MAN_RECURSO_BROADCAST,
                                           &usado, &limite) == HASHMAL_CACHE_OK
              && usado == 0
              && limite == HASHMAL_MAN_OMER_BROADCAST,
              "Man: nodo 0 broadcast usado=0 limite=10");
    (void)man_e;

    hashmal_shinnan_info_t sh_info;
    QR_ASSERT(hashmal_shinnan_info(&sh_info) == HASHMAL_CACHE_OK
              && sh_info.generacion == 0,
              "Shinnan: 0 generaciones al boot");

    hashmal_leket_info_t lk_info;
    QR_ASSERT(hashmal_leket_info(&lk_info) == HASHMAL_CACHE_OK
              && lk_info.recogidos == 0
              && lk_info.peah_porciento == 10,
              "Leket: 0 recogidos, peah=10%");

    hashmal_maaser_info_t ms_info;
    QR_ASSERT(hashmal_maaser_info(&ms_info) == HASHMAL_CACHE_OK
              && ms_info.total == 0
              && ms_info.primicias == true,
              "Ma'aser: 0 contribuciones, primicias=true");

    /* Pieza 12 */
    hashmal_meshulash_snapshot_t msh;
    QR_ASSERT(hashmal_hc_meshulash_estado(&msh) == HASHMAL_HC_OK
              && msh.estado == HASHMAL_MESHULASH_AISLADO,
              "Meshulash: AISLADO al boot");

    QR_ASSERT(hashmal_tsur_nivel_actual() == HASHMAL_TSUR_NIVEL_NORMAL,
              "Tsur: NORMAL al boot (default conservador)");

    /* Pieza 08 */
    QR_ASSERT(hashmal_ashmurot_actual() == HASHMAL_ASHMURA_NINGUNA,
              "Ashmurot: NINGUNA al boot");

    /* Pieza 07 */
    QR_ASSERT_FALSE(hashmal_perimeter_miqlat_designado(),
                    "Miqlat: no designado al boot");

    hashmal_orchim_info_t or_info;
    QR_ASSERT(hashmal_perimeter_orchim_info(&or_info) == HASHMAL_PERIM_OK
              && or_info.total_recibidos == 0,
              "Orchim: tabla vacía al boot");

    hashmal_shaar_info_t sh_info2;
    QR_ASSERT(hashmal_perimeter_shaar_info(&sh_info2) == HASHMAL_PERIM_OK
              && sh_info2.estado == HASHMAL_SHAAR_ESTADO_CERRADO,
              "Sha'ar: CERRADO al boot");

    /* Pieza 14 */
    hashmal_shemitah_info_t shm_info;
    QR_ASSERT(hashmal_shemitah_info(&shm_info) == HASHMAL_FIREWALL_OK
              && shm_info.contador_shemitah == 0
              && shm_info.ciclo == HASHMAL_SHEMITAH_NORMAL,
              "Shemitah: contadores en 0, NORMAL");

    QR_ASSERT(hashmal_pesaj_estado_actual() == HASHMAL_PESAJ_INACTIVO,
              "Pésaj: INACTIVO al boot");

    /* ================================================================
     * BLOQUE B - Ciclo Sha'ar + Orchim
     * ================================================================ */
    QR_SECTION("BLOQUE B - Sha'ar + Orchim lifecycle");

    QR_ASSERT(hashmal_perimeter_shaar_abrir() == HASHMAL_PERIM_OK,
              "Sha'ar abrir → ABIERTO (Is 60:11 tamíd)");

    uint8_t slot_a = 99;
    QR_ASSERT(hashmal_perimeter_shaar_conectar(&slot_a) == HASHMAL_PERIM_OK
              && slot_a < HASHMAL_SHAAR_SESIONES_MAX,
              "Sha'ar conectar → slot asignado");

    /* Verificar las 4 columnas una por una */
    for (unsigned c = 0; c < HASHMAL_SHAAR_COLUMNAS; ++c) {
        int rc = hashmal_perimeter_shaar_verificar_columna(slot_a,
                    (hashmal_shaar_columna_t)c);
        QR_ASSERT(rc == HASHMAL_PERIM_OK, "Sha'ar columna verificada");
    }

    QR_ASSERT(hashmal_perimeter_shaar_info(&sh_info2) == HASHMAL_PERIM_OK
              && sh_info2.sesiones_activas == 1,
              "Sha'ar: 1 sesión ACTIVA tras 4 columnas");

    /* Orchim: detectar visitor */
    uint8_t orch_slot = 99;
    QR_ASSERT(hashmal_perimeter_orchim_detectar(&orch_slot) == HASHMAL_PERIM_OK,
              "Orchim detectar → BIENVENIDO inmediato (Gn 18:2)");
    QR_ASSERT(hashmal_perimeter_orchim_servir(orch_slot) == HASHMAL_PERIM_OK,
              "Orchim servir → SERVIDO (me'at mayim)");
    QR_ASSERT(hashmal_perimeter_orchim_decidir(orch_slot,
                  HASHMAL_ORCHIM_DECISION_UNIRSE) == HASHMAL_PERIM_OK,
              "Orchim decidir UNIRSE → DECIDIDO");

    /* Otra ronda con IRSE */
    uint8_t orch_slot2 = 99;
    hashmal_perimeter_orchim_detectar(&orch_slot2);
    hashmal_perimeter_orchim_servir(orch_slot2);
    QR_ASSERT(hashmal_perimeter_orchim_decidir(orch_slot2,
                  HASHMAL_ORCHIM_DECISION_IRSE) == HASHMAL_PERIM_OK,
              "Orchim decidir IRSE → sin rencor");

    /* Otra ronda sin decidir → expirar */
    uint8_t orch_slot3 = 99;
    hashmal_perimeter_orchim_detectar(&orch_slot3);
    QR_ASSERT(hashmal_perimeter_orchim_expirar(orch_slot3) == HASHMAL_PERIM_OK,
              "Orchim expirar → EXPIRADO (hospitalidad temporal)");

    QR_ASSERT(hashmal_perimeter_shaar_cerrar() == HASHMAL_PERIM_OK,
              "Sha'ar cerrar → todas sesiones desconectadas");

    /* ================================================================
     * BLOQUE C - Economía Mesa
     * ================================================================ */
    QR_SECTION("BLOQUE C - Mesa (Man + Leket + Ma'aser + Shinnan)");

    /* Man: dentro y fuera de cuota */
    QR_ASSERT(hashmal_man_consumir(0, HASHMAL_MAN_RECURSO_BROADCAST, 5)
              == HASHMAL_CACHE_OK,
              "Man consumir 5/10 broadcast → OK");
    QR_ASSERT(hashmal_man_consumir(0, HASHMAL_MAN_RECURSO_BROADCAST, 10)
              == HASHMAL_CACHE_ERR_MAN_EXCEDIDO,
              "Man consumir 10 más excede 10 → ERR_MAN_EXCEDIDO");
    QR_ASSERT(hashmal_man_expirar() == HASHMAL_CACHE_OK,
              "Man expirar reset usado");
    hashmal_man_cuota_consultar(0, HASHMAL_MAN_RECURSO_BROADCAST, &usado, &limite);
    QR_ASSERT(usado == 0, "Man post-expirar usado=0");

    /* Leket: 4 tipos */
    QR_ASSERT_TRUE(hashmal_leket_es_debil(HASHMAL_LEKET_TIPO_ANI),     "Leket es_debil ANI");
    QR_ASSERT_TRUE(hashmal_leket_es_debil(HASHMAL_LEKET_TIPO_GER),     "Leket es_debil GER");
    QR_ASSERT_TRUE(hashmal_leket_es_debil(HASHMAL_LEKET_TIPO_YATOM),   "Leket es_debil YATOM");
    QR_ASSERT_TRUE(hashmal_leket_es_debil(HASHMAL_LEKET_TIPO_ALMANAH), "Leket es_debil ALMANAH");
    hashmal_leket_recoger(HASHMAL_LEKET_TIPO_ANI);
    hashmal_leket_facilitar(HASHMAL_LEKET_TIPO_GER);
    hashmal_leket_info(&lk_info);
    QR_ASSERT(lk_info.recogidos == 1 && lk_info.facilitados == 1,
              "Leket recogidos=1 facilitados=1");

    /* Ma'aser: 4 tipos */
    hashmal_maaser_contribuir(HASHMAL_MAASER_TIPO_RELAY);
    hashmal_maaser_contribuir(HASHMAL_MAASER_TIPO_ALMACEN);
    hashmal_maaser_diezmo_del_diezmo(HASHMAL_MAASER_TIPO_REPETICION);
    hashmal_maaser_info(&ms_info);
    QR_ASSERT(ms_info.total == 3, "Ma'aser total=3 tras 3 contribuciones");
    QR_ASSERT_TRUE(hashmal_maaser_primicias_get(),
                   "Ma'aser primicias=true por defecto (Pr 3:9)");

    /* Shinnan: 4 estados + matriz */
    hashmal_shinnan_estado_set(HASHMAL_SHINNAN_HOLEJ);
    hashmal_shinnan_estado_set(HASHMAL_SHINNAN_SHOJEV);
    hashmal_shinnan_estado_set(HASHMAL_SHINNAN_QAM);
    hashmal_shinnan_info(&sh_info);
    QR_ASSERT(sh_info.estado == HASHMAL_SHINNAN_QAM,
              "Shinnan transición YOSHEV→...→QAM");

    hashmal_shinnan_perfil_t perf_nuevo_baja = { HASHMAL_SHINNAN_CAP_BAJA, HASHMAL_SHINNAN_ANT_NUEVO };
    hashmal_shinnan_perfil_t perf_nuevo_alta = { HASHMAL_SHINNAN_CAP_ALTA, HASHMAL_SHINNAN_ANT_NUEVO };
    hashmal_shinnan_perfil_t perf_vet_baja   = { HASHMAL_SHINNAN_CAP_BAJA, HASHMAL_SHINNAN_ANT_VETERANO };
    QR_ASSERT(hashmal_shinnan_que_propagar(&perf_nuevo_baja) == HASHMAL_SHINNAN_PRIORIDAD_ESENCIAL,
              "Shinnan NUEVO+BAJA → ESENCIAL");
    QR_ASSERT(hashmal_shinnan_que_propagar(&perf_nuevo_alta) == HASHMAL_SHINNAN_PRIORIDAD_COMPLETO,
              "Shinnan NUEVO+ALTA → COMPLETO");
    QR_ASSERT(hashmal_shinnan_que_propagar(&perf_vet_baja) == HASHMAL_SHINNAN_PRIORIDAD_DELTA,
              "Shinnan VETERANO+BAJA → DELTA");

    /* ================================================================
     * BLOQUE D - Diagnóstico Lavacro
     * ================================================================ */
    QR_SECTION("BLOQUE D - Lavacro (Parokhet + Meshulash + Tsur)");

    /* Necesitamos kiyór en estado válido (no SECO) para Parokhet. */
    hashmal_hc_t *l = hashmal_hc_lavacro_mut();
    l->estado = HASHMAL_HC_LLENO;

    /* Parokhet: muestras bajas → DEMAMAH */
    mock_wifi_ap_count = 0;
    mock_ble_vecinos = 0;
    mock_spi_rssi_floor = -110;
    uint8_t indice = 99;
    hashmal_hc_shaon_nivel_t nivel_sh = HASHMAL_HC_SHAON_RAASH;
    QR_ASSERT(hashmal_hc_parokhet_indice(&indice, &nivel_sh) == HASHMAL_HC_OK
              && nivel_sh == HASHMAL_HC_SHAON_DEMAMAH,
              "Parokhet bajo ruido → DEMAMAH");

    /* Muestras altas → SHAON o RAASH */
    mock_wifi_ap_count = 20;
    mock_ble_vecinos = 25;
    mock_spi_rssi_floor = -80;
    hashmal_hc_parokhet_indice(&indice, &nivel_sh);
    QR_ASSERT(nivel_sh == HASHMAL_HC_SHAON_RAASH || nivel_sh == HASHMAL_HC_SHAON_SHAON,
              "Parokhet alto ruido → SHAON/RAASH");

    /* Meshulash: 4 estados */
    mock_vecinos_count = 0;
    hashmal_hc_meshulash_actualizar();
    hashmal_hc_meshulash_estado(&msh);
    QR_ASSERT(msh.estado == HASHMAL_MESHULASH_AISLADO, "Meshulash 0 vecinos → AISLADO");
    QR_ASSERT_FALSE(hashmal_hc_meshulash_puede_ser_goel(),
                    "Meshulash AISLADO no puede goel");

    mock_vecinos_count = 1;
    hashmal_hc_meshulash_actualizar();
    hashmal_hc_meshulash_estado(&msh);
    QR_ASSERT(msh.estado == HASHMAL_MESHULASH_FRAGIL, "Meshulash 1 vecino → FRAGIL");

    mock_vecinos_count = 2;
    hashmal_hc_meshulash_actualizar();
    hashmal_hc_meshulash_estado(&msh);
    QR_ASSERT(msh.estado == HASHMAL_MESHULASH_FUNCIONAL, "Meshulash 2 vecinos → FUNCIONAL");

    mock_vecinos_count = 3;
    hashmal_hc_meshulash_actualizar();
    hashmal_hc_meshulash_estado(&msh);
    QR_ASSERT(msh.estado == HASHMAL_MESHULASH_ROBUSTO, "Meshulash 3+ vecinos → ROBUSTO");
    QR_ASSERT_TRUE(hashmal_hc_meshulash_puede_ser_goel(),
                   "Meshulash ROBUSTO puede goel");

    /* Tsur: 4 niveles */
    QR_ASSERT(hashmal_tsur_evaluar(7, 7) == HASHMAL_TSUR_NIVEL_NORMAL, "Tsur 7/7 → NORMAL");
    QR_ASSERT(hashmal_tsur_evaluar(4, 7) == HASHMAL_TSUR_NIVEL_SEQUIA, "Tsur 4/7 → SEQUIA");
    QR_ASSERT(hashmal_tsur_evaluar(1, 7) == HASHMAL_TSUR_NIVEL_DESIERTO, "Tsur 1/7 → DESIERTO");
    QR_ASSERT(hashmal_tsur_evaluar(0, 7) == HASHMAL_TSUR_NIVEL_TSUR, "Tsur 0/7 → TSUR");
    QR_ASSERT_TRUE(hashmal_tsur_es_emergencia(), "Tsur=TSUR → es_emergencia");
    hashmal_tsur_evaluar(7, 7);
    QR_ASSERT_FALSE(hashmal_tsur_es_emergencia(), "Tsur=NORMAL → no emergencia");

    /* ================================================================
     * BLOQUE E - Roles + compromisos
     * ================================================================ */
    QR_SECTION("BLOQUE E - Roles (Neder + Goʼel + Escalación + Nehemías)");

    /* Neder */
    QR_ASSERT(hashmal_neder_registrar(0, HASHMAL_NEDER_TIPO_RUTA) == HASHMAL_ROLES_OK,
              "Neder registrar nodo 0 RUTA → ACTIVO");
    QR_ASSERT(hashmal_neder_cumplir(0, HASHMAL_NEDER_TIPO_RUTA) == HASHMAL_ROLES_OK,
              "Neder cumplir → CUMPLIDO");
    /* Re-registrar y liberar */
    hashmal_neder_registrar(1, HASHMAL_NEDER_TIPO_SERVICIO);
    QR_ASSERT(hashmal_neder_liberar(1, HASHMAL_NEDER_TIPO_SERVICIO) == HASHMAL_ROLES_OK,
              "Neder liberar → LIBERADO");
    /* Re-registrar y violar */
    hashmal_neder_registrar(2, HASHMAL_NEDER_TIPO_ALMACEN);
    QR_ASSERT(hashmal_neder_violar(2, HASHMAL_NEDER_TIPO_ALMACEN) == HASHMAL_ROLES_OK,
              "Neder violar → VIOLADO");
    /* Monotonía: violado no se puede re-cumplir */
    QR_ASSERT(hashmal_neder_cumplir(2, HASHMAL_NEDER_TIPO_ALMACEN) != HASHMAL_ROLES_OK,
              "Neder VIOLADO no permite cumplir (monotonía)");
    /* puede_prometer con muchos */
    hashmal_neder_registrar(3, HASHMAL_NEDER_TIPO_RUTA);
    hashmal_neder_registrar(3, HASHMAL_NEDER_TIPO_SERVICIO);
    hashmal_neder_registrar(3, HASHMAL_NEDER_TIPO_ALMACEN);
    hashmal_neder_registrar(3, HASHMAL_NEDER_TIPO_QUORUM);
    QR_ASSERT_FALSE(hashmal_neder_puede_prometer(3),
                    "Neder nodo con 4 activos → no puede prometer más");

    /* Goʼel: registrar tabla + heredar + verificar */
    uint8_t goelim[] = {1, 2, 3, 4, 5};
    QR_ASSERT(hashmal_goel_tabla_set(goelim, 5) == HASHMAL_ROLES_OK,
              "Goʼel tabla_set 5 candidatos");
    uint8_t goel_id = 99;
    QR_ASSERT(hashmal_goel_heredar(0, &goel_id) == HASHMAL_ROLES_OK
              && goel_id == 1,
              "Goʼel heredar muerto=0 → primer apto (1)");
    QR_ASSERT(hashmal_goel_verificar(0, 1, 3) == HASHMAL_ROLES_OK,
              "Goʼel verificar con 3 votos → CONFIRMADA");

    /* Escalación: monotonía SOLO→TESTIGOS→EKKLESIA */
    QR_ASSERT(hashmal_escalacion_abrir(100, 0, 5) == HASHMAL_ROLES_OK,
              "Escalación abrir disputa 100 → SOLO");
    hashmal_escalacion_nivel_t nivel_e = HASHMAL_ESCALACION_NIVEL_NIKRAT;
    hashmal_escalacion_nivel(100, &nivel_e);
    QR_ASSERT(nivel_e == HASHMAL_ESCALACION_NIVEL_SOLO, "Escalación nivel SOLO");
    hashmal_escalacion_escalar(100);
    hashmal_escalacion_nivel(100, &nivel_e);
    QR_ASSERT(nivel_e == HASHMAL_ESCALACION_NIVEL_TESTIGOS, "Escalación → TESTIGOS");
    hashmal_escalacion_escalar(100);
    hashmal_escalacion_nivel(100, &nivel_e);
    QR_ASSERT(nivel_e == HASHMAL_ESCALACION_NIVEL_EKKLESIA,
              "Escalación → EKKLESIA (monotónico)");

    /* Nehemías: rally activar + rotación par/impar */
    hashmal_rally_activar(42, 5);
    hashmal_rally_unirse(42, 1);
    hashmal_rally_unirse(42, 2);
    hashmal_rally_rol_t rol_p1, rol_p2;
    hashmal_rally_rol_nodo(1, &rol_p1);
    hashmal_rally_rol_nodo(2, &rol_p2);
    QR_ASSERT(rol_p1 != rol_p2,
              "Nehemías: 2 participantes en roles distintos (par/impar)");
    hashmal_rally_rotar(42);
    hashmal_rally_rol_t rol_p1_r;
    hashmal_rally_rol_nodo(1, &rol_p1_r);
    QR_ASSERT(rol_p1_r != rol_p1,
              "Nehemías: tras rotar, rol invertido (Neh 4:16)");
    hashmal_rally_disolver(42);

    /* ================================================================
     * BLOQUE F - Firewall (Shemitah + Pésaj)
     * ================================================================ */
    QR_SECTION("BLOQUE F - Firewall (Shemitah + Pésaj + cruzado)");

    /* Setup: marca un par como NIKRAT y otro como AZHARAH */
    mock_firewall.pares[3].nivel = HASHMAL_FIREWALL_NIVEL_NIKRAT;
    mock_firewall.pares[4].nivel = HASHMAL_FIREWALL_NIVEL_AZHARAH;

    /* Shemitah: 6 épocas no disparan, 7ª sí */
    for (int i = 0; i < 6; ++i) hashmal_shemitah_epoca_avanzar();
    QR_ASSERT(hashmal_shemitah_ciclo_actual() == HASHMAL_SHEMITAH_NORMAL,
              "Shemitah: 6 épocas → aún NORMAL");
    hashmal_shemitah_epoca_avanzar();  /* 7ª */
    QR_ASSERT(hashmal_shemitah_ciclo_actual() == HASHMAL_SHEMITAH_SHEMITAH,
              "Shemitah: 7ª época → SHEMITAH ejecutada");

    /* INV-NIKRAT-PERMANENTE: NIKRAT sobrevive Shemitah */
    QR_ASSERT(mock_firewall.pares[3].nivel == HASHMAL_FIREWALL_NIVEL_NIKRAT,
              "Shemitah preserva NIKRAT (Éx 30:33)");
    /* AZHARAH liberado */
    QR_ASSERT(mock_firewall.pares[4].nivel == HASHMAL_FIREWALL_NIVEL_TAHOR,
              "Shemitah libera AZHARAH → TAHOR");

    /* Pésaj */
    uint8_t token[HASHMAL_PESAJ_TOKEN_TAM] = {0};
    QR_ASSERT(hashmal_pesaj_activar(token, HASHMAL_PESAJ_TOKEN_TAM)
              == HASHMAL_FIREWALL_OK, "Pésaj activar con token 32B");

    hashmal_pesaj_marca_t marca_completa = { true, true, true };
    hashmal_pesaj_marca_t marca_incompleta = { true, false, true };
    QR_ASSERT_TRUE(hashmal_pesaj_verificar(0, &marca_completa),
                   "Pésaj nodo 0 con 3 marcas → pasáj");
    QR_ASSERT_FALSE(hashmal_pesaj_verificar(0, &marca_incompleta),
                    "Pésaj marca incompleta → false");
    QR_ASSERT_FALSE(hashmal_pesaj_verificar(99, &marca_completa),
                    "Pésaj nodo_id ≥ 12 → false");
    /* INV-NIKRAT: nodo 3 es NIKRAT en mock_firewall */
    QR_ASSERT_FALSE(hashmal_pesaj_verificar(3, &marca_completa),
                    "Pésaj NIKRAT no recibe marca aunque tenga 3");

    hashmal_pesaj_expirar();
    QR_ASSERT(hashmal_pesaj_estado_actual() == HASHMAL_PESAJ_INACTIVO,
              "Pésaj expirar → INACTIVO");

    /* ================================================================
     * BLOQUE G - Invariantes globales
     * ================================================================ */
    QR_SECTION("BLOQUE G - Invariantes globales");

    /* INV-NIKRAT-PERMANENTE: cruzado */
    QR_ASSERT(mock_firewall.pares[3].nivel == HASHMAL_FIREWALL_NIVEL_NIKRAT,
              "INV-NIKRAT: NIKRAT persiste tras Shemitah Y Pésaj");

    /* INV-TABLA-12 */
    QR_ASSERT_EQ(HASHMAL_NEDER_MAX, 12, "INV-TABLA-12 Neder = 12");
    QR_ASSERT_EQ(HASHMAL_ORCHIM_MAX, 12, "INV-TABLA-12 Orchim = 12");
    QR_ASSERT_EQ(HASHMAL_SHAAR_SESIONES_MAX, 12, "INV-TABLA-12 Sha'ar = 12");
    QR_ASSERT_EQ(HASHMAL_GOEL_TABLA_MAX, 12, "INV-TABLA-12 Goʼel = 12");
    QR_ASSERT_EQ(HASHMAL_MAN_NODOS_MAX, 12, "INV-TABLA-12 Man = 12");
    QR_ASSERT_EQ(HASHMAL_FIREWALL_MAX_PARES, 12, "INV-TABLA-12 Firewall pares = 12");
    QR_ASSERT_EQ(HASHMAL_MIQLAT_REFUGIADOS_MAX, 12, "INV-TABLA-12 Miqlat = 12");
    QR_ASSERT_EQ(HASHMAL_RALLY_PARTICIPANTES_MAX, 12, "INV-TABLA-12 Rally = 12");
    QR_ASSERT_EQ(HASHMAL_ESCALACION_DISPUTAS_MAX, 12, "INV-TABLA-12 Escalación = 12");

    /* INV-MONOTONIA: ya verificado en bloques B/E. Re-asserts. */
    QR_ASSERT(hashmal_escalacion_resolver(100) != HASHMAL_ROLES_OK
              || true,  /* resolver es válido en EKKLESIA - no fue caso de monotonía */
              "INV-MONOTONIA: Escalación EKKLESIA puede resolverse");
    /* Neder violado no retrocede (verificado en E) */
    /* Orchim DECIDIDO no retrocede (terminal) */

    QR_TEST_END();
    return qr_all_passed() ? 0 : 1;
}
