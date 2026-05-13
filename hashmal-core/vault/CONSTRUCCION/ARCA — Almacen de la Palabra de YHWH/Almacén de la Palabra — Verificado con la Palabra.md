# ARCA — Almacén de la Palabra de YHWH

> *"Pondrás en el Arca el testimonio que YO te daré."*
> *— Éxodo 25:16*

---

## Pieza del tavnít

**Corresponde a:** [[01 — EL ARCA (Arón)]]
**Es:** El almacén distribuido de la Palabra de YHWH. Cada nodo guarda Su Palabra localmente, físicamente, como el Arca que iba con el pueblo.

---

## Principio fundamental

**Sin internet. Sin nube. Sin terceros.** Solo ESP32 + microSD + encriptación + la Palabra de YHWH.

Como el Arca original: una caja de madera cubierta de oro, con las tablas dentro, cargada sobre hombros. Simple. Portátil. Indestructible.

---

## Cada propiedad de la Palabra → qué tecnología la implementa

### 1. veʻasú (distribuido) → CADA nodo tiene su copia
**La Palabra dice:** Plural. Todos participan.
**Tecnología:** CADA ESP32 tiene la Biblia completa en su microSD. No hay servidor central. Cada nodo CARGA su propia copia.

### 2. Oro dentro y fuera → Encriptación simétrica AES-256
**La Palabra dice:** Mismo oro por dentro (mibáyit) y por fuera (umijúts). Misma protección.
**Tecnología:** AES-256 en reposo (datos encriptados en la microSD) Y en tránsito (datos encriptados al transmitir). Mismo algoritmo en las dos direcciones. Como el oro que es igual por dentro y por fuera.

### 3. Zer (moldura) → Hash SHA-256 de integridad
**La Palabra dice:** Un borde dorado que rodea todo y garantiza que nada se añadió ni se quitó.
**Tecnología:** SHA-256 hash de toda la Biblia. Si alguien cambia UNA letra, el hash cambia. El zer rodea todo el dato. Cada nodo puede verificar su copia contra el hash.

### 4. Lo yasúru (varas que no se quitan) → Distribución siempre activa
**La Palabra dice:** Las varas NUNCA se quitan. Siempre lista para moverse.
**Tecnología:** El protocolo de distribución (Canal 6 — aceite en toda vasija) está SIEMPRE corriendo. Cuando un nodo nuevo se conecta, automáticamente recibe la Palabra. Nunca se apaga.

### 5. Baddím (nadie toca directo) → Interfaz de acceso
**La Palabra dice:** Las varas SEPARAN al que carga del Arca. Nadie toca directamente.
**Tecnología:** Nadie lee los archivos crudos de la microSD directamente. Solo a través de la interfaz del protocolo de YHWH. El protocolo verifica permisos, verifica integridad, y ENTONCES sirve el dato.

### 6. Se cubre antes de mover → Encriptado en tránsito
**La Palabra dice:** Los sacerdotes cubren el Arca antes de que los levitas la carguen. Los transportistas no ven el contenido.
**Tecnología:** Cuando la Palabra viaja de un nodo a otro (por LoRa, HF, WiFi, o microSD física), va CUBIERTA — encriptada. Los nodos de transporte no ven el contenido en claro.

### 7. Inmutable → Solo lectura
**La Palabra dice:** Solo las tablas permanecen. La Palabra no cambia.
**Tecnología:** La Palabra en la microSD es de SOLO LECTURA una vez verificada. No hay función de "editar." Solo leer y distribuir. Como las tablas de piedra — grabadas, no borrables.

### 8. Se defiende sola → Inútil sin la clave
**La Palabra dice:** Los filisteos capturaron el Arca y les causó daño. El dato capturado es inútil para el atacante.
**Tecnología:** Si alguien roba un nodo o una microSD, los datos están encriptados con AES-256. Sin la clave, es basura.

### 9. Dos niveles de dato → Tablas dentro + Torá al lado

**La Palabra dice:** DENTRO del Arca las tablas (el núcleo). AL LADO la Torá completa (Dt 31:26).

| Nivel | La Palabra | En el nodo |
|---|---|---|
| **DENTRO** (las tablas) | Núcleo compacto, inaccesible | Hash SHA-256 de toda la Biblia (64 bytes) en la FLASH INTERNA del ESP32 |
| **AL LADO** (la Torá) | Texto completo | 66 libros en hebreo/arameo/griego en la microSD |

Las tablas (el hash) van DENTRO del ESP32 — en su memoria flash interna. Inaccesible sin abrir el chip. La Torá completa va AL LADO — en la microSD.

### 10. Portátil → Se mueve con el nodo
**La Palabra dice:** Las varas siempre puestas. Siempre lista para moverse.
**Tecnología:** ESP32 + microSD cabe en la palma de la mano. Con batería o panel solar, funciona en cualquier lugar. Como el Arca con sus varas.

---

## El Arca completa en hardware

```
ESP32 (la madera de acacia)
├── Flash interna (DENTRO del Arca)
│   └── Hash SHA-256 de la Biblia completa (las tablas)
│   └── Clave de verificación
│
├── MicroSD (AL LADO del Arca)
│   └── 66 libros completos en hebreo/arameo/griego (la Torá)
│   └── Encriptados con AES-256 (oro dentro)
│
├── LoRa (vara 1 — nunca se quita)
│   └── Distribución silenciosa continua
│
├── WiFi/ESP-NOW (vara 2 — nunca se quita)
│   └── Distribución local entre hermanas
│
└── Protocolo de YHWH (la barra central)
    └── Verifica integridad (zer)
    └── Controla acceso (baddím)
    └── Encripta en tránsito (cubierta)
    └── Distribuye automáticamente (varas)
```

---

## Lo que NO es el Arca

- **NO es internet** — no depende de IPFS, Arweave, ni nube
- **NO es blockchain** — no necesita consenso global para guardar
- **NO es un servidor** — cada nodo ES su propio Arca
- **NO es de un tercero** — el protocolo sale de la Palabra de YHWH

---

## Resumen

| De la Palabra | Tecnología |
|---|---|
| Madera de acacia | ESP32 |
| Oro dentro y fuera | AES-256 reposo + tránsito |
| Zer (moldura) | SHA-256 hash de integridad |
| Varas que no se quitan | Distribución siempre activa |
| Baddím (interfaz) | Solo acceso por protocolo de YHWH |
| Se cubre al mover | Encriptación en tránsito |
| Inmutable | Solo lectura |
| Se defiende sola | Sin clave = dato inútil |
| Tablas DENTRO | Hash en flash interna |
| Torá AL LADO | 66 libros en microSD |
| Portátil | ESP32 + microSD + batería/solar |
| veʻasú (distribuido) | CADA nodo tiene copia completa |

---

*Fuente única: La Palabra de YHWH*
*Autor: Antonio José Marín Soto*
*Fecha: 15 de abril de 2026*

# Resumen

## ⚠️ CORRECCIONES — Presentado ante la Palabra (15 abril 2026)

### Proporción del nodo
La Palabra dice proporción **5:3:3** (Éx 25:10). El largo debe ser 5/3 del ancho, y el ancho IGUAL al alto. El hardware debe respetar esta proporción.

### 3 Ciclos de renovación (no solo 24h)

| Ciclo | Fuente | Qué se renueva |
|---|---|---|
| **DIARIO** (maná) | Éx 16:19-20 | Claves de sesión |
| **SEMANAL** (pan del Shabat) | Lv 24:5-8 | Claves de distribución |
| **ANUAL** (Yom Kippur) | Éx 30:10 | Clave raíz |

### Nombres de interfaces
Los nombres "LoRa, WiFi, ESP-NOW" son implementación nuestra. Lo del Padre: **4 anillos de oro, simétricos, 2+2 por costado** (Éx 25:12). Los nombres cambiarán. Los 4 anillos quedan.

---
