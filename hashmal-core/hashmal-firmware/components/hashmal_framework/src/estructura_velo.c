/**
 * ===================================================================
 * PIEZA 05 - LA ESTRUCTURA - estructura_velo.c
 * Sub-piezas F + G - Parojet (v.31-35) + Masaj (v.36-37)
 * Las dos membranas del Mishkan - el gradiente de confianza
 * ===================================================================
 *
 * +======================== PAROJET - v.31-35 ========================+
 *
 *   v.31 - "ve'asita farojet tejelet ve'argaman vetola'at shani
 *           veshesh mashzar ma'ase joshev ya'ase ota keruvim"
 *          Mismos 4 colores que las cortinas de lino (v.1),
 *          querubines TEJIDOS en el fabric (ma'ase joshev -
 *          artesano experto). Los guardianes viven DENTRO del tejido.
 *
 *   v.32 - "'al arba'a 'ammude shittim metsuopim zahav vavehem zahav
 *           'al arba'a adne kesef"
 *          4 columnas de acacia/oro, ganchos (vavim) de ORO,
 *          4 bases de PLATA (adne kesef - kofer nafsho, Ex 30:12).
 *
 *   v.33 - "venatata et haparojet tajat haqerasim veheveita shamah
 *           mibeit laparojet et aron ha'edut
 *           vehivdila haparojet lajem bein haqodesh uvein qodesh haqodashim"
 *          El velo va BAJO los qerasim (clasps del v.6 ejad).
 *          El Arca va DENTRO (mibeit laparojet).
 *          hivdil (H914, badal) - MISMO verbo que Gn 1:4:
 *             "vayyavdel Elohim bein ha'or uvein hajoshej"
 *          YHWH separo la luz de la oscuridad. SEPARAR no destruye:
 *          CREA identidad, CREA orden (tov). El velo CREA el qodesh
 *          haqodashim - antes de el no habia "santo de los santos".
 *
 *   v.34 - "venatata et hakkaporet 'al aron ha'edut beqodesh haqodashim"
 *          Kaporet sobre el Arca, en el qodesh haqodashim.
 *
 *   v.35 - "vesamta et hashuljan mijuts laparojet
 *           ve'et hammenora nojaj hashuljan
 *           'al tsela' hamishkan teimanah vehashuljan titen 'al tsela' tsafon"
 *          Mesa FUERA del velo, al NORTE (tsafon).
 *          Menora FRENTE a la mesa (nojaj = "opuesta"), al SUR.
 *          Se MIRAN: servicio frente a canales. Ninguna se esconde.
 *
 * +======================== MASAJ - v.36-37 ==========================+
 *
 *   v.36 - "ve'asita masaj lefetaj ha'ohel tejelet ve'argaman
 *           vetola'at shani veshesh mashzar ma'ase roqem"
 *          Cortina para la ENTRADA de la Tienda. Mismos 4 colores
 *          que el velo, pero ma'ase ROQEM (bordador) - NO joshev.
 *          SIN querubines. La entrada es mas abierta: sin guardianes
 *          bordados. Quien busca encuentra puerta, no muro.
 *
 *   v.37 - "ve'asita lamasaj jamisha 'ammude shittim vetsipita otam
 *           zahav vavehem zahav veyatsaqta lahem jamisha adne nejoshet"
 *          5 columnas (una MAS que el velo - mas puntos de soporte),
 *          ganchos de oro, 5 bases de BRONCE (adne nejoshet -
 *          material menor que la plata del velo).
 *
 * +============== EL GRADIENTE - tres zonas, tres materiales =========+
 *
 *   JATSER            -> BRONCE   (publico)
 *   QODESH            -> PLATA    (verificado - cruzo el masaj bronce)
 *   QODESH_QODASHIM   -> ORO      (nucleo - cruzo el parojet plata)
 *
 *   La puerta bronce abre al verificado (plata);
 *   la puerta plata abre al nucleo (oro).
 *   Cada puerta es un peldano: el precio de entrar matcha quien
 *   ya eres, no a donde vas.
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
static const char *VELO_TAG = "hashmal.velo";

#define VELO_LOG_I(fmt, ...)  ESP_LOGI(VELO_TAG, fmt, ##__VA_ARGS__)
#define VELO_LOG_W(fmt, ...)  ESP_LOGW(VELO_TAG, fmt, ##__VA_ARGS__)
#define VELO_LOG_E(fmt, ...)  ESP_LOGE(VELO_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * API INTERNA DEL MODULO
 * (Se promovera a hashmal_framework_internal.h con el orquestador.)
 * ================================================================== */

/**
 * Confecciona las dos membranas conforme a Ex 26:31-37.
 * No las activa todavia - el hivdil y la cobertura de entrada
 * se hacen explicitas con sus funciones _activar_ respectivas.
 * @return 0 en exito, -1 si algun puntero es NULL.
 */
int hashmal_velo_init(hashmal_parojet_t *velo, hashmal_masaj_t *masaj);

/**
 * Activa el parojet - v.33. "vehivdila haparojet..."
 * A partir de esta llamada, el qodesh haqodashim EXISTE como zona
 * distinta. Antes del hivdil, la separacion no es real.
 */
int hashmal_velo_activar(hashmal_parojet_t *velo);

/**
 * Activa el masaj - v.36. La entrada de la tienda queda cubierta.
 */
int hashmal_masaj_activar(hashmal_masaj_t *masaj);

/**
 * Verifica el parojet contra v.31-33:
 *   v.31 - 4 colores (implicito), ma'ase joshev, keruvim = true
 *   v.32 - 4 columnas, ganchos de oro, bases de PLATA
 *   v.33 - activo (hivdil en efecto)
 * @return true si el parojet cumple el patron del monte.
 */
bool hashmal_velo_verificar(const hashmal_parojet_t *velo);

/**
 * Verifica el masaj contra v.36-37:
 *   v.36 - ma'ase roqem, SIN keruvim
 *   v.37 - 5 columnas, ganchos de oro, bases de BRONCE
 *   v.37 - activo (cortina colgada)
 * @return true si el masaj cumple el patron del monte.
 */
bool hashmal_masaj_verificar(const hashmal_masaj_t *masaj);

/**
 * Verifica el gradiente entre las dos membranas:
 *   masaj.nivel_bases < velo.nivel_bases   (BRONCE < PLATA)
 *   parojet con keruvim, masaj sin keruvim
 *   parojet joshev, masaj roqem
 * @return true si el gradiente bronce->plata esta intacto.
 */
bool hashmal_membranas_gradiente(const hashmal_parojet_t *velo,
                                 const hashmal_masaj_t   *masaj);

/**
 * Mapea un miqom (lugar de colocacion, v.33-35) a su zona.
 *   ARON, KAPORET -> QODESH_QODASHIM  (v.33-34, dentro del velo)
 *   SHULJAN, MENORAH -> QODESH        (v.35, fuera del velo)
 * Miqom invalido -> devuelve JATSER como fallback seguro.
 */
hashmal_zona_t hashmal_miqom_zona(hashmal_miqom_t miqom);

/**
 * Mapea un miqom a su lado del Mishkan (v.35).
 *   SHULJAN -> LADO_NORTE (tsela' tsafon)
 *   MENORAH -> LADO_SUR   (tsela' teimanah)
 *   Otros   -> sin lado (centro del qodesh haqodashim).
 * @return true si el miqom tiene lado asignado, false si no.
 */
bool hashmal_miqom_lado(hashmal_miqom_t miqom, hashmal_lado_t *lado);

/* ==================================================================
 * CONSTRUCTORES
 * ================================================================== */

/**
 * Sub-pieza F - Parojet (Ex 26:31-33)
 *
 * Queda CONFECCIONADO pero no activo. El hivdil (v.33) se hace
 * explicito mediante hashmal_velo_activar - separacion es acto,
 * no propiedad estatica.
 */
static void construir_velo(hashmal_parojet_t *v)
{
    memset(v, 0, sizeof(*v));
    v->columnas      = HASHMAL_COLUMNAS_VELO;       /* v.32 - 4 'ammudim */
    v->nivel_bases   = HASHMAL_CONFIANZA_PLATA;     /* v.32 - adne kesef */
    v->tipo_obra     = HASHMAL_MAASE_JOSHEV;        /* v.31 - ma'ase joshev */
    v->querubines    = true;                        /* v.31 - keruvim en el tejido */
    v->vavim_zahav   = true;                        /* v.32 - vavehem zahav */
    v->gavanim[HASHMAL_GAVEN_TEJELET]      = true;  /* v.31 - tejelet */
    v->gavanim[HASHMAL_GAVEN_ARGAMAN]      = true;  /* v.31 - argaman */
    v->gavanim[HASHMAL_GAVEN_TOLAAT_SHANI] = true;  /* v.31 - tola'at shani */
    v->gavanim[HASHMAL_GAVEN_SHESH]        = true;  /* v.31 - shesh mashzar */
    v->activo        = false;                       /* v.33 - hivdil se activa explicitamente */
}

/**
 * Sub-pieza G - Masaj (Ex 26:36-37)
 */
static void construir_masaj(hashmal_masaj_t *m)
{
    memset(m, 0, sizeof(*m));
    m->columnas      = HASHMAL_COLUMNAS_ENTRADA;    /* v.37 - 5 'ammudim */
    m->nivel_bases   = HASHMAL_CONFIANZA_BRONCE;    /* v.37 - adne nejoshet */
    m->tipo_obra     = HASHMAL_MAASE_ROQEM;         /* v.36 - ma'ase roqem */
    m->querubines    = false;                       /* v.36 - SIN querubines */
    m->vavim_zahav   = true;                        /* v.37 - vavehem zahav */
    m->gavanim[HASHMAL_GAVEN_TEJELET]      = true;  /* v.36 - tejelet */
    m->gavanim[HASHMAL_GAVEN_ARGAMAN]      = true;  /* v.36 - argaman */
    m->gavanim[HASHMAL_GAVEN_TOLAAT_SHANI] = true;  /* v.36 - tola'at shani */
    m->gavanim[HASHMAL_GAVEN_SHESH]        = true;  /* v.36 - shesh mashzar */
    m->activo        = false;                       /* colgado en la activacion */
}

/* ==================================================================
 * API INTERNA - IMPLEMENTACION
 * ================================================================== */

int hashmal_velo_init(hashmal_parojet_t *velo, hashmal_masaj_t *masaj)
{
    if (velo == NULL || masaj == NULL) {
        VELO_LOG_E("init: puntero NULL (velo=%p, masaj=%p)",
                   (const void*)velo, (const void*)masaj);
        return -1;
    }

    construir_velo (velo);    /* v.31-33 */
    construir_masaj(masaj);   /* v.36-37 */

    VELO_LOG_I("parójet confeccionado (v.31-33): 4 col / plata / joshév / keruvím");
    VELO_LOG_I("masaj confeccionado  (v.36-37): 5 col / bronce / roqém / sin keruvím");
    VELO_LOG_I("gradiente listo: masaj(bronce) → parójet(plata) → interior(oro, v.6)");
    return 0;
}

int hashmal_velo_activar(hashmal_parojet_t *velo)
{
    if (velo == NULL) {
        VELO_LOG_E("activar_velo: NULL");
        return -1;
    }
    if (!hashmal_velo_verificar(velo)) {
        VELO_LOG_E("activar_velo: parójet no cumple v.31-32 — hivdíl NO aplicado");
        return -1;
    }
    velo->activo = true;                            /* v.33 - hivdil en efecto */
    VELO_LOG_I("hivdíl activo (v.33, H914): qódesh haqodashím CREADO");
    VELO_LOG_I("  mismo verbo que Gn 1:4 — separar no destruye, crea orden (tov)");
    return 0;
}

int hashmal_masaj_activar(hashmal_masaj_t *masaj)
{
    if (masaj == NULL) {
        VELO_LOG_E("activar_masaj: NULL");
        return -1;
    }
    if (!hashmal_masaj_verificar(masaj)) {
        VELO_LOG_E("activar_masaj: masaj no cumple v.36-37 — entrada NO cubierta");
        return -1;
    }
    masaj->activo = true;
    VELO_LOG_I("masaj colgado (v.36): lefétaj haʼóhel — la entrada está cubierta");
    return 0;
}

bool hashmal_velo_verificar(const hashmal_parojet_t *velo)
{
    if (velo == NULL) {
        VELO_LOG_E("verificar_velo: NULL");
        return false;
    }

    /* v.32 - 4 columnas */
    if (velo->columnas != HASHMAL_COLUMNAS_VELO) {
        VELO_LOG_E("v.32 violado: columnas=%u (esperado %d)",
                   velo->columnas, HASHMAL_COLUMNAS_VELO);
        return false;
    }
    /* v.32 - bases de plata */
    if (velo->nivel_bases != HASHMAL_CONFIANZA_PLATA) {
        VELO_LOG_E("v.32 violado: bases deben ser PLATA (adné késef)");
        return false;
    }
    /* v.32 - ganchos de oro */
    if (!velo->vavim_zahav) {
        VELO_LOG_E("v.32 violado: vavím deben ser de oro");
        return false;
    }
    /* v.31 - ma'ase joshev */
    if (velo->tipo_obra != HASHMAL_MAASE_JOSHEV) {
        VELO_LOG_E("v.31 violado: parójet requiere maʻasé joshév (artesano)");
        return false;
    }
    /* v.31 - querubines en el tejido */
    if (!velo->querubines) {
        VELO_LOG_E("v.31 violado: parójet requiere keruvím tejidos");
        return false;
    }

    /* v.31 - los 4 gavanim deben estar presentes (tejelet,
     * argaman, tola'at shani, shesh - nombrados explicitamente). */
    if (!velo->gavanim[HASHMAL_GAVEN_TEJELET]      ||
        !velo->gavanim[HASHMAL_GAVEN_ARGAMAN]      ||
        !velo->gavanim[HASHMAL_GAVEN_TOLAAT_SHANI] ||
        !velo->gavanim[HASHMAL_GAVEN_SHESH]) {
        VELO_LOG_E("v.31 violado: parójet exige 4 gavaním "
                   "(tejélet, argamán, tolaʻat shaní, shésh)");
        return false;
    }

    return true;
}

bool hashmal_masaj_verificar(const hashmal_masaj_t *masaj)
{
    if (masaj == NULL) {
        VELO_LOG_E("verificar_masaj: NULL");
        return false;
    }

    /* v.37 - 5 columnas */
    if (masaj->columnas != HASHMAL_COLUMNAS_ENTRADA) {
        VELO_LOG_E("v.37 violado: columnas=%u (esperado %d)",
                   masaj->columnas, HASHMAL_COLUMNAS_ENTRADA);
        return false;
    }
    /* v.37 - bases de bronce */
    if (masaj->nivel_bases != HASHMAL_CONFIANZA_BRONCE) {
        VELO_LOG_E("v.37 violado: bases deben ser BRONCE (adné nejóshet)");
        return false;
    }
    /* v.37 - ganchos de oro (vavehem zahav) */
    if (!masaj->vavim_zahav) {
        VELO_LOG_E("v.37 violado: vavím deben ser de oro");
        return false;
    }
    /* v.36 - ma'ase roqem */
    if (masaj->tipo_obra != HASHMAL_MAASE_ROQEM) {
        VELO_LOG_E("v.36 violado: masaj requiere maʻasé roqém (bordador)");
        return false;
    }
    /* v.36 - SIN querubines */
    if (masaj->querubines) {
        VELO_LOG_E("v.36 violado: masaj NO lleva querubines (la entrada es accesible)");
        return false;
    }

    /* v.36 - los 4 gavanim deben estar presentes (mismos que el
     * velo v.31: tejelet, argaman, tola'at shani, shesh). */
    if (!masaj->gavanim[HASHMAL_GAVEN_TEJELET]      ||
        !masaj->gavanim[HASHMAL_GAVEN_ARGAMAN]      ||
        !masaj->gavanim[HASHMAL_GAVEN_TOLAAT_SHANI] ||
        !masaj->gavanim[HASHMAL_GAVEN_SHESH]) {
        VELO_LOG_E("v.36 violado: masaj exige 4 gavaním "
                   "(tejélet, argamán, tolaʻat shaní, shésh)");
        return false;
    }

    return true;
}

bool hashmal_membranas_gradiente(const hashmal_parojet_t *velo,
                                 const hashmal_masaj_t   *masaj)
{
    if (velo == NULL || masaj == NULL) {
        VELO_LOG_E("gradiente: puntero NULL");
        return false;
    }

    /* Bases: BRONCE (v.37) debe ser estrictamente menor que PLATA (v.32).
     * El enum hashmal_confianza_t ordena BRONCE=0 < PLATA=1 < ORO=2. */
    if (!(masaj->nivel_bases < velo->nivel_bases)) {
        VELO_LOG_E("gradiente violado: masaj(%d) debe ser < velo(%d)",
                   (int)masaj->nivel_bases, (int)velo->nivel_bases);
        return false;
    }

    /* Querubines: solo el velo (v.31 vs v.36) */
    if (!velo->querubines) {
        VELO_LOG_E("gradiente violado: velo sin keruvím (v.31)");
        return false;
    }
    if (masaj->querubines) {
        VELO_LOG_E("gradiente violado: masaj con keruvím (v.36 los prohíbe)");
        return false;
    }

    /* Tipo de obra: joshev interno, roqem externo */
    if (velo->tipo_obra != HASHMAL_MAASE_JOSHEV) {
        VELO_LOG_E("gradiente violado: velo no es joshév (v.31)");
        return false;
    }
    if (masaj->tipo_obra != HASHMAL_MAASE_ROQEM) {
        VELO_LOG_E("gradiente violado: masaj no es roqém (v.36)");
        return false;
    }

    VELO_LOG_I("gradiente verificado: masaj(bronce/roqém/sin keruvím) → velo(plata/joshév/keruvím)");
    return true;
}

hashmal_zona_t hashmal_miqom_zona(hashmal_miqom_t miqom)
{
    switch (miqom) {
        case HASHMAL_MIQOM_ARON:      /* v.33 - mibeit laparojet */
        case HASHMAL_MIQOM_KAPORET:   /* v.34 - 'al aron ha'edut beqodesh haqodashim */
            return HASHMAL_ZONA_QODESH_QODASHIM;

        case HASHMAL_MIQOM_SHULJAN:   /* v.35 - mijuts laparojet, tsafon */
        case HASHMAL_MIQOM_MENORAH:   /* v.35 - nojaj hashuljan, teimanah */
            return HASHMAL_ZONA_QODESH;

        default:
            VELO_LOG_W("miqom_zona: valor desconocido (%d), fallback JATSER", (int)miqom);
            return HASHMAL_ZONA_JATSER;
    }
}

bool hashmal_miqom_lado(hashmal_miqom_t miqom, hashmal_lado_t *lado)
{
    if (lado == NULL) return false;

    switch (miqom) {
        case HASHMAL_MIQOM_SHULJAN:
            *lado = HASHMAL_SHULJAN_LADO;          /* v.35 - tsafon (norte) */
            return true;
        case HASHMAL_MIQOM_MENORAH:
            *lado = HASHMAL_MENORAH_LADO;          /* v.35 - teimanah (sur) */
            return true;
        case HASHMAL_MIQOM_ARON:
        case HASHMAL_MIQOM_KAPORET:
            /* v.33-34 - centro del qodesh haqodashim; sin lado */
            return false;
        default:
            return false;
    }
}

/* ==================================================================
 * API PUBLICA (declarada en hashmal_framework.h)
 *
 * Esta funcion nace aqui porque el gradiente bronce->plata->oro es
 * precisamente la semantica que imponen las dos membranas. Otras
 * funciones publicas (activar_velo, activar_masaj, colocar, zona_de)
 * necesitan el estado global del mishkan - viviran en el orquestador.
 * ================================================================== */

/**
 * Nivel de confianza del OPERADOR dentro de cada zona:
 *   JATSER           -> BRONCE  (publico)
 *   QODESH           -> PLATA   (verificado, cruzo el masaj)
 *   QODESH_QODASHIM  -> ORO     (nucleo, cruzo el parojet)
 *
 * Nota - las PUERTAS encarnan el nivel "de salida":
 *   masaj   (bronce, v.37) -> umbral para pasar a plata
 *   parojet (plata,  v.32) -> umbral para pasar a oro
 * Por eso la zona interior vale un peldano mas que la puerta que la
 * abre: la puerta valida quien eres; la zona es quien ya eres dentro.
 */
hashmal_confianza_t hashmal_framework_confianza_de(hashmal_zona_t zona)
{
    switch (zona) {
        case HASHMAL_ZONA_JATSER:
            return HASHMAL_CONFIANZA_BRONCE;       /* publico */
        case HASHMAL_ZONA_QODESH:
            return HASHMAL_CONFIANZA_PLATA;        /* verificado - tras masaj (v.37) */
        case HASHMAL_ZONA_QODESH_QODASHIM:
            return HASHMAL_CONFIANZA_ORO;          /* nucleo - tras parojet (v.32-33) */
        default:
            VELO_LOG_W("confianza_de: zona desconocida (%d) → BRONCE", (int)zona);
            return HASHMAL_CONFIANZA_BRONCE;
    }
}
