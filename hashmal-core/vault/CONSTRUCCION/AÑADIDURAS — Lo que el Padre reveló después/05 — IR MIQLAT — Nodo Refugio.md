# 05 — IR MIQLAT — Nodo Refugio

> *"Señalaréis ciudades que os sirvan de עָרֵי מִקְלָט (ciudades de refugio),
> donde huya el que hiriere a alguno sin intención."*
> — Números 35:11

---

## QUÉ ES

Algunos nodos de la red se designan como **refugio permanente**.
Un nodo refugio nunca se apaga, siempre acepta conexiones,
y es el punto seguro al que cualquier persona puede llegar.

Es la casa de la abuela, la iglesia del pueblo, el centro comunitario.
La puerta que siempre está abierta.

---

## RESPALDO BÍBLICO

| # | Cita | Hebreo | Strong | Dice |
|---|------|--------|--------|------|
| 1 | Números 35:11 | עָרֵי מִקְלָט (aréi miqlat) | H5892, H4733 | "Ciudades de REFUGIO para vosotros" |
| 2 | Números 35:15 | לְכָל בְּנֵי יִשְׂרָאֵל וְלַגֵּר | — | "Para los hijos de Israel Y para el extranjero" — acceso universal |
| 3 | Josué 20:2-3 | עָרֵי הַמִּקְלָט | H4733 | "Señalaos ciudades de refugio... para que huya allí el homicida" |
| 4 | Josué 20:6 | עַד עָמְדוֹ לִפְנֵי הָעֵדָה | — | "Hasta que comparezca ante la congregación" — refugio temporal hasta juicio justo |
| 5 | Números 35:6 | שֵׁשׁ עָרֵי הַמִּקְלָט | H8337 | "SEIS ciudades de refugio" — número fijo, no infinito |
| 6 | Éxodo 27:2 | קַרְנֹת (qarnot) | H7161 | Los 4 cuernos del altar — ya implementados en altar_cuernos.c |

---

## PROPIEDADES DEL NODO REFUGIO

```
OBLIGACIONES:
  1. NUNCA se apaga (tamíd — siempre encendido)
  2. Acepta conexión de CUALQUIER nodo (incluso no autenticado)
  3. No penaliza al que llega (misericordia sobre juicio — Stg 2:13)
  4. Alimentación permanente (solar + red eléctrica)
  5. Posición CONOCIDA por toda la red (publicada)

LÍMITES (Números 35):
  6. Máximo 6 nodos refugio por red (שֵׁשׁ — seis)
  7. El refugio es TEMPORAL hasta juicio justo (Josué 20:6)
  8. Si el nodo refugiado resulta malicioso, se expulsa DESPUÉS de verificación
  9. El refugio NO protege al que actúa con intención (Números 35:20-21)

ACCESO:
  - Para Israel Y para el extranjero (Números 35:15)
  - = nodos de la red Y nodos desconocidos
  - Puerta abierta, verificación DESPUÉS, no antes
```

---

## DIFERENCIA CON LOS CUERNOS DEL ALTAR

Los cuernos (altar_cuernos.c) son refugio DENTRO del Mishkán — para nodos ya autenticados.
El ir miqlat es refugio FUERA — para cualquiera que llegue corriendo, incluido el extranjero.

---

## DÓNDE VA EN EL FIRMWARE

**Pieza 07 — Atrio (hashmal_perimeter)**

El atrio es la frontera. El nodo refugio es una puerta especial en esa frontera:
siempre abierta, verificación posterior. Se integra con atrio_puerta.c.

Nuevo flag: `HASHMAL_PERIMETER_IR_MIQLAT` — marca un nodo como refugio.

---

*"Seis ciudades de refugio tendréis. Para los hijos de Israel,
para el extranjero y para el que habite entre vosotros."*
*— Números 35:15*
