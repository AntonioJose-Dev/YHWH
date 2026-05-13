# 09 — ESCALACIÓN — Primero Solo, Después Testigos, Después Todos

> *"Si tu hermano peca contra ti, ve y repréndelo estando tú y él SOLOS.
> Si te oye, has ganado a tu hermano."*
> — Mateo 18:15

---

## QUÉ ES

El protocolo de resolución de conflictos en la comunidad.
No se hace broadcast de los problemas — se escalan paso a paso.
Primero privado, después grupo pequeño, después toda la congregación.

---

## RESPALDO BÍBLICO

| # | Cita | Griego/Hebreo | Strong | Dice |
|---|------|---------------|--------|------|
| 1 | Mateo 18:15 | μεταξὺ σοῦ καὶ αὐτοῦ μόνου | G3342 | "ENTRE tú y él SOLO" — mensaje DIRECTO, privado |
| 2 | Mateo 18:16 | δύο ἢ τριῶν μαρτύρων | G3144 | "DOS o TRES testigos" — grupo pequeño (2-3 personas) |
| 3 | Mateo 18:17 | εἰπὲ τῇ ἐκκλησίᾳ | G1577 | "Dilo a la CONGREGACIÓN" — broadcast a todos |
| 4 | Mateo 18:17 | ὥσπερ ὁ ἐθνικός | G1482 | Si tampoco escucha → "sea como el gentil" — expulsión (nikrat) |
| 5 | Deuteronomio 19:15 | עַל פִּי שְׁנַיִם עֵדִים אוֹ... שְׁלֹשָׁה | H5707 | "Por boca de 2 ó 3 TESTIGOS se confirmará" — verificación |
| 6 | Proverbios 25:9 | רִיבְךָ רִיב אֶת רֵעֶךָ וְסוֹד אַחֵר אַל תְּגָל | H7453 | "Debate tu causa con tu prójimo, y NO descubras el secreto de otro" |

---

## LOS 3 NIVELES DE ESCALACIÓN

```
NIVEL 1 — SOLO (metaxú sou kai autou mónou):
  Mensaje DIRECTO entre 2 nodos.
  Cifrado punto a punto.
  Nadie más lo ve.
  "Ve y repréndelo estando tú y él SOLOS."
  → Si se resuelve aquí, fin. No sube.

NIVEL 2 — TESTIGOS (dúo é triōn martúrōn):
  Si Nivel 1 no funciona, se añaden 2 ó 3 testigos.
  Grupo pequeño de 3-4 nodos.
  Los testigos verifican ambas versiones.
  "Para que por boca de 2 ó 3 testigos conste toda palabra."
  → Si se resuelve aquí, fin. No sube.

NIVEL 3 — CONGREGACIÓN (ekklesía):
  Si Nivel 2 no funciona, se lleva a toda la comunidad.
  Broadcast a todos los nodos.
  La comunidad decide.
  "Si no los oye, dilo a la congregación."
  → Si tampoco escucha → nikrat (expulsión).
```

---

## POR QUÉ ESTO IMPORTA EN LA RED

Sin este protocolo, cualquier conflicto se vuelve público inmediatamente.
Eso destruye comunidades — lo hemos visto en Telegram, en grupos de WhatsApp,
en redes sociales. Un problema entre dos personas explota y divide a todos.

El Padre diseñó la solución: PRIMERO privado. La mayoría de problemas
se resuelven entre dos. Solo si falla, se escala. Y cada nivel tiene
más personas pero también más gravedad.

---

## DÓNDE VA EN EL FIRMWARE

Integrado con los tipos de mensaje de la Menorá (hashmal_comm):
- Nivel 1 = mensaje directo (destino = 1 nodo)
- Nivel 2 = mensaje a grupo (destino = grupo de 3-4)
- Nivel 3 = broadcast (destino = 0xFF, todos)

Y con el Firewall (hashmal_firewall) para la expulsión final si se llega a nikrat.

---

*"Si te oye, has ganado a tu hermano."*
*— Mateo 18:15*