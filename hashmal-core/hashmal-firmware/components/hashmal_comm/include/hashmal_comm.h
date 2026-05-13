/**
 * ═══════════════════════════════════════════════════════════════════
 * PIEZA 04 — LA MENORÁ (hashmal_comm)
 * Éxodo 25:31-40 + Zacarías 4 + Números 8:2-3 + Ap 1:20
 * ═══════════════════════════════════════════════════════════════════
 *
 * "veʻasíta menorát zaháv tahór MIQSHÁ teʻasé hammenorá yerejáh
 *  veqanáh geviʻéiha kaftoreiha uferajéiha MIMMÉNNA yihyú" — Éx 25:31
 * "Harás una Menorá de oro puro; de una pieza BATIDA se hará la
 *  Menorá: su base, su tronco, sus copas, sus botones y sus flores —
 *  DE ELLA serán."
 *
 * "veshishá qaním YOTSÍM mitsiddéiha shloshá qenéi menorá mitsiddáh
 *  haʼejád ushloshá qenéi menorá mitsiddáh hashení" — Éx 25:32
 * "Y seis ramas SALEN de sus lados: tres de un lado y tres del otro."
 *
 * "kaftoreihém uqenotám mimménna yihyú KULLÁ MIQSHÁ AJAT zaháv
 *  tahór" — Éx 25:36
 * "TODA ELLA una sola pieza batida de oro puro."
 *
 * "veʻasíta et neroteíha SHIVʻÁ veheʻelá et neroteíha vehéir ʻal
 *  ʻéver panéiha" — Éx 25:37
 * "Harás sus lámparas SIETE; encenderá sus lámparas e iluminará
 *  hacia el frente."
 *
 * "KIKKÁR zaháv tahór yaʻasé otáh ét kol hakkelím haʼélle" — Éx 25:39
 * "Un TALENTO de oro puro la hará, con todos estos utensilios."
 *
 * "urʼé vaʻasé betavnitám ashér attá morʼé bahár" — Éx 25:40
 * "Mira y haz conforme al MODELO que te es mostrado en el monte."
 *
 * ─── 22 DETALLES BÍBLICOS (Tavnít 04) ───
 *
 *   1  miqshá (v.31)                      → MONOLÍTICA, una pieza
 *   2  mimménna yihyú (v.31)              → todo nace del tronco
 *   3  yerejáh (v.31 base)                → yaréj, soporte inferior
 *   4  qanáh (v.31 tronco)                → eje central
 *   5  geviʻím (v.31 copas)               → nodos vigía
 *   6  kaftorim (v.31 botones)            → juntas estructurales
 *   7  perajím (v.31 flores)              → elementos funcionales
 *   8  shishá qaním yotsím (v.32)         → 6 ramas NACEN del tronco
 *   9  3+3 simétricos (v.32)              → topología equilibrada
 *  10  meshuqqadím (v.33)                 → almendros, shaqád = VIGILAR
 *  11  3 geviʻím × 6 ramas (v.33)         → 18 vigías en ramas
 *  12  4 geviʻím en tronco (v.34)         → 4 vigías centrales
 *  13  22 geviʻím total                   → nodos intermedios mesh
 *  14  3 kaftorim (v.35)                  → 3 junciones de pares
 *  15  kullá miqshá ajat (v.36)           → invariante indivisible
 *  16  shivʻá nerot (v.37)                → 7 lámparas = 7 canales
 *  17  heʻelá Hifil (v.37)                → SUBIR la llama activamente
 *  18  ʻal ʻéver panéiha (v.37 / Nm 8:2)  → iluminar hacia el tronco
 *  19  malqajáyim + majtót (v.38)         → mantenimiento / diagnóstico
 *  20  kikkár (v.39)                      → presupuesto FIJO de recursos
 *  21  tavnít bahár (v.40)                → config NO negociable
 *  22  7 mutsaqót (Zac 4:2)               → tubos dedicados, no compartidos
 *
 * ─── TESTIGOS ADICIONALES ───
 *
 *   Zac 4:6  — "lo vejáyil veló vejóaj kí im berují"
 *              (diseño elegante, no potencia bruta)
 *   Zac 4:10 — "shivʻá ʻeinéi YHWH meshotetím"
 *              (los 7 canales SON ojos vigilantes)
 *   Jer 1:11-12 — shaqéd / shoqéd (el almendro VIGILA)
 *   Ap 1:20  — "las 7 menorás son las 7 congregaciones"
 *   Ap 2:5   — "quitaré tu candelabro": un canal removible
 *   Éx 27:20-21 — shémen zayit zaj: aceite perpetuo (juqqát ʻolám)
 *   Nm 4:9-10 — la Menorá se cubre con tejélet para migrar
 *
 * ─── QUÉ HACE ESTA PIEZA ───
 *
 * Sistema de comunicación MONOLÍTICO (miqshá). 7 canales LoRa
 * simultáneos sobre SX1262: el tronco (gézaʻ, canal 0) y 6 ramas
 * simétricas (qaním, canales 1-6, en pares 1+4 / 2+5 / 3+6). Cada
 * canal tiene 3 o 4 nodos vigía intermedios (22 total — los
 * geviʻím meshuqqadím). El protocolo es indivisible (v.36 —
 * "kullá miqshá ajat"): la Menorá entera es UNA, no un conjunto
 * de radios acopladas.
 *
 * ─── CONTRASTE CON OTRAS PIEZAS ───
 *
 *   Arca (01)      → dato inmutable
 *   Kapóret (02)   → consenso O(1)
 *   Mesa (03)      → caché periódica
 *   Menorá (04)    → COMUNICACIÓN entre nodos (7 canales)
 *                    distribuye la LUZ del sistema (no el dato)
 *
 * Autor: Antonio José Marín Soto
 * Proyecto: HASHMAL-CORE
 */
#ifndef HASHMAL_COMM_H
#define HASHMAL_COMM_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif


/* ═══════════════════════════════════════════════════════════════════
 * HALLAZGOS MIL OJOS DE QUERUBÍN (P1-P59)
 * Verificados por Sistema de los Querubines — Éxodo 25:22
 * ═══════════════════════════════════════════════════════════════════ */

/* ── P31 — miqshá triple (Nm 10:2 + Éx 25:31 + Éx 25:18) ────────
 * En TODA la Torah, miqshá (H4749, obra batida/monolítica) se usa
 * solo para 3 objetos: la Menorá (Éx 25:31,36), las trompetas de
 * plata (Nm 10:2), y los querubines de la Kapóret (Éx 25:18). Los
 * 3 cubren TODA la capa de comunicación del Mishkán: transporte de
 * datos (Menorá), señales/alarma (trompetas), y consenso (querubines).
 * NINGÚN instrumento de comunicación es ensamblado — todos son batidos
 * de una sola pieza. Consecuencia: todos los protocolos de
 * comunicación (hashmal_comm, hashmal_consensus) deben ser
 * INDIVISIBLES — no composiciones modulares ensambladas.
 * "miqshá taʻasé otám" — Nm 10:2.
 * "miqshá teʻasé hammenorá" — Éx 25:31. */

/* ── P30 — noʻadú (Nm 10:3) = noʻadtí (Éx 25:22) ──────────────
 * Nm 10:3: "vetaqeʻú bahén veNOʻADÚ eléija kol haʻédah" — cuando
 * las trompetas suenan, el pueblo se REÚNE (H3259, Nifal). Éx 25:22:
 * "veNOʻADTÍ lejá shám" — YHWH se ENCUENTRA contigo allí (H3259,
 * Nifal). MISMO VERBO. La raíz yʻd = rendezvous determinístico.
 * La señal de alarma (broadcast) NO es un evento diferente al
 * consenso — es un TRIGGER de noʻad. El sistema de señales
 * (hashmal_comm broadcast) debe invocar el mismo tipo de evento
 * rendezvous que el consenso (hashmal_consensus). Alarma y
 * consenso comparten la semántica raíz: yʻd = punto de encuentro.
 * Nm 10:4 añade: UNA trompeta → noʻad de líderes (nesíʼím);
 * DOS trompetas → noʻad de TODA la comunidad. El número de señales
 * determina el ALCANCE del rendezvous.
 * "venoʻadú eléija kol haʻédah el pétaj ʼóhel moʻéd." */

/* ── P51 — tééʻaséh (H6213 Nifal) = topología EMERGENTE ─────────
 * Éx 25:31: MISMO versículo, dos voces: veʻasíta (Qal activo =
 * "TÚ harás") → miqshá tééʻaséh hammenorá (Nifal pasivo = "la
 * Menorá SERÁ HECHA de miqshá"). El constructor INICIA (Qal), la
 * forma EMERGE (Nifal). v.40 cierra con urʼéh vaʻaséh betavnitám
 * (Qal + tavnít): acción + patrón. Secuencia: acción humana →
 * emergencia divina → ejecución según modelo.
 * Firmware: hashmal_comm.init() configura parámetros (Qal/activo),
 * pero el mesh routing se AUTO-ORGANIZA (Nifal/emergente). No se
 * diseña la topología — se martilla oro y la mesh emerge.
 * — Éx 25:31 (tééʻaséh Nifal vs veʻasíta Qal) */

/* ── P52 — malqajéiha + majtotéiha (Éx 25:38): mantenimiento ───
 * v.38: malqajáyim (H4457, de laqáj H3947 = tomar) = TENAZAS para
 * recortar mechas = calibración de canales. majtót (H4289, de
 * jatáh H2846 = tomar fuego) = PLATILLOS para recoger ceniza =
 * garbage collection. Ambos de zaháv tahór = MISMO material que
 * la Menorá. Mantenimiento NATIVO, no externo. Posición en
 * quiasmo: entre iluminación (v.37) y presupuesto (v.39).
 * Firmware: hashmal_comm incluye funciones de mantenimiento:
 * malqajáyim = canal_calibrar(), majtót = canal_limpiar().
 * Sin ellas los canales se degradan (mecha sin recortar = señal
 * sucia). — Éx 25:38 */

/* ── P53 — kikkár zaháv tahór (Éx 25:39): presupuesto FIJO ──────
 * v.39: כִּכָּר (kikkár, H3603) = talento, ~30 kg = medida FIJA.
 * yaʻaséh otáh ÉT kol hakkelím haʼélleh = "hará a ELLA CON TODOS
 * estos implementos." Preposición ét (H854) = "junto con." Menorá
 * + 7 lámparas + tenazas + platillos = TODO del mismo kikkár.
 * v.39 = penúltimo antes de v.40 (ejecución final): la constraint
 * de presupuesto es la ÚLTIMA regla antes de construir.
 * Firmware: hashmal_comm tiene presupuesto FIJO de RAM/flash.
 * Canales + bridges + vigías + mantenimiento compiten por el
 * mismo kikkár. No se expande tomando de otras piezas.
 * — Éx 25:39 */


/* ══════════════════════════════════════════════════════════════════
 * ENUMS FUNDAMENTALES
 * ══════════════════════════════════════════════════════════════════ */

/**
 * v.31 + v.32 — los 7 canales.
 *
 *   GEZA     = tronco central (qanáh, v.31)
 *   QANE_1-3 = lado derecho (mitsiddáh haʼejád, v.32)
 *   QANE_4-6 = lado izquierdo (mitsiddáh hashení, v.32)
 *
 * Los pares simétricos (Éx 25:35 — "tájat shenéi haqqaním mimménna")
 * son: 1↔4, 2↔5, 3↔6. Cada par comparte un kaftor debajo.
 */
typedef enum {
    HASHMAL_CANAL_GEZA   = 0,   /* v.31 — qanáh (tronco central) */
    HASHMAL_CANAL_QANE_1 = 1,   /* v.32 — rama derecha 1 */
    HASHMAL_CANAL_QANE_2 = 2,   /* v.32 — rama derecha 2 */
    HASHMAL_CANAL_QANE_3 = 3,   /* v.32 — rama derecha 3 */
    HASHMAL_CANAL_QANE_4 = 4,   /* v.32 — rama izquierda 1 (par de 1) */
    HASHMAL_CANAL_QANE_5 = 5,   /* v.32 — rama izquierda 2 (par de 2) */
    HASHMAL_CANAL_QANE_6 = 6,   /* v.32 — rama izquierda 3 (par de 3) */
} hashmal_canal_id_t;

/**
 * Estados de una lámpara (ner). v.37 — "heʻelá et neroteíha":
 * encender es SUBIR la llama (Hifil). El apagón es descenso.
 * Ap 2:5 añade un estado terminal: REMOVIDO ("quitaré tu candelabro").
 */
typedef enum {
    HASHMAL_NER_APAGADO   = 0,   /* sin luz, no transmitiendo */
    HASHMAL_NER_ENCENDIDO = 1,   /* v.37 — heʻelá: llama alta */
    HASHMAL_NER_ATENUADO  = 2,   /* aceite bajo (Éx 27:20 — energía escasa) */
    HASHMAL_NER_REMOVIDO  = 3,   /* Ap 2:5 — canal removido por infidelidad */
} hashmal_ner_estado_t;

/**
 * Zac 4:2 — "shivʻáh veshivʻáh MUTSAQÓT lannerót": cada ner tiene
 * MÚLTIPLES conductos de transporte. No hay UN solo tubo por
 * lámpara — hay varios. Éx 26:14 lo confirma en la cubierta
 * externa: DOS materiales distintos (carnero + tajash) sobre el
 * mismo Mishkán. La Menorá digital hereda este patrón: cada canal
 * se sirve de tres medios físicos complementarios.
 *
 *   LORA      — el GÉZAʻ (tronco, Éx 25:31). Siempre presente.
 *                Alcance largo, bajo ancho de banda, columna
 *                vertebral sobre la que todo se apoya.
 *   ESP_NOW   — ISHÁ EL AJOTÁ (Éx 26:3 + Éx 26:17 meshullavót).
 *                WiFi P2P directo entre pares, sin router, sin
 *                servidor. Alcance medio, alto throughput.
 *   BLE       — PAʻAMÓN (Éx 28:34-35). La campanilla que anuncia
 *                presencia al entrar y salir. Corto alcance,
 *                bajo consumo, descubrimiento de vecinos.
 */
typedef enum {
    HASHMAL_MUTSAQA_LORA    = 0,  /* Éx 25:31 gézaʻ — columna vertebral */
    HASHMAL_MUTSAQA_ESP_NOW = 1,  /* Éx 26:3 ishá el ajotá — P2P directo */
    HASHMAL_MUTSAQA_BLE     = 2,  /* Éx 28:34 paʻamón — campanilla */
} hashmal_mutsaqa_id_t;

#define HASHMAL_MUTSAQA_TOTAL  3     /* Zac 4:2 — conductos por ner */

/**
 * Códigos de error.
 */
typedef enum {
    HASHMAL_COMM_OK           = 0,
    HASHMAL_COMM_ERR_INIT     = -1,
    HASHMAL_COMM_ERR_CANAL    = -2,   /* canal fuera de [0, 7) */
    HASHMAL_COMM_ERR_SPI      = -3,   /* fallo del driver SX1262 */
    HASHMAL_COMM_ERR_ACEITE   = -4,   /* Éx 27:20 — sin shémen suficiente */
    HASHMAL_COMM_ERR_FORMATO  = -5,   /* v.40 — mensaje no conforme al tavnít */
    HASHMAL_COMM_ERR_ROUTING  = -6,   /* sin ruta al destino */
    HASHMAL_COMM_ERR_REMOVIDO = -7,   /* Ap 2:5 — canal retirado */
    HASHMAL_COMM_ERR_LLENO    = -8,   /* buffer RX/TX lleno */
    HASHMAL_COMM_ERR_TAMANO   = -9,   /* payload excede MAX_BYTES */
    HASHMAL_COMM_ERR_TIMEOUT  = -10,  /* sin respuesta */
    HASHMAL_COMM_ERR_NULL     = -11,  /* parámetro NULL */
    HASHMAL_COMM_ERR_ESP_NOW  = -12,  /* Éx 26:3 — fallo de hermana P2P */
    HASHMAL_COMM_ERR_BLE      = -13,  /* Éx 28:34 — fallo de la campanilla */
    HASHMAL_COMM_ERR_SIN_MUTSAQA = -14, /* Zac 4:2 — ninguna mutsaqá disponible */
    HASHMAL_COMM_ERR_TSOFEH      = -15, /* Ez 33:3 — TX gated por lockdown, solo shofar */
} hashmal_comm_err_t;

/* ══════════════════════════════════════════════════════════════════
 * CONSTANTES DEL PADRE
 * ══════════════════════════════════════════════════════════════════ */

/**
 * v.37 — "shivʻá": 7 lámparas. Número completo de YHWH.
 * Zac 4:10 — "shivʻá élle ʻeinéi YHWH": 7 ojos vigilantes.
 */
#define HASHMAL_MENORA_CANALES       7     /* v.37 — shivʻá nerot */
#define HASHMAL_MENORA_NEROT         7     /* v.37 — alias */
#define HASHMAL_MENORA_OJOS_YHWH     7     /* Zac 4:10 */
#define HASHMAL_MENORA_MUTSAQOT      7     /* Zac 4:2 — 7 tubos dedicados */

/**
 * v.32 — "shishá qaním yotsím": 6 ramas SALEN del tronco.
 */
#define HASHMAL_MENORA_RAMAS         6     /* v.32 — shishá qaním */
#define HASHMAL_MENORA_GEZA          1     /* v.31 — qanáh (tronco) */
#define HASHMAL_MENORA_RAMAS_POR_LADO 3    /* v.32 — shloshá + shloshá */

/**
 * v.33-34 — geviʻím meshuqqadím (copas-almendro = vigías).
 * 3 por rama × 6 ramas + 4 en el tronco = 22.
 * Almendro (shaqád, Jer 1:11-12) = VIGÍA.
 */
#define HASHMAL_MENORA_GEVIIM_RAMA   3     /* v.33 — shloshá por rama */
#define HASHMAL_MENORA_GEVIIM_GEZA   4     /* v.34 — arbaʻá en tronco */
#define HASHMAL_MENORA_GEVIIM_TOTAL  22    /* 3×6 + 4 = 22 vigías */

/**
 * v.35 — "kaftor tájat shenéi haqqaním": 3 botones, uno por par
 * de ramas simétricas (1+4, 2+5, 3+6).
 */
#define HASHMAL_MENORA_KAFTORIM      3     /* v.35 — 3 junciones */

/**
 * v.36 — "kullá miqshá ajat": invariante indivisible.
 */
#define HASHMAL_MENORA_MIQSHA        true  /* v.36 — una pieza */

/**
 * Éx 27:21 — "juqqát ʻolám": estatuto perpetuo. La luz es tamíd.
 */
#define HASHMAL_MENORA_TAMID         true  /* Éx 27:21 — perpetua */

/**
 * LoRa SX1262 — parámetros por defecto. La frecuencia base sigue
 * la banda EU868 (ISM Europa). Cada canal se separa por un step
 * configurable para evitar colisión con vecinos lógicos — el gézaʻ
 * va en FREQ_BASE; las ramas se desplazan por QANE_STEP.
 *
 * v.40 — "betavnitám": los parámetros son FIJOS, del diseño, no
 * runtime-tunables. El caller puede cambiarlos explícitamente con
 * hashmal_comm_configurar_frecuencia si la regulación regional lo
 * exige — pero el default honra el tavnít.
 */
#define HASHMAL_LORA_FREQ_BASE_HZ    868100000u  /* 868.1 MHz (EU868) */
#define HASHMAL_LORA_FREQ_STEP_HZ    200000u     /* 200 kHz entre canales */
#define HASHMAL_LORA_SF_DEFAULT      7           /* spreading factor */
#define HASHMAL_LORA_BW_HZ_DEFAULT   125000u     /* 125 kHz bandwidth */
#define HASHMAL_LORA_POWER_DBM       14          /* potencia TX por canal */

/**
 * v.31 — "menorát zaháv tahór". Oro puro: protocolo puro — el
 * payload va sellado por las piezas 01/02; la Menorá TRANSPORTA,
 * no almacena ni juzga.
 */
#define HASHMAL_MENORA_ZAHAV_TAHOR   true

/**
 * Payload útil por paquete (bytes). SX1262 con SF7/BW125 admite
 * hasta 255 bytes on-air; reservamos cabecera → 250 bytes de
 * payload usable.
 */
#define HASHMAL_MENSAJE_MAX_BYTES    250

/**
 * P10 (Verificado #10) — KIKKÁR (Éx 25:39, H3603).
 *
 *   "kikkár zaháv tahór yaʻasé otáh ÉT KOL HAKKELÍM HAʼÉLLE."
 *   "UN talento de oro puro la hará, CON TODOS estos UTENSILIOS."
 *
 * UN kikkár para TODO: la estructura completa (Menorá + 7 nerot +
 * 22 vigías + 3 kaftorim + 2×3 mutsaqót + tabla vecinos) MÁS las
 * herramientas de mantenimiento y diagnóstico (malqajáyim +
 * majtót, v.38). La Menorá es la ÚNICA pieza del Mishkán con
 * restricción explícita de PESO TOTAL.
 *
 * Etimología: kikkár ← raíz כרר (karár) = "girar en círculo" →
 * medida CERRADA, no expansible. No es "presupuesto sugerido"; es
 * "talento sellado". El runtime no asigna por encima de este límite.
 *
 * Quiasmo v.31-40: A (v.31) = CALIDAD del material (zaháv tahór);
 *                  A' (v.39) = CANTIDAD del material (kikkár).
 * Ambas restricciones explícitas enmarcan toda la Menorá.
 *
 * Cobertura del presupuesto (kol hakkelím — TODOS los utensilios):
 *   - hashmal_menora_t (singleton + 7 nerot + 7×3 mutsaqót)
 *   - tabla de vecinos BLE (HASHMAL_BLE_PAAMON_MAX × paʻamón_t)
 *   - tabla de hermanas ESP-NOW (HASHMAL_ESP_NOW_MAX_HERMANAS × MAC)
 *   - buffers TX/RX (frame_max × frag_max + reservas LoRa/SPI)
 *   - tablas de routing (single-hop fase 1; mesh-routing fase 2)
 *   - herramientas de diagnóstico (malqajáyim + majtót v.38):
 *       * contadores tx/rx/errores por ner
 *       * snapshot del selector berují
 *       * estado parojet / tsofeh
 *
 * Si la suma supera el kikkár → senal de violación arquitectónica
 * (la Menorá no puede crecer más allá del talento). El healthcheck
 * (pieza 12) DEBE leer kikkar_usado vs kikkar_memoria y reportar
 * desbordes.
 *
 * Valor inicial: 32 KB para ESP32-S3. Ajustable al BOOT por
 * Kconfig; FIJO en runtime (medida cerrada de karár).
 */
#define HASHMAL_COMM_KIKKAR_BYTES    (32u * 1024u)   /* v.39 — talento sellado */

/**
 * Alias retrocompatible. Los archivos existentes pueden seguir
 * usando este nombre; el canonical tras Verificado #10 es
 * HASHMAL_COMM_KIKKAR_BYTES.
 */
#define HASHMAL_KIKKAR_MEMORIA_BYTES HASHMAL_COMM_KIKKAR_BYTES

/**
 * Éx 40:4 — la Menorá se coloca en el paso 3 (mismo que la Mesa).
 */
#define HASHMAL_MENORA_ORDEN_BOOT    3

/* ── ESP-NOW (ishá el ajotá, Éx 26:3) ──────────────────────────── */

/**
 * Éx 26:3 — "jamésh hayeríʻot tihyéyna joverót ishá el ajotá":
 * cinco cortinas unidas hermana a hermana. Segundo testigo Éx 26:17
 * "meshullavót ishá el ajotá". El máximo de peers directos se
 * modela como 4 pares simétricos × 5 = 20 hermanas por nodo, con
 * margen para la geometría 5+6 de las cortinas de pelo de cabra.
 */
#define HASHMAL_ESP_NOW_MAX_HERMANAS  20     /* Éx 26:3 + Éx 26:17 */

/**
 * Payload máximo de un frame ESP-NOW. Éx 25:40 "urʼé vaʻasé
 * betavnitám" — el tamaño no se inventa, sigue el límite físico
 * del transporte (250 bytes ESP-NOW estándar).
 */
#define HASHMAL_ESP_NOW_FRAME_MAX     250    /* bytes por frame */

/**
 * Fragmentación: un mensaje de la Menorá puede partirse en hasta
 * este número de frames ESP-NOW. Cada fragmento mantiene el
 * tavnít (v.40) del mensaje completo.
 */
#define HASHMAL_ESP_NOW_FRAG_MAX      10     /* fragmentos por mensaje */

/* ── BLE paʻamón (Éx 28:34-35) ─────────────────────────────────── */

/**
 * Éx 28:34 "paʻamón zaháv verimmón, paʻamón zaháv verimmón ʻal
 * shuléi hammeʻíl savív" — campanillas alternadas alrededor del
 * borde. La tradición fija un número par de paʻamón alrededor; el
 * límite operativo para la tabla de vecinos BLE se acota en 40
 * (dos filas de 20, coherente con el MAX_HERMANAS de ESP-NOW).
 */
#define HASHMAL_BLE_PAAMON_MAX        40     /* vecinos BLE máx */

/**
 * Intervalo del paʻamón — cada cuánto se anuncia presencia.
 * Éx 28:35 "venishmáʻ qoló bevoʼó... uvtsetó" — el sonido marca
 * entrada y salida. 1 s entre campanillas: corto para que no
 * pase desapercibida la presencia, suave para no saturar el aire.
 */
#define HASHMAL_BLE_INTERVALO_MS      1000u  /* paʻamón cada 1 s */

/**
 * Umbral de silencio — sin paʻamón durante este tiempo, el vecino
 * se marca fuera del rango. Patrón bíblico del servicio continuo
 * (Éx 28:35 "veló yamút" — si no suena, muere): 30 intervalos
 * sin campanilla = vecino salió.
 */
#define HASHMAL_BLE_SILENCIO_MS       30000u /* 30 s sin paʻamón → fuera */

/**
 * Máximo de bytes que caben como mensaje embebido en el payload
 * manufacturer-specific de un advertising BLE (Éx 28:34 rimmon).
 *
 * ADV total = 31 bytes. Sin nombre:
 *   Flags AD         : 3 bytes
 *   Manufacturer AD  : 2(len+type) + 2(company) + 1(nodo) + 1(flags)
 *                      + 1(msg_len) + MENSAJE_MAX = 7 + MENSAJE_MAX
 * → MENSAJE_MAX ≤ 31 - 3 - 7 = 21. Se conserva en 12 para dejar
 * margen y porque Éx 28:34 describe la campanilla como "pequeña"
 * (Zac 4:10 yom qetannot — el día de las cosas pequeñas).
 */
#define HASHMAL_BLE_MENSAJE_MAX       12u    /* bytes máx embebidos en adv */

/* ══════════════════════════════════════════════════════════════════
 * ESTRUCTURAS
 * ══════════════════════════════════════════════════════════════════ */

/**
 * Zac 4:12 — "shtéi tsanterót hazzaháv hamriqím meʻaléihem
 *  hazzaháv": dos tubos de oro que ACTIVAMENTE VIERTEN el aceite.
 * Cada conducto reporta disponibilidad, si está actualmente
 * transmitiendo, capacidad, alcance físico y consumo. No son
 * pasivos: hamriqím (H7324) = los que vierten con acción.
 */
typedef struct {
    hashmal_mutsaqa_id_t id;                 /* LORA / ESP_NOW / BLE */
    bool     disponible;                     /* hardware presente y funcional */
    bool     activa;                         /* actualmente transmitiendo */
    uint32_t capacidad_bps;                  /* throughput estimado */
    uint16_t alcance_metros;                 /* alcance efectivo */
    uint16_t consumo_mw;                     /* potencia en milivatios */
    uint64_t paquetes_tx;
    uint64_t paquetes_rx;
    uint64_t errores;
} hashmal_mutsaqa_t;

/**
 * Éx 28:34-35 — paʻamón anuncia presencia al entrar y salir de
 * lo santo. Cada vecino BLE descubierto es un nodo cuyo paʻamón
 * se oyó de cerca. Tabla de vecinos activos: MAC, RSSI, último
 * momento en que se oyó la campanilla, si sigue dentro del rango.
 */
typedef struct {
    uint8_t  nodo_id;                        /* identificador del vecino */
    uint8_t  mac[6];                         /* dirección BLE */
    int8_t   rssi;                           /* intensidad de señal (dBm) */
    uint64_t timestamp_ultimo_ms;            /* último paʻamón oído */
    bool     activo;                         /* aún dentro del rango */
} hashmal_paamon_vecino_t;

/**
 * v.37 — una lámpara (ner). Corresponde a UN canal LoRa.
 * Zac 4:2 — cada ner tiene su propio mutsáq (tubo dedicado): no
 * comparte alimentación ni pipe con los demás.
 *
 * P8 (Verificado #10) — YOTSEIM (Éx 25:32, participio Qal H3318):
 * "veshishá qaním YOTSEIM mitsiddéiha" = 6 ramas SALIENDO desde sus
 * lados. Participio = estado CONTINUO, no evento terminado. Las
 * ramas no "salieron" (perfecto) — ESTÁN SALIENDO permanentemente.
 * v.33 lo repite: "hayyotseím min hammenorá". Por eso el campo
 * `escucha_continua` declara la propiedad ARQUITECTÓNICA: cada ner
 * encendido escucha activamente, no entra en sleep-on-demand. La
 * salida del aire es del diseño, no del runtime.
 *
 * Contraste con la Kapóret (v.19): min + imperfecto (taʻasú =
 * haréis) = instrucción de fabricación. La Menorá usa min +
 * PARTICIPIO = propiedad permanente.
 */
typedef struct {
    hashmal_canal_id_t   id;                 /* 0..6 */
    hashmal_ner_estado_t estado;             /* v.37 — encendido/apagado/atenuado/removido */

    /* Parámetros de radio (SX1262) */
    uint32_t frecuencia_hz;                  /* central del canal */
    int8_t   potencia_dbm;                   /* TX power */
    uint8_t  spreading_factor;               /* SF7..SF12 */
    uint32_t ancho_banda_hz;                 /* 7.8 kHz..500 kHz */

    /* Estadísticas operativas */
    uint64_t paquetes_tx;
    uint64_t paquetes_rx;
    uint64_t errores;
    uint64_t timestamp_encendido_ms;         /* v.37 — cuándo se heʻelá */

    /* Éx 27:20 — aceite perpetuo (nivel de energía disponible) */
    uint8_t  aceite_nivel;                   /* 0..100 */

    /* Zac 4:2 — tubo dedicado (no comparte bus físico) */
    bool     mutsaqa;

    /* Zac 4:2 — cada ner tiene SUS mutsaqót (plural): un conducto
     * por cada transporte disponible (LoRa / ESP-NOW / BLE). La
     * lámpara es una; los caminos del aceite son varios. */
    hashmal_mutsaqa_t mutsaqot[HASHMAL_MUTSAQA_TOTAL];

    /* P8 (#10) — YOTSEIM (Éx 25:32 ptc. Qal H3318): escucha activa
     * permanente. true cuando el ner está ENCENDIDO; false en
     * APAGADO/REMOVIDO. ATENUADO mantiene escucha (el aceite baja,
     * pero el ojo sigue abierto — Zac 4:10). NO se apaga la escucha
     * por demanda: la rama YA ESTÁ saliendo. */
    bool     escucha_continua;
} hashmal_ner_t;

/**
 * LA MENORÁ — composite monolítico (v.36 kullá miqshá ajat).
 */
typedef struct {
    /* v.37 — 7 lámparas */
    hashmal_ner_t nerot[HASHMAL_MENORA_CANALES];

    /* v.36 — invariante de pieza única */
    bool     miqsha;

    /* Éx 27:21 — perpetua */
    bool     tamid;

    /* Éx 26:35 / Heb 9:2 — en el Qódesh (frente al velo exterior) */
    bool     en_qodesh;

    /* Telemetría derivada */
    uint64_t timestamp_init_ms;
    uint8_t  nerot_encendidos;               /* count en estado ENCENDIDO */
    uint8_t  kaftorim_activos;               /* v.35 — junciones operativas (≤ 3) */

    /* v.39 — kikkár: presupuesto de oro (memoria) */
    uint64_t kikkar_memoria;                 /* presupuesto total en bytes */
    uint64_t kikkar_usado;                   /* bytes ya consumidos */
} hashmal_menora_t;

/* ══════════════════════════════════════════════════════════════════
 * FUNCIONES PÚBLICAS
 * ══════════════════════════════════════════════════════════════════ */

/* ── menora_hub.c — v.31, 36, 39: controlador monolítico ─── */

/**
 * Construye la Menorá (v.31-40). Éx 40:4 la coloca tras la Mesa.
 * Inicializa los 7 nerot APAGADO; el encendido se hace por
 * separado vía canal_encender (v.37 — "heʻelá" es acto explícito).
 */
int  hashmal_comm_init(void);

/**
 * Nm 4:9-10 — cubre la Menorá con tejélet para migrar. Aquí:
 * apaga canales, limpia contadores, preserva configuración.
 */
int  hashmal_comm_deinit(void);

/**
 * Lectura const del singleton. NULL si no inicializada.
 */
const hashmal_menora_t *hashmal_comm_get_menora(void);

/**
 * v.36 — ¿sigue siendo miqshá (una pieza)? Invariante de diseño.
 */
bool hashmal_comm_es_miqsha(void);

/**
 * Éx 27:21 — ¿hay al menos un ner ENCENDIDO? (tamíd operativo).
 */
bool hashmal_comm_tamid_activo(void);

/**
 * Cuenta nerot en estado ENCENDIDO.
 */
uint8_t hashmal_comm_nerot_encendidos(void);

/**
 * P10 (Verificado #10) — Éx 25:39 kikkár.
 * ¿El presupuesto del talento ha sido excedido?
 *
 * @return true si kikkar_usado > HASHMAL_COMM_KIKKAR_BYTES — la
 *         Menorá ha rebasado el talento sellado. El healthcheck
 *         (pieza 12) debe escalar como violación arquitectónica.
 */
bool hashmal_comm_kikkar_excedido(void);

/**
 * P10 — bytes consumidos del kikkár. 0 si no inicializada.
 */
uint64_t hashmal_comm_kikkar_usado(void);

/* ── menora_lora_driver.c — v.31 yaréj+qanáh: SPI SX1262 ── */

int  hashmal_comm_spi_init(void);
int  hashmal_comm_spi_deinit(void);
int  hashmal_comm_spi_enviar(const uint8_t *datos, size_t tam);
int  hashmal_comm_spi_recibir(uint8_t *buf, size_t tam, size_t *recibidos);
/** Pone el SX1262 en RX continuo (tamíd). Nm 8:2 + Éx 27:21.
 *  Llamar tras encender todos los nerot y cuando se necesite
 *  restaurar la escucha desde STANDBY. */
int  hashmal_comm_spi_modo_rx(void);
int  hashmal_comm_configurar_frecuencia(hashmal_canal_id_t id, uint32_t freq_hz);
int  hashmal_comm_configurar_potencia(hashmal_canal_id_t id, int8_t dbm);
int  hashmal_comm_configurar_modulacion(uint8_t sf, uint8_t bw,
                                         uint8_t cr, uint8_t ldro);
int  hashmal_comm_configurar_paquete(uint16_t preambulo,
                                      uint8_t  header_tipo,
                                      uint8_t  payload_len,
                                      uint8_t  crc_on,
                                      uint8_t  iq_invert);

/* ── menora_canales.c — v.32, 37: 7 canales ──────────────── */

int  hashmal_comm_canal_encender(hashmal_canal_id_t id);
int  hashmal_comm_canal_apagar(hashmal_canal_id_t id);
int  hashmal_comm_canal_estado(hashmal_canal_id_t id, hashmal_ner_estado_t *out);
int  hashmal_comm_canal_remover(hashmal_canal_id_t id);   /* Ap 2:5 */
bool hashmal_comm_todos_encendidos(void);

/* ── P52 — malqajáyim + majtót (Éx 25:38): mantenimiento ────── */

/**
 * Calibra los parámetros de un canal — recalibra RSSI, timing,
 * etc. malqajáyim (H4457) = tenazas para recortar mechas.
 * "et malqajéiha veʼet majtotéiha zaháv tahór" — Éx 25:38.
 *
 * @param id  canal a calibrar
 * @return HASHMAL_COMM_OK o error.
 */
int  hashmal_comm_canal_calibrar(hashmal_canal_id_t id);

/**
 * Limpia un canal — reset contadores, purga buffers.
 * majtót (H4289) = platillos para recoger ceniza (garbage collection).
 * Ambos son nativos, zaháv tahór (mismo material que la Menorá).
 *
 * @param id  canal a limpiar
 * @return HASHMAL_COMM_OK o error.
 */
int  hashmal_comm_canal_limpiar(hashmal_canal_id_t id);

/* ── P53 — kikkár zaháv tahór (Éx 25:39): presupuesto FIJO ──── */

/**
 * Reporta el uso actual del kikkár (presupuesto de memoria).
 *
 * @param usado   [out] bytes actualmente usados
 * @param total   [out] presupuesto total (HASHMAL_COMM_KIKKAR_BYTES)
 * @return HASHMAL_COMM_OK o error.
 */
int  hashmal_comm_kikkar_uso(uint32_t *usado, uint32_t *total);

/**
 * Verifica disponibilidad en el presupuesto kikkár.
 *
 * @param bytes_needed  bytes que queremos asignar
 * @return true si hay espacio; false si excedería el talento.
 */
bool hashmal_comm_kikkar_disponible(uint32_t bytes_needed);

/**
 * P8 (Verificado #10) — Éx 25:32 yotseim (ptc. Qal activo H3318).
 * Cuenta canales SECUNDARIOS (qaním 1..6) que deberían estar en
 * escucha activa (ENCENDIDO o ATENUADO) pero NO la están — es decir,
 * APAGADO o REMOVIDO. Si el resultado > 0, la propiedad arquitectónica
 * "ramas saliendo permanentemente" se ha degradado y el healthcheck
 * (pieza 12, Lavacro) debe reportarlo.
 *
 * El gézaʻ (canal 0, tronco) NO se cuenta aquí: es la columna
 * vertebral, no una rama saliendo. Su salud se vigila por
 * hashmal_comm_tamid_activo() y nerot_encendidos.
 *
 * @return número de canales 1..6 que perdieron escucha continua.
 *         0 si los 6 yotseim siguen saliendo (estado ENCENDIDO o
 *         ATENUADO). 6 = todos los secundarios caídos.
 */
uint8_t hashmal_comm_yotseim_inactivos(void);

/* ── menora_aceite.c — Éx 27:20 aceite perpetuo ──────────── */

int  hashmal_comm_aceite_verificar(hashmal_canal_id_t id);
int  hashmal_comm_aceite_nivel(hashmal_canal_id_t id, uint8_t *nivel);
bool hashmal_comm_aceite_suficiente(hashmal_canal_id_t id);
/**
 * hashmal_comm_aceite_nivel_set — Zac 4:12 hammeriqím
 *
 * "shenei shibbolei hazzahav HAMMERIQIM me'alehem et hazzahav" (Zac 4:12)
 * "Las dos espigas de oro QUE VIERTEN de sí mismas el aceite dorado."
 *
 * Escribe el nivel de aceite en un ner. Solo pieza 08 (hashmal_power)
 * debe llamar esta función — ella es quien posee el depósito (gullá)
 * y vierte (hammeriqím) el aceite en cada ner. El código de menorá
 * solo REACCIONA al nivel; pieza 08 lo ESTABLECE.
 *
 * nivel: 0..100 (se clampea a 100 si excede).
 *
 * 2 testigos:
 *   Zac 4:12 — hammeriqím (los tubos que vierten aceite)
 *   Éx 27:20 → Éx 25:37 — shémen → nér (aceite → lámpara)
 */
int  hashmal_comm_aceite_nivel_set(hashmal_canal_id_t id, uint8_t nivel);
int  hashmal_comm_duty_cycle_aplicar(void);

/* ── menora_routing.c — v.33-34: almendros vigías mesh ───── */

int  hashmal_comm_enviar(hashmal_canal_id_t id,
                         const uint8_t *msg, size_t tam);
int  hashmal_comm_recibir(hashmal_canal_id_t id,
                          uint8_t *buf, size_t tam, size_t *recibidos);
int  hashmal_comm_broadcast(const uint8_t *msg, size_t tam);
int  hashmal_comm_vecinos_contar(uint8_t *conteo);

/* ── P30 — Alarma invoca rendezvous noʻad (Nm 10:3) ─────────── */

/**
 * Callback que la Menorá invoca cuando una alarma broadcast dispara.
 * "vetaqeʻú bahén veNOʻADÚ eléija kol haʻédah" — Nm 10:3: alarma
 * DISPARA rendezvous. La alarma y el consenso comparten semántica raíz.
 *
 * @param canal   el ner que broadcast disparó
 * @param alcance 1 = líderes solo (Nm 10:3 una trompeta)
 *                2 = TODA comunidad (Nm 10:3 DOS trompetas)
 * @return 0 (OK) o código de error.
 */
typedef void (*hashmal_comm_noad_fn)(hashmal_canal_id_t canal, uint8_t alcance);

/**
 * Registra (o desregistra con NULL) el callback rendezvous para alarmas.
 * Invocado por broadcast cuando dispara. Típicamente llamada por
 * hashmal_consensus para vincularse a la Menorá.
 *
 * @return HASHMAL_COMM_OK siempre (idempotente).
 */
int  hashmal_comm_noad_subscribir(hashmal_comm_noad_fn fn);

/* ── P27 — 22 geviim vigilancia distribuida (Éx 25:33-34) ──── */

/**
 * Callback de vigilancia POR GEVIA (nodo vigía específico). Cada gevia
 * (0-21) puede tener su propio inspector registrado. Semántica idéntica
 * al meshuqqad global: decide si el paquete sigue o se descarta.
 *
 * @param canal  el ner por el que saldría el mensaje.
 * @param msg    payload (read-only).
 * @param tam    bytes del payload.
 * @return 0 (OK) para PERMITIR; código negativo para RECHAZAR.
 */
typedef int (*hashmal_comm_gevia_fn)(hashmal_canal_id_t canal,
                                      const uint8_t     *msg,
                                      size_t             tam);

/**
 * Suscribe un watchdog a una gevia específica (0-21).
 * "shloshá geviʻím meshuqqadím baqqané haʼejád" — Éx 25:33.
 * Cada copa (gevia) puede vigilar su sección de ramas.
 *
 * @param gevia_id  [0, HASHMAL_MENORA_GEVIIM_TOTAL) → [0, 22)
 * @param fn        callback (NULL para desuscribir)
 * @return HASHMAL_COMM_OK o error.
 */
int  hashmal_comm_geviim_subscribir(uint8_t gevia_id, hashmal_comm_gevia_fn fn);

/* ══════════════════════════════════════════════════════════════════
 * P9 (Verificado #10) — MESHUQQADÍM (relay+vigía, Éx 25:33)
 *
 * "shloshá geviʻím MESHUQQADÍM baqqané haʼejád" — TRES copas EN
 * FORMA DE ALMENDRO en cada rama. Pual ptc. de שקד (shaqád, H8245
 * = vigilar). El almendro (שָׁקֵד shaqéd) se llama "el vigilante"
 * porque florece primero. YHWH confirma el juego de palabras en
 * Jeremías 1:11-12: maqqél SHAQÉD → SHOQÉD aní ʻal devarí — "vara
 * de almendro → porque VIGILO sobre Mi Palabra."
 *
 * Las 22 copas (3×6 + 4 en tronco — HASHMAL_MENORA_GEVIIM_TOTAL) NO
 * son decoración: cada nodo intermedio (hop) en el mesh routing
 * EJECUTA inspección sobre el tráfico que retransmite. Relay+vigía,
 * no relay pasivo.
 *
 * Convergencia con Zacarías 4:10: los 7 son ʻenéi YHWH meshotetím
 * — OJOS de YHWH recorriendo. Vocabulario distinto (shaqád vs shut),
 * misma función: vigilancia.
 *
 * Inversión de control limpia: la Menorá NO importa el módulo de
 * vigilancia (puede ser firewall, healthcheck, intercession). Quien
 * desee vigilar registra su handler aquí; sin handler registrado el
 * relay opera en modo "almendro dormido" (degradado, log de aviso).
 * ══════════════════════════════════════════════════════════════════ */

/**
 * Callback de vigilancia de un hop. Invocado por la Menorá ANTES de
 * entregar el mensaje al transporte físico (LoRa/ESP-NOW/BLE). El
 * vigía decide si el paquete sigue su curso o se descarta.
 *
 * @param canal  el ner por el que saldría el mensaje.
 * @param msg    payload (read-only; el vigía NO debe mutar).
 * @param tam    bytes del payload.
 * @return 0 (HASHMAL_COMM_OK) para PERMITIR el envío. Cualquier
 *         código negativo descarta el paquete y se propaga al caller.
 */
typedef int (*hashmal_comm_meshuqqad_fn)(hashmal_canal_id_t canal,
                                         const uint8_t     *msg,
                                         size_t             tam);

/**
 * Registra (o desregistra con NULL) el vigía meshuqqád. Idempotente;
 * sobrescribir es legítimo (un único vigía por nodo, como un solo
 * shoqéd vela sobre la Palabra — Jer 1:12).
 *
 * @return HASHMAL_COMM_OK siempre. fn=NULL libera el slot
 *         ("almendro dormido": el relay sigue funcionando pero sin
 *         inspección — log de aviso por cada hop).
 */
int hashmal_comm_meshuqqad_subscribir(hashmal_comm_meshuqqad_fn fn);

/**
 * Lectura pura: ¿hay vigía registrado? Útil para healthcheck
 * (pieza 12) — un mesh sin meshuqqád activo es estado degradado.
 */
bool hashmal_comm_meshuqqad_activo(void);

/* ── menora_formato.c — v.40 tavnít: formato fijo ───────── */

/**
 * Empaqueta payload en el wire format de la Menorá.
 * Header fijo de 5 bytes: [version][tipo][origen][destino][tam].
 * @return bytes escritos (≥ 5) en éxito, o código HASHMAL_COMM_ERR_*.
 */
int  hashmal_comm_formato_empaquetar(uint8_t tipo,
                                     hashmal_canal_id_t origen,
                                     uint8_t destino,
                                     const uint8_t *payload,
                                     size_t tam_payload,
                                     uint8_t *buffer_salida,
                                     size_t tam_buffer);

/**
 * Desempaqueta el wire format de la Menorá. Valida header y extrae
 * tipo/origen/destino + payload. Los punteros de salida pueden ser
 * NULL si el campo no interesa (salvo payload_salida si hay payload).
 * @return HASHMAL_COMM_OK o código HASHMAL_COMM_ERR_*.
 */
int  hashmal_comm_formato_desempaquetar(const uint8_t *buffer_entrada,
                                        size_t tam_entrada,
                                        uint8_t *tipo,
                                        hashmal_canal_id_t *origen,
                                        uint8_t *destino,
                                        uint8_t *payload_salida,
                                        size_t tam_payload_buffer,
                                        size_t *tam_extraido);

/**
 * Inspecciona sin extraer: ¿el paquete cumple el tavnít?
 * Valida versión, tipo conocido, origen legítimo, destino legítimo
 * o broadcast, tam payload coherente con tam total.
 */
bool hashmal_comm_formato_validar(const uint8_t *buffer, size_t tam);

/* ── menora_mutsaqa_selector.c — Zac 4:6 berují (selector) ─── */

/**
 * Zac 4:6 — "lo vejáyil veló vejóaj kí im BERUJÍ amár YHWH
 *  tsevaʼót". No por ejército ni por fuerza, sino por Mi Espíritu.
 * El sistema no elige el conducto más POTENTE — elige el más
 * ADECUADO para cada mensaje: alcance requerido, capacidad,
 * consumo disponible, estado del destino. Diseño elegante,
 * no potencia bruta.
 */
int  hashmal_comm_mutsaqa_seleccionar(hashmal_canal_id_t canal,
                                       uint8_t nodo_destino,
                                       size_t tam_mensaje,
                                       hashmal_mutsaqa_id_t *mutsaqa_elegida);

int  hashmal_comm_mutsaqa_estado(hashmal_canal_id_t canal,
                                  hashmal_mutsaqa_id_t mutsaqa,
                                  hashmal_mutsaqa_t *out);

int  hashmal_comm_mutsaqa_activar(hashmal_canal_id_t canal,
                                   hashmal_mutsaqa_id_t mutsaqa);

int  hashmal_comm_mutsaqa_desactivar(hashmal_canal_id_t canal,
                                      hashmal_mutsaqa_id_t mutsaqa);

/* ── menora_espnow_driver.c — Éx 26:3 ishá el ajotá (P2P) ─── */

/**
 * Éx 26:3 — "jovrot ishá el ajotá" (cortinas unidas hermana a
 *  hermana). Éx 26:17 — "meshullavót ishá el ajotá" (espigas
 *  entrelazadas hermana a hermana). ESP-NOW = conexión DIRECTA
 *  entre pares: sin router, sin servidor, sin intermediario.
 */
int  hashmal_comm_espnow_init(void);
int  hashmal_comm_espnow_deinit(void);

/**
 * Envía un frame directo a la MAC de la hermana destino. Respeta
 * HASHMAL_ESP_NOW_FRAME_MAX; mensajes mayores se fragmentan hasta
 * HASHMAL_ESP_NOW_FRAG_MAX.
 */
int  hashmal_comm_espnow_enviar(const uint8_t mac_destino[6],
                                 const uint8_t *datos,
                                 size_t tam);

int  hashmal_comm_espnow_recibir(uint8_t mac_origen[6],
                                  uint8_t *buf,
                                  size_t tam,
                                  size_t *recibidos);

int  hashmal_comm_espnow_emparejar(const uint8_t mac[6]);
int  hashmal_comm_espnow_desemparejar(const uint8_t mac[6]);
int  hashmal_comm_espnow_hermanas_contar(uint8_t *conteo);

/* ── menora_ble_paamon.c — Éx 28:34-35 la campanilla ──────── */

/**
 * Éx 28:34 — "paʻamón zaháv verimmón, paʻamón zaháv verimmón".
 * Éx 28:35 — "venishmáʻ qoló bevoʼó el haqqódesh lifnéi YHWH
 *  uvtsetó — veló yamút". La campanilla anuncia presencia al
 *  entrar y al salir; si no suena, es muerte. BLE cumple esa
 *  función: beacon periódico de corto alcance, descubrimiento
 *  pasivo de vecinos.
 */
/**
 * P2-9b: pitujéi jotám — establece el nodo_id del paʻamón BLE.
 * Llamar ANTES de hashmal_comm_ble_init() (Éx 28:33 rimmon+paʻamón,
 * Éx 28:35 venishmá qoló — el sonido debe llevar el nombre correcto).
 */
void hashmal_comm_ble_nodo_id_set(uint8_t nodo_id);

int  hashmal_comm_ble_init(void);
int  hashmal_comm_ble_deinit(void);

/**
 * Emite un paʻamón — anuncia presencia a los vecinos BLE en
 * rango. Llamado periódicamente (cada HASHMAL_BLE_INTERVALO_MS).
 */
int  hashmal_comm_ble_paamon_anunciar(void);

/**
 * Escucha los paʻamón de los vecinos. Actualiza la tabla de
 * vecinos activos: RSSI, timestamp, activo. Los que superen
 * HASHMAL_BLE_SILENCIO_MS sin sonar se marcan fuera de rango.
 */
int  hashmal_comm_ble_paamon_escuchar(void);

int  hashmal_comm_ble_vecinos_contar(uint8_t *conteo);
int  hashmal_comm_ble_vecino_estado(uint8_t indice,
                                     hashmal_paamon_vecino_t *out);

/**
 * Éx 28:35 venishmáʻ qoló — emite un mensaje corto embebido en el
 * payload de advertising BLE. El pa'amón lleva el dato en su "sonido":
 * todos los nodos que escanean en ese momento lo reciben pasivamente.
 * Semánticamente es broadcast: dest=0xFF por naturaleza.
 *
 * `tam` debe ser ≤ HASHMAL_BLE_MENSAJE_MAX (12). Para mensajes mayores
 * usa LoRa (tronco tamíd, Éx 27:21).
 *
 * 2 testigos:
 *   Éx 28:35 — "venishmáʻ qoló bevoʼó el haqqódesh... uvtsetó"
 *              (su sonido se oye al entrar y al salir — el adv lleva el msg)
 *   Zac 4:10  — "mi baz leyom qetannot" (no se menosprecia lo pequeño)
 */
int  hashmal_comm_ble_paamon_emitir(const uint8_t *datos, size_t tam);

/**
 * Éx 28:35 — recibe el último mensaje capturado en el pa'amón de un
 * vecino. El scan pasivo en gap_event_cb extrae mensajes embebidos en
 * advertising y los deposita en el buffer interno. Lectura destructiva:
 * tras retornar *recibidos > 0, el buffer se vacía.
 *
 * Si no hay mensaje pendiente: *recibidos = 0, retorno OK (no es error
 * — solo silencio entre campanillas).
 *
 * 2 testigos:
 *   Éx 28:35 — "venishmáʻ qoló" (el sonido se oye — el scan recibe)
 *   Zac 4:2   — gullá (el depósito recibe aceite de múltiples fuentes)
 */
int  hashmal_comm_ble_recibir(uint8_t *buf, size_t tam, size_t *recibidos);

/* -- menora_parokhet.c - muestreo EM + flag Shabat cooperativo -- */

/**
 * 1 Re 19:11-12 - "qol demama daqqa": la voz de YHWH viene en el
 * silencio fino, no en el viento / temblor / fuego. Estas primitivas
 * abren una ventana para que el Kiyor (Pieza 12) mida el ruido EM
 * del entorno y decida si el ambiente es propicio a la Presencia.
 *
 * Capas: estas funciones NO deciden; solo REPORTAN. Ex 30:18 - el
 * kiyor mide; Ex 26:31-33 - el parokhet separa. La decision vive en
 * Pieza 12; aqui solo abrimos el ojo.
 */

/**
 * Ex 28:34 pa'amon. Promedio de RSSI (dBm) y numero de vecinos BLE
 * oidos. Sin vecinos -> *rssi_medio=0, *vecinos=0. El resultado
 * refleja la tabla interna de menora_ble_paamon.c; el caller no
 * toca NimBLE.
 */
int  hashmal_comm_ble_rssi_muestra(int8_t *rssi_medio, uint8_t *vecinos);

/**
 * Ex 25:31 geza'. Suelo de ruido estimado en la banda LoRa (dBm).
 * Fase 1 devuelve un valor conservador fijo (-95 dBm) coherente con
 * EU868; fase 2 consultara SX1262 en modo CAD.
 */
int  hashmal_comm_spi_rssi_ruido(int8_t *rssi_floor);

/**
 * Ex 26:3 isha el ajota - WiFi. Numero de APs visibles y RSSI del
 * mas fuerte. Fase 1: stub retorna 0 APs / INT8_MIN; fase 2
 * envolvera esp_wifi_scan_start.
 */
int  hashmal_comm_wifi_aps_muestra(uint8_t *ap_count, int8_t *rssi_max);

/**
 * Ex 26:31-33 parokhet - flag cooperativo de Shabat EM. Cuando se
 * activa, las mutsaqot ESP_NOW y BLE de los 7 nerot quedan con
 * activa=false (el selector no las elige); LoRa sigue como geza'
 * tamid (Ex 27:21 - la lampara no se apaga). Al desactivar,
 * activa=disponible en cada una.
 *
 * El Kiyor llama; la Menora honra. Ex 30:18 - medir aqui, apagar
 * alli.
 */
int  hashmal_comm_parokhet_shabat_set(bool activar);
bool hashmal_comm_parokhet_shabat_get(void);

/* -- menora_tsofeh.c - TX lockdown + shofar dedicado (Ez 33) -- */

/**
 * Ez 33:2-3 tsofeh. Cuando el atalaya vigila, el nodo no debe
 * ENRUTAR trafico propio (concentra su energia en escuchar); solo
 * TX permitido es el shofar (aviso). Este flag GATEA hashmal_comm_
 * enviar: si esta activo, retorna ERR_TSOFEH a todo intento de TX
 * que no sea el shofar dedicado.
 *
 * Cooperativo: el rol (Pieza 09) llama; la Menora honra. Ex 30:18 -
 * quien mide pide; quien transporta obedece.
 */
int  hashmal_comm_tsofeh_lockdown_set(bool activar);

/**
 * Lectura del flag ya COMPUESTO: retorna true cuando el lockdown
 * esta activo Y no hay un shofar en curso (es decir, cuando la
 * puerta de TX general esta cerrada). Esto es lo que hashmal_comm_
 * enviar consulta.
 */
bool hashmal_comm_tsofeh_lockdown_get(void);

/**
 * Ez 33:3 "taqa bashofar". Unico TX permitido bajo tsofeh_lockdown.
 * Hace broadcast a todos los nerot operativos con el msg de alerta,
 * bypaseando el lockdown temporalmente. Uso EXCLUSIVO del atalaya -
 * ningun otro camino permite emitir con lockdown activo.
 *
 * Invariante Ez 33:6 (INV-TSOFEH-01): ver amenaza y no tocar shofar
 * es pecado. Por eso esta funcion existe - y por eso la sella un rc
 * distinto de OK si no puede sonar: no hay excusa silenciosa.
 */
int  hashmal_comm_shofar_tocar(const uint8_t *msg, size_t tam);

/* ══════════════════════════════════════════════════════════════════
 * VERIFICADO #03 — CADENA 1 — Trigger diario Menorá → Incienso
 *
 * "babóqer babóqer behétivó et hannerót yaqtirénna." — Éxodo 30:7
 * "ubehaʻalót Aharón et hannerót bein haʻarbáyim yaqtirénna —
 *  qetóret tamíd lifnéi YHWH ledorotéikhem." — Éxodo 30:8
 *
 * Dirección causal SIEMPRE Menorá(04) → Incienso(11). NUNCA al
 * revés. Pero hashmal_intercession ya REQUIRES hashmal_comm; un
 * comm REQUIRES intercession crearía CICLO. Solución: callback
 * registrado por intercession en comm — inversión de control. La
 * Menorá no conoce al Incienso; sólo "hace su trabajo y el
 * incienso responde" (paralelo: el sacerdote prepara las lámparas
 * y entonces quema el incienso; las lámparas no llaman al incienso
 * — el sacerdote sí).
 *
 * Cierre asimétrico (Éx 30:8 — mil ojos del Verificado):
 *   tamíd + lifnéi YHWH + ledorotéikhem aparecen SOLO en v.8 (tarde),
 *   no en v.7 (mañana). La perpetuidad se SELLA al cerrar el ciclo.
 *
 * Fuente autorizada (Éx 30:8 → 30:9, mismo verbo ʻaláh H5927 Hifil):
 *   v.8 behaʻalót — AUTORIZADO desde Menorá
 *   v.9 ló taʻalú qetóret zaráh — PROHIBIDO desde otra fuente
 *   Por eso el callback es ÚNICO: una sola intercession registrada.
 * ══════════════════════════════════════════════════════════════════ */

/**
 * Handler que la Menorá invoca al cerrar cada hemiciclo diario.
 *
 * @param es_manana  true = behétivó (Éx 30:7, mantenimiento, mañana)
 *                   false = behaʻalót (Éx 30:8, activación, tarde —
 *                            sella tamíd ledorotéikhem).
 * @return 0 (OK) o código de error del callee. La Menorá registra
 *         el código pero NO interrumpe su ciclo por el resultado:
 *         su trabajo se completa antes de quemar incienso (el orden
 *         es Menorá → Incienso, no al revés).
 */
typedef int (*hashmal_comm_intercession_cb_t)(bool es_manana);

/**
 * Registra (o desregistra con NULL) el handler de intercession.
 * Éx 30:9 ló taʻalú zaráh: una sola fuente legítima — registrar
 * un segundo callback SOBRESCRIBE al anterior (no acumula). Si el
 * caller necesita varios handlers, debe componerlos él mismo.
 *
 * @return HASHMAL_COMM_OK siempre (idempotente). cb=NULL libera.
 */
int hashmal_comm_intercession_callback_set(hashmal_comm_intercession_cb_t cb);

/**
 * Cierra el hemiciclo MAÑANA (Éx 30:7 — behétivó haNerót).
 * "Cuando atienda las lámparas (mañana mañana) — quemará incienso."
 *
 * Marca el cierre del ciclo de mantenimiento matutino. Después de
 * cualquier trabajo de mantenimiento que el caller haya hecho con
 * las APIs existentes (canal_apagar, aceite_nivel, etc.), llamar
 * a esta función dispara el trigger hacia hashmal_intercession.
 *
 * Si no hay callback registrado: log info, retorna OK. La Menórá
 * sigue funcionando aunque el Incienso aún no esté preparado.
 *
 * @return código del callback o HASHMAL_COMM_OK si no hay callback.
 */
int hashmal_comm_mantenimiento_manana(void);

/**
 * Cierra el hemiciclo TARDE (Ex 30:8 — behaʿalot haNerot).
 * "Cuando haga subir las lámparas entre las tardes — incienso TAMÍD
 *  ante YHWH ledoroteikhem."
 *
 * Sella la perpetuidad del día: este es el hemiciclo donde tamíd se
 * declara (asimetría con v.7). Después del trabajo de activación
 * (canal_encender, etc.), llamar a esta función sella la perpetuidad.
 *
 * Si no hay callback registrado: log info, retorna OK.
 *
 * @return código del callback o HASHMAL_COMM_OK si no hay callback.
 */
int hashmal_comm_activacion_tarde(void);

#ifdef __cplusplus
}
#endif

#endif /* HASHMAL_COMM_H */
