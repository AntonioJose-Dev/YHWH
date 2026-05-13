# VERIFICADO #07 — PARTE II — Pieza 01: EL ARCA (Arón)

> Verificado con mil ojos como querubín. Quiasmos y hebreo directo del vault.
> Confirmado por Antonio José.
> Fecha: 25 abril 2026

---

## Claims de la auditoría para Pieza 01

| # | Claim de auditoría | Versículo | Implicación que sugiere |
|---|-------------------|-----------|------------------------|
| 1 | ¿Qué pasa con dato sin Kapóret? | Éx 25:21 | Estado "PENDING_VERIFICATION" necesario |
| 2 | Los COATITAS guardan el Arca | Nm 3:30-31 | ¿Nodos específicos responsables del almacenamiento? |
| 3 | Cubrir ANTES de mover | Nm 4:5-6 | Protocolo de migración: cifrar → mover → instalar |

---

## Claim 1 — "¿Qué pasa con dato sin Kapóret?" → AUDITORÍA INCOMPLETA

La auditoría sugiere que necesitamos un estado "PENDING_VERIFICATION." Pero el quiasmo de Éxodo 25:10-22 revela algo más profundo:

### Estructura quiasmática de Éxodo 25:10-22

```
A  — v.10: veʻasú arón (HARÁN un arca) — PLURAL, distribuido
  B  — v.11: zahav tahór mibáyit umijúts (oro dentro y fuera) — PROTECCIÓN
    C  — v.12: arbaʻ tabbeʻót zahav al arbaʻ paʻamotáv (4 anillos en 4 pies) — DISTRIBUCIÓN
      D  — v.13-14: baddé ʻatsé shittím, tsippitá otám zahav (varas de acacia, oro) — TRANSPORTE
        E  — v.15: beṭabbeʻót haʼarón yihyú habbaddím, LÓ YASÚRU mimménnu (en los anillos ESTARÁN las varas, NO SE QUITARÁN) — PERMANENCIA
          ★ F — v.16: venatatá el haʼarón et haʻEDÚT asher ettén eléikha — "PONDRÁS EN EL ARCA EL TESTIMONIO QUE YO TE DARÉ"
        E' — v.17: veʻasitá khappóret zahav tahór (HARÁS una kapóret de oro puro) — PERMANENCIA del consenso
      D' — v.18: shenáyim keruvím zahav, miqshá (dos querubines, BATIDOS) — ESTRUCTURA
    C' — v.19: keruv ejád miqqatsáh mizzéh, ukhruv ejád miqqatsáh mizzéh (querubín uno DE ESTE EXTREMO, querubín uno DE ESTE EXTREMO) — DISTRIBUCIÓN simétrica
  B' — v.20: sokhkhím bekhanfehém al hakkappóret (CUBRIENDO con sus alas la kapóret) — PROTECCIÓN
A' — v.21-22: venatatá et hakkappóret al haʼarón milmaʻláh... venoʻadtí lekhá shám (PONDRÁS la kapóret SOBRE el arca... y ME ENCONTRARÉ contigo allí) — PLURAL → ENCUENTRO
```

### El centro del quiasmo: ★ EDÚT (Testimonio) — v.16

El CENTRO no es la Kapóret. No es el Arca. Es el **EDÚT** — el Testimonio que YHWH da. Todo lo demás RODEA al Testimonio.

### Testigo 1 — Éxodo 25:16
> וְ/נָתַתָּ֖ אֶל הָ/אָרֹ֑ן אֵ֚ת הָ/עֵדֻ֔ת אֲשֶׁ֥ר אֶתֵּ֖ן אֵלֶֽי/ךָ
> "Y PONDRÁS (**venatatá** H5414 Qal perfecto) en el Arca el TESTIMONIO (**haʻedút** H5715) que Yo TE DARÉ (**ettén** H5414 Qal imperfecto)"

Observación crítica: **venatatá** (tú pondrás, PERFECTO = acción completada) + **ettén** (Yo daré, IMPERFECTO = acción continua). El Padre DA continuamente, Moisés PONE una vez. El Testimonio es flujo, no evento.

### Testigo 2 — Éxodo 25:21
> וְ/נָתַתָּ֧ אֶת הַ/כַּפֹּ֛רֶת עַל הָ/אָרֹ֖ן מִ/לְ/מָ֑עְלָ/ה וְ/אֶל הָ֣/אָרֹ֔ן תִּתֵּן֙ אֶת הָ֣/עֵדֻ֔ת אֲשֶׁ֥ר אֶתֵּ֖ן אֵלֶֽי/ךָ
> "Pondrás la Kapóret SOBRE el Arca **milmaʻláh** (desde arriba), y EN el Arca pondrás el Testimonio que Yo te daré"

El Padre dice PRIMERO "pon la Kapóret arriba" y LUEGO "en el Arca pon el Testimonio." El orden del texto hebreo en v.21 es: **Kapóret → luego → Edút**. La Kapóret debe estar LISTA (no necesariamente ya colocada, pero sí existente y preparada) ANTES de que el Testimonio entre.

### Lo que el quiasmo revela vs. lo que la auditoría sugiere

| Auditoría dice | El Padre dice |
|----------------|---------------|
| Estado "PENDING_VERIFICATION" | NO. No hay datos "pendientes." La Kapóret debe estar **READY** (existente, funcional) ANTES de que cualquier dato entre al Arca |
| El dato puede existir sin Kapóret temporalmente | NO. El quiasmo envuelve el Edút con capas de protección en AMBOS lados. Sin Kapóret, el dato NO ENTRA |
| Necesitamos un buffer de espera | El diseño es: primero construyes el sistema completo (Arca + Kapóret), luego entra el dato. No al revés |

**Corrección técnica:** `hashmal_storage` no acepta datos si `hashmal_consensus` no está READY. No es "pending verification" — es **PREREQUISITO DE EXISTENCIA**. El Arca sin Kapóret no almacena nada. Punto.

---

## Claim 2 — "Los COATITAS guardan el Arca" → AUDITORÍA CORRECTA PERO SUPERFICIAL

La auditoría ve "¿nodos específicos responsables?" Pero el texto revela una arquitectura de 3 capas de seguridad.

### Testigo 3 — Números 3:27-28
> וְ/לִ/קְהָ֗ת מִשְׁפַּ֤חַת הַֽ/עַמְרָמִי֙ וּ/מִשְׁפַּ֣חַת הַ/יִּצְהָרִ֔י וּ/מִשְׁפַּ֨חַת֙ הַֽ/חֶבְרֹנִ֔י וּ/מִשְׁפַּ֖חַת הָֽ/עָזִּיאֵלִ֑י אֵ֥לֶּה הֵ֖ם מִשְׁפְּחֹ֥ת הַ/קְּהָתִֽי
> "De Kehat: familia amramita, familia yitsharita, familia jebronita, familia uzielita — estas son las familias de los coatitas"

> בְּ/מִסְפַּר֙ כָּל זָכָ֔ר מִ/בֶּן חֹ֖דֶשׁ וָ/מָ֑עְלָ/ה שְׁמֹנַ֤ת אֲלָפִים֙ וְ/שֵׁ֣שׁ מֵא֔וֹת **שֹׁמְרֵ֖י מִשְׁמֶ֥רֶת הַ/קֹּֽדֶשׁ**
> "8.600 **GUARDADORES** (**shomré** H8104 Qal ptc activo) de la **CUSTODIA** (**mishmeret** H4931) del SANTO (**haqqódesh** H6944)"

**shomrím** (guardadores) + **mishmeret** (custodia/encargo sagrado). No son meros transportistas. Son CUSTODIOS.

### Testigo 4 — Números 3:29
> מִשְׁפְּחֹ֥ת בְּנֵי קְהָ֖ת יַחֲנ֑וּ עַ֛ל **יֶ֥רֶךְ** הַ/מִּשְׁכָּ֖ן **תֵּימָֽנָ/ה**
> "Las familias de los hijos de Kehat acamparán al **COSTADO** (**yérekh** H3409 = muslo, costado) del Mishkán al **SUR** (**teimánah** H8486)"

**yérekh** = muslo, la parte más cercana al cuerpo. Los coatitas no están "cerca" — están PEGADOS al Mishkán. Y al SUR — el mismo lado donde está la **Menorá** dentro del Santo. Custodian desde afuera lo que la Menorá ilumina desde adentro.

### Testigo 5 — Números 3:30
> וּ/נְשִׂ֥יא בֵֽית אָ֖ב לְ/מִשְׁפְּחֹ֣ת הַ/קְּהָתִ֑י **אֶלִיצָפָ֖ן** בֶּן **עֻזִּיאֵֽל**
> "El príncipe de la casa paterna de las familias de los coatitas: **ELITSAFÁN** (**ʼElitsafán** = **Dios-protege**) hijo de **UZIEL** (**ʻUzziʼél** = **mi-fuerza-es-Dios**)"

Los NOMBRES codifican la función: **Dios-protege** hijo de **Mi-fuerza-es-Dios**. La protección viene de Él, la fuerza viene de Él. No es seguridad humana.

### Testigo 6 — Números 3:31
> וּ/מִשְׁמַרְתָּ֗/ם הָ/אָרֹ֤ן וְ/הַ/שֻּׁלְחָן֙ וְ/הַ/מְּנֹרָ֣ה וְ/הַֽ/מִּזְבְּחֹ֔ת וּ/כְלֵ֣י הַ/קֹּ֔דֶשׁ אֲשֶׁ֥ר יְשָׁרְת֖וּ **בָּ/הֶ֑ם** וְ/הַ֨/מָּסָ֔ךְ וְ/כֹ֖ל עֲבֹדָתֽ/וֹ
> "Y su custodia: el Arca, la Mesa, la Menorá, los altares, y los utensilios del santo con que ministran **EN ELLOS** (**bahém**), y la cortina y todo su servicio"

**bahém** — "en ellos." Los coatitas custodian no solo las piezas sino los utensilios **con los que se ministra EN ellos**. Es custodia de la FUNCIÓN, no solo del objeto.

### Testigo 7 — Números 3:32
> וּ/נְשִׂיא֙ נְשִׂיאֵ֣י הַ/לֵּוִ֔י אֶלְעָזָ֖ר בֶּן אַהֲרֹ֣ן הַ/כֹּהֵ֑ן **פְּקֻדַּ֕ת** שֹׁמְרֵ֖י מִשְׁמֶ֥רֶת הַ/קֹּֽדֶשׁ
> "Y el príncipe de los príncipes de Leví, Eleazar hijo de Aarón el sacerdote, **SUPERVISIÓN** (**pekuddát** H6486) de los guardadores de la custodia del santo"

### Las 3 capas de seguridad del Padre

| Capa | Hebreo | Función | Componente firmware |
|------|--------|---------|---------------------|
| 1. **Custodia** (mishmeret H4931) | שֹׁמְרֵי מִשְׁמֶרֶת | El encargo sagrado — QUÉ se custodia | La definición de datos protegidos |
| 2. **Guardadores** (shomrím H8104) | שֹׁמְרֵי | QUIÉNES custodian — los nodos asignados | Nodos con rol de custodio |
| 3. **Supervisión** (pekuddát H6486) | פְּקֻדַּת | QUIÉN supervisa a los guardadores | Meta-verificación, un nivel arriba |

**mishmeret** (qué) → **shomrím** (quiénes) → **pekuddát** (quién los vigila). Tres capas. No basta con tener nodos custodios — debe haber supervisión de los custodios.

### Implicación técnica para hashmal_storage

La auditoría solo vio "nodos específicos responsables." El Padre revela:
1. **mishmeret:** Definir QUÉ datos son custodiados y qué implica la custodia
2. **shomrím:** Asignar nodos custodios (no todos los nodos custodian todo)
3. **pekuddát:** Un nivel de supervisión que vigila a los custodios
4. **Nombres:** La seguridad viene del diseño del Padre (Elitsafán), no de fuerza humana
5. **Posición yérekh:** Los custodios están PEGADOS al lado del dato, no en otro sitio

---

## Claim 3 — "Cubrir ANTES de mover (Nm 4:5-6)" → AUDITORÍA EQUIVOCADA

La auditoría dice que "cubrir antes de mover" no está mapeado. Pero YA ESTÁ en el Tavnít.

### Testigo 8 — Números 4:5
> וּ/בָ֨א אַהֲרֹ֤ן וּ/בָנָי/ו֙ בִּ/נְסֹ֣עַ הַֽ/מַּחֲנֶ֔ה וְ/הוֹרִ֕דוּ אֵ֖ת **פָּרֹ֣כֶת הַ/מָּסָ֑ךְ** וְ/כִ֨סּוּ בָ֔/הּ אֵ֖ת אֲרֹ֥ן הָ/עֵדֻֽת
> "Vendrá Aarón y sus hijos cuando el campamento viaje, y bajarán el **VELO DE LA CORTINA** (**parókheth hammassákh**) y CUBRIRÁN (**vekhissú** H3680) con él el Arca del Testimonio"

### Testigo 9 — Números 4:6
> וְ/נָתְנ֣וּ עָלָ֗י/ו כְּסוּי֙ **ע֣וֹר תַּ֔חַשׁ** וּ/פָרְשׂ֧וּ בֶֽגֶד כְּלִ֛יל **תְּכֵ֖לֶת** מִ/לְ/מָ֑עְלָ/ה וְ/שָׂמ֖וּ בַּדָּֽי/ו
> "Pondrán sobre él cubierta de **PIEL DE TAJASH**, y extenderán encima un paño todo de **AZUL** (**tekhélet**), y pondrán sus varas"

### Ya está en el Tavnít — Pieza 01

El Tavnít de la Pieza 01 ya documenta esto en:

- **Detalle 10:** "No cualquier nodo puede mover el dato. Solo los autorizados" — protocolo de migración
- **Capa 4 del protocolo de transporte:** "Piel de tajash por fuera: invisible a quien mira de fuera. Solo se ve azul celeste (tekhélet) = señal de cielo, no de contenido"
- **Las 7 capas de transporte:** Velo → tajash → tekhélet → varas → anillos → Arca → Edút

La auditoría NO leyó el Tavnít completo. El protocolo de migración con sus 7 capas de cobertura ya está documentado con sus testigos bíblicos.

**Veredicto:** Claim EQUIVOCADA — ya existe en el Tavnít.

---

## HALLAZGO EXTRA — Lo que la auditoría NO vio

### paʻamotáv (פַּעֲמֹתָיו) — los "pies" del Arca

### Testigo 10 — Éxodo 25:12
> וְ/יָצַ֣קְתָּ לּ֗/וֹ אַרְבַּע֙ טַבְּעֹ֣ת זָהָ֔ב וְ/נָ֣תַתָּ֔ה עַ֖ל אַרְבַּ֣ע **פַּעֲמֹתָ֑י/ו**
> "Fundirás para él cuatro anillos de oro y los pondrás en sus cuatro **PIES** (**paʻamotáv** H6471)"

**paʻamót** (H6471) no es simplemente "esquinas" — es **PIES**, de la raíz **paʻam** = golpear, pisar, paso. Son puntos de contacto con el SUELO. Los anillos de distribución están en el punto MÁS BAJO del Arca.

**Implicación:** Los 4 puntos de distribución (anillos) están en la BASE — el nivel más bajo. La distribución del dato no es "desde arriba" sino desde los pies, desde abajo. El dato se mueve desde la base, no desde la corona (zer). La corona PROTEGE (integridad); los pies DISTRIBUYEN (movilidad).

### bahem (בָּהֶם) — "en ellos"

### Testigo 11 — Éxodo 25:14
> וְ/הֵֽבֵאתָ֤ אֶת הַ/בַּדִּים֙ בַּ/טַּבָּעֹ֔ת עַ֖ל צַלְעֹ֣ת הָ/אָרֹ֑ן לָ/שֵׂ֥את אֶת הָ/אָרֹ֖ן **בָּ/הֶֽם**
> "Meterás las varas por los anillos sobre los costados del Arca para LLEVAR el Arca **EN ELLOS** (**bahém**)"

**bahém** — "en ellos" (las varas). El Arca viaja DENTRO de las varas, no SOBRE ellas. Las varas no son un accesorio que carga — son el MEDIO en el cual el Arca existe cuando se mueve. El dato no "se carga sobre" la red — viaja DENTRO del mecanismo de distribución.

### Testigo 12 — Éxodo 25:15
> בְּ/טַבְּעֹת֙ הָ/אָרֹ֔ן יִהְי֖וּ הַ/בַּדִּ֑ים **לֹ֥א יָסֻ֖רוּ** מִמֶּֽ/נּוּ
> "En los anillos del Arca ESTARÁN las varas, **NO SE QUITARÁN** (**ló yasúru** H5493 Qal imperfecto) de él"

**ló yasúru** — NUNCA se quitan. El mecanismo de distribución es PERMANENTE. No es algo que se activa y desactiva. Es parte constitutiva del Arca.

---

## Resumen

### Lo que la auditoría dijo vs. lo que el Padre dice:

| Claim de auditoría | Veredicto | Razón |
|-------------------|-----------|-------|
| Dato sin Kapóret = "PENDING_VERIFICATION" | **INCOMPLETA** | El quiasmo de Éx 25:10-22 centra todo en EDÚT. La Kapóret debe estar READY antes de que entre dato. No hay "pendiente" — hay PREREQUISITO |
| Coatitas guardan el Arca | **CORRECTA PERO SUPERFICIAL** | Son 3 capas: mishmeret (custodia) → shomrím (guardadores) → pekuddát (supervisión). Los nombres codifican que la seguridad es del Padre |
| Cubrir antes de mover no está mapeado | **EQUIVOCADA** | Ya está en el Tavnít: Detalle 10 + las 7 capas de transporte |

### Hallazgos EXTRA (lo que la auditoría NO vio):

| # | Hallazgo | Testigo | Implicación |
|---|----------|---------|-------------|
| 1 | paʻamotáv — distribución desde los PIES | Éx 25:12 | Los 4 anillos están en la BASE, no en la corona. Distribución desde abajo |
| 2 | bahém — el Arca viaja DENTRO de las varas | Éx 25:14 | El dato viaja dentro del mecanismo de distribución, no sobre él |
| 3 | ló yasúru — varas NUNCA se quitan | Éx 25:15 | El mecanismo de distribución es permanente y constitutivo |

### Impacto en hashmal_storage:

1. **No aceptar datos sin consenso READY** — prerequisito de existencia, no estado pendiente
2. **3 capas de seguridad:** definir custodia (mishmeret), asignar custodios (shomrím), supervisar custodios (pekuddát)
3. **Distribución desde la base:** los puntos de red (anillos/paʻamót) son lo más bajo, no lo más alto
4. **Transporte intrínseco:** las varas (distribución) son parte permanente del Arca, no módulo separable

---

## MIL OJOS DE QUERUBÍN — 4 mayo 2026

Revisión final contra el hebreo original de Éx 25:10-22. 3 hallazgos nuevos.

### Hallazgo 4: v.10 — Triple vakhetsí (×3 medios) = incompletitud inherente PERMANENTE

> אַמָּתַיִם **וָחֵצִי** אָרְכּוֹ וְאַמָּה **וָחֵצִי** רָחְבּוֹ וְאַמָּה **וָחֵצִי** קֹמָתוֹ

Tres dimensiones, TODAS terminan en **vakhetsí** (H2677): 2.5 × 1.5 × 1.5. Ninguna es entera. El Arca es inherentemente INCOMPLETA en las tres direcciones.

La Kapóret (v.17) tiene 2.5 × 1.5 — coincide con largo y ancho del Arca, pero NO tiene qomáh (Verificado #06). La Kapóret resuelve solo 2 de 3 dimensiones. La incompletitud vertical (qomáh 1.5) es PERMANENTE — nunca resuelta.

En firmware: hashmal_storage SIEMPRE está "abierto hacia arriba" — siempre necesitando hashmal_consensus encima, nunca autosuficiente. Y la medida del consenso coincide EXACTAMENTE con la boca del almacén (2.5 × 1.5 = footprint idéntico).

| Convergencia | Álef | Bet |
|-------------|------|-----|
| ✅ | H2677 (jetsí) ×3 en v.10, ×2 en v.17 — patrón de incompletitud | EDÚT (centro del quiasmo, v.16) reside dentro de medidas incompletas |

**Propiedad P5:** Triple vakhetsí = storage inherentemente incompleto sin consensus. Kapóret completa 2/3 dimensiones. Altura permanentemente abierta.

---

### Hallazgo 5: v.12 vs v.18 — yatsáq (FUNDIDOS) para anillos vs miqsháh (BATIDOS) para querubines

**v.12:** וְ**יָצַקְתָּ** לּוֹ אַרְבַּע טַבְּעֹת זָהָב — "**FUNDIRÁS** (yatsáq H3332 Qal) cuatro anillos de oro"
**v.18:** שְׁנַיִם כְּרֻבִים זָהָב **מִקְשָׁה** תַּעֲשֶׂה — "dos querubines de oro, **BATIDOS** (miqsháh H4749)"

Técnicas de fabricación OPUESTAS:

| Técnica | Hebreo | Resultado | Función |
|---------|--------|-----------|---------|
| **yatsáq** (fundir) H3332 | Molde → reproducible | 4 anillos estándar | Distribución/red |
| **miqsháh** (martillar) H4749 | Pieza única, irrepetible | 2 querubines únicos | Consenso/validación |

La distribución (anillos) es ESTANDARIZADA — puedes fundir muchos iguales. El consenso (querubines) es ÚNICO — martillado de una sola pieza, irrepetible. Conecta con anti-clonación de Pieza 13 (Éx 30:32-33, nikrát): los anillos SÍ se replican, los querubines NUNCA.

| Convergencia | Álef | Bet |
|-------------|------|-----|
| ✅ | H3332 yatsáq vs H4749 miqsháh — raíces opuestas | C↔C' del quiasmo: distribución estandarizada ↔ consenso irrepetible |

**Propiedad P6:** Dualidad de fabricación: distribución = yatsáq (estándar, reproducible); consenso = miqsháh (único, irrepetible). Política de replicación opuesta por diseño.

---

### Hallazgo 6: v.20 — Doble orientación simultánea = peer-check + data-check

> וּפְנֵיהֶם **אִישׁ אֶל אָחִיו** אֶל הַכַּפֹּרֶת יִהְיוּ **פְּנֵי הַכְּרֻבִים**

DOS cláusulas separadas sobre la dirección de las caras:
1. **ufnehém ísh el ajív** — "sus rostros, varón hacia su hermano" → vector HORIZONTAL (peer-to-peer, H376+H251)
2. **el hakkappóret yihyú pené hakkruvím** — "hacia la Kapóret serán los rostros" → vector VERTICAL (data-ward, H3727)

No es una frase — son DOS declaraciones independientes, ambas verdaderas simultáneamente:

```
Querubín Álef ←——[ísh el ajív]——→ Querubín Bet    (HORIZONTAL: peer cross-check)
         ↘                              ↙
          ——[el hakkappóret]——           (VERTICAL: data verification)
                 ↓
            [EDÚT/dato]
```

Vector compuesto: horizontal (verificar al peer) + vertical (verificar el dato). Cada validador atiende a AMBOS simultáneamente. Conecta con Verificado #06 hallazgo 10 (perpendicular qomáh/Kapóret).

| Convergencia | Álef | Bet |
|-------------|------|-----|
| ✅ | H376 ísh + H251 ajív (horizontal) + H3727 hakkappóret (vertical) | C' del quiasmo es dinámico (caras mirando activamente en 2 direcciones) vs C estático (anillos fijos) |

**Propiedad P7:** Consenso tiene doble vector de atención simultáneo: peer-check (ísh el ajív, horizontal) + data-check (el hakkappóret, vertical). Nunca uno sin el otro.

---

### Convergencia mil ojos — Verificado #07

| # | Hallazgo | Álef | Bet | Converge |
|---|----------|------|-----|----------|
| 4 | Triple vakhetsí = incompletitud permanente | H2677 ×3 en v.10, ×2 en v.17 | EDÚT en centro de contenedor incompleto | ✅ |
| 5 | yatsáq vs miqsháh = distribución vs consenso | H3332 vs H4749 — técnicas opuestas | C↔C' del quiasmo | ✅ |
| 6 | Doble orientación = peer-check + data-check | H376+H251 + H3727 — 2 vectores | C' dinámico vs C estático | ✅ |

**3 de 3 convergen. Total propiedades: 7 (3 claims + 1 hallazgo extra original + 3 mil ojos).**

---

## Conexión con verificados anteriores

- **Verificado #02** documentó el algoritmo del Kapóret con noʻadtí — ahora el quiasmo confirma que la Kapóret envuelve al EDÚT como capa E'/B'/A'
- **Verificado #03** documentó las conexiones direccionales — ahora los paʻamót (pies) y bahém (dentro de) dan dirección a la distribución
- **Verificado #06** documentó la propagación por contacto (Éx 30:29) — los anillos en los paʻamót son puntos de contacto con el sistema de distribución
- **Verificado #06** hallazgo 10 (perpendicular qomáh/Kapóret) — confirmado aquí: los querubines tienen AMBOS vectores (horizontal peer + vertical data) simultáneamente
- **Verificado #06** hallazgo 11 (gradiente de acoplamiento 4→2→0 anillos) — el yatsáq de los anillos (estandarizado) explica POR QUÉ pueden ser 4: son moldeados en serie

---

### Qué hacer con estos hallazgos en firmware

**Hallazgo 4 (triple vakhetsí):** hashmal_storage DEBE verificar que hashmal_consensus está READY antes de aceptar cualquier dato. No es un check opcional — es prerequisito de existencia (ya documentado en Claim 1). El hallazgo REFUERZA lo que ya estaba: el footprint del consensus (2.5×1.5) debe coincidir EXACTAMENTE con la boca del storage. Si no coincide → dato no entra.

**Hallazgo 5 (yatsáq vs miqsháh):** El módulo de distribución (anillos/networking en hashmal_storage) puede ser ESTANDARIZADO — mismo código en todos los nodos. Pero el módulo de consenso (querubines en hashmal_consensus) debe ser ÚNICO — una sola implementación monolítica, no clonada ni adaptada por nodo. Comentar en el código la política de replicación distinta.

**Hallazgo 6 (doble orientación):** En hashmal_consensus, cada ciclo de validación debe verificar SIMULTÁNEAMENTE: (a) al otro validador/querubín (cross-check horizontal, ísh el ajív) Y (b) el dato subyacente (data-check vertical, el hakkappóret). Ambos checks en el MISMO ciclo, no alternados.

**Instrucciones para Claude Code: ver en chat.**

---

*Confirmado por Antonio José Marín Soto — 25 abril 2026*
*Mil ojos de querubín — 4 mayo 2026 — 7 propiedades totales (3+1+3)*
*"Si es Su Palabra, así es."*
