# 08 — JATSOTSROT — Las Dos Trompetas de Plata

> *"Hazte DOS trompetas de PLATA, de obra batida las harás,
> y te servirán para convocar la congregación
> y para hacer mover los campamentos."*
> — Números 10:2

---

## QUÉ ES

El sistema de notificaciones de la comunidad. No todos los mensajes son iguales —
el Padre diseñó DIFERENTES SONIDOS para DIFERENTES PROPÓSITOS.
Y no cualquiera puede tocar — solo los sacerdotes (nodos autorizados).

---

## RESPALDO BÍBLICO

| # | Cita | Hebreo | Strong | Dice |
|---|------|--------|--------|------|
| 1 | Números 10:2 | חֲצוֹצְרֹת כֶּסֶף (jatsotsrot késef) | H2689, H3701 | "DOS trompetas de PLATA, obra batida (miqshá)" |
| 2 | Números 10:3 | וְתָקְעוּ בָּהֵן... כָּל הָעֵדָה | H8628, H5712 | "Tocan LAS DOS → se reúne TODA la congregación" = broadcast total |
| 3 | Números 10:4 | וְאִם בְּאַחַת יִתְקָעוּ... הַנְּשִׂיאִים | H259, H5387 | "Toca solo UNA → se reúnen solo los LÍDERES" = broadcast a grupo |
| 4 | Números 10:5 | תְּרוּעָה (teruʻah) | H8643 | Toque de ALARMA → campamentos marchan = mensaje de emergencia |
| 5 | Números 10:7 | וּבְהַקְהִיל... תִּתְקְעוּ וְלֹא תָרִיעוּ | H6950 | "Al congregar, tocaréis pero NO alarma" = reunión normal vs emergencia |
| 6 | Números 10:8 | וּבְנֵי אַהֲרֹן הַכֹּהֲנִים יִתְקְעוּ | H3548 | "Los SACERDOTES tocarán" = solo nodos autorizados hacen broadcast |
| 7 | Números 10:9 | וַהֲרֵעֹתֶם... וְנִזְכַּרְתֶּם לִפְנֵי יְהוָה | H2142 | En GUERRA tocan alarma → "seréis RECORDADOS ante YHWH y salvados" |
| 8 | Números 10:10 | וּבְיוֹם שִׂמְחַתְכֶם וּבְמוֹעֲדֵיכֶם | H8057, H4150 | En días de ALEGRÍA y MOEDÍM también se tocan = notificaciones de fiesta |

---

## LOS TIPOS DE TOQUE (TIPOS DE MENSAJE)

```
2 TROMPETAS + TOQUE CONTINUO (teqiʻah):
  → Broadcast a TODA la congregación
  → "Reunión general — todos escuchen"
  → Mensaje comunitario normal

1 TROMPETA + TOQUE CONTINUO:
  → Broadcast solo a LÍDERES (nesiím)
  → "Reunión de líderes — solo grupo autorizado"
  → Mensaje a grupo específico

2 TROMPETAS + ALARMA (teruʻah):
  → EMERGENCIA para toda la congregación
  → "Peligro — acción inmediata"
  → Prioridad máxima, todos responden

TOQUE EN MOEDÍM:
  → Notificación de fiesta / día señalado
  → "Hoy es Shabat / Yom Teruah / etc."
  → Sincronizado con el calendario (Añadidura 03)

TOQUE EN GUERRA:
  → Señal de combate / amenaza externa
  → "Enemigo detectado — defensa activa"
  → Integrado con Modo Tsofeh (Añadidura 02)
```

---

## REGLA CLAVE: NO CUALQUIERA TOCA

Números 10:8 — "Los hijos de Aarón, los SACERDOTES, tocarán las trompetas."

No cualquier nodo puede hacer broadcast a toda la comunidad.
Solo los nodos con rol de sacerdote (Kohen) pueden enviar a todos.
Esto evita spam, abuso, y ruido en la red.

Un nodo Levita puede enviar a su grupo.
Un nodo base puede enviar mensajes directos.
Pero broadcast total = solo sacerdotes.

---

## DÓNDE VA EN EL FIRMWARE

**Pieza 04 — Menorá (hashmal_comm)** — ya tiene `hashmal_comm_broadcast`.
Se integra con **Pieza 09 — Vestiduras (hashmal_roles)** para verificar
que solo sacerdotes pueden hacer broadcast total.

Nuevo enum: tipos de toque (teqiʻah normal, teruʻah alarma, moedím, guerra).

---

*"Y serán para vosotros por estatuto perpetuo por vuestras generaciones."*
*— Números 10:8*
