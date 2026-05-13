/**
 * ===================================================================
 * PIEZA 03 - MESA - mesa_api.c
 * Sub-pieza: LOS 4 UTENSILIOS DE ORO - CREATE / READ / UPDATE / DELETE
 * Exodo 25:29 + Levitico 24:5-9
 * ===================================================================
 *
 * "ve'asita QE'AROTAV vejappotav uqesotav umenaqiyyotav asher
 *  yussaj bahen zahav tahor ta'ase otam" - Ex 25:29
 * "Haras sus PLATOS, sus CUCHARAS, sus JARRAS y sus COPAS con que
 *  se libara; de oro puro los haras."
 *
 * --- LOS 4 UTENSILIOS COMO CRUD ---
 *
 *   qe'arot   (qe'ara, plato)        CREATE  - donde se PONE el pan
 *   kappot    (kaf, cuchara/palma)   READ    - con que se TOMA
 *   qesot     (qasa, jarra/cubierta) UPDATE  - con que se REFRESCA
 *   menaqiyyot (menaqit, copa lib.)  DELETE  - con que se VIERTE
 *
 * Son CUATRO palabras DISTINTAS en el texto; no sinonimos.
 * Todos DE ORO PURO - cada operacion tiene la misma dignidad.
 *
 * --- SOPORTE -----------------------------------------------------
 *
 * "velaqajta SOLET" - Lv 24:5. Harina FINA. En firmware: el dato
 * entra ya refinado (el caller es responsable de no meter basura).
 *
 * "venatata 'al hamma'arejet levona zajja... le'AZJARA" - Lv 24:7.
 * Incienso puro como MEMORIAL. En firmware: cada operacion CRUD deja
 * huella en slot.azjara (contador + ultimo utensilio).
 *
 * "ve'ajalahu Aharon uvanav... qodesh qodashim hu lo" - Lv 24:9.
 * "Santidad de santidades es para el." El pan LEIDO conserva la
 * misma integridad que el PUESTO - la copia no diluye.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_cache.h              -> struct + API publica
 *   hashmal_cache_internal.h     -> shuljan_mut, ahora_ms, slot_legitimo
 *   hashmal_storage.h            -> HASHMAL_ZER_HASH_BYTES (32)
 *   mbedtls/sha256.h (ORO EGIPTO)-> SHA-256 per-slot
 *   esp_log.h (ORO EGIPTO)       -> MESA_LOG_*
 *
 * --- FLUJO COMUN DE VALIDACION ---
 *
 * Cada CRUD valida (en orden):
 *   1) Mesa levantada (shuljan_mut)
 *   2) slot in [0, 11]                       - Lv 24:5 shte 'esre
 *   3) parametros no-NULL y tamano legitimo - solet valida
 *   4) estado del slot compatible con la op - v.29 semantica
 *   5) misgeret_permitir()                  - v.25 rate check
 *   6) ejecuta + registra azjara            - Lv 24:7 memorial
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <inttypes.h>

#include "mbedtls/sha256.h"
#include "esp_log.h"

#include "hashmal_cache.h"
#include "hashmal_cache_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *MESA_TAG = "hashmal.mesa.api";

#define MESA_LOG_I(fmt, ...)  ESP_LOGI(MESA_TAG, fmt, ##__VA_ARGS__)
#define MESA_LOG_W(fmt, ...)  ESP_LOGW(MESA_TAG, fmt, ##__VA_ARGS__)
#define MESA_LOG_E(fmt, ...)  ESP_LOGE(MESA_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * HELPERS ESTATICOS - SHA-256 y AZJARA
 * ================================================================== */

/**
 * SHA-256 one-shot sobre un buffer. Encapsula mbedtls - cada archivo
 * del Mishkan encapsula su propia puerta al oro de Egipto (mismo
 * patron que arca_zer.c, arca_merkle.c, arca_sealed_core.c).
 *
 * @param in   buffer (puede ser NULL si len == 0)
 * @param len  bytes (0 valido -> SHA-256 del vacio, determinista)
 * @param out  salida de 32 bytes (no NULL)
 * @return 0 en exito.
 */
static int sha256_slot(const uint8_t *in, size_t len,
                       uint8_t out[HASHMAL_ZER_HASH_BYTES])
{
    if (out == NULL) return -1;
    if (in == NULL && len > 0u) return -1;

    mbedtls_sha256_context ctx;
    mbedtls_sha256_init(&ctx);

    int rc = mbedtls_sha256_starts(&ctx, 0);
    if (rc == 0 && len > 0u) rc = mbedtls_sha256_update(&ctx, in, len);
    if (rc == 0)             rc = mbedtls_sha256_finish(&ctx, out);

    mbedtls_sha256_free(&ctx);
    return rc;
}

/**
 * Lv 24:7 - "levona zajja... le'azjara" - incienso puro como
 * memorial. Cada operacion CRUD deja huella en slot.azjara.
 *
 * Formato (32 bytes):
 *   [0..3]   contador u32 little-endian de operaciones sobre el slot
 *   [4]      ultimo utensilio (hashmal_utensilio_t cast a uint8_t)
 *   [5..7]   reservado (0)
 *   [8..15]  timestamp (ms) de la ultima operacion, u64 little-endian
 *   [16..31] reservado (0)
 *
 * El layout es estable para lectura externa; el contador y el
 * timestamp son los dos datos primarios del memorial.
 */
static void azjara_registrar(hashmal_lejem_t *l,
                             hashmal_utensilio_t op,
                             uint64_t ahora_ms)
{
    uint32_t contador = 0u;
    memcpy(&contador, &l->azjara[0], sizeof(contador));
    contador += 1u;
    memcpy(&l->azjara[0], &contador, sizeof(contador));

    l->azjara[4] = (uint8_t)op;
    l->azjara[5] = 0u;
    l->azjara[6] = 0u;
    l->azjara[7] = 0u;

    memcpy(&l->azjara[8], &ahora_ms, sizeof(ahora_ms));
    /* bytes 16..31 quedan en 0 (reservado) */
}

/**
 * Recomputa el zer del slot sobre sus bytes[0..tamano] y lo
 * compara con el almacenado. True si coinciden.
 */
static bool zer_slot_coincide(const hashmal_lejem_t *l)
{
    uint8_t nuevo[HASHMAL_ZER_HASH_BYTES];
    if (sha256_slot(l->bytes, (size_t)l->tamano, nuevo) != 0) {
        return false;
    }
    return memcmp(nuevo, l->zer_slot, HASHMAL_ZER_HASH_BYTES) == 0;
}

/* ==================================================================
 * API PUBLICA - qe'ara (CREATE, v.29 - platos)
 * ================================================================== */

/**
 * hashmal_cache_qeara_poner - PON el pan en su plato
 *
 * qe'arot son los PLATOS: superficie donde el pan se coloca.
 * Operacion CREATE. Solo sobre slot VACIO o EXPIRADO (reemplazar
 * expirado cuenta como "crear" - el pan viejo ya no es solet).
 *
 * 2 testigos:
 *   Ex 25:29 - qe'arot de oro puro
 *   Lv 24:5  - solet (harina FINA): dato refinado
 */
int hashmal_cache_qeara_poner(uint8_t slot,
                              const uint8_t *bytes,
                              size_t tam,
                              uint8_t panim_origen,
                              uint8_t panim_destino)
{
    hashmal_shuljan_t *s = hashmal_cache_shuljan_mut();
    if (s == NULL) {
        MESA_LOG_E("qeara: Mesa no levantada");
        return HASHMAL_CACHE_ERR_INIT;
    }
    if (!hashmal_cache_slot_legitimo(slot)) {
        MESA_LOG_E("qeara: slot %u fuera de [0, %u) (Lv 24:5)",
                   (unsigned)slot, HASHMAL_SHULJAN_SLOTS);
        return HASHMAL_CACHE_ERR_INDICE;
    }
    if (bytes == NULL) {
        MESA_LOG_E("qeara: bytes NULL — sólet no puede ser vacío");
        return HASHMAL_CACHE_ERR_NULL;
    }
    if (tam == 0u || tam > HASHMAL_LEJEM_BYTES_MAX) {
        MESA_LOG_E("qeara: tam %u fuera de (0, %u]",
                   (unsigned)tam, HASHMAL_LEJEM_BYTES_MAX);
        return HASHMAL_CACHE_ERR_TAMANO;
    }

    /* P7 — Éx 25:30 doble paním OBLIGATORIO. */
    if (panim_origen == HASHMAL_PANIM_INVALIDO) {
        MESA_LOG_E("qeara: panim_origen ausente — el pan no tiene rostros "
                   "(no es léjem paním, v.30)");
        return HASHMAL_CACHE_ERR_NO_AUTORIZADO;
    }
    if (panim_destino == HASHMAL_PANIM_INVALIDO) {
        MESA_LOG_E("qeara: panim_destino ausente — el pan no está orientado "
                   "(no está lefanái, v.30)");
        return HASHMAL_CACHE_ERR_NO_AUTORIZADO;
    }

    hashmal_lejem_t *l = &s->slots[slot];

    /* Solo slots VACIO o EXPIRADO admiten qe'ara. FRESCO/SERVIDO
     * requieren qasa (UPDATE) - no se re-hornea encima sin refrescar
     * conscientemente. */
    if (l->estado == HASHMAL_LEJEM_FRESCO ||
        l->estado == HASHMAL_LEJEM_SERVIDO) {
        MESA_LOG_E("qeara: slot %u ocupado (estado=%d) — usa qasá para refrescar",
                   (unsigned)slot, (int)l->estado);
        return HASHMAL_CACHE_ERR_SLOT_LLENO;
    }

    /* v.25 - tofaj: rate check. Si el marco esta lleno, rechazo. */
    int rc = hashmal_cache_misgeret_permitir();
    if (rc != HASHMAL_CACHE_OK) {
        MESA_LOG_W("qeara: misgéret rechaza (rc=%d) — v.25", rc);
        return rc;
    }

    /* Hornear el pan: copia solet al slot. */
    const uint64_t ahora = hashmal_cache_ahora_ms();
    memset(l->bytes, 0, sizeof(l->bytes));  /* limpieza antes de copia */
    memcpy(l->bytes, bytes, tam);
    l->tamano = (uint16_t)tam;

    /* v.24 - zer por slot (SHA-256 del contenido). */
    rc = sha256_slot(l->bytes, (size_t)l->tamano, l->zer_slot);
    if (rc != 0) {
        MESA_LOG_E("qeara: SHA-256 falló (mbedtls=%d) — revirtiendo", rc);
        memset(l->bytes, 0, sizeof(l->bytes));
        memset(l->zer_slot, 0, HASHMAL_ZER_HASH_BYTES);
        l->tamano = 0u;
        return HASHMAL_CACHE_ERR_ZER_ROTO;
    }

    l->estado                = HASHMAL_LEJEM_FRESCO;
    l->timestamp_horneado_ms = ahora;
    l->timestamp_servido_ms  = 0u;

    /* P7 — v.30 doble paním (rostros del pan). */
    l->panim_origen  = panim_origen;
    l->panim_destino = panim_destino;

    /* Lv 24:7 - memorial. */
    azjara_registrar(l, HASHMAL_UTENSILIO_QEARA, ahora);

    MESA_LOG_I("qeʻará slot=%u — léjem horneado %u B, paním orígen=%u → "
               "destino=%u (Éx 25:30 léjem paním lefanái)",
               (unsigned)slot, (unsigned)tam,
               (unsigned)panim_origen, (unsigned)panim_destino);
    return HASHMAL_CACHE_OK;
}

/* ==================================================================
 * API PUBLICA - kaf (READ, v.29 - cucharas)
 * ================================================================== */

/**
 * hashmal_cache_kaf_tomar - TOMA (lee) sin consumir
 *
 * kappot viene de kaf: palma/cuchara. La palma RECIBE - no destruye
 * lo que toma. Por eso kaf_tomar no vacia el slot; solo lo marca
 * SERVIDO (ya fue accedido).
 *
 * tam_inout: in = capacidad del buffer; out = bytes efectivamente
 * escritos. Si la capacidad es menor que el tamano del pan, se
 * copia lo que cabe (truncado) - el caller sabra por el valor de
 * salida. Esto honra Lv 24:9 "qodesh qodashim": el fragmento que
 * cabe es tan sagrado como el entero.
 *
 * Antes de copiar, verificamos el zer del slot. Si la integridad
 * falla, el slot se marca EXPIRADO (no servimos pan alterado) y
 * retornamos ERR_ZER_ROTO.
 *
 * 2 testigos:
 *   Ex 25:29 - kappot de oro puro
 *   Lv 24:9  - "qodesh qodashim hu lo" (el servido conserva su santidad)
 */
int hashmal_cache_kaf_tomar(uint8_t slot,
                            uint8_t *out,
                            size_t *tam_inout)
{
    if (out == NULL || tam_inout == NULL) {
        MESA_LOG_E("kaf: out o tam_inout NULL");
        return HASHMAL_CACHE_ERR_NULL;
    }

    hashmal_shuljan_t *s = hashmal_cache_shuljan_mut();
    if (s == NULL) {
        MESA_LOG_E("kaf: Mesa no levantada");
        return HASHMAL_CACHE_ERR_INIT;
    }
    if (!hashmal_cache_slot_legitimo(slot)) {
        MESA_LOG_E("kaf: slot %u fuera de [0, %u)",
                   (unsigned)slot, HASHMAL_SHULJAN_SLOTS);
        return HASHMAL_CACHE_ERR_INDICE;
    }

    hashmal_lejem_t *l = &s->slots[slot];

    /* Solo FRESCO o SERVIDO son legibles. VACIO/EXPIRADO no tienen
     * pan servible. */
    if (l->estado != HASHMAL_LEJEM_FRESCO &&
        l->estado != HASHMAL_LEJEM_SERVIDO) {
        MESA_LOG_E("kaf: slot %u estado=%d (no legible)",
                   (unsigned)slot, (int)l->estado);
        return HASHMAL_CACHE_ERR_SLOT_VACIO;
    }

    /* v.25 - rate check. */
    int rc = hashmal_cache_misgeret_permitir();
    if (rc != HASHMAL_CACHE_OK) {
        MESA_LOG_W("kaf: misgéret rechaza (rc=%d) — v.25", rc);
        return rc;
    }

    /* v.24 - zer check ANTES de copiar. Si no casa, el pan esta
     * corrupto; no servimos veneno. Marcamos EXPIRADO para que el
     * proximo Shabat lo retire (Lv 24:8 ya'arejennu). */
    if (!zer_slot_coincide(l)) {
        l->estado = HASHMAL_LEJEM_EXPIRADO;
        MESA_LOG_E("kaf: slot %u — zer ROTO, marcado EXPIRADO (v.24)",
                   (unsigned)slot);
        return HASHMAL_CACHE_ERR_ZER_ROTO;
    }

    /* Copiar min(tamano, *tam_inout). */
    const uint64_t ahora   = hashmal_cache_ahora_ms();
    const size_t   cap_in  = *tam_inout;
    const size_t   disp    = (size_t)l->tamano;
    const size_t   n       = (cap_in < disp) ? cap_in : disp;

    if (n > 0u) {
        memcpy(out, l->bytes, n);
    }
    *tam_inout = n;

    /* Marcar SERVIDO - al menos una lectura vivida. */
    l->estado              = HASHMAL_LEJEM_SERVIDO;
    l->timestamp_servido_ms = ahora;

    azjara_registrar(l, HASHMAL_UTENSILIO_KAF, ahora);

    if (n < disp) {
        MESA_LOG_W("kaf slot=%u — TRUNCADO: servidos %u/%u B "
                   "(Lv 24:9: qódesh qodashím hu ló)",
                   (unsigned)slot, (unsigned)n, (unsigned)disp);
    } else {
        MESA_LOG_I("kaf slot=%u — servidos %u B (Éx 25:29)",
                   (unsigned)slot, (unsigned)n);
    }
    return HASHMAL_CACHE_OK;
}

/* ==================================================================
 * API PUBLICA - distribuir (P8: shuljan = shalaj = enviar)
 *
 * Ex 25:23 - SHULJAN (H7979) viene de la raiz SHALAJ (H7971 = enviar,
 * extender, mandar). Forma qutlan pasiva: "lo que ha sido extendido."
 * El Padre NO llama a la Mesa con raiz de almacenamiento (atsar/
 * shamar). La Mesa ENVIA. El almacenamiento es del Arca.
 *
 * Posicion enfrentada Mesa<->Menora (Ex 26:35) confirma: la Mesa
 * envia lo que la Menora transmite.
 *
 * Esta funcion es la PRIMARIA del caché para flujo de salida:
 * distribucion activa, no consulta pasiva. Entrega el pan al caller
 * junto con sus DOS panim (P7: identidad + orientacion).
 *
 * Internamente comparte logica con kaf_tomar (zer check, marca
 * SERVIDO, misgeret); la diferencia es semantica + paquete completo
 * (panim_origen y panim_destino entregados al receptor).
 * ================================================================== */

int hashmal_cache_distribuir(uint8_t  slot,
                             uint8_t *out,
                             size_t  *tam_inout,
                             uint8_t *out_panim_origen,
                             uint8_t *out_panim_destino)
{
    if (out == NULL || tam_inout == NULL ||
        out_panim_origen == NULL || out_panim_destino == NULL) {
        MESA_LOG_E("distribuir: alguno de out/tam_inout/panim_* NULL");
        return HASHMAL_CACHE_ERR_NULL;
    }

    hashmal_shuljan_t *s = hashmal_cache_shuljan_mut();
    if (s == NULL) {
        MESA_LOG_E("distribuir: Mesa no levantada");
        return HASHMAL_CACHE_ERR_INIT;
    }
    if (!hashmal_cache_slot_legitimo(slot)) {
        MESA_LOG_E("distribuir: slot %u fuera de [0, %u)",
                   (unsigned)slot, HASHMAL_SHULJAN_SLOTS);
        return HASHMAL_CACHE_ERR_INDICE;
    }

    hashmal_lejem_t *l = &s->slots[slot];

    /* Solo FRESCO o SERVIDO son distribuibles. */
    if (l->estado != HASHMAL_LEJEM_FRESCO &&
        l->estado != HASHMAL_LEJEM_SERVIDO) {
        MESA_LOG_E("distribuir: slot %u estado=%d (no distribuible)",
                   (unsigned)slot, (int)l->estado);
        return HASHMAL_CACHE_ERR_SLOT_VACIO;
    }

    /* P7 - sanity: el slot debe tener panim validos. Un FRESCO/SERVIDO
     * con panim invalidos es estado inconsistente (qeʻará/qasá los
     * exigen al crear/refrescar). Defensa adicional. */
    if (l->panim_origen == HASHMAL_PANIM_INVALIDO ||
        l->panim_destino == HASHMAL_PANIM_INVALIDO) {
        MESA_LOG_E("distribuir: slot %u con paním inválidos — "
                   "no es léjem paním (v.30)", (unsigned)slot);
        return HASHMAL_CACHE_ERR_NO_AUTORIZADO;
    }

    /* v.25 - rate check antes de extender (shalaj implica costo). */
    int rc = hashmal_cache_misgeret_permitir();
    if (rc != HASHMAL_CACHE_OK) {
        MESA_LOG_W("distribuir: misgéret rechaza (rc=%d)", rc);
        return rc;
    }

    /* v.24 - zer check antes de copiar. Si esta roto, marcamos
     * EXPIRADO y rechazamos: shalaj es ENVIO ACTIVO; no enviamos
     * dato corrupto a la red. */
    if (!zer_slot_coincide(l)) {
        l->estado = HASHMAL_LEJEM_EXPIRADO;
        MESA_LOG_E("distribuir: slot %u zer ROTO, marcado EXPIRADO "
                   "(v.24) — no se envía dato corrupto", (unsigned)slot);
        return HASHMAL_CACHE_ERR_ZER_ROTO;
    }

    /* Copiar min(tamano, *tam_inout). Misma semantica que kaf_tomar:
     * el fragmento que cabe es tan sagrado como el entero (Lv 24:9). */
    const uint64_t ahora  = hashmal_cache_ahora_ms();
    const size_t   cap_in = *tam_inout;
    const size_t   disp   = (size_t)l->tamano;
    const size_t   n      = (cap_in < disp) ? cap_in : disp;
    if (n > 0u) {
        memcpy(out, l->bytes, n);
    }
    *tam_inout = n;

    /* Entregar los DOS paním (P7) al receptor. */
    *out_panim_origen  = l->panim_origen;
    *out_panim_destino = l->panim_destino;

    /* Marcar SERVIDO + memorial (azjara). Reusamos el utensilio KAF
     * conceptualmente: distribuir desde el caché conserva la
     * trazabilidad del cucharón que extiende lo que ya estaba
     * extendido en la mesa. */
    l->estado              = HASHMAL_LEJEM_SERVIDO;
    l->timestamp_servido_ms = ahora;
    azjara_registrar(l, HASHMAL_UTENSILIO_KAF, ahora);

    if (n < disp) {
        MESA_LOG_W("distribuir slot=%u TRUNCADO: enviados %u/%u B, "
                   "paním %u→%u (Éx 25:23 shaláj)",
                   (unsigned)slot, (unsigned)n, (unsigned)disp,
                   (unsigned)*out_panim_origen,
                   (unsigned)*out_panim_destino);
    } else {
        MESA_LOG_I("distribuir slot=%u — enviados %u B, paním %u→%u "
                   "(Éx 25:23 shulján ← shaláj)",
                   (unsigned)slot, (unsigned)n,
                   (unsigned)*out_panim_origen,
                   (unsigned)*out_panim_destino);
    }
    return HASHMAL_CACHE_OK;
}

/* ==================================================================
 * API PUBLICA - qasa (UPDATE, v.29 - jarras)
 * ================================================================== */

/**
 * hashmal_cache_qasa_refrescar - REFRESCA el pan del slot
 *
 * qesot (de qasa): jarra/cubierta. Las jarras vierten/cubren para
 * RENOVAR. Operacion UPDATE. Sobreescribe el contenido, reinicia
 * timestamps. Equivalente ciclo a Lv 24:8 "ya'arejennu" aplicado
 * a un solo slot: pan nuevo sin pasar por el Shabat global.
 *
 * Requiere que el slot NO este VACIO (no se refresca lo que no
 * existe; para crear desde cero hay qe'ara).
 *
 * 2 testigos:
 *   Ex 25:29 - qesot de oro puro
 *   Lv 24:8  - ya'arejennu: renovar el pan
 */
int hashmal_cache_qasa_refrescar(uint8_t slot,
                                 const uint8_t *bytes,
                                 size_t tam,
                                 uint8_t panim_origen,
                                 uint8_t panim_destino)
{
    hashmal_shuljan_t *s = hashmal_cache_shuljan_mut();
    if (s == NULL) {
        MESA_LOG_E("qasa: Mesa no levantada");
        return HASHMAL_CACHE_ERR_INIT;
    }
    if (!hashmal_cache_slot_legitimo(slot)) {
        MESA_LOG_E("qasa: slot %u fuera de rango", (unsigned)slot);
        return HASHMAL_CACHE_ERR_INDICE;
    }
    if (bytes == NULL) {
        MESA_LOG_E("qasa: bytes NULL");
        return HASHMAL_CACHE_ERR_NULL;
    }
    if (tam == 0u || tam > HASHMAL_LEJEM_BYTES_MAX) {
        MESA_LOG_E("qasa: tam %u fuera de (0, %u]",
                   (unsigned)tam, HASHMAL_LEJEM_BYTES_MAX);
        return HASHMAL_CACHE_ERR_TAMANO;
    }

    /* P7 — Éx 25:30 doble paním obligatorio también al refrescar. */
    if (panim_origen == HASHMAL_PANIM_INVALIDO) {
        MESA_LOG_E("qasa: panim_origen ausente — refrescar sin rostros "
                   "viola v.30 léjem paním");
        return HASHMAL_CACHE_ERR_NO_AUTORIZADO;
    }
    if (panim_destino == HASHMAL_PANIM_INVALIDO) {
        MESA_LOG_E("qasa: panim_destino ausente — refrescar sin orientación "
                   "viola v.30 lefanái");
        return HASHMAL_CACHE_ERR_NO_AUTORIZADO;
    }

    hashmal_lejem_t *l = &s->slots[slot];

    if (l->estado == HASHMAL_LEJEM_VACIO) {
        MESA_LOG_E("qasa: slot %u VACIO — usa qeʻará para crear",
                   (unsigned)slot);
        return HASHMAL_CACHE_ERR_SLOT_VACIO;
    }

    /* v.25 - rate check. */
    int rc = hashmal_cache_misgeret_permitir();
    if (rc != HASHMAL_CACHE_OK) {
        MESA_LOG_W("qasa: misgéret rechaza (rc=%d)", rc);
        return rc;
    }

    /* Re-hornear: sobreescribir bytes y zer. */
    const uint64_t ahora = hashmal_cache_ahora_ms();
    memset(l->bytes, 0, sizeof(l->bytes));
    memcpy(l->bytes, bytes, tam);
    l->tamano = (uint16_t)tam;

    rc = sha256_slot(l->bytes, (size_t)l->tamano, l->zer_slot);
    if (rc != 0) {
        MESA_LOG_E("qasa: SHA-256 falló (mbedtls=%d) — estado inconsistente", rc);
        /* El contenido quedo escrito pero el zer no se calculo.
         * Marcamos EXPIRADO para que el Shabat lo retire. */
        memset(l->zer_slot, 0, HASHMAL_ZER_HASH_BYTES);
        l->estado = HASHMAL_LEJEM_EXPIRADO;
        return HASHMAL_CACHE_ERR_ZER_ROTO;
    }

    l->estado                = HASHMAL_LEJEM_FRESCO;
    l->timestamp_horneado_ms = ahora;
    l->timestamp_servido_ms  = 0u;     /* nuevo ciclo de servido */

    /* P7 — actualiza paním (refrescar puede cambiar origen/destino). */
    l->panim_origen  = panim_origen;
    l->panim_destino = panim_destino;

    azjara_registrar(l, HASHMAL_UTENSILIO_QASA, ahora);

    MESA_LOG_I("qasá slot=%u — léjem refrescado %u B, paním %u→%u "
               "(Lv 24:8 yaʻarejénnu, Éx 25:30)",
               (unsigned)slot, (unsigned)tam,
               (unsigned)panim_origen, (unsigned)panim_destino);
    return HASHMAL_CACHE_OK;
}

/* ==================================================================
 * API PUBLICA - menaqit (DELETE, v.29 - copas de libacion)
 * ================================================================== */

/**
 * hashmal_cache_menaqit_vaciar - VIERTE / vacia el slot
 *
 * menaqiyyot son copas de LIBACION: "asher yussaj bahen" (con que se
 * LIBARA, v.29). Libar = verter. DELETE: se vierte el contenido y
 * el slot queda vacio. El memorial (azjara) sigue acumulando -
 * incluso vaciar deja huella (Lv 24:7: la levona recuerda).
 *
 * Idempotente sobre VACIO (no-op silencioso). En ese caso, NO
 * consumimos rate: la operacion no hizo trabajo real.
 *
 * 2 testigos:
 *   Ex 25:29  - menaqiyyot asher yussaj bahen (con que se libara)
 *   Lv 24:7   - azjara persistente aun sin pan
 */
int hashmal_cache_menaqit_vaciar(uint8_t slot)
{
    hashmal_shuljan_t *s = hashmal_cache_shuljan_mut();
    if (s == NULL) {
        MESA_LOG_E("menaqit: Mesa no levantada");
        return HASHMAL_CACHE_ERR_INIT;
    }
    if (!hashmal_cache_slot_legitimo(slot)) {
        MESA_LOG_E("menaqit: slot %u fuera de rango", (unsigned)slot);
        return HASHMAL_CACHE_ERR_INDICE;
    }

    hashmal_lejem_t *l = &s->slots[slot];

    if (l->estado == HASHMAL_LEJEM_VACIO) {
        MESA_LOG_W("menaqit slot=%u: ya VACIO (idempotente, sin rate)",
                   (unsigned)slot);
        return HASHMAL_CACHE_OK;
    }

    /* v.25 - rate check. Consumimos cupo solo cuando hay trabajo. */
    int rc = hashmal_cache_misgeret_permitir();
    if (rc != HASHMAL_CACHE_OK) {
        MESA_LOG_W("menaqit: misgéret rechaza (rc=%d)", rc);
        return rc;
    }

    /* Verter el contenido. Preservamos la azjara para despues
     * registrar la libacion (memorial de TODAS las operaciones). */
    const uint64_t ahora = hashmal_cache_ahora_ms();
    memset(l->bytes,    0, sizeof(l->bytes));
    memset(l->zer_slot, 0, HASHMAL_ZER_HASH_BYTES);
    l->tamano                = 0u;
    l->timestamp_horneado_ms = 0u;
    l->timestamp_servido_ms  = 0u;
    l->estado                = HASHMAL_LEJEM_VACIO;
    /* P7 — slot vertido pierde sus rostros: vuelve a invalidos. */
    l->panim_origen          = HASHMAL_PANIM_INVALIDO;
    l->panim_destino         = HASHMAL_PANIM_INVALIDO;

    azjara_registrar(l, HASHMAL_UTENSILIO_MENAQIT, ahora);

    MESA_LOG_I("menaqít slot=%u — pan vertido (Éx 25:29)", (unsigned)slot);
    return HASHMAL_CACHE_OK;
}

/* ==================================================================
 * API PUBLICA - zer_verificar (v.24 - corona per-slot)
 * ================================================================== */

/**
 * hashmal_cache_zer_verificar - ?la corona del slot cierra?
 *
 * v.24 - "zer zahav saviv": la corona rodea. Aqui, per-slot:
 * recalcular SHA-256(slot.bytes[0..tamano]) y comparar con
 * slot.zer_slot. Si casa, el pan es integro; si no, alguien toco
 * la madera y marcamos el slot EXPIRADO (lo consumira el proximo
 * ya'arejennu, Lv 24:8).
 *
 * VACIO -> OK automatico (nada que verificar; zer vacio = zer vacio).
 *
 * 2 testigos:
 *   v.24    - zer zahav saviv (corona per-slot)
 *   Lv 24:8 - ya'arejennu retira el viejo; el corrupto tambien
 */
int hashmal_cache_zer_verificar(uint8_t slot)
{
    hashmal_shuljan_t *s = hashmal_cache_shuljan_mut();
    if (s == NULL) {
        MESA_LOG_E("zer_verificar: Mesa no levantada");
        return HASHMAL_CACHE_ERR_INIT;
    }
    if (!hashmal_cache_slot_legitimo(slot)) {
        MESA_LOG_E("zer_verificar: slot %u fuera de rango", (unsigned)slot);
        return HASHMAL_CACHE_ERR_INDICE;
    }

    hashmal_lejem_t *l = &s->slots[slot];

    /* Slot vacio - nada que verificar. zer_slot debe ser todo ceros,
     * y bytes tambien; cualquiera de ambos estando limpio es
     * coherente. OK automatico sin gasto de mbedtls. */
    if (l->estado == HASHMAL_LEJEM_VACIO) {
        return HASHMAL_CACHE_OK;
    }

    if (!zer_slot_coincide(l)) {
        /* Corona rota. Aplicamos la misma politica que kaf_tomar:
         * marcar EXPIRADO para que el Shabat lo retire. No borramos
         * aqui - el diagnostico puede requerir inspeccion posterior. */
        l->estado = HASHMAL_LEJEM_EXPIRADO;
        MESA_LOG_E("zer_verificar: slot %u — corona ROTA, marcado "
                   "EXPIRADO (v.24)", (unsigned)slot);
        return HASHMAL_CACHE_ERR_ZER_ROTO;
    }

    return HASHMAL_CACHE_OK;
}
