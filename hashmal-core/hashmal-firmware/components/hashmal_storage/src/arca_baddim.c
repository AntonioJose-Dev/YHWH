/**
 * ===================================================================
 * PIEZA 01 - EL ARCA - arca_baddim.c
 * Sub-pieza: LOS BADDIM - varas de transporte y distribucion
 * Exodo 25:13-15 + 7 capas de transporte
 * ===================================================================
 *
 * "ve'asita baddei 'atse shittim vetsipita otam zahav" - v.13
 * "Haras VARAS de madera de acacia y las cubriras de oro."
 *
 * "veheveta et habaddim batabba'ot 'al tsal'ot ha'aron
 *  laset et ha'aron bahem" - v.14
 * "Meteras las varas por los anillos a los lados del Arca,
 *  para LLEVARLA con ellas."
 *
 * "betabbe'ot ha'aron yihyu habaddim LO YASURU mimmenu" - v.15
 * "En los anillos del Arca estaran las varas; NO SE QUITARAN de el."
 *
 * --- PALABRAS HEBREAS ---
 *
 *   badd       (BD, H905)   separacion, parte. Las varas SEPARAN
 *                             al portador del contenido. Nadie toca
 *                             el Arca - sostiene las varas.
 *   laset      (LShAT, H5375) levantar, cargar; misma raiz que
 *                             "perdonar" (Ex 34:7). Las varas llevan
 *                             la carga como el perdon lleva el pecado.
 *   lo yasuru  (LA YSRW) UNICO mandamiento negativo entre las 15
 *                             piezas: algo que NUNCA se desconecta.
 *                             Transport layer always-on.
 *   batabba'ot (BTB'T)   EN los anillos. Las varas pasan
 *                             A TRAVES - no sueldan, no pegan.
 *                             Conexion que permite movimiento pero
 *                             no separacion.
 *   'al tsal'ot ('L TsL'T) sobre las COSTILLAS. Misma raiz que
 *                             Gn 2:21 (Adam). Puntos de conexion
 *                             organicos, no parches.
 *
 * --- 7 CAPAS DEL BADDIM (2 Querubines Independientes) ---
 *
 *   L1  varas permanentes (v.15)            <- ESTE ARCHIVO (verificar)
 *   L2  nube senala marcha (Nm 9:17-23)     <- fuera de scope local
 *   L3  formacion preservada (Nm 2)         <- fuera de scope local
 *   L4  cifrado en transito (Nm 4:5-15)     <- arca_cifrado.c (invocado aqui)
 *   L5  transportistas sin acceso (Nm 4:15) <- arca_cifrado.c (descubrir)
 *   L6  mishpat inviolable (2 Sam 6)        <- arca_interfaz.c (pendiente)
 *   L7  Arca precede (Nm 10:33-36)          <- ESTE ARCHIVO (preceder)
 *
 * --- 3 FUNCIONES PUBLICAS ---
 *
 *   baddim_verificar       L1 - "lo yasuru mimmenu"
 *   replicar_a_nodo        ve'asu plural (v.10) - todos tienen copia
 *   preceder_en_migracion  L7 - "quma YHWH"
 *
 * --- DEPENDENCIAS ---
 *
 *   arca_core.c     -> hashmal_storage_arca_mut / get_arca
 *   arca_zer.c      -> hashmal_storage_zer_verificar  (no enviar corrupto)
 *   arca_cifrado.c  -> hashmal_storage_cubrir         (L4 - ciphertext en transito)
 *   hashmal_comm    -> STUB (pieza 04 aun pendiente)
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <inttypes.h>

#include "esp_log.h"

#include "hashmal_storage.h"
#include "hashmal_storage_internal.h"

/* ==================================================================
 * ENCAPSULACION
 * ================================================================== */
static const char *BAD_TAG = "hashmal.arca.baddim";

#define BAD_LOG_I(fmt, ...)  ESP_LOGI(BAD_TAG, fmt, ##__VA_ARGS__)
#define BAD_LOG_W(fmt, ...)  ESP_LOGW(BAD_TAG, fmt, ##__VA_ARGS__)
#define BAD_LOG_E(fmt, ...)  ESP_LOGE(BAD_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * ESTADO LOCAL DE MARCHA
 *
 * Nm 10:33-36 marca dos estados posibles:
 *   "quma YHWH"  (v.35) - el Arca SE LEVANTA y precede
 *   "shuva YHWH" (v.36) - el Arca SE POSA, reposo
 *
 * Este flag refleja en cual estamos. No persiste entre boots -
 * es intrinsecamente volatil (la migracion es un evento, no un estado
 * almacenado).
 * ================================================================== */
static bool s_en_migracion = false;

/* ==================================================================
 * PAQUETE DE REPLICA (interno a este archivo)
 *
 * Estructura minima de metadatos que hashmal_comm (Menora) usara
 * para entregar una copia del Arca al nodo destino. El cuerpo real
 * (ciphertext + iv + tag para edut, bytes para tora) lo lee la
 * Menora directamente de arca_cifrado.c / arca->torah en el
 * momento del envio.
 * ================================================================== */
typedef struct {
    uint8_t  nodo_destino;
    uint32_t edut_len;                           /* = HASHMAL_EDUT_BYTES */
    uint32_t torah_len;                          /* 0 si no adjunta */
    uint8_t  zer_hash[HASHMAL_ZER_HASH_BYTES];   /* para verificacion remota */
    uint64_t zer_nonce;                          /* anti-replay */
    bool     cubierta;                           /* L4 - envelope listo */
} baddim_paquete_t;

/* ==================================================================
 * HELPERS ESTATICOS
 * ================================================================== */

/**
 * Valida un indice de anillo y que ese anillo nazca organicamente
 * de la tsela' (v.12, Gn 2:21 - "fundido_en_estructura").
 */
static bool anillo_ok(const hashmal_arca_t *arca, uint8_t idx)
{
    if (idx >= HASHMAL_ANILLOS_TOTAL) return false;
    if (arca->anillos[idx].id != idx) return false;
    if (!arca->anillos[idx].fundido_en_estructura) return false;
    return true;
}

/**
 * Valida una vara individual contra v.13-15.
 *
 *   v.13 - acacia + oro (marca estructural, fijada en arca_core.c)
 *   v.14 - pasa por sus 2 anillos a los lados (tsal'ot)
 *   v.15 - NUNCA se quita ("lo yasuru mimmenu")
 *   L1   - always-on (siempre_activa)
 */
static bool bad_ok(const hashmal_arca_t *arca, uint8_t b)
{
    if (b >= HASHMAL_BADDIM_TOTAL) return false;

    const hashmal_bad_t *bad = &arca->baddim[b];

    if (bad->id != b) {
        BAD_LOG_E("vara %u: id inconsistente (leído %u)", b, bad->id);
        return false;
    }

    /* v.15 - "lo yasuru mimmenu": NO se quitaran. */
    if (!bad->nunca_se_quita) {
        BAD_LOG_E("v.15 violado: vara %u marcada retirable", b);
        return false;
    }

    /* L1 - transport always-on. */
    if (!bad->siempre_activa) {
        BAD_LOG_E("L1 violado: vara %u no está siempre-activa", b);
        return false;
    }

    /* v.14 - "batabba'ot": pasa por sus 2 anillos, distintos. */
    if (bad->anillo_a == bad->anillo_b) {
        BAD_LOG_E("v.14 violado: vara %u con anillos duplicados (%u,%u)",
                  b, bad->anillo_a, bad->anillo_b);
        return false;
    }
    if (!anillo_ok(arca, bad->anillo_a)) {
        BAD_LOG_E("v.14/Gn 2:21 violado: vara %u anillo_a (%u) no orgánico",
                  b, bad->anillo_a);
        return false;
    }
    if (!anillo_ok(arca, bad->anillo_b)) {
        BAD_LOG_E("v.14/Gn 2:21 violado: vara %u anillo_b (%u) no orgánico",
                  b, bad->anillo_b);
        return false;
    }
    return true;
}

/**
 * STUB - interfaz hacia hashmal_comm (Menora, pieza 04, Ex 25:31-40).
 * Cuando la Menora este viva, este stub se reemplazara por la
 * invocacion real. Por ahora registra la preparacion y retorna OK.
 *
 * Los 7 canales LoRa de la Menora (1 tronco + 6 ramas) seran los
 * que lleven el paquete fisicamente entre nodos.
 */
static int menora_enviar_paquete_stub(uint8_t nodo,
                                      const baddim_paquete_t *pkt)
{
    BAD_LOG_W("[Menorá pendiente] paquete preparado para nodo %u: "
              "edut=%" PRIu32 " B, torá=%" PRIu32 " B, "
              "nonce=%" PRIu64 ", cubierta=%d",
              nodo,
              pkt->edut_len,
              pkt->torah_len,
              pkt->zer_nonce,
              (int)pkt->cubierta);
    BAD_LOG_W("[Menorá pendiente] envío real requiere pieza 04 (Éx 25:31-40)");
    return HASHMAL_ARCA_OK;
}

/* ==================================================================
 * API PUBLICA - L1
 * ================================================================== */

/**
 * hashmal_storage_baddim_verificar - L1 ("lo yasuru mimmenu", v.15)
 *
 * Asegura que las 2 varas permanecen en su sitio, always-on,
 * correctamente enganchadas a sus 2 anillos organicos.
 *
 * 2 testigos:
 *   Ex 25:15   - "lo yasuru mimmenu"
 *   1 Cr 15:15 - "baddim 'al ketefam" (los levitas llevaron el
 *                Arca con varas sobre sus hombros, tal como Moises
 *                mando segun la Palabra de YHWH)
 */
int hashmal_storage_baddim_verificar(void)
{
    const hashmal_arca_t *arca = hashmal_storage_get_arca();
    if (arca == NULL) {
        BAD_LOG_E("baddim_verificar: Arca no levantada");
        return HASHMAL_ARCA_ERR_INIT;
    }

    for (uint8_t b = 0; b < HASHMAL_BADDIM_TOTAL; ++b) {
        if (!bad_ok(arca, b)) {
            BAD_LOG_E("baddim_verificar: vara %u NO APTA "
                      "(v.15 lo yasúru / 1 Cr 15:15)", b);
            return HASHMAL_ARCA_ERR_BADDIM_QUITADA;
        }
    }

    BAD_LOG_I("baddim verificadas — 2 varas ʻomdót en anillos orgánicos "
              "(v.13-15, 1 Cr 15:15)");
    return HASHMAL_ARCA_OK;
}

/* ==================================================================
 * API PUBLICA - ve'asu plural (v.10) -> distribucion
 *
 * yatsáq — Éx 25:12: anillos FUNDIDOS (H3332), reproducibles. La capa
 * de distribución es estándar en todos los nodos. "veyatsáqta lo
 * arbáʻ tabbeʻót zaháv" — fundidos en MOLDE: cada nodo recibe una
 * copia idéntica del protocolo de distribución, byte a byte. Contraste
 * intencional con miqsháh (Éx 25:18, kaporet_core.c) — la distribución
 * se REPRODUCE; el consenso NO se reproduce, se forja una sola vez.
 * ================================================================== */

/**
 * hashmal_storage_replicar_a_nodo - Ex 25:10 (ve'asu) + Nm 10:33
 *
 * Prepara una replica para un nodo peer. Orden estricto, L1->L4:
 *
 *   1. Verificar baddim (L1 - transport listo, v.15)
 *   2. Verificar zer (no enviar dato corrupto; Ex 25:11 saviv)
 *   3. Cubrir (L4 - cifrado en transito, Nm 4:5-6)
 *   4. Construir metadatos del paquete
 *   5. Entregar a la interfaz de envio (Menora - stub por ahora)
 *   6. Esperar confirmacion (asincrona, via replica_confirmada)
 *
 * replicas_confirmadas NO se incrementa aqui - se incrementara
 * cuando hashmal_comm invoque hashmal_storage_replica_confirmada
 * tras el ACK del receptor.
 *
 * 2 testigos:
 *   Ex 25:10 - "ve'asu" (PLURAL: todos tienen copia)
 *   Nm 10:33 - "va'aron berit YHWH nosea' lifnehem" (el Arca
 *              iba delante del pueblo)
 */
int hashmal_storage_replicar_a_nodo(uint8_t nodo_id)
{
    const hashmal_arca_t *arca = hashmal_storage_get_arca();
    if (arca == NULL) {
        BAD_LOG_E("replicar: Arca no levantada");
        return HASHMAL_ARCA_ERR_INIT;
    }

    /* L1 - las varas deben estar listas antes de mover nada. */
    int rc = hashmal_storage_baddim_verificar();
    if (rc != HASHMAL_ARCA_OK) {
        BAD_LOG_E("replicar: baddim no aptas — réplica cancelada (L1)");
        return rc;
    }

    /* zer integro antes de cifrar y enviar: nunca propagamos dato
     * corrupto. Si el zer se rompio, parar aqui - no contaminar peers. */
    rc = hashmal_storage_zer_verificar();
    if (rc != HASHMAL_ARCA_OK) {
        BAD_LOG_E("replicar: zer roto — dato NO sale del nodo (Éx 25:11)");
        return HASHMAL_ARCA_ERR_ZER_ROTO;
    }

    /* L4 - cubrir (Nm 4:5-6). Idempotente si ya cubierta. */
    rc = hashmal_storage_cubrir();
    if (rc != HASHMAL_ARCA_OK) {
        BAD_LOG_E("replicar: cubrir falló — réplica cancelada (L4)");
        return rc;
    }

    /* Metadatos del paquete. Los bytes reales (edut cifrada + tora)
     * los lee hashmal_comm directamente de los modulos hermanos en
     * el momento del envio - aqui solo anunciamos la intencion. */
    baddim_paquete_t pkt;
    memset(&pkt, 0, sizeof(pkt));
    pkt.nodo_destino = nodo_id;
    pkt.edut_len     = HASHMAL_EDUT_BYTES;
    pkt.torah_len    = (arca->torah.bytes != NULL)
                         ? (uint32_t)arca->torah.tamano
                         : 0u;
    memcpy(pkt.zer_hash, arca->zer.hash, HASHMAL_ZER_HASH_BYTES);
    pkt.zer_nonce = arca->zer.nonce;
    pkt.cubierta  = true;   /* acabamos de cubrir exitosamente */

    /* Entrega a la Menora (stub). El envio real ocurre cuando
     * pieza 04 este viva. */
    rc = menora_enviar_paquete_stub(nodo_id, &pkt);
    if (rc != HASHMAL_ARCA_OK) {
        BAD_LOG_E("replicar: Menorá rechazó el paquete (rc=%d)", rc);
        return rc;
    }

    BAD_LOG_I("réplica ENTREGADA a la Menorá → nodo %u "
              "(veʻasú, Éx 25:10)", nodo_id);
    BAD_LOG_I("  esperando ACK del peer — replicas_confirmadas se "
              "actualizará vía replica_confirmada()");
    return HASHMAL_ARCA_OK;
}

/* ==================================================================
 * API PUBLICA - L7 (Arca precede)
 * ================================================================== */

/**
 * hashmal_storage_preceder_en_migracion - Nm 10:33-36
 *
 * "va'aron berit YHWH nosea' lifnehem derej sheloshet yamim
 *  latur lahem menuja" - Nm 10:33
 * "El Arca del Pacto de YHWH iba DELANTE de ellos tres dias de
 *  camino para buscarles reposo."
 *
 * "quma YHWH veyafutsu oyeveja" - Nm 10:35
 * "Levantate, YHWH, y que se dispersen Tus enemigos."
 *
 * L7 - el almacen primario se mueve PRIMERO. Los servicios
 * dependientes (cache, comm, auth...) esperan el reposo
 * ("shuva YHWH", v.36) antes de migrar ellos.
 *
 * Precondiciones: baddim aptas (L1), cubierta (L4), zer integro.
 * Si alguna falla -> no se precede: el Arca no se mueve corrupta.
 *
 * 2 testigos:
 *   Nm 10:33    - "nosea' lifnehem" (iba delante)
 *   Jos 3:3-4   - "cuando veais el Arca delante de vosotros,
 *                  entonces os movereis de vuestro lugar"
 */
int hashmal_storage_preceder_en_migracion(void)
{
    const hashmal_arca_t *arca = hashmal_storage_get_arca();
    if (arca == NULL) {
        BAD_LOG_E("preceder: Arca no levantada");
        return HASHMAL_ARCA_ERR_INIT;
    }

    if (s_en_migracion) {
        BAD_LOG_W("preceder: ya en migración (idempotente, no-op)");
        return HASHMAL_ARCA_OK;
    }

    /* L1 - varas aptas */
    int rc = hashmal_storage_baddim_verificar();
    if (rc != HASHMAL_ARCA_OK) {
        BAD_LOG_E("preceder: baddim no aptas (L1/v.15)");
        return rc;
    }

    /* zer integro antes de moverse */
    rc = hashmal_storage_zer_verificar();
    if (rc != HASHMAL_ARCA_OK) {
        BAD_LOG_E("preceder: zer roto — no se migra dato corrupto");
        return HASHMAL_ARCA_ERR_ZER_ROTO;
    }

    /* L4 - cubrir antes de cualquier movimiento (Nm 4:5-6) */
    rc = hashmal_storage_cubrir();
    if (rc != HASHMAL_ARCA_OK) {
        BAD_LOG_E("preceder: cubrir falló (L4/Nm 4:5-6)");
        return rc;
    }

    s_en_migracion = true;

    BAD_LOG_I("═══ 'QUMÁ YHWH' (Nm 10:35) — ARCA PRECEDE ═══");
    BAD_LOG_I("  el almacén primario se mueve PRIMERO (L7)");
    BAD_LOG_I("  servicios dependientes esperan 'shuvá YHWH' (v.36)");
    return HASHMAL_ARCA_OK;
}

/* ==================================================================
 * API INTERNA (hashmal_storage_internal.h)
 *
 * Invocadas por hashmal_comm cuando este viva. Cierran el ciclo
 * asincrono de migracion y replica.
 * ================================================================== */

int hashmal_storage_replica_confirmada(uint8_t nodo_destino)
{
    hashmal_arca_t *arca = hashmal_storage_arca_mut();
    if (arca == NULL) {
        BAD_LOG_E("replica_confirmada: Arca no levantada");
        return HASHMAL_ARCA_ERR_INIT;
    }

    /* Contador simple: una confirmacion por llamada. Una iteracion
     * futura llevara bitmap por nodo para evitar doble-conteo si un
     * peer ACK'ea dos veces por ruido de red. Por ahora: trust + count. */
    arca->replicas_confirmadas += 1u;
    arca->distribuida = true;   /* ve'asu cumplido: hay al menos 1 replica */

    BAD_LOG_I("réplica CONFIRMADA en nodo %u — total=%" PRIu32
              " (veʻasú plural, Éx 25:10)",
              nodo_destino, arca->replicas_confirmadas);
    return HASHMAL_ARCA_OK;
}

bool hashmal_storage_en_migracion(void)
{
    return s_en_migracion;
}

void hashmal_storage_en_migracion_fin(void)
{
    if (!s_en_migracion) {
        /* "shuva YHWH" sin marcha previa - no hay sobre que reposar. */
        return;
    }
    s_en_migracion = false;
    BAD_LOG_I("═══ 'SHUVÁ YHWH' (Nm 10:36) — ARCA EN REPOSO ═══");
    BAD_LOG_I("  servicios dependientes pueden avanzar");
}
