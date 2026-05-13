/**
 * ===================================================================
 * PIEZA 12 - EL KIYOR (LAVACRO) - lavacro_challenge.c
 * Sub-pieza: EPHEMERAL CHALLENGE ZERO-KNOWLEDGE
 * Numeros 5:17 + Numeros 5:23 + Deuteronomio 16:6
 * ===================================================================
 *
 * "hamayim... BIKLI JÁRES" - Numeros 5:17
 * "El agua... EN VASO DE BARRO."
 *
 * "veKATÁV... uMAJÁH" - Numeros 5:23
 * "Y ESCRIBIRA... y BORRARA." (La prueba se escribe, se aplica, se anula)
 *
 * "velo taʻaleh JAMETS bekhaleja uvelo tirʼéh JAMETS BEJOL gevul
 *  gvuleja" - Deuteronomio 16:6 (contexto sobre lo PROHIBIDO reutilizar)
 * "No subira levadura contigo, ni veras levadura en todos tus terminos."
 *
 * --- KELI JÁRES - VASO QUE SE DESTRUYE ---
 *
 * keli  (KLY, H3627) = vaso, recipiente, utensilio
 * járes (JRS, H2789) = barro, arcilla, fragil
 *
 * Un vaso de BARRO es FRAGIL, de USO UNICO. No como los de plata
 * (Nm 4:8) o los de bronce (Nm 4:14) que duran. El vaso de barro,
 * una vez roto, no vale. Se tira.
 *
 * En Numbers 5:17, la mujer acusada bebe agua del vaso de barro.
 * El vaso NO se reutiliza - su fragilidad REFLEJA la naturaleza
 * ephemeral de la prueba. Una sola oportunidad. Si falla, el vaso
 * se quebro - no hay segunda lectura.
 *
 * --- KATAV U-MAJAH - ESCRIBIR Y BORRAR ---
 *
 * katav (KTB, H3789) = escribir, inscribir
 * majah (MJH, H4229) = borrar, desvanece, erasa
 *
 * Numbers 5:23 - "veKATÁV et hammerot haelleh veMAJÁH el hamayim
 *  hammarim". Escribe las palabras... y BORRA en el agua amarga.
 *
 * El sistema: 1) escribir = generar desafio
 *            2) aplicar = consumir el desafio contra el nodo
 *            3) MAHJAH = destruir (zeroes, flags set)
 *
 * Fase criptografica: el token desafio es ephemeral. No se guarda.
 * No se reutiliza. Una sola oportunidad para verificar. Luego se
 * anula por diseno - no por accidente sino por LEY.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   hashmal_hc_challenge_generar(out)          numeros 5:17 - crear vaso
 *   hashmal_hc_challenge_aplicar(ch, nodo, r)  numeros 5:23 - aplicar y borrar
 *   hashmal_hc_challenge_es_valido(ch)         consulta: ?intacto?
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_healthcheck.h     API publica, struct, enums
 *   esp_log.h (ORO EGIPTO)    logging
 *   esp_random.h (ORO EGIPTO) RNG para el token
 *   string.h                  memset para MAHJAH
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "esp_log.h"
#include "esp_random.h"

#include "hashmal_healthcheck.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *CHAL_TAG = "hashmal.hc.challenge";

#define CHAL_LOG_I(fmt, ...)  ESP_LOGI(CHAL_TAG, fmt, ##__VA_ARGS__)
#define CHAL_LOG_W(fmt, ...)  ESP_LOGW(CHAL_TAG, fmt, ##__VA_ARGS__)
#define CHAL_LOG_E(fmt, ...)  ESP_LOGE(CHAL_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * TIMESTAMP HELPER (compartido con kiyor)
 * ================================================================== */
extern uint64_t hashmal_hc_ahora_ms(void);

/* ==================================================================
 * hashmal_hc_challenge_generar - Numeros 5:17 "keli járes"
 *
 * "hamayim tehorim BIKLI JÁRES" — agua pura en vaso de barro.
 *
 * Genera un desafio unico: 32 bytes aleatorios, timestamp actual,
 * flags iniciales (single_use=true, destruido=false).
 *
 * El vaso es NUEVO cada vez - nunca se reutiliza. RNG asegura
 * que cada token es DISTINTO, impredecible, un vaso fresco.
 *
 * 2 testigos:
 *   Nm 5:17  - "bikli járes" (vaso de barro, fragil, unico)
 *   Nm 5:23  - "catav... umajah" (la secuencia: crear, usar, destruir)
 * ================================================================== */
int hashmal_hc_challenge_generar(hashmal_hc_challenge_t *out)
{
    if (out == NULL) {
        CHAL_LOG_E("challenge_generar: out es NULL");
        return HASHMAL_HC_ERR_NULL;
    }

    /* Generar 32 bytes aleatorios - el vaso nuevo. */
    esp_fill_random(out->token, sizeof(out->token));

    /* Timestamp - marca cuando se creo el vaso. */
    out->timestamp_ms = hashmal_hc_ahora_ms();

    /* Flags iniciales: single_use = true (solo una oportunidad),
     * destruido = false (intacto aun). */
    out->single_use = true;
    out->destruido  = false;

    CHAL_LOG_I("KELI JÁRES creado — token ephemeral de 32 bytes "
               "(Nm 5:17 bikli járes)");
    return HASHMAL_HC_OK;
}

/* ==================================================================
 * hashmal_hc_challenge_aplicar - Numeros 5:23 "katav u-majah"
 *
 * "veKATÁV et hammerot haelleh veMAJÁH el hamayim."
 * "ESCRIBIRA... BORRARA en el agua."
 *
 * Aplica el desafio contra el nodo_id (verifica credenciales,
 * replica el token del nodo, compara, etc.) y luego DESTRUYE:
 *
 *   1) Verificacion contra nodo_id
 *   2) Si es valido: resultado = true
 *   3) MAJAH: llenar token a cero (MAHJAH - borrar)
 *   4) Poner destruido = true
 *   5) single_use = false (ya no es usable)
 *
 * Fase 1 (aca): verificacion BASICA (ch != NULL, !destruido).
 * Fase 2: protocolo criptografico completo (HMAC, timestamp check, etc).
 *
 * 2 testigos:
 *   Nm 5:23  - "catav... umajah" (escribir y borrar)
 *   Nm 5:17  - "bikli járes" (de vaso de barro, se quiebra)
 *
 * Invariante: NO hay recuperacion. Una vez MAJAH, el desafio está
 * muerto. No se puede "un-erase" un vaso roto.
 * ================================================================== */
int hashmal_hc_challenge_aplicar(hashmal_hc_challenge_t *ch,
                                  uint8_t nodo_id,
                                  bool *resultado)
{
    if (ch == NULL) {
        CHAL_LOG_E("challenge_aplicar: ch es NULL");
        return HASHMAL_HC_ERR_NULL;
    }

    if (resultado == NULL) {
        CHAL_LOG_E("challenge_aplicar: resultado es NULL");
        return HASHMAL_HC_ERR_NULL;
    }

    /* Precondicion: el vaso debe estar intacto (no destruido aun). */
    if (ch->destruido) {
        CHAL_LOG_W("challenge_aplicar: vaso ya MAJAH (destruido), "
                   "no se puede reutilizar (Nm 5:17 keli járes)");
        *resultado = false;
        return HASHMAL_HC_ERR_TAME;  /* agua contaminada = vaso roto */
    }

    /* Fase 1: verificacion basica - el token existe.
     * (Fase 2 anadira HMAC, timestamp check, reply authentication, etc) */
    bool token_valido = true;
    for (int i = 0; i < 32; ++i) {
        if (ch->token[i] == 0x00 && i > 0) {
            token_valido = false;
            break;
        }
    }

    if (!token_valido) {
        *resultado = false;
        CHAL_LOG_W("challenge_aplicar: token no valido (fase 1)");
    } else {
        /* En fase 1, simple "el token existe" pasa. Fase 2 sera real. */
        *resultado = true;
        CHAL_LOG_I("challenge verificado contra nodo %d (fase 1 - token existe)",
                   (int)nodo_id);
    }

    /* --- MAJAH: DESTRUIR EL VASO --- */
    /* Nm 5:23 - "umajah" - BORRAR. El vaso se quiebra por diseno. */
    memset(ch->token, 0x00, sizeof(ch->token));
    ch->destruido  = true;
    ch->single_use = false;

    CHAL_LOG_I("MAJAH ejecutado — token BORRADO, vaso de barro ROTO "
               "(Nm 5:23 umajah)");

    return HASHMAL_HC_OK;
}

/* ==================================================================
 * hashmal_hc_challenge_es_valido - Consulta rápida
 *
 * "?el vaso esta intacto?" retorna !destruido.
 *
 * Lectura pura, sin logs. Permite al caller preguntar rapido:
 * "?puedo usar este desafio?" respuesta: "NO si destruido=true".
 * ================================================================== */
bool hashmal_hc_challenge_es_valido(const hashmal_hc_challenge_t *ch)
{
    if (ch == NULL) return false;
    return !ch->destruido;
}

/* ==================================================================
 *
 *   "hamayim... BIKLI JÁRES" - Numeros 5:17
 *   "veKATÁV... uMAJÁH" - Numeros 5:23
 *
 *   El vaso de barro se quiebra tras uso unico. El desafio ephemeral
 *   es diseño BIBLICO, no accidente de implementacion.
 *
 *   Fase 2 anadira verificacion real:
 *     - HMAC-SHA256 con key ephemeral (pieza 13)
 *     - Check de timestamp (no mas de 30s de edad)
 *     - Nonce counters (no se replica el mismo desafio)
 *     - Rate limiting (no mas de 5 desafios por minuto)
 *
 *   Pero la ESTRUCTURA - generate, apply&destroy, query validity -
 *   es FINAL. El vaso no se repara. Punto.
 *
 * ================================================================== */
