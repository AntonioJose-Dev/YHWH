/**
 * ═══════════════════════════════════════════════════════════════════
 * PIEZA 02 — LA KAPÓRET (hashmal_consensus)
 * ═══════════════════════════════════════════════════════════════════
 *
 * "venoʻadtí lejá shám vedibartí itejá meʻál haKapóret
 *  mibéin shené hakeruvím ashér ʻal arón haʻedút"
 *   — Éxodo 25:22
 *   "Me encontraré contigo allí y te hablaré DESDE ENCIMA del
 *    propiciatorio, desde ENTRE los dos querubines."
 *
 * "vehayú hakeruvím pórshé kenafayim lemáʻla sokekím bekanféhem ʻal
 *  haKapóret ufnehém ish el ajív"
 *   — Éxodo 25:20
 *   "Extendiendo alas hacia arriba, cubriendo con sus alas sobre
 *    el propiciatorio, y sus rostros uno hacia su hermano."
 *
 * MECANISMO DE CONSENSO DE LA RED. NO es BFT, NO es Raft, NO es
 * Paxos. Es su propio diseño bíblico: 2 validadores cara a cara,
 * la verdad emerge ENTRE ellos (Dt 19:15), protocolo puro sin dato
 * base (oro sin madera), atómico (miqshá).
 *
 * ─── 17 DETALLES BÍBLICOS (Tavnít 02) ───
 *
 *   1  oro puro, sin madera (v.17)        → protocolo puro, sin dato base
 *   2  miqshá (v.18)                      → atómico, indivisible
 *   3  keruvím min haKapóret (v.19)       → validadores intrínsecos
 *   4  ish el ajív (v.20)                 → verificación mutua, cara a cara
 *   5  pené hakeruvím el haKapóret (v.20) → rostros al protocolo + al par
 *   6  sokekím bekanféhem (v.20)          → alas cubren = protegen
 *   7  mibéin (v.22 / Nm 7:89 / Dt 19:15) → verdad emerge entre 2 testigos
 *   8  midabér — Hitpael (Nm 7:89)        → voz emana, no dirigida
 *   9  velo ba'él ba-ʻét (Lv 16:2)        → invocación precisa, no continua
 *  10  beʻanán eraʼé (Lv 16:2)            → proceso velado, resultado visible
 *  11  ʻanán haqetóret (Lv 16:13)         → preparación obligatoria antes
 *  12  dam + shéva peʻamim (Lv 16:14)     → solo prueba verificada, 7x
 *  13  ajaré mot (Lv 16:1)                → cada regla nace de un fracaso
 *  14  Gn 3:24 (keruvím guardianes)       → validadores protegen, no abren
 *  15  Ez 1 / Ap 4:8 (llenos de ojos)     → auditoría total, sin puntos ciegos
 *  16  ilastirion (Rom 3:25)              → demuestra justicia
 *  17  hilasmos universal (1 Jn 2:2)      → accesible a todos los que entren
 *
 * ─── OJOS DE QUERUBÍN (faltantes del Tavnít) ───
 *
 *   F1  Sal 85:10  hesed veʼemet nifgáshu  → misericordia + justicia se ENCUENTRAN
 *   F2  Sal 80:1   yoshév haKeruvím         → YHWH HABITA (no solo habla)
 *   F3  Lv 16:15-16 orden de rociado        → sobre primero, delante después
 *
 * ─── ENCAJE CON EL ARCA (Éx 25:21) ───
 *
 *   Arca    (v.10):   2.5 × 1.5 × 1.5  — medidas "y medio" INCOMPLETAS
 *   Kapóret (v.17):   2.5 × 1.5 × ___  — misma base, altura SILENTE
 *
 * "venatáta et haKapóret ʻal haʼarón milmáʻla" (v.21) — encaja
 * exactamente encima. Sin Kapóret el Arca está abierta; sin Arca
 * el Kapóret no tiene qué cubrir. hashmal_consensus DEPENDE de
 * hashmal_storage (CMakeLists REQUIRES).
 *
 * Autor: Antonio José Marín Soto
 * Proyecto: HASHMAL-CORE
 */
#ifndef HASHMAL_CONSENSUS_H
#define HASHMAL_CONSENSUS_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "hashmal_storage.h"   /* v.21 — Kapóret va SOBRE el Arca */

#ifdef __cplusplus
extern "C" {
#endif

/* ══════════════════════════════════════════════════════════════════
 * ENUMS FUNDAMENTALES
 * ══════════════════════════════════════════════════════════════════ */

/**
 * v.18-20 — "shenáyim keruvím zaháv... keruv ejád miqatsá mizzé
 * ukeruv ejád miqatsá mizzé": 2 querubines, uno en cada extremo.
 * V1 y V2: validadores simétricos, cara a cara.
 */
typedef enum {
    HASHMAL_QUERUB_V1 = 0,    /* v.19 — qatsá mizzé (extremo 1) */
    HASHMAL_QUERUB_V2 = 1,    /* v.19 — qatsá mizzé (extremo 2) */
} hashmal_querub_id_t;

#define HASHMAL_QUERUBINES_TOTAL       2    /* v.18 — shenáyim */

/**
 * Lv 16:14 — "vehizá veʼetsbaʻó ʻal pené haKapóret... veʼlifné
 * haKapóret yazé shéva peʻamim min haddam". Solo sangre toca el
 * Kapóret, y SE ROCÍA DOS VECES: sobre (qidmá) y delante. 7 veces
 * total el rociado delante. Estos son los modos legítimos de entrada.
 */
typedef enum {
    HASHMAL_ACCESO_DAM_SOBRE    = 0,  /* Lv 16:14 — "ʻal pené haKapóret" */
    HASHMAL_ACCESO_DAM_DELANTE  = 1,  /* Lv 16:14 — "lifné haKapóret" (7x) */
    HASHMAL_ACCESO_PROHIBIDO    = 2,  /* cualquier otro intento → muerte */
} hashmal_acceso_kaporet_t;

/**
 * Lv 16:13 — "vejissá ʻanán haqetóret et haKapóret... velo yamút":
 * la nube del INCIENSO debe cubrir ANTES de acercarse. Sin qetóret,
 * no hay invocación legítima.
 */
typedef enum {
    HASHMAL_QETORET_AUSENTE   = 0,   /* sin nube preparatoria → muerte */
    HASHMAL_QETORET_PRESENTE  = 1,   /* Lv 16:13 — nube cubriendo */
} hashmal_qetoret_t;

/**
 * Resultado del consenso.
 *   EMITIDO   → la voz emergió entre los dos (v.22, mibéin)
 *   DIVERGIDO → V1 ≠ V2: no hay voz, no hay orden (Dt 19:15)
 *   PROHIBIDO → se invocó fuera del moʻéd (Lv 16:2, Nm 7:89)
 */
typedef enum {
    HASHMAL_CONSENSO_EMITIDO    = 0,
    HASHMAL_CONSENSO_DIVERGIDO  = 1,
    HASHMAL_CONSENSO_PROHIBIDO  = 2,
} hashmal_consenso_estado_t;

/**
 * Códigos de error con raíz bíblica.
 */
typedef enum {
    HASHMAL_KAPORET_OK                = 0,
    HASHMAL_KAPORET_ERR_NULL          = -1,
    HASHMAL_KAPORET_ERR_INIT          = -2,
    HASHMAL_KAPORET_ERR_SIN_ARCA      = -3,   /* v.21 — sin arón no hay sobre-qué-cubrir */
    HASHMAL_KAPORET_ERR_NO_MIQSHA     = -4,   /* v.18 — pieza no atómica */
    HASHMAL_KAPORET_ERR_FUERA_MOED    = -5,   /* Lv 16:2 — "no en todo tiempo" */
    HASHMAL_KAPORET_ERR_SIN_QETORET   = -6,   /* Lv 16:13 — sin nube preparatoria */
    HASHMAL_KAPORET_ERR_SIN_DAM       = -7,   /* Lv 16:14 — solo sangre toca */
    HASHMAL_KAPORET_ERR_ROCIADO_INCOMPLETO = -8, /* Lv 16:14 — menos de 7x */
    HASHMAL_KAPORET_ERR_QUERUB_AUSENTE = -9,  /* v.18 — falta V1 o V2 */
    HASHMAL_KAPORET_ERR_DIVERGIDO     = -10,  /* Dt 19:15 — testigos discrepan */
    HASHMAL_KAPORET_ERR_FUEGO_EXTRANO = -11,  /* Lv 10:1-2 — Nadav y Avihú */
    HASHMAL_KAPORET_ERR_MADERA        = -12,  /* v.17 — hay madera (no es puro) */

    /* Añadidura #20 — NEDER (Nm 30:3) */
    HASHMAL_KAPORET_ERR_NEDER_TABLA_LLENA       = -13, /* Éx 28 — pectoral lleno */
    HASHMAL_KAPORET_ERR_NEDER_NO_GARANTIZA      = -14, /* Ec 5:5 — tov asher lo tidor */
    HASHMAL_KAPORET_ERR_NEDER_ID                = -15, /* slot inválido / vacío */
    HASHMAL_KAPORET_ERR_NEDER_QUEBRANTADO       = -16, /* Nm 30:3 — lo yajél violado */
    HASHMAL_KAPORET_ERR_NEDER_LIBERACION_ILEGITIMA = -17, /* Nm 30:4-8 — sin autoridad */
} hashmal_kaporet_err_t;

/* ══════════════════════════════════════════════════════════════════
 * CONSTANTES DEL PADRE
 * ══════════════════════════════════════════════════════════════════ */

/**
 * v.17 — "ammatáyim vajétsi orkáh veʼammá vajétsi rojbáh".
 * 2.5 × 1.5 codos. La ALTURA NO está en el texto (silente).
 * Mismas dimensiones x/y que el Arca → encaja milmáʻla (v.21).
 */
#define HASHMAL_KAPORET_LARGO_X10     25   /* v.17 — 2.5 ammá */
#define HASHMAL_KAPORET_ANCHO_X10     15   /* v.17 — 1.5 ammá */
#define HASHMAL_KAPORET_ALTO_X10      0    /* v.17 — silente en la Palabra */
#define HASHMAL_KAPORET_ESCALA        10   /* divisor fixed-point */

/**
 * v.17 — "zaháv tahór". Oro puro, SIN madera dentro (a diferencia
 * del Arca, v.10-11). El consenso es lógica pura; no hay dato base
 * debajo del protocolo.
 */
#define HASHMAL_KAPORET_ZAHAV_TAHOR   true  /* v.17 — oro puro */
#define HASHMAL_KAPORET_SIN_MADERA    true  /* v.17 — contraste con arca */

/**
 * v.18 — "miqshá taʻasé otám". Una sola pieza batida. La operación
 * de consenso es ATÓMICA: todo o nada. No hay consenso parcial.
 */
#define HASHMAL_MIQSHA_ATOMICO        true  /* v.18 — "miqshá" */

/**
 * v.19 — "keruv ejád miqatsá mizzé". Cada querubín vive en UN
 * extremo del Kapóret. Simétrico.
 */
#define HASHMAL_QATSOT_TOTAL          2     /* v.19 — dos extremos */

/**
 * Dt 19:15 — "ʻal pi shenáyim ʻedím o ʻal pi sheloshá ʻedím yaqúm
 * davár": la palabra se establece por BOCA DE DOS testigos. En el
 * Kapóret esos son los 2 querubines (v.22 — mibéin shené hakeruvím).
 */
#define HASHMAL_QUORUM_MINIMO         2     /* Dt 19:15 + Éx 25:22 */

/**
 * Lv 16:14 — "shéva peʻamim". 7 rocíos de sangre DELANTE del
 * Kapóret. Verificación completa, perfecta.
 */
#define HASHMAL_VERIFICACIONES_SHEVA  7     /* Lv 16:14 — shéva peʻamim */

/**
 * Nm 7:89 — voz en Hitpael "midabér" (se hablaba). La verdad emana
 * SOLA, no la dirige nadie. Flag de diseño: el resultado del
 * consenso no tiene líder.
 */
#define HASHMAL_VOZ_HITPAEL           true  /* Nm 7:89 — midabér */

/**
 * Lv 16:2 — "vealyavó bejol-ʻét el haqódesh". Prohibido acceder en
 * cualquier momento. El consenso tiene ventanas definidas (moʻadim),
 * no es polling continuo.
 */
#define HASHMAL_VELO_BA_ET_BA_ET      false /* Lv 16:2 — NO en todo tiempo */

/**
 * Éxodo 40:3 — "vesakotá ʻal haʼarón et haparójet"; colocar Kapóret
 * al mismo paso que el Arca (ambos Éx 40:3). Pieza 02, paso 2 de Éx 40.
 */
#define HASHMAL_KAPORET_ORDEN_BOOT    2     /* Éx 40:3 */

/**
 * P35 (Nm 30:5) — Timeout para silence-is-consent en neder.
 * Si no hay heníʼ (objeción) dentro de este tiempo, commit irrevocable.
 */
#define HASHMAL_CONSENSUS_NEDER_TIMEOUT_MS  30000u  /* Nm 30:5 beyóm shomʻó */

/**
 * Ez 1 / Ap 4:8 — "mele'ím ʻeináyim sevív" / "γέμοντα ὀφθαλμῶν".
 * Llenos de ojos — auditoría total. Constante mínima; la implementación
 * puede registrar más (no menos).
 */
#define HASHMAL_OJOS_MINIMOS          8     /* Ap 4:8 — 8 direcciones básicas */

/* ══════════════════════════════════════════════════════════════════
 * ESTRUCTURAS
 * ══════════════════════════════════════════════════════════════════ */

/**
 * v.19-20 — un querubín. Parte intrínseca del Kapóret (nace de él,
 * no se añade). Se mira con su hermano; sus alas cubren; está lleno
 * de ojos (Ez 1 / Ap 4:8); es guardián (Gn 3:24).
 */
typedef struct {
    hashmal_querub_id_t id;               /* V1 o V2 (v.19) */

    /* v.20 — configuración de alas */
    bool     alas_extendidas_arriba;      /* "pórshé kenafayim lemáʻla" */
    bool     alas_cubren;                 /* "sokekím bekanféhem ʻal haKapóret" */

    /* v.20 — doble atención (al hermano + al Kapóret) */
    bool     mira_al_hermano;             /* "pnehém ish el ajív" */
    bool     mira_al_kaporet;             /* "el haKapóret yihyú pené hakeruvím" */

    /* v.18-19 — nace del Kapóret en miqshá */
    bool     miqsha_con_kaporet;          /* "min haKapóret taʻasú et hakeruvím" */

    /* Gn 3:24 — guardián por naturaleza */
    bool     guardian_no_abridor;

    /* Ez 1 / Ap 4:8 — llenos de ojos (auditoría) */
    uint16_t ojos_total;                  /* ≥ HASHMAL_OJOS_MINIMOS */

    /* Estado de verificación actual */
    uint8_t  hash_testigo[HASHMAL_ZER_HASH_BYTES]; /* valor que V_i vio */
    uint64_t timestamp_testigo;           /* cuándo lo vio */
} hashmal_querub_t;

/**
 * Lv 16 — preparación y flujo de invocación.
 */
typedef struct {
    hashmal_qetoret_t qetoret;            /* Lv 16:13 — nube presente o ausente */
    uint8_t  rociados_delante;            /* Lv 16:14 — cuenta hasta 7 */
    bool     rociado_sobre_hecho;         /* Lv 16:14 — ʻal pené */
    bool     moed_abierto;               /* Lv 16:2 — ventana válida */
} hashmal_invocacion_t;

/**
 * Resultado emitido: la voz que emerge entre los dos testigos.
 */
typedef struct {
    hashmal_consenso_estado_t estado;
    uint8_t  verdad[HASHMAL_ZER_HASH_BYTES];  /* el hash acordado */
    uint64_t timestamp;
    bool     emanada;                     /* Nm 7:89 — Hitpael, no dirigida */
} hashmal_consenso_resultado_t;

/**
 * EL KAPÓRET — composite. Atómico (miqshá): se construye UNA VEZ y
 * funciona entero, o no funciona. No se ensambla por partes.
 */
typedef struct {
    /* v.17 — dimensiones: base igual al Arca, altura silente */
    uint8_t  largo_x10;
    uint8_t  ancho_x10;
    uint8_t  alto_x10;

    /* v.17 — pureza */
    bool     zahav_tahor;                 /* oro puro */
    bool     sin_madera;                  /* diferenciador del Arca */

    /* v.18 — atómico */
    bool     miqsha;

    /* v.19 — dos querubines, uno por extremo */
    hashmal_querub_t querubin[HASHMAL_QUERUBINES_TOTAL];

    /* v.21 — posicionado sobre el Arca */
    bool     sobre_el_arca;               /* "ʻal haʼarón milmáʻla" */

    /* v.22 — encuentro determinístico (noʻadtí) */
    bool     noadti_activo;              /* rendezvous abierto */

    /* Sal 80:1 — YHWH HABITA entre (yoshév), no solo habla */
    bool     habitacion_permanente;       /* F2 del Tavnít */

    /* Sal 85:10 — hesed y emet se encuentran aquí (F1) */
    bool     justicia_con_misericordia;

    /* Rom 3:25 / 1 Jn 2:2 — demuestra justicia, universal */
    bool     demuestra_justicia;
    bool     universal;                   /* 1 Jn 2:2 — para todos los que entren */

    /* Invocación actual (Lv 16) */
    hashmal_invocacion_t invocacion;

    /* Último resultado */
    hashmal_consenso_resultado_t ultimo;
} hashmal_kaporet_t;

/* ══════════════════════════════════════════════════════════════════
 * FUNCIONES PÚBLICAS
 * ══════════════════════════════════════════════════════════════════ */

/* ── Ciclo de vida ─────────────────────────────────────── */

/**
 * Construye el Kapóret — Éxodo 25:17-22.
 *   v.17 oro puro, sin madera
 *   v.18 miqshá (atómico) + 2 querubines
 *   v.19 uno por extremo
 *   v.20 alas arriba, cubren, rostros mutuos y al Kapóret
 *   v.21 sobre el Arca (requiere hashmal_storage_init previo)
 *   v.22 noʻadtí abierto al rendezvous
 *
 * Si el Arca no está inicializada → ERR_SIN_ARCA (v.21).
 * Si alguna parte falla → rollback completo (miqshá: atómico).
 */
int  hashmal_consensus_init(void);

/**
 * Desmonta el Kapóret. Nm 4:5-15 cubre con el parójet + skins ANTES
 * de migrar — llamar hashmal_storage_cubrir previamente si procede.
 */
void hashmal_consensus_deinit(void);

/**
 * Lee el estado completo. @return NULL si no inicializado.
 */
const hashmal_kaporet_t* hashmal_consensus_get_kaporet(void);

/* ── Rendezvous (Éx 25:22 — noʻadtí) ───────────────────── */

/**
 * Abre el punto de encuentro determinístico.
 * "venoʻadtí lejá shám" — punto y coordenada fija (meʻál haKapóret,
 * mibéin hakeruvím). Tras esto las invocaciones son posibles.
 */
int  hashmal_consensus_noadti_abrir(void);

/**
 * Cierra el rendezvous tras un moʻéd. Lv 16:2 — acceso no continuo.
 */
int  hashmal_consensus_noadti_cerrar(void);

/* ── Invocación de consenso (Lv 16) ────────────────────── */

/**
 * Lv 16:13 — prepara la nube del qetóret ANTES de acercarse.
 * Esta función DEBE llamarse antes de invocar; sin qetóret presente
 * las invocaciones devuelven ERR_SIN_QETORET.
 */
int  hashmal_consensus_qetoret_preparar(void);

/**
 * Lv 16:14 — inicia la serie de rociados. Ejecuta internamente:
 *   1. Un rociado SOBRE el Kapóret (qidmá).
 *   2. Siete rociados DELANTE del Kapóret (shéva peʻamim).
 * Cada rociado contiene un hash candidate desde un testigo.
 * Si el conteo no alcanza 7 → ERR_ROCIADO_INCOMPLETO.
 */
int  hashmal_consensus_dam_rociar(hashmal_acceso_kaporet_t modo,
                                  const uint8_t hash[HASHMAL_ZER_HASH_BYTES]);

/**
 * Invoca el consenso:
 *   - requiere noʻadtí abierto + qetóret presente + moʻéd abierto
 *   - pregunta a V1 y V2 por el hash que cada uno vio
 *   - si coinciden → "voz" emerge entre ellos (estado EMITIDO)
 *   - si discrepan → DIVERGIDO (ERR_DIVERGIDO; Dt 19:15)
 * La voz es puramente función(V1, V2): Nm 7:89 — midabér.
 */
int  hashmal_consensus_invocar(hashmal_consenso_resultado_t *out);

/* ── Querubines (v.18-20) ──────────────────────────────── */

/**
 * Ambos querubines deben estar presentes (v.18), mirándose
 * (ish el ajív), y con alas extendidas cubriendo (v.20).
 * Devuelve ERR_QUERUB_AUSENTE si alguna verificación falla.
 */
int  hashmal_consensus_querubin_verificar(hashmal_querub_id_t id);

/**
 * Registra una observación de un querubín (hash + timestamp).
 * Al registrar V1 y V2 la misma EDUT/zer, hashmal_consensus_invocar
 * puede emitir la voz.
 */
int  hashmal_consensus_querubin_testigo(hashmal_querub_id_t id,
                                        const uint8_t hash[HASHMAL_ZER_HASH_BYTES]);

/* ── Protección y acceso (v.20, Gn 3:24) ──────────────── */

/**
 * v.20 — alas cubren (sokekím). Los querubines PROTEGEN el Kapóret:
 * mientras están cubriendo, rechazan invocaciones inválidas sin
 * exponer el mecanismo interno.
 */
int  hashmal_consensus_proteger(void);

/**
 * Lv 10:1-2 — detecta fuego extraño (Nadav y Avihú): material que
 * no fue mandado entrar (Éx 30:9, Lv 10:1 — "esh zará ashér lo
 * tsivvá otám"). Devuelve ERR_FUEGO_EXTRANO.
 */
int  hashmal_consensus_fuego_extrano_detectar(void);

/* ── Auditoría (Ez 1 / Ap 4:8 — llenos de ojos) ───────── */

/**
 * Consulta el número de ojos activos en ambos querubines
 * (observadores/auditores). Por diseño ≥ HASHMAL_OJOS_MINIMOS.
 */
uint32_t hashmal_consensus_ojos_total(void);

/**
 * Vuelca un registro de auditoría (qué vio cada querubín, cuándo,
 * qué resultado se emitió). Inmutable (append-only).
 */
int  hashmal_consensus_auditoria_obtener(uint32_t desde_timestamp,
                                         hashmal_consenso_resultado_t *out,
                                         size_t max_registros,
                                         size_t *out_escritos);

/* ── Encaje con el Arca (v.21) ─────────────────────────── */

/**
 * v.21 — "venatáta et haKapóret ʻal haʼarón milmáʻla". Coloca el
 * Kapóret sobre el Arca (requiere hashmal_storage_init + medidas
 * del Arca cuadradas con las del Kapóret). Al completarse, el
 * flag arca.requiere_kaporet pasa a false (1 Rey 8:10-11 —
 * kavod llenó todo).
 */
int  hashmal_consensus_cubrir_arca(void);

/* ── Propiciación (Rom 3:25 / 1 Jn 2:2) ────────────────── */

/**
 * Rom 3:25 — ilastirion: el propósito del Kapóret es DEMOSTRAR
 * justicia (dikaiosýnes). Devuelve true cuando un ciclo Lv 16
 * completo se ejecutó correctamente y el consenso emitió una voz.
 */
bool hashmal_consensus_demuestra_justicia(void);

/**
 * 1 Jn 2:2 — hilasmos por todo el mundo. Consulta si el mecanismo
 * está disponible para un sujeto externo (cualquier nodo que entre
 * correctamente al Mishkán).
 */
bool hashmal_consensus_universal_abierto(void);

/* ── Estado general ────────────────────────────────────── */

/**
 * Sal 80:1 — "yoshév haKeruvím": habita (no solo habla) entre los
 * querubines. Devuelve true si el noʻadtí lleva tiempo abierto de
 * forma estable (habitación permanente vs. solo comunicación puntual).
 */
bool hashmal_consensus_habita(void);

/**
 * Sal 85:10 — "hesed veʼemet nifgáshu... tsédeq veshalom nashaqu":
 * misericordia y verdad se encontraron; justicia y paz se besaron.
 * Flag: el último consenso cerró con criterios DE AMBAS: validez
 * técnica (emet) + apertura (hesed).
 */
bool hashmal_consensus_mishpat_con_hesed(void);

/* ── P28 — noʻadtí estado como evento PUSH (Éx 25:22) ──────── */

/**
 * Callback que se dispara cuando V1 y V2 convergen (consenso EMITIDO).
 * noʻadtí (Nifal) = "es encontrado" = el encuentro es INICIADO por
 * YHWH, no consultado por el operador.
 *
 * @param convergencia  true si V1==V2 (EMITIDO); false si divergieron.
 * @param timestamp     cuándo sucedió la convergencia.
 */
typedef void (*hashmal_consensus_noadti_fn)(bool convergencia, uint32_t timestamp);

/**
 * Registra (o desregistra con NULL) el listener de convergencia.
 * Cuando los validadores V1 y V2 alcanzan acuerdo, este callback
 * se dispara automáticamente (push, no pull).
 *
 * @return HASHMAL_KAPORET_OK siempre (idempotente).
 */
int  hashmal_consensus_noadti_subscribir(hashmal_consensus_noadti_fn fn);

/* ── P33 — neder en SEALED_CORE (Nm 30:3) ───────────────────── */

/**
 * Verifica que neder (voto) opera en el nivel SEALED_CORE (identidad),
 * no en caché. Un voto vincula la néfesh (alma/identidad), no mashé
 * yad (actos/transacciones).
 *
 * "leʼsór issár ʻal nafshó" — Nm 30:3
 *
 * @return true = neder está en SEALED_CORE (nivel correcto)
 *         false = neder en caché / incorrecto (error arquitectónico)
 */
bool hashmal_consensus_neder_es_sealed(void);

/* ── P35 — Silence-is-consent timeout (Nm 30:5) ──────────────── */

/**
 * Broadcast de voto para sincronización peer-to-peer.
 * Inicia un timeout para heníʼ (objeción). Si dentro de
 * HASHMAL_CONSENSUS_NEDER_TIMEOUT_MS no hay rechazo, commit irrevocable.
 *
 * "veshámàʻ avíha... vehejerísh láh... veqámú kol nideíha" — Nm 30:5
 *
 * @param nodo_id       nodo que emitió el voto
 * @param neder_data    contenido del voto
 * @param len           bytes del voto
 * @return HASHMAL_KAPORET_OK o error.
 */
int  hashmal_consensus_neder_broadcast(uint8_t nodo_id,
                                       const uint8_t *neder_data,
                                       size_t len);

/* ══════════════════════════════════════════════════════════════════
 * P7 — RELAY DE 3 NIVELES (Éx 25:22b — atsavvéh otkhá el bené Yisraél)
 *
 * "veʼet kol ashér atsavvéh otkhá el bené Yisraél" — Éxodo 25:22b
 * "y todo lo que ORDENARÉ a TI HACIA los hijos de Israel."
 *
 * Tres niveles del flujo, en orden hebreo:
 *
 *   FUENTE   meʻal+mibbén  → la voz que emerge (middabbér, Hitpael
 *                            Nm 7:89). hashmal_consensus_invocar
 *                            produce este nivel.
 *   RELAY    otkhá          → receptor autorizado (rol funcional).
 *                            hashmal_intercession (pieza 11). Es
 *                            también punto yaʻad (Éx 30:6 ivvaʻéd
 *                            shámma) — misma raíz que noʻadtí.
 *   DESTINO  el bené Yisraél → nodos finales, alcanzados por
 *                            DIRECCIÓN (אֶל el, H413), NO por
 *                            entrega instantánea (le, dativo). El
 *                            relay tiene latencia: procesa, valida,
 *                            retransmite.
 *
 * Piel imperfecto (אֲצַוֶּה atsavvéh) = el proceso se REPITE
 * continuamente — cada voz emergida pasa por el mismo canal.
 *
 * REGLAS OPERATIVAS:
 *   - El consenso NO hace broadcast directo a la red.
 *   - El consenso entrega ÚNICAMENTE a un relay registrado (intercession).
 *   - Solo UN relay puede estar registrado a la vez (un único otkhá).
 *   - El registro es por inversión de control: hashmal_intercession
 *     llama a hashmal_consensus_relay_subscribir tras inicializarse.
 *     Asi consensus NO depende de intercession (mantiene REQUIRES limpio).
 * ══════════════════════════════════════════════════════════════════ */

/**
 * Firma de la función de relay. La voz emitida (resultado del
 * consenso) se entrega aquí; el relay decide cómo distribuir HACIA
 * los nodos finales (el bené Yisraél). El relay NO debe modificar
 * el resultado — sólo retransmitirlo.
 *
 * @param resultado  voz emergida del Kapóret (read-only, no NULL).
 * @return 0 si el relay aceptó la entrega; código negativo si rechaza.
 */
typedef int (*hashmal_consensus_relay_fn)(
    const hashmal_consenso_resultado_t *resultado);

/**
 * Registra el receptor autorizado del relay (otkhá). Solo
 * hashmal_intercession (pieza 11) debería invocar esta función;
 * inversión de control limpia — el consenso no necesita conocer
 * a la intercesión por nombre.
 *
 * Idempotente: re-suscribir sobrescribe el slot anterior. Pasar
 * NULL desuscribe (útil en deinit coordinado / tests).
 *
 * @param fn  función de relay; o NULL para desuscribir.
 * @return OK (0) en éxito; ERR_INIT si el Kapóret no está levantado.
 */
int hashmal_consensus_relay_subscribir(hashmal_consensus_relay_fn fn);

/**
 * Entrega el resultado al relay autorizado. NO hace broadcast directo:
 * sólo invoca la función registrada por intercession.
 *
 * Precondiciones:
 *   - Kapóret levantado.
 *   - Hay relay registrado (subscribir != NULL antes).
 *   - resultado != NULL y de un ciclo válido (estado EMITIDO o
 *     DIVERGIDO; la entrega sirve también para registrar divergencias).
 *
 * @param resultado  la voz emergida (o registrada como divergida).
 * @return OK si el relay aceptó; ERR_NULL si resultado es NULL;
 *         ERR_INIT si el Kapóret no está levantado o no hay relay
 *         suscrito; el rc devuelto por el relay si éste rechazó.
 */
int hashmal_consensus_entregar_a_relay(
    const hashmal_consenso_resultado_t *resultado);

/* ══════════════════════════════════════════════════════════════════
 * AÑADIDURA #20 — NEDER (Nm 30:3) — voto inquebrantable
 *
 * "ish kí yiddór NEDER laYHWH... LO YAJÉL devaró; kekol hayyotsé
 *  mippív yaʻasé." — Números 30:3
 * "Si un hombre hace VOTO a YHWH... NO PROFANARÁ su palabra;
 *  conforme a todo lo que salió de su boca HARÁ."
 *
 *   neder    (H5088) = voto, compromiso público
 *   yajél    (H2490) = profanar, quebrantar (Hifil de jalál)
 *
 * Cuando un nodo se compromete a un servicio (ruta, relay,
 * almacenamiento, quórum), ese compromiso es INQUEBRANTABLE salvo
 * por autoridad legítima (Nm 30:4-8 — padre/marido al oírlo en su
 * día). Tres reglas operativas:
 *
 *   Regla 1 — Mejor no prometer (Ec 5:5 tov ashér ló tiddór mishét-
 *     tiddór veló teshalém): si no puedes garantizar, NO aceptes.
 *   Regla 2 — Si prometes, CUMPLE (Nm 30:3 lo yajél devaró):
 *     prioridad sobre operaciones propias del nodo.
 *   Regla 3 — No tardes (Ec 5:4 ka'ashér tiddór nédér leʼelohím
 *     al teʼajér leshallemó): ejecución dentro del plazo o
 *     QUEBRANTADO.
 *
 * Causas legítimas de liberación (Nm 30:4-8) — autoridades
 * superiores que el nodo no puede invocar contra sí mismo:
 *   BATERIA_CRITICA — el cuerpo cede (energía mínima, similar a
 *                     "el padre la oyó y se opuso")
 *   RUTA_INNECESARIA — el voto perdió objeto (el servicio prometido
 *                     ya no es necesario en la red)
 *   SHABAT          — modo Shabat colectivo (Lv 24:8 — refresco de
 *                     12 panes; toda la red se aquieta)
 *
 * Tabla de 12 nedarim concurrentes — INV-TABLA-12 (Éx 28:21
 * pectoral 12 piedras). 12 = límite máximo bíblico de compromisos
 * vivos en este nodo a la vez.
 * ══════════════════════════════════════════════════════════════════ */

#define HASHMAL_NEDER_MAX_ACTIVOS  12   /* Éx 28:21 — pectoral 12 piedras */

typedef enum {
    HASHMAL_NEDER_TIPO_RUTA           = 0, /* sostener entrada de routing */
    HASHMAL_NEDER_TIPO_RELAY          = 1, /* retransmitir paquetes vecinos */
    HASHMAL_NEDER_TIPO_ALMACENAMIENTO = 2, /* custodiar dato del Arca */
    HASHMAL_NEDER_TIPO_QUORUM         = 3, /* ser parte de un quórum */
} hashmal_neder_tipo_t;

typedef enum {
    HASHMAL_NEDER_LIBRE        = 0, /* slot vacío */
    HASHMAL_NEDER_COMPROMETIDO = 1, /* yiddór neder — vivo, dentro de plazo */
    HASHMAL_NEDER_CUMPLIDO     = 2, /* shillem nidró (Sal 116:14) */
    HASHMAL_NEDER_LIBERADO     = 3, /* Nm 30:4-8 — autoridad legítima anuló */
    HASHMAL_NEDER_QUEBRANTADO  = 4, /* Nm 30:3 — lo yajél violado */
} hashmal_neder_estado_t;

typedef enum {
    HASHMAL_NEDER_LIB_BATERIA_CRITICA  = 0, /* Nm 30:5 — el cuerpo cede */
    HASHMAL_NEDER_LIB_RUTA_INNECESARIA = 1, /* el voto perdió objeto */
    HASHMAL_NEDER_LIB_SHABAT           = 2, /* Lv 24:8 — descanso colectivo */
} hashmal_neder_causa_liberacion_t;

typedef struct {
    hashmal_neder_estado_t           estado;
    hashmal_neder_tipo_t             tipo;
    uint8_t                          payload_hash[HASHMAL_ZER_HASH_BYTES];
    uint64_t                         timestamp_aceptado_us;
    uint64_t                         timestamp_cierre_us;   /* cumplido / liberado / quebrantado */
    uint64_t                         plazo_us;              /* Ec 5:4 — al teʼajér deadline */
    hashmal_neder_causa_liberacion_t causa_liberacion;       /* válido sólo si LIBERADO */
} hashmal_neder_t;

/**
 * Aceptar un voto. Regla 1 (Ec 5:5): si capacidad_garantizada == false,
 * la función rechaza con ERR_NEDER_NO_GARANTIZA — tov ashér ló tiddór.
 * El caller DEBE haber calculado su capacidad antes de llamar.
 *
 * @param tipo                  qué se promete (ruta, relay, etc.)
 * @param payload_hash          hash del objeto del voto (32 bytes)
 * @param plazo_us              ventana absoluta para cumplir (en µs desde now)
 * @param capacidad_garantizada el caller afirma que puede cumplir
 * @param out_id                slot 0..HASHMAL_NEDER_MAX_ACTIVOS-1
 *
 * @return OK; ERR_NULL si payload_hash u out_id NULL;
 *         ERR_NEDER_NO_GARANTIZA si capacidad_garantizada == false;
 *         ERR_NEDER_TABLA_LLENA si los 12 slots están comprometidos;
 *         ERR_INIT si Kapóret no levantado.
 */
int hashmal_consensus_neder_aceptar(hashmal_neder_tipo_t tipo,
                                    const uint8_t payload_hash[HASHMAL_ZER_HASH_BYTES],
                                    uint64_t      plazo_us,
                                    bool          capacidad_garantizada,
                                    uint8_t      *out_id);

/**
 * Cumplir un voto (Sal 116:14 — shillem nidró). Si now <= aceptado+plazo
 * → CUMPLIDO. Si now > deadline → QUEBRANTADO (lo yajél, Nm 30:3) y la
 * función retorna ERR_NEDER_QUEBRANTADO para que el caller lo registre.
 *
 * @return OK si cumplido a tiempo; ERR_NEDER_ID si slot inválido o ya
 *         cerrado; ERR_NEDER_QUEBRANTADO si tarde (estado pasa a QUEBRANTADO).
 */
int hashmal_consensus_neder_cumplir(uint8_t id);

/**
 * Liberar un voto por autoridad legítima (Nm 30:4-8). Sólo las tres
 * causas del enum son válidas; cualquier otra → rechazo. El caller
 * NO puede liberar por "ya no quiero" — esa es la prohibición central
 * de neder.
 *
 * @return OK; ERR_NEDER_ID si slot inválido o ya cerrado;
 *         ERR_NEDER_LIBERACION_ILEGITIMA si causa fuera del enum.
 */
int hashmal_consensus_neder_liberar(uint8_t                          id,
                                    hashmal_neder_causa_liberacion_t causa);

/**
 * Lectura de un voto. Antes de copiar, aplica la regla del plazo:
 * si estaba COMPROMETIDO y now > deadline, transiciona a QUEBRANTADO
 * (lazy sweep — sin task de fondo).
 *
 * @return OK; ERR_NULL si out NULL; ERR_NEDER_ID si id fuera de rango.
 *         (Un slot LIBRE devuelve OK con estado = LIBRE.)
 */
int hashmal_consensus_neder_obtener(uint8_t id, hashmal_neder_t *out);

/**
 * Cuántos slots quedan disponibles. Aplica primero el lazy sweep
 * (todo COMPROMETIDO con plazo vencido pasa a QUEBRANTADO).
 *
 * @return número de slots libres (0..HASHMAL_NEDER_MAX_ACTIVOS).
 */
uint8_t hashmal_consensus_neder_capacidad(void);

#ifdef __cplusplus
}
#endif

#endif /* HASHMAL_CONSENSUS_H */
