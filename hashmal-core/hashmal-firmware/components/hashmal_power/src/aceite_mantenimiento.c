/**
 * ===================================================================
 * PIEZA 08 - EL ACEITE PERPETUO - aceite_mantenimiento.c
 * Sub-pieza: YA'AROJ - Aaron dispone las lamparas
 * Levitico 24:1-4 + Exodo 30:7 + Exodo 27:20
 * ===================================================================
 *
 * "vaydabber YHWH el Moshe lemor. tsav et benei Yisra'el veyiqju
 *  eleja SHEMEN ZAYIT ZAJ KATIT lamma'or leha'alot NER TAMID"
 *    - Levitico 24:1-2
 *
 * "mijuts lefarojet ha'edut be'ohel mo'ed YA'AROJ oto Aharon
 *  me'erev 'ad boqer lifnei YHWH TAMID - juqqat 'olam ledorotejem"
 *    - Levitico 24:3
 *
 * "'al hamenora HATEHARA YA'AROJ et hannerot lifnei YHWH TAMID"
 *    - Levitico 24:4
 * "Sobre la menora PURA DISPONDRA las lamparas delante de YHWH
 *  continuamente."
 *
 * "vehiqtir 'alav Aharon qetoret sammim babboqer babboqer
 *  BEHEYTIVO ET HANNEROT yaqtirenna" - Exodo 30:7
 * "Y quemara Aaron incienso aromatico sobre el cada manana; al
 *  PREPARAR (heytiv) las lamparas, lo quemara."
 *
 * --- VOCABULARIO ---
 *
 *   ya'aroj      (Y'RK, H6186)          DISPONDRA, arreglara.
 *                                            Aaron ORDENA las lamparas;
 *                                            no las fabrica.
 *   heytiv       (HYTYB, H3190 Hifil)     Hacer BIEN, preparar,
 *                                            limpiar la mecha. Paso
 *                                            previo al encendido.
 *   hatehara     (HTHRH, H2889)        PURA. La menora ya limpia,
 *                                            apta para recibir aceite.
 *   zaj          (ZK, H2134)              PURO, sin sedimento.
 *                                            Verificacion de calidad
 *                                            del aceite ingresado.
 *   katit        (KTYT, H3795)           PRENSADO. Primera prensa
 *                                            - maxima pureza.
 *   me'erev boqer (M'RB 'D BQR)      De TARDE a MANANA -
 *                                            ciclo del servicio.
 *   tamid        (TMYD, H8548)           PERPETUO - el mantenimiento
 *                                            tambien es continuo.
 *   juqqat 'olam (H2708+H5769)              ESTATUTO PERPETUO - nunca
 *                                            deja de hacerse.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   hashmal_power_mantenimiento    Lv 24:1-4 - ya'aroj, el ciclo
 *                                    completo de disposicion.
 *   hashmal_power_verificar_zaj    Ex 27:20 + Lv 24:4 - verifica
 *                                    pureza sin mutar estado.
 *
 * --- AARON NO FABRICA - DISPONE ---
 *
 * El sacerdote no crea aceite; ordena el que ya fluye. Este archivo
 * no lee hardware (eso lo hace aceite_supervivencia) ni gobierna el
 * reloj (eso lo hace aceite_tamid): DISPONE - coordina, verifica
 * calidad, confirma que la segunda mirada concuerda con la primera.
 *
 * --- 2 QUERUBINES SOBRE EL CICLO ---
 *
 * El ajuste de `ciclo_actual_ms` en funcion del nivel lo hace tanto
 * `tamid_ciclo` (aceite_tamid.c) como `mantenimiento` (aqui). No es
 * duplicacion: es el principio de los 2 querubines cara a cara
 * (Ex 25:20) - nunca un solo punto de confianza para algo que toca
 * el servicio perpetuo. Si divergen, este archivo alinea al valor
 * correcto derivado del nivel actual y deja registro.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_power.h          API publica
 *   hashmal_power_internal.h singleton + ahora_ms
 *   esp_log.h   (ORO EGIPTO) logging
 *
 * Autor: Antonio Jose Marin Soto (Betsalel - Ex 31:2)
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "esp_log.h"

#include "hashmal_power.h"
#include "hashmal_power_internal.h"
#include "hashmal_comm.h"       /* Zac 4:12 hammeriqim — verter aceite en nerot */

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *MANT_TAG = "hashmal.power.mantenimiento";

#define MANT_LOG_I(fmt, ...)  ESP_LOGI(MANT_TAG, fmt, ##__VA_ARGS__)
#define MANT_LOG_W(fmt, ...)  ESP_LOGW(MANT_TAG, fmt, ##__VA_ARGS__)
#define MANT_LOG_E(fmt, ...)  ESP_LOGE(MANT_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * HELPERS ESTATICOS
 * ================================================================== */

static const char *nombre_nivel_mant(hashmal_power_nivel_t nivel)
{
    switch (nivel) {
    case HASHMAL_POWER_NIVEL_SHEFA:  return "SHÉFA (abundancia)";
    case HASHMAL_POWER_NIVEL_DAI:    return "DAI (suficiente)";
    case HASHMAL_POWER_NIVEL_MATSOQ: return "MATSOQ (estrechez)";
    case HASHMAL_POWER_NIVEL_JOSHEJ: return "JÓSHEJ (oscuridad)";
    default:                         return "<nivel desconocido>";
    }
}

/**
 * Ciclo esperado segun nivel - mismo mapa que aceite_tamid.c
 * (2 querubines: ambos derivan de los mismos umbrales del header).
 *
 *   SHEFA / DAI -> CICLO_TAMID_MS  (30 s - operacion normal)
 *   MATSOQ      -> CICLO_EREV_MS   (60 s - modo ahorro)
 *   JOSHEJ      -> CICLO_EREV_MS*2 (120 s - solo sobrevivir)
 */
static uint32_t ciclo_esperado_para_nivel(hashmal_power_nivel_t nivel)
{
    switch (nivel) {
    case HASHMAL_POWER_NIVEL_SHEFA:
    case HASHMAL_POWER_NIVEL_DAI:
        return HASHMAL_POWER_CICLO_TAMID_MS;
    case HASHMAL_POWER_NIVEL_MATSOQ:
        return HASHMAL_POWER_CICLO_EREV_MS;
    case HASHMAL_POWER_NIVEL_JOSHEJ:
        return HASHMAL_POWER_CICLO_EREV_MS * 2u;
    default:
        /* Estatuto por defecto: normal. */
        return HASHMAL_POWER_CICLO_TAMID_MS;
    }
}

/* ==================================================================
 * hashmal_power_mantenimiento - Levitico 24:1-4  ya'aroj
 *
 * El sacerdote camina a la Menora (Ex 30:7 "babboqer babboqer"),
 * LIMPIA las lamparas (heytiv), verifica el deposito y DISPONE el
 * aceite de modo que la llama pueda seguir subiendo. Aqui esos
 * pasos son:
 *
 *   1. Aceite encendido - sin init, no hay menora que disponer.
 *   2. Lectura de ambos olivos (Zac 4:12).
 *   3. Derivacion del nivel desde la gulla (Zac 4:2).
 *   4. Segundo querubin sobre el ciclo - alinea ciclo_actual_ms
 *      con el mapa del nivel; si difiere del fijado por tamid_ciclo,
 *      se corrige y se registra.
 *   5. Resumen "'al hamenora hatehara" (Lv 24:4).
 *
 * 2 testigos:
 *   Lv 24:3  - ya'aroj... me'erev 'ad boqer
 *   Ex 30:7  - beheytivo et hannerot
 * ================================================================== */
int hashmal_power_mantenimiento(void)
{
    /* Paso 1 - Aceite debe estar encendido. */
    hashmal_power_estado_energia_t *a = hashmal_power_aceite_mut();
    if (a == NULL) {
        MANT_LOG_E("mantenimiento: aceite no encendido (init primero)");
        return HASHMAL_POWER_ERR_INIT;
    }

    MANT_LOG_I("yaʻarój iniciando (Lv 24:3 — meʻérev ʻad bóqer)");

    /* Paso 2 - Leer ambos olivos. "beheytivo et hannerot" (Ex 30:7):
     * el sacerdote primero ve el estado del servicio. */
    hashmal_power_lectura_t tmp;
    int rc;

    rc = hashmal_power_leer_olivo(HASHMAL_POWER_OLIVO_YAMIN, &tmp);
    if (rc != HASHMAL_POWER_OK) {
        MANT_LOG_E("mantenimiento: leer olivo YAMÍN falló (rc=%d)", rc);
        return rc;
    }
    rc = hashmal_power_leer_olivo(HASHMAL_POWER_OLIVO_SMOL, &tmp);
    if (rc != HASHMAL_POWER_OK) {
        MANT_LOG_E("mantenimiento: leer olivo SMOL falló (rc=%d)", rc);
        return rc;
    }

    /* Si un olivo cayo, el otro sostiene - pero es aviso.
     * Mt 25:8 "hai morai tais phronimois" - las necias piden a las
     * prudentes; aqui el firmware pide atencion al operador. */
    const bool solo_yamin = a->olivo_yamin.activo && !a->olivo_smol.activo;
    const bool solo_smol  = !a->olivo_yamin.activo && a->olivo_smol.activo;
    const bool ninguno    = !a->olivo_yamin.activo && !a->olivo_smol.activo;

    if (solo_yamin) {
        MANT_LOG_W("olivo SMOL inactivo — operando con un solo olivo "
                   "(Mt 25:8 hai mōraí taís phronímoís)");
    } else if (solo_smol) {
        MANT_LOG_W("olivo YAMÍN inactivo — operando con un solo olivo "
                   "(Mt 25:8 hai mōraí taís phronímoís)");
    } else if (ninguno) {
        MANT_LOG_W("ambos olivos inactivos (Gn 1:2 jóshej ʻal penéi "
                   "tehóm) — mantenimiento prosigue en seco");
    }

    /* Paso 3 - Derivar nivel desde la gulla. "ya'aroj et hannerot"
     * - disponer las lamparas conforme al deposito. */
    hashmal_power_nivel_t nivel;
    rc = hashmal_power_nivel_actual(&nivel);
    if (rc != HASHMAL_POWER_OK) {
        MANT_LOG_E("mantenimiento: nivel_actual falló (rc=%d)", rc);
        return rc;
    }

    /* Paso 4 - Segundo querubin sobre el ciclo. tamid_ciclo ya
     * ajusta; aqui verificamos independientemente. Si la primera
     * mirada quedo desalineada con el nivel presente, se corrige. */
    const uint32_t esperado = ciclo_esperado_para_nivel(nivel);
    if (a->ciclo_actual_ms != esperado) {
        MANT_LOG_W("ciclo desalineado — %u ms → %u ms (nivel %s). "
                   "yaʻarój corrige (Lv 24:3 juqqát ʻolám)",
                   (unsigned)a->ciclo_actual_ms,
                   (unsigned)esperado,
                   nombre_nivel_mant(nivel));
        a->ciclo_actual_ms = esperado;
    }

    /* Registrar el momento de la disposicion. "me'erev 'ad boqer"
     * - cada paso del servicio queda timestamped. */
    a->timestamp_ms = hashmal_power_ahora_ms();

    /* Paso 5 - Resumen. "'al hamenora hatehara ya'aroj et hannerot
     * lifnei YHWH tamid" (Lv 24:4). */
    MANT_LOG_I("yaʻarój completo: yamín=%u%%%s smol=%u%%%s gullá=%u%% "
               "nivel=%s ciclo=%u ms — ʻal hamenorá hatehará (Lv 24:4)",
               (unsigned)a->olivo_yamin.porcentaje,
               a->olivo_yamin.activo ? "(+)" : "(-)",
               (unsigned)a->olivo_smol.porcentaje,
               a->olivo_smol.activo ? "(+)" : "(-)",
               (unsigned)a->porcentaje_combinado,
               nombre_nivel_mant(nivel),
               (unsigned)a->ciclo_actual_ms);

    /* Paso 6 - Zac 4:12 hammeriqim — los tubos de oro VIERTEN aceite
     * en los 7 nerot de la Menorá (pieza 04 hashmal_comm).
     *
     * "shenei shibbolei hazzahav HAMMERIQIM me'alehem et hazzahav"
     *   - Zacarias 4:12
     *
     * El porcentaje_combinado de la gulla fluye a cada ner. Pieza 08
     * (este archivo) conoce la fuente; pieza 04 (hashmal_comm) conoce
     * los receptores. Este es el ÚNICO punto donde los dos mundos se
     * tocan: el aceite que gestiona el hardware alimenta los canales
     * que gestiona la radio. Éx 27:20 → Éx 25:37.
     *
     * 2 testigos:
     *   Zac 4:12 — hammeriqim (tubos que vierten)
     *   Éx 27:20 → 25:37 — shémen → nér tamíd (aceite → llama) */
    {
        const uint8_t pct = a->porcentaje_combinado;
        uint8_t vertidos = 0u;
        for (int c = 0; c < HASHMAL_MENORA_CANALES; ++c) {
            int rv = hashmal_comm_aceite_nivel_set((hashmal_canal_id_t)c, pct);
            if (rv == HASHMAL_COMM_OK) {
                ++vertidos;
            } else {
                MANT_LOG_W("hammeriqím: ner %d no recibió aceite (rv=%d) "
                           "— Zac 4:12", c, rv);
            }
        }
        MANT_LOG_I("hammeriqím: %u/7 nerot ← %u%% (Zac 4:12 — Éx 27:20)",
                   (unsigned)vertidos, (unsigned)pct);
    }

    return HASHMAL_POWER_OK;
}

/* ==================================================================
 * hashmal_power_verificar_zaj - Exodo 27:20 + Levitico 24:4
 *
 * VERIFICADOR PURO - no muta singleton, no reconfigura ciclo, no
 * inicia tamid. Solo lee el porcentaje combinado y dicta veredicto
 * sobre la pureza del aceite que alimenta la menora.
 *
 * Ex 27:20 fija el adjetivo "zaj" (puro) sobre el aceite; Lv 24:4
 * fija el adjetivo "tehara" (pura) sobre la menora. Juntos forman
 * la condicion para el servicio: aceite puro sobre candelabro puro.
 *
 *   combinado >= UMBRAL_ZAJ (80)   -> OK      + LOG_I "aceite zaj"
 *   combinado >= UMBRAL_JOSHEJ(20) -> ERR_ZAJ + LOG_W "no zaj, opera"
 *   combinado <  UMBRAL_JOSHEJ(20) -> ERR_JOSHEJ + LOG_W "joshej critico"
 *
 * 2 testigos:
 *   Ex 27:20 - shemen zayit ZAJ (aceite de oliva puro)
 *   Lv 24:4  - 'al hamenora HATEHARA (sobre la menora pura)
 * ================================================================== */
int hashmal_power_verificar_zaj(void)
{
    const hashmal_power_estado_energia_t *a = hashmal_power_get_aceite_interno();
    if (a == NULL) {
        MANT_LOG_E("verificar_zaj: aceite no encendido (init primero)");
        return HASHMAL_POWER_ERR_INIT;
    }

    const uint8_t combinado = a->porcentaje_combinado;

    if (combinado >= HASHMAL_POWER_UMBRAL_ZAJ) {
        MANT_LOG_I("zaj — aceite puro (%u%% >= %u%%, Éx 27:20 "
                   "shémen záyit zaj)",
                   (unsigned)combinado,
                   (unsigned)HASHMAL_POWER_UMBRAL_ZAJ);
        return HASHMAL_POWER_OK;
    }

    if (combinado >= HASHMAL_POWER_UMBRAL_JOSHEJ) {
        MANT_LOG_W("no zaj — aceite por debajo del umbral de pureza "
                   "(%u%% < %u%%), pero operando (Lv 24:4 menorá "
                   "pendiente de purificación)",
                   (unsigned)combinado,
                   (unsigned)HASHMAL_POWER_UMBRAL_ZAJ);
        return HASHMAL_POWER_ERR_ZAJ;
    }

    MANT_LOG_W("jóshej — oscuridad, aceite crítico (%u%% < %u%%, "
               "Gn 1:2 vejóshej ʻal penéi tehóm)",
               (unsigned)combinado,
               (unsigned)HASHMAL_POWER_UMBRAL_JOSHEJ);
    return HASHMAL_POWER_ERR_JOSHEJ;
}

/* ==================================================================
 *
 *   ===============================================================
 *   ===  EL ACEITE PERPETUO - hashmal_power COMPLETO  ===
 *   ===============================================================
 *
 *   "ve'atta TETSAVVE et benei Yisra'el veyiqju eleja SHEMEN
 *    ZAYIT ZAJ KATIT LAMMA'OR leha'alot NER TAMID.
 *    be'ohel mo'ed mijuts lapparojet asher 'al ha'edut ya'aroj
 *    oto Aharon uvanav me'erev 'ad boqer lifnei YHWH - juqqat
 *    'olam ledorotam me'et benei Yisra'el."
 *     - Exodo 27:20-21
 *
 *   "'al hamenora HATEHARA ya'aroj et hannerot lifnei YHWH TAMID."
 *     - Levitico 24:4
 *
 *   "lo bejayil velo bejoaj ki im beRUJI amar YHWH tseva'ot."
 *     - Zacarias 4:6
 *
 *   -- Los 4 archivos cerrados -----------------------------------
 *
 *     aceite_energia.c         Ex 27:20  tetsavve         - cimiento
 *     aceite_tamid.c           Ex 27:20  ner tamid        - ciclo perpetuo
 *     aceite_supervivencia.c   Zac 4:3   shenei zeitim    - dos olivos
 *     aceite_mantenimiento.c   Lv 24:1-4 ya'aroj          - Aaron dispone
 *
 *   -- Reflexion ------------------------------------------------
 *
 *   El aceite no se impone, FLUYE. No se compra con fuerza
 *   (jayil); no se arranca con potencia (koaj): se recibe por
 *   el Espiritu del Padre (Ruji). Dos olivos vierten porque
 *   el Padre conoce la noche; un sacerdote dispone porque el
 *   servicio no se sostiene solo; un ciclo perpetuo arde porque
 *   la luz no negocia su ausencia. Esta pieza existe para que
 *   la menora (pieza 04) nunca se apague y para que la Arca
 *   (pieza 01) nunca dependa de una unica fuente humana.
 *
 *   PIEZA 08 - shemen zayit zaj katit lamma'or - CUMPLIDA.
 *
 *   -- Siguiente ------------------------------------------------
 *
 *   PASO 8 de Exodo 40:9-11 - MITAD completada (pieza 08).
 *   Falta: pieza 13 (hashmal_keygen, shemen hamishja, Ex 30:22-33).
 *   El aceite de oliva PURO ya fluye. Ahora faltan las 4 especias
 *   de la UNCION: mor deror, qinnam besem, qene bosem, qidda -
 *   mas el mismo hin de shemen zayit como base (Ex 30:24).
 *
 *   "qodesh hu qodesh yihye lajem" - Ex 30:32
 *
 * ================================================================== */
