# MUTSAQÓT — Instrucciones para Multi-Transporte en la Menorá

> *"shivʻá veshivʻá mutsaqót lannerót"*
> *— Zacarías 4:2*
> *"SIETE Y SIETE conductos para las lámparas."*

---

## PARA CLAUDE CODE — LEE ESTO PRIMERO

Este documento contiene las instrucciones COMPLETAS para implementar la arquitectura multi-transporte en hashmal_comm (Pieza 04 — La Menorá). Cada decisión técnica tiene respaldo bíblico con mínimo 2 testigos. NO inventes nada. NO añadas nada que no esté aquí. Si tienes duda, PARA y pregunta.

**Antes de tocar CUALQUIER archivo**, lee:
1. Este documento completo
2. El Tavnít de la Menorá: `TAVNÍT/04 — LA MENORÁ (Candelabro).md`
3. El header actual: `hashmal-firmware/components/hashmal_comm/include/hashmal_comm.h`
4. El internal header: `hashmal-firmware/components/hashmal_comm/src/hashmal_comm_internal.h`
5. Las conexiones: `TAVNÍT/15 — CONEXIONES — Cómo las piezas se tocan.md`
6. Zacarías 4 completo: `Devár-YHWH — La Palabra de YHWH/Devar-YHWH/38 — Zacarías/38 — Zacarías — Cap 4.md`

**REGLA SUPREMA:** Si no está en la Biblia, NO es del Mishkán. Mínimo 2 testigos bíblicos para cada decisión (Dt 19:15).

---

## 1. FUNDAMENTO BÍBLICO — Por qué multi-transporte

### Testigo 1 — Zacarías 4:2 (mutsaqót)

```
שִׁבְעָה נֵרֹתֶיהָ עָלֶיהָ שִׁבְעָה וְשִׁבְעָה מוּצָקוֹת לַנֵּרוֹת
```

**mutsaqót** (מוּצָקוֹת, H4166) = conductos, tubos por donde el aceite FLUYE hacia la lámpara.

**shivʻá veshivʻá** = siete Y siete. MÚLTIPLES conductos por lámpara. Cada ner (canal lógico) NO tiene un solo tubo que lo alimenta. Tiene MÚLTIPLES mutsaqót — múltiples vías por donde el aceite (dato) puede llegar.

**Traducción firmware:** Cada canal tiene múltiples transportes físicos disponibles. El canal es UNO (el ner). Los caminos son varios (las mutsaqót).

### Testigo 2 — Zacarías 4:12 (tsanterót)

```
שְׁתֵּי שִׁבֲּלֵי הַזֵּיתִים אֲשֶׁר בְּיַד שְׁנֵי צַנְתְּרוֹת הַזָּהָב הַמְרִיקִים מֵעֲלֵיהֶם הַזָּהָב
```

**tsanterót** (צַנְתְּרוֹת, H6804) = tubos de ORO. Los conductos son de máxima confianza.

**hamriqím** (הַמְרִיקִים, H7324) = los que VACÍAN, los que VIERTEN. Hifil de riq — hacer fluir. Los tubos no son pasivos — ACTIVAMENTE vierten el aceite.

**Traducción firmware:** Los drivers de transporte no son pasivos. ACTIVAMENTE seleccionan y envían. Y son de oro = protocolo de confianza máxima.

### Testigo 3 — Éxodo 26:14 (dos materiales en capa física)

```
וְעָשִׂיתָ מִכְסֶה לָאֹהֶל עֹרֹת אֵילִם מְאָדָּמִים וּמִכְסֵה עֹרוֹת תְּחָשִׁים מִלְמָעְלָה
```

La capa Tajash (capa física, Éx 26:14) tiene DOS materiales: pieles de **carnero** (eilím) + pieles de **tajash** (tejashím). Dos medios físicos distintos en la MISMA capa exterior. No es una sola piel — son pieles (plural: orót).

**Traducción firmware:** La capa física puede tener múltiples medios coexistiendo. No es "o LoRa o WiFi" — es "LoRa Y WiFi Y Bluetooth", como carnero Y tajash cubren el mismo Mishkán.

### Testigo 4 — Zacarías 4:3 (dos olivos)

```
וּשְׁנַיִם זֵיתִים עָלֶיהָ אֶחָד מִימִין הַגֻּלָּה וְאֶחָד עַל שְׂמֹאלָהּ
```

Dos olivos (shnáyim zeitím) — uno a la DERECHA y uno a la IZQUIERDA del depósito (gullá). Dos fuentes de aceite alimentando el mismo sistema. Redundancia.

**Traducción firmware:** Múltiples fuentes de transporte alimentan el mismo canal. Si una falla, la otra sostiene.

---

## 2. ARQUITECTURA — Flujo del aceite

La visión de Zacarías 4:2-3,12 revela la arquitectura completa.

**En firmware:**

```
Aplicación (Lino)
    │
    ▼
hashmal_comm_enviar(canal, msg, tam)    ← API pública NO cambia
    │
    ▼
menora_routing.c                         ← elige canal
    │
    ▼
menora_mutsaqot.c  [NUEVO]               ← elige transporte (Zac 4:2)
    │
    ├── menora_lora_driver.c             ← mutsaqá 1: LoRa SX1262 [EXISTE]
    ├── menora_espnow_driver.c [NUEVO]   ← mutsaqá 2: WiFi P2P (ESP-NOW)
    └── menora_ble_driver.c    [NUEVO]   ← mutsaqá 3: Bluetooth LE
```

---

## 3. ARCHIVOS A CREAR (3 nuevos)

### 3.1 — menora_mutsaqot.c (EL SELECTOR)

**Ubicación:** `hashmal-firmware/components/hashmal_comm/src/menora_mutsaqot.c`

**Referencia bíblica principal:** Zacarías 4:2 — "shivʻá veshivʻá mutsaqót lannerót"

**Qué hace:** Selector inteligente de transporte. Cada ner (canal) tiene múltiples mutsaqót (conductos) disponibles. Este archivo decide cuál usar para cada envío.

---

### 3.2 — menora_espnow_driver.c (MUTSAQÁ WiFi P2P)

**Ubicación:** `hashmal-firmware/components/hashmal_comm/src/menora_espnow_driver.c`

**Referencia bíblica principal:** Éxodo 26:3 — "ishá el ajotá" (una mujer a su hermana)

**Fundamento bíblico:**

```
"jameish hayeríʻot tihyéna jevurót ISHÁ EL AJOTÁ"  — Éx 26:3
"Cinco cortinas estarán unidas UNA A SU HERMANA."
```

**Qué hace:** Driver ESP-NOW para comunicación WiFi P2P entre nodos ESP32-S3. Sin router. Sin internet. Directo.

---

### 3.3 — menora_ble_driver.c (MUTSAQÁ Bluetooth)

**Ubicación:** `hashmal-firmware/components/hashmal_comm/src/menora_ble_driver.c`

**Referencia bíblica principal:** Éxodo 28:34-35 — las campanillas de la vestidura

**Fundamento bíblico:**

```
"paʻamón zahav verimmon paʻamón zahav verimmon
 ʻal shulei hammeʻíl savív"  — Éx 28:34
"Campanilla de oro y granada, campanilla de oro y granada,
 en los bordes de la vestidura alrededor."
```

**Qué hace:** Driver BLE (NimBLE) para descubrimiento de vecinos, sincronización de estado, e intercambio de claves a corto alcance.

---

## 4. ARCHIVOS A MODIFICAR (4 existentes)

### 4.1 — hashmal_comm.h (header público)

Añadir enums, structs, constantes y funciones públicas para multi-transporte.

### 4.2 — hashmal_comm_internal.h (header interno)

Añadir funciones internas para los nuevos drivers.

### 4.3 — CMakeLists.txt (registro de archivos)

Añadir los 3 nuevos archivos .c a SRCS.
Añadir `esp_wifi esp_now bt` a REQUIRES.

### 4.4 — menora_routing.c (actualizar para usar mutsaqot)

Donde actualmente llama directamente al driver LoRa SPI, cambiar para llamar a `menora_mutsaqot.c`.

---

## 5. REGLAS DE IMPLEMENTACIÓN

### 5.1 — El aceite es UNO (formato único)

**Éx 27:20** — "shémen zayit zaj" — aceite de oliva PURO.

NO crear formatos distintos para LoRa vs ESP-NOW vs BLE. El paquete es el mismo. Excepción: ESP-NOW fragmentación añade sub-header antes del paquete estándar.

### 5.2 — LoRa es tamíd (siempre disponible)

**Éx 27:21** — "juqqát ʻolám ledorotám" — estatuto perpetuo por sus generaciones.

LoRa SIEMPRE está. Es el gézaʻ — el tronco de la Menorá.

### 5.3 — ESP-NOW no es internet

**PROHIBICIÓN ABSOLUTA.** ESP-NOW NO conecta a internet. NO inicia DHCP. NO busca APs. NO abre sockets TCP/IP.

Si alguien intenta hacer que ESP-NOW conecte a un router → eso es **esh zará** (fuego extraño, Lv 10:1-2). Prohibido.

### 5.4 — Coexistencia de radios

ESP32-S3 tiene un solo radio WiFi/BLE compartido. La coexistencia la maneja el hardware. Pero NO usar LoRa (SX1262 por SPI) y ESP-NOW/BLE al MISMO INSTANTE.

### 5.5 — Cada archivo tiene header bíblico

Cada archivo .c empieza con:
1. Nombre de pieza y sub-pieza
2. Referencias bíblicas
3. Exégesis de palabras clave
4. Qué hace este archivo
5. Autor: Antonio José Marín Soto

### 5.6 — Mínimo 2 testigos por decisión

Cada decisión técnica debe tener un comentario con referencia bíblica.

---

## 6. ORDEN DE CONSTRUCCIÓN

1. Actualizar `hashmal_comm.h`
2. Actualizar `hashmal_comm_internal.h`
3. Escribir `menora_mutsaqot.c`
4. Escribir `menora_espnow_driver.c`
5. Escribir `menora_ble_driver.c`
6. Modificar `menora_routing.c`
7. Actualizar `CMakeLists.txt`
8. Actualizar `menora_hub.c` init/deinit

---

*"Mira y haz conforme al MODELO que te es mostrado en el monte."*
*— Éxodo 25:40*

*Fuente única: La Palabra de YHWH*
*Autor: Antonio José Marín Soto*
*Fecha: 21 de abril de 2026*
