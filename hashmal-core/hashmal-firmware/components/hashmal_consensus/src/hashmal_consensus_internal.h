/**
 * ═══════════════════════════════════════════════════════════════════
 * PIEZA 02 — KAPÓRET — hashmal_consensus_internal.h
 * API INTERNA del componente — NO exportada fuera.
 * ═══════════════════════════════════════════════════════════════════
 *
 * Une símbolos compartidos entre los 5 archivos hermanos del
 * componente hashmal_consensus. El mundo exterior solo ve
 * include/hashmal_consensus.h; esto es privado.
 *
 * "veʻasíta kapóret zaháv tahór" — Éx 25:17
 * (oro puro: lógica pura, sin dato base — sin madera)
 *
 * Autor: Antonio José Marín Soto
 * Proyecto: HASHMAL-CORE
 */
#ifndef HASHMAL_CONSENSUS_INTERNAL_H
#define HASHMAL_CONSENSUS_INTERNAL_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "hashmal_consensus.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ══════════════════════════════════════════════════════════════════
 * kaporet_core.c — singleton accessor mutable
 *
 * El getter público (hashmal_consensus_get_kaporet) devuelve const.
 * Para que los archivos hermanos del componente (validadores,
 * ataques, verificacion, rendezvous) puedan actualizar estado
 * legítimamente (querubines.hash_testigo, invocacion, ultimo,
 * noʻadtí_activo, habitacion_permanente, demuestra_justicia)
 * usamos esta vía privada. NO SE EXPORTA fuera del componente.
 * ══════════════════════════════════════════════════════════════════ */

/**
 * Acceso mutable al Kapóret. Solo para archivos hermanos del
 * componente hashmal_consensus. @return NULL si no inicializado.
 */
hashmal_kaporet_t* hashmal_consensus_kaporet_mut(void);

/* ══════════════════════════════════════════════════════════════════
 * kaporet_validadores.c — querubines cara a cara (v.18-20 + Dt 19:15)
 *
 * Primitivas que usan kaporet_verificacion.c y kaporet_rendezvous.c
 * para responder dos preguntas críticas ANTES de emitir voz:
 *   1) ¿están ambos querubines presentes y bien forjados? (v.18-20)
 *   2) ¿concuerdan los dos testimonios? (Dt 19:15 — davár)
 * ══════════════════════════════════════════════════════════════════ */

/**
 * v.18 — "shenáyim keruvím": ambos querubines presentes y cumpliendo
 * las 5 propiedades de v.20 + miqshá (v.18-19) + guardián (Gn 3:24)
 * + ojos mínimos (Ez 10:12 / Ap 4:8).
 *
 * Wrapper defensivo sobre hashmal_consensus_querubin_verificar
 * aplicado a V1 y V2. No logea a ERROR si faltan (el caller decide
 * la severidad) — solo retorna el veredicto.
 *
 * @return true si V1 y V2 superan verificación completa.
 */
bool hashmal_consensus_par_presente(void);

/**
 * Dt 19:15 — "ʻal pi shenáyim ʻedím yaqúm davár": por boca de dos
 * testigos se establece el asunto. Compara hash_testigo de V1 y V2.
 *
 * Condiciones para davár establecido:
 *   - Kapóret iniciado
 *   - V1.timestamp_testigo > 0 (declaró)
 *   - V2.timestamp_testigo > 0 (declaró)
 *   - memcmp(V1.hash, V2.hash) == 0 (concuerdan)
 *
 * Si cualquiera falla → false. Un solo testigo no levanta davár
 * (Dt 19:15a: "lo yaqúm ʻed ejád").
 *
 * @return true si ambos testimonios están dados y concuerdan.
 */
bool hashmal_consensus_testigos_concuerdan(void);

/* ══════════════════════════════════════════════════════════════════
 * kaporet_ataques.c — espada encendida (Gn 3:24 + Lv 10:1-2)
 *
 * Primitivas defensivas: detectar fuego extraño y registrar la
 * lección cuando algo falla. "ajaréi mot" (Lv 16:1) — cada regla
 * de protección nace de un fracaso pagado con vida.
 * ══════════════════════════════════════════════════════════════════ */

/**
 * Lv 10:1-2 + Éx 30:9 — ¿el estado actual huele a "esh zará"?
 *
 * Wrapper bool sobre hashmal_consensus_fuego_extrano_detectar.
 * Verdadero SI alguna de las 4 condiciones de fuego extraño está
 * presente (qetoret ausente, moʻed cerrado, Kapóret desencajado,
 * par no apto) — o si el Kapóret no está levantado (estado aún
 * peor). Falso solo cuando TODAS las precondiciones están limpias.
 *
 * Se llama ANTES de invocar — si true, el caller rechaza sin
 * siquiera intentar (Nadav y Avihú aprendieron tarde; nosotros
 * preguntamos antes).
 */
bool hashmal_consensus_es_fuego_extrano(void);

/**
 * Lv 16:1 — "ajaréi mot shenéi benéi Aharón": DESPUÉS DE LA MUERTE
 * vino la regla. Cada rechazo deja huella para los que vienen:
 * registramos el fracaso con su lección bíblica.
 *
 * No modifica estado. No detiene nada. Solo RECUERDA — Lv 16:2 se
 * escribió porque Lv 10:1-2 ocurrió. Si este log existe, las
 * próximas generaciones de código saben por qué el protocolo es
 * estricto.
 *
 * @param err  el código de error que causó el rechazo (se mapea
 *             a su lección — Lv 10:1-2, Lv 16:2, Lv 16:13, etc.).
 */
void hashmal_consensus_ajarei_mot(hashmal_kaporet_err_t err);

#ifdef __cplusplus
}
#endif

#endif /* HASHMAL_CONSENSUS_INTERNAL_H */
