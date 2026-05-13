# VERIFICADO #04 — Sha'ar ha'Atrio: El Portal WiFi SÍ existe, pero faltan las BASES

> *«Abridme las puertas de justicia; entraré por ellas, alabaré a YAH. Este es el portal de YHWH; los justos entrarán por él.»*
> *— Salmo 118:19-20*

**Fecha original:** 25 abril 2026
**Re-verificado:** 26 abril 2026
**Método:** Sistema de los Querubines (Éx 25:22) — Querubín Álef + Querubín Bet → middabbér
**Fuente:** Éxodo 27:9-19 + Éxodo 38:17-20 + Salmo 118:19-20 + Salmo 100:4

---

## CONTENIDO ORIGINAL PRESERVADO — 3 testigos, corrección de auditoría

### Hallazgo original (auditoría)

"La Añadidura 22 (SHA'AR/Portal WiFi) está COMPLETAMENTE AUSENTE del firmware. El header de hashmal_perimeter no tiene nada."

### CORRECCIÓN — La auditoría estaba EQUIVOCADA

El header `hashmal_perimeter.h` (líneas 688-843) YA CONTIENE toda la lógica de estado del portal:

| Elemento | En el header | Referencia |
|----------|:---:|-----------|
| Estados CERRADO/ABIERTO/PAUSADO | ✅ | Is 60:11 |
| 4 columnas de verificación | ✅ | Éx 27:16 |
| 12 sesiones simultáneas | ✅ | Paralelo al Pectoral |
| Funciones: abrir/cerrar/pausar | ✅ | Is 60:11, Shabat |
| Funciones: conectar/verificar/desconectar | ✅ | Sal 100:4, Éx 27:16 |
| Coexistencia con Miqlat(05) y Orchim(21) | ✅ | Documentado |

El header dice explícitamente: *"Modelo fase 1: lógica de ESTADO del portal. Fase 2 wireará con ESP-IDF WiFi AP + HTTP server + bridge a comm."*

### Lo que FALTA (fase 2, previsto pero no escrito):

| Elemento | Función planificada | Referencia |
|----------|---------------------|-----------|
| Servidor HTTP embebido | `HASHMAL_SHAAR_PORTAL` | Portal cautivo ESP-IDF |
| Punto de acceso WiFi | `HASHMAL_SHAAR_WIFI` | ESP-IDF WiFi AP mode |
| Bridge WiFi↔LoRa | `HASHMAL_SHAAR_PUENTE` | Añadidura 22 |
| Web app (HTML/CSS/JS) | `HASHMAL_SHAAR_INTERFAZ` | hashmal-portal-shaar.html |

Esto es fase 2 — se wirea con ESP-IDF. No es un fallo del Tavnít.

---

### Testigo 1 — Éxodo 27:16 (la puerta completa)
> וּלְשַׁעַר הֶחָצֵר מָסָךְ עֶשְׂרִים אַמָּה **תְּכֵלֶת וְאַרְגָּמָן וְתוֹלַעַת שָׁנִי וְשֵׁשׁ מָשְׁזָר** מַעֲשֵׂה **רֹקֵם** **עַמֻּדֵיהֶם אַרְבָּעָה וְאַדְנֵיהֶם אַרְבָּעָה**
> "Para la PUERTA (**shaʻar** H8179) del atrio, una CORTINA de 20 codos de **AZUL** (tejélet H8504), **PÚRPURA** (argamán H713), **CARMESÍ** (tolaʻat shaní H8438+H8144) y **LINO TORCIDO** (shésh mashzár H8336+H7806), obra de **BORDADOR** (**roqém** H7551); sus **COLUMNAS CUATRO** (ʻammudéihem arbaʻáh) y sus **BASES CUATRO** (**ʼadnéihem arbaʻáh** H134)"

### Los 7 elementos del versículo vs. el header:

| # | Elemento hebreo | Strong | Significado | ¿En header? |
|---|-----------------|--------|-------------|:-----------:|
| 1 | שַׁעַר (shaʻar) | H8179 | PUERTA — una sola entrada | ✅ |
| 2 | מָסָךְ (masaj) | H4539 | CORTINA — separación visible | ✅ comentarios |
| 3 | 20 אַמָּה (ʻesrím ammá) | H6242+H520 | 20 codos = 1/5 del largo | ✅ constante |
| 4 | תְּכֵלֶת + אַרְגָּמָן + שָׁנִי + שֵׁשׁ | H8504+H713+H8144+H8336 | 4 materiales/colores | ⚠️ NO codificados |
| 5 | רֹקֵם (roqém) | H7551 | BORDADOR — arte en la interfaz | ⚠️ Solo comentario |
| 6 | עַמֻּדֵיהֶם אַרְבָּעָה | H5982+H702 | 4 COLUMNAS de verificación | ✅ enum + funciones |
| 7 | **אַדְנֵיהֶם אַרְבָּעָה** | **H134+H702** | **4 BASES de las columnas** | ❌ FALTA |

### Lo que FALTA según el Padre: Las 4 BASES (ʼadaním)

### Testigo 2 — Éxodo 27:17 (material de las bases)
> כָּל עַמּוּדֵי הֶחָצֵר סָבִיב מְחֻשָּׁקִים כֶּסֶף וָוֵיהֶם כָּסֶף **וְאַדְנֵיהֶם נְחֹשֶׁת**
> "Todas las columnas del atrio alrededor, bandas de PLATA, ganchos de PLATA, y sus **BASES de BRONCE** (**ʼadnéihem nejóshet**)"

Las bases son de **BRONCE** (nejóshet H5178). En el Tavnít, bronce = nivel de juicio, primer contacto, frontera.

### Testigo 3 — Éxodo 38:18 (la ejecución real)
> Confirma los 4 materiales + roqém + 4 columnas + 4 bases.

### ¿Qué son las BASES en el firmware?

Cada COLUMNA (verificación) se PARA sobre una BASE (criterio/estándar):

| Columna (ʻammúd) | Verificación | Base (ʼéden) | Criterio de bronce |
|-------------------|-------------|--------------|-------------------|
| 1 — IDENTIDAD | ¿Quién eres? | Base 1 | Tabla de identidades conocidas |
| 2 — AUTENTICACIÓN | ¿Tienes la clave? | Base 2 | Hash de contraseña WiFi |
| 3 — AUTORIZACIÓN | ¿Qué puedes hacer? | Base 3 | Tabla de roles (Pieza 09) |
| 4 — INTEGRIDAD | ¿Tu sesión es legítima? | Base 4 | Token temporal + timestamp |

Sin la base, la columna no se sostiene. Sin criterio, la verificación no tiene contra qué comparar.

### ¿Qué son los 4 COLORES en el firmware?

Los 4 materiales de la cortina (la interfaz visual):

| Material | Hebreo | Strong | Color | En la interfaz (hashmal-portal-shaar.html) |
|----------|--------|--------|-------|---------------------------------------------|
| Tejélet | תְּכֵלֶת | H8504 | AZUL | Header gradient inicio (#3B6CB4) |
| Argamán | אַרְגָּמָן | H713 | PÚRPURA | Header gradient medio (#7B4FA2) |
| Tolaʻat shaní | תּוֹלַעַת שָׁנִי | H8438+H8144 | CARMESÍ | Header gradient fin (#B83A3A) |
| Shésh mashzár | שֵׁשׁ מָשְׁזָר | H8336+H7806 | LINO BLANCO | Fondo limpio (#F5F0EB) |

Estos 4 colores YA están implementados en hashmal-portal-shaar.html (el mockup del portal). El Padre dijo los colores exactos de la interfaz.

### Roqém (bordador) — NO es josév (tejedor)

Detalle importante: la puerta del Atrio es obra de **roqém** (bordador, H7551). El Velo interior es obra de **josév** (tejedor, H2803 — teje querubines dentro del tejido).

Diferencia: el bordador trabaja POR ENCIMA del tejido (decoración visible). El tejedor integra DENTRO del tejido (seguridad intrínseca).

**Implicación:** La interfaz del portal (fase 2) es decoración VISIBLE — lo que el usuario ve. La seguridad real (Velo/Framework pieza 05) está TEJIDA por dentro, invisible al usuario. La puerta es BELLA por fuera, SEGURA por dentro.

---

## RE-VERIFICACIÓN CON SISTEMA DE LOS QUERUBINES

**8 hallazgos nuevos** que el original no vio.

---

### QUERUBÍN ÁLEF — Raíces, Strong, gramática

**Hallazgo nuevo 1 — v.16: masakh (H4539) vs qelaʻím (H7050) — Dos tipos de frontera**

El resto del atrio usa **qelaʻím** (H7050, קְלָעִים, cortinas/pantallas pasivas, v.9). Pero la puerta usa **masakh** (H4539, מָסָךְ = cubierta/pantalla activa). Son dos palabras DISTINTAS:

- **qelaʻím** = pantallas fijas, pasivas — los MUROS del perímetro. No se abren ni se cierran. Son la frontera estática.
- **masakh** = pantalla que CUBRE/DESCUBRE — se puede mover, abrir, manipular. Es un portal ACTIVO.

La misma palabra **masakh** aparece en Éx 26:36 para la entrada de la Tienda — el acceso activo al Qódesh. El sha'ar del Atrio y la entrada del Mishkán comparten el mismo tipo de frontera dinámica. En firmware: qelaʻím = firewall rules estáticas; masakh = portal con estado (ABIERTO/CERRADO/PAUSADO).

**Hallazgo nuevo 2 — v.17 + Éx 38:19: Tres capas de material en las columnas**

> כָּל עַמּוּדֵי הֶחָצֵר סָבִיב **מְחֻשָּׁקִים כֶּסֶף** **וָוֵיהֶם כָּסֶף** **וְאַדְנֵיהֶם נְחֹשֶׁת**

Las columnas tienen TRES capas de material de abajo hacia arriba:

1. **ʼadaním nejóshet** (bases de bronce, H134+H5178) = FUNDAMENTO, nivel de juicio, primer contacto
2. **vavéihem késef** (ganchos de plata, H2053+H3701) = CONEXIÓN entre componentes, precio de redención
3. **mejushshaqím késef** (bandas de plata, H2836 Pual + H3701) = APARIENCIA, identidad visible

Tres capas = tres niveles de seguridad apilados verticalmente. La base es bronce (zona frontera), la conexión y apariencia son plata (zona intermedia). En Éx 38:19 se añade: **tsippúi rashéihem késef** (recubrimiento de sus cabezas, H6826+H7218) — las CABEZAS de las columnas también son plata. Desde arriba, TODO parece plata. Desde abajo, se ve el bronce. La identidad verdadera se examina por el FUNDAMENTO, no por la apariencia.

**Hallazgo nuevo 3 — Éx 38:18: leʻummat qalʻéi hejatsér — "correspondiente a"**

> וְקוֹמָה בְרֹחַב חָמֵשׁ אַמּוֹת **לְעֻמַּת** קַלְעֵי הֶחָצֵר

**leʻummat** (H5980) = "correspondiente a, a la par de." La altura del masakh de la puerta (5 codos) es IDÉNTICA a la altura de los qelaʻím (muros, v.18). El portal NO se distingue por altura del resto del perímetro. Desde FUERA, no hay indicación visual de dónde está la puerta — tienes que SABER dónde es. El portal es flush con el muro. Seguridad por no-distinción visual.

**Hallazgo nuevo 4 — Proporciones: 20 de 50 codos, centrado 15-20-15**

La puerta tiene 20 ammáh (codos) de ancho. El lado este del atrio tiene 50 codos. Los "hombros" (katéf, H3802) a cada lado tienen 15 codos cada uno (v.14-15): 15 + 20 + 15 = 50.

La puerta es **2/5 del lado de entrada** = 40%. No es una rendija escondida, pero tampoco domina. El acceso es GENEROSO pero no excesivo. Y está CENTRADA entre dos hombros iguales (15=15) — simetría perfecta. Como los querubines a los dos lados de la Kapóret.

**Hallazgo nuevo 5 — v.14-15: katéf (H3802) = hombro**

> וַחֲמֵשׁ עֶשְׂרֵה אַמָּה קְלָעִים **לַכָּתֵף** עַמֻּדֵיהֶם שְׁלֹשָׁה... וְ**לַכָּתֵף הַשֵּׁנִית** חֲמֵשׁ עֶשְׂרֵה

**Katéf** (H3802) = hombro, lado. Los muros a cada lado de la puerta se llaman "hombros." La puerta está entre dos hombros — como una persona con los brazos abiertos. Cada hombro tiene 3 columnas + 3 bases. La puerta tiene 4 columnas + 4 bases. Proporción: 3-4-3 columnas. El centro (4) tiene UNA columna más que cada hombro (3). Mínima ventaja, máxima significancia.

**Hallazgo nuevo 6 — Sal 118:19-20: shaʻaréi tsédeq + zeh hashaʻar laYHWH**

> **פִּתְחוּ לִי שַׁעֲרֵי צֶדֶק** אָבֹא בָם אוֹדֶה יָהּ
> **זֶה הַשַּׁעַר לַיהוָה צַדִּיקִים יָבֹאוּ בוֹ**

**Pitjú** (H6605, abrir de par en par) + **shaʻaréi tsédeq** (H8179+H6664 = puertas de JUSTICIA). Y v.20: **zeh hashaʻar laYHWH** = "ESTE es el portal de YHWH" — **tsaddiqím yavóʼu vo** = "los JUSTOS entrarán por él." Dos palabras relacionadas: **tsédeq** (el estándar de justicia) y **tsaddiqím** (los que lo cumplen). La puerta filtra por CARÁCTER moral, no solo por identidad técnica. El portal no es neutral — tiene un estándar ético.

**Hallazgo nuevo 7 — Sal 100:4: Dos etapas de acceso**

> **בֹּאוּ שְׁעָרָיו בְּתוֹדָה חֲצֵרֹתָיו בִּתְהִלָּה**

**Boʼú sheʻaráv betodáh** = "Entrad por sus PUERTAS (H8179) con ACCIÓN DE GRACIAS (todáh, H8426)." Luego: **jatserótav bitehilláh** = "sus ATRIOS (H2691) con ALABANZA (tehilláh, H8416)." DOS etapas de acceso:

1. **Puerta (shaʻar) → con todáh** = reconocimiento, gratitud — autenticación básica
2. **Atrio (jatsér) → con tehilláh** = alabanza, compromiso profundo — autorización plena

El portal (pieza 07/Añadidura 22) es la PRIMERA etapa. El atrio (pieza 07 completa) es la SEGUNDA. No se entra al atrio directamente — se entra por la puerta con un requisito distinto y menor que el del atrio completo. Pipeline de acceso gradual.

**Hallazgo nuevo 8 — v.19: yetedót nejóshet (H3489+H5178) — Infraestructura oculta**

> וְכָל **יְתֵדֹת** הֶחָצֵר **נְחֹשֶׁת**

**Yetedót** (H3489) = estacas/clavos. TODAS las estacas del tabernáculo y del atrio son de bronce. Las estacas ANCLAN las cortinas al suelo — sin ellas, todo se cae con el viento. Son la infraestructura INVISIBLE: nadie las ve, pero todo depende de ellas. En firmware: las estacas son los servicios de infraestructura subyacentes (DNS, DHCP del AP WiFi, timer del portal cautivo) — nadie los ve, pero sin ellos el portal no se sostiene.

---

### QUERUBÍN BET — Estructura literaria, quiasmos

**Quiasmo de Éx 27:9-19 (especificación completa del Atrio)**

```
A   v.9-12: Lados SUR y NORTE — 100 codos, 20 columnas, 20 bases (bronce)
  B   v.13: Lado ESTE (frontal) — 50 codos de ancho
    C   v.14-15: DOS HOMBROS simétricos — 15+15 codos, 3+3 columnas, 3+3 bases
      D   v.16: <<<  EL SHA'AR  >>> — masakh 20 codos, 4 colores, roqém, 4+4
    C'  v.17: TODAS las columnas — plata(bandas)+plata(ganchos)+bronce(bases)
  B'  v.18: DIMENSIONES totales — 100×50×5 codos
A'  v.19: TODO el equipamiento + yetedót — bronce
```

El **SHA'AR es el CENTRO** de toda la especificación del Atrio. Todo antes describe los muros; todo después generaliza. La puerta es el PIVOTE del perímetro — el punto donde el pasivo (qelaʻím) se vuelve activo (masakh).

**Simetría 3-4-3 de las columnas del lado este**

```
Hombro izquierdo:  |||  (3 columnas, 15 codos de qelaʻím)
SHA'AR:            |||| (4 columnas, 20 codos de masakh, 4 colores)
Hombro derecho:    |||  (3 columnas, 15 codos de qelaʻím)
```

La simetría recuerda al Kapóret: dos querubines simétricos con el espacio entre ellos como punto de convergencia. Aquí: dos hombros simétricos con la puerta como punto de convergencia. El centro tiene UNA columna más — el mínimo necesario para crear diferenciación.

**Patrón de materiales descendente — Dos portales paralelos**

| Zona | Material dominante | Nivel |
|------|-------------------|-------|
| Qódesh haQodashím | Oro puro | Máximo |
| Qódesh | Oro | Alto |
| Entrada Tienda | masakh 4 colores + oro | Transición alto→medio |
| **SHA'AR** | **masakh 4 colores + plata+bronce** | **Transición medio→bajo** |
| Atrio muros | qelaʻím lino + plata+bronce | Básico |

Los DOS masakh (entrada de Tienda y puerta del Atrio) comparten los 4 colores pero difieren en metal de soporte. La puerta es el espejo exterior de la entrada de la Tienda. Dos portales, dos niveles, misma estructura visual.

---

### MIDDABBÉR — Lo que CONVERGE de entre los dos

| # | Hallazgo | Álef (raíz/Strong) | Bet (estructura/quiasmo) | ¿Converge? |
|---|----------|---------------------|--------------------------|------------|
| 1 | masakh vs qelaʻím = portal activo vs muro pasivo | H4539 vs H7050 — dos palabras distintas | masakh = centro D del quiasmo; qelaʻím = bordes A/A' | ✅ MIDDABBÉR |
| 2 | Tres capas en columnas (bronce→plata→plata) | H134+H2053+H2836 — tres Strong distintos | C' en quiasmo generaliza a TODAS las columnas | ✅ MIDDABBÉR |
| 3 | Portal flush con muro (leʻummat, H5980) | Altura idéntica 5 codos | Coherente con que D está DENTRO de la estructura, no sobresale | ✅ MIDDABBÉR |
| 4 | Proporción 20/50 = 2/5, centrado 15-20-15 | Cálculo numérico | Simetría 3-4-3 columnas refleja querubines | ✅ MIDDABBÉR |
| 5 | katéf = hombro, simetría bilateral | H3802 — término anatómico | C (dos hombros) enmarca D (puerta) en quiasmo | ✅ MIDDABBÉR |
| 6 | shaʻaréi tsédeq = filtrado ético | H8179+H6664 + H6662 tsaddiqím | Portal central filtra por carácter, no solo ID | ✅ MIDDABBÉR |
| 7 | Dos etapas: shaʻar(todáh) → jatsér(tehilláh) | H8179+H8426 → H2691+H8416 | Pipeline gradual: puerta→atrio, dos requisitos distintos | ✅ MIDDABBÉR |
| 8 | yetedót = infraestructura invisible | H3489+H5178 — bronce, anclas | A' del quiasmo cierra con infraestructura base | ✅ MIDDABBÉR |

**8 de 8 convergen.** Cero descartados.

---

### MIL OJOS DE QUERUBÍN — Hallazgos adicionales (Álef+Bet sobre Éx 38:15-21)

**Hallazgo 9 — Éx 38:15: מִ/זֶּה וּ/מִ/זֶּה (mizzéh umizzéh, H2088) — Fórmula bilateral querubínica**

> וְ/לַ/כָּתֵ֣ף הַ/שֵּׁנִ֗ית **מִ/זֶּ֤ה וּ/מִ/זֶּה֙** לְ/שַׁ֣עַר הֶֽ/חָצֵ֔ר

Los dos hombros (katéf) que flanquean la puerta usan la fórmula exacta **מִ/זֶּה וּ/מִ/זֶּה** — "de ESTE lado y de ESTE lado." Es la MISMA fórmula bilateral que describe a los querubines sobre la Kapóret en Éx 25:19: כְּרוּב מִקָּצָה **מִזֶּה** וּכְרוּב אֶחָד מִקָּצָה **מִזֶּה**. No es solo un paralelo estructural (3-4-3) — es un paralelo LÉXICO. El Padre usa las mismas palabras para las dos simetrías. El sha'ar es un Kapóret de bronce en la frontera exterior.

Querubín Álef: H2088 (zéh) bilateral, misma raíz en ambos contextos.
Querubín Bet: en el quiasmo del Atrio, la simetría C/C' (hombros) enmarca D (puerta) — idéntico al patrón de los querubines enmarcando el espacio de la Presencia.
→ ✅ MIDDABBÉR — converge.

**Hallazgo 10 — Éx 38:16: כָּל קַלְעֵי הֶחָצֵר סָבִיב שֵׁשׁ מָשְׁזָר — Perímetro monocromo absoluto**

> כָּל קַלְעֵ֧י הֶ/חָצֵ֛ר סָבִ֖יב **שֵׁ֥שׁ מָשְׁזָֽר**

Declaración categórica: TODAS (kol) las cortinas (qalʻéi) del atrio ALREDEDOR (savív) son SOLO lino torcido blanco (shésh mashzár, H8336+H7806). Sin colores. Sin adornos. Perímetro 100% monocromo. Los 4 colores (tejélet+argamán+shaní+shésh) aparecen EXCLUSIVAMENTE en el masakh del sha'ar (v.18). La puerta es el ÚNICO punto de diferenciación visual/funcional en todo el muro uniforme. Color = multi-protocolo. Blanco = protocolo único.

Querubín Álef: qalʻéi (H7050, forma constructa plural) + savív (H5439, circuito) + shésh mashzár (solo) — vocabulario de uniformidad total.
Querubín Bet: v.16 está entre los hombros (v.15) y las columnas (v.17) — es el CONTEXTO que hace resaltar al sha'ar (v.18) como excepción.
→ ✅ MIDDABBÉR — converge.

**Hallazgo 11 — Éx 38:21: אֵלֶּה פְקוּדֵי — Auditoría independiente inmediata**

> אֵ֣לֶּה **פְקוּדֵ֤י** הַ/מִּשְׁכָּן֙ מִשְׁכַּ֣ן הָ/עֵדֻ֔ת אֲשֶׁ֥ר **פֻּקַּ֖ד** עַל פִּ֣י מֹשֶׁ֑ה עֲבֹדַת֙ הַ/לְוִיִּ֔ם בְּ/יַד֙ **אִֽיתָמָ֔ר** בֶּֽן אַהֲרֹ֖ן

Inmediatamente después de completar el Atrio (yetedót, v.20), el SIGUIENTE versículo inicia la auditoría: **pequdéi** (H6485, participio pasivo Qal = "lo que fue contado/auditado"). Doble uso de la raíz paqad: pequdéi (ptc.) + puqqad (Pual perfecto = "fue auditado"). Y el auditor es **Itamar ben Aharón** — NO Betsalel el constructor. El constructor no se audita a sí mismo. Verificador independiente obligatorio.

Querubín Álef: paqad (H6485) = visitar, contar, auditar; doble forma (Qal ptc. + Pual perf.) enfatiza que la auditoría es RECIBIDA (pasiva), no autoimpuesta.
Querubín Bet: v.21 abre una nueva sección narrativa (auditoría) que CIERRA la construcción — la estructura literaria exige verificación final como cierre.
→ ✅ MIDDABBÉR — converge.

**Convergencia actualizada: 11 de 11.** (8 originales + 3 mil ojos). Cero descartados.

---

## PROPIEDADES DE RED — Original + Nuevas

### Propiedades originales (preservadas)

| # | Propiedad | Fuente |
|---|-----------|--------|
| P1 | **4 columnas = 4 verificaciones** (identidad, autenticación, autorización, integridad) | Éx 27:16 |
| P2 | **4 bases = 4 criterios** de bronce — sin base, la columna no se sostiene | Éx 27:16-17 (FALTA en header) |
| P3 | **Roqém vs josév:** interfaz visible (bordado) vs seguridad intrínseca (tejido) | Éx 27:16 vs Éx 26:31 |

### Propiedades nuevas (de los Querubines)

| # | Propiedad nueva | Fuente |
|---|----------------|--------|
| P4 | **Portal activo vs frontera pasiva:** masakh (dinámico, con estados) ≠ qelaʻím (estático, siempre cerrado). El portal es el ÚNICO punto donde el perímetro cambia de estado. | Éx 27:16 vs 27:9 |
| P5 | **Seguridad por capas verticales:** columnas con 3 niveles: bronce(fundamento) → plata(conexión) → plata(apariencia). La identidad verdadera se lee desde la base, no desde la superficie. | Éx 27:17, 38:19 |
| P6 | **Stealth portal:** la puerta tiene la MISMA altura que los muros (leʻummat). Desde fuera no se distingue visualmente dónde está el acceso. Security by design. | Éx 38:18 |
| P7 | **Simetría bilateral 3-4-3:** la puerta está centrada entre dos hombros (katéf) iguales. Mínima diferenciación (4 vs 3 columnas), máxima significancia. Patrón querubínico. | Éx 27:14-16 |
| P8 | **Filtrado ético, no solo técnico:** el portal filtra por tsédeq/tsaddiqím (justicia/justo). Acceso requiere carácter, no solo credenciales. | Sal 118:19-20 |
| P9 | **Pipeline de acceso gradual:** shaʻar(todáh) → jatsér(tehilláh). Dos etapas con requisitos DISTINTOS: reconocimiento básico primero, compromiso profundo después. | Sal 100:4 |
| P10 | **Infraestructura invisible (yetedót):** las estacas de bronce anclan todo sin ser vistas. Servicios subyacentes (DHCP, DNS, timers) que sostienen el portal. | Éx 27:19 |
| P11 | **Espejo de portales:** sha'ar del Atrio y masakh de la Tienda comparten estructura (4 colores, roqém) pero difieren en nivel de metal. Dos portales paralelos a dos niveles de profundidad. | Éx 27:16 vs Éx 26:36 |

### Propiedades de "Mil ojos de querubín" (hallazgos 9-11)

| # | Propiedad nueva | Fuente |
|---|----------------|--------|
| P12 | **Fórmula bilateral querubínica (mizzéh umizzéh):** los dos hombros que flanquean la puerta usan la MISMA fórmula bilateral מִ/זֶּה וּ/מִ/זֶּה (Éx 38:15) que los dos querubines que flanquean el Kapóret (Éx 25:19). No es solo paralelo estructural — es paralelo VERBAL. El sha'ar es un Kapóret exterior. | Éx 38:15 ↔ Éx 25:19 |
| P13 | **Perímetro monocromo, portal multi-protocolo:** Éx 38:16 declara que TODAS las cortinas del atrio alrededor son SOLO שֵׁשׁ מָשְׁזָר (lino torcido blanco). Los 4 colores (tejélet, argamán, shaní, shésh) aparecen EXCLUSIVAMENTE en el sha'ar. El portal es el ÚNICO punto de diferenciación funcional en todo el perímetro uniforme. Color = multi-canal. Blanco = canal único. | Éx 38:16 |
| P14 | **Auditoría independiente inmediata (pequdéi):** inmediatamente después de completar el Atrio (v.20), Éx 38:21 abre con אֵלֶּה פְקוּדֵי (ʼélleh pequdéi, H6485 ptc. pas.) = "estos son los recuentos/auditoría." Lo ejecuta Itamar ben Aharón — NO Betsalel el constructor. El constructor NO se audita a sí mismo. Después de inicializar el portal, un verificador INDEPENDIENTE (hashmal_healthcheck) debe confirmar que todo está correcto. | Éx 38:21 |

---

## RESUMEN: Qué hacer (actualizado)

### NO hacer (ya existe):
- La lógica de estado del portal ✅
- Las 4 columnas ✅
- El ciclo de sesiones ✅

### SÍ hacer (falta):

1. **Añadir las 4 BASES (ʼadaním) al header** — struct con criterio de verificación para cada columna
2. **Codificar los 4 colores** como constantes (ya están en el HTML, pero deben estar en el firmware como referencia)
3. **NUEVO: Implementar masakh como tipo distinto de qelaʻím** — estados dinámicos vs reglas estáticas
4. **NUEVO: Implementar las 3 capas de la columna** — base(criterio) + gancho(conexión) + banda(identidad visual)
5. **NUEVO: Pipeline de 2 etapas** — autenticación en sha'ar (todáh) → autorización en jatsér (tehilláh)
6. **NUEVO: Infraestructura yetedót** — servicios base (DHCP, DNS, timer) como struct separada
7. **Fase 2: wiring WiFi** — servidor HTTP, AP mode, bridge WiFi↔LoRa
8. **Fase 2: interfaz roqém** — la web app ya está diseñada (hashmal-portal-shaar.html), necesita almacenarse en partición de datos del ESP32
9. **NUEVO (mil ojos): Reforzar paralelo querubínico verbal** — documentar que la simetría 3-4-3 usa mizzéh umizzéh (Éx 38:15), misma fórmula que Éx 25:19
10. **NUEVO (mil ojos): Perímetro mono-protocolo vs portal multi-protocolo** — el firewall perimetral (qelaʻím) opera canal único; solo el sha'ar opera 4 canales (colores)
11. **NUEVO (mil ojos): Health check independiente post-init** — después de inicializar el portal, hashmal_healthcheck (NO hashmal_perimeter) debe verificar; el constructor no se audita a sí mismo (Éx 38:21, Itamar ≠ Betsalel)

---

## CONEXIÓN CON #01, #02, #03 RE-VERIFICADOS

- **#01 (Constructor cambia de modo):** masakh sha'ar en Éx 40:33 fue lo ÚLTIMO que Moisés instaló antes de que la Gloria llenara. El portal se activa al final — cierra el perímetro y HABILITA el sistema.
- **#02 (Kapóret):** La simetría 3-4-3 de los hombros y la puerta refleja el patrón de los querubines: dos iguales con el espacio central como punto de convergencia. *Mil ojos:* confirmado VERBALMENTE — mizzéh umizzéh (Éx 38:15) = misma fórmula que Éx 25:19.
- **#03 (Conexiones):** El pipeline gradual shaʻar→jatsér (Sal 100:4) es un CUARTO tipo de cadena direccional — cadena de ACCESO con requisitos crecientes. Complementa los 3 tipos del #03: trigger diario, autorización, y Yom Kippur.

---

*Re-verificado por el Sistema de los Querubines — Éxodo 25:22*
*«La verdad emerge de entre los dos»*
*Mil ojos de querubín — 26 abril 2026: 3 hallazgos añadidos (9-11), convergencia 11/11, propiedades 14 totales*

*Confirmado por Antonio José Marín Soto — 26 abril 2026*
*"Si es Su Palabra, así es."*
