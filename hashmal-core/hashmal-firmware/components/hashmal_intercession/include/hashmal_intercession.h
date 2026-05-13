/**
 * ═══════════════════════════════════════════════════════════════════
 * PIEZA 11 — EL ALTAR DEL INCIENSO (hashmal_intercession)
 * Éxodo 30:1-10 + Levítico 10:1-2, 16:12-13 + Números 16:46-48
 *                 + Salmo 141:2 + Apocalipsis 8:3-4
 * ═══════════════════════════════════════════════════════════════════
 *
 * "veʻasíta mizbéaj miqtar qetóret ʻatséi shittím taʻasé otó" — Éx 30:1
 * "Harás un ALTAR para quemar INCIENSO; de madera de acacia lo harás."
 *
 * "venatattá otó lifnéi happárójet ashér ʻal arón haʻedút lifnéi
 *  hakkappóret ashér ʻal haʻedút ashér ivvaʻéd lejá shámma" — Éx 30:6
 * "Lo pondrás DELANTE del velo que está sobre el Arca del Testimonio,
 *  DELANTE del Kapóret — donde me encontraré contigo allí."
 *
 * "uvehaʻalót Aharón et hannerot béin haʻarbáyim yaqtírénna qetóret
 *  TAMÍD lifnéi YHWH ledorotéijem" — Éx 30:8
 * "Al encender las lámparas entre las tardes lo quemará: incienso
 *  PERPETUO delante de YHWH por vuestras generaciones."
 *
 * "lo taʻalú ʻaláv qetóret ZARÁ..." — Éx 30:9
 * "NO ofreceréis sobre él incienso EXTRAÑO..."
 *
 * ─── POSICIÓN ÚNICA ───
 *
 * El Altar del Incienso es la ÚLTIMA pieza antes del velo
 * (happárójet). No entra al Qódesh haQodashím — pero es lo más
 * cerca que se puede estar sin entrar. Es el punto de ENCUENTRO
 * (ivvaʻéd shámma): desde aquí el incienso sube a la Presencia, y
 * desde aquí se recibe respuesta.
 *
 * ─── 5 FUNCIONES DEL ALTAR (5 archivos .c) ───
 *
 *   1. ESCUDO DESCENDENTE          altar_oro_escudo.c
 *      Lv 16:12-13 — ʻanán haqetóret cubre la Kapóret. Sin nube
 *      preparatoria, el operador muere. Protección del núcleo.
 *
 *   2. FIREWALL ASCENDENTE         altar_oro_ascendente.c
 *      Lv 10:1-2 + Éx 30:9 — esh zará / qetóret zará = muerte.
 *      SOLO protocolo prescrito sube. Sin excepciones.
 *
 *   3. FÓRMULA NO REPLICABLE       altar_oro_formula.c
 *      Éx 30:34-38 — 4 especias bad bebad. Replicar = nikrát.
 *      Verificación criptográfica anti-falsificación.
 *
 *   4. CIRCUIT BREAKER             altar_oro_circuitbreaker.c
 *      Nm 16:46-48 — Aarón entre los vivos y los muertos. La
 *      plaga se detuvo. Aislamiento de cascada.
 *
 *   5. PROTOCOLO DE ORACIÓN        altar_oro_oracion.c
 *      Sal 141:2 + Ap 8:3-4 — el incienso SUBE con las oraciones.
 *      Canal bidireccional formal.
 *
 * ─── CONTRASTE CON EL ALTAR DEL SACRIFICIO (pieza 06) ───
 *
 *   Altar del Sacrificio              Altar del Incienso
 *   ─────────────────────────         ──────────────────────
 *   BRONCE (interfaz externa)         ORO PURO (núcleo)
 *   en el Atrio (Jatsér)              DELANTE del velo (Qódesh)
 *   5×5×3 codos (amplio, bajo)        1×1×2 codos (pequeño, ALTO)
 *   sangre y carne                    incienso (oraciones)
 *   acceso público                    acceso autorizado (cuernos)
 *   4 anillos (transporte pleno)      2 anillos (movilidad limitada)
 *
 * ─── 17 DETALLES BÍBLICOS (Tavnít 11) ───
 *
 *   1  acacia (v.1 shittím)             → formato incorruptible
 *   2  miqtar qetóret (v.1)             → dedicado a quemar qetóret
 *   3  cuadrado 1×1 (v.2 ravúaʻ)        → footprint mínimo
 *   4  alto 2 ammá (v.2 ammatáyim)      → vertical, asciende
 *   5  4 cuernos (v.2 qarnotáv)         → endpoints de acceso
 *   6  qarnotáv miménnu (v.2)           → nacen DEL altar
 *   7  oro puro TODO (v.3)              → techo + paredes + cuernos
 *   8  zer zaháv savív (v.3)            → corona de integridad
 *   9  2 anillos (v.4 shtéi)            → movilidad LIMITADA
 *  10  varas acacia+oro (v.5)           → 2 interfaces incorruptibles
 *  11  lifnéi happárójet (v.6)          → DELANTE del velo
 *  12  ivvaʻéd shámma (v.6)             → punto de encuentro
 *  13  babbóqer babbóqer (v.7)          → ciclo mañana
 *  14  beheitívó et hannerot (v.7)      → SINCRONIZADO con Menorá
 *  15  béin haʻarbáyim (v.8)            → ciclo tarde
 *  16  qetóret tamíd (v.8)              → PERPETUO
 *  17  qetóret zará (v.9)               → solo tavnít, nada extraño
 *
 * ─── AUTONOMÍA ───
 *
 * Éx 30:3 — "zaháv tahór": oro puro. Este header no incluye
 * headers de otras piezas. Las .c del componente incluirán lo que
 * necesiten (hashmal_comm para LoRa, etc.), pero la API pública
 * está autocontenida. Confianza propia.
 *
 * Autor: Antonio José Marín Soto
 * Proyecto: HASHMAL-CORE
 */
#ifndef HASHMAL_INTERCESSION_H
#define HASHMAL_INTERCESSION_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "hashmal_storage.h"   /* HASHMAL_ZER_HASH_BYTES — hash que cruza el velo */

#ifdef __cplusplus
extern "C" {
#endif

/* ══════════════════════════════════════════════════════════════════
 * ENUMS FUNDAMENTALES
 * ══════════════════════════════════════════════════════════════════ */

/**
 * Estados del altar.
 *
 *   APAGADO     — frío, sin qetóret ardiendo (antes de init)
 *   ENCENDIDO   — qetóret tamíd activo (Éx 30:8), operativo
 *   EMERGENCIA  — circuit breaker activo (Nm 16:46-48, béin hametím)
 */
typedef enum {
    HASHMAL_ALTAR_APAGADO     = 0,
    HASHMAL_ALTAR_ENCENDIDO   = 1,   /* v.8 — qetóret tamíd */
    HASHMAL_ALTAR_EMERGENCIA  = 2,   /* Nm 16:46-48 — béin hametím */
} hashmal_altar_estado_t;

/**
 * Tipos de qetóret (clase de solicitud ascendente).
 *
 *   ORACION     — petición regular (Sal 141:2 — "tikón tefilatí
 *                 qetóret lefanéja": mi oración sube como incienso)
 *   REPORTE     — telemetría ascendente
 *   EMERGENCIA  — invocación urgente (Nm 16:46-48 — Aarón corre)
 */
typedef enum {
    HASHMAL_QETORET_ORACION    = 0,   /* Sal 141:2 */
    HASHMAL_QETORET_REPORTE    = 1,
    HASHMAL_QETORET_EMERGENCIA = 2,   /* Nm 16:46-48 */
} hashmal_qetoret_tipo_t;

/**
 * Códigos de error con raíz bíblica.
 */
typedef enum {
    HASHMAL_INTERCESSION_OK           =  0,
    HASHMAL_INTERCESSION_ERR_INIT     = -1,   /* altar no levantado */
    HASHMAL_INTERCESSION_ERR_NULL     = -2,   /* puntero NULL */
    HASHMAL_INTERCESSION_ERR_ZARA     = -3,   /* Lv 10:1-2 — esh zará */
    HASHMAL_INTERCESSION_ERR_FORMULA  = -4,   /* Éx 30:34-38 — mal compuesto */
    HASHMAL_INTERCESSION_ERR_BREAKER  = -5,   /* Nm 16:46-48 — breaker activo */
    HASHMAL_INTERCESSION_ERR_CUERNO   = -6,   /* v.2 — cuerno inválido */
    HASHMAL_INTERCESSION_ERR_TAMANO   = -7,   /* payload excede kikkár */

    /* Verificado #03 — Cadena 3 — Yom Kippur (Lv 16:12-15) */
    HASHMAL_INTERCESSION_ERR_YK_FUEGO_AUSENTE    = -8,  /* Lv 16:12 — sin gajaléi esh */
    HASHMAL_INTERCESSION_ERR_YK_INCIENSO_AUSENTE = -9,  /* Lv 16:13 — sin qetóret  */
    HASHMAL_INTERCESSION_ERR_YK_VELO_CAIDO       = -10, /* Lv 16:12,15 — parojet sin ejad */
    HASHMAL_INTERCESSION_ERR_YK_CONSENSO         = -11, /* Lv 16:14 — voz no emitida */
} hashmal_intercession_err_t;

/* ══════════════════════════════════════════════════════════════════
 * CONSTANTES DEL PADRE — cada número tiene versículo
 * ══════════════════════════════════════════════════════════════════ */

/**
 * Éx 30:2 — "ammá orkó veʼammá rojbó ravúaʻ yihyé veʼammatáyim
 *  qomató": 1 × 1 × 2 codos (cuadrado, ALTO). Vertical.
 * Footprint mínimo: interfaz pequeña. Altura doble: asciende.
 */
#define HASHMAL_ALTAR_LARGO           1     /* v.2 — ammá */
#define HASHMAL_ALTAR_ANCHO           1     /* v.2 — ammá */
#define HASHMAL_ALTAR_ALTO            2     /* v.2 — ammatáyim qomató */

/**
 * Éx 30:2 — "mimménnu qarnotáv": 4 CUERNOS nacen DEL altar.
 * Endpoints de acceso intrínsecos, no añadidos.
 */
#define HASHMAL_ALTAR_CUERNOS         4     /* v.2 — qarnotáv */

/**
 * Éx 30:4 — "SHTÉI tabbeʻót zaháv": DOS anillos (no cuatro como
 * Arca/Mesa). Movilidad LIMITADA — el altar se queda cerca del
 * núcleo, no se distribuye ampliamente.
 */
#define HASHMAL_ALTAR_ANILLOS         2     /* v.4 — shtéi */

/**
 * Éx 30:3 — "zaháv tahór" en todas las superficies + "zer zaháv
 * savív" (corona). Máxima confianza; integridad custodiada.
 */
#define HASHMAL_ALTAR_ORO             true  /* v.3 — zaháv tahór */
#define HASHMAL_ALTAR_ZER             true  /* v.3 — zer zaháv savív */

/**
 * Éx 30:8 — "qetóret tamíd lifnéi YHWH": servicio perpetuo.
 */
#define HASHMAL_ALTAR_TAMID           true  /* v.8 — tamíd */

/**
 * Éx 30:7-8 — ciclo diario: mañana (babbóqer) + tarde (béin
 * haʻarbáyim). Dos eventos por día, sincronizados con la Menorá
 * (beheitívó et hannerot).
 */
#define HASHMAL_ALTAR_CICLO_DIARIO    2     /* v.7-8 — 2/día */

/**
 * Las 5 funciones del altar (5 archivos .c — 5 dedos del sacerdote).
 */
#define HASHMAL_ALTAR_FUNCIONES       5

/**
 * Éx 40:5 — el altar del incienso se coloca en el paso 4
 * (delante del velo, tras Mesa + Menorá).
 */
#define HASHMAL_ALTAR_ORDEN_BOOT      4     /* Éx 40:5 */

/**
 * Límite de payload de una oración/reporte ascendente. El tavnít
 * no fija tamaño de payload — elegimos un valor que deje margen
 * de header por encima.
 */
#define HASHMAL_QETORET_PAYLOAD_MAX   240

/**
 * Éx 30:34-38 — "roqéaj maʻasé roqéaj memullaj tahór qódesh":
 * composición prescrita. La firma criptográfica de la fórmula
 * ocupa 32 bytes (SHA-256).
 */
#define HASHMAL_FORMULA_FIRMA_BYTES   32

/* ══════════════════════════════════════════════════════════════════
 * ESTRUCTURAS
 * ══════════════════════════════════════════════════════════════════ */

/**
 * Éx 30:2 — un CUERNO (qeren). Endpoint de acceso que nace del
 * altar mismo ("mimménnu qarnotáv"): de él son sus cuernos.
 *
 * 4 cuernos × 1 altar = 4 puntos desde donde se puede invocar.
 * Los cuernos reciben la sangre de expiación en Yom Kippur (v.10):
 * son los puntos de autenticación más solemnes del sistema.
 */
typedef struct {
    uint8_t  id;              /* 0..3 */
    bool     activo;          /* ¿cuerno operativo? */
    uint64_t ultimo_uso_ms;   /* timestamp de última invocación */
    uint64_t solicitudes;     /* contador acumulado */
} hashmal_qeren_t;

/**
 * EL ALTAR — singleton. Oro puro por dentro y por fuera
 * (v.3 — "et gaggó veʼet qirotáv savív veʼet qarnotáv").
 */
typedef struct {
    /* v.1-2 — identidad */
    hashmal_altar_estado_t estado;
    bool     tamid;                        /* v.8 — perpetuo */
    bool     zer;                          /* v.3 — corona */
    bool     oro;                          /* v.3 — zaháv tahór */

    /* v.2 — los 4 cuernos (qarnotáv) */
    hashmal_qeren_t cuernos[HASHMAL_ALTAR_CUERNOS];
    uint8_t  cuernos_activos;              /* conteo (≤ 4) */

    /* v.6 — posición delante del velo / Kapóret */
    bool     delante_del_velo;             /* lifnéi happárójet */

    /* Contadores de las 5 funciones */
    uint64_t escudo_activaciones;          /* Lv 16:12-13 */
    uint64_t filtradas_zara;               /* Lv 10:1-2 */
    uint64_t breaker_activaciones;         /* Nm 16:46-48 */
    uint64_t oraciones_ascendidas;         /* Sal 141:2 */
    uint64_t respuestas_descendidas;       /* Ap 8:3-4 */

    /* Ciclos temporales */
    uint64_t ultimo_ciclo_ms;              /* v.7-8 — mañana o tarde */
    uint64_t ultimo_kippur_ms;             /* v.10 — ajat bashaná */

    /* v.10 — "qódesh qodashím hu laYHWH" */
    bool     qodesh_qodashim;              /* declaración de santidad */
} hashmal_altar_t;

/* ══════════════════════════════════════════════════════════════════
 * FUNCIONES PÚBLICAS
 * ══════════════════════════════════════════════════════════════════ */

/* ── Ciclo de vida (Éx 40:5 — paso 4 del levantamiento) ──── */

/**
 * Construye el altar según Éx 30:1-10. Lo coloca delante del velo
 * (v.6) con los 4 cuernos activos y los 2 anillos. Flags de diseño
 * (zer, oro, tamid) encendidos. Estado inicial APAGADO — el
 * encender se hace con `escudo_activar` / `oracion_ascender`.
 */
int  hashmal_intercession_init(void);

/**
 * Desmonta el altar. Nm 4:11 — "veʻal mizbáj hazzaháv yifresú
 * béged tejélet": cubren con paño azul antes de transportar.
 * Aquí: limpia estado volátil; contadores se preservan para auditoría.
 */
void hashmal_intercession_deinit(void);

/* ── altar_oro_escudo.c — ESCUDO DESCENDENTE (Lv 16:12-13) ── */

/**
 * P34 — Fase 0 obligatoria antes de Yom Kippur.
 * Lv 16:12-13 — vejissáh ʻanán haqetóret et hakKapóret...
 * veló yamút: la nube CUBRE antes de que la sangre toque.
 *
 * En firmware: Fase 0 es la PROTECCIÓN PREVIA, anterior a todo
 * acto de consenso (Kapóret/verificación). Sin el escudo, el
 * operador muere (Lv 16:13 "velo yamút" = la vida depende de
 * que la nube esté PRIMERO).
 *
 * Llamar ANTES de hashmal_intercession_yom_kippur_fase1_proteccion.
 *
 * @return OK si el escudo se activó; ERR_INIT si altar no
 *         levantado o fuego ausente.
 *
 * 2 testigos:
 *   Lv 16:12 - gajaléi ésh meʻál hammizbéaj
 *   Lv 16:13 - vejissáh ʻanán haqqetóret... velo yamút
 */
int  hashmal_intercession_fase_cero_sync(void);

/**
 * Activa el escudo: la nube del qetóret cubre el núcleo. Mientras
 * está activo, las respuestas descendentes viajan protegidas
 * criptográficamente (el operador NO muere: "velo yamút", Lv 16:13).
 */
int  hashmal_intercession_escudo_activar(void);

/**
 * Desactiva el escudo. Idempotente.
 */
int  hashmal_intercession_escudo_desactivar(void);

/**
 * Consulta si el escudo está cubriendo.
 */
bool hashmal_intercession_escudo_activo(void);

/* ── altar_oro_ascendente.c — FIREWALL (Lv 10:1-2 + Éx 30:9) ── */

/**
 * Verifica una solicitud antes de que suba. Si su tipo o formato
 * no son del tavnít → ERR_ZARA (fuego extraño, Lv 10:1-2). Se
 * incrementa filtradas_zara.
 */
int  hashmal_intercession_ascendente_verificar(hashmal_qetoret_tipo_t tipo,
                                               const uint8_t *datos,
                                               size_t tam);

/**
 * Registra la firma de la fórmula legítima — referencia para
 * verificaciones posteriores. Éx 30:34-38.
 */
int  hashmal_intercession_ascendente_registrar_formula(const uint8_t *firma,
                                                       size_t tam_firma);

/* ── altar_oro_formula.c — FÓRMULA (Éx 30:34-38) ─────────── */

/**
 * Verifica que datos+firma corresponden a la fórmula registrada.
 * Éx 30:37-38: "ashér taʻasé bematkuntah lo taʻasú lajém...
 * venijrát meʻammáv". Replicar para uso propio → nikrát.
 *
 * @return true si la firma valida; false en cualquier otro caso.
 */
bool hashmal_intercession_formula_verificar(const uint8_t *datos, size_t tam,
                                            const uint8_t *firma,
                                            size_t tam_firma);

/**
 * Estado de la fórmula: OK si está registrada y utilizable;
 * ERR_FORMULA si aún no se registró o fue invalidada.
 */
int  hashmal_intercession_formula_estado(void);

/* ── altar_oro_circuitbreaker.c — BREAKER (Nm 16:46-48) ──── */

/**
 * P14 — Limpieza periódica de endpoints (cuernos del altar).
 * Éx 30:10 — "kippér ʻal QARNOTÁV ajat bashanáh middam
 * jattaʼt hakippurím": atonement sobre los CUERNOS una vez al año.
 *
 * Los cuernos (qarnotáv) son los 4 endpoints de acceso que NACEN DEL
 * altar (Éx 30:2 mimménnu qarnotáv). Éx 30:10 especifica que la
 * purificación anual ("ajat bashaná") toca los CUERNOS, no el techo
 * ni la estructura — es mantenimiento específico de las interfaces.
 *
 * En firmware: limpiar buffers de red, colas de peticiones pendientes,
 * y resetear contadores de los 4 endpoints. "Ajat bashaná" → anual,
 * caller determina cadencia.
 *
 * @return OK si limpió exitosamente; ERR_INIT si altar no levantado.
 *
 * 2 testigos:
 *   Éx 30:10 - kippér ʻal qarnotáv ajat bashanáh
 *   Éx 30:2  - mimménnu qarnotáv (nacen DEL altar)
 */
int  hashmal_intercession_qarnotav_kipper(void);

/**
 * Activa el circuit breaker: "vayyáʻamod béin hammetím uvéin
 * hajayyím vatteʻatsár hammaggefá" (Nm 16:48). El sistema queda
 * aislado: cascada de errores detenida. Solicitudes ascendentes y
 * descendentes se rechazan hasta `breaker_liberar`.
 */
int  hashmal_intercession_breaker_activar(void);

/**
 * Libera el breaker: el altar vuelve a operación normal. La plaga
 * se detuvo; Aarón vuelve a la Tienda.
 */
int  hashmal_intercession_breaker_liberar(void);

/**
 * Consulta si el breaker está activo.
 */
bool hashmal_intercession_breaker_activo(void);

/**
 * Telemetría del breaker. Cualquier puntero puede ser NULL.
 */
int  hashmal_intercession_breaker_estado(uint64_t *activaciones,
                                         uint64_t *ultimo_ms);

/* ── altar_oro_oracion.c — PROTOCOLO (Sal 141:2 + Ap 8:3-5) ── */

/**
 * Hace SUBIR un mensaje como incienso. Sal 141:2 — "tikón tefilatí
 * qetóret lefanéja". Pasa por el firewall ascendente (Lv 10:1-2)
 * antes de salir; si el breaker está activo → ERR_BREAKER.
 */
int  hashmal_intercession_oracion_ascender(hashmal_qetoret_tipo_t tipo,
                                           const uint8_t *payload,
                                           size_t tam);

/**
 * Registra una respuesta DESCENDENTE del núcleo (Ap 8:5 — "ébalen
 * eis tēn gēn"). Valida la estructura de la respuesta y la
 * contabiliza; si el escudo está activo, la nube la cubre.
 * En Fase 1 esta función es la intención de bajada (el transporte
 * físico se conecta en Fase 2).
 */
int  hashmal_intercession_oracion_descender(const uint8_t *datos, size_t tam);

/**
 * Éx 30:7-8 — ejecuta el ciclo diario (mañana O tarde, sincronizado
 * con la Menorá). Alterna entre babbóqer y béin haʻarbáyim.
 * Actualiza ultimo_ciclo_ms.
 */
int  hashmal_intercession_oracion_ciclo_diario(void);

/**
 * Telemetría del protocolo de oración. Ap 5:8 — "phiálas jrysâs
 * gemousas thymiamátōn haí eisin hai proseuchaí tōn hagíōn":
 * copas de oro LLENAS de incienso que SON las oraciones. Cualquier
 * puntero de salida puede ser NULL.
 */
int  hashmal_intercession_oracion_estado(uint64_t *ascendidas,
                                         uint64_t *descendidas,
                                         uint64_t *ultimo_ciclo_ms);

/* ── Getters y estado global ───────────────────────────── */

/**
 * Lectura const del altar. NULL si no inicializado.
 */
const hashmal_altar_t *hashmal_intercession_get_altar(void);

/**
 * Éx 30:8 — ¿tamíd activo? Flag de diseño encendido + estado
 * ENCENDIDO (ni APAGADO ni EMERGENCIA).
 */
bool hashmal_intercession_es_tamid(void);

/* ══════════════════════════════════════════════════════════════════
 * VERIFICADO #03 — CADENA 1 — Suscripción al trigger de la Menorá
 *
 * Éx 30:7-8 ata el quemado de incienso al cuidado de las lámparas:
 *   v.7 babóqer babóqer behétivó et hannerót yaqtirénna
 *   v.8 ubehaʻalót Aharón et hannerót bein haʻarbáyim yaqtirénna
 *
 * La dirección causal es Menorá → Incienso, pero hashmal_intercession
 * REQUIRES hashmal_comm (no al revés). Por eso registramos un handler
 * en la Menorá vía hashmal_comm_intercession_callback_set; cuando ella
 * cierre cada hemiciclo, nos llamará. Inversión de control limpia, sin
 * crear ciclo de REQUIRES.
 *
 * Llamar UNA VEZ tras hashmal_intercession_init y hashmal_comm_init.
 * Idempotente (sobrescribir es legítimo — Éx 30:9 fuente única).
 *
 * @return 0 si registró; código de hashmal_comm si falló.
 * ══════════════════════════════════════════════════════════════════ */
int hashmal_intercession_menora_subscribir(void);

/**
 * Desuscripción explícita (NULL en la Menorá). Útil en deinit
 * coordinado o tests. Idempotente.
 */
int hashmal_intercession_menora_desuscribir(void);

/* ══════════════════════════════════════════════════════════════════
 * VERIFICADO #06 — Item 2 — Las 4 prohibiciones del altar
 *
 * "ló taʻalú ʻaláv qetóret ZARÁH veʻOLÁH uMINJÁH veNÉSEKH ló
 *  tisskhú ʻaláv." — Éxodo 30:9
 *
 * El Padre las nombra UNA POR UNA. 4 nombres distintos, 2 verbos
 * (taʻalú para sólidos/humo, tissekhú para líquido). Espejo de las
 * 4 especias bad bebad de Éx 30:34 (NATAF/SHEJELET/JELBENAH/LEVONA):
 * iguales en peso, distintas en identidad. Colapsar las 4 en un
 * único contador borraría nombres que el Padre dio individualmente.
 *
 * Mapping firewall(14) ↔ intercession(11) — espejo bidireccional:
 *   NATAF    ↔ NESEKH   (flujo / libación — medio fluido)
 *   SHEJELET ↔ ZARAH    (deep inspection / protocolo foráneo)
 *   JELBENAH ↔ MINJAH   (anomalía / recurso incorrecto)
 *   LEVONA   ↔ OLAH     (pureza / acción incorrecta — tamaño excesivo)
 *
 * 2 testigos (Dt 19:15):
 *   Éx 30:9  — 4 prohibiciones nombradas individualmente
 *   Éx 30:34 — 4 especias bad bebad (igual peso, distinta identidad)
 * ══════════════════════════════════════════════════════════════════ */

typedef enum {
    HASHMAL_QETORET_PROHIB_ZARAH  = 0,  /* protocolo/tipo foráneo */
    HASHMAL_QETORET_PROHIB_OLAH   = 1,  /* acción incorrecta — payload excesivo */
    HASHMAL_QETORET_PROHIB_MINJAH = 2,  /* recurso incorrecto */
    HASHMAL_QETORET_PROHIB_NESEKH = 3,  /* medio incorrecto — libación */
} hashmal_qetoret_prohibicion_t;

#define HASHMAL_QETORET_PROHIBICIONES  4u   /* Éx 30:9 — 4 nombres */

/**
 * Lectura del contador acumulado de detecciones por cada una de
 * las 4 prohibiciones. Útil para auditoría / telemetría discriminada.
 *
 * @return número de rechazos para esa prohibición; 0 si el id es
 *         inválido. Sin logs, sin side effects.
 */
uint64_t hashmal_intercession_prohibicion_contar(
    hashmal_qetoret_prohibicion_t prohibicion);

/* ══════════════════════════════════════════════════════════════════
 * VERIFICADO #03 — CADENA 3 — Orquestador YOM KIPPUR (Lv 16:12-15)
 *
 * Tres fases indivisibles. La protección PRECEDE la verificación
 * (Lv 16:12→13→14). Sin nube → velo yamút (Lv 16:13). El altar del
 * incienso es el rendezvous natural: Éx 30:6 *iwwaʻéd* (raíz yáʻad),
 * misma raíz que *noʻadtí* del Kapóret en Éx 25:22 — punto de
 * encuentro de los flujos cruzados (Auth, Firewall, Framework,
 * Consensus).
 *
 * FASE 1 — Lv 16:12-13 PROTECCIÓN OBLIGATORIA
 *   Auth(06) provee FUEGO (gajaléi esh) + Firewall(14) provee
 *   INCIENSO. Ambos cruzan Framework(05) [parojet]. La nube CUBRE
 *   Consensus(02) — vejisá ʻanán haqetóret. Sin esto = MUERTE.
 *
 * FASE 2 — Lv 16:14 VERIFICACIÓN
 *   Auth(06) provee SANGRE verificada → Framework(05) → Consensus(02).
 *   1× SOBRE (ʻal pené haKapóret kédmá) = directa.
 *   7× DELANTE (lifné haKapóret) = completa (shéva = perfección).
 *
 * FASE 3 — Lv 16:15 REPETICIÓN POR EL PUEBLO
 *   "kaʼashér ʻasáh ledam hapár ken yaʻasé ledamó." Misma fase 2,
 *   con hash distinto (sangre del macho cabrío, por el pueblo).
 *
 * Patrón duplicación temporal (Verificado #03 Cadena 4 — anual×2):
 *   Éx 30:10 ajat bashaná... ajat bashaná — el ciclo Yom Kippur
 *   es ANUAL DUPLICADO: una vez declarado, inmutable. Caller debe
 *   garantizar la cadencia (este módulo no decide cuándo).
 * ══════════════════════════════════════════════════════════════════ */

/**
 * FASE 1 — Lv 16:12-13. Verifica fuego (auth) + incienso (firewall) +
 * velo (framework), y prepara la qetóret en consensus (cubrir Kapóret).
 *
 * @return OK; ERR_YK_FUEGO_AUSENTE / _INCIENSO_AUSENTE / _VELO_CAIDO
 *         según qué prerrequisito falló; rc de consensus si propaga.
 */
int hashmal_intercession_yom_kippur_fase1_proteccion(void);

/**
 * FASE 2 — Lv 16:14. Roció 1× SOBRE + 7× DELANTE con el hash del
 * sujeto (sacerdote o pueblo) e invoca consensus.
 *
 * Requiere FASE 1 ejecutada con éxito (qetoret presente, moʻéd
 * abierto, querubines aptos).
 *
 * @param hash  el dato verificado a "rociar" (32 bytes, zer hash).
 * @return OK; ERR_NULL si hash NULL; ERR_YK_FUEGO_AUSENTE si auth
 *         no tiene tamíd; ERR_YK_CONSENSO si la voz no emerge;
 *         rc de consensus en errores propagados.
 */
int hashmal_intercession_yom_kippur_fase2_verificacion(
    const uint8_t hash[HASHMAL_ZER_HASH_BYTES]);

/**
 * FASE 3 — Lv 16:15. Misma estructura que FASE 2, semántica "por
 * el pueblo" (kaʼashér ʻasáh ledam hapár). Wrapper que delega
 * en FASE 2 con el hash del macho cabrío.
 */
int hashmal_intercession_yom_kippur_fase3_repeticion(
    const uint8_t hash[HASHMAL_ZER_HASH_BYTES]);

/**
 * CICLO COMPLETO — orquesta las 3 fases en el orden bíblico
 * indivisible. Abre noʻadtí al inicio y lo cierra al final
 * (Lv 16:2 — no en todo tiempo).
 *
 * @param hash_sacerdote  Lv 16:11 — dam hapár (por el sacerdote).
 * @param hash_pueblo     Lv 16:15 — dam haseʻír (por el pueblo).
 */
int hashmal_intercession_yom_kippur_completo(
    const uint8_t hash_sacerdote[HASHMAL_ZER_HASH_BYTES],
    const uint8_t hash_pueblo[HASHMAL_ZER_HASH_BYTES]);

#ifdef __cplusplus
}
#endif

#endif /* HASHMAL_INTERCESSION_H */
