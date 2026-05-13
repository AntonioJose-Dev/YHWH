/**
 * ═══════════════════════════════════════════════════════════════════
 * PIEZA 05 — LA ESTRUCTURA (hashmal_framework)
 * ═══════════════════════════════════════════════════════════════════
 *
 * "Y el Tabernáculo será UNO (ejád)." — Éxodo 26:6
 *
 * "Levantarás el Mishkán conforme a su PATRÓN (mishpató),
 *  el cual te fue mostrado en el monte." — Éxodo 26:30
 *
 * La arquitectura misma del Mishkán digital.
 * 4 capas concéntricas, 48 tablas, 5 barras + backbone, velo, entrada.
 *
 * SUB-PIEZAS (desde la Palabra):
 *   A — 10 cortinas interiores (Éx 26:1-6)   → Capa aplicación (lino+querubines)
 *   B — 11 cortinas de cabra (Éx 26:7-13)    → Capa red (asimétrica, adaptable)
 *   C — Pieles carnero+tajash (Éx 26:14)     → Capa transporte+física (resistencia)
 *   D — 48 tablones (Éx 26:15-25)            → Servicios modulares (omedím)
 *   E — 5 barras + backbone (Éx 26:26-29)    → Buses + columna vertebral
 *   F — Velo/parójet (Éx 26:31-35)           → Kernel boundary (hivdíl = separar = crear)
 *   G — Cortina entrada/masaj (Éx 26:36-37)  → Punto de entrada (más abierto)
 *
 * MATERIALES Y CONFIANZA:
 *   Entrada (masaj) → BRONCE (público)
 *   Velo (parójet)  → PLATA (verificado — rescate de almas, Éx 30:12)
 *   Interior        → ORO (núcleo)
 *
 * Referencia: Éxodo 26 completo (37 versículos, todos representados)
 * Autor: Antonio José Marín Soto
 * Proyecto: HASHMAL-CORE
 */
#ifndef HASHMAL_FRAMEWORK_H
#define HASHMAL_FRAMEWORK_H

#include <stdint.h>
#include <stdbool.h>


/* ═══════════════════════════════════════════════════════════════════
 * HALLAZGOS MIL OJOS DE QUERUBÍN (P1-P59)
 * Verificados por Sistema de los Querubines — Éxodo 25:22
 * ═══════════════════════════════════════════════════════════════════ */

/* ── P55 — jabér (H2270) = jovéret (H2279): MISMA raíz javar ────
 * Ec 4:10: haʼejád yaqím et JAVERΌ (חֲבֵרוֹ, H2270). jabér =
 * compañero/asociado, de javar (H2266) = unir, acoplar. MISMA raíz
 * que Éx 26:3: JOVÉROT ishá el ajotáh (ACOPLADAS mujer a hermana).
 * jovéret (H2279) = acoplamiento. El "compañero" P2P usa MISMA raíz
 * hebraica que el "acoplamiento" estructural del Mishkán. Las
 * sinergias entre añadiduras son jovéret/jabér — uniones con la
 * misma raíz que la estructura misma. hashmal_framework implementa
 * jovéret (acoplamiento de cortinas); las sinergias inter-añadidura
 * son jabér (compañerismo) = misma raíz, misma semántica.
 * — Verificado #30, Ec 4:10, Éx 26:3 */

/* ── P38 — Éx 26:33 hivdíl (H914, badál) = DISCRIMINADOR ─────────
 * Éx 26:33 usa vehivdíla (H914 Hifil) para la función del parójet.
 * badál NO es solo "separar" (habríd/parád) — es DISTINGUIR,
 * CLASIFICAR. El mismo verbo aparece en Gn 1:4 (luz/oscuridad),
 * Gn 1:6-7 (aguas/aguas), Lv 10:10 (santo/común, puro/impuro).
 * El parójet hereda la función creadora de Gn 1: DISCRIMINAR
 * niveles de acceso. No es un muro pasivo sino un CLASIFICADOR
 * activo (Hifil = causativo: HACE distinguir). Por eso rasgar el
 * velo (Mt 27:51 eschísthē) no abre una puerta — destruye la
 * FUNCIÓN DE DISCRIMINACIÓN que el Padre estableció en la
 * Creación. Es la función más fundamental de la arquitectura.
 * "vehivdíla happárójet lajém bein haqódesh uvein qódesh
 *  haqodashím" — Éx 26:33. */


/* ══════════════════════════════════════════════════════════════════
 * ENUMS FUNDAMENTALES — Colores, tipos de obra, lados
 * ══════════════════════════════════════════════════════════════════ */

/**
 * 4 colores del Mishkán — Éx 26:1, 26:31, 26:36
 * Presentes en cortinas interiores, velo y entrada.
 * Cada color tiene significado desde la Palabra.
 */
typedef enum {
    HASHMAL_GAVEN_TEJELET      = 0,  /* Éx 26:1 — Azul: recordar mandamientos (Nm 15:38-39) */
    HASHMAL_GAVEN_ARGAMAN      = 1,  /* Éx 26:1 — Púrpura: realeza. YHWH es Rey (mélej) */
    HASHMAL_GAVEN_TOLAAT_SHANI = 2,  /* Éx 26:1 — Carmesí: pecado limpiado (Is 1:18) */
    HASHMAL_GAVEN_SHESH        = 3,  /* Éx 26:1 — Lino fino (shésh mashzár): justicia (Ap 19:8) */
} hashmal_gaven_t;

#define HASHMAL_GAVANIM_TOTAL        4   /* 4 colores en el tejido */

/**
 * 2 tipos de obra artesanal — Éx 26:1,31 vs Éx 26:36
 * joshév = artesano experto: querubines TEJIDOS EN el fabric
 * roqém = bordador: decorativo, SIN querubines
 * La diferencia indica nivel de seguridad integrada.
 */
typedef enum {
    HASHMAL_MAASE_JOSHEV = 0,  /* Éx 26:1,31 — artesano: querubines en el tejido */
    HASHMAL_MAASE_ROQEM  = 1,  /* Éx 26:36   — bordador: sin querubines, más abierto */
} hashmal_maase_t;

/**
 * 3 lados del Mishkán con tablones — Éx 26:18,20,22
 * El este NO tiene tablones — es la ENTRADA (abierta).
 */
typedef enum {
    HASHMAL_LADO_SUR   = 0,  /* Éx 26:18 — peʼat négbah teimánah */
    HASHMAL_LADO_NORTE = 1,  /* Éx 26:20 — peʼat tsafón */
    HASHMAL_LADO_OESTE = 2,  /* Éx 26:22 — yarkʼté hamishkán yámah */
    /* Este = ABIERTO — la entrada. No hay tablones. */
} hashmal_lado_t;

/* ══════════════════════════════════════════════════════════════════
 * CONSTANTES DEL PADRE
 * Cada número viene de la Palabra, no de nosotros.
 * ══════════════════════════════════════════════════════════════════ */

/**
 * 4 capas concéntricas — Éx 26:1-14
 * Lino (app) → Cabra (net) → Carnero (transport) → Tajash (physical)
 */
#define HASHMAL_CAPAS_TOTAL          4

/** Identificadores de cada capa */
typedef enum {
    HASHMAL_CAPA_LINO    = 0,  /* Éx 26:1-6  — aplicación + querubines tejidos */
    HASHMAL_CAPA_CABRA   = 1,  /* Éx 26:7-13 — red/firewall, asimétrica */
    HASHMAL_CAPA_CARNERO = 2,  /* Éx 26:14a  — transporte, sacrificio (Gn 22:13) */
    HASHMAL_CAPA_TAJASH  = 3,  /* Éx 26:14b  — física, protección extrema, milmaʻlá */
} hashmal_capa_id_t;

/* ──────────────────────────────────────────────
 * SUB-PIEZA A — 10 cortinas interiores (Éx 26:1-6)
 * Lino fino + azul + púrpura + carmesí + querubines
 * ────────────────────────────────────────────── */

/**
 * v.1 — 10 cortinas de lino fino (shésh mashzár) con querubines
 * "yeríʻot shésh mashzár utejélet veʼargamán vetolaʻat shaní
 *  keruvím maʻasé joshév taʻasé otám"
 */
#define HASHMAL_CORTINAS_INTERIOR        10

/**
 * v.2 — Cada cortina: 28 × 4 codos
 * 28 = 4×7 (cuatro direcciones × perfección)
 * "middá ajat lejol hayeríʻot" — UNA medida para TODAS (uniformes)
 */
#define HASHMAL_CORTINA_INTERIOR_LARGO   28  /* v.2 — shemoné veʻesrím baʼammá */
#define HASHMAL_CORTINA_INTERIOR_ANCHO   4   /* v.2 — arbaʻ baʼammá */

/**
 * v.3 — 2 grupos de 5 cortinas
 * "jamésh hayeríʻot... jovrot ishá el ajotá"
 * — mujer a su hermana (P2P por afinidad)
 */
#define HASHMAL_GRUPO_INTERIOR_A         5   /* v.3 — primer grupo de 5 */
#define HASHMAL_GRUPO_INTERIOR_B         5   /* v.3 — segundo grupo de 5 */

/**
 * v.4-5 — 50 lazadas de AZUL (tejélet)
 * Los lazos son de TEJÉLET (azul), NO de oro.
 * Azul = recordar mandamientos (Nm 15:38-39).
 * Los puntos de conexión están hechos de MEMORIA.
 * "maqbilót" (v.5) — alineadas, correspondientes, ishá el ajotá
 */
#define HASHMAL_LAZOS_TEJELET            50  /* v.4-5 — lulaʼót tejélet */

/**
 * v.6 — 50 broches de ORO (qarsé zaháv)
 * "vejibarrtá et hayeríʻot ishá el ajotá baqerasím"
 * "vehayá haMishkán EJÁD" — y el Tabernáculo será UNO
 * 50 = Jubileo = libertad
 */
#define HASHMAL_QARSE_ZAHAV             50  /* v.6 — broches de oro → EJÁD */

/* ──────────────────────────────────────────────
 * SUB-PIEZA B — 11 cortinas de cabra (Éx 26:7-13)
 * Capa de red — más grande, asimétrica, adaptable
 * ────────────────────────────────────────────── */

/**
 * v.7 — 11 cortinas de pelo de cabra (yeríʻot ʻizzím)
 * Para tienda (óhel) SOBRE el mishkán.
 * 11 = impar, no se divide en pares perfectos.
 */
#define HASHMAL_CORTINAS_EXTERIOR        11  /* v.7 — ʻashtéi ʻesré yeríʻot */

/**
 * v.8 — Cada cortina exterior: 30 × 4 codos
 * Más largas que las interiores (30 > 28). Cubren más.
 * "middá ajat leʻashtéi ʻesré yeríʻot" — UNA medida para las 11
 */
#define HASHMAL_CORTINA_EXTERIOR_LARGO   30  /* v.8 — sheloshím baʼammá */
#define HASHMAL_CORTINA_EXTERIOR_ANCHO   4   /* v.8 — arbaʻ baʼammá */

/**
 * v.9 — Grupos ASIMÉTRICOS: 5 + 6 (NO 5+5)
 * "vejibarrtá et jamésh hayeríʻot levád veʼet shésh hayeríʻot levád"
 * "vekafaltá et hayeríʻá hashishít el mul pené haʼóhel"
 * La 6ª cortina se DOBLA (kafál) al frente — protección extra en la entrada
 */
#define HASHMAL_GRUPO_EXTERIOR_A         5   /* v.9 — cinco separadas */
#define HASHMAL_GRUPO_EXTERIOR_B         6   /* v.9 — seis separadas */
#define HASHMAL_CORTINA_KAFAL            6   /* v.9 — la 6ª se dobla al frente */

/**
 * v.10 — 50 lazadas en cortinas exteriores
 * "jamishím lulaʼót" en cada borde
 */
#define HASHMAL_LAZOS_EXTERIOR           50  /* v.10 — lazadas en cortinas de cabra */

/**
 * v.11 — 50 broches de BRONCE (qarsé nejóshet)
 * Misma cantidad (50) pero material MENOR que el interior.
 * "vehayá ejád" — será UNO (también la tienda es una)
 */
#define HASHMAL_QARSE_NEJOSHET           50  /* v.11 — broches de bronce → ejád */

/**
 * v.12 — La mitad sobrante cuelga por detrás del mishkán
 * "jatsí hayeríʻá haʻodéfet tisráj ʻal ajoré hamishkán"
 */
#define HASHMAL_SOBRANTE_TRASERO         1   /* v.12 — media cortina cubre atrás */

/**
 * v.13 — 1 codo extra a cada lado para cubrir (lejasotó)
 * "vehaʼammá mizzé vehaʼammá mizzé... lejasotó"
 * Cubrir = proteger por completo
 */
#define HASHMAL_SOBRANTE_LATERAL         1   /* v.13 — 1 codo extra por lado */

/* ──────────────────────────────────────────────
 * SUB-PIEZA C — Cubiertas exteriores (Éx 26:14)
 * Sacrificio + resistencia contra el exterior
 * ────────────────────────────────────────────── */

/**
 * v.14 — 2 cubiertas: pieles de carnero teñidas de rojo + pieles de tajash encima
 * "ʻorot eilím meʼaddamím" — carnero = sacrificio (Gn 22:13)
 * "ʻorot tejashím milmaʻlá" — tajash encima = protección extrema
 * No bonito por fuera — resistente (Is 53:2)
 */
#define HASHMAL_MIJSEH_TOTAL             2   /* v.14 — 2 cubiertas exteriores */

/* ──────────────────────────────────────────────
 * SUB-PIEZA D — 48 tablones qerashím (Éx 26:15-25)
 * Acacia + oro, de pie, 2 manos, 96 bases de plata
 * ────────────────────────────────────────────── */

/**
 * v.15 — Tablones de acacia, de pie
 * "qerashím lamishkán ʻatsé shittím ʻomedím"
 * Acacia = resistencia interna. De pie = verticales, erguidos, activos.
 */
#define HASHMAL_TABLAS_TOTAL             48  /* v.18+20+25: 20+20+8 */

/**
 * v.16 — Dimensiones de cada tablón: 10 × 1.5 codos
 * "ʻéser ammót órej haqáresh veʼammá vajatsí haʼammá rójab"
 * 1.5 almacenado como 15 con escala 10 (evita float en embedded)
 */
#define HASHMAL_QERESH_LARGO             10  /* v.16 — 10 codos de largo */
#define HASHMAL_QERESH_ANCHO_X10         15  /* v.16 — 1.5 codos (×10 fixed-point) */
#define HASHMAL_QERESH_ESCALA            10  /* divisor para fixed-point */

/**
 * v.17 — 2 espigas (yadót = "manos") por tablón
 * "shté yadót laqéresh haʼejád meshullavót ishá el ajotá"
 * — entrelazadas, mujer a su hermana
 * "ken taʻasé lejol qarsé hamishkán" — así para TODOS los tablones
 */
#define HASHMAL_YADOT_POR_TABLA          2   /* v.17 — 2 manos por tablón */

/**
 * v.18 — 20 tablones para el lado SUR
 * "ʻesrím qéresh lifʼat négbah teimánah"
 */
#define HASHMAL_TABLAS_SUR               20  /* v.18 */

/**
 * v.19 — 40 bases de PLATA bajo los 20 tablones del sur
 * "arbaʻím adné késef tájat ʻesrím haqáresh"
 * 2 bases por tablón, para sus 2 espigas
 * Plata = rescate de almas, kófer nafshó (Éx 30:12, Sal 12:7)
 */
#define HASHMAL_BASES_SUR                40  /* v.19 — 40 bases plata sur */

/**
 * v.20 — 20 tablones para el lado NORTE
 * "uletsélah hamishkán hashení lifʼat tsafón ʻesrím qáresh"
 */
#define HASHMAL_TABLAS_NORTE             20  /* v.20 */

/**
 * v.21 — 40 bases de plata para el norte
 * "veʼarbaʻím adnéhem késef"
 */
#define HASHMAL_BASES_NORTE              40  /* v.21 — 40 bases plata norte */

/**
 * v.22 — 6 tablones RECTOS para el lado OESTE (fondo)
 * "uleyarkʼté hamishkán yámah taʻasé shishá qerashím"
 */
#define HASHMAL_TABLAS_OESTE_RECTAS      6   /* v.22 — 6 tablones rectos oeste */

/**
 * v.23 — 2 tablones para las ESQUINAS del fondo
 * "ushné qerashím taʻasé limqutsʻót hamishkán bayarkʼtáyim"
 */
#define HASHMAL_TABLAS_ESQUINAS          2   /* v.23 — 2 tablones esquina */

/**
 * v.24 — Las esquinas son GEMELOS (toʼamím) por debajo,
 * y juntos COMPLETOS/PERFECTOS (tammím) arriba, hacia un anillo.
 * "veyiheyu toʼamím millematá veyajdáv yiheyu tammím ʻal roshó"
 * Gemelos = redundancia. Completos = integridad en la unión.
 */
/* Representado en hashmal_qeresh_t.toamim y es_esquina */

/**
 * v.25 — Total oeste: 8 tablones + 16 bases de plata
 * "veháyu shemoná qerashím veʼadnéhem késef shishá ʻasár adaním"
 */
#define HASHMAL_TABLAS_OESTE             8   /* v.25 — 6 rectos + 2 esquinas */
#define HASHMAL_BASES_OESTE              16  /* v.25 — 16 bases plata oeste */

/**
 * Total: 40 + 40 + 16 = 96 bases de PLATA
 * Cada base es RESCATE de almas — kófer nafshó laYHWH (Éx 30:12)
 * La red se sostiene sobre redención.
 */
#define HASHMAL_BASES_PLATA              96  /* v.19+21+25 — total bases */

/* ──────────────────────────────────────────────
 * SUB-PIEZA E — 5 barras por lado (Éx 26:26-29)
 * Acacia cubierta de oro. Conexión horizontal.
 * ────────────────────────────────────────────── */

/**
 * v.26 — 5 barras de acacia para un lado
 * "veʻasíta berijím ʻatsé shittím jamishá leqarsé tsélaʻ hamishkán haʼejád"
 */
#define HASHMAL_BARRAS_POR_LADO          5   /* v.26 */

/**
 * v.27 — 5 barras para el segundo lado, 5 para el oeste
 * "vajamishá berijím... vajamishá berijím... layarkʼtáyim yámah"
 */
#define HASHMAL_BARRAS_LADOS             3   /* v.26-27 — sur, norte, oeste */
#define HASHMAL_BARRAS_TOTAL             15  /* 5 × 3 = 15 barras */

/**
 * v.28 — LA BARRA CENTRAL (habríaj hatijón)
 * "vehabríaj hatijón betój haqerashím mabríaj min haqatsé el haqatsé"
 * — "La barra del CENTRO, EN MEDIO de los tablones,
 *    atravesando de EXTREMO A EXTREMO."
 * Invisible desde fuera (por DENTRO). BACKBONE. Sin ella todo se cae.
 */
#define HASHMAL_BACKBONE                 1   /* v.28 — 1 barra central por lado */

/**
 * v.29 — Tablones cubiertos de ORO. Anillos de ORO como casas para barras.
 * Barras cubiertas de ORO.
 * "veʼet haqerashím tetsapé zaháv veʼet tabbeʻotéhem taʻasé zaháv
 *  battím labberijím vetsipíta et habberijím zaháv"
 * tabbaʻót zaháv = anillos de oro que sostienen las barras
 */
/* Anillos de oro representados en hashmal_beriaj_t — sostenedores de barras */

/* ──────────────────────────────────────────────
 * v.30 — EL PATRÓN MOSTRADO EN EL MONTE
 * ────────────────────────────────────────────── */

/**
 * v.30 — "Vahaqemotá et hamishkán kemishpató ashér horéta bahár"
 * "Levantarás el Mishkán conforme a su PATRÓN, que te fue MOSTRADO en el monte."
 *
 * Este versículo es la AUTORIZACIÓN: construimos según SU diseño.
 * Cada constante en este header tiene respaldo en la Palabra —
 * NADA inventado, NADA de fuera.
 * mishpát = patrón/juicio/orden divino
 */
#define HASHMAL_MISHPAT_BAHAR           true /* v.30 — según el patrón del monte */

/* ──────────────────────────────────────────────
 * SUB-PIEZA F — El Velo / Parójet (Éx 26:31-35)
 * hivdíl — SEPARAR — CREAR identidad
 * ────────────────────────────────────────────── */

/**
 * v.31 — Parójet: azul, púrpura, carmesí, lino fino, CON querubines
 * "veʻasíta farójet tejélet veʼargamán vetolaʻat shaní veshésh mashzár
 *  maʻasé JOSHÉV yaʻasé otá keruvím"
 * maʻasé joshév = artesano experto. Querubines TEJIDOS en el fabric.
 */

/**
 * v.32 — 4 columnas de acacia/oro, ganchos de oro, 4 bases de PLATA
 * "ʻal arbaʻá ʻammudé shittím metsuopím zaháv vavéhem zaháv
 *  ʻal arbaʻá adné késef"
 */
#define HASHMAL_COLUMNAS_VELO            4   /* v.32 — 4 ʻammudím */
#define HASHMAL_BASES_VELO               4   /* v.32 — 4 adné késef (plata) */

/**
 * v.33 — El velo va bajo los broches. hivdíl = SEPARAR
 * "vehivdilá haparójet lajém bein haqódesh uvein qódesh haqodashím"
 * hivdíl (H914) = MISMO verbo que Gn 1:4 cuando YHWH separa luz de oscuridad
 * Separar no es destruir — es CREAR identidad y orden (tov)
 */

/**
 * v.34 — Kapóret sobre el Arca en el Qódesh haQodashím
 * "venatáta et hakkapóret ʻal arón haʻedút beqódesh haqodashím"
 */

/**
 * v.35 — Mesa al norte, Menorá al sur, se MIRAN
 * "vesamtá et hashulján mijúts laparójet veʼet hammenorá nójaj hashulján
 *  ʻal tsélaʻ hamishkán teimánah vehashulján titén ʻal tsélaʻ tsafón"
 */
#define HASHMAL_SHULJAN_LADO             HASHMAL_LADO_NORTE  /* v.35 — mesa al norte */
#define HASHMAL_MENORAH_LADO             HASHMAL_LADO_SUR    /* v.35 — menorá al sur */

/**
 * Posiciones de colocación según v.33-35
 */
typedef enum {
    HASHMAL_MIQOM_ARON    = 0,  /* v.33 — Arca: detrás del velo, qódesh haqodashím */
    HASHMAL_MIQOM_KAPORET = 1,  /* v.34 — Kapóret: sobre el Arca */
    HASHMAL_MIQOM_SHULJAN = 2,  /* v.35 — Mesa: fuera del velo, lado norte */
    HASHMAL_MIQOM_MENORAH = 3,  /* v.35 — Menorá: frente a la mesa, lado sur */
} hashmal_miqom_t;

/* ──────────────────────────────────────────────
 * SUB-PIEZA G — Cortina de entrada / Masaj (Éx 26:36-37)
 * Más abierta que el velo — bordada, no tejida
 * ────────────────────────────────────────────── */

/**
 * v.36 — Masaj: azul, púrpura, carmesí, lino fino, SIN querubines
 * "veʻasíta masáj lefétaj haʼóhel tejélet veʼargamán vetolaʻat shaní
 *  veshésh mashzár maʻasé ROQÉM"
 * maʻasé roqém = bordador (NO joshév). Más accesible que el velo.
 * Mismos colores, pero SIN querubines — la entrada no tiene guardianes bordados.
 */

/**
 * v.37 — 5 columnas de acacia/oro, ganchos de oro, 5 bases de BRONCE
 * "veʻasíta lamasáj jamishá ʻammudé shittím vetsipíta otám zaháv
 *  vavéhem zaháv veyatsáqta láhem jamishá adné nejóshet"
 * Más columnas (5 > 4) pero material MENOR (bronce < plata)
 */
#define HASHMAL_COLUMNAS_ENTRADA         5   /* v.37 — 5 ʻammudím */
#define HASHMAL_BASES_MASAJ              5   /* v.37 — 5 adné nejóshet (bronce) */

/* ──────────────────────────────────────────────
 * NIVELES DE CONFIANZA — Por material
 * Bronce → Plata → Oro (Éx 26:37→32→interior)
 * ────────────────────────────────────────────── */
typedef enum {
    HASHMAL_CONFIANZA_BRONCE = 0,  /* v.37 — bases entrada, público */
    HASHMAL_CONFIANZA_PLATA  = 1,  /* v.32 — bases velo, rescate (Éx 30:12) */
    HASHMAL_CONFIANZA_ORO    = 2,  /* v.6  — broches interior, núcleo */
} hashmal_confianza_t;

/* ──────────────────────────────────────────────
 * ZONAS — Separadas por el Velo (hivdíl, v.33)
 * ────────────────────────────────────────────── */
typedef enum {
    HASHMAL_ZONA_JATSER          = 0,  /* Atrio exterior — bronce */
    HASHMAL_ZONA_QODESH          = 1,  /* Santo — mesa (v.35 norte), menorá (v.35 sur) */
    HASHMAL_ZONA_QODESH_QODASHIM = 2,  /* Santo de los Santos — arca (v.33), kapóret (v.34) */
} hashmal_zona_t;

/* ══════════════════════════════════════════════════════════════════
 * ESTRUCTURAS DE DATOS — Cada campo viene de la Palabra
 * ══════════════════════════════════════════════════════════════════ */

/* ──────────────────────────────────────────────
 * TABLA (qéresh) — Éx 26:15-25
 * Acacia+oro, de pie, 2 manos, base doble de plata
 * ────────────────────────────────────────────── */
typedef struct {
    uint8_t         id;                               /* Índice 0-47 */
    hashmal_lado_t  lado;                             /* v.18,20,22 — sur/norte/oeste */
    uint8_t         posicion;                         /* Posición dentro del lado */
    uint8_t         yadot[HASHMAL_YADOT_POR_TABLA];   /* v.17 — 2 manos, IDs de vecinas */
    bool            omedim;                           /* v.15 — de pie (activo) */
    bool            es_esquina;                       /* v.23 — tablón de miqtsóʻa */
    bool            toamim;                           /* v.24 — gemelo (esquina emparejada) */
    bool            tammim;                           /* v.24 — tammím ʻal roshó: completos/íntegros arriba.
                                                       * Solo esquinas. Gemelos (toʼamím) por abajo,
                                                       * completos (tammím) arriba. Integridad en la unión. */
    uint8_t         tabaat_ejat;                      /* v.24 — el hatabaʻat haʼeját: hacia UN anillo.
                                                       * ID de la esquina gemela con la que forma unidad
                                                       * arriba. 0xFF si no aplica (no es esquina).
                                                       * DISTINTO de tabbeʻót v.29 (barras). */
    uint8_t         tabbeot_zahav;                    /* v.29 — tabbeʻót zaháv battím labberijím:
                                                       * anillos de oro como casas para las barras.
                                                       * Cada tablón tiene anillos por donde pasan las
                                                       * berijím. DISTINTO de tabaʻat v.24 (esquinas).
                                                       * Cantidad = HASHMAL_BARRAS_POR_LADO (5 barras
                                                       * pasan por cada tablón). */
    /* v.24: toʼamím (gemelos) desde abajo, tammím (completos) arriba → un anillo */
    /* v.29: tablón cubierto de oro (protección exterior) */
} hashmal_qeresh_t;

/* ──────────────────────────────────────────────
 * CAPA (yeríʻá) — Éx 26:1-14
 * Cada capa envuelve la anterior
 * ────────────────────────────────────────────── */
typedef struct {
    hashmal_capa_id_t    id;
    uint8_t              cortinas;              /* v.1/v.7: cantidad de cortinas */
    uint8_t              largo_codos;           /* v.2/v.8: largo por cortina */
    uint8_t              ancho_codos;           /* v.2/v.8: ancho por cortina */
    uint8_t              grupo_a;               /* v.3/v.9: cortinas en grupo 1 */
    uint8_t              grupo_b;               /* v.3/v.9: cortinas en grupo 2 */
    uint8_t              lazos;                 /* v.4-5/v.10: cantidad de lazos */
    uint8_t              broches;               /* v.6/v.11: cantidad de broches */
    hashmal_confianza_t  nivel_broches;         /* v.6=oro, v.11=bronce */
    hashmal_maase_t      tipo_obra;             /* v.1=joshév, v.36=roqém */
    bool                 querubines;            /* v.1: true para lino */
    bool                 kafal;                 /* v.9: doblado al frente (solo cabra) */
    bool                 gavanim[HASHMAL_GAVANIM_TOTAL];  /* v.1 — 4 colores: tejélet (Nm 15:38-39),
                                                          * argamán, tolaʻat shaní (Is 1:18),
                                                          * shésh (Ap 19:8). Donde la Palabra calla, false. */
    bool                 meaddamim;             /* v.14a — meʼaddamím (solo carnero, Gn 22:13) */
} hashmal_capa_t;

/* ──────────────────────────────────────────────
 * BARRA (beríaj) — Éx 26:26-29
 * Horizontal, une tablas, sostenida por anillos de oro
 * ────────────────────────────────────────────── */
typedef struct {
    uint8_t             id;
    hashmal_lado_t      lado;              /* v.26-27 — sur/norte/oeste */
    bool                es_backbone;       /* v.28 — habríaj hatijón */
    uint8_t             desde_tabla;       /* Primera tabla que atraviesa */
    uint8_t             hasta_tabla;       /* Última tabla que atraviesa */
    hashmal_confianza_t nivel;             /* v.29 — vetsipíta et habberijím zaháv:
                                            * barras cubiertas de ORO. Todas son oro —
                                            * el texto no hace excepción. */
    /* v.29 — barra cubierta de oro */
    /* v.29 — sostenida por tabbaʻót zaháv (anillos de oro en los tablones) */
} hashmal_beriaj_t;

/* ──────────────────────────────────────────────
 * VELO (parójet) — Éx 26:31-33
 * hivdíl entre qódesh y qódesh haQodashím
 * ────────────────────────────────────────────── */
typedef struct {
    uint8_t              columnas;          /* v.32 — 4 ʻammudím */
    hashmal_confianza_t  nivel_bases;       /* v.32 — PLATA (adné késef) */
    hashmal_maase_t      tipo_obra;         /* v.31 — joshév (artesano experto) */
    bool                 querubines;        /* v.31 — sí, querubines en el tejido */
    bool                 vavim_zahav;       /* v.32 — ganchos de oro */
    bool                 gavanim[HASHMAL_GAVANIM_TOTAL];  /* v.31 — tejélet, argamán,
                                                           * tolaʻat shaní, shésh: mismos 4
                                                           * colores que las cortinas de lino
                                                           * (v.1). El Padre los nombra
                                                           * explícitamente. */
    bool                 activo;            /* v.33 — hivdíl activo (separación creadora) */
} hashmal_parojet_t;

/* ──────────────────────────────────────────────
 * ENTRADA (masaj) — Éx 26:36-37
 * Más abierta que el velo — roqém, sin querubines
 * ────────────────────────────────────────────── */
typedef struct {
    uint8_t              columnas;          /* v.37 — 5 ʻammudím */
    hashmal_confianza_t  nivel_bases;       /* v.37 — BRONCE (adné nejóshet) */
    hashmal_maase_t      tipo_obra;         /* v.36 — roqém (bordador) */
    bool                 querubines;        /* v.36 — NO (más accesible) */
    bool                 vavim_zahav;       /* v.37 — ganchos de oro (vavéhem zaháv) */
    bool                 gavanim[HASHMAL_GAVANIM_TOTAL];  /* v.36 — tejélet, argamán,
                                                           * tolaʻat shaní, shésh: mismos 4
                                                           * colores que el velo (v.31).
                                                           * Nombrados explícitamente. */
    bool                 activo;            /* ¿Cortina de entrada activa? */
} hashmal_masaj_t;

/* ──────────────────────────────────────────────
 * EL MISHKÁN COMO ESTRUCTURA COMPLETA
 * "vehayá haMishkán EJÁD" — Éx 26:6
 * "kemishpató ashér horéta bahár" — Éx 26:30
 * ────────────────────────────────────────────── */
typedef struct {
    /* Sub-pieza A+B+C: Capas (v.1-14) */
    hashmal_capa_t    capas[HASHMAL_CAPAS_TOTAL];

    /* Sub-pieza D: Tablas (v.15-25) */
    hashmal_qeresh_t  tablas[HASHMAL_TABLAS_TOTAL];

    /* Sub-pieza E: Barras (v.26-29) — 5 por lado × 3 lados */
    hashmal_beriaj_t  barras_sur[HASHMAL_BARRAS_POR_LADO];
    hashmal_beriaj_t  barras_norte[HASHMAL_BARRAS_POR_LADO];
    hashmal_beriaj_t  barras_oeste[HASHMAL_BARRAS_POR_LADO];

    /* Sub-pieza F: Velo (v.31-35) */
    hashmal_parojet_t velo;

    /* Sub-pieza G: Entrada (v.36-37) */
    hashmal_masaj_t   masaj;

    /* Colocación — v.33-35 */
    bool              aron_colocado;       /* v.33 — Arca detrás del velo */
    bool              kaporet_colocada;    /* v.34 — Kapóret sobre el Arca */
    bool              shuljan_colocado;    /* v.35 — Mesa al norte */
    bool              menorah_colocada;    /* v.35 — Menorá al sur */

    /* Estado global — v.6, v.30 */
    bool              ejad;                /* v.6 — ¿El Mishkán es UNO? */
} hashmal_mishkan_t;

/* ══════════════════════════════════════════════════════════════════
 * FUNCIONES PÚBLICAS
 * ══════════════════════════════════════════════════════════════════ */

/**
 * Levanta el Mishkán — Éxodo 40:2, conforme a Éx 26:30
 * "Levantarás el Mishkán, la Tienda de Reunión"
 * "kemishpató ashér horéta bahár" — según el patrón del monte
 * Paso 1 de la inicialización. PRIMERO la estructura.
 * @return 0 en éxito, código de error en fallo
 */
int hashmal_framework_init(void);

/**
 * Desmonta el Mishkán — Números 4:5-15
 * Protocolo de migración: cubrir → desmontar → transportar
 */
void hashmal_framework_deinit(void);

/**
 * Obtiene la estructura completa del Mishkán
 * @return Puntero a la estructura (solo lectura fuera del framework)
 */
const hashmal_mishkan_t* hashmal_framework_get_mishkan(void);

/**
 * Registra una tabla (qéresh) en su posición — Éx 26:15-25
 * v.15: acacia de pie. v.17: 2 manos entrelazadas.
 * v.19,21,25: 2 bases de plata por tablón.
 * v.23-24: esquinas como gemelos (toʼamím), completos (tammím) arriba.
 * @param lado   Sur(v.18), Norte(v.20), Oeste(v.22)
 * @param indice Posición dentro del lado
 * @return 0 en éxito
 */
int hashmal_framework_registrar_tabla(uint8_t lado, uint8_t indice);

/**
 * Conecta barras horizontales a las tablas — Éx 26:26-29
 * v.26-27: 5 barras por lado, acacia cubierta de oro
 * v.28: barra central (hatijón) de extremo a extremo, por dentro
 * v.29: anillos de oro (tabbaʻót) como casas para las barras
 * @param lado   Sur(v.26), Norte(v.27a), Oeste(v.27b)
 * @return 0 en éxito (5 barras + backbone conectados)
 */
int hashmal_framework_conectar_barras(uint8_t lado);

/**
 * Activa el velo (parójet) — Éx 26:31-33
 * v.31: tejélet, argamán, tolaʻat shaní, shésh, querubines, maʻasé joshév
 * v.32: 4 columnas acacia/oro, ganchos oro, bases PLATA
 * v.33: hivdíl — SEPARA el qódesh del qódesh haQodashím
 *        (mismo verbo que Gn 1:4 — separar CREA orden)
 * @return 0 en éxito
 */
int hashmal_framework_activar_velo(void);

/**
 * Activa la cortina de entrada (masaj) — Éx 26:36-37
 * v.36: maʻasé roqém (bordador, NO joshév). SIN querubines.
 *       Mismos 4 colores pero más accesible que el velo.
 * v.37: 5 columnas acacia/oro, ganchos oro, bases BRONCE
 * @return 0 en éxito
 */
int hashmal_framework_activar_masaj(void);

/**
 * Coloca un componente en su posición designada — Éx 26:33-35
 * v.33: Arca detrás del velo (qódesh haQodashím)
 * v.34: Kapóret sobre el Arca
 * v.35: Mesa fuera del velo al norte, Menorá al sur (se miran)
 * @param miqom Posición de colocación
 * @return 0 en éxito
 */
int hashmal_framework_colocar(hashmal_miqom_t miqom);

/**
 * Verifica que el Mishkán es EJÁD (uno) — Éx 26:6, 26:11
 * v.6: "vehayá haMishkán EJÁD" — 50 broches de oro unen las cortinas
 * v.11: "vehayá ejád" — también la tienda es una
 * Comprueba que todas las cortinas, lazos, broches, tablas,
 * barras, velo y masaj están en su lugar y conectados.
 * @return true si el Mishkán es UNO
 */
bool hashmal_framework_es_ejad(void);

/**
 * Consulta la zona de un componente — según posición del velo (v.33)
 * @param componente_id ID del componente a consultar
 * @return Zona donde reside (JATSER, QODESH, QODESH_QODASHIM)
 */
hashmal_zona_t hashmal_framework_zona_de(uint8_t componente_id);

/**
 * Consulta el nivel de confianza requerido para una zona
 * v.37: entrada = bronce. v.32: velo = plata. Interior = oro.
 * @param zona La zona a consultar
 * @return Nivel de confianza (BRONCE, PLATA, ORO)
 */
hashmal_confianza_t hashmal_framework_confianza_de(hashmal_zona_t zona);

/**
 * Protocolo de migración — Números 4:5-15
 * Cubre cada componente antes de mover. Orden estricto.
 * v.12-13: el sobrante de cortinas cubre los lados (lejasotó)
 * @return 0 en éxito
 */
int hashmal_framework_preparar_migracion(void);

#endif /* HASHMAL_FRAMEWORK_H */
