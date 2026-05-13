/*
 * ===================================================================
 * PIEZA 07 - EL ATRIO - atrio_miqlat.c
 * Añadidura 05: IR MIQLAT - CIUDAD DE REFUGIO
 * Numeros 35:6-34 + Josue 20:2-9 + Deuteronomio 19:1-13
 * ===================================================================
 *
 * "vehiqritem lakhem arim AREI MIQLAT tihyeinah lakhem;
 *  venas shamah rotseaj makkeh nefesh BISHGAGAH." - Numeros 35:11
 * "Senalad para vosotros ciudades de REFUGIO, para que HUYA alli
 *  el homicida que hubiere matado por ERROR."
 *
 * "shesh arei haMIQLAT tihyeinah lakhem." - Numeros 35:13-14
 * "SEIS seran vuestras ciudades de refugio."
 *
 * "vehayu ha'arim haeleh lemiqlat leKOL benei Yisrael veLAGER
 *  veLATOSHAV betokham." - Numeros 35:15
 * "Seran estas ciudades para refugio para TODOS los hijos de Israel
 *  Y para el EXTRANJERO Y el residente en medio de ellos."
 *
 * "ve'im beSETER yehadafennu... mot yumat hammakeh."
 *   - Numeros 35:20-21
 * "Pero si con EMBOSCADA (intencion) lo empujo... sera muerto el
 *  homicida." - el refugio NO protege al intencional.
 *
 * "vehayah 'ad 'omdo LIFNEY HA'EDAH LEMISHPAT." - Josue 20:6
 * "Hasta que comparezca ante la CONGREGACION para juicio."
 *
 * --- EXEGESIS OPERATIVA ---
 *
 *   miqlat (MQLT, H4733)  REFUGIO. Raiz qalat (H7038) = recoger,
 *                           acoger lo que se derrama. El refugio no
 *                           JUZGA; RECOGE.
 *
 *   shogeg (H7684)         POR ERROR. Condicion inicial del que se
 *                           acoge. El intencional (mezid, H2102) no
 *                           tiene derecho - ni siquiera a los cuernos
 *                           del altar (1 Re 2:28-31 caso de Yoav).
 *
 *   ger vetoshav (H1616, H8453) EXTRANJERO Y RESIDENTE. Nm 35:15
 *                           explicita que el refugio se extiende mas
 *                           alla del pueblo. En firmware: acepta
 *                           nodos no registrados en el pectoral.
 *
 * --- DIFERENCIA CON qarnot (altar_cuernos.c) ---
 *
 *   qarnot: refugio DENTRO del Mishkan, requiere autenticacion
 *           previa (era ya un sacerdote o representante de tribu).
 *           Casos 1 Re 1:50 Adoniya, 2:28 Yoav - ya eran del pueblo.
 *
 *   miqlat: refugio FUERA, AMPLIAMENTE. No requiere autenticacion.
 *           Nm 35:15 ger vetoshav - cualquier que llegue.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   designar         Nm 35:11 vehiqritem - flag miqlat local
 *   refugiar         Jos 20:3 venas shamah - registrar refugiado
 *   verificar        Jos 20:6 lifney ha'edah - veredicto externo
 *   expulsar         Nm 35:21 mot yumat - retirar slot (post-juicio)
 *   refugiado_estado lectura pura por node_id
 *   refugiados_contar lectura pura del count activo
 *
 * --- NO JUZGA ---
 *
 * El refugio no decide si el refugiado es shogeg o mezid. Esa
 * decision es de la 'edah (Jos 20:6) - Pieza 02 consenso, Pieza 06
 * auth, o API manual del operador. Miqlat solo hospeda hasta que
 * el veredicto llegue, y honra el veredicto cuando entra.
 *
 * --- LIMITE DE 6 SOFT ---
 *
 * Nm 35:6 fija 6 ciudades. El firmware no tiene autoridad central
 * para enforzarlo en red - solo logea WARN al designar recordando
 * el limite. Fase 2 podra discover-y-auto-deny via protocolo
 * broadcast en Pieza 04.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_perimeter.h        API publica (types, prototipos)
 *   hashmal_perim_internal.h   ahora_ms
 *   esp_log.h (ORO EGIPTO)     MIQLAT_LOG_*
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

#include "hashmal_perimeter.h"
#include "hashmal_perim_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *MIQLAT_TAG = "hashmal.atrio.miqlat";

#define MIQLAT_LOG_I(fmt, ...)  ESP_LOGI(MIQLAT_TAG, fmt, ##__VA_ARGS__)
#define MIQLAT_LOG_W(fmt, ...)  ESP_LOGW(MIQLAT_TAG, fmt, ##__VA_ARGS__)
#define MIQLAT_LOG_E(fmt, ...)  ESP_LOGE(MIQLAT_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * Estado interno - flag + tabla de refugiados
 * ================================================================== */
static bool g_miqlat_designado = false;

/* Tabla de refugiados. Slot vacio: estado = HASHMAL_MIQLAT_VACIO. */
static hashmal_miqlat_refugiado_t g_refugiados[HASHMAL_MIQLAT_REFUGIADOS_MAX];

/* ==================================================================
 * Helpers estaticos
 * ================================================================== */

static const char *nombre_motivo(hashmal_miqlat_motivo_t m)
{
    switch (m) {
    case HASHMAL_MIQLAT_MOTIVO_ATAQUE:      return "ATAQUE (perseguido)";
    case HASHMAL_MIQLAT_MOTIVO_ERROR:       return "ERROR (shogeg, Nm 35:11)";
    case HASHMAL_MIQLAT_MOTIVO_DESCONOCIDO:
    default:                                 return "DESCONOCIDO";
    }
}

static const char *nombre_estado(hashmal_miqlat_estado_t e)
{
    switch (e) {
    case HASHMAL_MIQLAT_EN_VERIFICACION: return "EN_VERIFICACION (Jos 20:6)";
    case HASHMAL_MIQLAT_LIMPIO:          return "LIMPIO";
    case HASHMAL_MIQLAT_MALICIOSO:       return "MALICIOSO (Nm 35:20)";
    case HASHMAL_MIQLAT_VACIO:
    default:                              return "VACIO";
    }
}

/* Busca por node_id - devuelve indice o -1 si no encontrado. Ignora
 * slots vacios. Si el caller quiere un slot para inserar, usa
 * buscar_slot_libre. */
static int buscar_por_node_id(uint8_t nodo_id)
{
    for (uint8_t i = 0; i < HASHMAL_MIQLAT_REFUGIADOS_MAX; ++i) {
        if (g_refugiados[i].estado == HASHMAL_MIQLAT_VACIO) continue;
        if (g_refugiados[i].nodo_id == nodo_id) return (int)i;
    }
    return -1;
}

/* Devuelve indice del primer slot vacio, o -1 si la tabla esta llena. */
static int buscar_slot_libre(void)
{
    for (uint8_t i = 0; i < HASHMAL_MIQLAT_REFUGIADOS_MAX; ++i) {
        if (g_refugiados[i].estado == HASHMAL_MIQLAT_VACIO) return (int)i;
    }
    return -1;
}

/* ==================================================================
 * API - hashmal_perimeter_miqlat_designar
 * ================================================================== */
int hashmal_perimeter_miqlat_designar(bool activar)
{
    if (activar == g_miqlat_designado) {
        /* Idempotente. */
        return HASHMAL_PERIM_OK;
    }
    g_miqlat_designado = activar;

    if (activar) {
        MIQLAT_LOG_I("miqlat DESIGNADO (Nm 35:11 vehiqritem) - "
                     "aviso operador: limite biblico SHESH=%u ciudades "
                     "(Nm 35:6); responsabilidad de conteo en red es "
                     "manual en fase 1",
                     (unsigned)HASHMAL_MIQLAT_CIUDADES_MAX);
    } else {
        MIQLAT_LOG_I("miqlat desactivado - refugiados existentes NO se "
                     "expulsan (Nm 35:28 'ad mot hakkohen); nuevos no "
                     "entraran");
    }
    return HASHMAL_PERIM_OK;
}

bool hashmal_perimeter_miqlat_designado(void)
{
    return g_miqlat_designado;
}

/* ==================================================================
 * API - hashmal_perimeter_miqlat_refugiar (Jos 20:3 venas shamah)
 * ================================================================== */
int hashmal_perimeter_miqlat_refugiar(uint8_t nodo_id,
                                       hashmal_miqlat_motivo_t motivo)
{
    if (!g_miqlat_designado) {
        MIQLAT_LOG_E("refugiar: nodo no designado miqlat (Nm 35:11)");
        return HASHMAL_PERIM_ERR_MIQLAT;
    }

    /* Nm 35:20-21 - si ya fue juzgado malicioso, no vuelve a entrar.
     * Ni siquiera a los cuernos del altar (1 Re 2:28-31). */
    int existente = buscar_por_node_id(nodo_id);
    if (existente >= 0) {
        hashmal_miqlat_estado_t est = g_refugiados[existente].estado;
        if (est == HASHMAL_MIQLAT_MALICIOSO) {
            MIQLAT_LOG_W("refugiar: nodo %u ya juzgado MALICIOSO "
                         "(Nm 35:20 beseter yehadafennu) - rechazado",
                         (unsigned)nodo_id);
            return HASHMAL_PERIM_ERR_MIQLAT_MALIC;
        }
        /* Ya registrado como EN_VERIFICACION o LIMPIO. Idempotente:
         * no incrementamos timestamp; solo refrescamos motivo si se
         * entrega uno mas especifico. */
        if (g_refugiados[existente].motivo == HASHMAL_MIQLAT_MOTIVO_DESCONOCIDO
            && motivo != HASHMAL_MIQLAT_MOTIVO_DESCONOCIDO) {
            g_refugiados[existente].motivo = motivo;
            MIQLAT_LOG_I("refugiar: nodo %u ya hospedado, motivo actualizado "
                         "-> %s", (unsigned)nodo_id, nombre_motivo(motivo));
        }
        return HASHMAL_PERIM_OK;
    }

    /* Buscar slot libre. */
    int slot = buscar_slot_libre();
    if (slot < 0) {
        MIQLAT_LOG_E("refugiar: tabla llena (%u refugiados) - "
                     "no hay mas acomodo",
                     (unsigned)HASHMAL_MIQLAT_REFUGIADOS_MAX);
        return HASHMAL_PERIM_ERR_MIQLAT_LLENO;
    }

    /* Alta: entra en EN_VERIFICACION, espera congregacion. */
    uint64_t ahora = hashmal_perim_ahora_ms();
    g_refugiados[slot].nodo_id                = nodo_id;
    g_refugiados[slot].motivo                 = motivo;
    g_refugiados[slot].estado                 = HASHMAL_MIQLAT_EN_VERIFICACION;
    g_refugiados[slot].timestamp_llegada_ms   = ahora;
    g_refugiados[slot].timestamp_veredicto_ms = 0ull;

    MIQLAT_LOG_I("refugiar: nodo %u entra slot %d motivo=%s "
                 "(Jos 20:3 venas shamah); espera juicio de la 'edah "
                 "(Jos 20:6)",
                 (unsigned)nodo_id, slot, nombre_motivo(motivo));
    return HASHMAL_PERIM_OK;
}

/* ==================================================================
 * API - hashmal_perimeter_miqlat_verificar (Jos 20:6 lifney ha'edah)
 * ================================================================== */
int hashmal_perimeter_miqlat_verificar(uint8_t nodo_id,
                                        hashmal_miqlat_veredicto_t veredicto)
{
    int idx = buscar_por_node_id(nodo_id);
    if (idx < 0) {
        MIQLAT_LOG_W("verificar: nodo %u no esta en tabla miqlat",
                     (unsigned)nodo_id);
        return HASHMAL_PERIM_ERR_MIQLAT;
    }

    hashmal_miqlat_estado_t nuevo;
    switch (veredicto) {
    case HASHMAL_MIQLAT_VEREDICTO_LIMPIO:
        nuevo = HASHMAL_MIQLAT_LIMPIO;
        break;
    case HASHMAL_MIQLAT_VEREDICTO_MALICIOSO:
        nuevo = HASHMAL_MIQLAT_MALICIOSO;
        break;
    default:
        return HASHMAL_PERIM_ERR_MIQLAT;
    }

    g_refugiados[idx].estado                 = nuevo;
    g_refugiados[idx].timestamp_veredicto_ms = hashmal_perim_ahora_ms();

    MIQLAT_LOG_I("verificar: nodo %u -> %s (Jos 20:6 lifney ha'edah)",
                 (unsigned)nodo_id, nombre_estado(nuevo));
    return HASHMAL_PERIM_OK;
}

/* ==================================================================
 * API - hashmal_perimeter_miqlat_expulsar (Nm 35:21 mot yumat)
 * ================================================================== */
int hashmal_perimeter_miqlat_expulsar(uint8_t nodo_id)
{
    int idx = buscar_por_node_id(nodo_id);
    if (idx < 0) {
        return HASHMAL_PERIM_ERR_MIQLAT;
    }

    MIQLAT_LOG_I("expulsar: nodo %u del slot %d (estado previo: %s)",
                 (unsigned)nodo_id, idx,
                 nombre_estado(g_refugiados[idx].estado));

    memset(&g_refugiados[idx], 0, sizeof(g_refugiados[idx]));
    /* memset deja estado = HASHMAL_MIQLAT_VACIO (0) por definicion. */
    return HASHMAL_PERIM_OK;
}

/* ==================================================================
 * API - lecturas puras
 * ================================================================== */
int hashmal_perimeter_miqlat_refugiado_estado(uint8_t nodo_id,
                                               hashmal_miqlat_refugiado_t *out)
{
    if (out == NULL) return HASHMAL_PERIM_ERR_NULL;

    int idx = buscar_por_node_id(nodo_id);
    if (idx < 0) return HASHMAL_PERIM_ERR_MIQLAT;

    memcpy(out, &g_refugiados[idx], sizeof(*out));
    return HASHMAL_PERIM_OK;
}

int hashmal_perimeter_miqlat_refugiados_contar(uint8_t *out_count)
{
    if (out_count == NULL) return HASHMAL_PERIM_ERR_NULL;

    uint8_t c = 0;
    for (uint8_t i = 0; i < HASHMAL_MIQLAT_REFUGIADOS_MAX; ++i) {
        hashmal_miqlat_estado_t e = g_refugiados[i].estado;
        if (e == HASHMAL_MIQLAT_EN_VERIFICACION
         || e == HASHMAL_MIQLAT_LIMPIO) {
            ++c;
        }
    }
    *out_count = c;
    return HASHMAL_PERIM_OK;
}

/* ==================================================================
 *
 *   "vehiqritem lakhem arim AREI MIQLAT tihyeinah." - Nm 35:11
 *
 *   "leKOL benei Yisrael veLAGER veLATOSHAV betokham." - Nm 35:15
 *
 *   "'ad 'omdo LIFNEY HA'EDAH LEMISHPAT." - Josue 20:6
 *
 *   "ve'im beSETER yehadafennu... mot yumat." - Nm 35:20-21
 *
 *   El refugio es AMPLIO - para Israel, el extranjero, el residente.
 *   La puerta se abre porque huir tambien es una forma de llegar.
 *   Pero el refugio no sustituye el juicio: hospeda hasta que la
 *   congregacion compareze. Y el intencional no entra - aunque huya.
 *   La misericordia del refugio no cubre la emboscada.
 *
 * ================================================================== */
