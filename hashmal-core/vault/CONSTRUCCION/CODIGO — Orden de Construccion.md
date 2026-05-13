# HASHMAL-CORE — Estado Real del Proyecto
# Actualizado: 9 de mayo de 2026

> *"Moises vio toda la obra, y he aqui que la habian hecho como YHWH habia mandado.
> Y Moises los bendijo."*
> — Exodo 39:43

---

## ESTADO GLOBAL

| Metrica | Valor |
|---------|-------|
| Archivos .c totales | **111** (108 en componentes + 3 en main) |
| Componentes completados | **14 / 14** |
| Firmware flasheado | **Si** — T-Deck Pro (COM5) + T-Beam 1W (COM6) |
| Hardware operativo | **Si** — LoRa, WiFi, EPD, GPS, I2C todos inicializados |
| Ultima compilacion | idf.py build — OK |

---

## DIRECTORIO UNICO

Todo el proyecto vive en un solo lugar:

```
C:\hashmal-core\
├── hashmal-firmware\     <- FIRMWARE (codigo C, ESP-IDF)
│   ├── main\             <- Punto de entrada (Ex 40:2-8)
│   ├── components\       <- 14 piezas del Tavnit
│   ├── managed_components\
│   ├── partitions.csv
│   ├── sdkconfig.defaults
│   ├── HASHMAL-GUIA.md
│   └── ESTRUCTURA.md
└── vault\                <- DOCUMENTACION (Tavnit, Verificados, Anadiduras)
    ├── TAVNIT\           <- 18 archivos (piezas 00-16)
    └── CONSTRUCCION\     <- Algoritmos, pruebas, verificados
```

Flashear: `idf.py -p COM5 flash` (Nodo 0 T-Deck Pro) | `idf.py -p COM6 flash` (Nodo 4 T-Beam)

---

## INVENTARIO COMPLETO — LAS 14 PIEZAS

### Paso 1 — hashmal_framework — ESTRUCTURA (Ex 26) [COMPLETO - 5 archivos]
- estructura_barras.c — 5 barras + backbone central
- estructura_capas.c — 4 capas: Lino/Cabra/Carnero/Tajash
- estructura_ejad.c — union ejad (Ex 26:6), P2P por afinidad
- estructura_tablas.c — 48 servicios, base doble de plata
- estructura_velo.c — kernel boundary hivdil (separar)

### Paso 2a — hashmal_storage — ARCA (Ex 25:10-16) [COMPLETO - 7 archivos]
- arca_core.c — nucleo WORM, almacen inmutable
- arca_sealed_core.c — 256 bytes en flash interna (el Testimonio)
- arca_cifrado.c — AES-256-GCM en reposo y en transito
- arca_merkle.c — hash de integridad (zer zahav)
- arca_baddim.c — varas always-on, distribucion permanente
- arca_zer.c — corona de hash superior
- arca_interfaz.c — API publica del almacen

### Paso 2b — hashmal_consensus — KAPORET (Ex 25:17-22) [COMPLETO - 6 archivos]
- kaporet_core.c — funcion pura deterministica (miqsha)
- kaporet_validadores.c — V1 y V2, dos querubines independientes
- kaporet_verificacion.c — cross-validation simetrica O(1)
- kaporet_relay.c — propagacion de consenso
- kaporet_rendezvous.c — punto de encuentro deterministico (noádti)
- kaporet_ataques.c — deteccion y penalizacion de ataques

### Paso 3a — hashmal_cache — MESA (Ex 25:23-30) [COMPLETO - 9 archivos]
- mesa_cache.c — 12 slots de cache (12 panes del Shuljan)
- mesa_api.c — 4 operaciones CRUD (4 patas de la Mesa)
- mesa_misgeret.c — rate limiter (misgeret = moldura/corona)
- mesa_disponibilidad.c — disponibilidad de la Palabra
- mesa_moedim.c — Calendario: epocas Shabat y Moedim
- mesa_man.c — gestion de recursos (un omer por cabeza, Ex 16)
- mesa_shinnan.c — propagacion de conocimiento (Dt 6:7)
- mesa_leket.c — recursos para debiles (Lv 19:9-10)
- mesa_maaser.c — contribucion a la red / diezmo (Mal 3:10)

### Paso 3b — hashmal_comm — MENORA (Ex 25:31-40) [COMPLETO - 11 archivos]
- menora_hub.c — singleton Menora, 7 canales, monolitico (miqsha)
- menora_lora_driver.c — driver SX1262 SPI directo (1018 lineas)
- menora_espnow_driver.c — driver ESP-NOW (isha el ajota)
- menora_ble_paamon.c — driver BLE (paamon = campanilla)
- menora_mutsaqot.c — selector multi-transporte (Zac 4:6 beruji)
- menora_canales.c — 7 canales LoRa (1 tronco + 6 ramas)
- menora_routing.c — mesh routing entre nodos
- menora_aceite.c — sincronizacion con aceite perpetuo (Ex 27:20)
- menora_formato.c — serializacion/deserializacion de mensajes
- menora_parokhet.c — detector EM (Anadidura 01, 1 Re 19:11-12)
- menora_tsofeh.c — modo atalaya/watchdog (Anadidura 02, Ez 33:2)

### Paso 4 — hashmal_intercession — ALTAR INCIENSO (Ex 30:1-10) [COMPLETO - 5 archivos]
- altar_oro_escudo.c — escudo descendente (Lv 16:12-13)
- altar_oro_formula.c — formula no replicable (Ex 30:37-38)
- altar_oro_circuitbreaker.c — circuit breaker (Nm 16:46-48)
- altar_oro_oracion.c — protocolo de comunicacion (Sal 141:2)
- altar_oro_ascendente.c — firewall ascendente (Lv 10:1-2)

### Paso 5 — hashmal_auth — ALTAR SACRIFICIO (Ex 27:1-8) [COMPLETO - 6 archivos]
- altar_reshet.c — auth por prueba de trabajo (reshet = red de bronce)
- altar_gateway.c — gateway de entrada/salida
- altar_cuernos.c — 4 cuernos, control de acceso perimetral
- altar_utensilios.c — 5 herramientas stateless (Ex 27:3)
- altar_ledger.c — registro inmutable de autorizaciones
- altar_fuego.c — fuego perpetuo (Lv 6:12-13)

### Paso 6 — hashmal_healthcheck — LAVACRO (Ex 30:17-21) [COMPLETO - 12 archivos]
- lavacro_core.c — health check obligatorio
- lavacro_vectores.c — 4 vectores de verificacion
- lavacro_espejos.c — autodiagnostico (espejos, Ex 38:8)
- lavacro_ataques.c — deteccion de ataques
- lavacro_challenge.c — retos de autenticidad
- lavacro_niveles.c — niveles de alerta
- lavacro_dos_niveles.c — verificacion en dos niveles
- lavacro_parokhet.c — modo silencio EM (Anadidura 01)
- lavacro_meshulash.c — cordon triple redundancia (Anadidura 13)
- lavacro_meshulash_sajar.c — propagacion en cordon triple
- lavacro_tsur.c — ultimo recurso (mayim min hatsur, Anadidura 16)
- lavacro_joq_olam.c — leyes permanentes del sistema

### Paso 7 — hashmal_perimeter — ATRIO (Ex 27:9-19) [COMPLETO - 11 archivos]
- atrio_perimetro.c — frontera, 60 columnas
- atrio_puerta.c — puerta unica de entrada (4 columnas, Ex 27:16)
- atrio_columnas.c — gestion de columnas del perimetro
- atrio_intencion.c — verificacion de intencion
- atrio_miqlat.c — nodo refugio (Anadidura 05, Nm 35:11)
- atrio_orchim.c — hospitalidad (Anadidura 21, Gn 18:1-5)
- atrio_shaar.c — portal WiFi cautivo (Anadidura 22, Ex 27:16)
- atrio_http_portal.c — servidor HTTP embebido
- atrio_wifi_ap.c — AP WiFi "HASHMAL" (192.168.4.1)
- atrio_dns_cautivo.c — DNS cautivo para redireccion al portal
- atrio_puente.c — bridge WiFi <-> LoRa

### Paso 8a — hashmal_power — ACEITE PERPETUO (Ex 27:20-21) [COMPLETO - 6 archivos]
- aceite_energia.c — gestion energia solar + bateria
- aceite_tamid.c — ciclos tamid (perpetuo, manana y tarde)
- aceite_ashmurot.c — vigilias nocturnas (Anadidura 04, Sal 63:6)
- aceite_anan_esh.c — nube y fuego (Anadidura 07, Ex 13:21-22)
- aceite_supervivencia.c — modo supervivencia de emergencia
- aceite_mantenimiento.c — mantenimiento del sistema energetico

### Paso 8b — hashmal_keygen — UNCION (Ex 30:22-33) [COMPLETO - 4 archivos]
- uncion_keygen.c — generacion root key (ratio 500-250-250-500)
- uncion_secuencia.c — secuencia de uncion de 8 componentes
- uncion_activacion.c — activacion de nodo
- uncion_prohibicion.c — anti-clonacion (nikrat, Ex 30:33)

### Paso 9a — hashmal_roles — VESTIDURAS (Ex 28) [COMPLETO - 14 archivos]
- vestiduras_roles.c — gestion de roles (Efod, Pectoral, etc.)
- vestiduras_efod.c — efod de 6 materiales (Ex 28:6)
- vestiduras_pectoral.c — 12 piedras, Urim y Tumim
- vestiduras_tzitz.c — sello Tzitz "Qodesh la-YHWH"
- vestiduras_coordinador.c — coordinacion entre roles
- vestiduras_monitor.c — monitoreo de estado de roles
- vestiduras_shutdown.c — protocolo de baja de rol
- vestiduras_tzitzit.c — recordatorio visual (Anadidura 06, Nm 15:38)
- vestiduras_jatsotsrot.c — trompetas de plata (Anadidura 08, Nm 10:2)
- vestiduras_escalacion.c — escalacion 3 niveles (Anadidura 09, Mt 18:15)
- vestiduras_nehemias.c — defensa distribuida (Anadidura 10, Neh 4:13)
- vestiduras_goel.c — herencia de nodo (Anadidura 12, Rt 4:1)
- vestiduras_neder.c — voto inquebrantable (Anadidura 20, Nm 30:2)
- vestiduras_tsofeh.c — atalaya en roles (Anadidura 02)

### Paso 9b — hashmal_onboarding — CONSAGRACION (Ex 29) [COMPLETO - 5 archivos]
- consagracion_onboarding.c — pipeline CERO->ONBOARDING->ACTIVO
- consagracion_fases.c — 7 ciclos de consagracion (Ex 29:35)
- consagracion_cuarentena.c — periodo de cuarentena (7 dias)
- consagracion_sellos.c — sellos de autorizacion por fase
- consagracion_lockdown.c — bloqueo de nodo no consagrado

### Paso 10 — hashmal_firewall — INCIENSO SAGRADO (Ex 30:34-38) [COMPLETO - 7 archivos]
- incienso_firewall.c — 4 reglas equitativas (bad bebad, Ex 30:34)
- incienso_formato.c — granularidad maxima (memullaj)
- incienso_circuitbreaker.c — circuit breaker de red
- incienso_heartbeat.c — latido del sistema (tamid)
- incienso_penalizacion.c — Warning -> throttle -> ban -> nikrat
- incienso_shemitah.c — reset periodico (Anadidura 14, Lv 25:1)
- incienso_pesaj.c — marcado de nodos seguros (Anadidura 15, Ex 12:7)

### main\ — Punto de Entrada [COMPLETO - 3 archivos]
- hashmal_main.c — orquestador Ex 40:2-8 (INIT->TRANSICION->LISTEN)
- hashmal_epd.c — driver e-paper GDEQ031T10 (UI del T-Deck)
- hashmal_menu.c — menu en pantalla

---

## HARDWARE OPERATIVO

| Hardware | Estado | Puerto |
|----------|--------|--------|
| T-Deck Pro ESP32-S3 | Flasheado | COM5 (Nodo 0) |
| T-Beam 1W ESP32-S3 | Flasheado | COM6 (Nodo 4) |
| SX1262 LoRa | Transmitiendo | SPI propio |
| Portal WiFi HASHMAL | Activo | 192.168.4.1 |
| Devar YHWH (66 libros) | En SPIFFS | particion devar 8MB |
| Pantalla e-paper EPD | Activa | SPI compartido |
| GPS | Inicializado | UART |
| Teclado TCA8418 | Activo | I2C 0x34 |
| Touch CST328 | Activo | I2C 0x1A |

Particiones: nvs(0x9000/24K) | factory(0x10000/3MB) | storage(0x310000/960K) | devar(0x400000/8MB)

---

## QUE FUNCIONA HOY

- Red LoRa — mensajes entre nodos por SX1262
- Portal WiFi "HASHMAL" — 192.168.4.1, pass: yisrael1
- Bridge WiFi <-> LoRa — mensajes que entran por WiFi salen por LoRa y viceversa
- Devar YHWH — 66 libros accesibles desde el portal
- Moedim — calendario biblico activo
- UI e-paper en T-Deck Pro
- Orden de inicializacion Ex 40:2-8 ejecutandose en boot
- Menora 7 canales: LoRa (tronco), ESP-NOW, BLE operativos

---

## PENDIENTE — LO QUE QUEDA

### Alta prioridad
| # | Tarea | Componente |
|---|-------|-----------|
| 1 | Test Kapóret entre dos nodos — verificar consenso real en hardware | hashmal_consensus |
| 2 | Definiciones en espanol en Devar — el portal muestra hebreo, faltan definiciones ES | hashmal_cache |
| 3 | Procedimiento de onboarding — como anadir un tercer nodo a la red | hashmal_onboarding |

### Media prioridad
| # | Tarea |
|---|-------|
| 4 | Verificar 22 anadiduras integradas en los headers (no solo en .c) |
| 5 | Test de portal WiFi cautivo — redireccion DNS |
| 6 | Calibracion bateria: BAT_RAW_MIN=1980, BAT_RAW_MAX=2770 |
| 7 | Test de Parokhet (detector EM) — Anadidura 01 |
| 8 | Test de Moedim — verificar que el calendario activa ciclos correctamente |

### Cuando ambos nodos esten probados
| # | Tarea |
|---|-------|
| 9 | Anadir tercer nodo a la red |
| 10 | Test de routing mesh (3+ nodos) |
| 11 | Prueba de Yom Kippur — sincronizacion total Kapóret |

---

## PROXIMOS PASOS INMEDIATOS

```
Paso A: Compilar
  cd C:\hashmal-core\hashmal-firmware
  idf.py build

Paso B: Flashear
  idf.py -p COM5 flash   (Nodo 0 — T-Deck Pro)
  idf.py -p COM6 flash   (Nodo 4 — T-Beam 1W)

Paso C: Monitor
  idf.py -p COM5 monitor
  idf.py -p COM6 monitor

Paso D: Test LoRa <-> WiFi
  Conectar al WiFi "HASHMAL" -> 192.168.4.1 -> Mensajes
  Verificar que Nodo 4 recibe

Paso E: Test Kapóret
  Nodo 0 propone -> Nodo 4 valida
  La verdad emerge de entre los dos querubines
```

---

## RESUMEN EJECUTIVO

El Mishkan digital esta construido y en pie. Las 14 piezas del Tavnit estan
implementadas en 111 archivos .c. El firmware corre en dos nodos fisicos.
LoRa transmite, el portal WiFi sirve la Palabra de YHWH, el bridge funciona.

Lo que queda no es construccion — es prueba y consagracion. El Kaporet
(consenso entre dos querubines cara a cara) esta escrito pero aun no ha sido
probado con los dos nodos reales simultaneamente. Ese es el siguiente paso.

"Asi fue acabada toda la obra del Mishkan de la Tienda de Reunion."
— Exodo 39:32

Actualizado: 9 mayo 2026
Antonio Jose Marin Soto
