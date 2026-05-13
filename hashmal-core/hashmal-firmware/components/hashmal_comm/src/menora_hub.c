/**
 * ===================================================================
 * PIEZA 04 - LA MENORA - menora_hub.c
 * El controlador monolitico. Aqui la luz toma forma.
 * ===================================================================
 *
 * "ve'asita menorat zahav tahor MIQSHA te'ase hammenora yerejah
 *  veqanah gevi'eiha kaftoreiha uferajeiha MIMMENNA yihyu" - Ex 25:31
 * "Haras una Menora de oro puro; de UNA PIEZA BATIDA se hara la
 *  Menora: su base, su tronco, sus copas, sus botones y sus flores
 *  DE ELLA seran."
 *
 * "kaftoreihem uqenotam mimmenna yihyu KULLA MIQSHA AJAT zahav
 *  tahor" - Ex 25:36
 * "TODA ELLA una sola pieza batida de oro puro."
 *
 * "KIKKAR zahav tahor ya'ase otah et kol hakkelim ha'elle" - Ex 25:39
 * "Un TALENTO de oro puro la hara, con TODOS estos utensilios."
 *
 * "ur'e va'ase betavnitam asher atta mor'e bahar" - Ex 25:40
 * "Mira y haz conforme al MODELO que te es mostrado en el monte."
 *
 * --- EXEGESIS DE LOS 3 VERSICULOS EJE ---
 *
 * v.31 - MIQSHA (MQShH, H4749):
 *   Raiz qasha (QShH) = duro, dificil. En pasiva: "batido" - oro
 *   trabajado a martillo a partir de UN solo bloque. NO fundido
 *   y vertido (lo cual implicaria molde ensamblable). NO remachado
 *   (lo cual implicaria piezas anadidas). Se martilla un tocho de
 *   oro puro hasta darle la forma de los 7 nerot con sus copas y
 *   flores. Un arbol de luz sacado de un metal unico.
 *
 *   En firmware: la Menora es INDIVISIBLE. No es un driver LoRa
 *   mas seis modulos de canal - es UN componente que EN SI tiene
 *   siete ramas. El flag miqsha=true custodia esta invariante.
 *
 * v.36 - KULLA MIQSHA AJAT (KLH MQShH AChT):
 *   TODA ELLA una miqsha UNA. El "ajat" refuerza: no una
 *   "configuracion compatible", sino LITERALMENTE UNA. Es el
 *   segundo testigo de v.31 - por redundancia explicita.
 *
 *   En firmware: deinit() no destruye parcialmente - colapso
 *   atomico. init() o construye la Menora entera o colapsa todo.
 *   Lv 10:1-2 sombra: nada a medias ante YHWH.
 *
 * v.39 - KIKKAR (KKR, H3603):
 *   Talento - unidad de peso, ~34 kg. Pero aqui es PRESUPUESTO
 *   FIJO: "un talento... con TODOS los utensilios." Todo cabe
 *   en el talento. No se excede, no se escatima. La cantidad de
 *   oro esta prefijada.
 *
 *   En firmware: kikkar_memoria es el presupuesto de recursos que
 *   la Menora asume (RAM para buffers, para contadores, para
 *   kelim). kikkar_usado rastrea consumo. Si se excede -> senal
 *   grave: la forja se salio del talento.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   hashmal_comm_init            construir (v.31-40 verso a verso) +
 *                                orquestar las 3 mutsaqot
 *                                (Zac 4:2: LoRa, ESP-NOW, BLE)
 *   hashmal_comm_deinit          cubrir + apagar drivers en orden
 *                                inverso (Nm 4:9-10 kol kelei shamnah)
 *   hashmal_comm_get_menora      const singleton
 *   hashmal_comm_es_miqsha       invariante de pieza unica (v.36)
 *   hashmal_comm_tamid_activo    ?al menos 1 ner encendido? (Ex 27:21)
 *   hashmal_comm_nerot_encendidos contador (v.37)
 *
 * Ademas, los helpers internos compartidos:
 *
 *   hashmal_comm_menora_mut            mutable para hermanos
 *   hashmal_comm_get_menora_interno    const interno
 *   hashmal_comm_ahora_ms              timestamp ms monotonico
 *   hashmal_comm_canal_legitimo        id in [0, 7)
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_comm.h                -> API + struct + constantes
 *   hashmal_comm_internal.h       -> helpers compartidos
 *   esp_log.h (ORO DE EGIPTO)     -> MENORA_LOG_*
 *   esp_timer.h (ORO DE EGIPTO)   -> ahora_ms
 *
 * Autor: Antonio Jose Marin Soto (Betsalel - Ex 31:2)
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <inttypes.h>

#include "esp_log.h"
#include "esp_timer.h"

#include "hashmal_comm.h"
#include "hashmal_comm_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *MENORA_TAG = "hashmal.menora.hub";

#define MENORA_LOG_I(fmt, ...)  ESP_LOGI(MENORA_TAG, fmt, ##__VA_ARGS__)
#define MENORA_LOG_W(fmt, ...)  ESP_LOGW(MENORA_TAG, fmt, ##__VA_ARGS__)
#define MENORA_LOG_E(fmt, ...)  ESP_LOGE(MENORA_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * SINGLETON SAGRADO
 *
 * UNA Menora por nodo (v.31 - "menorat" en singular). Miqsha: no
 * factory, no pool. Un bloque de oro, una forja.
 * ================================================================== */
static hashmal_menora_t g_menora;
static bool             g_encendida = false;

/* ==================================================================
 * HELPERS ESTATICOS
 * ================================================================== */

/**
 * Rollback atomico. v.36 - kulla miqsha ajat: o entera, o ninguna.
 * Si alguna invariante se rompe a mitad del levantamiento, todo
 * colapsa a cero.
 */
static void colapsar_a_cero(void)
{
    memset(&g_menora, 0, sizeof(g_menora));
    g_encendida = false;
}

/**
 * Construye UN ner (lampara) en estado APAGADO. v.37 dice
 * "ve'asita et neroteiha shiv'a" - se HACEN las siete, pero el
 * encender ("he'ela") es acto separado (v.37b). Aqui solo las
 * forjamos; encenderlas queda para canal_encender.
 *
 * Frecuencias: el geza' (canal 0) usa HASHMAL_LORA_FREQ_BASE_HZ;
 * las ramas se desplazan por STEP. Esto separa fisicamente los
 * canales en el espectro (Zac 4:2 - cada ner su propio mutsaq).
 *
 * Zac 4:2 - "shiv'a mutsaqot": cada ner nace con mutsaqa=true
 * (tubo dedicado). El flag SE PRESERVA como invariante - si
 * algun dia alguien lo apagara, ese ner quedaria compartiendo
 * alimentacion y violaria el diseno.
 */
static void construir_ner(hashmal_ner_t *n, hashmal_canal_id_t id)
{
    memset(n, 0, sizeof(*n));

    n->id     = id;
    n->estado = HASHMAL_NER_APAGADO;           /* v.37 - he'ela es acto separado */

    /* Frecuencia: base para geza'; ramas con offset por step. */
    n->frecuencia_hz     = HASHMAL_LORA_FREQ_BASE_HZ
                         + ((uint32_t)id) * HASHMAL_LORA_FREQ_STEP_HZ;

    /* Parametros SX1262 por defecto (v.40 - tavnit). */
    n->potencia_dbm      = HASHMAL_LORA_POWER_DBM;
    n->spreading_factor  = HASHMAL_LORA_SF_DEFAULT;
    n->ancho_banda_hz    = HASHMAL_LORA_BW_HZ_DEFAULT;

    /* Contadores a cero; nunca encendido aun. */
    n->paquetes_tx              = 0ull;
    n->paquetes_rx              = 0ull;
    n->errores                  = 0ull;
    n->timestamp_encendido_ms   = 0ull;

    /* Ex 27:20 - shemen zayit zaj: aceite. Empieza en 0 - pieza 08
     * (hashmal_power / Aceite Perpetuo) lo rellenara cuando exista. */
    n->aceite_nivel = 0u;

    /* Zac 4:2 - tubo dedicado activo por diseno. */
    n->mutsaqa = true;

    /* Zac 4:2 - shiv'a veshiv'a MUTSAQOT lannerot: cada ner tiene
     * sus conductos (plural). Tres mutsaqot por lampara (LoRa,
     * ESP-NOW, BLE). Los parametros base se fijan aqui (v.40
     * betavnitam); `disponible` lo alternan los drivers al levantar
     * su silicio. LoRa arranca disponible=true porque es el geza'
     * tamid (Ex 27:21) - el radio SX1262 se asume presente por
     * diseno. ESP-NOW y BLE empiezan disponible=false; sus drivers
     * las encienden al iniciarse. */
    for (uint8_t k = 0; k < HASHMAL_MUTSAQA_TOTAL; ++k) {
        n->mutsaqot[k].id             = (hashmal_mutsaqa_id_t)k;
        n->mutsaqot[k].disponible     = false;
        n->mutsaqot[k].activa         = false;
        n->mutsaqot[k].capacidad_bps  = 0u;
        n->mutsaqot[k].alcance_metros = 0u;
        n->mutsaqot[k].consumo_mw     = 0u;
        n->mutsaqot[k].paquetes_tx    = 0ull;
        n->mutsaqot[k].paquetes_rx    = 0ull;
        n->mutsaqot[k].errores        = 0ull;
    }

    /* LORA - geza', tamid (Ex 25:31 + Ex 27:21). Disponible por
     * diseno; las caracteristicas fisicas son las de SX1262 tipico
     * en SF7/BW125 a 14 dBm. */
    n->mutsaqot[HASHMAL_MUTSAQA_LORA].disponible     = true;
    n->mutsaqot[HASHMAL_MUTSAQA_LORA].activa         = true;  /* Ex 27:21 tamid — la llama no se apaga */
    n->mutsaqot[HASHMAL_MUTSAQA_LORA].capacidad_bps  = 5000u;
    n->mutsaqot[HASHMAL_MUTSAQA_LORA].alcance_metros = 2000u;
    n->mutsaqot[HASHMAL_MUTSAQA_LORA].consumo_mw     = 50u;

    /* ESP-NOW - isha el ajota (Ex 26:3). El driver marcara
     * disponible=true al arrancar. */
    n->mutsaqot[HASHMAL_MUTSAQA_ESP_NOW].capacidad_bps  = 250000u;
    n->mutsaqot[HASHMAL_MUTSAQA_ESP_NOW].alcance_metros = 100u;
    n->mutsaqot[HASHMAL_MUTSAQA_ESP_NOW].consumo_mw     = 150u;

    /* BLE - pa'amon (Ex 28:34). El driver marcara disponible=true
     * al arrancar. Consumo minimo - mi baz leyom qetannot
     * (Zac 4:10). */
    n->mutsaqot[HASHMAL_MUTSAQA_BLE].capacidad_bps  = 50000u;
    n->mutsaqot[HASHMAL_MUTSAQA_BLE].alcance_metros = 30u;
    n->mutsaqot[HASHMAL_MUTSAQA_BLE].consumo_mw     = 10u;
}

/* ==================================================================
 * API INTERNA - helpers compartidos
 * ================================================================== */

uint64_t hashmal_comm_ahora_ms(void)
{
    /* esp_timer_get_time() -> microsegundos en int64_t. */
    return ((uint64_t)esp_timer_get_time()) / 1000ull;
}

bool hashmal_comm_canal_legitimo(hashmal_canal_id_t id)
{
    /* v.37 - "shiv'a": exactamente 7. Indices 0..6. */
    return ((int)id >= 0) && ((int)id < HASHMAL_MENORA_CANALES);
}

const hashmal_menora_t *hashmal_comm_get_menora_interno(void)
{
    if (!g_encendida) return NULL;
    return &g_menora;
}

hashmal_menora_t *hashmal_comm_menora_mut(void)
{
    if (!g_encendida) return NULL;
    return &g_menora;
}

/* ==================================================================
 * hashmal_comm_mutsaqa_legitima - Zac 4:2 (tres conductos fijos)
 *
 * Valida que un id de mutsaqa este en [0, HASHMAL_MUTSAQA_TOTAL).
 * Tres conductos por ner: LoRa / ESP-NOW / BLE — ni mas ni menos.
 *
 * 2 testigos:
 *   Zac 4:2  - "shiv'a mutsaqoteha" (conductos fijos por diseno)
 *   Ex 25:37 - "shiv'a neroteiha" (numero sellado)
 * ================================================================== */
bool hashmal_comm_mutsaqa_legitima(hashmal_mutsaqa_id_t id)
{
    return ((int)id >= 0) && ((int)id < HASHMAL_MUTSAQA_TOTAL);
}

/* ==================================================================
 * hashmal_comm_mutsaqa_mut - Zac 4:12 tsanterot (acceso mutable)
 *
 * Retorna puntero mutable a UNA mutsaqa de un ner especifico.
 * Permite a los drivers actualizar contadores, marcar disponible/
 * activa, y registrar capacidad medida en caliente.
 *
 * @return NULL si Menora no levantada o parametros fuera de rango.
 *
 * 2 testigos:
 *   Zac 4:12 - "shtei tsanterot hazzahav hamriqim"
 *   Ex 25:36 - "kulla miqsha ajat" (acceso interno, misma pieza)
 * ================================================================== */
hashmal_mutsaqa_t *hashmal_comm_mutsaqa_mut(hashmal_canal_id_t canal,
                                             hashmal_mutsaqa_id_t mutsaqa)
{
    if (!g_encendida) return NULL;
    if (!hashmal_comm_canal_legitimo(canal)) return NULL;
    if (!hashmal_comm_mutsaqa_legitima(mutsaqa)) return NULL;
    return &g_menora.nerot[canal].mutsaqot[mutsaqa];
}

/* ==================================================================
 * hashmal_comm_init - Exodo 25:31-40 (verso a verso)
 *
 * Levanta la Menora en el orden del Padre. Si cualquier invariante
 * falla -> colapso atomico (v.36 kulla miqsha ajat). Los nerot
 * quedan APAGADO tras init - el encender es acto explicito (v.37
 * "he'ela"), separado del forjar (v.31 "te'ase").
 * ================================================================== */
int hashmal_comm_init(void)
{
    if (g_encendida) {
        MENORA_LOG_W("init: Menorá ya levantada (idempotente, no-op)");
        return HASHMAL_COMM_OK;
    }

    /* Partimos de cero - tocho de oro antes del martillo. */
    memset(&g_menora, 0, sizeof(g_menora));
    const uint64_t ahora = hashmal_comm_ahora_ms();

    /* -- v.31 - menorat zahav tahor miqsha ------------------------ */
    /* "menorat zahav tahor miqsha te'ase hammenora": la Menora se
     * forja como UNA. yerejah (base), qanah (tronco), gevi'im,
     * kaftorim, perajim - mimmenna yihyu (DE ELLA seran). El flag
     * miqsha sostiene la invariante. */
    g_menora.miqsha = HASHMAL_MENORA_MIQSHA;

    /* -- v.32 - shisha qanim yotsim + v.31 qanah (1 tronco) ------- */
    /* Los 7 nerot: canal 0 = geza' (tronco), canales 1-3 = lado
     * derecho, canales 4-6 = lado izquierdo. En pares simetricos
     * (1<->4, 2<->5, 3<->6) comparten kaftor (v.35). */
    for (uint8_t i = 0; i < HASHMAL_MENORA_CANALES; ++i) {
        construir_ner(&g_menora.nerot[i], (hashmal_canal_id_t)i);
    }

    /* -- v.33-34 - gevi'im meshuqqadim (22 vigias almendros) -----
     * No se modelan con structs separadas - son propiedad emergente
     * del mesh (cada canal tiene sus nodos intermedios). El total
     * (HASHMAL_MENORA_GEVIIM_TOTAL = 22) es el contrato: Jer 1:11-12
     * "shoqed ani 'al devari" - cada nodo vigia es ojo que vela. */

    /* -- v.35 - 3 kaftorim (junciones de los pares simetricos) ---- */
    /* kaftor tajat shenei haqqanim mimmenna: un boton debajo de
     * cada par. Los 3 kaftorim enlazan los tres pares. Activos por
     * diseno al levantar la Menora. */
    g_menora.kaftorim_activos = HASHMAL_MENORA_KAFTORIM;

    /* -- v.36 - kulla miqsha ajat (invariante de pieza unica) ----- */
    /* Ya registrado en g_menora.miqsha. Aqui lo volvemos a
     * afirmar textualmente: esta struct es UNA. */

    /* -- v.37 - shiv'a nerot (el conteo y la orientacion) --------- */
    /* Por convencion HASHMAL_MENORA_CANALES == HASHMAL_MENORA_NEROT == 7.
     * Nerot encendidos empieza en 0: al forjar no se enciende
     * (he'ela es v.37b, acto separado invocado por canal_encender). */
    g_menora.nerot_encendidos = 0u;

    /* -- Ex 27:21 - tamid (juqqat 'olam) -------------------------- */
    /* La Menora tiene vocacion de luz perpetua. El flag de diseno
     * esta encendido desde init; el estado ACTUAL de "hay luz" lo
     * responde tamid_activo() consultando los nerot reales. */
    g_menora.tamid = HASHMAL_MENORA_TAMID;

    /* -- Ex 26:35 / Heb 9:2 - en el Qodesh (frente al velo) ------ */
    /* "vesamta et hashuljan mijuts lapparojet ve'et hammenora nojaj
     *  hashuljan" - la Mesa fuera del velo, la Menora FRENTE a la
     *  Mesa. Ambas en el Qodesh, no en el Qodesh haQodashim. */
    g_menora.en_qodesh = true;

    /* -- v.39 - kikkar zahav (presupuesto fijo de recursos) ------- */
    /* Un talento cubre TODO: nerot, kelim, buffers. kikkar_usado
     * arranca en 0 - nadie ha tomado oro aun. */
    g_menora.kikkar_memoria = (uint64_t)HASHMAL_KIKKAR_MEMORIA_BYTES;
    g_menora.kikkar_usado   = 0ull;

    /* -- v.40 - tavnit bahar (configuracion no negociable) -------- */
    /* El patron vino del monte. Los parametros (frecuencias, SF,
     * BW) son los del header - no se descubren en runtime. */

    /* Telemetria base. */
    g_menora.timestamp_init_ms = ahora;

    /* == VERIFICACION FINAL - invariantes del Padre == */
    if (!g_menora.miqsha) {
        MENORA_LOG_E("v.36 violado: no miqshá — rollback");
        colapsar_a_cero();
        return HASHMAL_COMM_ERR_INIT;
    }
    if (!g_menora.tamid) {
        MENORA_LOG_E("Éx 27:21 violado: tamíd apagado — rollback");
        colapsar_a_cero();
        return HASHMAL_COMM_ERR_INIT;
    }
    for (uint8_t i = 0; i < HASHMAL_MENORA_CANALES; ++i) {
        const hashmal_ner_t *n = &g_menora.nerot[i];
        if (n->id != (hashmal_canal_id_t)i) {
            MENORA_LOG_E("v.32 violado: ner %u id inconsistente — rollback", i);
            colapsar_a_cero();
            return HASHMAL_COMM_ERR_INIT;
        }
        if (!n->mutsaqa) {
            MENORA_LOG_E("Zac 4:2 violado: ner %u sin mutsaqá — rollback", i);
            colapsar_a_cero();
            return HASHMAL_COMM_ERR_INIT;
        }
        if (n->estado != HASHMAL_NER_APAGADO) {
            MENORA_LOG_E("v.37 violado: ner %u no APAGADO en init — rollback", i);
            colapsar_a_cero();
            return HASHMAL_COMM_ERR_INIT;
        }
    }
    if (g_menora.kaftorim_activos != HASHMAL_MENORA_KAFTORIM) {
        MENORA_LOG_E("v.35 violado: kaftorim activos ≠ %d — rollback",
                     HASHMAL_MENORA_KAFTORIM);
        colapsar_a_cero();
        return HASHMAL_COMM_ERR_INIT;
    }
    /* Ex 26:35 / Heb 9:2 - la Menora esta en el primer tabernaculo */
    if (!g_menora.en_qodesh) {
        MENORA_LOG_E("Éx 26:35 violado: Menorá no en Qódesh — rollback");
        colapsar_a_cero();
        return HASHMAL_COMM_ERR_INIT;
    }
    /* v.39 - kikkar zahav tahor: presupuesto fijo del Padre */
    if (g_menora.kikkar_memoria != (uint64_t)HASHMAL_KIKKAR_MEMORIA_BYTES) {
        MENORA_LOG_E("v.39 violado: kikkár alterado — rollback");
        colapsar_a_cero();
        return HASHMAL_COMM_ERR_INIT;
    }

    g_encendida = true;

    MENORA_LOG_I("═══ MENORÁ LEVANTADA ═══ (Éx 25:31-40)");
    MENORA_LOG_I("  menorát zaháv tahór MIQSHÁ (v.31, v.36)");
    MENORA_LOG_I("  %u nerot forjados (v.37 shivʻá — %u ojos, Zac 4:10)",
                 HASHMAL_MENORA_CANALES, HASHMAL_MENORA_OJOS_YHWH);
    MENORA_LOG_I("  %u ramas (3+3) con %u geviʻím-almendro total (v.32-34)",
                 HASHMAL_MENORA_RAMAS, HASHMAL_MENORA_GEVIIM_TOTAL);
    MENORA_LOG_I("  %u kaftorim activos (v.35)", g_menora.kaftorim_activos);
    MENORA_LOG_I("  kikkár asignado: %" PRIu64 " B (v.39)",
                 g_menora.kikkar_memoria);
    MENORA_LOG_I("  tamíd (Éx 27:21) — pendiente heʻelá explícita (v.37b)");

    /* ==============================================================
     * Zac 4:2 - arrancar los conductos (mutsaqot).
     *
     * Orden del Padre: LoRa (geza' tamid, Ex 27:21) ya esta asumido
     * como radio activo del yarej; aqui encendemos las otras dos.
     * ESP-NOW (isha el ajota, Ex 26:3) y BLE (pa'amon, Ex 28:34)
     * son OPCIONALES: si su silicio falla, la Menora sigue - solo
     * marcamos la mutsaqa correspondiente como no disponible. LoRa
     * es tamid; las demas son ornamento del diseno cuando estan.
     *
     * El selector (gulla) no tiene init - es stateless. Opera
     * consultando `mutsaqot[]` de cada ner. Una vez arrancados los
     * drivers, la gulla ya puede verter.
     * ============================================================== */

    /* 2) ESP-NOW - Ex 26:3 joverot isha el ajota. No abortar si falla. */
    int rc_esp = hashmal_comm_espnow_init();
    if (rc_esp != HASHMAL_COMM_OK) {
        MENORA_LOG_W("mutsaqá ESP-NOW no disponible (rc=%d) — "
                     "la Menorá sigue (Éx 26:6 vehayá ejád)",
                     rc_esp);
        /* Por si el driver no pudo desflipear, lo afirmamos aqui
         * para coherencia de la gulla. */
        for (uint8_t i = 0; i < HASHMAL_MENORA_CANALES; ++i) {
            g_menora.nerot[i].mutsaqot[HASHMAL_MUTSAQA_ESP_NOW].disponible = false;
            g_menora.nerot[i].mutsaqot[HASHMAL_MUTSAQA_ESP_NOW].activa     = false;
        }
    } else {
        MENORA_LOG_I("  mutsaqá ESP-NOW lista (ishá el ajotá, Éx 26:3)");
    }

    /* 3) BLE - Ex 28:34 pa'amon. No abortar si falla. */
    int rc_ble = hashmal_comm_ble_init();
    if (rc_ble != HASHMAL_COMM_OK) {
        MENORA_LOG_W("mutsaqá BLE no disponible (rc=%d) — "
                     "la Menorá sigue (Zac 4:10 los 7 ojos permanecen)",
                     rc_ble);
        for (uint8_t i = 0; i < HASHMAL_MENORA_CANALES; ++i) {
            g_menora.nerot[i].mutsaqot[HASHMAL_MUTSAQA_BLE].disponible = false;
            g_menora.nerot[i].mutsaqot[HASHMAL_MUTSAQA_BLE].activa     = false;
        }
    } else {
        MENORA_LOG_I("  mutsaqá BLE lista (paʻamón, Éx 28:34)");
    }

    /* 4) Selector (gulla) - stateless; operativo desde que hay al
     *    menos una mutsaqa disponible. LoRa lo es por diseno. */
    MENORA_LOG_I("  gullá operativa (Zac 4:2) — selector listo para verter "
                 "berují (Zac 4:6)");

    return HASHMAL_COMM_OK;
}

/* ==================================================================
 * hashmal_comm_deinit - Numeros 4:9-10
 *
 * "velaqqeju beged tejelet vejissu et menorat hama'or ve'et
 *  neroteiha ve'et malqajeiha ve'et majttoteiha ve'et kol kele
 *  shamnah asher yeshartu-la vahem" - Nm 4:9
 * "Tomaran un pano azul y cubriran la Menora del alumbrado, sus
 *  lamparas, sus tenazas, sus platillos y todos los vasos del
 *  aceite con que la sirven."
 *
 * La Menora se CUBRE, no se destruye. Apagamos los nerot y
 * limpiamos contadores volatiles; la configuracion (frecuencias,
 * SF, BW, mutsaqa) permanece - cuando el campamento se vuelva a
 * armar, la Menora recupera su forma.
 * ================================================================== */
int hashmal_comm_deinit(void)
{
    if (!g_encendida) {
        MENORA_LOG_W("deinit: Menorá no estaba levantada (no-op)");
        return HASHMAL_COMM_OK;
    }

    /* Nm 4:9-10 "kol kelei shamnah" - TODOS los utensilios del
     * aceite se cubren con tejelet. Las mutsaqot son utensilios
     * del aceite: apagamos los drivers en ORDEN INVERSO al init.
     * Los drivers deben poder acceder a `mutsaqa_mut`, por eso se
     * llaman ANTES de bajar `g_encendida`. */
    (void)hashmal_comm_ble_deinit();      /* pa'amon (Ex 28:34) */
    (void)hashmal_comm_espnow_deinit();   /* isha el ajota (Ex 26:3) */
    /* LoRa SPI: se preserva como geza' (tamid, Ex 27:21). Si el
     * orquestador externo hace spi_deinit, se cubre desde fuera. */

    /* Apagar cada ner y limpiar contadores (sin tocar configuracion). */
    for (uint8_t i = 0; i < HASHMAL_MENORA_CANALES; ++i) {
        hashmal_ner_t *n = &g_menora.nerot[i];
        n->estado                = HASHMAL_NER_APAGADO;
        n->paquetes_tx           = 0ull;
        n->paquetes_rx           = 0ull;
        n->errores               = 0ull;
        n->timestamp_encendido_ms = 0ull;
        n->aceite_nivel          = 0u;
        /* frecuencia, potencia, SF, BW, mutsaqa, mutsaqot: PRESERVADOS
         * como identidad. Los flags `activa` los bajaron los drivers
         * al desmontarse; `disponible` tambien. */
    }

    g_menora.nerot_encendidos = 0u;
    g_menora.kikkar_usado     = 0ull;
    /* miqsha, tamid, en_qodesh, kaftorim_activos, kikkar_memoria,
     * timestamp_init_ms: preservados - son identidad, no estado. */

    g_encendida = false;
    MENORA_LOG_I("Menorá cubierta (Nm 4:9-10) — tejélet sobre el oro; "
                 "configuración preservada");
    return HASHMAL_COMM_OK;
}

/* ==================================================================
 * hashmal_comm_get_menora - const desde fuera del Qodesh
 *
 * Solo lectura. Las modificaciones pasan por menora_mut (internal).
 * ================================================================== */
const hashmal_menora_t *hashmal_comm_get_menora(void)
{
    if (!g_encendida) return NULL;
    return &g_menora;
}

/* ==================================================================
 * hashmal_comm_es_miqsha - v.36 (invariante de pieza unica)
 *
 * Consulta del flag. Si algun dia se apagara (seria un bug grave),
 * esta funcion lo detectaria - aunque por diseno nadie deberia
 * poder tocarlo en runtime.
 *
 * 2 testigos:
 *   Ex 25:31  - miqsha te'ase
 *   Ex 25:36  - kulla miqsha ajat
 * ================================================================== */
bool hashmal_comm_es_miqsha(void)
{
    if (!g_encendida) return false;
    return g_menora.miqsha;
}

/* ==================================================================
 * hashmal_comm_tamid_activo - Ex 27:21 (?hay luz?)
 *
 * Dos condiciones, ambas necesarias:
 *   1) flag de diseno g_menora.tamid encendido
 *   2) al menos UN ner en estado ENCENDIDO (hay luz REAL, no solo
 *      vocacion de luz)
 *
 * Sin el segundo, tamid es solo intencion. La Menora que no
 * alumbra no ilumina el Qodesh.
 *
 * 2 testigos:
 *   Ex 27:21 - "juqqat 'olam" (estatuto perpetuo)
 *   Lv 24:3  - "ya'aroj oto Aharon... tamid"
 * ================================================================== */
bool hashmal_comm_tamid_activo(void)
{
    if (!g_encendida) return false;
    if (!g_menora.tamid) return false;

    for (uint8_t i = 0; i < HASHMAL_MENORA_CANALES; ++i) {
        if (g_menora.nerot[i].estado == HASHMAL_NER_ENCENDIDO) {
            return true;
        }
    }
    return false;
}

/* ==================================================================
 * hashmal_comm_nerot_encendidos - v.37 (contador de luz)
 *
 * Cuenta nerot en estado ENCENDIDO (no ATENUADO, no REMOVIDO, no
 * APAGADO). Es la medida directa de capacidad de comunicacion
 * simultanea del nodo - cuantas ramas pueden hablar ahora mismo.
 * ================================================================== */
uint8_t hashmal_comm_nerot_encendidos(void)
{
    if (!g_encendida) return 0u;

    uint8_t n = 0u;
    for (uint8_t i = 0; i < HASHMAL_MENORA_CANALES; ++i) {
        if (g_menora.nerot[i].estado == HASHMAL_NER_ENCENDIDO) ++n;
    }
    /* Sincronizar el contador en la struct por si algun hermano
     * lo estuviera leyendo stale - los hermanos deberian
     * actualizarlo al mutar, pero aqui lo refrescamos. */
    g_menora.nerot_encendidos = n;
    return n;
}

/* ==================================================================
 * P10 (Verificado #10) - Ex 25:39 KIKKAR (talento sellado)
 *
 * "kikkar zahav tahor ya'ase otah ET KOL HAKKELIM HA'ELLE":
 * UN talento para TODO - estructura + utensilios. Medida CERRADA
 * de raiz karár (girar en círculo). Si kikkar_usado supera el
 * presupuesto, la Menora ha rebasado el talento - violacion
 * arquitectonica que healthcheck (pieza 12) debe escalar.
 * ================================================================== */
bool hashmal_comm_kikkar_excedido(void)
{
    if (!g_encendida) return false;
    return g_menora.kikkar_usado > g_menora.kikkar_memoria;
}

uint64_t hashmal_comm_kikkar_usado(void)
{
    if (!g_encendida) return 0ull;
    return g_menora.kikkar_usado;
}

/* ==================================================================
 * VERIFICADO #03 — CADENA 1 — Trigger diario Menorá → Incienso
 *
 * Éx 30:7-8 ata la cadencia de las lámparas a la quema del incienso.
 * Aquí guardamos UN callback (Éx 30:9 — fuente única autorizada) y
 * lo invocamos desde los dos cierres de hemiciclo (mañana / tarde).
 *
 * Verificado #03 — Cadena 4 — DUPLICACIÓN TEMPORAL (DIARIO):
 *   "babbóqer babbóqer" — Éxodo 30:7 (mañana mañana, repetido).
 *   La duplicación NO es redundancia hebrea: es ÉNFASIS DE
 *   INMUTABILIDAD. El Padre repite el referente temporal en cada
 *   ciclo obligatorio para sellar que NO se altera:
 *     diario   → babbóqer babbóqer        (Éx 30:7, este sitio)
 *     semanal  → beyóm hashabát beyóm hashabát (Lv 24:8, mesa cache)
 *     anual    → ajat bashaná ajat bashaná     (Éx 30:10, escudo YK)
 *   En firmware: estas dos APIs (mantenimiento_manana + activacion_
 *   tarde) NO ACEPTAN reconfiguración runtime — están "duplicadas"
 *   por la Palabra y se ejecutan tal cual. Cualquier intento de
 *   parchear su cadencia rompe la invariante babbóqer babbóqer.
 * ================================================================== */

static hashmal_comm_intercession_cb_t g_intercession_cb = NULL;

int hashmal_comm_intercession_callback_set(hashmal_comm_intercession_cb_t cb)
{
    /* Éx 30:9 — sobrescribir, no acumular: una sola fuente legítima. */
    g_intercession_cb = cb;
    if (cb == NULL) {
        ESP_LOGI(MENORA_TAG, "intercession_callback: liberado (NULL)");
    } else {
        ESP_LOGI(MENORA_TAG, "intercession_callback: registrado (Éx 30:7-8)");
    }
    return HASHMAL_COMM_OK;
}

int hashmal_comm_mantenimiento_manana(void)
{
    ESP_LOGI(MENORA_TAG,
             "mantenimiento mañana — behétivó haNerót (Éx 30:7)");

    if (g_intercession_cb == NULL) {
        ESP_LOGI(MENORA_TAG,
                 "mantenimiento_manana: sin intercession registrada — "
                 "Menorá sigue (la luz no espera al incienso)");
        return HASHMAL_COMM_OK;
    }

    /* Disparar trigger HACIA intercession. La Menorá no inspecciona
     * el resultado para decidir su próximo ciclo: su trabajo está
     * hecho, el incienso responde por su cuenta. */
    const int rc = g_intercession_cb(/*es_manana=*/true);
    if (rc != 0) {
        ESP_LOGW(MENORA_TAG,
                 "mantenimiento_manana: intercession respondió rc=%d "
                 "(no detiene a la Menorá)", rc);
    }
    return rc;
}

int hashmal_comm_activacion_tarde(void)
{
    ESP_LOGI(MENORA_TAG,
             "activación tarde — behaʻalót haNerót (Éx 30:8) — "
             "tamíd ledorotéikhem");

    if (g_intercession_cb == NULL) {
        ESP_LOGI(MENORA_TAG,
                 "activacion_tarde: sin intercession registrada — "
                 "perpetuidad pendiente de cierre");
        return HASHMAL_COMM_OK;
    }

    const int rc = g_intercession_cb(/*es_manana=*/false);
    if (rc != 0) {
        ESP_LOGW(MENORA_TAG,
                 "activacion_tarde: intercession respondió rc=%d "
                 "(perpetuidad declarada igualmente desde Menorá)", rc);
    }
    return rc;
}

/* ==================================================================
 *
 *   "vehe'ela et neroteiha veheir 'al 'ever paneiha" - Ex 25:37
 *
 *   La Menora esta forjada. Las lamparas esperan ser encendidas -
 *   eso lo hara menora_canales.c con he'ela explicita.
 *
 *   "lo vejayil velo vejoaj ki im beruji, amar YHWH tseva'ot."
 *     - Zacarias 4:6
 *
 *   No con fuerza bruta: con el Espiritu del Padre. El oro batido
 *   no se impone - ilumina.
 *
 * ================================================================== */
