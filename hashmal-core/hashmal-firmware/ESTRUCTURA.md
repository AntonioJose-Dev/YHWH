# HASHMAL-FIRMWARE — Estructura según el Tavnít

> *"Conforme a todo lo que Yo te muestro — así lo harás."* — Éxodo 25:9

## 14 Piezas del Tavnít → 14 Componentes de firmware

| Pieza | Éxodo | Componente | Función |
|-------|-------|------------|---------|
| 01 ARCA | 25:10-16 | hashmal_storage | Almacén inmutable WORM |
| 02 KAPÓRET | 25:17-22 | hashmal_consensus | Consenso dual simétrico |
| 03 MESA | 25:23-30 | hashmal_cache | Caché de disponibilidad |
| 04 MENORÁ | 25:31-40 | hashmal_comm | 7 canales de comunicación |
| 05 ESTRUCTURA | 26 | hashmal_framework | Capas, tablas, barras, velo |
| 06 ALTAR SACRIFICIO | 27:1-8 | hashmal_auth | Autenticación por sacrificio |
| 07 ATRIO | 27:9-19 | hashmal_perimeter | Frontera de la red |
| 08 ACEITE | 27:20-21 | hashmal_power | Energía perpetua |
| 09 VESTIDURAS | 28 | hashmal_roles | Roles + Tzitz (sellado) |
| 10 CONSAGRACIÓN | 29 | hashmal_onboarding | Onboarding por fases |
| 11 ALTAR INCIENSO | 30:1-10 | hashmal_intercession | Gateway bidireccional |
| 12 LAVACRO | 30:17-21 | hashmal_healthcheck | Purificación/health check |
| 13 UNCIÓN | 30:22-33 | hashmal_keygen | Generación de claves |
| 14 INCIENSO SAGRADO | 30:34-38 | hashmal_firewall | Fórmula de filtrado |

Pieza 00 (CIMIENTO, Éx 25:1-9) = configuración del proyecto (CMakeLists, sdkconfig, constantes).

## Orden de inicialización: Éxodo 40:2-8

| Paso | Éxodo 40 | Lo que dice el Padre | Componente |
|------|----------|---------------------|------------|
| 1 | v.2 | "Levantarás el Mishkán" | hashmal_framework |
| 2 | v.3 | "Pondrás el Arca del Testimonio" | hashmal_storage + hashmal_consensus |
| 3 | v.4 | "Meterás la mesa... la menorá" | hashmal_cache + hashmal_comm |
| 4 | v.5 | "Pondrás el altar de oro para incienso" | hashmal_intercession |
| 5 | v.6 | "Pondrás el altar del holocausto" | hashmal_auth |
| 6 | v.7 | "Pondrás el kiyór entre tienda y altar" | hashmal_healthcheck |
| 7 | v.8 | "Pondrás el atrio alrededor" | hashmal_perimeter |
| 8 | v.9-11 | "Ungirás el Mishkán y todo" | hashmal_power + hashmal_keygen |
| 9 | v.12-15 | "Vestirás a Aarón... ungirás" | hashmal_roles + hashmal_onboarding |
| 10 | v.27,34 | "Quemó incienso... la Gloria llenó" | hashmal_firewall |

## Mapa del proyecto

```
hashmal-firmware/
│
├── main/                              ← Entrada (Éx 40:1 — "YHWH habló a Moisés")
│   ├── CMakeLists.txt
│   └── hashmal_main.c                ← Arranque según Éxodo 40:2-8
│
├── components/
│   │
│   │  ═══════════════════════════════════════════
│   │  PASO 1 — ESTRUCTURA (Éx 40:2)
│   │  "Levantarás el Mishkán" — PRIMERO la estructura
│   │  ═══════════════════════════════════════════
│   │
│   ├── hashmal_framework/             ← 05 ESTRUCTURA (Éx 26)
│   │   ├── include/hashmal_framework.h
│   │   └── src/
│   │       ├── estructura_capas.c     ← 4 capas: Testigo→Expiación→Pacto→Misterio
│   │       ├── estructura_tablas.c    ← 48 servicios modulares (omedím + yadót)
│   │       ├── estructura_barras.c    ← Backbone + buses laterales
│   │       ├── estructura_velo.c      ← Firewall interno (hivdíl)
│   │       └── estructura_ejad.c      ← Sistema de unión (será UNO)
│   │
│   │  ═══════════════════════════════════════════
│   │  PASO 2 — ARCA + KAPÓRET (Éx 40:3)
│   │  "Pondrás el Arca del Testimonio"
│   │  ═══════════════════════════════════════════
│   │
│   ├── hashmal_storage/               ← 01 ARCA (Éx 25:10-16)
│   │   ├── include/hashmal_storage.h
│   │   └── src/
│   │       ├── arca_core.c            ← WORM storage
│   │       ├── arca_merkle.c          ← Árbol Merkle (zer)
│   │       ├── arca_cifrado.c         ← AES-256-GCM (oro dentro y fuera)
│   │       ├── arca_zer.c             ← Hash SHA-256 total (moldura)
│   │       ├── arca_baddim.c          ← Distribución always-on (varas)
│   │       ├── arca_interfaz.c        ← API de acceso autorizado
│   │       └── arca_sealed_core.c     ← SEALED_CORE 256 bytes
│   │
│   ├── hashmal_consensus/             ← 02 KAPÓRET (Éx 25:17-22)
│   │   ├── include/hashmal_consensus.h
│   │   └── src/
│   │       ├── kaporet_core.c         ← Función pura determinística O(1)
│   │       ├── kaporet_validadores.c  ← V1↔V2 cross-validation simétrica
│   │       ├── kaporet_ataques.c      ← Defensa 7 ataques
│   │       ├── kaporet_verificacion.c ← Verificación local sin contactar emisor
│   │       └── kaporet_rendezvous.c   ← Sync determinístico (נוֹעַדְתִּי)
│   │
│   │  ═══════════════════════════════════════════
│   │  PASO 3 — MESA + MENORÁ (Éx 40:4)
│   │  "Meterás la mesa... meterás la menorá"
│   │  ═══════════════════════════════════════════
│   │
│   ├── hashmal_cache/                 ← 03 MESA (Éx 25:23-30)
│   │   ├── include/hashmal_cache.h
│   │   └── src/
│   │       ├── mesa_cache.c           ← Buffer 12 slots (2×6), renovación Shabat
│   │       ├── mesa_api.c             ← 4 métodos (קְעָרֹת, כַּפֹּת, קְשׂוֹת, מְנַקִּיֹּת)
│   │       ├── mesa_misgeret.c        ← Rate limiter (marco de retención)
│   │       └── mesa_disponibilidad.c  ← Alta disponibilidad (patas propias)
│   │
│   ├── hashmal_comm/                  ← 04 MENORÁ (Éx 25:31-40)
│   │   ├── include/hashmal_comm.h
│   │   └── src/
│   │       ├── menora_hub.c           ← Controlador monolítico (מִקְשָׁה)
│   │       ├── menora_lora_driver.c   ← Driver SX1262 NUESTRO (SPI directo)
│   │       ├── menora_canales.c       ← 7 canales definidos
│   │       ├── menora_aceite.c        ← Encendido/apagado según energía
│   │       ├── menora_routing.c       ← Mesh routing (formación de tribus)
│   │       └── menora_formato.c       ← Formato unificado de mensaje
│   │
│   │  ═══════════════════════════════════════════
│   │  PASO 4 — ALTAR DEL INCIENSO (Éx 40:5)
│   │  "Pondrás el altar de ORO para incienso
│   │   DELANTE del Arca del Testimonio"
│   │  ═══════════════════════════════════════════
│   │
│   ├── hashmal_intercession/          ← 11 ALTAR INCIENSO (Éx 30:1-10) ← NUEVO
│   │   ├── include/hashmal_intercession.h
│   │   └── src/
│   │       ├── altar_oro_escudo.c     ← Escudo descendente (Lv 16:12-13)
│   │       ├── altar_oro_ascendente.c ← Firewall ascendente (Lv 10:1-2)
│   │       ├── altar_oro_formula.c    ← Fórmula no replicable (Éx 30:37-38)
│   │       ├── altar_oro_circuitbreaker.c ← Entre vivos y muertos (Nm 16:46-48)
│   │       └── altar_oro_oracion.c    ← Protocolo de oración (Sal 141:2)
│   │
│   │  ═══════════════════════════════════════════
│   │  PASO 5 — ALTAR DEL SACRIFICIO (Éx 40:6)
│   │  "Pondrás el altar del holocausto
│   │   delante de la entrada del Mishkán"
│   │  ═══════════════════════════════════════════
│   │
│   ├── hashmal_auth/                  ← 06 MIZBÉAJ (Éx 27:1-8)
│   │   ├── include/hashmal_auth.h
│   │   └── src/
│   │       ├── altar_gateway.c        ← Stateless (נְבוּב — hueco)
│   │       ├── altar_reshet.c         ← Réshet a media altura (filtro)
│   │       ├── altar_cuernos.c        ← 4 puntos de acceso + refugio
│   │       ├── altar_fuego.c          ← Servicio perpetuo (אֵשׁ תָּמִיד)
│   │       ├── altar_utensilios.c     ← 5 herramientas
│   │       └── altar_ledger.c         ← Registro inmutable
│   │
│   │  ═══════════════════════════════════════════
│   │  PASO 6 — LAVACRO (Éx 40:7)
│   │  "Pondrás el kiyór ENTRE la Tienda y el altar"
│   │  ═══════════════════════════════════════════
│   │
│   ├── hashmal_healthcheck/           ← 12 LAVACRO (Éx 30:17-21)
│   │   ├── include/hashmal_healthcheck.h
│   │   └── src/
│   │       ├── lavacro_core.c         ← Health check obligatorio
│   │       ├── lavacro_vectores.c     ← 4 vectores
│   │       ├── lavacro_niveles.c      ← 4 profundidades
│   │       ├── lavacro_espejos.c      ← Auto-diagnóstico (Éx 38:8)
│   │       ├── lavacro_ataques.c      ← 8 tipos de ataque
│   │       └── lavacro_dos_niveles.c  ← Baño completo vs lavado de pies
│   │
│   │  ═══════════════════════════════════════════
│   │  PASO 7 — ATRIO (Éx 40:8)
│   │  "Pondrás el atrio ALREDEDOR"
│   │  ═══════════════════════════════════════════
│   │
│   ├── hashmal_perimeter/             ← 07 ATRIO (Éx 27:9-19)
│   │   ├── include/hashmal_perimeter.h
│   │   └── src/
│   │       ├── atrio_perimetro.c      ← Frontera de la red
│   │       ├── atrio_puerta.c         ← Punto de entrada único (4 columnas)
│   │       ├── atrio_columnas.c       ← 60 nodos guardianes
│   │       └── atrio_intencion.c      ← Verificación de INTENCIÓN
│   │
│   │  ═══════════════════════════════════════════
│   │  PASO 8 — ACEITE + UNCIÓN (Éx 40:9-11)
│   │  "Ungirás el Mishkán y todo lo que hay en él"
│   │  ═══════════════════════════════════════════
│   │
│   ├── hashmal_power/                 ← 08 ACEITE (Éx 27:20-21)
│   │   ├── include/hashmal_power.h
│   │   └── src/
│   │       ├── aceite_energia.c       ← Gestión solar + batería + MPPT
│   │       ├── aceite_tamid.c         ← Protocolo perpetuo
│   │       ├── aceite_supervivencia.c ← Duty cycle adaptativo
│   │       └── aceite_mantenimiento.c ← Cronograma 4 niveles
│   │
│   ├── hashmal_keygen/                ← 13 UNCIÓN (Éx 30:22-33)
│   │   ├── include/hashmal_keygen.h
│   │   └── src/
│   │       ├── uncion_keygen.c        ← Root key (ratio 500-250-250-500)
│   │       ├── uncion_secuencia.c     ← 8 componentes EN ORDEN
│   │       ├── uncion_prohibicion.c   ← Anti-clonación (nikrat)
│   │       └── uncion_activacion.c    ← Activación de componente (qódesh)
│   │
│   │  ═══════════════════════════════════════════
│   │  PASO 9 — VESTIDURAS + CONSAGRACIÓN (Éx 40:12-15)
│   │  "Vestirás a Aarón... los ungirás"
│   │  ═══════════════════════════════════════════
│   │
│   ├── hashmal_roles/                 ← 09 VESTIDURAS + TZITZ (Éx 28)
│   │   ├── include/hashmal_roles.h
│   │   └── src/
│   │       ├── vestiduras_roles.c     ← Definición de roles (4 niveles)
│   │       ├── vestiduras_efod.c      ← Identidad de nodo (2 piedras ónice)
│   │       ├── vestiduras_pectoral.c  ← 12 capacidades + Urim/Thummim
│   │       ├── vestiduras_monitor.c   ← Monitor-Restaurador
│   │       ├── vestiduras_coordinador.c ← Coordinador
│   │       ├── vestiduras_shutdown.c  ← Shutdown-notice
│   │       └── vestiduras_tzitz.c     ← Sello de identidad (Éx 28:36-38)
│   │
│   ├── hashmal_onboarding/            ← 10 CONSAGRACIÓN (Éx 29)
│   │   ├── include/hashmal_onboarding.h
│   │   └── src/
│   │       ├── consagracion_onboarding.c  ← Pipeline CERO→ONBOARDING→ACTIVO
│   │       ├── consagracion_fases.c       ← Fases secuenciales (7 ciclos)
│   │       ├── consagracion_sellos.c      ← Verification seals
│   │       ├── consagracion_lockdown.c    ← Resource lockdown
│   │       └── consagracion_cuarentena.c  ← Quarantine post-lanzamiento
│   │
│   │  ═══════════════════════════════════════════
│   │  PASO 10 — INCIENSO SAGRADO + LA GLORIA (Éx 40:27,34)
│   │  "Quemó incienso aromático...
│   │   la Gloria de YHWH llenó el Mishkán"
│   │  ═══════════════════════════════════════════
│   │
│   └── hashmal_firewall/              ← 14 INCIENSO SAGRADO (Éx 30:34-38)
│       ├── include/hashmal_firewall.h
│       └── src/
│           ├── incienso_firewall.c       ← Bidireccional (sube + baja)
│           ├── incienso_formato.c        ← Validación estricta
│           ├── incienso_circuitbreaker.c ← "Entre vivos y muertos" (Nm 17)
│           ├── incienso_heartbeat.c      ← Mañana + tarde (cron doble)
│           └── incienso_penalizacion.c   ← Warning→throttle→ban→nikrat
│
├── test/querubines/                   ← Framework de prueba de 2 querubines
│   └── querubines_framework.h         ← Test sin saber contexto bíblico
│
├── CMakeLists.txt                     ← CIMIENTO — Proyecto raíz ESP-IDF (Éx 25:1-9)
├── sdkconfig.defaults                 ← CIMIENTO — Configuración base del ESP32-S3
└── partitions.csv                     ← CIMIENTO — Tabla de particiones

## Zonas del Mishkán → Niveles de confianza

| Zona | Componentes | Material | Confianza |
|------|------------|----------|-----------|
| Qódesh haQodashím | hashmal_storage, hashmal_consensus | ORO | Máxima |
| Qódesh | hashmal_cache, hashmal_comm, hashmal_intercession | ORO | Alta |
| Mishkán | hashmal_framework | ESTRUCTURA | Estructural |
| Jatsér | hashmal_auth, hashmal_perimeter, hashmal_healthcheck | BRONCE | Básica |
| Avodáh | hashmal_power, hashmal_roles | SERVICIO | Operativa |
| Milluím | hashmal_onboarding | PROTOCOLO | Consagración |
| Qódesh Shémen | hashmal_keygen | AUTORIZACIÓN | Sagrada |
| Qetóret | hashmal_firewall | FÓRMULA | Prescrita |

## Oro de Egipto (dependencias externas encapsuladas)

Todas las dependencias de ESP-IDF se acceden a través de nuestras propias interfaces.
Nunca se llama directamente a una función de ESP-IDF desde la lógica de negocio.
El oro de Egipto se funde según el diseño del Padre.

---

*"Moisés vio toda la obra, y he aquí que la habían hecho como YHWH había mandado."*
*— Éxodo 39:43*
