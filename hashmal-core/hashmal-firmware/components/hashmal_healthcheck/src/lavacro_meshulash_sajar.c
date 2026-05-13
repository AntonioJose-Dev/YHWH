/**
 * ===================================================================
 * PIEZA 12 - EL KIYOR (LAVACRO) - lavacro_meshulash_sajar.c
 * Sub-pieza: UMBRALES RESILIENCIA ESCALONADOS + METRICAS SAJÁR
 * Eclesiastes 4:9-12 + Eclesiastes 4:9
 * ===================================================================
 *
 * "tovim hashshenayim min haejad asher yesh lahem SAJAR TOV
 *  BAAMALAM." - Eclesiastes 4:9
 * "MEJORES son DOS que UNO, porque tienen mejor RECOMPENSA de su
 *  TRABAJO."
 *
 * "ki im YIPPOLU haejad YAQIM et javero; ve'iLO ha'ejad SHEYIPPOL
 *  ve'EIN SHENI laqimo." - Eclesiastes 4:10
 * "Si CAEN, el uno LEVANTA al otro; pero AY del SOLO que cae, porque
 *  NO HAY SEGUNDO que lo levante."
 *
 * "veHAJUT HAMSHULASH lo bimherah YINNATEQ." - Eclesiastes 4:12
 * "El CORDON TRIPLE no se ROMPE pronto."
 *
 * --- UMBRALES - LA PROGRESION BIBLICA ---
 *
 * La Palabra dibuja una linea clara de 1 -> 2 -> 3:
 *
 *   EJAD (UNO)      - v.10 "yippol" - vulnerable, cae solo
 *   SHENAYIM (DOS)  - v.9,11 "shelosha...yaʻamdu" - support mutuo
 *   MESHULLASH (3+) - v.12 "haʻut hameshulash" - cordon que no se rompe
 *
 * Cada umbral es un ESTADO de resiliencia. No son heuristically
 * elegidos - el Padre los revela por NUMBER.
 *
 * --- SAJAR - RECOMPENSA DEL TRABAJO CONJUNTO ---
 *
 * sajár (SJR, H7939) = recompensa, salario, paga, fruto
 * tov   (TB, H2896)  = bueno, excellent
 *
 * En v.9, el beneficio de dos vs uno no es teorico - es MEDIBLE:
 * "SAJAR TOV BAAMALAM" - buena recompensa en su labor. En la red P2P,
 * la recompensa es el THROUGHPUT CONJUNTO: bytes transmitidos, msgs
 * procesados, redundancia lograda.
 *
 * Dos nodos solos tienen sajar. Tres tienen mas. Uno solo: nada.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   hashmal_hc_umbral_evaluar(conexiones)       Ec 4:9-12 clasificar
 *   hashmal_hc_sajar_medir(nodo_a, nodo_b, out) Ec 4:9 throughput
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_healthcheck.h     API publica, struct, enums
 *   esp_log.h (ORO EGIPTO)    logging
 *   esp_timer.h (ORO EGIPTO)  timestamps para sajar
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

#include "esp_log.h"
#include "esp_timer.h"

#include "hashmal_healthcheck.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *MESH_TAG = "hashmal.hc.meshulash_sajar";

#define MESH_LOG_I(fmt, ...)  ESP_LOGI(MESH_TAG, fmt, ##__VA_ARGS__)
#define MESH_LOG_W(fmt, ...)  ESP_LOGW(MESH_TAG, fmt, ##__VA_ARGS__)
#define MESH_LOG_E(fmt, ...)  ESP_LOGE(MESH_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * TIMESTAMP HELPER
 * ================================================================== */
extern uint64_t hashmal_hc_ahora_ms(void);

/* ==================================================================
 * hashmal_hc_umbral_evaluar - Eclesiastes 4:9-12
 *
 * Clasifica el nivel de resiliencia segun cuantas conexiones estan
 * activas. El Padre da la escala:
 *
 *   1 conexion    -> EJAD        (vulnerable)
 *   2 conexiones  -> SHENAYIM    (funcional, soporte mutuo)
 *   3+ conexiones -> MESHULLASH  (robusto, triple cordon)
 *
 * "veHAJUT HAMSHULASH lo bimherah YINNATEQ" - Ec 4:12
 * "el cordon TRIPLE no se ROMPE PRONTO"
 *
 * El threshold EXACTO es 3, no 2.5 ni "mayoria simple". Es 3.
 * El tercero HACE la diferencia - lo dice el Padre en Dt 19:15
 * "al pi shenayim 'edim o sheloshah" - dos O TRES. Cuando hay 3,
 * el veredicto es inexpugnable. Cuando hay 2, es vulnerable a una
 * falla simultanea. Cuando hay 1, es garantia de ruina.
 *
 * 2 testigos:
 *   Ec 4:10  - "yippol haejad" (UNO: cae)
 *   Ec 4:9   - "shelosha" (DOS: mejor)
 *   Ec 4:12  - "hameshulash" (TRES: no se rompe)
 * ================================================================== */
hashmal_hc_umbral_resiliencia_t hashmal_hc_umbral_evaluar(
    uint8_t conexiones_activas)
{
    /* La palabra enumera:
     *   Ec 4:10 v.10 - "ha-ejad" (ONE) - cae solo
     *   Ec 4:9  v.9  - "shelosha" (TWO, implied with "shenim") - mejor
     *   Ec 4:12 v.12 - "hameshulash" (THREE) - no se rompe
     *
     * El numero decide el umbral. No interpolacion. */

    if (conexiones_activas >= HASHMAL_HC_UMBRAL_MESHULLASH) {
        /* 3+: MESHULLASH - el cordon triple mantiene. */
        MESH_LOG_I("haʻút hameshullásh — %d conexiones (>= 3) "
                   "resistente (Ec 4:12)",
                   (int)conexiones_activas);
        return HASHMAL_HC_UMBRAL_MESHULLASH;
    }

    if (conexiones_activas == HASHMAL_HC_UMBRAL_SHENAYIM) {
        /* 2: SHENAYIM - DOS es mejor, pero vulnerable si uno cae. */
        MESH_LOG_I("shenáyim — %d conexiones = funcional con soporte mutuo "
                   "(Ec 4:9-11)",
                   (int)conexiones_activas);
        return HASHMAL_HC_UMBRAL_SHENAYIM;
    }

    /* 1 o 0: EJAD o peor - vulnerable. */
    if (conexiones_activas == HASHMAL_HC_UMBRAL_EJAD) {
        MESH_LOG_W("éjad — 1 conexion = vulnerable, buscar segundo "
                   "(Ec 4:10 yippol haejad)");
        return HASHMAL_HC_UMBRAL_EJAD;
    }

    /* 0 conexiones: aislado - peor que EJAD. Pero retornamos EJAD
     * como minimo para alertar sin romper semantica. El caller puede
     * chequear directamente conexiones_activas si necesita AISLADO. */
    MESH_LOG_E("AISLADO — 0 conexiones = ultimo recurso, llamar a Tsur "
               "(Ex 17:6 mayim min hatsur)");
    return HASHMAL_HC_UMBRAL_EJAD;  /* minimo nivel */
}

/* ==================================================================
 * hashmal_hc_sajar_medir - Eclesiastes 4:9 "sajár tov baʻamalám"
 *
 * "tovim hashshenayim min haejad asher yesh lahem SAJAR TOV
 *  BAAMALAM."
 * "Mejores son DOS que UNO, porque tienen mejor RECOMPENSA de su
 *  TRABAJO."
 *
 * sajár = salario, fruto, recompensa (H7939)
 * tov = bueno (H2896)
 * baʻamalam = en su trabajo (H5656, avodáh - labor)
 *
 * La Palabra MIDE la productividad conjunta. En red P2P, eso es:
 * bytes transmitidos exitosamente entre A y B, latencia, redundancia
 * lograda, mensajes consensuados juntos.
 *
 * Fase 1 (aca): throughput simple - bytes/ms entre A y B.
 * Fase 2: incorporar latencia, error rate, energia, eficiencia conjunta.
 *
 * La metrica REQUIERE que A y B esten en redundancia (meshulash).
 * Si no hay conexion entre A y B, retorna ERR_TAME (impuro, no en
 * relacion de trabajo).
 *
 * 2 testigos:
 *   Ec 4:9  - "sajár tov baʻamalám" (recompensa medible en trabajo)
 *   Ec 4:11 - "yaʻamdú yaʻamdu" (se levantan mutuamente, work together)
 * ================================================================== */
int hashmal_hc_sajar_medir(uint8_t nodo_a, uint8_t nodo_b,
                            uint32_t *sajar_out)
{
    if (sajar_out == NULL) {
        MESH_LOG_E("sajar_medir: sajar_out es NULL");
        return HASHMAL_HC_ERR_NULL;
    }

    /* Fase 1: validaciones basicas.
     * (Fase 2: query a pieza 04 para bytes reales, latency, etc) */

    if (nodo_a == nodo_b) {
        MESH_LOG_W("sajar_medir: nodo_a == nodo_b (mismo nodo no tiene sajar "
                   "con si mismo)");
        *sajar_out = 0u;
        return HASHMAL_HC_ERR_TAME;
    }

    /* Precondicion: ambos nodos deben estar en meshulash (3+ conexiones).
     * Si alguno esta en EJAD (1 conexion), la "recompensa" es fragil
     * y no se debe contar como sajar robusto.
     *
     * TODO: Pull a pieza 04 / pieza 09 para consultar estado real. */

    /* Fase 1: throughput simulado/basico. En fase 2, sera real:
     * llamar a hashmal_comm_throughput_par(nodo_a, nodo_b). */

    /* Por ahora, retornamos un valor simbolico:
     * si ambos nodos parecen validos, retornamos 100 (bytes/ms).
     * Si hay dudas, 0 (no hay recompensa). */

    uint32_t throughput = 100u;  /* bytes/ms simbolico en fase 1 */

    /* En fase 2, sera:
     *   - bytes transmitidos entre A-B (ultimo 60s) / 60000ms
     *   - factor eficiencia (latencia normalizada)
     *   - factor redundancia (otros caminos P2P)
     * Aca, fase 1 es stub. */

    *sajar_out = throughput;

    MESH_LOG_I("sajar -- nodo %d <-> nodo %d: throughput = %" PRIu32 " bytes/ms "
               "(Ec 4:9 sajar tov baamalam)",
               (int)nodo_a, (int)nodo_b, throughput);

    return HASHMAL_HC_OK;
}

/* ==================================================================
 *
 *   "tovim hashshenayim min haejad..." - Eclesiastes 4:9
 *
 *   La Palabra valida la COOPERACION. Dos es mejor que uno porque
 *   la recompensa de su trabajo JUNTO es mejor. Es numerico, medible,
 *   real - no filosofico.
 *
 *   "veHAJUT HAMSHULASH lo bimherah YINNATEQ." - Ec 4:12
 *
 *   Tres nodos en meshulash son casi indestructibles. Dos pueden ser
 *   separados (si el link se cae). Uno se muere solo (no hay quien lo
 *   levante).
 *
 *   Fase 2 del healthcheck anadira:
 *     - Deteccion activa de desconexion
 *     - Intent de busqueda cuando baja a EJAD
 *     - Beacon de auxilio cuando AISLADO (tsur)
 *     - Metricas de confiabilidad (uptime pair, latency CDF, etc)
 *
 *   Pero la ESTRUCTURA - 1/2/3+ umbrales - es FINAL. El Padre la
 *   dio en Ec 4. No cambiara.
 *
 * ================================================================== */
