# 14 — SHEMITAH Y YOVEL — Reset Periódico (Libertad en la Tierra)

> *"Y en el año séptimo la tierra tendrá SHABAT de reposo,
> Shabat para YHWH."*
> — Levítico 25:4

> *"Y santificaréis el año CINCUENTA, y proclamaréis LIBERTAD
> en la tierra a todos sus moradores."*
> — Levítico 25:10

---

## QUÉ ES

El protocolo de reset periódico de la red. Cada 7 épocas,
la red hace SHEMITAH — limpieza de penalizaciones menores,
revisión de restricciones. Cada 49 épocas, YOVEL — Jubileo:
reset profundo, toda deuda cancelada, toda restricción liberada.

---

## RESPALDO BÍBLICO

| # | Cita | Hebreo | Strong | Dice |
|---|------|--------|--------|------|
| 1 | Levítico 25:3-4 | שֵׁשׁ שָׁנִים תִּזְרַע... וּבַשָּׁנָה הַשְּׁבִיעִת שַׁבַּת שַׁבָּתוֹן | H8337, H7637, H7676 | "SEIS años sembrarás... el SÉPTIMO será Shabat de reposo" — ciclo 6+1 |
| 2 | Levítico 25:4 | שָׂדְךָ לֹא תִזְרָע וְכַרְמְךָ לֹא תִזְמֹר | H7704, H3754 | "Tu campo NO sembrarás, tu viña NO podarás" — la red PARA de penalizar |
| 3 | Levítico 25:8 | וְסָפַרְתָּ לְךָ שֶׁבַע שַׁבְּתֹת שָׁנִים שֶׁבַע שָׁנִים שֶׁבַע פְּעָמִים | H7651 | "Contarás 7 sábados de años, 7 años 7 VECES" = 49 — mega-ciclo |
| 4 | Levítico 25:10 | וּקְרָאתֶם דְּרוֹר בָּאָרֶץ לְכָל יֹשְׁבֶיהָ | H1865, H7121 | "Proclamaréis LIBERTAD (deror) en la tierra a TODOS sus moradores" — reset total |
| 5 | Levítico 25:10 | יוֹבֵל הִוא תִּהְיֶה לָכֶם וְשַׁבְתֶּם אִישׁ אֶל אֲחֻזָּתוֹ | H3104, H272 | "JUBILEO (yovel) será — cada uno volverá a su POSESIÓN" — restauración original |
| 6 | Levítico 25:13 | בִּשְׁנַת הַיּוֹבֵל הַזֹּאת תָּשֻׁבוּ אִישׁ אֶל אֲחֻזָּתוֹ | H7725 | "En el año del Jubileo VOLVERÉIS cada uno a su posesión" — estado original |
| 7 | Deuteronomio 15:1-2 | מִקֵּץ שֶׁבַע שָׁנִים תַּעֲשֶׂה שְׁמִטָּה... שָׁמוֹט כָּל בַּעַל מַשֵּׁה יָדוֹ | H8059, H8058 | "Cada 7 años harás SHEMITAH... SOLTARÁ todo acreedor" — perdón de deudas |

---

## LOS 2 CICLOS DE RESET

```
SHEMITAH — Cada 7 épocas:
  "Cada 7 años harás remisión" (Dt 15:1).
  
  La red hace:
  1. REVISAR todas las penalizaciones activas.
     Nodos en warning → se limpia el warning.
     Nodos en throttle → se revisa si merecen liberación.
     "Soltará todo acreedor lo que prestó."
  
  2. LIMPIAR datos acumulados innecesarios.
     Logs antiguos → purgados.
     Cachés obsoletos → limpiados.
     "La tierra descansará."
  
  3. EVALUAR la salud general de la red.
     ¿Hay nodos que llevan 7 épocas penalizados?
     Si sí → caso a caso: ¿merecen otra oportunidad?
     "Tendrá Shabat de reposo."

YOVEL (JUBILEO) — Cada 49 épocas (7 × 7):
  "Santificaréis el año cincuenta y proclamaréis LIBERTAD" (Lv 25:10).
  
  La red hace:
  1. RESET TOTAL de penalizaciones.
     TODOS los bans temporales → liberados.
     TODOS los throttles → eliminados.
     TODOS los warnings → limpiados.
     "Proclamaréis deror (libertad) a TODOS sus moradores."
  
  2. RESTAURACIÓN de posiciones originales.
     Nodos que fueron movidos de rol → vuelven a su rol original.
     Rutas que fueron modificadas → se recalculan desde cero.
     "Cada uno volverá a su posesión."
  
  3. SINCRONIZACIÓN PROFUNDA.
     Todos los nodos sincronizan su estado completo.
     Es como Yom Kippur (Lv 16) pero para toda la estructura.
     La red "renace" limpia.

EXCEPCIÓN — NIKRAT NO SE PERDONA:
  El Jubileo libera penalizaciones TEMPORALES.
  Pero nikrat (expulsión por las 4 violaciones de Éx 30:33,38)
  es PERMANENTE. El Jubileo no deshace el karát.
  La tierra vuelve, pero el expulsado no vuelve.
```

---

## POR QUÉ ESTO IMPORTA

Sin reset periódico, las penalizaciones se ACUMULAN para siempre.
Un nodo que cometió un error menor hace 100 épocas sigue penalizado.
La red se vuelve cada vez más RESTRICTIVA, cada vez con menos nodos
activos, hasta que muere por falta de participantes.

El Padre lo resolvió: cada 7 épocas, PERDÓN. Cada 49, LIBERTAD TOTAL.
La red se renueva. Los que aprendieron la lección vuelven.
Solo los que cometieron las 4 violaciones graves quedan fuera.

Es MISERICORDIA codificada en protocolo.

---

## DÓNDE VA EN EL FIRMWARE

**Pieza 14 — Incienso (hashmal_firewall)** para el reset de penalizaciones.
**Pieza 03 — Mesa (hashmal_cache)** para la limpieza de datos (épocas Shabat).
**Pieza 12 — Lavacro (hashmal_healthcheck)** para la evaluación de salud.
**Añadidura 03 — Calendario Moedím** para el conteo de épocas.

Nuevos mecanismos:
- `HASHMAL_SHEMITAH_CONTADOR` — cuenta épocas (0-6, reset en 7)
- `HASHMAL_YOVEL_CONTADOR` — cuenta shemitot (0-6, Jubileo en 7)
- `HASHMAL_SHEMITAH_EJECUTAR` — limpieza de penalties menores
- `HASHMAL_YOVEL_EJECUTAR` — reset total (excepto nikrat)

---

*"Y satisfaré a los moradores de Sion de pan."*
*— Salmo 132:15*
