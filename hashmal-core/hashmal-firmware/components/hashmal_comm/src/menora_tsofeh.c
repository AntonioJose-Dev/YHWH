/*
 * ===================================================================
 * PIEZA 04 - LA MENORA - menora_tsofeh.c
 * Sub-pieza: LOCKDOWN DE TX + SHOFAR DEDICADO (Ez 33)
 * Ezequiel 33:2-6 + Numeros 10:8-9 + Ex 25:31 geza' tamid
 * ===================================================================
 *
 * "ben adam, dabber el benei ammekha veamarta aleihem: erets ki
 *  aviy aleiha jarev ulqejhu am haarets ish ejad miqtseihem
 *  venatnu oto lahem leTSOFEH. vera'a et hajerev ba'a 'al haarets
 *  veTAQA' bashSHOFAR vehizhir et ha'am." - Ezequiel 33:2-3
 * "Hijo de hombre, di a los hijos de tu pueblo: cuando Yo traiga
 *  espada sobre una tierra, y el pueblo tome a un hombre de entre
 *  ellos y lo ponga por ATALAYA, y el vea venir la espada sobre la
 *  tierra y TOQUE EL SHOFAR y avise al pueblo..."
 *
 * "vehatsofeh ki yir'a et hajerev ba'a veLO taqa' bashshofar, veha'am
 *  lo nizhar... damo miyyado adrosh" - Ezequiel 33:6
 * "Pero si el atalaya ve la espada venir y NO TOCA el shofar, y el
 *  pueblo no es avisado... su sangre demandare de SU mano."
 *
 * "utqa'tem bajatsotserot... venizkartem lifnei YHWH eloheikem"
 *   - Numeros 10:9
 * "Tocareis las trompetas... y sereis recordados delante de YHWH."
 *
 * --- INVARIANTE INV-TSOFEH-01 ---
 *
 * Ez 33:6 es LEY: ver amenaza y no avisar es sangre sobre la mano
 * del atalaya. En codigo: no existe un camino donde se detecte
 * amenaza SIN que el shofar haya sonado. El rol (Pieza 09)
 * implementa la atomicidad detect+shofar; este archivo provee el
 * mecanismo TX irrompible que lo hace posible bajo lockdown.
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   hashmal_comm_tsofeh_lockdown_set    activa/desactiva gate global
 *   hashmal_comm_tsofeh_lockdown_get    lectura compuesta (gated?)
 *   hashmal_comm_shofar_tocar           broadcast que bypassea el gate
 *
 * El gate: menora_routing.c lo consulta al inicio de enviar() y
 * rechaza con ERR_TSOFEH si esta activo. Shofar coloca un ticket
 * interno (g_shofar_burst) que vence el gate solo durante su
 * broadcast - atomico y sin fugas.
 *
 * --- EL GEZA' NO SE APAGA ---
 *
 * El lockdown prohibe TX ordinario, NO desmonta los nerot. LoRa
 * (geza', Ex 25:31) sigue operativa para el shofar y para RX. El
 * atalaya ESCUCHA mas, HABLA menos - pero cuando habla, habla a
 * todos con la trompeta (Nm 10:9).
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_comm.h            API publica
 *   hashmal_comm_internal.h   (no uso; flags son locales al archivo)
 *   esp_log.h (ORO EGIPTO)    TSOFEH_LOG_*
 *
 * Autor: Antonio Jose Marin Soto (Betsalel - Ex 31:2)
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "esp_log.h"

#include "hashmal_comm.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *TSOFEH_TAG = "hashmal.menora.tsofeh";

#define TSOFEH_LOG_I(fmt, ...)  ESP_LOGI(TSOFEH_TAG, fmt, ##__VA_ARGS__)
#define TSOFEH_LOG_W(fmt, ...)  ESP_LOGW(TSOFEH_TAG, fmt, ##__VA_ARGS__)
#define TSOFEH_LOG_E(fmt, ...)  ESP_LOGE(TSOFEH_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * Estado interno
 *
 * Dos flags. g_tsofeh_lockdown es la intencion declarada por el rol
 * (pieza 09). g_shofar_burst es un ticket de paso efimero que solo
 * vive durante el toque del shofar. La lectura compuesta
 * (lockdown_get) devuelve "TX actualmente cerrado" = lockdown activo
 * Y no hay shofar en curso.
 * ================================================================== */
static bool g_tsofeh_lockdown = false;
static bool g_shofar_burst    = false;

/* ==================================================================
 * hashmal_comm_tsofeh_lockdown_set
 *
 * Enciende o apaga la puerta de TX general. No muta estado de nerot
 * (a diferencia del Shabat de Parokhet que marca mutsaqot.activa):
 * el atalaya necesita que la Menora SIGA operativa para poder
 * escuchar y para poder tocar el shofar cuando toque. Apagar nerot
 * seria apagar al atalaya.
 * ================================================================== */
int hashmal_comm_tsofeh_lockdown_set(bool activar)
{
    g_tsofeh_lockdown = activar;
    if (activar) {
        TSOFEH_LOG_I("lockdown ON - tsofeh vigilando (Ez 33:2-3); "
                     "TX gated excepto shofar");
    } else {
        TSOFEH_LOG_I("lockdown OFF - atalaya descendio de la muralla; "
                     "TX restaurado");
    }
    return HASHMAL_COMM_OK;
}

/* ==================================================================
 * hashmal_comm_tsofeh_lockdown_get
 *
 * Retorna el valor COMPUESTO: "TX esta cerrado ahora mismo?". Se
 * consume desde menora_routing.c al inicio de hashmal_comm_enviar.
 * ================================================================== */
bool hashmal_comm_tsofeh_lockdown_get(void)
{
    return g_tsofeh_lockdown && !g_shofar_burst;
}

/* ==================================================================
 * hashmal_comm_shofar_tocar
 *
 * Ez 33:3 taqa' bashofar. Broadcast que ATRAVIESA el lockdown.
 *
 * Secuencia atomica:
 *   1. Validacion de parametros (NULL / 0 / >MAX).
 *   2. Levanta ticket g_shofar_burst = true.
 *   3. Llama a hashmal_comm_broadcast - pasara por enviar(), y al
 *      consultar lockdown_get() este devuelve false por el ticket.
 *   4. Baja el ticket inmediatamente despues.
 *   5. Log WARN del toque (sonar el shofar no es informacion, es
 *      alarma: merece WARN minimo).
 *
 * El ticket se baja INCLUSO si broadcast falla - no queremos dejar
 * un agujero permanente en el gate si el shofar suena mal.
 * ================================================================== */
int hashmal_comm_shofar_tocar(const uint8_t *msg, size_t tam)
{
    if (msg == NULL) {
        TSOFEH_LOG_E("shofar: msg NULL - Ez 33:3 taqa' sin aliento");
        return HASHMAL_COMM_ERR_NULL;
    }
    if (tam == 0u || tam > HASHMAL_MENSAJE_MAX_BYTES) {
        TSOFEH_LOG_E("shofar: tam %u fuera de (0, %d]",
                     (unsigned)tam, HASHMAL_MENSAJE_MAX_BYTES);
        return HASHMAL_COMM_ERR_TAMANO;
    }

    g_shofar_burst = true;
    int rc = hashmal_comm_broadcast(msg, tam);
    g_shofar_burst = false;

    if (rc == HASHMAL_COMM_OK) {
        TSOFEH_LOG_W("SHOFAR tocado (Ez 33:3) - %u B a todos los nerot",
                     (unsigned)tam);
    } else {
        TSOFEH_LOG_E("SHOFAR fallo (rc=%d) - Ez 33:6 invariante INV-TSOFEH-01 "
                     "en peligro: aviso no pudo salir", rc);
    }
    return rc;
}

/* ==================================================================
 *
 *   "erets ki aviy aleiha jarev ulqejhu am haarets ish ejad
 *    miqtseihem venatnu oto lahem leTSOFEH." - Ezequiel 33:2
 *
 *   "vera'a et hajerev ba'a 'al haarets veTAQA' bashshofar vehizhir
 *    et ha'am." - Ezequiel 33:3
 *
 *   "vehatsofeh ki yir'a et hajerev ba'a veLO taqa' bashshofar,
 *    damo miyyado adrosh." - Ezequiel 33:6
 *
 *   El atalaya ve y avisa - o su sangre cae sobre su mano. Por eso
 *   el shofar tiene su propia puerta en el codigo: nadie puede
 *   impedir al atalaya tocarlo cuando ve la espada. El lockdown
 *   cierra TX ordinario; el shofar abre el tiempo exacto que dura.
 *
 * ================================================================== */
