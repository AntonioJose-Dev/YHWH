/**
 * QUERUBINES TEST FRAMEWORK
 * ===========================
 *
 * "Y harás dos querubines de oro... el uno al extremo de un lado,
 *  y el otro al extremo del otro lado."
 * — Éxodo 25:18-19
 *
 * Filosofía de test:
 * -----------------
 * Cada módulo se prueba CON DOS QUERUBINES — dos verificadores
 * independientes que no saben del contexto bíblico. Solo ven:
 *   - Entradas
 *   - Salidas esperadas
 *   - Propiedades que deben cumplirse
 *
 * Si ambos querubines (tests) coinciden en que el módulo funciona,
 * la verdad emerge de ENTRE los dos (כַּפֹּרֶת — Kapóret).
 *
 * Reglas:
 * -------
 * 1. Ningún test menciona "Biblia", "Tabernáculo", "YHWH"
 *    — son pruebas CIEGAS de ingeniería pura
 * 2. Cada módulo tiene al menos 2 tests independientes (2 querubines)
 * 3. Los tests verifican PROPIEDADES, no implementación
 * 4. Si un test falla, el módulo NO se integra
 *
 * Autor: Antonio José Marín Soto
 * Proyecto: HASHMAL-CORE
 */

#ifndef QUERUBINES_FRAMEWORK_H
#define QUERUBINES_FRAMEWORK_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/* ══════════════════════════════════════
 * Contadores globales
 * ══════════════════════════════════════ */

static int _qr_total   = 0;
static int _qr_passed  = 0;
static int _qr_failed  = 0;

/* ══════════════════════════════════════
 * Macros de test
 * ══════════════════════════════════════ */

/**
 * QR_TEST_BEGIN — Inicia una suite de tests para un módulo
 * nombre: nombre del módulo bajo prueba (sin contexto bíblico)
 */
#define QR_TEST_BEGIN(nombre) \
    do { \
        _qr_total = 0; _qr_passed = 0; _qr_failed = 0; \
        printf("\n══════════════════════════════════════\n"); \
        printf("  QUERUBINES TEST: %s\n", nombre); \
        printf("══════════════════════════════════════\n\n"); \
    } while(0)

/**
 * QR_TEST_END — Finaliza la suite y reporta resultado
 */
#define QR_TEST_END() \
    do { \
        printf("\n──────────────────────────────────────\n"); \
        printf("  RESULTADO: %d/%d pasaron", _qr_passed, _qr_total); \
        if (_qr_failed > 0) { \
            printf(" (%d FALLARON)\n", _qr_failed); \
        } else { \
            printf(" — TODOS OK\n"); \
        } \
        printf("──────────────────────────────────────\n"); \
    } while(0)

/**
 * QR_ASSERT — Verifica una condición
 * Si falla, reporta archivo, línea y descripción
 */
#define QR_ASSERT(condicion, descripcion) \
    do { \
        _qr_total++; \
        if (condicion) { \
            _qr_passed++; \
            printf("  [OK] %s\n", descripcion); \
        } else { \
            _qr_failed++; \
            printf("  [FALLO] %s\n", descripcion); \
            printf("         en %s:%d\n", __FILE__, __LINE__); \
        } \
    } while(0)

/**
 * QR_ASSERT_EQ — Verifica igualdad entre dos valores
 */
#define QR_ASSERT_EQ(a, b, descripcion) \
    QR_ASSERT((a) == (b), descripcion)

/**
 * QR_ASSERT_NEQ — Verifica que dos valores son diferentes
 */
#define QR_ASSERT_NEQ(a, b, descripcion) \
    QR_ASSERT((a) != (b), descripcion)

/**
 * QR_ASSERT_TRUE — Verifica que un valor es verdadero
 */
#define QR_ASSERT_TRUE(val, descripcion) \
    QR_ASSERT((val) == true, descripcion)

/**
 * QR_ASSERT_FALSE — Verifica que un valor es falso
 */
#define QR_ASSERT_FALSE(val, descripcion) \
    QR_ASSERT((val) == false, descripcion)

/**
 * QR_ASSERT_NULL — Verifica que un puntero es NULL
 */
#define QR_ASSERT_NULL(ptr, descripcion) \
    QR_ASSERT((ptr) == NULL, descripcion)

/**
 * QR_ASSERT_NOT_NULL — Verifica que un puntero NO es NULL
 */
#define QR_ASSERT_NOT_NULL(ptr, descripcion) \
    QR_ASSERT((ptr) != NULL, descripcion)

/**
 * QR_ASSERT_MEM_EQ — Verifica igualdad de bloques de memoria
 */
#define QR_ASSERT_MEM_EQ(a, b, size, descripcion) \
    QR_ASSERT(memcmp((a), (b), (size)) == 0, descripcion)

/**
 * QR_SECTION — Marca una sección dentro de los tests
 * (para organización visual, sin efecto funcional)
 */
#define QR_SECTION(nombre) \
    printf("\n  --- %s ---\n", nombre)

/* ══════════════════════════════════════
 * Funciones auxiliares
 * ══════════════════════════════════════ */

/**
 * qr_all_passed — Retorna true si todos los tests pasaron
 */
static inline bool qr_all_passed(void) {
    return (_qr_failed == 0) && (_qr_total > 0);
}

/**
 * qr_get_total — Retorna el total de tests ejecutados
 */
static inline int qr_get_total(void) {
    return _qr_total;
}

/**
 * qr_get_failed — Retorna el total de tests fallidos
 */
static inline int qr_get_failed(void) {
    return _qr_failed;
}

#endif /* QUERUBINES_FRAMEWORK_H */
