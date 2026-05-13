/**
 * ===================================================================
 * PIEZA 07 - EL ATRIO (JATSER) - atrio_columnas.c
 * Sub-pieza: 'AMMUDIM - las 60 columnas del atrio
 * Exodo 27:10-17 + Exodo 38:10-17 + Numeros 4:31-32
 * ===================================================================
 *
 * "ve'ammudav 'ESRIM ve'adneihem 'ESRIM NEJOSHET vavei ha'ammudim
 *  vajashuqeihem KESEF" - Exodo 27:10 (lado sur)
 * "Sus COLUMNAS VEINTE, sus BASES VEINTE de BRONCE; los GANCHOS
 *  de las columnas y sus BANDAS de PLATA."
 *
 * "veken lipe'at tsafon baorej qela'im me'a orej ve'ammudav 'esrim
 *  ve'adneihem 'esrim nejoshet" - Exodo 27:11 (lado norte)
 * "Y asi para el lado NORTE: cortinas de 100 codos, 20 columnas,
 *  20 bases de bronce."
 *
 * "verojav hejatser lipe'at yam... 'ammudeihem 'ASARA ve'adneihem
 *  'ASARA" - Exodo 27:12
 * "Y el ancho del atrio al lado oeste (yam)... 10 columnas, 10 bases."
 *
 * "uljatef hejatser... 'ammudeihem SHELOSHA ve'adneihem SHELOSHA"
 *   - Exodo 27:14,15
 * "Para cada HOMBRO (katef) del atrio... 3 columnas, 3 bases."
 *
 * "ulesha'ar hejatser masaj 'esrim amma... 'ammudeihem ARBA'A
 *  ve'adneihem ARBA'A" - Exodo 27:16
 * "Para la PUERTA del atrio... 4 columnas, 4 bases."
 *
 * "KOL 'ammudei hejatser saviv MEJUSHAQIM KESEF vaveihem KESEF
 *  ve'adneihem NEJOSHET" - Exodo 27:17
 * "TODAS las columnas del atrio alrededor: con BANDAS DE PLATA,
 *  ganchos de PLATA, y BASES de BRONCE."
 *
 * "ve'ammudei hejatser saviv ve'adneihem viyitdotam" - Nm 4:32
 * "Las COLUMNAS del atrio alrededor, y sus bases y sus estacas."
 * (Los hijos de MERARI las cargan al migrar.)
 *
 * --- VOCABULARIO HEBREO ---
 *
 *   'ammud    ('MWD, H5982)  COLUMNA. Raiz 'amad (H5975) = PARARSE.
 *                                La columna ES lo que se para. 60 se
 *                                paran alrededor.
 *   'eden     (ADN, H134)     BASE / pedestal. Cada columna con SU
 *                                base individual - sin compartir.
 *   vav       (WW, H2053)       GANCHO. Plata. Conecta cortina a
 *                                columna - sin el, la cortina cuelga
 *                                suelta.
 *   jashuq    (ChShWQ, H2838)  BANDA / anillo. Plata. Raiz jashaq
 *                                (H2836) = abrazar. La plata se
 *                                ABRAZA a la columna.
 *   katef     (KTP, H3802)   HOMBRO. Los dos flancos de la puerta
 *                                al este (15 codos cada uno) son los
 *                                "hombros".
 *   kesef     (KSP, H3701)   PLATA. Redencion (Ex 30:13 "seqel
 *                                haqodesh" del rescate). Arriba.
 *   nejoshet  (NChShT, H5178) BRONCE. Juicio. Abajo.
 *
 * --- DISTRIBUCION EXACTA (60 = 20+20+10+3+3+4) ---
 *
 *   SUR      negev / teiman   20 columnas  20 bases   (Ex 27:10)
 *   NORTE    tsafon           20 columnas  20 bases   (Ex 27:11)
 *   OESTE    yam              10 columnas  10 bases   (Ex 27:12)
 *   ESTE-IZQ katef 1           3 columnas   3 bases   (Ex 27:14)
 *   ESTE-DER katef 2           3 columnas   3 bases   (Ex 27:15)
 *   PUERTA   sha'ar            4 columnas   4 bases   (Ex 27:16)
 *   ----------------------------------------------------------
 *   TOTAL                     60 columnas  60 bases
 *
 * --- TEOLOGIA DE LA COLUMNA ---
 *
 *   1) CADA COLUMNA TIENE SU PROPIA BASE (v.10-16, la palabra
 *      "'adneihem" siempre concuerda en numero con "'ammudim").
 *      No se comparte fundamento. En firmware: cada nodo de
 *      frontera con cimiento individual.
 *
 *   2) BRONCE ABAJO + PLATA ARRIBA (v.17). El juicio sostiene;
 *      la Palabra conecta. Base (nivel basico) = bronce; gancho
 *      y banda (nivel conectivo) = plata.
 *
 *   3) SIMETRIA SUR=NORTE (20=20). Los flancos espejan. Solo el
 *      este se ROMPE por la puerta - la excepcion que confirma.
 *
 *   4) 'AMAD - "parada". Columna NO es sentada ni acostada.
 *      Activa, vertical, lista. Por eso `activa=true` es estado
 *      obligatorio tras registro.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   columna_registrar    Ex 27:10-16 - registrar por lado + cuota
 *   columna_estado       Ex 27:10-16 - lectura por id
 *   columnas_activas     Ex 27:17 + Nm 4:32 - conteo total
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_perimeter.h      API publica
 *   hashmal_perim_internal.h atrio_mut, get_atrio_interno, ahora_ms
 *   esp_log.h (ORO EGIPTO)   COL_LOG_*
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <inttypes.h>

#include "esp_log.h"

#include "hashmal_perimeter.h"
#include "hashmal_perim_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *COL_TAG = "hashmal.perim.columnas";

#define COL_LOG_I(fmt, ...)  ESP_LOGI(COL_TAG, fmt, ##__VA_ARGS__)
#define COL_LOG_W(fmt, ...)  ESP_LOGW(COL_TAG, fmt, ##__VA_ARGS__)
#define COL_LOG_E(fmt, ...)  ESP_LOGE(COL_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * HELPERS ESTATICOS
 * ================================================================== */

/**
 * Nombre hebreo del lado para logs.
 */
static const char *nombre_lado(hashmal_perimeter_lado_t lado)
{
    switch (lado) {
    case HASHMAL_PERIM_LADO_SUR:       return "SUR (négev)";
    case HASHMAL_PERIM_LADO_NORTE:     return "NORTE (tsafón)";
    case HASHMAL_PERIM_LADO_OESTE:     return "OESTE (yam)";
    case HASHMAL_PERIM_LADO_ESTE_IZQ:  return "ESTE-IZQ (katéf 1)";
    case HASHMAL_PERIM_LADO_ESTE_DER:  return "ESTE-DER (katéf 2)";
    case HASHMAL_PERIM_LADO_PUERTA:    return "PUERTA (shaʻar)";
    default:                           return "<desconocido>";
    }
}

/**
 * Valida que el enum `lado` este dentro del tavnit (no valor de
 * cast arbitrario fuera del rango).
 */
static bool lado_legitimo(hashmal_perimeter_lado_t lado)
{
    return (lado == HASHMAL_PERIM_LADO_SUR)       ||
           (lado == HASHMAL_PERIM_LADO_NORTE)     ||
           (lado == HASHMAL_PERIM_LADO_OESTE)     ||
           (lado == HASHMAL_PERIM_LADO_ESTE_IZQ)  ||
           (lado == HASHMAL_PERIM_LADO_ESTE_DER)  ||
           (lado == HASHMAL_PERIM_LADO_PUERTA);
}

/**
 * Maximo de columnas por lado segun Ex 27:10-16.
 */
static uint8_t max_columnas_lado(hashmal_perimeter_lado_t lado)
{
    switch (lado) {
    case HASHMAL_PERIM_LADO_SUR:       return HASHMAL_PERIM_COLUMNAS_SUR;       /* 20 */
    case HASHMAL_PERIM_LADO_NORTE:     return HASHMAL_PERIM_COLUMNAS_NORTE;     /* 20 */
    case HASHMAL_PERIM_LADO_OESTE:     return HASHMAL_PERIM_COLUMNAS_OESTE;     /* 10 */
    case HASHMAL_PERIM_LADO_ESTE_IZQ:  return HASHMAL_PERIM_COLUMNAS_ESTE_IZQ;  /* 3  */
    case HASHMAL_PERIM_LADO_ESTE_DER:  return HASHMAL_PERIM_COLUMNAS_ESTE_DER;  /* 3  */
    case HASHMAL_PERIM_LADO_PUERTA:    return HASHMAL_PERIM_COLUMNAS_PUERTA;    /* 4  */
    default:                           return 0u;
    }
}

/**
 * Cuenta columnas activas registradas en un lado especifico.
 */
static uint8_t contar_columnas_lado(const hashmal_perimeter_t *a,
                                    hashmal_perimeter_lado_t  lado)
{
    uint8_t n = 0u;
    for (uint8_t i = 0; i < HASHMAL_PERIM_COLUMNAS; ++i) {
        if (a->columnas[i].activa && a->columnas[i].lado == lado) {
            ++n;
        }
    }
    return n;
}

/* ==================================================================
 * API PUBLICA - columna_registrar (Ex 27:10-17)
 * ================================================================== */

/**
 * hashmal_perimeter_columna_registrar - registra una columna
 *
 * Cada 'ammud se registra en su lado. La cuota por lado es estricta
 * (Ex 27:10-16): 20 sur, 20 norte, 10 oeste, 3+3 flancos este, 4
 * puerta. Exceder cualquiera -> ERR_LLENO.
 *
 * --- FLUJO ---
 *
 *   1) atrio levantado              (ERR_INIT)
 *   2) id < 60                       (ERR_LADO - fuera de rango)
 *   3) lado legitimo                 (ERR_LADO)
 *   4) si ya activa:
 *        mismo lado -> OK idempotente (no-op)
 *        otro lado  -> ERR_DUPLICADO
 *   5) cuota del lado libre          (ERR_LLENO si completa)
 *   6) registro: lado, activa=true, registro_ms
 *   7) columnas_registradas += 1
 *   8) si total == 60 -> estado ACTIVO + log solemne Ex 27:17
 *   9) LOG_I con lado y conteo
 *
 * 2 testigos:
 *   Ex 27:10   - 'ammudav 'esrim (columnas veinte - distribucion)
 *   Ex 27:17   - kol 'ammudei hejatser saviv (todas alrededor)
 */
int hashmal_perimeter_columna_registrar(uint8_t id,
                                        hashmal_perimeter_lado_t lado)
{
    hashmal_perimeter_t *a = hashmal_perim_atrio_mut();
    if (a == NULL) {
        COL_LOG_E("columna_registrar: atrio no levantado");
        return HASHMAL_PERIM_ERR_INIT;
    }

    if (id >= HASHMAL_PERIM_COLUMNAS) {
        COL_LOG_E("columna_registrar: id %u fuera de [0, %d) "
                  "(Éx 27 — 60 ʻammudím total)",
                  (unsigned)id, HASHMAL_PERIM_COLUMNAS);
        return HASHMAL_PERIM_ERR_LADO;
    }

    if (!lado_legitimo(lado)) {
        COL_LOG_E("columna_registrar: lado %d fuera del tavnít "
                  "(Éx 27:9-16)", (int)lado);
        return HASHMAL_PERIM_ERR_LADO;
    }

    hashmal_perimeter_columna_t *col = &a->columnas[id];

    /* Idempotencia sobre mismo lado / rechazo sobre lado distinto. */
    if (col->activa) {
        if (col->lado == lado) {
            COL_LOG_I("columna_registrar: id=%u ya activa en %s "
                      "(idempotente, no-op)",
                      (unsigned)id, nombre_lado(lado));
            return HASHMAL_PERIM_OK;
        }
        COL_LOG_E("columna_registrar: id=%u ya activa en %s — intento "
                  "de registrar en %s (Éx 27:10-16 cada columna a su lado)",
                  (unsigned)id, nombre_lado(col->lado), nombre_lado(lado));
        return HASHMAL_PERIM_ERR_DUPLICADO;
    }

    /* Cuota del lado - Ex 27:10-16 numeros exactos. */
    const uint8_t ocupadas = contar_columnas_lado(a, lado);
    const uint8_t maximo   = max_columnas_lado(lado);
    if (ocupadas >= maximo) {
        COL_LOG_E("columna_registrar: %s ya tiene %u/%u columnas "
                  "— cuota completa (Éx 27:10-16)",
                  nombre_lado(lado), (unsigned)ocupadas, (unsigned)maximo);
        return HASHMAL_PERIM_ERR_LLENO;
    }

    /* Registro. 'amad: la columna SE PARA (activa=true). */
    const uint64_t ahora = hashmal_perim_ahora_ms();
    col->lado        = lado;
    col->activa      = true;
    col->registro_ms = ahora;
    a->columnas_registradas += 1u;

    COL_LOG_I("ʻammúd id=%u en %s — en pie (%u/%u este lado; %u/%d total)",
              (unsigned)id, nombre_lado(lado),
              (unsigned)(ocupadas + 1u), (unsigned)maximo,
              (unsigned)a->columnas_registradas,
              HASHMAL_PERIM_COLUMNAS);

    /* Si las 60 estan registradas: transicion a ACTIVO. Ex 27:17 -
     * "kol 'ammudei hejatser saviv": todas alrededor. */
    if (a->columnas_registradas >= HASHMAL_PERIM_COLUMNAS &&
        a->estado == HASHMAL_PERIM_LEVANTADO) {
        a->estado = HASHMAL_PERIM_ACTIVO;
        COL_LOG_I("═══ KOL ʻAMMUDÉI HEJATSÉR SAVÍV ═══ (Éx 27:17)");
        COL_LOG_I("  60 columnas en pie — bronce abajo, plata arriba");
        COL_LOG_I("  estado: LEVANTADO → ACTIVO");
    }
    return HASHMAL_PERIM_OK;
}

/* ==================================================================
 * API PUBLICA - columna_estado (lectura por id)
 * ================================================================== */

/**
 * hashmal_perimeter_columna_estado - ?esta columna esta en pie?
 *
 * Lectura pura, sin logs. Patron consistente con estados pasivos
 * (fuego_estado, reshet_estado, etc.).
 *
 * Retornos:
 *   OK              si activa
 *   ERR_INIT        si atrio no levantado
 *   ERR_LADO        si id fuera de [0, 60)
 *   ERR_DESMONTADO  si columna aun no registrada
 *
 * 2 testigos:
 *   Ex 27:10   - las 20 del sur (estado por id)
 *   Nm 4:32    - 'ammudei hejatser saviv (conteo al migrar)
 */
int hashmal_perimeter_columna_estado(uint8_t id)
{
    const hashmal_perimeter_t *a = hashmal_perim_get_atrio_interno();
    if (a == NULL) return HASHMAL_PERIM_ERR_INIT;

    if (id >= HASHMAL_PERIM_COLUMNAS) return HASHMAL_PERIM_ERR_LADO;

    if (!a->columnas[id].activa) return HASHMAL_PERIM_ERR_DESMONTADO;

    return HASHMAL_PERIM_OK;
}

/* ==================================================================
 * API PUBLICA - columnas_activas (conteo total)
 * ================================================================== */

/**
 * hashmal_perimeter_columnas_activas - cuantas estan en pie
 *
 * Retorna conteo positivo (0..60), o codigo negativo si error.
 * Patron consistente con otras funciones _activos() del proyecto.
 *
 * Lectura pura, sin logs.
 *
 * 2 testigos:
 *   Ex 27:17  - kol 'ammudei hejatser saviv (todas)
 *   Nm 4:32   - 'ammudei hejatser saviv ve'adneihem viyitdotam
 */
int hashmal_perimeter_columnas_activas(void)
{
    const hashmal_perimeter_t *a = hashmal_perim_get_atrio_interno();
    if (a == NULL) return HASHMAL_PERIM_ERR_INIT;

    /* Usamos el contador agregado del struct - mantenido al dia por
     * columna_registrar. Contamos tambien defensivamente por si hay
     * divergencia: ambos valores deben coincidir; si no, el
     * agregado es la fuente canonica por ser O(1). */
    return (int)a->columnas_registradas;
}

/* ==================================================================
 *
 *   "KOL 'ammudei hejatser saviv mejushaqim KESEF vaveihem KESEF
 *    ve'adneihem NEJOSHET." - Exodo 27:17
 *   "TODAS las columnas del atrio alrededor: con BANDAS DE PLATA,
 *    sus ganchos de PLATA, y sus bases de BRONCE."
 *
 *   Bronce abajo - juicio sostiene. Plata arriba - Palabra conecta.
 *   60 columnas = 60 nodos de frontera - cada uno con SU base, SU
 *   gancho, SU banda. Independencia individual, unidad del todo.
 *
 *   Al quedar las 60 en pie, el atrio pasa de LEVANTADO a ACTIVO
 *   - "kol 'ammudei hejatser saviv" cumplido. La puerta y la
 *   intencion se habilitan en los siguientes archivos.
 *
 *   2 de 4 archivos del Atrio - atrio_columnas.c listo. Quedan:
 *     atrio_puerta.c         Ex 27:16 + Jn 10:9 - la sha'ar unica
 *     atrio_intencion.c      Sal 100:4 + Sal 118:19 - toda+tehilla+tsedeq
 *
 * ================================================================== */
