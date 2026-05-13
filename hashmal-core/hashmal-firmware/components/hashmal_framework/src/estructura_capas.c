/**
 * ===================================================================
 * PIEZA 05 - LA ESTRUCTURA - estructura_capas.c
 * Sub-piezas A, B, C - Las 4 capas concentricas (Exodo 26:1-14)
 * ===================================================================
 *
 *   v.1-6   - Sub-pieza A: 10 cortinas de lino (shesh mashzar) con
 *             querubines tejidos (ma'ase joshev).
 *             -> CAPA APLICACION. La mas interna. Guardianes en el tejido.
 *             50 lula'ot tejelet (azul = memoria, Nm 15:38-39)
 *             + 50 qarse zahav (broches de oro) -> "vehaya haMishkan EJAD" (v.6).
 *
 *   v.7-13  - Sub-pieza B: 11 cortinas de pelo de cabra ('izzim).
 *             -> CAPA RED. Mas grande (30 > 28), ASIMETRICA (5 + 6).
 *             La 6a cortina se DOBLA (kafal) al frente - refuerzo
 *             en la entrada (v.9). 50 lazos + 50 qarse nejoshet
 *             (bronce) - "vehaya ejad" (v.11).
 *             Sobrantes: media cortina por detras (v.12),
 *             1 codo por lado para CUBRIR (lejasoto, v.13).
 *
 *   v.14a   - Sub-pieza C1: pieles de carnero tenidas de rojo
 *             ("'orot eilim me'addamim").
 *             -> CAPA TRANSPORTE. Carnero = sacrificio (Gn 22:13,
 *             el carnero que YHWH proveyo en lugar de Isaac).
 *             La red se mueve sobre precio pagado.
 *
 *   v.14b   - Sub-pieza C2: pieles de tajash ENCIMA ("milma'la").
 *             -> CAPA FISICA. Proteccion extrema. Lo que toca el
 *             exterior hostil. "No hermoso por fuera" (Is 53:2) -
 *             la belleza vive dentro.
 *
 * PATRON - cada capa ENVUELVE la anterior:
 *   lino (app)  subset  cabra (net)  subset  carnero (transp.)  subset  tajash (phys.)
 *
 * GRADIENTE DE CONFIANZA (v.6 vs v.11):
 *   interior = ORO    (qarse zahav, v.6)
 *   exterior = BRONCE (qarse nejoshet, v.11)
 *
 * "kemishpato asher horeta bahar" - Ex 26:30
 * ("conforme a su patron, que te fue mostrado en el monte")
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "esp_log.h"           /* ORO DE EGIPTO - encapsulado solo aqui (Ex 12:35-36) */

#include "hashmal_framework.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF - "oro de Egipto" (Ex 12:35-36)
 * Toda llamada a la plataforma vive tras estas macros.
 * La logica biblica del resto del archivo no la conoce.
 * ================================================================== */
static const char *CAPAS_TAG = "hashmal.capas";

#define CAPAS_LOG_I(fmt, ...)  ESP_LOGI(CAPAS_TAG, fmt, ##__VA_ARGS__)
#define CAPAS_LOG_W(fmt, ...)  ESP_LOGW(CAPAS_TAG, fmt, ##__VA_ARGS__)
#define CAPAS_LOG_E(fmt, ...)  ESP_LOGE(CAPAS_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * API INTERNA DEL MODULO
 *
 * Estas funciones seran consumidas por estructura_ejad.c (el
 * orquestador). Cuando lo construyamos, las prototiparemos en
 * hashmal_framework_internal.h. Por ahora quedan declaradas aqui
 * con enlace externo (sin `static`) - estables y auditables.
 * ================================================================== */

/**
 * Levanta las 4 capas conforme a Ex 26:1-14.
 * @param capas  Array de HASHMAL_CAPAS_TOTAL elementos a inicializar.
 * @return 0 en exito, -1 si el puntero es NULL.
 */
int hashmal_capas_init(hashmal_capa_t capas[HASHMAL_CAPAS_TOTAL]);

/**
 * Verifica invariantes biblicas del conjunto de capas:
 *   v.1  - lino tiene querubines + ma'ase joshev
 *   v.3  - grupo_a + grupo_b = cortinas (lino)
 *   v.6  - lino: 50 lazos == 50 broches (oro)    -> EJAD
 *   v.8  - cabra cubre mas que lino (30 > 28)
 *   v.9  - cabra asimetrica (5 + 6) + kafal al frente
 *   v.11 - cabra: 50 lazos == 50 broches (bronce) -> ejad
 *   v.14 - carnero y tajash: 1 mijseh cada uno, sin querubines
 *   gradiente - interior=ORO / exterior=BRONCE
 * @return true si el conjunto respeta el patron del monte (v.30).
 */
bool hashmal_capas_verificar(const hashmal_capa_t capas[HASHMAL_CAPAS_TOTAL]);

/**
 * Nombre hebreo legible de la capa (para logs y depuracion).
 * Hablamos en el idioma del Padre.
 */
const char* hashmal_capas_nombre(hashmal_capa_id_t id);

/* ==================================================================
 * CONSTRUCTORES POR SUB-PIEZA
 * Una funcion por capa; cada linea apunta al versiculo que la manda.
 * ================================================================== */

/**
 * Sub-pieza A - Capa LINO (aplicacion)
 * Fuente: Exodo 26:1-6
 *
 * v.1  - "yeri'ot shesh mashzar utejelet ve'argaman vetola'at shani
 *         keruvim ma'ase joshev ta'ase otam"
 *        10 cortinas de lino fino + 4 colores + querubines, obra de
 *        artesano experto (joshev).
 * v.2  - "shemone ve'esrim ba'amma... arba' ba'amma" - 28 x 4 codos.
 *        "midda ajat lejol hayeri'ot" - UNA medida para TODAS.
 * v.3  - "jovrot isha el ajota" - mujer a su hermana: 2 grupos de 5.
 * v.4-5 - "lula'ot tejelet" - 50 lazadas de AZUL (tejelet).
 *        Azul = recordar los mandamientos (Nm 15:38-39). Los puntos
 *        de conexion estan hechos de MEMORIA. "maqbilot" - alineadas.
 * v.6  - "jamishim qarse zahav... vehaya haMishkan EJAD" - 50 broches
 *        de ORO y el Mishkan sera UNO. 50 = Jubileo (Lv 25:10).
 */
static void construir_lino(hashmal_capa_t *c)
{
    memset(c, 0, sizeof(*c));
    c->id            = HASHMAL_CAPA_LINO;
    c->cortinas      = HASHMAL_CORTINAS_INTERIOR;        /* v.1  - 10 yeri'ot */
    c->largo_codos   = HASHMAL_CORTINA_INTERIOR_LARGO;   /* v.2  - 28 amma */
    c->ancho_codos   = HASHMAL_CORTINA_INTERIOR_ANCHO;   /* v.2  - 4 amma */
    c->grupo_a       = HASHMAL_GRUPO_INTERIOR_A;         /* v.3  - 5 */
    c->grupo_b       = HASHMAL_GRUPO_INTERIOR_B;         /* v.3  - 5 */
    c->lazos         = HASHMAL_LAZOS_TEJELET;            /* v.4-5 - 50 azul */
    c->broches       = HASHMAL_QARSE_ZAHAV;              /* v.6  - 50 oro */
    c->nivel_broches = HASHMAL_CONFIANZA_ORO;            /* v.6  - zahav */
    c->tipo_obra     = HASHMAL_MAASE_JOSHEV;             /* v.1  - joshev */
    c->querubines    = true;                             /* v.1  - keruvim */
    c->kafal         = false;                            /* v.9  - no aplica */
    c->gavanim[HASHMAL_GAVEN_TEJELET]      = true;       /* v.1  - tejelet (azul, Nm 15:38-39) */
    c->gavanim[HASHMAL_GAVEN_ARGAMAN]      = true;       /* v.1  - argaman (purpura, realeza) */
    c->gavanim[HASHMAL_GAVEN_TOLAAT_SHANI] = true;       /* v.1  - tola'at shani (carmesi, Is 1:18) */
    c->gavanim[HASHMAL_GAVEN_SHESH]        = true;       /* v.1  - shesh mashzar (lino, Ap 19:8) */
    /* meaddamim = false - v.14a aplica solo al carnero; memset ya lo dejo false. */
}

/**
 * Sub-pieza B - Capa CABRA (red)
 * Fuente: Exodo 26:7-13
 *
 * v.7  - "yeri'ot 'izzim le'ohel 'al haMishkan 'ashtei 'esre yeri'ot"
 *        11 cortinas de pelo de cabra - tienda SOBRE el mishkan.
 *        11 es IMPAR: no se parte en mitades perfectas.
 * v.8  - "sheloshim ba'amma... arba' ba'amma" - 30 x 4 codos.
 *        Mas largas que el lino (30 > 28) -> CUBREN mas.
 * v.9  - "jamesh... shesh... vekafalta et hayeri'a hashishit
 *         el mul pene ha'ohel"
 *        5 + 6 (ASIMETRICO). La 6a se DOBLA al frente (kafal) -
 *        refuerzo donde mas se necesita: la entrada.
 * v.10 - 50 lula'ot en cada borde.
 * v.11 - "jamishim qarse nejoshet... vehaya ejad"
 *        50 broches de BRONCE - mismo numero, material inferior.
 *        Tambien la tienda es UNA.
 * v.12-13 - Sobrantes: media cortina cuelga por detras (v.12);
 *        1 codo a cada lado para CUBRIR (lejasoto, v.13).
 *
 * NOTA - La Palabra SILENCIA el tipo de obra aqui. Por ausencia de
 * querubines (contraste con v.1,31) asignamos HASHMAL_MAASE_ROQEM
 * como inferencia conservadora, NO como afirmacion.
 */
static void construir_cabra(hashmal_capa_t *c)
{
    memset(c, 0, sizeof(*c));
    c->id            = HASHMAL_CAPA_CABRA;
    c->cortinas      = HASHMAL_CORTINAS_EXTERIOR;        /* v.7  - 11 yeri'ot */
    c->largo_codos   = HASHMAL_CORTINA_EXTERIOR_LARGO;   /* v.8  - 30 amma */
    c->ancho_codos   = HASHMAL_CORTINA_EXTERIOR_ANCHO;   /* v.8  - 4 amma */
    c->grupo_a       = HASHMAL_GRUPO_EXTERIOR_A;         /* v.9  - 5 */
    c->grupo_b       = HASHMAL_GRUPO_EXTERIOR_B;         /* v.9  - 6 */
    c->lazos         = HASHMAL_LAZOS_EXTERIOR;           /* v.10 - 50 */
    c->broches       = HASHMAL_QARSE_NEJOSHET;           /* v.11 - 50 bronce */
    c->nivel_broches = HASHMAL_CONFIANZA_BRONCE;         /* v.11 - nejoshet */
    c->tipo_obra     = HASHMAL_MAASE_ROQEM;              /* v.7 silente; inferido (sin keruvim) */
    c->querubines    = false;                            /* v.7-13 - sin querubines */
    c->kafal         = true;                             /* v.9  - la 6a se dobla */
    /* v.7 CALLA sobre colores - solo dice 'izzim. memset deja los 4 gavanim en false. */
    /* meaddamim = false - v.14a solo carnero; memset ya lo dejo false. */
}

/**
 * Sub-pieza C1 - Capa CARNERO (transporte)
 * Fuente: Exodo 26:14a - "'orot eilim me'addamim"
 *
 * Carnero = sacrificio (Gn 22:13 - "un carnero trabado... en lugar
 * de su hijo"). La red se MUEVE sobre precio pagado: ningun paquete
 * atraviesa esta capa sin pasar por sacrificio.
 *
 * v.14a SILENCIA dimensiones, lazos y broches. Representamos 1
 * mijseh (cubierta unica) y dejamos los campos de yeri'ot en cero
 * - esos pertenecen a v.1-13, no aqui.
 */
static void construir_carnero(hashmal_capa_t *c)
{
    memset(c, 0, sizeof(*c));
    c->id            = HASHMAL_CAPA_CARNERO;
    c->cortinas      = 1;                                /* v.14a - 1 mijseh */
    c->largo_codos   = 0;                                /* v.14a silente */
    c->ancho_codos   = 0;                                /* v.14a silente */
    c->grupo_a       = 0;                                /* no hay grupos */
    c->grupo_b       = 0;
    c->lazos         = 0;                                /* sin lula'ot */
    c->broches       = 0;                                /* sin qerasim */
    c->nivel_broches = HASHMAL_CONFIANZA_BRONCE;         /* exterior -> basico */
    c->tipo_obra     = HASHMAL_MAASE_ROQEM;              /* v.14 silente; sin keruvim */
    c->querubines    = false;
    c->kafal         = false;
    /* v.14a CALLA sobre los 4 gavanim - memset deja todos en false. */
    c->meaddamim     = true;                             /* v.14a - 'orot eilim me'addamim (Gn 22:13) */
}

/**
 * Sub-pieza C2 - Capa TAJASH (fisica)
 * Fuente: Exodo 26:14b - "'orot tejashim milma'la"
 *
 * La palabra "milma'la" ("por encima") FIJA esta capa como la MAS
 * exterior - la que toca el mundo hostil directamente. Tajash =
 * proteccion extrema. "No hay parecer en el" (Is 53:2) - la belleza
 * vive dentro, no fuera.
 *
 * Mismo silencio escritural que carnero: 1 mijseh, sin dimensiones,
 * sin lazos ni broches.
 */
static void construir_tajash(hashmal_capa_t *c)
{
    memset(c, 0, sizeof(*c));
    c->id            = HASHMAL_CAPA_TAJASH;
    c->cortinas      = 1;                                /* v.14b - 1 mijseh milma'la */
    c->largo_codos   = 0;                                /* v.14b silente */
    c->ancho_codos   = 0;
    c->grupo_a       = 0;
    c->grupo_b       = 0;
    c->lazos         = 0;
    c->broches       = 0;
    c->nivel_broches = HASHMAL_CONFIANZA_BRONCE;         /* exterior extremo */
    c->tipo_obra     = HASHMAL_MAASE_ROQEM;
    c->querubines    = false;
    c->kafal         = false;
    /* v.14b CALLA sobre colores y sobre me'addamim - memset ya dejo todo en false. */
}

/* ==================================================================
 * API INTERNA - IMPLEMENTACION
 * ================================================================== */

int hashmal_capas_init(hashmal_capa_t capas[HASHMAL_CAPAS_TOTAL])
{
    if (capas == NULL) {
        CAPAS_LOG_E("init: puntero NULL");
        return -1;
    }

    /* Orden estricto: interior -> exterior (v.1 -> v.14b).
     * Cada capa envuelve a la anterior; no es arbitrario. */
    construir_lino   (&capas[HASHMAL_CAPA_LINO]);     /* v.1-6  */
    construir_cabra  (&capas[HASHMAL_CAPA_CABRA]);    /* v.7-13 */
    construir_carnero(&capas[HASHMAL_CAPA_CARNERO]);  /* v.14a  */
    construir_tajash (&capas[HASHMAL_CAPA_TAJASH]);   /* v.14b  */

    CAPAS_LOG_I("4 capas levantadas (Ex 26:1-14): shésh → ʻizzím → eilím → tejashím");
    return 0;
}

bool hashmal_capas_verificar(const hashmal_capa_t capas[HASHMAL_CAPAS_TOTAL])
{
    if (capas == NULL) {
        CAPAS_LOG_E("verificar: puntero NULL");
        return false;
    }

    const hashmal_capa_t *lino    = &capas[HASHMAL_CAPA_LINO];
    const hashmal_capa_t *cabra   = &capas[HASHMAL_CAPA_CABRA];
    const hashmal_capa_t *carnero = &capas[HASHMAL_CAPA_CARNERO];
    const hashmal_capa_t *tajash  = &capas[HASHMAL_CAPA_TAJASH];

    /* v.1 - lino: querubines + ma'ase joshev (obligatorio) */
    if (!lino->querubines || lino->tipo_obra != HASHMAL_MAASE_JOSHEV) {
        CAPAS_LOG_E("v.1 violado: lino exige keruvím + maʻasé joshév");
        return false;
    }

    /* v.1 - lino: los 4 gavanim deben estar presentes (tejelet,
     * argaman, tola'at shani, shesh - nombrados explicitamente). */
    if (!lino->gavanim[HASHMAL_GAVEN_TEJELET]      ||
        !lino->gavanim[HASHMAL_GAVEN_ARGAMAN]      ||
        !lino->gavanim[HASHMAL_GAVEN_TOLAAT_SHANI] ||
        !lino->gavanim[HASHMAL_GAVEN_SHESH]) {
        CAPAS_LOG_E("v.1 violado: lino exige 4 gavaním "
                    "(tejélet, argamán, tolaʻat shaní, shésh)");
        return false;
    }

    /* v.14a - me'addamim solo sobre el carnero (Gn 22:13). Ninguna
     * otra capa puede llevarlo; carnero lo debe llevar. */
    if (!carnero->meaddamim ||
        lino->meaddamim  ||
        cabra->meaddamim ||
        tajash->meaddamim) {
        CAPAS_LOG_E("v.14a violado: solo carnero lleva meʼaddamím");
        return false;
    }

    /* v.3 - grupo_a + grupo_b = cortinas (10 = 5+5) */
    if (lino->grupo_a + lino->grupo_b != lino->cortinas) {
        CAPAS_LOG_E("v.3 violado: grupos(%u+%u) != cortinas(%u) en lino",
                    lino->grupo_a, lino->grupo_b, lino->cortinas);
        return false;
    }

    /* v.6 - 50 lazos == 50 broches de ORO -> EJAD */
    if (lino->nivel_broches != HASHMAL_CONFIANZA_ORO ||
        lino->broches      != lino->lazos          ||
        lino->broches      != HASHMAL_QARSE_ZAHAV) {
        CAPAS_LOG_E("v.6 violado: 50 lulaʼót tejélet ≡ 50 qarsé zaháv → ejád");
        return false;
    }

    /* v.8 - cabra cubre mas que lino (largo estrictamente mayor) */
    if (cabra->largo_codos <= lino->largo_codos) {
        CAPAS_LOG_E("v.8 violado: cabra(%u) debe superar a lino(%u)",
                    cabra->largo_codos, lino->largo_codos);
        return false;
    }

    /* v.9 - asimetria 5+6 y la 6a doblada al frente */
    if (cabra->grupo_a == cabra->grupo_b) {
        CAPAS_LOG_E("v.9 violado: cabra debe ser ASIMÉTRICA (5+6)");
        return false;
    }
    if (!cabra->kafal) {
        CAPAS_LOG_E("v.9 violado: falta kafál — la 6ª se dobla al frente");
        return false;
    }
    if (cabra->grupo_a + cabra->grupo_b != cabra->cortinas) {
        CAPAS_LOG_E("v.9 violado: grupos(%u+%u) != cortinas(%u) en cabra",
                    cabra->grupo_a, cabra->grupo_b, cabra->cortinas);
        return false;
    }

    /* v.11 - 50 lazos == 50 broches de BRONCE -> ejad */
    if (cabra->nivel_broches != HASHMAL_CONFIANZA_BRONCE ||
        cabra->broches      != cabra->lazos             ||
        cabra->broches      != HASHMAL_QARSE_NEJOSHET) {
        CAPAS_LOG_E("v.11 violado: 50 lulaʼót ≡ 50 qarsé nejóshet → ejád");
        return false;
    }

    /* v.14a - carnero: 1 mijseh sin querubines, capa exterior */
    if (carnero->cortinas != 1 || carnero->querubines) {
        CAPAS_LOG_E("v.14a violado: carnero = 1 mijseh, sin keruvím");
        return false;
    }

    /* v.14b - tajash: 1 mijseh "milma'la" (la mas exterior) */
    if (tajash->cortinas != 1 || tajash->querubines) {
        CAPAS_LOG_E("v.14b violado: tajash = 1 mijseh milmaʻlá, sin keruvím");
        return false;
    }

    /* Gradiente biblico: interior=ORO (v.6) / exterior=BRONCE (v.11) */
    if (lino->nivel_broches  != HASHMAL_CONFIANZA_ORO ||
        cabra->nivel_broches != HASHMAL_CONFIANZA_BRONCE) {
        CAPAS_LOG_E("gradiente violado: interior debe ser oro, exterior bronce");
        return false;
    }

    CAPAS_LOG_I("4 capas conformes al patrón del monte (Ex 26:30)");
    return true;
}

const char* hashmal_capas_nombre(hashmal_capa_id_t id)
{
    switch (id) {
        case HASHMAL_CAPA_LINO:    return "shésh (lino)";      /* v.1  */
        case HASHMAL_CAPA_CABRA:   return "ʻizzím (cabra)";    /* v.7  */
        case HASHMAL_CAPA_CARNERO: return "eilím (carnero)";   /* v.14a */
        case HASHMAL_CAPA_TAJASH:  return "tejashím (tajash)"; /* v.14b */
        default:                   return "(desconocida)";
    }
}
