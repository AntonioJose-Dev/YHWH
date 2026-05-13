/**
 * ═══════════════════════════════════════════════════════════════════
 * PIEZA 01 — ARCA — hashmal_storage_internal.h
 * API INTERNA del componente — NO exportada fuera.
 * ═══════════════════════════════════════════════════════════════════
 *
 * Une símbolos compartidos entre los 7 archivos hermanos del
 * componente hashmal_storage. El mundo exterior solo ve
 * include/hashmal_storage.h; esto es privado.
 *
 * "vetsipíta otó zaháv tahór mibáyit umijúts" — Éxodo 25:11
 * (la misma protección adentro que afuera)
 *
 * Autor: Antonio José Marín Soto
 * Proyecto: HASHMAL-CORE
 */
#ifndef HASHMAL_STORAGE_INTERNAL_H
#define HASHMAL_STORAGE_INTERNAL_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "hashmal_storage.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ══════════════════════════════════════════════════════════════════
 * arca_core.c — singleton accessor mutable
 *
 * El getter público (hashmal_storage_get_arca) devuelve const.
 * Para que los archivos hermanos del componente puedan actualizar
 * estado legítimamente (zer.hash, baddim flags, replicas_confirmadas,
 * etc.) usamos esta vía privada. NO SE EXPORTA fuera del componente.
 * ══════════════════════════════════════════════════════════════════ */

/**
 * Acceso mutable al Arca. Solo para archivos hermanos del
 * componente hashmal_storage. @return NULL si no inicializada.
 */
hashmal_arca_t* hashmal_storage_arca_mut(void);

/* ══════════════════════════════════════════════════════════════════
 * arca_cifrado.c — ORO (Éx 25:11, "zaháv tahór mibáyit umijúts")
 *
 * AES-256-GCM encapsulado sobre mbedtls. Una sola CLAVE: la misma
 * adentro (at-rest) que afuera (in-transit) — "mibáyit umijúts".
 * ══════════════════════════════════════════════════════════════════ */

/**
 * Cifra `plain_len` bytes con AES-256-GCM.
 * IV debe ser único por cifrado (se rellena aquí con RNG hardware).
 * AAD no se usa en esta versión (silente en el texto bíblico).
 *
 * @param plain       entrada en claro
 * @param plain_len   bytes de plain
 * @param cipher      salida cifrada (tamaño = plain_len)
 * @param iv_out      IV usado (salida, 12 B)
 * @param tag_out     tag de autenticación (salida, 16 B)
 * @return 0 en éxito, código mbedtls negado en fallo.
 */
int hashmal_oro_cifrar(const uint8_t *plain, size_t plain_len,
                       uint8_t *cipher,
                       uint8_t iv_out[HASHMAL_AES_IV_BYTES],
                       uint8_t tag_out[HASHMAL_AES_TAG_BYTES]);

/**
 * Descifra verificando tag. Si el tag no coincide → ERR_ZER_ROTO
 * (integridad violada — alguien tocó la madera).
 *
 * @param cipher      entrada cifrada
 * @param cipher_len  bytes del cifrado
 * @param iv          IV usado al cifrar (12 B)
 * @param tag         tag de autenticación (16 B)
 * @param plain_out   salida en claro (tamaño = cipher_len)
 * @return 0 en éxito, -1 en fallo de autenticación.
 */
int hashmal_oro_descifrar(const uint8_t *cipher, size_t cipher_len,
                          const uint8_t iv[HASHMAL_AES_IV_BYTES],
                          const uint8_t tag[HASHMAL_AES_TAG_BYTES],
                          uint8_t *plain_out);

/**
 * Provisión transitoria de la clave. En producción la entrega
 * hashmal_keygen (Pieza 13, Shémen haMishjá, Éx 30:22-33). Mientras
 * tanto se genera una por boot con RNG hardware.
 *
 * Llamar al menos una vez antes de cifrar/descifrar. Idempotente.
 */
void hashmal_oro_key_provisionar(void);

/**
 * ¿Hay clave lista? Útil para pre-chequeos.
 */
bool hashmal_oro_key_lista(void);

/**
 * Destruye de forma irreversible la clave AES volátil y cualquier
 * envelope vigente. Tras llamarla: key_lista == false, cifrar/descifrar
 * rechazan hasta re-provisión.
 *
 * 1 Sam 5:3-4 — "ushté kapót yadáv kerutót el hamiftán": las dos
 * palmas de Dagón cortadas en el umbral. Sin manos no hay manipulación.
 * Usada por hashmal_storage_dagon_caiga (arca_interfaz.c) cuando se
 * detecta exfiltración: la edut sigue cifrada, pero ilegible ya para
 * siempre sin la clave que ardió con el puerto.
 */
void hashmal_oro_key_destruir(void);

/* ══════════════════════════════════════════════════════════════════
 * arca_baddim.c — BADDÍM (Éx 25:13-15, varas always-on)
 *
 * Las siguientes primitivas son internas porque dependen de un
 * ciclo asíncrono con hashmal_comm (Menorá, pieza 04) que aún no
 * existe. Cuando la Menorá esté viva, llamará aquí para cerrar
 * el ciclo de réplica (veʻasú plural, v.10).
 * ══════════════════════════════════════════════════════════════════ */

/**
 * Confirma que una réplica llegó correctamente a un nodo remoto.
 * Incrementa replicas_confirmadas y marca distribuida=true.
 * Llamado asíncronamente por hashmal_comm cuando el peer ACK'ea.
 * @return 0 en éxito, ERR_INIT si Arca no levantada.
 */
int  hashmal_storage_replica_confirmada(uint8_t nodo_destino);

/**
 * ¿El Arca está actualmente en migración? (L7 — "qumá YHWH")
 */
bool hashmal_storage_en_migracion(void);

/**
 * Señala fin de migración — Nm 10:36 "shuvá YHWH" (vuelve).
 * El Arca se posa. Los servicios dependientes pueden avanzar.
 */
void hashmal_storage_en_migracion_fin(void);

/* ══════════════════════════════════════════════════════════════════
 * arca_merkle.c — consistencia distribuida (Dt 19:15, 2 testigos)
 *
 * Durante la réplica (veʻasú plural, v.10), los nodos intercambian
 * el Merkle root del zer. Dos testigos = dos hashes que concuerdan.
 * ══════════════════════════════════════════════════════════════════ */

/**
 * Exporta el Merkle root actual (para anunciarlo a un peer).
 * Recalcula y verifica antes de exportar — nunca se anuncia un zer
 * stale o roto. Éx 25:11 — la moldura es visible desde fuera, sin
 * revelar la madera debajo.
 * @return 0 en éxito.
 */
int  hashmal_storage_merkle_raiz_exportar(uint8_t out_hash[HASHMAL_ZER_HASH_BYTES]);

/**
 * Compara el zer local con un hash remoto recibido de un peer.
 * Dt 19:15 — "ʻal pí shenéi ʻedím yaqúm davár": por boca de dos
 * testigos se establece el asunto. Si los hashes difieren, los
 * nodos divergen (ERR_ZER_ROTO).
 * @return 0 si coinciden, ERR_ZER_ROTO si no.
 */
int  hashmal_storage_merkle_comparar(const uint8_t remoto_hash[HASHMAL_ZER_HASH_BYTES]);

/**
 * 1 Rey 8:9 — "ein baʼarón raq shené lujót": solo las tablas
 * quedan. Desvincula el séfer haTorá (torah.bytes = NULL, tamano
 * = 0, zer_externo a cero) y RECALCULA el zer — el Merkle tree
 * vuelve a una sola hoja (edut). La edut NO se toca.
 * @return 0 en éxito.
 */
int  hashmal_storage_torah_desvincular(void);

/* ══════════════════════════════════════════════════════════════════
 * arca_sealed_core.c — restauración NVS (Dt 10:1-4, segundas tablas)
 *
 * Fase 2 — Tarea 1: persistencia SEALED_CORE en flash NVS.
 * ══════════════════════════════════════════════════════════════════ */

/**
 * Restaura la edut desde NVS si existe (Dt 10:1-4 — segundas tablas,
 * mismo contenido que las primeras). Llamado por arca_core.c al final
 * de hashmal_storage_init. Si no hay datos NVS (primera vez) = no-op.
 *
 * "venatán eléjá et shenéi lujót haʼavánim karishoním" — Dt 10:1
 * "Y te daré las dos tablas de piedra como las primeras."
 *
 * Nota: NO verifica requiere_kaporet. Éx 25:16 ordena poner la edut
 * DENTRO (v.16) y LUEGO colocar la Kapóret encima (v.21). La edut
 * puede estar restaurada antes de que el Consensus la selle encima.
 *
 * @return 0 en éxito o si NVS vacío (primera vez); <0 en error real.
 */
int hashmal_storage_edut_restaurar_nvs(void);

#ifdef __cplusplus
}
#endif

#endif /* HASHMAL_STORAGE_INTERNAL_H */
