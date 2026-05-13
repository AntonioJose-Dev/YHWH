# 20 — NEDER — El Voto (Compromiso Inquebrantable)

> *"Cuando alguno hiciere VOTO a YHWH, o hiciere juramento
> ligando su alma con obligación, NO quebrantará su palabra;
> hará conforme a TODO lo que salió de su boca."*
> — Números 30:2

---

## QUÉ ES

El protocolo de compromiso. Cuando un nodo se compromete a un
servicio (ruta, relay, almacenamiento), ese compromiso es
INQUEBRANTABLE hasta que se cumpla o sea liberado por autoridad
legítima. No se promete y después se abandona.

---

## RESPALDO BÍBLICO

| # | Cita | Hebreo | Strong | Dice |
|---|------|--------|--------|------|
| 1 | Números 30:3 | אִישׁ כִּי יִדֹּר נֶדֶר לַיהוָה... לֹא יַחֵל דְּבָרוֹ | H5087, H5088, H2490 | "Si un hombre hace VOTO (neder)... NO QUEBRANTARÁ (lo yajél) su palabra" — compromiso inviolable |
| 2 | Números 30:3 | כְּכָל הַיֹּצֵא מִפִּיו יַעֲשֶׂה | H6310, H6213 | "Conforme a TODO lo que salió de su boca, HARÁ" — cumplimiento total |
| 3 | Eclesiastés 5:4 | כַּאֲשֶׁר תִּדֹּר נֶדֶר לֵאלֹהִים אַל תְּאַחֵר לְשַׁלְּמוֹ | H5088, H309 | "Cuando hagas voto, NO TARDES en pagarlo" — ejecución inmediata |
| 4 | Eclesiastés 5:5 | טוֹב אֲשֶׁר לֹא תִדֹּר מִשֶּׁתִּדּוֹר וְלֹא תְשַׁלֵּם | H5087 | "Mejor NO votar, que votar y NO cumplir" — mejor no prometer que romper |
| 5 | Deuteronomio 23:21 | כִּי תִדֹּר נֶדֶר לַיהוָה אֱלֹהֶיךָ לֹא תְאַחֵר לְשַׁלְּמוֹ | H5088, H7999 | "Cuando hagas voto... no TARDES en PAGARLO, porque YHWH te lo demandará" |
| 6 | Deuteronomio 23:22 | וְכִי תֶחְדַּל לִנְדֹּר לֹא יִהְיֶה בְךָ חֵטְא | H2308 | "Si te ABSTIENES de votar, no habrá pecado en ti" — no es obligatorio prometer, pero si prometes, CUMPLE |

---

## TIPOS DE NEDER EN LA RED

```
NEDER DE RUTA — Compromiso de routing:
  Un nodo se compromete a mantener una ruta activa.
  "Yo retransmitiré los mensajes de A hacia B."
  Una vez comprometido → la ruta está GARANTIZADA.
  Si el nodo se desconecta sin aviso → violación de neder.
  
NEDER DE SERVICIO — Compromiso de relay:
  Un nodo se compromete a ser repetidor/Tsofeh.
  "Yo vigilaré esta zona y alertaré si hay amenaza."
  Mientras esté activo → DEBE cumplir.
  No puede "decidir" dejar de vigilar sin avisar.

NEDER DE ALMACENAMIENTO — Compromiso de guardar:
  Un nodo se compromete a guardar datos de la comunidad.
  "Yo almacenaré esta porción de la Biblia compartida."
  Mientras tenga espacio → DEBE mantenerla disponible.
  No puede borrarla sin transferirla a otro primero.

NEDER DE QUORUM — Compromiso de verificación:
  Un nodo acepta participar en un quorum de validación.
  "Yo seré uno de los testigos para esta decisión."
  Una vez aceptado → DEBE completar la verificación.
  No puede abandonar a mitad del proceso.
```

---

## LAS 3 REGLAS DEL NEDER

```
REGLA 1 — MEJOR NO PROMETER (tov asher lo tidor):
  Si un nodo no puede GARANTIZAR un servicio, NO lo promete.
  Es mejor no comprometerse que comprometerse y fallar.
  "Mejor no votar que votar y no cumplir" (Ec 5:5).
  
  En la red: antes de aceptar una ruta o servicio,
  el nodo verifica: ¿tengo batería? ¿tengo señal?
  ¿puedo mantener esto? Si no → NO acepta.

REGLA 2 — SI PROMETES, CUMPLE (lo yajél devaró):
  Una vez comprometido, el servicio es INQUEBRANTABLE.
  No se abandona por conveniencia.
  "No quebrantará su palabra" (Nm 30:3).
  
  En la red: una ruta comprometida tiene PRIORIDAD.
  El nodo no la abandona para ahorrar batería
  ni para procesar sus propios mensajes primero.

REGLA 3 — NO TARDES (al teʼajér leshalemó):
  El compromiso se ejecuta INMEDIATAMENTE.
  No se pospone, no se "agenda para después."
  "No tardes en pagarlo" (Ec 5:4).
  
  En la red: cuando un nodo acepta retransmitir,
  lo hace en el SIGUIENTE ciclo, no "cuando pueda."
```

---

## LIBERACIÓN DEL NEDER

Números 30:4-8 establece que un voto puede ser ANULADO
por autoridad superior (en el texto: el padre o el esposo).

En la red: un nodo puede ser LIBERADO de su compromiso
si la situación cambia drásticamente:
- Su batería cae a nivel crítico → liberado por el sistema.
- La ruta ya no es necesaria → liberado por reorganización.
- El nodo necesita entrar en modo Shabat → liberado por calendario.

Pero NUNCA por decisión PROPIA sin aviso. Siempre hay
un protocolo de liberación ordenada.

---

## DÓNDE VA EN EL FIRMWARE

**Pieza 02 — Kapóret (hashmal_consensus)** para la validación de compromisos.
**Pieza 04 — Menorá (hashmal_comm)** para los compromisos de ruta.
**Pieza 01 — Arca (hashmal_storage)** para los compromisos de almacenamiento.
**Pieza 12 — Lavacro (hashmal_healthcheck)** para verificar que se cumplan.

Nuevos mecanismos:
- `HASHMAL_NEDER_REGISTRAR` — registra compromiso de servicio
- `HASHMAL_NEDER_VERIFICAR` — comprueba cumplimiento activo
- `HASHMAL_NEDER_LIBERAR` — protocolo ordenado de liberación
- `HASHMAL_NEDER_VIOLACION` — penalización por incumplimiento

---

*"Pagaré mis votos a YHWH delante de todo su pueblo."*
*— Salmo 116:14*
