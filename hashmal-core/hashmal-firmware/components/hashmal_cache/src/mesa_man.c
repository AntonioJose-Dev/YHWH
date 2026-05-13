/*
 * ===================================================================
 * PIEZA 03 - LA MESA - mesa_man.c
 * Añadidura 11: MAN - GESTION DE RECURSOS (omer por cabeza)
 * Exodo 16:1-36 + Numeros 11:7-9 + Salmo 78:24-25 + Juan 6:31-58
 * ===================================================================
 *
 * "vayyomer YHWH el Moshe: hineni mamtir lakhem LEHEM min hashshamayim
 *  veyatsa ha'am velaqtu DEVAR YOM BEYOMO." - Exodo 16:4
 * "He aqui yo os hare llover PAN del cielo; y el pueblo saldra y
 *  recogera la PORCION DE CADA DIA en su dia."
 *
 * "liqtu mimmennu ish lefi okhlo OMER LAGGULGOLET mispar
 *  nafshoteikhem." - Exodo 16:16
 * "Recoged segun su comer, OMER POR CABEZA segun el numero de
 *  vuestras almas."
 *
 * "vayyamoddu va'omer veLO HE'EDIF HAMMARBEH veHAMMAM'IT LO HEJSIR;
 *  ish lefi okhlo laqatu." - Exodo 16:18
 * "Lo midieron por omer: NO LE SOBRO al que recogio MUCHO, NI LE
 *  FALTO al que recogio POCO."
 *
 * "vayomer Moshe alehem: ish AL YOTER MIMMENNU 'ad boqer." - Ex 16:19
 * "No DEJEIS NADA hasta la manana."
 *
 * "velo SHAM'U el Moshe vayyotiru anashim mimmennu 'ad boqer
 *  vayyarum TOLA'IM vayyiv'ash." - Exodo 16:20
 * "No oyeron a Moises - dejaron hasta la manana, crio GUSANOS y
 *  APESTO."
 *
 * "uvayyom hashshishi laqtu LEHEM MISHNEH shenei ha'omer la'ejad."
 *   - Exodo 16:22
 * "El sexto dia recogieron pan DOBLE, dos omeres por persona."
 *
 * "shesheh yamim tilqetuhu uvayyom hashshvi'i SHABBAT lo yiheyeh
 *  vo." - Exodo 16:26
 * "SEIS dias recogereis, el SEPTIMO Shabat - no habra."
 *
 * --- EXEGESIS OPERATIVA ---
 *
 *   omer (H6016)            MEDIDA fija (~2.2 L). Unidad standard,
 *                            no negociable.
 *
 *   gulgolet (H1538)        CABEZA. La asignacion es PER CAPITA, no
 *                            por familia o por jerarquia.
 *
 *   lefi okhlo (H6310+H398) "SEGUN SU COMER". La cuota se ajusta a
 *                            la necesidad real, pero el omer es el
 *                            techo - nadie pide mas.
 *
 *   he'dif (H5736)          SOBRAR. El que recogio mucho NO le
 *                            sobro - al medirlo, le quedo el omer
 *                            justo. Equidad MILAGROSA: la
 *                            redistribucion la hizo el milagro,
 *                            no el algoritmo.
 *
 *   tola'im (H8438)         GUSANOS. Lo guardado mas alla del dia
 *                            se PUDRIO. La acumulacion no es virtud.
 *
 *   mishneh (H4932)         DOBLE. Solo el sexto dia, anticipando
 *                            el reposo del septimo. Excepcion ritual,
 *                            no permanente.
 *
 *   devar yom beyomo (H1697+H3117) "PORCION DE CADA DIA en su dia".
 *                            Recurso diario, no acumulativo. El
 *                            man de hoy no es el de manana.
 *
 * --- COEXISTENCIA CON misgeret (mesa_misgeret.c) ---
 *
 *   misgeret (Ex 25:25) - rate limit POR slot caché (limite local
 *                          por estructura).
 *   man     (Ex 16)    - cuota PER NODO POR RECURSO (limite global
 *                          por consumidor).
 *
 * Son ortogonales: misgeret protege la Mesa de inundacion en sus
 * propios slots; man protege a los nodos de acapararse entre si.
 * Ambos coexisten sin tocar el otro - un mensaje broadcast cruza
 * misgeret de cache (si toca cache) y consume cuota MAN del emisor.
 *
 * --- DISTRIBUCION FASE 1 ---
 *
 * Defaults iguales para todos los nodos (Ex 16:18 lo he'dif). Los
 * limites son constantes; ningun nodo recibe trato especial. Fase 2
 * podra exponer API para ajustar el omer por recurso si una red
 * con perfil distinto lo necesita.
 *
 * --- MISHNEH MANUAL FASE 1 ---
 *
 * El flag mishne se activa por API explicita. Caller (operator o
 * wiring futuro a Moedim) decide cuando es vispera de Shabat. Auto-
 * detect requiere "saber el ciclo siguiente" - el motor de moedim
 * publica el ACTUAL, no el proximo. Manual es predecible y
 * testeable; auto-detect es PR futuro.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_cache.h        API publica
 *   esp_log.h (ORO EGIPTO) MAN_LOG_*
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <inttypes.h>

#include "esp_log.h"

#include "hashmal_cache.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *MAN_TAG = "hashmal.mesa.man";

#define MAN_LOG_I(fmt, ...)  ESP_LOGI(MAN_TAG, fmt, ##__VA_ARGS__)
#define MAN_LOG_W(fmt, ...)  ESP_LOGW(MAN_TAG, fmt, ##__VA_ARGS__)
#define MAN_LOG_E(fmt, ...)  ESP_LOGE(MAN_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * Estado interno
 *
 * Tabla 2D: [nodo_id][recurso] = bytes/mensajes/slots usados en el
 * ciclo actual. limite_actual es UN solo array por recurso: todos
 * los nodos comparten el mismo techo (Ex 16:18 equidad).
 * ================================================================== */
static uint32_t g_usado[HASHMAL_MAN_NODOS_MAX][HASHMAL_MAN_RECURSOS_TOTAL];
static uint32_t g_limite_actual[HASHMAL_MAN_RECURSOS_TOTAL] = {
    HASHMAL_MAN_OMER_BROADCAST,
    HASHMAL_MAN_OMER_DIRECTO,
    HASHMAL_MAN_OMER_BYTES_LORA,
    HASHMAL_MAN_OMER_SLOTS,
};

/* Mishneh flag: si true, en el proximo `_expirar` se duplican los
 * limites Y el flag se consume (one-shot, Ex 16:22 fue UN dia).
 * Si en el ciclo siguiente se quiere doble otra vez, el caller
 * debe re-llamar `_mishne_set(true)`. */
static bool g_mishne_pendiente = false;

/* Defaults base para retornar a la normalidad en el ciclo
 * post-mishneh. Constantes a partir de los #define. */
static const uint32_t g_omer_base[HASHMAL_MAN_RECURSOS_TOTAL] = {
    HASHMAL_MAN_OMER_BROADCAST,
    HASHMAL_MAN_OMER_DIRECTO,
    HASHMAL_MAN_OMER_BYTES_LORA,
    HASHMAL_MAN_OMER_SLOTS,
};

/* ==================================================================
 * Helpers estaticos
 * ================================================================== */

static const char *nombre_recurso(hashmal_man_recurso_t r)
{
    switch (r) {
    case HASHMAL_MAN_RECURSO_BROADCAST:   return "BROADCAST";
    case HASHMAL_MAN_RECURSO_DIRECTO:     return "DIRECTO";
    case HASHMAL_MAN_RECURSO_BYTES_LORA:  return "BYTES_LORA";
    case HASHMAL_MAN_RECURSO_SLOTS_CACHE: return "SLOTS_CACHE";
    default:                              return "<recurso desconocido>";
    }
}

static bool nodo_legitimo(uint8_t nodo_id)
{
    return nodo_id < HASHMAL_MAN_NODOS_MAX;
}

static bool recurso_legitimo(hashmal_man_recurso_t r)
{
    return (unsigned)r < HASHMAL_MAN_RECURSOS_TOTAL;
}

/* ==================================================================
 * API - hashmal_man_consumir (Ex 16:16 liqtu)
 * ================================================================== */
int hashmal_man_consumir(uint8_t nodo_id,
                          hashmal_man_recurso_t recurso,
                          uint32_t cantidad)
{
    if (!nodo_legitimo(nodo_id)) {
        MAN_LOG_E("consumir: nodo %u fuera de [0, %u)",
                  (unsigned)nodo_id, (unsigned)HASHMAL_MAN_NODOS_MAX);
        return HASHMAL_CACHE_ERR_INDICE;
    }
    if (!recurso_legitimo(recurso)) {
        MAN_LOG_E("consumir: recurso %d invalido", (int)recurso);
        return HASHMAL_CACHE_ERR_INDICE;
    }
    if (cantidad == 0u) {
        /* Cero es no-op valido. */
        return HASHMAL_CACHE_OK;
    }

    uint32_t usado_actual = g_usado[nodo_id][(unsigned)recurso];
    uint32_t limite       = g_limite_actual[(unsigned)recurso];

    /* Ex 16:18 - no le sobro al que recogio mucho: o cabe el omer
     * entero o no cabe. Sin overflow check de uint32 (fase 1 - los
     * cantidades son pequeñas). */
    if (usado_actual + cantidad > limite) {
        MAN_LOG_W("consumir: nodo %u %s usado=%" PRIu32 " + cant=%" PRIu32
                  " > limite=%" PRIu32 " - ERR_MAN_EXCEDIDO "
                  "(Ex 16:16 omer la-gulgolet)",
                  (unsigned)nodo_id, nombre_recurso(recurso),
                  usado_actual, cantidad, limite);
        return HASHMAL_CACHE_ERR_MAN_EXCEDIDO;
    }

    g_usado[nodo_id][(unsigned)recurso] = usado_actual + cantidad;
    return HASHMAL_CACHE_OK;
}

/* ==================================================================
 * API - hashmal_man_cuota_consultar (lectura pura)
 * ================================================================== */
int hashmal_man_cuota_consultar(uint8_t nodo_id,
                                 hashmal_man_recurso_t recurso,
                                 uint32_t *out_usado,
                                 uint32_t *out_limite)
{
    if (!nodo_legitimo(nodo_id) || !recurso_legitimo(recurso)) {
        return HASHMAL_CACHE_ERR_INDICE;
    }
    if (out_usado != NULL)  *out_usado  = g_usado[nodo_id][(unsigned)recurso];
    if (out_limite != NULL) *out_limite = g_limite_actual[(unsigned)recurso];
    return HASHMAL_CACHE_OK;
}

/* ==================================================================
 * API - hashmal_man_expirar (Ex 16:19-20 - lo guardado cria gusanos)
 *
 * Reset todos los contadores `usado`. Aplica mishneh si pendiente
 * (Ex 16:22 - duplica limites para este ciclo, consume el flag).
 * Si no hay mishneh, restaura los limites a los defaults base.
 * ================================================================== */
int hashmal_man_expirar(void)
{
    /* Reset usado total - lo no consumido se evapora (Ex 16:19-20). */
    memset(g_usado, 0, sizeof(g_usado));

    if (g_mishne_pendiente) {
        for (unsigned r = 0; r < HASHMAL_MAN_RECURSOS_TOTAL; ++r) {
            g_limite_actual[r] = g_omer_base[r] * 2u;
        }
        g_mishne_pendiente = false;  /* one-shot */
        MAN_LOG_I("expirar: nuevo ciclo - LEHEM MISHNEH aplicado "
                  "(Ex 16:22 vispera Shabat); limites x2");
    } else {
        /* Restaurar a base si veniamos de un mishneh previo. */
        for (unsigned r = 0; r < HASHMAL_MAN_RECURSOS_TOTAL; ++r) {
            g_limite_actual[r] = g_omer_base[r];
        }
        MAN_LOG_I("expirar: nuevo ciclo - omer base restaurado "
                  "(Ex 16:4 devar yom beyomo)");
    }
    return HASHMAL_CACHE_OK;
}

/* ==================================================================
 * API - hashmal_man_mishne_set / _activo (Ex 16:22)
 * ================================================================== */
int hashmal_man_mishne_set(bool activar)
{
    if (activar == g_mishne_pendiente) {
        return HASHMAL_CACHE_OK;  /* idempotente */
    }
    g_mishne_pendiente = activar;
    if (activar) {
        MAN_LOG_I("mishne ARMADO (Ex 16:22 lehem mishneh) - "
                  "el proximo expirar duplicara los limites");
    } else {
        MAN_LOG_I("mishne CANCELADO antes de ser aplicado");
    }
    return HASHMAL_CACHE_OK;
}

bool hashmal_man_mishne_activo(void)
{
    return g_mishne_pendiente;
}

/* ==================================================================
 *
 *   "DEVAR YOM BEYOMO." - Exodo 16:4
 *
 *   "OMER LAGGULGOLET." - Exodo 16:16
 *
 *   "LO HE'DIF HAMMARBEH veHAMMAM'IT LO HEJSIR." - Exodo 16:18
 *
 *   "ish AL YOTER MIMMENNU 'ad boqer." - Exodo 16:19
 *
 *   "vayyarum TOLA'IM vayyiv'ash." - Exodo 16:20
 *
 *   "LEHEM MISHNEH shenei ha'omer la'ejad." - Exodo 16:22
 *
 *   "shesheh yamim tilqetuhu uvayyom hashshvi'i SHABBAT." - Ex 16:26
 *
 *   La porcion del dia es del dia. Quien recoge mas de lo justo
 *   no se enriquece - le sobra y se pudre. Quien recoge menos no
 *   pasa hambre - el omer es justo para todos. La acumulacion no
 *   es economia, es corrupcion (tola'im). Y solo en vispera del
 *   reposo se permite el doble - porque el reposo no recoge,
 *   descansa.
 *
 *   Asi el firmware: cuotas diarias por cabeza, sin acumulacion.
 *   Lo no usado no se hereda al ciclo siguiente. La equidad es
 *   estructural, no algoritmica - el omer ya es el omer.
 *
 * ================================================================== */
