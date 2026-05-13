/**
 * ===================================================================
 * PIEZA 12 - EL KIYOR (LAVACRO) - lavacro_niveles.c
 * Sub-pieza: VELO YAMUTU - niveles de deteccion (tahor -> mavet)
 * Exodo 30:20-21 + Levitico 13 + Levitico 14:7 + Numeros 16:48
 * ===================================================================
 *
 * "bevoam el ohel mo'ed yirjatsu mayim VELO YAMUTU o begishtam el
 *  hammizbeaj lesharet lehaqtir ishe laYHWH" - Exodo 30:20
 * "Al entrar a la Tienda se lavaran con agua Y NO MORIRAN, o al
 *  acercarse al altar para SERVIR, para quemar ofrenda a YHWH."
 *
 * "verajatsu yadeihem veragleihem VELO YAMUTU vehayeta lahem
 *  JUQQAT 'OLAM lo ulezar'o ledorotam" - Exodo 30:21
 * "Lavaran manos y pies Y NO MORIRAN; sera ESTATUTO PERPETUO para
 *  el y para su simiente por sus generaciones."
 *
 * --- DOBLE TESTIGO DE MUERTE ---
 *
 * "yamutu" (YMTW, H4191 mut - morir, Qal imperfecto 3pl) aparece
 * DOS VECES, una en v.20 y otra en v.21, versiculos CONSECUTIVOS.
 *
 * Dt 19:15 - "al pi shenei 'edim yaqum davar" (por boca de DOS
 * testigos se establece el asunto). El Padre aplica Su propia regla
 * aqui: dos testigos contiguos dicen "muerte si no te lavas". Es el
 * UNICO caso del Mishkan donde la penalizacion por omision se
 * repite en dos versos seguidos. Enfasis maximo.
 *
 * --- 4 NIVELES - escala de severidad ---
 *
 *   TAHOR  (H2889)  PURO         Lv 13:13 "vetiher" - limpio
 *   TAME   (H2930)  IMPURO       Lv 13:3 "vetime'" - declarado impuro
 *   JATA   (H2398)  PECADO       Lv 5:1 "ki tejeta" - transgresion
 *   MAVET  (H4194)  MUERTE       Ex 30:20-21 "yamutu" - muerte
 *
 * Los niveles son ENUMERADOS (0..3) para que la comparacion de
 * severidad sea directa: mas alto = mas severo.
 *
 * --- GRADUAL (bayom hashevi'i, Lv 13:5) ---
 *
 * El sacerdote NO declara muerte instantanea. En Lv 13:4-6 examina
 * la llaga el dia 1, espera al dia 7, vuelve a examinar. Proceso
 * OBSERVACIONAL. Aqui: cada llamada a nivel_subir/nivel_reducir
 * mueve UN PASO. Gradual. El sistema observa, espera, re-examina.
 *
 * --- NIVEL GLOBAL = MAX (peor domina) ---
 *
 * Lv 13: si UNA llaga es impura, TODA la persona es impura - la
 * contaminacion no se "promedia". El peor punto DOMINA. Asi,
 * nivel_global = MAX(yadayim, raglayim, levav, mayim). Si UN solo
 * vector alcanza MAVET -> todo el nodo es MAVET -> el kiyor entero
 * se CONTAMINA (el agua deja de servir hasta que el vector
 * afectado se recupere).
 *
 * --- muertes_prevenidas - el contador sagrado ---
 *
 * Cada vez que un vector SALE de MAVET (nivel_reducir desde MAVET
 * a JATA), incrementamos `muertes_prevenidas`. Es la encarnacion
 * literal de "velo yamutu": el lavamiento PREVINO la muerte.
 *
 * Nm 16:48 - "vayya'amod BEIN hammetim UVEIN hajjayyim vatte'atsar
 *  hammaggefa": Aharon se puso ENTRE los muertos y los vivos, y
 * la mortandad CESO. Cada incremento de muertes_prevenidas es un
 * eco de ese acto: el sistema se paro entre el nodo muerto y el
 * nodo vivo, y la plaga se detuvo.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   nivel_subir(vec)               Lv 13:3  - declarar impuro, gradual
 *   nivel_reducir(vec)             Lv 14:7  - purificar, gradual
 *   nivel_consultar(vec)           lectura per-vector (ante duda MAVET)
 *   nivel_global_consultar()       lectura MAX de los 4
 *   niveles_estado()               lectura pura del subsistema
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_healthcheck.h     API publica
 *   hashmal_hc_internal.h     lavacro_mut, get_lavacro_interno,
 *                              ahora_ms, vector_legitimo, nivel_legitimo
 *   esp_log.h (ORO EGIPTO)    NIV_LOG_*
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <inttypes.h>

#include "esp_log.h"

#include "hashmal_healthcheck.h"
#include "hashmal_hc_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *NIV_TAG = "hashmal.hc.niveles";

#define NIV_LOG_I(fmt, ...)  ESP_LOGI(NIV_TAG, fmt, ##__VA_ARGS__)
#define NIV_LOG_W(fmt, ...)  ESP_LOGW(NIV_TAG, fmt, ##__VA_ARGS__)
#define NIV_LOG_E(fmt, ...)  ESP_LOGE(NIV_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * HELPERS ESTATICOS
 * ================================================================== */

/**
 * Nombre hebreo del vector, para logs.
 */
static const char *nombre_vector(hashmal_hc_vector_t v)
{
    switch (v) {
    case HASHMAL_HC_VEC_YADAYIM:   return "yadáyim";
    case HASHMAL_HC_VEC_RAGLAYIM:  return "ragláyim";
    case HASHMAL_HC_VEC_LEVAV:     return "levav";
    case HASHMAL_HC_VEC_MAYIM:     return "máyim";
    default:                       return "<desconocido>";
    }
}

/**
 * Etiqueta del nivel, para logs.
 */
static const char *nombre_nivel(hashmal_hc_nivel_t n)
{
    switch (n) {
    case HASHMAL_HC_TAHOR: return "TAHÓR";
    case HASHMAL_HC_TAME:  return "TAMÉ";
    case HASHMAL_HC_JATA:  return "JATÁ";
    case HASHMAL_HC_MAVET: return "MÁVET";
    default:               return "<desconocido>";
    }
}

/**
 * Calcula el nivel global = MAX de los 4 vectores.
 * Lv 13: una sola llaga impura contamina la persona entera.
 */
static hashmal_hc_nivel_t calcular_nivel_global(const hashmal_hc_t *l)
{
    hashmal_hc_nivel_t max = HASHMAL_HC_TAHOR;
    for (uint8_t i = 0; i < HASHMAL_HC_VECTORES; ++i) {
        if ((int)l->vectores[i].nivel > (int)max) {
            max = l->vectores[i].nivel;
        }
    }
    return max;
}

/* ==================================================================
 * API PUBLICA - nivel_subir (Lv 13:3 vetime')
 * ================================================================== */

/**
 * hashmal_hc_nivel_subir - declarar impureza ASCENDENTE (gradual)
 *
 * Sube UN escalon la severidad del vector indicado. El sacerdote
 * NO declara muerte de golpe (Lv 13:4-6 - observa 7 dias): avanza
 * un paso por llamada.
 *
 *   TAHOR -> TAME          Lv 13:3 "vetime'" - declara tame
 *   TAME  -> JATA          Lv 5:1 "ki tejeta" - transgresion
 *   JATA  -> MAVET         Ex 30:20-21 "yamutu" - muerte
 *   MAVET -> MAVET         ya maximo (idempotente)
 *
 * Si al subir el nivel_global alcanza MAVET, el kiyor se CONTAMINA:
 * el agua deja de servir (estado -> CONTAMINADO) hasta que el
 * vector afectado se reduzca.
 *
 * 2 testigos:
 *   Lv 13:3     - vetime' oto hakkohen (el sacerdote declara)
 *   Ex 30:20-21 - yamutu x 2 (doble testigo de muerte)
 */
int hashmal_hc_nivel_subir(hashmal_hc_vector_t vec)
{
    hashmal_hc_t *l = hashmal_hc_lavacro_mut();
    if (l == NULL) {
        NIV_LOG_E("nivel_subir: kiyór no levantado");
        return HASHMAL_HC_ERR_INIT;
    }
    if (!hashmal_hc_vector_legitimo(vec)) {
        NIV_LOG_E("nivel_subir: vec %d fuera de [0, %d)",
                  (int)vec, HASHMAL_HC_VECTORES);
        return HASHMAL_HC_ERR_VECTOR;
    }
    if (l->estado == HASHMAL_HC_SECO) {
        NIV_LOG_E("nivel_subir: estado SECO — sin agua no hay juicio "
                  "(Éx 30:18 máyim ausente)");
        return HASHMAL_HC_ERR_SECO;
    }

    hashmal_hc_vector_estado_t *v = &l->vectores[vec];

    /* Idempotencia en tope. */
    if (v->nivel == HASHMAL_HC_MAVET) {
        NIV_LOG_W("nivel_subir: %s ya en MÁVET (idempotente, no sube más)",
                  nombre_vector(vec));
        return HASHMAL_HC_OK;
    }

    /* Subir UN escalon (gradual - Lv 13:5 bayom hashevi'i). */
    const hashmal_hc_nivel_t nuevo = (hashmal_hc_nivel_t)((int)v->nivel + 1);
    v->nivel           = nuevo;
    v->ultimo_check_ms = hashmal_hc_ahora_ms();

    /* Recalcular global (MAX). */
    l->nivel_global = calcular_nivel_global(l);

    /* Si el global cayo a MAVET, el kiyor se contamina. */
    if (l->nivel_global == HASHMAL_HC_MAVET &&
        l->estado      != HASHMAL_HC_CONTAMINADO) {
        l->estado = HASHMAL_HC_CONTAMINADO;
        NIV_LOG_E("═══ VELÓ YAMÚTU — Éx 30:20-21 ═══ doble testigo: "
                  "%s alcanzó MÁVET → estado CONTAMINADO",
                  nombre_vector(vec));
    }

    /* Log por el nuevo nivel — nombre_nivel traduce el enum. */
    if (nuevo == HASHMAL_HC_MAVET) {
        NIV_LOG_E("yamútu (Éx 30:20-21) -- vector %s -> %s",
                  nombre_vector(vec), nombre_nivel(nuevo));
    } else {
        NIV_LOG_W("nivel_subir -- vector %s -> %s",
                  nombre_vector(vec), nombre_nivel(nuevo));
    }
    return HASHMAL_HC_OK;
}

/* ==================================================================
 * API PUBLICA - nivel_reducir (Lv 14:7 vetiher)
 * ================================================================== */

/**
 * hashmal_hc_nivel_reducir - purificacion DESCENDENTE (gradual)
 *
 * Baja UN escalon la severidad del vector indicado. Lv 14:7 -
 * "vetiher et hattame" (purificara al impuro). Gradual, como el
 * subir.
 *
 *   MAVET -> JATA          salida de muerte -> velo yamutu cumplido
 *   JATA  -> TAME          descenso de transgresion
 *   TAME  -> TAHOR         Lv 14:7 "vetiher" - purificado
 *   TAHOR -> TAHOR         ya minimo (idempotente)
 *
 * --- CASO ESPECIAL: SALIDA DE MAVET ---
 *
 * Ex 30:20 - "velo yamutu". Si un vector sale de MAVET, la muerte
 * FUE prevenida: muertes_prevenidas++. Nm 16:48 - "vayya'amod bein
 * hammetim uvein hajjayyim": Aharon entre muertos y vivos, plaga
 * detenida. El contador es eco de ese acto.
 *
 * Si ademas el estado era CONTAMINADO y ya ningun vector esta en
 * MAVET (global < MAVET), el agua vuelve a servir: estado ->
 * PURIFICANDO.
 *
 * 2 testigos:
 *   Lv 14:7    - vetiher et hattame (purificacion)
 *   Nm 16:48   - vayya'amod bein hammetim (entre muertos y vivos)
 */
int hashmal_hc_nivel_reducir(hashmal_hc_vector_t vec)
{
    hashmal_hc_t *l = hashmal_hc_lavacro_mut();
    if (l == NULL) {
        NIV_LOG_E("nivel_reducir: kiyór no levantado");
        return HASHMAL_HC_ERR_INIT;
    }
    if (!hashmal_hc_vector_legitimo(vec)) {
        NIV_LOG_E("nivel_reducir: vec %d fuera de [0, %d)",
                  (int)vec, HASHMAL_HC_VECTORES);
        return HASHMAL_HC_ERR_VECTOR;
    }
    if (l->estado == HASHMAL_HC_SECO) {
        NIV_LOG_E("nivel_reducir: estado SECO — sin agua no hay "
                  "purificación (Ez 36:25 máyim pendiente)");
        return HASHMAL_HC_ERR_SECO;
    }

    hashmal_hc_vector_estado_t *v = &l->vectores[vec];

    /* Idempotencia en suelo. */
    if (v->nivel == HASHMAL_HC_TAHOR) {
        NIV_LOG_W("nivel_reducir: %s ya TAHÓR (idempotente)",
                  nombre_vector(vec));
        return HASHMAL_HC_OK;
    }

    /* Bajar UN escalon. */
    const hashmal_hc_nivel_t prev  = v->nivel;
    const hashmal_hc_nivel_t nuevo = (hashmal_hc_nivel_t)((int)v->nivel - 1);
    v->nivel           = nuevo;
    v->ultimo_check_ms = hashmal_hc_ahora_ms();

    /* Caso especial: salir de MAVET - muerte PREVENIDA. */
    const bool salio_de_mavet = (prev == HASHMAL_HC_MAVET);
    if (salio_de_mavet) {
        l->muertes_prevenidas += 1ull;
        NIV_LOG_I("═══ MUERTE PREVENIDA ═══ (Éx 30:20 veló yamútu) — "
                  "vector %s: MÁVET → JATÁ; muertes_prevenidas=%" PRIu64,
                  nombre_vector(vec), l->muertes_prevenidas);
        NIV_LOG_I("  Nm 16:48 — vayyaʻamód béin hammetím uvéin hajjayyím");
    }

    /* Recalcular global. */
    l->nivel_global = calcular_nivel_global(l);

    /* Restaurar agua si CONTAMINADO y ya no hay MAVET en ningun vector. */
    if (l->estado       == HASHMAL_HC_CONTAMINADO &&
        l->nivel_global != HASHMAL_HC_MAVET) {
        l->estado = HASHMAL_HC_PURIFICANDO;
        NIV_LOG_I("agua restaurada — CONTAMINADO → PURIFICANDO "
                  "(Ez 36:25 máyim tehorim)");
    }

    /* Log por el nuevo nivel. */
    switch (nuevo) {
    case HASHMAL_HC_TAHOR:
        NIV_LOG_I("vetihér (Lv 14:7) — vector %s → TAHÓR",
                  nombre_vector(vec));
        break;
    case HASHMAL_HC_TAME:
        NIV_LOG_I("nivel reducido — vector %s → TAMÉ",
                  nombre_vector(vec));
        break;
    case HASHMAL_HC_JATA:
        NIV_LOG_I("nivel reducido — vector %s → JATÁ",
                  nombre_vector(vec));
        break;
    default:
        break;
    }
    return HASHMAL_HC_OK;
}

/* ==================================================================
 * API PUBLICA - nivel_consultar (lectura pura per-vector)
 * ================================================================== */

/**
 * hashmal_hc_nivel_consultar - ?que nivel tiene este vector?
 *
 * Lectura pura, sin logs. Ante duda (kiyor no levantado o vec
 * invalido) retorna MAVET - principio de muerte preventiva: si no
 * se puede verificar, tratar como maxima restriccion hasta aclarar.
 *
 * 2 testigos:
 *   Lv 13:3    - "vetime' oto hakkohen" (el sacerdote declara)
 *   Sal 24:4   - "neqi jappayim uvar levav" (lectura del estado)
 */
hashmal_hc_nivel_t hashmal_hc_nivel_consultar(hashmal_hc_vector_t vec)
{
    const hashmal_hc_t *l = hashmal_hc_get_lavacro_interno();
    if (l == NULL) return HASHMAL_HC_MAVET;
    if (!hashmal_hc_vector_legitimo(vec)) return HASHMAL_HC_MAVET;
    return l->vectores[vec].nivel;
}

/* ==================================================================
 * API PUBLICA - nivel_global_consultar (MAX de los 4)
 * ================================================================== */

/**
 * hashmal_hc_nivel_global_consultar - el nivel del nodo entero
 *
 * Lv 13: si UNA llaga es impura, TODA la persona es impura. Aqui
 * el nivel global es el MAX de los 4 vectores - ya precalculado
 * por nivel_subir / nivel_reducir.
 *
 * Lectura pura, sin logs. Ante duda -> MAVET.
 *
 * 2 testigos:
 *   Lv 13:46  - "mi-juts lammajane" (el impuro queda fuera -
 *               el todo se contamina)
 *   Ex 30:20  - velo yamutu (la muerte del todo, no de la parte)
 */
hashmal_hc_nivel_t hashmal_hc_nivel_global_consultar(void)
{
    const hashmal_hc_t *l = hashmal_hc_get_lavacro_interno();
    if (l == NULL) return HASHMAL_HC_MAVET;
    return l->nivel_global;
}

/* ==================================================================
 * API PUBLICA - niveles_estado (lectura pura del subsistema)
 * ================================================================== */

/**
 * hashmal_hc_niveles_estado - ?el sistema de niveles opera?
 *
 * Patron consistente con vectores_estado, fuego_estado, etc.
 * Lectura pura sin logs.
 *
 *   OK         si estado in { LLENO, PURIFICANDO }
 *   ERR_SECO   si estado == SECO
 *   ERR_TAME   si estado == CONTAMINADO
 *   ERR_INIT   si no levantado
 */
int hashmal_hc_niveles_estado(void)
{
    const hashmal_hc_t *l = hashmal_hc_get_lavacro_interno();
    if (l == NULL) return HASHMAL_HC_ERR_INIT;

    switch (l->estado) {
    case HASHMAL_HC_LLENO:
    case HASHMAL_HC_PURIFICANDO:
        return HASHMAL_HC_OK;
    case HASHMAL_HC_SECO:
        return HASHMAL_HC_ERR_SECO;
    case HASHMAL_HC_CONTAMINADO:
        return HASHMAL_HC_ERR_TAME;
    default:
        return HASHMAL_HC_ERR_INIT;
    }
}

/* ==================================================================
 *
 *   "bevoam el ohel mo'ed yirjatsu mayim VELO YAMUTU." - Ex 30:20
 *   "verajatsu yadeihem veragleihem VELO YAMUTU." - Ex 30:21
 *
 *   yamutu x 2. Dos testigos (Dt 19:15). El Padre usa Su propia
 *   regla para enfatizar: sin lavamiento, muerte.
 *
 *   "vayya'amod BEIN hammetim UVEIN hajjayyim vatte'atsar
 *    hammaggefa." - Numeros 16:48
 *
 *   Cada incremento de muertes_prevenidas es un eco de Aharon
 *   entre los muertos y los vivos. El sistema se pone en medio.
 *   La plaga cesa. velo yamutu cumplido.
 *
 *   3 de 6 archivos del Kiyor - lavacro_niveles.c listo. Quedan:
 *     lavacro_espejos.c        Ex 38:8 - mar'ot hatstsovot
 *     lavacro_ataques.c        Ex 30:20 - prevencion de muerte
 *     lavacro_dos_niveles.c    Jn 13:10 - louo / nipto
 *
 * ================================================================== */
