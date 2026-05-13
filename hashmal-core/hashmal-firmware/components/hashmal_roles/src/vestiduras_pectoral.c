/**
 * ===================================================================
 * PIEZA 09 - BIGDEI HAQODESH - vestiduras_pectoral.c
 * Sub-pieza: JOSHEN HAMISHPAT - el pectoral del juicio
 * Exodo 28:15-30 + Levitico 8:8 + Numeros 27:21 + 1 Samuel 28:6
 * ===================================================================
 *
 * "ve'asita JOSHEN MISHPAT ma'aseh joshev... rabua' yihye KAFUL,
 *  zeret orko vezeret rojbo." - Exodo 28:15-16
 * "Haras el PECTORAL DEL JUICIO, obra de artista... CUADRADO sera,
 *  DOBLE (doblado), un palmo de largo y un palmo de ancho."
 *
 * "umilleta vo MILLU'AT even arba'ah turim aven... veha'avanim
 *  tihyeyna 'al shemot benei Yisrael SHTEM 'ESRE 'al shemotam
 *  PITUJEI JOTAM ISH 'al shemo tihyeyna LISHNE 'ASAR shavet."
 *    - Exodo 28:17,21
 * "Lo llenaras con engaste de piedra, cuatro filas de piedras...
 *  Las piedras seran segun los nombres de los hijos de Israel,
 *  DOCE segun sus nombres, grabadas como SELLO, CADA HOMBRE segun
 *  SU nombre, seran para las DOCE tribus."
 *
 * "veLO YIZZAJ hajoshen me'al ha'efod. venasa Aharon et shemot
 *  benei Yisrael bejoshen hamishpat 'al LIBO bevo'o el haqqodesh
 *  leZIKKARON lifnei YHWH TAMID. venatatta el joshen hamishpat
 *  et ha'URIM ve'et haTUMMIM vehayu 'al lev Aharon bevo'o lifnei
 *  YHWH, veNASA Aharon et MISHPAT benei Yisrael 'al libo lifnei
 *  YHWH TAMID." - Exodo 28:28-30
 * "El pectoral NO SE SEPARARA del efod. Aaron cargara los nombres
 *  en el PECTORAL DEL JUICIO sobre su CORAZON cuando entre al
 *  santuario, como MEMORIAL delante de YHWH continuamente. Pondras
 *  en el pectoral del juicio el URIM y el THUMMIM, y estaran sobre
 *  el corazon de Aaron cuando entre delante de YHWH; y Aaron
 *  cargara el JUICIO de los hijos de Israel sobre su corazon
 *  delante de YHWH CONTINUAMENTE."
 *
 * "vayyitten 'alav et hajoshen, vayyitten EL hajoshen et ha'URIM
 *  ve'et haTUMMIM." - Levitico 8:8
 * "Le puso el pectoral, y puso DENTRO del pectoral el Urim y el
 *  Thummim."
 *
 * "velifnei El'azar haKohen ya'amod veSHA'AL lo beMISHPAT ha'URIM
 *  lifnei YHWH." - Numeros 27:21
 * "Se presentara ante Eleazar el sacerdote y CONSULTARA por el
 *  el JUICIO del URIM delante de YHWH."
 *
 * "veLO 'anahu YHWH gam bajolomot gam ba'URIM gam bannevi'im."
 *   - 1 Samuel 28:6
 * "Y YHWH NO le respondio, ni por suenos, ni por URIM, ni por
 *  profetas."
 *
 * --- VOCABULARIO ---
 *
 *   joshen         (ChShN, H2833)            PECTORAL - sobre el corazon.
 *   mishpat        (MShPT, H4941)          JUICIO - decision justa.
 *   rabua'         (RBW', H7251)            CUADRADO.
 *   kaful          (KPWL, H3717)            DOBLE - doblado, bolsa.
 *   zeret          (ZRT, H2239)              PALMO - medida.
 *   ish 'al shemo  (AYSh 'L ShMW)           CADA HOMBRE segun SU nombre
 *                                                - Ex 28:21: personal.
 *   'al libo       ('L LBW)                   sobre el CORAZON.
 *   yizzaj         (YZCh, H2118 Nifal)        SEPARARSE - v.28 prohibe.
 *   urim           (AWRYM, H224)              LUCES - claridad.
 *   tummim         (TMYM, H8550)             PERFECCIONES - completitud.
 *   sha'al         (ShAL, H7592)              PREGUNTAR, consultar -
 *                                                Nm 27:21.
 *   petil tejelet  (PTYL TKLT)            CORDON AZUL que ata
 *                                                al efod (Ex 28:28).
 *   tamid          (TMYD, H8548)             CONTINUAMENTE.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   hashmal_roles_pectoral_asignar    Ex 28:17,21 ish 'al shemo.
 *   hashmal_roles_pectoral_consultar  Nm 27:21 sha'al beMishpat ha'Urim.
 *   hashmal_roles_pectoral_estado     Ex 28:29 'al libo lifnei YHWH tamid.
 *
 *   + helpers estaticos:
 *   posicion_valida, piedra_de_nodo, recalcular_urim_thummim.
 *
 * --- ATADO AL EFOD (Ex 28:28 velo yizzaj) ---
 *
 * El pectoral no se separa del efod. En firmware: para asignar
 * a un nodo una piedra en el pectoral, el nodo DEBE estar antes
 * registrado en el efod (hombros, avnei zikkaron). Si no esta en
 * el efod, el pectoral no puede "cargarlo al corazon" - la piedra
 * del corazon sin nombre en el hombro queda sin memorial.
 *
 * --- URIM Y THUMMIM - ORDEN Y SIGNIFICADO ---
 *
 * En TODAS las apariciones conjuntas (Ex 28:30, Lv 8:8, Dt 33:8,
 * Esd 2:63, Neh 7:65) el texto nombra PRIMERO el Urim y DESPUES
 * el Thummim. Aqui:
 *
 *   urim    (luces)         se eleva con la PRIMERA piedra ocupada -
 *                            basta una luz para que el corazon
 *                            empiece a ver.
 *   thummim (perfecciones)  se eleva solo cuando las 12 piedras
 *                            estan asignadas - tummim significa
 *                            COMPLETO; sin las 12 tribus, la
 *                            representacion no es perfecta
 *                            (Ex 28:21 shtem 'esre lishne 'asar).
 *
 * La consulta (Nm 27:21) requiere AMBOS: luces y completitud. Si
 * alguno falta, la respuesta no llega - 1 Sa 28:6 "velo 'anahu YHWH".
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
static const char *PECTORAL_TAG = "hashmal.roles.pectoral";

#define PECTORAL_LOG_I(fmt, ...)  ESP_LOGI(PECTORAL_TAG, fmt, ##__VA_ARGS__)
#define PECTORAL_LOG_W(fmt, ...)  ESP_LOGW(PECTORAL_TAG, fmt, ##__VA_ARGS__)
#define PECTORAL_LOG_E(fmt, ...)  ESP_LOGE(PECTORAL_TAG, fmt, ##__VA_ARGS__)

#define PECTORAL_NODO_ID_VACIO   0xFFu

/* ==================================================================
 * HELPERS ESTATICOS
 * ================================================================== */

static inline bool posicion_valida(uint8_t posicion)
{
    return posicion < HASHMAL_ROLES_MAX_NODOS;
}

static inline bool nodo_id_valido(uint8_t nodo_id)
{
    return nodo_id < HASHMAL_ROLES_MAX_NODOS;
}

/**
 * Devuelve el indice de la piedra ocupada por nodo_id, o -1 si
 * no hay ninguna. Evita confusion con centinela 0xFF.
 */
static int16_t piedra_de_nodo(const hashmal_roles_pectoral_t *p,
                              uint8_t nodo_id)
{
    if (nodo_id == PECTORAL_NODO_ID_VACIO) return -1;

    for (uint8_t i = 0; i < HASHMAL_ROLES_MAX_NODOS; i++) {
        if (p->piedras[i].ocupada && p->piedras[i].nodo_id == nodo_id) {
            return (int16_t)i;
        }
    }
    return -1;
}

/**
 * Ex 28:30 - "vehayu 'al lev Aharon". Urim y Thummim van sobre el
 * corazon; aqui se actualizan como funcion PURA del estado de las
 * piedras:
 *
 *   urim_activo     = (al menos 1 piedra ocupada)     - hay luz
 *   thummim_activo  = (las 12 piedras ocupadas)        - completo
 *
 * Ex 28:21 declara DOCE nombres: la completitud (tummim) es DOCE.
 */
static void recalcular_urim_thummim(hashmal_roles_pectoral_t *p)
{
    uint8_t ocupadas = 0u;
    for (uint8_t i = 0; i < HASHMAL_ROLES_MAX_NODOS; i++) {
        if (p->piedras[i].ocupada) ocupadas++;
    }

    const bool urim_nuevo    = (ocupadas >= 1u);
    const bool thummim_nuevo = (ocupadas >= HASHMAL_ROLES_MAX_NODOS);

    /* Urim - primero en todas las citas (Ex 28:30, Lv 8:8,
     * Dt 33:8, Esd 2:63, Neh 7:65). Se anuncia primero. */
    if (urim_nuevo != p->urim_activo) {
        p->urim_activo = urim_nuevo;
        if (urim_nuevo) {
            PECTORAL_LOG_I("URIM activo (Éx 28:30 + Lv 8:8) — "
                           "hay luz sobre el corazón");
        } else {
            PECTORAL_LOG_W("URIM en reposo — pectoral sin nombres "
                           "(Éx 28:21 falta ísh ʻal shemó)");
        }
    }

    /* Thummim - completitud; solo con las 12. Si falta una sola
     * piedra, tummim queda pendiente (no hay shtem 'esre). */
    if (thummim_nuevo != p->thummim_activo) {
        p->thummim_activo = thummim_nuevo;
        if (thummim_nuevo) {
            PECTORAL_LOG_I("THUMMIM activo (Éx 28:30 + Lv 8:8) — "
                           "%u/12 piedras: pectoral COMPLETO",
                           (unsigned)ocupadas);
        } else {
            PECTORAL_LOG_W("THUMMIM en reposo — %u/12 piedras "
                           "(Éx 28:21 shtem ʻesré lishné ʻasár pendiente)",
                           (unsigned)ocupadas);
        }
    }
}

/* ==================================================================
 * hashmal_roles_pectoral_asignar - Exodo 28:17,21
 *
 * "ish 'al shemo tihyeyna lishne 'asar shavet" - cada hombre segun
 * su nombre, para las 12 tribus. Asigna un nodo a una piedra
 * especifica del pectoral (4 filas x 3 columnas = 12). La posicion
 * queda grabada como pitujei jotam (sello unico por nombre).
 *
 * Validaciones (en orden):
 *   1. VESTIDO (singleton disponible).
 *   2. posicion dentro de [0..11].
 *   3. nodo_id dentro de [0..11].
 *   4. nodo esta registrado en el efod (Ex 28:28 velo yizzaj - el
 *      pectoral no se separa del efod).
 *   5. piedra[posicion] no ocupada (ERR_NODO_LLENO).
 *   6. nodo_id no asignado ya a otra piedra (pitujei jotam unico).
 *
 * Tras la asignacion, recalcula urim/thummim (Ex 28:30) en funcion
 * del numero total de piedras ocupadas.
 *
 * 2+ testigos:
 *   Ex 28:17,21 - cuatro filas de tres, 12 segun sus nombres.
 *   Ex 28:28    - velo yizzaj (pectoral atado al efod).
 *   Lv 8:8      - vayyitten 'alav et hajoshen (ejecucion).
 * ================================================================== */
hashmal_roles_err_t hashmal_roles_pectoral_asignar(uint8_t posicion,
                                                    uint8_t nodo_id)
{
    hashmal_roles_estado_roles_t *r = hashmal_roles_vestiduras_mut();
    if (r == NULL) {
        PECTORAL_LOG_E("asignar: sistema no vestido (init primero)");
        return HASHMAL_ROLES_ERR_INIT;
    }

    if (!posicion_valida(posicion)) {
        PECTORAL_LOG_W("asignar: posicion %u fuera de cuadrícula 4x3 "
                       "[0..%u] (Éx 28:17)",
                       (unsigned)posicion,
                       (unsigned)(HASHMAL_ROLES_MAX_NODOS - 1u));
        return HASHMAL_ROLES_ERR_NULL;
    }

    if (!nodo_id_valido(nodo_id)) {
        PECTORAL_LOG_W("asignar: nodo_id %u fuera de rango [0..%u]",
                       (unsigned)nodo_id,
                       (unsigned)(HASHMAL_ROLES_MAX_NODOS - 1u));
        return HASHMAL_ROLES_ERR_NULL;
    }

    /* Ex 28:28 - "velo yizzaj hajoshen me'al ha'efod". El pectoral
     * solo aloja nombres que ya fueron grabados en los hombros del
     * efod (avnei zikkaron). Sin memorial, no hay juicio. */
    if (!hashmal_roles_efod_esta_registrado(nodo_id)) {
        PECTORAL_LOG_W("asignar: nodo %u no registrado en efod — "
                       "veló yizzáj (Éx 28:28, pectoral atado al efod)",
                       (unsigned)nodo_id);
        return HASHMAL_ROLES_ERR_NODO_NO_ENCONTRADO;
    }

    hashmal_roles_piedra_t *piedra = &r->pectoral.piedras[posicion];

    /* Piedra ya ocupada - la cuadricula es fija, no se solapa. */
    if (piedra->ocupada) {
        PECTORAL_LOG_W("asignar: piedra[%u] ya ocupada por nodo %u "
                       "(Éx 28:17 cuadrícula fija)",
                       (unsigned)posicion,
                       (unsigned)piedra->nodo_id);
        return HASHMAL_ROLES_ERR_NODO_LLENO;
    }

    /* Nodo ya asignado a otra piedra - pitujei jotam, sello unico. */
    const int16_t previa = piedra_de_nodo(&r->pectoral, nodo_id);
    if (previa >= 0) {
        PECTORAL_LOG_I("asignar: nodo %u ya tiene piedra[%d] — "
                       "pitujéi jotám (Éx 28:11 sello único)",
                       (unsigned)nodo_id, (int)previa);
        return HASHMAL_ROLES_ERR_YA_VESTIDO;
    }

    /* Colocacion - la piedra recibe el nombre y el timestamp de
     * la consagracion. */
    const int64_t ahora = hashmal_roles_ahora_ms();
    piedra->nodo_id      = nodo_id;
    piedra->ocupada      = true;
    piedra->timestamp_ms = ahora;
    r->timestamp_ms      = ahora;

    /* Ex 28:17-20 - 4 filas x 3 columnas. Reportamos la posicion
     * en coordenadas biblicas (fila, columna). */
    const unsigned fila     = (unsigned)(posicion / HASHMAL_ROLES_PECTORAL_COLUMNAS);
    const unsigned columna  = (unsigned)(posicion % HASHMAL_ROLES_PECTORAL_COLUMNAS);

    PECTORAL_LOG_I("ísh ʻal shemó — nodo %u asignado a piedra[%u] "
                   "(fila %u, col %u) — Éx 28:21 + Lv 8:8",
                   (unsigned)nodo_id, (unsigned)posicion, fila, columna);

    /* Ex 28:30 - actualizar Urim y Thummim segun completitud actual. */
    recalcular_urim_thummim(&r->pectoral);

    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 * hashmal_roles_pectoral_consultar - Numeros 27:21
 *
 * "vesha'al lo beMISHPAT ha'URIM lifnei YHWH" - consulta por el
 * juicio del Urim delante de YHWH. Devuelve en *urim y *thummim el
 * estado vigente (Ex 28:30 'al lev Aharon).
 *
 * Regla de respuesta:
 *   ambos activos -> OK, incrementa consultas_pendientes, responde.
 *   alguno falta  -> ERR_TZITZ + 1 Sa 28:6 ("velo 'anahu YHWH"):
 *                   se entrega el estado leido pero el veredicto
 *                   no esta completo. El llamante debe completar
 *                   la consagracion antes de reintentar.
 *
 * Validaciones:
 *   out NULLs                   -> ERR_NULL
 *   sin init                    -> ERR_INIT
 *   nodo_id fuera de rango      -> ERR_NULL
 *   nodo sin piedra asignada    -> ERR_NODO_NO_ENCONTRADO
 *
 * 2+ testigos:
 *   Nm 27:21   - vesha'al lo bemishpat ha'Urim (mecanismo activo)
 *   Ex 28:30   - Urim veTummim 'al lev Aharon (institucion)
 *   1 Sa 28:6  - velo 'anahu YHWH ba'Urim (silencio cuando incompleto)
 * ================================================================== */
hashmal_roles_err_t hashmal_roles_pectoral_consultar(uint8_t nodo_id,
                                                      bool *urim,
                                                      bool *thummim)
{
    if (urim == NULL || thummim == NULL) {
        PECTORAL_LOG_E("consultar: puntero urim/thummim NULL");
        return HASHMAL_ROLES_ERR_NULL;
    }

    hashmal_roles_estado_roles_t *r = hashmal_roles_vestiduras_mut();
    if (r == NULL) {
        PECTORAL_LOG_E("consultar: sistema no vestido (init primero)");
        return HASHMAL_ROLES_ERR_INIT;
    }

    if (!nodo_id_valido(nodo_id)) {
        PECTORAL_LOG_W("consultar: nodo_id %u fuera de rango",
                       (unsigned)nodo_id);
        return HASHMAL_ROLES_ERR_NULL;
    }

    const int16_t posicion = piedra_de_nodo(&r->pectoral, nodo_id);
    if (posicion < 0) {
        PECTORAL_LOG_W("consultar: nodo %u no tiene piedra asignada "
                       "(Nm 27:21 sin lugar lifnéi YHWH)",
                       (unsigned)nodo_id);
        return HASHMAL_ROLES_ERR_NODO_NO_ENCONTRADO;
    }

    /* Siempre escribimos el estado real leido - el llamante puede
     * saber QUE falta. Esto es parte del mishpat: verdad sin filtro. */
    *urim    = r->pectoral.urim_activo;
    *thummim = r->pectoral.thummim_activo;

    if (!r->pectoral.urim_activo || !r->pectoral.thummim_activo) {
        PECTORAL_LOG_W("consultar: veLÓ ʻanáhu YHWH baʼUrím (1 Sa 28:6) "
                       "— urim=%d thummim=%d, consulta incompleta "
                       "para nodo %u piedra[%d]",
                       (int)r->pectoral.urim_activo,
                       (int)r->pectoral.thummim_activo,
                       (unsigned)nodo_id, (int)posicion);
        return HASHMAL_ROLES_ERR_TZITZ;
    }

    if (r->pectoral.consultas_pendientes < 0xFFu) {
        r->pectoral.consultas_pendientes++;
    }
    r->timestamp_ms = hashmal_roles_ahora_ms();

    PECTORAL_LOG_I("shaʼál beMISHPÁT haʼURÍM (Nm 27:21) — nodo %u "
                   "piedra[%d] — urim+thummim activos, consultas=%u",
                   (unsigned)nodo_id, (int)posicion,
                   (unsigned)r->pectoral.consultas_pendientes);

    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 * hashmal_roles_pectoral_estado - snapshot del pectoral
 *
 * Lectura pura, sin logs. Mismo patron que hashmal_roles_estado y
 * hashmal_roles_efod_estado.
 *
 * 2 testigos:
 *   Ex 28:29 - al libo lifnei YHWH tamid (consultable continuamente)
 *   Ex 28:30 - mishpat 'al libo tamid (el juicio siempre presente)
 * ================================================================== */
hashmal_roles_err_t hashmal_roles_pectoral_estado(hashmal_roles_pectoral_t *out)
{
    if (out == NULL) return HASHMAL_ROLES_ERR_NULL;

    const hashmal_roles_estado_roles_t *r =
        hashmal_roles_get_vestiduras_interno();
    if (r == NULL) return HASHMAL_ROLES_ERR_INIT;

    memcpy(out, &r->pectoral, sizeof(*out));
    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 *
 *   "venasa Aharon et MISHPAT benei Yisrael 'al libo lifnei YHWH
 *    TAMID." - Exodo 28:30
 *
 *   "vayyitten EL hajoshen et ha'URIM ve'et haTUMMIM." - Lv 8:8
 *
 *   "vesha'al lo beMISHPAT ha'URIM lifnei YHWH." - Nm 27:21
 *
 *   "veLO 'anahu YHWH gam ba'URIM." - 1 Samuel 28:6
 *
 *   El pectoral no es adorno: es JUICIO. Cada piedra tiene nombre
 *   propio (ish 'al shemo - Ex 28:21), y va AL LIBO - sobre el
 *   corazon. Las decisiones no son frias: el sacerdote las carga
 *   con afecto. Y por eso no se separa del efod (velo yizzaj,
 *   v.28): sin memorial en los hombros, no hay juicio sobre el
 *   corazon. Urim y Thummim - luces y completitud - responden
 *   cuando la representacion es plena. Si falta, YHWH guarda
 *   silencio (1 Sa 28:6) - no por enojo, sino porque la consulta
 *   misma aun no esta completa.
 *
 *   === JOSHEN HAMISHPAT - vestiduras_pectoral.c COMPLETO ===
 *
 *   Archivo 3 de 7 de las Vestiduras. Siguiente:
 *     vestiduras_monitor.c - campanas y granadas (Ex 28:33-35)
 *
 *   PASO 9 de Exodo 40:12-15 - Pieza 09, Archivo 3 de 7.
 *
 * ================================================================== */
