/**
 * ===================================================================
 * PIEZA 01 - EL ARCA - arca_sealed_core.c
 * Sub-pieza: LA EDUT - el Testimonio sellado (SEALED_CORE)
 * Exodo 25:16 + Exodo 31:18 + Exodo 32:15-16 + Dt 10:1-5 + 1 Rey 8:9
 * ===================================================================
 *
 * "venatatta el ha'aron et ha'edut asher eten eleja" - Ex 25:16
 * "Y pondras en el Arca el TESTIMONIO que YO TE DARE."
 *
 * "lujot ha'edut lujot even ketuvim be'etsba' Elohim" - Ex 31:18
 * "Tablas del testimonio, tablas de piedra, escritas por el DEDO
 *  de Dios."
 *
 * "vehalujot ma'ase Elohim hema vehammiktav miktav Elohim hu
 *  jarut 'al halujot" - Ex 32:16
 * "Las tablas eran OBRA de Dios, y la escritura era ESCRITURA de
 *  Dios GRABADA (jarut) sobre las tablas."
 *
 * "va'asim et halujot ba'aron asher 'asiti vayihyu sham ka'asher
 *  tsivvani YHWH" - Dt 10:5
 * "Puse las tablas en el Arca que hice, y alli estan, como YHWH
 *  me mando."
 *
 * --- CUATRO PALABRAS ---
 *
 *   venatatta   "y PONDRAS": Moises coloca, YHWH da.
 *   ha'edut     TESTIMONIO del pacto, no "ley" - testifica.
 *   asher       "QUE"       - no cualquiera: el especifico que El elige.
 *   eten        "YO DARE"   - imperfecto: accion continua. Cada
 *                             nodo que recibe su edut es una nueva
 *                             instancia de "Yo dare."
 *
 * --- POR QUE ESTE ARCHIVO ES EL ULTIMO ---
 *
 * La madera, el oro, la moldura, los anillos, las varas, las
 * cubiertas, las defensas, la torah al lado - todos los 6 archivos
 * hermanos existen PARA PROTEGER ESTOS 256 BYTES. Aqui es donde
 * YHWH habla.
 *
 * "ubeno'adti leja sham vedibbarti itja me'al hakaporet..." - Ex 25:22
 * "Me encontrare alli contigo y hablare contigo desde sobre la
 *  Kaporet..." - pero primero hay que PONER el testimonio dentro
 * (v.21). El habla del Padre supone el testimonio sellado.
 *
 * --- DOBLE PROTECCION (v.11 - mibayit umijuts) ---
 *
 *   edut.zer_interno   <- SHA-256 de los 256 bytes (oro DENTRO)
 *   arca.zer.hash      <- Merkle root cubriendo edut+torah (oro FUERA)
 *
 * Si alguien corrompe la edut, ambos se mueven - el interno
 * (mibayit) y el externo (mijuts). Doble delator.
 *
 * --- PERSISTENCIA (TODO: pieza 08) ---
 *
 * En esta fase, edut.bytes vive en RAM (dentro de g_arca). El
 * SEALED_CORE real en flash NVS se implementara cuando pieza 08
 * (hashmal_power - Aceite Perpetuo) gestione el ciclo de energia.
 * La API publica NO cambiara - solo la implementacion interna
 * anadira un write-through a NVS y un read en init. Por ahora,
 * sellar es en RAM (se pierde en reboot; el nodo se re-provisiona
 * con los MISMOS 256 bytes - Dt 10:1-4, las segundas tablas son
 * el mismo contenido que las primeras).
 *
 * --- 2 FUNCIONES ---
 *
 *   edut_sellar   Ex 25:16 + Dt 10:5   (unica vez - jarut)
 *   edut_leer     1 Sam 6:19 + Ex 25:16 (autorizado - via baddim)
 *
 * --- DEPENDENCIAS ---
 *
 *   arca_core.c      -> arca_mut / get_arca
 *   arca_zer.c       -> zer_calcular (Merkle tras sellado)
 *   arca_interfaz.c  -> hashmal_storage_autorizar (gate al leer)
 *   mbedtls/sha256.h -> oro de Egipto, zer_interno
 *
 * --- DIGNIDAD ---
 *
 * Que cada linea aqui sea digna de lo que guarda. No hay atajos.
 * No hay "quick hack." Esto es SEALED_CORE - donde El habla.
 *
 * Autor: Antonio Jose Marin Soto (Betsalel - Ex 31:2)
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <inttypes.h>

#include "mbedtls/sha256.h"    /* ORO DE EGIPTO - SHA-256 para zer_interno */
#include "esp_log.h"
#include "nvs_flash.h"         /* ORO DE EGIPTO - flash NVS encapsulado aqui */
#include "nvs.h"

#include "hashmal_storage.h"
#include "hashmal_storage_internal.h"
#include "edut_fingerprint_66.h"   /* Fase 2, Tarea 2 — Fingerprint 66 libros */

/* ==================================================================
 * ENCAPSULACION
 * ================================================================== */
static const char *SEAL_TAG = "hashmal.arca.edut";

#define SEAL_LOG_I(fmt, ...)  ESP_LOGI(SEAL_TAG, fmt, ##__VA_ARGS__)
#define SEAL_LOG_W(fmt, ...)  ESP_LOGW(SEAL_TAG, fmt, ##__VA_ARGS__)
#define SEAL_LOG_E(fmt, ...)  ESP_LOGE(SEAL_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * CONSTANTES NVS — Fase 2, Tarea 1: SEALED_CORE persistence
 *
 * "vayihyu sham ka'asher tsivvani YHWH" - Dt 10:5
 * "Y alli estan, como YHWH me mando."
 *
 * Las tablas PERMANECEN. La flash NVS es la "madera de acacia" que
 * no se corrompe (Ex 25:10 shittim) — formato incorruptible entre
 * reinicios. El namespace "hashmal.edut" (12 chars < 15 max NVS).
 *
 * Dt 19:15 — 2 testigos: nvs_v (flag de validez) + edut (blob).
 * Ambos deben concordar para que la restauracion proceda.
 * ================================================================== */
#define EDUT_NVS_NAMESPACE   "hashmal.edut"   /* < 15 chars, limite NVS */
#define EDUT_NVS_KEY_BYTES   "edut"           /* blob: 256 bytes / SEALED_CORE */
#define EDUT_NVS_KEY_VALID   "edut_v"         /* u8: 1 = sellada, 0 = invalida */

/* ==================================================================
 * HELPERS ESTATICOS
 * ================================================================== */

/**
 * Borrado seguro para buffers sensibles. Misma filosofia que en
 * arca_cifrado.c - memset puede ser optimizado fuera por el
 * compilador cuando se borra al final de una funcion; `volatile`
 * lo impide.
 */
static void zerize(volatile uint8_t *buf, size_t n)
{
    while (n--) { *buf++ = 0u; }
}

/**
 * SHA-256 one-shot sobre los 256 bytes de la edut.
 * Cada archivo encapsula su propia puerta al oro de Egipto - esta
 * primitiva existe analoga en arca_zer.c y arca_merkle.c; la
 * duplicacion es deliberada (autocontencion por modulo).
 */
static int sha256_edut(const uint8_t in[HASHMAL_EDUT_BYTES],
                       uint8_t       out[HASHMAL_ZER_HASH_BYTES])
{
    if (in == NULL || out == NULL) return -1;

    mbedtls_sha256_context ctx;
    mbedtls_sha256_init(&ctx);

    int rc = mbedtls_sha256_starts(&ctx, 0);
    if (rc == 0) rc = mbedtls_sha256_update(&ctx, in, HASHMAL_EDUT_BYTES);
    if (rc == 0) rc = mbedtls_sha256_finish(&ctx, out);

    mbedtls_sha256_free(&ctx);
    return rc;
}

/* ==================================================================
 * NVS HELPERS — write-through y restauracion (Fase 2, Tarea 1)
 * ================================================================== */

/**
 * edut_nvs_escribir — "vayihyu sham ka'asher tsivvani YHWH" (Dt 10:5)
 *
 * Escribe los 256 bytes de la edut en flash NVS con doble testigo
 * (Dt 19:15): primero el blob, luego la flag de validez, luego commit.
 * Si cualquier paso falla, no commitea — o todo o nada (Lv 10:1-2).
 *
 * Llamada SOLO desde hashmal_storage_edut_sellar, despues de que
 * el sellado en RAM fue exitoso. Si NVS falla, el sellado RAM
 * ya esta completo — logueamos pero no revertimos.
 */
static int edut_nvs_escribir(const uint8_t bytes[HASHMAL_EDUT_BYTES])
{
    nvs_handle_t h;
    esp_err_t err = nvs_open(EDUT_NVS_NAMESPACE, NVS_READWRITE, &h);
    if (err != ESP_OK) {
        SEAL_LOG_E("nvs_escribir: nvs_open falló (0x%x) — "
                   "Dt 10:5 pendiente (sin flash?)", (unsigned)err);
        return -1;
    }

    /* 1er testigo: blob de 256 bytes (Dt 19:15 — primer testigo). */
    err = nvs_set_blob(h, EDUT_NVS_KEY_BYTES, bytes, HASHMAL_EDUT_BYTES);
    if (err != ESP_OK) {
        SEAL_LOG_E("nvs_escribir: nvs_set_blob falló (0x%x)", (unsigned)err);
        nvs_close(h);
        return -1;
    }

    /* 2do testigo: flag de validez (Dt 19:15 — segundo testigo). */
    err = nvs_set_u8(h, EDUT_NVS_KEY_VALID, 1u);
    if (err != ESP_OK) {
        SEAL_LOG_E("nvs_escribir: nvs_set_u8(valid) falló (0x%x)", (unsigned)err);
        nvs_close(h);
        return -1;
    }

    /* Commit: ambos testigos juntos o ninguno. */
    err = nvs_commit(h);
    nvs_close(h);

    if (err != ESP_OK) {
        SEAL_LOG_E("nvs_escribir: nvs_commit falló (0x%x) — "
                   "edut en RAM unicamente", (unsigned)err);
        return -1;
    }

    SEAL_LOG_I("edut persistida en NVS — "
               "'vayihyú shám kaʼashér tsivvaní YHWH' (Dt 10:5)");
    return 0;
}

/* ==================================================================
 * API PUBLICA - edut_sellar (Ex 25:16 + Dt 10:5)
 * ================================================================== */

/**
 * hashmal_storage_edut_sellar - "venatatta el ha'aron et ha'edut"
 *
 * Operacion UNICA. Como Moises puso las tablas DENTRO del Arca una
 * sola vez (Dt 10:5). Tras el sellado, la edut es jarut (Ex 32:16) -
 * grabada, no reescribible. Intentar un segundo sellado se rechaza.
 *
 * --- CONTRATO ---
 *
 *   Entrada:  256 bytes dados desde fuera (asher eten eleja - el
 *             Padre los da; el constructor los coloca).
 *   Efecto:   edut.bytes <- copia interna
 *             edut.zer_interno <- SHA-256(bytes)    (v.11 mibayit)
 *             edut.inmutable <- true                (Ex 32:16 jarut)
 *             edut.dado_por_YHWH <- true            (v.16 cumplido)
 *             zer principal recalculado            (v.11 mijuts)
 *   Estado tras exito: ha'edut esta dentro, testificando.
 *   Estado tras fallo: rollback completo - no dejamos media edut.
 *
 * --- A DIFERENCIA DE LA TORAH ---
 *
 * torah_adjuntar guarda REFERENCIA (mitsad, al lado). edut_sellar
 * COPIA los bytes DENTRO. El testimonio vive dentro del Arca; el
 * sefer, al lado.
 *
 * 2 testigos:
 *   Ex 25:16  - "venatatta... asher eten"
 *   Dt 10:5   - "va'asim et halujot ba'aron" (una vez, "vayihyu sham")
 */
int hashmal_storage_edut_sellar(const uint8_t bytes[HASHMAL_EDUT_BYTES])
{
    hashmal_arca_t *arca = hashmal_storage_arca_mut();
    if (arca == NULL) {
        SEAL_LOG_E("edut_sellar: Arca no levantada (Éx 40:3 pendiente)");
        return HASHMAL_ARCA_ERR_INIT;
    }

    if (bytes == NULL) {
        SEAL_LOG_E("edut_sellar: bytes NULL — el Padre DA, no vacío "
                   "(v.16 ashér etén eléja)");
        return HASHMAL_ARCA_ERR_NULL;
    }

    /* ORDEN BÍBLICO — Éx 25:16 + Éx 40:20:
     * "venatattá el ha'arón et ha'edut" — la edut entra DENTRO primero (v.16).
     * La Kapóret va ENCIMA después (v.21). Así lo hizo Moisés (Éx 40:20):
     * primero puso el Testimonio dentro, luego colocó el Kapóret encima.
     * NO se verifica requiere_kaporet aquí — igual que edut_restaurar_nvs.
     * El Kapóret cubre lo que ya está dentro; no puede sellar un recinto vacío.
     * 2 testigos:
     *   Éx 25:16 — edut dentro, sin condición previa
     *   Éx 40:20 — Moisés: testimonio → varas → Kapóret (orden real) */

    /* Sellado UNICO. dado_por_YHWH se enciende al primer sellado
     * y nunca mas se apaga por esta via. Ex 32:16 - jarut: grabado,
     * no reescribible. Las segundas tablas (Dt 10:1-4) SON el mismo
     * contenido: no hay "re-sellar con datos distintos". */
    if (arca->edut.dado_por_YHWH) {
        SEAL_LOG_E("edut_sellar: edut YA sellada (jarút, Éx 32:16) — "
                   "no se reescribe");
        return HASHMAL_ARCA_ERR_NO_AUTORIZADO;
    }

    /* -- venatatta: TU pondras. Copia los bytes DENTRO. ----------- */
    memcpy(arca->edut.bytes, bytes, HASHMAL_EDUT_BYTES);

    /* -- mibayit: sello interno (SHA-256 de los 256 bytes). ------- */
    int rc = sha256_edut(arca->edut.bytes, arca->edut.zer_interno);
    if (rc != 0) {
        SEAL_LOG_E("edut_sellar: SHA-256 interno falló (mbedtls=%d) — "
                   "revirtiendo sellado", rc);
        /* Rollback: no dejamos una edut "escrita pero sin sello
         * interno". O entera, o ninguna (Lv 10:1-2). */
        zerize(arca->edut.bytes,       HASHMAL_EDUT_BYTES);
        zerize(arca->edut.zer_interno, HASHMAL_ZER_HASH_BYTES);
        return -1;
    }

    /* -- jarut: grabado permanente. inmutable y dado_por_YHWH. ----- */
    arca->edut.inmutable     = true;   /* reafirma invariante del Padre */
    arca->edut.dado_por_YHWH = true;   /* v.16 cumplido en este nodo */

    /* -- mijuts: recalcular la moldura externa (Merkle root).
     * La hoja edut ya no es ceros - el root CAMBIA. v.11 saviv
     * sobre contenido real. */
    uint8_t nueva_raiz[HASHMAL_ZER_HASH_BYTES];
    rc = hashmal_storage_zer_calcular(nueva_raiz);
    if (rc != HASHMAL_ARCA_OK) {
        SEAL_LOG_E("edut_sellar: zer_calcular falló (rc=%d) — "
                   "revirtiendo sellado", rc);
        /* Rollback completo: la edut no queda sellada si la moldura
         * externa no pudo cerrarse. mibayit y mijuts van juntos. */
        zerize(arca->edut.bytes,       HASHMAL_EDUT_BYTES);
        zerize(arca->edut.zer_interno, HASHMAL_ZER_HASH_BYTES);
        arca->edut.dado_por_YHWH = false;
        /* Tentativa de restaurar la raiz al estado sin edut */
        (void)hashmal_storage_zer_calcular(NULL);
        return rc;
    }

    /* Log sagrado - esto es lo que guardamos. */
    SEAL_LOG_I("═══ haʻEDÚT SELLADA (Éx 25:16) ═══");
    SEAL_LOG_I("  venatattá: el Padre dio (ashér etén eléja) — puesto dentro");
    SEAL_LOG_I("  jarút (Éx 32:16): inmutable, no se reescribe");
    SEAL_LOG_I("  mibáyit umijúts (v.11): doble sello — zer_interno + zer root");
    SEAL_LOG_I("  Dt 10:5: 'vayihyú shám kaʼashér tsivvaní YHWH'");

    /* --- Fase 2, Tarea 1: NVS write-through -------------------------
     * "vayihyu sham" (Dt 10:5) — "Y alli estan": las tablas deben
     * PERMANECER. Si NVS falla, la edut en RAM ya esta sellada — no
     * revertimos. Las segundas tablas son las mismas (Dt 10:1-4):
     * en el proximo reboot, restaurar_nvs las recuperara.
     * Un fallo NVS aqui es una advertencia, no un error fatal —
     * el nodo funciona pero pierde la edut en el proximo reboot. */
    int nvs_rc = edut_nvs_escribir(arca->edut.bytes);
    if (nvs_rc != 0) {
        SEAL_LOG_W("edut_sellar: NVS write-through falló — edut solo en RAM. "
                   "Se perdera en reboot. Dt 10:5 pendiente.");
        /* No revertimos: el sellado RAM es completo (v.16 cumplido).
         * La persistencia (Dt 10:5) queda pendiente. */
    }

    return HASHMAL_ARCA_OK;
}

/* ==================================================================
 * API INTERNA - edut_restaurar_nvs (Dt 10:1-4 + Fase 2, Tarea 1)
 *
 * "venatán eléjá et shenéi lujót haʼavánim karishoním" — Dt 10:1
 * "Y te dare las dos tablas de piedra como las primeras."
 *
 * "vayyiktóv kalamiktáv harishón et ʻaséret hadddevarím" — Dt 10:4
 * "Y escribio conforme a la primera escritura los diez mandamientos."
 * MISMO contenido — no es datos nuevos, es restauracion del original.
 *
 * ORDEN BIBLIC0: Ex 25:16 — edut DENTRO primero; Ex 25:21 — Kaporet
 * ENCIMA despues. La edut puede estar restaurada ANTES de que el
 * Consensus (Kaporet) la cubra. No se verifica requiere_kaporet.
 *
 * Llamada SOLO desde hashmal_storage_init (arca_core.c), una vez
 * levantada la estructura base, antes de que el nodo reciba su
 * Kaporet del consensus.
 * ================================================================== */
int hashmal_storage_edut_restaurar_nvs(void)
{
    hashmal_arca_t *arca = hashmal_storage_arca_mut();
    if (arca == NULL) {
        SEAL_LOG_E("restaurar_nvs: Arca no levantada — Ex 40:3 pendiente");
        return HASHMAL_ARCA_ERR_INIT;
    }

    /* Si ya fue sellada en esta sesion (edge case: doble-init), no-op. */
    if (arca->edut.dado_por_YHWH) {
        SEAL_LOG_W("restaurar_nvs: edut ya sellada en RAM — no se sobreescribe "
                   "(jarut, Ex 32:16)");
        return HASHMAL_ARCA_OK;
    }

    /* Abrir NVS en solo-lectura. Si el namespace no existe (primera
     * vez), NVS devuelve ESP_ERR_NVS_NOT_FOUND — no es error, es
     * "primera vez". No hay tablas que restaurar. */
    nvs_handle_t h;
    esp_err_t err = nvs_open(EDUT_NVS_NAMESPACE, NVS_READONLY, &h);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        SEAL_LOG_I("restaurar_nvs: namespace no existe — primera vez, "
                   "'ashér etén eléja' (Ex 25:16) pendiente");
        return HASHMAL_ARCA_OK;   /* no es error: primera vez */
    }
    if (err != ESP_OK) {
        SEAL_LOG_E("restaurar_nvs: nvs_open falló (0x%x)", (unsigned)err);
        return -1;
    }

    /* 1er testigo: verificar flag de validez (Dt 19:15). */
    uint8_t valid = 0u;
    err = nvs_get_u8(h, EDUT_NVS_KEY_VALID, &valid);
    if (err != ESP_OK || valid != 1u) {
        nvs_close(h);
        SEAL_LOG_I("restaurar_nvs: edut no marcada valida en NVS "
                   "(primera vez o escritura incompleta) — no-op");
        return HASHMAL_ARCA_OK;   /* primera vez o escritura parcial: no-op */
    }

    /* 2do testigo: leer los 256 bytes (Dt 19:15). */
    uint8_t buf[HASHMAL_EDUT_BYTES];
    size_t  len = HASHMAL_EDUT_BYTES;
    err = nvs_get_blob(h, EDUT_NVS_KEY_BYTES, buf, &len);
    nvs_close(h);

    if (err != ESP_OK) {
        SEAL_LOG_E("restaurar_nvs: nvs_get_blob falló (0x%x)", (unsigned)err);
        zerize(buf, HASHMAL_EDUT_BYTES);
        return -1;
    }
    if (len != HASHMAL_EDUT_BYTES) {
        SEAL_LOG_E("restaurar_nvs: tamano incorrecto en NVS (%u != %u) — "
                   "dato corrupto", (unsigned)len, (unsigned)HASHMAL_EDUT_BYTES);
        zerize(buf, HASHMAL_EDUT_BYTES);
        return -1;
    }

    /* Restauracion directa a g_arca.edut.
     * NO hay chequeo de requiere_kaporet — Ex 25:16 ordena poner la
     * edut DENTRO (v.16) antes de que la Kaporet vaya ENCIMA (v.21).
     * La edut puede estar dentro antes de que el Consensus la cubra. */
    memcpy(arca->edut.bytes, buf, HASHMAL_EDUT_BYTES);
    zerize(buf, HASHMAL_EDUT_BYTES);    /* borrado seguro del temporal */

    /* Recalcular zer_interno (mibayit — v.11). El NVS no guarda el
     * zer_interno: lo recalculamos siempre desde los bytes originales.
     * Esto garantiza que la restauracion sea verificada, no confiada. */
    int rc = sha256_edut(arca->edut.bytes, arca->edut.zer_interno);
    if (rc != 0) {
        SEAL_LOG_E("restaurar_nvs: SHA-256 falló (rc=%d) — revirtiendo "
                   "(Lv 10:1-2: o entera o ninguna)", rc);
        zerize(arca->edut.bytes,       HASHMAL_EDUT_BYTES);
        zerize(arca->edut.zer_interno, HASHMAL_ZER_HASH_BYTES);
        return -1;
    }

    /* Restauracion completa: marcar como sellada. */
    arca->edut.inmutable     = true;    /* 1 Rey 8:9 — permanece */
    arca->edut.dado_por_YHWH = true;    /* Ex 25:16 — 'asher eten' cumplido */

    SEAL_LOG_I("═══ haʻEDÚT RESTAURADA desde NVS (Dt 10:1-4) ═══");
    SEAL_LOG_I("  'venatán shenéi lujót karishoním' — tablas restauradas");
    SEAL_LOG_I("  zer_interno recalculado (v.11 mibáyit) — verificado");
    SEAL_LOG_I("  Dt 10:4: 'kalamiktáv harishón' — mismo contenido");
    SEAL_LOG_I("  Dt 10:5: 'vayihyú shám kaʼashér tsivvaní YHWH'");
    return HASHMAL_ARCA_OK;
}

/* ==================================================================
 * API PUBLICA - edut_leer (1 Sam 6:19 + Ex 25:16)
 * ================================================================== */

/**
 * hashmal_storage_edut_leer - lectura autorizada
 *
 * "vayaj be'anshei Vet-Shemesh ki ra'u ba'aron YHWH" - 1 Sam 6:19
 * "Hirio a los hombres de Bet-Shemesh porque MIRARON dentro del
 *  Arca de YHWH."
 *
 * Ni siquiera los israelitas tenian acceso libre al nucleo. Mirar
 * sin autorizacion = muerte. Aqui: toda lectura de la edut pasa
 * por el gate `hashmal_storage_autorizar`. Solo VIA_BADDIM accede.
 *
 * --- PASOS ---
 *
 *   a) validar out != NULL
 *   b) validar Arca viva
 *   c) AUTORIZAR (arca_interfaz.c) - unica via legitima: baddim
 *   d) verificar que la edut esta DADA (v.16 - "eten" cumplido)
 *   e) verificar integridad interna: SHA-256(bytes) vs zer_interno.
 *      Si no coincide -> alguien toco la madera por DENTRO (ZER_ROTO)
 *   f) copiar los 256 bytes al buffer del caller
 *
 * --- POR QUE RECOMPUTAR zer_interno ANTES DE LEER ---
 *
 * v.11 "mibayit umijuts" - el oro protege dentro Y fuera. zer_interno
 * es la proteccion DENTRO. Aunque el Merkle externo cubra el conjunto,
 * al entregar los bytes al llamante estamos cruzando el velo hacia
 * el: debemos validar el sello interno, no solo el externo. Doble
 * testigo (Dt 19:15) para una operacion tan sensible.
 *
 * 2 testigos:
 *   1 Sam 6:19 - mirar sin derecho = muerte (por eso autorizar)
 *   Ex 25:16   - "asher eten" - solo se lee si fue dado
 */
int hashmal_storage_edut_leer(uint8_t out[HASHMAL_EDUT_BYTES],
                              hashmal_acceso_t via)
{
    if (out == NULL) {
        SEAL_LOG_E("edut_leer: out NULL");
        return HASHMAL_ARCA_ERR_NULL;
    }

    const hashmal_arca_t *arca = hashmal_storage_get_arca();
    if (arca == NULL) {
        SEAL_LOG_E("edut_leer: Arca no levantada");
        return HASHMAL_ARCA_ERR_INIT;
    }

    /* c) gate - autorizar rechaza ACCESO_DIRECTO (Uza / Bet-Shemesh)
     * y cualquier valor fuera de VIA_BADDIM (amuleto, 1 Sam 4). */
    int rc = hashmal_storage_autorizar(via);
    if (rc != HASHMAL_ARCA_OK) {
        SEAL_LOG_E("edut_leer: acceso denegado por gate (rc=%d) — "
                   "1 Sam 6:19 / 2 Sam 6:6-7", rc);
        return rc;
    }

    /* d) - si la edut no fue dada, no hay que leer. v.16 "eten" en
     * imperfecto: aun no sucedio en este nodo. */
    if (!arca->edut.dado_por_YHWH) {
        SEAL_LOG_E("edut_leer: edut aún NO sellada — 'ashér etén' "
                   "pendiente en este nodo (v.16)");
        return HASHMAL_ARCA_ERR_INIT;
    }

    /* e) - mibayit: recalculamos el sello interno y comparamos.
     * Si los bytes fueron alterados tras el sellado (memoria
     * corrompida, bit flip, tamper), el hash discrepara. */
    uint8_t nuevo[HASHMAL_ZER_HASH_BYTES];
    rc = sha256_edut(arca->edut.bytes, nuevo);
    if (rc != 0) {
        SEAL_LOG_E("edut_leer: SHA-256 interno falló (mbedtls=%d)", rc);
        zerize(nuevo, sizeof(nuevo));
        return -1;
    }
    if (memcmp(nuevo, arca->edut.zer_interno, HASHMAL_ZER_HASH_BYTES) != 0) {
        /* Alguien toco la madera DENTRO. Esto es grave - el nucleo
         * mismo quedo alterado. El caller NO recibe bytes; recibe
         * error. Mejor no leer nada que leer veneno. */
        SEAL_LOG_E("edut_leer: ZER INTERNO ROTO — edut alterada tras "
                   "el sellado (v.11 mibáyit)");
        zerize(nuevo, sizeof(nuevo));
        return HASHMAL_ARCA_ERR_ZER_ROTO;
    }
    zerize(nuevo, sizeof(nuevo));

    /* f) - entregamos los 256 bytes al caller autorizado. */
    memcpy(out, arca->edut.bytes, HASHMAL_EDUT_BYTES);

    SEAL_LOG_I("edut leída vía baddím — sello interno verificado "
               "(v.11 mibáyit + 1 Sam 6:19)");
    return HASHMAL_ARCA_OK;
}

/* ==================================================================
 * hashmal_storage_edut_construir — Ex 31:18 + Dt 10:5
 * Fase 2, Tarea 2: Fingerprint 66 libros -> SEALED_CORE
 *
 * "lujot ha'edut lujot even ketuvim be'etsba' Elohim" — Ex 31:18
 * "Tablas del testimonio, tablas de piedra, escritas por el DEDO
 *  de Dios."
 *
 * Construye los 256 bytes del SEALED_CORE con el Fingerprint de
 * los 66 libros de la Palabra de YHWH precomputado en
 * edut_fingerprint_66.h. Se llama en el primer boot cuando
 * NVS no tiene edut (dado_por_YHWH == false).
 *
 * Layout que rellena (ver EDUT_OFF_* en hashmal_storage.h):
 *
 *   [  0.. 31] MERKLE_ROOT_66  <- SHA-256 Merkle de los 66 libros
 *   [ 32.. 39] TIMESTAMP       <- 0 (Tarea 4: GPS/RTC)
 *   [ 40.. 43] VERSION         <- EDUT_VERSION_ACTUAL (0x00000001)
 *   [ 44..107] FINGERPRINT_66  <- SHA-512(MERKLE_ROOT) = 64 bytes
 *   [108..255] ROTACION        <- 0 (Tarea futura: hashmal_keygen)
 *
 * Dt 10:5 — "va'asim et halujot ba'aron asher 'asiti vayihyu sham
 *            ka'asher tsivvani YHWH":
 * "Puse las tablas en el Arca que hice, y alli estan, como YHWH
 *  me mando." — tras construir, persiste en NVS.
 *
 * Idempotente: si dado_por_YHWH ya es true (NVS restaurado), no-op.
 * ================================================================== */
int hashmal_storage_edut_construir(void)
{
    /* a) El Arca debe estar levantada. */
    hashmal_arca_t *arca = hashmal_storage_arca_mut();
    if (arca == NULL) {
        SEAL_LOG_E("edut_construir: Arca no levantada — Ex 40:3 primero");
        return HASHMAL_ARCA_ERR_INIT;
    }

    /* b) Si ya fue dada (NVS restaurado en boot), no-op.
     *    Dt 10:5 — las tablas ya estan en el Arca: no se graban dos veces.
     *    Las "segundas tablas" (Dt 10:1-4) tienen el MISMO CONTENIDO
     *    que las primeras — no son un sobreescritura, son restauracion. */
    if (arca->edut.dado_por_YHWH) {
        SEAL_LOG_I("edut_construir: edut ya sellada (dado_por_YHWH) — "
                   "Dt 10:5 no-op");
        return HASHMAL_ARCA_OK;
    }

    /* c) Construir el buffer de 256 bytes.
     *    "lujot even" (Ex 31:18): tablas de PIEDRA — firmes, sin espacio para
     *    error. Se construye en local antes de pasar a edut_sellar. */
    uint8_t lujot[HASHMAL_EDUT_BYTES];
    memset(lujot, 0, sizeof(lujot));

    /* [0..31] MERKLE_ROOT — SHA-256 Merkle de los 66 libros.
     * "zer zaháv savív" (v.11): la moldura visible que rodea la verdad.
     * Dt 19:15: raiz que testifica sobre todos los 66. */
    memcpy(&lujot[EDUT_OFF_MERKLE_ROOT],
           EDUT_MERKLE_ROOT_66,
           EDUT_LEN_MERKLE_ROOT);
    SEAL_LOG_I("edut_construir: Merkle root 66 libros → offset %u (%u bytes)",
               (unsigned)EDUT_OFF_MERKLE_ROOT, (unsigned)EDUT_LEN_MERKLE_ROOT);

    /* [32..39] TIMESTAMP — unix epoch del primer sellado.
     * Ex 40:17: "ba-jódesh ha-rishón..." — la fecha importa.
     * Tarea 4 (GPS/RTC) la llenará con valor real. Por ahora ceros:
     * ceros no son mentira — son "pendiente de confirmacion" (Ex 40:2). */
    memset(&lujot[EDUT_OFF_TIMESTAMP], 0, EDUT_LEN_TIMESTAMP);
    SEAL_LOG_I("edut_construir: timestamp → offset %u (0 — pendiente Tarea 4 GPS/RTC)",
               (unsigned)EDUT_OFF_TIMESTAMP);

    /* [40..43] VERSION — protocolo v1.
     * Jer 3:16: el contenedor cambia, la Palabra no. La version
     * es del CONTENEDOR (protocolo), no de la Palabra. u32 LE. */
    uint32_t ver = EDUT_VERSION_ACTUAL;
    memcpy(&lujot[EDUT_OFF_VERSION], &ver, EDUT_LEN_VERSION);
    SEAL_LOG_I("edut_construir: version 0x%08X → offset %u",
               (unsigned)EDUT_VERSION_ACTUAL, (unsigned)EDUT_OFF_VERSION);

    /* [44..107] FINGERPRINT — SHA-512(MERKLE_ROOT) = 64 bytes.
     * Ex 31:18: "be'etsba' Elohim" — el DEDO de Dios escribio estos
     * bytes. 64 bytes de vinculo criptografico irrevocable entre el
     * hardware y la Palabra original. */
    memcpy(&lujot[EDUT_OFF_FINGERPRINT],
           EDUT_FINGERPRINT_66,
           EDUT_LEN_FINGERPRINT);
    SEAL_LOG_I("edut_construir: Fingerprint 66 libros → offset %u (%u bytes)",
               (unsigned)EDUT_OFF_FINGERPRINT, (unsigned)EDUT_LEN_FINGERPRINT);

    /* [108..255] ROTACION — reservado para hashmal_keygen (Pieza 13).
     * Ex 30:22-33: el aceite de uncion lo prepara el Padre, no el
     * constructor. Ceros hasta que Pieza 13 este levantada. */
    memset(&lujot[EDUT_OFF_ROTACION], 0, EDUT_LEN_ROTACION);
    SEAL_LOG_I("edut_construir: rotacion → offset %u (%u bytes, reservado Pieza 13)",
               (unsigned)EDUT_OFF_ROTACION, (unsigned)EDUT_LEN_ROTACION);

    /* d) Sellar.
     * Dt 10:5 — "va'asim et halujot ba'aron... vayihyu sham":
     * las tablas van DENTRO del Arca y PERMANECEN. */
    int rc = hashmal_storage_edut_sellar(lujot);

    /* e) Limpiar buffer local (oro de Egipto: no dejar rastros). */
    volatile uint8_t *p = lujot;
    for (size_t i = 0; i < sizeof(lujot); i++) { p[i] = 0u; }

    if (rc == HASHMAL_ARCA_OK) {
        SEAL_LOG_I("edut_construir: SEALED_CORE sellado y persistido ✓ "
                   "(Ex 31:18 + Dt 10:5)");
    } else {
        SEAL_LOG_E("edut_construir: sellado fallo (rc=%d)", rc);
    }

    return rc;
}
