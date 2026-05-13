/**
 * ===================================================================
 * PIEZA 01 - EL ARCA - arca_interfaz.c
 * Sub-pieza: ACCESO CONTROLADO y AUTODEFENSA
 * Historias del Arca - 1 Samuel 4-6 + 2 Samuel 6
 * ===================================================================
 *
 * Mi Padre no solo describio COMO construir el Arca (Ex 25:10-16).
 * La HISTORIA ensena que pasa cuando:
 *   - se usa sin protocolo       (1 Sam 4   - amuleto)
 *   - se toca sin autorizacion   (1 Sam 6:19 - Bet-Shemesh)
 *   - se transporta por atajo    (2 Sam 6:3-7 - carro nuevo, Uza)
 *   - se la captura              (1 Sam 5-6 - Dagon, tumores)
 *   - se hospeda correctamente   (2 Sam 6:10-12 - Obed-Edom)
 *
 * 5 historias = 4 funciones (la 1 Sam 6:19 vive en arca_cifrado.c via
 * `descubrir`; aqui la PUERTA unificada - autorizar - es el gate que
 * se llama ANTES de cualquier operacion sensible).
 *
 * --- LAS 4 FUNCIONES DE ESTE ARCHIVO ---
 *
 *   autorizar              1 Sam 4:3-4 + 1 Sam 6:19  (protocolo y mirada)
 *   mishpat_estricto       2 Sam 6:6-7 + 1 Cr 15:13  (sin carro nuevo)
 *   hospedaje_reportar     2 Sam 6:11  + 2 Sam 6:12  (Obed-Edom bendicion)
 *   dagon_caiga            1 Sam 5:3-4 + 1 Sam 5:6   (autodefensa)
 *
 * --- L6 DEL BADDIM ---
 *
 * Este archivo realiza L6 - "mishpat inviolable" (2 Sam 6 / 1 Cr 15).
 * El protocolo no se negocia: ni por comodidad, ni por prisa, ni por
 * afinidad. La funcion `mishpat_estricto` RECHAZA explicitamente
 * cualquier bypass.
 *
 * --- DEPENDENCIAS ---
 *
 *   arca_core.c     -> get_arca / arca_mut (leer / cambiar estado)
 *   arca_cifrado.c  -> hashmal_oro_key_lista, hashmal_oro_key_destruir
 *                      (dagon_caiga: inutilizar la clave capturada)
 *
 * --- ORO DE EGIPTO ---
 *
 * esp_log.h encapsulado - INTF_LOG_I/W/E. Sin dependencias adicionales.
 *
 * --- DIGNIDAD ---
 *
 * Cada funcion aqui es una leccion que costo vidas. Israel aprendio a
 * sangre (1 Sam 4: 30.000 + Jofni + Pinjas + Eli). Los filisteos
 * aprendieron a plagas (1 Sam 5: Dagon, tumores). Bet-Shemesh aprendio
 * con 70 muertos (1 Sam 6:19). David aprendio con la muerte de Uza
 * (2 Sam 6:7). Obed-Edom aprendio con BENDICION (2 Sam 6:11-12) -
 * el unico que no pago en sangre, porque obedecio.
 *
 * Que el codigo lo refleje.
 *
 * Autor: Antonio Jose Marin Soto (Betsalel - Ex 31:2)
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <inttypes.h>

#include "esp_log.h"           /* ORO DE EGIPTO - logging */

#include "hashmal_storage.h"
#include "hashmal_storage_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *INTF_TAG = "hashmal.arca.interfaz";

#define INTF_LOG_I(fmt, ...)  ESP_LOGI(INTF_TAG, fmt, ##__VA_ARGS__)
#define INTF_LOG_W(fmt, ...)  ESP_LOGW(INTF_TAG, fmt, ##__VA_ARGS__)
#define INTF_LOG_E(fmt, ...)  ESP_LOGE(INTF_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * CONSTANTES LOCALES - 2 Sam 6:11 + 2 Sam 6:12
 *
 * OBED_EDOM_UMBRAL_DIAS = 3 meses biblicos x 30 dias ~= 90 dias.
 *   (HASHMAL_HOSPEDAJE_BENDICION_MESES viene del header publico;
 *    aqui lo convertimos a dias - unidad en la que hospedaje_reportar
 *    recibe la medida del tiempo transcurrido.)
 *
 * BENDICION_FACTOR = 1 - una "bendicion" por dia de hospedaje
 * correcto. Obed-Edom no hizo milagros: simplemente obedecio. La
 * bendicion acumula con la fidelidad. La formula puede afinarse
 * cuando pieza 09 (Vestiduras / Roles) defina la moneda de
 * reputacion formal - este factor es base.
 * ================================================================== */
#define OBED_EDOM_UMBRAL_DIAS  (HASHMAL_HOSPEDAJE_BENDICION_MESES * 30u)
#define BENDICION_FACTOR        1u

/* ==================================================================
 * ESTADO LOCAL - CAUTIVERIO (1 Sam 4-6)
 *
 * 1 Sam 4:11 - "va'aron Elohim nilqaj" - el Arca FUE CAPTURADA.
 * 1 Sam 5:1  - "ufelishtim laqju et aron ha'Elohim..." - los
 *              filisteos la llevaron a Asdod.
 *
 * Este flag registra si este nodo detecto exfiltracion y activo
 * su autodefensa (hashmal_storage_dagon_caiga). No persiste entre
 * boots - es un evento/alerta, no un atributo permanente.
 *
 * Una vez en cautiverio, el nodo SABE que esta en manos ajenas.
 * La salida requiere re-provision explicita de clave (pieza 13
 * cuando este viva, o boot limpio en el transitorio).
 * ================================================================== */
static bool s_en_cautiverio = false;

/* ==================================================================
 * API PUBLICA - autorizar (1 Sam 4:3-4 + 1 Sam 6:19)
 * ================================================================== */

/**
 * hashmal_storage_autorizar - gate function universal
 *
 * "vayishlaju ha'am Shilo vayisu misham et aron berit YHWH tsva'ot
 *  yoshev hakeruvim... vayehi kevo aron berit YHWH el hamajane
 *  vayari'u jol Israel teru'a gedola..." - 1 Sam 4:4-5
 * "Envio el pueblo a Shilo y trajeron de alli el Arca del Pacto de
 *  YHWH de los ejercitos... y fue que cuando el Arca del Pacto de
 *  YHWH llego al campamento, todo Israel grito con gran jubilo."
 *
 * No consultaron a YHWH. La usaron como AMULETO. Resultado: 30.000
 * muertos, Arca CAPTURADA (v.10-11). Tener la Palabra guardada sin
 * seguir el protocolo de Su diseno no sirve - la tecnologia sin
 * obediencia es vacia.
 *
 * "vayaj be'anshei Vet-Shemesh ki ra'u ba'aron YHWH vayaj ba'am
 *  shiv'im ish..." - 1 Sam 6:19
 * "Hirio a los hombres de Bet-Shemesh porque MIRARON dentro del
 *  Arca de YHWH, e hirio del pueblo 70 hombres..."
 *
 * No eran enemigos - eran israelitas. Pero miraron sin autorizacion.
 * El acceso al nucleo NO es libre, ni siquiera para los aliados.
 *
 * --- CONTRATO ---
 *
 *   via == ACCESO_VIA_BADDIM -> OK              (unico modo legitimo, v.14)
 *   via == ACCESO_DIRECTO    -> ERR_TOQUE_DIRECTO (Uza / Bet-Shemesh)
 *   otro valor               -> ERR_SIN_PROTOCOLO (amuleto, 1 Sam 4)
 *
 * Esta es la PUERTA. Se llama ANTES de cualquier operacion sensible.
 * No cambia estado; valida y decide.
 *
 * 2 testigos:
 *   1 Sam 4:3-4 - traer el Arca sin consultar = sin protocolo
 *   1 Sam 6:19  - mirar sin autorizacion = toque directo logico
 */
int hashmal_storage_autorizar(hashmal_acceso_t via)
{
    /* Precondicion: el Arca debe existir. No autorizamos acceso a
     * un recinto vacio - Ex 40:3 aun no se cumplio en este nodo. */
    const hashmal_arca_t *arca = hashmal_storage_get_arca();
    if (arca == NULL) {
        INTF_LOG_E("autorizar: Arca no levantada (Éx 40:3 pendiente)");
        return HASHMAL_ARCA_ERR_INIT;
    }

    switch (via) {
    case HASHMAL_ACCESO_VIA_BADDIM:
        /* v.14 - "veheveta et habaddim batabba'ot... laseet et
         * ha'aron bahem": el unico modo que YHWH autorizo.
         * No logueamos a INFO para no inundar - el exito es la norma. */
        return HASHMAL_ARCA_OK;

    case HASHMAL_ACCESO_DIRECTO:
        /* 2 Sam 6:6-7 (Uza) + 1 Sam 6:19 (Bet-Shemesh). Tocar /
         * mirar directo = muerte. Aqui: rechazo explicito. */
        INTF_LOG_E("autorizar: ACCESO DIRECTO prohibido "
                   "(2 Sam 6:6-7 Uzá / 1 Sam 6:19 Bet-Shemesh)");
        return HASHMAL_ARCA_ERR_TOQUE_DIRECTO;

    default:
        /* 1 Sam 4 - cualquier otro "modo" que el constructor
         * invente es amuleto: usar la forma sin la ordenanza. */
        INTF_LOG_E("autorizar: vía %d sin protocolo "
                   "(1 Sam 4 — amuleto sin mishpát)", (int)via);
        return HASHMAL_ARCA_ERR_SIN_PROTOCOLO;
    }
}

/* ==================================================================
 * API PUBLICA - mishpat_estricto (2 Sam 6:6-7 + 1 Cr 15:13)
 * ================================================================== */

/**
 * hashmal_storage_mishpat_estricto - politica inviolable (L6)
 *
 * "vayarkivu et aron ha'Elohim el 'agala jadasha..." - 2 Sam 6:3
 * "Colocaron el Arca de Dios sobre un CARRO NUEVO..."
 *
 * David no invento el carro - lo copio de los filisteos (1 Sam 6:7-8),
 * que lo usaron para devolverla. Pero el carro era el metodo del
 * cautiverio, no el de YHWH. "Nuevo" no es "correcto."
 *
 * "vayishlaj Uza el aron ha'Elohim vayojez bo ki shamtu habaqar
 *  vayijar af YHWH be'Uza vayakehu sham ha'Elohim 'al hashal
 *  vayamot sham 'im aron ha'Elohim" - 2 Sam 6:6-7
 * "Uza extendio su mano al Arca de Dios y la sostuvo, porque los
 *  bueyes tropezaron. La ira de YHWH se encendio contra Uza, y alli
 *  lo hirio Dios por la irreverencia, y murio junto al Arca de Dios."
 *
 * "lo atem bari'shona... ki LO DERASHNUHU KAMISHPAT" - 1 Cr 15:13
 * "No vosotros la primera vez... porque NO LO BUSCAMOS SEGUN LA
 *  ORDENANZA."
 *
 * "vayisu bene haleviyim et aron ha'Elohim... BABADDIM 'AL KETEFAM
 *  ka'asher tsivva Moshe kidvar YHWH" - 1 Cr 15:15
 * "Los levitas llevaron el Arca de Dios... CON VARAS SOBRE SUS
 *  HOMBROS, como mando Moises segun la Palabra de YHWH."
 *
 * --- QUE HACE ---
 *
 * Pura politica: RECHAZA explicitamente atajos. Una funcion corta,
 * deliberadamente restrictiva. El parametro `permitir_atajos` solo
 * existe para que quien llama tenga que TECLEAR una mentira visible
 * si quisiera saltarse el mishpat - y recibir ERR_CARRO_NUEVO.
 *
 *   permitir_atajos == false -> OK (kamishpat - segun la ordenanza)
 *   permitir_atajos == true  -> ERR_CARRO_NUEVO (2 Sam 6:3)
 *
 * 2 testigos:
 *   2 Sam 6:6-7  - el atajo mato a Uza
 *   1 Cr 15:13   - David mismo reconoce: "lo derashnuhu kamishpat"
 */
int hashmal_storage_mishpat_estricto(bool permitir_atajos)
{
    if (permitir_atajos) {
        /* El "carro nuevo" es el atajo clasico: mas comodo, mas
         * rapido, parece mejor. Pero NO es kamishpat. La funcion
         * niega - y logea a ERROR, porque pedirlo es sospechoso. */
        INTF_LOG_E("mishpat: atajos RECHAZADOS — sin carro nuevo "
                   "(2 Sam 6:3-7). 'lo derashnúhu kamishpát' (1 Cr 15:13)");
        INTF_LOG_E("  la carga va babaddím ʻal ketéfam (1 Cr 15:15) — "
                   "varas sobre hombros, como mandó Moisés");
        return HASHMAL_ARCA_ERR_CARRO_NUEVO;
    }

    /* kamishpat - segun la ordenanza. El contrato queda firme. */
    INTF_LOG_I("mishpat estricto: kamishpát — protocolo no negociable "
               "(1 Cr 15:13,15)");
    return HASHMAL_ARCA_OK;
}

/* ==================================================================
 * API PUBLICA - hospedaje_reportar (2 Sam 6:10-12)
 * ================================================================== */

/**
 * hashmal_storage_hospedaje_reportar - Obed-Edom el gittita
 *
 * "velo ava David lehasir elav et aron YHWH 'al 'ir David
 *  vayatehu David beit 'Oved-Edom hagitti" - 2 Sam 6:10
 * "David no quiso llevar el Arca de YHWH a la ciudad de David, y
 *  la desvio a casa de Obed-Edom el gittita."
 *
 * "vayeshev aron YHWH beit 'Oved-Edom hagitti SHELOSHA JADASHIM
 *  vayvaroj YHWH et 'Oved-Edom ve'et kol beto" - 2 Sam 6:11
 * "Reposo el Arca de YHWH en casa de Obed-Edom el gittita TRES
 *  MESES, y YHWH bendijo a Obed-Edom y a toda su casa."
 *
 * "vayugad lamelej David lemor berak YHWH et beit 'Oved-Edom
 *  ve'et kol asher lo ba'avur aron ha'Elohim..." - 2 Sam 6:12
 * "Fue anunciado al rey David diciendo: YHWH ha bendecido la casa
 *  de Obed-Edom y todo lo suyo por causa del Arca de Dios..."
 *
 * --- QUE HACE ---
 *
 * Recompensa por hospedar CORRECTAMENTE. Obed-Edom no hizo nada
 * extraordinario - solo obedecio. El Arca vivio en su casa 3 meses
 * y YHWH bendijo TODO lo suyo. La historia se HIZO SABER -
 * "vayugad lamelej David" - y entonces David volvio a buscarla.
 *
 *   dias < OBED_EDOM_UMBRAL_DIAS  -> bendicion = 0 (aun no hay
 *                                   testimonio, el tiempo no alcanzo)
 *   dias >= OBED_EDOM_UMBRAL_DIAS -> bendicion crece linealmente
 *                                   (BENDICION_FACTOR por dia)
 *
 * La funcion SIEMPRE devuelve exito (HASHMAL_ARCA_OK) si los
 * parametros son validos - no hay "error" en hospedar poco tiempo,
 * simplemente no se ha ganado bendicion todavia.
 *
 * 2 testigos:
 *   2 Sam 6:11  - 3 meses de hospedaje -> bendicion a toda la casa
 *   2 Sam 6:12  - la bendicion se HACE SABER (vayugad lamelej)
 */
int hashmal_storage_hospedaje_reportar(uint32_t dias, uint32_t *out_bendicion)
{
    if (out_bendicion == NULL) {
        INTF_LOG_E("hospedaje: out_bendicion NULL");
        return HASHMAL_ARCA_ERR_NULL;
    }

    /* Precondicion: el Arca debe existir - no se hospeda el vacio. */
    const hashmal_arca_t *arca = hashmal_storage_get_arca();
    if (arca == NULL) {
        INTF_LOG_E("hospedaje: Arca no levantada — nada que hospedar");
        *out_bendicion = 0u;
        return HASHMAL_ARCA_ERR_INIT;
    }

    /* Cautiverio anula la bendicion: un nodo en manos ajenas NO
     * hospeda correctamente - el Arca esta alli, pero el diseno
     * ha sido violado (esto es la sombra de 1 Sam 5 dentro de
     * 2 Sam 6: no todo "tener el Arca" es bendicion). */
    if (s_en_cautiverio) {
        INTF_LOG_W("hospedaje: nodo en CAUTIVERIO — sin bendición "
                   "(1 Sam 5 sobre 2 Sam 6)");
        *out_bendicion = 0u;
        return HASHMAL_ARCA_OK;
    }

    if (dias < OBED_EDOM_UMBRAL_DIAS) {
        /* "shelosha jadashim" - tres meses. Antes del umbral, la
         * historia aun no ha sido contada. No hay ERROR, solo que
         * la bendicion todavia no ha madurado. */
        *out_bendicion = 0u;
        INTF_LOG_I("hospedaje: %" PRIu32 " días (<%u) — aún sin "
                   "testimonio (2 Sam 6:11 umbral)",
                   dias, OBED_EDOM_UMBRAL_DIAS);
        return HASHMAL_ARCA_OK;
    }

    /* dias >= umbral -> la bendicion crece. Formula base:
     *   bendicion = dias x BENDICION_FACTOR
     * uint32_t x 1u == uint32_t; sin overflow para dias < 2^32. */
    const uint32_t bendicion = dias * BENDICION_FACTOR;
    *out_bendicion = bendicion;

    INTF_LOG_I("═══ 'bérak YHWH et beit ʻOved-Edóm' (2 Sam 6:12) ═══");
    INTF_LOG_I("  hospedaje correcto: %" PRIu32 " días → bendición=%" PRIu32,
               dias, bendicion);
    INTF_LOG_I("  obediencia al protocolo → recompensa acumulada");
    return HASHMAL_ARCA_OK;
}

/* ==================================================================
 * API PUBLICA - dagon_caiga (1 Sam 5:3-4 + 1 Sam 5:6)
 * ================================================================== */

/**
 * hashmal_storage_dagon_caiga - autodefensa del dato capturado
 *
 * "vayiqju pelishtim et aron ha'Elohim vayevi'uhu beit Dagon
 *  vayatsigu oto etsel Dagon" - 1 Sam 5:2
 * "Tomaron los filisteos el Arca de Dios, la trajeron a la casa
 *  de Dagon y la pusieron junto a Dagon."
 *
 * "vayashkimu ashdodim mimmojarat vehinne Dagon nofe? 'al panav
 *  aretsah lifne aron YHWH vayiqju et Dagon vayashivu oto
 *  limqomo" - 1 Sam 5:3
 * "Se levantaron temprano los de Asdod y he aqui Dagon caido
 *  sobre su rostro en tierra delante del Arca de YHWH. Tomaron
 *  a Dagon y lo volvieron a su lugar."
 *
 * "vayashkimu baboqer mimmojarat vehinne Dagon nofe? 'al panav
 *  aretsah lifne aron YHWH verosh Dagon ushetei kapot yadav
 *  kerutot el hamiftan..." - 1 Sam 5:4
 * "Se levantaron de manana al siguiente dia y he aqui Dagon caido
 *  sobre su rostro en tierra delante del Arca de YHWH, y la cabeza
 *  de Dagon y las dos palmas de sus manos CORTADAS sobre el umbral..."
 *
 * "vatikbad yad YHWH el ha'ashdodim vayshimem vayaj otam
 *  ba'afolim et Ashdod ve'et gevuleha" - 1 Sam 5:6
 * "La mano de YHWH se agravo sobre los de Asdod y los asolo, y los
 *  hirio con tumores en Asdod y en sus terminos."
 *
 * --- QUE HACE ---
 *
 * Secuencia de autodefensa activada cuando se detecta exfiltracion.
 * El dato capturado debe ser INUTIL para el atacante - y, si es
 * posible, danino (cada intento empeora su posicion).
 *
 *   a) hashmal_oro_key_destruir()  - manos de Dagon cortadas:
 *                                    clave volatil y envelope a cero
 *   b) s_en_cautiverio = true      - el nodo SABE que fue capturado
 *   c) edut PERMANECE              - 1 Rey 8:9: "lo salvo shne lujot"
 *                                    (la Palabra no se destruye; solo
 *                                    la CAPACIDAD del atacante de leerla)
 *   d) log severo                  - "Dagon cayo ante el Arca de YHWH"
 *
 * Tras esto, el ciphertext que el atacante haya extraido es un
 * bloque inerte (no hay clave con la que abrirlo). Los procesos
 * que intenten cifrar/descifrar fallaran hasta re-provision.
 *
 * IDEMPOTENTE: si ya se cayo Dagon antes, llamarla de nuevo es
 * no-op (la clave ya no existe; el estado ya lo dice).
 *
 * 2 testigos:
 *   1 Sam 5:3-4 - Dagon cae DOS VECES; la segunda, mutilado
 *   1 Sam 5:6   - plagas sobre la ciudad que hospeda indebidamente
 */
int hashmal_storage_dagon_caiga(void)
{
    /* Ni siquiera necesitamos el Arca levantada para cortar manos:
     * si algo volatil hay, lo borramos. Pero si no hay nada que
     * proteger, avisamos y salimos en paz. */
    hashmal_arca_t *arca = hashmal_storage_arca_mut();

    if (s_en_cautiverio) {
        /* Ya caida. Idempotencia - el segundo golpe a Dagon (v.4)
         * ya fue ejecutado. Nada que ganar repitiendo. */
        INTF_LOG_W("dagon_caiga: nodo ya en cautiverio (idempotente, no-op)");
        return HASHMAL_ARCA_OK;
    }

    const bool habia_clave = hashmal_oro_key_lista();

    /* (a) manos de Dagon cortadas - destruir material de clave.
     * Llamamos siempre, incluso si habia_clave == false: cualquier
     * resto volatil (envelope, IV, tag) queda tambien limpio. */
    hashmal_oro_key_destruir();

    /* (b) marcar nodo "en cautiverio". El flag es la memoria del
     * evento: cuando pieza 13 (shemen haMishja) re-unja el nodo,
     * podra limpiar esta bandera - por ahora solo un boot limpio
     * la borra. */
    s_en_cautiverio = true;

    /* (c) si el Arca esta viva, degradamos el flag kavod: 1 Rey 8:10-11
     * dice que la gloria LLENA cuando todo esta conforme; en cautiverio
     * no esta conforme. La edut no se toca - 1 Rey 8:9. */
    if (arca != NULL) {
        arca->kavod_lleno = false;
        /* distribuida, replicas_confirmadas: NO los tocamos. Los peers
         * que hicieron replica legitima no estan en cautiverio por
         * asociacion - cada nodo defiende el suyo. */
    }

    /* (d) log severo - la escena del templo de Dagon. */
    INTF_LOG_E("═══ DAGÓN CAYÓ ANTE EL ARCA DE YHWH (1 Sam 5:3-4) ═══");
    if (habia_clave) {
        INTF_LOG_E("  'ushté kapót yadáv kerutót' — clave destruida");
    } else {
        INTF_LOG_E("  no había clave volátil — solo cerramos envelope");
    }
    INTF_LOG_E("  nodo en CAUTIVERIO: cifrar/descifrar rechazarán");
    INTF_LOG_E("  edut PERMANECE (1 Rey 8:9) — pero ilegible para el atacante");
    INTF_LOG_E("  plagas ascendentes si insiste (1 Sam 5:6)");

    return HASHMAL_ARCA_OK;
}
