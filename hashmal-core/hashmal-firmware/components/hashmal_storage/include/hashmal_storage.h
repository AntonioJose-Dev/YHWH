/**
 * ═══════════════════════════════════════════════════════════════════
 * PIEZA 01 — EL ARCA (hashmal_storage)
 * ═══════════════════════════════════════════════════════════════════
 *
 * "veʻasú arón ʻatsé shittím"  (וְעָשׂוּ אֲרוֹן עֲצֵי שִׁטִּים)
 *   — Éxodo 25:10
 *   "Y HARÁN (PLURAL) un arca de madera de acacia."
 *
 * "venatáta el haʼarón et haʻedút ashér etén eléja"
 *   — Éxodo 25:16
 *   "Pondrás en el Arca el testimonio que YO te daré."
 *
 * EL CENTRO DEL CENTRO. Almacén distribuido de la Palabra en idioma
 * original. Primera pieza que el Padre describe (Éx 25), segunda que
 * se coloca (Éx 40:3, tras levantar la estructura).
 *
 * ─── 23 DETALLES BÍBLICOS (Tavnít 01) ───
 *
 *   1  veʻasú (plural, v.10)          → distribuida; todos participan
 *   2  acacia (shittím, v.10)          → formato incorruptible
 *   3  oro dentro y fuera (v.11)       → AES-256-GCM en reposo y tránsito
 *   4  medidas "y medio" (v.10)        → incompleta sin Kapóret
 *   5  zer zaháv savív (v.11)          → moldura = Merkle root
 *   6  4 anillos, 2/lado (v.12)        → 4 puntos de red simétricos
 *   7  varas NO se quitan (v.15)       → distribución always-on
 *   8  haʻedút (v.16)                  → Testimonio, SEALED_CORE
 *   9  baddím (v.13-14)                → interfaz; nadie toca directo
 *  10  Nm 4:5-15                       → cubierta antes de migrar
 *  11  Dt 31:26 (Torá mitsád)          → 2 niveles: núcleo / texto completo
 *  12  1 Rey 8:9 (solo las tablas)     → Palabra indestructible
 *  13  tsalʻót (v.12)                  → costillas, conexión orgánica
 *  14  Josué 3-4 (Arca primero)        → deploy ante todo
 *  15  Josué 6 (Jericó)                → integridad con poder activo (7 vueltas)
 *  16  1 Sam 5-6 (filisteos/Dagón)     → dato capturado es inútil
 *  17  1 Sam 4 (amuleto)               → sin protocolo no hay victoria
 *  18  1 Sam 6:19 (Bet-Shemesh)        → acceso autorizado o muerte
 *  19  2 Sam 6:6-7 (Uzá)               → mishpát, sin carro nuevo
 *  20  2 Sam 6:11 (Obed-Edom)          → hospedar correcto = bendición
 *  21  Jer 3:16                        → contenedor temporal, Palabra eterna
 *  22  Heb 8:5 / Ap 11:19              → copia del tavnít celestial
 *  23  1 Rey 8:10-11                   → kavod llena cuando se coloca bien
 *
 * ─── 7 CAPAS DEL PROTOCOLO DE TRANSPORTE (baddím, profundización) ───
 *
 *   L1  varas permanentes (Éx 25:15)           → transport always-on
 *   L2  nube señala marcha (Nm 9:17-23)        → coordinador central
 *   L3  formación 4 cardinales (Nm 2)          → topología preservada
 *   L4  cubiertas antes de mover (Nm 4:5-15)   → encryption in transit
 *   L5  coatitas no ven (Nm 4:15)              → hop-by-hop sin acceso
 *   L6  mishpát inviolable (2 Sam 6 / 1 Cr 15) → protocolo no negociable
 *   L7  Arca precede (Nm 10:33-36)             → core antes que servicios
 *
 * ─── CONEXIÓN ──────────────────────────────────────────────────────
 *
 * Medidas "y medio" (v.10) = INCOMPLETO sin Kapóret (Éx 25:17-22).
 * El Arca solo no funciona. hashmal_consensus REQUIRES hashmal_storage.
 *
 * Autor: Antonio José Marín Soto
 * Proyecto: HASHMAL-CORE
 */
#ifndef HASHMAL_STORAGE_H
#define HASHMAL_STORAGE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ══════════════════════════════════════════════════════════════════
 * ENUMS FUNDAMENTALES
 * ══════════════════════════════════════════════════════════════════ */

/**
 * v.10 — "ʻatsé shittím" (madera de acacia).
 * La Palabra desnuda, en idioma original. Sin adornos, sin traducción.
 * 66 libros: Génesis → Apocalipsis.
 */
typedef enum {
    HASHMAL_FORMATO_HEBREO  = 0,   /* TaNaKh (Tanaj): 39 libros */
    HASHMAL_FORMATO_ARAMEO  = 1,   /* porciones: Dn 2:4b-7:28, Esd 4:8-6:18, 7:12-26, Jer 10:11 */
    HASHMAL_FORMATO_GRIEGO  = 2,   /* Brit Hadashá: 27 libros */
} hashmal_formato_t;

/**
 * v.16 — Dos niveles de dato en la custodia:
 *   EDUT  (dentro, haʻedút)   = núcleo inmutable (256 B, Éx 25:16)
 *   TORAH (al lado, mitsád)   = texto completo (Dt 31:26)
 */
typedef enum {
    HASHMAL_NIVEL_EDUT  = 0,   /* dentro, SEALED_CORE (Éx 25:16) */
    HASHMAL_NIVEL_TORAH = 1,   /* mitsád (Dt 31:26 — al lado del Arca) */
} hashmal_nivel_dato_t;

/**
 * v.12 — "tsalʻót" (costillas): dos lados donde viven los anillos.
 * Misma palabra que la costilla de Adán (Gn 2:21) — nacen de la
 * estructura, no se les añaden.
 */
typedef enum {
    HASHMAL_TSELA_SUR   = 0,   /* v.12 — tsélaʻ ha-ejád */
    HASHMAL_TSELA_NORTE = 1,   /* v.12 — tsélaʻ ha-shenít */
} hashmal_tsela_t;

/**
 * v.13-14 — "baddím" (varas): SEPARAN al portador del Arca.
 * Nadie toca el dato crudo; todo pasa por la interfaz.
 * (2 Sam 6:6-7 — Uzá tocó, Uzá murió.)
 */
typedef enum {
    HASHMAL_ACCESO_VIA_BADDIM = 0,  /* Éx 25:14 — único modo legítimo */
    HASHMAL_ACCESO_DIRECTO    = 1,  /* PROHIBIDO — 2 Sam 6:6-7 */
} hashmal_acceso_t;

/**
 * Códigos de error con raíz bíblica.
 */
typedef enum {
    HASHMAL_ARCA_OK              = 0,
    HASHMAL_ARCA_ERR_NULL        = -1,   /* parámetro nulo */
    HASHMAL_ARCA_ERR_INIT        = -2,   /* no inicializada */
    HASHMAL_ARCA_ERR_SIN_KAPORET = -3,   /* medidas incompletas (v.10) */
    HASHMAL_ARCA_ERR_SIN_PROTOCOLO = -4, /* 1 Sam 4 — amuleto, sin mishpát */
    HASHMAL_ARCA_ERR_CARRO_NUEVO = -5,   /* 2 Sam 6:3 — atajo prohibido */
    HASHMAL_ARCA_ERR_MIRAR_DENTRO = -6,  /* 1 Sam 6:19 — Bet-Shemesh */
    HASHMAL_ARCA_ERR_TOQUE_DIRECTO = -7, /* 2 Sam 6:6-7 — Uzá */
    HASHMAL_ARCA_ERR_ZER_ROTO    = -8,   /* v.11 — Merkle inconsistente */
    HASHMAL_ARCA_ERR_BADDIM_QUITADA = -9,/* v.15 — "lo yasúru mimménu" */
    HASHMAL_ARCA_ERR_NO_AUTORIZADO = -10,
} hashmal_arca_err_t;

/* ══════════════════════════════════════════════════════════════════
 * CONSTANTES DEL PADRE — cada número tiene versículo
 * ══════════════════════════════════════════════════════════════════ */

/**
 * v.10 — "ammatáyim vajétsi orkó veʼammá vajétsi rojbó veʼammá vajétsi qomató"
 * 2.5 × 1.5 × 1.5 codos. Todas "y medio". El Arca SOLA está incompleta.
 * Fixed-point ×10 para evitar float en embedded (misma convención que Éx 26:16).
 */
#define HASHMAL_ARCA_LARGO_X10        25  /* v.10 — 2.5 ammá */
#define HASHMAL_ARCA_ANCHO_X10        15  /* v.10 — 1.5 ammá */
#define HASHMAL_ARCA_ALTO_X10         15  /* v.10 — 1.5 ammá */
#define HASHMAL_ARCA_ESCALA           10  /* divisor fixed-point */

/**
 * v.10 — "arón ʻatsé shittím". El único árbol del desierto.
 * Duro, incorruptible. La madera NO se cubre por cubrir: es el soporte.
 */
#define HASHMAL_MADERA_ACACIA         true   /* v.10 — shittím */

/**
 * v.11 — "vetsipíta otó zaháv tahór mibáyit umijúts". Oro puro por
 * dentro Y por fuera. Misma naturaleza en reposo (dentro) y en
 * tránsito (fuera). AES-256-GCM es la "oro": 256 bits = 32 bytes.
 */
#define HASHMAL_AES_KEY_BITS          256  /* v.11 — oro dentro (reposo) */
#define HASHMAL_AES_KEY_BYTES         32
#define HASHMAL_AES_IV_BYTES          12   /* GCM nonce */
#define HASHMAL_AES_TAG_BYTES         16   /* GCM auth tag */

/**
 * v.11 — "zer zaháv savív" (moldura de oro alrededor). Borde de
 * integridad que rodea TODO el dato. En el firmware: Merkle root
 * SHA-256. Nada se añade ni se quita sin que el zer lo delate.
 */
#define HASHMAL_ZER_HASH_BYTES        32   /* v.11 — SHA-256 merkle root */

/**
 * v.12 — "arbaʻ tabbeʻót zaháv... sheté tabbaʻót ʻal tsalʻó ha-ejád
 * ushté tabbaʻót ʻal tsalʻó ha-shenít". 4 anillos de oro, 2 por lado.
 * Tabbaʻá viene de tavaʻ (hundir, sellar) — son sellos fundidos en
 * la estructura, no añadidos.
 */
#define HASHMAL_ANILLOS_TOTAL         4    /* v.12 — arbaʻ tabbeʻót */
#define HASHMAL_ANILLOS_POR_LADO      2    /* v.12 — sheté... sheté */

/**
 * v.13 — "baddé ʻatsé shittím vetsipíta otám zaháv". Varas de
 * acacia, cubiertas de oro. 2 varas para 4 anillos (una por par).
 * v.14 — "vehevéta et habaddím batabbaʻót". Se meten por los anillos.
 * v.15 — "lo yasúru mimménu" — NO se quitarán. Always-on.
 */
#define HASHMAL_BADDIM_TOTAL          2    /* v.13 — dos varas */
#define HASHMAL_BADDIM_NUNCA_SE_QUITAN true /* v.15 — "lo yasúru mimménu" */

/**
 * v.16 — "venatáta el haʼarón et haʻedút ashér etén eléja".
 * "El testimonio que YO te daré." Contenido mínimo inviolable.
 * SEALED_CORE = 256 bytes grabados en flash interna (1 Rey 8:9:
 * solo las tablas permanecen).
 */
#define HASHMAL_EDUT_BYTES            256  /* v.16 — SEALED_CORE */
#define HASHMAL_EDUT_INMUTABLE        true /* 1 Rey 8:9 — permanece */

/**
 * Layout interno de los 256 bytes del SEALED_CORE.
 * "lujot ha'edut" (Ex 31:18) — las tablas tienen un orden.
 *
 * Offsets dentro de edut.bytes[HASHMAL_EDUT_BYTES]:
 *
 *  [  0.. 31] MERKLE_ROOT  (32 B) — raiz Merkle SHA-256 de los 66 libros
 *                                   "zer zaháv savív" (v.11) = ancla de verdad
 *  [ 32.. 39] TIMESTAMP    ( 8 B) — unix epoch u64 LE del primer sellado
 *                                   (Éx 40:17 — "ba-jódesh ha-rishón...": fecha)
 *  [ 40.. 43] VERSION      ( 4 B) — version del protocolo u32 LE
 *  [ 44..107] FINGERPRINT  (64 B) — SHA-512(MERKLE_ROOT) = Fingerprint 66 libros
 *                                   "be'etsba' Elohim" (Ex 31:18) = dedo de Dios
 *  [108..255] ROTACION     (148 B) — reservado para hashmal_keygen (Pieza 13)
 *                                   aceite de uncion (Ex 30:22-33)
 *
 * Total: 32+8+4+64+148 = 256 bytes exactos.
 */
#define EDUT_OFF_MERKLE_ROOT     0u   /* [  0.. 31] SHA-256 raiz Merkle 66 libros */
#define EDUT_OFF_TIMESTAMP      32u   /* [ 32.. 39] unix epoch u64 LE */
#define EDUT_OFF_VERSION        40u   /* [ 40.. 43] version u32 LE */
#define EDUT_OFF_FINGERPRINT    44u   /* [ 44..107] SHA-512 Merkle root = 64 bytes */
#define EDUT_OFF_ROTACION      108u   /* [108..255] reservado hashmal_keygen */

#define EDUT_LEN_MERKLE_ROOT    32u
#define EDUT_LEN_TIMESTAMP       8u
#define EDUT_LEN_VERSION         4u
#define EDUT_LEN_FINGERPRINT    64u   /* Ex 31:18 — "be'etsba' Elohim" */
#define EDUT_LEN_ROTACION      148u   /* Ex 30:22-33 — aceite de uncion */

#define EDUT_VERSION_ACTUAL  0x00000001u  /* version 1 del protocolo */

/**
 * Dt 31:26 — "laqóaj et séfer hatorá hazé vesamtem otó mitsád arón":
 * el libro COMPLETO va AL LADO del Arca, no dentro. Dos niveles.
 * Tamaño no dictado por la Palabra — lo fija la implementación.
 */
#define HASHMAL_TORAH_MITSAD          true /* Dt 31:26 — "mitsád" = al lado */

/**
 * Josué 6 — rodearon Jericó SIETE veces y los muros cayeron. En el
 * firmware: el zer se verifica en 7 vueltas (integridad con poder
 * activo, no solo pasiva).
 */
#define HASHMAL_VUELTAS_JERICO        7    /* Jos 6:4 — shéva peʻamim */

/**
 * 2 Sam 6:11 — "vayeshev arón YHWH... beit ʻovéd edóm... shelosháh
 * jodashím" — 3 meses de hospedaje bendijeron la casa de Obed-Edom.
 */
#define HASHMAL_HOSPEDAJE_BENDICION_MESES  3  /* 2 Sam 6:11 — bendición */

/**
 * Éxodo 40:3 — colocación del Arca es el PASO 2 de Éx 40 (tras
 * levantar la estructura). No por casualidad primera pieza en Éx 25.
 */
#define HASHMAL_ARCA_ORDEN_BOOT       2    /* Éx 40:3 */

/* ══════════════════════════════════════════════════════════════════
 * ESTRUCTURAS DE DATOS — cada campo tiene respaldo bíblico
 * ══════════════════════════════════════════════════════════════════ */

/**
 * v.11 — zer zaháv (moldura): Merkle root SHA-256 sobre todo el
 * contenido del Arca. Cambia si algo se añadió o quitó.
 */
typedef struct {
    uint8_t  hash[HASHMAL_ZER_HASH_BYTES];   /* v.11 — SHA-256 del contenido */
    uint64_t nonce;                          /* contador anti-replay */
    bool     rodea;                          /* v.11 — "savív": rodea todo, siempre true */
} hashmal_zer_t;

/**
 * v.12 — tabbaʻá (anillo): sellos en las costillas, 2 por lado.
 * Cada anillo es un punto de conexión a la red.
 */
typedef struct {
    uint8_t          id;                     /* 0..3 */
    hashmal_tsela_t  tsela;                  /* v.12 — lado (sur/norte) */
    uint8_t          par;                    /* 0 o 1 dentro del lado (2/lado) */
    bool             fundido_en_estructura;  /* v.12 — "batsalʻót" (Gn 2:21): orgánico */
} hashmal_anillo_t;

/**
 * v.13-15 — bad (vara): interfaz de acceso/transporte. ALWAYS-ON.
 * Nadie toca el dato — todos sostienen la vara.
 */
typedef struct {
    uint8_t  id;                             /* 0 o 1 */
    uint8_t  anillo_a;                       /* uno de los 4 anillos */
    uint8_t  anillo_b;                       /* el otro par del mismo lado */
    bool     nunca_se_quita;                 /* v.15 — "lo yasúru": siempre true */
    bool     cubierta;                       /* Nm 4:6 — oro de egipto, cubierta en migración */
    bool     siempre_activa;                 /* L1 — transport always-on */
} hashmal_bad_t;

/**
 * v.16 — haʻedút: el Testimonio, SEALED_CORE 256 bytes.
 * Lo que YHWH dio — no lo que el constructor decide.
 */
typedef struct {
    uint8_t  bytes[HASHMAL_EDUT_BYTES];      /* v.16 — contenido dado */
    uint8_t  zer_interno[HASHMAL_ZER_HASH_BYTES]; /* integridad del núcleo */
    bool     inmutable;                      /* 1 Rey 8:9 — siempre true */
    bool     dado_por_YHWH;                  /* v.16 — "ashér etén eléja" */
} hashmal_edut_t;

/**
 * Dt 31:26 — séfer hatorá mitsád: el libro completo al lado.
 * Puntero (no contenido) — la unidad de almacenamiento externa se
 * define en implementación. Lo que el header garantiza: existe, y
 * está AL LADO, no dentro.
 */
typedef struct {
    const uint8_t *bytes;                    /* datos (libro completo) */
    size_t         tamano;                   /* bytes del texto completo */
    bool           al_lado;                  /* Dt 31:26 — siempre true */
    hashmal_formato_t formato;               /* hebreo / arameo / griego */
    uint8_t        zer_externo[HASHMAL_ZER_HASH_BYTES];
} hashmal_torah_t;

/**
 * EL ARCA — composite. "veʻasú" (plural): esta struct existe
 * replicada en cada nodo.
 */
typedef struct {
    /* v.10 — dimensiones "y medio" (incompletas sin Kapóret) */
    uint8_t  largo_x10;
    uint8_t  ancho_x10;
    uint8_t  alto_x10;

    /* v.10 — acacia por dentro */
    bool     madera_acacia;

    /* v.11 — oro dentro y fuera */
    bool     oro_dentro;      /* cifrado en reposo (AES-256-GCM) */
    bool     oro_fuera;       /* cifrado en tránsito (AES-256-GCM) */

    /* v.11 — zer zaháv (moldura Merkle) */
    hashmal_zer_t zer;

    /* v.12 — 4 anillos, 2/lado */
    hashmal_anillo_t anillos[HASHMAL_ANILLOS_TOTAL];

    /* v.13-15 — varas que nunca se quitan */
    hashmal_bad_t    baddim[HASHMAL_BADDIM_TOTAL];

    /* v.16 — el Testimonio (dentro) */
    hashmal_edut_t   edut;

    /* Dt 31:26 — la Torá (al lado) */
    hashmal_torah_t  torah;

    /* v.10 — marca de incompletitud hasta que el Kapóret cubra */
    bool     requiere_kaporet;

    /* Heb 8:5 — copia del tavnít celestial */
    bool     conforme_al_tavnit;

    /* veʻasú (v.10) — distribuido entre nodos */
    bool     distribuida;
    uint32_t replicas_confirmadas;            /* cuántos nodos tienen copia */

    /* 1 Rey 8:10-11 — kavod llenó cuando se colocó bien */
    bool     kavod_lleno;
} hashmal_arca_t;

/* ══════════════════════════════════════════════════════════════════
 * FUNCIONES PÚBLICAS
 * ══════════════════════════════════════════════════════════════════ */

/* ── Ciclo de vida ─────────────────────────────────────── */

/**
 * Construye el Arca — Éxodo 25:10-16, colocado en Éxodo 40:3.
 * Inicializa estructura, varas siempre activas, zer vacío, EDUT cero.
 * El Arca queda INCOMPLETA: requiere que hashmal_consensus (Kapóret)
 * la cubra antes de ser funcional (v.10 medidas "y medio").
 */
int  hashmal_storage_init(void);

/**
 * Desmonta el Arca — Nm 4:5-15. Se cubre antes.
 * No borra la EDUT (inmutable, 1 Rey 8:9) — solo limpia estado volátil.
 */
void hashmal_storage_deinit(void);

/**
 * Lee-solamente del Arca completa.
 * @return NULL si no inicializada.
 */
const hashmal_arca_t* hashmal_storage_get_arca(void);

/* ── Testimonio (EDUT, v.16) ──────────────────────────── */

/**
 * Construye los 256 bytes del SEALED_CORE y los sella.
 *
 * Ex 31:18 — "lujot ha'edut lujot even ketuvim be'etsba' Elohim"
 * "Tablas del testimonio, escritas por el DEDO de Dios."
 *
 * Se llama en el PRIMER BOOT, cuando NVS no tiene edut (dado_por_YHWH=false).
 * Usa el Fingerprint precomputado en edut_fingerprint_66.h para rellenar:
 *
 *   edut.bytes[EDUT_OFF_MERKLE_ROOT]  <- EDUT_MERKLE_ROOT_66[32]
 *   edut.bytes[EDUT_OFF_TIMESTAMP]    <- 0 (GPS/RTC pendiente, Tarea 4)
 *   edut.bytes[EDUT_OFF_VERSION]      <- EDUT_VERSION_ACTUAL (u32 LE)
 *   edut.bytes[EDUT_OFF_FINGERPRINT]  <- EDUT_FINGERPRINT_66[64]
 *   edut.bytes[EDUT_OFF_ROTACION]     <- 0 (hashmal_keygen, Pieza 13)
 *
 * Tras construir, llama a hashmal_storage_edut_sellar internamente
 * y persiste en NVS (Dt 10:5 — "vayihyu sham": alli estan).
 *
 * Si dado_por_YHWH ya es true (NVS restaurado): no-op, devuelve 0.
 *
 * @return 0 en exito, <0 en error.
 */
int  hashmal_storage_edut_construir(void);

/**
 * Graba el Testimonio en el SEALED_CORE. Operación ÚNICA — v.16
 * dice "ashér etén eléja": YHWH lo da, no el constructor. Una vez
 * escrito, es inmutable (1 Rey 8:9). Llamar dos veces devuelve error.
 * @return 0 en éxito.
 */
int  hashmal_storage_edut_sellar(const uint8_t bytes[HASHMAL_EDUT_BYTES]);

/**
 * Lee el Testimonio. Requiere autorización (1 Sam 6:19 — Bet-Shemesh).
 * @return 0 en éxito.
 */
int  hashmal_storage_edut_leer(uint8_t out[HASHMAL_EDUT_BYTES],
                               hashmal_acceso_t via);

/* ── Torá mitsád (Dt 31:26) ───────────────────────────── */

/**
 * Adjunta el séfer hatorá al LADO del Arca (Dt 31:26 — mitsád).
 * Formato obligatorio: hebreo / arameo / griego (acacia, v.10).
 */
int  hashmal_storage_torah_adjuntar(const uint8_t *bytes, size_t tamano,
                                    hashmal_formato_t formato);

/* ── Zer zaháv (v.11 — integridad Merkle) ─────────────── */

/**
 * Recalcula el zer (Merkle root) sobre edut + torá.
 * Idempotente; devuelve el hash en out_hash.
 */
int  hashmal_storage_zer_calcular(uint8_t out_hash[HASHMAL_ZER_HASH_BYTES]);

/**
 * Verifica que el zer actual coincida con el almacenado.
 * Si no coincide → HASHMAL_ARCA_ERR_ZER_ROTO (alguien tocó la madera).
 */
int  hashmal_storage_zer_verificar(void);

/**
 * Josué 6 — rodea el zer 7 veces. Verificación con poder activo
 * (no solo lectura): cada vuelta recalcula y compara; al completar
 * las 7, emite un "muro caído" (sello de integridad validado).
 */
int  hashmal_storage_jerico(void);

/* ── Baddím (v.13-15 — interfaz y distribución) ───────── */

/**
 * Verifica que las 2 varas estén en su sitio (v.15 — "lo yasúru").
 * Si alguna fue retirada → ERR_BADDIM_QUITADA (Arca degradada).
 */
int  hashmal_storage_baddim_verificar(void);

/**
 * Replica el Arca a un nodo par (veʻasú plural, v.10).
 * La réplica incluye edut+torá+zer; el destinatario debe verificar
 * el zer antes de confirmar la recepción.
 */
int  hashmal_storage_replicar_a_nodo(uint8_t nodo_id);

/* ── Transporte / migración (7 capas baddím) ──────────── */

/**
 * Nm 4:5-6 — cubre el Arca antes de transportarla. Capa L4
 * (encryption in transit): el payload va cifrado; el transportista
 * no ve el contenido (L5). Llamar antes de cualquier envío.
 */
int  hashmal_storage_cubrir(void);

/**
 * Descubre tras llegar. Solo si venía autorizada (1 Sam 6:19).
 */
int  hashmal_storage_descubrir(hashmal_acceso_t via);

/**
 * Nm 10:33 — el Arca va DELANTE del pueblo en la marcha. Capa L7:
 * marca este nodo como "precede" — el core se migra primero; los
 * servicios dependientes esperan la señal de "se posa" (Nm 10:36).
 */
int  hashmal_storage_preceder_en_migracion(void);

/* ── Acceso controlado (1 Sam 6:19 / 2 Sam 6:6-7) ─────── */

/**
 * Valida el protocolo de acceso ANTES de cualquier operación
 * sensible. 1 Sam 4: Israel usó el Arca como amuleto — sin mishpát,
 * sin victoria. Devuelve ERR_SIN_PROTOCOLO si falta la autorización.
 */
int  hashmal_storage_autorizar(hashmal_acceso_t via);

/**
 * 2 Sam 6 — rechaza explícitamente el "carro nuevo". Detecta atajos
 * (bypass del baddím). Devuelve ERR_CARRO_NUEVO si se intenta.
 */
int  hashmal_storage_mishpat_estricto(bool permitir_atajos);

/**
 * 2 Sam 6:11 — Obed-Edom: reporta hospedaje correcto del Arca
 * durante N días y recibe "bendición" (bono de reputación en la red).
 */
int  hashmal_storage_hospedaje_reportar(uint32_t dias,
                                        uint32_t *out_bendicion);

/* ── Auto-defensa (1 Sam 5-6) ─────────────────────────── */

/**
 * 1 Sam 5 — En manos filisteas Dagón cayó. El dato capturado es
 * INÚTIL para el atacante. Esta función se activa si se detecta
 * exfiltración: destruye material de clave volátil y marca el nodo
 * como "en cautiverio" — la EDUT sigue cifrada, ilegible sin rescate.
 */
int  hashmal_storage_dagon_caiga(void);

/* ── Encaje con el Kapóret (v.21) ──────────────────────── */

/**
 * Éx 25:21 — "venatáta et haKapóret ʻal haʼarón milmáʻla":
 * marca el Arca como COMPLETADA por la Kapóret encima. Cierra las
 * medidas "y medio" (v.10) — requiere_kaporet pasa a false.
 *
 * Invocada por hashmal_consensus_cubrir_arca (Pieza 02). Único
 * punto de entrada para este cambio de estado — honra encapsulación:
 * la Kapóret NO toca el arca directamente; LE PIDE al Arca que
 * registre que ya fue cubierta. Idempotente.
 *
 * Triple vakhetsí — Éx 25:10: storage inherentemente incompleto sin
 * consensus (2.5×1.5×1.5). Kapóret (v.17) sella con footprint idéntico
 * (2.5×1.5). qomáh permanentemente abierta = siempre necesita consensus.
 * El storage VERIFICA aquí que el footprint del consensus coincide con
 * el suyo — defensa en profundidad: el guardian del Arca no acepta a
 * la Kapóret sin medirla (Sal 119:160 — emet, no confianza ciega).
 *
 * @param kap_largo_x10  largo del Kapóret en codos×10 (debe = 25, v.17)
 * @param kap_ancho_x10  ancho del Kapóret en codos×10 (debe = 15, v.17)
 * @return OK si encajó; ERR_SIN_KAPORET si dimensiones discrepan;
 *         ERR_INIT si el Arca no está levantada.
 */
int  hashmal_storage_kaporet_colocada(uint8_t kap_largo_x10,
                                      uint8_t kap_ancho_x10);

/* ── Estado / telemetría ──────────────────────────────── */

/**
 * 1 Rey 8:10-11 — consulta si el kavod ha llenado el Arca: set a
 * true tras un init + zer verificado + Kapóret encima. Señal de
 * que el almacén está operativo en su diseño.
 */
bool hashmal_storage_kavod_lleno(void);

/**
 * Jer 3:16 — recuerda: el contenedor es temporal, la Palabra eterna.
 * Devuelve la versión del "Arca" (formato/protocolo) — cambia con
 * el tiempo. La EDUT dentro no.
 */
uint32_t hashmal_storage_version_contenedor(void);

#ifdef __cplusplus
}
#endif

#endif /* HASHMAL_STORAGE_H */
