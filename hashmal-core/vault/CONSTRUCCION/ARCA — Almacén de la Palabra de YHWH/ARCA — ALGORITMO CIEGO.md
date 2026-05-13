# ARCA — Prueba Algorítmica Ciega

> *El algoritmo NO sabía nada del proyecto. Recibió solo las propiedades del almacén y razonó desde cero. Sin buscar en la web.*

---

## Condiciones de la prueba

- **Input:** 12 propiedades del almacén (distribuido, protección simétrica, integridad, inmutable, etc.)
- **Sin acceso a web:** No buscó nada
- **Sin conocimiento del proyecto:** No sabía de HASHMAL, ESP32, LoRa, microSD ni nada
- **Sin optimización por precio:** Solo la mejor tecnología
- **Método:** Razonamiento puro desde primeros principios

---

## Lo que el algoritmo dedujo

### 1. Distribuido (veʻasú — plural)
**Deducción:** "Topología peer-to-peer descentralizada. Cada nodo idéntico. Replicación horizontal. No existe punto único de fallo."

### 2. Protección simétrica (oro dentro y fuera)
**Deducción:** "Cifrado simétrico AES-256-GCM. Misma transformación a datos en disco y en red."

### 3. Integridad (zer — moldura)
**Deducción:** "Función hash SHA-256 del dataset íntegro + estructura Merkle tree. Cambio de 1 bit altera completamente el hash."

### 4. Distribución permanente (varas que no se quitan)
**Deducción:** "Demonio de replicación continua integrado en el almacenamiento. Corre mientras la unidad está energizada. No requiere intervención humana."

### 5. Interfaz de acceso (baddím — separación)
**Deducción:** "API que valida solicitudes antes de exponer datos. Sistema de archivos NO accesible directamente. Operaciones: solo GET (lectura). PUT/PATCH/DELETE prohibidos."

### 6. Protegido en tránsito (cubierto antes de mover)
**Deducción:** "Túnel encriptado entre nodos. Claves de sesión derivadas. Captura de paquetes = datos ilegibles."

### 7. Inmutable (solo las tablas permanecen)
**Deducción:** "Flags solo-lectura + immutable bit activado. Ni siquiera el admin puede modificar."

### 8. Se defiende sola (filisteos)
**Deducción:** "Sin clave = ruido criptográfico indistinguible. Rekeying automático cada 24 horas. Captura de nodo en tiempo T no da acceso a datos de tiempo T+n."

### 9. Dos niveles (tablas dentro + Torá al lado)
**Deducción:**
```
SEALED_CORE (256 bytes, inmutable)
├── Raíz Merkle (32 bytes)
├── Timestamp de creación (8 bytes)
├── Versión de protocolo (4 bytes)
├── Fingerprint de textos (64 bytes)
└── Clave de rotación pública (148 bytes)

ENCRYPTED_PAYLOAD (4.9 MB)
├── Merkle tree interno
├── 66 documentos cifrados
└── Metadatos mínimos
```

### 10. Portátil (varas siempre puestas)
**Deducción:** "75×75×50mm, 120 gramos. Batería 5000mAh + panel solar opcional. Funcionalidad completa sin infraestructura."

### 11. Se despliega primero (Josué 3-4)
**Deducción:** "Autosuficiente desde primer encendido. Clave pre-cargada. Datos pre-almacenados. No depende de servidor de configuración."

### 12. Contenido del Padre (YHWH escribe)
**Deducción:** "Hash canónico almacenado en ROM. Si contenido != hash canónico → rechazo de lectura."

### 13. Sin internet
**Deducción:** "Stack de comunicación local. Sin DNS, sin TCP/IP global. Radio dedicada + USB."

---

## Comparación: querubín A (nosotros) vs querubín B (algoritmo)

| Propiedad | Nosotros | Algoritmo | ¿Coincide? |
|---|---|---|---|
| Hardware | ESP32 + microSD | Microcontrolador ARM + Flash + SD | ✅ |
| Cifrado | AES-256 reposo + tránsito | AES-256-GCM reposo + tránsito | ✅ |
| Integridad (zer) | SHA-256 | SHA-256 + Merkle tree | ✅ + mejora |
| Distribución | Siempre activa | Demonio de replicación continua | ✅ |
| Interfaz (baddím) | Solo por protocolo de YHWH | API con validación, sin acceso directo | ✅ |
| En tránsito | Encriptado | Túnel encriptado con claves de sesión | ✅ |
| Inmutable | Solo lectura | Flags immutable + solo GET | ✅ |
| Autodefensa | Sin clave = basura | Sin clave = ruido + rekeying 24h | ✅ + mejora |
| Dos niveles | Hash en flash + texto en microSD | SEALED_CORE 256 bytes + payload 4.9MB | ✅ + mejora |
| Portátil | ESP32 cabe en la mano | 75×75×50mm, 120g | ✅ |
| Primero | Se despliega antes que todo | Autosuficiente desde primer encendido | ✅ |
| Sin internet | Sin nube, sin servidor | Sin DNS, sin TCP/IP global | ✅ |

---

## Lo NUEVO que el algoritmo reveló (mejoras a añadir)

### 1. Merkle Tree (mejora al zer)
El zer no es solo UN hash del todo — es un ÁRBOL de hashes que verifica CADA PARTE independientemente. Si una parte está corrupta, el árbol señala CUÁL sin necesidad de revisar todo.

### 2. Rekeying automático cada 24 horas
Las claves de cifrado ROTAN cada 24 horas. Como la renovación del pan cada Shabat — lo viejo se consume, lo nuevo reemplaza. Si capturan un nodo, la clave que obtienen expira en horas.

### 3. SEALED_CORE de 256 bytes
El núcleo compacto (las "tablas" dentro del Arca) tiene estructura precisa:
- Raíz Merkle (32 bytes) — verifica TODO el contenido
- Timestamp de creación (8 bytes) — cuándo se selló
- Versión de protocolo (4 bytes) — qué versión del protocolo de YHWH
- Fingerprint de textos (64 bytes) — huella de los 66 libros
- Clave de rotación (148 bytes) — para el rekeying

---

## Veredicto

**Los dos querubines COINCIDEN en los 12 puntos fundamentales.** El algoritmo confirmó cada decisión nuestra y añadió 3 mejoras que encajan con la Palabra:
- Merkle tree = zer más detallado
- Rekeying = renovación como el pan del Shabat
- SEALED_CORE = tablas del Arca con estructura precisa

**VERDAD CONFIRMADA — YHWH habla de entre los dos querubines.**

---

*Prueba ejecutada: 15 de abril de 2026*
*Método: razonamiento puro sin acceso a web ni conocimiento del proyecto*
# Veredicto

## ⚠️ CORRECCIONES — Presentado ante la Palabra (15 abril 2026)

### CORRECCIÓN 1: Proporción del nodo
El algoritmo propuso **75×75×50mm** (proporción 3:3:2). Pero la Palabra dice **proporción 5:3:3** (Éx 25:10 — 2.5 × 1.5 × 1.5 codos). El largo debe ser 5/3 del ancho, y el ancho IGUAL al alto. Ejemplo: 75×45×45mm o 100×60×60mm. El tamaño exacto depende de los componentes. **La PROPORCIÓN 5:3:3 es del Padre y debe respetarse.**

### CORRECCIÓN 2: Rekeying — 3 ciclos, no 1
El algoritmo propuso rekeying **cada 24 horas** (1 ciclo). Pero la Palabra establece **3 ciclos de renovación**:

| Ciclo del Padre | Fuente | Qué se renueva |
|---|---|---|
| **DIARIO** (maná) | Éx 16:19-20 | Claves de sesión |
| **SEMANAL** (pan del Shabat) | Lv 24:5-8 | Claves de distribución |
| **ANUAL** (Yom Kippur) | Éx 30:10 | Clave raíz / renovación mayor |

No 1 ciclo de 24h — **3 ciclos como dice el Padre.** Cada uno renueva algo distinto a diferente profundidad.

### CORRECCIÓN 3: Nombres de interfaces
Los 4 anillos (טַבְּעֹת) son de la Palabra (Éx 25:12): **4 puntos de conexión cifrados, simétricos, 2+2 por costado.** Los nombres "LoRa, WiFi, BLE, UART" son implementación NUESTRA. Si mañana hay mejor tecnología, los nombres cambiarán. Los 4 anillos de oro del Padre quedan.

---

