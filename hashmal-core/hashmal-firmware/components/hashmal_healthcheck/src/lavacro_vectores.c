/**
 * ===================================================================
 * PIEZA 12 - EL KIYOR (LAVACRO) - lavacro_vectores.c
 * Sub-pieza: YADEIHEM VERAGLEIHEM - 4 vectores de verificacion
 * Exodo 30:19 + Salmo 24:3-4 + Ezequiel 36:25 + Efesios 5:26
 * ===================================================================
 *
 * "verajatsu Aharon uvanav MIMMENNU et YADEIHEM veet RAGLEIHEM"
 *   - Ex 30:19
 * "Lavaran Aharon y sus hijos DE EL sus MANOS y sus PIES."
 *
 * "mi ya'aleh behar YHWH... neqi JAPPAYIM uvar LEVAV"
 *   - Salmo 24:3-4
 * "?Quien subira al monte de YHWH? El limpio de MANOS y puro de
 *  CORAZON."
 *
 * "vezaraqti 'aleijem MAYIM TEHORIM" - Ezequiel 36:25
 * "Y rociare sobre vosotros AGUA LIMPIA."
 *
 * "katharisas to loutro tou hydatos EN RHEMATI" - Efesios 5:26
 * "Limpiandola con el lavamiento del agua POR LA PALABRA."
 *
 * --- 4 VECTORES DE VERIFICACION ---
 *
 *   VEC_YADAYIM   manos     Ex 30:19   ACCIONES del nodo
 *   VEC_RAGLAYIM  pies      Ex 30:19   RUTAS del nodo
 *   VEC_LEVAV     corazon   Sal 24:4   INTENCION del nodo
 *   VEC_MAYIM     agua      Ex 30:18   REFERENCIA EXTERNA
 *
 * Los 2 primeros vienen de Ex 30:19 (mandato directo del Padre).
 * El tercero lo anade Sal 24:4 (requisito para subir al monte).
 * El cuarto viene del agua misma (Ex 30:18 + Ez 36:25 + Ef 5:26).
 *
 * --- mimmennu - DE EL ---
 *
 * "mimmennu" (Ex 30:19) - todos lavan DEL MISMO kiyor. El singleton.
 * Los 4 vectores operan sobre el mismo lavacro.
 * hashmal_hc_lavacro_mut() (de hashmal_hc_internal.h) da acceso
 * mutable al singleton para actualizar contadores y estados.
 *
 * --- CRITERIO DE LA FASE 1 ---
 *
 * En esta primera fase, cada verificacion lee el NIVEL CORRIENTE
 * del vector:
 *   TAHOR                    -> check PASA (contadores + purificaciones)
 *   TAME / JATA / MAVET      -> check FALLA (contadores + contaminaciones)
 *
 * Los NIVELES se mueven SOLO desde `lavacro_niveles.c`. Aqui solo
 * leemos ese nivel para reportar: verificar_* es el REPORTADOR, no
 * el JUEZ. Asi el sacerdote lava y MIRA; el Padre decide el nivel
 * por Su Palabra (lavacro_niveles.c aplica los movimientos segun
 * reglas y eventos).
 *
 * Fase 2 anadira verificacion REAL (acciones en la red, rutas, intent
 * patterns, comparacion contra golden image). El contrato publico no
 * cambiara - solo la profundidad del check.
 *
 * --- LLENO -> PURIFICANDO EN EL PRIMER CHECK ---
 *
 * "bevoam el ohel mo'ed yirjatsu mayim" (Ex 30:20) - al ENTRAR, se
 * lavaran. El verbo esta en imperfecto (accion repetida). La primera
 * invocacion de un verificar_* pasa el kiyor de LLENO (agua quieta)
 * a PURIFICANDO (agua en uso). Subsecuentes llamadas quedan en
 * PURIFICANDO. Reflejo del servicio sacerdotal: una vez en servicio,
 * siempre en servicio mientras haya agua.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   verificar_yadayim    Ex 30:19 + Stg 4:8   - manos/acciones
 *   verificar_raglayim   Ex 30:19 + Pr 4:26   - pies/rutas
 *   verificar_levav      Sal 24:4 + Jr 17:10  - corazon/intencion
 *   verificar_mayim      Ex 30:18 + Ez 36:25  - agua/referencia externa
 *   vectores_estado      lectura pura
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_healthcheck.h     API publica
 *   hashmal_hc_internal.h     lavacro_mut, get_lavacro_interno,
 *                              ahora_ms, vector_legitimo
 *   esp_log.h (ORO EGIPTO)    VEC_LOG_*
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
static const char *VEC_TAG = "hashmal.hc.vectores";

#define VEC_LOG_I(fmt, ...)  ESP_LOGI(VEC_TAG, fmt, ##__VA_ARGS__)
#define VEC_LOG_W(fmt, ...)  ESP_LOGW(VEC_TAG, fmt, ##__VA_ARGS__)
#define VEC_LOG_E(fmt, ...)  ESP_LOGE(VEC_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * HELPER ESTATICO - verificar_vector (logica central comun)
 *
 * Centraliza el flujo de los 4 verificar_* publicos. Cada wrapper
 * invoca esta funcion con su enum y luego anade el log especifico
 * con sus testigos biblicos. Asi la arquitectura hereda la unidad
 * de "mimmennu" - del mismo kiyor bebe cada vector.
 *
 * Flujo (orden estricto):
 *   1) lavacro_mut() != NULL     -> si NULL: ERR_INIT
 *   2) vector_legitimo(vec)      -> si no: ERR_VECTOR
 *   3) estado == SECO            -> ERR_SECO (sin agua)
 *   4) estado == CONTAMINADO     -> ERR_TAME (agua impura global)
 *   5) estado == LLENO           -> transicion -> PURIFICANDO
 *   6) lectura del nivel del vector:
 *        TAHOR                   -> pasa: checks_pasados + purificaciones + OK
 *        TAME / JATA / MAVET     -> falla: checks_fallidos + contaminaciones + ERR_TAME
 * ================================================================== */
static int verificar_vector(hashmal_hc_vector_t vec)
{
    hashmal_hc_t *l = hashmal_hc_lavacro_mut();
    if (l == NULL) {
        return HASHMAL_HC_ERR_INIT;
    }

    if (!hashmal_hc_vector_legitimo(vec)) {
        return HASHMAL_HC_ERR_VECTOR;
    }

    /* Estados que impiden servicio. */
    if (l->estado == HASHMAL_HC_SECO) {
        return HASHMAL_HC_ERR_SECO;
    }
    if (l->estado == HASHMAL_HC_CONTAMINADO) {
        return HASHMAL_HC_ERR_TAME;
    }

    /* Solo LLENO o PURIFICANDO prosiguen. */
    if (l->estado != HASHMAL_HC_LLENO &&
        l->estado != HASHMAL_HC_PURIFICANDO) {
        return HASHMAL_HC_ERR_INIT;
    }

    /* Ex 30:20 "bevoam yirjatsu" - al entrar, se lavan. La primera
     * invocacion transiciona el agua quieta (LLENO) a agua en uso
     * (PURIFICANDO). Las siguientes ya estan en PURIFICANDO. */
    if (l->estado == HASHMAL_HC_LLENO) {
        l->estado = HASHMAL_HC_PURIFICANDO;
    }

    /* Phase 1: leer el nivel corriente del vector (lavacro_niveles.c
     * lo mueve segun reglas). Aqui reportamos, no juzgamos. */
    hashmal_hc_vector_estado_t *v = &l->vectores[vec];
    v->ultimo_check_ms = hashmal_hc_ahora_ms();

    if (v->nivel == HASHMAL_HC_TAHOR) {
        /* neqi jappayim - manos limpias (Sal 24:4). */
        v->checks_pasados  += 1ull;
        l->purificaciones  += 1ull;
        return HASHMAL_HC_OK;
    }

    /* TAME / JATA / MAVET - diagnostico normal de impureza. */
    v->checks_fallidos  += 1ull;
    l->contaminaciones  += 1ull;
    return HASHMAL_HC_ERR_TAME;
}

/* ==================================================================
 * API PUBLICA - verificar_yadayim (Ex 30:19 + Stg 4:8)
 * ================================================================== */

/**
 * hashmal_hc_verificar_yadayim - las MANOS (acciones)
 *
 * Ex 30:19 - "et yadeihem": las MANOS se lavan primero en el
 * texto. Lo que el nodo HACE es la primera superficie de contacto
 * con el Mishkan: escrituras, transacciones, operaciones.
 *
 * Fase 1: lee el nivel corriente del vector YADAYIM (mantenido por
 * lavacro_niveles.c). TAHOR -> pasa; cualquier otro -> falla.
 *
 * Fase 2: verificacion real de acciones contra politica (firmas,
 * autorizacion via pieza 06, patrones esperados).
 *
 * 2 testigos:
 *   Sal 24:4 - "neqi jappayim" (inocente de palmas)
 *   Stg 4:8  - "katharisate jeiras" (limpiad las manos)
 */
int hashmal_hc_verificar_yadayim(void)
{
    const int rc = verificar_vector(HASHMAL_HC_VEC_YADAYIM);

    if (rc == HASHMAL_HC_OK) {
        VEC_LOG_I("yadáyim TAHÓR — neqí jappáyim (Sal 24:4); "
                  "manos limpias, acción legítima");
    } else if (rc == HASHMAL_HC_ERR_TAME) {
        VEC_LOG_W("yadáyim TAMÉ — katharísate jeíras (Stg 4:8); "
                  "acción contaminada registrada");
    } else {
        VEC_LOG_E("verificar yadáyim: error de infraestructura rc=%d", rc);
    }
    return rc;
}

/* ==================================================================
 * API PUBLICA - verificar_raglayim (Ex 30:19 + Pr 4:26)
 * ================================================================== */

/**
 * hashmal_hc_verificar_raglayim - los PIES (rutas)
 *
 * Ex 30:19 - "veet ragleihem": los PIES se lavan despues de las
 * manos. Por donde el nodo CAMINA: rutas de comunicacion,
 * destinos, paths de routing.
 *
 * Pr 4:26 - "palles ma'gal ragleja": examina la senda de tus pies.
 * El lavado de pies es examen de la TRAYECTORIA.
 *
 * 2 testigos:
 *   Ex 30:19 - "veet ragleihem" (los pies)
 *   Pr 4:26  - "palles ma'gal ragleja" (examina la senda)
 */
int hashmal_hc_verificar_raglayim(void)
{
    const int rc = verificar_vector(HASHMAL_HC_VEC_RAGLAYIM);

    if (rc == HASHMAL_HC_OK) {
        VEC_LOG_I("ragláyim TAHÓR — pallés maʻgal ragléja (Pr 4:26); "
                  "sendas examinadas, rutas legítimas");
    } else if (rc == HASHMAL_HC_ERR_TAME) {
        VEC_LOG_W("ragláyim TAMÉ — ruta contaminada registrada "
                  "(Éx 30:19 ragléihem)");
    } else {
        VEC_LOG_E("verificar ragláyim: error de infraestructura rc=%d", rc);
    }
    return rc;
}

/* ==================================================================
 * API PUBLICA - verificar_levav (Sal 24:4 + Jr 17:10)
 * ================================================================== */

/**
 * hashmal_hc_verificar_levav - el CORAZON (intencion)
 *
 * Ex 30:19 solo nombra manos y pies. El vector LEVAV lo ANADE el
 * Salmo 24:4 como requisito para "subir al monte de YHWH":
 * "neqi jappayim UVAR LEVAV" - limpio de manos Y puro de corazon.
 *
 * Manos sin corazon = accion sin intencion: el nodo PUEDE hacer lo
 * correcto por la razon equivocada. Jr 17:10 responde: "YO YHWH
 * escudrino el corazon" (joqer lev). La intencion NO se esconde.
 *
 * Fase 1: estado del vector. Fase 2: intent verification profunda
 * (pattern analysis, comportamiento temporal, coherencia con acciones).
 *
 * 2 testigos:
 *   Sal 24:4  - "uvar levav" (puro de corazon)
 *   Jr 17:10  - "ani YHWH joqer lev" (YHWH escudrina el corazon)
 *   [3er testigo: 1 Cr 28:9 - "YHWH doresh kol levavot"]
 */
int hashmal_hc_verificar_levav(void)
{
    const int rc = verificar_vector(HASHMAL_HC_VEC_LEVAV);

    if (rc == HASHMAL_HC_OK) {
        VEC_LOG_I("levav BAR — puro de corazón (Sal 24:4 uvár levav); "
                  "intención legítima");
    } else if (rc == HASHMAL_HC_ERR_TAME) {
        VEC_LOG_W("levav TAMÉ — joqér lev (Jr 17:10); intención "
                  "escudriñada, contaminación registrada");
    } else {
        VEC_LOG_E("verificar levav: error de infraestructura rc=%d", rc);
    }
    return rc;
}

/* ==================================================================
 * API PUBLICA - verificar_mayim (Ex 30:18 + Ez 36:25 + Ef 5:26)
 * ================================================================== */

/**
 * hashmal_hc_verificar_mayim - el AGUA (referencia externa)
 *
 * El agua NO la genera el kiyor. La PONE Moises desde fuera:
 * Ex 30:18 - "venatatta shamma mayim". Ez 36:25 - "vezaraqti
 * 'aleijem mayim tehorim": YO rociare AGUA LIMPIA sobre vosotros.
 * El estandar viene de ARRIBA, no del nodo.
 *
 * Ef 5:26 - "to loutro tou hydatos EN RHEMATI": el lavamiento del
 * agua POR LA PALABRA. La referencia canonica es la Palabra misma.
 * El nodo se compara contra la Palabra, no contra si mismo.
 *
 * Fase 1: estado del vector MAYIM. Fase 2: comparacion contra
 * golden image distribuida (hash canonico, estado esperado).
 *
 * 2 testigos:
 *   Ez 36:25 - "mayim tehorim" (agua limpia desde arriba)
 *   Ef 5:26  - "en rhemati" (por la Palabra)
 */
int hashmal_hc_verificar_mayim(void)
{
    const int rc = verificar_vector(HASHMAL_HC_VEC_MAYIM);

    if (rc == HASHMAL_HC_OK) {
        VEC_LOG_I("máyim TEHORIM — agua limpia (Ez 36:25); referencia "
                  "canónica coincide (Ef 5:26 en rhḗmati)");
    } else if (rc == HASHMAL_HC_ERR_TAME) {
        VEC_LOG_W("máyim TAMÉ — referencia canónica diverge "
                  "(Ef 5:26 rhḗmati no cumplido)");
    } else {
        VEC_LOG_E("verificar máyim: error de infraestructura rc=%d", rc);
    }
    return rc;
}

/* ==================================================================
 * API PUBLICA - vectores_estado (lectura pura)
 * ================================================================== */

/**
 * hashmal_hc_vectores_estado - ?los 4 vectores estan operables?
 *
 * Lectura pura - sin logs. Mismo patron que fuego_estado del altar,
 * reshet_estado, ledger_estado: el caller consulta silenciosamente
 * y decide que hacer.
 *
 * Retornos:
 *   OK          si estado in { LLENO, PURIFICANDO } (hay agua)
 *   ERR_SECO    si estado == SECO (sin agua)
 *   ERR_TAME    si estado == CONTAMINADO (agua impura global)
 *   ERR_INIT    si no levantado
 */
int hashmal_hc_vectores_estado(void)
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
 *   "verajatsu Aharon uvanav MIMMENNU et YADEIHEM veet
 *    RAGLEIHEM." - Exodo 30:19
 *
 *   "neqi JAPPAYIM uvar LEVAV." - Salmo 24:4
 *
 *   "vezaraqti 'aleijem MAYIM TEHORIM." - Ezequiel 36:25
 *
 *   4 vectores del mismo kiyor: manos (acciones), pies (rutas),
 *   corazon (intencion), agua (referencia externa). El sacerdote
 *   lava DE EL - mimmennu - del mismo lavacro. Uno solo, cuatro
 *   superficies, una sola agua.
 *
 *   El Padre mira los 4 a la vez. Quien sube al monte pasa el
 *   examen ENTERO, no solo las manos.
 *
 *   2 de 6 archivos del Kiyor - lavacro_vectores.c listo. Quedan:
 *     lavacro_niveles.c        Ex 30:20-21 - velo yamutu x 2
 *     lavacro_espejos.c        Ex 38:8 - mar'ot hatstsovot
 *     lavacro_ataques.c        Ex 30:20 - prevencion de muerte
 *     lavacro_dos_niveles.c    Jn 13:10 - louo / nipto
 *
 * ================================================================== */
