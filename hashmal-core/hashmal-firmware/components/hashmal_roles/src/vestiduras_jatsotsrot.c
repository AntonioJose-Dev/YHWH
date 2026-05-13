/*
 * ===================================================================
 * PIEZA 09 - VESTIDURAS - vestiduras_jatsotsrot.c
 * Añadidura 08: LAS DOS TROMPETAS DE PLATA
 * Numeros 10:1-10 + Exodo 19:16 + Salmo 98:6 + 1 Cronicas 15:24
 * ===================================================================
 *
 * "'aseh lekha SHETEI JATSOTSROT KESEF MIQSHAH ta'aseh otam vehayu
 *  lekha leMIQRA ha'edah ulmass'A et hammajanot." - Numeros 10:2
 * "Hazte DOS TROMPETAS DE PLATA, de obra BATIDA las haras, para
 *  CONVOCAR a la congregacion y para MOVER los campamentos."
 *
 * "vetaq'u bahen veNO'ADU eleikha kol ha'edah el petach Ohel
 *  Mo'ed." - Numeros 10:3
 * "Cuando tocaren CON ELLAS (las dos), se CONGREGARA ante ti TODA
 *  la congregacion a la puerta del Ohel Mo'ed."
 *
 * "ve'im beACHAT yitqa'u veNO'ADU eleikha hanesi'im rashei alfei
 *  Yisrael." - Numeros 10:4
 * "Si tocaren con UNA sola se congregaran los NESI'IM, cabezas de
 *  los millares de Israel."
 *
 * "utqa'tem TERUAH venas'u hamachanot." - Numeros 10:5
 * "Tocareis alarma y marcharan los campamentos."
 *
 * "UVHAQHIL et haqqahal TITQE'U velo TARI'U." - Numeros 10:7
 * "Al CONGREGAR la asamblea tocareis pero NO tocareis alarma."
 *
 * "uvnei Aharon HAKKOHANIM yitq'eu bajatsotsrot." - Numeros 10:8
 * "Los hijos de Aaron, LOS SACERDOTES, tocaran las trompetas."
 *
 * "veki tavo'u MILJAMAH... vaHARE'OTEM bajatsotsrot venizkartem
 *  lifnei YHWH... venosha'tem meoyveikhem." - Numeros 10:9
 * "Cuando fueres a la GUERRA tocareis alarma, y sereis RECORDADOS
 *  ante YHWH y SALVADOS de vuestros enemigos."
 *
 * "uvyom SIMJATKHEM uvMO'ADEIKHEM... utqa'tem bajatsotsrot." - Nm 10:10
 * "Y en el dia de VUESTRA ALEGRIA y en vuestros MO'ADIM tocareis."
 *
 * --- EXEGESIS OPERATIVA ---
 *
 *   jatsotsrot (H2689, pl.)    TROMPETAS - dualidad en el instrumento.
 *                               Dos, no una. Pueden sonar juntas o
 *                               separadas; el numero de trompetas que
 *                               SUENAN distingue el tipo de toque.
 *
 *   kesef miqshah (H3701+H4749) PLATA BATIDA - sola una pieza golpeada,
 *                               no soldada. Integridad del instrumento.
 *
 *   teqiah vs teruah (H8628 vs H8643) CONVOCAR vs ALARMAR. Nm 10:7
 *                               ordena no confundirlos. Teqiah es toque
 *                               LARGO y claro; teruah es pulso
 *                               entrecortado, de emergencia.
 *
 *   yitq'eu haKohanim (H3548)  SOLO LOS SACERDOTES tocan. No cualquier
 *                               israelita. Autoridad canalizada.
 *
 * --- DISTINCION CON EL SHOFAR (Añadidura 02) ---
 *
 *   Shofar (cuerno de carnero): Ez 33 atalaya, emergencia detectada,
 *                               bypasea tsofeh_lockdown.
 *   Jatsotsrot (trompetas plata): Nm 10 sacerdotes, ritual + tipos,
 *                               broadcast autorizado por rol, NO
 *                               bypasea tsofeh_lockdown.
 *
 * Convergen en propiedades (ambos "suenan") pero son instrumentos
 * distintos con reglas distintas. Si el nodo es atalaya Y sacerdote,
 * el caller elige cual API invocar segun el caso.
 *
 * --- AUTORIZACION (Nm 10:8) ---
 *
 * Tabla implementada en autorizar_interna:
 *
 *   Rol/Tipo       SHETAYIM  AJAT   TERUAH  MOEDIM  MILJAMAH
 *   NINGUNO          -        -      -       -        -
 *   ISRAELITA        -        -      -       -        -
 *   LEVI             -        si     -       si       -
 *   KOHEN            si       si     si      si       si
 *   KOHEN_GADOL      si       si     si      si       si
 *
 * Levi coopera en AJAT (llamar lideres) y MOEDIM (fiestas), pero
 * no en emergencias (TERUAH / MILJAMAH) - esas son Kohen+.
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_roles.h          API publica + rol estado
 *   hashmal_comm.h           broadcast
 *   esp_log.h (ORO EGIPTO)   JATS_LOG_*
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

#include "hashmal_roles.h"
#include "hashmal_comm.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *JATS_TAG = "hashmal.roles.jatsotsrot";

#define JATS_LOG_I(fmt, ...)  ESP_LOGI(JATS_TAG, fmt, ##__VA_ARGS__)
#define JATS_LOG_W(fmt, ...)  ESP_LOGW(JATS_TAG, fmt, ##__VA_ARGS__)
#define JATS_LOG_E(fmt, ...)  ESP_LOGE(JATS_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * Tabla de autorizacion (Nm 10:3-10) - funcion pura
 * ================================================================== */
static bool autorizar_interna(hashmal_roles_tipo_rol_t rol,
                              hashmal_jatsotsrot_tipo_t tipo)
{
    switch (rol) {
    case HASHMAL_ROLES_ROL_NINGUNO:
    case HASHMAL_ROLES_ROL_ISRAELITA:
        /* Nm 10:8 - el toque de trompeta es oficio sacerdotal.
         * Israelita comun no. */
        return false;

    case HASHMAL_ROLES_ROL_LEVI:
        /* Cooperador ritual. Puede llamar lideres (AJAT) y anunciar
         * fiestas (MOEDIM). Emergencias las hace el sacerdote. */
        return (tipo == HASHMAL_JATSOTSROT_TEQIAH_AJAT)
            || (tipo == HASHMAL_JATSOTSROT_MOEDIM);

    case HASHMAL_ROLES_ROL_KOHEN:
    case HASHMAL_ROLES_ROL_KOHEN_GADOL:
        /* Nm 10:8 - beney Aharon haKohanim yitq'eu: autoridad plena. */
        return true;
    }
    return false;
}

/* Helper nombre de tipo - tambien exportado como API publica. */
const char *hashmal_jatsotsrot_tipo_nombre(hashmal_jatsotsrot_tipo_t tipo)
{
    switch (tipo) {
    case HASHMAL_JATSOTSROT_TEQIAH_SHETAYIM: return "TEQIAH SHETAYIM (Nm 10:3, toda la 'edah)";
    case HASHMAL_JATSOTSROT_TEQIAH_AJAT:     return "TEQIAH AJAT (Nm 10:4, solo nesi'im)";
    case HASHMAL_JATSOTSROT_TERUAH:          return "TERUAH (Nm 10:5, alarma)";
    case HASHMAL_JATSOTSROT_MOEDIM:          return "MOEDIM (Nm 10:10, fiestas)";
    case HASHMAL_JATSOTSROT_MILJAMAH:        return "MILJAMAH (Nm 10:9, guerra)";
    default:                                  return "<tipo desconocido>";
    }
}

/* ==================================================================
 * API - hashmal_jatsotsrot_autorizar (lectura pura)
 * ================================================================== */
bool hashmal_jatsotsrot_autorizar(hashmal_roles_tipo_rol_t rol,
                                   hashmal_jatsotsrot_tipo_t tipo)
{
    return autorizar_interna(rol, tipo);
}

/* ==================================================================
 * API - hashmal_jatsotsrot_tocar (Nm 10:8 yitq'eu haKohanim)
 * ================================================================== */
hashmal_roles_err_t hashmal_jatsotsrot_tocar(hashmal_jatsotsrot_tipo_t tipo,
                                              uint8_t emisor_nodo_id,
                                              const uint8_t *payload,
                                              size_t tam_payload)
{
    if (payload == NULL) {
        JATS_LOG_E("tocar: payload NULL");
        return HASHMAL_ROLES_ERR_NULL;
    }
    if (tam_payload == 0u
     || tam_payload + HASHMAL_JATSOTSROT_HEADER_BYTES > (size_t)HASHMAL_MENSAJE_MAX_BYTES) {
        JATS_LOG_E("tocar: tam_payload %u fuera de (0, %u]",
                   (unsigned)tam_payload,
                   (unsigned)(HASHMAL_MENSAJE_MAX_BYTES - HASHMAL_JATSOTSROT_HEADER_BYTES));
        return HASHMAL_ROLES_ERR_JATSOTSROT;
    }

    if (emisor_nodo_id >= HASHMAL_ROLES_MAX_NODOS) {
        JATS_LOG_E("tocar: emisor_nodo_id=%u fuera de [0, %u)",
                   (unsigned)emisor_nodo_id,
                   (unsigned)HASHMAL_ROLES_MAX_NODOS);
        return HASHMAL_ROLES_ERR_NODO_NO_ENCONTRADO;
    }

    /* Lookup rol del emisor. Nm 10:8 - solo sacerdotes tocan. */
    hashmal_roles_estado_roles_t st;
    memset(&st, 0, sizeof(st));
    hashmal_roles_err_t rc_st = hashmal_roles_estado(&st);
    if (rc_st != HASHMAL_ROLES_OK) {
        JATS_LOG_E("tocar: no se pudo leer estado roles (rc=%d)", (int)rc_st);
        return rc_st;
    }

    hashmal_roles_tipo_rol_t rol = st.nodos[emisor_nodo_id].rol;
    if (!autorizar_interna(rol, tipo)) {
        JATS_LOG_W("tocar: nodo %u rol=%d NO autorizado para %s "
                   "(Nm 10:8 yitq'eu haKohanim)",
                   (unsigned)emisor_nodo_id, (int)rol,
                   hashmal_jatsotsrot_tipo_nombre(tipo));
        return HASHMAL_ROLES_ERR_JATSOTSROT;
    }

    /* Nm 10:7 - distincion explicita: teqiah != teruah. Solo log
     * semantico; la autorizacion ya fue verificada por tipo. */
    if (tipo == HASHMAL_JATSOTSROT_TERUAH
     || tipo == HASHMAL_JATSOTSROT_MILJAMAH) {
        JATS_LOG_W("tocar: alarma (%s) - NO es convocacion (Nm 10:7)",
                   hashmal_jatsotsrot_tipo_nombre(tipo));
    }

    /* Compose wire: [tipo_byte][payload...]. Fase 1 - receptor
     * parseara byte 0 cuando se implemente consumo. */
    uint8_t wire_buf[HASHMAL_MENSAJE_MAX_BYTES];
    wire_buf[0] = (uint8_t)tipo;
    memcpy(&wire_buf[1], payload, tam_payload);
    size_t wire_len = HASHMAL_JATSOTSROT_HEADER_BYTES + tam_payload;

    /* Dispatch. Si tsofeh_lockdown (Añadidura 02) esta activo, el
     * broadcast fallara con ERR_TSOFEH - traducimos a
     * ERR_JATSOTSROT para que el caller vea "trompeta fallo" en el
     * dominio del rol. */
    int rc = hashmal_comm_broadcast(wire_buf, wire_len);
    if (rc != HASHMAL_COMM_OK) {
        JATS_LOG_E("tocar: broadcast fallo rc=%d (tipo=%s, emisor=%u)",
                   rc, hashmal_jatsotsrot_tipo_nombre(tipo),
                   (unsigned)emisor_nodo_id);
        return HASHMAL_ROLES_ERR_JATSOTSROT;
    }

    JATS_LOG_I("tocar: %s - emisor=%u (rol=%d) - %u B + header "
               "(Nm 10:8 haKohanim yitq'eu)",
               hashmal_jatsotsrot_tipo_nombre(tipo),
               (unsigned)emisor_nodo_id, (int)rol,
               (unsigned)tam_payload);
    return HASHMAL_ROLES_OK;
}

/* ==================================================================
 *
 *   "'aseh lekha SHETEI JATSOTSROT KESEF MIQSHAH." - Numeros 10:2
 *
 *   "uvnei Aharon HAKKOHANIM yitq'eu." - Numeros 10:8
 *
 *   "TITQE'U velo TARI'U." - Numeros 10:7
 *
 *   "vaHARE'OTEM... veNIZKARTEM lifnei YHWH venosha'tem." - Nm 10:9
 *
 *   Dos trompetas de plata batida. Suenan DOS, se rene todo; suena
 *   UNA, se rene el consejo. Alarma (teruah) mueve el campamento;
 *   convocatoria (teqiah) lo asienta. Nm 10:7 ordena no confundir
 *   los dos toques - el pueblo que no distingue convocatoria de
 *   alarma vive en palpitacion constante.
 *
 *   Y solo los sacerdotes tocan: la autoridad no es del que tiene
 *   aliento, sino del que esta vestido de oficio. El firmware honra
 *   ese filtro: comprobar rol antes de broadcast. Asi la senal
 *   publica es la del Padre, no la de cualquiera.
 *
 * ================================================================== */
