# Verificado #25 — PARTE VII — Estado de Integración de Añadiduras en Headers

> *«De entre los dos querubines que están sobre el Arca del Testimonio, de allí me declararé a ti»*
> *— Éxodo 25:22*

**Fecha:** 25 abril 2026
**Método:** Sistema de los Querubines (Éx 25:22)
**Fuente auditada:** AUDITORÍA "Mil Ojos como Querubín" — Tablas de PARTE VII (líneas 349-379)
**Verificación contra:** Headers EXPANDIDOS en `Mishkán-YHWH/hashmal-firmware/components/*/include/*.h`

---

## HALLAZGO ESTRUCTURAL — La auditoría examinó stubs, no headers reales

La auditoría fue realizada contra los headers del directorio **CÓDIGO/** (stubs de ~20 líneas, solo `init/deinit`). Los headers REALES del vault **Mishkán-YHWH/** ya están EXPANDIDOS (700-1000+ líneas cada uno) con las 22 añadiduras integradas. Esto cambia RADICALMENTE el estado de las 18 claims de la auditoría.

**Evidencia directa (wc -l):**
- `CÓDIGO/hashmal_cache.h` → 21 líneas (stub)
- `Mishkán-YHWH/.../hashmal_cache.h` → 1044 líneas (expandido con MAN, SHINNAN, LEKET, MA'ASER, MOEDÍM)
- `CÓDIGO/hashmal_comm.h` → 19 líneas (stub)
- `CÓDIGO/hashmal_perimeter.h` → 19 líneas (stub)
- `CÓDIGO/hashmal_power.h` → 19 líneas (stub)
- vs. vault Mishkán-YHWH: 850, 511 líneas respectivamente

---

## TABLA 1 — "Errores definidos pero SIN funciones" (8 entries)

### 1. Cache (03) + Añadidura 11 (MAN) — ✅ RESUELTO

- **Claim auditoría:** Error ✅, funciones ❌
- **Estado REAL:** Error `ERR_MAN_EXCEDIDO` (línea 136) ✅ + funciones `hashmal_man_consumir`, `hashmal_man_cuota_consultar`, `hashmal_man_expirar`, `hashmal_man_mishne_set`, `hashmal_man_mishne_activo` → sección `mesa_man.c` con 4 funciones, enum de 4 recursos, struct de estado, constantes con Éx 16.

### 2. Cache (03) + Añadidura 17 (SHINNAN) — ✅ RESUELTO

- **Claim auditoría:** Error ✅, funciones ❌
- **Estado REAL:** Error `ERR_SHINNAN` (línea 137) ✅ + funciones `hashmal_shinnan_estado_set`, `hashmal_shinnan_que_propagar`, `hashmal_shinnan_propagar`, `hashmal_shinnan_info` → sección `mesa_shinnan.c` con 4 estados (Dt 6:7), matriz capacidad×antigüedad (Prov 22:6).

### 3. Cache (03) + Añadidura 18 (LEKET) — ✅ RESUELTO

- **Claim auditoría:** Error ✅, funciones ❌
- **Estado REAL:** Error `ERR_LEKET` (línea 138) ✅ + funciones `hashmal_leket_es_debil`, `hashmal_leket_recoger`, `hashmal_leket_facilitar`, `hashmal_leket_peah_get`, `hashmal_leket_info` → sección `mesa_leket.c` con 4 tipos vulnerabilidad (Lv 19:10 + Dt 24:19), 3 formas espigueo.

### 4. Cache (03) + Añadidura 19 (MA'ASER) — ✅ RESUELTO

- **Claim auditoría:** Error ✅, funciones ❌
- **Estado REAL:** Error `ERR_MAASER` (línea 139) ✅ + funciones `hashmal_maaser_contribuir`, `hashmal_maaser_diezmo_del_diezmo`, `hashmal_maaser_primicias_get`, `hashmal_maaser_info` → sección `mesa_maaser.c` con 4 tipos contribución, Nm 18:26 (diezmo del diezmo), Pr 3:9 (primicias).

### 5. Healthcheck (12) + Añadidura 13 (CORDÓN TRIPLE) — ✅ RESUELTO

- **Claim auditoría:** Error ✅, funciones ❌
- **Estado REAL:** Error `ERR_MESHULASH` (línea 155) ✅ + funciones `hashmal_hc_meshulash_actualizar`, `hashmal_hc_meshulash_estado`, `hashmal_hc_meshulash_buscar`, `hashmal_hc_meshulash_puede_ser_goel` → sección `lavacro_meshulash.c` con 4 estados (Ec 4:9-12), integración con Go'el (Añadidura 12).

### 6. Healthcheck (12) + Añadidura 16 (MAYIM MIN HATSUR) — ✅ RESUELTO

- **Claim auditoría:** Error ✅, funciones ❌
- **Estado REAL:** Error `ERR_TSUR` (línea 156) ✅ + funciones `hashmal_tsur_evaluar`, `hashmal_tsur_nivel_actual`, `hashmal_tsur_info`, `hashmal_tsur_es_emergencia` → sección `lavacro_tsur.c` con 4 niveles (Éx 17:6), 5 testigos bíblicos.

### 7. Keygen (13) + Añadidura 15 (DAM AL HAMEZUZOT) — ⚠️ DEBATIBLE

- **Claim auditoría:** Error ✅, funciones ❌
- **Estado REAL:** NO hay error de Mezuzot en `hashmal_keygen.h`. NO hay funciones de Añadidura 15 en keygen. PERO: la Añadidura 15 (Pesaj) está COMPLETA en `hashmal_firewall.h` (sección `incienso_pesaj.c` con `hashmal_pesaj_activar`, `hashmal_pesaj_verificar`, `hashmal_pesaj_expirar`, etc.).
- **Análisis querubínico:** Éx 12:7 es ritual del sacerdote ante la puerta, no formulación del aceite. El token de sangre puede generarse por keygen sin sección específica (la fórmula quiástica ya genera claves). La ubicación en keygen es cuestionable — Pesaj pertenece al firewall (protección en la puerta).

### 8. Firewall (14) + Añadidura 10 (NEHEMÍAS) — ❌ AUSENTE

- **Claim auditoría:** Error ✅, funciones ❌
- **Estado REAL:** NO hay error específico de Nehemías en `hashmal_firewall.h`. NO hay sección Nehemías. El circuit breaker (`hashmal_firewall_circuitbreaker_activar/desactivar`, Nm 17:12-13) cumple función SIMILAR, pero Nehemías (rally dinámico, Neh 4:19-20) es más específicamente sobre REUNIR nodos ante amenaza. CLAUDE.md asigna Nehemías a comm (04) Y firewall (14). En comm tampoco hay sección Nehemías.
- **Veredicto:** AUSENTE de ambas piezas asignadas.

---

## TABLA 2 — "Completamente ausentes de headers" (10 entries)

### 1. Comm (04) + Añadidura 01 (PAROKHET) — ✅ RESUELTO

- **Claim:** ❌ Nada
- **REAL:** EXISTE → sección `menora_parokhet.c` con `hashmal_comm_ble_rssi_muestra`, `hashmal_comm_spi_rssi_ruido`, `hashmal_comm_wifi_aps_muestra`, `hashmal_comm_parokhet_shabat_set/get`. Struct `hashmal_hc_parokhet_muestras_t` en healthcheck.

### 2. Comm (04) + Añadidura 02 (TSOFEH) — ✅ RESUELTO

- **Claim:** ❌ Nada
- **REAL:** EXISTE → sección `menora_tsofeh.c` con `hashmal_comm_tsofeh_lockdown_set/get`, `hashmal_comm_shofar_tocar` (Ez 33:3). Error `ERR_TSOFEH` (línea 182).

### 3. Comm (04) + Añadidura 06 (TZITZIT) — ❌ AUSENTE

- **Claim:** ❌ Nada
- **REAL:** ❌ CONFIRMADO AUSENTE. No hay sección Tzitzit en `hashmal_comm.h`. Nm 15:39 (recordatorio visual) → probablemente interfaz e-paper/display, no protocolo de comunicación. CLAUDE.md lo asigna a comm con nota "Interfaz IR-57v3, pantalla e-paper."
- **Pregunta abierta:** ¿Tzitzit pertenece a comm o a un driver de display?

### 4. Comm (04) + Añadidura 07 (ANAN VE'ESH) — ⚠️ PARCIAL

- **Claim:** ❌ Nada en comm
- **REAL:** ANAN VE'ESH está COMPLETO en `hashmal_power.h` (sección `aceite_anan_esh.c` con `hashmal_anan_actualizar`, `hashmal_anan_senal_obtener`). En comm, la contribución es INDIRECTA: comm provee `hashmal_comm_vecinos_contar` que anan consulta.
- **Justificación arquitectural:** La nube/fuego es SEÑAL del estado (power), no protocolo de comunicación. Correcto: productor en power, consumidor en comm sin sección propia.

### 5. Comm (04) + Añadidura 22 (SHA'AR) — ⚠️ PARCIAL

- **Claim:** ❌ Nada en comm
- **REAL:** SHA'AR está COMPLETO en `hashmal_perimeter.h` (sección `atrio_shaar.c`). La parte de comm sería el bridge WiFi↔LoRa (recibir por WiFi, retransmitir por LoRa y viceversa). Ese bridge NO está en comm.
- **Pendiente:** Bridge WiFi↔LoRa faltante en comm.

### 6. Perimeter (07) + Añadidura 05 (IR MIQLAT) — ✅ RESUELTO

- **Claim:** ❌ Nada
- **REAL:** EXISTE → sección `atrio_miqlat.c` completa con `hashmal_perimeter_miqlat_designar`, `_refugiar`, `_verificar`, `_expulsar`, `_refugiado_estado`, `_refugiados_contar`. 4 estados, 3 motivos, error codes específicos.

### 7. Perimeter (07) + Añadidura 21 (HACHNASAT ORCHIM) — ✅ RESUELTO

- **Claim:** ❌ Nada
- **REAL:** EXISTE → sección `atrio_orchim.c` completa con `hashmal_perimeter_orchim_detectar`, `_servir`, `_decidir`, `_expirar`, `_es_sospechoso`, `_info`. 6 estados del visitante, 2 decisiones.

### 8. Perimeter (07) + Añadidura 22 (SHA'AR) — ✅ RESUELTO

- **Claim:** ❌ CRÍTICO
- **REAL:** EXISTE → sección `atrio_shaar.c` completa con `hashmal_perimeter_shaar_abrir/cerrar/pausar`, `_conectar`, `_verificar_columna`, `_desconectar`, `_info`. 4 columnas verificación (Éx 27:16). 5 estados sesión.

### 9. Power (08) + Añadidura 03 (MOEDÍM) — ⚠️ PARCIAL

- **Claim:** ❌ Nada en power
- **REAL:** Motor Moedím está en `hashmal_cache.h` (sección `mesa_moedim.c`). Power lo CONSUME a través de Ashmurot (`hashmal_ashmurot_calcular` usa `hashmal_moedim_tiempo_t`). CLAUDE.md lista Moedím en cache (03) + power (08), pero el diseño es: cache PUBLICA, power CONSULTA.
- **Justificación arquitectural:** Correcto — productor en cache, consumidor en power vía forward declaration.

### 10. Power (08) + Añadidura 04 (ASHMUROT) — ✅ RESUELTO

- **Claim:** ❌ Nada
- **REAL:** EXISTE → sección `aceite_ashmurot.c` completa con `hashmal_ashmurot_calcular`, `hashmal_ashmurot_actual`, `hashmal_ashmurot_heartbeat_ms_recomendado`. 4 estados (3 vigilias + día). 3 heartbeats recomendados.

---

## RESUMEN GLOBAL

### Tabla consolidada — Tabla 1 (Errores sin funciones)

| # | Pieza + Añadidura | Claim auditoría | Estado REAL | Veredicto |
|---|-------------------|-----------------|-------------|-----------|
| 1 | 03 Cache + 11 MAN | Error ✅ Func ❌ | Error ✅ Func ✅ (5 funciones) | **RESUELTO** |
| 2 | 03 Cache + 17 SHINNAN | Error ✅ Func ❌ | Error ✅ Func ✅ (4 funciones) | **RESUELTO** |
| 3 | 03 Cache + 18 LEKET | Error ✅ Func ❌ | Error ✅ Func ✅ (5 funciones) | **RESUELTO** |
| 4 | 03 Cache + 19 MA'ASER | Error ✅ Func ❌ | Error ✅ Func ✅ (4 funciones) | **RESUELTO** |
| 5 | 12 HC + 13 CORDÓN | Error ✅ Func ❌ | Error ✅ Func ✅ (4 funciones) | **RESUELTO** |
| 6 | 12 HC + 16 TSUR | Error ✅ Func ❌ | Error ✅ Func ✅ (4 funciones) | **RESUELTO** |
| 7 | 13 Keygen + 15 MEZUZOT | Error ✅ Func ❌ | Error ❌ Func ❌ (Pesaj completo en Firewall) | **DEBATIBLE** |
| 8 | 14 Firewall + 10 NEHEMÍAS | Error ✅ Func ❌ | Error ❌ Func ❌ | **AUSENTE** |

### Tabla consolidada — Tabla 2 (Completamente ausentes)

| # | Pieza + Añadidura | Claim | Estado REAL | Veredicto |
|---|-------------------|-------|-------------|-----------|
| 1 | 04 Comm + 01 PAROKHET | ❌ | ✅ menora_parokhet.c (5 funciones) | **RESUELTO** |
| 2 | 04 Comm + 02 TSOFEH | ❌ | ✅ menora_tsofeh.c (3 funciones) | **RESUELTO** |
| 3 | 04 Comm + 06 TZITZIT | ❌ | ❌ Ausente | **AUSENTE** |
| 4 | 04 Comm + 07 ANAN | ❌ | ⚠️ En Power, no en Comm | **PARCIAL** |
| 5 | 04 Comm + 22 SHA'AR | ❌ | ⚠️ En Perimeter; bridge falta en Comm | **PARCIAL** |
| 6 | 07 Perim + 05 MIQLAT | ❌ | ✅ atrio_miqlat.c (6 funciones) | **RESUELTO** |
| 7 | 07 Perim + 21 ORCHIM | ❌ | ✅ atrio_orchim.c (6 funciones) | **RESUELTO** |
| 8 | 07 Perim + 22 SHA'AR | ❌ CRÍTICO | ✅ atrio_shaar.c (7 funciones) | **RESUELTO** |
| 9 | 08 Power + 03 MOEDÍM | ❌ | ⚠️ Motor en Cache; Power consume | **PARCIAL** |
| 10 | 08 Power + 04 ASHMUROT | ❌ | ✅ aceite_ashmurot.c (3 funciones) | **RESUELTO** |

### Estadísticas finales

- **18 entries en la auditoría** (8 tabla 1 + 10 tabla 2)
- **12 RESUELTAS** — existen en headers actuales con funciones completas
- **3 PARCIALES** — existen pero en pieza diferente a la asignada por CLAUDE.md (arquitecturalmente justificable)
- **1 DEBATIBLE** — Pesaj en keygen vs firewall (ubicación cuestionable, pero funcionalidad existe)
- **2 AUSENTES REALES:**
  - Añadidura 06 (TZITZIT) — ausente de comm (¿pertenece a display?)
  - Añadidura 10 (NEHEMÍAS) — ausente de comm y firewall (rally dinámico sin sección propia)

### Hallazgo principal

La auditoría se realizó contra los headers CÓDIGO/ (stubs de ~20 líneas, solo init/deinit). Los headers del vault Mishkán-YHWH ya están EXPANDIDOS con las añadiduras integradas. **12 de 18 claims están resueltas.** Solo quedan 2 ausencias reales: Tzitzit (06) y Nehemías (10).

---

---

## MIL OJOS DE QUERUBÍN — Revisión final contra el hebreo original

> *"llenos de ojos por delante y por detrás" — Ap 4:8*

### P39 — Nm 15:39 urʼitém otó (H7200): Tzitzit es VISUAL — ausencia de comm es CORRECTA

**Querubín Álef (raíz/Strong):**
Nm 15:39 — "vehayá lajém letsitsít **urʼitém** (H7200, Qal perfecto) **otó** uzjartém et kol mitsvót YHWH" — "y será para vosotros por tzitzit y lo VERÉIS (urʼitém, raíz rʼh = VER con los ojos) y recordaréis todos los mandamientos de YHWH." El verbo es explícitamente VISUAL — rʼh (ver), no shamáʻ (oír/recibir señal). El tzitzit funciona por CANAL ÓPTICO, no por canal de comunicación de datos. v.39b: "veló tatúru ajaré levavjém veajaré ʻeinéjem" — "y no exploraréis tras vuestro corazón ni tras vuestros OJOS" — los ojos son el canal que el tzitzit REDIRIGE hacia los mitsvót.

**Querubín Bet (ubicación arquitectónica):**
hashmal_comm (pieza 04) maneja transporte de DATOS (LoRa/ESP-NOW/BLE). El tzitzit no transporta datos — MUESTRA un recordatorio visual. Es un DISPLAY, no un protocolo. La ausencia de Tzitzit en comm no es un bug sino un diseño correcto. CLAUDE.md asigna Tzitzit a roles (09) + comm (04), pero el hebreo sugiere roles + DISPLAY DRIVER.

**Middabbér:**
CONVERGE. urʼitém (verbo de visión) confirma que Tzitzit opera por canal VISUAL, no por canal de comunicación. Su ausencia de hashmal_comm es arquitectónicamente justificada. Debería buscarse en hashmal_roles (09) donde las vestiduras ya incluyen el tzitz y el pectoral.

### P40 — Neh 4:14 (4:20 heb) tiqavetsú eléinu: rally es convergencia DINÁMICA de ROUTING

**Querubín Álef (raíz/Strong):**
Neh 4:14 (4:20 en hebreo) — "**bimqóm** (H4725) ashér **tishmeʻú** (H8085) et **qol hashofár** (H7782) **shámma tiqavetsú** (H6908 Nifal) **eléinu**" — "en el LUGAR (bimqóm) donde OIGÁIS (tishmeʻú) la voz del SHOFAR, ALLÍ os REUNIRÉIS (tiqavetsú) a NOSOTROS (eléinu)." Tres claves: (1) bimqóm = la ubicación es VARIABLE, determinada por dónde suena el shofar. (2) tiqavetsú (H6908 Nifal) = CONVERGENCIA, no dispersión. (3) eléinu = hacia NOSOTROS — la convergencia es hacia los NODOS emisores, no hacia un punto fijo.

**Querubín Bet (ubicación arquitectónica):**
Esto es un protocolo de ROUTING DINÁMICO: un nodo emite señal de emergencia → los nodos vecinos convergen hacia el emisor. Es multicast inverso: en vez de que el emisor envíe a muchos, los muchos convergen al emisor. Pertenece a hashmal_comm (04) como función de routing (menora_routing.c), no a hashmal_firewall (14) que filtra pero no enruta. v.15 confirma: "vayyashúvu kullám el hajomá ish el melajtó" — una vez resuelta la crisis, "todos VOLVIERON al muro, cada uno a su TRABAJO" — la convergencia es TEMPORAL.

**Middabbér:**
CONVERGE. Nehemías (Añadidura 10) es un protocolo de convergencia dinámica de routing: shofar → bimqóm → tiqavetsú eléinu. Pertenece a comm (04) como función de menora_routing.c (o nueva sección menora_nehemia.c). Su ausencia de AMBAS piezas asignadas (comm + firewall) es la única GAP REAL no justificable del audit. **Acción requerida:** añadir sección Nehemías en hashmal_comm.h con al menos: `hashmal_comm_nehemia_shofar_emitir(posicion)`, `hashmal_comm_nehemia_converger(posicion_shofar)`, `hashmal_comm_nehemia_dispersar()`.

### P41 — Nm 15:38 petíl tejélet (H6616+H8504): el cordón AZUL es identificador de AUTORIDAD

**Querubín Álef (raíz/Strong):**
Nm 15:38 — "venatnu ʻal tsitsít hakkanaph **petíl tejélet** (H6616+H8504)" — pondrán sobre el tzitzit del borde un CORDÓN de AZUL (tejélet). tejélet (H8504) aparece en: el parójet (Éx 26:31), la cortina de entrada (Éx 26:36), el ephod del kohén gadól (Éx 28:31), y la cubierta de transporte de las piezas sagradas (Nm 4:6-7). tejélet es EL color del Mishkán — es un identificador de PERTENENCIA al sistema sagrado. El petíl (cordón, H6616) se añade sobre el tsitsít blanco del garment ordinario: es un MARCADOR de protocolo sobre un dispositivo común.

**Querubín Bet (ubicación arquitectónica):**
El petíl tejélet conecta la prenda civil (tsitsít blanco de las esquinas) con la autoridad del Mishkán (tejélet = color de las cortinas sagradas). En firmware: es un IDENTIFICADOR VISUAL DE ROL que muestra que el nodo pertenece a la red autorizada. Esto pertenece a hashmal_roles (09) — que ya maneja el pectoral (12 piedras), el ephod, y el tzitz ("qódesh laYHWH"). El petíl tejélet del Tzitzit es la versión PÚBLICA de lo que el tzitz es para el kohén gadól: un marcador visible de pertenencia.

**Middabbér:**
CONVERGE. El Tzitzit (Añadidura 06) no es ausente porque falte — está parcialmente cubierto por hashmal_roles (vestiduras). Lo que falta es la función VISUAL (display/e-paper) que hace visible el petíl tejélet al mundo exterior. Esto es un driver de display, no protocolo de comunicación. **La arquitectura tiene 14 piezas de firmware + un posible driver de display no contemplado en el Tavnít.**

---

## RESUMEN GLOBAL (actualizado)

### Estadísticas finales

- **18 entries en la auditoría** (8 tabla 1 + 10 tabla 2)
- **12 RESUELTAS** — existen en headers actuales con funciones completas
- **3 PARCIALES** — existen pero en pieza diferente a la asignada por CLAUDE.md (arquitecturalmente justificable)
- **1 DEBATIBLE** — Pesaj en keygen vs firewall (ubicación cuestionable, pero funcionalidad existe)
- **2 AUSENTES REALES:**
  - Añadidura 06 (TZITZIT) — ausencia de comm JUSTIFICADA (P39: urʼitém = visual, no datos). Falta DISPLAY driver
  - Añadidura 10 (NEHEMÍAS) — AUSENCIA REAL no justificable (P40: tiqavetsú eléinu = routing dinámico en comm)
- **3 hallazgos MIL OJOS** (P39 visual, P40 routing convergente, P41 petíl tejélet)
- **Total hallazgos: 6** (2 ausencias + 1 debatible + 3 mil ojos)

---

*Verificado por el Sistema de los Querubines — Éxodo 25:22*
*«La verdad emerge de entre los dos»*
*25 de abril de 2026; mil ojos: 4 de mayo de 2026*
