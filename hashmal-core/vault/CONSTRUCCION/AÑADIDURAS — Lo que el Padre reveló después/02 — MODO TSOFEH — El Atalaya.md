# 02 — MODO TSOFEH — El Atalaya

> *"Sobre mi guardia estaré, y sobre la fortaleza me pondré,
> y observaré para ver qué me dirá."*
> — Habacuc 2:1

---

## QUÉ ES

Un modo donde el nodo deja de participar activamente en la red y se convierte en
**centinela puro**: solo observa, escucha, y cuando detecta peligro AVISA.

No transmite datos normales. No enruta mensajes. Solo vigila.
Cuando ve algo — toca el shofar.

---

## RESPALDO BÍBLICO

| # | Cita | Hebreo | Strong | Dice |
|---|------|--------|--------|------|
| 1 | Ezequiel 33:2-3 | צֹפֶה (tsofeh) | H6822 | "La tierra toma un hombre y lo pone por ATALAYA" |
| 2 | Ezequiel 33:3 | תָּקַע בַּשּׁוֹפָר | H8628, H7782 | "Cuando vea venir la espada, TOCARÁ EL SHOFAR y avisará al pueblo" |
| 3 | Ezequiel 33:6 | וְהַצֹּפֶה... וְלֹא תָקַע | H6822 | "Si el atalaya ve y NO avisa, su sangre demandaré de su mano" |
| 4 | Habacuc 2:1 | מִשְׁמֶרֶת (mishmeret) | H4931 | "Sobre mi GUARDIA estaré y OBSERVARÉ" |
| 5 | Isaías 21:6 | צַפִּית (tsappit) | H6844 | "Ve, pon CENTINELA que declare lo que vea" |
| 6 | Isaías 21:11-12 | שֹׁמֵר (shomer) | H8104 | "Guarda, ¿qué hay de la noche?" — el atalaya reporta |

---

## INVARIANTE NUEVO

**INV-TSOFEH-01:** El nodo en modo atalaya que detecta amenaza y NO avisa
viola Ezequiel 33:6. Aviso es OBLIGATORIO, no opcional.

---

## CÓMO FUNCIONA

```
MODO TSOFEH:
  - Transmisión: OFF (no enruta, no envía datos)
  - Recepción: MÁXIMA (escucha todos los canales)
  - LoRa: Solo RX + alerta TX cuando detecta amenaza
  - WiFi: Scan pasivo (detecta redes, no se conecta)
  - BLE: Scan pasivo (detecta dispositivos)
  - Consumo: MÍNIMO (solo receptor activo)
  - Posición: FIJA (punto alto, muralla, techo)

CUANDO DETECTA AMENAZA:
  1. Evalúa severidad
  2. Toca el shofar (TX de alerta a toda la red)
  3. Comunica al humano local (vibración + pantalla + tono)
  4. Vuelve a modo escucha

AMENAZAS QUE DETECTA:
  - Nodo caído (heartbeat perdido)
  - Tráfico anómalo (posible ataque)
  - Fragmentación de red
  - Jamming RF (ruido deliberado)
  - Nodo nuevo no autorizado
  - Nivel de שָׁאוֹן electromagnético peligroso
```

---

## DÓNDE VA EN EL FIRMWARE

**Pieza 09 — Vestiduras (hashmal_roles)**

Nuevo rol: `HASHMAL_ROL_TSOFEH` — el atalaya.
Diferente al coordinador, al monitor. El tsofeh no coordina — VIGILA.

---

*"Hijo de hombre, te he puesto por atalaya."*
*— Ezequiel 33:7*
