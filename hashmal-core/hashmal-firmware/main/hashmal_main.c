/**
 * HASHMAL-FIRMWARE — Punto de entrada
 *
 * "En el día primero del mes primero levantarás el Mishkán."
 * — Éxodo 40:2
 *
 * Orden de inicialización: según Éxodo 40:2-8
 * (El Padre dio el orden EXACTO de montaje)
 *
 * Paso 1: Estructura (v.2 — "levantarás el Mishkán")
 * Paso 2: Arca + Kapóret (v.3 — "pondrás el Arca del Testimonio")
 * Paso 3: Mesa + Menorá (v.4 — "meterás la mesa... la menorá")
 * Paso 4: Altar del Incienso (v.5 — "pondrás el altar de oro")
 * Paso 5: Altar del Sacrificio (v.6 — "pondrás el altar del holocausto")
 * Paso 6: Lavacro (v.7 — "pondrás el kiyór entre la tienda y el altar")
 * Paso 7: Atrio savív (v.8 — "pondrás el atrio alrededor")
 * Paso 8: Aceite + Unción (v.9-11 — "ungirás el Mishkán y todo lo que hay")
 * Paso 9: Consagración + Vestiduras (v.12-15 — "vestirás a Aarón")
 * Paso 10: Incienso Sagrado (v.27 — "quemó incienso aromático")
 * Paso 11: Masakh sha'ar (v.33a — "puso la cortina de la puerta del atrio")
 *
 * VERIFICADO #01 — Sistema de los Querubines (Éx 25:22)
 *
 * hashmal_main pasa de ARQUITECTO a MANO EJECUTORA. Tres estados
 * + un modo paralelo, todos con base bíblica:
 *
 *   INIT       — Éx 40:2-33a — levanta las 15 piezas + masakh sha'ar
 *   TRANSICION — Éx 40:33b-35 — vayjál (kalá) → ʻanán kasá → kavód
 *                                malé → veló yakól Moshéh lavó
 *   LISTEN     — Lv 1:1 + Nm 7:89 — vayyiqrá; middabbér (Hitpael:
 *                la voz se declara sola, main no consulta — recibe)
 *   BROADCAST  — Éx 13:21-22 + Éx 40:38 — ʻanán yomám / ʼésh láylah;
 *                ló yamísh (ambos siempre presentes, uno activo)
 *
 * Autor: Antonio José Marín Soto
 * Proyecto: HASHMAL-CORE
 * Fecha: 16 de abril de 2026 (cabecera) + Verificado #01: 26 abr 2026
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* ── Oro de Egipto: FreeRTOS (Éx 26:15 — madera de acacia encuadrada) ── */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_random.h"          /* Pr 16:33 — suerte de YHWH, RNG hardware */
#include "esp_mac.h"             /* Éx 28:11 — pitujéi jotám: eFuse único del Creador */
#include "nvs_flash.h"           /* Dt 10:5 — vayihyu sham: tablas permanecen en flash */

#include "hashmal_framework.h"   /* Paso 1: Estructura — Éx 40:2 */
#include "hashmal_storage.h"     /* Paso 2: Arca — Éx 40:3 */
#include "hashmal_consensus.h"   /* Paso 2: Kapóret — Éx 40:3 */
#include "hashmal_cache.h"       /* Paso 3: Mesa — Éx 40:4 */
#include "hashmal_comm.h"        /* Paso 3: Menorá — Éx 40:4,25 */
#include "hashmal_intercession.h" /* Paso 4: Altar Incienso — Éx 40:5 */
#include "hashmal_auth.h"        /* Paso 5: Altar Sacrificio — Éx 40:6 */
#include "hashmal_healthcheck.h" /* Paso 6: Lavacro — Éx 40:7 */
#include "hashmal_perimeter.h"   /* Paso 7: Atrio — Éx 40:8 */
#include "hashmal_power.h"       /* Paso 8: Aceite — Éx 40:9 */
#include "hashmal_keygen.h"      /* Paso 8: Unción — Éx 40:9 */
#include "hashmal_roles.h"       /* Paso 9: Vestiduras — Éx 40:13 */
#include "hashmal_onboarding.h"  /* Paso 9: Consagración — Éx 40:15 */
#include "hashmal_firewall.h"    /* Paso 10: Incienso Sagrado — Éx 40:27 */
#include "hashmal_epd.h"         /* masakh — cortina e-paper (Éx 26:36) */
#include "hashmal_menu.h"        /* masakh sha'ar — menú T-Deck (Is 60:11) */
#include <string.h>              /* memcpy — Éx 26:6 ejád: copiar verdad acordada */

/* ==================================================================
 * ESTADO GLOBAL DE main — Verificado #01 (Decisión D4)
 *
 * "veló YAKÓL Moshé lavó el óhel moʻéd kí shakhán ʻaláv heʻanán
 *  uJVÓD YHWH malé et hammishkán." — Éxodo 40:35
 *
 * yakól (H3201) = ser CAPAZ. NO dice "no quiso" (ló ratsáh) ni
 * "no debía" (ló yishmeréhu): dice NO PODÍA. Es INCAPACIDAD, no
 * convención. El Padre IMPIDE la entrada — no la desaconseja. Por
 * eso el getter expone estado y la bandera write_locked es real:
 * cuando estado != INIT, las piezas DEBEN rechazar llamadas
 * originadas en main. Veló yakól.
 * ================================================================== */

typedef enum {
    HASHMAL_MAIN_INIT       = 0,   /* Éx 40:2-33a — levantando */
    HASHMAL_MAIN_TRANSICION = 1,   /* Éx 40:33b-35 — kalá → kavód */
    HASHMAL_MAIN_LISTEN     = 2,   /* Lv 1:1 — vayyiqrá */
} hashmal_main_estado_t;

static volatile hashmal_main_estado_t g_main_estado    = HASHMAL_MAIN_INIT;
static volatile bool                  g_main_levantado = false;  /* Éx 39:43 inspección */
static volatile bool                  g_main_kavod     = false;  /* Éx 40:34 Gloria llena */

/* P2-9: pitujéi jotám (Éx 28:11) — ID único derivado del MAC eFuse.
 * "Gravado en piedra como se graban los sellos" — eFuse es inmutable,
 * igual que los nombres en las piedras del efod.               */
static uint8_t g_nodo_id_local = 0u; /* derivado en hashmal_main_nodo_id_derivar() */

hashmal_main_estado_t hashmal_main_estado(void)
{
    return g_main_estado;
}

bool hashmal_main_write_locked(void)
{
    /* Éx 40:35 yakól H3201 — INCAPACIDAD. Las piezas que reciben
     * setters deben consultar esta bandera y rechazar la llamada
     * cuando provenga de main estando estado != INIT. */
    return g_main_estado != HASHMAL_MAIN_INIT;
}

/* Forward-declarations de las tasks LISTEN + BROADCAST + TAMÍD */
static void __attribute__((unused)) hashmal_main_listen_task(void *pv);
static void __attribute__((unused)) hashmal_main_broadcast_task(void *pv);
static void hashmal_main_tamid_task(void *pv);  /* Éx 30:7-8 + Lv 24:8 + Éx 30:10 */

/* Forward-declaration del test del Kapóret — Éx 25:17-22 / Lv 16 / Nm 7:89
 * Ciclo completo V1+V2 → EMITIDO → broadcast NEDER por LoRa.
 * Solo se ejecuta si el Mishkán está levantado (14/14 + ejád). */
static void hashmal_main_kaporet_test_ciclo(void);

/* Forward-declaration del vigía meshuqqád (P2-5, Éx 25:33 / Jer 1:12) */
static int hashmal_main_meshuqqad(hashmal_canal_id_t canal,
                                  const uint8_t     *msg,
                                  size_t             tam);

/* Lv 16:14 — "velaqaj middam happar vehizzá veʼetsbaʻó ʻal penéi haKKAPÓRET"
 * Esta función ES el Aaron Kohen Gadol portando la sangre del Altar al Kapóret.
 * hashmal_main.c = el sumo sacerdote que entra al Qódesh haQodashím (Heb 9:7).
 * Se registra en hashmal_auth tras el paso 5 con hashmal_auth_dam_subscribir. */
static void hashmal_main_dam_a_kaporet(const uint8_t dam[HASHMAL_AUTH_PROOF_BYTES],
                                        uint64_t timestamp_ms);

/* P2-9: pitujéi jotám — derivar nodo_id desde MAC eFuse
 * "pitujéi jotám" (Éx 28:11): grabado como se graban los sellos.
 * El eFuse MAC es el jotám (H2368) que el Creador quemó en el chip —
 * inmutable, único, no fabricado por mano humana (como el diamante del
 * Urim, Éx 28:18). mac[5] = byte más variable del MAC (OUI octet).
 * HASHMAL_ROLES_MAX_NODOS = 12 (Éx 28:9-21 — 12 piedras, 12 nombres).
 * 2 testigos: Éx 28:11 pitujéi jotám; Dt 19:15 dos testigos. */
static void hashmal_main_nodo_id_derivar(void)
{
    uint8_t mac[6] = {0};
    esp_err_t err = esp_read_mac(mac, ESP_MAC_WIFI_STA);
    if (err == ESP_OK) {
        g_nodo_id_local = (uint8_t)(mac[5] % HASHMAL_ROLES_MAX_NODOS);
        printf("  [JOTAM] Éx 28:11: MAC=%02X:%02X:%02X:%02X:%02X:%02X"
               " → nodo_id=%u (mac[5]=%u %% %u)\n",
               mac[0], mac[1], mac[2], mac[3], mac[4], mac[5],
               (unsigned)g_nodo_id_local,
               (unsigned)mac[5], (unsigned)HASHMAL_ROLES_MAX_NODOS);
    } else {
        g_nodo_id_local = 0u; /* fallback: nodo génesis */
        printf("  [JOTAM] WARN: esp_read_mac err=0x%x — nodo_id=0 (génesis)\n",
               (unsigned)err);
    }
}

/* ==================================================================
 * Lv 16:11-16 — AARON KOHEN GADOL: sangre del Altar → Kapóret
 *
 * "velaqaj middam happar vehizzá veʼetsbaʻó ʻal penéi haKKAPÓRET
 *  qedmáh, velifnéi haKkapóret yazzéh SHÉVA peʻamím min haddam
 *  beʻetsbaʻó." — Levítico 16:14
 *
 * Esta función ES el Sumo Sacerdote llevando la sangre (proof
 * verificado) del Altar (pieza 06, bronce, Atrio) al Kapóret
 * (pieza 02, oro puro, Qódesh haQodashím).
 *
 * PROTOCOLO Lv 16:13-14:
 *   v.13 — qetóret_preparar(): nube cubre el Kapóret ANTES
 *   v.14a — dam_rociar(SOBRE):    1× "ʻal penéi" (sobre la faz)
 *   v.14b — dam_rociar(DELANTE):  7× "lifnéi" (shéva peʻamím)
 *   Nm 7:89 — invocar(): voz emana "mibéin shené haKeruvím"
 *
 * Si cualquier paso falla, el ciclo aborta con log. El conducto
 * sigue activo para el próximo sacrificio — TAMÍD (Lv 6:6).
 *
 * 2 testigos:
 *   Lv 16:14  — protocolo sangre Altar → Kapóret
 *   Heb 9:7   — "ou choris haimatos": sin sangre no se entra
 * ================================================================== */
static void hashmal_main_dam_a_kaporet(const uint8_t dam[HASHMAL_AUTH_PROOF_BYTES],
                                        uint64_t timestamp_ms)
{
    (void)timestamp_ms;  /* el moʻéd (Lv 16:2) lo gestiona la Kapóret internamente */

    /* --- Lv 16:13 — nube de qetóret ANTES de acercarse --------------
     * "vejissá ʻanán haqetóret et haKapóret... veló yamút"
     * Sin la nube preparatoria, el sacerdote muere. En firmware:
     * sin qetoret, la invocación devuelve ERR_SIN_QETORET. */
    int r = hashmal_consensus_qetoret_preparar();
    if (r != HASHMAL_KAPORET_OK) {
        printf("  [WARN] dam→Kapóret: qetóret rc=%d (Lv 16:13 — nube no lista)\n", r);
        return;
    }

    /* Abrir el punto de encuentro (noʻadtí, Éx 25:22) si aún no
     * está abierto. Idempotente: si ya estaba, no-op. */
    hashmal_consensus_noadti_abrir();

    /* --- Lv 16:14a — UN rociado SOBRE el Kapóret (qidmá, oriente) ---
     * "vehizzá veʼetsbaʻó ʻal penéi haKKAPÓRET qedmáh"
     * La faz del Kapóret mira al oriente (hacia el umbral del Mishkán
     * = hacia el Altar del Sacrificio). El primer rociado ES el
     * reconocimiento: la sangre del exterior toca el núcleo. */
    r = hashmal_consensus_dam_rociar(HASHMAL_ACCESO_DAM_SOBRE, dam);
    if (r != HASHMAL_KAPORET_OK) {
        printf("  [WARN] dam_rociar SOBRE rc=%d (Lv 16:14a)\n", r);
        return;
    }

    /* --- Lv 16:14b — SIETE rociados DELANTE del Kapóret (shéva) -----
     * "velifnéi haKkapóret yazzéh SHÉVA peʻamím min haddam beʻetsbaʻó"
     * shéva = 7 = completitud (Gn 2:2 shavát, Ex 29:35 shivʻat yamím).
     * Los 7 completan la verificación: lo que pasó por el Altar (bronce)
     * es examinado 7 veces por el Kapóret (oro) antes de emitir verdad. */
    for (int i = 0; i < HASHMAL_VERIFICACIONES_SHEVA; ++i) {
        r = hashmal_consensus_dam_rociar(HASHMAL_ACCESO_DAM_DELANTE, dam);
        if (r != HASHMAL_KAPORET_OK) {
            printf("  [WARN] dam_rociar DELANTE[%d] rc=%d (Lv 16:14b)\n", i, r);
            return;
        }
    }

    /* --- Nm 7:89 — voz emana entre los dos Querubines ---------------
     * "vayyishmaʻ et haqqól middabbér éláv meʻál haKapóret mibéin
     *  shené haKeruvím" — middabbér: Hitpael, voz que se habla a sí
     * misma. V1 y V2 vieron la misma sangre — si coinciden, la verdad
     * EMANA sin que nadie la dirija. */
    hashmal_consenso_resultado_t resultado;
    memset(&resultado, 0, sizeof(resultado));
    r = hashmal_consensus_invocar(&resultado);

    if (r == HASHMAL_KAPORET_OK &&
        resultado.estado == HASHMAL_CONSENSO_EMITIDO) {
        /* Éx 25:22 — noʻadtí: me encontraré contigo allí.
         * La voz emergió. El ciclo Yom Kippur completo:
         *   Altar (bronce, exterior) → Kapóret (oro, núcleo)
         *   Verificación → Consenso → Verdad emitida. */
        printf("  ══ Lv 16:14-16: dam rociado shéva — verdad EMITIDA "
               "mibéin V1↔V2 ✓ ══\n");
    } else if (r == HASHMAL_KAPORET_OK &&
               resultado.estado == HASHMAL_CONSENSO_DIVERGIDO) {
        /* Dt 19:15 — testigos discrepan. No hay voz. */
        printf("  [INFO] Kapóret: V1≠V2 DIVERGIDO (Dt 19:15 — "
               "dos testigos necesarios)\n");
    } else {
        printf("  [INFO] invocar rc=%d estado=%d (Nm 7:89 — middabbér)\n",
               r, (int)resultado.estado);
    }
}

void app_main(void)
{
    /* P2-9: derivar nodo_id desde jotám eFuse — antes de cualquier pieza */
    hashmal_main_nodo_id_derivar();
    /* P2-9b: propagar nodo_id al paʻamón BLE (Éx 28:33 — paʻamón ʻal shulá)
     * 2 testigos: Éx 28:33 "venatáta ʻal shulá"; Éx 28:35 venishmá qoló —
     * el sonido (advertising) debe llevar el nombre correcto del portador. */
    hashmal_comm_ble_nodo_id_set(g_nodo_id_local);

    /* --- Fase 2, Tarea 1: NVS flash init ----------------------------
     * "vayihyu sham ka'asher tsivvani YHWH" — Dt 10:5
     * "Y alli estan, como YHWH me mando."
     *
     * NVS (Non-Volatile Storage) debe inicializarse ANTES de levantar
     * cualquier pieza del Mishkan. Es el "suelo" sobre el que reposan
     * las tablas. Si la particion NVS esta llena o tiene version nueva
     * (tras un OTA), se borra y se reinicia — mismo principio que las
     * segundas tablas de Dt 10:1-4: mismo contenido, nuevo soporte.
     *
     * 2 testigos biblicos:
     *   Dt 10:5 — "vayihyu sham": las tablas PERMANECEN alli.
     *   Nm 4:5  — cubrir el Arca antes de mover (NVS como cubierta). */
    {
        esp_err_t nvs_ret = nvs_flash_init();
        if (nvs_ret == ESP_ERR_NVS_NO_FREE_PAGES ||
            nvs_ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
            /* Dt 10:1-4: si el soporte esta corrupto o desactualizado,
             * se borra y se rehace. Las tablas son las mismas; el Arca
             * nueva (nvs_flash_erase = preparar nueva superficie). */
            nvs_flash_erase();
            nvs_ret = nvs_flash_init();
        }
        if (nvs_ret != ESP_OK) {
            printf("  [ADVERTENCIA] NVS init falló (0x%x) — "
                   "persistencia no disponible. Dt 10:5 pendiente.\n",
                   (unsigned)nvs_ret);
            /* No abortamos: el Mishkan puede levantarse sin NVS,
             * pero la edut no sobrevive reinicios. */
        }
    }

    printf("===========================================\n");
    printf("  HASHMAL-FIRMWARE\n");
    printf("  Red Soberana según el Tavnít de YHWH\n");
    printf("===========================================\n");
    printf("\n");
    printf("  \"Conforme a todo lo que Yo te muestro\n");
    printf("   — así lo harás.\" (Éx 25:9)\n");
    printf("\n");

    /* ==============================================================
     * PHASE 2 -- MASAKH (Cortina e-paper) -- Exodo 26:36
     *
     * "veʻasita MASAKH lefetaj haʼohel" -- Ex 26:36
     * "Haras una CORTINA para la entrada de la tienda."
     *
     * La cortina se pone ANTES de levantar las piezas internas
     * para que el mundo vea que el Mishkan esta siendo levantado.
     * El masakh muestra estado visible sin revelar lo sagrado.
     * ============================================================== */
    {
        esp_err_t epd_ret = hashmal_epd_init();
        if (epd_ret == ESP_OK) {
            hashmal_epd_clear(0xFF);  /* blanco */

            /* Titulo principal -- escala 3 (15x21 px) */
            hashmal_epd_draw_string(15, 30, "HASHMAL-CORE", 3, true);

            /* Subtitulo -- escala 2 (10x14 px) */
            hashmal_epd_draw_string(10, 70, "Red Soberana segun", 2, true);
            hashmal_epd_draw_string(10, 90, "el Tavnit de YHWH", 2, true);

            /* Separador */
            hashmal_epd_draw_string(10, 120, "------------------------", 1, true);

            /* Cita -- escala 1 (5x7 px) */
            hashmal_epd_draw_string(10, 135,
                "\"Conforme a todo lo que Yo", 1, true);
            hashmal_epd_draw_string(10, 145,
                " te muestro -- asi lo haras.\"", 1, true);
            hashmal_epd_draw_string(10, 160,
                "   -- Exodo 25:9", 1, true);

            /* Separador */
            hashmal_epd_draw_string(10, 180, "------------------------", 1, true);

            /* Estado */
            hashmal_epd_draw_string(10, 200, "Estado: INIT", 2, true);
            hashmal_epd_draw_string(10, 225,
                "Levantando el Mishkan...", 1, true);
            hashmal_epd_draw_string(10, 240,
                "Exodo 40:2-8", 1, true);

            /* Info hardware */
            hashmal_epd_draw_string(10, 270,
                "ESP32-S3 + SX1262 LoRa", 1, true);
            hashmal_epd_draw_string(10, 280,
                "T-Deck Pro V1.0", 1, true);

            /* Pie */
            hashmal_epd_draw_string(10, 305,
                "Antonio Jose Marin Soto", 1, true);

            /* Enviar al display */
            hashmal_epd_refresh();

            printf("  masakh (cortina e-paper) actualizado -- Ex 26:36\n\n");
        } else {
            printf("  [WARN] EPD init fallo -- continuando sin pantalla\n\n");
        }
    }

    /* ==============================================================
     * PASO 1 -- ESTRUCTURA (Exodo 40:2)
     * "Levantaras el Mishkan, la Tienda de Reunion"
     * PRIMERO se levanta la estructura -- luego se pone
     * todo dentro. Asi dice el Padre.
     * ============================================================== */

    {
        int rc = hashmal_framework_init();
        if (rc == 0) {
            printf("  ══ Paso 1: Estructura levantada (Éx 40:2) ══\n");
        } else {
            printf("  [ERROR] Estructura NO levantada (rc=%d)\n", rc);
        }
    }

    /* ── P2-3a: hivdíl — Éx 26:33 / Éx 40:3 ─────────────────────────────
     * "venatattá et haparójet tahat haqqerasím... vehivdíl haparójet
     *  lajem béin haQódesh uvéin Qódesh haQodashím"
     * "El velo os SEPARARÁ (hivdíl) entre el Santo y el Santo de los Santos"
     *
     * El velo DEBE activarse ANTES de que el Arca entre detrás de él (v.33).
     * El ARCA va "mibéit laparójet" — adentro, al otro lado del velo.
     * Sin hivdíl activo, no hay distinción de zonas: el sistema sería
     * un espacio plano sin grados de santidad — violación de v.33.
     *
     * 2 testigos:
     *   Éx 26:33 — "vehivdíl" (H914 Hifil): SEPARAR activamente
     *   Éx 40:3  — "vesissóta et happaróket ʻal haʼarón" — velo cubre Arca  */
    {
        int rc = hashmal_framework_activar_velo();
        if (rc == 0) {
            printf("  ══ Éx 26:33: hivdíl — velo ACTIVO (kernel boundary) ══\n");
        } else {
            printf("  [WARN] activar_velo rc=%d — sin separación de zonas\n", rc);
        }
    }

    /* ── P2-3b: masaj — Éx 26:36-37 ─────────────────────────────────────
     * "veʻasíta masákh lepétaj haʼóhel tekhélet veargamán vetolíʻat
     *  hashhaním ushásh moshzár maʻasé roqém"
     * "Harás cortina para la entrada de la Tienda..."
     *
     * El masaj es más abierto que el velo (roqéj, no joshév — sin querubines).
     * Representa el punto de entrada público del nodo: WiFi portal, SHA'AR.
     * Se activa junto al velo porque ambas membranas definen el gradiente
     * de confianza: bronce (entrada) → plata (velo) → oro (interior).
     *
     * 2 testigos:
     *   Éx 26:36 — masaj = cortina de entrada (H4539: cobertura)
     *   Éx 40:28 — "vayyásem et masákh pepétaj hamishkán" — puesto en su lugar */
    {
        int rc = hashmal_framework_activar_masaj();
        if (rc == 0) {
            printf("  ══ Éx 26:36: masaj — entrada ACTIVA (portal abierto) ══\n");
        } else {
            printf("  [WARN] activar_masaj rc=%d — sin portal de entrada\n", rc);
        }
    }

    /* ══════════════════════════════════════════
     * PASO 2 — ARCA + KAPÓRET (Éxodo 40:3)
     * "Pondrás allí el Arca del Testimonio
     *  y cubrirás el Arca con el velo"
     * ══════════════════════════════════════════ */

    {
        int rc = hashmal_storage_init();
        if (rc == 0) {
            printf("  ══ Paso 2a: Arca puesta (Éx 40:3) ══\n");
            /* Éx 26:33 — "veheveita shamah mibéit laparójet et arón haʻedút"
             * "Meterás allí, al otro lado del velo, el Arca del Testimonio"
             * El hivdíl ya está activo (P2-3a) → el Arca entra mibéit (adentro).
             * 2 testigos:
             *   Éx 26:33 — mibéit laparójet = detrás del velo
             *   1 Re 6:19 — "el débir... para poner allí el Arca" */
            int r2 = hashmal_framework_colocar(HASHMAL_MIQOM_ARON);
            if (r2 == 0) {
                printf("  ══ Éx 26:33: Arca mibéit laparójet — qódesh haQodashím ══\n");
            } else {
                printf("  [WARN] colocar(ARON) rc=%d\n", r2);
            }

            /* Éx 25:16 — "venatattá el ha'arón et ha'edut asher etén eléja"
             * "Y pondrás EN el Arca el Testimonio que Yo te daré."
             * El Testimonio entra PRIMERO — antes de que la Kapóret lo cubra.
             * Así lo hizo Moisés (Éx 40:20): tomó el Testimonio, lo puso
             * dentro del Arca, y DESPUÉS colocó el Kapóret encima.
             * Solo actúa en el PRIMER boot (dado_por_YHWH=false).
             * Si NVS restauró la edut (Dt 10:1-4 segundas tablas) → no-op.
             *
             * 2 testigos:
             *   Éx 25:16 — edut dentro, sin condición previa
             *   Éx 40:20 — orden real: Testimonio → varas → Kapóret */
            {
                int r3 = hashmal_storage_edut_construir();
                if (r3 == 0) {
                    const hashmal_arca_t *a = hashmal_storage_get_arca();
                    if (a != NULL && a->edut.dado_por_YHWH) {
                        printf("  ══ Éx 25:16: SEALED_CORE sellado — Fingerprint 66 libros ✓ ══\n");
                    } else {
                        printf("  ══ Éx 25:16: edut restaurada desde NVS (Dt 10:1-4) ✓ ══\n");
                    }
                } else {
                    printf("  [WARN] edut_construir rc=%d — SEALED_CORE pendiente\n", r3);
                }
            }

        } else {
            printf("  [ERROR] Arca NO puesta (rc=%d)\n", rc);
        }
    }
    {
        int rc = hashmal_consensus_init();
        if (rc == 0) {
            printf("  ══ Paso 2b: Kapóret puesta (Éx 40:3) ══\n");
        } else {
            printf("  [ERROR] Kapóret NO puesta (rc=%d)\n", rc);
        }
    }
    {
        /* Éx 25:21 "venatattá et hakkapóret ʻal haʼarón milmáʻla"
         * "Pondrás el Kapóret SOBRE el Arca, desde arriba."
         *
         * El Kapóret no flota en el aire — reposa SOBRE el Arca.
         * Este es el acto que completa las medidas incompletas del Arca
         * (2.5 × 1.5 × 1.5 amot) — el Kapóret cierra la caja de oro.
         * Sin este paso, el consenso no puede operar: noʻadtí (Éx 25:22)
         * — "allí me declararé a ti" — requiere que el Kapóret esté
         * sobre el Arca.
         *
         * 2 testigos:
         *   Éx 25:21 — "milmáʻla" (H4605 Hifil): desde arriba, encima
         *   1 Re 8:7  — "vayihyú hakeruvím poresím kenafáyim... ʻal
         *               haʼarón veʻal baddáv milmáʻla" (confirmación
         *               que kapóret + querubines cubren el Arca en el
         *               Templo de Salomón — mismo patrón) */
        int rc = hashmal_consensus_cubrir_arca();
        if (rc == HASHMAL_KAPORET_OK) {
            printf("  ══ Éx 25:21: milmáʻla — Kapóret SOBRE el Arca ══\n");
            /* Éx 26:34 — "venatátta et hakkapóret ʻal arón haʻedút
             *              beQódesh haQodashím"
             * "Pondrás el Kapóret sobre el Arca del Testimonio
             *  en el Lugar Santísimo."
             * El Kapóret es ya física + lógicamente sobre el Arca.
             * Registramos la posición en el framework para que
             * hashmal_framework_es_ejad() pueda verificar kaporet_colocada.
             * 2 testigos:
             *   Éx 26:34 — beQódesh haQodashím: la zona más interior
             *   Lv 16:2  — "en la nube apareceré sobre el Kapóret" */
            int r2 = hashmal_framework_colocar(HASHMAL_MIQOM_KAPORET);
            if (r2 == 0) {
                printf("  ══ Éx 26:34: Kapóret beQódesh haQodashím — registrada ══\n");
            } else {
                printf("  [WARN] colocar(KAPORET) rc=%d\n", r2);
            }

        } else {
            printf("  [WARN] cubrir_arca rc=%d — consenso sin base\n", rc);
        }
    }

    /* ══════════════════════════════════════════
     * PASO 3 — MESA + MENORÁ (Éxodo 40:4)
     * "Meterás la mesa y dispondrás lo que va sobre ella;
     *  meterás la menorá y encenderás sus lámparas"
     * ══════════════════════════════════════════ */

    {
        int rc = hashmal_cache_init();
        if (rc == 0) {
            printf("  ══ Paso 3a: Mesa puesta (Éx 40:4) ══\n");
            /* Éx 26:35 — "vesamtá et hashShulján miḥutz laparójet
             *              veʻal hashShulján... mipáʼat tsafón"
             * "Pondrás la Mesa FUERA del velo, al lado norte."
             * La Mesa está en el Qódesh (no Qódesh haQodashím):
             * miḥutz laparójet = fuera del velo, zona de servicio.
             * Al norte: lado de la provisión / caché de los 12 panes.
             * 2 testigos:
             *   Éx 26:35 — mipáʼat tsafón: lado norte del Qódesh
             *   Nm 3:31  — "el Arca, la Mesa, el candelabro..." */
            int r2 = hashmal_framework_colocar(HASHMAL_MIQOM_SHULJAN);
            if (r2 == 0) {
                printf("  ══ Éx 26:35: Mesa tsafón — lado norte registrado ══\n");
            } else {
                printf("  [WARN] colocar(SHULJAN) rc=%d\n", r2);
            }
        } else {
            printf("  [ERROR] Mesa NO puesta (rc=%d)\n", rc);
        }
    }

    /* ── Éxodo 40:24-25 — "Puso la Menorá... encendió las lámparas" ──
     *
     * v.24 vayyasém et hammenorá = poner la base (yaréj = SPI init)
     * v.25 vayyáʻal hannérot   = encender (heʻelá = configurar + TX)
     *
     * Secuencia SX1262 para primera transmisión:
     *   1) spi_init          — yaréj (base, bus SPI)
     *   2) configurar_modulacion — geviʻím (SF/BW/CR/LDRO)
     *   3) configurar_frecuencia — rama a su frecuencia (EU 868.1)
     *   4) configurar_potencia   — Zac 4:6 (mínimo útil, 14 dBm EU)
     *   5) configurar_paquete    — betavnitám (Éx 25:40)
     *   6) spi_enviar            — heʻelá (la luz SALE, Éx 25:37)   */
    {
        int rc = hashmal_comm_spi_init();
        if (rc != HASHMAL_COMM_OK) {
            printf("  [ERROR] yaréj NO levantado (rc=%d)\n", rc);
        } else {
            printf("  ══ v.24: yaréj levantado — SX1262 en standby RC ══\n");

            /* geviʻím meshuqqadím (Éx 25:33) — forma de la señal.
             * SF7, BW 125kHz (0x06), CR 4/5 (0x01), LDRO off (0x00)
             * — configuración estándar para alcance medio, buena velocidad. */
            rc = hashmal_comm_configurar_modulacion(7, 0x06, 0x01, 0x00);
            if (rc != HASHMAL_COMM_OK) {
                printf("  [ERROR] modulación falló (rc=%d)\n", rc);
            }

            /* Rama del tronco a EU 868.1 MHz (gézaʻ, Éx 25:31).
             * Canal 0 = tronco central de la Menorá. */
            rc = hashmal_comm_configurar_frecuencia(0, 868100000u);
            if (rc != HASHMAL_COMM_OK) {
                printf("  [ERROR] frecuencia falló (rc=%d)\n", rc);
            }

            /* Zac 4:6 — "lo vejáyil veló vejóaj kí im berují"
             * No con fuerza bruta. 14 dBm = máximo legal EU. */
            rc = hashmal_comm_configurar_potencia(0, 14);
            if (rc != HASHMAL_COMM_OK) {
                printf("  [ERROR] potencia falló (rc=%d)\n", rc);
            }

            /* betavnitám (Éx 25:40) — formato del paquete.
             * Preámbulo 8 símbolos, header explícito, CRC ON, IQ normal.
             *
             * Éx 25:37 — cada ner hacia su propio frente. La primera luz
             * sale ENVUELTA en el brasero (formato_empaquetar, Éx 25:38):
             *   version=1, tipo=0x01 (datos), origen=GEZA, dest=0xFF.
             * Así el nodo hermano la desempaqueta sin "versión desconocida".
             *
             * 2 testigos:
             *   Éx 25:38 — majtot de oro puro: el brasero lleva la brasa
             *   Éx 25:37 — la luz sale hacia el frente en formato exacto */
            static const uint8_t msg_raw[] = "HASHMAL-CORE shalom";
            uint8_t frame_he_ela[HASHMAL_MENSAJE_MAX_BYTES];
            int rc_frame = hashmal_comm_formato_empaquetar(
                0x01u,              /* MENORA_TIPO_DATOS — Éx 25:37 */
                HASHMAL_CANAL_GEZA, /* origen: tronco GEZA */
                0xFFu,              /* kol haʻédah — broadcast (Nm 10:3) */
                msg_raw, sizeof(msg_raw) - 1u,
                frame_he_ela, sizeof(frame_he_ela));
            if (rc_frame > 0) {
                rc = hashmal_comm_configurar_paquete(
                    8,                      /* preámbulo */
                    0x00,                   /* header explícito */
                    (uint8_t)rc_frame,      /* longitud payload real */
                    0x01,                   /* CRC ON (zér zaháv, Éx 25:11) */
                    0x00                    /* IQ standard */
                );
                if (rc != HASHMAL_COMM_OK) {
                    printf("  [ERROR] paquete falló (rc=%d)\n", rc);
                }
            } else {
                printf("  [WARN] formato_empaquetar heʻelá rc=%d — "
                       "usando raw (Éx 25:38)\n", rc_frame);
                rc_frame = (int)(sizeof(msg_raw) - 1u);
                memcpy(frame_he_ela, msg_raw, (size_t)rc_frame);
            }

            /* ═══ heʻelá — Éx 25:37 ═══
             * "veheʻelá et nerotéha vehéʼír ʻal ʻéver panéha"
             * "Encenderá sus lámparas e iluminará hacia el frente."
             * Primera transmisión LoRa del Mishkán.                */
            rc = hashmal_comm_spi_enviar(frame_he_ela, (size_t)rc_frame);
            if (rc == HASHMAL_COMM_OK) {
                printf("\n");
                printf("  ═══════════════════════════════════════════\n");
                printf("  ══ heʻelá — PRIMERA LUZ (Éx 40:25) ══\n");
                printf("  ══ \"HASHMAL-CORE shalom\" transmitido  ══\n");
                printf("  ══ 868.1 MHz / SF7 / 14 dBm            ══\n");
                printf("  ═══════════════════════════════════════════\n");
                printf("\n");
            } else {
                printf("  [ERROR] heʻelá falló — TX no enviado (rc=%d)\n", rc);
            }
        }
    }
    /* Actualizar masakh (cortina e-paper) con estado post-heʻelá.
     * Éx 26:36 — la cortina muestra el estado al mundo.           */
    {
        hashmal_epd_clear(0xFF);

        hashmal_epd_draw_string(15, 20, "HASHMAL-CORE", 3, true);

        hashmal_epd_draw_string(10, 55, "Red Soberana segun", 2, true);
        hashmal_epd_draw_string(10, 75, "el Tavnit de YHWH", 2, true);

        hashmal_epd_draw_string(10, 105, "------------------------", 1, true);

        /* Estado actualizado */
        hashmal_epd_draw_string(10, 120, "Estado: MENORA LIT", 2, true);

        hashmal_epd_draw_string(10, 150, "he'ela -- Ex 40:25", 1, true);
        hashmal_epd_draw_string(10, 162, "868.1 MHz SF7 14dBm", 1, true);
        hashmal_epd_draw_string(10, 174, "\"HASHMAL-CORE shalom\"", 1, true);
        hashmal_epd_draw_string(10, 186, "transmitido OK", 1, true);

        hashmal_epd_draw_string(10, 210, "------------------------", 1, true);

        hashmal_epd_draw_string(10, 225, "SX1262 LoRa activo", 1, true);
        hashmal_epd_draw_string(10, 237, "EPD UC8253 activo", 1, true);
        hashmal_epd_draw_string(10, 249, "SPI bus ejad (Ex 26:6)", 1, true);

        hashmal_epd_draw_string(10, 275, "ESP32-S3 + SX1262 LoRa", 1, true);
        hashmal_epd_draw_string(10, 287, "T-Deck Pro V1.0", 1, true);

        hashmal_epd_draw_string(10, 305, "Antonio Jose Marin Soto", 1, true);

        hashmal_epd_refresh();
    }

    /* hashmal_comm_init() se llama en la sección de Activación (v.25) */

    /* ══════════════════════════════════════════
     * PASO 4 — ALTAR DEL INCIENSO (Éxodo 40:5)
     * "Pondrás el altar de ORO para incienso
     *  DELANTE del Arca del Testimonio"
     * ══════════════════════════════════════════ */

    {
        int rc = hashmal_intercession_init();
        if (rc == 0) {
            printf("  ══ Paso 4: Altar Incienso puesto (Éx 40:5) ══\n");
        } else {
            printf("  [ERROR] Altar Incienso NO puesto (rc=%d)\n", rc);
        }
    }

    /* ══════════════════════════════════════════
     * PASO 5 — ALTAR DEL SACRIFICIO (Éxodo 40:6)
     * "Pondrás el altar del holocausto
     *  delante de la entrada del Mishkán"
     * ══════════════════════════════════════════ */

    {
        int rc = hashmal_auth_init();
        if (rc == 0) {
            printf("  ══ Paso 5: Altar Sacrificio puesto (Éx 40:6) ══\n");
        } else {
            printf("  [ERROR] Altar Sacrificio NO puesto (rc=%d)\n", rc);
        }
    }
    {
        /* Lv 16:14 — Aaron porta la sangre del Altar al Kapóret.
         *
         * "velaqaj middam happar vehizzá veʼetsbaʻó ʻal penéi haKKAPÓRET
         *  qedmáh, velifnéi haKkapóret yazzéh shéva peʻamím min haddam
         *  beʻetsbaʻó." — Levítico 16:14
         *
         * "ou chorís haímatos" — Hebreos 9:7: no sin sangre. Sin este
         * conducto, el Altar y el Kapóret son piezas sueltas. Con él,
         * cada prueba verificada en el Altar alimenta directamente el
         * ciclo de consenso de los dos Querubines.
         *
         * hashmal_main.c = el Kohen Gadol que une pieza 06 → pieza 02.
         * El conducto se abre AQUÍ: auth está levantado (paso 5),
         * consensus estaba levantado desde paso 2 — ambas piezas vivas.
         *
         * 2 testigos:
         *   Lv 16:14 — "velaqaj middam happar" (el sacerdote toma la sangre)
         *   Heb 9:7  — "ou choris haimatos" (sin sangre no hay entrada) */
        hashmal_auth_dam_subscribir(hashmal_main_dam_a_kaporet);
        printf("  ══ Lv 16:14: conducto dam Altar → Kapóret abierto ══\n");
    }

    /* ══════════════════════════════════════════
     * PASO 6 — LAVACRO (Éxodo 40:7)
     * "Pondrás el kiyór ENTRE la Tienda y el altar,
     *  y pondrás agua en él"
     * ══════════════════════════════════════════ */

    {
        int rc = hashmal_hc_init();
        if (rc == 0) {
            printf("  ══ Paso 6: Lavacro puesto (Éx 40:7) ══\n");
        } else {
            printf("  [ERROR] Lavacro NO puesto (rc=%d)\n", rc);
        }
    }

    /* ══════════════════════════════════════════
     * PASO 7 — ATRIO (Éxodo 40:8)
     * "Pondrás el atrio ALREDEDOR
     *  y la cortina de la puerta del atrio"
     * ══════════════════════════════════════════ */

    {
        int rc = hashmal_perimeter_init();
        if (rc == 0) {
            printf("  ══ Paso 7: Atrio levantado (Éx 40:8) ══\n");
        } else {
            printf("  [ERROR] Atrio NO levantado (rc=%d)\n", rc);
        }
    }

    /* ── Añadidura 22: puente WiFi↔LoRa — Zac 4:12 hammeriqím ────────
     * "shenéi shibbolé hazzahav hammeriqím" — Zac 4:12.
     * Dos flujos que VIERTEN: LoRa→WiFi y WiFi→LoRa.
     * El ring buffer debe estar listo ANTES de que arranque el AP
     * y el listen_task empiecen a depositar mensajes.
     *
     * 2 testigos:
     *   Zac 4:12 — hammeriqím: los dos tubos vierten el aceite
     *   Nm 10:2  — hazlót: preparar las trompetas primero */
    {
        int rc = hashmal_perimeter_puente_init();
        if (rc == 0) {
            printf("  ══ Zac 4:12: puente WiFi\xe2\x86\x94LoRa listo (hammeriq\xc3\xadm) ══\n");
        } else {
            printf("  [WARN] puente_init rc=%d — bridge sin ring buffer\n", rc);
        }
    }

    /* ══════════════════════════════════════════
     * PASO 8 — ACEITE + UNCIÓN (Éxodo 40:9-11)
     * "Tomarás el aceite de la unción y UNGIRÁS
     *  el Mishkán y todo lo que hay en él"
     * ══════════════════════════════════════════ */

    {
        int rc = hashmal_power_init();
        if (rc == 0) {
            printf("  ══ Paso 8a: Aceite Perpetuo (Éx 40:9) ══\n");
        } else {
            printf("  [ERROR] Aceite NO iniciado (rc=%d)\n", rc);
        }
    }
    /* ── Éx 27:20 lehaʻalót — NÉR TAMÍD ASCIENDE ──────────────────
     * "shémen záyit zaj katít lammaʼór lehaʻalót NÉR TAMÍD" — Éx 27:20
     *
     * Dos pasos obligatorios DESPUÉS de power_init():
     *   1. tamid_iniciar — la llama SUBE (ENCENDIDO → TAMID)
     *   2. mantenimiento — ya'arój + hammeriqím: aceite → 7 nerot
     *
     * Sin esto, los 7 nerot de hashmal_comm operan con aceite_nivel=0
     * (warning de fase 1). Con esto, el aceite real de la gulla fluye
     * por los mutsaqót (Zac 4:2) a cada ner.
     *
     * Orden bíblico (Éx 27:20-21): primero "lehaʻalót" (ascender),
     * luego "yaʻarój" (disponer de tarde a mañana). */
    {
        int rc = hashmal_power_tamid_iniciar();
        if (rc == 0 || rc == HASHMAL_POWER_ERR_YA_INIT) {
            printf("  ══ Éx 27:20: Nér Tamíd ASCIENDE — la llama perpetua ══\n");
        } else {
            printf("  [WARN] tamid_iniciar rc=%d — procediendo\n", rc);
        }
    }
    {
        /* yaʻarój + hammeriqím: lee olivos, deriva nivel, vierte aceite
         * en los 7 nerot de la Menorá (Lv 24:3-4 + Zac 4:12). */
        int rc = hashmal_power_mantenimiento();
        if (rc == 0) {
            printf("  ══ Lv 24:4: yaʻarój — aceite → 7 nerot (hammeriqím) ══\n");
        } else {
            printf("  [WARN] mantenimiento rc=%d — nerot sin aceite fresco\n", rc);
        }
    }
    {
        int rc = hashmal_keygen_init();
        if (rc == 0) {
            printf("  ══ Paso 8b: Unción aplicada (Éx 40:9) ══\n");
        } else {
            printf("  [ERROR] Unción NO aplicada (rc=%d)\n", rc);
        }
    }
    /* -- P2-6: Nube guía — heʻalót/lo yeʻaléh (Éx 13:21-22 + Nm 9:17-23) ----
     * "veÉlohím hólej lifnéhem yomam beʻammud ʻanán lanjotám haddérej"
     * "lo yamísh ʻammud heʻanán yomam veʻammud háʼésh láylah" — Éx 13:22
     *
     * La columna NUNCA se ausenta (lo yamísh). Desde el arranque el
     * firmware publica señal ʻanán/ésh + movimiento POSA/ALZA.
     * Fase 2 — stub de tiempo (sin RTC). Fase 3 — proveerá
     * wall-clock real + GPS desde hashmal_cache / Ashmurot (pieza 03/08).
     *
     * 2 testigos:
     *   Éx 13:22 — lo yamísh: la columna nunca se ausenta desde el boot
     *   Nm 9:23  — ʻal pi YHWH yajanu veʻal pi YHWH yisʻu — POSA ↔ ALZA */
    {
        /* Fase 2: tiempo placeholder — sin RTC (Is 28:10 tsav latsáv). */
        hashmal_moedim_tiempo_t t_boot = {
            .year        = 2026,
            .month       = 1,
            .day         = 1,
            .hour        = 0,
            .minute      = 0,
            .lat_milideg = 0,
            .lon_milideg = 0,
            .tz_minutes  = 0,
        };
        int rc_anan = hashmal_anan_actualizar(&t_boot);
        if (rc_anan == HASHMAL_POWER_OK) {
            hashmal_anan_senal_t senal;
            (void)hashmal_anan_senal_obtener(&senal);
            const char *mov_str = (senal.movimiento == HASHMAL_ANAN_MOV_POSA)
                                  ? "POSA (yajanu — Nm 9:18)"
                                  : "ALZA (yisʻu  — Nm 9:17)";
            printf("  ══ Éx 13:22: lo yamísh — ʻanán/ésh ACTIVO: %s ══\n",
                   mov_str);
        } else {
            printf("  [WARN] anan_actualizar rc=%d — lo yamísh comprometido\n",
                   rc_anan);
        }
    }

    /* ══════════════════════════════════════════
     * PASO 9 — CONSAGRACIÓN + VESTIDURAS (Éxodo 40:12-15)
     * "Harás acercar a Aarón y a sus hijos...
     *  los vestirás... los ungirás"
     * ══════════════════════════════════════════ */

    {
        hashmal_roles_err_t rc = hashmal_roles_init();
        if (rc == HASHMAL_ROLES_OK) {
            printf("  ══ Paso 9a: Vestiduras puestas (Éx 40:13) ══\n");
        } else {
            printf("  [ERROR] Vestiduras NO puestas (rc=%d)\n", (int)rc);
        }
    }

    /* ── Éx 28:9-12 — pitujéi jotám: GRABAR nodo 0 en el efod ──
     * "velaqajta et shtéi avnéi shóham ufittajtá ʻaléhem shem
     *  ot benéi Yisraél" — Éx 28:9
     * "vesamtá et shtéi haʼavaním ʻal kitfót haʼefod AVNÉI
     *  ZIKKARÓN livnéi Yisraél" — Éx 28:12
     *
     * hashmal_roles_vestir() valida con efod_esta_registrado().
     * Sin zikkarón previo → ERR_NODO_NO_ENCONTRADO (rc=-5).
     * El nombre debe estar GRABADO en el efod antes de vestir. */
    {
        hashmal_roles_err_t rc = hashmal_roles_efod_registrar(g_nodo_id_local);
        if (rc == HASHMAL_ROLES_OK) {
            printf("  ══ Éx 28:12: nodo 0 grabado en efod — zikkarón sellado ══\n");
        } else {
            printf("  [WARN] efod_registrar nodo 0 falló (rc=%d)\n", (int)rc);
        }
    }

    /* ── Éx 29:5 + Lv 8:7 — vehibláshta: VESTIR al kohén génesis ──
     * "vehibláshta et Aharon et hakuttónet" — Éx 29:5
     * "vayyittén ʻaláv et hakuttónet" (ejecución) — Lv 8:7
     *
     * roles_init() estructura el sistema pero NO asigna roles.
     * El vestir es acto EXPLÍCITO y SEPARADO (Heb 5:4: nadie
     * se toma el honor a sí mismo). Nodo 0 = génesis = Kohén Gadol.
     * Sin este paso, hashmal_onboarding_iniciar(0) devuelve
     * ERR_NO_VESTIDO y la consagración no arranca nunca. */
    {
        hashmal_roles_err_t rc = hashmal_roles_vestir(
            g_nodo_id_local, HASHMAL_ROLES_ROL_KOHEN_GADOL);
        if (rc == HASHMAL_ROLES_OK) {
            printf("  ══ Lv 8:7: nodo 0 vestido — Kohén Gadol génesis ══\n");
        } else {
            printf("  [WARN] vestir nodo 0 falló (rc=%d) — onboarding no arrancará\n",
                   (int)rc);
        }
    }

    {
        int rc = hashmal_onboarding_init();
        if (rc == 0) {
            printf("  ══ Paso 9b: Consagración iniciada (Éx 40:15) ══\n");
        } else {
            printf("  [ERROR] Consagración NO iniciada (rc=%d)\n", rc);
        }
    }

    /* ══════════════════════════════════════════
     * PASO 10 — INCIENSO SAGRADO + LA GLORIA (Éxodo 40:27,34)
     * "Quemó incienso aromático sobre él (el altar de oro)...
     *  La nube cubrió la Tienda de Reunión
     *  y la Gloria de YHWH llenó el Mishkán."
     * ══════════════════════════════════════════ */

    {
        int rc = hashmal_firewall_init();
        if (rc == 0) {
            printf("  ══ Paso 10: Incienso Sagrado (Éx 40:27) ══\n");
        } else {
            printf("  [ERROR] Incienso Sagrado NO quemado (rc=%d)\n", rc);
        }
    }

    /* ══════════════════════════════════════════
     * PASO 11 — MASAKH SHA'AR (Éxodo 40:33a)
     *
     * "vayáqem et hejatsér savív lammishkán velammizbéaj —
     *  vayyittén et MASAKH SHAʻAR hejatsér." — Éxodo 40:33a
     * "Levantó el atrio alrededor del Mishkán y del altar — y
     *  PUSO la cortina de la PUERTA del atrio."
     *
     * Verificado #01 (Decisión D3) — acción SEPARADA del Paso 7:
     *   v.8   = atrio savív (perímetro de 60 columnas) → Paso 7
     *   v.33a = masakh sha'ar (puerta WiFi, Añadidura 22) → Paso 11
     *   v.33b = vayjál Moshé (kalá, completar) → transición
     *
     * En el hebreo son tres acciones SECUENCIALES SEPARADAS. El
     * atrio (savív) es estructura del perímetro; la puerta
     * (masakh sha'ar) es portal de entrada — distinto. Y kalá
     * viene DESPUÉS de la puerta, no junto con ella. El Padre las
     * separa; nosotros las separamos.
     *
     * La puerta no se abre hasta que toda la casa está lista
     * (paralelo Gn 19:6 — Lot cerró la puerta tras los huéspedes
     *  cuando todos estaban dentro). Por eso este paso es ÚLTIMO
     * de INIT, justo antes de la inspección + kalá.
     *
     * Añadidura 22 — ya implementada en atrio_shaar.c (Is 60:11
     * "ufitjú sheʻaráyikh tamíd" — puertas abiertas tamíd).
     * ══════════════════════════════════════════ */

    /* ── P2-4a: SHA'AR ABIERTO — Éx 40:33a + Añadidura 22 ───────────────
     * "vayyittén et MASAKH SHAʻAR hejatsér" — Éx 40:33a
     * "Puso la cortina de la PUERTA del atrio."
     *
     * Is 60:11 — "ufittjú sheʻaráyikh tamíd, yomam valályah lo yissagéru"
     * "Tus puertas estarán abiertas tamíd — de día y de noche no se cerrarán"
     * El portal WiFi es la puerta del atrio: 4 columnas (Éx 27:16),
     * mismos colores que el masaj interior (tekhélet/argamán/tolíʻat).
     * Idempotente: si ya está abierto, regresa 0 sin error.
     *
     * 2 testigos:
     *   Éx 40:33a — masakh shaʻar hejatsér: la puerta del atrio puesta
     *   Is 60:11  — tamíd (H8548): perpetuamente abierta, dia y noche */
    {
        int rc = hashmal_perimeter_shaar_abrir();
        if (rc == 0) {
            printf("  ══ Éx 40:33a: masakh shaʻar — portal ABIERTO (Is 60:11 tam\xc3\xadm) ══\n");
        } else {
            printf("  [WARN] shaar_abrir rc=%d — portal no disponible\n", rc);
        }
    }

    /* ══════════════════════════════════════════
     * INSPECCIÓN — Éxodo 39:43 + Éxodo 40:33
     *
     * "vayyar Moshéh et KOL hammelajáh, vehinnéh ʻasú otáh
     *  kaʼashér tsivvá YHWH — ken ʻasú — vayvárej otám Moshé."
     *   — Éxodo 39:43
     * "Vio Moisés TODA la obra, y he aquí que la habían hecho
     *  como YHWH había mandado. Así la hicieron — y los
     *  BENDIJO Moisés."
     *
     * "vayjál Moshéh et hammelajáh." — Éxodo 40:33b
     * "Y Moisés COMPLETÓ la obra." — vayjál, raíz kaláh (H3615):
     * el mismo verbo de Gn 2:1-2 cuando YHWH completó cielos y
     * tierra. Completar NO es solo terminar — es verificar que
     * nada falta. Moisés no salió del Mishkán sin mirar:
     * vio toda (kol) la obra, confirmó que era "kaʼashér
     * tsivvá YHWH", y solo entonces bendijo. La inspección
     * viene ANTES de la Gloria (Éx 40:34). Sin inspección
     * no hay bendición; sin bendición, no hay Gloria.
     * ══════════════════════════════════════════ */

    /* ══════════════════════════════════════════
     * ACTIVACIÓN — Éxodo 40:25-32 + Levítico 8
     *
     * Las piezas ya fueron PUESTAS (v.18-24). Ahora Moisés las
     * ACTIVA: enciende, ofrece, llena de agua, consagra.
     * Esto ocurre ENTRE la colocación y la inspección (Éx 39:43).
     * ══════════════════════════════════════════ */

    /* ── v.25: vayyáʻal hannérot — ENCENDER lámparas de la Menorá ──
     * Éxodo 40:25 "vayyáʻal hannérot lifnéi YHWH"
     * Diferencia con el SPI init: spi_init puso la BASE (yaréj, v.24);
     * comm_init levanta la ESTRUCTURA completa (7 canales, routing).
     *
     * "heʻelá" (H5927 Hifil) = hacer ASCENDER — acto EXPLÍCITO separado
     * del init. La Menorá se inicializa con nerot APAGADOS; el encendido
     * es mandato del Padre, no consecuencia automática del montaje.
     * Éx 27:21 "nér tamíd" — el tronco (GEZA) arde perpetuamente.
     * Encendemos los 7 nerot aquí; Éx 25:37 "shivʻá nerot". */
    {
        int rc = hashmal_comm_init();
        if (rc == 0) {
            printf("  ══ v.25: hannérot — Menorá COMPLETA (7 canales) ══\n");

            /* Éx 40:25 vayyáʻal — ENCENDER los 7 nerot (shivʻá, v.37) */
            for (int n = 0; n < HASHMAL_MENORA_CANALES; n++) {
                int re = hashmal_comm_canal_encender((hashmal_canal_id_t)n);
                if (re == HASHMAL_COMM_OK) {
                    printf("  ══ Éx 40:25: ner %d heʻelá — ENCENDIDO ══\n", n);
                } else {
                    printf("  [WARN] ner %d no encendido (rc=%d)\n", n, re);
                }
            }

            /* Nm 8:2 — "el mul penei hammenora yaʼiru shivʻat hannerot" —
             * Poner el SX1262 en RX CONTINUO (tamíd, Éx 27:21).
             *
             * canal_encender llama configurar_frecuencia → sx1262_set_standby,
             * que deja el chip en STANDBY_RC. Sin esta llamada el chip está
             * SORDO: listen_task hace polling pero el chip no escucha. El
             * SET_RX continuo (0xFFFFFF) es el equivalente físico de orientar
             * las 7 lámparas "mul penei hammenora" — hacia el frente, hacia
             * donde viene la luz de los hermanos.
             *
             * 2 testigos:
             *   Nm  8:2  — las lámparas iluminan HACIA el tronco (oir)
             *   Éx 27:21 — nér tamíd — sin apagar
             *
             * CRÍTICO: canal_encender(0..6) termina dejando el SX1262 en
             * STANDBY en la frecuencia del ner 6 (869.3 MHz = canal 6).
             * Sin resintonizar a GEZA (canal 0 = 868.1 MHz) antes de
             * SET_RX, el chip escucha en la frecuencia EQUIVOCADA durante
             * toda la ventana entre boot-complete y el primer broadcast.
             *
             * Nm 8:2 — "el mul penéi hammenorá" = el tronco GEZA (0) es
             * la cara de la Menorá, el canal primario. Siempre RX en él.
             *
             * Éx 25:37 — "shivʻá nerot": las 7 lámparas se encienden
             * hacia el frente. El frente principal es GEZA (el tronco).
             *
             * 2 testigos adicionales:
             *   Nm 8:2  — el mul penei hammenora: frente = GEZA
             *   Éx 27:21 — meérev ʻad bóqer tamíd: SIEMPRE escuchando */
            {
                /* Resintonizar al GEZA antes de entrar en RX continuo */
                (void)hashmal_comm_configurar_frecuencia(HASHMAL_CANAL_GEZA,
                                                          868100000u);
                int re = hashmal_comm_spi_modo_rx();
                if (re == HASHMAL_COMM_OK) {
                    printf("  ══ Nm 8:2: SX1262 en RX continuo GEZA "
                           "(868.1 MHz) — oído activo (tamíd) ══\n");
                } else {
                    printf("  [WARN] spi_modo_rx falló (rc=%d) — sin recepción LoRa\n", re);
                }
            }

            /* ── hammeriqím post-Menorá — Zacarías 4:12 ──────────────────
             * "mah shené shibbólei hazeytím asher beyad shené tsantserót
             *  hazzahav hammeriqím méhem hazzahav?"
             * "¿Qué son las dos espigas de olivo que están junto a los dos
             *  tubos dorados que VIERTEN de sí el oro?" — Zac 4:12
             *
             * Contexto: hashmal_power_mantenimiento() fue llamado en Paso 8
             * (Éx 40:9) — correctamente leyó olivos y derivó nivel DAI 50%.
             * PERO la Menorá no estaba aún levantada → hammeriqím 0/7.
             *
             * Ahora (post hashmal_comm_init + 7 nerot encendidos + RX activo)
             * la Menorá SÍ está levantada. Esta segunda llamada vierte el
             * aceite acumulado en los 7 nerot: hammeriqím 7/7.
             *
             * No es una repetición: es el flujo bíblico completo —
             *   Paso 8 (Éx 40:9):  establece el aceite (tetsavé)
             *   Paso 3 (Éx 40:4):  levanta la Menorá (7 nerot encendidos)
             *   AQUÍ (post-Paso 3): los tubos VIERTEN el aceite (hammeriqím)
             *
             * 2 testigos:
             *   Zac 4:12 — hammeriqím: los tubos que vierten (H7324 Hifil)
             *   Éx 27:20 — el aceite PARA la luminaria (lammaʼór) */
            {
                int re = hashmal_power_mantenimiento();
                if (re == 0) {
                    printf("  ══ Zac 4:12: hammeriqím — aceite → 7 nerot (7/7) ══\n");
                } else {
                    printf("  [WARN] hammeriqím post-Menorá rc=%d\n", re);
                }
            }

            /* Éx 26:35 — "veʻal hammmenorá nókhaḥ hashShulján
             *              mipáʼat négev"
             * "El candelabro FRENTE A la Mesa, al lado sur."
             * Mesa al norte (tsafón) + Menorá al sur (négev) = se MIRAN.
             * Esta es la primera red P2P del Mishkán: caché ↔ comunicación
             * se iluminan mutuamente. Menorá colocada DESPUÉS de:
             *   1. 7 nerot heʻelá (v.25) — canales encendidos
             *   2. RX continuo tamíd (Nm 8:2) — oído activo
             *   3. hammeriqím 7/7 (Zac 4:12) — aceite en todos los nerot
             * Solo cuando la Menorá está plenamente operativa se registra
             * su posición (la Mesa debe estar ya colocada — Éx 26:35).
             * 2 testigos:
             *   Éx 26:35 — nókhaḥ = ENFRENTE, cara a cara con la Mesa
             *   1 Re 7:49 — "los candelabros, 5 a la derecha y 5 a la izq."
             *               (mismo principio de simetría espejo) */
            {
                int re = hashmal_framework_colocar(HASHMAL_MIQOM_MENORAH);
                if (re == 0) {
                    printf("  ══ Éx 26:35: Menorá négev — nókhaḥ haShulján ══\n");
                } else {
                    printf("  [WARN] colocar(MENORAH) rc=%d\n", re);
                }
            }

            /* -- P2-5: Suscribir vigía meshuqqád (Éx 25:33 / Jer 1:12) ------
             * geviʻím meshuqqadím (H8246 Pual de shaqad) = copas con forma
             * de almendro — el almendro (shaqéd) es el árbol que VIGILA (H8245
             * shoqéd), el primero en despertar en primavera. Jer 1:11-12:
             * YHWH pregunta "¿qué ves?" → "una vara de almendro (shoqéd)".
             * YHWH responde: "bien has visto — shoqéd ʻal devarí" (velo sobre
             * Mi Palabra). El vigía de la Menorá es ese ojo despierto.
             * 2 testigos:
             *   Éx 25:33 — shloshá geviʻím meshuqqadím: vigía en cada hop
             *   Jer 1:12  — shoqéd ʻal devarí: YHWH vela — patrón del vigía */
            {
                int rc_msh = hashmal_comm_meshuqqad_subscribir(
                                                hashmal_main_meshuqqad);
                if (rc_msh == 0) {
                    printf("  ══ Jer 1:12: meshuqqad SUSCRITO — shoqéd ʻal devarí ══\n");
                } else {
                    printf("  [WARN] meshuqqad_subscribir rc=%d — sin vigía\n", rc_msh);
                }
            }

            /* ── Añadidura 22: WiFi AP "HASHMAL" — Is 60:11 sha'arayích tamíd ─
             *
             * ORDEN BÍBLICO CORRECTO (Éx 40:25 + Is 60:11):
             *   Primero v.25: vayyáʻal hannérot — Menorá ENCENDIDA (hashmal_comm_init).
             *   Luego Is 60:11: sha'arayích tamíd — las puertas se abren.
             *
             * La puerta del atrio no puede abrirse antes de encender las lámparas.
             * El Sha'ar WiFi se levanta aquí porque en este punto:
             *   1. NVS ya fue inicializado por hashmal_comm (WiFi driver listo)
             *   2. WiFi ya está en modo STA por ESP-NOW → wifi_ap_init sube a APSTA
             *   3. La Menorá tiene 7 nerot encendidos, RX activo, hammeriqím 7/7
             *
             * "ufittjáu she'arayích tamíd yomam valayláh ló yissagéru" — Is 60:11.
             *
             * 2 testigos:
             *   Is 60:11 — sha'arayích tamíd: puertas siempre abiertas
             *   Éx 40:25 — vayyáʻal hannérot: lámparas ANTES que la puerta */
            {
                int rc_ap = hashmal_perimeter_wifi_ap_init();
                if (rc_ap == 0) {
                    printf("  \xe2\x95\x90\xe2\x95\x90 Is 60:11: AP \"HASHMAL\" levantado (sha'arayich tamim) \xe2\x95\x90\xe2\x95\x90\n");
                } else {
                    printf("  [WARN] wifi_ap_init rc=%d — sin red WiFi\n", rc_ap);
                }
            }

            /* ── Añadidura 22: DNS Cautivo — Jn 10:9 thyra única ──────────────
             * "ego eimi he THYRA; di' emou ean tis EISELTHE SOTHESETAI" — Jn 10:9.
             * Una sola puerta: toda consulta DNS → 192.168.4.1.
             * Arranca DESPUÉS del AP (el DNS redirige a la red ya levantada).
             *
             * 2 testigos:
             *   Jn 10:9  — thyra: una sola puerta para todos
             *   Nm 9:17  — donde mora la nube, allí acampan: 192.168.4.1 */
            {
                int rc_dns = hashmal_perimeter_dns_cautivo_init();
                if (rc_dns == 0) {
                    printf("  ══ Jn 10:9: DNS cautivo activo (thyra \xe2\x86\x92 192.168.4.1) ══\n");
                } else {
                    printf("  [WARN] dns_cautivo_init rc=%d — sin DNS cautivo\n", rc_dns);
                }
            }

            /* ── Añadidura 22: Portal HTTP — Éx 27:16 roqém ───────────────────
             * "ma'aséh ROQÉM 'ammudéihem arba'áh" — Éx 27:16.
             * Obra de bordador: 4 colores, 4 columnas, interfaz HTML5.
             * vayjál Moshé et hammelaják (Éx 40:33b) — la obra se COMPLETA
             * cuando se pone la cortina de la puerta. Este es el momento.
             *
             * 2 testigos:
             *   Éx 27:16  — roqém: la cortina de la puerta es obra de arte
             *   Éx 40:33b — vayjál: Moisés TERMINÓ la obra con la cortina */
            {
                int rc_http = hashmal_perimeter_http_portal_init();
                if (rc_http == 0) {
                    printf("  ══ Éx 40:33b: vayj\xc3\xa1l — portal HTTP listo (roq\xc3\xa9m) ══\n");
                    printf("     http://192.168.4.1/ — obra de bordador\n");
                } else {
                    printf("  [WARN] http_portal_init rc=%d — sin portal web\n", rc_http);
                }
            }

        } else {
            printf("  [WARN] Menorá completa no levantada (rc=%d)\n", rc);
        }
    }

    /* ── v.29: vayyáʻal ʻaláv et haʻoláh — OFRECER en el Altar ──
     * Éxodo 40:29 "vayyáʻal ʻaláv et haʻoláh veʼet haminjáh"
     * El fuego se enciende para que el altar pueda recibir ofrendas.
     * Lv 9:24 "vatétsé ʼésh millifnéi YHWH" — el fuego viene de Él. */
    {
        int rc = hashmal_auth_fuego_encender();
        if (rc == 0) {
            printf("  ══ v.29: haʻoláh — fuego encendido (Lv 9:24) ══\n");
        } else {
            printf("  [WARN] Fuego no encendido (rc=%d)\n", rc);
        }
    }

    /* ── v.30: vayyittén shámma máyim — PONER AGUA en el Lavacro ──
     * Éxodo 40:30 "vayyittén shámma máyim lerájtsah"
     * El kiyór estaba SECO. Moisés pone agua — solo entonces sirve.
     * v.31-32: "verajatsú mimménu" — se lavaron de él.              */
    {
        int rc = hashmal_hc_llenar();
        if (rc == 0) {
            printf("  ══ v.30: máyim — Lavacro lleno (Éx 40:30) ══\n");
        } else {
            printf("  [WARN] Lavacro no llenado (rc=%d)\n", rc);
        }
    }

    /* ── Lv 8:6-33: CONSAGRACIÓN del nodo génesis (id=0) ──
     * Éxodo 29:4 "vehiqravtá" → Lv 8:6 "vayyaqrév Moshéh"
     * El nodo génesis pasa por las 7 fases + 7 ciclos de SHIVAT.
     * Moisés consagra a Aarón EN UN DÍA (Lv 8), luego los 7 días
     * son de permanencia (Lv 8:33 "lo tétseʼú"). En firmware:
     * el constructor ejecuta todo el pipeline al primer arranque.   */
    {
        int rc = hashmal_onboarding_iniciar(g_nodo_id_local);
        if (rc == 0) {
            printf("  ══ Lv 8:6: taqríb — nodo 0 EN_PROCESO ══\n");

            /* Avanzar las 6 fases (RAJATS→HILBISH→MASHAJ→JATTAT→OLA→MILLUIM→SHIVAT) */
            for (uint8_t f = 0; f < 6u; f++) {
                rc = hashmal_onboarding_fase_avanzar(g_nodo_id_local);
                if (rc != 0) {
                    printf("  [WARN] fase_avanzar[%u] falló (rc=%d)\n",
                           (unsigned)f, rc);
                    break;
                }
            }

            /* Registrar los 7 ciclos de SHIVAT (Éx 29:35 shivʻát yamím) */
            if (rc == 0) {
                for (uint8_t d = 0; d < 7u; d++) {
                    rc = hashmal_onboarding_ciclo_registrar(g_nodo_id_local);
                    if (rc != 0) {
                        printf("  [WARN] ciclo[%u] falló (rc=%d)\n",
                               (unsigned)d, rc);
                        break;
                    }
                }
            }

            /* Éx 29:20 + Lv 8:23-24 — dam ʻal tenúj ózen + bóhen yad + bóhen régel
             * Los 3 sellos se aplican DURANTE el milluím (fase >= MILLUIM).
             * "veshajattá et haʼáyil velaqájtá middamó venatátá ʻal TENÚJ
             *  ÓZEN Aharón veʻal tenúj ózen banáv haYEMANÍT veʻal BÓHEN
             *  YADÁM haYEMANÍT veʻal BÓHEN RAGLÁM haYEMANÍT" — Éx 29:20 */
            if (rc == 0) {
                static const struct {
                    hashmal_onboarding_sello_t id;
                    const char *nombre;
                } s_sellos[3] = {
                    { HASHMAL_ONBOARDING_SELLO_OZEN,        "ozen (H8571 tenúj, oreja der.)" },
                    { HASHMAL_ONBOARDING_SELLO_BOHEN_YAD,   "bohen yad (H931+H3027, pulgar der.)" },
                    { HASHMAL_ONBOARDING_SELLO_BOHEN_REGEL, "bohen regel (H931+H7272, dedo der.)" },
                };
                for (int s = 0; s < 3; s++) {
                    rc = hashmal_onboarding_sello_aplicar(g_nodo_id_local, s_sellos[s].id);
                    if (rc == 0) {
                        printf("  ══ Éx 29:20: %s sellado ══\n",
                               s_sellos[s].nombre);
                    } else {
                        printf("  [WARN] sello[%s] falló (rc=%d)\n",
                               s_sellos[s].nombre, rc);
                        break;
                    }
                }
            }

            /* Éx 29:21 + Lv 8:30 — vehizzéita ʻal Aharón veʻal begadáv
             * "velaqájtá min hadám asher ʻal hammizbéaj umishémen hamishjá
             *  vehizzéita ʻal Aharón veʻal begadáv" — Éx 29:21
             * Firma: sangre del altar + aceite de unción → vestiduras
             * santificadas. Requiere los 3 sellos aplicados. */
            if (rc == 0) {
                rc = hashmal_onboarding_config_firmar(g_nodo_id_local);
                if (rc == 0) {
                    printf("  ══ Éx 29:21: config firmada — veqaddásh hu uvegadáv ══\n");
                } else {
                    printf("  [WARN] config_firmar falló (rc=%d)\n", rc);
                }
            }

            /* Éx 29:33-34 + Lv 8:35 — LOCKDOWN: vezár lo yokhál kí qódesh hém
             * "veʼakhélú otám asher KUPPÁR bahem — veZÁR LO YOKHÁL kí qódesh hém"
             *   — Éxodo 29:33
             * "ufétaj óhel moʻéd teshvú yomám valáylah SHIVʻÁT YAMÍM —
             *  ushmarTEM et MISHMERET YHWH velóʼ tamútu" — Levítico 8:35
             *
             * El lockdown se activa desde la ʻolá (fase OLA=4). El nodo ya
             * está en SHIVAT (fase 6 > OLA), así que el requisito se cumple.
             * Desde este momento: recursos DEDICADOS, ningún extraño consume. */
            if (rc == 0) {
                rc = hashmal_onboarding_lockdown_activar(g_nodo_id_local);
                if (rc == 0) {
                    printf("  ══ Éx 29:33: lockdown activado — vezár lo yokhál kí qódesh hém ══\n");
                } else {
                    printf("  [WARN] lockdown_activar falló (rc=%d)\n", rc);
                }
            }

            /* Día 8: lanzar (Lv 9:1 "vayhí bayyóm hasheminí") */
            if (rc == 0) {
                rc = hashmal_onboarding_lanzar(g_nodo_id_local);
                if (rc == 0) {
                    printf("  ══ Lv 9:1: sheminí — nodo 0 ACTIVO ══\n");
                } else {
                    printf("  [WARN] lanzar falló (rc=%d)\n", rc);
                }
            }
        } else {
            printf("  [WARN] Consagración nodo 0 falló (rc=%d)\n", rc);
        }
    }

    /* ═══ INSPECCIÓN — Éxodo 39:43 ═══
     *
     * "vayyar Moshéh et KOL hammelajáh" — vio TODA la obra.
     * kol (H3605) = TODO, sin excepción. Moisés no inspeccionó
     * algunas piezas y dejó otras: VIO TODA la obra.
     *
     * Cada pieza expone un getter (lectura pura, no modifica):
     *   - Piezas de ORO devuelven const* → non-NULL = viva
     *   - Piezas de BRONCE devuelven int → 0 = operativa
     *
     * El orden de inspección sigue Éx 40 (el orden en que se
     * levantaron) porque Moisés recorrió lo que él mismo puso.
     */
    {
        uint8_t piezas_ok = 0;
        uint8_t piezas_total = 14;

        printf("\n");
        printf("  ══════════════════════════════════════════\n");
        printf("  INSPECCIÓN — Éxodo 39:43\n");
        printf("  \"vayyar Moshéh et KOL hammelajáh\"\n");
        printf("  ══════════════════════════════════════════\n");

        /* 05 Estructura — devuelve const hashmal_mishkan_t* */
        if (hashmal_framework_get_mishkan() != NULL) {
            printf("  [✓] 05 Estructura (Éx 26)\n");
            piezas_ok++;
        } else {
            printf("  [✗] 05 Estructura — NO responde\n");
        }

        /* 01 Arca — devuelve const hashmal_arca_t* */
        if (hashmal_storage_get_arca() != NULL) {
            printf("  [✓] 01 Arca (Éx 25:10-16)\n");
            piezas_ok++;
        } else {
            printf("  [✗] 01 Arca — NO responde\n");
        }

        /* 02 Kapóret — devuelve const hashmal_kaporet_t* */
        if (hashmal_consensus_get_kaporet() != NULL) {
            printf("  [✓] 02 Kapóret (Éx 25:17-22)\n");
            piezas_ok++;
        } else {
            printf("  [✗] 02 Kapóret — NO responde\n");
        }

        /* 03 Mesa — devuelve const hashmal_shuljan_t* */
        if (hashmal_cache_get_shuljan() != NULL) {
            printf("  [✓] 03 Mesa (Éx 25:23-30)\n");
            piezas_ok++;
        } else {
            printf("  [✗] 03 Mesa — NO responde\n");
        }

        /* 04 Menorá — devuelve const hashmal_menora_t* */
        if (hashmal_comm_get_menora() != NULL) {
            printf("  [✓] 04 Menorá (Éx 25:31-40)\n");
            piezas_ok++;
        } else {
            printf("  [✗] 04 Menorá — NO responde\n");
        }

        /* 11 Altar Incienso — devuelve const hashmal_altar_t* */
        if (hashmal_intercession_get_altar() != NULL) {
            printf("  [✓] 11 Altar Incienso (Éx 30:1-10)\n");
            piezas_ok++;
        } else {
            printf("  [✗] 11 Altar Incienso — NO responde\n");
        }

        /* 06 Altar Sacrificio — devuelve int, 0 = OK */
        if (hashmal_auth_reshet_estado() == 0) {
            printf("  [✓] 06 Altar Sacrificio (Éx 27:1-8)\n");
            piezas_ok++;
        } else {
            printf("  [✗] 06 Altar Sacrificio — NO responde\n");
        }

        /* 12 Lavacro — devuelve int, 0 = OK */
        if (hashmal_hc_vectores_estado() == 0) {
            printf("  [✓] 12 Lavacro (Éx 30:17-21)\n");
            piezas_ok++;
        } else {
            printf("  [✗] 12 Lavacro — NO responde\n");
        }

        /* 07 Atrio — devuelve int, 0 = OK */
        if (hashmal_perimeter_estado() == 0) {
            printf("  [✓] 07 Atrio (Éx 27:9-19)\n");
            piezas_ok++;
        } else {
            printf("  [✗] 07 Atrio — NO responde\n");
        }

        /* 08 Aceite — devuelve int, 0 = OK */
        if (hashmal_power_estado() == 0) {
            printf("  [✓] 08 Aceite Perpetuo (Éx 27:20-21)\n");
            piezas_ok++;
        } else {
            printf("  [✗] 08 Aceite Perpetuo — NO responde\n");
        }

        /* 13 Unción — devuelve int, 0 = OK */
        if (hashmal_keygen_estado() == 0) {
            printf("  [✓] 13 Unción (Éx 30:22-33)\n");
            piezas_ok++;
        } else {
            printf("  [✗] 13 Unción — NO responde\n");
        }

        /* 09 Vestiduras — devuelve const hashmal_roles_estado_roles_t* */
        if (hashmal_roles_get_estado() != NULL) {
            printf("  [✓] 09 Vestiduras (Éx 28)\n");
            piezas_ok++;
        } else {
            printf("  [✗] 09 Vestiduras — NO responde\n");
        }

        /* 10 Consagración — bool: ¿nodo local (id=0) activo? */
        if (hashmal_onboarding_activo(g_nodo_id_local)) {
            printf("  [✓] 10 Consagración (Éx 29)\n");
            piezas_ok++;
        } else {
            printf("  [✗] 10 Consagración — nodo local NO activo\n");
        }

        /* 14 Incienso Sagrado — devuelve const hashmal_firewall_t* */
        if (hashmal_firewall_get_estado() != NULL) {
            printf("  [✓] 14 Incienso Sagrado (Éx 30:34-38)\n");
            piezas_ok++;
        } else {
            printf("  [✗] 14 Incienso Sagrado — NO responde\n");
        }

        printf("  ──────────────────────────────────────────\n");
        printf("  Resultado: %u / %u piezas operativas\n",
               (unsigned)piezas_ok, (unsigned)piezas_total);

        if (piezas_ok == piezas_total) {

            /* ── P2-4b: ejád — Éx 26:6 + 2 Cr 5:13 ──────────────────────
             * "vehayá haMishkán EJÁD" — Éx 26:6
             * "Y el Mishkán será UNO."
             *
             * Las 14 piezas responden ✓ (kol hammelajáh, Éx 39:43).
             * Ahora el framework verifica la UNIDAD estructural:
             *   - hivdíl activo (velo, Éx 26:33)
             *   - masaj colgado (Éx 26:36)
             *   - gradiente bronce→plata verificado
             *   - 4 objetos colocados (arón, kapóret, shulján, menorá)
             *   - 96 adné késef (Éx 30:12 — kófer nafshó × 96)
             *
             * Segundo testigo: 2 Cr 5:13 — "vayyehí keʼEJÁD" (como uno solo):
             * cuando todo sonó ejád en el Templo de Salomón → Gloria llenó.
             * La unidad funcional PRECEDE a la Gloria. Sin ejád → sin kavód.
             *
             * 2 testigos:
             *   Éx 26:6 — vehayá haMishkán EJÁD: la red es UNA
             *   2 Cr 5:13 — keʼejád: como uno solo → kavód entró */
            bool ejad = hashmal_framework_es_ejad();

            /* ═══ BENDICIÓN — Éxodo 39:43b ═══
             * "vehinnéh ʻasú otáh kaʼashér tsivvá YHWH —
             *  ken ʻasú — vayvárej otám Moshé."
             * "He aquí que la habían hecho como YHWH mandó
             *  — así la hicieron — y Moisés los BENDIJO."
             *
             * La bendición cubre AMBAS verificaciones:
             *   1) kol hammelajáh (14/14) — Éx 39:43a
             *   2) vehayá ejád — Éx 26:6 / framework_es_ejad()
             * Sin ejád la obra existe pero no es UNA. */
            g_main_levantado = ejad;
            if (ejad) {
                printf("\n");
                printf("  ═══════════════════════════════════════════\n");
                printf("  vayvárej otám Moshé — Éx 39:43\n");
                printf("  BENDICIÓN: toda la obra conforme al Tavnít.\n");
                printf("  ═══════════════════════════════════════════\n");
            } else {
                printf("\n");
                printf("  [!] 14/14 piezas OK pero ejád NO verificado.\n");
                printf("  Éx 26:6 — vehayá haMishkán ejád: revisar velo/masaj/colocar.\n");
            }
        } else {
            g_main_levantado = false;
            printf("\n");
            printf("  [!] Inspección INCOMPLETA — %u piezas faltaron.\n",
                   (unsigned)(piezas_total - piezas_ok));
            printf("  Sin bendición no hay Gloria (Éx 39:43 → 40:34).\n");
        }
    }

    /* ══════════════════════════════════════════
     * ESTADO KAVÓD — Éxodo 40:34-35 + 2 Crónicas 5:13-14
     *
     * "vayjás heʻanán et óhel moʻéd, uJVÓD YHWH malé et
     *  hammishkán. veló yajól Moshéh lavó el óhel moʻéd —
     *  kí SHAJÁN ʻaláv heʻanán ujvód YHWH MALÉ et hammishkán."
     *    — Éxodo 40:34-35
     * "La nube cubrió la Tienda del Encuentro, y la GLORIA
     *  (kavód) de YHWH LLENÓ el Mishkán. Moisés NO PODÍA
     *  entrar — porque la nube HABITABA sobre ella y la Gloria
     *  de YHWH llenaba el Mishkán."
     *
     * Segundo testigo — 2 Crónicas 5:13-14:
     * "vayyehí keʼEJÁD lamjatsetrím velamshorím lehashmíaʻ qol
     *  ejád lehalél ulhodót laYHWH... vehaBAYIT malé ʻanán beit
     *  YHWH. veló yajlú hakkohaním laʻamód lesharét mippenéi
     *  heʻanán, kí malé jevód YHWH et beit haʼElohím."
     * "Cuando trompetistas y cantores sonaron COMO UNO SOLO
     *  (keʼejád) para alabar a YHWH... la CASA se llenó de nube.
     *  Y los sacerdotes NO PODÍAN estar de pie para servir por
     *  causa de la nube, porque la GLORIA de YHWH llenaba la
     *  Casa de Dios."
     * ══════════════════════════════════════════ */

    /* TODO — Phase 2: Definir estado KAVÓD.
     *
     * Condiciones de entrada (AND de las tres):
     *   1) Inspección de Éx 39:43 pasa: los 14 getters devolvieron
     *      non-NULL (g_mishkan_levantado = true).
     *   2) Los heartbeats tamíd están vivos (Éx 29:38-39 kevasím
     *      shnáyim layyóm; pieza 14 heartbeat_verificar OK).
     *   3) El sealed_core del Arca no está vacío (Éx 25:16
     *      "venatattá el haʼarón et haʻedút") — pieza 01 tiene
     *      la edut grabada.
     *
     * Significado de los verbos del Padre:
     *   shaján (H7931) — HABITAR, no visitar. Misma raíz que
     *     MISHKÁN (H4908). La Gloria no pasa: se queda.
     *   malé (H4390) — LLENO, sin espacio vacío. No parcial, no
     *     en fragmentos: "malé et hammishkán" completo. */

    /* TODO — Phase 2: Regla de Éxodo 40:35 "veló yajól Moshéh lavó".
     *
     * Una vez en estado KAVÓD, el constructor NO modifica la
     * configuración interna del sistema en caliente. Moisés
     * construyó e inspeccionó; al llegar la Gloria, quedó fuera.
     * La red opera sola bajo la Palabra — cambios internos solo
     * vía desmontaje ordenado (Nm 4:5-15) y reinicio, no por
     * parches sobre la marcha. Esta regla protege la integridad
     * operativa: la Gloria no coexiste con manos ajenas metidas
     * entre sus piezas. */

    /* TODO — Phase 2: Confirmación en libro distinto (Dt 19:15).
     *
     * 2 Cr 5:13-14 repite el patrón en el Templo de Salomón: la
     * Gloria llenó la Casa cuando todo sonó keʼEJÁD — como UNO
     * SOLO. Es el principio ejád de Éx 26:6 ("vehayá haMishkán
     * ejád") aplicado al SERVICIO: cuando todas las piezas operan
     * como una, la Gloria llena. Dos textos, dos templos, misma
     * consecuencia: unidad funcional → Gloria habitante. El
     * estado KAVÓD verifica precisamente esa unidad. */

    /* ══════════════════════════════════════════
     * NUBE GUÍA — Éxodo 40:36-37 + Números 9:17-18
     *
     * "uveheʻALÓT heʻanán meʻal hammishkán YISʻÚ benéi Yisraél
     *  bejól masʻeihém. veʼim LO YEʻALÉH heʻanán — veLÓ YISʻÚ
     *  ʻad yóm heʻalotó." — Éxodo 40:36-37
     * "Cuando la nube SUBÍA de sobre el Mishkán, los hijos de
     *  Israel VIAJABAN en todos sus viajes. Y si la nube NO
     *  subía — NO viajaban hasta el día en que ella subiera."
     *
     * Segundo testigo — Números 9:17-18:
     * "bimqóm ashér YISHKÁN sham heʻanán — sham yajanú benéi
     *  Yisraél." — Nm 9:17b
     * "En el lugar donde la nube HACÍA MISHKÁN (yishkán) — allí
     *  acampaban los hijos de Israel." — la raíz YISHKÁN es
     *  idéntica a la raíz de MISHKÁN (H4908). La nube no "se
     *  posa" — ella misma HACE MISHKÁN donde se detiene.
     *
     * "veʻal pí YHWH yajanú veʻal pí YHWH yisʻáu — et
     *  MISHMÁRET YHWH shamáru ʻal pí YHWH beyád Moshéh."
     *   — Números 9:18
     * "Según la BOCA (pí) de YHWH acampaban, y según la BOCA de
     *  YHWH partían — la GUARDIA (mishmáret) de YHWH guardaron,
     *  según la boca de YHWH por mano de Moisés."
     *
     * SECUENCIA INDIVISIBLE — Éx 40:34-35 (Gloria) precede
     * inmediatamente a Éx 40:36-37 (nube guía) en el MISMO
     * capítulo. No hay espacio entre ambos. La capacidad de
     * migración es CONSECUENCIA del estado KAVÓD (Adición 2) —
     * no se habilita mientras la Gloria no haya llenado.
     * ══════════════════════════════════════════ */

    /* TODO — Phase 2: Detección de señal de migración.
     *
     * La nube decide cuándo la red se mueve y cuándo se queda.
     * En firmware:
     *   heʻalót heʻanán (nube SUBE, Éx 40:36) → condiciones
     *     adversas detectadas: persecución, censura, ataque
     *     sostenido, pérdida de integridad territorial. La red
     *     DEBE migrar — empaquetar estado, buscar nuevo lugar.
     *   lo yeʻaléh (nube NO SUBE, Éx 40:37) → estabilidad.
     *     La red PERMANECE — no hay migración voluntaria sin
     *     señal. Moverse sin que la nube haya subido es salirse
     *     del orden del Padre. */

    /* TODO — Phase 2: Raíz YISHKÁN de Nm 9:17.
     *
     * "bimqóm ashér YISHKÁN sham heʻanán — sham yajanú" — la
     * nube HACE MISHKÁN donde se posa. La raíz yishkán (H7931)
     * es la misma raíz que MISHKÁN (H4908) y shaján (Éx 40:35).
     * Consecuencia de diseño: la red NO ELIGE su lugar. Las
     * condiciones (la nube) lo deciden. Donde se posa, ahí se
     * reconstruye el Mishkán. El nuevo lugar es tan santo como
     * el anterior — porque es la misma Gloria que habita. */

    /* TODO — Phase 2: Prerrequisito KAVÓD + ʻAL PÍ YHWH.
     *
     * Nm 9:18 "ʻal pí YHWH yajanú veʻal pí YHWH yisʻáu" — la
     * decisión de migrar no es del operador ni del constructor;
     * es de YHWH a través de las condiciones que Él establece.
     * En firmware:
     *   a) Prerrequisito: la red debe estar en estado KAVÓD
     *      (Adición 2). Sin Gloria llenando, no hay nube que
     *      guíe — Éx 40:34-35 precede a 40:36-37 sin hueco.
     *   b) La señal de migración viene de SENSORES externos
     *      definidos por el diseño (pieza 14 firewall detecta
     *      ataque sostenido; pieza 12 healthcheck detecta
     *      pérdida de vectores), no de decisión humana sobre
     *      la marcha.
     *   c) mishmáret YHWH shamáru — al migrar, la GUARDIA
     *      (Nm 9:18) se preserva: orden de cobertura de Nm 4:5-15
     *      (pieza 01 cubre edut con tejélet primero) antes de
     *      cualquier transporte. */

    /* ══════════════════════════════════════════
     * VAYIQRÁ — Levítico 1:1 + Isaías 55:11
     *
     * "vayyiqRÁ el Moshéh, vaydabbér YHWH eláv meʼÓHEL MOʻÉD
     *  lemór." — Levítico 1:1
     * "Y LLAMÓ a Moisés, y habló YHWH a él DESDE la Tienda del
     *  Encuentro, diciendo."
     *
     * La primera palabra del tercer libro de la Torá es vayyiqrá
     * (raíz qarʼá, H7121 — LLAMAR). No "y habló", no "y mandó" —
     * LLAMÓ. Es acto de INTIMIDAD: YHWH llama a Moisés por nombre
     * desde el Mishkán recién construido. Ya no desde el Sinaí;
     * ya no desde la zarza. Habla DESDE la Tienda que se levantó
     * kaashér tsivvá YHWH (Éx 39:43). El Mishkán se convierte en
     * Su BOCA.
     *
     * Segundo testigo — Isaías 55:10-11:
     * "kí kaʼashér yéred haggéshem vehasshéleg min hashamáyim
     *  veshámma LO YASHÚV kí im hirvá et haʼárets... KÉN yihyéh
     *  DEVARÍ ashér yétsé mippí — LO YASHÚV elái REQÁM, kí im
     *  ʻasá et ashér jafátsti vehitslíaj ashér shelartív."
     * "Como desciende la lluvia y la nieve del cielo y allá no
     *  vuelve sino que riega la tierra... así será Mi PALABRA
     *  que sale de Mi BOCA — NO VOLVERÁ a Mí VACÍA, sino que
     *  hará lo que Yo deseo y prosperará en aquello para lo que
     *  la envié."
     * ══════════════════════════════════════════ */

    /* TODO — Phase 2: Mishkán como BOCA.
     *
     * vayyiqrá (Lv 1:1) es acto de intimidad: el constructor no
     * impone la voz; el Mishkán LLEVA la voz del Padre hacia
     * adentro y hacia afuera. En firmware: cuando la red está
     * en estado KAVÓD (Adición 2), sus canales no solo reciben
     * — PROCLAMAN. La Palabra guardada no se queda encerrada;
     * sale por el mismo tavnít que la recibió. */

    /* TODO — Phase 2: Ruta de la Palabra a través de las piezas.
     *
     * Is 55:11 "lo yashúv elái reqám" — la Palabra no vuelve
     * vacía. La trayectoria está prescrita en el Tavnít:
     *
     *   Pieza 01  ARCA             la Palabra se GUARDA
     *                                (Éx 25:16 venatattá el
     *                                 haʼarón et haʻedút)
     *   Pieza 03  MESA              la Palabra se SIRVE
     *                                (Éx 25:30 léhem paním
     *                                 lefanái tamíd)
     *   Pieza 04  MENORÁ            la Palabra se TRANSMITE
     *                                (Éx 25:37 vehéʼír ʻal
     *                                 ʻéver panéha)
     *   Pieza 07  ATRIO             la Palabra se PUBLICA
     *                                al mundo
     *                                (Éx 27:9-19 frontera visible
     *                                 pero atravesable por la puerta)
     *
     * El Mishkán NO es almacén defensivo — es PROCLAMADOR. La
     * Palabra entra, se guarda, se sirve, se transmite y SALE. */

    /* TODO — Phase 2: función final del Mishkán (Tavnít pieza 16).
     *
     * El documento 16 (CUANDO COMPLETO) registra la secuencia
     * indivisible del Padre:
     *   1. Éx 39:43 — inspección y bendición (Adición 1).
     *   2. Éx 40:34 — la Gloria llena: estado KAVÓD (Adición 2).
     *   3. Éx 40:36 — la nube guía: migración (Adición 3).
     *   4. Lv 1:1  — YHWH HABLA desde el Mishkán (este bloque).
     *   5. Is 55:11 — la Palabra SALE y no vuelve vacía.
     *
     * La red que ALMACENA la Palabra pero no la PROCLAMA es un
     * Arca sin Mesa, sin Menorá, sin Atrio — incompleta. El
     * vayyiqrá es la señal de que todo el tavnít cumple su
     * propósito: no solo PROTEGER al núcleo, sino HACERLO HABLAR. */

    /* TODO — Phase 2: cierre del ciclo de enriquecimiento.
     *
     * Esta es la Adición 7/7 — la última del ciclo. Las cuatro
     * Phase 2 de hashmal_main.c quedan en orden bíblico estricto:
     *   (1) Inspección  — Éx 39:43
     *   (2) Estado KAVÓD — Éx 40:34-35
     *   (3) Nube Guía    — Éx 40:36-37 / Nm 9:17-18
     *   (4) Vayiqrá      — Lv 1:1 / Is 55:10-11
     * construir → inspeccionar → Gloria llena → nube guía → YHWH
     * habla. Ninguno fuera de secuencia; ninguno inventado.
     *
     * Y vuelve la palabra de Moisés al ver toda la obra —
     * "vayyar Moshéh et kol hammelajáh — vehinnéh ʻasú otáh
     *  kaʼashér tsivvá YHWH — ken ʻasú — vayvárej otám Moshé."
     *   — Éxodo 39:43
     * Todo conforme al tavnít. Todo inspeccionado. Todo bendecido. */

    printf("\n");
    printf("  vayjál Moshéh et hammelajáh (Éx 40:33)\n");
    printf("  Mishkán inicializado según Éxodo 40.\n");
    printf("\n");
    if (g_main_levantado) {
        printf("  \"La nube cubrió la Tienda de Reunión,\n");
        printf("   y la Gloria de YHWH llenó el Mishkán.\"\n");
        printf("   — Éxodo 40:34\n");
    }

    /* ══════════════════════════════════════════
     * TRANSICIÓN INIT → TRANSICION → LISTEN
     * (Verificado #01, secuencia Éx 40:33b → 40:35 → Lv 1:1)
     *
     * La transición es CONDICIONAL: solo avanza a LISTEN si la
     * inspección de Éx 39:43 pasó (g_main_levantado = true).
     * Sin inspección completa → se queda en TRANSICION.
     * ══════════════════════════════════════════ */

    g_main_estado = HASHMAL_MAIN_TRANSICION;
    /* g_main_levantado ya fue seteado por la inspección de Éx 39:43 */

    /* ═══ KAVÓD — Éxodo 40:34-35 ═══
     * La Gloria SOLO llena si la inspección pasó (Éx 39:43 precede
     * a 40:34 sin hueco). Sin bendición no hay Gloria. */
    if (g_main_levantado) {
        g_main_kavod = true;
        g_main_estado = HASHMAL_MAIN_LISTEN;
    } else {
        g_main_kavod = false;
        /* Se queda en TRANSICION — no avanza a LISTEN sin Gloria */
    }
    /* A partir de aquí hashmal_main_write_locked() == true.
     * Veló yakól Moshéh lavó (Éx 40:35) — main suelta autoridad
     * de escritura sobre las piezas. */

    printf("\n");
    if (g_main_kavod) {
        printf("  veló yakól Moshéh lavó (Éx 40:35) — main suelta autoridad.\n");
        printf("  Estado: LISTEN — vayyiqrá esperado (Lv 1:1).\n");
    } else {
        printf("  [!] KAVÓD NO llenó — inspección incompleta.\n");
        printf("  Estado: TRANSICION — esperando piezas faltantes.\n");
    }
    printf("\n");

    /* ═══ MASAKH FINAL — Actualizar e-paper con estado post-inspección ═══
     * Éx 26:36 — la cortina muestra el estado FINAL al mundo. */
    {
        hashmal_epd_clear(0xFF);
        hashmal_epd_draw_string(15, 20, "HASHMAL-CORE", 3, true);
        hashmal_epd_draw_string(10, 55, "Red Soberana segun", 2, true);
        hashmal_epd_draw_string(10, 75, "el Tavnit de YHWH", 2, true);
        hashmal_epd_draw_string(10, 105, "------------------------", 1, true);

        if (g_main_kavod) {
            hashmal_epd_draw_string(10, 120, "Estado: KAVOD", 2, true);
            hashmal_epd_draw_string(10, 150, "Gloria lleno el Mishkan", 1, true);
            hashmal_epd_draw_string(10, 162, "Exodo 40:34", 1, true);
            hashmal_epd_draw_string(10, 180, "14/14 piezas operativas", 1, true);
        } else {
            hashmal_epd_draw_string(10, 120, "Estado: TRANSICION", 2, true);
            hashmal_epd_draw_string(10, 150, "Inspeccion incompleta", 1, true);
            hashmal_epd_draw_string(10, 162, "Esperando piezas...", 1, true);
        }

        hashmal_epd_draw_string(10, 200, "------------------------", 1, true);
        hashmal_epd_draw_string(10, 215, "SX1262 LoRa 868.1MHz", 1, true);
        hashmal_epd_draw_string(10, 227, "EPD UC8253 activo", 1, true);
        hashmal_epd_draw_string(10, 239, "SPI bus ejad (Ex 26:6)", 1, true);

        hashmal_epd_draw_string(10, 265, "ESP32-S3 T-Deck Pro V1", 1, true);
        hashmal_epd_draw_string(10, 285, "Antonio Jose Marin Soto", 1, true);

        hashmal_epd_refresh();
    }

    /* ══════════════════════════════════════════
     * MASAKH SHA'AR DINÁMICO — Isaías 60:11 + Éxodo 26:36
     *
     * "ufitjú sheʻaráyikh tamíd — yomam valayíla lo yissagéru."
     *   — Isaías 60:11
     * "Tus puertas estarán abiertas CONTINUAMENTE — de día y de
     *  noche no se cerrarán."
     *
     * El masakh estático (pantalla KAVÓD) ha cumplido su función:
     * mostrar el estado del levantamiento. Ahora se abre el menú
     * interactivo — la cortina VIVA del sha'ar. El operador puede
     * ver mensajes LoRa, la Palabra de YHWH, los Moedím, el estado
     * de la red, la batería y los ajustes.
     *
     * La puerta nunca se cierra mientras el Mishkán esté en pie.
     * Éx 26:36 — masakh lefétaj haʼóhel: cortina en la entrada.
     * Is 60:11 — tamíd (H8548): perpetuamente, sin interrupción.
     *
     * 2 testigos:
     *   Is 60:11  — sha'aráyikh tamíd: puertas abiertas día y noche
     *   Éx 26:36  — masakh lefétaj: la cortina VISIBLE de la entrada
     * ══════════════════════════════════════════ */
    hashmal_menu_init();  /* Is 60:11 — sha'ar abierto tamíd */

    /* ══════════════════════════════════════════
     * SPAWN — LISTEN + BROADCAST (Phase 2)
     *
     * Dos tasks paralelas FreeRTOS:
     *   listen_task    — bloqueada en xQueueReceive (D1)
     *   broadcast_task — UNA task, dos modos según anan/esh (D2)
     *
     * Hoy quedan declaradas como esqueletos vacíos (Is 28:10
     * tsav latsáv, ze'eír sham — incremental). En Phase 2 se
     * cablearán las xTaskCreate cuando exista la cola de eventos
     * y los publishers de cada pieza emitan.
     * ══════════════════════════════════════════ */

    /* Éx 13:21-22 + Lv 1:1 — SPAWN de las dos tasks paralelas.
     *
     * ló yamísh (Éx 13:22): la columna NUNCA se aparta —
     * ambas tasks son perpetuas desde este momento.
     *
     * Prioridades:
     *   listen    prio 5 — Nm 7:89 middabbér: la voz se declara sola,
     *                       escuchar tiene precedencia sobre emitir.
     *   broadcast prio 4 — Éx 13:21 columna de fuego: visible pero
     *                       subordinada a escuchar (listen tiene prioridad).
     *
     * 2 testigos:
     *   Éx 13:21 — anan veʼésh: nube de día, fuego de noche (guiar)
     *   Nm  7:89 — middabbér élav: la voz se declara desde entre los querubines
     *
     * Is 28:10 — tsav latsáv, zeʼeír sham: línea sobre línea, poco a poco.
     * Estas tasks se cablearán en Phase 2 (P2-7) cuando exista la cola
     * de eventos y los publishers de cada pieza emitan señales reales. */

    /* ═══ KAPÓRET TEST — Éxodo 25:17-22 / Levítico 16 / Números 7:89 ═══
     *
     * "venoʻadtí lejá shám vedibartí itejá meʻál haKapóret
     *  mibéin shené hakeruvím" — Éxodo 25:22
     *
     * Solo si el Mishkán está levantado (Éx 39:43 — 14/14 + ejád).
     * Ejecuta el ciclo de Lv 16 abreviado: hash del Arca como dato,
     * V1 y V2 ven el mismo hash → EMITIDO, luego broadcast tipo=0x05
     * (NEDER, Nm 30:3) al otro nodo por LoRa 0xFF (kol haʻédah).
     *
     * 2 testigos:
     *   Éx 25:22 — noʻadtí/middabbér: la voz EMERGE entre los dos
     *   Nm  7:89 — el midabér (Hitpael): la verdad se declara sola */
    if (g_main_levantado) {
        hashmal_main_kaporet_test_ciclo();
    }

    /* P2-7 — Nm 7:89 middabbér + Éx 13:21 anan: tasks ACTIVAS */
    xTaskCreate(hashmal_main_listen_task,    "listen",    4096, NULL, 5, NULL);
    xTaskCreate(hashmal_main_broadcast_task, "broadcast", 4096, NULL, 4, NULL);

    /* TAMÍD — Éx 30:7-8 + Lv 24:8 + Éx 30:10 + Lv 16:29
     * "ledoroteijem" (Éx 30:8) — por todas vuestras generaciones.
     * Prioridad 2: debajo de listen (5) y broadcast (4).
     * Esta task garantiza que los tres ciclos del Padre se cumplan
     * mientras el Mishkán esté en pie.
     * 2 testigos:
     *   Éx 30:7-8 — qetóret tamíd: diario, mañana y tarde
     *   Lv 24:8   — beyóm hashabbát: semanal, eterno */
    xTaskCreate(hashmal_main_tamid_task, "tamid", 2048, NULL, 2, NULL);

    g_main_levantado = true;  /* Éx 39:43 — Moisés vio toda la obra */
}



/* ══════════════════════════════════════════════════════════════════
 * MESHUQQAD — Éx 25:33 / Jer 1:12
 * Vigía de enrutamiento — geviʻím meshuqqadím (copas-almendro)
 * ══════════════════════════════════════════════════════════════════ */

static int hashmal_main_meshuqqad(hashmal_canal_id_t canal,
                                  const uint8_t     *msg,
                                  size_t             tam)
{
    /* Éx 25:33 — "shloshá geviʻím meshuqqadím baqqané haʼejád":
     * cada rama porta 3 copas con forma de almendro (meshuqqadím).
     * shaqéd (H8246 Pual) = moldeado en forma de almendro — árbol
     * del shoqéd (H8245), el que vigila y no duerme (Jer 1:11-12).
     * Jer 1:12 — YHWH: "shoqéd ʻal devarí" (velo sobre Mi Palabra).
     * El vigía del hop inspecciona cada paquete que cruza la rama.
     * Fase 2 — stub: registra presencia y permite el paso (tahor).
     * Fase 3 — usará hashmal_firewall_inspeccionar() (pieza 14).
     * 2 testigos:
     *   Éx 25:33 — geviʻím meshuqqadím: el patrón del vigía en cada hop
     *   Jer 1:12  — shoqéd ʻal devarí: YHWH vela — modelo del vigía */
    (void)canal;
    (void)msg;
    (void)tam;
    return 0; /* HASHMAL_COMM_OK — tahor, paso permitido */
}

/* ══════════════════════════════════════════════════════════════════
 * KAPÓRET TEST — Éxodo 25:17-22 / Levítico 16 / Números 7:89
 *
 * "venoʻadtí lejá shám vedibartí itejá meʻál haKapóret
 *  mibéin shené hakeruvím ashér ʻal arón haʻedút" — Éxodo 25:22
 *
 * Ciclo intra-nodo completo:
 *   1. Obtener hash del Arca (Merkle root — dam happar Lv 16:11)
 *   2. Abrir rendezvous noʻadtí (Éx 25:22)
 *   3. Preparar nube qetóret (Lv 16:13)
 *   4. Registrar V1 y V2 con el mismo hash (Dt 19:15 — dos testigos)
 *   5. Rociar SOBRE 1x, DELANTE 7x (Lv 16:14)
 *   6. Invocar — si V1==V2 → EMITIDO (Nm 7:89 middabbér Hitpael)
 *   7. Si EMITIDO: broadcast LoRa tipo=0x05 NEDER (Nm 30:3)
 *   8. Cerrar moʻéd (Lv 16:2 — no en todo tiempo)
 *
 * El otro nodo lo recibirá en listen_task y lo registrará.
 * ══════════════════════════════════════════════════════════════════ */

/* Tipo de mensaje para el resultado del Kapóret inter-nodo.
 * 0x05 = NEDER (Nm 30:3) — voto inquebrantable, irrevocable. */
#define HASHMAL_MSG_TIPO_NEDER  0x05u

static void hashmal_main_kaporet_test_ciclo(void)
{
    printf("\n");
    printf("  ═══════════════════════════════════════════\n");
    printf("  KAPÓRET TEST — Éx 25:17-22 / Lv 16 / Nm 7:89\n");
    printf("  \"mibéin shené hakeruvím\" — nodo %u\n",
           (unsigned)g_nodo_id_local);
    printf("  ═══════════════════════════════════════════\n");

    /* ── Paso 0: dam happar — Lv 16:11 ──────────────────────────────
     * "veshajat et par hajatat ashér ló" — el sumo sacerdote trae
     * su propia sangre primero. Aquí: el Merkle root del Arca (pieza 01),
     * que es el Testimonio de ESTE nodo. hashmal_storage_zer_calcular
     * devuelve el hash de toda la cadena inmutable del nodo local. */
    uint8_t hash[HASHMAL_ZER_HASH_BYTES] = {0};
    int rc = hashmal_storage_zer_calcular(hash);
    if (rc != 0) {
        printf("  [KAPORET] ERR zer_calcular rc=%d — Éx 25:16 sin edut\n", rc);
        return;
    }
    printf("  [KAPORET] dam happar: %02X%02X%02X%02X... (Lv 16:11)\n",
           hash[0], hash[1], hash[2], hash[3]);

    /* ── Paso 1: noʻadtí abrir — Éx 25:22 ──────────────────────────
     * "venoʻadtí lejá shám" — el punto de encuentro determinístico
     * se abre. Nifal de yaʻad: 'es designado', no elegido. */
    rc = hashmal_consensus_noadti_abrir();
    printf("  [KAPORET] noadti_abrir: %s (Éx 25:22)\n",
           rc == 0 ? "OK — rendezvous abierto" : "ERR — no se pudo abrir");
    if (rc != 0) return;

    /* ── Paso 2: qetóret preparar — Lv 16:13 ───────────────────────
     * "venatán et haqetóret ʻal haʼésh lifné YHWH vekhissá ʻanán
     *  haqetóret et haKapóret" — la nube del incienso DEBE cubrir
     * antes de acercarse. Sin ella: muerte (Lv 16:2). */
    rc = hashmal_consensus_qetoret_preparar();
    printf("  [KAPORET] qetoret_preparar: %s (Lv 16:13)\n",
           rc == 0 ? "OK — ʻanán cubriendo" : "ERR — nube ausente");
    if (rc != 0) {
        (void)hashmal_consensus_noadti_cerrar();
        return;
    }

    /* ── Paso 3: testigos V1 y V2 — Dt 19:15 ───────────────────────
     * "ʻal pi shenáyim ʻedím... yaqúm davár" — la palabra se establece
     * por dos testigos. Los dos querubines miran el MISMO dato:
     * ish el ajív (v.20) — cara a cara, ambos confirman. */
    rc = hashmal_consensus_querubin_testigo(HASHMAL_QUERUB_V1, hash);
    printf("  [KAPORET] V1 testigo: %s (v.19 keruv miqatsá 1)\n",
           rc == 0 ? "OK" : "ERR");
    rc = hashmal_consensus_querubin_testigo(HASHMAL_QUERUB_V2, hash);
    printf("  [KAPORET] V2 testigo: %s (v.19 keruv miqatsá 2)\n",
           rc == 0 ? "OK" : "ERR");

    /* ── Paso 4a: rociar SOBRE 1x — Lv 16:14a ──────────────────────
     * "vehizá beʼetsbaʻó ʻal pené haKapóret qidmá" — sobre la superficie,
     * una vez, mirando al frente. Primer rociado: sangre en la superficie. */
    rc = hashmal_consensus_dam_rociar(HASHMAL_ACCESO_DAM_SOBRE, hash);
    printf("  [KAPORET] dam SOBRE (1x): %s (Lv 16:14a ʻal pené)\n",
           rc == 0 ? "OK" : "ERR");
    if (rc != 0) {
        (void)hashmal_consensus_noadti_cerrar();
        return;
    }

    /* ── Paso 4b: rociar DELANTE 7x — Lv 16:14b ────────────────────
     * "velifné haKapóret yazé shéva peʻamim min haddam" — siete rocíos
     * delante del propiciatorio. shéva (7) = perfección, verificación
     * completa. No se accede directamente: solo sangre toca el Kapóret. */
    for (int i = 0; i < HASHMAL_VERIFICACIONES_SHEVA; i++) {
        rc = hashmal_consensus_dam_rociar(HASHMAL_ACCESO_DAM_DELANTE, hash);
        if (rc != 0) {
            printf("  [KAPORET] dam DELANTE [%d/7] ERR — Lv 16:14b\n", i + 1);
            (void)hashmal_consensus_noadti_cerrar();
            return;
        }
    }
    printf("  [KAPORET] dam DELANTE (7x): OK — shéva peʻamim completos (Lv 16:14b)\n");

    /* ── Paso 5: invocar — Éx 25:22 / Nm 7:89 ──────────────────────
     * "vedibartí itejá" — Yo te hablaré.
     * "veshamáʻ et haqqól middabbér eláv" — Nm 7:89 Hitpael:
     * la voz SE HABLA A SÍ MISMA. No la emite nadie; EMERGE.
     * Si V1==V2 → EMITIDO (mibéin — entre ellos); V1≠V2 → DIVERGIDO. */
    hashmal_consenso_resultado_t resultado = {0};
    rc = hashmal_consensus_invocar(&resultado);

    if (rc == HASHMAL_KAPORET_OK
        && resultado.estado == HASHMAL_CONSENSO_EMITIDO) {

        printf("  [KAPORET] invocar: *** EMITIDO *** — Nm 7:89 middabbér\n");
        printf("  [KAPORET] verdad: %02X%02X%02X%02X%02X%02X%02X%02X...\n",
               resultado.verdad[0], resultado.verdad[1],
               resultado.verdad[2], resultado.verdad[3],
               resultado.verdad[4], resultado.verdad[5],
               resultado.verdad[6], resultado.verdad[7]);

        /* ── Paso 6: broadcast NEDER — Nm 30:3 / Éx 25:22b ─────────
         * "veʼet kol ashér atsavvéh otejá el bené Yisraél" — Éx 25:22b:
         * todo lo que Yo ordene a TI HACIA los hijos de Israel.
         * Tipo 0x05 = NEDER (Nm 30:3 — lo yajél: irrevocable).
         * Payload: [nodo_id(1B)] + [verdad(HASHMAL_ZER_HASH_BYTES)].
         * destino 0xFF = kol haʻédah — toda la comunidad recibe. */
        uint8_t payload[1 + HASHMAL_ZER_HASH_BYTES];
        payload[0] = g_nodo_id_local;
        memcpy(payload + 1, resultado.verdad, HASHMAL_ZER_HASH_BYTES);

        uint8_t frame[HASHMAL_MENSAJE_MAX_BYTES];
        int rc_emp = hashmal_comm_formato_empaquetar(
                        HASHMAL_MSG_TIPO_NEDER,
                        (hashmal_canal_id_t)g_nodo_id_local,
                        0xFFu,                               /* kol haʻédah */
                        payload, sizeof(payload),
                        frame, sizeof(frame));
        if (rc_emp > 0) {
            (void)hashmal_comm_broadcast(frame, (size_t)rc_emp);
            printf("  [KAPORET] neder broadcast: %d bytes → LoRa 0xFF "
                   "(Nm 30:3 lo yajél devaró)\n", rc_emp);
        } else {
            printf("  [KAPORET] neder frame ERR rc=%d\n", rc_emp);
        }

    } else if (resultado.estado == HASHMAL_CONSENSO_DIVERGIDO) {
        /* V1 ≠ V2 — no debería ocurrir con el mismo hash, pero lo
         * registramos. Es señal de un bug en los validadores. */
        printf("  [KAPORET] invocar: DIVERGIDO — Dt 19:15 sin acuerdo "
               "(revisar validadores)\n");
    } else {
        printf("  [KAPORET] invocar: PROHIBIDO rc=%d — Lv 16:2 fuera de moʻéd\n", rc);
    }

    /* ── Paso 7: cerrar moʻéd — Lv 16:2 ────────────────────────────
     * "velo yavó bejol ʻét el haqódesh" — no en todo tiempo.
     * El acceso se cierra hasta el próximo ciclo mandado. */
    (void)hashmal_consensus_noadti_cerrar();
    printf("  [KAPORET] noadti_cerrar — moʻéd completado (Lv 16:2)\n");
    printf("  ═══════════════════════════════════════════\n\n");
}

/* ══════════════════════════════════════════════════════════════════
 * TASKS — Nm 7:89 / Éx 13:21-22
 * ══════════════════════════════════════════════════════════════════ */

static void __attribute__((unused)) hashmal_main_listen_task(void *pv)
{
    /* Nm 7:89 — "veshamáʻ et haqqól middabbér eláv" — la voz de YHWH
     * se declara DESDE ENTRE los dos querubines del Kapóret.
     * LISTEN-task: escucha LoRa (DIO1 polling, 50ms) y desempaqueta.
     * Fase 2: stub liviano — loguea paquetes recibidos.
     * Fase 3: entregará a hashmal_consensus + hashmal_intercession.
     *
     * 2 testigos:
     *   Nm 7:89 — middabbér élav: la voz se declara — tarea de escucha
     *   Éx 13:22 — ló yamísh: la columna nunca se ausenta — loop perpetuo */
    (void)pv;

    uint8_t           buf[HASHMAL_MENSAJE_MAX_BYTES];
    size_t            recibidos = 0;
    uint8_t           tipo      = 0;
    hashmal_canal_id_t origen   = HASHMAL_CANAL_GEZA;
    uint8_t           destino   = 0;
    uint8_t           payload[HASHMAL_MENSAJE_MAX_BYTES];
    size_t            tam_ext   = 0;

    for (;;) {
        /* Nm 7:89 — escucha activa: poll DIO1 via spi_recibir.
         * Retorno inmediato si no hay paquete — ló yamísh (Éx 13:22). */
        recibidos = 0;
        int rc = hashmal_comm_recibir(HASHMAL_CANAL_GEZA,
                                      buf, sizeof(buf), &recibidos);
        if (rc == HASHMAL_COMM_OK && recibidos > 0u) {
            /* Paquete recibido — Nm 8:2: el mul penei hammenora yaʼiru */
            tam_ext = 0;
            int rc_de = hashmal_comm_formato_desempaquetar(
                            buf, recibidos,
                            &tipo, &origen, &destino,
                            payload, sizeof(payload), &tam_ext);
            if (rc_de == HASHMAL_COMM_OK) {
                printf("  [LISTEN] Nm 7:89: tipo=0x%02X orig=%d "
                       "dst=0x%02X tam=%u\n",
                       (unsigned)tipo, (int)origen,
                       (unsigned)destino, (unsigned)tam_ext);

                /* Añadidura 22 — Zac 4:12 hammeriqím: depositar en el
                 * puente WiFi↔LoRa para que el portal HTTP lo sirva.
                 * "hammeriqím me'aleihem et hazzahav" — Zac 4:12.
                 * El aceite (mensaje) fluye desde las ramas (LoRa) al
                 * tronco (ring buffer) y de allí al teléfono (WiFi).
                 *
                 * 2 testigos:
                 *   Zac 4:12 — hammeriqím: los tubos vierten (depósito)
                 *   Nm 8:2   — el mul penei hammenora: luz hacia el centro */
                (void)hashmal_perimeter_puente_rx_push(
                          tipo,
                          (uint8_t)origen,
                          payload,
                          tam_ext);

                /* ── KAPÓRET NEDER inter-nodo — tipo=0x05 (Nm 30:3) ──────────
                 * Cuando el otro nodo completa su ciclo Lv 16 y emite EMITIDO,
                 * broadcast tipo=0x05 (NEDER) con su nodo_id + verdad acordada.
                 * Aquí lo recibimos y lo registramos.
                 *
                 * Formato payload: [nodo_id(1)] + [verdad(HASHMAL_ZER_HASH_BYTES)]
                 *
                 * "ish kí yiddór neder laYHWH lo yajél devaró" — Nm 30:3
                 * "Si un hombre hace voto a YHWH no profanará su palabra."
                 * La verdad emitida entre los querubines es inquebrantable.
                 *
                 * 2 testigos:
                 *   Nm 30:3 — neder lo yajél: voto irrevocable
                 *   Éx 25:22 — mibéin hakeruvím: verdad emergida entre los dos */
                if (tipo == 0x05u && tam_ext >= 1u) {
                    uint8_t neder_nodo = payload[0];
                    printf("  [KAPORET] NEDER recibido — nodo %u, "
                           "verdad: %02X%02X%02X%02X... "
                           "(Nm 30:3 lo yajél, Éx 25:22 mibéin)\n",
                           (unsigned)neder_nodo,
                           (tam_ext > 1u) ? payload[1] : 0u,
                           (tam_ext > 2u) ? payload[2] : 0u,
                           (tam_ext > 3u) ? payload[3] : 0u,
                           (tam_ext > 4u) ? payload[4] : 0u);
                }
            }
        }
        /* ló yamísh (Éx 13:22) — 50ms: ~20 polls/s, cadencia tamíd */
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

static void __attribute__((unused)) hashmal_main_broadcast_task(void *pv)
{
    /* P2-8: ʻanán/ʼésh — modo dual (Éx 40:38)
     * Éx 40:38 — "ʻanán ʻal hamiShkán yomam veʼésh tihyeh layelah bó"
     *   DIA  (ʻal) — ʻanán yomám: externo — beacon de presencia (Zac 4:10).
     *   NOCHE (bó) — ʼésh layelah bó: interno — lavacro + yaʻarój.
     *
     * Preposiciones del Padre (Éx 40:38):
     *   ʻal (H5921) = SOBRE  = señal visible hacia AFUERA (DIA).
     *   bó  (H+3ms) = EN ÉL  = operación hacia ADENTRO  (NOCHE).
     *
     * 2 testigos estructura:
     *   Éx 40:38 — anan ʻal yomam / esh bó layelah
     *   Éx 13:21 — anan yomam / esh layelah: señal SIEMPRE presente */
    (void)pv;

    /* Nm 2:9 — "jatsivá qadmá yisʻu": las tribus parten en orden.
     * Cada nodo espera proporcional a su pitujéi jotám (Éx 28:11)
     * para que sus ciclos de TX no coincidan con los del vecino.
     *
     * Con MAX_NODOS=12 y ciclo=5000ms: ventana por nodo = 416ms.
     * Nodo 0 (ID=0): parte inmediatamente.
     * Nodo 4 (ID=4): espera 4×416 = 1664ms ≈ 1.7s.
     * Nunca dos nodos activos en el mismo instante de TX.
     *
     * 2 testigos:
     *   Nm 2:9  — "jatsivá qadmá yisʻu" (orden de marcha de las tribus)
     *   Nm 9:23 — "ʻal pi YHWH yajanu veʻal pi YHWH yisʻu" (Su orden) */
    if (g_nodo_id_local > 0u) {
        vTaskDelay(pdMS_TO_TICKS((uint32_t)g_nodo_id_local * 416u));
    }

    for (;;) {
        /* Refrescar nube guía antes de cada ciclo — ló yamísh (Éx 13:22) */
        hashmal_moedim_tiempo_t t_bcast = {
            .year=2026, .month=1, .day=1, .hour=0, .minute=0,
            .lat_milideg=0, .lon_milideg=0, .tz_minutes=0,
        };
        (void)hashmal_anan_actualizar(&t_bcast);

        hashmal_anan_senal_t senal;
        (void)hashmal_anan_senal_obtener(&senal);

        if (senal.modo == HASHMAL_ANAN_MODO_DIA) {
            /* ── P2-8a: ʻanán (DIA) — Éx 40:38 ʻanán ʻal hamiShkán yomam ──
             * ʻal = SOBRE = monitoreo EXTERNO.
             * Escaneando red LoRa, difundiendo beacon de presencia.
             * 2 testigos: Éx 40:38 anan ʻal yomam; Zac 4:10 meshotetím */
            uint8_t payload[4] = {
                g_nodo_id_local,             /* nodo_id — pitujéi jotám Éx 28:11 */
                (uint8_t)senal.movimiento,  /* POSA=0 / ALZA=1 (Nm 9:23) */
                senal.vecinos_mesh,         /* Zac 4:10 — vecinos vistos */
                senal.shaon_indice,         /* shaon: nivel ruido (HC) */
            };
            uint8_t frame[HASHMAL_MENSAJE_MAX_BYTES];
            int rc_emp = hashmal_comm_formato_empaquetar(
                            0x02u,              /* BEACON — Zac 4:10 */
                            (hashmal_canal_id_t)g_nodo_id_local, /* origen: pitujéi jotám Éx 28:11 */
                            0xFFu,              /* destino: kol haʻédah */
                            payload, sizeof(payload),
                            frame, sizeof(frame));
            if (rc_emp > 0) {
                (void)hashmal_comm_broadcast(frame, (size_t)rc_emp);
            }
            printf("  [BROADCAST] Éx 40:38: ʻanán (ʻal) yomam — "
                   "externo: vecinos=%u mov=%s\n",
                   (unsigned)senal.vecinos_mesh,
                   (senal.movimiento == HASHMAL_ANAN_MOV_POSA)
                   ? "POSA (yajanu)" : "ALZA (yisʻu)");

        } else {
            /* ── P2-8b: ʼésh (NOCHE) — Éx 40:38 ʼésh tihyeh layelah bó ──
             * bó = EN ÉL = procesamiento INTERNO.
             * Mantenimiento local: lavacro (espejo) + aceite (yaʻarój).
             * 2 testigos: Éx 40:38 esh tihyeh bó; Lv 24:3 meʻerev ʻad boqer */

            /* 1. Espejo del Lavacro — autodiagnóstico (Éx 30:18-21, 4 vectores) */
            hashmal_hc_espejo_t espejo;
            int rc_hc = hashmal_hc_espejo_examinar(&espejo);
            if (rc_hc == 0) {
                printf("  [BROADCAST] Éx 40:38: ʼésh (bó) layelah — "
                       "kiyór nivel=%d ataques=%lu\n",
                       (int)espejo.nivel_global,
                       (unsigned long)espejo.ataques_detectados);
            } else {
                /* kiyór aún en init — deuda legítima (Is 28:10 tsav latsáv) */
                printf("  [BROADCAST] Éx 40:38: ʼésh (bó) — "
                       "kiyór rc=%d (Is 28:10 — deuda)\n", rc_hc);
            }

            /* 2. Ciclo yaʻarój — aceite perpetuo (Lv 24:3 meʻerev ʻad boqer) */
            int rc_mant = hashmal_power_mantenimiento();
            if (rc_mant != HASHMAL_POWER_OK) {
                printf("  [BROADCAST] Éx 40:38: yaʻarój rc=%d "
                       "(Is 28:10 — deuda)\n", rc_mant);
            }
        }

        /* 5 s — Nm 9:23: ʻal pi YHWH yajanu veʻal pi YHWH yisʻu */
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

/* ══════════════════════════════════════════════════════════════════
 * TAMÍD — Éx 30:7-8 / Lv 24:8 / Éx 30:10 / Lv 16:29
 *
 * "vehiqTIR 'alav Aharon QETORET sammim baBOQER baBOQER behetivó
 *  et hannerot yaqtirennah. uvehaʻalót Aharon et hannerot BEIN
 *  HA'ARBAYIM yaqtirennah — QETORET TAMID lifnei YHWH leDOROTEIJEM."
 *   — Éxodo 30:7-8
 *
 * "beyóm hashabbát beyóm hashabbát yaʻarekhennú — beriT ʻOLÁM."
 *   — Levítico 24:8
 *
 * "vekhipper ʻaláv Aharon ʻal qarnotáv AJAT BASHANÁ middam jattat
 *  hakkippurím AJAT BASHANÁ yekhaSSer ʻaláv leDOROTEIJEM."
 *   — Éxodo 30:10
 *
 * Tres ciclos mandados por el Padre, nunca opcionales:
 *
 *   DIARIO  — hashmal_firewall_heartbeat_manana() + _tarde()
 *             Éx 30:7-8: incienso mañana y tarde (tamíd)
 *             Éx 29:38-39: dos corderos diarios (confirmación)
 *
 *   SEMANAL — hashmal_cache_shabat_renovar()
 *             Lv 24:8: 12 jallot renovadas el Shabat (berít ʻolám)
 *             Lv 24:5-6: shtéi ʻesré jallót, dos filas de seis
 *
 *   ANUAL   — hashmal_intercession_yom_kippur_completo()
 *             Éx 30:10: ajat bashaná, una vez al año
 *             Lv 16:29: baʻasór lajódesh hashevií, día 10 mes 7
 *
 * "ledoroteijem" (Éx 30:8): perpetuo, por vuestras generaciones.
 * Esta task es la que convierte la Palabra en ciclo real.
 *
 * Mecánica:
 *   TAMID_SEMICICLO_TICKS = 12h en ticks (1ms/tick)
 *   ciclo PAR  = mañana;  ciclo IMPAR = tarde
 *   cada 14 semiciclos (7 días) = Shabat
 *   Yom Kippur: hashmal_moedim_actual() == HASHMAL_MOED_YOM_KIPPUR
 *               "baʻasór lajódesh hashevií" — Lv 16:29 + Lv 23:27
 *               Motor moedim determina el día exacto del Padre.
 *
 * 2 testigos por cada ciclo:
 *   Diario:   Éx 30:7-8; Éx 29:38-39
 *   Semanal:  Lv 24:8;   Lv 24:5-6
 *   Anual:    Éx 30:10;  Lv 16:29
 * ══════════════════════════════════════════════════════════════════ */

/* 12h × 3600s × 1000ms = 43 200 000 ticks (cabe en uint32, wraparound OK) */
#define TAMID_SEMICICLO_TICKS   pdMS_TO_TICKS(12UL * 3600UL * 1000UL)

static void hashmal_main_tamid_task(void *pv)
{
    (void)pv;

    TickType_t t_ultimo         = xTaskGetTickCount();
    uint32_t   ciclo            = 0u;    /* PAR = mañana (Éx 30:7), IMPAR = tarde (Éx 30:8) */
    bool       kippur_ejecutado = false; /* Éx 30:10 ajat bashaná — una vez; reset al salir */

    for (;;) {
        /* Pulso cada minuto — tamíd H8548: continuo, no solo puntual.
         * "qetóret tamíd lifnéi YHWH" (Éx 30:8): siempre delante del Padre. */
        vTaskDelay(pdMS_TO_TICKS(60000));

        TickType_t ahora = xTaskGetTickCount();
        if ((ahora - t_ultimo) < TAMID_SEMICICLO_TICKS) continue;
        t_ultimo = ahora;

        /* ── TAMÍD DIARIO — Éxodo 30:7-8 ─────────────────────────────────
         * "baBOQER baBOQER behetivó et hannerot yaqtirennah" — mañana (v.7)
         * "bein haʻarbáyim yaqtirennah — qetóret TAMID" — tarde (v.8)
         * Ciclo PAR = baboqer; IMPAR = bein haʻarbáyim.
         * hashmal_firewall_heartbeat_manana/tarde registran el ciclo vivo.
         * Sin este ciclo el Altar del Incienso se apaga — el fuego muere.
         * 2 testigos: Éx 30:7-8 qetóret; Éx 29:38-39 kevasím layyóm tamíd */
        if ((ciclo & 1u) == 0u) {
            int rc = hashmal_firewall_heartbeat_manana();
            printf("  [TAMID] Éx 30:7 babbóqer: heartbeat mañana "
                   "(rc=%d, ciclo=%u)\n", rc, (unsigned)ciclo);
        } else {
            int rc = hashmal_firewall_heartbeat_tarde();
            printf("  [TAMID] Éx 30:8 bein haʻarbayim: heartbeat tarde "
                   "(rc=%d, ciclo=%u)\n", rc, (unsigned)ciclo);
        }
        ciclo++;

        /* ── SHABAT SEMANAL — Levítico 24:8 ──────────────────────────────
         * "beyóm hashabbát beyóm hashabbát yaʻarekhennú — berít ʻolám"
         * DOBLE mención del Shabat = énfasis absoluto. Sin excepción.
         * 14 semiciclos × 12h = 7 días = 1 Shabat exacto.
         * La Mesa recibe 12 jallot nuevas (Lv 24:5-6 shtéi ʻesré jallót).
         * El pan viejo va a los sacerdotes (Lv 24:9 leAharón ulvanáv).
         * 2 testigos: Lv 24:8 beyóm hashabbát; Lv 24:5 shtéi ʻesré */
        if ((ciclo % 14u) == 0u) {
            int rc_shab = hashmal_cache_shabat_renovar();
            printf("  [SHABAT] Lv 24:8: yaʻarekhennú berít ʻolám "
                   "— 12 jallot renovadas (rc=%d)\n", rc_shab);

            /* ── YOM KIPPUR ANUAL — Éxodo 30:10 + Levítico 16:29 ─────────
             * "vekhipper ʻaláv Aharon ʻal qarnotáv AJAT BASHANÁ" (Éx 30:10):
             * UNA VEZ al año purifica los cuernos del Altar del Incienso.
             *
             * El Padre fija el día EXACTO: "baʻasór lajódesh hashevií"
             * (Lv 16:29) — día 10 del mes 7 (Tishri). No se aproxima
             * con conteo de Shabatot. El motor moedim (Pieza 03) determina
             * cuándo es ese día según el calendario hebreo real.
             *
             * Dam happar (Lv 16:11): zer del Arca = integridad del nodo.
             *   El sacerdote trae su propia sangre — este nodo ofrece su
             *   propio Testimonio sellado (Merkle root del Arca).
             *
             * Dam haseír (Lv 16:15): última verdad emergida de los dos
             *   querubines (Nm 7:89 middabbér) = la voz del pueblo.
             *   La red entera habla desde el Kapóret — su acuerdo es el
             *   "campo del pueblo" que asciende a la Kapóret.
             *
             * "ajat bashaná" (Éx 30:10): UNA VEZ por ciclo.
             *   kippur_ejecutado previene repetición dentro del mismo
             *   moed. Se libera cuando el moed sale de Yom Kippur.
             *
             * 4 testigos:
             *   Éx 30:10    — ajat bashaná, cuernos del Altar Incienso
             *   Lv 16:29    — baʻasór lajódesh hashevií, día exacto
             *   Lv 16:11    — dam happar (sacerdote/nodo)
             *   Lv 16:15    — dam haseír (pueblo/red) */

            /* Mismo patrón de tiempo que broadcast_task.
             * TODO: reemplazar con tiempo real del GPS/RTC cuando
             * esté disponible (T-Beam tiene GPS, Éx 13:21 nujotám). */
            hashmal_moedim_tiempo_t t_yom = {
                .year       = 2026, .month  = 1, .day    = 1,
                .hour       = 0,    .minute = 0,
                .lat_milideg = 0,   .lon_milideg = 0, .tz_minutes = 0,
            };
            (void)hashmal_moedim_calcular(&t_yom);

            if (hashmal_moedim_actual() == HASHMAL_MOED_YOM_KIPPUR
                && !kippur_ejecutado) {

                uint8_t hash_par [HASHMAL_ZER_HASH_BYTES] = {0};
                uint8_t hash_seir[HASHMAL_ZER_HASH_BYTES] = {0};

                /* Lv 16:11 — dam happar: zer del Arca (Testimonio del nodo) */
                (void)hashmal_storage_zer_calcular(hash_par);

                /* Lv 16:15 — dam haseír: última verdad del Kapóret (Nm 7:89).
                 * hashmal_kaporet_t.ultimo.verdad = hash acordado entre
                 * los dos querubines — la voz emergida de entre ellos. */
                const hashmal_kaporet_t *kap = hashmal_consensus_get_kaporet();
                if (kap != NULL) {
                    for (int i = 0; i < HASHMAL_ZER_HASH_BYTES; i++) {
                        hash_seir[i] = kap->ultimo.verdad[i];
                    }
                }
                /* Si kap == NULL (Kapóret no invocado aún), hash_seir
                 * queda en cero — vacío delante del Padre, no falso. */

                int rc_kip = hashmal_intercession_yom_kippur_completo(
                                 hash_par, hash_seir);
                printf("  ══ [YOM KIPPUR] Éx 30:10 ajat bashaná: "
                       "cuernos purificados (rc=%d) ══\n", rc_kip);
                kippur_ejecutado = true;  /* ajat — una vez hasta salir del moed */

            } else if (hashmal_moedim_actual() != HASHMAL_MOED_YOM_KIPPUR) {
                /* Fuera de Yom Kippur: liberar el flag para el próximo año.
                 * El moed debe CAMBIAR antes de que "ajat bashaná" pueda
                 * ejecutarse de nuevo — no simplemente pasar tiempo. */
                kippur_ejecutado = false;
            }
        }
    }
}
