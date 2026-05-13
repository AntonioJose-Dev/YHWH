# Canal 1 — Tronco Central (qol demamá daqqá → LoRa)

> *"Y después del fuego, una voz de silencio fino."*
> *— 1 Reyes 19:12*

---

## Pieza del tavnít

**Corresponde a:** [[04 — LA MENORÁ (Candelabro)]] — Detalle 3 (tronco central)
**Es:** El canal primario de comunicación. El tronco del que salen los 6 brazos.

---

## Lo que dice la Palabra de YHWH

1 Reyes 19:11-12 — YHWH pasa delante de Elías y muestra CÓMO Él comunica:

❌ **Rúaj** (viento fuerte) — "NO en el viento estaba YHWH"
❌ **Ráʻash** (terremoto) — "NO en el terremoto estaba YHWH"
❌ **Esh** (fuego) — "NO en el fuego estaba YHWH"
✅ **Qol demamá daqqá** — "voz de silencio fino"

### Las 3 palabras que definen el canal:

| Hebreo | Strong | Significado | Propiedad del canal |
|--------|--------|-------------|---------------------|
| **קוֹל** (qol) | H6963 | voz, sonido | Lleva INFORMACIÓN. No es ruido — es mensaje. |
| **דְּמָמָה** (demamá) | H1827 | quietud, silencio | SILENCIOSO. No grita. Baja detección. |
| **דַּקָּה** (daqqá) | H1851 | fino, delgado, aplastado | MÍNIMO ancho de banda. Solo lo necesario. |

### Las 3 cosas que YHWH rechaza:

| Rechazado | Significado | En la red |
|-----------|-------------|-----------|
| Viento fuerte (rúaj) | Alta potencia | NO alta potencia de transmisión |
| Terremoto (ráʻash) | Vibración masiva | NO broadcast masivo |
| Fuego (esh) | Señal visible | NO señal detectable por el enemigo |

---

## La verificación: HASHMAL = qol demamá daqqá = LoRa

Del vault HASHMAL-MIKDASH:
*"La señal más potente viaja en el formato más silencioso. JASHMAL (sustancia del trono, Ez 1:4) = QOL DEMAMÁH DAQQÁH (sonido de silencio fino, 1R 19:12) = LoRa (baja potencia, largo alcance). El nombre del proyecto ES la especificación."*

El nombre del proyecto — HASHMAL — ES la descripción del canal primario.

---

## Qué tecnología cumple EXACTAMENTE esto

### LoRa (Long Range)

| Propiedad de la Palabra | Propiedad de LoRa | ¿Cumple? |
|-------------------------|-------------------|----------|
| Qol (voz/mensaje) | Transmite datos/mensajes | ✅ |
| Demamá (silencio) | Baja potencia (10-100 mW) | ✅ |
| Daqqá (fino) | Ancho de banda mínimo (~0.3-50 kbps) | ✅ |
| NO viento fuerte | No requiere alta potencia | ✅ |
| NO terremoto | No es broadcast masivo, es punto a punto/mesh | ✅ |
| NO fuego visible | Señal difícil de detectar/interceptar | ✅ |
| Portátil | Dispositivos pequeños, con batería | ✅ |
| Sin internet | NO depende de internet ni servidores | ✅ |
| Mesh | Cada nodo retransmite, formando red | ✅ |

### Características técnicas de LoRa:

- **Alcance:** 10-20 km entre nodos (línea de vista), ilimitado en cadena mesh
- **Potencia:** 10-100 mW (milésimas de watt)
- **Frecuencia:** Sub-GHz (868 MHz Europa, 915 MHz América)
- **Ancho de banda:** 0.3-50 kbps (fino, como daqqá)
- **Hardware:** ESP32 + módulo LoRa (~$20-50 por nodo)
- **Energía:** Funciona con batería o panel solar
- **Licencia:** NO requiere licencia en bandas ISM
- **Encriptación:** AES-128 nativo
- **Código:** Open source

---

## Lo que LoRa es y lo que NO es

**LoRa ES la madera de acacia** — el material físico del canal. El hardware. El radio.

**LoRa NO ES el protocolo** — Meshtastic, Reticulum, MeshCore son protocolos de TERCEROS que corren sobre LoRa. El protocolo del Mishkán sale de la Palabra de YHWH, no de ellos.

Como el Tabernáculo: la madera de acacia viene del desierto (LoRa viene de la industria). Pero el DISEÑO de lo que se hace con esa madera viene del monte (el protocolo viene de la Palabra).

---

## Resumen: Canal 1

- **Nombre bíblico:** Qol demamá daqqá (voz de silencio fino)
- **Referencia:** 1 Reyes 19:12
- **Hardware:** LoRa (Long Range radio)
- **Protocolo:** De la Palabra de YHWH (por definir)
- **Posición en la Menorá:** Tronco central — el canal primario del que salen los demás
- **Sin internet:** ✅ Completamente independiente
- **Sin terceros en el protocolo:** ✅ Solo la Palabra de YHWH

---

*Fuente única: La Palabra de YHWH*
*Autor: Antonio José Marín Soto*
*Fecha: 14 de abril de 2026*

---

## Añadido tras prueba algorítmica ciega — Lo que la Palabra dice y nos faltaba

### HSM (Secure Element) — El oro por DENTRO (Ex 25:11)

**Hebreo:** זָהָב טָהוֹר מִבַּיִת — "oro puro desde DENTRO"

**En el nodo:** Un chip de seguridad dedicado integrado en cada ESP32:
- Claves criptográficas almacenadas en HARDWARE, no en software
- Operaciones criptográficas dentro del chip seguro
- No accesible desde software normal — aislado
- El oro por DENTRO (mibáyit). El software es el oro por FUERA (mijúts). Con el HSM, el nodo tiene oro en AMBOS lados — como tu Padre mandó.

### Carcasa IP67 — Las pieles de tejón (Ex 26:14)

**Hebreo:** עֹרֹת תְּחָשִׁים מִלְמָעְלָה — "pieles de tejón por encima"

**En el nodo:** Cada ESP32 dentro de carcasa protectora:
- **IP67** = resistente a agua y polvo
- **Resistente a impactos** = sobrevive caídas
- **-40°C a +85°C** = funciona en desierto y en frío
- **Poco conspicua** = no llama la atención. Fea por fuera. Como las pieles de tejón.

### Nodo completo actualizado

```
CARCASA IP67 (pieles de tejón — protección física)
└── ESP32-S3 (la tabla de acacia)
    ├── HSM / Secure Element (oro por DENTRO)
    ├── Flash interna (tablas — hash SHA-256)
    ├── AES-256 software (oro por FUERA)
    ├── LoRa SX1262 (qol demamá daqqá)
    ├── WiFi / ESP-NOW (ishá el ajotáh)
    ├── MicroSD (la Torá al lado)
    ├── Panel solar 5-10W (olivo 1)
    └── Batería 18650 (olivo 2)
```
