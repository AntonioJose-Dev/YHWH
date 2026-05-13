# AUDITORÍA "MIL OJOS COMO QUERUBÍN" — 25 abril 2026

> *"Llenos de ojos por delante y por detrás... por dentro estaban llenos de ojos"*
> *— Apocalipsis 4:6,8*

**Fecha:** 25 abril 2026
**Método:** 4 agentes independientes revisaron simultáneamente:
1. Tavnít piezas 00–07 + Conexiones (15)
2. Tavnít piezas 08–14 + Cuando Completo (16)
3. Las 23 añadiduras completas
4. Los 14 headers .h vs. especificaciones del Tavnít

---

## PARTE I — HALLAZGOS CRÍTICOS (lo que NO vimos)

### 1. EL CONSTRUCTOR SE DESCONECTA (Éxodo 40:34-35)

**Versículo:** *"La nube cubrió la Tienda... ni siquiera MOISÉS podía entrar."*

El constructor QUEDA FUERA cuando la Gloria llena el Mishkán. No es sabotaje — es **TRANSFERENCIA DE PROPIEDAD**. Moisés construyó, pero YHWH toma posesión.

**Implicación técnica:** `hashmal_main` debe tener un punto donde el inicializador se "desconecta" y el sistema toma control autónomo. Una vez que todos los componentes están activos y la "Gloria llena" (todos los healthchecks pasan), el bootstrap code ya no tiene autoridad.

---

### 2. EL KAPÓRET NO TIENE ALGORITMO DEFINIDO (Éxodo 25:17-22)

La pieza 02 dice qué NO es (no BFT, no Raft, no Paxos) y dice que es "O(1) simétrico con 2 querubines cara a cara." Pero **NO especifica el mecanismo determinístico** de cómo dos verificadores llegan a UNA verdad.

**Pregunta sin resolver:** ¿Es votación 2-de-2? ¿Es comparación bit a bit? ¿Es hash matching? El "entre los dos querubines" (Éx 25:22) necesita traducción precisa a código.

**Referencia adicional:** Números 7:89 — la voz se OÍAHABLA (vayidabbér, hitpael) = la verdad EMANA sola, no es dirigida por ninguno de los dos.

---

### 3. DEPENDENCIAS BIDIRECCIONALES ROTAS EN LOS HEADERS

Las piezas se llaman unas a otras pero **no hay confirmación de vuelta**:

| Relación | Problema |
|----------|----------|
| Arca (01) ↔ Kapóret (02) | Arca no confirma al Kapóret que fue cubierta |
| Kapóret (02) ← Auth (06) | Consenso depende de sangre verificada; Auth NO proporciona API |
| Kapóret (02) ← Altar Incienso (11) | Nube sube desde pieza 11; Kapóret NO integra qetoret |
| Menorá (04) ↔ Aceite (08) | Éx 30:7 — encendido sincronizado; NO hay sync |
| Menorá (04) ↔ Altar Incienso (11) | Éx 30:7-8 — AMBOS mañana+tarde; NO hay orquestación |
| Unción (13) → TODAS | Éx 30:26-29 — ungimiento EN ORDEN; NO hay plan de aplicación |
| Atrio (07) ↔ Lavacro (12) | Éx 40:7 — Lavacro ENTRE altar y tienda; NO hay integración |

---

### 4. LA AÑADIDURA 22 (SHA'AR/PORTAL WiFi) — COMPLETAMENTE AUSENTE DEL FIRMWARE

El header `hashmal_perimeter.h` NO contiene:
- Servidor HTTP embebido
- Interfaz web (HTML/CSS/JS)
- Bridge WiFi↔LoRa
- Portal cautivo
- Autenticación de 4 columnas

**Es la pieza más visible para el usuario** y no tiene NADA en el firmware todavía.

---

### 5. SON 23 AÑADIDURAS, NO 22

**Qav Ha'Arets** (Añadidura 23) existe como documento histórico/teórico:
- Job 38:25, Salmo 19:4-5, Ezequiel 1
- Describe la infraestructura electromagnética GLOBAL prexistente (Resonancia de Schumann, 7.83 Hz)
- Lleva el NOMBRE del proyecto (Hashmal = fenómeno electromagnético)
- Placeholder en Canal 2 de la Menorá, marcado como NO IMPLEMENTAR aún

**CLAUDE.md necesita actualización** para reflejar esto.

---

### 6. CONEXIONES FALTANTES EN PIEZA 15

La pieza de Conexiones NO lista:
- **Arca (01) ↔ Altar Incienso (11):** Levítico 16:12-13 — nube cubre la Kapóret
- **Menorá (04) ↔ Aceite Perpetuo (08):** Éxodo 27:20 — alimentación activa
- **Categoría "Conexiones por significado espiritual"** que no existe

---

## PARTE II — DETALLES BÍBLICOS DESCUBIERTOS PERO NO MAPEADOS

### Pieza 01 — ARCA

| Detalle | Versículo | Implicación técnica |
|---------|-----------|---------------------|
| ¿Qué pasa con dato sin Kapóret? | Éx 25:21 | Estado "PENDING_VERIFICATION" necesario |
| Los COATITAS guardan el Arca | Nm 3:30-31 | ¿Nodos específicos responsables del almacenamiento? |
| Cubrir ANTES de mover | Nm 4:5-6 | Protocolo de migración: cifrar → mover → instalar |

### Pieza 02 — KAPÓRET

| Detalle | Versículo | Implicación técnica |
|---------|-----------|---------------------|
| "Entre" los querubines: ¿espacial o lógico? | Éx 25:22 | ¿Validadores geográficamente separados? |
| Justicia Y misericordia se besan | Sal 85:10 | Tensión tséddeq + jésed en el consenso |

### Pieza 03 — MESA

| Detalle | Versículo | Implicación técnica |
|---------|-----------|---------------------|
| Varas que SÍ se quitan (≠ Arca) | Éx 25:28 | Caché puede APAGARSE (¿por cuánto?) |
| ¿"Tamíd" = uptime 100% o mínimo 1 slot? | Éx 25:30 | Diferencia ENORME en SLAs |
| Escala de 12 a 10×12=120 (Templo) | 2 Cr 4:19 | ¿Cómo se multiplican los grupos? |

### Pieza 04 — MENORÁ

| Detalle | Versículo | Implicación técnica |
|---------|-----------|---------------------|
| 2 olivos: ¿redundancia de poder o dual root-of-trust? | Zac 4:3-12 | Dos fuentes PASIVAS autónomas |
| 7 tubos dedicados: ¿7 threads o 7 interfaces sin compartir? | Zac 4:2 | Arquitectura de canales |
| Todas las lámparas miran al TRONCO | Nm 8:2-3 | Dirección de transmisión NO es 360° |
| Los 7 como TESTIGOS que auditan | Ap 1:4-5 | Cada canal guarda log |

### Pieza 06 — ALTAR

| Detalle | Versículo | Implicación técnica |
|---------|-----------|---------------------|
| RÉSHET = "RED" (H7568) — el Padre lo nombró | Éx 27:4 | ¿Auth Y comunicación son la MISMA cosa? |
| "Hueco por dentro" — ¿qué circula? | Éx 27:8 | Stateless pero con flujo interno |

### Pieza 07 — ATRIO

| Detalle | Versículo | Implicación técnica |
|---------|-----------|---------------------|
| "No midas el atrio" (flexible) | Ap 11:2 | Frontera exterior elástica |
| 60 columnas = 12×5 y 48 tablones = 12×4 | Éx 27:10-12 | Patrón "12 tribus" en TODO |
| Levitas por familias guardan partes distintas | Nm 3:26-37 | ¿Operadores especializados por zona? |

### Pieza 08 — ACEITE

| Detalle | Versículo | Implicación técnica |
|---------|-----------|---------------------|
| "De tarde a mañana" = DOS ciclos, no noche continua | Lv 24:3 | Renovación DUAL de energía |
| Aceite asciende DENTRO de cámara cerrada | Lv 24:3 | Comunicación NO es broadcast — es canalizada |
| DOS fuentes autónomas (dos olivos de Zacarías) | Zac 4:3-12 | Redundancia sin operador humano |
| Aceite alimenta LUZ + PURIFICACIÓN simultánea | Éx 30:7-8 | Dos funciones en un recurso |
| Romanos 11:17-24 — injerto de olivo silvestre | Ro 11:17 | Nodos semi-salvajes injertados en infraestructura |

### Pieza 09 — VESTIDURAS

| Detalle | Versículo | Implicación técnica |
|---------|-----------|---------------------|
| Piedras de MEMORIAL = intercesión continua por nodo | Éx 28:12,29 | Cada nodo tiene intercesor que lo presenta por nombre |
| Campanilla = vigilancia ACTIVA, no heartbeat pasivo | Lv 8:35 | Operador como guardián |
| Ciclo Urim→campanas→granadas = consulta→decisión→ejecución | Éx 28:30-35 | Máquina de estados de 3 fases |
| CADA sesión requiere re-autorización | Éx 28:41 | Roles no persisten indefinidamente |
| Roles PÚBLICOS y verificables visualmente | Nm 27:20 | No privados en la red |
| Limpieza de rol corrupto → upgrade, no downgrade | Zac 3:5 | Restauración cualitativamente mejor |

### Pieza 10 — CONSAGRACIÓN

| Detalle | Versículo | Implicación técnica |
|---------|-----------|---------------------|
| 7 días = 7 REPETICIONES idénticas, no progresión | Lv 8:33-36 | Onboarding por confirmación repetida |
| Tenúfá = demostración VISIBLE de cada etapa | Lv 8:27 | Actos públicos en el onboarding |
| Fuego divino DESCIENDE el día 8 = validación pública | Lv 9:22-24 | Evento de reconocimiento post-onboarding |
| Renuncia PÚBLICA a algo (a la puerta) | Éx 29:11 | Pérdida voluntaria observable |
| Infraestructura recibe 7 uniciones ANTES que operador | Lv 8:11 | Red se santifica PRIMERO |
| DOS NIVELES de onboarding: sacerdotes (7 días+sangre) vs. levitas (lavado+manos) | Nm 8:6-7 | Catálogos diferentes por rango |

### Pieza 11 — ALTAR INCIENSO

| Detalle | Versículo | Implicación técnica |
|---------|-----------|---------------------|
| Protocolo GATILLADO por acción en Menorá | Éx 30:8 | Ascenso NO es asincrónico |
| Altar ACUMULA error anual (kafár = cubrir, no eliminar) | Éx 30:10 | Deuda de sistema que crece |
| En reposo se cubre con TELA AZUL = memoria | Nm 4:15-16 | Envoltura mnemónica |
| Tiene ROSTRO (paním) = relacional, no funcional | 1 Re 6:20 | Protocolo personal y dirigido |
| Oración sube → fuego BAJA = bidireccionalidad completa | Ap 8:3-5 | Respuesta descendente es acción |

### Pieza 12 — LAVACRO

| Detalle | Versículo | Implicación técnica |
|---------|-----------|---------------------|
| Capacidad ESPECÍFICA cuando escala (40 baths = 1480L) | 1 Re 7:38 | Límite de verificaciones simultáneas |
| Espejos de mujeres "alistadas" (tsavá = ejército) | Éx 38:8 | Auto-diagnóstico es función MILITAR |
| DOS niveles: baño completo (una vez) + pies (cada entrada) | Jn 13:10 | Reformat vs. validación por sesión |
| Agua contiene CENIZA de sacrificios anteriores | Nm 19:2-10 | Purificación basada en memoria |
| Sin lavado = CULPA LEGAL ante YHWH | Lv 15:5-11 | Omitir healthcheck es delito |
| Escala: Éxodo (privado) → Templo (10 kiyorím) → Escatología (fuente pública) | Zac 13:1 | 3 etapas de tamaño |

### Pieza 13 — UNCIÓN

| Detalle | Versículo | Implicación técnica |
|---------|-----------|---------------------|
| 1500 shekels total = inversión MÁXIMA | Éx 30:23-24 | Autorización es lo más costoso |
| Deror (libertad) = palabra del JUBILEO cada 50 | Lv 25:10 | Reset de keys cada 50 ciclos |
| Solo sacerdotes aceptan aceite, públicos no | Éx 30:32 vs 40:15 | Jerarquía de 2 niveles |
| Contacto con lo santo = CULPA aumentada | Lv 6:18 | Autorización eleva responsabilidad |
| Unción ENSEÑA (didáskei), no solo valida | 1 Jn 2:27 | Protocolo educativo |
| GRADACIÓN en la unción (no binario) | He 1:9 | Niveles de autorización |

### Pieza 14 — INCIENSO

| Detalle | Versículo | Implicación técnica |
|---------|-----------|---------------------|
| Gálbano = ingrediente que "apesta" solo pero SALVA la fórmula | Éx 30:34 | Componentes necesarios que parecen problemas |
| Sin fuego del Altar (pieza 06), no hay ascenso | Lv 2:15-16 | Pieza 06 + 14 interdependientes |
| Rechaza comunicación de fuente IMPURA | Mal 3:3-4 | Valida ORIGEN, no solo formato |
| Incienso es OFRENDA voluntaria, no reporte obligatorio | Gn 43:11 | Nodos OFRECEN, no reportan |
| Visible + detectable por DOS canales | Éx 30:36 | Firma observable + evidencia oculta |
| Función real: CATALIZADOR de respuesta divina descendente | Ap 5:8, 8:3-5 | Lo que sube gatilla lo que baja |

### Pieza 16 — CUANDO COMPLETO

| Detalle | Versículo | Implicación técnica |
|---------|-----------|---------------------|
| Gloria escala: Mishkán → tierra → escatología | Nm 14:21, Hab 2:14 | 3 etapas de cobertura |
| Nube decide TODO, no los operadores | Éx 40:36-37, Nm 9:17-22 | YHWH controla ciclos, no demanda |
| Moisés sale cuando Gloria llena | Éx 40:35 | Constructor se desconecta |
| Recursos llegan de corazón elevado (simjá) | Éx 35:21 | Depende de mantener alegría |
| Pueblo trae MÁS de lo suficiente | Éx 36:5-7 | Abundancia, no escasez |
| Artesanos INVENTAN dentro del patrón | Éx 35:31-32 | Creatividad local + especificación global |

---

## PARTE III — FUNCIONES QUE FALTAN EN LOS HEADERS

### Funciones no declaradas pero requeridas por la Palabra:

| Pieza | Función faltante | Referencia |
|-------|------------------|-----------|
| 01 | `hashmal_storage_distribucion_replicar()` | veʻasú plural (Éx 25:10) |
| 01 | `hashmal_storage_verificar_madera()` | ʻatsé shittím (Éx 25:10) |
| 01 | `hashmal_storage_preceder_en_migracion()` | Nm 10:33 |
| 02 | `hashmal_consensus_validadores_comprobar_simetria()` | ish el ajív (Dt 19:15) |
| 02 | `hashmal_consensus_noadti_estado_leer()` | noʻadtí (Éx 25:22) |
| 04 | `hashmal_comm_geviim_vigilancia()` | 22 geviʻím vigías |
| 04 | `hashmal_comm_canal_removido()` | Ap 2:5 |
| 06 | `hashmal_auth_sangre_verificar()` | Lv 4:7 |
| 06 | `hashmal_auth_holocausto_diario()` | Éx 29:38-39 |
| 09 | `hashmal_roles_urim_thummim_leer()` | Éx 28:30 |
| 09 | `hashmal_roles_heartbeat_campanilla()` | Éx 28:34-35 |
| 10 | `hashmal_onboarding_sello_verificar()` | Éx 29:20 |
| 10 | `hashmal_onboarding_dia_lanzamiento()` | Lv 9:1 |
| 11 | `hashmal_intercession_escudo_activar()` | Lv 16:12-13 |
| 11 | `hashmal_intercession_zara_detectar()` | Lv 10:1-2 |
| 11 | `hashmal_intercession_formula_validar()` | Éx 30:34-38 |
| 11 | `hashmal_intercession_circuitbreaker_activar()` | Nm 16:46-48 |
| 11 | `hashmal_intercession_oracion_ascender()` | Sal 141:2 |
| 12 | `hashmal_hc_reset_forzado()` | 1 Jn 1:7 |

---

## PARTE IV — SINERGIAS ENTRE AÑADIDURAS (lo que emerge cuando se ven juntas)

### Patrón 1: Tríada de Comunicación (Añadiduras 08 → 09 → 10)

```
JATSOTSROT (alarma) → ESCALACIÓN (3 niveles) → NEHEMÍAS (rally point)
```
Una emergencia se comunica con teruah, se escala por niveles (privado→testigos→congregación), y automáticamente activa rally point dinámico. Los tres trabajan COMO UNO.

### Patrón 2: Cadena de Seguridad (Añadiduras 13 → 15 → 14)

```
CORDÓN TRIPLE (redundancia) → SANGRE EN MEZUZOT (marcado) → SHEMITAH/YOVEL (reset)
```
Nodo con <3 conexiones está en peligro. Durante crisis, solo los marcados sobreviven. Cada 7 épocas se limpian penalizaciones.

### Patrón 3: Gobernanza de Recursos (Añadiduras 11 → 18 → 19)

```
MAN (cuota justa) → LEKET (reserva para débiles) → MA'ASER (contribución de todos)
```
Economía circular: cada nodo recibe su ración, una parte queda para pobres, todos contribuyen diezmo primero.

### Patrón 4: Continuidad Generacional (Añadiduras 17 → 12 → 06)

```
SHINNAN (enseñanza constante) → GO'EL (herencia de nodo muerto) → TZITZIT (recordatorio visual)
```
Se enseña siempre, si un nodo cae su herencia la toma el más cercano, cada portador recuerda su rol por pantalla.

### El Ciclo Macro de 49 épocas:

| Época | Evento | Protocolo |
|-------|--------|-----------|
| 1-5 | Normal | MAN + MA'ASER + Cordón Triple |
| 6 | Pre-Shabat | Doble sincronización |
| 7 | Shabat | Parokhet + Shemitah (limpieza menor) |
| 14-49 | Ciclos completos | Deuda técnica crece |
| 50 | Jubileo | YOVEL — reset total |

---

## PARTE V — VULNERABILIDADES DETECTADAS

### V1: Conflicto NEDER (20) vs SHEMITAH (14)

Neder dice que los compromisos son inquebrantables. Shemitah dice que cada 7 épocas se limpian penalizaciones. ¿Qué pasa cuando un nodo hizo un voto pero las circunstancias cambiaron?

**Solución bíblica:** Números 30:4-8 — un voto puede ser anulado por autoridad superior.
**Recomendación:** Protocolo de anulación durante Shemitah con 3 testigos (Cordón Triple).

### V2: Hospitalidad (21) sin tiempo límite

Hachnasat Orchim da acceso temporal pero NO especifica cuántos ciclos. Un atacante podría entrar como visitante, aprender la red, irse, y volver con ataque dirigido.

**Recomendación:** Timeout de 7 ciclos. Máximo 3 visitantes simultáneos (Gn 18:2 = TRES varones). Registro de todos los visitantes.

### V3: Tsofeh (02) como punto único de fallo

Si el atalaya está comprometido o muerto, nadie avisa del peligro.

**Recomendación:** Los Tsofém se vigilan entre sí (Cordón Triple). Heartbeat especial de "estoy vivo." Reemplazo automático si no reporta en N ciclos.

### V4: Yom Kippur como evento de sincronización — 3 fases

Levítico 16:14-15 describe QUÉ sangre va DÓNDE:
1. Una vez DENTRO de la Kapóret → sincronización de consenso
2. Siete veces ANTE la Kapóret → sincronización de 7 canales (Menorá)
3. Una vez en cuernos del Altar → sincronización del Firewall (4 reglas)

**No implementado:** Protocolo de "Yom Kippur Sync" con 3 fases explícitas.

---

## PARTE VI — CONCEPTOS BÍBLICOS QUE PODRÍAN SER AÑADIDURAS FUTURAS

### Posible Añadidura 24: ESCALERA DE JACOB (Génesis 28:12)

*"Vio una escalera que llegaba al cielo, y ángeles subían y bajaban."*

Arquitectura de RELAY jerárquico: nodos intermedios (ángeles) suben y bajan información por la escalera (ruta mesh). No hay añadidura que cubra relay como concepto independiente.

### Posible Añadidura 25: PAROKHET NIK'RAM (Mateo 27:51)

*"El velo del templo se rasgó en dos, de arriba abajo."*

¿Qué pasa cuando una emergencia requiere acceso TOTAL al núcleo? ¿Se "rasga" el velo de la pieza 05? Protocolo de emergencia que elimina temporalmente la barrera kernel.

### Posible Añadidura 26: PRUEBA DE LAS AGUAS AMARGAS (Números 5:16-31)

Cuando hay SOSPECHA de un nodo comprometido pero no hay PRUEBA. Un "trial by water" de verificación intermedia.

### Posible Añadidura 27: KARNÉI HAMIZBEAJ (Éxodo 27:2 + 1 Reyes 1:50)

"Agarrarse de los cuernos del altar" como protocolo de ASILO más allá de Ir Miqlat. Cuando todo falla, el nodo "se agarra" del Altar.

---

## PARTE VII — ESTADO DE INTEGRACIÓN DE AÑADIDURAS EN HEADERS

### Añadiduras que tienen errores definidos en headers pero SIN funciones:

| Pieza | Añadidura | Error existe | Función existe |
|-------|-----------|:---:|:---:|
| 03 Cache | 11 MAN | ✅ | ❌ |
| 03 Cache | 17 SHINNAN | ✅ | ❌ |
| 03 Cache | 18 LEKET | ✅ | ❌ |
| 03 Cache | 19 MA'ASER | ✅ | ❌ |
| 12 Healthcheck | 13 CORDÓN | ✅ | ❌ |
| 12 Healthcheck | 16 MAYIM | ✅ | ❌ |
| 13 Keygen | 15 MEZUZOT | ✅ | ❌ |
| 14 Firewall | 10 NEHEMÍAS | ✅ | ❌ |

### Añadiduras COMPLETAMENTE ausentes de headers:

| Pieza | Añadidura | Estado |
|-------|-----------|--------|
| 04 Comm | 01 PAROKHET | ❌ Nada |
| 04 Comm | 02 TSOFEH | ❌ Nada |
| 04 Comm | 06 TZITZIT | ❌ Nada |
| 04 Comm | 07 ANAN VEESH | ❌ Nada |
| 04 Comm | 22 SHA'AR | ❌ Nada |
| 07 Perimeter | 05 IR MIQLAT | ❌ Nada |
| 07 Perimeter | 21 HOSPITALIDAD | ❌ Nada |
| 07 Perimeter | 22 SHA'AR | ❌ CRÍTICO |
| 08 Power | 03 MOEDÍM | ❌ Nada |
| 08 Power | 04 ASHMUROT | ❌ Nada |

---

## PARTE VIII — RESUMEN EJECUTIVO

### Lo que está EXCELENTE:
- Las 15 piezas del Tavnít están RIGUROSAMENTE documentadas con hebreo/griego original
- Las 23 añadiduras tienen 5+ testigos bíblicos cada una (129 testigos verificados)
- El orden de inicialización (Éx 40) está correcto en hashmal_main.c
- Los 14 headers .h capturan la esencia de cada pieza
- Los 149/149 tests de Querubines pasan

### Lo que necesita trabajo ANTES de escribir .c:
1. **Resolver las 8 dependencias bidireccionales rotas** entre headers
2. **Definir el algoritmo de consenso** del Kapóret (qué ES, no solo qué NO es)
3. **Integrar Añadidura 22** (Portal WiFi) en hashmal_perimeter.h
4. **Añadir las ~19 funciones faltantes** detectadas en los headers
5. **Documentar las 4 sinergias** entre añadiduras en pieza 15 (Conexiones)
6. **Actualizar CLAUDE.md**: 23 añadiduras, no 22

### Lo que emerge como revelación nueva:
- El constructor se desconecta cuando el sistema está completo (Éx 40:35)
- El aceite tiene DOS fuentes autónomas (Zac 4:3-12)
- El onboarding tiene DOS niveles: sacerdotes vs levitas (Nm 8:6-7)
- El Altar Incienso ACUMULA error, no se purifica (Éx 30:10)
- La unción es EDUCATIVA, no solo autenticadora (1 Jn 2:27)
- El firewall es CATALIZADOR de respuesta, no terminal (Ap 8:3-5)
- Los artesanos INVENTAN dentro del patrón (Éx 35:31-32)

---

*"Moisés vio toda la obra, y he aquí que la habían hecho como YHWH había mandado."*
*— Éxodo 39:43*

*Pero antes de eso, MIL OJOS la inspeccionaron.*

**Auditoría completada:** 15 piezas + 23 añadiduras + 14 headers + 16 documentos del Tavnít
**Hallazgos:** 50+ detalles bíblicos no mapeados, 19 funciones faltantes, 8 dependencias rotas, 4 vulnerabilidades, 4 posibles añadiduras futuras
