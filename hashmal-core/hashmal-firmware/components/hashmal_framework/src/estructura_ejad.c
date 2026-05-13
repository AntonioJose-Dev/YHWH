/**
 * ===================================================================
 * PIEZA 05 - LA ESTRUCTURA - estructura_ejad.c
 * EL ORQUESTADOR - los 50 broches de oro (v.6) que hacen EJAD
 * ===================================================================
 *
 *   "jamishim qarse zahav vejibarrta et hayeri'ot isha el ajota
 *    baqerasim vehaya haMishkan EJAD" - Exodo 26:6
 *   "50 broches de oro y uniras las cortinas una a otra con los broches,
 *    y el Tabernaculo sera UNO."
 *
 *   "vahaqemota et hamishkan kemishpato asher horeta bahar" - Exodo 26:30
 *   "Levantaras el Mishkan conforme a su patron, mostrado en el monte."
 *
 *   "bayom hajodesh harishon be'ejad lajodesh taqim
 *    et mishkan ohel mo'ed" - Exodo 40:2
 *   "El dia primero del mes primero levantaras el Mishkan, la Tienda
 *    de Reunion."
 *
 * --- LO QUE HACE ESTE ARCHIVO ---
 *
 * Toma las 4 piezas hermanas (capas, tablas, barras, velo+masaj) y
 * las une en UN SOLO tabernaculo. Este archivo es los 50 broches del
 * v.6 traducidos a codigo: sin el hay cortinas sueltas; con el hay
 * Mishkan.
 *
 * Aqui vive el estado global del hashmal_mishkan_t y se implementa
 * toda la API publica declarada en hashmal_framework.h, excepto
 * hashmal_framework_confianza_de() que nace en estructura_velo.c
 * (donde se establece el gradiente bronce->plata->oro).
 *
 * --- ORDEN DE LEVANTAMIENTO (Ex 40:17-33) ---
 *
 *   El Torah PROCLAMA las piezas en un orden (Ex 26:1-37 - cortinas,
 *   tablas, barras, velo, entrada) pero las ENSAMBLA en otro (Ex
 *   40:17-33 - bases, tablas, barras, columnas, luego la tienda
 *   encima). Esta funcion sigue el orden de ENSAMBLAJE:
 *
 *     1. hashmal_tablas_init    (v.40:18 - qerashim + bases)
 *     2. hashmal_barras_init    (v.40:18 - berijim)
 *     3. hashmal_velo_init      (v.40:18 - columnas del velo y masaj)
 *     4. hashmal_capas_init     (v.40:19 - "extendio la tienda")
 *
 *   La ACTIVACION del velo (hivdil, v.33), la cobertura del masaj
 *   (v.36) y la colocacion de aron/kaporet/mesa/menora (v.33-35)
 *   son pasos SEPARADOS - Exodo 40 los ordena DESPUES de levantar
 *   la estructura. El llamador los invoca explicitamente.
 *
 * --- CONCURRENCIA ---
 *
 *   Disenado para inicializacion en boot y operacion desde una tarea
 *   coordinadora unica. No hay mutex interno; si mas adelante se
 *   accede desde multiples tareas, envolver el estado global con
 *   una seccion critica.
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "esp_log.h"                /* ORO DE EGIPTO - encapsulado solo aqui */

#include "hashmal_framework.h"
#include "hashmal_framework_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *EJAD_TAG = "hashmal.ejad";

#define EJAD_LOG_I(fmt, ...)  ESP_LOGI(EJAD_TAG, fmt, ##__VA_ARGS__)
#define EJAD_LOG_W(fmt, ...)  ESP_LOGW(EJAD_TAG, fmt, ##__VA_ARGS__)
#define EJAD_LOG_E(fmt, ...)  ESP_LOGE(EJAD_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * ESTADO GLOBAL DEL MISHKAN
 *
 * Memoria estatica. No hay asignacion dinamica en el framework - la
 * estructura del Padre no pide malloc.
 * ================================================================== */
static hashmal_mishkan_t s_mishkan;
static bool              s_inicializado = false;

/* ==================================================================
 * HELPERS
 * ================================================================== */

/**
 * Convierte (lado, indice) a ID global de tabla.
 * Debe coincidir con el layout usado por estructura_tablas.c:
 *   sur   -> [0, TABLAS_SUR)
 *   norte -> [TABLAS_SUR, TABLAS_SUR+TABLAS_NORTE)
 *   oeste -> [TABLAS_SUR+TABLAS_NORTE, TABLAS_TOTAL)
 * @return true si el (lado, indice) es valido; id escrito en *id_out.
 */
static bool tabla_id_desde(hashmal_lado_t lado, uint8_t indice, uint8_t *id_out)
{
    if (id_out == NULL) return false;
    switch (lado) {
        case HASHMAL_LADO_SUR:
            if (indice >= HASHMAL_TABLAS_SUR) return false;
            *id_out = indice;
            return true;
        case HASHMAL_LADO_NORTE:
            if (indice >= HASHMAL_TABLAS_NORTE) return false;
            *id_out = (uint8_t)(HASHMAL_TABLAS_SUR + indice);
            return true;
        case HASHMAL_LADO_OESTE:
            if (indice >= HASHMAL_TABLAS_OESTE) return false;
            *id_out = (uint8_t)(HASHMAL_TABLAS_SUR + HASHMAL_TABLAS_NORTE + indice);
            return true;
        default:
            return false;
    }
}

/**
 * Devuelve el array de 5 berijim del lado indicado.
 * @return puntero al array, o NULL si el lado no tiene tablas (este abierto).
 */
static hashmal_beriaj_t* barras_del_lado(hashmal_lado_t lado)
{
    switch (lado) {
        case HASHMAL_LADO_SUR:   return s_mishkan.barras_sur;
        case HASHMAL_LADO_NORTE: return s_mishkan.barras_norte;
        case HASHMAL_LADO_OESTE: return s_mishkan.barras_oeste;
        default:                 return NULL;
    }
}

/* ==================================================================
 * API PUBLICA - IMPLEMENTACION
 * ================================================================== */

/**
 * hashmal_framework_init - Exodo 40:2
 *
 * "bayom hajodesh harishon be'ejad lajodesh taqim et mishkan"
 * "El primer dia del primer mes levantaras el Mishkan."
 *
 * Sigue Ex 40:17-33: frame primero (tablas, barras, columnas), tienda
 * despues (capas). Velo y masaj quedan CONFECCIONADOS pero NO activos
 * - su activacion es paso aparte (Ex 40:21, 28), correspondiendo a
 * hashmal_framework_activar_velo / _activar_masaj.
 */
int hashmal_framework_init(void)
{
    if (s_inicializado) {
        EJAD_LOG_W("init: ya inicializado (no-op)");
        return 0;
    }

    memset(&s_mishkan, 0, sizeof(s_mishkan));

    /* Ex 40:18 - bases, qerashim, barras, columnas */
    if (hashmal_tablas_init(s_mishkan.tablas) != 0) {
        EJAD_LOG_E("init paso 1 falló: tablas (v.15-25)");
        return -1;
    }
    if (hashmal_barras_init(s_mishkan.barras_sur,
                            s_mishkan.barras_norte,
                            s_mishkan.barras_oeste) != 0) {
        EJAD_LOG_E("init paso 2 falló: barras (v.26-29)");
        return -1;
    }
    if (hashmal_velo_init(&s_mishkan.velo, &s_mishkan.masaj) != 0) {
        EJAD_LOG_E("init paso 3 falló: velo + masaj (v.31-37)");
        return -1;
    }

    /* Ex 40:19 - "extendio la tienda sobre el mishkan" */
    if (hashmal_capas_init(s_mishkan.capas) != 0) {
        EJAD_LOG_E("init paso 4 falló: capas (v.1-14)");
        return -1;
    }

    /* Verificacion estructural inmediata */
    if (!hashmal_tablas_verificar(s_mishkan.tablas)) {
        EJAD_LOG_E("init: tablas no pasan verificación");
        return -1;
    }
    if (!hashmal_barras_verificar(s_mishkan.barras_sur,
                                  s_mishkan.barras_norte,
                                  s_mishkan.barras_oeste)) {
        EJAD_LOG_E("init: barras no pasan verificación");
        return -1;
    }
    if (!hashmal_velo_verificar (&s_mishkan.velo))  {
        EJAD_LOG_E("init: parójet no pasa verificación");
        return -1;
    }
    if (!hashmal_masaj_verificar(&s_mishkan.masaj)) {
        EJAD_LOG_E("init: masaj no pasa verificación");
        return -1;
    }
    if (!hashmal_membranas_gradiente(&s_mishkan.velo, &s_mishkan.masaj)) {
        EJAD_LOG_E("init: gradiente bronce→plata→oro violado");
        return -1;
    }
    if (!hashmal_capas_verificar(s_mishkan.capas)) {
        EJAD_LOG_E("init: capas no pasan verificación");
        return -1;
    }

    /* Estado inicial: estructura levantada; velo/masaj inactivos;
     * aron/kaporet/mesa/menora sin colocar (se hace en pasos siguientes). */
    s_mishkan.aron_colocado    = false;
    s_mishkan.kaporet_colocada = false;
    s_mishkan.shuljan_colocado = false;
    s_mishkan.menorah_colocada = false;
    s_mishkan.ejad             = false;     /* aun no: falta activacion + colocacion */

    s_inicializado = true;

    EJAD_LOG_I("═══ MISHKÁN LEVANTADO ═══ (Éx 40:2)");
    EJAD_LOG_I("  48 qerashím + 96 adné késef + 15 berijím (3 hatijones)");
    EJAD_LOG_I("  4 capas: shésh → ʻizzím → eilím → tejashím");
    EJAD_LOG_I("  velo y masaj confeccionados (inactivos hasta el hivdíl)");
    EJAD_LOG_I("  faltan: activar_velo, activar_masaj, colocar arón/kapóret/mesa/menorá");
    return 0;
}

void hashmal_framework_deinit(void)
{
    if (!s_inicializado) {
        EJAD_LOG_W("deinit: no estaba inicializado (no-op)");
        return;
    }
    memset(&s_mishkan, 0, sizeof(s_mishkan));
    s_inicializado = false;
    EJAD_LOG_I("Mishkán desmontado — estado cero");
}

const hashmal_mishkan_t* hashmal_framework_get_mishkan(void)
{
    if (!s_inicializado) return NULL;
    return &s_mishkan;
}

int hashmal_framework_registrar_tabla(uint8_t lado, uint8_t indice)
{
    if (!s_inicializado) {
        EJAD_LOG_E("registrar_tabla: framework no inicializado");
        return -1;
    }
    uint8_t id = 0u;
    if (!tabla_id_desde((hashmal_lado_t)lado, indice, &id)) {
        EJAD_LOG_E("registrar_tabla: (lado=%u, indice=%u) fuera de rango", lado, indice);
        return -1;
    }

    const hashmal_qeresh_t *t = &s_mishkan.tablas[id];
    if (t->id != id || (hashmal_lado_t)lado != t->lado) {
        EJAD_LOG_E("registrar_tabla: id/lado inconsistente en %u", id);
        return -1;
    }
    if (!t->omedim) {
        EJAD_LOG_E("v.15 violado: qéresh %u no está de pie", id);
        return -1;
    }

    EJAD_LOG_I("qéresh %u registrado en %s[%u] — ʻomedím (v.15)",
               id, hashmal_tablas_lado_nombre(t->lado), indice);
    return 0;
}

int hashmal_framework_conectar_barras(uint8_t lado)
{
    if (!s_inicializado) {
        EJAD_LOG_E("conectar_barras: framework no inicializado");
        return -1;
    }
    hashmal_beriaj_t *grupo = barras_del_lado((hashmal_lado_t)lado);
    if (grupo == NULL) {
        EJAD_LOG_E("conectar_barras: lado %u sin barras (este abierto)", lado);
        return -1;
    }

    /* Verifica el lado completo: 5 barras, 1 hatijon extremo-a-extremo. */
    uint8_t backbones = 0u;
    for (uint8_t p = 0; p < HASHMAL_BARRAS_POR_LADO; ++p) {
        if (grupo[p].lado != (hashmal_lado_t)lado) {
            EJAD_LOG_E("conectar_barras: pos %u lado inconsistente", p);
            return -1;
        }
        if (grupo[p].es_backbone) backbones++;
    }
    if (backbones != HASHMAL_BACKBONE) {
        EJAD_LOG_E("v.28 violado: lado %u tiene %u hatijones (esperado %d)",
                   lado, backbones, HASHMAL_BACKBONE);
        return -1;
    }

    EJAD_LOG_I("barras conectadas en %s: 5 berijím + 1 hatijón (v.26-28)",
               hashmal_tablas_lado_nombre((hashmal_lado_t)lado));
    return 0;
}

int hashmal_framework_activar_velo(void)
{
    if (!s_inicializado) {
        EJAD_LOG_E("activar_velo: framework no inicializado");
        return -1;
    }
    return hashmal_velo_activar(&s_mishkan.velo);
}

int hashmal_framework_activar_masaj(void)
{
    if (!s_inicializado) {
        EJAD_LOG_E("activar_masaj: framework no inicializado");
        return -1;
    }
    return hashmal_masaj_activar(&s_mishkan.masaj);
}

int hashmal_framework_colocar(hashmal_miqom_t miqom)
{
    if (!s_inicializado) {
        EJAD_LOG_E("colocar: framework no inicializado");
        return -1;
    }

    switch (miqom) {

        case HASHMAL_MIQOM_ARON:
            /* v.33 - "veheveita shamah mibeit laparojet et aron ha'edut":
             * el Arca va DENTRO del velo -> el velo debe estar ya activo. */
            if (!s_mishkan.velo.activo) {
                EJAD_LOG_E("v.33 violado: Arca antes del hivdíl (activa el velo primero)");
                return -1;
            }
            s_mishkan.aron_colocado = true;
            EJAD_LOG_I("Arca colocada mibéit laparójet (v.33) — qódesh haqodashím");
            return 0;

        case HASHMAL_MIQOM_KAPORET:
            /* v.34 - "venatata et hakkaporet 'al aron ha'edut":
             * la Kaporet va SOBRE el Arca -> el Arca debe estar colocada. */
            if (!s_mishkan.aron_colocado) {
                EJAD_LOG_E("v.34 violado: Kapóret sin Arca debajo");
                return -1;
            }
            s_mishkan.kaporet_colocada = true;
            EJAD_LOG_I("Kapóret colocada sobre el Arca (v.34) — encuentro sobre el Testimonio");
            return 0;

        case HASHMAL_MIQOM_SHULJAN: {
            /* v.35 - "vesamta et hashuljan mijuts laparojet... 'al tsela' tsafon":
             * la Mesa va FUERA del velo -> el velo debe estar activo (para que
             * "mijuts laparojet" tenga sentido). */
            if (!s_mishkan.velo.activo) {
                EJAD_LOG_E("v.35 violado: Mesa antes del hivdíl (activa el velo primero)");
                return -1;
            }
            hashmal_lado_t lado = HASHMAL_LADO_SUR;
            if (!hashmal_miqom_lado(HASHMAL_MIQOM_SHULJAN, &lado) ||
                 lado != HASHMAL_SHULJAN_LADO) {
                EJAD_LOG_E("v.35 violado: Mesa debe ir al norte (tsafón)");
                return -1;
            }
            s_mishkan.shuljan_colocado = true;
            EJAD_LOG_I("Mesa colocada mijúts laparójet, tsélaʻ tsafón (v.35) — lado norte");
            return 0;
        }

        case HASHMAL_MIQOM_MENORAH: {
            /* v.35 - "ve'et hammenora nojaj hashuljan... teimanah":
             * la Menora FRENTE a la Mesa - ella va primero. */
            if (!s_mishkan.velo.activo) {
                EJAD_LOG_E("v.35 violado: Menorá antes del hivdíl (activa el velo primero)");
                return -1;
            }
            if (!s_mishkan.shuljan_colocado) {
                EJAD_LOG_E("v.35 violado: Menorá sin Mesa — debe ir nójaj hashulján");
                return -1;
            }
            hashmal_lado_t lado = HASHMAL_LADO_NORTE;
            if (!hashmal_miqom_lado(HASHMAL_MIQOM_MENORAH, &lado) ||
                 lado != HASHMAL_MENORAH_LADO) {
                EJAD_LOG_E("v.35 violado: Menorá debe ir al sur (teimánah)");
                return -1;
            }
            s_mishkan.menorah_colocada = true;
            EJAD_LOG_I("Menorá colocada nójaj hashulján, tsélaʻ teimánah (v.35) — lado sur");
            return 0;
        }

        default:
            EJAD_LOG_E("colocar: miqom desconocido (%d)", (int)miqom);
            return -1;
    }
}

bool hashmal_framework_es_ejad(void)
{
    if (!s_inicializado) {
        EJAD_LOG_E("es_ejad: framework no inicializado");
        return false;
    }

    /* 1. Estructura - cuatro sub-piezas pasan sus invariantes */
    if (!hashmal_capas_verificar(s_mishkan.capas)) {
        EJAD_LOG_E("es_ejad: capas (v.1-14) fallan");       return false;
    }
    if (!hashmal_tablas_verificar(s_mishkan.tablas)) {
        EJAD_LOG_E("es_ejad: tablas (v.15-25) fallan");     return false;
    }
    if (!hashmal_barras_verificar(s_mishkan.barras_sur,
                                  s_mishkan.barras_norte,
                                  s_mishkan.barras_oeste)) {
        EJAD_LOG_E("es_ejad: barras (v.26-29) fallan");     return false;
    }
    if (!hashmal_velo_verificar (&s_mishkan.velo))  {
        EJAD_LOG_E("es_ejad: parójet (v.31-33) falla");     return false;
    }
    if (!hashmal_masaj_verificar(&s_mishkan.masaj)) {
        EJAD_LOG_E("es_ejad: masaj (v.36-37) falla");       return false;
    }
    if (!hashmal_membranas_gradiente(&s_mishkan.velo, &s_mishkan.masaj)) {
        EJAD_LOG_E("es_ejad: gradiente bronce→plata violado");
        return false;
    }

    /* 2. Activacion - hivdil en efecto (v.33) y masaj colgado (v.36) */
    if (!s_mishkan.velo.activo) {
        EJAD_LOG_E("es_ejad: hivdíl no activo (v.33) — falta activar_velo");
        return false;
    }
    if (!s_mishkan.masaj.activo) {
        EJAD_LOG_E("es_ejad: masaj no colgado (v.36) — falta activar_masaj");
        return false;
    }

    /* 3. Colocacion - v.33-35 */
    if (!s_mishkan.aron_colocado) {
        EJAD_LOG_E("es_ejad: Arca no colocada (v.33)");       return false;
    }
    if (!s_mishkan.kaporet_colocada) {
        EJAD_LOG_E("es_ejad: Kapóret no colocada (v.34)");    return false;
    }
    if (!s_mishkan.shuljan_colocado) {
        EJAD_LOG_E("es_ejad: Mesa no colocada (v.35 norte)"); return false;
    }
    if (!s_mishkan.menorah_colocada) {
        EJAD_LOG_E("es_ejad: Menorá no colocada (v.35 sur)"); return false;
    }

    /* 4. Testigo de fundacion - 96 adne kesef (v.19+21+25) */
    uint16_t bases = hashmal_tablas_bases_plata(s_mishkan.tablas);
    if (bases != HASHMAL_BASES_PLATA) {
        EJAD_LOG_E("es_ejad: bases de plata = %u (esperado %d)",
                   bases, HASHMAL_BASES_PLATA);
        return false;
    }

    /* Todo verificado. El Mishkan es UNO. */
    s_mishkan.ejad = true;
    EJAD_LOG_I("═══ vehayá haMishkán EJÁD ═══ (Éx 26:6, 26:30)");
    EJAD_LOG_I("  estructura verificada · membranas activas · objetos colocados");
    EJAD_LOG_I("  96 adné késef (kófer nafshó, Éx 30:12) — cimiento de rescate");
    return true;
}

/**
 * hashmal_framework_zona_de - mapeo de componente_id a zona.
 *
 * IDs derivan de la numeracion del Tavnit (00-14, CLAUDE.md):
 *
 *   QODESH_QODASHIM (detras del velo, v.33)
 *     01 ARCA, 02 KAPORET
 *
 *   QODESH (fuera del velo, dentro del masaj, v.35)
 *     03 MESA (norte), 04 MENORA (sur), 05 ESTRUCTURA,
 *     08 ACEITE (para la menora), 11 ALTAR INCIENSO,
 *     13 UNCION (shemen haMishja), 14 INCIENSO SAGRADO (qetoret)
 *
 *   JATSER (atrio - fuera del masaj)
 *     00 CIMIENTO, 06 ALTAR SACRIFICIO, 07 ATRIO, 09 VESTIDURAS,
 *     10 CONSAGRACION, 12 LAVACRO
 *
 * Desconocido -> JATSER (zona de menor privilegio - falla segura).
 */
hashmal_zona_t hashmal_framework_zona_de(uint8_t componente_id)
{
    switch (componente_id) {
        /* v.33-34 - qodesh haqodashim */
        case 1:   /* ARCA */
        case 2:   /* KAPORET */
            return HASHMAL_ZONA_QODESH_QODASHIM;

        /* v.35 y alrededores - qodesh */
        case 3:   /* MESA */
        case 4:   /* MENORA */
        case 8:   /* ACEITE */
        case 11:  /* ALTAR INCIENSO - delante del velo (Ex 30:6) */
        case 13:  /* SHEMEN haMISHJA */
        case 14:  /* QETORET haSAMMIM */
            return HASHMAL_ZONA_QODESH;

        /* jatser */
        case 0:   /* CIMIENTO */
        case 5:   /* ESTRUCTURA - no esta EN una zona, CONTIENE todas.
                   * Como objeto fisico: se ensambla en el jatser
                   * (Ex 40:17-19) y se transporta al descubierto
                   * (Nm 4:31-32). */
        case 6:   /* ALTAR SACRIFICIO (Ex 27:1-8) */
        case 7:   /* ATRIO (Ex 27:9-19) */
        case 9:   /* VESTIDURAS */
        case 10:  /* CONSAGRACION */
        case 12:  /* LAVACRO (Ex 30:17-21, entre tienda y altar) */
            return HASHMAL_ZONA_JATSER;

        default:
            EJAD_LOG_W("zona_de: componente_id=%u desconocido → JATSER", componente_id);
            return HASHMAL_ZONA_JATSER;
    }
}

/**
 * hashmal_framework_preparar_migracion - Numeros 4:5-15
 *
 * "uvanosoa hamajane uva'u Aharon uvanav vehoridu et parojet hamasaj
 *  vejissu bah et aron ha'edut"  - Nm 4:5
 *
 * Antes de mover el campamento: los sacerdotes BAJAN el velo y
 * CUBREN el Arca con el. Despues cubren la Mesa, la Menora, el
 * Altar de oro, los utensilios. Solo entonces los levitas cargan.
 * Nadie toca nada sin cubrir primero (v.15).
 *
 * En el firmware: invertir la activacion/colocacion, dejar la
 * estructura intacta (los qerashim y barras viajan enteros).
 */
int hashmal_framework_preparar_migracion(void)
{
    if (!s_inicializado) {
        EJAD_LOG_E("migracion: framework no inicializado");
        return -1;
    }

    /* Nm 4:5-15 - orden de cobertura, interior -> exterior */

    /* 1) Arca + Kaporet (v.5-6): se cubren con el velo mismo */
    s_mishkan.kaporet_colocada = false;
    s_mishkan.aron_colocado    = false;

    /* 2) Mesa (v.7-8) */
    s_mishkan.shuljan_colocado = false;

    /* 3) Menora (v.9-10) */
    s_mishkan.menorah_colocada = false;

    /* 4) Velo baja (implicito en v.5 - el velo YA cubrio el Arca
     * al bajarse). Masaj tambien se descuelga. */
    s_mishkan.velo.activo  = false;
    s_mishkan.masaj.activo = false;

    /* Estructura (tablas, barras, capas) permanece - viaja entera. */
    s_mishkan.ejad = false;

    EJAD_LOG_I("═══ MIGRACIÓN PREPARADA ═══ (Nm 4:5-15)");
    EJAD_LOG_I("  Arca/Kapóret/Mesa/Menorá cubiertos");
    EJAD_LOG_I("  velo y masaj descolgados");
    EJAD_LOG_I("  estructura (qerashim + berijim + capas) lista para transporte");
    return 0;
}
