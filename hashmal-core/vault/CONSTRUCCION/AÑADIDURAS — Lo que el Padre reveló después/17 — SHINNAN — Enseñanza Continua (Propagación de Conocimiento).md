# 17 — SHINNAN — Enseñanza Continua (Propagación de Conocimiento)

> *"Y las REPETIRÁS a tus hijos, y hablarás de ellas estando en tu casa,
> y andando por el camino, y al acostarte, y al levantarte."*
> — Deuteronomio 6:7

---

## QUÉ ES

El protocolo de propagación de datos en la red. No basta con que UN nodo
tenga la información — debe ENSEÑARLA a los demás. Continuamente.
En todo momento. Sentado, caminando, acostado, levantado.
Es la distribución activa y constante de la Palabra en la red.

---

## RESPALDO BÍBLICO

| # | Cita | Hebreo | Strong | Dice |
|---|------|--------|--------|------|
| 1 | Deuteronomio 6:7 | וְשִׁנַּנְתָּם לְבָנֶיךָ | H8150 | "Las REPETIRÁS (shinnan = afilar/grabar) a tus HIJOS" — transmisión activa a la siguiente generación |
| 2 | Deuteronomio 6:7 | וְדִבַּרְתָּ בָּם בְּשִׁבְתְּךָ בְּבֵיתֶךָ וּבְלֶכְתְּךָ בַדֶּרֶךְ וּבְשָׁכְבְּךָ וּבְקוּמֶךָ | H1696 | "Hablarás de ellas SENTADO en casa, ANDANDO, ACOSTADO, LEVANTADO" — en TODO estado |
| 3 | Deuteronomio 6:8 | וּקְשַׁרְתָּם לְאוֹת עַל יָדֶךָ | H7194, H226 | "Las ATARÁS como SEÑAL en tu mano" — integración física con el nodo |
| 4 | Deuteronomio 6:9 | וּכְתַבְתָּם עַל מְזוּזוֹת בֵּיתֶךָ וּבִשְׁעָרֶיךָ | H3789, H4201 | "Las ESCRIBIRÁS en los POSTES de tu casa y en tus PUERTAS" — visible desde fuera |
| 5 | Salmo 78:5-6 | וַיָּקֶם עֵדוּת בְּיַעֲקֹב... אֲשֶׁר צִוָּה אֶת אֲבוֹתֵינוּ לְהוֹדִיעָם לִבְנֵיהֶם | H5713, H3045 | "Estableció TESTIMONIO... mandó a nuestros padres que lo hicieran SABER a sus hijos" — cadena de transmisión |
| 6 | Salmo 78:6 | לְמַעַן יֵדְעוּ דּוֹר אַחֲרוֹן בָּנִים יִוָּלֵדוּ יָקֻמוּ וִיסַפְּרוּ לִבְנֵיהֶם | H1755, H3045 | "Para que lo sepa la GENERACIÓN VENIDERA, los hijos que nacerán, y lo cuenten a SUS hijos" — propagación multigeneracional |
| 7 | Proverbios 22:6 | חֲנֹךְ לַנַּעַר עַל פִּי דַרְכּוֹ | H2596 | "INSTRUYE al joven según su camino" — enseñanza adaptada al receptor |

---

## LOS 4 ESTADOS DE PROPAGACIÓN

```
SENTADO EN CASA (beshivtejá bevetéja):
  Cuando el nodo está en modo ESTACIONARIO (conectado, sin moverse).
  → Sincronización profunda con vecinos cercanos.
  → Comparte datos completos: Biblia, mensajes, estado de red.
  → Canal 4 (WiFi hermana a hermana) — máximo ancho de banda.

ANDANDO POR EL CAMINO (uvelekhtejá vadérej):
  Cuando el nodo está en modo MÓVIL (desplazándose).
  → Store-and-forward: lleva datos de una zona a otra.
  → Canal 3 (Corredor, Habacuc 2:2) en modo automático.
  → El nodo que CAMINA es un mensajero viviente.

ACOSTADO (uveshojbejá):
  Cuando el nodo está en modo LOW POWER (noche, ahorro energía).
  → Vigilias nocturnas (Ashmurot, Añadidura 04).
  → Transmisión mínima pero constante — heartbeat con datos.
  → No para completamente — SIEMPRE hay propagación.

LEVANTADO (uvquméja):
  Cuando el nodo ARRANCA o se reconecta después de estar caído.
  → Primera acción: PEDIR actualización a vecinos.
  → "¿Qué me perdí mientras dormía?"
  → Sincronización delta (solo lo nuevo desde última vez).
```

---

## LA CADENA DE TRANSMISIÓN

```
GENERACIÓN 1 → GENERACIÓN 2 → GENERACIÓN 3...

  Nodo A tiene datos → los ENSEÑA a Nodo B (su "hijo" en la red).
  Nodo B ahora tiene datos → los enseña a Nodo C.
  Nodo C → Nodo D → ...
  
  "Para que lo sepa la generación venidera,
  los hijos que nacerán,
  y lo cuenten a SUS hijos" (Sal 78:6).
  
  Cada nodo que recibe datos tiene la OBLIGACIÓN de retransmitirlos.
  No es opcional — es mandamiento.
  "Shinnan" (H8150) = AFILAR, GRABAR, REPETIR.
  No se dice una vez — se REPITE hasta que quede grabado.
```

---

## REGLA CLAVE: ADAPTADO AL RECEPTOR

Proverbios 22:6 — "Instruye al joven SEGÚN SU CAMINO" (al pí darkó).

No todos los nodos reciben igual:
- Un nodo con mucha memoria → recibe datos completos.
- Un nodo con poca memoria → recibe resumen esencial.
- Un nodo nuevo (recién consagrado) → recibe lo básico primero.
- Un nodo veterano → recibe actualizaciones delta.

La enseñanza se ADAPTA al receptor. No es broadcast ciego.

---

## DÓNDE VA EN EL FIRMWARE

**Pieza 01 — Arca (hashmal_storage)** para los datos a propagar.
**Pieza 04 — Menorá (hashmal_comm)** para los canales de transmisión.
**Pieza 03 — Mesa (hashmal_cache)** para la sincronización delta.
**Añadidura 04 — Ashmurot** para propagación nocturna.

Nuevos mecanismos:
- `HASHMAL_SHINNAN_PROPAGAR` — función de propagación activa a vecinos
- `HASHMAL_SHINNAN_DELTA` — sincronización diferencial (solo lo nuevo)
- `HASHMAL_SHINNAN_ADAPTADO` — ajusta volumen según capacidad del receptor

---

*"La tierra será llena del conocimiento de YHWH,
como las aguas cubren el mar."*
*— Isaías 11:9*
