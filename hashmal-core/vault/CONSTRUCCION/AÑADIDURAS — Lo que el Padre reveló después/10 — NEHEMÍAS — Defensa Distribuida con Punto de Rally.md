# 10 — NEHEMÍAS — Defensa Distribuida con Punto de Rally

> *"La obra es grande y extensa, y nosotros estamos separados en el muro,
> lejos unos de otros. En el lugar donde oigáis el sonido del SHOFAR,
> allí os reuniréis con nosotros."*
> — Nehemías 4:19-20

---

## QUÉ ES

Cuando la red detecta un ataque o emergencia, el shofar suena
y los nodos se agrupan AUTOMÁTICAMENTE hacia la zona del problema.
No hay un punto fijo de rally — es DINÁMICO. Donde suena, allí van.

---

## RESPALDO BÍBLICO

| # | Cita | Hebreo | Strong | Dice |
|---|------|--------|--------|------|
| 1 | Nehemías 4:13 | וָאַעֲמִיד... מִשְׁפָּחוֹת עִם חַרְבוֹתֵיהֶם | H4940 | "Puse al pueblo por FAMILIAS con sus espadas" — defensa por grupos |
| 2 | Nehemías 4:14 | אַל תִּירְאוּ... הִלָּחֲמוּ | H3898 | "No temáis... PELEÁD por vuestros hermanos" — defensa mutua |
| 3 | Nehemías 4:15 | כֻּלָּנוּ אֶל הַחוֹמָה אִישׁ אֶל מְלַאכְתּוֹ | H4399 | "Cada uno a SU obra" — cada nodo en su posición, trabajo distribuido |
| 4 | Nehemías 4:17 | בְּאַחַת יָדוֹ עֹשֶׂה בַמְּלָאכָה וְאַחַת מַחֲזֶקֶת הַשָּׁלַח | H7973 | "Una mano trabaja, la otra sostiene el arma" — servicio + defensa simultáneos |
| 5 | Nehemías 4:19-20 | בִּמְקוֹם אֲשֶׁר תִּשְׁמְעוּ אֶת קוֹל הַשּׁוֹפָר שָׁמָּה תִּקָּבְצוּ אֵלֵינוּ | H7782, H6908 | "Donde OIGÁIS el shofar, allí os REUNIRÉIS" — punto de rally dinámico |
| 6 | Nehemías 4:16 | חֲצִי נְעָרַי עֹשִׂים בַּמְּלָאכָה וַחֲצִי מַחֲזִיקִים | — | "MITAD trabaja, MITAD vigila" — rotación de roles defensa/servicio |

---

## CÓMO FUNCIONA EN LA RED

```
ESTADO NORMAL:
  Cada nodo en su posición, haciendo su trabajo.
  "Cada uno a su obra en el muro" (v.15).
  La red funciona distribuida, cada nodo independiente.

AMENAZA DETECTADA:
  Un nodo Tsofeh (Añadidura 02) detecta ataque.
  Toca SHOFAR — señal de alarma (teruʻah, Añadidura 08).
  El shofar incluye la UBICACIÓN de la amenaza.

RALLY DINÁMICO:
  "Donde oigáis el shofar, allí os reuniréis" (v.20).
  Los nodos cercanos a la amenaza se agrupan automáticamente.
  Forman cluster defensivo temporal alrededor de la zona atacada.
  Los nodos lejanos mantienen operación normal.

DEFENSA ACTIVA:
  "Una mano trabaja, la otra sostiene el arma" (v.17).
  Los nodos en rally: mitad mantienen servicio, mitad defienden.
  No se para toda la red — solo la zona afectada responde.

VUELTA A NORMAL:
  Amenaza neutralizada → shofar toca teqiʻah (tono normal).
  Nodos vuelven a sus posiciones originales.
  "Cada uno a su obra."
```

---

## LA LECCIÓN DE NEHEMÍAS PARA LA RED

Nehemías tenía un problema: el muro era ENORME y la gente estaba
SEPARADA. No podía tener un ejército en cada punto. Solución:
cada familia trabaja en su sección, pero cuando suena el shofar,
todos corren al punto de peligro.

Es EXACTAMENTE el problema de una red mesh distribuida:
los nodos están separados, no puedes tener defensa masiva
en cada punto. Pero cuando un nodo detecta ataque, avisa,
y los vecinos se agrupan para defender.

Y lo más importante: "UNA MANO trabaja, la OTRA sostiene el arma."
La red NUNCA para completamente. Siempre hay servicio activo
incluso durante un ataque.

---

## DÓNDE VA EN EL FIRMWARE

**Pieza 04 — Menorá (hashmal_comm)** para la señal de rally.
**Pieza 14 — Incienso Sagrado (hashmal_firewall)** para la defensa.
**Añadidura 02 — Modo Tsofeh** para la detección inicial.
**Añadidura 08 — Jatsotsrot** para los tipos de toque.

Nuevo mecanismo: `HASHMAL_RALLY_POINT` — coordenadas dinámicas
de agrupación cuando se detecta amenaza.

---

*"Nuestro Dios peleará por nosotros."*
*— Nehemías 4:20*
