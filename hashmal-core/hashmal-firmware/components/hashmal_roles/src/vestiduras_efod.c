/**
 * ===================================================================
 * PIEZA 09 - BIGDEI HAQODESH - vestiduras_efod.c
 * Sub-pieza: AVNEI ZIKKARON - los hombros del efod
 * Exodo 28:6-14 + 1 Samuel 2:28 + Exodo 39:2-7
 * ===================================================================
 *
 * "ve'asu et ha'EFOD: zahav tejelet ve'argaman tola'at shani
 *  veshesh moshzar ma'ase joshev." - Exodo 28:6
 *
 * "velaqajta et shtei avnei SHOHAM ufittajta 'aleyhem shemot benei
 *  Yisrael: SHESHA mishemotam 'al ha'even ha'ejat, ve'et shemot
 *  hasheshah hannotarim 'al ha'even hashshenit keTOLEDOTAM."
 *    - Exodo 28:9-10
 * "Tomaras dos piedras de ONICE y grabaras en ellas los nombres
 *  de los hijos de Israel: SEIS nombres en una piedra, y los otros
 *  SEIS nombres en la otra piedra, segun su ORDEN DE NACIMIENTO."
 *
 * "ma'ase JARASH even PITUJEI JOTAM tefattaj et shtei ha'avanim 'al
 *  shemot benei Yisrael, musabbot MISHBETSOT zahav ta'aseh otam.
 *  vesamta et shtei ha'avanim 'al kitfot ha'efod AVNEI ZIKKARON
 *  livnei Yisrael, veNASA Aharon et shemotam lifnei YHWH 'al shtei
 *  khetefav leZIKKARON." - Exodo 28:11-12
 * "Obra de grabador de piedra, grabados como SELLO, grabaras las dos
 *  piedras segun los nombres de los hijos de Israel, engastadas en
 *  ENGASTES de oro. Pondras las dos piedras sobre las hombreras del
 *  efod como PIEDRAS DE MEMORIAL para los hijos de Israel; y Aaron
 *  CARGARA sus nombres delante de YHWH sobre sus dos hombros, COMO
 *  MEMORIAL."
 *
 * "va'evjar oto mikkol shivtei Yisrael li leKOHEN... velijbor EFOD
 *  lefanay." - 1 Samuel 2:28
 * "Lo elegi de todas las tribus de Israel para Mi como sacerdote...
 *  y para llevar EFOD ante Mi."
 *
 * "vayya'asu et AVNEI hashoham musabbot MISHBETSOT zahav mefuttajot
 *  PITUJEI JOTAM 'al shemot benei Yisrael. vayyasem otam 'al kitfot
 *  ha'efod AVNEI ZIKKARON livnei Yisrael." - Exodo 39:6-7
 * "Labraron las piedras de onice engastadas en engastes de oro,
 *  grabadas como sello con los nombres de los hijos de Israel; y
 *  las pusieron sobre las hombreras del efod, piedras de MEMORIAL
 *  para los hijos de Israel."
 *
 * --- VOCABULARIO ---
 *
 *   efod           (APWD, H646)             Chaleco de autoridad.
 *   ketefot        (KTPT, H3802 pl)       HOMBRERAS - unen
 *                                              por encima.
 *   avnei shoham   (ABNY ShHM, H68+H7718) Piedras de ONICE -
 *                                              2, una por hombro.
 *   pitujei jotam  (PTWChY ChTM,
 *                   H6603+H2368)                Grabado como SELLO -
 *                                              irreversible.
 *   mishbetsot     (MShBTsWT, H4865 pl)    ENGASTES de oro - el
 *                                              engaste valida la piedra.
 *   zikkaron       (ZKRN, H2146)            MEMORIAL, recuerdo -
 *                                              el sacerdote "carga"
 *                                              los nombres.
 *   nasa           (NShA, H5375)              CARGAR, llevar.
 *   toledot        (TWLDT, H8435)           GENERACIONES / orden
 *                                              de nacimiento.
 *   keToledotam    (KTWLDTM)                 segun su orden - Ex
 *                                              28:10 fija el orden.
 *   sharsherot     (ShRShRT, H8333)          CADENAS de oro puro.
 *   'avotot        ('BTT, H5688)              CORDONES trenzados.
 *
 * --- KETOLEDOTAM - ORDEN DE NACIMIENTO (Ex 28:10) ---
 *
 * El Padre fija el ORDEN: seis en la piedra derecha, seis en la
 * izquierda, "keToledotam" - por orden de nacimiento. No es
 * alfabetico ni arbitrario: respeta la llegada. En firmware:
 * registro FIFO por hombro. Los primeros 6 nodos ocupan el hombro
 * derecho; del 7o al 12o llenan el izquierdo. Cuando un nodo se
 * retira, su hueco NO se compacta - queda centinela 0xFF, para
 * que el orden original de llegada sea legible siempre (zikkaron
 * historico, v.12).
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   hashmal_roles_efod_registrar         Ex 28:9-12  grabar nombre.
 *   hashmal_roles_efod_quitar            salida controlada.
 *   hashmal_roles_efod_esta_registrado   lectura pura bool.
 *   hashmal_roles_efod_estado            snapshot del efod.
 *
 *   + helpers estaticos:
 *   hombro_y_slot_libre            siguiente hueco keToledotam.
 *   buscar_en_hombros              localiza un nodo_id.
 *   actualizar_memorial            Ex 28:12 memorial on/off.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_roles.h
 *   hashmal_roles_internal.h
 *   esp_log.h    (ORO EGIPTO)  logging
 *
 * Autor: Antonio Jose Marin Soto (Betsalel - Ex 31:2)
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "esp_log.h"

#include "hashmal_roles.h"
#include "hashmal_roles_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *EFOD_TAG = "hashmal.roles.efod";

#define EFOD_LOG_I(fmt, ...)  ESP_LOGI(EFOD_TAG, fmt, ##__VA_ARGS__)
#define EFOD_LOG_W(fmt, ...)  ESP_LOGW(EFOD_TAG, fmt, ##__VA_ARGS__)
#define EFOD_LOG_E(fmt, ...)  ESP_LOGE(EFOD_TAG, fmt, ##__VA_ARGS__)

/* Centinela compartido con vestiduras_roles.c - definido aqui
 * nuevamente para ambito de traduccion propio. Valor identico. */
#define EFOD_NODO_ID_VACIO   0xFFu

/* ==================================================================
 * HELPERS ESTATICOS
 * ================================================================== */

/**
 * ?Nodo_id dentro del rango valido [0..MAX_NODOS-1]? El centinela
 * 0xFF queda fuera por definicion.
 */
static inline bool nodo_id_valido(uint8_t nodo_id)
{
    return nodo_id < HASHMAL_ROLES_MAX_NODOS;
}

/**
 * Localiza el siguiente hueco libre respetando keToledotam
 * (Ex 28:10): primero hombro derecho (posiciones 0..5), luego
 * izquierdo (6..11 vistos como 0..5 del hombro izquierdo).
 *
 * Salida:
 *   *is_derecho = true si el hueco esta en el hombro derecho.
 *   *slot       = indice [0..HASHMAL_ROLES_EFOD_HOMBRO-1].
 * Retorno:
 *   true  si hay hueco.
 *   false si ambos hombros estan llenos (efod completo).
 */
static bool hombro_y_slot_libre(const hashmal_roles_efod_t *efod,
                                bool *is_derecho,
                                uint8_t *slot)
{
    for (uint8_t i = 0; i < HASHMAL_ROLES_EFOD_HOMBRO; i++) {
        if (efod->hombro_derecho[i] == EFOD_NODO_ID_VACIO) {
            *is_derecho = true;
            *slot       = i;
            return true;
        }
    }
    for (uint8_t i = 0; i < HASHMAL_ROLES_EFOD_HOMBRO; i++) {
        if (efod->hombro_izquierdo[i] == EFOD_NODO_ID_VACIO) {
            *is_derecho = false;
            *slot       = i;
            return true;
        }
    }
    return false;
}

/**
 * Busca un nodo_id en ambos hombros. Si lo encuentra, devuelve
 * true y escribe en *is_derecho[slot. Usa comparacion directa
 * contra el centinela para no confundir "vacio" con "encontrado".
 */
static bool buscar_en_hombros(const hashmal_roles_efod_t *efod,
                              uint8_t nodo_id,
                              bool *is_derecho,
                              uint8_t *slot)
{
    if (nodo_id == EFOD_NODO_ID_VACIO) return false;

    for (uint8_t i = 0; i < HASHMAL_ROLES_EFOD_HOMBRO; i++) {
        if (efod->hombro_derecho[i] == nodo_id) {
            *is_derecho = true;
            *slot       = i;
            return true;
        }
    }
    for (uint8_t i = 0; i < HASHMAL_ROLES_EFOD_HOMBRO; i++) {
        if (efod->hombro_izquierdo[i] == nodo_id) {
            *is_derecho = false;
            *slot       = i;
            return true;
        }
    }
    return false;
}

/**
 * Ex 28:12 "lezikkaron" - el memorial esta activo cuando hay al
 * menos un nombre cargado. Aaron "nasa et shemotam": si no hay
 * nombres, no hay carga; cuando llega el primero, la memoria se
 * enciende.
 */
static void actualizar_memorial(hashmal_roles_efod_t *efod)
{
    const bool nuevo = (efod->total_registrados > 0u);
    if (nuevo != efod->memorial_activo) {
        efod->memorial_activo = nuevo;
        if (nuevo) {
            EFOD_LOG_I("memorial ACTIVO (Éx 28:12 lezikkarón) — "
                       "Aharón venasá et shemotám lifnéi YHWH");
        } else {
            EFOD_LOG_W("memorial en reposo — hombros vacíos "
                       "(Éx 28:12 sin nombres que cargar)");
        }
    }
}

/* ==================================================================
 * hashmal_roles_efod_registrar - Exodo 28:9-12
 *
 * "vefittajta 'aleyhem shemot benei Yisrael: shesha mishemotam 'al
 *  ha'even ha'ejat, ve'et shemot hasheshah hannotarim 'al ha'even
 *  hashshenit keToledotam."
 *
 * Graba un nombre en la piedra de onice: encuentra el primer hueco
 * segun keToledotam, coloca el nodo_id, engasta (mishbetsa) el
 * registro incrementando total_registrados, y eleva el memorial si
 * procede.
 *
 * Validaciones (en orden):
 *   1. Subsistema VESTIDO (singleton disponible).
 *   2. nodo_id dentro de [0..MAX_NODOS-1].
 *   3. nodo_id no registrado previamente -> error informativo,
 *      no se duplica nombre (v.11 pitujei jotam - el sello es unico).
 *   4. Hay hueco libre (total_registrados < MAX_NODOS).
 *
 * 2 testigos:
 *   Ex 28:9-12 - mandato del registro con pitujei jotam
 *   Ex 39:6-7  - ejecucion por Betsalel en el taller
 *   1 Sa 2:28  - "velijbor efod lefanay" (libro distinto)
 * ================================================================== */
hashmal_roles_err_t hashmal_roles_efod_registrar(uint8_t nodo_id)
{
    hashmal_roles_estado_roles_t *r = hashmal_roles_vestiduras_mut();
    if (r == NULL) {
        EFOD_LOG_E("registrar: sistema no vestido (init primero)");
        return HASHMAL_ROLES_ERR_INIT;
    }

    if (!nodo_id_valido(nodo_id)) {
        EFOD_LOG_W("registrar: nodo_id %u fuera de rango [0..%u]",
                   (unsigned)nodo_id,
                   (unsigned)(HASHMAL_ROLES_MAX_NODOS - 1u));
        return HASHMAL_ROLES_ERR_NULL;
    }

    /* Ex 28:11 "pitujei jotam" - el sello es unico. No se graba el
     * mismo nombre dos veces sobre el onice. */
    bool    is_derecho;
    uint8_t slot;
    if (buscar_en_hombros(&r->efod, nodo_id, &is_derecho, &slot)) {
        EFOD_LOG_I("registrar: nodo %u ya grabado en hombro %s[%u] "
                   "(Éx 28:11 pitujéi jotám — sello ya puesto)",
                   (unsigned)nodo_id,
                   is_derecho ? "derecho" : "izquierdo",
                   (unsigned)slot);
        return HASHMAL_ROLES_ERR_YA_VESTIDO;
    }

    /* Ex 28:9 "shtei avnei shoham" - solo dos piedras, 6+6 nombres. */
    if (r->efod.total_registrados >= HASHMAL_ROLES_MAX_NODOS) {
        EFOD_LOG_W("registrar: efod LLENO — 12 nombres ya grabados "
                   "(Éx 28:9 shtéi avnéi shóham, 6+6)");
        return HASHMAL_ROLES_ERR_NODO_LLENO;
    }

    /* KeToledotam (Ex 28:10) - primer hueco libre, derecho antes
     * que izquierdo. */
    if (!hombro_y_slot_libre(&r->efod, &is_derecho, &slot)) {
        /* Coherencia defensiva: total_registrados < MAX pero no hay
         * hueco -> corrupcion del singleton. */
        EFOD_LOG_E("registrar: contadores inconsistentes "
                   "(total=%u, sin hueco físico)",
                   (unsigned)r->efod.total_registrados);
        return HASHMAL_ROLES_ERR_NODO_LLENO;
    }

    if (is_derecho) {
        r->efod.hombro_derecho[slot] = nodo_id;
    } else {
        r->efod.hombro_izquierdo[slot] = nodo_id;
    }
    r->efod.total_registrados++;
    r->timestamp_ms = hashmal_roles_ahora_ms();

    EFOD_LOG_I("pitujéi jotám — nodo %u grabado en hombro %s[%u] "
               "(%u/12) — avnéi zikkarón (Éx 28:12 + Éx 39:6-7)",
               (unsigned)nodo_id,
               is_derecho ? "derecho" : "izquierdo",
               (unsigned)slot,
               (unsigned)r->efod.total_registrados);

    /* Ex 28:12 "lezikkaron" - el primer nombre enciende el memorial. */
    actualizar_memorial(&r->efod);

    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 * hashmal_roles_efod_quitar - salida controlada
 *
 * Un nodo se retira del efod (desregistro). El hueco NO se compacta:
 * queda centinela 0xFF para preservar el orden keToledotam original
 * (Ex 28:10) - el memorial historico no se reescribe. Si el nodo
 * era el ultimo, el memorial vuelve a reposo (v.12 sin nombres).
 *
 * Validaciones:
 *   1. VESTIDO.
 *   2. nodo_id dentro de rango.
 *   3. nodo_id existe en algun hombro (si no -> ERR_NODO_NO_ENCONTRADO).
 *
 * 2 testigos:
 *   Ex 28:12 - zikkaron (el registro es memorial, no se falsifica)
 *   Nm 20:26-28 - vehafshet et Aharon: vestiduras se quitan con orden
 * ================================================================== */
hashmal_roles_err_t hashmal_roles_efod_quitar(uint8_t nodo_id)
{
    hashmal_roles_estado_roles_t *r = hashmal_roles_vestiduras_mut();
    if (r == NULL) {
        EFOD_LOG_E("quitar: sistema no vestido (init primero)");
        return HASHMAL_ROLES_ERR_INIT;
    }

    if (!nodo_id_valido(nodo_id)) {
        EFOD_LOG_W("quitar: nodo_id %u fuera de rango [0..%u]",
                   (unsigned)nodo_id,
                   (unsigned)(HASHMAL_ROLES_MAX_NODOS - 1u));
        return HASHMAL_ROLES_ERR_NULL;
    }

    bool    is_derecho;
    uint8_t slot;
    if (!buscar_en_hombros(&r->efod, nodo_id, &is_derecho, &slot)) {
        EFOD_LOG_W("quitar: nodo %u no está grabado en el efod "
                   "(Éx 28:12 no hay memorial que retirar)",
                   (unsigned)nodo_id);
        return HASHMAL_ROLES_ERR_NODO_NO_ENCONTRADO;
    }

    if (is_derecho) {
        r->efod.hombro_derecho[slot] = EFOD_NODO_ID_VACIO;
    } else {
        r->efod.hombro_izquierdo[slot] = EFOD_NODO_ID_VACIO;
    }
    if (r->efod.total_registrados > 0u) {
        r->efod.total_registrados--;
    }
    r->timestamp_ms = hashmal_roles_ahora_ms();

    EFOD_LOG_I("nodo %u retirado del hombro %s[%u] (%u/12) — "
               "Nm 20:28 vestiduras se transfieren",
               (unsigned)nodo_id,
               is_derecho ? "derecho" : "izquierdo",
               (unsigned)slot,
               (unsigned)r->efod.total_registrados);

    actualizar_memorial(&r->efod);

    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 * hashmal_roles_efod_esta_registrado - lectura pura
 *
 * Ex 28:12 "lezikkaron" - cualquier componente consulta el memorial
 * sin alterarlo. Sin logs, sin side effects. Mismo patron que
 * vestiduras_roles.c::hashmal_roles_activo.
 *
 *   sin init / fuera de rango / no registrado -> false
 *   grabado                                   -> true
 * ================================================================== */
bool hashmal_roles_efod_esta_registrado(uint8_t nodo_id)
{
    const hashmal_roles_estado_roles_t *r =
        hashmal_roles_get_vestiduras_interno();
    if (r == NULL) return false;
    if (!nodo_id_valido(nodo_id)) return false;

    bool    is_derecho;
    uint8_t slot;
    return buscar_en_hombros(&r->efod, nodo_id, &is_derecho, &slot);
}

/* ==================================================================
 * hashmal_roles_efod_estado - snapshot del efod
 *
 * Lectura pura, copia por valor al buffer del llamante. Mismo
 * patron que hashmal_roles_estado pero limitado a la struct efod.
 *
 * 2 testigos:
 *   Ex 28:29 - al libo lifnei YHWH tamid (consultable siempre)
 *   Ex 28:12 - zikkaron (el memorial existe para ser visto)
 * ================================================================== */
hashmal_roles_err_t hashmal_roles_efod_estado(hashmal_roles_efod_t *out)
{
    if (out == NULL) return HASHMAL_ROLES_ERR_NULL;

    const hashmal_roles_estado_roles_t *r =
        hashmal_roles_get_vestiduras_interno();
    if (r == NULL) return HASHMAL_ROLES_ERR_INIT;

    memcpy(out, &r->efod, sizeof(*out));
    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 *
 *   "vesamta et shtei ha'avanim 'al kitfot ha'EFOD AVNEI ZIKKARON
 *    livnei Yisrael, veNASA Aharon et shemotam lifnei YHWH 'al
 *    shtei khetefav leZIKKARON." - Exodo 28:12
 *
 *   "vayyasem otam 'al kitfot ha'efod AVNEI ZIKKARON livnei
 *    Yisrael." - Exodo 39:7
 *
 *   "va'evjar oto mikkol shivtei Yisrael li leKOHEN... velijbor
 *    EFOD lefanay." - 1 Samuel 2:28
 *
 *   El efod no es adorno: es CARGA. Aaron no exhibe los nombres -
 *   los lleva. "venasa et shemotam" dos veces en el texto (v.12
 *   y v.29): una para los hombros, otra para el corazon. Aqui
 *   cumplimos la primera: seis en la piedra derecha, seis en la
 *   izquierda, keToledotam - FIFO por llegada. El hueco de quien
 *   parte no se compacta; el memorial conserva el orden historico.
 *   Y el sello, pitujei jotam, se graba UNA sola vez por nombre:
 *   registrar dos veces es duplicar el sello, y eso el Padre no
 *   lo hace.
 *
 *   === AVNEI ZIKKARON - vestiduras_efod.c COMPLETO ===
 *
 *   Archivo 2 de 7 de las Vestiduras. Siguiente:
 *     vestiduras_pectoral.c - las 12 piedras del juicio (Ex 28:15-30)
 *
 *   PASO 9 de Exodo 40:12-15 - Pieza 09, Archivo 2 de 7.
 *
 * ================================================================== */
