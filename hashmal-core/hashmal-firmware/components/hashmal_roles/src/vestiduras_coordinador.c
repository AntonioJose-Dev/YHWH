/**
 * ===================================================================
 * PIEZA 09 - BIGDEI HAQODESH - vestiduras_coordinador.c
 * Sub-pieza: HAQREV VEHILBISH - designar y vestir a otros
 * Exodo 28:1,41 + Exodo 29:5 + Levitico 8:7 +
 *   Zacarias 3:3-5 + Isaias 61:10 + Hebreos 5:4 + 1 Pedro 2:9
 * ===================================================================
 *
 * "ve'attah HAQREV eleja et Aharon ajija ve'et banav itto mittoj
 *  benei Yisrael leJAHANO li." - Exodo 28:1
 * "Y TU haras ACERCAR a ti a Aaron tu hermano y a sus hijos con el,
 *  de en medio de los hijos de Israel, para que me SIRVAN como
 *  sacerdotes."
 *
 * "veHILBASHTA otam et Aharon ajija ve'et banav itto uMASHAJTA otam
 *  umille'TA et yadam veqiddashTA otam vejihanu li." - Exodo 28:41
 * "LOS VESTIRAS - a Aaron y a sus hijos - los UNGIRAS, LLENARAS
 *  sus manos y CONSAGRARAS, y me serviran como sacerdotes."
 *
 * "velaqajta et habegadim veHILBASHTA et Aharon et hakuttonet ve'et
 *  me'il ha'efod ve'et ha'efod ve'et hajoshen." - Exodo 29:5
 * "Tomaras las vestiduras y VESTIRAS a Aaron con la tunica, el
 *  manto del efod, el efod y el pectoral."
 *
 * "vayyitten 'alav et hakuttonet vayyajgor oto ba'avnet vayyalbesh
 *  oto et hamme'il vayyitten 'alav et ha'efod." - Levitico 8:7
 * "Le PUSO la tunica, lo cino con el cinturon, le vistio el manto
 *  y le puso el efod." - Moises EJECUTA; Aaron es vestido, no se
 *  viste.
 *
 * "viHoshua' hayah lavush begadim TSO'IM ve'omed lifnei hammal'aj.
 *  vayya'an vayyomer el ha'omedim lefanav lemor: HASIRU habegadim
 *  hatstso'im me'alav. vayyomer elav: re'eh he'evarti me'aleja
 *  'avoneja veHALBESH otja MAJALATSOT." - Zacarias 3:3-4
 * "Y Josue estaba vestido de vestiduras SUCIAS y estaba de pie
 *  delante del angel. Respondio y dijo a los que estaban delante
 *  de el: QUITADLE las vestiduras sucias de sobre el. Y le dijo:
 *  mira, HE QUITADO de ti tu iniquidad y te he VESTIDO con ropas
 *  de GALA."
 *
 * "sos asis baYHWH... ki HILBISHANI BIGDEI YESHA', me'il TSEDAQA
 *  ye'atani." - Isaias 61:10
 * "Con gozo me gozare en YHWH... porque me VISTIO con vestiduras
 *  de SALVACION, con manto de JUSTICIA me cubrio."
 *
 * "kai ouj heauto tis lambanei ten timen, alla kaloumenos hypo tou
 *  Theou kathosper kai Aaron." - Hebreos 5:4
 * "Nadie toma para si mismo este honor, sino el llamado por Dios,
 *  como lo fue Aaron."
 *
 * "hymeis de genos eklekton, BASILEION HIERATEUMA, ethnos hagion,
 *  laos eis peripoiesin." - 1 Pedro 2:9
 * "Vosotros sois linaje escogido, SACERDOCIO REAL, nacion santa,
 *  pueblo de posesion."
 *
 * --- VOCABULARIO ---
 *
 *   haqrev         (HQRB, H7126 Hif)        Hacer ACERCAR, designar.
 *   vehiblashta    (WHLBShT,
 *                    H3847 Hif)                 Y VESTIRAS - causativo:
 *                                                otro viste al nodo.
 *   vayyitten      (WYTN)                    Y PUSO - Lv 8:7,
 *                                                ejecucion.
 *   lejaheno       (LKHNW, H3547 Pi)        Para SERVIR como
 *                                                sacerdote.
 *   majalatsot     (MChLTsWT, H4254)          Ropas de GALA -
 *                                                Zac 3:4.
 *   tso'im         (TsWAYM, H6674)              SUCIAS - las
 *                                                vestiduras del fallo.
 *   he'evarti      (H'BRTY, H5674 Hif)     HE QUITADO -
 *                                                limpieza antes de
 *                                                revestir.
 *   bigdei yesha'  (BGDY YSh')               Vestiduras de
 *                                                SALVACION (Is 61:10).
 *   me'il tsedaqa  (M'YL TsDQH)                Manto de
 *                                                JUSTICIA (Is 61:10).
 *   hafshet        (HPShT, H6584 Hif)          DESVESTIR - Nm 20:26.
 *   basileion hierateuma (basileion ierateuma,
 *                         G935+G2406)            SACERDOCIO REAL
 *                                                (1 Pe 2:9).
 *   ouj heauto lambanei                          NADIE toma para si
 *                                                (Heb 5:4).
 *
 * --- PRINCIPIOS INQUEBRANTABLES DEL COORDINADOR ---
 *
 *   A. NADIE SE VISTE A SI MISMO - Heb 5:4 + Ex 28:41 (Hifil
 *      causativo). El coordinador actua SOBRE otros nodos. Este
 *      archivo no expone "autoasignacion".
 *
 *   B. TODOS PUEDEN SERVIR - 1 Pe 2:9 "basileion hierateuma".
 *      El enum tipo_rol_t refleja niveles; ningun nodo queda
 *      excluido por origen.
 *
 *   C. RESTAURACION, NO DESTRUCCION - Zac 3:4 + Is 61:10. El que
 *      falla se LIMPIA (majalatsot); no se borra del memorial
 *      del efod ni se raspa su tzitz.
 *
 *   D. DESIGNADO SOBRE REGISTRADO - Ex 28:28 "velo yizzaj":
 *      solo se viste a quien esta grabado en los hombros del
 *      efod (avnei zikkaron). Sin memorial, no hay designacion.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   hashmal_roles_vestir        Ex 28:41 + Lv 8:7  vestir por
 *                                 primera vez con un rol.
 *   hashmal_roles_desvestir     Nm 20:26-28  retirar rol sin
 *                                 destruir memorial ni tzitz.
 *   hashmal_roles_restaurar     Zac 3:3-5 + Is 61:10  limpiar
 *                                 y devolver majalatsot.
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

#include "esp_log.h"

#include "hashmal_roles.h"
#include "hashmal_roles_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *COORD_TAG = "hashmal.roles.coordinador";

#define COORD_LOG_I(fmt, ...)  ESP_LOGI(COORD_TAG, fmt, ##__VA_ARGS__)
#define COORD_LOG_W(fmt, ...)  ESP_LOGW(COORD_TAG, fmt, ##__VA_ARGS__)
#define COORD_LOG_E(fmt, ...)  ESP_LOGE(COORD_TAG, fmt, ##__VA_ARGS__)

#define COORD_NODO_ID_VACIO   0xFFu

/* ==================================================================
 * HELPERS ESTATICOS
 * ================================================================== */

static inline bool nodo_id_valido(uint8_t nodo_id)
{
    return nodo_id < HASHMAL_ROLES_MAX_NODOS;
}

static inline bool rol_valido(hashmal_roles_tipo_rol_t rol)
{
    /* NINGUNO es parte del enum pero no es una designacion valida
     * para vestir: Ex 32:25 peru'a - no se viste con "nada". */
    return (rol == HASHMAL_ROLES_ROL_ISRAELITA)
        || (rol == HASHMAL_ROLES_ROL_LEVI)
        || (rol == HASHMAL_ROLES_ROL_KOHEN)
        || (rol == HASHMAL_ROLES_ROL_KOHEN_GADOL);
}

static const char *nombre_rol(hashmal_roles_tipo_rol_t rol)
{
    switch (rol) {
    case HASHMAL_ROLES_ROL_NINGUNO:     return "NINGUNO (peruʻá)";
    case HASHMAL_ROLES_ROL_ISRAELITA:   return "ISRAELITA (basíleion hieráteuma)";
    case HASHMAL_ROLES_ROL_LEVI:        return "LEVI (hivdíl shévet haLeví)";
    case HASHMAL_ROLES_ROL_KOHEN:       return "KOHEN (lejahén lí)";
    case HASHMAL_ROLES_ROL_KOHEN_GADOL: return "KOHEN GADOL (shémen hamishjá)";
    default:                            return "<rol desconocido>";
    }
}

/* ==================================================================
 * hashmal_roles_vestir - Exodo 28:41 + Levitico 8:7 + Hebreos 5:4
 *
 * "vehiblashta otam" (Ex 28:41, Hifil causativo) - TU los vistes.
 * Lv 8:7 lo ejecuta: "vayyitten 'alav". El nodo es OBJETO del verbo,
 * no sujeto. Nadie se viste a si mismo (Heb 5:4).
 *
 * Validaciones (en orden):
 *   1. VESTIDO global (singleton disponible).
 *   2. nodo_id en rango [0..MAX_NODOS-1].
 *   3. rol entre los designables (rechaza NINGUNO por peru'a).
 *   4. Nodo registrado en efod (Ex 28:28 velo yizzaj: sin memorial,
 *      no hay designacion).
 *   5. Nodo aun no vestido (Heb 5:4: una designacion por persona,
 *      segunda investidura requiere restaurar).
 *
 * Tras vestir:
 *   - n->rol       = rol designado
 *   - n->vestido   = true
 *   - n->nodo_id   = nodo_id  (si aun tenia centinela)
 *   - nodos_activos del singleton se incrementa si paso de
 *     no-vestido a vestido.
 *   - timestamp_ms actualizado.
 *
 * 2+ testigos:
 *   Ex 28:41 - vehilbashta (mandato causativo)
 *   Lv 8:7   - vayyitten (ejecucion, libro distinto)
 *   Heb 5:4  - oudeis heauto lambanei (NT - nadie se autodesigna)
 *   1 Pe 2:9 - basileion hierateuma (NT - todos elegibles)
 * ================================================================== */
hashmal_roles_err_t hashmal_roles_vestir(uint8_t nodo_id,
                                          hashmal_roles_tipo_rol_t rol)
{
    hashmal_roles_estado_roles_t *r = hashmal_roles_vestiduras_mut();
    if (r == NULL) {
        COORD_LOG_E("vestir: sistema no vestido (init primero)");
        return HASHMAL_ROLES_ERR_INIT;
    }

    if (!nodo_id_valido(nodo_id)) {
        COORD_LOG_W("vestir: nodo_id %u fuera de rango [0..%u]",
                    (unsigned)nodo_id,
                    (unsigned)(HASHMAL_ROLES_MAX_NODOS - 1u));
        return HASHMAL_ROLES_ERR_NULL;
    }

    if (!rol_valido(rol)) {
        COORD_LOG_W("vestir: rol %d inválido (Éx 32:25 peruʻá — "
                    "no se viste con NINGUNO)",
                    (int)rol);
        return HASHMAL_ROLES_ERR_ROL_INVALIDO;
    }

    /* Ex 28:28 - velo yizzaj. El pectoral no se separa del efod;
     * analogamente, el rol no se asigna sin memorial previo. */
    if (!hashmal_roles_efod_esta_registrado(nodo_id)) {
        COORD_LOG_W("vestir: nodo %u sin memorial en efod — "
                    "Éx 28:12 zikkarón requerido antes de "
                    "haqrév (Éx 28:1)",
                    (unsigned)nodo_id);
        return HASHMAL_ROLES_ERR_NODO_NO_ENCONTRADO;
    }

    hashmal_roles_nodo_t *n = &r->nodos[nodo_id];

    /* Heb 5:4 - oudeis heauto lambanei. Una designacion por nodo;
     * reinvestir exige restaurar primero (Zac 3:4). */
    if (n->vestido) {
        COORD_LOG_I("vestir: nodo %u ya vestido como %s — "
                    "Heb 5:4 oudeís heautó lambánei (requiere "
                    "desvestir/restaurar para re-asignar)",
                    (unsigned)nodo_id, nombre_rol(n->rol));
        return HASHMAL_ROLES_ERR_YA_VESTIDO;
    }

    /* Designacion - el rol viene de arriba, la ejecucion la
     * hace el coordinador (Moises -> Aaron, Ex 29:5). */
    const int64_t ahora = hashmal_roles_ahora_ms();
    n->nodo_id     = nodo_id;
    n->rol         = rol;
    n->vestido     = true;
    /* El heartbeat no se fuerza aqui: sigue siendo responsabilidad
     * del nodo "taner la campana" para pasar a SONANDO. */
    if (r->nodos_activos < HASHMAL_ROLES_MAX_NODOS) {
        r->nodos_activos++;
    }
    r->timestamp_ms = ahora;

    COORD_LOG_I("haqrév — nodo %u designado como %s — "
                "Éx 28:41 vehilbáshta + Lv 8:7 vayyittén "
                "(1 Pe 2:9 basíleion hieráteuma)",
                (unsigned)nodo_id, nombre_rol(rol));

    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 * hashmal_roles_desvestir - Numeros 20:26-28
 *
 * "vehafshet et Aharon et begadav vehilbashtam et El'azar beno"
 *   - Nm 20:26
 * Las vestiduras se transfieren. El nombre en el efod y el sello
 * del tzitz NO se tocan: el efod es memorial historico (Ex 28:12
 * lezikkaron) y el tzitz es irreversible (Ex 28:36 pitujei jotam).
 * Solo se retira el rol operativo.
 *
 * Validaciones:
 *   1. VESTIDO global.
 *   2. nodo_id en rango.
 *   3. Nodo vestido (si no -> ERR_INIT informativo).
 *
 * 2 testigos:
 *   Nm 20:26-28 - vehafshet (vestiduras se transfieren, no destruyen)
 *   Nm 4:5-6    - vejissu (cubrir lo sagrado al migrar)
 * ================================================================== */
hashmal_roles_err_t hashmal_roles_desvestir(uint8_t nodo_id)
{
    hashmal_roles_estado_roles_t *r = hashmal_roles_vestiduras_mut();
    if (r == NULL) {
        COORD_LOG_E("desvestir: sistema no vestido (init primero)");
        return HASHMAL_ROLES_ERR_INIT;
    }

    if (!nodo_id_valido(nodo_id)) {
        COORD_LOG_W("desvestir: nodo_id %u fuera de rango",
                    (unsigned)nodo_id);
        return HASHMAL_ROLES_ERR_NULL;
    }

    hashmal_roles_nodo_t *n = &r->nodos[nodo_id];

    if (!n->vestido) {
        COORD_LOG_I("desvestir: nodo %u no estaba vestido "
                    "(Nm 20:26 sin begadím que transferir)",
                    (unsigned)nodo_id);
        return HASHMAL_ROLES_ERR_INIT;
    }

    /* P49 — Rol irremovible en defensa (Neh 4:17)
     * "ʼein pojshtím begadéinu" — "no nos quitamos nuestras vestiduras"
     * beged (H899) = vestidura/rol; pojshtím (H6584) = remover/quitar.
     * En defensa perimetral (Pieza 07 activa, Atrio), los roles NO se
     * pueden remover. Neh 4:13-14 declara que durante amenaza, los
     * defensores mantienen sus vestiduras. Cortamos la remoción con
     * ERR_SHUTDOWN (fase de defensa en curso).
     *
     * 2+ testigos:
     *   Neh 4:17  — ʼein pojshtím begadéinu (no quitamos vestiduras)
     *   Neh 4:13-14 — contexto: Atrio bajo defensa, rally de oración
     *   Éx 27:9-19 — Jatsér (Pieza 07) es barrera perimetral */

    /* Verificación: ¿está el Atrio (Pieza 07) en defensa activa?
     * Declaración forward de función externa en hashmal_perimeter.h.
     * Esta función consulta el estado de defensa perimetral.
     * Si en defensa: RECHAZA remoción de roles. */
    extern bool hashmal_perimeter_en_defensa(void);

    if (hashmal_perimeter_en_defensa()) {
        COORD_LOG_W("[BEGED IRREMOVIBLE] desvestir: nodo %u "
                    "NO PUEDE ser desvestido durante defensa "
                    "perimetral — Neh 4:17 ʼein pojshtím begadéinu "
                    "(no quitamos vestiduras). Atrio (Pieza 07) "
                    "bajo amenaza (Éx 27:9-19 Jatsér barrera). "
                    "Neh 4:13-14 rally: roles FIJOS durante defensa.",
                    (unsigned)nodo_id);
        return HASHMAL_ROLES_ERR_SHUTDOWN;
    }

    const hashmal_roles_tipo_rol_t rol_previo = n->rol;

    /* El rol operativo se retira; memorial (efod) y sello (tzitz)
     * se preservan. Nm 20:28: vestiduras pasan a otro, pero el
     * nombre permanece. */
    n->rol     = HASHMAL_ROLES_ROL_NINGUNO;
    n->vestido = false;

    if (r->nodos_activos > 0u) {
        r->nodos_activos--;
    }
    r->timestamp_ms = hashmal_roles_ahora_ms();

    COORD_LOG_I("vehafshét — nodo %u desvestido (era %s). "
                "Nombre preservado en efod (Éx 28:12), "
                "tzitz preservado (Éx 28:36). Nm 20:26-28.",
                (unsigned)nodo_id, nombre_rol(rol_previo));

    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 * hashmal_roles_restaurar - Zacarias 3:3-5 + Isaias 61:10
 *
 * "hasiru habegadim hatstso'im me'alav... halbesh otja majalatsot"
 *   - Zac 3:4
 * El sacerdote que fallo no se destruye: se le quitan las
 * vestiduras sucias (estado previo de fallo) y se le viste con
 * majalatsot (ropas de gala). Is 61:10 lo amplia: bigdei yesha'
 * (vestiduras de salvacion) y me'il tsedaqa (manto de justicia).
 *
 * Semantica: el rol se preserva. Lo que cambia es el ESTADO
 * global a RESTAURADO y el flag vestido se asegura en true. El
 * heartbeat se reinicia (ultimo_latido_ms=0) para que la primera
 * campana nueva (monitor_campana) marque el renacer del servicio.
 *
 * Validaciones:
 *   1. VESTIDO global.
 *   2. nodo_id en rango.
 *   3. Nodo registrado en efod (memorial preservado).
 *   4. Nodo con rol asignado previo (rol != NINGUNO). Si no tiene
 *      rol previo, no hay nada que restaurar - usar `vestir`.
 *
 * 2+ testigos:
 *   Zac 3:3-5  - hasiru habegadim hatstso'im, halbesh majalatsot
 *   Is 61:10   - bigdei yesha', me'il tsedaqa (libro distinto)
 *   Lc 15:22   - "stolen ten proten" - vestidura primera al hijo
 *                 prodigo restaurado (paralelo NT)
 * ================================================================== */
hashmal_roles_err_t hashmal_roles_restaurar(uint8_t nodo_id)
{
    hashmal_roles_estado_roles_t *r = hashmal_roles_vestiduras_mut();
    if (r == NULL) {
        COORD_LOG_E("restaurar: sistema no vestido (init primero)");
        return HASHMAL_ROLES_ERR_INIT;
    }

    if (!nodo_id_valido(nodo_id)) {
        COORD_LOG_W("restaurar: nodo_id %u fuera de rango",
                    (unsigned)nodo_id);
        return HASHMAL_ROLES_ERR_NULL;
    }

    if (!hashmal_roles_efod_esta_registrado(nodo_id)) {
        COORD_LOG_W("restaurar: nodo %u sin memorial en efod "
                    "(Éx 28:12 — sin zikkarón, nada que restaurar)",
                    (unsigned)nodo_id);
        return HASHMAL_ROLES_ERR_NODO_NO_ENCONTRADO;
    }

    hashmal_roles_nodo_t *n = &r->nodos[nodo_id];

    if (n->rol == HASHMAL_ROLES_ROL_NINGUNO) {
        COORD_LOG_W("restaurar: nodo %u sin rol previo — "
                    "Zac 3:4 requiere begadím (aunque tsoʼím) que "
                    "reemplazar. Use `vestir` para primera investidura.",
                    (unsigned)nodo_id);
        return HASHMAL_ROLES_ERR_ROL_INVALIDO;
    }

    const hashmal_roles_tipo_rol_t rol_preservado = n->rol;
    const bool era_vestido = n->vestido;

    /* "hasiru habegadim hatstso'im" - simbolicamente, el fallo
     * previo se "retira" reseteando heartbeat. El rol se preserva
     * (el sacerdote sigue siendo sacerdote) - lo que cambia es
     * la limpieza. */
    n->vestido          = true;
    n->ultimo_latido_ms = 0;
    n->heartbeat        = HASHMAL_ROLES_HEARTBEAT_SILENCIO;
    /* nodo.rol se conserva - "eres el mismo sacerdote, ahora limpio". */
    (void)rol_preservado;

    if (!era_vestido && r->nodos_activos < HASHMAL_ROLES_MAX_NODOS) {
        r->nodos_activos++;
    }

    /* Estado global - Zac 3:4 halbesh majalatsot. RESTAURADO es
     * un estado superior a VESTIDO en el enum (valor mayor): el
     * sistema que ya paso por limpieza lleva la marca. */
    r->estado       = HASHMAL_ROLES_ESTADO_RESTAURADO;
    r->timestamp_ms = hashmal_roles_ahora_ms();

    COORD_LOG_I("majalatsót — nodo %u restaurado como %s "
                "(heʻevárti meʻaléja ʻavonéja, Zac 3:4; "
                "bigdéi yéshaʻ + meʻíl tsedaqá, Is 61:10)",
                (unsigned)nodo_id, nombre_rol(n->rol));

    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 *
 *   "veHILBASHTA otam... umashajTA otam... vejihanu li."
 *     - Exodo 28:41
 *
 *   "vayyitten 'alav et hakuttonet." - Levitico 8:7
 *
 *   "ouj heauto tis lambanei ten timen." - Hebreos 5:4
 *
 *   "he'evarti me'aleja 'avoneja veHALBESH otja MAJALATSOT."
 *     - Zacarias 3:4
 *
 *   "basileion hierateuma, ethnos hagion." - 1 Pedro 2:9
 *
 *   Nadie se viste a si mismo. El coordinador - que actua en
 *   nombre del llamado del Padre - viste a otros. Designa sobre
 *   quien ya esta registrado en el memorial (efod, hombros), y
 *   nunca sobre besar adam ajeno al sistema. Si un nodo falla,
 *   no se borra: se limpian sus vestiduras (Zac 3) y se vuelve
 *   al servicio con majalatsot. El memorial no se reescribe; el
 *   tzitz no se raspa; solo la operacion se refresca. Asi el
 *   servicio vuelve sin deshonra.
 *
 *   Todos pueden servir (1 Pe 2:9); nadie se designa solo
 *   (Heb 5:4); el que cae es levantado (Is 61:10).
 *
 *   === HAQREV VEHILBISH - vestiduras_coordinador.c COMPLETO ===
 *
 *   Archivo 5 de 7 de las Vestiduras. Siguiente:
 *     vestiduras_shutdown.c - apagado digno (Dt 31-34)
 *
 *   PASO 9 de Exodo 40:12-15 - Pieza 09, Archivo 5 de 7.
 *
 * ================================================================== */
