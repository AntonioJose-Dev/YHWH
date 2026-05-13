/**
 * ===================================================================
 * PIEZA 06 --ALTAR DEL SACRIFICIO --hashmal_auth_internal.h
 * API INTERNA del componente --NO exportada fuera.
 * ===================================================================
 *
 * "NEVÚV lujót taʻasé otó --kaashér herʼá otejá bahár" --Éx 27:8
 * "HUECO de tablas lo harás --como se te mostró en el monte."
 *
 * El interior del altar ES HUECO. Aquí viven los símbolos que los
 * 6 archivos hermanos comparten para operar como una sola pieza
 * --pero el hueco NO se exporta: lo que circula dentro (fuego,
 * cenizas, humo) pertenece al altar, no al exterior.
 *
 * ===================================================================
 * P7 (Verificado #11) --DOS CAPAS: SHITTÍM (interior) + NEJÓSHET (ext.)
 * ===================================================================
 *
 *   Éx 27:1  --"ʻatséi shittím":  ACACIA (H7848) por dentro.
 *               La MISMA madera del Arca (Éx 25:10): durabilidad
 *               incorruptible. La lógica de verificación es shittím:
 *               estable, no expuesta, no expandible en runtime.
 *
 *   Éx 27:2b --"vetsippíta otó NEJÓSHET":  Piel de tsipáh (H6823)
 *               = recubrimiento INTENSIVO de bronce. Trust básico
 *               de Atrio (Jatsér), zona pública. La interfaz de
 *               cara al exterior es nejóshet: simple, robusta, expuesta.
 *
 * CONTRASTE CON EL ARCA (Éx 25:11): el Arca tiene oro DENTRO Y FUERA
 * (mibáyit umikhúts) --el Altar SOLO tiene bronce FUERA. Interior
 * acacia desnuda + exterior bronce. NO se invierte: nada de oro
 * adentro (privilegio reservado a Qódesh haQodashím), nada de
 * acacia expuesta (la madera no toca el mundo, el bronce sí).
 *
 * REGLAS ARQUITECTÓNICAS DE ESTE COMPONENTE:
 *   1. La lógica interna de verificación es STATIC en cada .c
 *      hermano (sha256_calcular, comparar_constante, random_llenar,
 *      colapsar_a_cero, construir_qeren, etc.). NO se exportan ni
 *      siquiera por hashmal_auth_internal.h: solo viven en su .c.
 *      Marcador: [shittim -- acacia interior, no expuesta (Ex 27:2)]
 *
 *   2. La API pública (hashmal_auth.h) es la cara nejóshet: simple,
 *      con códigos de error tipados, sin punteros mutables salidos
 *      del módulo, sin function pointers que permitan reescribir
 *      la lógica interna. Trust básico: el Atrio recibe; el Qódesh
 *      decide.
 *      Marcador: [nejoshet -- interfaz bronce, trust Atrio (Ex 27:2)]
 *
 *   3. Los símbolos compartidos entre hermanos (mut accessor,
 *      ahora_ms, cuerno_legitimo) van en este header --son acacia
 *      INTERIOR DEL COMPONENTE (no static al .c, pero tampoco
 *      exportados al exterior). Análogo a las tablas de acacia
 *      ensambladas con tirantes (v.7-8): cada tabla es independiente
 *      pero todas forman un solo cuerpo de acacia hueco.
 *
 *   4. La lógica interna NO se modifica en runtime. No hay setters
 *      para sustituir verificadores; no hay registro de plugins;
 *      no hay variables globales mutables que un hilo externo
 *      pueda alterar. Una vez levantada la pieza (Éx 40:6), su
 *      acacia es de una vez por todas (Éx 27:8 --kaashér herʼá).
 *
 * Autor: Antonio José Marín Soto
 * Proyecto: HASHMAL-CORE
 */
#ifndef HASHMAL_AUTH_INTERNAL_H
#define HASHMAL_AUTH_INTERNAL_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "hashmal_auth.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================================================================
 * altar_gateway.c --singleton y helpers compartidos
 *
 * El getter público (hashmal_auth_get_altar) devuelve const. Para
 * que los archivos hermanos (reshet, cuernos, fuego, utensilios,
 * ledger) puedan actualizar estado (cuernos, contadores, timestamps)
 * usamos esta vía privada. NO se exporta fuera del componente.
 * ================================================================== */

/**
 * Acceso mutable al altar. Solo para archivos hermanos del
 * componente hashmal_auth. @return NULL si no levantado.
 */
hashmal_auth_t *hashmal_auth_altar_mut(void);

/**
 * Lectura const desde dentro del componente --wrapper equivalente
 * al getter público pero con nombre distinto para subrayar el
 * ámbito interno.
 */
const hashmal_auth_t *hashmal_auth_get_altar_interno(void);

/**
 * Monotonic clock en milisegundos. Encapsula esp_timer_get_time()
 * (microsegundos) y convierte. Compartido para que todos los
 * archivos usen la misma unidad en timestamps (challenges, ciclos
 * mañana/tarde, ledger).
 */
uint64_t hashmal_auth_ahora_ms(void);

/**
 * Valida id de cuerno en [0, HASHMAL_AUTH_CUERNOS) = [0, 4).
 * Éx 27:2 --"arbaʻ pinnotáv": las 4 esquinas del altar.
 */
bool hashmal_auth_cuerno_legitimo(uint8_t id);

/**
 * P9 (Verificado #11) --Lv 6:3-4 --añade UNA entrada al ring buffer
 * de déshen. Cada verificación (challenge / validado / rechazado)
 * llama aquí para registrar su residuo. Si el buffer está lleno,
 * la entrada más vieja se descarta (incremento de
 * deshen_descartados) --Lv 6:4 manda SACAR el déshen, no acumular.
 *
 * Helper acacia interior --visible solo a los .c hermanos.
 */
void hashmal_auth_deshen_append(hashmal_auth_evento_t tipo);

/**
 * Lv 16:14 --llamar al conductor de sangre si está registrado.
 * Helper acacia interior: invoca s_dam_fn (vive en altar_gateway.c)
 * desde altar_reshet.c tras una verificación exitosa. No hace nada
 * si el conductor no está suscrito (s_dam_fn == NULL).
 *
 * @param dam          32 bytes SHA-256 del proof verificado
 * @param timestamp_ms cuando fue verificado (ms monótonos)
 */
void hashmal_auth_dam_llamar(const uint8_t *dam, uint64_t timestamp_ms);

#ifdef __cplusplus
}
#endif

#endif /* HASHMAL_AUTH_INTERNAL_H */
