# 11 — MAN (Maná) — Gestión de Recursos (Un Omer por Cabeza)

> *"Recoged de él cada uno según lo que pueda COMER;
> un OMER por cabeza, conforme al número de vuestras personas."*
> — Éxodo 16:16

---

## QUÉ ES

El protocolo de gestión de recursos de la red. Cada nodo recibe
lo que NECESITA — ni más, ni menos. No se acapara. No se acumula
más de lo debido. Y antes del descanso, se prepara doble.

---

## RESPALDO BÍBLICO

| # | Cita | Hebreo | Strong | Dice |
|---|------|--------|--------|------|
| 1 | Éxodo 16:16 | לִקְטוּ מִמֶּנּוּ אִישׁ לְפִי אָכְלוֹ עֹמֶר לַגֻּלְגֹּלֶת | H3950, H6016, H1538 | "Recoged cada uno según su COMER, un OMER por CABEZA" — cuota justa por nodo |
| 2 | Éxodo 16:18 | וְלֹא הֶעְדִּיף הַמַּרְבֶּה וְהַמַּמְעִיט לֹא הֶחְסִיר | H5736, H2637 | "El que recogió MUCHO no le SOBRÓ, el que recogió POCO no le FALTÓ" — distribución equitativa |
| 3 | Éxodo 16:19 | אִישׁ אַל יוֹתֵר מִמֶּנּוּ עַד בֹּקֶר | H3498 | "NADIE deje de él hasta mañana" — no acaparar, usar y renovar |
| 4 | Éxodo 16:20 | וַיָּרֻם תּוֹלָעִים וַיִּבְאַשׁ | H7311, H8438 | "Crió GUSANOS y APESTÓ" — lo acaparado se corrompe |
| 5 | Éxodo 16:22 | לָקְטוּ לֶחֶם מִשְׁנֶה | H4932 | "Recogieron pan DOBLE" el sexto día — preparación antes del descanso |
| 6 | Éxodo 16:26 | שֵׁשֶׁת יָמִים תִּלְקְטוּהוּ וּבַיּוֹם הַשְּׁבִיעִי שַׁבָּת לֹא יִהְיֶה בּוֹ | H7676 | "SEIS días recogeréis, el SÉPTIMO es Shabat — no habrá" — ciclo de uso y descanso |
| 7 | Éxodo 16:4 | וְלָקְטוּ דְּבַר יוֹם בְּיוֹמוֹ | H3117 | "Recogerán la PORCIÓN de cada DÍA en su día" — recurso diario, no acumulativo |

---

## LAS 5 REGLAS DEL MANÁ PARA LA RED

```
REGLA 1 — UN OMER POR CABEZA (lefí ojló):
  Cada nodo tiene una CUOTA de recursos (ancho de banda, slots de mensaje,
  tiempo de transmisión). No importa si es un T-Deck Pro o un Heltec V3 —
  cada uno recibe según lo que puede PROCESAR.
  "Un omer por cabeza, conforme al número de vuestras personas."

REGLA 2 — NI SOBRA NI FALTA (lo heʻdíf, lo hejsír):
  El que tiene más capacidad no acapara más canal.
  El que tiene menos capacidad no se queda sin servicio.
  Rate limiting equitativo — cada nodo tiene su porción justa.
  "El que recogió mucho no le sobró, el que recogió poco no le faltó."

REGLA 3 — NO GUARDAR HASTA MAÑANA (al yotér):
  Los mensajes temporales, cachés de tránsito, datos de sesión
  EXPIRAN al final del ciclo. No se acumulan indefinidamente.
  Lo que se guarda de más → se corrompe (gusanos = corrupción de datos).
  "Nadie deje de él hasta mañana."

REGLA 4 — DOBLE ANTES DEL DESCANSO (léjem mishné):
  Antes de un ciclo de Shabat (Añadidura 03), los nodos hacen
  DOBLE sincronización — envían todo lo pendiente, confirman todo
  lo recibido. Porque durante el Shabat, la actividad se reduce.
  "Recogieron pan doble el sexto día."

REGLA 5 — PORCIÓN DIARIA (devár yom beyomó):
  Los recursos se asignan POR CICLO, no de forma permanente.
  Cada nuevo ciclo, cada nodo recibe su porción fresca.
  No hay "reservas permanentes" de ancho de banda.
  "La porción de cada día en su día."
```

---

## CÓMO FUNCIONA EN LA RED

```
CICLO NORMAL (días 1-5):
  Cada nodo tiene cuota diaria de:
  - Mensajes broadcast: X por ciclo (según rol)
  - Mensajes directos: Y por ciclo
  - Ancho de banda LoRa: Z bytes por ciclo
  - Slots de caché: según Mesa (pieza 03)

  Si un nodo intenta EXCEDER su cuota:
  → Rate limiter (misgéret de la Mesa) lo frena.
  → "El que recogió mucho no le sobró."

  Si un nodo NO usa toda su cuota:
  → No se acumula para el siguiente ciclo.
  → Se renueva fresca cada ciclo.
  → "No guardéis hasta mañana."

DÍA 6 (pre-Shabat):
  DOBLE sincronización.
  Todos los mensajes pendientes se envían.
  Todas las confirmaciones se completan.
  El caché se refresca (Levítico 24:8).
  "Recogieron pan doble."

DÍA 7 (Shabat):
  Actividad mínima — solo heartbeat y emergencias.
  No se asignan nuevas cuotas.
  La red DESCANSA.
  "El séptimo es Shabat — no habrá."

DATOS QUE EXPIRAN:
  Mensajes de tránsito no entregados → expiran al final del ciclo.
  Caché de routing temporal → se limpia.
  Lo que no se usa → no se guarda.
  "Crió gusanos y apestó."
```

---

## POR QUÉ ESTO IMPORTA

Sin este protocolo, un solo nodo podría:
- Inundar la red con mensajes (spam/DDoS)
- Acaparar todo el ancho de banda LoRa
- Acumular datos obsoletos que consumen memoria
- No respetar los ciclos de descanso de la red

El Padre lo resolvió en el desierto con el maná:
PORCIÓN JUSTA, RENOVABLE, SIN ACUMULACIÓN.

---

## DÓNDE VA EN EL FIRMWARE

**Pieza 03 — Mesa (hashmal_cache)** para la cuota de slots y expiración.
**Pieza 04 — Menorá (hashmal_comm)** para el rate limiting de mensajes.
**Pieza 14 — Incienso (hashmal_firewall)** para la penalización por exceso.
**Añadidura 03 — Calendario Moedím** para los ciclos de Shabat.

Nuevos mecanismos:
- `HASHMAL_MAN_CUOTA` — cuota por nodo por ciclo
- `HASHMAL_MAN_EXPIRACION` — TTL de datos temporales
- `HASHMAL_MAN_MISHNE` — doble sincronización pre-Shabat

---

*"Pan del cielo les diste en su hambre."*
*— Nehemías 9:15*