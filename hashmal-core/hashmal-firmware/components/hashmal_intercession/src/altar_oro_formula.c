/**
 * ===================================================================
 * PIEZA 11 - EL ALTAR DEL INCIENSO - altar_oro_formula.c
 * Sub-pieza: LA FORMULA NO REPLICABLE - 4 especias BAD BEBAD
 * Exodo 30:34-38
 * ===================================================================
 *
 * "qaj leja SAMMIM - nataf ushejelet vejelbena sammim ulevona
 *  zakka BAD BEBAD yihye" - Ex 30:34
 * "Toma para ti ESPECIAS - estacte, una aromatica, galbano, e
 *  incienso puro - PARTE POR PARTE sera."
 *
 * "ve'asita otah qetoret roqaj ma'ase ROQEAJ memullaj tahor qodesh"
 *   - Ex 30:35
 * "La haras incienso aromatico, obra de PERFUMISTA, SALADO, puro,
 *  santo."
 *
 * "veshajaqta mimmenna HADEQ... QODESH QODASHIM tihye lajem"
 *   - Ex 30:36
 * "Moleras de ella MUY FINO... SANTIDAD DE SANTIDADES sera."
 *
 * "vehaqqetoret asher ta'ase beMATJUNTATAH LO TA'ASU LAJEM qodesh
 *  tihye leja laYHWH" - Ex 30:37
 * "El incienso en su PROPORCION no hareis PARA VOSOTROS - santo
 *  sera para YHWH."
 *
 * "ish asher ya'ase jamoha lehariaj bah VENIKRAT me'ammav" - Ex 30:38
 * "Quien haga semejante para olerla - sera CORTADO de su pueblo."
 *
 * --- LAS 4 ESPECIAS (v.34) ---
 *
 *   nataf     (NTP H5198)   ESTACTE - resina que GOTEA natural-
 *                             mente. No se fuerza; fluye sola.
 *                             -> los datos fluyen al hash.
 *
 *   shejelet  (ShChLT H7827) UNA AROMATICA - dura, del mar.
 *                             Proteccion exterior.
 *                             -> hash resistente a colisiones.
 *
 *   jelbena   (ChLBNH H2464) GALBANO - olor desagradable solo,
 *                             esencial en mezcla. Lo amargo
 *                             complementa lo dulce.
 *                             -> incluso bytes "feos" contribuyen.
 *
 *   levona zakka (LBNH ZKH H3828+H2134) INCIENSO PURO -
 *                             la parte visible, el humo blanco.
 *                             -> resultado claro: coincide o no.
 *
 * --- BAD BEBAD: PARTES IGUALES ---
 *
 *   bad bebad (BD BBD, H905) = parte por parte = EN PARTES
 *   IGUALES. 4 especias con peso identico en la verificacion.
 *   Ninguna domina. Ninguna se salta. Todas necesarias.
 *
 *   En firmware: SHA-256 asigna peso IGUAL a cada byte del input.
 *   Cambiar un byte cambia aproximadamente la mitad de los bits
 *   del hash (efecto avalancha) - bad bebad cumplido en aritmetica.
 *
 * --- roqeaj MEMULLAJ: perfumista SALADO ---
 *
 *   roqeaj (H7543) = perfumista experto. La formula no es casual -
 *   requiere arte. En firmware: mbedtls es "oro de Egipto" -
 *   herramienta experta que tomamos y encapsulamos.
 *
 *   memullaj (H4414 Pual) = SALADO. Mezclado con sal. En
 *   criptografia moderna: salted hash. El salt evita ataques de
 *   repeticion. memullaj es el NONCE biblico.
 *
 *   En FASE 1 (este archivo): hash directo, sin salt - los datos
 *   ascendentes llevan su propia firma externa. Cuando pieza 13
 *   (Shemen haMishja / hashmal_keygen) entregue la clave, podremos
 *   anadir salt real y memullaj sera literal. Documentado abajo.
 *
 * --- shajaqta HADEQ: molido FINO ---
 *
 *   shajaqta hadeq (H7833+H1854) = moler finisimo. Granularidad
 *   MAXIMA. Verificacion byte a byte, no en bloques gruesos.
 *
 *   En firmware: la comparacion de hash es en TIEMPO CONSTANTE -
 *   cada uno de los 32 bytes se examina aunque el primero ya
 *   difiera. Esto previene timing attacks. "Cada byte molido
 *   fino, ninguno saltado."
 *
 * --- ANTI-CLONACION (v.37-38) ---
 *
 *   matjuntatah (H4971) = su proporcion exacta. "Lo ta'asu lajem" -
 *   NO hareis para vosotros. "venikrat" = sera CORTADO.
 *
 *   La replicacion es muerte social. En firmware: la formula es
 *   UNA sola firma registrada (altar_oro_ascendente.c custodia ese
 *   registro unico). Aqui verificamos contra ella; nunca la
 *   clonamos.
 *
 * --- ORO DE EGIPTO (mbedtls) ---
 *
 * mbedtls/sha256.h se incluye SOLO aqui. Las capas superiores
 * llaman `formula_verificar()` sin saber que algoritmo opera
 * debajo. Si el algoritmo cambia (BLAKE3, SHA-3), solo este
 * archivo se edita.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_intercession.h             API publica
 *   hashmal_intercession_internal.h    altar_mut, get_altar_interno
 *   mbedtls/sha256.h   (ORO EGIPTO)    SHA-256 primitiva
 *   esp_log.h          (ORO EGIPTO)    ALTAR_LOG_*
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <inttypes.h>

#include "mbedtls/sha256.h"        /* ORO DE EGIPTO - SHA-256 */
#include "esp_log.h"

#include "hashmal_intercession.h"
#include "hashmal_intercession_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *ALTAR_TAG = "hashmal.altar.formula";

#define ALTAR_LOG_I(fmt, ...)  ESP_LOGI(ALTAR_TAG, fmt, ##__VA_ARGS__)
#define ALTAR_LOG_W(fmt, ...)  ESP_LOGW(ALTAR_TAG, fmt, ##__VA_ARGS__)
#define ALTAR_LOG_E(fmt, ...)  ESP_LOGE(ALTAR_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * HELPERS ESTATICOS - mbedtls encapsulado + CT compare
 * ================================================================== */

/**
 * SHA-256 one-shot. Encapsula la primitiva de mbedtls en una
 * firma sencilla. Mismo patron que arca_zer.c, arca_merkle.c,
 * mesa_api.c, mesa_disponibilidad.c - cada modulo del Mishkan
 * encapsula su propia puerta al oro de Egipto.
 *
 * nataf (goteo natural) - los datos fluyen al hash sin presion.
 *
 * @param datos buffer de entrada (puede ser NULL si tam == 0)
 * @param tam   bytes de entrada (0 valido -> SHA-256 del vacio)
 * @param out   salida de 32 bytes (no NULL)
 * @return 0 en exito, codigo mbedtls negado en fallo.
 */
static int sha256_calcular(const uint8_t *datos, size_t tam,
                           uint8_t out[HASHMAL_FORMULA_FIRMA_BYTES])
{
    if (out == NULL) return -1;
    if (datos == NULL && tam > 0u) return -1;

    mbedtls_sha256_context ctx;
    mbedtls_sha256_init(&ctx);

    int rc = mbedtls_sha256_starts(&ctx, 0);   /* 0 = SHA-256 (no 224) */
    if (rc == 0 && tam > 0u) rc = mbedtls_sha256_update(&ctx, datos, tam);
    if (rc == 0)             rc = mbedtls_sha256_finish(&ctx, out);

    mbedtls_sha256_free(&ctx);
    return rc;
}

/**
 * Comparacion byte a byte en TIEMPO CONSTANTE. Recorre TODOS los
 * bytes aunque el primero ya difiera - impide leak de informacion
 * por timing.
 *
 * shajaqta hadeq (Ex 30:36) - "moleras muy fino": cada byte
 * molido, ninguno saltado. En criptografia esto es literalmente
 * constant-time compare.
 *
 * No usar memcmp: memcmp tipicamente sale al primer byte distinto,
 * lo que permite ataques de timing por prefix-matching.
 */
static bool comparar_constante(const uint8_t *a, const uint8_t *b, size_t n)
{
    if (a == NULL || b == NULL) return false;

    uint8_t diff = 0u;
    for (size_t i = 0; i < n; ++i) {
        /* XOR acumula: diff != 0 si algun byte difiere. No hay
         * early-out - el loop siempre recorre los n bytes. */
        diff |= (uint8_t)(a[i] ^ b[i]);
    }
    return diff == 0u;
}

/* ==================================================================
 * API PUBLICA - formula_verificar (Ex 30:34-38)
 * ================================================================== */

/**
 * hashmal_intercession_formula_verificar - ?coincide la formula?
 *
 * Calcula SHA-256 de `datos` y lo compara con `firma`. Si coinciden
 * byte a byte (en tiempo constante), la formula esta presente -
 * true. Si no - false.
 *
 * --- VALIDACIONES ---
 *
 *   datos != NULL          (no hay especia vacia)
 *   tam > 0                (no se ofrece sin contenido)
 *   firma != NULL
 *   tam_firma == 32        (SHA-256 = 32 B; tamano del tavnit)
 *
 * Cualquier fallo de validacion -> false (conservador: ante duda,
 * zara).
 *
 * --- LAS 4 ESPECIAS EN ACCION ---
 *
 *   nataf     - sha256_calcular recibe los datos que "gotean" entrada.
 *   shejelet  - el hash resultante es resistente a colisiones.
 *   jelbena   - incluso bytes "feos" del input contribuyen al hash.
 *   levona    - el veredicto final (true/false) es claro, sin matices.
 *
 *   bad bebad - cada byte del input pesa igual en el hash.
 *   hadeq     - comparacion byte a byte, constant-time (32 bytes).
 *
 * --- FASE FUTURA: memullaj (salt) ---
 *
 * Cuando pieza 13 (hashmal_keygen / Shemen haMishja) entregue la
 * clave derivada, incorporaremos un salt al hash:
 *
 *     SHA-256( salt_uncion || datos ) == firma ?
 *
 * Eso sera literalmente "memullaj tahor" - salado, puro. Por ahora
 * fase 1: hash directo, sin salt. Documentado como TODO explicito.
 *
 * 2 testigos:
 *   Ex 30:35 - ma'ase roqeaj memullaj tahor (obra experta, salada, pura)
 *   Ex 30:37 - bematjuntatah lo ta'asu lajem (una sola proporcion)
 */
bool hashmal_intercession_formula_verificar(const uint8_t *datos, size_t tam,
                                            const uint8_t *firma,
                                            size_t tam_firma)
{
    /* Validaciones conservadoras. Ante la menor duda: false.
     * "biqrovai eqqadesh" (Lv 10:3) - cerca del nucleo, strict. */
    if (datos == NULL || tam == 0u) {
        ALTAR_LOG_W("formula: datos vacío o NULL");
        return false;
    }
    if (firma == NULL) {
        ALTAR_LOG_W("formula: firma NULL");
        return false;
    }
    if (tam_firma != HASHMAL_FORMULA_FIRMA_BYTES) {
        ALTAR_LOG_W("formula: tam_firma=%u ≠ %d (SHA-256 = 32 B)",
                    (unsigned)tam_firma, HASHMAL_FORMULA_FIRMA_BYTES);
        return false;
    }

    /* Calcular SHA-256(datos). nataf - los datos gotean al roqeaj. */
    uint8_t calculado[HASHMAL_FORMULA_FIRMA_BYTES];
    int rc = sha256_calcular(datos, tam, calculado);
    if (rc != 0) {
        ALTAR_LOG_E("formula: sha256 falló (mbedtls=%d)", rc);
        /* Limpieza defensiva antes de salir. */
        memset(calculado, 0, sizeof(calculado));
        return false;
    }

    /* shajaqta hadeq - comparacion constant-time, byte a byte. */
    const bool coincide = comparar_constante(calculado, firma,
                                             HASHMAL_FORMULA_FIRMA_BYTES);

    /* Limpiar el hash calculado: no dejar rastros de material
     * sensible en pila. */
    memset(calculado, 0, sizeof(calculado));

    if (coincide) {
        ALTAR_LOG_I("formula: firma VERIFICA (Éx 30:35 maʻasé roqéaj)");
    } else {
        /* No es ERROR - el rechazo por firma incorrecta es del
         * firewall (altar_oro_ascendente.c), que logea alli con
         * ERR_ZARA. Aqui solo informamos INFO del resultado. */
        ALTAR_LOG_I("formula: firma NO coincide — zará para el filtro ascendente");
    }
    return coincide;
}

/* ==================================================================
 * API PUBLICA - formula_estado
 * ================================================================== */

/**
 * hashmal_intercession_formula_estado - ?listos para verificar?
 *
 * Reporta si el subsistema de formula esta operativo:
 *   - HASHMAL_INTERCESSION_OK       si el altar esta levantado
 *                                    (mbedtls es parte del runtime;
 *                                     si estamos aqui, esta cargado)
 *   - HASHMAL_INTERCESSION_ERR_INIT si el altar no esta levantado
 *
 * No reporta si HAY firma registrada (eso lo custodia
 * altar_oro_ascendente.c). Reporta la DISPONIBILIDAD del mecanismo
 * de verificacion - herramienta lista.
 *
 * 2 testigos:
 *   Ex 30:36 - qodesh qodashim (el estado es sagrado)
 *   Ex 30:34 - bad bebad yihye (la proporcion SERA: mecanismo vigente)
 */
int hashmal_intercession_formula_estado(void)
{
    const hashmal_altar_t *a = hashmal_intercession_get_altar_interno();
    if (a == NULL) {
        ALTAR_LOG_E("formula_estado: altar no levantado");
        return HASHMAL_INTERCESSION_ERR_INIT;
    }
    return HASHMAL_INTERCESSION_OK;
}

/* ==================================================================
 *
 *   "nataf ushejelet vejelbena sammim ulevona zakka - BAD BEBAD
 *    yihye" - Exodo 30:34
 *
 *   "ve'asita otah qetoret roqaj ma'ase roqeaj memullaj tahor
 *    qodesh" - Exodo 30:35
 *
 *   "vehaqqetoret asher ta'ase bematjuntatah LO TA'ASU LAJEM...
 *    ish asher ya'ase jamoha lehariaj bah VENIKRAT me'ammav."
 *     - Exodo 30:37-38
 *
 *   4 especias. Partes iguales. Molidas fino. Proporcion unica.
 *   Quien replique - nikrat.
 *
 * ================================================================== */
