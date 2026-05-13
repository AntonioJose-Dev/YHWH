/**
 * ═══════════════════════════════════════════════════════════════════
 * PIEZA 08 — EL ACEITE PERPETUO (SHÉMEN ZÁYIT ZAJ KATÍT) — hashmal_power
 * Éxodo 27:20-21 + Levítico 24:1-4 + Zacarías 4:1-6 + Mateo 25:1-13
 * ═══════════════════════════════════════════════════════════════════
 *
 * "veʼattá tetsavvé et benéi Yisraʼél veyiqjú eléja SHÉMEN ZÁYIT
 *  ZAJ KATÍT LAMMAʼÓR lehaʻalót NÉR TAMÍD" — Éxodo 27:20
 * "Tú ORDENARÁS a los hijos de Israel que te traigan ACEITE DE
 *  OLIVA PURO, MACHACADO, PARA LA LUMINARIA, para hacer ASCENDER
 *  lámpara CONTINUAMENTE."
 *
 * "beʼóhel moʻéd MIJÚTS LAPPARÓJET ashér ʻal haʻédut yaʻarój otó
 *  Aharón uvanáv meʻÉREV ʻad BÓQER lifnéi YHWH JUQQÁT ʻOLÁM
 *  ledorotám meʼét benéi Yisraʼél" — Éxodo 27:21
 * "En la Tienda del Encuentro, FUERA DEL VELO que está sobre el
 *  Testimonio, lo DISPONDRÁN Aarón y sus hijos de TARDE a MAÑANA
 *  delante de YHWH — estatuto PERPETUO por sus generaciones."
 *
 * ─── VOCABULARIO ───
 *
 *   tetsavvé       (תְּצַוֶּה, H6680 Piel)   ORDENARÁS. Mandato —
 *                                             no sugerencia.
 *   shémen         (שֶׁמֶן, H8081)           ACEITE. La energía.
 *   záyit          (H2132)                    OLIVA. Árbol que resiste
 *                                             sequía y revive.
 *   zaj            (זַךְ, H2134)             PURO, claro, sin
 *                                             sedimento. Energía
 *                                             sin contaminación.
 *   katít          (כָּתִית, H3795)          MACHACADO, prensado.
 *                                             Primera prensa bajo
 *                                             presión — máxima pureza.
 *   lammaʼór       (לַמָּאוֹר, H3974)        PARA LA LUMINARIA. Un
 *                                             solo propósito: sostener
 *                                             la luz.
 *   lehaʻalót      (לְהַעֲלֹת, H5927 Hifil)  Hacer ASCENDER. La
 *                                             llama SUBE; no se
 *                                             "enciende" — se eleva.
 *   nér            (נֵר, H5216)              LÁMPARA. El canal que
 *                                             recibe el aceite.
 *   tamíd          (תָּמִיד, H8548)          CONTINUO, perpetuo —
 *                                             sin downtime.
 *   yaʻarój        (יַעֲרֹךְ, H6186)         DISPONDRÁ, ordenará.
 *                                             Aarón arregla las
 *                                             lámparas cada día.
 *   meʻérev bóqer  (מֵעֶרֶב עַד בֹּקֶר)      De TARDE a MAÑANA.
 *                                             Opera en la oscuridad.
 *   juqqát ʻolám   (חֻקַּת עוֹלָם, H2708+H5769) ESTATUTO PERPETUO.
 *                                             Protocolo inmutable.
 *
 * ─── DISTINCIÓN DE LA PIEZA 13 ───
 *
 *   Pieza 08 (ESTE archivo) — shémen záyit zaj katít
 *     Aceite PURO de olivas machacadas.
 *     Un solo propósito: lammaʼór — ENERGÍA para las LÁMPARAS.
 *     Éx 27:20.
 *
 *   Pieza 13 (hashmal_keygen) — shémen hamishjá
 *     Aceite de la UNCIÓN.
 *     4 especias añadidas al aceite base (Éx 30:23-24).
 *     Propósito: consagrar / AUTORIZAR / generar claves.
 *
 *   MISMA base de oliva; propósitos DISTINTOS. Este header se
 *   ocupa EXCLUSIVAMENTE del aceite de las lámparas.
 *
 * ─── TESTIGOS ADICIONALES ───
 *
 *   Lv 24:1-4    — hamenorá hatehará (la menorá PURA ANTES de
 *                   recibir aceite). Canal limpio primero.
 *   Zac 4:3,11-12 — DOS OLIVOS a cada lado, vertiendo aceite
 *                   dorado por tubos. Redundancia (solar + batería).
 *   Zac 4:6      — "lo vejáyil veló vejóaj ki im berují" — no
 *                   por fuerza bruta, sino elegancia del Espíritu.
 *   Mt 25:1-13   — 10 vírgenes, el aceite NO SE COMPARTE. Cada
 *                   nodo necesita su propia fuente.
 *   Mt 26:36 /
 *   Gat shemaním — Getsemaní = "prensa de aceites". El aceite
 *                   más puro viene del sacrificio más grande.
 *
 * Autor: Antonio José Marín Soto
 * Proyecto: HASHMAL-CORE
 */
#ifndef HASHMAL_POWER_H
#define HASHMAL_POWER_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ══════════════════════════════════════════════════════════════════
 * ENUMS FUNDAMENTALES
 * ══════════════════════════════════════════════════════════════════ */

/**
 * 4 niveles de energía según capacidad disponible. El peor escala
 * gradual hacia adelante (SHEFA → DAI → MATSOQ → JOSHEJ), como el
 * aceite que mengua cuando no se repone.
 *
 *   SHEFA   (שֶׁפַע, "abundancia")        — todas las lámparas encendidas
 *   DAI     (דַּי, H1767 "suficiente")   — operación normal
 *   MATSOQ  (מָצוֹק, H4689 "estrechez")  — modo ahorro
 *   JOSHEJ  (חֹשֶׁךְ, H2822 "oscuridad") — solo emergencia
 */
typedef enum {
    HASHMAL_POWER_NIVEL_SHEFA  = 0,  /* abundancia */
    HASHMAL_POWER_NIVEL_DAI    = 1,  /* suficiente */
    HASHMAL_POWER_NIVEL_MATSOQ = 2,  /* estrechez — Sal 107:6 */
    HASHMAL_POWER_NIVEL_JOSHEJ = 3,  /* oscuridad — Gn 1:2 */
} hashmal_power_nivel_t;

/**
 * Zac 4:3,11-12 — los DOS OLIVOS que vierten aceite dorado a la
 * menorá, uno a cada lado. En firmware: dos fuentes de energía
 * redundantes.
 *
 *   YAMIN  (yamín, derecha)   — solar (lo que amanece primero)
 *   SMOL   (semol, izquierda) — batería (almacén permanente)
 */
typedef enum {
    HASHMAL_POWER_OLIVO_YAMIN = 0,  /* Zac 4:3 — olivo derecho */
    HASHMAL_POWER_OLIVO_SMOL  = 1,  /* Zac 4:3 — olivo izquierdo */
} hashmal_power_olivo_t;

/**
 * Estados del subsistema.
 *
 *   APAGADO    — aún no iniciado (antes de init)
 *   ENCENDIDO  — inicializado pero ciclo perpetuo no activo
 *   TAMID      — Éx 27:20 nér tamíd — ciclo perpetuo operativo
 */
typedef enum {
    HASHMAL_POWER_ESTADO_APAGADO   = 0,
    HASHMAL_POWER_ESTADO_ENCENDIDO = 1,  /* inicializado */
    HASHMAL_POWER_ESTADO_TAMID     = 2,  /* Éx 27:20 — nér tamíd activo */
} hashmal_power_estado_t;

/* ══════════════════════════════════════════════════════════════════
 * CONSTANTES DEL PADRE — cada número tiene versículo
 * ══════════════════════════════════════════════════════════════════ */

/**
 * Umbrales de calidad del aceite (porcentaje de carga combinada):
 *
 *   ZAJ     >= 80%  — aceite puro (zaj, Éx 27:20). Operación plena.
 *   KATIT   >= 50%  — aceite prensado bajo presión aún sostiene.
 *   JOSHEJ  < 20%   — oscuridad, solo emergencia.
 *
 * Los umbrales son políticas de diseño; no números literales del
 * texto bíblico. El texto fija los CONCEPTOS (zaj, katít); los
 * porcentajes son nuestro tavnít operacional.
 */
#define HASHMAL_POWER_UMBRAL_ZAJ      80u  /* Éx 27:20 — zaj (puro) */
#define HASHMAL_POWER_UMBRAL_KATIT    50u  /* Éx 27:20 — katít (prensado) */
#define HASHMAL_POWER_UMBRAL_JOSHEJ   20u  /* Gn 1:2 — jóshej (oscuridad) */

/**
 * Ciclos del servicio perpetuo. Éx 27:21 "meʻérev ʻad bóqer":
 * de tarde a mañana. Dos ritmos — normal (día) y ahorro (noche).
 */
#define HASHMAL_POWER_CICLO_TAMID_MS  30000u  /* 30 s — monitoreo normal */
#define HASHMAL_POWER_CICLO_EREV_MS   60000u  /* 60 s — nocturno/ahorro */

/**
 * Éx 40:9 — la unción (y con ella, la energía operativa) se coloca
 * en el paso 8 del levantamiento. Pieza 08 es el aceite de lámparas,
 * complemento de pieza 13 (aceite de unción).
 */
#define HASHMAL_POWER_ORDEN_BOOT       8   /* Éx 40:9-11 */

/* ══════════════════════════════════════════════════════════════════
 * ESTRUCTURAS
 * ══════════════════════════════════════════════════════════════════ */

/**
 * Lectura instantánea de un olivo (Zac 4 — dos fuentes). Porcentaje
 * 0..100, voltaje en mV, y flag de actividad (puede estar físicamente
 * presente pero desconectado/drenado).
 */
typedef struct {
    hashmal_power_olivo_t olivo;        /* YAMIN o SMOL */
    uint8_t               porcentaje;   /* 0..100 */
    uint16_t              voltaje_mv;   /* milivoltios */
    bool                  activo;       /* fuente en servicio */
} hashmal_power_lectura_t;

/**
 * Estado global del subsistema. Combina lectura de los 2 olivos,
 * el nivel agregado, y metadatos del ciclo activo. Pensado para
 * telemetría completa en una sola llamada.
 */
typedef struct {
    hashmal_power_nivel_t   nivel;               /* SHEFA / DAI / MATSOQ / JOSHEJ */
    hashmal_power_estado_t  estado;              /* APAGADO / ENCENDIDO / TAMID */
    hashmal_power_lectura_t olivo_yamin;         /* Zac 4 — derecho */
    hashmal_power_lectura_t olivo_smol;          /* Zac 4 — izquierdo */
    uint8_t                 porcentaje_combinado;/* 0..100 */
    uint32_t                ciclo_actual_ms;     /* duración del ciclo en curso */
    int64_t                 timestamp_ms;        /* momento del snapshot */
} hashmal_power_estado_energia_t;

/* ══════════════════════════════════════════════════════════════════
 * CÓDIGOS DE ERROR
 * ══════════════════════════════════════════════════════════════════ */

typedef enum {
    HASHMAL_POWER_OK          =  0,
    HASHMAL_POWER_ERR_NULL    = -1,   /* puntero NULL */
    HASHMAL_POWER_ERR_INIT    = -2,   /* subsistema no inicializado */
    HASHMAL_POWER_ERR_YA_INIT = -3,   /* ya inicializado (idempotente) */
    HASHMAL_POWER_ERR_DRIVER  = -4,   /* fallo del oro de Egipto (driver) */
    HASHMAL_POWER_ERR_ZAJ     = -5,   /* Éx 27:20 — bajo umbral de pureza */
    HASHMAL_POWER_ERR_JOSHEJ  = -6,   /* Gn 1:2 — oscuridad, crítico */
    HASHMAL_POWER_ERR_MENORA  = -7,   /* Lv 24:2-4 — menorá no tehará */
} hashmal_power_err_t;

/* ══════════════════════════════════════════════════════════════════
 * FUNCIONES PÚBLICAS — agrupadas por archivo
 * ══════════════════════════════════════════════════════════════════ */

/* ── aceite_energia.c — Éx 27:20 tetsavvé ──────────────── */

/**
 * Éx 27:20 "tetsavvé" — inicializa el subsistema de energía.
 * Configura ADCs, lee olivos por primera vez, marca estado
 * ENCENDIDO. El ciclo perpetuo (tamíd) se arranca aparte con
 * `tamid_iniciar`.
 */
int  hashmal_power_init(void);

/**
 * Desmonta. Nm 4:9-10 — se cubre la menorá con tejélet antes de
 * migrar; análogamente aquí preservamos configuración, apagamos
 * ciclo perpetuo, liberamos recursos de driver.
 */
void hashmal_power_deinit(void);

/**
 * Estado operativo global (lectura pura sin logs). OK si inicializado;
 * ERR_INIT si no. Patrón consistente con los *_estado de otras piezas.
 */
int  hashmal_power_estado(void);

/**
 * ¿El nér tamíd está encendido? true si estado == TAMID (ciclo
 * perpetuo operando). Sin logs.
 */
int  hashmal_power_activo(void);

/* ── aceite_tamid.c — Éx 27:20-21 nér tamíd ─────────────── */

/**
 * Éx 27:20 "lehaʻalót nér tamíd" — inicia el ciclo perpetuo.
 * Transición ENCENDIDO → TAMID. Idempotente si ya en TAMID.
 */
int  hashmal_power_tamid_iniciar(void);

/**
 * Detiene el ciclo perpetuo — solo para shutdown controlado (deinit).
 * Lv 6:5 advierte "lo tijbé" (no se apagará). Esta función existe
 * solo para desmontaje, no uso rutinario. Transición TAMID →
 * ENCENDIDO (configuración preservada).
 */
int  hashmal_power_tamid_detener(void);

/**
 * Éx 27:21 "yaʻarój... meʻérev ʻad bóqer" — UN ciclo de monitoreo:
 * lee olivos, evalúa nivel, aplica política (ciclo normal vs ahorro
 * nocturno), actualiza estado. El driver temporal del sistema lo
 * llama cada HASHMAL_POWER_CICLO_*_MS.
 */
int  hashmal_power_tamid_ciclo(void);

/**
 * Estado del ciclo perpetuo (lectura pura). OK si TAMID operativo;
 * ERR_INIT si no arrancado. Sin logs.
 */
int  hashmal_power_tamid_estado(void);

/* ── aceite_supervivencia.c — Zac 4:3,11-12 dos olivos ── */

/**
 * Zac 4:3 — lee UN olivo específico. Rellena la struct `lectura`
 * con porcentaje, voltaje y flag de actividad.
 */
int  hashmal_power_leer_olivo(hashmal_power_olivo_t olivo,
                              hashmal_power_lectura_t *lectura);

/**
 * Determina el nivel de energía combinado según umbrales ZAJ,
 * KATIT, JOSHEJ. Escribe el resultado en `nivel`.
 */
int  hashmal_power_nivel_actual(hashmal_power_nivel_t *nivel);

/**
 * Snapshot completo del subsistema (lectura pura). Rellena todos
 * los campos de `estado` con la mejor información disponible —
 * telemetría de un solo vistazo (health checks, dashboard).
 */
int  hashmal_power_energia_estado(hashmal_power_estado_energia_t *estado);

/* ── aceite_mantenimiento.c — Lv 24:1-4 yaʻarój ────────── */

/**
 * Lv 24:1-4 "yaʻarój otó Aharón" — Aarón DISPONE las lámparas.
 * Operación de mantenimiento regular: ajusta duty cycle según el
 * nivel de energía actual, balancea carga de olivos, aplica
 * política de ahorro si necesario.
 */
int  hashmal_power_mantenimiento(void);

/**
 * Éx 27:20 "zaj" — ¿es la energía PURA? Retorna OK si el
 * porcentaje combinado >= HASHMAL_POWER_UMBRAL_ZAJ (80%). Si
 * estamos por debajo → ERR_ZAJ; si bajo JOSHEJ → ERR_JOSHEJ.
 */
int  hashmal_power_verificar_zaj(void);

/* ══════════════════════════════════════════════════════════════════
 * ASHMUROT - Las Vigilias de la Noche (Añadidura 04)
 *
 * "zakhartikha 'al yetsu'ay beASHMUROT ehgeh bakh." - Salmo 63:6
 * "En las VIGILIAS de la noche medito en Ti."
 *
 * "haashmoret hatikhonah." - Jueces 7:19 (vigilia media)
 * "beashmoret haboqer." - Exodo 14:24 (vigilia de la manana)
 *
 * Sub-ciclos NOCTURNOS del tamid. La noche (sunset -> sunrise)
 * se divide en 3 tercios iguales. Cada tercio publica estado +
 * heartbeat recomendado. Piezas consumidoras (04 Menora pa'amon,
 * 09 roles heartbeat, 02 Tsofeh cadencia) pueden subscribirse
 * por pull cuando lo requieran - este modulo solo PUBLICA.
 *
 * Usa Meeus publico de Pieza 03 (Añadidura 03) para sunset/sunrise.
 * ══════════════════════════════════════════════════════════════════ */

/**
 * Los 3 tercios de la noche + estado "dia" (NINGUNA).
 *
 *   NINGUNA  - estamos de dia (entre sunrise y sunset)
 *   PRIMERA  - sunset -> sunset + 1/3 noche  (Lam 2:19 rishona)
 *   MEDIA    - 1/3 -> 2/3                    (Jue 7:19 tikhonah, atacar)
 *   MANANA   - 2/3 -> sunrise                (Ex 14:24 haboqer, actuar)
 */
typedef enum {
    HASHMAL_ASHMURA_NINGUNA = 0,
    HASHMAL_ASHMURA_PRIMERA = 1,   /* Lam 2:19 */
    HASHMAL_ASHMURA_MEDIA   = 2,   /* Jue 7:19 hatikhonah */
    HASHMAL_ASHMURA_MANANA  = 3,   /* Ex 14:24 haboqer */
} hashmal_ashmura_t;

/**
 * Heartbeat recomendado por vigilia. No vinculante: los consumidores
 * deciden honrarlo o no (la Menora no se detiene ni se queda sin
 * ritmo - Ex 27:20 ner tamid; el pulso simplemente respira).
 *
 *   PRIMERA = 30 s  (transicion dia->noche, actividad normal)
 *   MEDIA   = 120 s (bajo consumo, Jue 7:19 - solo amenaza)
 *   MANANA  = 60 s  (reactivacion gradual, Ex 14:24 - preparacion)
 *   NINGUNA = 0     (dia: los consumidores usan su propio default)
 */
#define HASHMAL_ASHMURA_HB_PRIMERA_MS   30000u
#define HASHMAL_ASHMURA_HB_MEDIA_MS    120000u
#define HASHMAL_ASHMURA_HB_MANANA_MS    60000u

/* -- aceite_ashmurot.c - Añadidura 04 ─────────────────── */

/**
 * Forward declaration: input structure lives in Pieza 03. Evita
 * que el header de Pieza 08 arrastre todo el include tree de
 * hashmal_cache; el caller incluye hashmal_cache.h si lo necesita.
 */
struct hashmal_moedim_tiempo;

/**
 * Calcula la vigilia actual desde tiempo + ubicacion. Usa
 * hashmal_moedim_sunset_hour / _sunrise_hour de Pieza 03 para
 * determinar la noche. Si estamos de dia, publica NINGUNA.
 *
 * @return HASHMAL_POWER_OK si exito; ERR_NULL si t NULL.
 */
int hashmal_ashmurot_calcular(const struct hashmal_moedim_tiempo *t);

/**
 * Lectura pura: ultima vigilia calculada. NINGUNA si _calcular
 * nunca se ha llamado, si es dia, o si region polar sin
 * frontera solar.
 */
hashmal_ashmura_t hashmal_ashmurot_actual(void);

/**
 * Lectura pura: heartbeat recomendado en ms para la vigilia
 * actual (0 si NINGUNA). Los consumidores lo consultan cuando
 * quieran subscribirse; nadie esta obligado.
 */
uint32_t hashmal_ashmurot_heartbeat_ms_recomendado(void);

/* ══════════════════════════════════════════════════════════════════
 * ANAN VE'ESH - Nube y Fuego (Añadidura 07)
 *
 * "YHWH holekh lifneihem yomam ammud ANAN lanjotam hadderekh
 *  velaylah ammud ESH lehair lahem." - Exodo 13:21
 * "YHWH iba delante de ellos: columna de NUBE de dia para guiarlos,
 *  columna de FUEGO de noche para alumbrarles."
 *
 * "LO YAMISH ammud he'anan yomam ve'ammud ha'esh laylah lifney
 *  ha'am." - Exodo 13:22
 * "NO SE APARTABA la columna de nube de dia ni la columna de fuego
 *  de noche."
 *
 * "beyom he'alot he'anan yis'u benei Yisrael; uvmiqom asher yishkon
 *  he'anan sham yajanu." - Numeros 9:17
 * "Cuando la nube se ALZABA marchaban; donde la nube se POSABA
 *  acampaban."
 *
 * --- MODELO DOS MODOS + DOS MOVIMIENTOS ---
 *
 * Lo yamish (Ex 13:22): SIEMPRE hay uno de cada eje:
 *   eje modo:        DIA (anan) | NOCHE (esh)    - de Ashmurot
 *   eje movimiento:  POSA        | ALZA          - de shaon + vecinos
 *
 * Hardware hints para drivers futuros:
 *   color_hint:  VERDE / AMBAR / ROJO / APAGADO
 *   patron_hint: BREATH (calma) / PULSE / VIBRATE_UP / STATIC
 *
 * Nm 9:23 - 'al pi YHWH yajanu ve'al pi YHWH yis'au: el modulo
 * PUBLICA el "mandato"; el operador/hardware lo honra.
 * ══════════════════════════════════════════════════════════════════ */

typedef enum {
    HASHMAL_ANAN_MODO_DIA   = 0,   /* Ex 13:21 - columna de nube */
    HASHMAL_ANAN_MODO_NOCHE = 1,   /* Ex 13:21 - columna de fuego */
} hashmal_anan_modo_t;

typedef enum {
    HASHMAL_ANAN_MOV_POSA = 0,     /* Nm 9:18 yajanu - acampar, red OK */
    HASHMAL_ANAN_MOV_ALZA = 1,     /* Nm 9:17 yis'u - marchar, amenaza */
} hashmal_anan_movimiento_t;

typedef enum {
    HASHMAL_ANAN_COLOR_APAGADO = 0,
    HASHMAL_ANAN_COLOR_VERDE   = 1,   /* calma */
    HASHMAL_ANAN_COLOR_AMBAR   = 2,   /* advertencia diurna */
    HASHMAL_ANAN_COLOR_ROJO    = 3,   /* alerta nocturna */
} hashmal_anan_color_t;

typedef enum {
    HASHMAL_ANAN_PATRON_NINGUNO    = 0,
    HASHMAL_ANAN_PATRON_BREATH     = 1,   /* respiracion lenta - POSA */
    HASHMAL_ANAN_PATRON_PULSE      = 2,   /* pulso - transicion */
    HASHMAL_ANAN_PATRON_VIBRATE_UP = 3,   /* vibracion ascendente - ALZA */
    HASHMAL_ANAN_PATRON_STATIC     = 4,   /* fijo - ESH nocturno steady */
} hashmal_anan_patron_t;

/**
 * Frame publicado para el driver futuro. Incluye los dos ejes
 * (modo y movimiento) mas hints de color/patron derivados, y un
 * campo de refugio local (fase 1 - miqlat directional es fase 2
 * con broadcast de posicion).
 */
typedef struct {
    hashmal_anan_modo_t       modo;
    hashmal_anan_movimiento_t movimiento;
    hashmal_anan_color_t      color_hint;
    hashmal_anan_patron_t     patron_hint;

    /* Datos subyacentes que motivan la senal */
    uint8_t  vecinos_mesh;        /* hashmal_comm_vecinos_contar */
    uint8_t  shaon_indice;        /* hashmal_hc_parokhet_indice */
    bool     miqlat_local_activo; /* este nodo es ciudad de refugio */
    uint8_t  miqlat_refugiados;   /* refugiados hospedados aqui */

    uint64_t timestamp_ms;        /* cuando se actualizo */
} hashmal_anan_senal_t;

/* -- aceite_anan_esh.c - Añadidura 07 ─────────────────── */

/* Reutilizamos la forward-decl de hashmal_moedim_tiempo (ya
 * declarada arriba para Ashmurot). Sin re-declaracion para evitar
 * duplicados en compilacion estricta. */

/**
 * Ex 13:21-22 - actualiza la senal anan/esh desde cuatro fuentes:
 *   Ashmurot (local, Añadidura 04) -> modo DIA/NOCHE
 *   Parokhet (Pieza 12, Añadidura 01) -> shaon
 *   Menora vecinos (Pieza 04) -> densidad red
 *   Miqlat (Pieza 07, Añadidura 05) -> estado refugio local
 *
 * Lo yamish (Ex 13:22): despues de esta llamada SIEMPRE hay un
 * modo y un movimiento publicados. Si una fuente falla, se usa
 * default conservador (POSA + VERDE + BREATH).
 *
 * @return HASHMAL_POWER_OK si exito; ERR_NULL si t NULL.
 */
int hashmal_anan_actualizar(const struct hashmal_moedim_tiempo *t);

/**
 * Lectura pura: copia la senal actual al buffer del caller. El
 * driver de LED/haptic (fase 2) consume hints para pintar luz y
 * pulsar vibracion.
 */
int hashmal_anan_senal_obtener(hashmal_anan_senal_t *out);

#ifdef __cplusplus
}
#endif

#endif /* HASHMAL_POWER_H */
