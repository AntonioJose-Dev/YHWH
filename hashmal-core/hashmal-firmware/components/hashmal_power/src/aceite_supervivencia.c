/**
 * ===================================================================
 * PIEZA 08 - EL ACEITE PERPETUO - aceite_supervivencia.c
 * Sub-pieza: SHENEI ZEITIM - los dos olivos (redundancia energetica)
 * Zacarias 4:2-3,6,11-12 + Mateo 25:1-13
 * ===================================================================
 *
 * "menorat ZAHAV kulla vegullata 'al roshah veshiv'a neroteha
 *  'aleha... ushenei ZEITIM 'aleha EJAD MIN YAMIN hammazhera
 *  ve'EJAD 'al SMOLAH" - Zacarias 4:2-3
 * "Una menora de ORO toda ella, con su DEPOSITO encima, y siete
 *  lamparas sobre ella... y DOS OLIVOS sobre ella, UNO a la DERECHA
 *  del deposito y UNO a la IZQUIERDA."
 *
 * "shenei shibbolei hazzahav HAMMERIQIM me'alehem et hazzahav"
 *   - Zacarias 4:12
 * "Dos ESPIGAS de oro que VIERTEN de si mismas el oro (aceite dorado)."
 *
 * "lo bejayil velo bejoaj ki im beRuji amar YHWH tseva'ot"
 *   - Zacarias 4:6
 * "No con fuerza ni con poder, sino con MI ESPIRITU, dice YHWH
 *  de los ejercitos."
 *
 * "hai de morai elabon elaion en tois angeiois meta ton lampadon
 *  heauton" - Mateo 25:4
 * "Las prudentes tomaron ACEITE en sus VASIJAS junto con sus
 *  lamparas." - cada virgen su propia reserva; no se comparte.
 *
 * --- VOCABULARIO ---
 *
 *   zeitim       (ZYTYM, H2132 pl)      OLIVOS - las dos fuentes.
 *   yamin        (YMYN, H3225)           DERECHA - olivo solar.
 *   smol         (ShMAL, H8040)          IZQUIERDA - olivo bateria.
 *   gulla        (GLH, H1543)         DEPOSITO, cuenco - reserva
 *                                           combinada encima.
 *   shibbolei hazzahav                     ESPIGAS DE ORO - los canales
 *                                           (ADC en fase 2).
 *   hammeriqim   (H7324 Hifil pl)          QUE VIERTEN - flujo continuo.
 *   jayil        (ChYL, H2428)            EJERCITO, poder militar - NO.
 *   koaj         (KCh, H3581)            FUERZA - NO por potencia bruta.
 *   Ruji         (RWChY, H7307)           MI ESPIRITU - diseno del Padre.
 *   elaion       (elaion, G1637)           aceite (LXX traduce shemen).
 *   angeia       (aggeia, G30 pl)          vasijas - reserva propia.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   hashmal_power_leer_olivo       Zac 4:12 - vierte de un olivo.
 *   hashmal_power_nivel_actual     Zac 4:2 - gulla determina nivel.
 *   hashmal_power_energia_estado   Zac 4:2 - menora entera de un vistazo.
 *
 *   + helper estatico:
 *   nombre_olivo                   nombre legible para logs.
 *
 * --- SEPARACION CON aceite_tamid.c ---
 *
 * aceite_tamid.c REACCIONA al nivel - ajusta el ciclo, pero no
 * toca hardware. Este archivo (aceite_supervivencia.c) LEE los
 * olivos - actualiza lecturas y deriva nivel. Asi se respeta la
 * division de responsabilidades: tamid es RELOJ; supervivencia
 * es OJO.
 *
 * --- FASE 1 vs FASE 2 ---
 *
 * Fase 1 (actual): sin hardware. Los olivos se SIMULAN con valores
 * seguros por defecto - el esqueleto esta completo y listo para
 * recibir la lectura ADC del ESP32-S3 cuando el driver exista. El
 * flujo, las transiciones, los umbrales, los logs y la API publica
 * son los definitivos - solo cambia la fuente del dato.
 *
 * Fase 2 (futuro): driver/adc_oneshot leera voltaje en GPIO
 * dedicados para solar y bateria. La zona marcada [ FASE 2 ]
 * contiene la simulacion que sera reemplazada.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_power.h          API publica (lectura_t, estado_energia_t)
 *   hashmal_power_internal.h singleton + ahora_ms
 *   esp_log.h   (ORO EGIPTO) logging
 *
 *   NO incluir driver/ aqui todavia. La "conexion a los tubos de oro"
 *   (Zac 4:12) se hara en fase 2.
 *
 * Autor: Antonio Jose Marin Soto (Betsalel - Ex 31:2)
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"   /* Zac 4:12 shibbolei hazzahav — tubos de oro al hardware */

#include "hashmal_power.h"
#include "hashmal_power_internal.h"

/* ==================================================================
 * TUBOS DE ORO — Zacarias 4:12 shibbolei hazzahav hammeriqim
 *
 * "ushenei shibbolei hazzahav HAMMERIQIM me'alehem et hazzahav"
 * "Dos espigas de oro que VIERTEN de si mismas el aceite dorado."
 *
 * Hardware: divisor de tension 1:2 sobre GPIO4 (ADC1_CH3).
 *   Vbat_real = Vadc * 2  (dos resistencias iguales)
 *   LiPo 0%%   = 3.0 V / 2 = 1.50 V -> raw ~= 1980
 *   LiPo 100%% = 4.2 V / 2 = 2.10 V -> raw ~= 2770
 *
 * Ex 27:20 KATIT (H3795): "prensado" = medido con precision maxima.
 * No se estima, no se redondea — se lee el voltaje real.
 *
 * 2 testigos:
 *   Zac 4:3   — ejad min yamin ve'ejad smolah (dos olivos = dos fuentes)
 *   Zac 4:12  — shibbolei hazzahav hammeriqim (tubos que VIERTEN)
 * ================================================================== */
#define ACEITE_ADC_UNIT      ADC_UNIT_1
#define ACEITE_ADC_CHANNEL   ADC_CHANNEL_3   /* GPIO4 — T-Deck Pro */
#define ACEITE_ADC_ATTEN     ADC_ATTEN_DB_12 /* rango 0..3.1 V */
#define ACEITE_BAT_RAW_MIN   1980u           /* 0%%  — 3.0V / 2 = 1.50V */
#define ACEITE_BAT_RAW_MAX   2770u           /* 100%% — 4.2V / 2 = 2.10V */
#define ACEITE_ADC_DIVISOR   2u              /* divisor de tension x2 */
#define ACEITE_ADC_VREF_MV   3100u           /* Vref ADC_ATTEN_DB_12 en mV */
#define ACEITE_ADC_BITS      4095u           /* 12-bit: 2^12 - 1 */

/* Handle del ADC — un solo "tubo" por pieza. Ex 27:20 "SHEMEN" singular. */
static adc_oneshot_unit_handle_t s_bat_adc = NULL;

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *SUPER_TAG = "hashmal.power.supervivencia";

#define SUPER_LOG_I(fmt, ...)  ESP_LOGI(SUPER_TAG, fmt, ##__VA_ARGS__)
#define SUPER_LOG_W(fmt, ...)  ESP_LOGW(SUPER_TAG, fmt, ##__VA_ARGS__)
#define SUPER_LOG_E(fmt, ...)  ESP_LOGE(SUPER_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * FASE 1 - constantes de simulacion
 *
 * Zac 4:6 "lo bejayil velo bejoaj": no forzamos valores extremos.
 * Simulamos la mitad - un "dai" prudente - hasta que el driver
 * real del ESP32-S3 entregue el voltaje verdadero.
 * ================================================================== */
#define SUPER_FASE1_PCT_ACTIVO     50u   /* valor seguro medio */
#define SUPER_FASE1_MV_POR_PCT     42u   /* 50% -> ~2100 mV (LiPo) */

/* ==================================================================
 * HELPERS ESTATICOS
 * ================================================================== */

static const char *nombre_olivo(hashmal_power_olivo_t olivo)
{
    switch (olivo) {
    case HASHMAL_POWER_OLIVO_YAMIN: return "YAMÍN (derecho — solar)";
    case HASHMAL_POWER_OLIVO_SMOL:  return "SMOL (izquierdo — batería)";
    default:                        return "<olivo desconocido>";
    }
}

static const char *nombre_nivel_sup(hashmal_power_nivel_t nivel)
{
    switch (nivel) {
    case HASHMAL_POWER_NIVEL_SHEFA:  return "SHÉFA (abundancia)";
    case HASHMAL_POWER_NIVEL_DAI:    return "DAI (suficiente)";
    case HASHMAL_POWER_NIVEL_MATSOQ: return "MATSOQ (estrechez)";
    case HASHMAL_POWER_NIVEL_JOSHEJ: return "JÓSHEJ (oscuridad)";
    default:                         return "<nivel desconocido>";
    }
}

/* ==================================================================
 * aceite_adc_init / aceite_adc_deinit
 *
 * "veyiqju eleja shemen zayit zaj KATIT" - Exodo 27:20
 * El aceite se TRAE ("veyiqju"): aqui se establece el canal fisico.
 *
 * Llamado desde aceite_energia.c / hashmal_power_init y _deinit.
 * Solo olivo SMOL (bateria) tiene ADC en Fase 2.
 * Olivo YAMIN (solar) — sensor futuro; de momento sin hardware.
 *
 * 2 testigos:
 *   Ex 27:20  — katit (prensado = precision real, no estimado)
 *   Zac 4:12  — hammeriqim (los tubos que VIERTEN — flujo activo)
 * ================================================================== */
void aceite_adc_init(void)
{
    adc_oneshot_unit_init_cfg_t cfg = {
        .unit_id  = ACEITE_ADC_UNIT,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    esp_err_t rc = adc_oneshot_new_unit(&cfg, &s_bat_adc);
    if (rc != ESP_OK) {
        SUPER_LOG_W("ADC init fallo (rc=%d) — simulacion activa "
                    "(Zac 4:6 beRuji fallback)", (int)rc);
        s_bat_adc = NULL;
        return;
    }
    adc_oneshot_chan_cfg_t ch = {
        .bitwidth = ADC_BITWIDTH_12,
        .atten    = ACEITE_ADC_ATTEN,
    };
    adc_oneshot_config_channel(s_bat_adc, ACEITE_ADC_CHANNEL, &ch);
    SUPER_LOG_I("shibbolei hazzahav (Zac 4:12) — ADC GPIO4 katit listo");
}

void aceite_adc_deinit(void)
{
    if (s_bat_adc) {
        adc_oneshot_del_unit(s_bat_adc);
        s_bat_adc = NULL;
        SUPER_LOG_I("ADC liberado (Nm 4:9 beged tejelet — cubierto)");
    }
}

/* ==================================================================
 * hashmal_power_leer_olivo - Zacarias 4:12
 *
 * "shenei shibbolei hazzahav HAMMERIQIM" - las dos espigas de oro
 * que VIERTEN. Cada llamada es una espiga vertiendo hacia la gulla.
 *
 * 2 testigos:
 *   Zac 4:3   - ejad min yamin ve'ejad smolah (dos olivos)
 *   Zac 4:12  - shibbolei hazzahav hammeriqim (tubos que vierten)
 * ================================================================== */
int hashmal_power_leer_olivo(hashmal_power_olivo_t olivo,
                             hashmal_power_lectura_t *lectura)
{
    if (lectura == NULL) {
        SUPER_LOG_E("leer_olivo: lectura NULL");
        return HASHMAL_POWER_ERR_NULL;
    }

    hashmal_power_estado_energia_t *a = hashmal_power_aceite_mut();
    if (a == NULL) {
        SUPER_LOG_E("leer_olivo: aceite no encendido (init primero)");
        return HASHMAL_POWER_ERR_INIT;
    }

    /* Seleccionar la espiga correspondiente. "ejad min yamin
     * ve'ejad 'al smolah" - Zac 4:3. Solo existen dos. */
    hashmal_power_lectura_t *destino;
    switch (olivo) {
    case HASHMAL_POWER_OLIVO_YAMIN:
        destino = &a->olivo_yamin;
        break;
    case HASHMAL_POWER_OLIVO_SMOL:
        destino = &a->olivo_smol;
        break;
    default:
        SUPER_LOG_E("leer_olivo: olivo fuera de [YAMIN, SMOL]");
        return HASHMAL_POWER_ERR_NULL;
    }

    /* -- FASE 2 — "shibbolei hazzahav hammeriqim" (Zac 4:12) -------
     *
     * Los tubos de oro VIERTEN aceite real desde el hardware.
     * Olivo SMOL (bateria) -> ADC GPIO4, divisor 1:2.
     * Olivo YAMIN (solar)  -> sin sensor Fase 2, fallback seguro.
     * Si ADC no disponible -> fallback Fase 1 (Zac 4:6 beRuji).
     *
     * Ex 27:20 KATIT — lectura de precision, no estimacion.
     * ----------------------------------------------------------- */
    destino->olivo = olivo;
    if (destino->activo) {
        if (s_bat_adc != NULL && olivo == HASHMAL_POWER_OLIVO_SMOL) {
            /* SMOL (bateria) — leer ADC real Zac 4:12 */
            int raw = 0;
            if (adc_oneshot_read(s_bat_adc, ACEITE_ADC_CHANNEL, &raw) == ESP_OK) {
                /* Porcentaje: interpolacion lineal entre RAW_MIN y RAW_MAX.
                 * Ex 27:20 katit — cada unidad raw importa. */
                uint8_t pct;
                if (raw <= (int)ACEITE_BAT_RAW_MIN) {
                    pct = 0u;
                } else if (raw >= (int)ACEITE_BAT_RAW_MAX) {
                    pct = 100u;
                } else {
                    pct = (uint8_t)(((uint32_t)(raw - (int)ACEITE_BAT_RAW_MIN) * 100u)
                                    / (ACEITE_BAT_RAW_MAX - ACEITE_BAT_RAW_MIN));
                }
                /* Voltaje real: raw -> mV (12-bit, 0..VREF) * divisor.
                 * Vbat = Vadc * ACEITE_ADC_DIVISOR (dos resistencias iguales).
                 * Clamped al rango fisico LiPo: 2800..4200 mV.
                 * Ex 27:20 ZAJ (H2134): puro — sin valores imposibles. */
                uint32_t vadc_mv = ((uint32_t)raw * ACEITE_ADC_VREF_MV) / ACEITE_ADC_BITS;
                uint32_t vbat_mv = vadc_mv * ACEITE_ADC_DIVISOR;
                if (vbat_mv > 4200u) vbat_mv = 4200u;           /* LiPo maximo fisico */
                if (pct > 0u && vbat_mv < 2800u) vbat_mv = 2800u; /* LiPo minimo activo */
                destino->porcentaje = pct;
                destino->voltaje_mv = (uint16_t)vbat_mv;
            } else {
                /* Lectura ADC fallo — Zac 4:6 beRuji fallback */
                SUPER_LOG_W("ADC read fallo — fallback simulacion (beRuji)");
                destino->porcentaje = (uint8_t)SUPER_FASE1_PCT_ACTIVO;
                destino->voltaje_mv = (uint16_t)(SUPER_FASE1_PCT_ACTIVO
                                                  * SUPER_FASE1_MV_POR_PCT);
            }
        } else {
            /* YAMIN (solar sin sensor Fase 2) o ADC no disponible.
             * Zac 4:6 "lo bejayil velo bejoaj ki im beRuji" — no forzamos. */
            destino->porcentaje = (uint8_t)SUPER_FASE1_PCT_ACTIVO;
            destino->voltaje_mv = (uint16_t)(SUPER_FASE1_PCT_ACTIVO
                                              * SUPER_FASE1_MV_POR_PCT);
        }
    } else {
        /* Gn 1:2 joshej — olivo inactivo, sin aceite. */
        destino->porcentaje = 0u;
        destino->voltaje_mv = 0u;
    }
    /* -- FIN FASE 2 ------------------------------------------- */

    /* Actualizar timestamp del snapshot - la espiga acaba de verter. */
    a->timestamp_ms = hashmal_power_ahora_ms();

    /* Entregar copia al llamante (snapshot - Mt 25:4 angeia propia). */
    memcpy(lectura, destino, sizeof(*lectura));

    SUPER_LOG_I("olivo %s vertió: %u%% (%u mV) %s",
                nombre_olivo(olivo),
                (unsigned)destino->porcentaje,
                (unsigned)destino->voltaje_mv,
                destino->activo ? "activo" : "inactivo");

    return HASHMAL_POWER_OK;
}

/* ==================================================================
 * hashmal_power_nivel_actual - Zacarias 4:2
 *
 * "vegullata 'al roshah" - la gulla esta ENCIMA de la menora. Desde
 * arriba ve el flujo de los dos olivos combinados. Aqui se agrega
 * la energia y se mapea al nivel operativo (SHEFA / DAI / MATSOQ /
 * JOSHEJ) usando los umbrales del header.
 *
 * Combinacion (los dos olivos):
 *   ambos activos  -> MAX(yamin, smol)   (el mas fuerte sostiene)
 *   solo uno       -> ese.porcentaje     (Mt 25:4 cada virgen su angeion)
 *   ninguno        -> 0                  (Gn 1:2 joshej 'al penei tehom)
 *
 * 2 testigos:
 *   Zac 4:2   - gullata 'al roshah (deposito central)
 *   Ex 27:20  - zaj / katit (umbrales de pureza)
 * ================================================================== */
int hashmal_power_nivel_actual(hashmal_power_nivel_t *nivel)
{
    if (nivel == NULL) {
        SUPER_LOG_E("nivel_actual: nivel NULL");
        return HASHMAL_POWER_ERR_NULL;
    }

    hashmal_power_estado_energia_t *a = hashmal_power_aceite_mut();
    if (a == NULL) {
        SUPER_LOG_E("nivel_actual: aceite no encendido (init primero)");
        return HASHMAL_POWER_ERR_INIT;
    }

    /* -- Combinar los dos olivos en la gulla --------------------- */
    uint8_t combinado;
    if (a->olivo_yamin.activo && a->olivo_smol.activo) {
        /* Zac 4:3 - dos olivos verten; el deposito refleja al mas
         * fuerte. La redundancia protege: si uno mengua, el otro
         * sostiene. Esto NO es suma (no sobrecontar) - es MAX. */
        combinado = (a->olivo_yamin.porcentaje > a->olivo_smol.porcentaje)
                    ? a->olivo_yamin.porcentaje
                    : a->olivo_smol.porcentaje;
    } else if (a->olivo_yamin.activo) {
        combinado = a->olivo_yamin.porcentaje;
    } else if (a->olivo_smol.activo) {
        combinado = a->olivo_smol.porcentaje;
    } else {
        /* Gn 1:2 - joshej 'al penei tehom. */
        combinado = 0u;
    }

    /* -- Mapeo a nivel - Ex 27:20 umbrales de pureza ------------- */
    hashmal_power_nivel_t nuevo;
    if (combinado >= HASHMAL_POWER_UMBRAL_ZAJ) {
        nuevo = HASHMAL_POWER_NIVEL_SHEFA;
    } else if (combinado >= HASHMAL_POWER_UMBRAL_KATIT) {
        nuevo = HASHMAL_POWER_NIVEL_DAI;
    } else if (combinado >= HASHMAL_POWER_UMBRAL_JOSHEJ) {
        nuevo = HASHMAL_POWER_NIVEL_MATSOQ;
    } else {
        nuevo = HASHMAL_POWER_NIVEL_JOSHEJ;
    }

    /* -- Persistir y reportar ------------------------------------ */
    const hashmal_power_nivel_t anterior = a->nivel;
    a->nivel                = nuevo;
    a->porcentaje_combinado = combinado;
    a->timestamp_ms         = hashmal_power_ahora_ms();

    *nivel = nuevo;

    SUPER_LOG_I("gullá (Zac 4:2): %u%% → nivel %s",
                (unsigned)combinado, nombre_nivel_sup(nuevo));

    if (nuevo != anterior) {
        /* Zac 4:6 - el cambio no se impone, se anuncia. */
        SUPER_LOG_W("nivel %s → %s — %u%% combinado",
                    nombre_nivel_sup(anterior),
                    nombre_nivel_sup(nuevo),
                    (unsigned)combinado);
    }

    return HASHMAL_POWER_OK;
}

/* ==================================================================
 * hashmal_power_energia_estado - Zacarias 4:2 (la menora entera)
 *
 * "menorat ZAHAV kulla vegullata 'al roshah veshiv'a neroteha
 *  'aleha" - Zacarias 4:2
 * La vision del profeta es TODO el conjunto de un solo vistazo:
 * deposito, lamparas, olivos. Esta funcion es ese "ojo" - snapshot
 * completo del subsistema de energia.
 *
 * Hace los pasos del profeta:
 *   1. Lee ambas espigas (Zac 4:12 hammeriqim).
 *   2. Deriva el nivel de la gulla (Zac 4:2).
 *   3. Copia el singleton al llamante (Mt 25:4 - su propia vasija).
 *
 * 2 testigos:
 *   Zac 4:2   - menorat zahav kulla (toda la menora en una vision)
 *   Mt 25:4   - elaion en tois angeiois (aceite en vasija propia)
 * ================================================================== */
int hashmal_power_energia_estado(hashmal_power_estado_energia_t *estado)
{
    if (estado == NULL) {
        SUPER_LOG_E("energia_estado: estado NULL");
        return HASHMAL_POWER_ERR_NULL;
    }

    hashmal_power_estado_energia_t *a = hashmal_power_aceite_mut();
    if (a == NULL) {
        SUPER_LOG_E("energia_estado: aceite no encendido (init primero)");
        return HASHMAL_POWER_ERR_INIT;
    }

    /* 1. Leer ambos olivos - Zac 4:12 hammeriqim. Ignoramos el
     *    contenido local; lo que queda es el singleton actualizado. */
    hashmal_power_lectura_t tmp;
    int rc;

    rc = hashmal_power_leer_olivo(HASHMAL_POWER_OLIVO_YAMIN, &tmp);
    if (rc != HASHMAL_POWER_OK) {
        SUPER_LOG_E("energia_estado: leer olivo YAMÍN falló (rc=%d)", rc);
        return rc;
    }

    rc = hashmal_power_leer_olivo(HASHMAL_POWER_OLIVO_SMOL, &tmp);
    if (rc != HASHMAL_POWER_OK) {
        SUPER_LOG_E("energia_estado: leer olivo SMOL falló (rc=%d)", rc);
        return rc;
    }

    /* 2. Derivar el nivel - la gulla. */
    hashmal_power_nivel_t nivel_tmp;
    rc = hashmal_power_nivel_actual(&nivel_tmp);
    if (rc != HASHMAL_POWER_OK) {
        SUPER_LOG_E("energia_estado: nivel_actual falló (rc=%d)", rc);
        return rc;
    }

    /* 3. Snapshot atomico al llamante (Mt 25:4 - su propia angeia). */
    memcpy(estado, a, sizeof(*estado));

    SUPER_LOG_I("snapshot menorá: nivel=%s, gullá=%u%%, yamín=%u%%%s, "
                "smol=%u%%%s, estado=%d",
                nombre_nivel_sup(estado->nivel),
                (unsigned)estado->porcentaje_combinado,
                (unsigned)estado->olivo_yamin.porcentaje,
                estado->olivo_yamin.activo ? "(+)" : "(-)",
                (unsigned)estado->olivo_smol.porcentaje,
                estado->olivo_smol.activo ? "(+)" : "(-)",
                (int)estado->estado);

    return HASHMAL_POWER_OK;
}

/* ==================================================================
 *
 *   "shenei ZEITIM 'aleha EJAD MIN YAMIN... ve'EJAD 'al SMOLAH."
 *     - Zacarias 4:3
 *
 *   "lo bejayil velo bejoaj ki im beRUJI amar YHWH tseva'ot."
 *     - Zacarias 4:6
 *   "No con fuerza ni con poder, sino con Mi Espiritu, dice YHWH."
 *
 *   Dos olivos, no uno. La redundancia no es desconfianza - es
 *   sabiduria. El Padre diseno DOS fuentes porque conoce la noche:
 *   cuando el sol duerme (yamin mengua), la bateria sostiene (smol
 *   vierte); cuando la bateria se agota, el sol regresa. La gulla
 *   - el deposito sobre la menora - no mide el esfuerzo, mide lo
 *   que basta. Por eso el nivel se determina por el MAXIMO disponible,
 *   no por la suma: donde hay uno, hay menora encendida.
 *
 *   Y la clave final: "lo bejayil velo bejoaj" - esto no se sostiene
 *   por potencia bruta. Se sostiene porque fue disenado asi por el
 *   Espiritu del Padre.
 *
 *   === SHENEI ZEITIM - aceite_supervivencia.c COMPLETO ===
 *
 *   Archivo 3 de 4 del Aceite Perpetuo. Siguiente:
 *     aceite_mantenimiento.c - Aaron DISPONE (Lv 24:1-4 ya'aroj)
 *
 *   PASO 8 de Exodo 40:9-11 - Archivo 3 de 4 del Aceite Perpetuo.
 *
 * ================================================================== */
