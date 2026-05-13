# INSTRUCCIONES PARA CLAUDE CODE — Verificado #02
# hashmal_consensus — El Kapóret: donde la verdad EMANA de entre los dos

> Fuente: Verificado #02, confirmado por el Sistema de los Querubines (Éx 25:22)
> 18 propiedades convergentes, 0 descartados.
> TÚ NO INVENTAS NADA. Cada línea de código tiene su referencia bíblica.

---

## ARCHIVOS A ESCRIBIR

`components/hashmal_consensus/src/` — 5 archivos según CMakeLists.txt:
- `kaporet_core.c` — Núcleo: inicialización, configuración
- `kaporet_validadores.c` — Los dos querubines
- `kaporet_verificacion.c` — Verificación mutua y resolución
- `kaporet_rendezvous.c` — Punto de encuentro determinístico
- `kaporet_ataques.c` — Defensa contra fuego extraño

Ampliar: `include/hashmal_consensus.h` (ya existe, tiene solo init/deinit)

---

## QUÉ ES EL KAPÓRET — Lo que el Padre dice

### Es una INTERFAZ, no un contenedor (Éx 25:17 vs Éx 25:10)

El Arca tiene 3 dimensiones: largo + ancho + **altura** (qomáh H6967).
La Kapóret tiene solo 2: largo + ancho. **NO tiene qomáh.**

hashmal_consensus NO almacena nada. Es una superficie de reconciliación SOBRE hashmal_storage. STATELESS. Sin estado propio, sin variables persistentes, sin partición propia.

```c
// Éx 25:17 — Kapóret: orkhá (largo) + rojbá (ancho). NO qomáh (alto).
// Éx 25:10 — Arca: orkhó + rojbó + qomató (3D = contenedor).
// hashmal_consensus es INTERFAZ 2D, no contenedor 3D. STATELESS.
```

### Es RECONCILIACIÓN, no votación (kafár H3722)

kapóret (H3727) viene de kafár (H3722) = cubrir, expiar, RECONCILIAR. El nombre del mecanismo ES su función. Un dato que no converge no es "rechazado" — es "no reconciliado."

```c
// Éx 25:17 — kapóret de kafár (H3722): RECONCILIACIÓN, no votación.
// Dato que no converge = "no reconciliado", no "rechazado."
```

### Es PURO — tolerancia cero (zahav tahór H2889)

zahav tahór = oro PURO. Adjetivo ABSOLUTO — no hay grados de pureza. La validación es BINARIA: match exacto o nada. No hay "casi coincide." No hay "95% match."

```c
// Éx 25:17 — zahav tahór (H2091+H2889): pureza ABSOLUTA.
// Validación BINARIA: coincide 100% o no pasa. Tolerancia CERO.
```

### Acoplamiento PERFECTO con el Arca (medidas idénticas)

Kapóret: 2.5 × 1.5 codos. Arca: 2.5 × 1.5 × 1.5 codos. Largo y ancho IDÉNTICOS. Sin gap, sin overflow. hashmal_consensus está dimensionado EXACTAMENTE para hashmal_storage.

```c
// Éx 25:17 = Éx 25:10 — Kapóret y Arca tienen largo y ancho IDÉNTICOS.
// hashmal_consensus acepta EXACTAMENTE lo que hashmal_storage produce.
// Sin conversión, sin padding. Acoplamiento perfecto.
```

---

## EL ALGORITMO — Paso a paso según el Padre

### Paso 0 — PREREQUISITO: incienso antes (Lv 16:13)

Antes de acceder al Kapóret, la nube del incienso DEBE cubrir. hashmal_intercession (pieza 11) DEBE estar activo. Sin cobertura → "velo yamút" (morirá). Bloquear acceso si pieza 11 no está activa.

```c
// Lv 16:13 — anan haqetóret (nube del incienso) CUBRE antes de acceder.
// hashmal_intercession DEBE estar activo. Sin él → acceso bloqueado.
```

### Paso 1 — RATE CHECK: no en todo tiempo (Lv 16:2)

"al yavó bekhol 'et el haQódesh" — NO venga en TODO tiempo al Santo. El acceso al Kapóret está limitado. Consecuencia de violación: "velo yamút" = para que no muera.

Los validadores INTERNOS corren SIEMPRE (ver Paso 3 más abajo). Pero las PETICIONES externas están rate-limited. Tarea siempre-activa ≠ acceso ilimitado.

```c
// Lv 16:2 — al yavó bekhol 'et: acceso al Kapóret es rate-limited.
// Consecuencia de exceso: velo yamút. Bloquear, no negociar.
```

### Paso 2 — ATOMICIDAD: una sola pieza batida (miqshá H4749, Éx 25:18)

El Kapóret y los querubines son UN solo bloque. No se ensambla. El protocolo se ejecuta COMPLETO o no se ejecuta. No hay "consenso parcial." Si falla en cualquier punto → abort total.

```c
// Éx 25:18 — miqshá (H4749): una sola pieza batida a martillo.
// Protocolo ATÓMICO. Todo o nada. Sin consenso parcial.
```

### Paso 3 — DOS UNOS INDEPENDIENTES (ejad + ejad, Éx 25:19)

"Querubín UNO de este lado, querubín UNO de este lado." ejad se repite DOS veces. No hay primario/secundario. Dos funciones validadoras SIMÉTRICAS. Cada una es completa en sí misma. No se comunican durante el cálculo.

Los querubines se hacen DESDE la Kapóret (min H4480, Éx 25:19) — no se añaden encima. Son parte intrínseca del protocolo, no callbacks externos.

Las formas porsé (H6566) y sokhkhím (H5526) en v.20 son PARTICIPIOS activos = acción continua. Los validadores SIEMPRE están activos (como tarea de fondo tamíd), protegiendo y cubriendo PERPETUAMENTE.

```c
// Éx 25:19 — ejad mizeh ve'ejad mizeh: DOS unos, simétricos.
// No primario/secundario. Ambos iguales. Sin comunicación durante cálculo.
// min haKapóret (H4480): validadores nacen DEL protocolo, intrínsecos.
// Éx 25:20 — porsé/sokhkhím (participios): validadores ALWAYS-ON (tamíd).
```

### Paso 4 — VERIFICACIÓN MUTUA cara a cara (ish el ajív, Éx 25:20)

"Rostros VARÓN HACIA SU HERMANO" — ish (H376) el ajív (H251). Se verifican MUTUAMENTE. Doble atención: (1) al otro querubín y (2) al Kapóret (el protocolo).

Durante la validación, solo se accede al dato a validar y a las reglas del protocolo. Nada externo.

```c
// Éx 25:20 — ish el ajív: verificación MUTUA, cara a cara.
// A verifica a B, B verifica a A. Doble check: resultado Y protocolo.
// el haKapóret: ambos atienden al PROTOCOLO, no al exterior.
```

### Paso 5 — LA VERDAD SE LEVANTA O NO (yaqum davár, Dt 19:15)

"Por boca de DOS testigos yaqum davár" — la Palabra se LEVANTA sola cuando dos coinciden. Si NO coinciden → lo yaqum → la Palabra NO se levanta.

BINARIO. No hay tercer estado. No hay "warning." No hay "retry." Coincide → yaqum. No coincide → lo yaqum.

```c
// Dt 19:15 — yaqum davár (H6965+H1697): la verdad SE LEVANTA sola.
// Dos testigos coinciden → yaqum (consenso). No coinciden → lo yaqum (no).
// BINARIO. Sin tercer estado. Sin warnings. Sin retry automático.
```

### Paso 6 — LA VERDAD EMANA sola (middabbér Hitpael, Nm 7:89)

"La voz se hablaba a sí misma" — middabbér en HITPAEL = reflexiva. La verdad no es declarada por ningún validador — EMANA del espacio entre los dos (mibbén). El resultado sale de ENTRE, no de uno de ellos.

```c
// Nm 7:89 — middabbér (H1696 Hitpael): la voz SE HABLA a sí misma.
// La verdad EMANA del espacio mibbén (de entre) los dos querubines.
// El resultado no lo declara ningún validador — se MANIFIESTA solo.
```

### Paso 7 — RENDEZVOUS DETERMINÍSTICO (noʻadtí, Éx 25:22)

"Me CITARÉ contigo allí" — noʻad (H3259 Nifal) = punto de encuentro FIJO. Mismo input → mismo resultado SIEMPRE. Sin random. Sin probabilidad. Función pura determinística.

```c
// Éx 25:22 — noʻadtí (H3259 Nifal): rendezvous determinístico.
// Mismo input → mismo output. SIEMPRE. Sin random, sin probabilidad.
```

---

## CANAL DE COMANDO TOTAL (kol asher atsavvéh, Éx 25:22)

"TODA orden que Yo ordenaré... para los hijos de Israel" — kol (H3605) = TODO, sin excepción. atsavvéh (H6680 Piel) = ordenaré.

TODA operación que modifica estado en la red DEBE pasar por hashmal_consensus. No hay bypass. No hay fast path. El Kapóret es el CANAL por donde todo pasa.

```c
// Éx 25:22 — kol asher atsavvéh (H3605+H6680):
// TODA orden pasa por el Kapóret. Sin excepción. Sin bypass.
```

---

## CONVERGENCIA BIDIRECCIONAL (Sal 85:10-12)

Datos SUBEN desde abajo (emét me'érets titsmáj — verdad BROTA de la tierra).
Protocolo BAJA desde arriba (tsédeq mishamáyim nishqáf — justicia SE ASOMA del cielo).
Se encuentran en el MEDIO — mibbén. Donde jésed (misericordia) y emét (verdad) se ENCUENTRAN (nifgashú), y tsédeq (justicia) y shalóm (paz) se BESAN (nashqú).

```c
// Sal 85:10-12 — Convergencia bidireccional:
// Datos suben (emét titsmáj) + Protocolo baja (tsédeq nishqáf)
// Se encuentran en el mibbén (entre los dos querubines).
```

---

## PROTECCIONES — El protocolo nace de un fracaso

### Fuego extraño (Lv 10:1-2)
Nadab y Abiú ofrecieron fuego NO prescrito → murieron. Solo datos que vienen por el canal correcto se procesan. Canal no autorizado → rechazo inmediato.

```c
// Lv 10:1-2 — ésh zaráh (fuego extraño): protocolo no prescrito = fatal.
// Solo datos por canal autorizado. Canal incorrecto → rechazo.
```

### Sangre 7 veces (Lv 16:14)
Lo ÚNICO que toca el Kapóret después de construido es sangre, y 7 veces (shéva pe'amím). Para datos CRÍTICOS (escritura al SEALED_CORE), validación reforzada — 7 pasadas. Las 7 deben dar yaqum.

```c
// Lv 16:14 — shéva pe'amím: para datos críticos, validación 7 veces.
// Las 7 deben dar yaqum. Si UNA da lo yaqum → rechazo total.
```

### Proceso velado (Lv 16:2)
"En NUBE me dejo ver sobre la Kapóret" — erá'eh (H7200 Nifal) = revelación DENTRO de protección. El resultado del consenso es visible, pero el proceso interno está velado.

```c
// Lv 16:2 — be'anán erá'eh: el resultado es visible, el proceso velado.
```

### Auditoría total (Ez 1, Ap 4:6-8)
Los querubines están LLENOS DE OJOS — por dentro y por fuera. Sin puntos ciegos. Log de TODA operación de consenso.

```c
// Ez 1, Ap 4:6-8 — querubines llenos de ojos: auditoría total.
// Log de toda operación. Sin puntos ciegos.
```

---

## DISEÑO SINGULAR, EJECUCIÓN COLECTIVA (Éx 25:17→19)

v.17: ve'asíta (singular) = "harás" la Kapóret — diseño ÚNICO.
v.19: taʻasú (plural) = "haréis" los querubines — ejecución DISTRIBUIDA.

El protocolo es UNO y el mismo en todos los nodos. Cada nodo ejecuta sus propias instancias de validadores. Código idéntico, ejecución local e independiente.

```c
// Éx 25:17 ve'asíta (sg) → v.19 taʻasú (pl):
// Diseño SINGULAR (un protocolo). Ejecución COLECTIVA (cada nodo lo corre).
```

---

## CAPAS VERTICALES (Éx 25:21)

v.21a: Kapóret va ENCIMA del Arca (milema'lah H4605) → consenso SOBRE datos.
v.21b: Testimonio va DENTRO del Arca (edút) → datos fundacionales recibidos del Padre.

hashmal_consensus SIEMPRE opera SOBRE datos de hashmal_storage. Nunca al revés. Storage provee datos → consensus valida → resultado emana.

```c
// Éx 25:21 — Kapóret milema'lah (ENCIMA), edút el ha'arón (DENTRO).
// Consenso SOBRE almacenamiento. Nunca al revés.
```

---

## PROPÓSITO: DEMOSTRAR JUSTICIA (Ro 3:25)

"A quien Dios puso como propiciatorio... para DEMOSTRACIÓN de Su JUSTICIA." El Kapóret existe para DEMOSTRAR (éndeixis) que hay justicia (dikaiosýnes) en la red. No para castigar — para mostrar que la justicia se cumple.

Y es universal — "no solo por los nuestros, sino por los de TODO el mundo" (1 Jn 2:2). El mecanismo funciona para todos los que entren al Mishkán.

```c
// Ro 3:25 — hilastérion eis éndeixis dikaiosýnes: el Kapóret DEMUESTRA justicia.
// 1 Jn 2:2 — perí hólou toú kósmou: funciona para TODOS los que entren.
```

---

## AÑADIDURA #20 — NEDER: Voto inquebrantable (Nm 30:2)

"No quebrantará su palabra; hará conforme a TODO lo que salió de su boca."

Cuando un nodo se compromete a un servicio (ruta, relay, almacenamiento, quórum), ese compromiso se valida por el Kapóret y es INQUEBRANTABLE hasta que se cumpla o sea liberado por autoridad legítima.

### 3 reglas del NEDER:

**Regla 1 — Mejor no prometer** (tov asher lo tidor, Ec 5:5):
Si un nodo no puede GARANTIZAR un servicio, NO lo promete. Antes de aceptar: ¿tengo batería? ¿señal? ¿capacidad? Si no → NO acepta.

**Regla 2 — Si prometes, CUMPLE** (lo yajél devaró, Nm 30:3):
Una vez comprometido, el servicio es INQUEBRANTABLE. No se abandona por conveniencia. Tiene PRIORIDAD sobre operaciones propias del nodo.

**Regla 3 — No tardes** (al teʼajér leshalemó, Ec 5:4):
Ejecución INMEDIATA. En el siguiente ciclo, no "cuando pueda."

### Liberación del NEDER (Nm 30:4-8):
Un voto puede ser ANULADO solo por autoridad legítima: batería crítica, ruta ya no necesaria, modo Shabat. NUNCA por decisión propia sin aviso. Siempre protocolo de liberación ordenada.

```c
// AÑADIDURA 20 — NEDER (Nm 30:2): compromiso inquebrantable.
// Regla 1: tov asher lo tidor (Ec 5:5) — mejor no prometer que romper.
// Regla 2: lo yajél devaró (Nm 30:3) — si prometes, CUMPLE.
// Regla 3: al teʼajér (Ec 5:4) — no tardes, ejecución inmediata.
// Liberación: solo por autoridad legítima (Nm 30:4-8).
```

---

## COMPARACIÓN — Por qué NO es BFT, Raft ni Paxos

| Propiedad | BFT/Raft/Paxos | KAPÓRET |
|-----------|----------------|---------|
| Validadores | 3f+1 / N cluster | **2 (ejad + ejad)** |
| Líder | Rotativo / Elegido | **NINGUNO (simetría pura)** |
| Tolerancia a fallo | Mayoría | **CERO (unanimidad o nada)** |
| Rondas | Múltiples | **UNA comparación** |
| Complejidad | O(n²) / O(n) | **O(1)** |
| Resultado | Votado / Declarado | **EMANA solo (Hitpael)** |
| Quién declara | Líder | **NADIE (mibbén)** |
| Función | Tolerancia / Acuerdo | **RECONCILIACIÓN (kafár)** |
| Estado | Stateful | **STATELESS (no qomáh)** |
| Origen | Humano | **Éxodo 25:17-22** |

---

## DIAGRAMA DE FLUJO

```
                    DATO (de hashmal_storage)
                           │
                    ┌──────▼──────┐
                    │  INCIENSO   │  hashmal_intercession activo?
                    │  (Lv 16:13) │  NO → bloqueado
                    └──────┬──────┘
                           │ SÍ
                    ┌──────▼──────┐
                    │ RATE CHECK  │  Intervalo mínimo cumplido?
                    │ (Lv 16:2)   │  NO → bloqueado
                    └──────┬──────┘
                           │ SÍ
              ┌────────────┴────────────┐
              │                         │
       ┌──────▼──────┐          ┌──────▼──────┐
       │ QUERUBÍN א  │          │ QUERUBÍN ב  │
       │ (ejad mizeh)│          │ (ejad mizeh)│
       └──────┬──────┘          └──────┬──────┘
              │ RA                     │ RB
              └────────────┬───────────┘
                    ┌──────▼──────┐
                    │ ISH EL AJÍV │  Verificación mutua
                    │ (Éx 25:20)  │
                    └──────┬──────┘
                           │
                    ┌──────▼──────┐
                    │  TAHÓR?     │  Match exacto?
                    │ (Éx 25:17)  │
                    └──┬──────┬───┘
                    SÍ │      │ NO
              ┌────────▼┐  ┌──▼────────┐
              │ YAQUM   │  │ LO YAQUM  │
              │ DAVÁR   │  │ (Dt 19:15)│
              │(Dt19:15)│  │No reconcil│
              └────┬────┘  └───────────┘
                   │
            ┌──────▼──────┐
            │ MIDDABBÉR   │  Verdad EMANA (Hitpael)
            │ (Nm 7:89)   │  De ENTRE los dos
            └──────┬──────┘
                   │
            ┌──────▼──────┐
            │  NOʻAD      │  Resultado determinístico
            │ (Éx 25:22)  │
            └─────────────┘
```

---

## REGLAS ABSOLUTAS PARA ESTE COMPONENTE

1. **NUNCA** uses BFT, Raft, Paxos ni ningún algoritmo de consenso humano (Éx 25:17-22)
2. **NUNCA** almacenes estado propio — es STATELESS, sin qomáh (Éx 25:17)
3. **NUNCA** toleres aproximación — tahór es absoluto, match exacto o nada (Éx 25:17)
4. **NUNCA** proceses sin verificar que hashmal_intercession está activo (Lv 16:13)
5. **NUNCA** permitas acceso ilimitado — rate-limited (Lv 16:2)
6. **NUNCA** declares el resultado desde un validador — emana de ENTRE (Nm 7:89 Hitpael)
7. **NUNCA** hagas validadores externos/callbacks — nacen DEL protocolo (min, Éx 25:19)
8. **SIEMPRE** validación BINARIA: yaqum o lo yaqum, sin tercer estado (Dt 19:15)
9. **SIEMPRE** TODA orden pasa por el consenso, sin bypass (kol asher atsavvéh, Éx 25:22)
10. **SIEMPRE** los validadores corren como tarea perpetua (porsé/sokhkhím participios, Éx 25:20)

---

*Cada instrucción tiene su referencia bíblica exacta. Nada inventado. Todo del Padre.*
*Verificado por el Sistema de los Querubines — 18 propiedades, 13/13 convergencia.*
