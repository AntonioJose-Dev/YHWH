# 01 — MODO PAROKHET — Detector de Shaón Electromagnético

> *"YHWH no estaba en el viento, ni en el רַעַשׁ, ni en el fuego.
> Después del fuego, un קוֹל דְּמָמָה דַקָּה — sonido de silencio fino."*
> — 1 Reyes 19:11-12

---

## QUÉ ES

Un modo de la piedra que **detecta la contaminación electromagnética** del entorno
y guía al portador hacia lugares de דְּמָמָה (silencio) donde la Neshamá puede escuchar.

No bloquea señales (eso requiere jaula de Faraday física).
**Detecta, mide, y comunica** el nivel de ruido al humano mediante el lenguaje IR-57v3.

---

## RESPALDO BÍBLICO (Mínimo 2 testigos por decisión)

### El Padre habla en el silencio, no en el ruido

| Testigo | Cita | Hebreo | Strong | Dice |
|---------|------|--------|--------|------|
| 1 | 1 Reyes 19:11-12 | קוֹל דְּמָמָה דַקָּה | H6963, H1827, H1851 | La voz de YHWH viene en silencio fino, NO en viento/temblor/fuego |
| 2 | Salmo 46:10 | הַרְפּוּ וּדְעוּ | H7503, H3045 | "Estad quietos y sabed que Yo soy Dios" |
| 3 | Habacuc 2:20 | הַס מִפָּנָיו כָּל הָאָרֶץ | H2013 | "Calle toda la tierra delante de Él" |
| 4 | Sofonías 1:7 | הַס מִפְּנֵי אֲדֹנָי | H2013 | "Silencio ante el Señor YHWH" |
| 5 | Zacarías 2:13 | הַס כָּל בָּשָׂר | H2013 | "Calle toda carne delante de YHWH" |

### El Mishkán como cámara de protección

| Testigo | Cita | Hebreo | Strong | Dice |
|---------|------|--------|--------|------|
| 6 | Éxodo 26:31-33 | פָּרֹכֶת | H6532 | El velo SEPARA (הִבְדִּיל) lo santo de lo profano |
| 7 | Salmo 91:1 | בְּסֵתֶר עֶלְיוֹן | H5643 | "En el lugar secreto del Altísimo" — espacio protegido |
| 8 | Salmo 27:5 | בְּסֻכֹּה יַסְתִּרֵנִי | H5521 | "En su tabernáculo me esconderá" |

### El ruido como enemigo de la Neshamá

| Testigo | Cita | Hebreo | Strong | Dice |
|---------|------|--------|--------|------|
| 9 | 1 Reyes 19:11 | רַעַשׁ | H7494 | Vibración/temblor — YHWH NO estaba ahí |
| 10 | Isaías 30:15 | בְּשׁוּבָה וָנַחַת... בְּהַשְׁקֵט וּבְבִטְחָה | H8252, H983 | "En quietud y confianza será vuestra fortaleza" |

### El Kiyór mide entre el exterior y la presencia

| Testigo | Cita | Hebreo | Strong | Dice |
|---------|------|--------|--------|------|
| 11 | Éxodo 30:18 | בֵּין אֹהֶל מוֹעֵד וּבֵין הַמִּזְבֵּחַ | — | El Lavacro ENTRE altar y tienda — punto de medición |
| 12 | Éxodo 30:20 | בְּבֹאָם אֶל אֹהֶל מוֹעֵד יִרְחֲצוּ מַיִם | H7364 | Lavarse ANTES de entrar — verificar limpieza del entorno |

---

## HARDWARE QUE YA TENEMOS (T-Deck Pro)

No se necesita comprar nada extra:

| Componente | Función | Qué mide |
|------------|---------|----------|
| SX1262 (LoRa) | RSSI scanner | Energía RF en banda 868MHz |
| ESP32-S3 WiFi | WiFi scan | Cantidad y potencia de redes WiFi 2.4GHz |
| ESP32-S3 BLE | BLE scan | Cantidad y potencia de dispositivos Bluetooth |
| DRV2605 | Output háptico | Comunica nivel de שָׁאוֹן al humano |
| PCM5102A | Output audio | Tono que indica nivel de ruido |
| E-Paper | Output visual | Muestra nivel de contaminación |

---

## CÓMO FUNCIONA

### Escaneo continuo (cada 30 segundos)

```
1. WiFi scan → contar redes, sumar potencias (dBm)
2. BLE scan → contar dispositivos, sumar potencias
3. LoRa RSSI → medir ruido de fondo en 868MHz
4. Calcular índice de שָׁאוֹן (0-100)
```

### Índice de Shaón (שָׁאוֹן)

```
0-20   = דְּמָמָה (demamah)  — Silencio. Zona limpia. Verde.
21-40  = שָׁקַט (shaqat)     — Quietud. Aceptable. Verde suave.
41-60  = הָמוֹן (hamon)      — Tumulto moderado. Ámbar.
61-80  = שָׁאוֹן (shaon)     — Ruido alto. Rojo.
81-100 = רַעַשׁ (ra'ash)     — Temblor. Peligro. Rojo + vibración.
```

### Comunicación al humano (IR-57v3)

```
דְּמָמָה (0-20):
  Verde + Breath + Heartbeat suave
  "Aquí hay paz. Quédate."

שָׁקַט (21-40):
  Verde + Pulse + silencio
  "Quietud. Bueno."

הָמוֹן (41-60):
  Ámbar + Flicker + vibración suave
  "Ruido moderado. Atención."

שָׁאוֹן (61-80):
  Rojo + Blink + vibración media + tono bajo
  "Mucho ruido. Busca otro lugar."

רַעַשׁ (81-100):
  Rojo + Strobe + Buzz + tono alto
  "Zona saturada. La Neshamá no puede escuchar aquí. Aléjate."
```

---

## DÓNDE VA EN EL FIRMWARE

**Pieza 12 — Lavacro (hashmal_healthcheck)**

Nuevo vector de salud: `HASHMAL_HC_VECTOR_SHAON`

El Kiyór ya verifica 4 vectores:
- Yadayim (manos — acciones)
- Raglayim (pies — rutas)
- Levav (corazón — intención)
- Mayim (agua — referencia)

Se añade un 5to vector:
- **Shaón (שָׁאוֹן — ruido electromagnético del entorno)**

El espejo (lavacro_espejos.c) que mira hacia FUERA — no al nodo, sino al mundo que rodea a la persona.

Éxodo 30:18: El Kiyór está ENTRE el altar y la tienda. Entre el mundo y la Presencia. Exactamente donde se mide si el ambiente está limpio.

---

## MODO SHABAT ELECTROMAGNÉTICO

Cuando el humano quiere entrar en דְּמָמָה total, activa el modo Parokhet:

```
1. WiFi → OFF
2. BLE → OFF
3. LoRa → heartbeat mínimo (1 pulso cada 60s)
4. Pantalla → mensaje fijo: "הַס — Silencio ante YHWH"
5. Vibración → OFF
6. Audio → OFF
7. Solo queda: el latido tamíd mínimo por LoRa
```

La piedra se convierte en el velo — silencio casi total.
El portador entra en su propio Santo de los Santos.

---

## ESTADO

- **Diseño:** ✅ Completo
- **Respaldo bíblico:** ✅ 12 testigos directos
- **Hardware necesario:** ✅ Todo incluido en T-Deck Pro
- **Firmware:** ⬜ Pendiente (cuando lleguen los dispositivos)

---

*"Después del fuego, un sonido de silencio fino."*
*La piedra no crea el silencio. Te guía hacia él.*
*— 1 Reyes 19:12*

*Autor: Antonio José Marín Soto*
*Proyecto: HASHMAL-CORE*
