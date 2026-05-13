/**
 * ═══════════════════════════════════════════════════════════════════
 * PIEZA 03 — LA MESA DEL PAN (hashmal_cache)
 * Éxodo 25:23-30 + Levítico 24:5-9
 * ═══════════════════════════════════════════════════════════════════
 *
 * "veʻasíta shulján ʻatsé shittím ammatáyim orkó veʼammá rojbó
 *  veʼammá vajétsi qomató" — Éx 25:23
 * "Harás una MESA de madera de acacia: dos codos de largo, un
 *  codo de ancho, y codo y medio de alto."
 *
 * "venatáta ʻal hashulján léhem paním lefanái TAMÍD" — Éx 25:30
 * "Pondrás sobre la mesa pan de la PRESENCIA delante de Mí
 *  CONTINUAMENTE."
 *
 * "velaqájta sólet veʼafíta otáh shté ʻesré jallót... vesamtá
 *  otám shté maʻarajót SHESH hammaʻarájet" — Lv 24:5-6
 * "Tomarás harina fina y cocerás DOCE tortas... las pondrás en
 *  DOS FILAS, SEIS por fila."
 *
 * "beyóm hashabbát beyóm hashabbát yaʻarejénnu... berít ʻolám"
 *   — Lv 24:8
 * "Cada SHABAT lo renovará... pacto eterno."
 *
 * LA MESA está en el Qódesh (Lugar Santo), no en el Qódesh
 * haQodashím. Expuesta más que el Arca/Kapóret, pero aún sagrada.
 * Caché con 12 slots, 4 operaciones CRUD (los utensilios), un
 * misgéret (rate limiter), épocas de Shabat (refresco cíclico).
 *
 * ─── 19 DETALLES BÍBLICOS (Tavnít 03) ───
 *
 *   1  shulján (v.23)                → mesa = superficie de servicio/caché
 *   2  acacia (shittím, v.23)         → formato incorruptible
 *   3  dimensiones 2×1×1.5 (v.23)     → menor que el Arca, qomá "y medio"
 *   4  qomá "y medio" (v.23)          → depende del Arca para completarse
 *   5  oro puro (v.24)                → pureza sin "mibáyit umijúts"
 *   6  zer zaháv savív (v.24)         → Merkle sobre los 12 slots
 *   7  misgéret tófaj (v.25)          → marco = rate limiter, 1 palmo fijo
 *   8  zer SOBRE misgéret (v.25)      → doble protección del marco
 *   9  4 anillos (v.26-27)            → 4 puntos de conexión
 *  10  varas (v.28) SIN "lo yasúru"   → transport opcional, no always-on
 *  11  qeʻarót / kappót / qesót /
 *      menaqiyyót (v.29)              → 4 utensilios = 4 CRUD
 *  12  léhem paním (v.30)             → pan de la presencia, dato disponible
 *  13  TAMÍD (v.30)                   → perpetuo, mesa nunca vacía
 *  14  sólet (Lv 24:5)                → harina fina, dato refinado
 *  15  12 jallót (Lv 24:5)            → 12 slots
 *  16  2 filas × 6 (Lv 24:6)          → geometría del caché
 *  17  levoná zajjá (Lv 24:7)         → metadata / azjará por slot
 *  18  Shabat yaʻarejénnu (Lv 24:8)   → ciclo de refresco
 *  19  berít ʻolám (Lv 24:8)          → ciclo perpetuo
 *
 * ─── CONTRASTES CON EL ARCA ───
 *
 *   Arca (01)                           Mesa (03)
 *   ────────────────────────────────    ────────────────────────────────
 *   Qódesh haQodashím                   Qódesh (expuesta)
 *   oro mibáyit umijúts (v.11)          oro sin "dentro y fuera" (v.24)
 *   baddím "lo yasúru" (v.15)           varas sin tal mandato (v.26-28)
 *   1 contenido (edut) + 1 al lado      12 slots en 2×6
 *   sin misgéret                        misgéret tófaj (v.25)
 *   testimonio inmutable                caché con Shabat cycle
 *
 * Autor: Antonio José Marín Soto
 * Proyecto: HASHMAL-CORE
 */
#ifndef HASHMAL_CACHE_H
#define HASHMAL_CACHE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "hashmal_storage.h"   /* reutiliza HASHMAL_ZER_HASH_BYTES */

#ifdef __cplusplus
extern "C" {
#endif


/* ═══════════════════════════════════════════════════════════════════
 * HALLAZGOS MIL OJOS DE QUERUBÍN (P1-P59)
 * Verificados por Sistema de los Querubines — Éxodo 25:22
 * ═══════════════════════════════════════════════════════════════════ */

/* ── P32 — Lv 25:6 leʼojláh universal: Shemitah = open access ──
 * Lv 25:6 — "vehayetáh shabát haʼárets LAJÉM LEʼOJLÁH" — durante
 * la Shemitah (7ª época), el producto de la tierra es para COMER
 * para TODOS: tú, tu siervo, tu sierva, tu asalariado, tu
 * residente, y el extranjero (hagarím ʻimmáj). Lv 25:7 extiende
 * incluso a los animales salvajes (layajáh). Esto revela un
 * CUARTO NIVEL de gobernanza de recursos que el Verificado 22 no
 * tenía: Man (cuota diaria) → Leket (rincones para débiles) →
 * Ma'aser (contribución al servicio) → SHEMITAH (acceso total).
 * Cada 7 épocas de caché, los slots no solo se REFRESCAN (Shabat
 * semanal, Lv 24:8) sino que el acceso se ABRE a TODOS los nodos,
 * incluyendo los que normalmente no tienen permiso de lectura.
 * "shabát haʼárets… lajém leʼojláh." */


/* ══════════════════════════════════════════════════════════════════
 * ENUMS FUNDAMENTALES
 * ══════════════════════════════════════════════════════════════════ */

/**
 * Estados de un slot (una torta de pan) a lo largo de su vida.
 *
 *   VACIO     → slot sin pan (antes del primer qeʻará o después de
 *               menaqit). Lv 24:8 tras retirar el pan viejo.
 *   FRESCO    → recién horneado (puesto o refrescado), no leído aún.
 *               Estado primario para "léhem tamíd" (v.30).
 *   SERVIDO   → leído al menos una vez (cucharón tomó); sigue utilizable
 *               hasta expirar, pero ya no es "virgen".
 *   EXPIRADO  → ciclo Shabat elapsó sin refresco; necesita yaʻarejénnu
 *               (Lv 24:8). No se lee de un expirado sin riesgo de dato
 *               rancio.
 */
typedef enum {
    HASHMAL_LEJEM_VACIO    = 0,
    HASHMAL_LEJEM_FRESCO   = 1,
    HASHMAL_LEJEM_SERVIDO  = 2,
    HASHMAL_LEJEM_EXPIRADO = 3,
} hashmal_lejem_estado_t;

/**
 * P9 — Éx 25:23 — qomáh vakhetsí: completud de la Mesa.
 *
 *   Dimensiones (v.23): 2 × 1 × 1.5 codos.
 *     largo (orkó)  = 2     entero
 *     ancho (rojbó) = 1     entero
 *     alto (qomató) = 1.5   ÚNICO con vakhetsí (y-mitad)
 *
 * El plano horizontal (superficie de servicio) es COMPLETO; sólo la
 * altura (qomá H6967, de qum H6965 = levantarse) tiene vakhetsí.
 * v.30 lo cierra: "venatáta ʻal hashulján LÉJEM PANÍM" — el
 * CONTENIDO encima completa la mesa.
 *
 * Contraste con el Arca (v.10): TRIPLE vakhetsí en las 3 dimensiones
 * → necesita OTRA pieza (Kapóret). La Mesa necesita SOLO su propio
 * contenido — la incompletud se resuelve internamente (par A/A' del
 * quiasmo: A = dimensiones, A' = contenido encima).
 *
 *   LEVANTADO  estructura inicializada, 0 slots ocupados → qomáh
 *              sin resolver (mesa lista, todavía sin pan).
 *   COMPLETO   ≥ 1 slot con dato válido (FRESCO o SERVIDO) → qomáh
 *              resuelta (pan encima). v.30 cumplido para este nodo.
 *
 * El healthcheck (pieza 12 — Lavacro) debe reportar la diferencia:
 * un caché LEVANTADO no es un fallo, pero tampoco es léjem paním
 * tamíd (v.30) hasta que tenga al menos un pan.
 */
typedef enum {
    HASHMAL_CACHE_ESTADO_NO_INIT    = 0,  /* aún no init() */
    HASHMAL_CACHE_ESTADO_LEVANTADO  = 1,  /* v.23 — estructura, 0 slots ocupados */
    HASHMAL_CACHE_ESTADO_COMPLETO   = 2,  /* v.30 — ≥1 slot FRESCO/SERVIDO */
} hashmal_cache_completud_t;

/**
 * v.29 — los 4 utensilios de la Mesa, cada uno una operación CRUD.
 * qeʻarót, kappót, qesót, menaqiyyót son 4 palabras DISTINTAS en el
 * texto; no son sinónimos. Son 4 operaciones.
 *
 *   QEARA    (qeʻará, platos)   → CREATE (donde se PONE el pan)
 *   KAF      (kaf, cuchara)     → READ   (con qué se TOMA)
 *   QASA     (qasá, jarra)      → UPDATE (con qué se REFRESCA / cubre)
 *   MENAQIT  (menaqít, copa)    → DELETE (con qué se VIERTE / limpia)
 */
typedef enum {
    HASHMAL_UTENSILIO_QEARA   = 0,  /* v.29 — qeʻarót, CREATE */
    HASHMAL_UTENSILIO_KAF     = 1,  /* v.29 — kappót,   READ   */
    HASHMAL_UTENSILIO_QASA    = 2,  /* v.29 — qesót,    UPDATE */
    HASHMAL_UTENSILIO_MENAQIT = 3,  /* v.29 — menaqiyyót, DELETE */
} hashmal_utensilio_t;

/**
 * Códigos de error.
 */
typedef enum {
    HASHMAL_CACHE_OK               = 0,
    HASHMAL_CACHE_ERR_NULL         = -1,
    HASHMAL_CACHE_ERR_INIT         = -2,
    HASHMAL_CACHE_ERR_SLOT_VACIO   = -3,  /* leer/refrescar sobre VACIO */
    HASHMAL_CACHE_ERR_SLOT_LLENO   = -4,  /* qeʻará sobre FRESCO/SERVIDO */
    HASHMAL_CACHE_ERR_MISGERET     = -5,  /* v.25 — tófaj: rate limit */
    HASHMAL_CACHE_ERR_FUERA_SHABAT = -6,  /* Lv 24:8 — ciclo expirado */
    HASHMAL_CACHE_ERR_NO_AUTORIZADO = -7,
    HASHMAL_CACHE_ERR_INDICE       = -8,  /* slot fuera de [0, 11] */
    HASHMAL_CACHE_ERR_ZER_ROTO     = -9,  /* v.24 — zer no casa */
    HASHMAL_CACHE_ERR_TAMANO       = -10, /* tam > LEJEM_BYTES_MAX */
    HASHMAL_CACHE_ERR_MAN_EXCEDIDO = -11, /* Añadidura 11 - Ex 16:16 omer excedido */
    HASHMAL_CACHE_ERR_SHINNAN      = -12, /* Añadidura 17 - Dt 6:7 veshinantam */
    HASHMAL_CACHE_ERR_LEKET        = -13, /* Añadidura 18 - Lv 19:9-10 espigueo */
    HASHMAL_CACHE_ERR_MAASER       = -14, /* Añadidura 19 - Mal 3:10 diezmo */
} hashmal_cache_err_t;

/* ══════════════════════════════════════════════════════════════════
 * CONSTANTES DEL PADRE
 * ══════════════════════════════════════════════════════════════════ */

/**
 * v.23 — "ammatáyim orkó veʼammá rojbó veʼammá vajétsi qomató".
 * 2 × 1 × 1.5 codos. Largo y ancho ENTEROS (≠ Arca); solo qomá
 * "y medio" — depende del Arca para "cerrar" la altura.
 */
#define HASHMAL_SHULJAN_LARGO_X10      20   /* v.23 — 2 ammá */
#define HASHMAL_SHULJAN_ANCHO_X10      10   /* v.23 — 1 ammá */
#define HASHMAL_SHULJAN_ALTO_X10       15   /* v.23 — 1.5 ammá */
#define HASHMAL_SHULJAN_ESCALA         10

/**
 * v.23 — "ʻatsé shittím". Acacia, igual que el Arca.
 */
#define HASHMAL_MESA_MADERA_ACACIA     true  /* v.23 — shittím */

/**
 * v.24 — "zaháv tahór". Oro puro. A diferencia del Arca (v.11)
 * NO se especifica "mibáyit umijúts" — la Mesa tiene oro solo
 * externo. Menos secreto, más expuesta.
 */
#define HASHMAL_MESA_ZAHAV_TAHOR       true  /* v.24 — zaháv tahór */

/**
 * Lv 24:5-6 — "shté ʻesré jallót... shté maʻarajót shesh
 * hammaʻarájet". DOCE tortas, DOS filas, SEIS por fila.
 */
#define HASHMAL_SHULJAN_SLOTS          12    /* Lv 24:5 — shté ʻesré */
#define HASHMAL_SHULJAN_FILAS          2     /* Lv 24:6 — shté maʻarajót */
#define HASHMAL_SHULJAN_POR_FILA       6     /* Lv 24:6 — shesh hammaʻarájet */

/**
 * v.25 — "misgéret tófaj savív". Marco de UN PALMO alrededor.
 * Tófaj es medida fija (≈ 7.5 cm). En firmware: rate limiter con
 * parámetros fijos.
 */
#define HASHMAL_MESA_MISGERET_TOFAJ    1     /* v.25 — tófaj (1 palmo) */

/**
 * Ventana de tasa (ms) y capacidad por ventana. La medida en
 * versículo es física (tófaj); aquí la operacionalizamos: cuántas
 * peticiones caben "entre los palmos del marco" por unidad de tiempo.
 */
#define HASHMAL_MISGERET_VENTANA_MS    1000u /* 1 s */
#define HASHMAL_MISGERET_MAX_VENTANA   60u   /* 60 peticiones / s */

/**
 * v.26 — "arbaʻá tabbeʻót zaháv". 4 anillos, dos por lado (v.26b
 * "ʻal arbaʻ hapeʼót"). Mismo número que el Arca.
 */
#define HASHMAL_MESA_ANILLOS_TOTAL     4     /* v.26 */

/**
 * v.28 — "veʻasíta et habbaddím". Varas. A diferencia del Arca
 * (v.15 "lo yasúru mimménu"), la Mesa NO tiene prohibición de
 * retirar sus varas — por eso varas_puestas es bool con transición.
 */
#define HASHMAL_MESA_BADDIM_TOTAL      2     /* v.28 */

/**
 * v.30 — "léhem paním lefanái TAMÍD". Continuamente. Flag de
 * diseño: la mesa nunca está totalmente vacía mientras tamíd=true.
 */
#define HASHMAL_MESA_TAMID             true  /* v.30 — tamíd */

/**
 * Lv 24:8 — "beyóm hashabbát". Un ciclo de 7 días. Para firmware:
 * expresado en milisegundos. berít ʻolám — repite para siempre.
 */
#define HASHMAL_SHABAT_CICLO_MS        604800000u  /* 7×24×3600×1000 ms */

/**
 * Tamaño máximo del contenido de un slot (una jallá). Elección de
 * implementación — la Palabra fija la cantidad (12) y la disposición
 * (2×6), no el tamaño individual.
 */
#define HASHMAL_LEJEM_BYTES_MAX        256   /* bytes por slot */

/**
 * P7 — Éx 25:30 — DOBLE PANÍM:
 *
 *   "venatáta ʻal hashulján LÉHEM PANÍM lefanái TAMÍD"
 *   "Pondrás sobre la mesa el PAN DE LOS ROSTROS delante de Mí
 *    continuamente."
 *
 * paním (H6440 = rostros) aparece DOS veces en el versículo con
 * funciones opuestas:
 *
 *   léhem PANÍM     → el pan TIENE rostros (identidad: qué nodo lo generó)
 *   leFANÁI         → el pan está orientado HACIA el rostro (qué fuente
 *                     lo validó)
 *
 * Un dato sin identidad NO es léjem paním. Un dato sin orientación
 * NO está lefanái. Necesita AMBOS — middabbér: identidad propia Y
 * dirección hacia la fuente.
 *
 * Sentinel para "rostros ausentes": ningún slot legítimo puede llevar
 * 0 en panim_origen ni en panim_destino. qeʻará rechaza tal pan.
 */
#define HASHMAL_PANIM_INVALIDO         0u    /* v.30 — rostros ausentes */

/**
 * Lv 24:7 — "venatáta ʻal hammaʻaréjet LEVONÁ zajjá vehayetá
 * lalléhem leʼAZJARÁ". Incienso puro sobre cada fila, como MEMORIAL
 * (azjará). En firmware: metadata por slot (hash + etiquetas).
 */
#define HASHMAL_AZJARA_BYTES           32    /* Lv 24:7 — memorial */

/**
 * Éx 40:4 — la Mesa se coloca en paso 3 (tras Arca/Kapóret).
 */
#define HASHMAL_MESA_ORDEN_BOOT        3     /* Éx 40:4 */

/* ══════════════════════════════════════════════════════════════════
 * ESTRUCTURAS
 * ══════════════════════════════════════════════════════════════════ */

/**
 * Un "pan" — el contenido de un slot. Incluye su propia azjará
 * (memorial, Lv 24:7) y timestamps para el ciclo Shabat.
 *
 * P7 — Éx 25:30 doble paním (léjem PANÍM lefanái):
 *   panim_origen  = identidad (qué nodo generó este pan)
 *   panim_destino = orientación (hacia qué fuente fue validado)
 * Ambos OBLIGATORIOS y != HASHMAL_PANIM_INVALIDO. Sin paním_origen
 * el pan no tiene rostros (no es léjem paním). Sin paním_destino
 * no está orientado hacia ningún rostro (no está lefanái).
 */
typedef struct {
    uint8_t  bytes[HASHMAL_LEJEM_BYTES_MAX];
    uint16_t tamano;                               /* bytes usados (≤ MAX) */
    uint8_t  zer_slot[HASHMAL_ZER_HASH_BYTES];     /* v.24 — SHA-256 del slot */
    hashmal_lejem_estado_t estado;

    /* P7 — v.30 — DOBLE PANÍM (rostros: identidad + orientación) */
    uint8_t  panim_origen;                         /* léjem paním: nodo origen */
    uint8_t  panim_destino;                        /* lefanái: fuente validadora */

    /* Lv 24:7 — levoná zajjá: memorial (metadata por slot) */
    uint8_t  azjara[HASHMAL_AZJARA_BYTES];

    /* Lv 24:8 — yaʻarejénnu: tiempo de horneado y de último servicio */
    uint64_t timestamp_horneado_ms;
    uint64_t timestamp_servido_ms;
} hashmal_lejem_t;

/**
 * v.25 — misgéret tófaj. Rate limiter del marco de un palmo.
 * Un palmo es medida fija → la ventana es fija; las peticiones
 * cuentan y desbordarse es caer FUERA del marco.
 */
typedef struct {
    uint8_t  tofaj;                    /* v.25 — 1 palmo, medida fija */
    uint32_t peticiones_ventana;       /* contador actual */
    uint32_t max_ventana;              /* capacidad por ventana */
    uint32_t ventana_ms;               /* duración de la ventana */
    uint64_t timestamp_ventana_ms;     /* inicio de la ventana actual */
} hashmal_misgeret_t;

/**
 * LA MESA — composite. Singleton por nodo. Vista 2D (filas×columnas)
 * se obtiene por slots[fila*6 + col]; mantenemos slots[12] como
 * almacenamiento primario (Lv 24:5 ordena 12 tortas).
 */
typedef struct {
    /* v.23 — dimensiones (largo y ancho enteros, alto "y medio") */
    uint8_t  largo_x10;
    uint8_t  ancho_x10;
    uint8_t  alto_x10;

    /* v.23-24 — madera + oro externo */
    bool     madera_acacia;
    bool     zahav_tahor;

    /* v.24 — zer zaháv savív (Merkle sobre los 12 slots) */
    uint8_t  zer_exterior[HASHMAL_ZER_HASH_BYTES];
    uint64_t zer_nonce;

    /* v.25 — misgéret tófaj (rate limiter) */
    hashmal_misgeret_t misgeret;

    /* Lv 24:5-6 — 12 slots (2 filas × 6) */
    hashmal_lejem_t slots[HASHMAL_SHULJAN_SLOTS];

    /* v.26-28 — anillos fijos, varas con transición (SIN "lo yasúru") */
    uint8_t  anillos_total;            /* = HASHMAL_MESA_ANILLOS_TOTAL */
    bool     varas_puestas;            /* v.26-28 — puede cambiar */

    /* v.30 — léhem paním tamíd */
    bool     tamid_activo;             /* "lefanái tamíd" */

    /* Lv 24:8 — ciclo Shabat */
    uint32_t shabat_ciclo_ms;          /* = HASHMAL_SHABAT_CICLO_MS */
    uint64_t shabat_ultimo_refresco_ms;

    /* Heb 9:2 — "skené... en hé he te lyjnía kaì he trápeza": la
     * Mesa está en el primer tabernáculo (Qódesh), no en el
     * Qódesh haQodashím. Flag de diseño. */
    bool     en_qodesh;                /* true, en el Lugar Santo */
} hashmal_shuljan_t;

/* ══════════════════════════════════════════════════════════════════
 * FUNCIONES PÚBLICAS
 * ══════════════════════════════════════════════════════════════════ */

/* ── Ciclo de vida ─────────────────────────────────────── */

/**
 * Construye la Mesa — Éx 25:23-28 + Lv 24:5-8. Éx 40:4 la coloca
 * tras el Arca/Kapóret.
 */
int  hashmal_cache_init(void);

/**
 * Desmonta la Mesa. Nm 3:31 — los coatitas llevaban la Mesa bajo
 * cubiertas; aquí solo limpiamos estado volátil.
 */
void hashmal_cache_deinit(void);

/**
 * Lee-solamente del singleton. @return NULL si no inicializada.
 */
const hashmal_shuljan_t* hashmal_cache_get_shuljan(void);

/* ── Los 4 UTENSILIOS (v.29 — CRUD) ────────────────────── */

/**
 * qeʻarót — CREATE. Pone un pan nuevo en un slot VACIO.
 * Requiere misgéret (rate check). Si slot ya tiene contenido
 * (FRESCO/SERVIDO) → ERR_SLOT_LLENO.
 *
 * P7 — Éx 25:30 doble paním OBLIGATORIO. Ambos panim_* deben ser
 * != HASHMAL_PANIM_INVALIDO; si alguno es 0 → ERR_NO_AUTORIZADO
 * (el pan SIN rostros no es léjem paním; SIN orientación no es lefanái).
 *
 *   slot           ∈ [0, 11]
 *   bytes          contenido (referencia copiada al slot)
 *   tam            bytes ∈ (0, HASHMAL_LEJEM_BYTES_MAX]
 *   panim_origen   identidad: nodo que generó este dato (≠ 0)
 *   panim_destino  orientación: fuente que validó este dato (≠ 0)
 */
int  hashmal_cache_qeara_poner(uint8_t slot,
                               const uint8_t *bytes,
                               size_t tam,
                               uint8_t panim_origen,
                               uint8_t panim_destino);

/**
 * kappót — READ. Toma (lee) el pan del slot. Marca SERVIDO si
 * estaba FRESCO. No destructivo (el contenido permanece hasta
 * menaqit/shabat).
 *
 *   slot        ∈ [0, 11]
 *   out         buffer de salida (tamaño ≥ *tam_inout)
 *   tam_inout   in: capacidad del buffer; out: bytes escritos
 */
int  hashmal_cache_kaf_tomar(uint8_t slot,
                             uint8_t *out,
                             size_t *tam_inout);

/**
 * qesót — UPDATE. Refresca el contenido de un slot FRESCO/SERVIDO.
 * Reinicia timestamps (yaʻarejénnu, Lv 24:8). No permite refresh
 * sobre VACIO (usar qeʻará).
 *
 * P7 — Refrescar también requiere doble paním (la identidad y la
 * orientación pueden cambiar al refrescar — pan nuevo, rostros nuevos).
 * Ambos panim_* != HASHMAL_PANIM_INVALIDO o → ERR_NO_AUTORIZADO.
 */
int  hashmal_cache_qasa_refrescar(uint8_t slot,
                                  const uint8_t *bytes,
                                  size_t tam,
                                  uint8_t panim_origen,
                                  uint8_t panim_destino);

/**
 * menaqiyyót — DELETE. Vierte (vacía) el slot. Estado → VACIO.
 * Idempotente sobre VACIO (no-op).
 */
int  hashmal_cache_menaqit_vaciar(uint8_t slot);

/* ══════════════════════════════════════════════════════════════════
 * P8 — DISTRIBUCIÓN (Éx 25:23 — shulján ← raíz shaláj H7971)
 *
 *   שֻׁלְחָן (shulján, H7979) ← שלח (shaláj, H7971 = enviar / extender)
 *   Forma quṭlán pasiva = "lo que ha sido extendido / enviado"
 *
 * El Padre NO llama a esta pieza con raíz de almacenamiento:
 *   ¬ אצר (atsár, H686, atesorar)
 *   ¬ שמר (shamár, H8104, guardar)
 *
 * El nombre codifica la función: la MESA es el lugar desde donde los
 * datos se ENVÍAN. El almacenamiento es del Arca (arón, hashmal_storage).
 * La distribución es de la Mesa (shulján, hashmal_cache). Mesa ↔
 * Menorá se miran (Éx 26:35): la Mesa envía lo que la Menorá transmite.
 *
 * `hashmal_cache_distribuir` es la PRIMARIA: refleja envío activo. La
 * existente `kaf_tomar` (READ pasivo) sigue disponible pero su rol
 * conceptual es subordinado — toma con cuchara lo que la mesa ya
 * extendió. La distribución entrega además los DOS paním (origen +
 * destino, P7) para que el receptor sepa "quién hizo este pan y
 * hacia qué fuente fue validado."
 * ══════════════════════════════════════════════════════════════════ */

/**
 * Éx 25:23 — distribuye (envía) el contenido del slot al caller,
 * con la identidad y la orientación. Marca el slot SERVIDO. Pasa
 * por misgéret (rate) y verifica zer antes de copiar.
 *
 * @param slot               ∈ [0, 11]
 * @param out                buffer de salida (no NULL)
 * @param tam_inout          in: capacidad; out: bytes escritos
 * @param out_panim_origen   identidad del nodo que generó el dato (no NULL)
 * @param out_panim_destino  fuente contra la que fue validado (no NULL)
 * @return OK; ERR_NULL; ERR_INDICE; ERR_INIT; ERR_ZER_ROTO;
 *         ERR_SLOT_VACIO; ERR_MISGERET.
 */
int hashmal_cache_distribuir(uint8_t  slot,
                             uint8_t *out,
                             size_t  *tam_inout,
                             uint8_t *out_panim_origen,
                             uint8_t *out_panim_destino);

/* ── Misgéret (v.25 — rate limiter) ────────────────────── */

/**
 * ¿La petición actual cabe dentro del marco (tófaj)? Avanza el
 * contador de la ventana actual; si lo excede → ERR_MISGERET.
 * Llamada internamente por qeʻará/qesá (mutadoras); útil también
 * como pre-check externo.
 */
int  hashmal_cache_misgeret_permitir(void);

/* ── Shabat (Lv 24:8 — yaʻarejénnu) ────────────────────── */

/**
 * Renueva el ciclo: marca todos los slots EXPIRADO como VACIO (el
 * pan viejo se retira para que los sacerdotes lo coman — Lv 24:9);
 * actualiza shabat_ultimo_refresco_ms. Pacto eterno (berít ʻolám):
 * esta función se llama periódicamente por el driver del ciclo.
 */
int  hashmal_cache_shabat_renovar(void);

/* ── Integridad (v.24 — zer zaháv savív) ──────────────── */

/**
 * Verifica el zer_slot de un slot (per-slot corona). Si el
 * contenido fue tocado entre oro y oro → ERR_ZER_ROTO.
 */
int  hashmal_cache_zer_verificar(uint8_t slot);

/* ── Estado / telemetría ──────────────────────────────── */

/**
 * v.30 — ¿léhem paním lefanái tamíd? Devuelve true si el flag
 * tamid_activo está encendido Y al menos un slot está FRESCO
 * (la mesa no está totalmente vacía).
 */
bool hashmal_cache_tamid_activo(void);

/**
 * Cuenta slots en estado FRESCO. Útil para health checks y
 * telemetría de disponibilidad del caché.
 */
uint32_t hashmal_cache_slots_frescos(void);

/**
 * P9 — Éx 25:23 + v.30 — qomáh vakhetsí: estado de completud de la
 * Mesa. Distingue:
 *
 *   NO_INIT     — antes de hashmal_cache_init()
 *   LEVANTADO   — estructura inicializada pero NINGÚN slot tiene
 *                 dato válido (qomá sin resolver). Mesa lista, sin pan.
 *   COMPLETO    — ≥ 1 slot en FRESCO o SERVIDO (qomá resuelta).
 *                 v.30 cumplido para este nodo: léjem encima.
 *
 * Funcion pura (no muta). El healthcheck (pieza 12, Lavacro) debe
 * usar este getter para reportar mesa LEVANTADA pero todavía sin
 * pan — no es fallo, pero no es léjem paním tamíd (v.30) tampoco.
 */
hashmal_cache_completud_t hashmal_cache_completud(void);

/* ── Varas (v.26-28, sin "lo yasúru") ──────────────────── */

/**
 * Pone las varas — habilita transporte del caché entre nodos.
 * A diferencia del Arca (v.15 "lo yasúru"), en la Mesa las varas
 * PUEDEN ponerse y quitarse.
 */
int  hashmal_cache_varas_poner(void);

/**
 * Quita las varas — deshabilita el transporte. Idempotente.
 */
int  hashmal_cache_varas_quitar(void);

/* ══════════════════════════════════════════════════════════════════
 * MOEDIM - Tiempos Senalados (Añadidura 03)
 *
 * "eleh MOADEI YHWH miqra'ei qodesh asher tiqre'u otam bemoadam."
 *   - Levitico 23:4
 * "Estos son los TIEMPOS SENALADOS de YHWH... los proclamareis en
 *  SUS TIEMPOS."
 *
 * "veayu le'OTOT uleMOADIM ulyamim veshanim." - Genesis 1:14
 * "Seran para SENALES y para TIEMPOS SENALADOS..."
 *
 * Eje paralelo al ciclo ya'arejennu (Lv 24:8, mesa_disponibilidad.c):
 * el ciclo de renovacion del pan sigue intacto. Este motor agrega
 * el CALENDARIO de moedim de Lv 23 + Lv 25 sin tocar aquello.
 *
 * Capa: publicacion pura. El motor CALCULA moed actual y publica
 * comportamiento. Piezas consumidoras (08 aceite, etc.) CONSULTAN
 * por pull - nadie esta obligado a reaccionar.
 * ══════════════════════════════════════════════════════════════════ */

/**
 * Los 7 tiempos senalados (mo'adim) reconocidos en fase 1:
 *
 *   NINGUNO     - dia ordinario
 *   SHABAT      - Lv 23:3 - dia 7 de la semana (descanso)
 *   PESAJ       - Lv 23:5 - 14 Nisan (verificacion integridad)
 *   SHAVUOT     - Lv 23:15-16 - 50 dias post-Pesaj (renovacion)
 *   YOM_TERUAH  - Lv 23:24 - 1 Tishri (alerta, alta vigilancia)
 *   YOM_KIPPUR  - Lv 23:27 - 10 Tishri (sincronizacion total)
 *   SUKKOT      - Lv 23:34 - 15-21 Tishri (modo comunidad)
 *   JUBILEO     - Lv 25:9 - cada 50 anos desde epoch (shofar, reset)
 */
typedef enum {
    HASHMAL_MOED_NINGUNO    = 0,
    HASHMAL_MOED_SHABAT     = 1,   /* Lv 23:3 */
    HASHMAL_MOED_PESAJ      = 2,   /* Lv 23:5 */
    HASHMAL_MOED_SHAVUOT    = 3,   /* Lv 23:15-16 */
    HASHMAL_MOED_YOM_TERUAH = 4,   /* Lv 23:24 */
    HASHMAL_MOED_YOM_KIPPUR = 5,   /* Lv 23:27 */
    HASHMAL_MOED_SUKKOT     = 6,   /* Lv 23:34 */
    HASHMAL_MOED_JUBILEO    = 7,   /* Lv 25:9 - 50 anos */
} hashmal_moed_t;

/**
 * Bitmap de comportamientos que las piezas consumidoras pueden
 * honrar. Combinables con OR - un mo'ed puede activar varios
 * simultaneos (Yom Kippur = SINC_TOTAL + LIMPIEZA_PROFUNDA +
 * BAJO_CONSUMO, etc.).
 */
typedef enum {
    HASHMAL_COMPORT_NINGUNO           = 0u,
    HASHMAL_COMPORT_BAJO_CONSUMO      = 1u << 0,  /* Shabat, Yom Kippur */
    HASHMAL_COMPORT_SOLO_HEARTBEAT    = 1u << 1,  /* Shabat */
    HASHMAL_COMPORT_SINC_TOTAL        = 1u << 2,  /* Yom Kippur */
    HASHMAL_COMPORT_LIMPIEZA_PROFUNDA = 1u << 3,  /* Yom Kippur */
    HASHMAL_COMPORT_HANDSHAKE         = 1u << 4,  /* Shavuot */
    HASHMAL_COMPORT_INTEGRIDAD        = 1u << 5,  /* Pesaj */
    HASHMAL_COMPORT_ALTA_VIGILANCIA   = 1u << 6,  /* Yom Teruah */
    HASHMAL_COMPORT_MODO_COMUNIDAD    = 1u << 7,  /* Sukkot */
    HASHMAL_COMPORT_RESET_DEUDA       = 1u << 8,  /* Jubileo */
} hashmal_comportamiento_t;

/**
 * Entrada al motor: wall-clock + ubicacion. El atardecer se calcula
 * con Meeus; lat_milideg/lon_milideg defaultean a 0/0 (ecuador,
 * sunset ~18h UTC), funcionando aun sin GPS. Cuando haya GPS, el
 * integrador pasa lat/lon reales y tz_minutes correcto.
 *
 * year, month, day son GREGORIANOS. El motor convierte internamente
 * a dia biblico (sunset-to-sunset) y a fecha hebrea aproximada.
 */
typedef struct hashmal_moedim_tiempo {
    uint16_t year;          /* Gregoriano, p.ej. 2026 */
    uint8_t  month;         /* 1-12 */
    uint8_t  day;           /* 1-31 */
    uint8_t  hour;          /* 0-23 - hora local */
    uint8_t  minute;        /* 0-59 */
    int32_t  lat_milideg;   /* lat * 1000; Jerusalen ~31768 */
    int32_t  lon_milideg;   /* lon * 1000; Jerusalen ~35214 */
    int16_t  tz_minutes;    /* offset UTC (min). Por defecto 0. */
} hashmal_moedim_tiempo_t;

/**
 * Jubileo: 50 anos desde epoch (Lv 25:9). El epoch es el ano de
 * comisionado del Mishkan digital - configurable al boot. Defecto
 * si nadie lo provee.
 */
#define HASHMAL_JUBILEO_ANIOS   50u
#define HASHMAL_JUBILEO_EPOCH   2024u

/* -- mesa_moedim.c - motor de calendario ───────────────── */

/**
 * Gn 1:14 / Lv 23:4 - calcula el mo'ed actual. Input: tiempo +
 * ubicacion. Aplica Meeus para atardecer; determina dia biblico;
 * mapea a uno de los 7 mo'adim (o NINGUNO). Persiste el resultado
 * internamente para consumo por _actual() / _comportamiento().
 *
 * @return HASHMAL_CACHE_OK si exito; ERR_NULL si t NULL.
 */
int hashmal_moedim_calcular(const hashmal_moedim_tiempo_t *t);

/**
 * Lectura pura: ultimo mo'ed calculado. NINGUNO si _calcular no se
 * ha llamado todavia.
 */
hashmal_moed_t hashmal_moedim_actual(void);

/**
 * Lectura pura: bitmap de comportamientos activos ahora. Las piezas
 * consumidoras (p.ej. Pieza 08 aceite_tamid) lo consultan cada
 * tick para honrar Shabat / Yom Kippur / etc. sin acoplarse al
 * calendario.
 */
uint32_t hashmal_moedim_comportamiento(void);

/* -- Meeus publico (Añadidura 04) ─────────────────────── */

/**
 * Genesis 1:14 ot ulmo'adim - las lumbreras para senales. Devuelve
 * la hora LOCAL (0..24) del atardecer para la fecha/ubicacion dada.
 * Salida -1.0 si region polar y el sol no se pone ese dia.
 *
 * @return HASHMAL_CACHE_OK si calculable; ERR_NULL si t/out NULL;
 *         ERR_INIT si region polar (out_hour_local = -1.0).
 */
int hashmal_moedim_sunset_hour(const hashmal_moedim_tiempo_t *t,
                                double *out_hour_local);

/**
 * Analogo para el amanecer. Misma semantica.
 */
int hashmal_moedim_sunrise_hour(const hashmal_moedim_tiempo_t *t,
                                 double *out_hour_local);

/* ══════════════════════════════════════════════════════════════════
 * MAN - Gestion de Recursos (Añadidura 11)
 *
 * "liqtu mimmennu ish lefi okhlo OMER LAGGULGOLET mispar nafshoteikhem
 *  ish la'asher beoholo tiqqaju." - Exodo 16:16
 * "Recoged segun su comer, OMER POR CABEZA segun el numero de
 *  vuestras almas: el hombre tomara para los que estan en su tienda."
 *
 * "vayyamoddu va'omer veLO HE'EDIF HAMMARBEH veHAMMAM'IT LO HEJSIR;
 *  ish lefi okhlo laqatu." - Exodo 16:18
 * "Lo midieron por omer: NO LE SOBRO al que recogio MUCHO, NI LE
 *  FALTO al que recogio POCO."
 *
 * "ish AL YOTER MIMMENNU 'ad boqer." - Exodo 16:19
 * "Que NADIE deje hasta la manana."
 *
 * "vayyarum tola'im VAYYIVASH." - Exodo 16:20
 * "Crio GUSANOS y APESTO" - lo acaparado se corrompe.
 *
 * "uvayyom hashishi laqtu lehem MISHNEH shenei ha'omer la'ejad."
 *   - Exodo 16:22
 * "El sexto dia recogieron pan DOBLE, dos omeres por persona"
 *  (vispera de Shabat).
 *
 * "shesheh yamim tilqetuhu uvayyom hashshvi'i SHABBAT lo yiheyeh
 *  vo." - Exodo 16:26
 * "SEIS dias recogereis, el SEPTIMO Shabat - no habra."
 *
 * "veyatsa ha'am velaqtu DEVAR YOM BEYOMO." - Exodo 16:4
 * "El pueblo saldra y recogera la PORCION DE CADA DIA en su dia."
 *
 * Capa: el modulo PUBLICA cuotas (omer por nodo por recurso) +
 * contadores de uso. Consumidores (Pieza 04 broadcast, Pieza 14
 * firewall, Pieza 03 misma para slots) consultan por pull antes
 * de cada operacion. Lo no usado al expirar se PURGA (Ex 16:19-20).
 * ══════════════════════════════════════════════════════════════════ */

/**
 * Los 4 recursos contabilizados (omer por cabeza, por recurso).
 */
typedef enum {
    HASHMAL_MAN_RECURSO_BROADCAST   = 0,  /* mensajes broadcast */
    HASHMAL_MAN_RECURSO_DIRECTO     = 1,  /* mensajes punto a punto */
    HASHMAL_MAN_RECURSO_BYTES_LORA  = 2,  /* bytes transmitidos LoRa */
    HASHMAL_MAN_RECURSO_SLOTS_CACHE = 3,  /* slots de cache reservados */
} hashmal_man_recurso_t;

#define HASHMAL_MAN_RECURSOS_TOTAL  4u

/**
 * Capacidad de cabezas (gulgolet). 12 paralelo a las 12 piedras
 * del Pectoral / 12 tribus - misma convencion que Miqlat /
 * Escalacion / Rally en Pieza 09.
 */
#define HASHMAL_MAN_NODOS_MAX       12u

/**
 * Omer por cabeza por recurso - defaults de fase 1. Iguales para
 * todos los nodos (Ex 16:18 - "no le sobro al de mucho, no le
 * falto al de poco").
 */
#define HASHMAL_MAN_OMER_BROADCAST    10u   /* mensajes/ciclo */
#define HASHMAL_MAN_OMER_DIRECTO     100u   /* mensajes/ciclo */
#define HASHMAL_MAN_OMER_BYTES_LORA 8192u   /* bytes/ciclo */
#define HASHMAL_MAN_OMER_SLOTS         4u   /* slots/ciclo */

/**
 * Snapshot del consumo por nodo. Los valores `usado` se resetean
 * a 0 en cada `_expirar` (Ex 16:19-20 - lo no consumido se purga,
 * no se acumula).
 */
typedef struct {
    uint8_t  nodo_id;
    uint32_t usado[HASHMAL_MAN_RECURSOS_TOTAL];
    uint32_t limite[HASHMAL_MAN_RECURSOS_TOTAL];
} hashmal_man_estado_t;

/* -- mesa_man.c - Añadidura 11 ─────────────────────────── */

/**
 * Ex 16:16 liqtu - intenta consumir `cantidad` del recurso para
 * el nodo. Si excederia el limite del ciclo actual, ERR_MAN_EXCEDIDO.
 *
 * Ex 16:18 - no le sobro al de mucho: si la peticion excede, NO se
 * consume parcial; o cabe entera o no cabe.
 */
int hashmal_man_consumir(uint8_t nodo_id,
                          hashmal_man_recurso_t recurso,
                          uint32_t cantidad);

/**
 * Lectura pura: cuanto va consumido y cual es el limite actual del
 * recurso para el nodo. Cualquiera de los out punteros puede ser
 * NULL si no interesa.
 */
int hashmal_man_cuota_consultar(uint8_t nodo_id,
                                 hashmal_man_recurso_t recurso,
                                 uint32_t *out_usado,
                                 uint32_t *out_limite);

/**
 * Ex 16:19-20 - cierra el ciclo. Reset todos los `usado` a 0 - lo
 * no consumido NO se acumula al proximo ciclo (cria gusanos). Si
 * el flag mishne esta activo: el limite del proximo ciclo se
 * duplica (Ex 16:22 lehem mishneh) y el flag se consume (one-shot).
 */
int hashmal_man_expirar(void);

/**
 * Ex 16:22 - prepara el doble del proximo ciclo (vispera de Shabat).
 * El flag se aplica EN el proximo `_expirar` y se consume en ese
 * acto - no permanece persistente. Si ya estaba activo, idempotente.
 */
int  hashmal_man_mishne_set(bool activar);
bool hashmal_man_mishne_activo(void);

/* ══════════════════════════════════════════════════════════════════
 * SHINNAN - Ensenanza Continua / Propagacion (Añadidura 17)
 *
 * "veSHINNANTAM levaneikha vediBARTA bam beSHIVTekha bevetekha
 *  uveLEKHTekha vadderekh uveSHOJBekha uveQUMekha." - Deuteronomio 6:7
 * "Las REPETIRAS a tus hijos y hablaras de ellas SENTADO en tu
 *  casa, ANDANDO por el camino, ACOSTADO y LEVANTADO."
 *
 * "uqshartam le'ot 'al yadekha vehayu letotafot ben 'eineikha."
 *   - Deuteronomio 6:8
 * "Las ATARAS como SENAL en tu mano y como FRONTALES entre tus ojos."
 *
 * "ukhtavtam 'al MEZUZOT beitekha uvish'arekha." - Deuteronomio 6:9
 * "Las ESCRIBIRAS en los POSTES de tu casa y en tus PUERTAS."
 *
 * "vayyaqem 'edut beYa'aqov... asher tsivvah et avoteinu lehodi'am
 *  livneihem; lema'an YEDE'U DOR AJARON banim yivvaledu yaqumu
 *  visapperu LIVNEIHEM." - Salmo 78:5-6
 * "Establecio TESTIMONIO en Yaakov... mando a nuestros padres
 *  hacerlo SABER a sus hijos, para que lo SEPA la GENERACION
 *  VENIDERA, los hijos que naceran, y lo cuenten a SUS hijos."
 *
 * "JANOKH lanna'ar 'al pi DARKO." - Proverbios 22:6
 * "INSTRUYE al joven SEGUN SU CAMINO." (ensenanza adaptada).
 *
 * Modelo: 4 estados (Dt 6:7) que ningun estado APAGA la propagacion -
 * varia la intensidad. El receptor se categoriza por capacidad y
 * antiguedad; el emisor decide QUE enviarle (Prov 22:6 'al pi darko).
 *
 * Capa: decision + contabilidad fase 1. La transmision real (wiring
 * con comm) es fase 2.
 * ══════════════════════════════════════════════════════════════════ */

#define HASHMAL_SHINNAN_ESTADOS  4u   /* Dt 6:7 yoshev/holekh/shojev/qam */

/**
 * 4 estados de propagacion (Dt 6:7) - ningun estado apaga, varia
 * la intensidad.
 *
 *   YOSHEV - sentado en casa, sync profunda
 *   HOLEJ  - andando, store-and-forward
 *   SHOJEV - acostado, low power, propagacion minima
 *   QAM    - levantado, recien arranco, pide delta a vecinos
 */
typedef enum {
    HASHMAL_SHINNAN_YOSHEV = 0,   /* Dt 6:7 beshivtejá */
    HASHMAL_SHINNAN_HOLEJ  = 1,   /* Dt 6:7 uvelekhtejá vadérej */
    HASHMAL_SHINNAN_SHOJEV = 2,   /* Dt 6:7 uveshojbejá */
    HASHMAL_SHINNAN_QAM    = 3,   /* Dt 6:7 uvquméja */
} hashmal_shinnan_estado_t;

/**
 * Capacidad de memoria del receptor.
 */
typedef enum {
    HASHMAL_SHINNAN_CAP_BAJA  = 0,
    HASHMAL_SHINNAN_CAP_MEDIA = 1,
    HASHMAL_SHINNAN_CAP_ALTA  = 2,
} hashmal_shinnan_capacidad_t;

/**
 * Antiguedad del receptor en la red.
 */
typedef enum {
    HASHMAL_SHINNAN_ANT_NUEVO    = 0,
    HASHMAL_SHINNAN_ANT_ACTIVO   = 1,
    HASHMAL_SHINNAN_ANT_VETERANO = 2,
} hashmal_shinnan_antiguedad_t;

/**
 * Que propagar al receptor (Prov 22:6 'al pi darko - segun camino).
 *
 *   ESENCIAL - solo lo critico (NUEVOs con poca capacidad)
 *   COMPLETO - todo (NUEVO con ALTA capacidad - aprovecha el espacio)
 *   DELTA    - solo lo nuevo desde su ultima sync (ACTIVO/VETERANO
 *              ya conocen la base)
 */
typedef enum {
    HASHMAL_SHINNAN_PRIORIDAD_ESENCIAL = 0,
    HASHMAL_SHINNAN_PRIORIDAD_COMPLETO = 1,
    HASHMAL_SHINNAN_PRIORIDAD_DELTA    = 2,
} hashmal_shinnan_prioridad_t;

/**
 * Perfil del receptor para decidir que enviarle.
 */
typedef struct {
    hashmal_shinnan_capacidad_t  capacidad;
    hashmal_shinnan_antiguedad_t antiguedad;
} hashmal_shinnan_perfil_t;

/**
 * Snapshot del estado del modulo.
 */
typedef struct {
    hashmal_shinnan_estado_t  estado;
    uint32_t                  generacion;            /* veces propagado */
    int64_t                   ultimo_propagacion_ms;
    uint32_t                  total_propagados;
} hashmal_shinnan_info_t;

/* -- mesa_shinnan.c - Añadidura 17 ─────────────────────── */

/**
 * Dt 6:7 - cambia el estado del propagador. Caller decide cuando
 * (en fase 2 puede ligarse a Ashmurot / movimiento). Idempotente.
 */
int hashmal_shinnan_estado_set(hashmal_shinnan_estado_t estado);

/**
 * Prov 22:6 'al pi darko - decide QUE propagar al receptor segun
 * su perfil (capacidad + antiguedad). Funcion pura.
 *
 * Matriz (capacidad x antiguedad):
 *
 *   capacidad \ antiguedad | NUEVO    | ACTIVO | VETERANO
 *   BAJA                   | ESENCIAL | ESENCIAL | DELTA
 *   MEDIA                  | ESENCIAL | DELTA    | DELTA
 *   ALTA                   | COMPLETO | DELTA    | DELTA
 */
hashmal_shinnan_prioridad_t
hashmal_shinnan_que_propagar(const hashmal_shinnan_perfil_t *perfil);

/**
 * Dt 6:7 veshinantam - registra un acto de propagacion. Incrementa
 * generacion + total + actualiza timestamp. Fase 1 = solo
 * contabilidad + log. Fase 2 = wirea con comm para envio real.
 *
 * @return OK; ERR_NULL si perfil NULL.
 */
int hashmal_shinnan_propagar(const hashmal_shinnan_perfil_t *perfil);

/**
 * Lectura pura: snapshot completo (estado + contadores + timestamps).
 */
int hashmal_shinnan_info(hashmal_shinnan_info_t *out);

/* ══════════════════════════════════════════════════════════════════
 * LEKET - Espigueo - Reserva para los Debiles (Añadidura 18)
 *
 * "uvqutsrkhem et qetsir artsekhem LO TEKHALLEH PE'AT sadekha
 *  liqtsor veLEQET qetsirkha LO TELAQQET. ukharmkha lo te'olel
 *  ufret karmkha lo telaqqet; LE'ANI VELAGGER ta'azov otam, ani
 *  YHWH Eloheikhem." - Levitico 19:9-10
 * "Cuando segueis vuestra cosecha, NO acabareis de segar el RINCON
 *  de tu campo, ni el ESPIGUEO de tu siega recogeras... Para el
 *  POBRE y el EXTRANJERO los DEJARAS, yo soy YHWH vuestro Dios."
 *
 * "ki tiqtsor qetsirkha vesadekha veshakhajta 'omer bassadeh lo
 *  tashuv leqajto LAGGER LAYYATOM VELA'ALMANAH yiheyeh." - Dt 24:19
 * "Si segares tu mies en tu campo y olvidares un MANOJO, no
 *  volveras por el; sera para el EXTRANJERO, el HUERFANO y la VIUDA."
 *
 * "vatomer Rut hammoaviyah el Naomi: ELKHAH NA HASSADEH VAALAQATAH
 *  VASHIBBOLIM ajar asher emtsa jen be'einav." - Rut 2:2
 * "Rut la moabita dijo a Naomi: TE RUEGO, IRE AL CAMPO Y ESPIGARE
 *  ENTRE LAS ESPIGAS de aquel a cuyos ojos hallare gracia."
 *
 * "vegam SHOL TASHOLLU lah min HATSEVATIM va'azavtem velikketah
 *  velo tig'aru bah." - Rut 2:16
 * "Y aun DEJAD CAER A PROPOSITO para ella algo de los MANOJOS,
 *  y dejadlo para que ella lo recoja, y no la riniais."
 *
 * Modelo: complementa a Man (Añadidura 11). Man = cuota justa por
 * cabeza para TODOS. Leket = reserva ADICIONAL para los debiles.
 * No se contradicen - se suman. La equidad de Man es la base; la
 * misericordia de Leket es la abundancia que sobra.
 *
 * Capa: contabilidad + decision fase 1. La reserva real de ancho de
 * banda es wiring fase 2 con comm.
 * ══════════════════════════════════════════════════════════════════ */

#define HASHMAL_LEKET_PEAH_DEFAULT  10u   /* 10% reservado por defecto */
#define HASHMAL_LEKET_TIPOS          4u   /* ani / ger / yatom / almanah */
#define HASHMAL_LEKET_FORMAS         3u   /* peah / leqet / shijejah */

/**
 * 4 tipos de vulnerabilidad (Lv 19:10 + Dt 24:19).
 *
 *   ANI      - pobre: recursos limitados (bateria, memoria, senal)
 *   GER      - extranjero: recien llegado, sin vecinos establecidos
 *   YATOM    - huerfano: sin goel que lo redima (Añadidura 12)
 *   ALMANAH  - viuda: perdio conexion principal (< 3 vecinos)
 */
typedef enum {
    HASHMAL_LEKET_TIPO_ANI     = 0,  /* Lv 19:10 'ani */
    HASHMAL_LEKET_TIPO_GER     = 1,  /* Lv 19:10 ger */
    HASHMAL_LEKET_TIPO_YATOM   = 2,  /* Dt 24:19 yatom */
    HASHMAL_LEKET_TIPO_ALMANAH = 3,  /* Dt 24:19 almanah */
} hashmal_leket_tipo_t;

/**
 * 3 formas de espigueo (Lv 19 + Dt 24).
 *
 *   PEAH     - rincon del campo: porcentaje fijo reservado
 *   LEQET    - espigas caidas: paquetes sin destino disponibles
 *   SHIJEJAH - manojo olvidado: datos no reclamados
 */
typedef enum {
    HASHMAL_LEKET_FORMA_PEAH     = 0,  /* Lv 19:9 pe'ah */
    HASHMAL_LEKET_FORMA_LEQET    = 1,  /* Lv 19:9 leqet */
    HASHMAL_LEKET_FORMA_SHIJEJAH = 2,  /* Dt 24:19 shijejah */
} hashmal_leket_forma_t;

/**
 * Snapshot del estado del modulo.
 */
typedef struct {
    uint8_t  peah_porciento;       /* % reservado del campo */
    uint32_t recogidos;            /* veces que un debil recogio */
    uint32_t facilitados;          /* veces que un fuerte facilito (Boaz) */
    int64_t  timestamp_ultimo_ms;  /* ultimo evento */
} hashmal_leket_info_t;

/* -- mesa_leket.c - Añadidura 18 ───────────────────────── */

/**
 * Lectura pura: el tipo es valido (i.e. el caller declaro un nodo
 * como debil). Fase 1: siempre true para tipos validos. Fase 2
 * podra cruzar con Meshulash/Goel/onboarding/power para confirmar.
 */
bool hashmal_leket_es_debil(hashmal_leket_tipo_t tipo);

/**
 * Rut 2:2 - un nodo debil recoge espigas. Incrementa contador
 * historico + timestamp.
 *
 * @return OK; ERR_LEKET si tipo invalido.
 */
int hashmal_leket_recoger(hashmal_leket_tipo_t tipo);

/**
 * Rut 2:16 - un nodo fuerte facilita activamente acceso a un
 * debil (shol tasholu). Incrementa contador de facilitados.
 *
 * @return OK; ERR_LEKET si tipo invalido.
 */
int hashmal_leket_facilitar(hashmal_leket_tipo_t tipo);

/**
 * Lectura pura: porcentaje reservado de pe'ah (default 10%).
 */
uint8_t hashmal_leket_peah_get(void);

/**
 * Lectura pura: snapshot completo (porcentaje + contadores).
 */
int hashmal_leket_info(hashmal_leket_info_t *out);

/* ══════════════════════════════════════════════════════════════════
 * MA'ASER - El Diezmo - Contribucion a la Red (Añadidura 19)
 *
 * "haviu et kol HAMMA'ASER el BEIT HAOTSAR vihi teref beveiti
 *  uvjanuni na bazot amar YHWH tseva'ot im lo eftaj lakhem et
 *  arubot hashshamayim vahariqoti lakhem berakhah 'ad bli day."
 *   - Malaquias 3:10
 * "Traed TODO el DIEZMO al ALFOLI y haya alimento en mi Casa;
 *  probadme en esto, dice YHWH de los ejercitos, si no os abrire
 *  las ventanas de los cielos y derramare bendicion hasta que
 *  sobreabunde."
 *
 * "velivnei Levi hinneh natatti kol MA'ASER beYisrael lenajalah
 *  jelef 'avodatam asher hem 'ovdim et 'avodat ohel mo'ed."
 *   - Numeros 18:21
 * "A los Levitas he dado todo DIEZMO en Israel por heredad, en
 *  reemplazo de su servicio en el Ohel Mo'ed."
 *
 * "vaharemotem mimmennu TERUMAT YHWH MA'ASER MIN HAMMA'ASER."
 *   - Numeros 18:26
 * "Ofrecereis ofrenda a YHWH: DIEZMO DEL DIEZMO." (los que
 *  reciben tambien dan; nadie esta exento)
 *
 * "umalki-tsedeq melekh shalem hotsi lejem vayayin... vayitten
 *  lo MA'ASER MIKKOL." - Genesis 14:18,20
 * "Malki-Tsedeq rey de Salem saco pan y vino... y le DIO DIEZMO
 *  DE TODO." (Abraham; principio anterior a la Ley)
 *
 * "kabbed et YHWH MEHONEKHA umeRESHIT kol tevu'atekha." - Pr 3:9
 * "Honra a YHWH de tus BIENES y de las PRIMICIAS de todos tus
 *  frutos." (LO PRIMERO, no lo sobrante)
 *
 * Modelo: complementa a Man y Leket. Tres dimensiones de la
 * economia del Mishkan:
 *   Man    (Ex 16):  cuota justa que RECIBES.
 *   Leket  (Lv 19):  reserva que DEJAS para los debiles.
 *   Ma'aser (Nm 18): porcion que DAS a la red.
 *
 * Capa: contabilidad pura fase 1. La transferencia real (relay,
 * almacen, etc.) es wiring fase 2 con comm + storage.
 * ══════════════════════════════════════════════════════════════════ */

#define HASHMAL_MAASER_PORCIENTO_DEFAULT  10u   /* 10% = diezmo */
#define HASHMAL_MAASER_TIPOS               4u   /* relay/almacen/repeticion/proc */

/**
 * 4 formas de contribucion a la red.
 *
 *   RELAY         - retransmitir mensajes ajenos
 *   ALMACEN       - guardar datos comunitarios (Mal 3:10 beit haotsar)
 *   REPETICION    - amplificar senal para nodos lejanos
 *   PROCESAMIENTO - verificar mensajes / validar nodos / quorum
 */
typedef enum {
    HASHMAL_MAASER_TIPO_RELAY         = 0,
    HASHMAL_MAASER_TIPO_ALMACEN       = 1,  /* Mal 3:10 beit haotsar */
    HASHMAL_MAASER_TIPO_REPETICION    = 2,
    HASHMAL_MAASER_TIPO_PROCESAMIENTO = 3,
} hashmal_maaser_tipo_t;

/**
 * Snapshot del estado del modulo.
 */
typedef struct {
    uint8_t  porciento;                              /* % dedicado a la red */
    uint32_t contribuciones[HASHMAL_MAASER_TIPOS];   /* contadores por tipo */
    uint32_t total;                                  /* suma de todos los tipos */
    bool     primicias;                              /* contribuye antes de uso propio */
    int64_t  timestamp_ultima_ms;
} hashmal_maaser_info_t;

/* -- mesa_maaser.c - Añadidura 19 ──────────────────────── */

/**
 * Gn 14:20 vayyitten lo ma'aser mikkol - registra una contribucion
 * del tipo indicado. Incrementa contador especifico + total +
 * timestamp.
 *
 * @return OK; ERR_MAASER si tipo invalido.
 */
int hashmal_maaser_contribuir(hashmal_maaser_tipo_t tipo);

/**
 * Nm 18:26 ma'aser min hamma'aser - el que recibe tambien da. Los
 * nodos de servicio (repetidores, validadores) tambien contribuyen
 * al nivel superior. Incrementa contadores como una contribucion
 * normal pero con LOG distintivo "ma'aser min hamma'aser".
 *
 * @return OK; ERR_MAASER si tipo invalido.
 */
int hashmal_maaser_diezmo_del_diezmo(hashmal_maaser_tipo_t tipo);

/**
 * Pr 3:9 mereshit - lectura del flag de primicias. True por
 * defecto: la contribucion va antes del uso propio. Fase 2 puede
 * exponer setter cuando el caller lo necesite.
 */
bool hashmal_maaser_primicias_get(void);

/**
 * Lectura pura: snapshot completo del estado del modulo.
 */
int hashmal_maaser_info(hashmal_maaser_info_t *out);

/* ══════════════════════════════════════════════════════════════════
 * SHEMITAH - Acceso Abierto cada 7 Épocas (Añadidura 14, P32)
 *
 * "vehayetah shabat ha'arets LAJÉM LE'OHLAH" - Levitico 25:6
 * "La tierra reposara para ustedes ABIERTA para COMER."
 *
 * Durante la Shemitah (cada 7ª época), el acceso se abre a TODOS.
 * No solo se refrescan los slots (Lv 24:8, já'arejennu regular),
 * sino que la restricción de permisos se LEVANTA temporalmente.
 * Lv 25:7 extiende incluso a los animales salvajes.
 *
 * Éx 40:3 (después de Arca/Kapóret) coloca la Mesa en paso 3;
 * Lv 25:6 abre las compuertas durante Shemitah. El acceso se
 * recupera al terminar la época (burí = podrá recoger de nuevo
 * a la época siguiente).
 *
 * @return true si la época actual ES Shemitah (mod 7 == 0);
 *         false en caso contrario.
 *
 * Ref: Lv 25:6 shabát ha'árets lejém leʼojláh
 */
bool hashmal_cache_es_shemitah(void);

/**
 * Lv 25:6 - abre TEMPORALMENTE todos los slots a cualquier nodo.
 * Remueve restricciones de acceso durante la época Shemitah.
 *
 * Postcondición:
 *   - Todos los slots FRESCO/SERVIDO pueden ser leídos por cualquier nodo
 *   - No hay verificación de autorización en _distribuir/_kaf_tomar
 *   - Al expirar la Shemitah, el acceso vuelve a estar restringido
 *
 * @return OK si la apertura se ejecuta;
 *         ERR_FUERA_SHABAT si no estamos en Shemitah;
 *         ERR_INIT si caché no inicializado.
 *
 * Ref: Lv 25:6 leʼojláh (para comer — acceso público)
 */
int hashmal_cache_shemitah_abrir(void);

#ifdef __cplusplus
}
#endif

#endif /* HASHMAL_CACHE_H */
