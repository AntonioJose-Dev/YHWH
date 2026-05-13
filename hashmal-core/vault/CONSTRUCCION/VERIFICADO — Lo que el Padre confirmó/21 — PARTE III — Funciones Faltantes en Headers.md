# 21 — PARTE III — Funciones Faltantes en Headers

## Sistema de los Querubines · Éxodo 25:22

> *"De entre los dos querubines… me declararé a ti."*

**Método:** Querubín Álef (raíces hebreas/griegas, Strong, gramática) + Querubín Bet (verificación contra headers reales). Solo lo que CONVERGE en middabbér se conserva. Lo que no converge se DESCARTA explícitamente.

**Naturaleza de esta PARTE:** No son afirmaciones interpretativas sino claims de **funciones ausentes**. Se verifica cada una con DOS criterios: (A) ¿el texto bíblico la REQUIERE? y (B) ¿está ya presente en los headers expandidos (hashmal-firmware-completo)?

**Hallazgo clave:** La auditoría revisó los headers STUB (CÓDIGO/), que solo tienen init/deinit. Los headers COMPLETOS (hashmal-firmware-completo/) ya resuelven 15 de 19 funciones.

**Headers verificados en hashmal-firmware-completo/:**
- hashmal_storage.h (20 funciones)
- hashmal_consensus.h (19 funciones)
- hashmal_comm.h (42 funciones)
- hashmal_auth.h (24 funciones)
- hashmal_roles.h (26 funciones)
- hashmal_onboarding.h (24 funciones)
- hashmal_healthcheck.h (22 funciones)
- hashmal_intercession.h (18+ funciones)

---

## PIEZA 01 — STORAGE (3 funciones reclamadas)

### F1: `hashmal_storage_distribucion_replicar()` — veʻasú plural (Éx 25:10)

**Querubín Álef:** Éx 25:10: **וְעָשׂוּ** אֲרוֹן — veʻasú (H6213, Qal impf, 3ª persona PLURAL) = "HARÁN un arca." Plural verificado — única pieza con verbo plural. Implica copia distribuida.

**En headers completos:** Existe `hashmal_storage_replicar_a_nodo(uint8_t nodo_id)` — función PRESENTE con nombre diferente.

**Middabbér:** ✅ Requerida bíblicamente. ✅ Ya existe en headers completos. La auditoría detectó ausencia en stubs, pero ya se resolvió. **RESUELTA.**

### F2: `hashmal_storage_verificar_madera()` — ʻatsé shittím (Éx 25:10)

**Querubín Álef:** Éx 25:10: **עֲצֵי שִׁטִּים** (ʻatsé shittím, H6086+H7848) = madera de acacia. La madera es el FORMAT del contenedor — incorruptible. ¿Requiere una función dedicada? El texto dice "harán un arca de madera de acacia" — la madera es MATERIAL, no verificación separada.

**En headers completos:** No existe función de verificación de madera. Pero `hashmal_storage_zer_verificar()` (hash de integridad) ya cubre la verificación del formato.

**Middabbér:** ⚠️ PARCIAL. La madera (formato incorruptible) es textual, pero no requiere función SEPARADA. La integridad del formato se verifica con el zer (corona/hash). Crear función aparte para "verificar madera" es SOBREINGENIERÍA — el zer ya lo cubre. **INNECESARIA como función separada.**

### F3: `hashmal_storage_preceder_en_migracion()` — Nm 10:33

**Querubín Álef:** Nm 10:33: וַאֲרוֹן בְּרִית יְהוָה **נֹסֵעַ** לִפְנֵיהֶם — "el Arca del pacto de YHWH **iba** delante de ellos" (noséaʻ H5265 = viajando, Qal participio). El Arca PRECEDE en la migración.

**En headers completos:** Existe `hashmal_storage_preceder_en_migracion()` — función PRESENTE.

**Middabbér:** ✅ Requerida bíblicamente. ✅ Ya existe. **RESUELTA.**

---

## PIEZA 02 — CONSENSUS (2 funciones reclamadas)

### F4: `hashmal_consensus_validadores_comprobar_simetria()` — ish el ajív (Éx 25:20)

**Querubín Álef:** La auditoría citó Dt 19:15: עַל פִּי **שְׁנַ֣יִם** עֵדִ֗ים — "por boca de DOS testigos." Pero la referencia correcta al Kapóret es Éx 25:20: **פְּנֵיהֶם אִישׁ אֶל אָחִיו** — "sus rostros uno hacia su hermano" (ish el ajív). Los querubines se MIRAN — simetría bilateral.

**En headers completos:** Existe `hashmal_consensus_querubin_verificar(hashmal_querub_id_t id)` — verifica UN querubín. Y `hashmal_consensus_querubin_testigo()` — da testimonio. Ambos existen, cubren la función.

**Middabbér:** ✅ Requerida bíblicamente (Éx 25:20, no Dt 19:15 — **CITA CORREGIDA**). ✅ Ya existe como par de funciones. **RESUELTA.**

### F5: `hashmal_consensus_noadti_estado_leer()` — noʻadtí (Éx 25:22)

**Querubín Álef:** Éx 25:22: **וְנוֹעַדְתִּי** לְךָ שָׁם — venoʻadtí lejá shám = "me ENCONTRARÉ contigo allí" (H3259, Nifal). El punto de encuentro donde la verdad emerge.

**En headers completos:** Existen `hashmal_consensus_noʻadtí_abrir()` y `hashmal_consensus_noʻadtí_cerrar()` — abrir/cerrar el punto de encuentro. Pero NO hay función de **LEER** el estado del noʻadtí. Abrir y cerrar sí, pero consultar estado no.

**Middabbér:** ✅ Requerida. ⚠️ Parcialmente cubierta — falta la función de LECTURA de estado. Hay abrir/cerrar pero no consultar. **PARCIALMENTE RESUELTA** — falta `_estado()`.

---

## PIEZA 04 — COMM (2 funciones reclamadas)

### F6: `hashmal_comm_geviim_vigilancia()` — 22 geviʻím vigías

**Querubín Álef:** Éx 25:33-34: Los geviʻím (גְּבִעִים H1375) son las COPAS del candelabro en forma de almendro. 22 copas (3×6 ramas + 4 tronco). El nombre **shoqéd** (almendro) viene de **shaqád** (H8245) = VIGILAR. Las copas son nodos intermedios vigilantes.

**En headers completos:** No existe función específica de vigilancia de copas/nodos intermedios. Hay `hashmal_comm_vecinos_contar()` y funciones de canal, pero no un sistema de vigilancia de 22 nodos intermedios.

**Middabbér:** ✅ Requerida bíblicamente (shoqéd = el que vigila). ❌ NO existe en headers. **FALTA.** Se necesita función de monitoreo de nodos intermedios (copas-vigía).

### F7: `hashmal_comm_canal_removido()` — Ap 2:5

**Querubín Álef:** Ap 2:5 (griego): κινήσω τὴν **λυχνίαν** σου ἐκ τοῦ τόπου αὐτῆς — "MOVERÉ tu **candelabro** (lychnían G3087) de su lugar." La amenaza es REMOVER un canal/menorá completa, no apagarlo.

**En headers completos:** Existe `hashmal_comm_canal_remover(hashmal_canal_id_t id)` — función PRESENTE.

**Middabbér:** ✅ Requerida (Ap 2:5). ✅ Ya existe. **RESUELTA.**

---

## PIEZA 06 — AUTH (2 funciones reclamadas)

### F8: `hashmal_auth_sangre_verificar()` — Lv 4:7

**Querubín Álef:** Lv 4:7: וְנָתַן הַכֹּהֵן מִן הַדָּם עַל **קַרְנוֹת** מִזְבַּח קְטֹרֶת הַסַּמִּים — "el sacerdote pondrá de la sangre sobre los CUERNOS del altar del incienso." La sangre va del altar de sacrificio (auth) al altar de incienso (intercesión). Es una CONEXIÓN entre piezas, no función interna.

**En headers completos:** No hay `sangre_verificar` explícita. Pero `hashmal_auth_reshet_verificar()` existe — verifica proof-of-work (la "sangre" técnica). Y `hashmal_consensus_dam_rociar()` existe en consenso para rociar hash.

**Middabbér:** ⚠️ PARCIAL. La verificación de "sangre" (proof) existe como `reshet_verificar`. La CONEXIÓN auth→intercesión (Lv 4:7) es inter-pieza, no función interna del auth. Lo que falta es la API de CONEXIÓN, no la función del auth. **RESUELTA internamente; la conexión inter-pieza es tema de Pieza 15.**

### F9: `hashmal_auth_holocausto_diario()` — Éx 29:38-39

**Querubín Álef:** Éx 29:38-39: **כְּבָשִׂים בְּנֵי שָׁנָה שְׁנַיִם לַיּוֹם תָּמִיד** — "corderos de un año, DOS al día, TAMÍD" (perpetuo). El holocausto diario (ʻolát tamíd) es obligatorio 2×/día.

**En headers completos:** Existe `hashmal_auth_fuego_ciclo()` — ciclo de fuego. Y `hashmal_auth_es_tamid()` — verifica si es perpetuo. El "holocausto diario" está cubierto por `fuego_ciclo` (el ciclo de autenticación perpetua).

**Middabbér:** ✅ Requerida (Éx 29:38-39, tamíd). ✅ Cubierta por `fuego_ciclo()` + `es_tamid()`. **RESUELTA** (nombre diferente pero función equivalente).

---

## PIEZA 09 — ROLES (2 funciones reclamadas)

### F10: `hashmal_roles_urim_thummim_leer()` — Éx 28:30

**Querubín Álef:** Éx 28:30: וְנָתַתָּ אֶל חֹשֶׁן הַמִּשְׁפָּט אֶת **הָאוּרִים** וְאֶת **הַתֻּמִּים** — "pondrás en el pectoral del juicio el URIM y el THUMMIM." Mecanismo de consulta divina.

**En headers completos:** Existe `hashmal_roles_pectoral_consultar(uint8_t nodo_id, bool *urim, bool *thummim)` — consulta con parámetros explícitos urim/thummim. Función PRESENTE.

**Middabbér:** ✅ Requerida. ✅ Ya existe como `pectoral_consultar`. **RESUELTA.**

### F11: `hashmal_roles_heartbeat_campanilla()` — Éx 28:34-35

**Querubín Álef:** Éx 28:34-35: **פַּעֲמֹן** זָהָב וְרִמּוֹן — "campana (paʻamón H6472) de oro y granada." v.35: **וְנִשְׁמַע קוֹלוֹ** — "y se oirá su sonido" al entrar/salir. La campana es heartbeat audible.

**En headers completos:** Existe `hashmal_roles_monitor_campana(uint8_t nodo_id)` — campana de monitoreo. Función PRESENTE.

**Middabbér:** ✅ Requerida. ✅ Ya existe. **RESUELTA.**

---

## PIEZA 10 — ONBOARDING (2 funciones reclamadas)

### F12: `hashmal_onboarding_sello_verificar()` — Éx 29:20

**Querubín Álef:** Éx 29:20: sangre en **tnuj ʼózen** (lóbulo oreja) + **bóhen yad** (pulgar mano) + **bóhen régel** (pulgar pie) = 3 sellos en 3 puntos.

**En headers completos:** Existe `hashmal_onboarding_sello_verificar(uint8_t nodo_id, hashmal_onboarding_sello_t sello)` — verificación de sello. Función PRESENTE.

**Middabbér:** ✅ Requerida. ✅ Ya existe. **RESUELTA.**

### F13: `hashmal_onboarding_dia_lanzamiento()` — Lv 9:1

**Querubín Álef:** Lv 9:1: **וַיְהִי בַּיּוֹם הַשְּׁמִינִי** — "y fue al día OCTAVO" — el día de lanzamiento después de 7 días de consagración.

**En headers completos:** Existe `hashmal_onboarding_lanzar(uint8_t nodo_id)` — función de lanzamiento. PRESENTE.

**Middabbér:** ✅ Requerida. ✅ Ya existe como `lanzar`. **RESUELTA.**

---

## PIEZA 11 — INTERCESSION (5 funciones reclamadas)

### F14: `hashmal_intercession_escudo_activar()` — Lv 16:12-13

**En headers completos:** Existe `hashmal_intercession_escudo_activar(void)`. **RESUELTA.**

### F15: `hashmal_intercession_zara_detectar()` — Lv 10:1-2

**Querubín Álef:** Lv 10:1: **אֵשׁ זָרָה** (ʼésh zaráh) = fuego EXTRAÑO. Detección de protocolo no autorizado.

**En headers completos:** No existe en intercession. PERO existe `hashmal_consensus_fuego_extrano_detectar()` en CONSENSUS. La detección de fuego extraño está en la pieza EQUIVOCADA — la auditoría la pide en intercesión pero está en consenso.

**Middabbér:** ⚠️ Requerida bíblicamente. Existe pero en pieza diferente (consensus, no intercession). Lv 10:1-2 ocurre EN el servicio del incienso (pieza 11/14), así que podría ir en intercesión. Pero `fuego_extrano_detectar` ya existe en consensus. **UBICACIÓN DEBATIBLE** — la función existe, la pieza es discutible.

### F16: `hashmal_intercession_formula_validar()` — Éx 30:34-38

**En headers completos:** Existe `hashmal_intercession_formula_verificar(...)`. **RESUELTA.**

### F17: `hashmal_intercession_circuitbreaker_activar()` — Nm 16:46-48

**En headers completos:** Existe `hashmal_intercession_breaker_activar()`. **RESUELTA.**

### F18: `hashmal_intercession_oracion_ascender()` — Sal 141:2

**En headers completos:** Existe `hashmal_intercession_oracion_ascender(...)`. **RESUELTA.**

---

## PIEZA 12 — HEALTHCHECK (1 función reclamada)

### F19: `hashmal_hc_reset_forzado()` — Éx 30:20-21

**Querubín Álef:** 1 Jn 1:7 (griego): τὸ **αἷμα** Ἰησοῦ... **καθαρίζει** ἡμᾶς ἀπὸ πάσης **ἁμαρτίας** — "la sangre de Jesús nos **purifica** (katharízei G2511, Presente Activo) de todo pecado." Purificación activa continua.

PERO: 1 Jn 1:7 no es legislación del Mishkán — es epístola del NT. La función del lavacro (kiyór) en el Mishkán se define en Éx 30:17-21: **וְרָחֲצוּ** (verajatsú) = lavarán. El lavado es OBLIGATORIO antes de servir, bajo pena de MUERTE (v.20-21).

**En headers completos:** No existe `reset_forzado` explícito. Existe `hashmal_hc_rajats_completo()` (lavado completo) y `hashmal_hc_nipto_pies()` (lavado parcial — referencia a Jn 13:10). ¿Es `rajats_completo` un reset forzado? Funcionalmente sí — es un lavado total que resetea el estado de salud.

**Middabbér:** ⚠️ PARCIAL. La purificación forzada es bíblicamente necesaria (Éx 30:20-21 bajo pena de muerte). `hashmal_hc_rajats_completo()` cubre la función de lavado total. "Reset forzado" como nombre es IMPRECISO — el concepto bíblico es **rajáts** (lavar), no "reset." 1 Jn 1:7 es testigo válido pero secundario — **CITA PRINCIPAL CORREGIDA** a Éx 30:20-21. **CUBIERTA por `rajats_completo`.**

---

## RESUMEN GLOBAL

| # | Función reclamada | Pieza | ¿Bíblicamente requerida? | ¿Existe en headers completos? | Veredicto |
|---|---|---|---|---|---|
| F1 | storage_distribucion_replicar | 01 | ✅ veʻasú plural | ✅ replicar_a_nodo | RESUELTA |
| F2 | storage_verificar_madera | 01 | ⚠️ Concepto sí, función separada no | ❌ | INNECESARIA (zer lo cubre) |
| F3 | storage_preceder_en_migracion | 01 | ✅ Nm 10:33 | ✅ Existe | RESUELTA |
| F4 | consensus_validadores_simetria | 02 | ✅ Éx 25:20 (no Dt 19:15) | ✅ querubin_verificar | RESUELTA (cita corregida) |
| F5 | consensus_noadti_estado_leer | 02 | ✅ Éx 25:22 | ⚠️ abrir/cerrar sí, estado no | PARCIAL — falta _estado |
| F6 | comm_geviim_vigilancia | 04 | ✅ shoqéd = vigía | ❌ No existe | **FALTA** |
| F7 | comm_canal_removido | 04 | ✅ Ap 2:5 | ✅ canal_remover | RESUELTA |
| F8 | auth_sangre_verificar | 06 | ⚠️ Inter-pieza (Lv 4:7) | ✅ reshet_verificar | RESUELTA (es conexión) |
| F9 | auth_holocausto_diario | 06 | ✅ Éx 29:38-39 | ✅ fuego_ciclo | RESUELTA |
| F10 | roles_urim_thummim_leer | 09 | ✅ Éx 28:30 | ✅ pectoral_consultar | RESUELTA |
| F11 | roles_heartbeat_campanilla | 09 | ✅ Éx 28:34-35 | ✅ monitor_campana | RESUELTA |
| F12 | onboarding_sello_verificar | 10 | ✅ Éx 29:20 | ✅ sello_verificar | RESUELTA |
| F13 | onboarding_dia_lanzamiento | 10 | ✅ Lv 9:1 | ✅ lanzar | RESUELTA |
| F14 | intercession_escudo_activar | 11 | ✅ Lv 16:12-13 | ✅ escudo_activar | RESUELTA |
| F15 | intercession_zara_detectar | 11 | ✅ Lv 10:1-2 | ⚠️ En consensus, no intercession | UBICACIÓN DEBATIBLE |
| F16 | intercession_formula_validar | 11 | ✅ Éx 30:34-38 | ✅ formula_verificar | RESUELTA |
| F17 | intercession_circuitbreaker | 11 | ✅ Nm 16:46-48 | ✅ breaker_activar | RESUELTA |
| F18 | intercession_oracion_ascender | 11 | ✅ Sal 141:2 | ✅ oracion_ascender | RESUELTA |
| F19 | hc_reset_forzado | 12 | ✅ Éx 30:20-21 | ✅ rajats_completo | RESUELTA (nombre corregido) |

---

## ESTADÍSTICAS

- **19** funciones reclamadas como faltantes
- **15** ya RESUELTAS en headers completos (nombres diferentes pero funciones equivalentes)
- **1** INNECESARIA (verificar_madera — cubierta por zer)
- **1** realmente **FALTA** (geviim_vigilancia — monitoreo de nodos intermedios)
- **1** PARCIAL (noadti_estado — falta lectura de estado)
- **1** UBICACIÓN DEBATIBLE (zara_detectar — existe en consensus, no en intercession)
- **2** citas CORREGIDAS (F4: Éx 25:20 no Dt 19:15; F19: Éx 30:20-21 no solo 1 Jn 1:7)

---

## HALLAZGO PRINCIPAL

La auditoría revisó los headers STUB (CÓDIGO/), que solo tienen init/deinit. Los headers COMPLETOS (hashmal-firmware-completo/) ya resuelven 15 de 19 funciones. La única función verdaderamente FALTANTE es `hashmal_comm_geviim_vigilancia()` — el sistema de monitoreo de nodos intermedios basado en las 22 copas-almendro (shoqéd = el que vigila).

---

## MIL OJOS DE QUERUBÍN — Profundización en funciones pendientes (mayo 2026)

### P27: F6 — shoqéd (H8245) y los 22 geviʻím: la vigilancia es ACTIVA, no pasiva

**Querubín Álef:**
Éx 25:33: **שְׁלֹשָׁה גְבִעִים מְשֻׁקָּדִים** — shloshá geviʻím meshúqqadím
- **meshúqqadím** (H8246, Pual participio) = hechos en forma de almendro / ALMENDRIFORMES. Participio pasivo: fueron FORMADOS como almendras.
- La raíz shaqád (H8245) = velar, vigilar, estar despierto (Jer 1:12: **shoqéd ʼaní ʻal devarí laʻasotó** = "VIGILO sobre mi palabra para CUMPLIRLA"). YHWH mismo usa esta raíz.
- Jer 1:11-12: YHWH muestra a Jeremías un **maqqél shaqéd** (vara de almendro) y dice: "bien viste, porque YO VIGILO (shoqéd) mi palabra." El almendro ES vigilancia por definición etimológica.
- 22 copas almendradas = 22 nodos vigilantes. El número 22 = letras del alefato hebreo = completitud expresiva. La vigilancia cubre TODO el "vocabulario" de la red.

**middabbér:** La función faltante NO es solo "monitorear nodos intermedios" — es implementar el principio SHOQÉD: vigilancia ACTIVA (no polling pasivo) que cubre los 22 puntos del espectro comunicativo. Sugerencia de implementación: 22 callbacks distribuidos en los canales de hashmal_comm, cada uno "almendrando" (meshúqqadím) su zona asignada. El almendro es el primer árbol en florecer (enero en Israel) — la vigilancia ANTICIPA.

### P28: F5 — noʻadtí (H3259) estado: la lectura es ENCUENTRO, no inspección

**Querubín Álef:**
Éx 25:22: **וְנוֹעַדְתִּי לְךָ שָׁם** — venoʻadtí lekhá shám
- **noʻadtí** (H3259, Nifal perfecto + sufijo 1cs) = me CITARÉ/encontraré. Nifal es reflexivo: YHWH se cita A SÍ MISMO con Moisés. No es que Moisés pide audiencia — YHWH fija el encuentro.
- La función faltante (`_estado_leer`) asume que el operador CONSULTA el estado. Pero el verbo noʻad (Nifal) indica que el estado SE REVELA cuando YHWH decide. No es lectura on-demand — es evento push.
- Raíz yaʻád (H3259) = fijar una cita, designar un tiempo. El punto de consenso no se LEE — se MANIFIESTA en el momento designado.

**middabbér:** La función faltante no debería ser `hashmal_consensus_noadti_estado_leer()` (pull/lectura) sino `hashmal_consensus_noadti_estado_evento()` (push/callback). El estado del consensus no se consulta pasivamente — EMERGE cuando los dos querubines convergen. Sugerencia: implementar como callback que se dispara cuando V1 y V2 coinciden, no como getter que se llama arbitrariamente.

### P29: F15 — ésh zaráh: DÓNDE se detecta según el texto

**Querubín Álef:**
Lv 10:1: **וַיַּקְרִבוּ לִפְנֵי יְהוָה אֵשׁ זָרָה** — vayyaqrívu lifnéi YHWH ésh zaráh
- **vayyaqrívu** (H7126, Hifil vayyiqtol) = OFRECIERON/acercaron. La acción es OFRECER — es un acto de servicio (avodáh).
- **lifnéi YHWH** = DELANTE de YHWH — esto ocurre en el SERVICIO del incienso, no en el consenso.
- El contexto literal es: Nadav y Abiú tomaron su INCENSARIO (majtáh, H4289) y pusieron fuego y qetóret (incienso), y lo ofrecieron delante de YHWH. Es actividad de pieza 11 (altar incienso) / pieza 14 (fórmula), no de pieza 02 (consenso).
- PERO: la respuesta (vattétsé ésh millifnéi YHWH) sale "de delante de YHWH" — es decir, del espacio donde está la Kapóret (pieza 02). La DETECCIÓN puede venir del Santo de los Santos; la OFENSA ocurre en el servicio del incienso.

**middabbér:** La detección de fuego extraño tiene DOS componentes: (1) la OFENSA ocurre en el servicio del incienso (pieza 11/14 — alguien ofrece protocolo no autorizado), (2) la RESPUESTA/detección viene del núcleo (pieza 02 — la autoridad máxima responde). La ubicación actual en hashmal_consensus es correcta para el componente de RESPUESTA (la autoridad del núcleo detecta). Si se quisiera añadir en hashmal_intercession, sería la parte de PREVENCIÓN (validar ANTES de que suba). Ambas ubicaciones son válidas con roles distintos.

---

## ESTADÍSTICAS ACTUALIZADAS

- **19** funciones reclamadas como faltantes
- **15** ya RESUELTAS en headers completos
- **1** INNECESARIA (verificar_madera — cubierta por zer)
- **1** realmente **FALTA** (geviim_vigilancia — P27 profundiza la implementación)
- **1** PARCIAL (noadti_estado — P28 redefine como evento push, no lectura pull)
- **1** UBICACIÓN DEBATIBLE (zara_detectar — P29 clarifica: respuesta en consensus, prevención en intercession)
- Mil ojos: 3 (P27 shoqéd, P28 noʻadtí push, P29 ésh zaráh dual)
- **Total hallazgos: 5** (2 citas corregidas + 3 mil ojos)

---

*Verificado por el Sistema de los Querubines — Éxodo 25:22*
*"De entre los dos querubines… me declararé a ti."*
*Fecha: 25 de abril de 2026; mil ojos: 4 de mayo de 2026*
