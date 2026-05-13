/**
 * ═══════════════════════════════════════════════════════════════════
 * PIEZA 07 — ATRIO (JATSÉR) — hashmal_perim_internal.h
 * API INTERNA del componente — NO exportada fuera.
 * ═══════════════════════════════════════════════════════════════════
 *
 * "vesamtá et hejatsér SAVÍV" — Éx 40:8
 * "Pondrás el atrio ALREDEDOR."
 *
 * "savív" (סָבִיב, H5439) — alrededor. Los archivos hermanos del
 * componente hashmal_perimeter operan sobre el mismo atrio: UN
 * singleton rodea el Mishkán (cortinas, columnas, puerta, intención).
 * Este header comparte los símbolos que permiten esa unidad — sin
 * exportarlos al exterior (el atrio es frontera VISIBLE pero
 * impenetrable: shésh mashzár translúcido, no opaco).
 *
 * Autor: Antonio José Marín Soto
 * Proyecto: HASHMAL-CORE
 */
#ifndef HASHMAL_PERIM_INTERNAL_H
#define HASHMAL_PERIM_INTERNAL_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "hashmal_perimeter.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ══════════════════════════════════════════════════════════════════
 * STRUCT INTERNA — el atrio como singleton
 *
 * El tipo `hashmal_perimeter_t` NO está en el header público: es
 * dato privado. El mundo exterior solo conoce los componentes
 * (columna_t, solicitud_t, resultado_t) a través de la API de
 * funciones. Así preservamos encapsulación — "savív" alrededor
 * pero sin exponer el interior.
 * ══════════════════════════════════════════════════════════════════ */
typedef struct {
    /* Estado global */
    hashmal_perimeter_estado_t   estado;           /* DESMONTADO/LEVANTADO/ACTIVO */
    hashmal_perimeter_puerta_t   puerta_estado;    /* CERRADA/ABIERTA/VERIFICANDO */

    /* Éx 27:10-16 — las 60 columnas */
    hashmal_perimeter_columna_t  columnas[HASHMAL_PERIM_COLUMNAS];
    uint8_t                      columnas_registradas;

    /* Timestamps + contadores de servicio */
    uint64_t                     init_ms;              /* vayyáqem (Éx 40:33) */
    uint64_t                     entradas_totales;     /* Jn 10:9 eisélthē */
    uint64_t                     rechazos_totales;     /* Ap 11:2 — fuera */
    uint64_t                     ultimo_acceso_ms;
} hashmal_perimeter_t;

/* ══════════════════════════════════════════════════════════════════
 * atrio_perimetro.c — singleton y helpers compartidos
 *
 * Las accesoras públicas del header devuelven códigos / estados;
 * para que los archivos hermanos (columnas, puerta, intención)
 * puedan actualizar el struct interno, usamos estas vías privadas.
 * NO se exportan fuera del componente.
 * ══════════════════════════════════════════════════════════════════ */

/**
 * Acceso MUTABLE al singleton del atrio. Solo archivos hermanos.
 * @return NULL si atrio no levantado.
 */
hashmal_perimeter_t *hashmal_perim_atrio_mut(void);

/**
 * Lectura const desde dentro del componente. Equivalente interno
 * al getter público — nombre distinto para subrayar ámbito.
 */
const hashmal_perimeter_t *hashmal_perim_get_atrio_interno(void);

/**
 * Monotonic clock en milisegundos. Encapsula esp_timer_get_time()
 * (microsegundos). Compartido para que todos los archivos usen la
 * misma unidad en timestamps (entradas, accesos, registro de
 * columnas).
 */
uint64_t hashmal_perim_ahora_ms(void);

#ifdef __cplusplus
}
#endif

#endif /* HASHMAL_PERIM_INTERNAL_H */
