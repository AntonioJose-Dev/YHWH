/**
 * ===================================================================
 * PIEZA 06 - ALTAR DEL SACRIFICIO - altar_reshet.c
 * Sub-pieza: LA RESHET - la RED del Padre (MKBR RED NEJOSHET)
 * Exodo 27:4-5 + Levitico 22:20 + Exodo 29:37
 * ===================================================================
 *
 * "ve'asita lo MIJBAR ma'ase RESHET NEJOSHET ve'asita 'al
 *  HARRESHET arba' TABBE'OT nejoshet 'al arba' qetsotav" - Ex 27:4
 * "Haras para el una REJILLA, obra de RED de bronce; y haras sobre
 *  la RED cuatro ANILLOS de bronce en sus cuatro extremos."
 *
 * "venatatta otah tajat KARKOV hammizbeaj MILLEMATTA vehayeta
 *  harreshet 'ad JATSI hammizbeaj" - Ex 27:5
 * "La pondras debajo del BORDE del altar, por ABAJO, y la red
 *  llegara hasta la MITAD del altar."
 *
 * "kol asher bo MUM lo taqrivu ki LO YIHYE LERATSON lajem" - Lv 22:20
 * "Todo lo que tenga DEFECTO no lo ofrecereis - porque NO SERA
 *  ACEPTADO por vosotros."
 *
 * "kol hannoguea' bammizbeaj YIQDASH" - Ex 29:37
 * "Todo lo que TOQUE el altar sera SANTIFICADO."
 *
 * --- LA PALABRA DEL PADRE ---
 *
 *   reshet   (RShT, H7568)   RED. El Padre NOMBRO esta pieza como
 *                              red en el versiculo. No es metafora
 *                              nuestra: es el nombre celestial. La
 *                              autenticacion del Mishkan digital ES
 *                              la reshet del altar del sacrificio.
 *
 *   mijbar   (MKBR, H4345)  REJILLA / ENREJADO. Es el FILTRO
 *                              fisico. Deja pasar lo legitimo, retiene
 *                              lo que no corresponde. Challenge-response
 *                              es exactamente eso: filtro.
 *
 *   tabbe'ot (TB'T)         ANILLOS. 4 anillos en los 4 extremos
 *                              de la red - puntos de anclaje. No
 *                              concentrados; distribuidos al borde.
 *
 *   karkov   (KRKB, H3749) BORDE / cornisa. El limite superior
 *                              del altar, sobre el cual esta el fuego.
 *
 *   jatsi    (ChTsY, H2677)    MITAD. La reshet esta a MEDIA altura.
 *                              Ni abajo (tierra, request crudo) ni
 *                              arriba (fuego, servicio consagrado):
 *                              ENTRE. Es la interfaz por excelencia.
 *
 *   mum      (MWM, H3971)      DEFECTO. Lv 22:20 - lo defectuoso no
 *                              sube. La verificacion rechaza sin
 *                              matices.
 *
 *   yiqdash  (YQDSh)         SERA SANTIFICADO. Ex 29:37 - lo que
 *                              pasa la reshet queda "yiqdash":
 *                              autenticado, marcado como verificado.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   reshet_challenge    Ex 27:4 - el mijbar lanza la red (challenge)
 *   reshet_verificar    Ex 27:5 + Lv 22:20 - pasa la mitad o cae abajo
 *   reshet_estado       lectura del operativo
 *
 * --- NEVUV HEREDADO ---
 *
 * La reshet es parte del altar; hereda su propiedad NEVUV (hueco,
 * stateless). No guardamos estado local: los contadores viven en el
 * singleton del altar, y cada challenge/verificar es independiente.
 *
 * --- ORO DE EGIPTO ---
 *
 * mbedtls/sha256.h y esp_random.h se incluyen SOLO aqui. Como en
 * altar_oro_formula.c: cada modulo encapsula su puerta al oro de
 * Egipto. Si manana SHA-256 se sustituye por BLAKE3, o si el RNG
 * cambia, solo este archivo se edita.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_auth.h                 API publica
 *   hashmal_auth_internal.h        altar_mut, get_altar_interno, ahora_ms
 *   mbedtls/sha256.h (ORO EGIPTO)  SHA-256 del proof
 *   esp_random.h     (ORO EGIPTO)  TRNG del ESP32-S3
 *   esp_log.h        (ORO EGIPTO)  AUTH_LOG_*
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
#include "esp_random.h"
#include "esp_log.h"

#include "hashmal_auth.h"
#include "hashmal_auth_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *AUTH_TAG = "hashmal.auth.reshet";

#define AUTH_LOG_I(fmt, ...)  ESP_LOGI(AUTH_TAG, fmt, ##__VA_ARGS__)
#define AUTH_LOG_W(fmt, ...)  ESP_LOGW(AUTH_TAG, fmt, ##__VA_ARGS__)
#define AUTH_LOG_E(fmt, ...)  ESP_LOGE(AUTH_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * HELPERS ESTATICOS - mbedtls + esp_random encapsulados
 * ================================================================== */

/**
 * SHA-256 one-shot. Misma implementacion que altar_oro_formula.c
 * (por intencion: cada modulo se auto-contiene). mbedtls se
 * inicializa, se alimenta el dato, se finaliza, se libera.
 *
 * @param datos entrada (puede ser NULL si tam == 0)
 * @param tam   bytes de entrada
 * @param out   salida fija de HASHMAL_AUTH_PROOF_BYTES (32)
 * @return 0 en exito, codigo mbedtls negado en fallo.
 */
/* shittím — acacia interior, no expuesta (Éx 27:2) */
static int sha256_calcular(const uint8_t *datos, size_t tam,
                           uint8_t out[HASHMAL_AUTH_PROOF_BYTES])
{
    if (out == NULL) return -1;
    if (datos == NULL && tam > 0u) return -1;

    mbedtls_sha256_context ctx;
    mbedtls_sha256_init(&ctx);

    int rc = mbedtls_sha256_starts(&ctx, 0);   /* 0 = SHA-256 */
    if (rc == 0 && tam > 0u) rc = mbedtls_sha256_update(&ctx, datos, tam);
    if (rc == 0)             rc = mbedtls_sha256_finish(&ctx, out);

    mbedtls_sha256_free(&ctx);
    return rc;
}

/**
 * Comparacion byte a byte en TIEMPO CONSTANTE. Ex 30:36 "shajaqta
 * hadeq" (moler fino) aplica tambien aqui: cada byte examinado,
 * ninguno saltado. Sin early-out -> no leak por timing.
 *
 * Mismo patron que altar_oro_formula.c::comparar_constante. NO
 * usar memcmp (sale al primer byte distinto = timing leak).
 */
/* shittím — acacia interior, no expuesta (Éx 27:2) */
static bool comparar_constante(const uint8_t *a, const uint8_t *b, size_t n)
{
    if (a == NULL || b == NULL) return false;

    uint8_t diff = 0u;
    for (size_t i = 0; i < n; ++i) {
        diff |= (uint8_t)(a[i] ^ b[i]);
    }
    return diff == 0u;
}

/**
 * Llena un buffer con bytes aleatorios del TRNG del ESP32-S3.
 * Encapsula esp_fill_random - oro de Egipto del generador de
 * hardware. En Fase 2 podria sustituirse por un KDF determinista
 * derivado del estado de la red (v.4 "ma'ase reshet" - obra
 * artesanal, no casual).
 */
/* shittím — acacia interior, no expuesta (Éx 27:2) */
static void random_llenar(uint8_t *buf, size_t len)
{
    if (buf == NULL || len == 0u) return;
    esp_fill_random(buf, len);
}

/* ==================================================================
 * API PUBLICA - reshet_challenge (Ex 27:4 - el mijbar lanza la red)
 * ================================================================== */

/**
 * hashmal_auth_reshet_challenge - genera un desafio criptografico
 *
 * Ex 27:4 - "ma'ase RESHET NEJOSHET". La reshet se arroja, y quien
 * quiera acercarse al fuego debe resolver la prueba. En firmware:
 * TRNG del ESP32-S3 produce 64 bytes (HASHMAL_AUTH_CHALLENGE_MAX)
 * que el solicitante debera usar para construir su proof.
 *
 * --- PRECONDICIONES ---
 *
 *   altar levantado                     (estructura existe)
 *   reshet_activa                        (fuego_encender ya corrio)
 *   challenge_out, tam_out no NULL
 *
 * --- EFECTOS ---
 *
 *   challenge_out[0..CHALLENGE_MAX-1] = bytes aleatorios TRNG
 *   *tam_out                          = HASHMAL_AUTH_CHALLENGE_MAX
 *   altar->challenges_emitidos       += 1
 *   altar->ultimo_challenge_ms        = ahora_ms()
 *
 * Nota: se asume que challenge_out tiene capacidad >= CHALLENGE_MAX.
 * El header declara el tamano como contrato; callers sin buffer
 * suficiente es bug del llamante.
 *
 * 2 testigos:
 *   Ex 27:4  - ma'ase reshet nejoshet (obra de red)
 *   Ex 31:3  - ruaj Elohim bejojma... (diseno artesanal)
 */
/* nejóshet — interfaz bronce, trust Atrio (Éx 27:2) */
int hashmal_auth_reshet_challenge(uint8_t *challenge_out, size_t *tam_out)
{
    if (challenge_out == NULL || tam_out == NULL) {
        AUTH_LOG_E("challenge: parámetro NULL");
        return HASHMAL_AUTH_ERR_NULL;
    }

    hashmal_auth_t *a = hashmal_auth_altar_mut();
    if (a == NULL) {
        AUTH_LOG_E("challenge: altar no levantado");
        return HASHMAL_AUTH_ERR_INIT;
    }

    if (!a->reshet_activa) {
        AUTH_LOG_E("challenge: réshet no activa — fuego_encender pendiente "
                   "(Lv 6:5 uviʻér ʻaléha ʻetsím)");
        return HASHMAL_AUTH_ERR_INIT;
    }

    /* Generar los bytes aleatorios. El caller debe asegurar que
     * challenge_out tiene capacidad >= CHALLENGE_MAX. */
    random_llenar(challenge_out, (size_t)HASHMAL_AUTH_CHALLENGE_MAX);
    *tam_out = (size_t)HASHMAL_AUTH_CHALLENGE_MAX;

    /* Registro en el singleton. */
    const uint64_t ahora        = hashmal_auth_ahora_ms();
    a->challenges_emitidos     += 1ull;
    a->ultimo_challenge_ms      = ahora;

    /* P9 — Lv 6:3-4 — deshen: cada operación deja residuo. */
    hashmal_auth_deshen_append(HASHMAL_AUTH_EVT_CHALLENGE);

    AUTH_LOG_I("challenge emitido: %d bytes (Éx 27:4 maʻasé réshet); "
               "total=%" PRIu64,
               HASHMAL_AUTH_CHALLENGE_MAX, a->challenges_emitidos);
    return HASHMAL_AUTH_OK;
}

/* ==================================================================
 * API PUBLICA - reshet_verificar (Ex 27:5 + Lv 22:20 + Ex 29:37)
 * ================================================================== */

/**
 * hashmal_auth_reshet_verificar - pasa la mitad o cae abajo
 *
 * Ex 27:5 - "'ad JATSI hammizbeaj": la reshet llega a la mitad. Lo
 * que pasa la prueba sube al fuego (santificado, Ex 29:37 yiqdash).
 * Lo que no pasa queda ABAJO - Lv 22:20 "lo yihye leratson" (no
 * aceptado).
 *
 * --- PROTOCOLO ---
 *
 *   hash_calculado = SHA-256(datos)
 *   coincide       = comparar_constante(hash_calculado, proof, 32)
 *   si coincide:  pruebas_validadas++, retornar OK
 *   si no:        pruebas_rechazadas++, retornar ERR_PROOF
 *
 * Sin matices: binario. Ex 29:37 yiqdash o Lv 22:20 rechazado.
 *
 * --- PRECONDICIONES ---
 *
 *   altar levantado
 *   reshet_activa
 *   datos no NULL, tam > 0
 *   proof no NULL, proof_tam == HASHMAL_AUTH_PROOF_BYTES (32)
 *
 * --- LIMPIEZA DEFENSIVA ---
 *
 * memset(hash_calculado, 0) antes de retornar - no dejar material
 * sensible en pila. Mismo patron que altar_oro_formula.c.
 *
 * --- NIVEL DE LOG ---
 *
 * LOG_I para AMBOS resultados. El rechazo NO es emergencia -
 * es filtro normal. Muchas ofrendas con mum se rechazaban en el
 * templo; era operacion rutinaria del sacerdote. Solo si algo
 * rompe el flujo (mbedtls falla) seria LOG_E.
 *
 * 2 testigos:
 *   Lv 22:20  - mum -> lo yihye leratson (defecto = rechazo)
 *   Ex 29:37  - yiqdash (lo que toca el altar es consagrado)
 */
/* nejóshet — interfaz bronce, trust Atrio (Éx 27:2) */
int hashmal_auth_reshet_verificar(const uint8_t *datos, size_t tam,
                                  const uint8_t *proof, size_t proof_tam)
{
    hashmal_auth_t *a = hashmal_auth_altar_mut();
    if (a == NULL) {
        AUTH_LOG_E("verificar: altar no levantado");
        return HASHMAL_AUTH_ERR_INIT;
    }

    if (!a->reshet_activa) {
        AUTH_LOG_E("verificar: réshet no activa — fuego_encender pendiente");
        return HASHMAL_AUTH_ERR_INIT;
    }

    if (datos == NULL || tam == 0u) {
        AUTH_LOG_E("verificar: datos NULL o vacío (nada que ofrecer)");
        return HASHMAL_AUTH_ERR_NULL;
    }

    if (proof == NULL) {
        AUTH_LOG_E("verificar: proof NULL");
        return HASHMAL_AUTH_ERR_NULL;
    }

    if (proof_tam != (size_t)HASHMAL_AUTH_PROOF_BYTES) {
        AUTH_LOG_W("verificar: proof_tam=%u ≠ %d (SHA-256) — Lv 22:20 mum",
                   (unsigned)proof_tam, HASHMAL_AUTH_PROOF_BYTES);
        /* Tamano incorrecto = proof defectuoso = rechazo. Contamos
         * como rechazo (mum) para que las estadisticas reflejen la
         * realidad: se presento una ofrenda, se rechazo. */
        a->pruebas_rechazadas += 1ull;
        /* P9 — Lv 6:3-4 — deshen del rechazo por tamaño. */
        hashmal_auth_deshen_append(HASHMAL_AUTH_EVT_RECHAZADO);
        return HASHMAL_AUTH_ERR_PROOF;
    }

    /* Calcular SHA-256(datos). */
    uint8_t calculado[HASHMAL_AUTH_PROOF_BYTES];
    int rc = sha256_calcular(datos, tam, calculado);
    if (rc != 0) {
        AUTH_LOG_E("verificar: sha256 falló (mbedtls=%d)", rc);
        /* Error de infraestructura, NO rechazo de la ofrenda; no
         * contamos como rechazo ni como aceptacion. Limpieza de
         * hash antes de salir. */
        memset(calculado, 0, sizeof(calculado));
        return HASHMAL_AUTH_ERR_PROOF;
    }

    /* Comparacion constant-time. shajaqta hadeq - cada byte molido. */
    const bool coincide = comparar_constante(calculado, proof,
                                             HASHMAL_AUTH_PROOF_BYTES);

    /* Limpieza defensiva: no dejar el hash calculado en pila. */
    memset(calculado, 0, sizeof(calculado));

    if (coincide) {
        /* Ex 29:37 - "kol hannoguea' bammizbeaj YIQDASH": toco el
         * altar, fue consagrado. La prueba sube al fuego. */
        a->pruebas_validadas += 1ull;
        /* P9 — Lv 6:3-4 — deshen del sacrificio aceptado. */
        hashmal_auth_deshen_append(HASHMAL_AUTH_EVT_VALIDADO);
        AUTH_LOG_I("verificar: OK — yiqdásh (Éx 29:37); total=%" PRIu64,
                   a->pruebas_validadas);

        /* Lv 16:14 — "velaqaj middam happar vehizzá":
         * el proof verificado ES la sangre (dam). Lo entregamos al
         * conductor de sangre registrado (hashmal_main.c como Aaron
         * Kohen Gadol). El conducto lleva el dam al Kapóret para
         * rociar SOBRE (1×) y DELANTE (7× shéva peʻamím).
         *
         * NOTA: proof ES el hash SHA-256 que acaba de pasar
         * comparar_constante — tiene exactamente HASHMAL_AUTH_PROOF_BYTES
         * bytes. Pasamos ahora_ms como timestamp del sacrificio.
         *
         * 2 testigos:
         *   Lv 16:14 — "min haddam be'etsba'o" — la sangre del toro
         *   Heb 9:7  — "ou choris haimatos" — no sin sangre */
        hashmal_auth_dam_llamar(proof, hashmal_auth_ahora_ms());

        return HASHMAL_AUTH_OK;
    }

    /* Lv 22:20 - mum detectado. La ofrenda no sube. Quien la trajo
     * lo sabe: el rechazo es explicito, sin ambiguedad. */
    a->pruebas_rechazadas += 1ull;
    /* P9 — Lv 6:3-4 — deshen del rechazo (también es residuo). */
    hashmal_auth_deshen_append(HASHMAL_AUTH_EVT_RECHAZADO);
    AUTH_LOG_I("verificar: proof NO coincide — lo yihyé leratsón "
               "(Lv 22:20); rechazadas=%" PRIu64,
               a->pruebas_rechazadas);
    return HASHMAL_AUTH_ERR_PROOF;
}

/* ==================================================================
 * API PUBLICA - reshet_estado
 * ================================================================== */

/**
 * hashmal_auth_reshet_estado - ?la red esta lanzada y operativa?
 *
 * Dos condiciones:
 *   altar levantado      -> si no: ERR_INIT
 *   reshet_activa        -> si no: ERR_INIT (el fuego no prendio)
 *
 * Lectura pura. Sin side-effects. Sin logs en lectura normal.
 */
int hashmal_auth_reshet_estado(void)
{
    const hashmal_auth_t *a = hashmal_auth_get_altar_interno();
    if (a == NULL) {
        return HASHMAL_AUTH_ERR_INIT;
    }
    if (!a->reshet_activa) {
        return HASHMAL_AUTH_ERR_INIT;
    }
    return HASHMAL_AUTH_OK;
}

/* ==================================================================
 *
 *   "ve'asita lo MIJBAR ma'ase RESHET NEJOSHET... arba' TABBE'OT
 *    nejoshet 'al arba' qetsotav." - Exodo 27:4
 *
 *   "vehayeta harreshet 'ad JATSI hammizbeaj." - Exodo 27:5
 *
 *   "kol hannoguea' bammizbeaj YIQDASH." - Exodo 29:37
 *
 *   "kol asher bo MUM lo taqrivu ki LO YIHYE LERATSON." - Levitico 22:20
 *
 *   ma'ase RESHET NEJOSHET - obra de RED de bronce.
 *   La reshet del Padre filtra: lo valido sube al fuego,
 *   lo defectuoso queda abajo. jatsi hammizbeaj - la mitad.
 *
 *   2 de 6 archivos del Altar del Sacrificio -
 *   altar_reshet.c listo. Quedan:
 *     altar_cuernos.c      v.2   - 4 esquinas + 1 Re 1:50 refugio
 *     altar_fuego.c        Lv 6:5-6 - esh tamid, encender y ciclar
 *     altar_utensilios.c   v.3   - los 5 kelim
 *     altar_ledger.c       Ap 6:9 - almas bajo el altar
 *
 * ================================================================== */
