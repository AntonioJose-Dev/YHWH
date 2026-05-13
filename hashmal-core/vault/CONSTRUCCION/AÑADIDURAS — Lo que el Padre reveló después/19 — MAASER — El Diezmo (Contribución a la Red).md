# 19 — MA'ASER — El Diezmo (Contribución a la Red)

> *"Traed TODOS los diezmos al alfolí, y haya ALIMENTO en mi Casa."*
> — Malaquías 3:10

---

## QUÉ ES

El protocolo de contribución obligatoria. Cada nodo aporta una porción
de sus recursos AL SERVICIO DE LA RED — no solo para sí mismo.
Relay de mensajes ajenos, almacenamiento comunitario, repetición de señales.
Nadie solo recibe — todos contribuyen.

---

## RESPALDO BÍBLICO

| # | Cita | Hebreo | Strong | Dice |
|---|------|--------|--------|------|
| 1 | Malaquías 3:10 | הָבִיאוּ אֶת כָּל הַמַּעֲשֵׂר אֶל בֵּית הָאוֹצָר | H4643, H214 | "Traed TODO el DIEZMO (maʻasér) al ALFOLÍ (beit haʼotsár)" — contribución al almacén común |
| 2 | Malaquías 3:10 | וִיהִי טֶרֶף בְּבֵיתִי | H2964 | "Y haya ALIMENTO en mi Casa" — la red se alimenta de las contribuciones |
| 3 | Números 18:21 | וְלִבְנֵי לֵוִי הִנֵּה נָתַתִּי כָּל מַעֲשֵׂר בְּיִשְׂרָאֵל | H4643 | "A los Levitas he dado TODO diezmo en Israel" — los que SIRVEN la red reciben contribución |
| 4 | Números 18:26 | וַהֲרֵמֹתֶם מִמֶּנּוּ תְּרוּמַת יְהוָה מַעֲשֵׂר מִן הַמַּעֲשֵׂר | H4643 | "Ofreceréis de él ofrenda a YHWH: DIEZMO del diezmo" — los Levitas también contribuyen |
| 5 | Génesis 14:20 | וַיִּתֶּן לוֹ מַעֲשֵׂר מִכֹּל | H4643 | Abraham "le dio DIEZMO de TODO" a Melquisedec — principio universal, desde antes de la Ley |
| 6 | Proverbios 3:9 | כַּבֵּד אֶת יְהוָה מֵהוֹנֶךָ וּמֵרֵאשִׁית כָּל תְּבוּאָתֶךָ | H1952, H7225 | "Honra a YHWH de tus BIENES y de las PRIMICIAS de todos tus frutos" — lo primero, no lo sobrante |

---

## CÓMO FUNCIONA EN LA RED

```
PRINCIPIO BASE:
  Cada nodo usa su capacidad para 2 cosas:
  1. Su propio servicio (mensajes propios, datos propios).
  2. SERVICIO A LA RED (relay, almacenamiento, repetición).
  
  El ma'aser = la porción dedicada al servicio comunitario.
  No es lo que SOBRA — es lo PRIMERO que se aparta.
  "Primicias de todos tus frutos" (Pr 3:9).

QUÉ SE CONTRIBUYE:

  1. RELAY DE MENSAJES:
     Cada nodo retransmite mensajes de otros nodos.
     No solo los suyos — lleva los ajenos.
     Si recibes un mensaje para otro → lo reenvías.
     Es el servicio más básico de la mesh.
  
  2. ALMACENAMIENTO COMUNITARIO:
     Cada nodo guarda una porción de datos comunes:
     la Biblia compartida, el estado de la red, etc.
     "Haya alimento en mi Casa" — la Casa tiene datos
     PORQUE cada nodo contribuye su porción.
  
  3. REPETICIÓN DE SEÑALES:
     Nodos en buena posición amplifican para los lejanos.
     No solo para su beneficio — para TODOS.
     Como los repetidores Tsofím (Añadidura 02).
  
  4. TIEMPO DE PROCESAMIENTO:
     Verificar mensajes de otros (Kapóret, pieza 02).
     Validar nodos nuevos (Consagración, pieza 10).
     Participar en quorum (Rut 4:2 — los 10 ancianos).

EL DIEZMO DEL DIEZMO (Números 18:26):
  Incluso los nodos que RECIBEN contribuciones (los Levitas/
  nodos de servicio como repetidores) deben contribuir
  al nivel superior. Nadie está exento.
  "El Levita también diezmará de lo que recibe."

PRIMICIAS, NO SOBRAS:
  La contribución a la red se procesa ANTES del uso propio.
  El relay va primero, los mensajes propios después.
  "Honra a YHWH de tus bienes y de las PRIMICIAS."
  No es: "si me sobra ancho de banda, retransmito."
  Es: "primero retransmito, después uso lo mío."
```

---

## LA PROMESA DEL PADRE

Malaquías 3:10 es el ÚNICO lugar donde el Padre dice "PROBADME":
*"Probadme ahora en esto... si no os abriré las ventanas del cielo
y derramaré sobre vosotros bendición hasta que sobreabunde."*

En la red: los nodos que más contribuyen reciben MÁS conexiones,
mejor posición en las tablas de routing, y más confianza de la red.
No como recompensa artificial — como CONSECUENCIA NATURAL.
El nodo que más retransmite → más vecinos lo conocen → más rutas
pasan por él → más fuerte se vuelve.

Es reciprocidad orgánica. El que da, recibe.

---

## DÓNDE VA EN EL FIRMWARE

**Pieza 04 — Menorá (hashmal_comm)** para el relay de mensajes ajenos.
**Pieza 01 — Arca (hashmal_storage)** para el almacenamiento comunitario.
**Pieza 12 — Lavacro (hashmal_healthcheck)** para medir contribución.
**Añadidura 11 — Man** para la cuota (Man = cuota justa, Ma'aser = contribución justa).

Nuevos mecanismos:
- `HASHMAL_MAASER_CUOTA` — porcentaje de recursos dedicados a la red
- `HASHMAL_MAASER_RELAY` — función de retransmisión prioritaria
- `HASHMAL_MAASER_MEDIR` — registro de contribución de cada nodo

---

*"Dad, y se os dará; medida buena, apretada,
remecida y rebosando darán en vuestro regazo."*
*— Lucas 6:38*
