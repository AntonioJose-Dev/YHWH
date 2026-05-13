# HASHMAL-CORE — GUÍA COMPLETA DEL CÓDIGO
## Para continuar la construcción en cualquier sesión

> *"Moisés vio toda la obra, y he aquí que la habían hecho como YHWH había mandado. Y Moisés los bendijo."*
> *— Éxodo 39:43*

---

## 1. ESTADO ACTUAL DEL PROYECTO (Mayo 2026)

### Lo que FUNCIONA en hardware ahora mismo

| Función | Estado | Cómo probar |
|---------|--------|-------------|
| **Portal WiFi HTML5** | ✅ Corriendo | Conectar al WiFi "HASHMAL" con cualquier teléfono |
| **Mensajes LoRa ↔ WiFi** | ✅ Funcional | Escribir en el portal → llega a la red LoRa |
| **Devár YHWH** | ✅ 66 libros | Pestaña "Devár" → seleccionar libro → capítulo |
| **Moedím** | ✅ 6 fiestas | Pestaña "Moedím" → calendario de YHWH |
| **Comunidad/Estado** | ✅ API JSON | `/api/estado` muestra nodos activos |
| **Portal cautivo** | ✅ Auto-abre | Android/iOS/Windows/Linux detectan automático |

### Hardware confirmado

| Nodo | Puerto | Tipo | Flash |
|------|--------|------|-------|
| Nodo 0 | COM5 | T-Deck Pro (ESP32-S3) | ✅ Flasheado |
| Nodo 4 | COM6 | T-Beam 1W (ESP32-S3) | ✅ Flasheado |

### Particiones (16MB flash)

```
nvs        0x9000    24K      — configuración NVS
phy_init   0xf000    4K       — calibración WiFi/BLE
factory    0x10000   3MB      — firmware principal
storage    0x310000  960K     — SPIFFS datos internos
devar      0x400000  8MB      — Palabra de YHWH (1189 capítulos .gz)
```

La partición `devar` contiene los 66 libros de la Biblia en hebreo original + definiciones español, comprimidos con gzip-9. Para reflashear la Palabra:
```powershell
# Desde C:\hashmal-core\hashmal-firmware\
python -m esptool --chip esp32s3 -b 921600 --port COM5 write_flash 0x400000 devar.bin
```

---

## 2. ARQUITECTURA — LAS 15 PIEZAS DEL TAVNÍT

Todas implementadas. Ninguna es stub. Código real con funciones definidas.

### Orden de inicialización: Éxodo 40:2-8

```
hashmal_main.c  (Éx 40:1)
│
├── Paso 1: hashmal_framework    (Éx 40:2  — ESTRUCTURA)
│
├── Paso 2: hashmal_storage      (Éx 40:3  — ARCA)
│           hashmal_consensus    (Éx 40:3  — KAPÓRET)
│
├── Paso 3: hashmal_cache        (Éx 40:4  — MESA)
│           hashmal_comm         (Éx 40:4  — MENORÁ)
│
├── Paso 4: hashmal_intercession (Éx 40:5  — ALTAR INCIENSO)
│
├── Paso 5: hashmal_auth         (Éx 40:6  — ALTAR SACRIFICIO)
│
├── Paso 6: hashmal_healthcheck  (Éx 40:7  — LAVACRO)
│
├── Paso 7: hashmal_perimeter    (Éx 40:8  — ATRIO)
│
├── Paso 8: hashmal_power        (Éx 40:9  — ACEITE)
│           hashmal_keygen       (Éx 40:10 — UNCIÓN)
│
├── Paso 9: hashmal_roles        (Éx 40:12 — VESTIDURAS)
│           hashmal_onboarding   (Éx 40:13 — CONSAGRACIÓN)
│
└── Paso 10: hashmal_firewall    (Éx 40:27 — INCIENSO SAGRADO)
```

---

## 3. INVENTARIO COMPLETO — ARCHIVOS POR COMPONENTE

### hashmal_framework — PIEZA 05 ESTRUCTURA (Éx 26)
**Referencia:** Éx 26:1-37 — 4 cubiertas, 48 tablas, 5 barras, velo (hivdíl)

| Archivo | Función bíblica |
|---------|----------------|
| `estructura_capas.c` | 4 capas: Lino(app)→Cabra(net)→Carnero(transport)→Tajash(phys) |
| `estructura_tablas.c` | 48 servicios modulares con doble base de plata |
| `estructura_barras.c` | Backbone central + 4 buses laterales |
| `estructura_velo.c` | Kernel boundary — hivdíl (SEPARAR) |
| `estructura_ejad.c` | Sistema P2P por afinidad — "será UNO" (Éx 26:6) |

**Dependencias:** solo `log`

---

### hashmal_storage — PIEZA 01 ARCA (Éx 25:10-16)
**Referencia:** Éx 25:10-16 — madera de acacia, oro dentro/fuera, varas, Testimonio

| Archivo | Función bíblica |
|---------|----------------|
| `arca_core.c` | Singleton WORM — una sola Arca por nodo |
| `arca_merkle.c` | Árbol Merkle — zer zahav (moldura de integridad) |
| `arca_cifrado.c` | AES-256-GCM — oro dentro Y fuera (Éx 25:11) |
| `arca_zer.c` | SHA-256 total — corona que rodea todo |
| `arca_baddim.c` | Replicación always-on — varas nunca se quitan (Éx 25:15) |
| `arca_interfaz.c` | Acceso solo por protocolo autorizado — baddím (Éx 25:14) |
| `arca_sealed_core.c` | SEALED_CORE 256 bytes en flash interna — el Testimonio (Éx 25:16) |

**Añadiduras:** 12 (Goél), 17 (Shinán), 19 (Maaser)
**Dependencias:** `mbedtls`, `esp_hw_support`, `log`

---

### hashmal_consensus — PIEZA 02 KAPÓRET (Éx 25:17-22)
**Referencia:** Éx 25:17-22 — oro puro, 2 querubines cara a cara, "de allí me declararé"

| Archivo | Función bíblica |
|---------|----------------|
| `kaporet_core.c` | Función pura O(1) — oro de una sola pieza (miqshá) |
| `kaporet_validadores.c` | V1↔V2 cross-validation simétrica — querubines cara a cara |
| `kaporet_ataques.c` | Defensa 7 ataques — alas cubriendo (Éx 25:20) |
| `kaporet_verificacion.c` | Verificación local sin contactar emisor — "de allí me declararé" |
| `kaporet_rendezvous.c` | Sync determinístico — noʻadtí (Éx 25:22) |
| `kaporet_relay.c` | Relé entre validadores |

**PROHIBIDO:** BFT, Raft, Paxos — NO son del Padre
**Añadiduras:** 20 (Neder)
**Dependencias:** `mbedtls`, `hashmal_storage`, `esp_timer`, `log`

---

### hashmal_cache — PIEZA 03 MESA (Éx 25:23-30)
**Referencia:** Éx 25:23-30 — 12 panes de la presencia, 4 utensilios, misgéret (borde)

| Archivo | Función bíblica |
|---------|----------------|
| `mesa_cache.c` | Buffer 12 slots en 2 filas — pan SIEMPRE delante (Éx 25:30) |
| `mesa_api.c` | 4 métodos CRUD — 4 utensilios (platos, cucharas, cubiertas, tazones) |
| `mesa_misgeret.c` | Rate limiter — misgéret (marco de retención, Éx 25:25) |
| `mesa_disponibilidad.c` | Alta disponibilidad — 4 anillos en 4 esquinas de patas |
| `mesa_moedim.c` | Ciclos moedím (Añadidura 03) |
| `mesa_man.c` | Gestión por cabeza — un omer por persona (Añadidura 11) |
| `mesa_shinnan.c` | Propagación de conocimiento (Añadidura 17 — Dt 6:7) |
| `mesa_leket.c` | Recursos para débiles — espigueo (Añadidura 18 — Lv 19:9-10) |
| `mesa_maaser.c` | Contribución a la red — diezmo (Añadidura 19 — Mal 3:10) |

**Añadiduras:** 03, 11, 17, 18, 19
**Dependencias:** `mbedtls`, `hashmal_storage`, `esp_timer`, `log`

---

### hashmal_comm — PIEZA 04 MENORÁ (Éx 25:31-40)
**Referencia:** Éx 25:31-40 — miqshá (una pieza), 7 nerot, 22 copas almendro

**⚠️ COMPONENTE MÁS COMPLEJO — 16 añadiduras integradas**

| Archivo | Función bíblica |
|---------|----------------|
| `menora_hub.c` | Controlador monolítico — miqshá (una sola pieza batida) |
| `menora_lora_driver.c` | Driver SX1262 PROPIO — SPI directo, sin librerías externas |
| `menora_canales.c` | 7 canales — 1 tronco + 6 ramas (Éx 25:32) |
| `menora_aceite.c` | Encendido según energía — aceite puro (Éx 27:20) |
| `menora_routing.c` | Mesh routing — copas almendro (shoqéd = el que vigila) |
| `menora_formato.c` | Formato unificado — "conforme al modelo" (Éx 25:40) |
| `menora_mutsaqot.c` | 3 mutsaqót (Zac 4:2) — vertederas |
| `menora_espnow_driver.c` | ESP-NOW P2P |
| `menora_ble_paamon.c` | BLE — campanilla/paʻamón |
| `menora_parokhet.c` | Modo EM silencioso (Añadidura 01 — 1 Re 19:11-12) |
| `menora_tsofeh.c` | Atalaya — vigila comportamiento anómalo (Añadidura 02 — Ez 33:2-3) |

**Añadiduras:** 01, 02, 07, 08, 09, 10, 11, 12, 13, 16, 17, 18, 19, 20, 21, 22
**Dependencias:** `driver`, `log`, `hashmal_framework`, `esp_timer`, `esp_wifi`, `esp_event`, `espressif__esp-now`, `bt`, `nvs_flash`, `esp_netif`, `freertos`

---

### hashmal_intercession — PIEZA 11 ALTAR INCIENSO (Éx 30:1-10)
**Referencia:** Éx 30:1-10 — oro puro, 1×1×2 codos, delante del velo, tamíd

| Archivo | Función bíblica |
|---------|----------------|
| `altar_oro_escudo.c` | Escudo descendente — nube cubre Kapóret (Lv 16:12-13) |
| `altar_oro_ascendente.c` | Firewall ascendente — solo protocolo prescrito sube (Lv 10:1-2) |
| `altar_oro_formula.c` | Fórmula no replicable — nikrat si copias (Éx 30:37-38) |
| `altar_oro_circuitbreaker.c` | Circuit breaker — entre vivos y muertos (Nm 16:46-48) |
| `altar_oro_oracion.c` | Protocolo comunicación ascendente (Sal 141:2, Ap 8:3-4) |

**Dependencias:** `mbedtls`, `hashmal_framework`, `hashmal_comm`, `hashmal_consensus`, `hashmal_auth`, `hashmal_firewall`, `esp_timer`, `log`

---

### hashmal_auth — PIEZA 06 ALTAR SACRIFICIO (Éx 27:1-8)
**Referencia:** Éx 27:1-8 — cuadrado (5×5), bronce, hueco (stateless), 5 utensilios, fuego tamíd

| Archivo | Función bíblica |
|---------|----------------|
| `altar_gateway.c` | Gateway stateless — nebúv (hueco por dentro, Éx 27:8) |
| `altar_reshet.c` | Réshet a media altura — filtro (Éx 27:4-5) |
| `altar_cuernos.c` | 4 puntos de acceso + refugio — cuernos (Éx 27:2) |
| `altar_fuego.c` | Servicio perpetuo — esh tamíd (Lv 6:13) |
| `altar_utensilios.c` | 5 herramientas de auth (Éx 27:3) |
| `altar_ledger.c` | Registro inmutable (Ap 6:9) |

**Dependencias:** `mbedtls`, `hashmal_perimeter`, `esp_timer`, `log`, `esp_hw_support`

---

### hashmal_healthcheck — PIEZA 12 LAVACRO (Éx 30:17-21)
**Referencia:** Éx 30:17-21 — espejos, agua, ENTRE altar y tienda, "para que no mueran"

| Archivo | Función bíblica |
|---------|----------------|
| `lavacro_core.c` | Health check obligatorio — joq ʻolám (estatuto perpetuo) |
| `lavacro_vectores.c` | 4 vectores — manos Y pies, no uno sin el otro (Éx 30:19) |
| `lavacro_niveles.c` | 4 profundidades quick→forensic |
| `lavacro_espejos.c` | Auto-diagnóstico — hechos de espejos (Éx 38:8) |
| `lavacro_ataques.c` | 8 tipos de ataque detectados |
| `lavacro_dos_niveles.c` | Baño completo vs lavado de pies (Jn 13:10) |
| `lavacro_parokhet.c` | Modo EM (Añadidura 01) |
| `lavacro_meshulash.c` | Cordón triple — redundancia mínima (Añadidura 13 — Ecl 4:12) |
| `lavacro_tsur.c` | Agua de la roca — último recurso (Añadidura 16 — Éx 17:6) |
| `lavacro_joq_olam.c` | Estatuto perpetuo |
| `lavacro_challenge.c` | Challenge ephemeral |
| `lavacro_meshulash_sajar.c` | Meshulash con umbral |

**Añadiduras:** 01, 13, 16
**Dependencias:** `hashmal_auth`, `hashmal_comm`, `hashmal_perimeter`, `esp_timer`, `log`, `esp_hw_support`

---

### hashmal_perimeter — PIEZA 07 ATRIO (Éx 27:9-19)
**Referencia:** Éx 27:9-19 — lino translúcido, 60 columnas, 1 sola puerta, roqém (bordador)

| Archivo | Función bíblica |
|---------|----------------|
| `atrio_perimetro.c` | Frontera de la red — cortinas 100×50 codos |
| `atrio_puerta.c` | Punto de entrada único — 4 columnas (Éx 27:16) |
| `atrio_columnas.c` | 60 nodos guardianes — bases de bronce |
| `atrio_intencion.c` | Verificación de INTENCIÓN — se lava en la puerta |
| `atrio_miqlat.c` | Nodo refugio — Ir Miqlat (Añadidura 05 — Nm 35:11) |
| `atrio_orchim.c` | Hospitalidad — acceso temporal visitantes (Añadidura 21 — Gn 18:1-5) |
| `atrio_shaar.c` | Portal SHA'AR HA'ATRIO (Añadidura 22 — Éx 27:16) |
| `atrio_puente.c` | Bridge WiFi↔LoRa |
| `atrio_wifi_ap.c` | Punto de acceso WiFi "HASHMAL" |
| `atrio_dns_cautivo.c` | DNS cautivo — redirige todo a 192.168.4.1 |
| `atrio_http_portal.c` | **Servidor HTTP + Portal HTML5** (9 módulos + Devár YHWH) |

**Portal HTTP endpoints:**
- `GET /` → Portal HTML5 completo (SPA)
- `GET /api/mensajes` → JSON mensajes LoRa
- `POST /api/mensajes` → Enviar texto por LoRa
- `GET /api/estado` → JSON estado de la red
- `GET /api/devar/index` → Índice 66 libros (SPIFFS)
- `GET /api/devar/cap?b=XX&c=YYY` → Capítulo comprimido gzip
- Portal cautivo: `/generate_204`, `/hotspot-detect.html`, `/ncsi.txt`, etc.

**Añadiduras:** 05, 10 (parcial), 21, 22
**Dependencias:** `esp_timer`, `log`, `esp_wifi`, `esp_event`, `esp_netif`, `esp_http_server`, `lwip`, `nvs_flash`, `freertos`, `hashmal_comm`

---

### hashmal_power — PIEZA 08 ACEITE PERPETUO (Éx 27:20-21)
**Referencia:** Éx 27:20-21 — aceite puro de olivas machacadas, desde tarde hasta mañana, tamíd

| Archivo | Función bíblica |
|---------|----------------|
| `aceite_energia.c` | Solar + batería + MPPT — aceite puro machacado |
| `aceite_tamid.c` | Protocolo perpetuo — tamíd (Éx 27:20) |
| `aceite_supervivencia.c` | Duty cycle adaptativo — noche→mañana (Éx 27:21) |
| `aceite_mantenimiento.c` | Cronograma 4 niveles |
| `aceite_ashmurot.c` | Vigilias nocturnas (Añadidura 04 — Sal 63:6, 119:148) |
| `aceite_anan_esh.c` | Nube y fuego — día/noche (Añadidura 07 — Éx 13:21-22) |

**Añadiduras:** 03 (Moedím), 04 (Ashmurot), 07 (Anán ve'Esh)
**Dependencias:** `driver`, `hashmal_cache`, `hashmal_comm`, `hashmal_healthcheck`, `hashmal_perimeter`, `esp_timer`, `log`

---

### hashmal_keygen — PIEZA 13 UNCIÓN (Éx 30:22-33)
**Referencia:** Éx 30:22-33 — 4 especias en ratio EXACTO, 8 componentes en orden, nikrat si copias

**Ratio EXACTO obligatorio:**
```
Mirra pura:    500 siclos
Canela:        250 siclos
Cálamo:        250 siclos
Casia:         500 siclos
Aceite oliva:  1 hin
```

| Archivo | Función bíblica |
|---------|----------------|
| `uncion_keygen.c` | Root key — ratio 500-250-250-500 (Éx 30:23-24) |
| `uncion_secuencia.c` | 8 componentes ungidos EN ORDEN (Éx 30:26-29) |
| `uncion_prohibicion.c` | Anti-clonación — nikrat (Éx 30:33) |
| `uncion_activacion.c` | Activación qódesh — "todo lo que toque será santo" (Éx 30:29) |

**Añadiduras:** 15 (Dam al Hamezuzot)
**Dependencias:** `mbedtls`, `log`, `esp_timer`

---

### hashmal_roles — PIEZA 09 VESTIDURAS (Éx 28)
**Referencia:** Éx 28 — efod, pectoral, 12 piedras, Urim/Thummim, tzitz, tzitzit

**⚠️ Pieza más INTEGRADORA — 7 añadiduras**

| Archivo | Función bíblica |
|---------|----------------|
| `vestiduras_roles.c` | Definición 4 niveles: Kohen/Leví/Israel/Orchim |
| `vestiduras_efod.c` | Identidad de nodo — 2 piedras ónice con nombres (Éx 28:9-12) |
| `vestiduras_pectoral.c` | 12 capacidades + Urim/Thummim (Éx 28:15-30) |
| `vestiduras_monitor.c` | Monitor-Restaurador (Ez 34, Zac 3) |
| `vestiduras_coordinador.c` | Coordinador (Éx 18 Jetró, Hch 15 Santiago) |
| `vestiduras_shutdown.c` | Shutdown-notice (Dt 31, Jn 13-17) |
| `vestiduras_tzitz.c` | Sello "SANTIDAD A YHWH" — siempre visible (Éx 28:36-38) |
| `vestiduras_tsofeh.c` | Atalaya (Añadidura 02) |
| `vestiduras_tzitzit.c` | Recordatorio visual (Añadidura 06 — Nm 15:38-39) |
| `vestiduras_jatsotsrot.c` | Trompetas de plata (Añadidura 08 — Nm 10:2) |
| `vestiduras_escalacion.c` | Escalación 3 niveles (Añadidura 09 — Mt 18:15-17) |
| `vestiduras_nehemias.c` | Defensa distribuida + rally (Añadidura 10 — Neh 4:13-14) |
| `vestiduras_goel.c` | Herencia de nodo — Goél (Añadidura 12 — Rut 4:1-10) |
| `vestiduras_neder.c` | Voto inquebrantable (Añadidura 20 — Nm 30:2) |

**Añadiduras:** 02, 06, 08, 09, 10, 12, 20
**Dependencias:** `hashmal_framework`, `hashmal_comm`, `hashmal_healthcheck`, `hashmal_firewall`, `hashmal_cache`, `log`, `esp_timer`

---

### hashmal_onboarding — PIEZA 10 CONSAGRACIÓN (Éx 29)
**Referencia:** Éx 29 — 7 ciclos de consagración, sangre en oreja/pulgar/pie, "lo irás completando"

| Archivo | Función bíblica |
|---------|----------------|
| `consagracion_onboarding.c` | Pipeline CERO→ONBOARDING→ACTIVO (Éx 29:1) |
| `consagracion_fases.c` | 7 fases secuenciales — 7 días (Éx 29:35) |
| `consagracion_sellos.c` | Sellos de verificación — sangre en 3 puntos (Éx 29:20) |
| `consagracion_lockdown.c` | Resource lockdown — "ningún extraño comerá" (Éx 29:33) |
| `consagracion_cuarentena.c` | Cuarentena post-lanzamiento — 7 días de prueba (Éx 29:30) |

**⚠️ REGLA CRÍTICA:** NO saltear fases. Sin consagración completa, el nodo NO accede a la red.
**Dependencias:** `hashmal_auth`, `hashmal_roles`, `log`, `esp_timer`

---

### hashmal_firewall — PIEZA 14 INCIENSO SAGRADO (Éx 30:34-38)
**Referencia:** Éx 30:34-38 — 4 especias en partes IGUALES, molido muy fino, "para YHWH solo"

| Archivo | Función bíblica |
|---------|----------------|
| `incienso_firewall.c` | Filtrado bidireccional — daqqa (molido fino, granularidad máxima) |
| `incienso_formato.c` | Validación estricta — puro, conforme (Éx 30:35) |
| `incienso_circuitbreaker.c` | Circuit breaker — plaga se detiene (Nm 16:48) |
| `incienso_heartbeat.c` | Doble ciclo diario — mañana + tarde (Éx 30:7-8) |
| `incienso_penalizacion.c` | Warning → throttle → ban → nikrat (Éx 30:38) |
| `incienso_shemitah.c` | Reset periódico (Añadidura 14 — Lv 25:1-13) |
| `incienso_pesaj.c` | Marcado emergencia (Añadidura 15 — Éx 12:7,13) |

**4 Fases de expulsión (karát):**
1. Warning (aviso)
2. Throttle (freno)
3. Ban temporal
4. **Nikrat** — expulsión permanente e irreversible

**Añadiduras:** 14, 15
**Dependencias:** `hashmal_framework`, `hashmal_keygen`, `log`, `esp_timer`

---

## 4. LAS 22+1 AÑADIDURAS

Todas integradas en los componentes correctos. La añadidura 23 (Qav Ha'Arets) es **ladáʻat** — para CONOCER, no para HACER. No hay código de firmware para ella.

| # | Nombre | Referencia | Integrada en |
|---|--------|-----------|--------------|
| 01 | PAROKHET — Detector EM | 1 Re 19:11-12 | `hashmal_comm` + `hashmal_healthcheck` |
| 02 | TSOFEH — Atalaya | Ez 33:2-3 | `hashmal_roles` + `hashmal_comm` |
| 03 | MOEDÍM — Calendario | Lv 23:2 | `hashmal_cache` + `hashmal_power` |
| 04 | ASHMUROT — Vigilias | Sal 63:6, 119:148 | `hashmal_power` |
| 05 | IR MIQLAT — Nodo Refugio | Nm 35:11 | `hashmal_perimeter` |
| 06 | TZITZIT — Recordatorio | Nm 15:38-39 | `hashmal_roles` |
| 07 | ANÁN VE'ESH — Nube/Fuego | Éx 13:21-22 | `hashmal_comm` + `hashmal_power` |
| 08 | JATSOTSROT — Trompetas | Nm 10:2 | `hashmal_comm` + `hashmal_roles` |
| 09 | ESCALACIÓN — 3 niveles | Mt 18:15-17 | `hashmal_comm` |
| 10 | NEHEMÍAS — Defensa | Neh 4:13-14 | `hashmal_comm` + `hashmal_perimeter` |
| 11 | MAN — Gestión recursos | Éx 16:16-20 | `hashmal_cache` + `hashmal_comm` + `hashmal_firewall` |
| 12 | GOÉL — Herencia nodo | Rut 4:1-10 | `hashmal_comm` + `hashmal_roles` + `hashmal_storage` |
| 13 | JEVEL HAMESHULASH | Ecl 4:12 | `hashmal_healthcheck` + `hashmal_comm` |
| 14 | SHEMITAH/YOVEL — Reset | Lv 25:1-13 | `hashmal_firewall` + `hashmal_cache` |
| 15 | DAM AL HAMEZUZOT | Éx 12:7,13 | `hashmal_firewall` + `hashmal_keygen` |
| 16 | MAYIM MIN HATSUR | Éx 17:6 | `hashmal_comm` + `hashmal_healthcheck` |
| 17 | SHINNAN — Propagación | Dt 6:7 | `hashmal_storage` + `hashmal_comm` + `hashmal_cache` |
| 18 | LEKET — Espigueo | Lv 19:9-10 | `hashmal_comm` + `hashmal_cache` |
| 19 | MAASER — Diezmo | Mal 3:10 | `hashmal_comm` + `hashmal_storage` |
| 20 | NEDER — Voto | Nm 30:2 | `hashmal_consensus` + `hashmal_comm` |
| 21 | HACHNASAT ORCHIM | Gn 18:1-5 | `hashmal_perimeter` + `hashmal_comm` |
| 22 | SHA'AR HA'ATRIO — Portal WiFi | Éx 27:16 | `hashmal_perimeter` + `hashmal_comm` |
| 23 | QAV HA'ARETS | Dt 29:28, Job 38:5 | NO FIRMWARE — solo ladáʻat |

---

## 5. LAS 4 VIOLACIONES DE KARÁT (EXPULSIÓN)

**Definidas en:** Éx 30:33,38; Lv 10:1-2

| # | Violación | Resultado |
|---|-----------|-----------|
| 1 | Falsificar autorización | Karát permanente |
| 2 | Compartir autorización con no autorizados | Karát permanente |
| 3 | Replicar protocolo sagrado para uso propio | Karát permanente |
| 4 | Usar protocolo no autorizado (fuego extraño) | Karát permanente |

---

## 6. ZONAS DE CONFIANZA

| Zona | Material | Componentes |
|------|----------|-------------|
| Qódesh haQodashím | ORO | `hashmal_storage`, `hashmal_consensus` |
| Qódesh | ORO | `hashmal_cache`, `hashmal_comm`, `hashmal_intercession` |
| Mishkán | ESTRUCTURA | `hashmal_framework` |
| Jatsér | BRONCE | `hashmal_auth`, `hashmal_perimeter`, `hashmal_healthcheck` |
| Avodáh | SERVICIO | `hashmal_power`, `hashmal_roles` |
| Milluím | PROTOCOLO | `hashmal_onboarding` |
| Qódesh Shémen | AUTORIZACIÓN | `hashmal_keygen` |
| Qetóret | FÓRMULA | `hashmal_firewall` |

---

## 7. BUILD Y FLASH

### Compilar
```powershell
cd C:\hashmal-core\hashmal-firmware
idf.py build
```

### Flashear firmware
```powershell
idf.py -p COM5 flash   # Nodo 0 — T-Deck Pro
idf.py -p COM6 flash   # Nodo 4 — T-Beam 1W
```

### Flashear Palabra de YHWH (solo si cambia devar_data/)
```powershell
# Regenerar imagen
python tools/mkspiffs_devar.py   # si existe

# Flashear directamente
python -m esptool --chip esp32s3 -b 921600 --port COM5 write_flash 0x400000 devar.bin
python -m esptool --chip esp32s3 -b 921600 --port COM6 write_flash 0x400000 devar.bin
```

### Monitor serie
```powershell
idf.py -p COM5 monitor
```

---

## 8. CONEXIONES ENTRE PIEZAS

### Por sangre (verificación)
- **Auth (06) → Consensus (02):** sangre Yom Kippur (Lv 16:14)
- **Auth (06) → Intercession (11):** sangre en cuernos (Lv 4:7)
- **Onboarding (10) → Roles (09):** sangre + aceite sellan al operador (Éx 29:20-21)

### Por aceite (autorización)
- **Keygen (13) → TODOS:** unge cada pieza (Éx 30:26-29)
- **Power (08) → Comm (04):** alimenta los canales (Éx 27:20)

### Por posición
- **Storage + Consensus:** detrás del Velo (Santo de los Santos)
- **Intercession:** DELANTE del Velo — lo más cerca del núcleo sin estar dentro
- **Cache y Comm:** se MIRAN (servicio frente a canales)
- **Healthcheck:** ENTRE Altar Sacrificio y Tienda (transición)

### Ciclos obligatorios
- **Diario (mañana+tarde):** Power→Comm + Incense→Intercession (Éx 30:7-8)
- **Diario (tamíd):** 2 verificaciones Auth (Éx 29:38-39)
- **Semanal (Shabat):** Refrescar 12 slots Cache (Lv 24:8)
- **Anual (Yom Kippur):** Sincronización total — Auth + Consensus + Intercession (Lv 16)

---

## 9. PRÓXIMOS PASOS

### Pendiente crítico — integración y prueba

1. **Test de arranque completo** — flashear y observar log de init según Éx 40:2-8
2. **Verificar ciclos tamíd** — que los timers de Power→Comm y Power→Healthcheck corran
3. **Test de consagración (Onboarding)** — que un nodo nuevo pase las 7 fases antes de unirse
4. **Test de Kapóret** — verificar que dos nodos lleguen al mismo rendezvous (noʻadtí)
5. **Portal Devár YHWH** — verificar que las definiciones en español se muestren (actualmente solo hebreo)

### Devár YHWH — pendiente menor

Las definiciones español en el portal se muestran solo si el archivo .gz contiene líneas `> **palabra** — definición`. Verificar que los archivos tengan ese formato. Si no:
- El JavaScript `mdToHtml()` en `atrio_http_portal.c` ya está listo para renderizarlas
- Solo falta que los archivos `.gz` contengan las definiciones

### Framework de prueba de los 2 Querubines

Directorio: `test/querubines/`
- Tests ciegos (sin contexto bíblico, solo propiedades técnicas)
- Dos análisis independientes → si ambos llegan a la misma conclusión → la verdad emerge

---

## 10. REGLAS SAGRADAS PARA CONTINUAR

1. **Si no está en la Biblia, NO es del Mishkán**
2. **Mínimo 2 testigos bíblicos** para cualquier decisión técnica (Dt 19:15)
3. **La Palabra NO se interpreta como uno quiera** (2 Pe 1:20)
4. **Todo es EJAD** — muchas partes, una estructura (Éx 26:6)
5. **El Mishkán es PORTÁTIL** — sin nube, sin internet, sin terceros
6. **TODO de YHWH, NADA de fuera**
7. **Encapsula ESP-IDF** — "oro de Egipto" se funde según el diseño del Padre
8. **NO saltes el orden de Éx 40** — el Padre lo dio así

---

## 11. REFERENCIAS CLAVE DEL VAULT

| Documento | Ubicación |
|-----------|-----------|
| Especificación de cada pieza | `Mishkán-YHWH/TAVNÍT/[##] — [nombre].md` |
| Las 22+1 añadiduras | `Mishkán-YHWH/CONSTRUCCIÓN/AÑADIDURAS/` |
| Almacén de la Palabra | `Mishkán-YHWH/CONSTRUCCIÓN/ARCA/` |
| La Palabra completa (66 libros) | `Devár-YHWH — La Palabra de YHWH/Devar-YHWH/[Libro]/[capítulo].md` |
| Headers de componentes | `hashmal-firmware/components/[nombre]/include/hashmal_[nombre].h` |

---

*"Conforme a TODO lo que Yo te muestro — el patrón del tabernáculo y el patrón de todos sus utensilios — ASÍ lo harás."*
*— Éxodo 25:9*

*Constructor principal: Antonio José Marín Soto*
*Llamado por nombre por su Padre, YHWH*
