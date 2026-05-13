/*
 * mock_integral.c
 * All cross-piece stubs needed for the integral Querubines test 5.
 *
 * Provides external-linkage stubs for:
 *   - ahora_ms() helpers (one per piece) - simple monotonic counters
 *   - mut/get accessors for piece singletons - real local instances
 *   - cross-piece public APIs (comm, roles, firewall) - controllable stubs
 *
 * Test harness can manipulate the exposed mock_* globals to simulate
 * different scenarios (e.g., set mock_vecinos_count = 5 before calling
 * hashmal_hc_meshulash_actualizar).
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "hashmal_perimeter.h"
#include "hashmal_perim_internal.h"        /* hashmal_perimeter_t (struct) */
#include "hashmal_roles.h"
#include "hashmal_healthcheck.h"
#include "hashmal_firewall.h"
#include "hashmal_power.h"
#include "hashmal_cache.h"
#include "hashmal_comm.h"

/* ==================================================================
 * Monotonic counters for ahora_ms() helpers
 * ================================================================== */
static int64_t g_mock_clock_ms = 1000;

uint64_t hashmal_perim_ahora_ms(void)         { return (uint64_t)(g_mock_clock_ms += 1); }
int64_t  hashmal_roles_ahora_ms(void)         { return            (g_mock_clock_ms += 1); }
uint64_t hashmal_hc_ahora_ms(void)            { return (uint64_t)(g_mock_clock_ms += 1); }
int64_t  hashmal_firewall_ahora_ms(void)      { return            (g_mock_clock_ms += 1); }
int64_t  hashmal_power_ahora_ms(void)         { return            (g_mock_clock_ms += 1); }

/* ==================================================================
 * Singleton instances for mut/get accessors
 * ================================================================== */
static hashmal_perimeter_t        g_mock_atrio;
static hashmal_roles_estado_roles_t g_mock_vestiduras;
static hashmal_hc_t               g_mock_kiyor;
hashmal_firewall_t                mock_firewall;          /* exposed to test */
static hashmal_power_estado_energia_t g_mock_aceite;
static hashmal_menora_t           g_mock_menora;

hashmal_perimeter_t *hashmal_perim_atrio_mut(void)             { return &g_mock_atrio; }
const hashmal_perimeter_t *hashmal_perim_get_atrio_interno(void) { return &g_mock_atrio; }

hashmal_roles_estado_roles_t *hashmal_roles_vestiduras_mut(void) { return &g_mock_vestiduras; }
const hashmal_roles_estado_roles_t *hashmal_roles_get_vestiduras_interno(void) { return &g_mock_vestiduras; }

hashmal_hc_t *hashmal_hc_lavacro_mut(void)            { return &g_mock_kiyor; }
const hashmal_hc_t *hashmal_hc_get_lavacro_interno(void) { return &g_mock_kiyor; }

hashmal_firewall_t *hashmal_firewall_incienso_mut(void) { return &mock_firewall; }
const hashmal_firewall_t *hashmal_firewall_get_incienso_interno(void) { return &mock_firewall; }
const hashmal_firewall_t *hashmal_firewall_get_estado(void) { return &mock_firewall; }

hashmal_power_estado_energia_t *hashmal_power_aceite_mut(void) { return &g_mock_aceite; }
const hashmal_power_estado_energia_t *hashmal_power_get_aceite_interno(void) { return &g_mock_aceite; }

hashmal_menora_t *hashmal_comm_menora_mut(void)         { return &g_mock_menora; }
const hashmal_menora_t *hashmal_comm_get_menora(void)   { return &g_mock_menora; }

/* ==================================================================
 * hashmal_hc internal validators
 * ================================================================== */
bool hashmal_hc_vector_legitimo(hashmal_hc_vector_t v)  { return (unsigned)v < HASHMAL_HC_VECTORES; }
bool hashmal_hc_nivel_legitimo(hashmal_hc_nivel_t n)    { return (unsigned)n <= HASHMAL_HC_MAVET; }

/* ==================================================================
 * hashmal_comm public stubs (controllable)
 * ================================================================== */
uint8_t mock_vecinos_count   = 0;     /* hashmal_comm_vecinos_contar */
int8_t  mock_ble_rssi_medio  = 0;
uint8_t mock_ble_vecinos     = 0;
int8_t  mock_spi_rssi_floor  = -95;
uint8_t mock_wifi_ap_count   = 0;
int8_t  mock_wifi_rssi_max   = INT8_MIN;
uint32_t mock_broadcast_count = 0;     /* count of broadcasts sent */

int hashmal_comm_vecinos_contar(uint8_t *conteo)
{
    if (conteo == NULL) return HASHMAL_COMM_ERR_NULL;
    *conteo = mock_vecinos_count;
    return HASHMAL_COMM_OK;
}

int hashmal_comm_broadcast(const uint8_t *msg, size_t tam)
{
    (void)msg; (void)tam;
    ++mock_broadcast_count;
    return HASHMAL_COMM_OK;
}

int hashmal_comm_ble_rssi_muestra(int8_t *rssi_medio, uint8_t *vecinos)
{
    if (rssi_medio == NULL || vecinos == NULL) return HASHMAL_COMM_ERR_NULL;
    *rssi_medio = mock_ble_rssi_medio;
    *vecinos    = mock_ble_vecinos;
    return HASHMAL_COMM_OK;
}

int hashmal_comm_spi_rssi_ruido(int8_t *rssi_floor)
{
    if (rssi_floor == NULL) return HASHMAL_COMM_ERR_NULL;
    *rssi_floor = mock_spi_rssi_floor;
    return HASHMAL_COMM_OK;
}

int hashmal_comm_wifi_aps_muestra(uint8_t *ap_count, int8_t *rssi_max)
{
    if (ap_count == NULL || rssi_max == NULL) return HASHMAL_COMM_ERR_NULL;
    *ap_count = mock_wifi_ap_count;
    *rssi_max = mock_wifi_rssi_max;
    return HASHMAL_COMM_OK;
}

int hashmal_comm_ble_vecinos_contar(uint8_t *conteo)
{
    if (conteo == NULL) return HASHMAL_COMM_ERR_NULL;
    *conteo = mock_ble_vecinos;
    return HASHMAL_COMM_OK;
}

int hashmal_comm_ble_vecino_estado(uint8_t indice, hashmal_paamon_vecino_t *out)
{
    (void)indice; (void)out;
    return HASHMAL_COMM_ERR_CANAL;  /* no vecinos */
}

/* Parokhet shabat + Tsofeh lockdown + Shofar (Pieza 04 stubs from
 * Añadiduras 01 + 02 - real impls live in menora_parokhet.c /
 * menora_tsofeh.c which we don't link here). */
static bool g_parokhet_shabat = false;
static bool g_tsofeh_lockdown = false;

int hashmal_comm_parokhet_shabat_set(bool activar)
{
    g_parokhet_shabat = activar;
    return HASHMAL_COMM_OK;
}
bool hashmal_comm_parokhet_shabat_get(void) { return g_parokhet_shabat; }

int hashmal_comm_tsofeh_lockdown_set(bool activar)
{
    g_tsofeh_lockdown = activar;
    return HASHMAL_COMM_OK;
}
bool hashmal_comm_tsofeh_lockdown_get(void) { return g_tsofeh_lockdown; }

int hashmal_comm_shofar_tocar(const uint8_t *msg, size_t tam)
{
    (void)msg; (void)tam;
    ++mock_broadcast_count;
    return HASHMAL_COMM_OK;
}

/* ==================================================================
 * hashmal_roles public stubs
 * ================================================================== */
hashmal_roles_err_t hashmal_roles_estado(hashmal_roles_estado_roles_t *out)
{
    if (out == NULL) return HASHMAL_ROLES_ERR_NULL;
    memcpy(out, &g_mock_vestiduras, sizeof(*out));
    return HASHMAL_ROLES_OK;
}

hashmal_roles_err_t hashmal_roles_monitor_estado_nodo(uint8_t nodo_id,
                                                       hashmal_roles_heartbeat_t *out)
{
    if (out == NULL) return HASHMAL_ROLES_ERR_NULL;
    if (nodo_id >= HASHMAL_ROLES_MAX_NODOS) return HASHMAL_ROLES_ERR_NODO_NO_ENCONTRADO;
    *out = g_mock_vestiduras.nodos[nodo_id].heartbeat;
    return HASHMAL_ROLES_OK;
}

const hashmal_roles_estado_roles_t *hashmal_roles_get_estado(void)
{
    return &g_mock_vestiduras;
}
