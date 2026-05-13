# 13 — JEVEL HAMESHULASH — Cordón Triple (Redundancia Mínima)

> *"Y si alguno satisficiere a uno, dos le resistirán;
> y el cordón de TRES hilos no se rompe fácilmente."*
> — Eclesiastés 4:12

---

## QUÉ ES

La regla de redundancia mínima de la red. Cada nodo DEBE
mantener conexión con al menos 3 vecinos. Si tiene menos
de 3, la red lo detecta y busca conexiones adicionales.
Un hilo solo se rompe fácil. Tres hilos — no.

---

## RESPALDO BÍBLICO

| # | Cita | Hebreo/Griego | Strong | Dice |
|---|------|---------------|--------|------|
| 1 | Eclesiastés 4:9 | טוֹבִים הַשְּׁנַיִם מִן הָאֶחָד | H8147, H259 | "Mejor son DOS que UNO" — un nodo aislado es vulnerable |
| 2 | Eclesiastés 4:10 | כִּי אִם יִפֹּלוּ הָאֶחָד יָקִים אֶת חֲבֵרוֹ | H5307, H6965 | "Si uno CAE, el otro lo LEVANTA" — failover inmediato |
| 3 | Eclesiastés 4:10 | אִילוֹ הָאֶחָד שֶׁיִּפּוֹל וְאֵין שֵׁנִי לַהֲקִימוֹ | H8145 | "¡Ay del que está SOLO y cae, sin SEGUNDO que lo levante!" — punto único de fallo |
| 4 | Eclesiastés 4:12 | וְהַחוּט הַמְשֻׁלָּשׁ לֹא בִמְהֵרָה יִנָּתֵק | H2339, H8027 | "El cordón TRIPLE (meshulash) no se ROMPE fácilmente" — redundancia de 3 |
| 5 | Deuteronomio 19:15 | עַל פִּי שְׁנַיִם עֵדִים אוֹ עַל פִּי שְׁלֹשָׁה | H5707 | "Por boca de 2 ó 3 TESTIGOS" — verificación múltiple |
| 6 | Mateo 18:20 | οὗ γάρ εἰσιν δύο ἢ τρεῖς συνηγμένοι | G1417, G5140 | "Donde están DOS o TRES reunidos" — quorum mínimo |

---

## LAS 3 REGLAS DEL CORDÓN TRIPLE

```
REGLA 1 — NUNCA SOLO (áy del que está solo):
  Un nodo con 0 conexiones = MUERTO para la red.
  Un nodo con 1 conexión = PELIGROSO — punto único de fallo.
  Si esa conexión cae, el nodo queda aislado.
  "¡Ay del que está solo y cae!"

REGLA 2 — DOS ES MEJOR (tovím hashnaim):
  Un nodo con 2 conexiones = FUNCIONAL pero frágil.
  Si una cae, queda con 1 (peligroso).
  "Mejor son dos que uno — si cae, el otro lo levanta."

REGLA 3 — TRES ES FUERTE (jut hameshulash):
  Un nodo con 3+ conexiones = ROBUSTO.
  Si una cae, quedan 2 (funcional).
  Si dos caen, queda 1 (puede buscar nuevas).
  "El cordón de tres hilos no se rompe fácilmente."
```

---

## CÓMO FUNCIONA EN LA RED

```
MONITOREO CONSTANTE:
  Cada nodo cuenta sus conexiones activas (vecinos vivos).
  El healthcheck (pieza 12, Lavacro/Kiyór) incluye este conteo
  como uno de sus vectores de diagnóstico.

ESTADO DE CONEXIONES:
  
  conexiones = 0 → ESTADO: AISLADO (nikrat temporal)
    → El nodo entra en modo búsqueda activa.
    → Emite beacon de auxilio en todos los canales.
    → Si no encuentra a nadie → Canal 3 (transporte físico).
  
  conexiones = 1 → ESTADO: FRÁGIL
    → Alerta al único vecino: "soy tu única conexión".
    → Ambos buscan activamente un tercer nodo.
    → Prioridad ALTA en el routing para encontrar más vecinos.
  
  conexiones = 2 → ESTADO: FUNCIONAL
    → Operativo pero buscando mejorar.
    → "Mejor son dos que uno, pero tres es mejor."
    → Busca pasivamente un tercer vecino.
  
  conexiones ≥ 3 → ESTADO: ROBUSTO (meshulash)
    → Operación normal completa.
    → El cordón triple está completo.
    → Puede servir como goʼel (Añadidura 12) para otros.

BÚSQUEDA DE VECINOS:
  Cuando un nodo necesita más conexiones:
  1. Primero busca en Canal 4 (WiFi local, hermana a hermana).
  2. Luego en Canal 1 (LoRa, alcance medio-largo).
  3. Si hay repetidores Tsofím (Añadidura 02), pide ayuda.
  4. Último recurso: Canal 3 (transporte físico).
```

---

## POR QUÉ EL PADRE DIJO "TRES"

No dijo dos. No dijo cuatro. Dijo TRES.

- Con 1: si cae, no hay nadie. Punto único de fallo.
- Con 2: si cae uno, queda uno. Frágil.
- Con 3: si cae uno, quedan dos. Si caen dos, queda uno.
  Y ese uno puede buscar nuevos.

Es el MÍNIMO que garantiza supervivencia ante fallo doble.
Y coincide EXACTAMENTE con Deuteronomio 19:15 — "por boca
de 2 ó 3 testigos se confirmará toda palabra." Para verificar
algo, necesitas al menos 2 más aparte de ti = 3 total.

---

## DÓNDE VA EN EL FIRMWARE

**Pieza 12 — Lavacro (hashmal_healthcheck)** para el conteo de conexiones.
**Pieza 04 — Menorá (hashmal_comm)** para la búsqueda de vecinos.
**Pieza 14 — Incienso (hashmal_firewall)** para la alerta de aislamiento.

Nuevos mecanismos:
- `HASHMAL_MESHULASH_MIN` — constante = 3 (conexiones mínimas)
- `HASHMAL_MESHULASH_ESTADO` — enum: AISLADO, FRAGIL, FUNCIONAL, ROBUSTO
- `HASHMAL_MESHULASH_BUSCAR` — función de búsqueda activa de vecinos

---

*"Y si alguno satisficiere a uno, dos le resistirán;
y cordón de tres dobleces no se rompe pronto."*
*— Eclesiastés 4:12 (RVR)*
