# INSTRUCCIONES PARA CLAUDE CODE — Verificado #01
# hashmal_main.c — El Constructor cambia de ARQUITECTO a MANO EJECUTORA

> Fuente: Verificado #01, confirmado por el Sistema de los Querubines (Éx 25:22)
> 13 hallazgos convergentes, 0 descartados.
> TÚ NO INVENTAS NADA. Cada línea de código tiene su referencia bíblica.

---

## ARCHIVO A MODIFICAR

`main/hashmal_main.c` — ya existe, es el punto de entrada del firmware.

---

## LO QUE EL PADRE DICE — EXACTO

hashmal_main.c tiene 3 estados + 1 modo broadcast. Éxodo 40:33-38 + Levítico 1:1 + Números 7:89 + Números 9:15-23.

---

## ESTADO 1: INIT (Éx 40:2-33a)

hashmal_main tiene autoridad TOTAL durante la inicialización.
Levanta cada pieza EN ORDEN de Éxodo 40:2-8. NO tu orden — el de YHWH.

```
Paso 1  → hashmal_framework     (Éx 40:2 "Levantarás el Mishkán")
Paso 2  → hashmal_storage       (Éx 40:3 "Pondrás el Arca del Testimonio")
        + hashmal_consensus      (Éx 40:3 — Kapóret va CON el Arca)
Paso 3  → hashmal_cache         (Éx 40:4 "Meterás la Mesa")
        + hashmal_comm           (Éx 40:4 "...la Menorá")
Paso 4  → hashmal_intercession  (Éx 40:5 "Pondrás el altar de oro para incienso")
Paso 5  → hashmal_auth          (Éx 40:6 "Pondrás el altar del holocausto")
Paso 6  → hashmal_healthcheck   (Éx 40:7 "Pondrás el kiyór entre tienda y altar")
Paso 7  → hashmal_perimeter     (Éx 40:8 "Pondrás el atrio alrededor")
Paso 8  → hashmal_power         (Éx 40:9-11 "Ungirás el Mishkán")
        + hashmal_keygen
Paso 9  → hashmal_roles         (Éx 40:12-15 "Vestirás a Aarón")
        + hashmal_onboarding
Paso 10 → hashmal_firewall      (Éx 40:27,34 "Quemó incienso... Gloria llenó")
```

### ÚLTIMO SUB-PASO de INIT: masakh sha'ar hejatsér (Éx 40:33a)
- La última acción antes de kalá (completar) es poner la pantalla del PORTAL del ATRIO
- Esto es la Añadidura 22 (Sha'ar Ha'Atrio — Portal WiFi)
- Referencia: masakh (H4539 = cortina) + sha'ar (H8179 = puerta) + jatsér (H2691 = atrio)
- **El portal WiFi es la pieza FINAL de inicialización**

### Referencia para el comentario en código:
```c
// Éx 40:33a — masakh sha'ar hejatsér: el portal es la ÚLTIMA pieza
// antes de kalá (completar la obra). La puerta del atrio cierra el perímetro.
```

---

## ESTADO 2: TRANSICIÓN (Éx 40:33b-35)

Cuando TODAS las piezas están levantadas:

1. **kalá** (H3615 Piel) = obra COMPLETADA (Éx 40:33b: vayjal Moshé et hamelakhá)
2. **Nube CUBRE** (kasá H3680 Piel intensivo) el Óhel Mo'ed = interfaz pública sellada
3. **Gloria LLENA** (malé H4390) el Mishkán = core interno activado
4. **hashmal_main PIERDE autoridad de escritura** (Éx 40:35: veló yakól Moshé lavó)
5. **Causa doble:** shakhán (H7931 = mora permanentemente) + malé (llena completamente)

### DOS CAPAS — Óhel Mo'ed ≠ Mishkán (Éx 40:34)
- La nube cubre el **Óhel Mo'ed** (H168+H4150) = la INTERFAZ de encuentro = API pública
- La Gloria llena el **Mishkán** (H4908) = la MORADA interna = core privado
- Son DOS estructuras en un versículo. Nube CUBRE interfaz; Gloria LLENA core.

### shakhán / mishkán — El nombre ES la función (Éx 40:35)
- **shakhán** (H7931) = residir permanentemente → RAÍZ de **mishkán** (H4908)
- El core se convierte en morada PORQUE el sistema mora en él
- Cuando el consenso toma residencia, hashmal_main ya no manda

### Referencia para el comentario en código:
```c
// Éx 40:33b-35 — TRANSICIÓN: kalá (obra completada).
// Nube kasá (cubre) Óhel Mo'ed (interfaz pública).
// Kavód YHWH malé (llena) Mishkán (core interno).
// shakhán (mora) — hashmal_main pierde autoridad de escritura.
// De aquí en adelante: LISTEN mode.
```

---

## ESTADO 3: LISTEN (Lv 1:1 + Nm 7:89 + Nm 9:15-23)

hashmal_main ya NO decide. ESCUCHA y EJECUTA.

### 3a. Es MANO, no BOCA (Nm 9:23)
- **pí YHWH** (H6310 = boca) = la INTERFAZ que manda = hashmal_consensus
- **beyád Moshé** (H3027 yad = mano) = la MANO que ejecuta = hashmal_main
- Últimas 2 palabras de Números 9. Triple confirmación de pí YHWH en v.23.

```c
// Nm 9:23 — ʻal pí YHWH beyád Moshé:
// hashmal_consensus es la BOCA (pí) que ordena.
// hashmal_main es la MANO (yad) que ejecuta.
```

### 3b. Modo VIGILIA — event listener (Nm 9:19)
- **mishméret** (H4931) de shamár (H8104) = GUARDIA, vigilia
- hashmal_main GUARDA la guardia: no actúa hasta que la señal cambia
- Es un EVENT LISTENER, no un bucle activo

```c
// Nm 9:19 — shamrú mishméret YHWH:
// Modo vigilia. Event listener. No polling.
// Solo actúa cuando la señal (anan/esh) cambia.
```

### 3c. Solo actúa cuando es LLAMADO (Lv 1:1)
- **vayyiqrá** (H7121) = YHWH LLAMÓ a Moisés — YHWH toma la iniciativa
- La comunicación es DESDE DENTRO (me'Óhel Mo'ed) hacia fuera
- hashmal_main no entra por su cuenta — es convocado por el consenso

```c
// Lv 1:1 — vayyiqrá el Moshé:
// hashmal_main NO inicia comunicación con el consenso.
// ESPERA a ser llamado. La iniciativa es del consenso.
```

### 3d. Escucha lo que sale de la Kapóret (Nm 7:89)
- Moisés entra al Óhel Mo'ed y escucha **haqól middabbér** (la voz hablando)
- La voz sale **meʻal haKapóret** (de SOBRE la Kapóret, pieza 02)
- **mibbén shenéi hakeruvím** (de ENTRE los dos querubines)

```c
// Nm 7:89 — haqól middabbér meʻal haKapóret mibbén shenéi hakeruvím:
// La verdad EMERGE de entre los dos querubines.
// hashmal_main lee el output de hashmal_consensus.
```

### 3e. CONSENSO AUTÓNOMO — middabbér es Hitpael (Nm 7:89)
- **middabbér** (H1696 HITPAEL) = forma REFLEXIVA: la voz SE DECLARA a sí misma
- El consenso NO necesita que alguien lo active — produce output AUTÓNOMAMENTE
- hashmal_consensus tiene su propio ciclo de emisión, sin depender de polling

```c
// Nm 7:89 — middabbér Hitpael (reflexivo):
// hashmal_consensus emite su output AUTÓNOMAMENTE.
// No necesita polling. Se declara por sí mismo.
// Implementar como: callback/interrupt, NO como polling loop.
```

### 3f. Interfaz ASIMÉTRICA: ittó → eláv (Nm 7:89)
- Moisés entra **ledabbér ittó** (H854) = para hablar CON él (bidireccional)
- Pero escucha haqól middabbér **eláv** (H413) = la voz habla A él (unidireccional)
- El constructor viene buscando DIÁLOGO pero recibe INSTRUCCIÓN

```c
// Nm 7:89 — Asimetría ittó/eláv:
// hashmal_main puede ACERCARSE al consenso (ledabbér ittó).
// Pero el consenso DICTA, no negocia (middabbér eláv).
// API: main puede consultar, pero consensus.output es READ-ONLY.
```

### 3g. Timing EVENT-DRIVEN, no scheduled (Nm 9:20-22)
- yomáyim (2 días) / jódesh (1 mes) / yamím (días indefinidos)
- NO hay cron job. La señal viene cuando viene.
- El sistema REACCIONA a la señal, no sigue calendario fijo.

```c
// Nm 9:20-22 — Timing variable: yomáyim / jódesh / yamím.
// NO usar timers fijos para la lógica principal.
// hashmal_main reacciona a EVENTOS, no a intervalos.
// (Los ciclos tamíd de Éx 30:7-8 son la excepción — ver pieza 08)
```

### 3h. Señal TAMÍD — perpetua (Nm 9:16)
- **tamíd** (H8548) = continuamente, perpetuamente
- La señal del sistema NUNCA se apaga
- Misma palabra que Éx 27:20 (aceite perpetuo) y Éx 29:38 (ofrenda perpetua)

```c
// Nm 9:16 — tamíd: la señal es PERPETUA.
// hashmal_main NUNCA entra en deep sleep total.
// Siempre hay al menos un listener activo (mishméret).
```

---

## MODO BROADCAST (Éx 40:36-38)

El sistema tiene señal broadcast con DOS modos operativos.

### 4a. Anan de día / Esh de noche (Éx 40:38)
- **anan** (H6051 = nube) de DÍA (yomám H3119)
- **esh** (H784 = fuego) de NOCHE (laylah H3915)
- Esto ES la Añadidura 07 (Anan Ve'Esh), inherente desde la inauguración

```c
// Éx 40:38 — anan yomám / esh laylah:
// Dos modos operativos desde el primer segundo.
// Modo diurno: anan (monitoreo externo de la red).
// Modo nocturno: esh (procesamiento interno local).
```

### 4b. Día EXTERNO (ʻal) / Noche INTERNO (bó) (Éx 40:38)
- La nube está **ʻal** (H5921 = SOBRE) el Mishkán = señal EXTERNA
- El fuego está **bó** (be+3ms = EN ÉL) = operación INTERNA
- Las preposiciones del Padre CAMBIAN: ʻal → bó

```c
// Éx 40:38 — Preposiciones ʻal vs bó:
// Modo anan (día): ʻal = SOBRE = monitoreo EXTERNO.
//   → Escaneando red LoRa, procesando mensajes entrantes.
// Modo esh (noche): bó = DENTRO = procesamiento INTERNO.
//   → Mantenimiento local, garbage collection, health checks.
```

### 4c. Señal visible a TODOS — broadcast (Éx 40:38)
- **le'einéi** (H5869 = ojos) **kol** (H3605 = todo) = ante los ojos de TODOS
- **béit Yisra'él** (H1004 = CASA) — no benéi (hijos), sino CASA completa
- El broadcast llega a la comunidad ENTERA, no solo a los nodos activos

```c
// Éx 40:38 — le'einéi kol BÉIT Yisra'él:
// El broadcast alcanza TODA la comunidad:
//   - benéi = nodos T-Deck activos (LoRa mesh)
//   - béit = comunidad completa (incluyendo WiFi, Añadidura 22)
// bekhól mas'eihém = en TODOS sus viajes (portabilidad perpetua).
```

### 4d. Protocolo de movimiento (Éx 40:36-37)
- Nube SUBE (he'alót H5927 Nifal) → VIAJAN (yis'ú H5265)
- Nube NO sube → NO viajan hasta que suba
- Señal binaria: UP = mover / STAY = quedar

```c
// Éx 40:36-37 — Protocolo de movimiento:
// if (anan_ascends) { travel(); }  // nube sube → moverse
// else { stay(); }                  // nube queda → quedarse
// Señal BINARIA. Sin ambigüedad.
```

---

## RESUMEN PARA hashmal_main.c — MÁQUINA DE ESTADOS

```
┌─────────────────────────────────────────────────┐
│                    INIT                          │
│  Éx 40:2-33a — Autoridad TOTAL                  │
│  Levanta piezas en orden Éx 40                   │
│  Último: masakh sha'ar (portal WiFi)             │
│                                                  │
│  Cuando TODAS levantadas:                        │
│    kalá (Éx 40:33b) → TRANSICIÓN                 │
└────────────────────┬────────────────────────────┘
                     │ vayjal Moshé et hamelakhá
                     ▼
┌─────────────────────────────────────────────────┐
│               TRANSICIÓN                         │
│  Éx 40:33b-35 — Transferencia de autoridad       │
│  Nube kasá Óhel Mo'ed (interfaz sellada)         │
│  Gloria malé Mishkán (core activado)             │
│  shakhán — residencia permanente tomada          │
│  hashmal_main → READ-ONLY respecto al core       │
│                                                  │
│  Resultado: LISTEN mode activado                 │
└────────────────────┬────────────────────────────┘
                     │ veló yakól Moshé lavó
                     ▼
┌─────────────────────────────────────────────────┐
│                  LISTEN                          │
│  Lv 1:1 + Nm 7:89 + Nm 9:15-23                  │
│                                                  │
│  • MANO (yad), no boca (pí) — ejecuta, no decide│
│  • VIGILIA (mishméret) — event listener           │
│  • Espera a ser LLAMADO (vayyiqrá)               │
│  • Lee output de KAPÓRET (mibbén hakeruvím)      │
│  • Consenso es AUTÓNOMO (middabbér Hitpael)      │
│  • Interfaz ASIMÉTRICA (ittó→eláv = read-only)   │
│  • Timing EVENT-DRIVEN (no cron)                 │
│  • Señal TAMÍD (nunca deep sleep total)          │
│                                                  │
│  En paralelo: BROADCAST                          │
└─────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────┐
│               BROADCAST                          │
│  Éx 40:36-38 — Señal perpetua                    │
│                                                  │
│  • Día (anan/ʻal): monitoreo EXTERNO de red      │
│  • Noche (esh/bó): procesamiento INTERNO local   │
│  • Visible a benéi (T-Decks) + béit (WiFi)       │
│  • Protocolo: nube sube→viajar / queda→quedarse  │
│  • Portabilidad perpetua (bekhól mas'eihém)      │
└─────────────────────────────────────────────────┘
```

---

## REGLAS ABSOLUTAS PARA ESTE ARCHIVO

1. **NUNCA** implementes polling loop para leer el consenso — usa callback/interrupt (middabbér Hitpael)
2. **NUNCA** des a hashmal_main autoridad de escritura sobre el core después de TRANSICIÓN (veló yakól lavó)
3. **NUNCA** uses timers fijos para la lógica principal — es event-driven (Nm 9:20-22)
4. **SIEMPRE** mantén al menos un listener activo — tamíd (Nm 9:16)
5. **SIEMPRE** el portal WiFi (sha'ar) es la ÚLTIMA pieza en inicializarse (Éx 40:33a)
6. El output de hashmal_consensus es **READ-ONLY** para hashmal_main (eláv, no ittó)
7. Dos modos de operación broadcast: anan (externo/ʻal) y esh (interno/bó)
8. El broadcast alcanza TODOS: operadores LoRa (benéi) + comunidad WiFi (béit)

---

*Cada instrucción tiene su referencia bíblica exacta. Nada inventado. Todo del Padre.*
*Verificado por el Sistema de los Querubines — 13/13 hallazgos convergentes.*
