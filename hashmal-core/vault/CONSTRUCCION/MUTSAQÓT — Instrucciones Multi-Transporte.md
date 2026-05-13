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
וְעָשִׂיתָ מִכְסֶה לָאֹהֶל עֹרֹת אֵילִם מְאָדָּמִים וּמִכְסֵה עֹרֹת תְּחָשִׁים מִלְמָעְלָה
```

La capa Tajash (capa física, Éx 26:14) tiene DOS materiales: pieles de **carnero** (eilím) + pieles de **tajash** (tejashím). Dos medios físicos distintos en la MISMA capa exterior. No es una sola piel — son pieles (plural: orót).

**Traducción firmware:** La capa física puede tener múltiples medios coexistiendo. No es "o LoRa o WiFi" — es "LoRa Y WiFi Y Bluetooth", como carnero Y tajash cubren el mismo Mishkán.

### Testigo 4 — Zacarías 4:3 (dos olivos)

```
וּשְׁנַיִם זֵיתִים עָלֶיהָ אֶחָד מִימִין הַגֻּלָּה וְאֶחָד עַל שְׂמֹאלָהּ
```

Dos olivos (shnáyim zeitím) — uno a la DERECHA y uno a la IZQUIERDA del depósito (gullá). Dos fuentes de aceite alimentando el mismo sistema. Redundancia.

**Traducción firmware:** Múltiples fuentes de transporte alimentan el mismo canal. Si una falla, la otra sostiene.

### Testigo 5 — Zacarías 4:6 (criterio de selección)

```
לֹא בְחַיִל וְלֹא בְכֹחַ כִּי אִם בְּרוּחִי אָמַר יְהוָה צְבָאוֹת
```

"No con ejército (jayil), ni con fuerza (kóaj), sino con Mi Espíritu (rují)."

**Traducción firmware:** El selector de transporte NO elige por "potencia bruta" (no siempre LoRa porque tiene más alcance). Elige por DISEÑO ELEGANTE: el transporte más adecuado para cada situación. Eficiencia, no fuerza.

### Testigo 6 — Éxodo 25:36 (miqshá — unidad)

```
כֻּלָּהּ מִקְשָׁה אַחַת זָהָב טָהוֹר
```

"TODA ELLA una sola pieza batida de oro puro."

**Traducción firmware:** Los tres transportes NO son tres menorás. Son tres mutsaqót de la MISMA Menorá. La pieza sigue siendo UNA (miqshá). No se crea un componente nuevo — se extiende hashmal_comm.

### Testigo 7 — Éxodo 27:20 (aceite zaj)

```
שֶׁמֶן זַיִת זָךְ כָּתִית לַמָּאוֹר
```

"Aceite de oliva PURO (zaj), PRENSADO (katít), para la luminaria."

**Traducción firmware:** El dato que viaja por cualquier mutsaqá debe ser el MISMO formato puro. No importa si va por LoRa, WiFi o BLE — el aceite es zaj. El formato de paquete (menora_formato.c) es ÚNICO para todos los transportes.

---

## 2. ARQUITECTURA — Flujo del aceite

La visión de Zacarías 4:2-3,12 revela la arquitectura completa:

```
     🫒 Olivo derecho        🫒 Olivo izquierdo
     (zeitím yamín)          (zeitím smol)
           │                       │
           │   tsanterót zahav     │
           └──────┐   ┌───────────┘
                  ▼   ▼
              ┌─────────┐
              │  GULLÁ   │  ← Depósito central (Zac 4:2)
              │ (cuenco) │
              └────┬─────┘
                   │
         ┌─────┬──┴──┬─────┐
         ▼     ▼     ▼     ▼      ← mutsaqót (múltiples por ner)
       ┌───┐ ┌───┐ ┌───┐ ┌───┐
       │NER│ │NER│ │NER│ │NER│ ... (7 nerot)
       │ 0 │ │ 1 │ │ 2 │ │ 3 │
       └───┘ └───┘ └───┘ └───┘

    Cada NER tiene múltiples mutsaqót:
       ┌─────────────────────┐
       │      NER (canal)    │
       │  ┌─────┬─────┬────┐│
       │  │LoRa │ESP- │BLE ││  ← 3 mutsaqót
       │  │     │NOW  │    ││
       │  └─────┴─────┴────┘│
       └─────────────────────┘
```

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

**Header del archivo:**
```
PIEZA 04 — LA MENORÁ — menora_mutsaqot.c
Sub-pieza: LOS CONDUCTOS — selección de transporte
Zacarías 4:2 (mutsaqót) + Zac 4:6 (berují) + Zac 4:12 (tsanterót)
```

**Qué hace:** Selector inteligente de transporte. Cada ner (canal) tiene múltiples mutsaqót (conductos) disponibles. Este archivo decide cuál usar para cada envío.

**Tipo enum para los transportes (mutsaqót):**
```c
/**
 * Zac 4:2 — mutsaqót: los conductos por donde el aceite llega al ner.
 * Cada mutsaqá es un medio físico de transporte distinto.
 *
 * LORA    = LoRa SX1262 — largo alcance (2-15 km), bajo ancho de banda
 * ESPNOW  = ESP-NOW — WiFi P2P directo (100-200 m), alto ancho de banda
 * BLE     = Bluetooth Low Energy — proximidad (10-30 m), bajo consumo
 *
 * "orót eilím meadammím veorót tejashím milmaʻlá" — Éx 26:14
 * Dos materiales en la misma capa = múltiples medios coexistiendo.
 */
typedef enum {
    HASHMAL_MUTSAQA_LORA   = 0,  /* Zac 4:2 — conducto 1: LoRa SX1262 */
    HASHMAL_MUTSAQA_ESPNOW = 1,  /* Zac 4:2 — conducto 2: WiFi P2P (ESP-NOW) */
    HASHMAL_MUTSAQA_BLE    = 2,  /* Zac 4:2 — conducto 3: Bluetooth LE */
} hashmal_mutsaqa_tipo_t;

#define HASHMAL_MUTSAQOT_TOTAL  3  /* Zac 4:2 — conductos disponibles */
```

**Estructura para estado de una mutsaqá:**
```c
/**
 * Estado operativo de un conducto (mutsaqá).
 * Zac 4:12 — tsanterót: cada tubo tiene estado propio.
 */
typedef struct {
    hashmal_mutsaqa_tipo_t tipo;
    bool     disponible;        /* ¿el hardware está presente y funcional? */
    bool     activa;            /* ¿encendida para este ner? */
    uint8_t  prioridad;         /* 0=más alta. Orden de preferencia */
    uint32_t max_payload_bytes; /* capacidad máxima por envío */
    uint32_t alcance_estimado_m;/* metros estimados */
    uint8_t  energia_relativa;  /* 0-100, consumo relativo */
    uint64_t ultimo_uso_ms;
    uint64_t errores;
} hashmal_mutsaqa_t;
```

**Estructura extendida del ner (actualizar hashmal_ner_t):**
Añadir al struct existente `hashmal_ner_t`:
```c
    /* Zac 4:2 — mutsaqót: múltiples conductos por lámpara */
    hashmal_mutsaqa_t mutsaqot[HASHMAL_MUTSAQOT_TOTAL];
    uint8_t           mutsaqot_activas;  /* cuántas disponibles */
```

**Funciones públicas de menora_mutsaqot.c:**

```c
/* ── menora_mutsaqot.c — Zac 4:2: selector de conductos ─── */

/**
 * Zac 4:2 — Inicializa las mutsaqót de todos los nerot.
 * Detecta qué hardware está presente (LoRa, ESP-NOW, BLE).
 * Se llama DESPUÉS de que los drivers individuales estén listos.
 */
int  hashmal_comm_mutsaqot_init(void);

/**
 * Zac 4:6 — "No por fuerza sino por Mi Espíritu."
 * Selecciona la mutsaqá ÓPTIMA para un envío dado.
 *
 * Criterios de selección (en orden de prioridad):
 *   1. ¿El dato CABE? (tam <= max_payload_bytes)
 *   2. ¿La mutsaqá está DISPONIBLE y ACTIVA?
 *   3. ¿La energía es SUFICIENTE? (Éx 27:20 — aceite zaj)
 *   4. Elegancia: la de menor consumo que cumpla 1-3
 *
 * @param canal   El ner que quiere transmitir
 * @param tam     Tamaño del payload en bytes
 * @param destino Nodo destino (para estimar distancia si se conoce)
 * @param out     Tipo de mutsaqá seleccionada
 * @return HASHMAL_COMM_OK o error
 */
int  hashmal_comm_mutsaqa_seleccionar(hashmal_canal_id_t canal,
                                       size_t tam,
                                       uint8_t destino,
                                       hashmal_mutsaqa_tipo_t *out);

/**
 * Envía por la mutsaqá indicada. Delega al driver correspondiente.
 * @return HASHMAL_COMM_OK o código de error del driver.
 */
int  hashmal_comm_mutsaqa_enviar(hashmal_mutsaqa_tipo_t tipo,
                                  const uint8_t *datos, size_t tam);

/**
 * Recibe por la mutsaqá indicada.
 */
int  hashmal_comm_mutsaqa_recibir(hashmal_mutsaqa_tipo_t tipo,
                                   uint8_t *buf, size_t tam,
                                   size_t *recibidos);

/**
 * ¿Cuántas mutsaqót tiene activas un ner dado?
 */
uint8_t hashmal_comm_mutsaqot_activas(hashmal_canal_id_t canal);

/**
 * ¿Está disponible una mutsaqá específica? (hardware presente + funcional)
 */
bool hashmal_comm_mutsaqa_disponible(hashmal_mutsaqa_tipo_t tipo);
```

**Lógica del selector (Zac 4:6 — berují):**

```
REGLA 1 — Si tam > 250 bytes:
    → LoRa NO puede (max 250). Intentar ESP-NOW.
    → Si ESP-NOW no disponible, fragmentar para LoRa.

REGLA 2 — Si tam <= 250 bytes Y hay ESP-NOW disponible Y destino cercano (<200m):
    → Preferir ESP-NOW (más rápido, menos energía por byte).

REGLA 3 — Si es heartbeat, discovery, o intercambio de claves (tam < 50):
    → Preferir BLE si disponible (menor consumo).

REGLA 4 — Si nada más está disponible:
    → LoRa SIEMPRE es el fallback. Es el gézaʻ — el tronco.
    → LoRa siempre funciona. Es tamíd (Éx 27:21).

REGLA 5 — "No por fuerza" (Zac 4:6):
    → Entre dos mutsaqót igualmente capaces, elegir la de MENOR consumo.
    → Eficiencia sobre potencia.
```

**Referencia bíblica para cada regla en comentarios del código:**
- Regla 1: "kikkár zaháv tahór" (Éx 25:39) — respetar el presupuesto. Si no cabe en el tubo, usar otro.
- Regla 2: "lo vejáyil veló vejóaj" (Zac 4:6) — no desperdiciar potencia LoRa si el vecino está cerca.
- Regla 3: "shémen zayit zaj" (Éx 27:20) — aceite puro = mínimo consumo para la tarea mínima.
- Regla 4: "juqqát ʻolám" (Éx 27:21) — LoRa es perpetuo, siempre encendido, el tamíd del sistema.
- Regla 5: "kí im berují" (Zac 4:6) — por Mi Espíritu = diseño inteligente, no fuerza bruta.

---

### 3.2 — menora_espnow_driver.c (MUTSAQÁ WiFi P2P)

**Ubicación:** `hashmal-firmware/components/hashmal_comm/src/menora_espnow_driver.c`

**Referencia bíblica principal:** Éxodo 26:3 — "ishá el ajotá" (una mujer a su hermana)

**Header del archivo:**
```
PIEZA 04 — LA MENORÁ — menora_espnow_driver.c
Sub-pieza: MUTSAQÁ 2 — WiFi P2P (ESP-NOW)
Éx 26:3 (ishá el ajotá) + Zac 4:2 (mutsaqót) + Zac 4:12 (tsanterót zahav)
```

**Fundamento bíblico:**

```
"jameish hayeríʻot tihyéna jevurót ISHÁ EL AJOTÁ"  — Éx 26:3
"Cinco cortinas estarán unidas UNA A SU HERMANA."
```

ishá el ajotá = mujer a su hermana = conexión DIRECTA entre pares iguales. No hay servidor. No hay intermediario. Hermana a hermana. Esto ES WiFi P2P — ESP-NOW: dos ESP32 hablándose directo sin router, sin internet, sin nada en medio.

**Segundo testigo:**
```
"meshullavót ISHÁ EL AJOTÁ"  — Éx 26:17
"Conectadas (meshullavót) UNA A SU HERMANA."
```

meshullavót (מְשֻׁלָּבֹת, H7947) = entrelazadas, conectadas. De shalav = unir escalones. Conexión directa, entrelazada, sin intermediario.

**Qué hace:** Driver ESP-NOW para comunicación WiFi P2P entre nodos ESP32-S3. Sin router. Sin internet. Directo.

**Características técnicas de ESP-NOW:**
- Protocolo de Espressif, layer 2, sin TCP/IP
- Alcance: 100-200 metros (línea de vista, puede más con antena)
- Velocidad: hasta 1 Mbps
- Payload máximo por frame: 250 bytes (pero se puede fragmentar con rapidez)
- Hasta 20 peers simultáneos (encrypted) o 10 (if mixed)
- NO necesita WiFi conectado a un router. Usa WiFi en modo station o softAP
- Encriptación CCMP (AES-128) por par de peers
- Broadcast y unicast
- Callback de confirmación de entrega

**APIs ESP-IDF a encapsular (ORO DE EGIPTO — nunca llamar directo desde lógica de negocio):**

```c
/* Estas se encapsulan detrás de nuestras funciones.
   "Oro de Egipto" — se funde según el diseño del Padre. */

#include "esp_now.h"
#include "esp_wifi.h"

/* Encapsulaciones necesarias: */
esp_now_init()           → espnow_init_interno()
esp_now_register_send_cb → espnow_registrar_cb_envio()
esp_now_register_recv_cb → espnow_registrar_cb_recepcion()
esp_now_send()           → espnow_enviar_frame()
esp_now_add_peer()       → espnow_agregar_hermana()
esp_now_del_peer()       → espnow_quitar_hermana()
esp_wifi_init()          → espnow_wifi_preparar()
esp_wifi_start()         → espnow_wifi_levantar()
```

**Funciones públicas (añadir al header hashmal_comm.h):**

```c
/* ── menora_espnow_driver.c — Éx 26:3 ishá el ajotá: WiFi P2P ── */

/**
 * Éx 26:3 — "ishá el ajotá": levantar la conexión hermana-a-hermana.
 * Inicializa WiFi en modo station + ESP-NOW.
 * NO conecta a ningún router. NO toca internet.
 */
int  hashmal_comm_espnow_init(void);

/**
 * Desmontar la mutsaqá ESP-NOW. Libera WiFi y peers.
 */
int  hashmal_comm_espnow_deinit(void);

/**
 * Enviar datos por ESP-NOW a un peer (o broadcast).
 * @param mac_destino  MAC del peer destino (NULL = broadcast)
 * @param datos        Payload (ya formateado por menora_formato.c)
 * @param tam          Tamaño en bytes
 */
int  hashmal_comm_espnow_enviar(const uint8_t *mac_destino,
                                 const uint8_t *datos, size_t tam);

/**
 * Callback de recepción (registrado internamente).
 * Pone los datos recibidos en el buffer RX del ner correspondiente.
 */
/* INTERNA — no exponer en API pública, se registra en init */

/**
 * Agregar una hermana (peer). Éx 26:3 — ishá el ajotá.
 * @param mac  MAC address del peer ESP32 vecino
 */
int  hashmal_comm_espnow_agregar_hermana(const uint8_t mac[6]);

/**
 * Quitar una hermana (peer). Ap 2:5 — puede perder su lugar.
 */
int  hashmal_comm_espnow_quitar_hermana(const uint8_t mac[6]);

/**
 * ¿La mutsaqá ESP-NOW está disponible? (WiFi inicializado + ESP-NOW activo)
 */
bool hashmal_comm_espnow_disponible(void);

/**
 * Número de hermanas (peers) conectadas.
 */
uint8_t hashmal_comm_espnow_hermanas(void);
```

**Fragmentación para payloads grandes (fotos/video):**
ESP-NOW tiene límite de 250 bytes por frame. Para datos grandes:

```
Protocolo de fragmentación — "lechem happaním" (pan de la presencia)

Lv 24:5-6: "doce tortas" (12 panes), cada uno de 2/10 de efa.
El pan grande se divide en porciones IGUALES y CONTADAS.

Header de fragmento (5 bytes):
  [msg_id: 2 bytes][frag_num: 1 byte][frag_total: 1 byte][flags: 1 byte]

Payload útil por fragmento: 250 - 5 = 245 bytes
Máximo fragmentos: 255 → máximo mensaje: 255 × 245 = 62,475 bytes (~61 KB)
Para fotos comprimidas (JPEG): suficiente para fotos de calidad media.
```

**Reglas de implementación:**
1. WiFi se inicia en modo WIFI_MODE_STA (station) pero SIN conectar a ningún AP. Solo para ESP-NOW.
2. Encriptación por par: cada peer tiene clave compartida (derivada de hashmal_keygen, pieza 13).
3. Canal WiFi: usar canal 1 por defecto (configurable).
4. NO iniciar WiFi si LoRa está en modo de bajo consumo extremo (respetar aceite, Éx 27:20).
5. Cada función ESP-IDF encapsulada detrás de función propia (oro de Egipto).
6. Callbacks de envío/recepción son internos, no expuestos.

---

### 3.3 — menora_ble_driver.c (MUTSAQÁ Bluetooth)

**Ubicación:** `hashmal-firmware/components/hashmal_comm/src/menora_ble_driver.c`

**Referencia bíblica principal:** Éxodo 28:34-35 — las campanillas de la vestidura

**Header del archivo:**
```
PIEZA 04 — LA MENORÁ — menora_ble_driver.c
Sub-pieza: MUTSAQÁ 3 — Bluetooth Low Energy
Éx 28:34-35 (paʻamón, campanillas) + Zac 4:2 (mutsaqót)
```

**Fundamento bíblico:**

```
"paʻamón zahav verimmon paʻamón zahav verimmon
 ʻal shulei hammeʻíl savív"  — Éx 28:34
"Campanilla de oro y granada, campanilla de oro y granada,
 en los bordes de la vestidura alrededor."
```

paʻamón (פַּעֲמוֹן, H6472) = campanilla. Pequeña. Suena al MOVERSE. Sirve para:
1. ANUNCIAR presencia ("venishmaʻ qoló" — su sonido será OÍDO, v.35)
2. Corto alcance — solo se oye de CERCA
3. Bajo consumo — es una campanilla, no un shofar

Esto ES Bluetooth: señal de corto alcance, bajo consumo, que ANUNCIA presencia y permite descubrimiento de vecinos.

**Segundo testigo:**

```
"venishmaʻ qoló bevoʼó el haqqódesh lifnéi YHWH
 uvetseitó veló yamút"  — Éx 28:35
"Y se oirá su sonido al ENTRAR al lugar santo delante de YHWH
 y al SALIR, para que no muera."
```

El paʻamón anuncia ENTRADA y SALIDA. En firmware: BLE anuncia cuando un nodo ENTRA al rango (discovery) y cuando SALE (disconnect). Es protocolo de PRESENCIA.

**Qué hace:** Driver BLE (NimBLE) para descubrimiento de vecinos, sincronización de estado, e intercambio de claves a corto alcance.

**Características técnicas de BLE en ESP32-S3:**
- Stack: NimBLE (más ligero que Bluedroid, menos RAM)
- Alcance: 10-30 metros típico (hasta 100m en campo abierto)
- Velocidad: ~2 Mbps (BLE 5.0 en ESP32-S3)
- Bajo consumo: diseñado para coin cells
- Advertising: anuncia presencia sin conexión
- GATT: para intercambio de datos con conexión
- Coexiste con WiFi en el mismo radio (ESP32-S3 maneja coexistencia)

**APIs ESP-IDF a encapsular (ORO DE EGIPTO):**

```c
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "host/ble_gap.h"
#include "host/ble_gatt.h"

/* Encapsulaciones: */
nimble_port_init()       → ble_init_interno()
ble_gap_adv_start()      → ble_anunciar_presencia()
ble_gap_disc()           → ble_descubrir_vecinos()
ble_gattc_write_flat()   → ble_enviar_dato()
```

**Funciones públicas (añadir al header hashmal_comm.h):**

```c
/* ── menora_ble_driver.c — Éx 28:34 paʻamón: Bluetooth LE ──── */

/**
 * Éx 28:34 — "paʻamón zahav": levantar la campanilla de oro.
 * Inicializa NimBLE stack. Advertising + GATT server.
 */
int  hashmal_comm_ble_init(void);

/**
 * Apagar la campanilla.
 */
int  hashmal_comm_ble_deinit(void);

/**
 * Éx 28:35 — "venishmaʻ qoló": se oirá su sonido.
 * Anuncia presencia por BLE advertising. Incluye ID de nodo
 * y estado básico para que vecinos lo detecten.
 */
int  hashmal_comm_ble_anunciar(void);

/**
 * Descubrir vecinos BLE (scan). Cada vecino detectado se
 * registra con su MAC, RSSI, y datos de advertising.
 * @param vecinos     Buffer para resultados
 * @param max         Máximo vecinos a reportar
 * @param encontrados Número real encontrado
 * @param timeout_ms  Duración del scan
 */
int  hashmal_comm_ble_descubrir(hashmal_ble_vecino_t *vecinos,
                                 uint8_t max,
                                 uint8_t *encontrados,
                                 uint32_t timeout_ms);

/**
 * Enviar dato corto por BLE GATT a un peer conectado.
 * Para heartbeats, estado, intercambio de claves.
 * @param mac_destino MAC del peer BLE
 * @param datos       Payload (formateado por menora_formato.c)
 * @param tam         Tamaño (típicamente < 50 bytes)
 */
int  hashmal_comm_ble_enviar(const uint8_t mac_destino[6],
                              const uint8_t *datos, size_t tam);

/**
 * ¿La mutsaqá BLE está disponible? (NimBLE stack activo)
 */
bool hashmal_comm_ble_disponible(void);
```

**Estructura para vecinos BLE:**
```c
/**
 * Éx 28:35 — vecino detectado por la campanilla.
 * "venishmaʻ qoló bevoʼó" — se oye su sonido al ENTRAR.
 */
typedef struct {
    uint8_t  mac[6];         /* dirección BLE del vecino */
    int8_t   rssi;           /* fuerza de señal (distancia aproximada) */
    uint8_t  nodo_id;        /* ID del nodo Hashmal (del advertising) */
    uint8_t  estado;         /* estado básico del vecino */
    uint64_t timestamp_ms;   /* cuándo se detectó */
} hashmal_ble_vecino_t;
```

**Usos específicos de BLE en la red:**
1. **Discovery** — "venishmaʻ qoló bevoʼó" (Éx 28:35): detectar vecinos cercanos
2. **Heartbeat** — "paʻamón zahav verimmon" (Éx 28:34): señal periódica de vida
3. **Intercambio de claves** — handshake inicial entre nodos nuevos (pieza 13, unción)
4. **Sincronización de estado** — estado compacto de la red local
5. **NO para datos grandes** — para eso está ESP-NOW

---

## 4. ARCHIVOS A MODIFICAR (3 existentes)

### 4.1 — hashmal_comm.h (header público)

**Añadir:**
1. El enum `hashmal_mutsaqa_tipo_t` (ver sección 3.1)
2. La constante `HASHMAL_MUTSAQOT_TOTAL` (= 3)
3. La estructura `hashmal_mutsaqa_t` (ver sección 3.1)
4. La estructura `hashmal_ble_vecino_t` (ver sección 3.3)
5. Añadir campo `mutsaqot[]` al struct `hashmal_ner_t` existente
6. Todas las funciones públicas de los 3 archivos nuevos
7. Nuevos errores:
   ```c
   HASHMAL_COMM_ERR_ESPNOW  = -12,  /* fallo ESP-NOW */
   HASHMAL_COMM_ERR_BLE     = -13,  /* fallo BLE */
   HASHMAL_COMM_ERR_MUTSAQA = -14,  /* ninguna mutsaqá disponible */
   HASHMAL_COMM_ERR_FRAG    = -15,  /* error de fragmentación */
   ```
8. Constantes de ESP-NOW:
   ```c
   #define HASHMAL_ESPNOW_MAX_HERMANAS   20    /* Éx 26:3 — máx peers */
   #define HASHMAL_ESPNOW_FRAME_BYTES    250   /* frame máximo */
   #define HASHMAL_ESPNOW_FRAG_HEADER    5     /* header de fragmento */
   #define HASHMAL_ESPNOW_FRAG_PAYLOAD   245   /* útil por fragmento */
   #define HASHMAL_ESPNOW_FRAG_MAX       255   /* máx fragmentos */
   #define HASHMAL_ESPNOW_MAX_MENSAJE    62475 /* 255 × 245 bytes */
   ```
9. Constantes de BLE:
   ```c
   #define HASHMAL_BLE_MAX_VECINOS       20    /* Éx 28:34 — campanillas */
   #define HASHMAL_BLE_ADV_INTERVAL_MS   1000  /* intervalo advertising */
   #define HASHMAL_BLE_SCAN_TIMEOUT_MS   3000  /* duración scan */
   ```
10. Actualizar el comentario de cabecera: donde dice "7 canales LoRa" cambiar a "7 canales multi-transporte (LoRa + ESP-NOW + BLE)" y añadir referencia a Zac 4:2.
11. Añadir al REQUIRES del comentario: Zac 4:2, Zac 4:6, Zac 4:12, Éx 26:3, Éx 26:14, Éx 28:34-35

**NO cambiar:** La API pública de envío/recepción (hashmal_comm_enviar, hashmal_comm_recibir, hashmal_comm_broadcast). Estas funciones siguen igual — internamente ahora pasarán por menora_mutsaqot.c para elegir transporte.

### 4.2 — hashmal_comm_internal.h (header interno)

**Añadir funciones internas para los nuevos drivers:**
```c
/* ── mutsaqot internals ── */

/**
 * Enviar por una mutsaqá específica. Usado por menora_routing.c
 * cuando menora_mutsaqot.c ya eligió el transporte.
 */
int hashmal_comm_mutsaqa_enviar_interno(hashmal_mutsaqa_tipo_t tipo,
                                         const uint8_t *datos,
                                         size_t tam);
```

### 4.3 — CMakeLists.txt (registro de archivos)

**Ubicación:** `hashmal-firmware/components/hashmal_comm/CMakeLists.txt`

**Cambiar de:**
```cmake
idf_component_register(
    SRCS "src/menora_hub.c" "src/menora_lora_driver.c" "src/menora_canales.c" "src/menora_aceite.c" "src/menora_routing.c" "src/menora_formato.c"
    INCLUDE_DIRS "include"
    PRIV_INCLUDE_DIRS "src"
    REQUIRES driver hashmal_framework esp_timer
)
```

**A:**
```cmake
idf_component_register(
    SRCS "src/menora_hub.c"
         "src/menora_lora_driver.c"
         "src/menora_canales.c"
         "src/menora_aceite.c"
         "src/menora_routing.c"
         "src/menora_formato.c"
         "src/menora_mutsaqot.c"
         "src/menora_espnow_driver.c"
         "src/menora_ble_driver.c"
    INCLUDE_DIRS "include"
    PRIV_INCLUDE_DIRS "src"
    REQUIRES driver hashmal_framework esp_timer esp_wifi esp_now bt
)
```

**Nota:** `esp_wifi` y `esp_now` son necesarios para ESP-NOW. `bt` es necesario para NimBLE. Todos son "oro de Egipto" — se encapsulan.

### 4.4 — menora_routing.c (actualizar para usar mutsaqot)

**Cambio conceptual:** Actualmente `menora_routing.c` llama directamente al driver LoRa SPI para enviar. Ahora debe llamar a `menora_mutsaqot.c` que decidirá qué transporte usar.

**Cambiar:**
Donde actualmente hace:
```c
hashmal_comm_spi_enviar(datos, tam);
```

Debe hacer:
```c
hashmal_mutsaqa_tipo_t transporte;
hashmal_comm_mutsaqa_seleccionar(canal, tam, destino, &transporte);
hashmal_comm_mutsaqa_enviar(transporte, datos, tam);
```

**NO cambiar** la lógica de routing (mesh, vecinos, tablas). Solo el punto donde entrega al driver.

---

## 5. REGLAS DE IMPLEMENTACIÓN

### 5.1 — El aceite es UNO (formato único)

**Éx 27:20** — "shémen zayit zaj" — aceite de oliva PURO.

El formato de paquete definido en `menora_formato.c` es ÚNICO para todos los transportes. El header [version][tipo][origen][destino][tam] no cambia. Lo que cambia es el TUBO por donde viaja, no el ACEITE.

**NO crear formatos distintos para LoRa vs ESP-NOW vs BLE.** El paquete es el mismo. menora_formato.c sigue siendo el único formateador.

Excepción: la fragmentación ESP-NOW añade un sub-header de fragmento ANTES del paquete estándar. Pero el paquete en sí es el mismo.

### 5.2 — LoRa es tamíd (siempre disponible)

**Éx 27:21** — "juqqát ʻolám ledorotám" — estatuto perpetuo por sus generaciones.

LoRa es el backbone. SIEMPRE está. Aunque ESP-NOW y BLE estén disponibles, LoRa nunca se apaga (salvo modo supervivencia extrema del aceite, pieza 08). Es el gézaʻ — el tronco de la Menorá.

### 5.3 — ESP-NOW no es internet

**PROHIBICIÓN ABSOLUTA.** ESP-NOW usa el radio WiFi pero NO conecta a internet. NO inicia DHCP. NO busca APs. NO abre sockets TCP/IP. Se inicializa WiFi en modo STA sin conectar, solo para que ESP-NOW use la capa 2.

Si alguien intenta hacer que ESP-NOW conecte a un router → eso es **esh zará** (fuego extraño, Lv 10:1-2). Prohibido.

### 5.4 — Coexistencia de radios

ESP32-S3 tiene un solo radio WiFi/BLE compartido. La coexistencia la maneja el hardware (modem sleep + arbitraje). Pero:
- NO usar LoRa (SX1262 por SPI) y ESP-NOW/BLE al MISMO INSTANTE en el mismo milisegundo. Serializar.
- LoRa es radio externo (SX1262). WiFi/BLE es radio interno del ESP32. Pueden coexistir con cuidado.
- El selector (menora_mutsaqot.c) debe garantizar que no se solapen transmisiones conflictivas.

### 5.5 — Cada archivo tiene header bíblico

Igual que todos los demás archivos del firmware. Cada archivo .c empieza con:
1. Nombre de pieza y sub-pieza
2. Referencias bíblicas (hebreo + traducción)
3. Exégesis de las palabras clave
4. Qué hace este archivo
5. Dependencias
6. Autor: Antonio José Marín Soto

### 5.6 — Mínimo 2 testigos por decisión

Cada decisión técnica en el código debe tener un comentario con referencia bíblica. Si una decisión solo tiene 1 testigo → buscar el segundo. Si no hay segundo → PARA y pregunta.

---

## 6. ORDEN DE CONSTRUCCIÓN

1. **Primero:** Actualizar `hashmal_comm.h` — el contrato (los nuevos tipos, structs, funciones)
2. **Segundo:** Actualizar `hashmal_comm_internal.h` — las funciones internas
3. **Tercero:** Escribir `menora_mutsaqot.c` — el selector (Zac 4:2)
4. **Cuarto:** Escribir `menora_espnow_driver.c` — mutsaqá WiFi P2P (Éx 26:3)
5. **Quinto:** Escribir `menora_ble_driver.c` — mutsaqá BLE (Éx 28:34)
6. **Sexto:** Modificar `menora_routing.c` — pasar envíos por mutsaqot en vez de directo a SPI
7. **Séptimo:** Actualizar `CMakeLists.txt`
8. **Octavo:** Actualizar `menora_hub.c` init/deinit — inicializar las 3 mutsaqót

---

## 7. RESUMEN BÍBLICO

| Concepto técnico | Palabra hebrea | Referencia | Significado |
|---|---|---|---|
| Multi-transporte | mutsaqót (מוּצָקוֹת) | Zac 4:2 | Múltiples conductos por lámpara |
| Tubos de oro | tsanterót (צַנְתְּרוֹת) | Zac 4:12 | Conductos activos de máxima confianza |
| WiFi P2P (ESP-NOW) | ishá el ajotá (אִשָּׁה אֶל אֲחֹתָהּ) | Éx 26:3 | Hermana a hermana, directo |
| Bluetooth LE | paʻamón (פַּעֲמוֹן) | Éx 28:34-35 | Campanilla, anuncia presencia |
| Selector inteligente | berují (בְּרוּחִי) | Zac 4:6 | Por Mi Espíritu, no por fuerza |
| Formato único | shémen zaj (שֶׁמֶן זָךְ) | Éx 27:20 | Aceite puro — un solo formato |
| LoRa siempre activo | tamíd (תָּמִיד) | Éx 27:21 | Perpetuo, siempre encendido |
| Capa física múltiple | orót tejashím (עֹרוֹת תְּחָשִׁים) | Éx 26:14 | Múltiples pieles = múltiples medios |
| Unidad | miqshá ajat (מִקְשָׁה אַחַת) | Éx 25:36 | Todo UNA pieza, no tres sistemas |
| No internet | esh zará (אֵשׁ זָרָה) | Lv 10:1-2 | Fuego extraño = prohibido |

---

*"Mira y haz conforme al MODELO que te es mostrado en el monte."*
*— Éxodo 25:40*

*Fuente única: La Palabra de YHWH*
*Autor: Antonio José Marín Soto*
*Fecha: 21 de abril de 2026*
