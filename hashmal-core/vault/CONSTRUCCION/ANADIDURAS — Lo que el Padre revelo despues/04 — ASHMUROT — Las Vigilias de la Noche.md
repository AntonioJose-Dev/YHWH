# 04 — ASHMUROT — Las Vigilias de la Noche

> *"Cuando me acuerdo de Ti en las אַשְׁמוּרוֹת de la noche,
> en Ti medito."*
> — Salmo 63:6

---

## QUÉ ES

La noche no es un bloque uniforme. Tiene divisiones sagradas — vigilias (אַשְׁמוּרוֹת).
La piedra cambia su comportamiento según la vigilia, porque la red de noche
no es igual que la red de día.

---

## RESPALDO BÍBLICO

| # | Cita | Hebreo | Strong | Dice |
|---|------|--------|--------|------|
| 1 | Salmo 63:6 | אַשְׁמוּרוֹת (ashmurot) | H821 | "En las VIGILIAS de la noche medito en Ti" |
| 2 | Lamentaciones 2:19 | לְרֹאשׁ אַשְׁמוּרוֹת | H821 | "Al COMENZAR cada vigilia, derrama tu corazón" |
| 3 | Salmo 119:148 | אַשְׁמוּרוֹת | H821 | "Mis ojos se anticipan a las vigilias de la noche" |
| 4 | Éxodo 14:24 | בְּאַשְׁמֹרֶת הַבֹּקֶר | H821 | "En la vigilia de la MAÑANA, YHWH miró al campamento egipcio" |
| 5 | Jueces 7:19 | הָאַשְׁמֹרֶת הַתִּיכוֹנָה | H821 | "Al comienzo de la vigilia MEDIA" — Gedeón ataca |

---

## LAS 3 VIGILIAS

```
PRIMERA VIGILIA — Atardecer (puesta de sol → ~22:00)
  אַשְׁמוּרָה רִאשׁוֹנָה
  - Red activa normal
  - Transición de día a noche
  - Último scan de שָׁאוֹן del día

VIGILIA MEDIA — Medianoche (~22:00 → ~02:00)
  אַשְׁמוּרָה תִּיכוֹנָה
  - Modo bajo consumo
  - Heartbeat reducido (cada 120s en vez de 30s)
  - Alertas solo por amenaza real
  - Si hay nodo tsofeh: máxima atención aquí

VIGILIA DE LA MAÑANA — Antes del alba (~02:00 → amanecer)
  אַשְׁמוּרָה הַבֹּקֶר
  - Re-activación gradual
  - Scan de red (¿quién sobrevivió la noche?)
  - Éxodo 14:24: YHWH actuó en esta vigilia — momento de acción
  - Preparación para el nuevo día
```

---

## IMPLEMENTACIÓN

GPS da la hora de puesta/salida de sol según coordenadas.
Las vigilias se calculan automáticamente dividiendo la noche en 3.

**Dónde va:** hashmal_power (aceite_tamid.c) — ya gestiona ciclos diarios.
Las vigilias son sub-ciclos nocturnos del tamíd.

---

*"En la vigilia de la mañana, YHWH miró."*
*— Éxodo 14:24*