/**
 * ===================================================================
 * PIEZA 05 - LA ESTRUCTURA - estructura_tablas.c
 * Sub-pieza D - Los 48 tablones (qerashim) - Exodo 26:15-25
 * ===================================================================
 *
 *   v.15 - "qerashim lamishkan 'atse shittim 'omedim"
 *          Tablones de acacia, de PIE. Verticales, activos, erguidos.
 *
 *   v.16 - "'eser ammot orej haqaresh ve'amma vajatsi ha'amma rojab"
 *          10 codos de largo x 1.5 de ancho. Medida UNA para todos.
 *          (dimensiones ya constantes en hashmal_framework.h)
 *
 *   v.17 - "shte yadot laqeresh ha'ejad meshullavot isha el ajota"
 *          2 "manos" entrelazadas, mujer a su hermana. Cada qeresh
 *          se acopla con sus 2 vecinas - P2P por afinidad local.
 *          "ken ta'ase lejol qarse hamishkan" - ASI para todos.
 *
 *   v.18 - 20 qerashim para el lado SUR (pe'at negbah teimanah)
 *   v.19 - 40 adne kesef bajo los 20 del sur - 2 por tabla
 *   v.20 - 20 qerashim para el lado NORTE (pe'at tsafon)
 *   v.21 - 40 adne kesef para el norte
 *   v.22 - 6 qerashim RECTOS para el OESTE (yark'te hamishkan yamah)
 *   v.23 - 2 qerashim para las ESQUINAS del fondo (limquts'ot)
 *   v.24 - "to'amim millemata veyajdav yiheyu tammim 'al rosho
 *           el hataba'at ha'ejat"
 *          GEMELOS por abajo, COMPLETOS juntos arriba, hacia UN anillo.
 *          Redundancia desde la base + integridad en la union.
 *   v.25 - Total oeste: 8 qerashim + 16 adne kesef
 *
 * TOTALES (derivados del texto):
 *   48 qerashim = 20 (sur) + 20 (norte) + 6 rectos + 2 esquinas (oeste)
 *   96 adne kesef = 2 x 48  - doble fundamento por tabla
 *
 * PLATA = RESCATE DE ALMAS (Ex 30:11-16):
 *   Cada aden ("base") fue fundida del medio siclo recogido como
 *   "kofer nafsho laYHWH" - rescate de su alma. La red entera se
 *   sostiene sobre redencion, no sobre merito.
 *
 * ESTE = ABIERTO:
 *   El cuarto lado no lleva tablas. Es la entrada - cubierta por el
 *   masaj (v.36-37), no por qerashim. Modelado con el centinela
 *   YADOT_SIN_VECINO en las posiciones extremas de sur y norte.
 *
 * "kemishpato asher horeta bahar" - Ex 26:30
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
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *TABLAS_TAG = "hashmal.tablas";

#define TABLAS_LOG_I(fmt, ...)  ESP_LOGI(TABLAS_TAG, fmt, ##__VA_ARGS__)
#define TABLAS_LOG_W(fmt, ...)  ESP_LOGW(TABLAS_TAG, fmt, ##__VA_ARGS__)
#define TABLAS_LOG_E(fmt, ...)  ESP_LOGE(TABLAS_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * CONSTANTE LOCAL - "sin vecino"
 *
 * Centinela para las yadot que miran al ESTE (la entrada). El cuarto
 * lado no lleva tablas; el mundo ahi esta cubierto por el masaj
 * (v.36-37), no por qerashim. Usar 0xFF es seguro: los IDs validos
 * son 0..47 (uint8_t).
 * ================================================================== */
#define YADOT_SIN_VECINO        0xFFu

/* ==================================================================
 * API INTERNA DEL MODULO
 * (Se promovera a hashmal_framework_internal.h con el orquestador.)
 * ================================================================== */

/**
 * Levanta los 48 qerashim conforme a Ex 26:15-25.
 * @param tablas  Array de HASHMAL_TABLAS_TOTAL a inicializar.
 * @return 0 en exito, -1 si puntero NULL.
 */
int hashmal_tablas_init(hashmal_qeresh_t tablas[HASHMAL_TABLAS_TOTAL]);

/**
 * Verifica invariantes biblicas del conjunto:
 *   v.15 - todos omedim (de pie)
 *   v.17 - yadot reciprocos: si A->B entonces B->A (isha el ajota)
 *   v.18 - 20 en sur, sin esquinas
 *   v.20 - 20 en norte, sin esquinas
 *   v.22 - 6 rectos en oeste
 *   v.23 - 2 esquinas en oeste
 *   v.24 - esquinas <==> to'amim (2 emparejadas)
 *   v.25 - 8 total oeste -> 48 total
 *   este - abierto: yadot extremas apuntan al centinela SIN_VECINO
 * @return true si el conjunto respeta el patron del monte (v.30).
 */
bool hashmal_tablas_verificar(const hashmal_qeresh_t tablas[HASHMAL_TABLAS_TOTAL]);

/**
 * Cuenta los adne kesef (bases de plata) requeridos: 2 por cada
 * tabla de pie. Total esperado = 96 = HASHMAL_BASES_PLATA (v.19+21+25).
 * Cada base es kofer nafsho - rescate de un alma (Ex 30:12).
 */
uint16_t hashmal_tablas_bases_plata(const hashmal_qeresh_t tablas[HASHMAL_TABLAS_TOTAL]);

/**
 * Nombre hebreo del lado (para logs). v.18, 20, 22.
 */
const char* hashmal_tablas_lado_nombre(hashmal_lado_t lado);

/* ==================================================================
 * HELPERS DE INDICE
 * Mapeo lado+posicion -> id global (0..47).
 * Derivado explicitamente de las constantes del header - si el
 * Padre cambiara algo, el mapeo se ajusta solo.
 * ================================================================== */
static inline uint8_t id_sur  (uint8_t pos) { return pos; }
static inline uint8_t id_norte(uint8_t pos) { return (uint8_t)(HASHMAL_TABLAS_SUR + pos); }
static inline uint8_t id_oeste(uint8_t pos) {
    return (uint8_t)(HASHMAL_TABLAS_SUR + HASHMAL_TABLAS_NORTE + pos);
}

/* Posiciones canonicas de las 2 esquinas en el arreglo oeste.
 * v.22 dice "6 rectos" -> posiciones 0..5.
 * v.23 dice "2 esquinas" -> posiciones 6 (SW) y 7 (NW). */
#define OESTE_POS_ESQUINA_SW    (HASHMAL_TABLAS_OESTE_RECTAS)        /* 6 */
#define OESTE_POS_ESQUINA_NW    (HASHMAL_TABLAS_OESTE_RECTAS + 1u)   /* 7 */

/* ==================================================================
 * CONSTRUCTORES POR LADO
 * ================================================================== */

/**
 * Lado SUR - Ex 26:18-19
 *
 * 20 qerashim en fila, acacia de pie (v.15). Position 0 = extremo
 * este (la entrada, sin vecina); position 19 = extremo oeste,
 * acoplada a la esquina SW (v.23-24, to'amim).
 *
 * v.17 - "shte yadot... meshullavot isha el ajota":
 *        yadot[0] = hermana hacia el este (o SIN_VECINO si borde)
 *        yadot[1] = hermana hacia el oeste (o esquina SW si pos=19)
 */
static void construir_sur(hashmal_qeresh_t *t, uint8_t pos)
{
    memset(t, 0, sizeof(*t));
    t->id           = id_sur(pos);
    t->lado         = HASHMAL_LADO_SUR;             /* v.18 - negbah teimanah */
    t->posicion     = pos;
    t->omedim       = true;                         /* v.15 - 'omedim */
    t->es_esquina   = false;                        /* v.23 - esquinas solo oeste */
    t->toamim       = false;                        /* v.24 - to'amim solo en esquinas */
    t->tammim       = false;                        /* v.24 - tammim solo en esquinas */
    t->tabaat_ejat  = (uint8_t)YADOT_SIN_VECINO;    /* v.24 - no aplica (sur no es esquina) */
    t->tabbeot_zahav = HASHMAL_BARRAS_POR_LADO;     /* v.29 - 5 anillos de oro, casas para las 5 barras */

    t->yadot[0] = (pos == 0u)
                  ? (uint8_t)YADOT_SIN_VECINO                        /* este abierto */
                  : id_sur((uint8_t)(pos - 1u));
    t->yadot[1] = (pos == (uint8_t)(HASHMAL_TABLAS_SUR - 1u))
                  ? id_oeste(OESTE_POS_ESQUINA_SW)                   /* ultimo sur <-> esquina SW */
                  : id_sur((uint8_t)(pos + 1u));
}

/**
 * Lado NORTE - Ex 26:20-21
 *
 * Espejo del sur: 20 qerashim, position 0 = extremo este (sin
 * vecina), position 19 = extremo oeste, acoplado a la esquina NW.
 */
static void construir_norte(hashmal_qeresh_t *t, uint8_t pos)
{
    memset(t, 0, sizeof(*t));
    t->id           = id_norte(pos);
    t->lado         = HASHMAL_LADO_NORTE;           /* v.20 - tsafon */
    t->posicion     = pos;
    t->omedim       = true;                         /* v.15 */
    t->es_esquina   = false;
    t->toamim       = false;
    t->tammim       = false;                        /* v.24 - tammim solo en esquinas */
    t->tabaat_ejat  = (uint8_t)YADOT_SIN_VECINO;    /* v.24 - no aplica (norte no es esquina) */
    t->tabbeot_zahav = HASHMAL_BARRAS_POR_LADO;     /* v.29 - 5 anillos de oro, casas para las 5 barras */

    t->yadot[0] = (pos == 0u)
                  ? (uint8_t)YADOT_SIN_VECINO                        /* este abierto */
                  : id_norte((uint8_t)(pos - 1u));
    t->yadot[1] = (pos == (uint8_t)(HASHMAL_TABLAS_NORTE - 1u))
                  ? id_oeste(OESTE_POS_ESQUINA_NW)                   /* ultimo norte <-> esquina NW */
                  : id_norte((uint8_t)(pos + 1u));
}

/**
 * Lado OESTE - Ex 26:22-25
 *
 * 8 qerashim = 6 rectos (v.22) + 2 esquinas (v.23).
 *
 * Trayectoria fisica sur -> norte a lo largo del muro oeste:
 *   esquina SW (pos 6) -> rectos 0..5 (pos 0..5) -> esquina NW (pos 7)
 *
 * v.24 - "to'amim millemata" (gemelos por abajo) +
 *        "yajdav... tammim 'al rosho el hataba'at ha'ejat"
 *        (completos juntos arriba, hacia UN anillo).
 *        Los qerashim de esquina estan marcados es_esquina=true
 *        y toamim=true porque LA ESCRITURA los empareja como
 *        pareja propia del miqtso'a (no como objetos sueltos).
 */
static void construir_oeste(hashmal_qeresh_t *t, uint8_t pos)
{
    memset(t, 0, sizeof(*t));
    t->id       = id_oeste(pos);
    t->lado     = HASHMAL_LADO_OESTE;               /* v.22 - yamah */
    t->posicion = pos;
    t->omedim   = true;                             /* v.15 */

    if (pos < HASHMAL_TABLAS_OESTE_RECTAS) {
        /* Rectos 0..5 - v.22 */
        t->es_esquina    = false;
        t->toamim        = false;
        t->tammim        = false;                     /* v.24 - rectos no son tammim */
        t->tabaat_ejat   = (uint8_t)YADOT_SIN_VECINO; /* v.24 - no aplica (recto no es esquina) */
        t->tabbeot_zahav = HASHMAL_BARRAS_POR_LADO;   /* v.29 - 5 anillos de oro, casas para las 5 barras */

        /* yadot[0] = hacia el sur (esquina SW si pos=0, si no, recto anterior) */
        t->yadot[0] = (pos == 0u)
                      ? id_oeste(OESTE_POS_ESQUINA_SW)
                      : id_oeste((uint8_t)(pos - 1u));
        /* yadot[1] = hacia el norte (esquina NW si pos=5, si no, recto siguiente) */
        t->yadot[1] = (pos == (uint8_t)(HASHMAL_TABLAS_OESTE_RECTAS - 1u))
                      ? id_oeste(OESTE_POS_ESQUINA_NW)
                      : id_oeste((uint8_t)(pos + 1u));
    } else if (pos == OESTE_POS_ESQUINA_SW) {
        /* Esquina SW - v.23-24 */
        t->es_esquina    = true;
        t->toamim        = true;
        t->tammim        = true;                            /* v.24 - tammim 'al rosho */
        t->tabaat_ejat   = id_oeste(OESTE_POS_ESQUINA_NW);  /* v.24 - SW y NW son to'amim abajo,
                                                             * tammim arriba, unidos en UN anillo */
        t->tabbeot_zahav = HASHMAL_BARRAS_POR_LADO;         /* v.29 - 5 anillos de oro, casas para las 5 barras */
        t->yadot[0] = id_sur((uint8_t)(HASHMAL_TABLAS_SUR - 1u));   /* pareada con ultimo sur */
        t->yadot[1] = id_oeste(0u);                                 /* pareada con primer recto */
    } else {
        /* Esquina NW (pos == OESTE_POS_ESQUINA_NW) - v.23-24 */
        t->es_esquina    = true;
        t->toamim        = true;
        t->tammim        = true;                            /* v.24 - tammim 'al rosho */
        t->tabaat_ejat   = id_oeste(OESTE_POS_ESQUINA_SW);  /* v.24 - NW y SW son to'amim abajo,
                                                             * tammim arriba, unidos en UN anillo */
        t->tabbeot_zahav = HASHMAL_BARRAS_POR_LADO;         /* v.29 - 5 anillos de oro, casas para las 5 barras */
        t->yadot[0] = id_oeste((uint8_t)(HASHMAL_TABLAS_OESTE_RECTAS - 1u)); /* ultimo recto */
        t->yadot[1] = id_norte((uint8_t)(HASHMAL_TABLAS_NORTE - 1u));        /* ultimo norte */
    }
}

/* ==================================================================
 * API INTERNA - IMPLEMENTACION
 * ================================================================== */

int hashmal_tablas_init(hashmal_qeresh_t tablas[HASHMAL_TABLAS_TOTAL])
{
    if (tablas == NULL) {
        TABLAS_LOG_E("init: puntero NULL");
        return -1;
    }

    /* Sur (v.18) */
    for (uint8_t p = 0; p < HASHMAL_TABLAS_SUR; ++p) {
        construir_sur(&tablas[id_sur(p)], p);
    }
    /* Norte (v.20) */
    for (uint8_t p = 0; p < HASHMAL_TABLAS_NORTE; ++p) {
        construir_norte(&tablas[id_norte(p)], p);
    }
    /* Oeste (v.22 + v.23 = v.25) */
    for (uint8_t p = 0; p < HASHMAL_TABLAS_OESTE; ++p) {
        construir_oeste(&tablas[id_oeste(p)], p);
    }

    TABLAS_LOG_I("48 qerashím levantados (Ex 26:15-25): 20 sur + 20 norte + 6+2 oeste");
    TABLAS_LOG_I("Este abierto (entrada, v.36-37 → masaj)");
    TABLAS_LOG_I("96 adné késef requeridos — cada uno kófer nafshó (Éx 30:12)");
    return 0;
}

bool hashmal_tablas_verificar(const hashmal_qeresh_t tablas[HASHMAL_TABLAS_TOTAL])
{
    if (tablas == NULL) {
        TABLAS_LOG_E("verificar: puntero NULL");
        return false;
    }

    uint8_t n_sur = 0, n_norte = 0, n_oeste = 0;
    uint8_t n_rectos = 0, n_esquinas = 0, n_toamim = 0, n_tammim = 0;
    uint8_t n_abiertas_este = 0;

    /* ----- Pasada 1 - cada qeresh individual ----- */
    for (uint8_t i = 0; i < HASHMAL_TABLAS_TOTAL; ++i) {
        const hashmal_qeresh_t *t = &tablas[i];

        /* id debe coincidir con indice (integridad de colocacion) */
        if (t->id != i) {
            TABLAS_LOG_E("id(%u) != índice(%u)", t->id, i);
            return false;
        }

        /* v.15 - 'omedim: todos de pie, ninguno caido */
        if (!t->omedim) {
            TABLAS_LOG_E("v.15 violado: qéresh %u no está de pie", i);
            return false;
        }

        /* v.29 - tabbe'ot zahav battim labberijim: cada tablon lleva
         * HASHMAL_BARRAS_POR_LADO anillos de oro como casas para las
         * 5 barras que lo atraviesan. */
        if (t->tabbeot_zahav != HASHMAL_BARRAS_POR_LADO) {
            TABLAS_LOG_E("v.29 violado: qéresh %u tiene %u anillos (esperado %d)",
                         i, t->tabbeot_zahav, HASHMAL_BARRAS_POR_LADO);
            return false;
        }

        /* Conteo por lado + reglas locales */
        switch (t->lado) {
            case HASHMAL_LADO_SUR:
                n_sur++;
                if (t->es_esquina || t->toamim) {
                    TABLAS_LOG_E("v.18/23 violado: sur %u no debe ser esquina/toʼam", i);
                    return false;
                }
                /* extremo este: yadot[0] debe ser SIN_VECINO */
                if (t->posicion == 0u && t->yadot[0] == YADOT_SIN_VECINO) {
                    n_abiertas_este++;
                }
                break;

            case HASHMAL_LADO_NORTE:
                n_norte++;
                if (t->es_esquina || t->toamim) {
                    TABLAS_LOG_E("v.20/23 violado: norte %u no debe ser esquina/toʼam", i);
                    return false;
                }
                if (t->posicion == 0u && t->yadot[0] == YADOT_SIN_VECINO) {
                    n_abiertas_este++;
                }
                break;

            case HASHMAL_LADO_OESTE:
                n_oeste++;
                if (t->es_esquina) n_esquinas++;
                else               n_rectos++;
                if (t->toamim)     n_toamim++;
                if (t->tammim)     n_tammim++;

                /* v.24 - esquina <==> to'am (emparejamiento biblico) */
                if (t->es_esquina != t->toamim) {
                    TABLAS_LOG_E("v.24 violado: esquina y toʼam desacoplados en %u", i);
                    return false;
                }
                /* v.24 - esquina <==> tammim (completos arriba solo en esquinas) */
                if (t->es_esquina != t->tammim) {
                    TABLAS_LOG_E("v.24 violado: tammím desacoplado de esquina en %u", i);
                    return false;
                }
                break;

            default:
                TABLAS_LOG_E("lado inválido (%d) en qéresh %u", (int)t->lado, i);
                return false;
        }

        /* Yadot: IDs validos o centinela */
        for (uint8_t k = 0; k < HASHMAL_YADOT_POR_TABLA; ++k) {
            uint8_t v = t->yadot[k];
            if (v == YADOT_SIN_VECINO) continue;
            if (v >= HASHMAL_TABLAS_TOTAL) {
                TABLAS_LOG_E("yad(%u) de qéresh %u → id inválido %u", k, i, v);
                return false;
            }
            if (v == i) {
                TABLAS_LOG_E("v.17 violado: qéresh %u se acopla consigo mismo", i);
                return false;
            }
        }
    }

    /* ----- Conteos globales ----- */
    if (n_sur != HASHMAL_TABLAS_SUR) {
        TABLAS_LOG_E("v.18 violado: sur=%u (esperado %d)", n_sur, HASHMAL_TABLAS_SUR);
        return false;
    }
    if (n_norte != HASHMAL_TABLAS_NORTE) {
        TABLAS_LOG_E("v.20 violado: norte=%u (esperado %d)", n_norte, HASHMAL_TABLAS_NORTE);
        return false;
    }
    if (n_oeste != HASHMAL_TABLAS_OESTE) {
        TABLAS_LOG_E("v.25 violado: oeste=%u (esperado %d)", n_oeste, HASHMAL_TABLAS_OESTE);
        return false;
    }
    if (n_rectos != HASHMAL_TABLAS_OESTE_RECTAS) {
        TABLAS_LOG_E("v.22 violado: rectos=%u (esperado %d)", n_rectos, HASHMAL_TABLAS_OESTE_RECTAS);
        return false;
    }
    if (n_esquinas != HASHMAL_TABLAS_ESQUINAS) {
        TABLAS_LOG_E("v.23 violado: esquinas=%u (esperado %d)", n_esquinas, HASHMAL_TABLAS_ESQUINAS);
        return false;
    }
    if (n_toamim != HASHMAL_TABLAS_ESQUINAS) {
        TABLAS_LOG_E("v.24 violado: toʼamím=%u (esperado %d)", n_toamim, HASHMAL_TABLAS_ESQUINAS);
        return false;
    }

    /* v.24 - tammim 'al rosho: exactamente 2 (una por esquina). */
    if (n_tammim != HASHMAL_TABLAS_ESQUINAS) {
        TABLAS_LOG_E("v.24 violado: tammím=%u (esperado %d)", n_tammim, HASHMAL_TABLAS_ESQUINAS);
        return false;
    }

    /* Este debe estar abierto en sur y norte (2 extremos sin vecina) */
    if (n_abiertas_este != 2u) {
        TABLAS_LOG_E("este no está abierto correctamente: %u (esperado 2)", n_abiertas_este);
        return false;
    }

    /* ----- Pasada 2 - v.17 reciprocidad (isha el ajota) ----- */
    for (uint8_t i = 0; i < HASHMAL_TABLAS_TOTAL; ++i) {
        for (uint8_t k = 0; k < HASHMAL_YADOT_POR_TABLA; ++k) {
            uint8_t v = tablas[i].yadot[k];
            if (v == YADOT_SIN_VECINO) continue;

            bool reciproco = false;
            for (uint8_t kk = 0; kk < HASHMAL_YADOT_POR_TABLA; ++kk) {
                if (tablas[v].yadot[kk] == i) { reciproco = true; break; }
            }
            if (!reciproco) {
                TABLAS_LOG_E("v.17 violado: yad %u→%u sin retorno %u→%u", i, v, v, i);
                return false;
            }
        }
    }

    /* ----- Pasada 3 - v.24 reciprocidad del anillo (el hataba'at ha'ejat) -----
     * Cada tammim apunta a su pareja via tabaat_ejat; la pareja debe
     * ser tambien tammim y apuntar de vuelta (isha el ajota, arriba). */
    for (uint8_t i = 0; i < HASHMAL_TABLAS_TOTAL; ++i) {
        const hashmal_qeresh_t *t = &tablas[i];
        if (!t->tammim) continue;

        uint8_t pareja = t->tabaat_ejat;
        if (pareja == YADOT_SIN_VECINO || pareja >= HASHMAL_TABLAS_TOTAL) {
            TABLAS_LOG_E("v.24 violado: tabaʻat inválida en esquina %u → %u",
                         i, pareja);
            return false;
        }
        if (!tablas[pareja].tammim) {
            TABLAS_LOG_E("v.24 violado: tabaʻat %u→%u pero %u no es tammím",
                         i, pareja, pareja);
            return false;
        }
        if (tablas[pareja].tabaat_ejat != i) {
            TABLAS_LOG_E("v.24 violado: tabaʻat no recíproca entre esquinas %u y %u",
                         i, pareja);
            return false;
        }
    }

    /* ----- Bases de plata (v.19+21+25 = 96) ----- */
    uint16_t bases = hashmal_tablas_bases_plata(tablas);
    if (bases != HASHMAL_BASES_PLATA) {
        TABLAS_LOG_E("v.19+21+25 violado: bases=%u (esperado %d)", bases, HASHMAL_BASES_PLATA);
        return false;
    }

    TABLAS_LOG_I("48 qerashím verificados: 20 sur + 20 norte + 6+2 oeste; este abierto");
    TABLAS_LOG_I("v.17 recíproco (ishá el ajotá); v.24 esquinas toʼamím (×2)");
    TABLAS_LOG_I("96 adné késef — fundados sobre kófer nafshó (Éx 30:12)");
    return true;
}

uint16_t hashmal_tablas_bases_plata(const hashmal_qeresh_t tablas[HASHMAL_TABLAS_TOTAL])
{
    if (tablas == NULL) return 0u;

    /* v.19, v.21, v.25: 2 adne kesef bajo cada qeresh de pie.
     * La plata viene del medio siclo de cada alma (Ex 30:11-16):
     *   "zeh yitnu... majatsit hasheqel... kofer nafsho laYHWH"
     * Cada base = rescate. Doble base = doble testigo (Dt 19:15). */
    uint16_t bases = 0u;
    for (uint8_t i = 0; i < HASHMAL_TABLAS_TOTAL; ++i) {
        if (tablas[i].omedim) {
            bases = (uint16_t)(bases + 2u);
        }
    }
    return bases;
}

const char* hashmal_tablas_lado_nombre(hashmal_lado_t lado)
{
    switch (lado) {
        case HASHMAL_LADO_SUR:   return "négbah teimánah (sur)";   /* v.18 */
        case HASHMAL_LADO_NORTE: return "tsafón (norte)";            /* v.20 */
        case HASHMAL_LADO_OESTE: return "yámah (oeste, al mar)";     /* v.22 */
        default:                 return "(lado inválido)";
    }
}
