/**
 * ===================================================================
 * PIEZA 06 - ALTAR DEL SACRIFICIO - altar_fuego.c
 * Sub-pieza: ESH TAMID - el fuego perpetuo
 * Lv 6:5-6 + Ex 29:38-39 + Lv 9:24 + 1 Re 18:38 + 2 Cr 7:1
 * ===================================================================
 *
 * "vehaESH 'al hammizbeaj TUQAD bo LO TIJBE uvi'ER 'aleha hakkohen
 *  'ETSIM baBBOQER baBBOQER..." - Lv 6:5
 * "El FUEGO sobre el altar ARDERA en el, NO SE APAGARA; el sacerdote
 *  encendera LENA sobre el cada MANANA por la manana..."
 *
 * "ESH TAMID tuqad 'al hammizbeaj LO TIJBE" - Lv 6:6
 * "FUEGO PERPETUO ardera sobre el altar, NO SE APAGARA."
 *
 * "vattetse ESH millifnei YHWH vattojal 'al hammizbeaj et ha'olah..."
 *   - Lv 9:24
 * "SALIO FUEGO de delante de YHWH y CONSUMIO sobre el altar el
 *  holocausto..."
 *
 * "vattippol ESH YHWH vattojal et ha'olah..." - 1 Re 18:38
 * "CAYO FUEGO de YHWH y consumio el holocausto..." (Elias, Carmelo)
 *
 * "vehaESH YARDA mehashshamayim vattojal ha'olah..." - 2 Cr 7:1
 * "El FUEGO DESCENDIO del cielo y consumio el holocausto..." (Salomon)
 *
 * "zeh asher ta'ase 'al hammizbeaj: kevasim shenei yom bayyom
 *  TAMID..." - Ex 29:38
 * "Esto haras sobre el altar: DOS corderos al dia, SIEMPRE..." (2/dia)
 *
 * --- VOCABULARIO HEBREO ---
 *
 *   esh          (ASh, H784)        FUEGO. Consume, verifica, acepta.
 *   tamid        (TMYD, H8548)    PERPETUO / siempre. Misma palabra
 *                                    que la ofrenda diaria y la menora.
 *                                    NUNCA se interrumpe.
 *   tuqad        (TWQD, H3344)     ARDERA (Hofal pasiva). El fuego
 *                                    ES ENCENDIDO - no se enciende solo.
 *   lo tijbe     (LA TKBH)       NO se apagara (H3808 + H3518).
 *                                    Doble enfasis: negacion + extinguir.
 *   uvi'er       (WB'R, H1197)    Encendera / anadira. El sacerdote
 *                                    mantiene, no crea. Disciplina.
 *   'etsim       ('TsYM, H6086)    LENA. Sin lena el fuego muere.
 *                                    Mantenimiento cada manana.
 *   babboqer     (BBQR, H1242)  MANANA. Ciclo matutino.
 *                                    Duplicado en Lv 6:5: cada manana.
 *   bein ha'arbayim (BYN H'RBYM, H6153) ENTRE LAS TARDES.
 *                                    Ciclo vespertino. Segunda verificacion.
 *   vattetse     (WTTsA, H3318)   SALIO. El fuego NO lo enciende el
 *                                    hombre - SALE de delante de YHWH.
 *   vattojal     (WTAKL, H398)   CONSUMIO. Verificacion total; la
 *                                    ofrenda ACEPTADA desaparece en la
 *                                    aceptacion.
 *   kevasim      (KBShYM, H3532) CORDEROS. Las 2 verificaciones diarias.
 *   shenei       (ShNY, H8147)     DOS. Ni 1 ni 3: exactamente 2.
 *
 * --- ESH TAMID = IDENTIDAD, NO FEATURE ---
 *
 * El fuego no es un "anadido" del altar: es lo que define al altar
 * como altar. Sin fuego, es solo una caja de acacia con bronce - no
 * funcion. Lv 6:6 lo dice por segunda vez (v.5 lo dice, v.6 lo
 * REPITE como mandato): "lo tijbe". El fuego nunca se apaga. Si se
 * apaga, el altar deja de ser altar.
 *
 * En firmware: es_tamid() consulta el singleton para responder "?el
 * fuego sigue ardiendo?". El flag `tamid` esta encendido desde init
 * (diseno), pero el fuego OPERATIVO requiere ademas fuego_encender
 * (que activa la reshet y los cuernos).
 *
 * --- TRES TESTIGOS DEL FUEGO CELESTIAL ---
 *
 *   Lv 9:24     - Consagracion del tabernaculo: vattetse esh millifnei YHWH
 *   1 Re 18:38  - Elias en el Carmelo: vattippol esh YHWH
 *   2 Cr 7:1    - Dedicacion del templo de Salomon: vehaesh yarda mehashshamayim
 *
 * Tres veces. El fuego BAJA de YHWH. El hombre no lo inventa -
 * solo lo mantiene (uvi'er... 'etsim babboqer). En firmware:
 * fuego_encender representa el momento celestial (el fuego
 * aparece); fuego_ciclo representa la disciplina del sacerdote
 * (mantener con lena dos veces al dia).
 *
 * --- QUE HACE ESTE ARCHIVO ---
 *
 *   fuego_encender   Lv 9:24 - el fuego baja (UNA VEZ, idempotente)
 *   fuego_ciclo      Ex 29:38-39 - 2/dia, manana + tarde, tamid
 *   fuego_estado     Lv 6:6 - lectura pura del estado
 *
 * --- DEPENDENCIAS ---
 *
 *   hashmal_auth.h                 API publica (incluye cuerno_activar)
 *   hashmal_auth_internal.h        altar_mut, get_altar_interno, ahora_ms
 *   esp_log.h (ORO EGIPTO)         AUTH_LOG_*
 *
 * Sin mbedtls, sin esp_random: el fuego no hace cripto (eso es
 * reshet). Solo administra su propia presencia.
 *
 * Autor: Antonio Jose Marin Soto
 * Proyecto: HASHMAL-CORE
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <inttypes.h>

#include "esp_log.h"

#include "hashmal_auth.h"
#include "hashmal_auth_internal.h"

/* ==================================================================
 * ENCAPSULACION ESP-IDF
 * ================================================================== */
static const char *AUTH_TAG = "hashmal.auth.fuego";

#define AUTH_LOG_I(fmt, ...)  ESP_LOGI(AUTH_TAG, fmt, ##__VA_ARGS__)
#define AUTH_LOG_W(fmt, ...)  ESP_LOGW(AUTH_TAG, fmt, ##__VA_ARGS__)
#define AUTH_LOG_E(fmt, ...)  ESP_LOGE(AUTH_TAG, fmt, ##__VA_ARGS__)

/* ==================================================================
 * ESTADO LOCAL - contador de ciclos para alternancia manana/tarde
 *
 * Ex 29:38-39 manda DOS corderos al dia: uno "babboqer" y otro
 * "bein ha'arbayim". Para alternar entre los dos en invocaciones
 * sucesivas de fuego_ciclo usamos un contador file-static -
 * mismo patron que altar_oro_oracion.c (s_ciclos_completados).
 *
 * El contador NO es "estado del altar" en sentido teologico (el
 * NEVUV del altar custodia datos sensibles); es contabilidad
 * operacional del servicio diario. Como los turnos de sacerdotes
 * (1 Cr 24): quien sirve manana y quien de tarde es una rotacion
 * administrativa, no parte del altar mismo.
 * ================================================================== */
static uint64_t s_ciclos_completados = 0ull;

/* ==================================================================
 * API PUBLICA - fuego_encender (Lv 9:24 + 1 Re 18:38 + 2 Cr 7:1)
 * ================================================================== */

/**
 * hashmal_auth_fuego_encender - vattetse esh millifnei YHWH
 *
 * El instante en que el fuego BAJA por primera vez. Tres testigos
 * biblicos lo narran (Lv 9:24, 1 Re 18:38, 2 Cr 7:1): el fuego no
 * es de hechura humana - es de YHWH. El hombre prepara la ofrenda;
 * YHWH envia el fuego.
 *
 * En firmware: acto unico de consagracion operativa del altar. El
 * gateway (altar_gateway.c) ya construyo la estructura; aqui se
 * "enciende" - se activan los cuernos (Lv 4:25 sangre sobre los
 * qarnotav), se activa la reshet, y queda listo para recibir
 * ofrendas (challenges + proofs).
 *
 * --- PRECONDICIONES ---
 *
 *   altar levantado                (estructura existe)
 *   estado == ENCENDIDO            (init ya paso; g_altar listo)
 *
 * --- EFECTOS ---
 *
 *   Para cada i en [0, 4): cuerno_activar(i)  (Lv 4:25)
 *   altar->reshet_activa = true               (v.4-5 opera)
 *   altar->ultimo_ciclo_ms = ahora_ms()       (primer "ciclo" = encendido)
 *
 * --- IDEMPOTENCIA ---
 *
 * Lv 9:24 ocurrio UNA VEZ. El fuego original baja una sola vez.
 * Despues el sacerdote MANTIENE (Lv 6:5 lena cada manana) pero
 * NO re-inicia el fuego. Aqui: si reshet_activa ya es true, el
 * fuego ya bajo. No re-encendemos.
 *
 * Detector de idempotencia: `reshet_activa` (inicializada a false
 * en gateway::init, solo puesta a true aqui). Si ya es true, el
 * fuego ya se encendio.
 *
 * 2 testigos:
 *   Lv 9:24 - vattetse esh millifnei YHWH (consagracion)
 *   2 Cr 7:1 - vehaesh yarda mehashshamayim (dedicacion del templo)
 *   [3er testigo: 1 Re 18:38 - Elias en Carmelo]
 */
int hashmal_auth_fuego_encender(void)
{
    hashmal_auth_t *a = hashmal_auth_altar_mut();
    if (a == NULL) {
        AUTH_LOG_E("fuego_encender: altar no levantado");
        return HASHMAL_AUTH_ERR_INIT;
    }

    if (a->estado != HASHMAL_AUTH_ENCENDIDO) {
        AUTH_LOG_E("fuego_encender: estado altar=%d ≠ ENCENDIDO — init incompleto",
                   (int)a->estado);
        return HASHMAL_AUTH_ERR_INIT;
    }

    /* Idempotencia - Lv 9:24 ocurrio una sola vez. */
    if (a->reshet_activa) {
        AUTH_LOG_I("fuego_encender: fuego ya encendido (Lv 9:24 no se repite); "
                   "sacerdote mantiene con ʻetsím (Lv 6:5)");
        return HASHMAL_AUTH_OK;
    }

    /* Activar los 4 cuernos - Lv 4:25 sangre sobre los qarnotav.
     * Degradacion parcial: si alguno falla, log pero continuar -
     * los cuernos que si se activaron ya operan. */
    uint8_t n_activados = 0u;
    for (uint8_t i = 0; i < HASHMAL_AUTH_CUERNOS; ++i) {
        int rc = hashmal_auth_cuerno_activar(i);
        if (rc == HASHMAL_AUTH_OK) {
            ++n_activados;
        } else {
            AUTH_LOG_E("fuego_encender: cuerno %u rechazó sangre (rc=%d) — "
                       "continuando con degradación parcial",
                       (unsigned)i, rc);
        }
    }

    /* La reshet se enciende CON el fuego (v.4-5 depende del servicio
     * operativo). A partir de aqui challenge/verificar operan. */
    a->reshet_activa = true;

    /* Timestamp del encendido = primer "ciclo" del altar. */
    a->ultimo_ciclo_ms = hashmal_auth_ahora_ms();

    /* Verificacion post-encendido: es_tamid() ahora DEBE retornar true
     * (tamid del diseno + ENCENDIDO + al menos 1 cuerno activo). */
    if (!hashmal_auth_es_tamid()) {
        AUTH_LOG_E("fuego_encender: es_tamid=false tras encendido — "
                   "estado inconsistente (Lv 6:6 violado)");
        /* No revertimos reshet_activa/cuernos: el dano esta hecho,
         * el operador debe investigar. Retornamos ERR_FUEGO para
         * que el llamante sepa. */
        return HASHMAL_AUTH_ERR_FUEGO;
    }

    AUTH_LOG_I("═══ FUEGO ENCENDIDO ═══ (Lv 9:24)");
    AUTH_LOG_I("  vattetsé ésh millifnéi YHWH vattójal ʻal hammizbéaj");
    AUTH_LOG_I("  cuernos activados: %u/%u (Lv 4:25 dám ʻal qarnót)",
               (unsigned)n_activados, HASHMAL_AUTH_CUERNOS);
    AUTH_LOG_I("  réshet activa (Éx 27:4 maʻasé réshet nejóshet)");
    AUTH_LOG_I("  tamíd lifnéi YHWH (Lv 6:6 lo tijbé)");
    return HASHMAL_AUTH_OK;
}

/* ==================================================================
 * API PUBLICA - fuego_ciclo (Ex 29:38-39 + Lv 6:5)
 * ================================================================== */

/**
 * hashmal_auth_fuego_ciclo - kevasim shenei yom bayyom tamid
 *
 * Ex 29:38-39 - DOS corderos al dia: uno babboqer (manana) y otro
 * bein ha'arbayim (entre las tardes). Lv 6:5 - el sacerdote anade
 * lena CADA MANANA. Dos ciclos diarios, sincronizados con el
 * servicio regular.
 *
 * En firmware: cada invocacion es UN ciclo completado. Alterna
 * manana/tarde por paridad - ciclo 0 = manana (primer cordero del
 * dia, v.39 "hakkeves ejad"), ciclo 1 = tarde (segundo cordero,
 * v.39 "hakkeves hashSheni"), ciclo 2 = manana del siguiente dia,
 * etc.
 *
 * --- PRECONDICIONES ---
 *
 *   altar levantado
 *   tamid (flag) activo
 *   reshet_activa (el fuego ya se encendio)
 *
 * Si el fuego no esta encendido -> ERR_FUEGO (Lv 6:6 violado -
 * no hay tamid, se apago). Es grave: el altar dejo de ser altar.
 *
 * --- EFECTOS ---
 *
 *   s_ciclos_completados += 1
 *   altar->ultimo_ciclo_ms = ahora_ms()
 *   (estado operacional del altar inalterado)
 *
 * 2 testigos:
 *   Ex 29:38 - kevasim shenei yom bayyom tamid (2 corderos, siempre)
 *   Lv 6:5   - uvi'er 'aleha 'etsim babboqer babboqer (lena cada manana)
 */
int hashmal_auth_fuego_ciclo(void)
{
    hashmal_auth_t *a = hashmal_auth_altar_mut();
    if (a == NULL) {
        AUTH_LOG_E("fuego_ciclo: altar no levantado");
        return HASHMAL_AUTH_ERR_INIT;
    }

    /* Lv 6:6 - "lo tijbe". Si el fuego esta apagado, no hay ciclo
     * que mantener. Esto es CONDICION GRAVE: el altar debio tener
     * fuego perpetuo desde fuego_encender. */
    if (!a->tamid || !a->reshet_activa) {
        AUTH_LOG_E("fuego_ciclo: fuego APAGADO — Lv 6:6 lo tijbé violado "
                   "(tamíd=%d, réshet=%d)",
                   (int)a->tamid, (int)a->reshet_activa);
        return HASHMAL_AUTH_ERR_FUEGO;
    }

    /* Alternancia manana/tarde por paridad del contador ANTES de
     * incrementar - ciclo 0 (first) = manana, consistente con Ex
     * 29:39 "hakkeves ejad" que se ofrece primero. */
    const bool es_manana = ((s_ciclos_completados & 1ull) == 0ull);
    s_ciclos_completados += 1ull;

    /* Timestamp del ciclo en el singleton (coexiste con el contador
     * file-static - el timestamp es "cuando" y el contador es
     * "cual numero de ciclo"). */
    const uint64_t ahora = hashmal_auth_ahora_ms();
    a->ultimo_ciclo_ms = ahora;

    if (es_manana) {
        AUTH_LOG_I("═══ ciclo BABBÓQER ═══ @%" PRIu64 " ms "
                   "(Éx 29:39 hakkéves ejád; Lv 6:5 ʻetsím); "
                   "total=%" PRIu64,
                   ahora, s_ciclos_completados);
    } else {
        AUTH_LOG_I("═══ ciclo BÉIN HAʻARBAYIM ═══ @%" PRIu64 " ms "
                   "(Éx 29:39 hakkéves hashShení; tamíd); "
                   "total=%" PRIu64,
                   ahora, s_ciclos_completados);
    }
    return HASHMAL_AUTH_OK;
}

/* ==================================================================
 * API PUBLICA - fuego_alimentar (P9 Verificado #11)
 *
 * "uvi'ér 'aleha hakkohén 'etsím BABBÓQER BABBÓQER" - Lv 6:5
 * "esh tamíd TUQAD ('al hammizbéaj LO TIJBÉH" - Lv 6:6
 *
 * tuqad (H3344) = Hofal pasivo-causativo: SERA HECHO arder. El
 * altar NO se enciende a si mismo - hashmal_power (Pieza 08) lo
 * alimenta. Esta funcion es la UNICA via legitima de mantenimiento;
 * fuego_ciclo se conserva para compatibilidad pero alimentar es
 * el nombre canonico que hace explicita la dependencia externa.
 *
 * INVARIANTE TAMID ONE-WAY (Lv 6:6 lo tijbéh):
 *   Una vez encendido (fuego_encender), el modulo NO PUEDE
 *   apagarse a si mismo. Ningun .c de hashmal_auth contiene
 *   asignacion `tamid = false`. La transicion solo viene por
 *   hashmal_auth_deinit (decision externa = beged argaman, Nm 4:13).
 *   "lo tijbéh" se respeta arquitectonicamente: el fuego no se
 *   apaga; se cubre.
 * ================================================================== */

/* nejóshet — interfaz bronce, trust Atrio (Éx 27:2) */
int hashmal_auth_fuego_alimentar(void)
{
    hashmal_auth_t *a = hashmal_auth_altar_mut();
    if (a == NULL) {
        AUTH_LOG_E("fuego_alimentar: altar no levantado");
        return HASHMAL_AUTH_ERR_INIT;
    }

    /* Lv 6:6 lo tijbéh - si el fuego se apago, es violacion grave
     * (ningun camino legitimo lo apaga). El alimentador externo
     * recibe ERR_FUEGO; debe disparar diagnostico, no re-encender. */
    if (!a->tamid || !a->reshet_activa) {
        AUTH_LOG_E("fuego_alimentar: fuego APAGADO (Lv 6:6 lo tijbéh "
                   "violado; tamíd=%d, réshet=%d) — alimentación rechazada",
                   (int)a->tamid, (int)a->reshet_activa);
        return HASHMAL_AUTH_ERR_FUEGO;
    }

    /* Hofal pasivo: causacion externa registrada. */
    a->fuego_alimentaciones += 1ull;
    a->ultimo_ciclo_ms = hashmal_auth_ahora_ms();

    AUTH_LOG_I("fuego alimentado (Hofal túqad, Lv 6:5 babóqer babóqer); "
               "alimentaciones=%" PRIu64,
               a->fuego_alimentaciones);
    return HASHMAL_AUTH_OK;
}

/* ==================================================================
 * API PUBLICA - fuego_estado (Lv 6:6 lectura pura, retorno int)
 * ================================================================== */

/**
 * hashmal_auth_fuego_estado - ?arde el fuego?
 *
 * Lectura pura del estado. Retorno binario segun la Palabra:
 *
 *   OK         si tamid Y reshet_activa (el fuego arde - Lv 6:6)
 *   ERR_FUEGO  en cualquier otro caso (se apago o no encendio)
 *
 * Unificamos "no init" y "apagado" bajo ERR_FUEGO: ambos significan
 * funcionalmente que el altar NO esta proveyendo servicio. La
 * causa especifica (falta de init vs fuego apagado) la puede
 * determinar el caller consultando otras APIs.
 *
 * Sin logs - como cuerno_activo() y reshet_estado(): lectura
 * limpia, evita ruido en polling.
 *
 * 2 testigos:
 *   Lv 6:6  - esh tamid... lo tijbe (criterio binario)
 *   Lv 6:5  - tuqad bo (condicion activa: "ardiendo en el")
 */
int hashmal_auth_fuego_estado(void)
{
    const hashmal_auth_t *a = hashmal_auth_get_altar_interno();
    if (a == NULL)              return HASHMAL_AUTH_ERR_FUEGO;
    if (!a->tamid)              return HASHMAL_AUTH_ERR_FUEGO;
    if (!a->reshet_activa)      return HASHMAL_AUTH_ERR_FUEGO;
    return HASHMAL_AUTH_OK;
}

/* ==================================================================
 * API PUBLICA - fuego_activo (Lv 6:6 lectura pura, retorno bool)
 *
 * "ESH TAMID tuqad 'al hammizbeaj LO TIJBE" — Lv 6:6
 * "FUEGO PERPETUO ardera... NO SE APAGARA."
 *
 * Variante bool de fuego_estado() — misma logica, tipo de retorno
 * distinto. El header declara ambas porque algunos callers (como
 * hashmal_intercession, Pieza 11) necesitan un bool directo para
 * guardias de comprobacion rapida:
 *
 *   if (!hashmal_auth_fuego_activo()) return ERR_FUEGO_AUSENTE;
 *
 * El Altar del Incienso (Pieza 11) no puede operar si el fuego
 * del Altar del Sacrificio (Pieza 06) no arde: Lv 16:12-13
 * manda que las brasas del altar enciendan el incienso del Kippur.
 * Sin fuego en el altar de abajo, el incienso de arriba no sube.
 *
 * Sin logs — lectura pura, sin ruido en polling frecuente.
 *
 * 2 testigos:
 *   Lv 6:6   — esh tamid lo tijbe (criterio de actividad)
 *   Lv 16:12 — velaqah meló hammajtah geshalim esh meʻal hammizbéaj
 *              (las brasas VIENEN del altar — requiere fuego activo)
 * ================================================================== */
bool hashmal_auth_fuego_activo(void)
{
    return (hashmal_auth_fuego_estado() == HASHMAL_AUTH_OK);
}

/* ==================================================================
 *
 *   "ESH TAMID tuqad 'al hammizbeaj LO TIJBE." - Levitico 6:6
 *
 *   "vattetse ESH millifnei YHWH vattojal 'al hammizbeaj et
 *    ha'olah..." - Levitico 9:24
 *
 *   "zeh asher ta'ase 'al hammizbeaj: kevasim shenei yom bayyom
 *    TAMID." - Exodo 29:38
 *
 *   El fuego BAJA de YHWH (Lv 9:24). El sacerdote lo mantiene con
 *   lena cada manana (Lv 6:5). Pero NUNCA lo enciende el - solo
 *   lo alimenta.
 *
 *   En firmware:
 *     fuego_encender  = el momento celestial (Lv 9:24, una vez)
 *     fuego_ciclo     = la disciplina del sacerdote (2/dia)
 *     fuego_estado    = la lectura del "lo tijbe"
 *
 *   4 de 6 archivos del Altar del Sacrificio -
 *   altar_fuego.c listo. Quedan:
 *     altar_utensilios.c   Ex 27:3 - los 5 kelim (sirot, ya'im,
 *                                    mizraqot, mizlegot, majtot)
 *     altar_ledger.c       Ap 6:9  - las almas bajo el altar
 *
 * ================================================================== */
