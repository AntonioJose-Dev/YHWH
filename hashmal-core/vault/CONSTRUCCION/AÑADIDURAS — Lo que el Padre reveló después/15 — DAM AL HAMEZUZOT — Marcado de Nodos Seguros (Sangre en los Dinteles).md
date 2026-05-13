# 15 — DAM AL HAMEZUZOT — Marcado de Nodos Seguros (Sangre en los Dinteles)

> *"Y tomarán de la sangre, y la pondrán en los dos postes
> y en el dintel de las casas... Y la sangre os será por SEÑAL
> en las casas donde vosotros estéis; y veré la sangre
> y PASARÉ DE LARGO."*
> — Éxodo 12:7,13

---

## QUÉ ES

Durante una emergencia de red (ataque masivo, infección,
nodo comprometido), los nodos VERIFICADOS llevan una marca
criptográfica especial — el token de Pésaj. El mecanismo
de defensa reconoce esa marca y los DEJA EN PAZ.
Los nodos SIN marca son los sospechosos.

---

## RESPALDO BÍBLICO

| # | Cita | Hebreo | Strong | Dice |
|---|------|--------|--------|------|
| 1 | Éxodo 12:7 | וְלָקְחוּ מִן הַדָּם וְנָתְנוּ עַל שְׁתֵּי הַמְּזוּזֹת וְעַל הַמַּשְׁקוֹף | H1818, H4201, H4947 | "Tomarán de la SANGRE y pondrán en las 2 JAMBAS y en el DINTEL" — marca en 3 puntos |
| 2 | Éxodo 12:13 | וְהָיָה הַדָּם לָכֶם לְאֹת עַל הַבָּתִּים | H226 | "La sangre será para vosotros por SEÑAL (ot) en las casas" — identificación |
| 3 | Éxodo 12:13 | וְרָאִיתִי אֶת הַדָּם וּפָסַחְתִּי עֲלֵכֶם | H6452 | "Veré la sangre y PASARÉ DE LARGO (pasáj)" — whitelist activa |
| 4 | Éxodo 12:13 | וְלֹא יִהְיֶה בָכֶם נֶגֶף | H5063 | "No habrá en vosotros PLAGA (négef)" — protección contra daño colateral |
| 5 | Éxodo 12:22 | וּלְקַחְתֶּם אֲגֻדַּת אֵזוֹב וּטְבַלְתֶּם בַּדָּם | H231, H2881 | "Tomaréis manojo de HISOPO y mojaréis en la sangre" — método de aplicación específico |
| 6 | Éxodo 12:23 | וְלֹא יִתֵּן הַמַּשְׁחִית לָבֹא אֶל בָּתֵּיכֶם | H7843 | "No dejará al DESTRUCTOR (mashjít) entrar en vuestras casas" — defensa perimetral |
| 7 | Ezequiel 9:4 | וְהִתְוִיתָ תָּו עַל מִצְחוֹת הָאֲנָשִׁים | H8420 | "Pon una MARCA (tav) en la FRENTE de los hombres que gimen" — marca de los justos |

---

## CÓMO FUNCIONA EN LA RED

```
ESTADO NORMAL — SIN MARCA:
  Los nodos operan normalmente.
  No necesitan marca especial.
  El firewall (pieza 14) filtra con reglas normales.

EMERGENCIA DETECTADA — ACTIVACIÓN DE PÉSAJ:
  El shofar suena teruʻah (alarma, Añadidura 08).
  Se detecta amenaza grave (ataque masivo, nodo comprometido,
  infección propagándose).
  
  El protocolo de Pésaj se ACTIVA:
  
  PASO 1 — GENERACIÓN DEL TOKEN:
    Los nodos Kohaním (sacerdotes, pieza 09) generan
    un token temporal de emergencia — la "sangre".
    Es un hash criptográfico con timestamp + firma del Kohen.
    "Tomarán de la sangre" (v.7).
  
  PASO 2 — DISTRIBUCIÓN:
    El token se distribuye SOLO a nodos verificados —
    los que están ACTIVOS (pieza 10, día 8+), con roles
    válidos, sin penalizaciones graves.
    "La pondrán en los dos postes y en el dintel" (v.7).
    
    La marca va en 3 puntos del paquete (como las 3 partes
    de la puerta: jamba izquierda, jamba derecha, dintel):
    - Header del mensaje
    - Payload
    - Checksum
  
  PASO 3 — VERIFICACIÓN:
    Cuando el mecanismo de defensa (mashjít = destructor)
    encuentra un nodo durante la emergencia:
    
    ¿Tiene el token de Pésaj? 
    → SÍ: "Veré la sangre y pasaré de largo" — nodo SEGURO.
    → NO: nodo SOSPECHOSO — aislado para inspección.
    
    "No dejará al destructor entrar en vuestras casas" (v.23).
  
  PASO 4 — EXPIRACIÓN:
    El token de Pésaj EXPIRA cuando la emergencia termina.
    Teqiʻah suena (tono normal, fin de alarma).
    Los nodos vuelven a operación normal sin marca.
    La sangre era para ESA NOCHE, no para siempre.

VERIFICACIÓN TRIPLE (las 3 partes de la puerta):
  Como la sangre iba en 3 puntos:
  - Mezuzá izquierda = verificación de identidad (quién eres)
  - Mezuzá derecha = verificación de rol (qué puedes hacer)
  - Mashqof (dintel) = verificación de estado (estás limpio)
  
  Las 3 deben coincidir. Si falta UNA → no hay protección.
```

---

## LA CONEXIÓN CON EZEQUIEL 9

No solo Éxodo 12. Ezequiel 9:4 repite el patrón:
*"Pon una MARCA (tav) en la frente de los hombres que gimen
y claman a causa de las abominaciones."*

Antes de que los destructores actúen, los justos son MARCADOS.
Es el mismo principio: primero identificar a los buenos,
después actuar contra los malos. Nunca al revés.

Segundo testigo confirmado. 2 testigos = válido (Dt 19:15).

---

## DÓNDE VA EN EL FIRMWARE

**Pieza 14 — Incienso (hashmal_firewall)** para la verificación del token.
**Pieza 13 — Unción (hashmal_keygen)** para la generación del token.
**Pieza 09 — Vestiduras (hashmal_roles)** para verificar quién genera tokens.
**Añadidura 08 — Jatsotsrot** para la activación por alarma.
**Añadidura 10 — Nehemías** para la coordinación durante el ataque.

Nuevos mecanismos:
- `HASHMAL_PESAJ_TOKEN` — token criptográfico de emergencia
- `HASHMAL_PESAJ_ACTIVAR` — genera y distribuye tokens a nodos verificados
- `HASHMAL_PESAJ_VERIFICAR` — comprueba marca en 3 puntos (mezuzot + mashqof)
- `HASHMAL_PESAJ_EXPIRAR` — limpia tokens cuando termina la emergencia

---

*"Es sacrificio de PÉSAJ a YHWH, el cual PASÓ DE LARGO
las casas de los hijos de Israel en Egipto."*
*— Éxodo 12:27*
