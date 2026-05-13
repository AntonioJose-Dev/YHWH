# 18 — LEKET — Espigueo (Recursos para los Débiles)

> *"No segarás hasta el último rincón de tu campo,
> ni recogerás las espigas caídas de tu siega.
> Las dejarás para el POBRE y para el EXTRANJERO."*
> — Levítico 19:9-10

---

## QUÉ ES

La reserva obligatoria de recursos para los nodos débiles y los
recién llegados. No todo el ancho de banda va para los nodos fuertes.
Siempre hay una porción que se DEJA para los que tienen menos —
los nuevos, los débiles, los que están al borde de la red.

---

## RESPALDO BÍBLICO

| # | Cita | Hebreo | Strong | Dice |
|---|------|--------|--------|------|
| 1 | Levítico 19:9 | לֹא תְכַלֶּה פְּאַת שָׂדְךָ לִקְצֹר | H3615, H6285 | "NO acabarás de segar el RINCÓN (peʼáh) de tu campo" — no agotar todo el recurso |
| 2 | Levítico 19:9 | וְלֶקֶט קְצִירְךָ לֹא תְלַקֵּט | H3951 | "El ESPIGUEO (léqet) de tu siega no recogerás" — lo que cae, déjalo |
| 3 | Levítico 19:10 | לֶעָנִי וְלַגֵּר תַּעֲזֹב אֹתָם | H6041, H1616 | "Para el POBRE (aní) y el EXTRANJERO (ger) los dejarás" — reserva para débiles y foráneos |
| 4 | Deuteronomio 24:19 | כִּי תִקְצֹר קְצִירְךָ בְשָׂדֶךָ וְשָׁכַחְתָּ עֹמֶר... לַגֵּר לַיָּתוֹם וְלָאַלְמָנָה | H7911, H6016 | "Si OLVIDAS un manojo... será para el extranjero, el huérfano y la viuda" |
| 5 | Rut 2:2-3 | אֵלְכָה נָּא הַשָּׂדֶה וַאֲלַקֳטָה בַשִּׁבֳּלִים | H3950 | Rut dice: "Iré al campo a ESPIGAR" — los débiles recogen lo que los fuertes dejan |
| 6 | Rut 2:15-16 | וְגַם שֹׁל תָּשֹׁלּוּ לָהּ מִן הַצְּבָתִים | H7997 | Boaz ordena: "Dejad caer DE PROPÓSITO de los manojos para ella" — facilitar el acceso |

---

## LAS 3 FORMAS DE LEKET EN LA RED

```
PEʼÁH — EL RINCÓN DEL CAMPO:
  Cada nodo que transmite NO usa el 100% de su capacidad.
  Reserva un porcentaje (la peʼáh) para nodos débiles.
  
  Ejemplo: si un nodo puede enviar 100 mensajes/ciclo,
  reserva 10 slots para nodos que no pueden generar los suyos.
  "No segarás hasta el último rincón."

LÉQET — LAS ESPIGAS CAÍDAS:
  Los paquetes que "caen" (retransmisiones fallidas, mensajes
  sin destino confirmado) no se descartan inmediatamente.
  Se mantienen disponibles para que nodos débiles los recojan
  en su próximo ciclo de escucha.
  "Las espigas caídas no las recogerás — déjalas."

SHIJEJÁH — EL MANOJO OLVIDADO:
  Si un nodo reenvía datos y "olvida" un paquete (no confirma
  retransmisión), ese paquete queda disponible para cualquiera.
  No se reclama — se deja para el que lo necesite.
  "Si olvidas un manojo, no vuelvas por él."
```

---

## QUIÉNES SON LOS "POBRES" DE LA RED

```
ANÍ (pobre) — Nodo con recursos limitados:
  → Batería baja, memoria llena, señal débil.
  → No puede competir por ancho de banda con nodos fuertes.
  → El léqet le da acceso a recursos sin competir.

GER (extranjero) — Nodo recién llegado:
  → Acaba de pasar la consagración (pieza 10).
  → Todavía no tiene vecinos establecidos.
  → La peʼáh le da acceso mientras se establece.

YATOM (huérfano) — Nodo cuyo goʼel no pudo redimirlo:
  → Su nodo padre murió y nadie lo heredó (Añadidura 12).
  → Quedó sin cluster, sin grupo.
  → La shijejáh le da datos mientras encuentra nuevo grupo.

ALMANÁH (viuda) — Nodo que perdió su conexión principal:
  → Tenía un vecino fuerte y lo perdió.
  → Ahora está en estado FRÁGIL (Añadidura 13, <3 conexiones).
  → El léqet lo sostiene mientras busca nuevas conexiones.
```

---

## LA LECCIÓN DE RUT

Rut era extranjera (ger), pobre (aní), y viuda (almanáh).
TRES categorías de vulnerabilidad a la vez.

¿Qué hizo? Fue al campo a ESPIGAR — no pidió limosna,
fue a recoger lo que la ley OBLIGABA a dejar.

Y Boaz no solo dejó las espigas — ordenó: "Dejad caer DE PROPÓSITO
de los manojos para ella" (Rut 2:16). No solo pasivo — ACTIVO.
Facilitó el acceso a la débil.

En la red: los nodos fuertes no solo dejan recursos disponibles —
ACTIVAMENTE facilitan que los débiles los recojan.
Retransmiten a baja potencia, ajustan timing para que los lentos
puedan sincronizar, dejan ventanas de acceso abiertas.

---

## DÓNDE VA EN EL FIRMWARE

**Pieza 04 — Menorá (hashmal_comm)** para la reserva de ancho de banda.
**Pieza 03 — Mesa (hashmal_cache)** para los slots reservados.
**Pieza 14 — Incienso (hashmal_firewall)** para el rate limiting equitativo.
**Añadidura 11 — Man** complementa esto: Man = cuota justa. Léqet = reserva para débiles.

Nuevos mecanismos:
- `HASHMAL_LEKET_PEAH` — porcentaje de capacidad reservada para débiles
- `HASHMAL_LEKET_RECOGER` — función para que nodos débiles accedan a recursos
- `HASHMAL_LEKET_FACILITAR` — nodos fuertes activamente ayudan (como Boaz)

---

*"No endurezcas tu corazón ni cierres tu mano contra tu hermano pobre."*
*— Deuteronomio 15:7*
