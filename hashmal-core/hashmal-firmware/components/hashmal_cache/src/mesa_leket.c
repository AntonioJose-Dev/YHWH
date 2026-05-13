/*
 * ===================================================================
 * PIEZA 03 - LA MESA - mesa_leket.c
 * Añadidura 18: LEKET - Espigueo (Reserva para los Debiles)
 * Levitico 19:9-10 + Levitico 23:22 + Deuteronomio 24:19-22
 *                  + Rut 2:1-23 + Santiago 1:27
 * ===================================================================
 *
 * "uvqutsrkhem et qetsir artsekhem LO TEKHALLEH PE'AT sadekha
 *  liqtsor veLEQET qetsirkha LO TELAQQET. ukharmkha lo te'olel
 *  ufret karmkha lo telaqqet; LE'ANI VELAGGER ta'azov otam, ani
 *  YHWH Eloheikhem." - Levitico 19:9-10
 * "Cuando segueis vuestra cosecha, NO acabareis de segar el RINCON
 *  de tu campo, ni el ESPIGUEO de tu siega recogeras... Para el
 *  POBRE y el EXTRANJERO los DEJARAS, yo soy YHWH vuestro Dios."
 *
 * "uvqutsrkhem et qetsir artsekhem lo tekhalleh pe'at sadekha
 *  beqotsrekha veleqet qetsirkha lo telaqqet; le'ani velagger
 *  ta'azov otam, ani YHWH Eloheikhem." - Levitico 23:22
 * (Repeticion en el contexto de las fiestas - el espigueo es
 *  parte del calendario sagrado)
 *
 * "ki tiqtsor qetsirkha vesadekha veSHAKHAJTA 'OMER bassadeh lo
 *  tashuv leqajto LAGGER LAYYATOM VELA'ALMANAH yiheyeh; lema'an
 *  yevarekhekha YHWH Eloheikha bekhol ma'aseh yadeikha." - Dt 24:19
 * "Si segares tu mies en tu campo y OLVIDARES un MANOJO, no
 *  volveras por el; sera para el EXTRANJERO, el HUERFANO y la
 *  VIUDA, para que YHWH te bendiga en toda obra de tus manos."
 *
 * "vatomer Rut hammoaviyah el Naomi: ELKHAH NA HASSADEH VAALAQATAH
 *  VASHIBBOLIM ajar asher emtsa jen be'einav; vatomer lah lekhi
 *  vitti." - Rut 2:2
 * "Rut la moabita dijo a Naomi: TE RUEGO, IRE AL CAMPO Y ESPIGARE
 *  ENTRE LAS ESPIGAS de aquel a cuyos ojos hallare gracia. Ella
 *  le respondio: ve, hija mia."
 *
 * "vegam SHOL TASHOLLU lah min HATSEVATIM va'azavtem velikketah
 *  velo tig'aru bah." - Rut 2:16
 * "Y aun DEJAD CAER A PROPOSITO para ella algo de los MANOJOS,
 *  y dejadlo para que ella lo recoja, y no la riniais."
 *
 * "threskeia kathara kai amiantos para to theo kai patri haute
 *  estin: episkeptesthai orphanous kai cheras en te thlipsei
 *  auton." - Santiago 1:27
 * "La RELIGION pura y sin macha delante de Dios es: VISITAR a
 *  HUERFANOS y VIUDAS en su afliccion." (cumplimiento NT)
 *
 * --- EXEGESIS OPERATIVA ---
 *
 *   pe'ah (H6285)            RINCON / ESQUINA. Lit. "lado, borde."
 *                             La porcion que SE DEJA - no la que se
 *                             toma. Lv 19:9 prohibe SEGARLA, no
 *                             solo cosecharla. Es reserva activa,
 *                             no descuido.
 *
 *   leqet (H3951)            ESPIGAS CAIDAS. Lo que se desprende
 *                             del manojo durante la cosecha. NO se
 *                             recoge. Queda en el suelo para quien
 *                             pueda agacharse.
 *
 *   shijejah (H7911)         "EL OLVIDADO." Dt 24:19. Si olvidas
 *                             un manojo, NO VUELVAS por el.
 *                             Inversion teologica del descuido: lo
 *                             que olvidaste pertenece al que necesita.
 *
 *   ani / ger (H6041 / H1616) POBRE / EXTRANJERO. Los dos primeros
 *                             beneficiarios en Lv 19. ger no es
 *                             pueblo - es el otro que vive entre
 *                             nosotros. Recursos para AMBOS.
 *
 *   yatom / almanah (H3490 / H490) HUERFANO / VIUDA. Dt 24
 *                             ANADE estos a la lista. Cuatro
 *                             categorias completas de vulnerabilidad.
 *
 *   shol tasholu (H7997)     "DEJAD CAER A PROPOSITO." Rut 2:16.
 *                             Boaz NO solo cumple el minimo legal -
 *                             FACILITA activamente. La misericordia
 *                             excede el mandato.
 *
 *   threskeia kathara (G2356+G2513) "RELIGION PURA." Stg 1:27. La
 *                             marca cristiana de pureza no es
 *                             ritual - es atencion al huerfano y la
 *                             viuda. Cumplimiento del leket.
 *
 * --- COMPLEMENTA A MAN (Añadidura 11) - NO SE CONTRADICE ---
 *
 *   Man   (Ex 16):  cuota IGUAL para todos (omer lagulgolet).
 *                    Ni mas ni menos al fuerte que al debil.
 *   Leket (Lv 19):  reserva ADICIONAL para los debiles.
 *                    El fuerte recibe omer; el debil recibe omer
 *                    + acceso al leket.
 *
 * El omer es la JUSTICIA (todos iguales en lo basico). El leket es
 * la MISERICORDIA (los debiles tienen acceso adicional). Ambos
 * coexisten. La economia biblica integra equidad y compasion sin
 * confundirlas.
 *
 * --- 4 TIPOS, 3 FORMAS ---
 *
 *   tipos: ANI (pobre) / GER (extranjero) / YATOM (huerfano) /
 *          ALMANAH (viuda)
 *   formas: PEAH (rincon, % fijo) / LEQET (caidos, oportunista) /
 *           SHIJEJAH (olvidado, no-reclamados)
 *
 * En el firmware fase 1 el caller declara el tipo del receptor.
 * Fase 2 wireara con Meshulash (ALMANAH si <3 vecinos), Goel
 * (YATOM si sin redentor), onboarding (GER si recien consagrado),
 * power (ANI si bateria baja).
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_cache.h         API publica
 *   esp_log.h (ORO EGIPTO)  LKT_LOG_*
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <inttypes.h>

#include "esp_log.h"

#include "hashmal_cache.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *LKT_TAG = "hashmal.mesa.leket";

#define LKT_LOG_I(fmt, ...)  ESP_LOGI(LKT_TAG, fmt, ##__VA_ARGS__)
#define LKT_LOG_W(fmt, ...)  ESP_LOGW(LKT_TAG, fmt, ##__VA_ARGS__)
#define LKT_LOG_E(fmt, ...)  ESP_LOGE(LKT_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * Estado interno
 * ================================================================== */
static uint8_t  g_peah_porciento = HASHMAL_LEKET_PEAH_DEFAULT;
static uint32_t g_recogidos      = 0;
static uint32_t g_facilitados    = 0;
static int64_t  g_timestamp_ms   = 0;

/* ==================================================================
 * Helpers estaticos
 * ================================================================== */

static const char *nombre_tipo(hashmal_leket_tipo_t t)
{
    switch (t) {
    case HASHMAL_LEKET_TIPO_ANI:     return "ANI (Lv 19:10 pobre)";
    case HASHMAL_LEKET_TIPO_GER:     return "GER (Lv 19:10 extranjero)";
    case HASHMAL_LEKET_TIPO_YATOM:   return "YATOM (Dt 24:19 huerfano)";
    case HASHMAL_LEKET_TIPO_ALMANAH: return "ALMANAH (Dt 24:19 viuda)";
    default:                          return "<tipo desconocido>";
    }
}

static bool tipo_legitimo(hashmal_leket_tipo_t t)
{
    return (unsigned)t < HASHMAL_LEKET_TIPOS;
}

/* ==================================================================
 * API - hashmal_leket_es_debil (Lv 19:10 ani / ger)
 * ================================================================== */
bool hashmal_leket_es_debil(hashmal_leket_tipo_t tipo)
{
    /* Fase 1: declaracion externa, siempre true para tipos validos.
     * Fase 2 cruza con Meshulash/Goel/onboarding/power para
     * verificar la condicion real del nodo. */
    return tipo_legitimo(tipo);
}

/* ==================================================================
 * API - hashmal_leket_recoger (Rut 2:2 elkhah na hassadeh)
 * ================================================================== */
int hashmal_leket_recoger(hashmal_leket_tipo_t tipo)
{
    if (!tipo_legitimo(tipo)) {
        LKT_LOG_E("recoger: tipo %d invalido", (int)tipo);
        return HASHMAL_CACHE_ERR_LEKET;
    }

    g_recogidos    += 1u;
    g_timestamp_ms  = (int64_t)g_recogidos;  /* contador como tiempo logico */

    LKT_LOG_I("leket: %s recogio espigas (#%" PRIu32 " total) - "
              "Rut 2:2 elkhah na hassadeh va'alaqatah",
              nombre_tipo(tipo), g_recogidos);
    return HASHMAL_CACHE_OK;
}

/* ==================================================================
 * API - hashmal_leket_facilitar (Rut 2:16 shol tasholu)
 * ================================================================== */
int hashmal_leket_facilitar(hashmal_leket_tipo_t tipo)
{
    if (!tipo_legitimo(tipo)) {
        LKT_LOG_E("facilitar: tipo %d invalido", (int)tipo);
        return HASHMAL_CACHE_ERR_LEKET;
    }

    g_facilitados  += 1u;
    g_timestamp_ms  = (int64_t)(g_recogidos + g_facilitados);

    LKT_LOG_I("leket: facilitado para %s (Rut 2:16 shol tasholu - "
              "Boaz dejo caer a proposito; #%" PRIu32 " total)",
              nombre_tipo(tipo), g_facilitados);
    return HASHMAL_CACHE_OK;
}

/* ==================================================================
 * API - lecturas puras
 * ================================================================== */
uint8_t hashmal_leket_peah_get(void)
{
    return g_peah_porciento;
}

int hashmal_leket_info(hashmal_leket_info_t *out)
{
    if (out == NULL) return HASHMAL_CACHE_ERR_NULL;
    out->peah_porciento     = g_peah_porciento;
    out->recogidos          = g_recogidos;
    out->facilitados        = g_facilitados;
    out->timestamp_ultimo_ms = g_timestamp_ms;
    return HASHMAL_CACHE_OK;
}

/* ==================================================================
 *
 *   "LO TEKHALLEH PE'AT sadekha liqtsor veLEQET qetsirkha LO
 *    TELAQQET; LE'ANI VELAGGER ta'azov otam." - Levitico 19:9-10
 *
 *   "veSHAKHAJTA 'OMER bassadeh lo tashuv leqajto LAGGER LAYYATOM
 *    VELA'ALMANAH yiheyeh." - Deuteronomio 24:19
 *
 *   "ELKHAH NA HASSADEH VAALAQATAH VASHIBBOLIM." - Rut 2:2
 *
 *   "vegam SHOL TASHOLLU lah min HATSEVATIM." - Rut 2:16
 *
 *   "threskeia kathara: episkeptesthai orphanous kai cheras."
 *     - Santiago 1:27
 *
 *   La cosecha completa no es del cosechador. El rincon es de los
 *   pobres; las espigas caidas son de los extranjeros; el manojo
 *   olvidado es del huerfano y la viuda. La economia del Padre
 *   no maximiza extraccion - reserva por diseno. Y Boaz amplio:
 *   no solo cumplio el minimo legal del leket, sino que ORDENO
 *   dejar caer mas a proposito (shol tasholu, Rut 2:16). La
 *   misericordia excede el mandato.
 *
 *   El firmware honra el patron: Man da el omer igual a todos
 *   (justicia); Leket reserva el rincon para los debiles
 *   (misericordia). Las dos no se contradicen, se suman. La red
 *   biblica integra equidad y compasion sin confundirlas.
 *
 * ================================================================== */
