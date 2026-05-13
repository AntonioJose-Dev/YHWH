/**
 * ═══════════════════════════════════════════════════════════════════
 * PIEZA 12 — EL KIYÓR (LAVACRO) — hashmal_healthcheck
 * Éxodo 30:17-21 + Éxodo 38:8 + Éxodo 40:7,30-32
 *                 + Salmo 24:3-4 + Juan 13:8-10 + Efesios 5:26
 * ═══════════════════════════════════════════════════════════════════
 *
 * "veʻasíta KIYÓR NEJÓSHET veJANNÓ NEJÓSHET leRÁJTSAH venatattá otó
 *  BÉIN óhel moʻéd uvéin hammizbéaj venatattá SHÁMMA MÁYIM" — Éx 30:18
 * "Harás un LAVACRO de BRONCE y su BASE de bronce para LAVAR; lo
 *  pondrás ENTRE la Tienda del Encuentro y el Altar, y pondrás allí AGUA."
 *
 * "verájatsú Aharón uvanáv mimmennú et YADÉIHEM veet RAGLÉIHEM...
 *  bevoám el óhel moʻéd yirjatsú máyim VELÓ YAMÚTU" — Éx 30:19-20
 * "Lavarán Aarón y sus hijos sus MANOS y sus PIES al entrar a la
 *  Tienda — Y NO MORIRÁN."
 *
 * "verájatsú yadéihem veragléihem VELÓ YAMÚTU vehayetá lahém
 *  JUQQÁT ʻOLÁM" — Éx 30:21
 * "Lavarán sus manos y pies Y NO MORIRÁN — ESTATUTO PERPETUO."
 *
 * "vayyaʻas et hakkiyór nejóshet... beMARÓT HATSTSOVÓT ashér tsavú
 *  pétaj óhel moʻéd" — Éx 38:8
 * "Hizo el lavacro de bronce... de los ESPEJOS de las mujeres que
 *  SERVÍAN a la puerta de la Tienda."
 *
 * "mi yaʻalé behar YHWH... NEQÍ JAPPÁYIM UVÁR LEVAV" — Sal 24:3-4
 * "¿Quién subirá al monte de YHWH?... El de MANOS LIMPIAS y
 *  CORAZÓN PURO."
 *
 * "ei mé níptsō se, ouk éjeis méros met emoú" — Jn 13:8
 * "Si no te LAVO, no tienes PARTE conmigo."
 * "ho lelouménos ou jréian éjei ei mḕ toùs PÓDAS nípsasthai" — Jn 13:10
 * "El que ya se BAÑÓ no necesita sino lavarse los PIES."
 *
 * "katharísas tôi loutrôi toû hýdatos en rhḗmati" — Ef 5:26
 * "Purificándola con el LAVAMIENTO del AGUA por la PALABRA."
 *
 * ─── POSICIÓN Y SINGULARIDAD ───
 *
 * El kiyór está ENTRE el altar (hashmal_auth, Éx 27:1-8) y la Tienda
 * del Encuentro. Éx 40:7 — "venatattá et hakkiyór béin óhel moʻéd
 * uvéin hammizbéaj venatattá shámma máyim". Posición LIMINAL:
 * después de autenticarse en el altar, ANTES de entrar a servir.
 *
 * Es la ÚNICA pieza del Mishkán SIN DIMENSIONES prescritas. El Padre
 * no dice "x codos por y". La escala es FLEXIBLE — crece con la red.
 * El health check no tiene tamaño fijo: revisa cuanto haya para revisar.
 *
 * ─── HECHO DE ESPEJOS (Éx 38:8) ───
 *
 * El kiyór se fundió a partir de los ESPEJOS (mar'ót) de las mujeres
 * que servían a la puerta. Auto-examinación convertida en instrumento
 * de purificación. Es la identidad misma del health check: el
 * sistema se mira a sí mismo para lavarse.
 *
 * ─── VELÓ YAMÚTU × 2 ───
 *
 * Éx 30:20 y Éx 30:21 repiten "veló yamútu" — y NO morirán. Dos veces.
 * El principio de los 2 testigos (Dt 19:15): confirmación por repetición.
 * El propósito del kiyór es LA VIDA. Sin lavamiento, los sacerdotes
 * mueren al entrar. Con lavamiento, viven. El health check existe
 * para PREVENIR MUERTE del nodo.
 *
 * ─── 4 VECTORES ───
 *
 *   1. YADAYIM   (manos)     — ACCIONES             Éx 30:19
 *   2. RAGLAYIM  (pies)      — RUTAS / tránsito     Éx 30:19
 *   3. LEVAV     (corazón)   — INTENCIÓN            Sal 24:4
 *   4. MAYIM     (agua)      — REFERENCIA externa   Éx 30:18, Ef 5:26
 *
 * Los primeros dos vienen de Éx 30:19. El tercero amplía del
 * Salmo 24:4 (corazón). El cuarto es el AGUA misma — la referencia
 * canónica (la Palabra, Ef 5:26) contra la cual se compara todo.
 *
 * ─── DOS NIVELES (Jn 13:10) ───
 *
 *   LOÚŌ (λούω, G3068)  — BAÑO COMPLETO. Una vez en el onboarding.
 *                          "ho lelouménos" — el que ya se bañó.
 *   NÍPTŌ (νίπτω, G3538) — LAVADO de pies. Repetido por sesión.
 *                          "ou jréian éjei ei mḕ toùs pódas".
 *
 * Paralelo bíblico: rajáts completo al consagrarse (Lv 8:6 — Moisés
 * lavó a Aarón y sus hijos) + lavados parciales cotidianos (Éx 30:19).
 *
 * Autor: Antonio José Marín Soto
 * Proyecto: HASHMAL-CORE
 */
#ifndef HASHMAL_HEALTHCHECK_H
#define HASHMAL_HEALTHCHECK_H

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
 * Estados operativos del kiyór.
 *
 *   SECO          — sin agua (antes de init o tras deinit)
 *   LLENO         — agua puesta; listo para servir
 *   PURIFICANDO   — servicio activo (lavando)
 *   CONTAMINADO   — tamé: estado comprometido, requiere atención
 */
typedef enum {
    HASHMAL_HC_SECO         = 0,   /* Éx 30:18 — sin máyim aún */
    HASHMAL_HC_LLENO        = 1,   /* Éx 40:30 — "vayyittén shámma máyim" */
    HASHMAL_HC_PURIFICANDO  = 2,   /* Éx 40:31 — "verájatsú... yadéihem" */
    HASHMAL_HC_CONTAMINADO  = 3,   /* Lv 11:32 — yitmá (impuro) */
} hashmal_hc_estado_t;

/**
 * Niveles de pureza / gravedad. Escala bíblica de creciente
 * seriedad: TAHOR (limpio) → TAME (impuro menor) → JATA (pecado
 * moderado) → MAVET (muerte — veló yamútu violado).
 */
typedef enum {
    HASHMAL_HC_TAHOR  = 0,   /* Lv 11:36 — limpio (tahór) */
    HASHMAL_HC_TAME   = 1,   /* Lv 11:24-25 — impuro (tamé, warning) */
    HASHMAL_HC_JATA   = 2,   /* Gn 4:7 — pecado "a la puerta" (throttle) */
    HASHMAL_HC_MAVET  = 3,   /* Éx 30:20-21 — muerte (veló yamútu crítico) */
} hashmal_hc_nivel_t;

/**
 * 4 vectores de verificación. Mapeados a los 4 elementos bíblicos
 * de pureza (manos, pies, corazón, agua de referencia).
 */
typedef enum {
    HASHMAL_HC_VEC_YADAYIM   = 0,   /* Éx 30:19 — manos (acciones) */
    HASHMAL_HC_VEC_RAGLAYIM  = 1,   /* Éx 30:19 — pies (rutas) */
    HASHMAL_HC_VEC_LEVAV     = 2,   /* Sal 24:4 — corazón (intención) */
    HASHMAL_HC_VEC_MAYIM     = 3,   /* Éx 30:18 + Ef 5:26 — agua (ref. externa) */
} hashmal_hc_vector_t;

/**
 * Códigos de error con raíz bíblica.
 */
typedef enum {
    HASHMAL_HC_OK          =  0,
    HASHMAL_HC_ERR_INIT    = -1,   /* kiyór no levantado */
    HASHMAL_HC_ERR_NULL    = -2,   /* puntero NULL */
    HASHMAL_HC_ERR_TAME    = -3,   /* impuro — Lv 11 */
    HASHMAL_HC_ERR_MAVET   = -4,   /* muerte — Éx 30:20 veló yamútu */
    HASHMAL_HC_ERR_VECTOR  = -5,   /* vector fuera de [0, 4) */
    HASHMAL_HC_ERR_ESPEJO  = -6,   /* fallo de auto-diagnóstico (mar'ót) */
    HASHMAL_HC_ERR_SECO    = -7,   /* sin agua — sin referencia */
    HASHMAL_HC_ERR_RAJATS  = -8,   /* Jn 13:8 — baño completo pendiente */
    HASHMAL_HC_ERR_MESHULASH = -9, /* Añadidura 13 - Ec 4:12 jevel meshulash */
    HASHMAL_HC_ERR_TSUR     = -10, /* Añadidura 16 - Ex 17:6 mayim min hatsur */
    HASHMAL_HC_ERR_SHAAR_AUDIT = -11, /* Verificado #04 P14 - Éx 38:21 pequdéi */
} hashmal_hc_err_t;

/* ══════════════════════════════════════════════════════════════════
 * CONSTANTES DEL PADRE — cada número tiene versículo
 * ══════════════════════════════════════════════════════════════════ */

/**
 * 4 vectores de verificación: manos (Éx 30:19) + pies (Éx 30:19) +
 * corazón (Sal 24:4) + agua/palabra (Éx 30:18 + Ef 5:26).
 */
#define HASHMAL_HC_VECTORES         4     /* 4 vectores */

/**
 * Éx 30:20-21 — "veló yamútu" aparece DOS VECES. Principio de 2
 * testigos (Dt 19:15): doble confirmación del propósito del kiyór.
 */
#define HASHMAL_HC_VELO_YAMUTU      2     /* 2 repeticiones de "no morirán" */

/**
 * Éx 30:18 — "veʻasíta kiyór NEJÓSHET". Bronce, como el altar.
 * Mismo material = misma zona de confianza exterior.
 */
#define HASHMAL_HC_NEJOSHET         true  /* Éx 30:18 — bronce */

/**
 * Éx 40:7 — "béin óhel moʻéd uvéin hammizbéaj". ENTRE la Tienda y
 * el altar. Posición liminal: después de autenticarse, antes de
 * servir.
 */
#define HASHMAL_HC_BEIN             true  /* Éx 40:7 — posición entre */

/**
 * Éx 30:21 — "vehayetá lahém JUQQÁT ʻOLÁM". Estatuto perpetuo.
 * El lavamiento es mandato permanente, generación tras generación.
 */
#define HASHMAL_HC_JUQQAT_OLAM      true  /* Éx 30:21 — estatuto perpetuo */

/**
 * La ÚNICA pieza del Mishkán SIN dimensiones prescritas en la Palabra.
 * Éxodo describe todo otro mueble con codos exactos — el kiyór no.
 * Diseño: el health check es FLEXIBLE en escala; crece con la red.
 */
#define HASHMAL_HC_SIN_DIMENSIONES  true  /* única pieza sin medidas */

/**
 * Éx 40:7 — el kiyór se coloca en el paso 6 del levantamiento,
 * después del altar del holocausto (paso 5, Éx 40:6).
 */
#define HASHMAL_HC_ORDEN_BOOT       6     /* Éx 40:7 */

/* ══════════════════════════════════════════════════════════════════
 * ESTRUCTURAS
 * ══════════════════════════════════════════════════════════════════ */

/**
 * Estado por vector (uno por cada yadayim/raglayim/levav/mayim).
 */
typedef struct {
    hashmal_hc_nivel_t nivel;           /* tahor/tame/jata/mavet */
    uint64_t ultimo_check_ms;           /* timestamp último verificar */
    uint64_t checks_pasados;            /* tahor acumulados */
    uint64_t checks_fallidos;           /* tame/jata/mavet acumulados */
} hashmal_hc_vector_estado_t;

/**
 * Reporte del espejo (Éx 38:8 — mar'ót hatstsovót). Snapshot COMPLETO
 * del auto-diagnóstico del kiyór: niveles por vector + contadores +
 * estado global + timestamps. Como las mar'ót mostraban la cara
 * entera, esta struct captura el nodo entero en un momento.
 *
 * Los campos indexados por vector usan el orden del enum:
 *   [0] yadayim, [1] raglayim, [2] levav, [3] mayim
 */
typedef struct {
    uint64_t            timestamp_ms;
    hashmal_hc_estado_t estado;
    hashmal_hc_nivel_t  nivel_global;

    /* Per-vector — orden: yadayim, raglayim, levav, mayim */
    hashmal_hc_nivel_t  nivel[HASHMAL_HC_VECTORES];
    uint64_t            checks_pasados[HASHMAL_HC_VECTORES];
    uint64_t            checks_fallidos[HASHMAL_HC_VECTORES];

    /* Contadores globales del servicio */
    uint64_t            purificaciones;
    uint64_t            contaminaciones;
    uint64_t            muertes_prevenidas;   /* Éx 30:20 veló yamútu */

    /* Ataques y dos niveles (Jn 13:10) */
    uint64_t            ataques_detectados;
    bool                rajats_completo;
    uint64_t            nipto_count;

    /* Meta-reflexión: cuántas veces se ha tomado este espejo */
    uint64_t            reflexiones;
} hashmal_hc_espejo_t;

/**
 * EL KIYÓR — singleton. SIN dimensiones (flexible), de bronce,
 * posicionado ENTRE la tienda y el altar.
 */
typedef struct {
    /* Estado global */
    hashmal_hc_estado_t estado;

    /* Identidad estructural */
    bool     kiyor_nejoshet;             /* Éx 30:18 — bronce */
    bool     bein_ohel_mizbeaj;          /* Éx 40:7 — entre tienda y altar */
    bool     juqqat_olam;                /* Éx 30:21 — estatuto perpetuo */
    bool     conforme_al_tavnit;         /* Éx 25:9 — patrón del monte */

    /* 4 vectores de verificación */
    hashmal_hc_vector_estado_t vectores[HASHMAL_HC_VECTORES];
    hashmal_hc_nivel_t         nivel_global;

    /* Éx 38:8 — espejo (mar'ót): auto-diagnóstico */
    bool     espejo_activo;              /* las mar'ót están montadas */
    uint64_t reflexiones;                /* veces que se examinó */
    uint64_t ultimo_espejo_ms;

    /* Ataques (veló yamútu × 2, Éx 30:20-21) */
    uint64_t ataques_detectados;
    uint64_t ultimo_ataque_ms;

    /* Dos niveles (Jn 13:10 — loúō / níptō) */
    bool     rajats_completo;            /* baño completo ya hecho */
    uint64_t rajats_ms;                  /* cuándo se consagró */
    uint64_t nipto_count;                /* lavados de pies */
    uint64_t ultimo_nipto_ms;

    /* Contadores de servicio */
    uint64_t purificaciones;             /* checks exitosos globales */
    uint64_t contaminaciones;            /* transiciones a tame/jata */
    uint64_t muertes_prevenidas;         /* veló yamútu — salvadas */
} hashmal_hc_t;

/* ══════════════════════════════════════════════════════════════════
 * FUNCIONES PÚBLICAS — agrupadas por archivo
 * ══════════════════════════════════════════════════════════════════ */

/* ── lavacro_core.c — Éx 30:18 + Éx 40:7 ────────────────── */

/**
 * Levanta el kiyór según Éx 30:17-21 + Éx 40:7. Paso 6 del
 * levantamiento. Estado inicial SECO — llenar_agua se hace por
 * separado.
 */
int hashmal_hc_init(void);

/**
 * Drena y desmonta. Nm 4:14 — los kelím se cubren con paños al
 * migrar; aquí se seca el agua y se preserva configuración.
 */
void hashmal_hc_deinit(void);

/**
 * Lectura const del singleton. NULL si no levantado.
 */
const hashmal_hc_t *hashmal_hc_get_lavacro(void);

/**
 * Éx 30:18 — "venatattá shámma máyim". Pone agua. Transición
 * SECO → LLENO. La palabra es la referencia (Ef 5:26).
 */
int hashmal_hc_llenar(void);

/**
 * Estado global del kiyór como lectura pura. Responde al combinado
 * de estado + niveles por vector.
 */
int hashmal_hc_estado_global(void);

/* ── lavacro_vectores.c — Éx 30:19 + Sal 24:3-4 ─────────── */

/**
 * Éx 30:19 — verifica YADAYIM (manos: acciones).
 */
int hashmal_hc_verificar_yadayim(void);

/**
 * Éx 30:19 — verifica RAGLAYIM (pies: rutas / tránsito).
 */
int hashmal_hc_verificar_raglayim(void);

/**
 * Sal 24:4 — verifica LEVAV (corazón: intención).
 */
int hashmal_hc_verificar_levav(void);

/**
 * Éx 30:18 + Ef 5:26 — verifica MAYIM (agua: referencia canónica).
 */
int hashmal_hc_verificar_mayim(void);

/**
 * Estado global de los 4 vectores (lectura pura).
 */
int hashmal_hc_vectores_estado(void);

/* ── lavacro_niveles.c — Éx 30:20-21 (veló yamútu × 2) ──── */

/**
 * Lv 13:3 "vetiméʼ" — sube UN nivel (gradual, un paso) del vector
 * indicado: TAHOR → TAME → JATA → MAVET. Si ya MAVET, idempotente.
 * Si al subir nivel_global alcanza MAVET → estado CONTAMINADO.
 */
int hashmal_hc_nivel_subir(hashmal_hc_vector_t vec);

/**
 * Lv 14:7 "vetihér" — baja UN nivel (gradual) del vector indicado:
 * MAVET → JATA → TAME → TAHOR. Si ya TAHOR, idempotente. Si vec
 * sale de MAVET → muertes_prevenidas++ (veló yamútu cumplido). Si
 * estado era CONTAMINADO y ya ningún vector es MAVET → restaura
 * a PURIFICANDO.
 */
int hashmal_hc_nivel_reducir(hashmal_hc_vector_t vec);

/**
 * Lectura pura del nivel de UN vector. Ante duda (no init o vec
 * inválido) retorna MAVET — principio de muerte preventiva.
 */
hashmal_hc_nivel_t hashmal_hc_nivel_consultar(hashmal_hc_vector_t vec);

/**
 * Lectura pura del nivel GLOBAL (MAX de los 4 vectores).
 * Ante duda → MAVET.
 */
hashmal_hc_nivel_t hashmal_hc_nivel_global_consultar(void);

/**
 * Lectura pura del estado del subsistema de niveles.
 */
int hashmal_hc_niveles_estado(void);

/* ── lavacro_espejos.c — Éx 38:8 mar'ót hatstsovót ──────── */

/**
 * Éx 38:8 "bemar'ót hatstsovót" — toma un SNAPSHOT completo del
 * kiyór en el buffer del caller. Estado + niveles por vector +
 * contadores + ataques + dos niveles. Es lectura PURA del singleton
 * (solo actualiza el contador de reflexiones y timestamp de examen).
 *
 * Funciona incluso con agua CONTAMINADA (Stg 1:23 — el espejo
 * muestra siempre). Solo SECO lo bloquea (no hay superficie donde
 * reflejar).
 */
int hashmal_hc_espejo_examinar(hashmal_hc_espejo_t *reporte);

/**
 * Estado del espejo (lectura pura, sin logs).
 * Opera en LLENO / PURIFICANDO / CONTAMINADO. SECO → ERR_SECO.
 */
int hashmal_hc_espejo_estado(void);

/* ── lavacro_ataques.c — Nm 17:12-13 (vayyárats béin hammetím) ── */

/**
 * Registra un ATAQUE detectado contra un vector específico. Aharón
 * corrió (vayyárats, Nm 17:12) y se puso entre los muertos y los
 * vivos (v.13). Aquí: incrementa ataques_detectados, incrementa
 * checks_fallidos del vector, y llama nivel_subir(vec) para
 * escalar la severidad un paso (gradual, como la plaga que avanza).
 *
 * Opera incluso en estado CONTAMINADO — la vigilancia nunca para
 * (Ez 33:6 el atalaya es culpable si ve y no alerta). Solo SECO
 * bloquea.
 */
int hashmal_hc_ataque_registrar(hashmal_hc_vector_t vec);

/**
 * Estado de detección (lectura pura, sin logs).
 * Opera en LLENO / PURIFICANDO / CONTAMINADO. SECO → ERR_SECO.
 */
int hashmal_hc_ataques_estado(void);

/* ── lavacro_dos_niveles.c — Jn 13:10 (loúō / níptō) ───── */

/**
 * Jn 13:10 loúō — BAÑO COMPLETO. Única vez en el onboarding.
 * Idempotente: si ya se hizo, no re-baña. Transición LLENO →
 * PURIFICANDO.
 */
int hashmal_hc_rajats_completo(void);

/**
 * Jn 13:10 níptō — lavado de PIES, cotidiano por sesión. Requiere
 * rajáts completo previo (Jn 13:10 "ho lelouménos"). Incrementa
 * nipto_count.
 */
int hashmal_hc_nipto_pies(void);

/**
 * Estado de los dos niveles (lectura pura).
 */
int hashmal_hc_dos_niveles_estado(void);

/* ══════════════════════════════════════════════════════════════════
 * PAROKHET - Detector de Shaon Electromagnetico (Añadidura 01)
 *
 * "ve'asita PAROKHET... vehivdilah happarokhet lakem ben haqqodesh
 *  uven qodesh haqqodashim." - Exodo 26:31-33
 * "velo baruaj YHWH... velo bara'ash... ve'ajar ha'esh qol DEMAMAH
 *  DAQQAH." - 1 Reyes 19:11-12
 *
 * Capa sobre el lavacro: mira HACIA AFUERA (ambiente EM) donde los
 * espejos miran hacia adentro (auto-diagnostico). NO anade un 5to
 * vector al enum - los 4 de Ex 30:19 + Sal 24:4 son literales. El
 * ruido EM se expresa como INDICE separado, con 5 niveles biblicos.
 *
 * Ex 30:18 - el kiyor esta entre tienda y altar: mide el entorno.
 * Pieza 04 expone las primitivas de muestreo (WiFi/BLE/LoRa); este
 * sub-modulo consume y clasifica.
 * ══════════════════════════════════════════════════════════════════ */

/**
 * 5 niveles de shaon segun la Palabra. El orden es ascendente en
 * ruido y descendente en aptitud para la Presencia.
 *
 *   DEMAMAH  - 1 Re 19:12 (silencio fino, voz del Padre)
 *   SHAQAT   - Is 30:15 (quietud y confianza)
 *   HAMON    - tumulto moderado
 *   SHAON    - ruido alto
 *   RAASH    - 1 Re 19:11 (temblor, YHWH NO esta aqui)
 */
typedef enum {
    HASHMAL_HC_SHAON_DEMAMAH = 0,   /* 1 Re 19:12 */
    HASHMAL_HC_SHAON_SHAQAT  = 1,   /* Is 30:15 */
    HASHMAL_HC_SHAON_HAMON   = 2,   /* tumulto moderado */
    HASHMAL_HC_SHAON_SHAON   = 3,   /* ruido alto */
    HASHMAL_HC_SHAON_RAASH   = 4,   /* 1 Re 19:11 */
} hashmal_hc_shaon_nivel_t;

/**
 * Umbrales superiores de cada banda (indice 0..100).
 *   [ 0 .. DEMAMAH_MAX ] -> DEMAMAH
 *   [ DEMAMAH_MAX+1 .. SHAQAT_MAX ] -> SHAQAT
 *   ... y asi hasta RAASH.
 */
#define HASHMAL_HC_SHAON_DEMAMAH_MAX  20
#define HASHMAL_HC_SHAON_SHAQAT_MAX   40
#define HASHMAL_HC_SHAON_HAMON_MAX    60
#define HASHMAL_HC_SHAON_SHAON_MAX    80
#define HASHMAL_HC_SHAON_RAASH_MAX   100

/**
 * Muestra cruda del entorno EM en un instante. 3 fuentes: WiFi
 * (Ex 26:3 isha el ajota), BLE (Ex 28:34 pa'amon), LoRa (Ex 25:31
 * geza'). timestamp_ms marca cuando se tomo.
 *
 *   wifi_rssi_max  = INT8_MIN -> "sin dato" (fuente no muestreada)
 *   ble_rssi_medio = 0 cuando vecinos = 0
 */
typedef struct {
    uint8_t  wifi_ap_count;              /* APs visibles */
    int8_t   wifi_rssi_max;              /* dBm del mas fuerte */
    uint8_t  ble_vecinos;                /* cuenta BLE activa */
    int8_t   ble_rssi_medio;             /* dBm medio */
    int8_t   lora_rssi_floor;            /* dBm suelo LoRa */
    uint64_t timestamp_ms;
} hashmal_hc_parokhet_muestras_t;

/* ── lavacro_parokhet.c ─────────────────────────────────── */

/**
 * Ex 26:31-33 - toma una muestra cruda del ambiente EM. Consulta
 * las 3 primitivas de Pieza 04 (WiFi / BLE / LoRa) y rellena *out.
 * Una fuente caida no invalida las otras (Zac 4:3 dos olivos:
 * redundancia).
 *
 * Precondiciones: kiyor levantado y estado != SECO.
 */
int hashmal_hc_parokhet_scan(hashmal_hc_parokhet_muestras_t *out);

/**
 * Calcula el indice 0..100 y el nivel de shaon. Llama internamente
 * a parokhet_scan y combina los 3 componentes EN PARTES IGUALES
 * (Ex 30:34 bad bebad - ni ponderacion ni priorizacion). El nivel
 * se deriva del indice via los umbrales DEMAMAH_MAX..SHAON_MAX.
 *
 * RAASH (>80) emite LOG_W. NO registra ataque automatico - la
 * decision de escalar a ataque la toma el caller.
 */
int hashmal_hc_parokhet_indice(uint8_t *indice_0_100,
                                hashmal_hc_shaon_nivel_t *nivel);

/**
 * Ex 26:31-33 parokhet. Activa/desactiva el Shabat EM delegando en
 * Pieza 04 (flag cooperativo). Al activar: WiFi + BLE quiesados;
 * LoRa queda como geza' tamid (Ex 27:21). Al desactivar: se
 * restaura activa=disponible donde el hardware lo permite.
 *
 * Ex 30:18 - el kiyor MIDE y AVISA. Apagar transporte es oficio de
 * Pieza 04; aqui solo se pide.
 */
int hashmal_hc_parokhet_shabat(bool activar);

/* ══════════════════════════════════════════════════════════════════
 * MESHULASH - El Cordon Triple (Añadidura 13)
 *
 * "tovim hashshenayim min haejad asher yesh lahem sakhar tov
 *  baamalam." - Eclesiastes 4:9
 * "MEJORES son DOS que UNO, porque tienen mejor recompensa de su
 *  trabajo."
 *
 * "ki im YIPPOLU haejad YAQIM et javero; ve'iLO ha'ejad
 *  SHEYIPPOL ve'EIN SHENI laqimo." - Eclesiastes 4:10
 * "Si CAEN, el uno LEVANTA al otro; pero AY del SOLO que cae,
 *  porque NO HAY SEGUNDO que lo levante."
 *
 * "veHAJUT HAMSHULASH lo bimherah YINNATEQ." - Eclesiastes 4:12
 * "El cordon TRIPLE no se ROMPE pronto."
 *
 * "'al pi shenayim 'edim o sheloshah." - Deuteronomio 19:15
 * "Por boca de DOS o TRES." (verificacion multiple)
 *
 * "hou gar eisin DUO e TREIS synegmenoi eis to emon onoma, ekei
 *  eimi en meso auton." - Mateo 18:20
 * "Donde estan DOS o TRES reunidos en mi nombre, alli estoy yo
 *  en medio de ellos."
 *
 * Capa: este sub-modulo cuenta CONEXIONES como vector de salud y
 * publica estado + intent de busqueda. Pull a Pieza 04
 * (vecinos_contar). El "buscar" en fase 1 es solo intent: caller
 * con acceso a otras piezas (Tsofeh / Canal 3) ejecuta la busqueda
 * concreta. La integracion con Goel (Añadidura 12) se expone via
 * predicate (`puede_ser_goel`) sin wiring forzado.
 * ══════════════════════════════════════════════════════════════════ */

/**
 * Umbral biblico - Ec 4:12 jaJUT haMSHULASH. Tres, ni dos ni cuatro.
 */
#define HASHMAL_MESHULASH_MIN  3u

/**
 * Cuatro estados segun conexiones activas:
 *
 *   AISLADO    (0)   nikrat temporal - beacon de auxilio, ultimo recurso
 *   FRAGIL     (1)   ay del solo que cae (Ec 4:10) - busqueda alta
 *   FUNCIONAL  (2)   mejor dos que uno (Ec 4:9) - busqueda pasiva del 3o
 *   ROBUSTO    (3+)  cordon completo (Ec 4:12) - apto para servir como goel
 */
typedef enum {
    HASHMAL_MESHULASH_AISLADO   = 0,
    HASHMAL_MESHULASH_FRAGIL    = 1,
    HASHMAL_MESHULASH_FUNCIONAL = 2,
    HASHMAL_MESHULASH_ROBUSTO   = 3,
} hashmal_meshulash_estado_t;

/**
 * Snapshot del estado actual + flag de intent de busqueda.
 *
 *   busqueda_pendiente: el modulo recomienda buscar conexiones (true
 *   cuando estado < ROBUSTO Y _buscar fue invocado). Caller con
 *   acceso a Pieza 04/09 ejecuta el escaneo y limpia el flag al
 *   reportar nuevas conexiones (via _actualizar).
 */
typedef struct {
    hashmal_meshulash_estado_t  estado;
    uint8_t                     conexiones;
    bool                        busqueda_pendiente;
    int64_t                     timestamp_ms;
} hashmal_meshulash_snapshot_t;

/* -- lavacro_meshulash.c - Añadidura 13 ────────────────── */

/**
 * Ec 4:9-12 - cuenta conexiones activas (pull a Pieza 04
 * vecinos_contar) y clasifica en uno de los 4 estados. Si pasa de
 * >= MIN a < MIN, NO dispara busqueda automatica - hay que llamar
 * _buscar explicitamente.
 *
 * @return OK si exito; ERR_MESHULASH si comm rechaza la consulta.
 */
int hashmal_hc_meshulash_actualizar(void);

/**
 * Lectura pura del snapshot.
 */
int hashmal_hc_meshulash_estado(hashmal_meshulash_snapshot_t *out);

/**
 * Marca intent de busqueda. Fase 1: solo loguea estrategia
 * priorizada (WiFi -> LoRa -> Tsofim -> Canal 3) y pone flag
 * busqueda_pendiente=true. Caller externo ejecuta escaneo
 * concreto y reporta resultado via siguiente _actualizar.
 *
 * No-op si estado ya ROBUSTO - el cordon triple no necesita buscar.
 */
int hashmal_hc_meshulash_buscar(void);

/**
 * Predicate Ec 4:12 - puede este nodo servir como goel
 * (Añadidura 12). Solo cierto cuando estado == ROBUSTO. Permite
 * a Goel checar antes de proponer self como redentor sin tener
 * que importar este header del lado contrario.
 */
bool hashmal_hc_meshulash_puede_ser_goel(void);

/* ══════════════════════════════════════════════════════════════════
 * TSUR - Mayim min haTsur - Provision de Emergencia (Añadidura 16)
 *
 * "hineni 'omed lefaneikha sham 'al hatsur beJOREV vehikkita
 *  vatsur veyatsu mimmennu MAYIM veshatah ha'am." - Exodo 17:6
 * "Yo ESTARE delante de ti alli sobre la ROCA en HOREB; GOLPEARAS
 *  la roca y saldran de ella AGUAS, y BEBERA el pueblo."
 *
 * "qaj et hammatteh vehaqhel et ha'edah... vediBARTEM el HASSELA
 *  le'eineihem venatan meimav." - Numeros 20:8
 * "Toma la vara, reune la asamblea... HABLAREIS a la PENA y dara
 *  sus aguas." (segunda vez: hablar, no golpear).
 *
 * "hammotsi lekha mayim mitsur hajalamish." - Deuteronomio 8:15
 * "El que SACO agua del PEDERNAL (lo mas duro)."
 *
 * "yevaqqa' tsurim bammidbar... vayyotsi nozlim missela'." - Sal 78:15-16
 * "HENDIO ROCAS en el desierto... saco CORRIENTES de la pena."
 *
 * "vayyazuvu mayim vayyivqa' tsur." - Isaias 48:21
 * "Corrieron AGUAS - hendio la ROCA."
 *
 * Modelo: 4 niveles segun cuantos canales reportan vida.
 *   NORMAL    - todos los canales (no hay sed)
 *   SEQUIA    - mayoria viva (Ex 17:3 vayyitsma ha'am - hubo sed)
 *   DESIERTO  - quedan pocos (Ex 17:1 ein mayim - sin agua)
 *   TSUR      - cero canales (Ex 17:6 vehikkita vatsur - golpea)
 *
 * Capa: diagnostico puro. _evaluar(activos, total) clasifica y
 * publica. NO activa Canal 3 ni hardware - eso es wiring fase 2.
 * La roca SIEMPRE esta lista (Ex 17:6 hineni 'omed) - el modulo
 * no requiere init.
 * ══════════════════════════════════════════════════════════════════ */

#define HASHMAL_TSUR_CANALES_TOTAL    7u   /* Ex 25:31-40 menorah de 7 */
#define HASHMAL_TSUR_UMBRAL_SEQUIA    4u   /* >= 4 de 7 = mayoria viva */
#define HASHMAL_TSUR_UMBRAL_DESIERTO  1u   /* >= 1 = aun queda algo */

/**
 * Niveles de provision de agua (canales activos en la red).
 *
 *   NORMAL   - todos los canales (== TOTAL)
 *   SEQUIA   - mayoria viva (>= UMBRAL_SEQUIA pero < TOTAL)
 *   DESIERTO - pocos quedan (>= UMBRAL_DESIERTO pero < UMBRAL_SEQUIA)
 *   TSUR     - cero canales (la roca debe ser golpeada)
 */
typedef enum {
    HASHMAL_TSUR_NIVEL_NORMAL   = 0,
    HASHMAL_TSUR_NIVEL_SEQUIA   = 1,
    HASHMAL_TSUR_NIVEL_DESIERTO = 2,
    HASHMAL_TSUR_NIVEL_TSUR     = 3,
} hashmal_tsur_nivel_t;

/**
 * Snapshot del estado actual del modulo.
 */
typedef struct {
    hashmal_tsur_nivel_t nivel;
    uint8_t              canales_activos;
    uint8_t              total_canales;
    int64_t              timestamp_ms;
    uint32_t             activaciones_historicas;  /* veces que llego a TSUR */
} hashmal_tsur_info_t;

/* -- lavacro_tsur.c - Añadidura 16 ─────────────────────── */

/**
 * Ex 17:6 - clasifica la salud de la red por canales vivos vs total.
 * Pull: el caller (futuro healthcheck principal) le pasa los datos
 * de comm; este modulo solo evalua y publica. Si la transicion
 * empeora hacia TSUR, incrementa activaciones_historicas.
 *
 * @return el nivel calculado (tambien queda persistido).
 */
hashmal_tsur_nivel_t hashmal_tsur_evaluar(uint8_t canales_activos,
                                            uint8_t total_canales);

/**
 * Lectura pura: ultimo nivel publicado por _evaluar.
 */
hashmal_tsur_nivel_t hashmal_tsur_nivel_actual(void);

/**
 * Lectura pura: snapshot completo (nivel + contadores + timestamps).
 */
int hashmal_tsur_info(hashmal_tsur_info_t *out);

/**
 * Helper de consulta rapida: true si nivel >= DESIERTO. Permite a
 * otros modulos preguntar "estoy en emergencia?" sin importar el
 * detalle del nivel exacto.
 */
bool hashmal_tsur_es_emergencia(void);

/* ══════════════════════════════════════════════════════════════════
 * VERIFICADO #04 — P14 — Auditoría independiente del Sha'ar HaAtrio
 *
 * "élleh fequdéi haMishkán... ʻavodát haLeviím beyád Itamár ben
 *  Aharón hakohén." — Éxodo 38:21
 *
 * pequdéi (H6485) = inventario / cómputo / auditoría. Tras completar
 * el Mishkán, viene la auditoría INMEDIATA. Crítico: el auditor
 * (Itamar) es DIFERENTE del constructor (Betsalel). El módulo NO
 * se verifica a sí mismo. hashmal_perimeter expone estado read-only;
 * hashmal_healthcheck es el auditor externo (paralelo Lavacro: el
 * sacerdote se mira en el espejo, no se mira a sí mismo).
 *
 * Caller (hashmal_main, en INIT antes de TRANSICION) DEBE invocar
 * esta función después de hashmal_perimeter_shaar_abrir().
 *
 * Pull pattern: leemos via hashmal_perimeter_shaar_info /
 * _shaar_base_estado / _shaar_sesion_info — sin que perimeter
 * sepa de healthcheck.
 * ══════════════════════════════════════════════════════════════════ */

typedef struct {
    bool portal_abierto;             /* g_estado == ABIERTO */
    bool stealth_activo;             /* P6 — leʻummat (Éx 38:18) */
    bool sin_sesiones_huerfanas;     /* sesiones ACTIVO requieren intent + 4 cols */
    uint8_t bases_configuradas;      /* P2 — 0..4 (Phase 1 acepta 0) */
    uint8_t sesiones_activas;        /* eco de _shaar_info */
    uint8_t sesiones_pendientes;     /* eco de _shaar_info */
    bool veredicto_ok;               /* AND de los 3 booleanos críticos */
} hashmal_hc_shaar_auditoria_t;

/**
 * Auditoría completa del sha'ar tras inicialización. Validaciones:
 *   1) portal abierto                       (P4 masakh activo)
 *   2) stealth flag activo                  (P6 leʻummat)
 *   3) ninguna sesión ACTIVO sin intent     (P8 + P1 + P5 acoplados)
 *
 * @return OK si veredicto_ok=true; ERR_SHAAR_AUDIT si alguna
 *         invariante crítica falla; ERR_NULL si out es NULL.
 *
 * El struct out se rellena SIEMPRE (incluso en error) para que el
 * caller vea exactamente qué falló — Itamar no oculta su informe.
 */
int hashmal_hc_auditar_shaar(hashmal_hc_shaar_auditoria_t *out);

/* ══════════════════════════════════════════════════════════════════
 * P17 — JOQAT OLAM — ESTATUTO PERPETUO, SIN BYPASS
 *
 * "verajatsu yadeihem veragleihem VELO YAMUTU vehayeta lahem
 *  JUQQAT 'OLAM lo ulezar'o ledorotam." — Éxodo 30:21
 * "Y será para él estatuto PERPETUO, para él y para su simiente
 *  por sus generaciones."
 *
 * JUQQAT 'OLAM (H2706+H5769) = decreto constitucional permanente.
 * NO hay disable flag. NO hay bypass. El lavamiento es perpetuo.
 *
 * Compile-time assertion: el código MISMO refleja esta verdad.
 * ══════════════════════════════════════════════════════════════════ */

/**
 * P17 — Verificación: hashmal_hc es joq ʻolám (estatuto perpetuo).
 * Siempre retorna true. Sin excepciones. Sin bypass.
 *
 * Referencia bíblica: Éx 30:21 "vehayetá lahém juqqát ʻolám"
 * Strong: H2706 (juqqáh, decreto), H5769 (ʻolám, perpetuo).
 *
 * @return true siempre — el healthcheck NO se puede deshabilitar.
 */
bool hashmal_hc_es_joq_olam(void);

/* ══════════════════════════════════════════════════════════════════
 * P36+P37 — EPHEMERAL CHALLENGE ZERO-KNOWLEDGE
 *
 * "hamayim... BIKLI JÁRES" + "veKATÁV... uMAJÁH" — Números 5:17, 5:23
 * Vaso de barro que se DESTRUYE después de usar.
 *
 * keli járes (H2789) = vessel of clay, DESTROYED after single use.
 * Desafío one-time, token efímero, jamás reutilizable.
 * ══════════════════════════════════════════════════════════════════ */

/**
 * Un desafío efímero: token único, timestamp, bandera de destrucción.
 * Números 5:17 — el vaso es de barro, se usa UNA sola vez.
 */
typedef struct {
    uint8_t  token[32];              /* Challenge de 32 bytes */
    uint64_t timestamp_ms;           /* Cuándo fue generado */
    bool     single_use;             /* Flag: one-time only */
    bool     destruido;              /* Flag: ya desintegrado (MAHJAH) */
} hashmal_hc_challenge_t;

/**
 * P36 — Genera un desafío efímero nuevo.
 *
 * Números 5:17 — "máyim qedoshím BIKLI JÁRES"
 * Strong: H2789 keli (vessel), járes (clay).
 *
 * El token es aleatorio, single_use=true, destruido=false.
 *
 * @param[out] out Desafío recién creado (si NULL, retorna ERR_NULL).
 * @return OK si éxito; ERR_NULL si out es NULL.
 */
int hashmal_hc_challenge_generar(hashmal_hc_challenge_t *out);

/**
 * P36+P37 — Aplica el desafío a un nodo y lo DESTRUYE.
 *
 * Números 5:23 — "veKATÁV... uMAJÁH" (ESCRIBIR y BORRAR/DESTRUIR).
 * Strong: H3789 kátav (write), H4229 majáh (blot out/erase).
 *
 * El token se valida, se usa para verificación, luego se anula:
 *   - destruido = true
 *   - token bytes se rellenan a cero (MAHJAH)
 *   - single_use = false (ya no es usable)
 *
 * @param[in,out] ch Desafío a aplicar y destruir.
 * @param[in] nodo_id Identificador del nodo a verificar.
 * @param[out] resultado true si el nodo pasó la prueba.
 * @return OK si éxito; ERR_NULL si ch es NULL; ERR_TAME si ya destruido.
 */
int hashmal_hc_challenge_aplicar(hashmal_hc_challenge_t *ch,
                                  uint8_t nodo_id,
                                  bool *resultado);

/**
 * P36 — Verifica si un desafío aún es válido (no destruido).
 *
 * Números 5:17 — el vaso debe estar íntegro; una vez roto, no vale.
 *
 * @param[in] ch Desafío a consultar.
 * @return true si aún válido (destruido=false); false si ya MAHJAH.
 */
bool hashmal_hc_challenge_es_valido(const hashmal_hc_challenge_t *ch);

/* ══════════════════════════════════════════════════════════════════
 * P54 — TRES UMBRALES DE RESILIENCIA ESCALONADOS
 *
 * "tovim hashshenayim min haejad... mejores son DOS que UNO...
 *  veHAJUT HAMSHULASH lo bimherah YINNATEQ." — Eclesiastés 4:9-12
 *
 * Tres niveles según conexiones activas:
 *   EJAD (1)        — vulnerable, alerta
 *   SHENAYIM (2)    — funcional, soporte mutuo
 *   MESHULLASH (3+) — resistente, completo
 * ══════════════════════════════════════════════════════════════════ */

/**
 * P54 — Tres umbrales bíblicos de resiliencia.
 *
 * Eclesiastés 4:9-12: la progresión de 1 → 2 → 3 conexiones
 * refleja la palabra del Padre: ejad (uno), shenayim (dos),
 * meshullash (tres).
 */
typedef enum {
    HASHMAL_HC_UMBRAL_EJAD       = 1u,   /* Ec 4:10 "yippól" — cae solo */
    HASHMAL_HC_UMBRAL_SHENAYIM   = 2u,   /* Ec 4:11 "yaʻamdu" — se levanta */
    HASHMAL_HC_UMBRAL_MESHULLASH = 3u,   /* Ec 4:12 cordón triple no se rompe */
} hashmal_hc_umbral_resiliencia_t;

/**
 * P54 — Evalúa el umbral de resiliencia según conexiones activas.
 *
 * Ec 4:9-12 — la palabra de YHWH distingue 1, 2, 3:
 *   - 1 conexión: ay del solo que cae (yippol - v.10)
 *   - 2 conexiones: mejor dos que uno (shenim - v.9), se levanta uno al otro
 *   - 3+ conexiones: cordón triple no se rompe pronto (meshullash - v.12)
 *
 * Strong: H259 ejad (one), H8147 shnáyim (two), H7969 shálosh (three).
 *
 * @param[in] conexiones_activas Cantidad de conexiones P2P activas.
 * @return El umbral clasificado (EJAD, SHENAYIM, o MESHULLASH).
 */
hashmal_hc_umbral_resiliencia_t hashmal_hc_umbral_evaluar(
    uint8_t conexiones_activas);

/* ══════════════════════════════════════════════════════════════════
 * P56 — MÉTRICA SAJÁR THROUGHPUT CONJUNTO
 *
 * "tovim hashshenayim min haejad asher yesh lahem SAJAR TOV
 *  BAAMALAM." — Eclesiastés 4:9
 * "Mejores son DOS que UNO, porque tienen mejor RECOMPENSA de su
 *  TRABAJO."
 *
 * sajár (H7939) = recompensa, paga, fruto del trabajo.
 * Mide la PRODUCTIVIDAD CONJUNTA de dos nodos cooperando.
 * ══════════════════════════════════════════════════════════════════ */

/**
 * P56 — Mide el throughput/productividad conjunto entre dos nodos.
 *
 * Eclesiastés 4:9 — "sajár tov baʻamalám": buena recompensa en su
 * trabajo. En la red P2P, la métrica es el FRUTO CONJUNTO: bytes
 * transmitidos, mensajes procesados, consenso logrado, redundancia
 * efectiva entre dos nodos.
 *
 * Strong: H7939 sajár (reward, wages), H2896 tov (good).
 *
 * Fase 1: métrica simple de throughput. Fase 2: incorporar eficiencia
 * energética, latencia, ratio error, etc.
 *
 * @param[in] nodo_a Identificador del primer nodo.
 * @param[in] nodo_b Identificador del segundo nodo.
 * @param[out] sajar_out Resultado de la métrica (bytes/ms o similar).
 *                       NULL si no hay conexión activa.
 * @return OK si éxito; ERR_NULL si sajar_out es NULL;
 *         ERR_TAME si conexión no disponible (no en redundancia).
 */
int hashmal_hc_sajar_medir(uint8_t nodo_a, uint8_t nodo_b,
                            uint32_t *sajar_out);

#ifdef __cplusplus
}
#endif

#endif /* HASHMAL_HEALTHCHECK_H */
