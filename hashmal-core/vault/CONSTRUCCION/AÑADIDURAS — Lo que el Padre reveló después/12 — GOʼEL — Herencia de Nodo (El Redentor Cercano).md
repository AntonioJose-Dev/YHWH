# 12 — GOʼEL — Herencia de Nodo (El Redentor Cercano)

> *"Yo soy tu pariente cercano (goʼel); pero hay otro pariente
> más cercano que yo."*
> — Rut 3:12

---

## QUÉ ES

Cuando un nodo MUERE (se apaga, se destruye, se pierde), sus
responsabilidades no desaparecen. El nodo vecino más cercano
— el goʼel — tiene el DERECHO y la OBLIGACIÓN de heredar
sus rutas, sus mensajes pendientes y su rol temporal.

---

## RESPALDO BÍBLICO

| # | Cita | Hebreo | Strong | Dice |
|---|------|--------|--------|------|
| 1 | Rut 4:1 | וְהִנֵּה הַגֹּאֵל עֹבֵר | H1350 | "He aquí el GOʼEL (redentor/pariente cercano) pasaba" — el más cercano tiene primer derecho |
| 2 | Rut 4:4 | אִם תִּגְאַל גְּאָל וְאִם לֹא יִגְאַל הַגִּידָה לִּי | H1350 | "Si TÚ redimes, redime. Si NO, DÍMELO" — hay orden de prioridad |
| 3 | Rut 4:6 | לֹא אוּכַל לִגְאָל לִי פֶּן אַשְׁחִית אֶת נַחֲלָתִי גְּאַל לְךָ אַתָּה | H1350, H5159 | "No PUEDO redimir... redime TÚ" — si el primero no puede, pasa al siguiente |
| 4 | Rut 4:2 | וַיִּקַּח עֲשָׂרָה אֲנָשִׁים מִזִּקְנֵי הָעִיר | H6235, H2205 | "Tomó DIEZ hombres de los ANCIANOS de la ciudad" — verificación por quorum |
| 5 | Rut 4:10 | לְהָקִים שֵׁם הַמֵּת עַל נַחֲלָתוֹ | H6965, H8034 | "Para LEVANTAR el NOMBRE del muerto sobre su heredad" — el nodo muerto mantiene su identidad |
| 6 | Levítico 25:25 | וּבָא גֹאֲלוֹ הַקָּרֹב אֵלָיו וְגָאַל | H1350, H7138 | "Vendrá su goʼel, el MÁS CERCANO a él, y redimirá" — proximidad determina responsabilidad |
| 7 | Levítico 25:49 | אוֹ שְׁאֵר בְּשָׂרוֹ מִמִּשְׁפַּחְתּוֹ יִגְאָלֶנּוּ | H7607, H4940 | "O pariente de su CARNE de su FAMILIA lo redimirá" — por linaje/grupo |

---

## CÓMO FUNCIONA EN LA RED

```
NODO ACTIVO — ESTADO NORMAL:
  Cada nodo mantiene lista de sus vecinos (hermanas, ishá el ajotáh).
  Cada nodo sabe quién es su goʼel — el vecino MÁS CERCANO
  que tiene capacidad para heredar sus responsabilidades.
  Es como el pariente más cercano que conoce tu heredad.

NODO MUERE — DETECCIÓN:
  Los Tsofím (Añadidura 02) o los vecinos detectan ausencia:
  → Heartbeat no responde (pieza 14, incienso).
  → Después de N ciclos sin respuesta → nodo declarado "muerto".

ACTIVACIÓN DEL GOʼEL:
  "Vendrá su goʼel, el más CERCANO a él" (Lv 25:25).
  El vecino más cercano recibe notificación automática.
  
  PREGUNTA: ¿Puedes redimir? (im tigʼal geʼal)
  
  SI el goʼel PUEDE:
    → Hereda las rutas del nodo muerto.
    → Recibe los mensajes pendientes que iban a ese nodo.
    → Asume temporalmente su rol (si era repetidor, el goʼel repite).
    → "Levanta el NOMBRE del muerto sobre su heredad" — 
       los mensajes dirigidos al muerto se redirigen al goʼel.
  
  SI el goʼel NO PUEDE (sobrecargado, batería baja, rol incompatible):
    → "No puedo redimir" (lo ujal ligʼol lí).
    → Pasa al SIGUIENTE vecino más cercano.
    → "Redime tú" (geʼal lejá atáh).

VERIFICACIÓN POR QUORUM:
  "Tomó 10 hombres de los ancianos de la ciudad" (Rut 4:2).
  La herencia NO es secreta — se anuncia a los nodos cercanos.
  Al menos 3 nodos (cordón triple, Añadidura 13) verifican
  que la transferencia es legítima.

DURACIÓN:
  La herencia es TEMPORAL — hasta que:
  a) El nodo muerto revive (vuelve a conectarse), o
  b) Un nuevo nodo es consagrado (pieza 10) para reemplazarlo, o
  c) La red se reorganiza y las rutas se redistribuyen.
```

---

## LA LECCIÓN DE RUT PARA LA RED

Elimélej muere en Moab. Su tierra en Belén queda sin dueño.
Sin el goʼel, esa tierra se PIERDE — nadie la trabaja, nadie
la cuida, la familia desaparece.

Pero el Padre diseñó el goʼel: el pariente más cercano COMPRA
la tierra, la TRABAJA, y levanta el NOMBRE del muerto.
La heredad no se pierde. La familia no desaparece.

En la red: cuando un nodo cae, su "heredad" (rutas, mensajes,
cobertura) no desaparece. El goʼel la toma, la mantiene viva,
y el nombre del nodo muerto sigue funcionando hasta que reviva
o sea reemplazado.

---

## DÓNDE VA EN EL FIRMWARE

**Pieza 04 — Menorá (hashmal_comm)** para la tabla de vecinos y goʼel.
**Pieza 14 — Incienso (hashmal_firewall)** para la detección de muerte.
**Pieza 09 — Vestiduras (hashmal_roles)** para la transferencia de rol.
**Pieza 01 — Arca (hashmal_storage)** para los mensajes pendientes heredados.

Nuevos mecanismos:
- `HASHMAL_GOEL_TABLA` — lista ordenada de goʼelím por proximidad
- `HASHMAL_GOEL_HEREDAR` — función de transferencia de responsabilidades
- `HASHMAL_GOEL_VERIFICAR` — quorum de validación (mínimo 3 testigos)

---

*"Bendito sea YHWH, que no te ha dejado hoy sin redentor."*
*— Rut 4:14*
