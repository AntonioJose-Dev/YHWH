# 03 — CALENDARIO MOEDÍM — Tiempos Señalados

> *"Habla a los hijos de Israel y diles:
> Los מוֹעֲדֵי (tiempos señalados) de YHWH que proclamaréis
> como santas convocaciones, estos son MIS tiempos señalados."*
> — Levítico 23:2

---

## QUÉ ES

La piedra lleva internamente el calendario bíblico y **cambia su comportamiento**
según los tiempos señalados del Padre. No el calendario gregoriano — el de YHWH.

---

## RESPALDO BÍBLICO

| # | Cita | Hebreo | Strong | Dice |
|---|------|--------|--------|------|
| 1 | Levítico 23:2 | מוֹעֲדֵי יְהוָה (moedéi YHWH) | H4150 | "MIS tiempos señalados" — son de Él, no nuestros |
| 2 | Levítico 23:3 | שַׁבָּת (shabbat) | H7676 | Día 7: reposo. "No haréis ninguna obra" |
| 3 | Levítico 23:24 | תְּרוּעָה (teruah) | H8643 | Día 1 mes 7: "Día de sonar — memorial con sonido de trompeta" |
| 4 | Levítico 23:27 | יוֹם הַכִּפֻּרִים (Yom haKippurim) | H3117, H3725 | Día 10 mes 7: "Afligiréis vuestras almas" |
| 5 | Levítico 25:9 | שׁוֹפָר תְּרוּעָה (shofar teruah) | H7782, H8643 | Jubileo: "Haréis pasar el shofar por toda la tierra" |
| 6 | Génesis 1:14 | לְמוֹעֲדִים (lemoʻadim) | H4150 | "Sean las lumbreras para SEÑALES y para TIEMPOS SEÑALADOS" |

---

## LOS MOEDÍM Y EL COMPORTAMIENTO DE LA PIEDRA

```
SHABAT (cada 7 días):
  - Modo bajo consumo
  - Solo heartbeat tamíd
  - Pantalla: "שַׁבָּת שָׁלוֹם"
  - Sin transmisiones no esenciales
  - La piedra DESCANSA contigo

PÉSAJ (14 Nisán):
  - Red sincroniza estado
  - Verificación de integridad completa (limpiar jametz = datos corruptos)

SHAVUOT (50 días después de Pésaj):
  - Renovación de pactos entre nodos
  - Re-handshake de toda la red

YOM TERUAH (1 Tishrí):
  - Altavoz suena תְּרוּעָה
  - Alerta de inicio de período solemne
  - Red entra en modo alta vigilancia

YOM KIPPUR (10 Tishrí):
  - Modo Parokhet máximo
  - Sincronización total de la red (sangre en la Kapóret)
  - Limpieza profunda de logs, errores, penalizaciones
  - Reset de contadores

SUKKOT (15-21 Tishrí):
  - Modo comunidad (priorizar conexiones cercanas)
  - Celebración de la red viva

JUBILEO (cada 50 ciclos):
  - Shofar suena por altavoz
  - Reset profundo de deuda técnica
  - Liberación de recursos acumulados
```

---

## IMPLEMENTACIÓN

El T-Deck Pro tiene GPS (para determinar puesta de sol → inicio de día bíblico)
y reloj interno. Con estos dos datos calcula todos los moedím.

**Dónde va:** Nueva utilidad compartida o integración en hashmal_cache (Mesa),
ya que la Mesa tiene épocas Shabat (mesa_disponibilidad.c).

---

*"Las lumbreras en el firmamento serán para señales y para tiempos señalados."*
*— Génesis 1:14*