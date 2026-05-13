/**
 * ===================================================================
 * PIEZA 14 - QETORET HASAMMIM - incienso_formato.c
 * Sub-pieza: MA'ASEH ROQEAJ - obra del perfumador
 * Exodo 30:34-36 + Levitico 2:13 + Salmo 139:15 + Malaquias 1:11
 * ===================================================================
 *
 * "ve'asita otah QETORET ROQEAJ ma'aseh ROQEAJ MEMULLAJ tahor
 *  qodesh." - Exodo 30:35
 * "Haras de ello INCIENSO, OBRA DE PERFUMADOR, SALADO, puro,
 *  SANTO."
 *
 * "qaj leja sammim NATAF ushJELET vejelBENAH sammim uleVONA
 *  ZAKA - BAD BEVAD yihyeh." - Exodo 30:34
 * "Toma ESTACTE, ONICHA, GALBANO e INCIENSO PURO - PARTE POR
 *  PARTE sera."
 *
 * "vejol qorban minjatjeja bamMELAJ timlaj, velo tashbit MELAJ
 *  BERIT Eloheja me'al minjateja - 'al kol qorbanja taqriv
 *  MELAJ." - Levitico 2:13
 * "Y toda ofrenda de cereal tuya, con SAL la salaras; y no haras
 *  faltar la SAL del PACTO de tu Dios sobre tu ofrenda - sobre
 *  toda ofrenda tuya ofreceras SAL."
 *
 * "lo nijcad 'otsmi mimmejja asher 'useiti vasseter ruqqamti
 *  beTAJTIYYOT arets." - Salmo 139:15
 * "No fue escondido de Ti mi cuerpo, lo que fue hecho en lo oculto,
 *  lo que fue bordado en las PROFUNDIDADES de la tierra."
 *
 * "ki mimmizraj-shemesh ve'ad mevo'o gadol shemi bagoyim,
 *  uvekhol maqom MUQTAR muggash lishmi uminja TEHORA." - Mal 1:11
 * "Porque desde el nacimiento del sol hasta su ocaso es grande
 *  mi nombre entre las naciones; y en todo lugar se OFRECE a mi
 *  nombre INCIENSO y ofrenda PURA."
 *
 * --- VOCABULARIO ---
 *
 *   roqeaj       (RQCh, H7543)              PERFUMADOR experto.
 *   ma'aseh roqeaj                             OBRA del perfumador -
 *                                               arte, no improvisacion.
 *   memullaj     (MMLCh, H4414 Pual)        SALADO - sellado con
 *                                               sal del pacto (Lv 2:13).
 *   melaj berit  (MLCh BRYT,
 *                  H4417+H1285)                 SAL del PACTO - senal
 *                                               de pacto inalterable.
 *   nataf        (NTP, H5198)                 GOTA, estacte - flujo
 *                                               natural, medido.
 *   shejelet     (ShChLT, H7827)              ONICHA - concha marina,
 *                                               lo profundo.
 *   jelbenah     (ChLBNH, H2464)             GALBANO - resina
 *                                               amarga sola, balance
 *                                               en la mezcla.
 *   levona zaja  (LBWNH ZKH,
 *                  H3828+H2134)                   INCIENSO PURO,
 *                                               blanco, sin mezcla.
 *   bad bevad    (BD BBD)                    PARTE POR PARTE -
 *                                               partes IGUALES.
 *   tahor        (THWR, H2889)                 PURO, sin contaminacion.
 *   tajtiyyot arets (TChTYWT ARTs)          LO PROFUNDO de la
 *                                               tierra - Sal 139:15.
 *   muqtar       (MQTR, H6999 Hof)            QUEMADO, ofrecido.
 *
 * --- LAS 4 REGLAS - BAD BEVAD (Exodo 30:34) ---
 *
 * Ninguna domina. Todas pesan igual. La primera que rechaza
 * decide. El orden fijo del pipeline es:
 * nataf -> shejelet -> jelbenah -> levona.
 *
 *   1. NATAF     - FLUJO. ?Fluye naturalmente? Tamano razonable
 *                   (no vacio, no desbordado). Ex 30:34 nataf +
 *                   Pr 25:16 "devash matsata ejol dayyejja" (lo
 *                   justo, no excesivo).
 *
 *   2. SHEJELET  - PROFUNDIDAD. Identificador de protocolo en rango
 *                   valido. Lo fuera del rango es esh zara (Lv 10:1).
 *                   Ex 30:34 shejelet + Sal 139:15 (lo profundo
 *                   inspeccionado) + Nm 1:2-3 (registro por id).
 *
 *   3. JELBENAH  - ANOMALIA. Sin patrones de flooding/replay
 *                   (max 75% de bytes identicos). El galbano se
 *                   tolera si beneficia al conjunto - la anomalia
 *                   excesiva no beneficia. Ex 30:34 jelbenah +
 *                   Ec 7:16 "al tehi tsaddiq harbeh" (balance).
 *
 *   4. LEVONA    - PUREZA. Integridad verificable por checksum
 *                   (2 bytes al final). Ex 30:34 levona zaja +
 *                   Dt 19:15 (2 testigos) + Mal 1:11 (minja tehora).
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   hashmal_firewall_formato_validar           pipeline completo,
 *                                                con logs.
 *   hashmal_firewall_formato_verificar_regla   una regla, sin logs
 *                                                (funcion pura).
 *
 *   + helpers estaticos (kelim):
 *   regla_nataf      flujo      (Ex 30:34 nataf)
 *   regla_shejelet   profundidad(Ex 30:34 shejelet)
 *   regla_jelbenah   anomalia   (Ex 30:34 jelbenah)
 *   regla_levona     pureza     (Ex 30:34 levona zaja)
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_firewall.h
 *   hashmal_firewall_internal.h   (no estrictamente necesario,
 *                                    pero se incluye por coherencia
 *                                    con los hermanos)
 *   esp_log.h (ORO EGIPTO)        logging para formato_validar
 *
 * Autor: Antonio Jose Marin Soto (Betsalel - Ex 31:2)
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "esp_log.h"

#include "hashmal_firewall.h"
#include "hashmal_firewall_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *FMT_TAG = "hashmal.firewall.formato";

#define FMT_LOG_I(fmt, ...)  ESP_LOGI(FMT_TAG, fmt, ##__VA_ARGS__)
#define FMT_LOG_W(fmt, ...)  ESP_LOGW(FMT_TAG, fmt, ##__VA_ARGS__)
#define FMT_LOG_E(fmt, ...)  ESP_LOGE(FMT_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * UMBRALES DE LAS 4 REGLAS - cada uno con dos testigos
 * ================================================================== */

/**
 * NATAF - tamano maximo razonable para un mensaje LoRa (SX1262
 * admite payloads hasta 255; reservamos margen para la pila).
 * Ex 30:34 nataf (gota: pequeno, medido) + Pr 25:16 "devash
 * matsata ejol dayyejja" (lo justo, no excesivo).
 */
#define NATAF_MAX_LEN                250u   /* Ex 30:34 + Pr 25:16 */

/**
 * SHEJELET - rango valido del byte identificador de protocolo.
 * Las 15 piezas del Tavnit ocupan [0x01..0x0E]; reservamos 0x0F
 * para "reservado futuro". 0x00 y > 0x0F son zar (esh zara).
 * Ex 30:34 shejelet + Nm 1:2-3 (registro por id).
 */
#define SHEJELET_PROTOCOLO_MIN       0x01u  /* Ex 30:34 + Nm 1:2-3 */
#define SHEJELET_PROTOCOLO_MAX       0x0Fu  /* 15 piezas del Tavnit */

/**
 * JELBENAH - maximo % de bytes identicos en el mensaje. Por encima,
 * indicio de flooding/replay. Ex 30:34 jelbenah (se tolera si
 * beneficia) + Ec 7:16 "al tehi tsaddiq harbeh" (ni exceso).
 */
#define JELBENAH_UMBRAL_REPETICION   75u    /* Ex 30:34 + Ec 7:16 */

/**
 * JELBENAH - longitud minima para aplicar la regla. Por debajo,
 * la medida no tiene sentido estadistico.
 */
#define JELBENAH_LEN_MIN              8u

/**
 * LEVONA - bytes de checksum al final del mensaje. 2 bytes para
 * 2 testigos (Dt 19:15 "al pi shnayim 'edim"). Ex 30:34 levona
 * zaja (pureza verificable) + Dt 19:15 (dos testigos).
 */
#define LEVONA_CHECKSUM_BYTES         2u    /* Ex 30:34 + Dt 19:15 */

/* ==================================================================
 * HELPERS LOCALES (kelim autocontenidos)
 * ================================================================== */

static inline bool regla_valida(hashmal_firewall_regla_t r)
{
    return (unsigned)r < HASHMAL_FIREWALL_REGLAS_TOTAL;
}

static const char *nombre_regla_fmt(hashmal_firewall_regla_t r)
{
    switch (r) {
    case HASHMAL_FIREWALL_REGLA_NATAF:
        return "NATAF (H5198 — flujo, Éx 30:34 + Job 36:27)";
    case HASHMAL_FIREWALL_REGLA_SHEJELET:
        return "SHEJELET (H7827 — profundidad, Éx 30:34 + Sal 139:15)";
    case HASHMAL_FIREWALL_REGLA_JELBENAH:
        return "JELBENAH (H2464 — anomalía, Éx 30:34 + Ro 8:28)";
    case HASHMAL_FIREWALL_REGLA_LEVONA:
        return "LEVONA (H3828+H2134 — pureza, Éx 30:34 + Mal 1:11)";
    default:
        return "<regla desconocida>";
    }
}

/* ==================================================================
 * REGLA 1 - NATAF (flujo natural, Ex 30:34 + Pr 25:16)
 *
 * La gota fluye naturalmente: ni falta ni exceso. El mensaje debe
 * tener longitud razonable: no vacio, no desbordado.
 *
 *   len == 0         -> ERR_FORMATO (no fluye: no hay qetoret)
 *   len > MAX_LEN    -> ERR_FORMATO (desborda: no es nataf, es rio)
 *   caso contrario   -> OK
 * ================================================================== */
static int regla_nataf(const uint8_t *datos, size_t len)
{
    (void)datos;  /* nataf solo examina el volumen, no el contenido */

    if (len == 0u)              return HASHMAL_FIREWALL_ERR_FORMATO;
    if (len > NATAF_MAX_LEN)    return HASHMAL_FIREWALL_ERR_FORMATO;
    return HASHMAL_FIREWALL_OK;
}

/* ==================================================================
 * REGLA 2 - SHEJELET (profundidad / protocolo, Ex 30:34 + Sal 139:15)
 *
 * La concha esconde en lo profundo. El primer byte del mensaje es
 * el identificador de protocolo (uno de los 15 del Tavnit). Un
 * identificador fuera del rango valido es esh zara (Lv 10:1) -
 * protocolo no prescrito, no reconocido.
 *
 *   len == 0                         -> ERR_FORMATO (sin cabecera)
 *   primer byte fuera de [MIN..MAX]  -> ERR_ZAR (esh zara)
 *   caso contrario                    -> OK
 * ================================================================== */
static int regla_shejelet(const uint8_t *datos, size_t len)
{
    if (datos == NULL || len == 0u) {
        return HASHMAL_FIREWALL_ERR_FORMATO;
    }

    const uint8_t protocolo = datos[0];
    if (protocolo < SHEJELET_PROTOCOLO_MIN ||
        protocolo > SHEJELET_PROTOCOLO_MAX) {
        /* Protocolo extrano - Lv 10:1 esh zara. */
        return HASHMAL_FIREWALL_ERR_ZAR;
    }
    return HASHMAL_FIREWALL_OK;
}

/* ==================================================================
 * REGLA 3 - JELBENAH (anomalia, Ex 30:34 + Ec 7:16)
 *
 * El galbano es aceptable en su proporcion; en exceso se vuelve
 * intolerable. Si un solo byte domina mas del UMBRAL % del mensaje,
 * el patron es flooding/replay - anomalia que no beneficia al
 * conjunto.
 *
 *   len < LEN_MIN                       -> OK (muestra insuficiente)
 *   max(count(byte)) * 100 > UMBRAL*len -> ERR_FORMATO
 *   caso contrario                       -> OK
 * ================================================================== */
static int regla_jelbenah(const uint8_t *datos, size_t len)
{
    if (datos == NULL) return HASHMAL_FIREWALL_ERR_FORMATO;

    /* Mensajes cortos no permiten medicion estadistica significativa. */
    if (len < JELBENAH_LEN_MIN) return HASHMAL_FIREWALL_OK;

    uint16_t frecuencia[256];
    for (uint16_t i = 0; i < 256u; i++) frecuencia[i] = 0u;

    for (size_t i = 0; i < len; i++) {
        frecuencia[datos[i]]++;
    }

    uint16_t maximo = 0u;
    for (uint16_t i = 0; i < 256u; i++) {
        if (frecuencia[i] > maximo) maximo = frecuencia[i];
    }

    /* Comparacion en enteros: maximo/len > UMBRAL/100
     * <=> 100 * maximo > UMBRAL * len. Usamos 32 bits para evitar
     * overflow con len hasta 250. */
    const uint32_t izq = 100u * (uint32_t)maximo;
    const uint32_t der = (uint32_t)JELBENAH_UMBRAL_REPETICION * (uint32_t)len;

    if (izq > der) {
        return HASHMAL_FIREWALL_ERR_FORMATO;
    }
    return HASHMAL_FIREWALL_OK;
}

/* ==================================================================
 * REGLA 4 - LEVONA (pureza / checksum, Ex 30:34 + Dt 19:15)
 *
 * El incienso puro no admite mancha. Verificamos integridad con
 * 2 bytes de checksum al final del mensaje (dos testigos,
 * Dt 19:15). Esquema simple y balanceado:
 *
 *   esperado[0] = XOR de los bytes en posiciones pares  de [0..len-3]
 *   esperado[1] = XOR de los bytes en posiciones impares de [0..len-3]
 *
 * Se compara contra datos[len-2..len-1]. Desacuerdo -> ERR_FORMATO.
 *
 *   len < CHECKSUM_BYTES+1   -> ERR_FORMATO (no hay que verificar)
 *   checksum no coincide     -> ERR_FORMATO
 *   caso contrario            -> OK
 *
 * Fase 2: reemplazar con HMAC-SHA256 del mensaje usando la root
 * key del hashmal_keygen (pieza 13). El esquema actual es
 * deterministico y suficiente para detectar corrupcion basica.
 * ================================================================== */
static int regla_levona(const uint8_t *datos, size_t len)
{
    if (datos == NULL) return HASHMAL_FIREWALL_ERR_FORMATO;

    /* Necesitamos al menos CHECKSUM_BYTES+1 para tener cuerpo + 2
     * bytes de testigo. */
    if (len <= (size_t)LEVONA_CHECKSUM_BYTES) {
        return HASHMAL_FIREWALL_ERR_FORMATO;
    }

    const size_t cuerpo_len = len - LEVONA_CHECKSUM_BYTES;

    uint8_t esperado_par = 0u;
    uint8_t esperado_imp = 0u;
    for (size_t i = 0; i < cuerpo_len; i++) {
        if ((i & 1u) == 0u) {
            esperado_par ^= datos[i];
        } else {
            esperado_imp ^= datos[i];
        }
    }

    const uint8_t recibido_par = datos[len - 2u];
    const uint8_t recibido_imp = datos[len - 1u];

    if (esperado_par != recibido_par || esperado_imp != recibido_imp) {
        return HASHMAL_FIREWALL_ERR_FORMATO;
    }
    return HASHMAL_FIREWALL_OK;
}

/* ==================================================================
 * hashmal_firewall_formato_verificar_regla - funcion PURA
 *
 * Ejecuta UNA regla sobre el mensaje. Sin logs, sin side effects,
 * sin estadisticas (esto ultimo lo hace el pipeline en
 * incienso_firewall.c). Es el "laboratorio" que devuelve el
 * resultado sin opinar.
 *
 *   regla fuera de enum    -> ERR_REGLA_INVALIDA
 *   datos NULL             -> ERR_NULL
 *   segun regla            -> OK / ERR_FORMATO / ERR_ZAR
 *
 * 2 testigos:
 *   Ex 30:34  - las 4 especias nombradas individualmente
 *   Ex 30:35  - ma'aseh roqeaj (obra exacta, cada ingrediente
 *                contribuye su propiedad especifica)
 * ================================================================== */
int hashmal_firewall_formato_verificar_regla(hashmal_firewall_regla_t regla,
                                              const uint8_t *datos,
                                              size_t len)
{
    if (!regla_valida(regla)) {
        return HASHMAL_FIREWALL_ERR_REGLA_INVALIDA;
    }
    if (datos == NULL && len > 0u) {
        return HASHMAL_FIREWALL_ERR_NULL;
    }

    switch (regla) {
    case HASHMAL_FIREWALL_REGLA_NATAF:    return regla_nataf(datos, len);
    case HASHMAL_FIREWALL_REGLA_SHEJELET: return regla_shejelet(datos, len);
    case HASHMAL_FIREWALL_REGLA_JELBENAH: return regla_jelbenah(datos, len);
    case HASHMAL_FIREWALL_REGLA_LEVONA:   return regla_levona(datos, len);
    default:
        return HASHMAL_FIREWALL_ERR_REGLA_INVALIDA;
    }
}

/* ==================================================================
 * hashmal_firewall_formato_validar - pipeline completo
 *
 * Aplica las 4 reglas en orden fijo (nataf -> shejelet -> jelbenah ->
 * levona). La primera que rechaza decide. Funcion publica CON logs
 * (informa la regla que fallo).
 *
 * NO actualiza estadisticas ni penalizaciones - es util para
 * validar un mensaje sin contexto de par. Para filtrado completo
 * con contabilidad, usar hashmal_firewall_filtrar_ascendente o
 * _descendente (incienso_firewall.c).
 *
 * 2+ testigos:
 *   Ex 30:34  - bad bevad (las 4 especias en partes iguales)
 *   Ex 30:35  - ma'aseh roqeaj (obra compuesta, orden preciso)
 *   Ex 30:36  - hadeq (molido fino, granularidad maxima)
 * ================================================================== */
int hashmal_firewall_formato_validar(const uint8_t *datos, size_t len)
{
    if (datos == NULL && len > 0u) {
        FMT_LOG_W("validar: datos NULL con len=%zu", len);
        return HASHMAL_FIREWALL_ERR_NULL;
    }

    /* Orden fijo - mismo que pipeline_filtrar (bad bevad simetrico). */
    static const hashmal_firewall_regla_t ORDEN[HASHMAL_FIREWALL_REGLAS_TOTAL] = {
        HASHMAL_FIREWALL_REGLA_NATAF,
        HASHMAL_FIREWALL_REGLA_SHEJELET,
        HASHMAL_FIREWALL_REGLA_JELBENAH,
        HASHMAL_FIREWALL_REGLA_LEVONA,
    };

    for (uint8_t i = 0; i < HASHMAL_FIREWALL_REGLAS_TOTAL; i++) {
        const int rc = hashmal_firewall_formato_verificar_regla(
            ORDEN[i], datos, len);
        if (rc == HASHMAL_FIREWALL_OK) continue;

        if (rc == HASHMAL_FIREWALL_ERR_ZAR) {
            FMT_LOG_E("validar: len=%zu rechazado por %s → ZAR "
                      "(ésh zará, Lv 10:1-2)",
                      len, nombre_regla_fmt(ORDEN[i]));
        } else {
            FMT_LOG_W("validar: len=%zu rechazado por %s → TAME "
                      "(rc=%d)",
                      len, nombre_regla_fmt(ORDEN[i]), rc);
        }
        return rc;
    }

    FMT_LOG_I("validar: len=%zu TAHOR — pasa las 4 reglas "
              "bad bevad (Éx 30:34) maʻaséh roqéaj (Éx 30:35)",
              len);
    return HASHMAL_FIREWALL_OK;
}

/* ==================================================================
 * P23 — hashmal_firewall_validar_levona - Levoná Zajá (Éx 30:34)
 *
 * Validador de pureza: estructura 3+1 del incienso sagrado.
 * Levoná (H3828) = incienso puro, blanco, sin mezcla.
 * Zajá (H2134) = limpio, puro, sin defecto.
 *
 * "sammím ulevoná zajá — PARTE POR PARTE" (Éx 30:34).
 * Las 3 primeras reglas (natáf, shejélet, jelbenáh) BUSCAN problemas.
 * Levoná CONFIRMA que no hay contaminación residual.
 *
 * Ref: Éx 30:34 levoná zajá (H3828+H2134 pureza confirmada)
 * ================================================================== */
int hashmal_firewall_validar_levona(const uint8_t *pkt, size_t len)
{
    if (pkt == NULL && len > 0u) {
        return HASHMAL_FIREWALL_ERR_NULL;
    }

    /* Fase 1: heurística simple. Levoná zajá = blanco, sin mezcla.
     * Sin bytes de relleno artificial (0x00 consecutivos) y sin
     * patrones de contaminación. Fase 2: HMAC-SHA256 de integridad. */

    if (len < 3u) {
        /* Muy corto para validación estadística */
        return HASHMAL_FIREWALL_OK;
    }

    /* Buscar "contaminación": secuencias anómalas que indiquen
     * mezcla no autorizada. Aquí buscamos patrones sospechosos:
     * demasiados ceros seguidos (relleno), o cambios abruptos. */
    uint8_t cero_consecutivos = 0u;
    for (size_t i = 0; i < len; i++) {
        if (pkt[i] == 0x00u) {
            cero_consecutivos++;
            /* Más de 25% de ceros = relleno artificial (no puro) */
            if (cero_consecutivos > (len / 4u)) {
                FMT_LOG_W("validar_levona: contaminación detectada "
                          "(%u ceros en %zu bytes)",
                          cero_consecutivos, len);
                return HASHMAL_FIREWALL_ERR_FORMATO;
            }
        } else {
            cero_consecutivos = 0u;
        }
    }

    /* Sin contaminación detectada — levoná zajá (puro). */
    return HASHMAL_FIREWALL_OK;
}

/* ==================================================================
 *
 *   "ve'asita otah QETORET ROQEAJ ma'aseh ROQEAJ MEMULLAJ tahor
 *    qodesh." - Exodo 30:35
 *
 *   "qaj leja sammim NATAF ushJELET vejelBENAH sammim uleVONA
 *    ZAKA - BAD BEVAD yihyeh." - Exodo 30:34
 *
 *   "vekhol qorban minjatjeja bamMELAJ timlaj." - Levitico 2:13
 *
 *   "uvekhol maqom MUQTAR muggash lishmi uminja TEHORA."
 *     - Malaquias 1:11
 *
 *   El roqeaj no improvisa: mezcla con exactitud, sala con pacto,
 *   muele hasta lo fino. Las 4 reglas son las 4 especias - ninguna
 *   sobra, ninguna domina. Nataf mide el flujo (lo natural no se
 *   fuerza); shejelet mira al fondo (el protocolo debe estar en la
 *   lista, lo extrano es esh zara); jelbenah tolera lo dificil sin
 *   permitir que domine (ni flooding ni replay); levona verifica la
 *   pureza con dos testigos al final (dos bytes, dos testigos,
 *   Dt 19:15). La primera que rechaza decide - no se gasta incienso
 *   en lo que ya fue declarado impuro.
 *
 *   El laboratorio no opina: informa. Por eso `verificar_regla` es
 *   funcion pura - sin logs, sin side effects, deterministica. Solo
 *   `validar` (wrapper orquestador) deja trazo en el log, nombrando
 *   la regla que fallo con su raiz hebrea.
 *
 *   === MA'ASEH ROQEAJ - incienso_formato.c COMPLETO ===
 *
 *   -- Los 5 archivos hermanos -----------------------------------
 *
 *     incienso_firewall.c        QETORET HADEQ
 *     incienso_formato.c         MA'ASEH ROQEAJ             <- ESTE
 *     incienso_circuitbreaker.c  BEIN HAMMETIM UVEIN HAJJAYYIM
 *     incienso_heartbeat.c       TAMID BABOQER UVEIN HA'ARBAYIM
 *     incienso_penalizacion.c    VENIKRAT ME'AMMAV
 *
 *   Archivo 2 de 5 del Incienso Sagrado. Siguiente:
 *     incienso_circuitbreaker.c - bein hammetim uvein hajjayyim
 *                                   (Nm 17:13)
 *
 *   PASO 10 de Exodo 40:27,34 - Pieza 14, Archivo 2 de 5.
 *
 * ================================================================== */
