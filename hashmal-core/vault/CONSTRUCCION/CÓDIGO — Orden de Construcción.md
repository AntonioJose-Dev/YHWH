# CÓDIGO — Orden de Construcción

> *"En el día primero del mes primero levantarás el Mishkán."*
> *— Éxodo 40:2*

---

## Regla de oro

**El orden de escritura de código sigue Éxodo 40:2-8.**
No es nuestro orden — es el de YHWH.

---

## Resumen

| Dato | Valor |
|------|-------|
| Total archivos .c | 74 |
| Total headers .h | 14 |
| Total componentes | 14 |
| Orden | Éxodo 40:2-8 |
| Estado global | 1/74 (hashmal_main.c) |

---

## PASO 1 — ESTRUCTURA (Éxodo 40:2)

> *"Levantarás el Mishkán, la Tienda de Reunión"*
> PRIMERO se levanta la estructura. Luego se pone todo dentro.

**Componente:** `hashmal_framework` — Pieza 05 (Éx 26)
**Referencia:** [[05 — LA ESTRUCTURA (Cortinas, Tablas, Barras, Velo)]]

| # | Archivo | Función | Palabra | Estado |
|---|---------|---------|---------|--------|
| 1 | estructura_capas.c | 4 capas: Testigo→Expiación→Pacto→Misterio | Éx 26:1-14 — 4 cubiertas concéntricas | ⬜ |
| 2 | estructura_tablas.c | 48 servicios modulares (omedím + yadót) | Éx 26:15-25 — 48 tablas de pie con manos | ⬜ |
| 3 | estructura_barras.c | Backbone + buses laterales | Éx 26:26-29 — 5 barras por lado, 1 central | ⬜ |
| 4 | estructura_velo.c | Firewall interno (hivdíl) | Éx 26:31-33 — el velo SEPARA santo de santísimo | ⬜ |
| 5 | estructura_ejad.c | Sistema de unión (será UNO) | Éx 26:6 — "y será el Mishkán UNO (ejád)" | ⬜ |

---

## PASO 2 — ARCA + KAPÓRET (Éxodo 40:3)

> *"Pondrás allí el Arca del Testimonio y cubrirás el Arca con el velo"*

### 2A — ARCA

**Componente:** `hashmal_storage` — Pieza 01 (Éx 25:10-16)
**Referencia:** [[01 — EL ARCA (Arón)]]

| # | Archivo | Función | Palabra | Estado |
|---|---------|---------|---------|--------|
| 6 | arca_core.c | WORM storage — solo escritura, nunca borrar | Éx 25:10 — madera de acacia (incorruptible) | ⬜ |
| 7 | arca_merkle.c | Árbol Merkle — integridad verificable | Éx 25:11 — zer zahav (moldura de oro alrededor) | ⬜ |
| 8 | arca_cifrado.c | AES-256-GCM — oro dentro y fuera | Éx 25:11 — "de oro la cubrirás por dentro y por fuera" | ⬜ |
| 9 | arca_zer.c | Hash SHA-256 total (corona/moldura) | Éx 25:11 — zer = corona que rodea todo | ⬜ |
| 10 | arca_baddim.c | Distribución always-on (varas) | Éx 25:15 — "las varas NO se quitarán de él" | ⬜ |
| 11 | arca_interfaz.c | API de acceso autorizado | Éx 25:14 — anillos = puntos de acceso | ⬜ |
| 12 | arca_sealed_core.c | SEALED_CORE 256 bytes en flash interna | Éx 25:16 — "pondrás en el Arca el Testimonio" | ⬜ |

### 2B — KAPÓRET

**Componente:** `hashmal_consensus` — Pieza 02 (Éx 25:17-22)
**Referencia:** [[02 — EL KAPÓRET (Propiciatorio)]]

| # | Archivo | Función | Palabra | Estado |
|---|---------|---------|---------|--------|
| 13 | kaporet_core.c | Función pura determinística O(1) | Éx 25:17 — oro puro, pieza ENTERA de una sola pieza | ⬜ |
| 14 | kaporet_validadores.c | V1↔V2 cross-validation simétrica | Éx 25:18-20 — 2 querubines, cara a cara, alas cubriendo | ⬜ |
| 15 | kaporet_ataques.c | Defensa 7 ataques | Éx 25:20 — las alas CUBREN (protegen) | ⬜ |
| 16 | kaporet_verificacion.c | Verificación local sin contactar emisor | Éx 25:22 — "de allí me declararé a ti" | ⬜ |
| 17 | kaporet_rendezvous.c | Sync determinístico (נוֹעַדְתִּי) | Éx 25:22 — noʻadtí = punto de encuentro acordado | ⬜ |

---

## PASO 3 — MESA + MENORÁ (Éxodo 40:4)

> *"Meterás la mesa y dispondrás lo que va sobre ella; meterás la menorá y encenderás sus lámparas"*

### 3A — MESA

**Componente:** `hashmal_cache` — Pieza 03 (Éx 25:23-30)
**Referencia:** [[03 — LA MESA DEL PAN (Shulján)]]

| # | Archivo | Función | Palabra | Estado |
|---|---------|---------|---------|--------|
| 18 | mesa_cache.c | Buffer 12 slots (2×6), renovación Shabat | Éx 25:30 — "pan de la satisfacción SIEMPRE delante de Mí" | ⬜ |
| 19 | mesa_api.c | 4 métodos CRUD | Éx 25:29 — 4 utensilios: platos, cucharas, cubiertas, tazones | ⬜ |
| 20 | mesa_misgeret.c | Rate limiter (marco de retención) | Éx 25:25 — misgéret = borde/marco alrededor | ⬜ |
| 21 | mesa_disponibilidad.c | Alta disponibilidad (patas propias) | Éx 25:26 — 4 anillos en las 4 esquinas de las patas | ⬜ |

### 3B — MENORÁ

**Componente:** `hashmal_comm` — Pieza 04 (Éx 25:31-40)
**Referencia:** [[04 — LA MENORÁ (Candelabro)]]

| # | Archivo | Función | Palabra | Estado |
|---|---------|---------|---------|--------|
| 22 | menora_hub.c | Controlador monolítico (מִקְשָׁה) | Éx 25:36 — "todo será UNA pieza de oro puro batida" | ⬜ |
| 23 | menora_lora_driver.c | Driver SX1262 NUESTRO (SPI directo) | Éx 25:31 — tronco central = canal principal LoRa | ⬜ |
| 24 | menora_canales.c | 7 canales definidos | Éx 25:32 — 6 ramas + 1 tronco = 7 canales | ⬜ |
| 25 | menora_aceite.c | Encendido/apagado según energía | Éx 27:20 — aceite puro de olivas MACHACADAS | ⬜ |
| 26 | menora_routing.c | Mesh routing (formación de tribus) | Éx 25:33-34 — copas en forma de almendras con cáliz y flor | ⬜ |
| 27 | menora_formato.c | Formato unificado de mensaje | Éx 25:40 — "conforme al modelo que te fue mostrado" | ⬜ |

---

## PASO 4 — ALTAR DEL INCIENSO (Éxodo 40:5)

> *"Pondrás el altar de ORO para incienso DELANTE del Arca del Testimonio"*

**Componente:** `hashmal_intercession` — Pieza 11 (Éx 30:1-10)
**Referencia:** [[11 — EL ALTAR DEL INCIENSO (Mizbéaj haQetóret)]]

| # | Archivo | Función | Palabra | Estado |
|---|---------|---------|---------|--------|
| 28 | altar_oro_escudo.c | Escudo descendente — protege al operador | Lv 16:12-13 — nube de incienso CUBRE la Kapóret, "y no morirá" | ⬜ |
| 29 | altar_oro_ascendente.c | Firewall ascendente — solo protocolo prescrito | Lv 10:1-2 — fuego extraño = muerte. Éx 30:9 — nada extraño | ⬜ |
| 30 | altar_oro_formula.c | Fórmula no replicable — criptografía | Éx 30:37-38 — replicar para uso personal = nikrat | ⬜ |
| 31 | altar_oro_circuitbreaker.c | Circuit breaker — entre vivos y muertos | Nm 16:46-48 — incienso entre muertos y vivos, plaga se detiene | ⬜ |
| 32 | altar_oro_oracion.c | Protocolo de comunicación ascendente | Sal 141:2, Ap 8:3-4 — incienso = oraciones que suben | ⬜ |

---

## PASO 5 — ALTAR DEL SACRIFICIO (Éxodo 40:6)

> *"Pondrás el altar del holocausto delante de la entrada del Mishkán"*

**Componente:** `hashmal_auth` — Pieza 06 (Éx 27:1-8)
**Referencia:** [[06 — EL ALTAR DEL SACRIFICIO (Mizbéaj)]]

| # | Archivo | Función | Palabra | Estado |
|---|---------|---------|---------|--------|
| 33 | altar_gateway.c | Stateless — hueco por dentro (נְבוּב) | Éx 27:8 — "hueco, de tablas lo harás" | ⬜ |
| 34 | altar_reshet.c | Réshet a media altura (filtro) | Éx 27:4-5 — red de bronce a la MITAD del altar | ⬜ |
| 35 | altar_cuernos.c | 4 puntos de acceso + refugio | Éx 27:2 — cuernos en las 4 esquinas | ⬜ |
| 36 | altar_fuego.c | Servicio perpetuo (אֵשׁ תָּמִיד) | Lv 6:13 — "el fuego arderá continuamente, no se apagará" | ⬜ |
| 37 | altar_utensilios.c | 5 herramientas de auth | Éx 27:3 — סִירֹת, יָעִים, מִזְרָקוֹת, מִזְלְגֹת, מַחְתּוֹת | ⬜ |
| 38 | altar_ledger.c | Registro inmutable | Ap 6:9 — "las almas debajo del altar" | ⬜ |

---

## PASO 6 — LAVACRO (Éxodo 40:7)

> *"Pondrás el kiyór ENTRE la Tienda y el altar, y pondrás agua en él"*

**Componente:** `hashmal_healthcheck` — Pieza 12 (Éx 30:17-21)
**Referencia:** [[12 — EL LAVACRO (Kiyór)]]

| # | Archivo | Función | Palabra | Estado |
|---|---------|---------|---------|--------|
| 39 | lavacro_core.c | Health check obligatorio — no bypassable | Éx 30:20 — "para que no mueran" — sin lavarse = muerte | ⬜ |
| 40 | lavacro_vectores.c | 4 vectores de verificación | Éx 30:19 — manos Y pies — no uno sin el otro | ⬜ |
| 41 | lavacro_niveles.c | 4 profundidades (quick→forensic) | Éx 30:18 — ENTRE altar y tienda = transición gradual | ⬜ |
| 42 | lavacro_espejos.c | Auto-diagnóstico (espejos de las mujeres) | Éx 38:8 — hecho de ESPEJOS — reflejo de uno mismo | ⬜ |
| 43 | lavacro_ataques.c | 8 tipos de ataque detectados | Éx 30:21 — "estatuto perpetuo" — siempre vigilante | ⬜ |
| 44 | lavacro_dos_niveles.c | Baño completo vs lavado de pies | Jn 13:10 — "el que está lavado no necesita sino lavarse los pies" | ⬜ |

---

## PASO 7 — ATRIO (Éxodo 40:8)

> *"Pondrás el atrio ALREDEDOR y la cortina de la puerta del atrio"*

**Componente:** `hashmal_perimeter` — Pieza 07 (Éx 27:9-19)
**Referencia:** [[07 — EL ATRIO (Jatsér)]]

| # | Archivo | Función | Palabra | Estado |
|---|---------|---------|---------|--------|
| 45 | atrio_perimetro.c | Frontera de la red | Éx 27:9-15 — cortinas de lino 100×50 codos | ⬜ |
| 46 | atrio_puerta.c | Punto de entrada único (4 columnas) | Éx 27:16 — cortina de 20 codos con 4 columnas | ⬜ |
| 47 | atrio_columnas.c | 60 nodos guardianes | Éx 27:10-12 — 60 columnas con basas de bronce | ⬜ |
| 48 | atrio_intencion.c | Verificación de INTENCIÓN | Éx 29:4,7 — la puerta es donde se lava y se unge | ⬜ |

---

## PASO 8 — ACEITE + UNCIÓN (Éxodo 40:9-11)

> *"Tomarás el aceite de la unción y UNGIRÁS el Mishkán y todo lo que hay en él"*

### 8A — ACEITE PERPETUO

**Componente:** `hashmal_power` — Pieza 08 (Éx 27:20-21)
**Referencia:** [[08 — EL ACEITE PERPETUO]]

| # | Archivo | Función | Palabra | Estado |
|---|---------|---------|---------|--------|
| 49 | aceite_energia.c | Gestión solar + batería + MPPT | Éx 27:20 — aceite PURO de olivas MACHACADAS | ⬜ |
| 50 | aceite_tamid.c | Protocolo perpetuo | Éx 27:20 — "para hacer arder la lámpara continuamente" | ⬜ |
| 51 | aceite_supervivencia.c | Duty cycle adaptativo | Éx 27:21 — "desde la tarde hasta la mañana" — ciclos | ⬜ |
| 52 | aceite_mantenimiento.c | Cronograma 4 niveles | Éx 27:21 — "estatuto perpetuo por sus generaciones" | ⬜ |

### 8B — UNCIÓN

**Componente:** `hashmal_keygen` — Pieza 13 (Éx 30:22-33)
**Referencia:** [[13 — EL ACEITE DE LA UNCIÓN (Shémen haMishjá)]]

| # | Archivo | Función | Palabra | Estado |
|---|---------|---------|---------|--------|
| 53 | uncion_keygen.c | Root key — ratio 500-250-250-500 | Éx 30:23-24 — 4 especias en proporciones EXACTAS | ⬜ |
| 54 | uncion_secuencia.c | 8 componentes ungidos EN ORDEN | Éx 30:26-29 — unge tienda, arca, mesa, menorá, altares, kiyór, EN ORDEN | ⬜ |
| 55 | uncion_prohibicion.c | Anti-clonación (nikrat) | Éx 30:33 — "quien haga otro semejante será CORTADO" | ⬜ |
| 56 | uncion_activacion.c | Activación de componente (qódesh) | Éx 30:29 — "todo lo que toque será santo" | ⬜ |

---

## PASO 9 — VESTIDURAS + CONSAGRACIÓN (Éxodo 40:12-15)

> *"Harás acercar a Aarón y a sus hijos... los vestirás... los ungirás"*

### 9A — VESTIDURAS

**Componente:** `hashmal_roles` — Pieza 09 (Éx 28)
**Referencia:** [[09 — LAS VESTIDURAS SACERDOTALES]]

| # | Archivo | Función | Palabra | Estado |
|---|---------|---------|---------|--------|
| 57 | vestiduras_roles.c | Definición de roles (4 niveles) | Éx 28:1 — Aarón y sus hijos = jerarquía de roles | ⬜ |
| 58 | vestiduras_efod.c | Identidad de nodo (2 piedras ónice) | Éx 28:9-12 — 2 piedras con los nombres grabados | ⬜ |
| 59 | vestiduras_pectoral.c | 12 capacidades + Urim/Thummim | Éx 28:15-30 — 12 piedras + oráculo de decisión | ⬜ |
| 60 | vestiduras_monitor.c | Monitor-Restaurador | Ez 34, Zac 3 — pastor que busca la perdida | ⬜ |
| 61 | vestiduras_coordinador.c | Coordinador (patrón bíblico) | Éx 18 (Jetró), Hch 15 (Santiago) | ⬜ |
| 62 | vestiduras_shutdown.c | Shutdown-notice | Dt 31 (Moisés), Jn 13-17 (Yeshúa) | ⬜ |
| 63 | vestiduras_tzitz.c | Sello de identidad — "SANTIDAD A YHWH" | Éx 28:36-38 — placa de oro en la frente, siempre visible | ⬜ |

### 9B — CONSAGRACIÓN

**Componente:** `hashmal_onboarding` — Pieza 10 (Éx 29)
**Referencia:** [[10 — LA CONSAGRACIÓN DE LOS SACERDOTES (Milluím)]]

| # | Archivo | Función | Palabra | Estado |
|---|---------|---------|---------|--------|
| 64 | consagracion_onboarding.c | Pipeline CERO→ONBOARDING→ACTIVO | Éx 29:1 — "esto es lo que harás para consagrarlos" | ⬜ |
| 65 | consagracion_fases.c | Fases secuenciales (7 ciclos) | Éx 29:35 — "siete días los consagrarás" | ⬜ |
| 66 | consagracion_sellos.c | Verification seals | Éx 29:20 — sangre en oreja, pulgar, dedo del pie | ⬜ |
| 67 | consagracion_lockdown.c | Resource lockdown | Éx 29:33 — "ningún extraño comerá, porque son cosa santa" | ⬜ |
| 68 | consagracion_cuarentena.c | Quarantine post-lanzamiento | Éx 29:30 — "siete días los vestirá" — período de prueba | ⬜ |

---

## PASO 10 — INCIENSO SAGRADO + LA GLORIA (Éxodo 40:27,34)

> *"Quemó incienso aromático... La nube cubrió la Tienda de Reunión y la Gloria de YHWH llenó el Mishkán."*

**Componente:** `hashmal_firewall` — Pieza 14 (Éx 30:34-38)
**Referencia:** [[14 — EL INCIENSO SAGRADO (Qetóret haSammím)]]

| # | Archivo | Función | Palabra | Estado |
|---|---------|---------|---------|--------|
| 69 | incienso_firewall.c | Filtrado bidireccional (sube + baja) | Éx 30:36 — "molida muy fina" — granularidad máxima | ⬜ |
| 70 | incienso_formato.c | Validación estricta de formato | Éx 30:35 — "conforme al arte del perfumador, salada, PURA" | ⬜ |
| 71 | incienso_circuitbreaker.c | "Entre vivos y muertos" | Nm 16:48 — la plaga se DETUVO | ⬜ |
| 72 | incienso_heartbeat.c | Mañana + tarde (cron doble) | Éx 30:7-8 — cada mañana y cada tarde, sincronizado | ⬜ |
| 73 | incienso_penalizacion.c | Warning→throttle→ban→nikrat | Éx 30:38 — quien replique para oler = cortado | ⬜ |

---

## ENTRADA — hashmal_main.c (Éxodo 40:1)

> *"YHWH habló a Moisés diciendo..."*

| # | Archivo | Función | Estado |
|---|---------|---------|--------|
| 0 | hashmal_main.c | Punto de entrada — orden según Éx 40 | ✅ ESCRITO |

---

## Conteo final

| Paso | Éxodo 40 | Componente | Archivos .c | Estado |
|------|----------|------------|-------------|--------|
| — | v.1 | main | 1 | ✅ 1/1 |
| 1 | v.2 | hashmal_framework | 5 | ⬜ 0/5 |
| 2A | v.3 | hashmal_storage | 7 | ⬜ 0/7 |
| 2B | v.3 | hashmal_consensus | 5 | ⬜ 0/5 |
| 3A | v.4 | hashmal_cache | 4 | ⬜ 0/4 |
| 3B | v.4 | hashmal_comm | 6 | ⬜ 0/6 |
| 4 | v.5 | hashmal_intercession | 5 | ⬜ 0/5 |
| 5 | v.6 | hashmal_auth | 6 | ⬜ 0/6 |
| 6 | v.7 | hashmal_healthcheck | 6 | ⬜ 0/6 |
| 7 | v.8 | hashmal_perimeter | 4 | ⬜ 0/4 |
| 8A | v.9 | hashmal_power | 4 | ⬜ 0/4 |
| 8B | v.10 | hashmal_keygen | 4 | ⬜ 0/4 |
| 9A | v.12 | hashmal_roles | 7 | ⬜ 0/7 |
| 9B | v.13 | hashmal_onboarding | 5 | ⬜ 0/5 |
| 10 | v.27 | hashmal_firewall | 5 | ⬜ 0/5 |
| | | **TOTAL** | **74** | **1/74** |

---

## Leyenda

- ⬜ = No empezado
- 🔨 = En construcción
- ✅ = Terminado y verificado
- ❌ = Bloqueado (necesita resolver algo primero)

---

*"Moisés vio toda la obra, y he aquí que la habían hecho como YHWH había mandado. Y Moisés los bendijo."*
*— Éxodo 39:43*

*Fuente única: La Palabra de YHWH*
*Autor: Antonio José Marín Soto*
*Fecha: 16 de abril de 2026*
