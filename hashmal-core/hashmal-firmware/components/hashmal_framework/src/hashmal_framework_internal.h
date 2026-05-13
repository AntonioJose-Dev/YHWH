/**
 * ═══════════════════════════════════════════════════════════════════
 * PIEZA 05 — LA ESTRUCTURA — hashmal_framework_internal.h
 * API INTERNA del componente (NO exportada fuera)
 * ═══════════════════════════════════════════════════════════════════
 *
 * Une las APIs de los 4 archivos hermanos (capas, tablas, barras,
 * velo) para que el orquestador (estructura_ejad.c) las coordine.
 *
 * Estos símbolos son privados del componente hashmal_framework.
 * El mundo exterior ve únicamente lo declarado en include/hashmal_framework.h.
 *
 * "vehayá haMishkán EJÁD" — Éx 26:6
 *
 * Autor: Antonio José Marín Soto
 * Proyecto: HASHMAL-CORE
 */
#ifndef HASHMAL_FRAMEWORK_INTERNAL_H
#define HASHMAL_FRAMEWORK_INTERNAL_H

#include <stdbool.h>
#include <stdint.h>

#include "hashmal_framework.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ═════════════════════ estructura_capas.c (v.1-14) ═════════════════════ */

/** Levanta las 4 capas concéntricas (lino → cabra → carnero → tajash). */
int  hashmal_capas_init      (hashmal_capa_t capas[HASHMAL_CAPAS_TOTAL]);

/** Verifica invariantes v.1, v.3, v.6, v.8, v.9, v.11, v.14 y el gradiente. */
bool hashmal_capas_verificar (const hashmal_capa_t capas[HASHMAL_CAPAS_TOTAL]);

/** Nombre hebreo de la capa (shésh / ʻizzím / eilím / tejashím). */
const char* hashmal_capas_nombre(hashmal_capa_id_t id);

/* ═════════════════════ estructura_tablas.c (v.15-25) ═════════════════════ */

/** Levanta los 48 qerashím — 20 sur + 20 norte + 6 rectos + 2 esquinas. */
int  hashmal_tablas_init      (hashmal_qeresh_t tablas[HASHMAL_TABLAS_TOTAL]);

/** Verifica v.15, v.17 (reciprocidad), v.18/20/22-25, este abierto, 96 bases. */
bool hashmal_tablas_verificar (const hashmal_qeresh_t tablas[HASHMAL_TABLAS_TOTAL]);

/** Cuenta adné késef (2 por tabla de pie). Esperado: HASHMAL_BASES_PLATA=96. */
uint16_t hashmal_tablas_bases_plata(const hashmal_qeresh_t tablas[HASHMAL_TABLAS_TOTAL]);

/** Nombre hebreo del lado (négbah / tsafón / yámah). */
const char* hashmal_tablas_lado_nombre(hashmal_lado_t lado);

/* ═════════════════════ estructura_barras.c (v.26-29) ═════════════════════ */

/** Levanta las 15 berijím (5 por lado × 3 lados). */
int  hashmal_barras_init(
    hashmal_beriaj_t sur  [HASHMAL_BARRAS_POR_LADO],
    hashmal_beriaj_t norte[HASHMAL_BARRAS_POR_LADO],
    hashmal_beriaj_t oeste[HASHMAL_BARRAS_POR_LADO]);

/** Verifica v.26-28: 5 por lado, un hatijón por lado extremo-a-extremo. */
bool hashmal_barras_verificar(
    const hashmal_beriaj_t sur  [HASHMAL_BARRAS_POR_LADO],
    const hashmal_beriaj_t norte[HASHMAL_BARRAS_POR_LADO],
    const hashmal_beriaj_t oeste[HASHMAL_BARRAS_POR_LADO]);

/** Cuenta backbones totales (esperado: HASHMAL_BARRAS_LADOS = 3). */
uint8_t hashmal_barras_backbones_total(
    const hashmal_beriaj_t sur  [HASHMAL_BARRAS_POR_LADO],
    const hashmal_beriaj_t norte[HASHMAL_BARRAS_POR_LADO],
    const hashmal_beriaj_t oeste[HASHMAL_BARRAS_POR_LADO]);

/* ═════════════════════ estructura_velo.c (v.31-37) ═════════════════════ */

/** Confecciona parójet (v.31-32) + masaj (v.36-37). No activa todavía. */
int  hashmal_velo_init (hashmal_parojet_t *velo, hashmal_masaj_t *masaj);

/** Activa el parójet — v.33. hivdíl en efecto → qódesh haqodashím creado. */
int  hashmal_velo_activar (hashmal_parojet_t *velo);

/** Activa el masaj — v.36. La cortina queda colgada en la entrada. */
int  hashmal_masaj_activar(hashmal_masaj_t   *masaj);

/** Verifica el parójet contra v.31-33. */
bool hashmal_velo_verificar (const hashmal_parojet_t *velo);

/** Verifica el masaj contra v.36-37. */
bool hashmal_masaj_verificar(const hashmal_masaj_t   *masaj);

/** Verifica el gradiente bronce → plata entre las dos membranas. */
bool hashmal_membranas_gradiente(const hashmal_parojet_t *velo,
                                 const hashmal_masaj_t   *masaj);

/** Zona de un miqom (v.33-35). ARON/KAPORET → QODESH_QODASHIM; MESA/MENORA → QODESH. */
hashmal_zona_t hashmal_miqom_zona(hashmal_miqom_t miqom);

/** Lado asignado a un miqom (v.35). SHULJAN → NORTE, MENORAH → SUR. */
bool hashmal_miqom_lado(hashmal_miqom_t miqom, hashmal_lado_t *lado);

#ifdef __cplusplus
}
#endif

#endif /* HASHMAL_FRAMEWORK_INTERNAL_H */
