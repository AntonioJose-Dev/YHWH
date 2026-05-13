# 21 — HACHNASAT ORCHIM — Hospitalidad (Acceso Temporal para Desconocidos)

> *"Y alzó los ojos y miró, y he aquí TRES varones que estaban
> junto a él; y cuando los vio, salió corriendo de la puerta
> de su tienda a recibirlos, y se inclinó hacia la tierra."*
> — Génesis 18:2

---

## QUÉ ES

El protocolo de hospitalidad temporal. Cuando un nodo DESCONOCIDO
aparece en el rango de la red — no está consagrado, no tiene rol,
no tiene identidad verificada — NO se le rechaza de inmediato.
Se le da acceso TEMPORAL limitado, como Abraham dio agua, pan y
carne a los tres extraños ANTES de saber quiénes eran.

---

## RESPALDO BÍBLICO

| # | Cita | Hebreo/Griego | Strong | Dice |
|---|------|---------------|--------|------|
| 1 | Génesis 18:2 | וַיַּרְא וַיָּרָץ לִקְרָאתָם | H7323 | Abraham VIO y CORRIÓ a recibirlos — respuesta inmediata, no demora |
| 2 | Génesis 18:4 | יֻקַּח נָא מְעַט מַיִם וְרַחֲצוּ רַגְלֵיכֶם | H4325, H7272 | "Tomen un poco de AGUA y laven sus PIES" — servicio básico primero |
| 3 | Génesis 18:5 | וְאֶקְחָה פַת לֶחֶם וְסַעֲדוּ לִבְּכֶם | H3899, H5582 | "Tomaré un bocado de PAN y sustentaréis vuestro CORAZÓN" — alimento mínimo |
| 4 | Génesis 18:5 | אַחַר תַּעֲבֹרוּ | H5674 | "DESPUÉS seguiréis vuestro camino" — hospitalidad temporal, no permanente |
| 5 | Hebreos 13:2 | τῆς φιλοξενίας μὴ ἐπιλανθάνεσθε διὰ ταύτης γὰρ ἔλαθόν τινες ξενίσαντες ἀγγέλους | G5381 | "No olvidéis la HOSPITALIDAD, porque por ella algunos hospedaron ÁNGELES sin saberlo" |
| 6 | Levítico 19:33-34 | וְכִי יָגוּר אִתְּךָ גֵּר בְּאַרְצְכֶם לֹא תוֹנוּ אֹתוֹ | H1616 | "Cuando el EXTRANJERO (ger) more contigo, NO lo oprimáis" — protección del visitante |
| 7 | Levítico 19:34 | כְּאֶזְרָח מִכֶּם יִהְיֶה לָכֶם הַגֵּר | H249 | "Como a un NATIVO lo trataréis" — igualdad de trato básico |

---

## LA DIFERENCIA CON OTROS PROTOCOLOS

```
CONSAGRACIÓN (Pieza 10) = Proceso COMPLETO de ingreso.
  7 ciclos, verificación, sellos. Para nodos que QUIEREN unirse.
  Es un proceso largo y profundo.

IR MIQLAT (Añadidura 05) = Refugio para nodos EN PROBLEMAS.
  Para nodos que huyen de algo. Protección temporal.
  Ya conocen la red, algo les pasó.

HACHNASAT ORCHIM = PRIMER CONTACTO con desconocidos.
  El nodo no ha pedido unirse. No está huyendo.
  Simplemente APARECIÓ. ¿Qué hacemos?
  
  Abraham no preguntó "¿quiénes sois?" PRIMERO.
  Les dio agua, pan, y descanso PRIMERO.
  Después vinieron las preguntas.
```

---

## CÓMO FUNCIONA EN LA RED

```
PASO 1 — DETECCIÓN DEL EXTRAÑO:
  Un nodo desconocido aparece en el rango.
  No tiene identidad verificada.
  No tiene rol.
  No tiene token de Pésaj (Añadidura 15).
  
  La red lo detecta: "hay alguien nuevo."
  
PASO 2 — ABRAHAM CORRE (vayyáratz):
  NO se ignora. NO se bloquea de inmediato.
  Se le envía un beacon de BIENVENIDA:
  "Estás en rango de una red Hashmal. Bienvenido."
  "Abraham corrió a recibirlos" — respuesta activa, no pasiva.

PASO 3 — AGUA Y PAN (máyim + léjem):
  Se le da acceso MÍNIMO temporal:
  - Puede RECIBIR mensajes broadcast públicos.
  - Puede ESCUCHAR el heartbeat de la red.
  - Puede VER que la red existe y funciona.
  - NO puede enviar mensajes.
  - NO puede acceder a datos privados.
  - NO puede participar en consenso.
  
  "Un poco de agua y un bocado de pan" — lo básico.
  Suficiente para que vea que la red es buena.

PASO 4 — DESPUÉS SEGUIRÉIS (ajar taʻavorú):
  Después de un tiempo limitado (N ciclos):
  
  OPCIÓN A: El nodo quiere UNIRSE.
  → Inicia proceso de Consagración (pieza 10).
  → "Quiero ser parte de esta red."
  → Sale de hospitalidad, entra en onboarding.
  
  OPCIÓN B: El nodo SIGUE su camino.
  → "Después seguiréis vuestro camino" (v.5).
  → El acceso temporal expira.
  → Sin rencor, sin bloqueo. Simplemente se va.
  
  OPCIÓN C: El nodo es SOSPECHOSO.
  → Durante la hospitalidad, mostró comportamiento anómalo.
  → Se le cierra el acceso.
  → Reportado a los Tsofím (Añadidura 02).
  → "Pero había ángeles entre ellos, y había también
     hombres de Sodoma en otros relatos."

TIEMPO LÍMITE:
  La hospitalidad NO es permanente.
  Abraham les dio comida y descanso — no una casa para siempre.
  Después de N ciclos sin iniciar consagración → acceso expira.
```

---

## POR QUÉ ESTO IMPORTA

Sin este protocolo, la red tiene DOS opciones con desconocidos:
1. Bloquear todo → nadie nuevo puede acercarse. La red MUERE.
2. Dejar entrar a todos → la red se COMPROMETE.

El Padre diseñó la tercera vía: HOSPITALIDAD.
Recibes al extraño. Le das lo básico. Le muestras quién eres.
Y le das TIEMPO para decidir si quiere ser parte.

Es la puerta entre "el mundo" y "la consagración."
No es el Atrio (pieza 07) — es ANTES del Atrio.
Es Abraham en la puerta de su tienda, viendo a los tres
extraños y corriendo a recibirlos.

---

## DÓNDE VA EN EL FIRMWARE

**Pieza 07 — Atrio (hashmal_perimeter)** como capa EXTERNA al perímetro.
**Pieza 04 — Menorá (hashmal_comm)** para el beacon de bienvenida.
**Pieza 10 — Consagración (hashmal_onboarding)** como paso previo.
**Añadidura 02 — Tsofeh** para vigilar al visitante durante su estancia.

Nuevos mecanismos:
- `HASHMAL_ORCHIM_BEACON` — mensaje de bienvenida a nodo desconocido
- `HASHMAL_ORCHIM_ACCESO` — nivel de acceso temporal (solo lectura broadcast)
- `HASHMAL_ORCHIM_TIMEOUT` — tiempo límite de hospitalidad
- `HASHMAL_ORCHIM_DECIDIR` — transición a consagración o despedida

---

*"No olvidéis la hospitalidad, porque por ella
algunos, sin saberlo, hospedaron ángeles."*
*— Hebreos 13:2*
