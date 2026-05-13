/**
 * ===================================================================
 * PIEZA 07 — EL ATRIO — atrio_http_portal.c
 * Añadidura 22: SHA'AR HA'ATRIO — Servidor HTTP + Portal HTML5
 * Éxodo 27:16 + Salmo 100:4 + Juan 10:9 + Isaías 60:11
 * ===================================================================
 *
 * "uleSHA'AR HEJATSER MASAKH 'esrím ammáh tekhélet veargamán
 *  vetola'at shaní veshésh mashzár ma'aséh ROQÉM"
 *   — Éxodo 27:16
 * "Para la PUERTA del atrio, cortina de 20 codos: azul, púrpura,
 *  carmesí y lino torcido, obra de BORDADOR."
 *
 * roqém (H7551) = el artesano que TEJE COLORES. A diferencia del
 * velo (josév — teje querubines), el portal es bordado con arte:
 * tekhélet (azul), argamán (púrpura), tola'at shaní (carmesí),
 * shésh (lino blanco). Esta es la interfaz HTML: 4 colores = 4 hilos.
 *
 * "bo'ú she'aráv betodáh jatserotáv bitehilláh" — Salmo 100:4
 * "Entrad por sus PUERTAS con ACCIÓN DE GRACIAS, por sus ATRIOS
 *  con ALABANZA."
 * El que llega con todá (H8426) y tehilláh (H8416) es bienvenido.
 * La interfaz debe ser digna de recibir con ese espíritu.
 *
 * "ego eimi he THYRA; di' emou ean tis EISELTHE SOTHESETAI" — Jn 10:9
 * "Yo soy la PUERTA; el que por Mí ENTRARE, será SALVO."
 * Una sola interfaz, un solo punto de acceso, todo pasa por aquí.
 *
 * "ufittjáu she'arayích TAMID yomam valayláh ló yissagéru" — Is 60:11
 * "Tus puertas estarán ABIERTAS CONTINUAMENTE, día y noche."
 * El servidor HTTP corre tamíd — nunca se cierra durante operación.
 *
 * --- RUTAS ---
 *
 *  GET  /                      → Portal HTML5 completo (SPA)
 *  GET  /api/mensajes          → JSON: mensajes LoRa recibidos
 *  POST /api/mensajes          → Enviar texto por LoRa
 *  GET  /api/estado            → JSON: estado de la red
 *
 *  Detección de portal cautivo (redirección automática del SO):
 *  GET  /generate_204          → 204 No Content (Android / Chrome)
 *  GET  /hotspot-detect.html   → 200 "Success" (iOS / macOS)
 *  GET  /success.html          → 200 "Success" (iOS legacy)
 *  GET  /library/test/success.html → 200 "Success" (iOS CDN path)
 *  GET  /connecttest.txt       → 200 texto plano (Windows)
 *  GET  /ncsi.txt              → 200 texto plano (Windows 10+)
 *  GET  /captive.txt           → 200 texto plano (Linux)
 *
 * 2 testigos:
 *   Éx 27:16 — roqém: la interfaz es obra de bordador (bella, no tosca)
 *   Is 60:11 — tamíd: el servidor corre perpetuamente
 *
 * Autor: Antonio José Marín Soto — Hashmal-Core
 */

#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "esp_log.h"
#include "esp_http_server.h"
#include "esp_spiffs.h"          /* Éx 25:16 — Pondrás el Testimonio en el Arca */

#include "hashmal_perimeter.h"

/* ===================================================================
 * ENCAPSULACIÓN ESP-IDF
 * =================================================================== */
static const char *HTTP_TAG = "hashmal.perimeter.http";
#define HTTP_LOG_I(fmt, ...)  ESP_LOGI(HTTP_TAG, fmt, ##__VA_ARGS__)
#define HTTP_LOG_W(fmt, ...)  ESP_LOGW(HTTP_TAG, fmt, ##__VA_ARGS__)
#define HTTP_LOG_E(fmt, ...)  ESP_LOGE(HTTP_TAG, fmt, ##__VA_ARGS__)

/* ===================================================================
 * CONSTANTES
 * =================================================================== */
#define HTTP_POST_MAX    256u    /* máximo bytes en POST body */
#define HTTP_JSON_BUF    2048u   /* buffer para JSON de mensajes */
#define HTTP_STAT_BUF    256u    /* buffer para JSON de estado */
#define DEVAR_IDX_BUF    4096u   /* buffer para index.json de la Palabra */
#define DEVAR_CAP_BUF    32768u  /* buffer para capítulo .gz (max ~28KB — Sal 119 es el más largo) */

/* ===================================================================
 * ESTADO SINGLETON
 * =================================================================== */
static httpd_handle_t g_server       = NULL;
static bool           g_devar_mounted = false;

/* ===================================================================
 * PORTAL HTML5 — obra de bordador (Éx 27:16 roqém)
 *
 * tekhélet = #4A90D9 (azul — Nm 15:38, hilo de reminiscencia)
 * argamán  = #9B59B6 (púrpura — Ex 25:4, realeza)
 * tola'at  = #C0392B (carmesí — Lv 14:4, sacrificio/redención)
 * shésh    = #F5F0E8 (lino — Ex 27:9, pureza visible)
 *
 * 4 colores bordados = 4 columnas de la puerta (Éx 27:16).
 * =================================================================== */

/* ===================================================================
 * PORTAL HTML5 — 9 módulos (Añadidura 22 — SHA'AR HA'ATRIO)
 * Éx 27:16 — roqém: obra de bordador con 4 hilos
 * 9 módulos: Mensajes, Devár, Moedím, Comunidad, Mapa,
 *            Cámara, Notas, Archivos, Ajustes
 * =================================================================== */
static const char PORTAL_HTML[] =
    "<!DOCTYPE html>\r\n"
    "<html lang=\"es\">\r\n"
    "<head>\r\n"
    "<meta charset=\"UTF-8\">\r\n"
    "<meta name=\"viewport\" content=\"width=device-width,initial-scale=1\">\r\n"
    "<title>HASHMAL</title><style>\r\n"
    /* 4 hilos de Éx 27:16 + colores base */
    ":root{--tk:#4A90D9;--ag:#9B59B6;--to:#C0392B;--sh:#F5F0E8;"
          "--bg:#0D1117;--bg2:#161B22;--bg3:#21262D;"
          "--tx:#E6EDF3;--dim:#8B949E;--bd:#30363D;--ok:#2EA043}\r\n"
    "*{box-sizing:border-box;margin:0;padding:0}\r\n"
    "body{background:var(--bg);color:var(--tx);"
         "font-family:system-ui,sans-serif;min-height:100vh;padding-bottom:62px}\r\n"
    /* Header */
    "header{background:var(--bg2);border-bottom:2px solid var(--tk);"
           "padding:10px 16px;display:flex;align-items:center;gap:10px}\r\n"
    ".logo{font-size:20px;font-weight:800;letter-spacing:3px;color:var(--sh)}\r\n"
    ".logo em{color:var(--tk);font-style:normal}\r\n"
    ".heb{font-size:11px;color:var(--ag)}\r\n"
    ".dot{width:8px;height:8px;border-radius:50%;background:var(--ok);"
         "margin-left:auto;box-shadow:0 0 6px var(--ok)}\r\n"
    /* Nav inferior — 9 módulos */
    "nav{position:fixed;bottom:0;left:0;right:0;background:var(--bg2);"
        "border-top:1px solid var(--bd);display:flex;z-index:100;"
        "height:60px;overflow-x:auto;-webkit-overflow-scrolling:touch}\r\n"
    ".nt{flex:0 0 auto;min-width:64px;display:flex;flex-direction:column;"
        "align-items:center;justify-content:center;gap:1px;"
        "background:none;border:none;color:var(--dim);font-size:9px;"
        "cursor:pointer;padding:4px 2px}\r\n"
    ".nt.on{color:var(--tk)}\r\n"
    ".ic{font-size:17px;line-height:1}\r\n"
    /* Paneles */
    ".pnl{display:none;padding:14px;min-height:80vh}\r\n"
    ".pnl.on{display:block}\r\n"
    /* Mensajes */
    ".mlist{max-height:calc(100vh - 210px);overflow-y:auto}\r\n"
    ".msg{background:var(--bg2);border-left:3px solid var(--tk);"
         "margin:4px 0;padding:8px 10px;border-radius:0 5px 5px 0}\r\n"
    ".msg.tx{border-left-color:var(--ag)}\r\n"
    ".mmeta{font-size:10px;color:var(--dim);margin-bottom:3px}\r\n"
    ".mtxt{font-size:13px;line-height:1.4}\r\n"
    ".empty{color:var(--dim);font-size:13px;padding:20px;text-align:center;line-height:1.8}\r\n"
    ".mform{display:flex;gap:8px;margin-top:10px}\r\n"
    ".minp{flex:1;background:var(--bg3);border:1px solid var(--bd);"
          "color:var(--tx);padding:9px 11px;border-radius:5px;font-size:14px;outline:none}\r\n"
    ".minp:focus{border-color:var(--tk)}\r\n"
    ".mbtn{background:var(--tk);color:#fff;border:none;"
          "padding:9px 18px;border-radius:5px;font-size:14px;font-weight:600;cursor:pointer}\r\n"
    /* Cards generales */
    ".card{background:var(--bg2);border:1px solid var(--bd);border-radius:8px;padding:12px;margin:6px 0}\r\n"
    ".g2{display:grid;grid-template-columns:1fr 1fr;gap:8px;margin-bottom:10px}\r\n"
    ".cval{font-size:28px;font-weight:700;color:var(--tk);text-align:center}\r\n"
    ".cval.ag{color:var(--ag)}\r\n"
    ".clbl{font-size:11px;color:var(--dim);margin-top:4px;text-align:center}\r\n"
    ".badge{display:inline-block;padding:3px 8px;border-radius:12px;"
           "font-size:11px;background:var(--bg3);border:1px solid var(--bd);margin:3px}\r\n"
    ".badge.ok{border-color:var(--ok);color:var(--ok)}\r\n"
    /* Versículos */
    ".vheb{font-size:15px;direction:rtl;text-align:right;color:var(--sh);line-height:1.8;margin-bottom:8px}\r\n"
    ".ves{font-size:12px;color:var(--tx);line-height:1.5;margin-bottom:5px}\r\n"
    ".vref{font-size:11px;color:var(--ag);font-weight:600}\r\n"
    /* Moedim */
    ".mo{display:flex;align-items:center;gap:10px}\r\n"
    ".mo-ic{font-size:22px;width:32px;text-align:center;flex-shrink:0}\r\n"
    ".mo-b{padding:3px 8px;border-radius:10px;font-size:10px;font-weight:600;white-space:nowrap;flex-shrink:0}\r\n"
    ".mo-b.sha{background:var(--ok);color:#fff}\r\n"
    ".mo-b.prx{background:var(--tk);color:#fff}\r\n"
    ".mo-b.pas{background:var(--bg3);color:var(--dim)}\r\n"
    /* Nodos */
    ".nrow{display:flex;align-items:center;gap:10px}\r\n"
    ".ndot{width:10px;height:10px;border-radius:50%;flex-shrink:0}\r\n"
    ".ndot.on{background:var(--ok);box-shadow:0 0 5px var(--ok)}\r\n"
    ".ndot.off{background:var(--dim)}\r\n"
    /* Notas */
    ".nota{width:100%;min-height:calc(100vh - 230px);background:var(--bg2);"
          "border:1px solid var(--bd);color:var(--tx);padding:12px;"
          "border-radius:8px;font-size:14px;font-family:system-ui,sans-serif;"
          "resize:none;outline:none;line-height:1.6}\r\n"
    ".nota:focus{border-color:var(--tk)}\r\n"
    /* Ajustes */
    ".aj{display:flex;align-items:center;justify-content:space-between;"
        "padding:7px 0;border-bottom:1px solid var(--bd)}\r\n"
    ".aj:last-child{border-bottom:none}\r\n"
    ".ajl{font-size:13px;color:var(--dim)}\r\n"
    ".ajv{font-size:13px;font-weight:600}\r\n"
    ".ajinp{background:var(--bg3);border:1px solid var(--bd);color:var(--tx);"
           "padding:5px 8px;border-radius:5px;font-size:13px;width:120px;"
           "text-align:right;outline:none}\r\n"
    ".ajinp:focus{border-color:var(--tk)}\r\n"
    /* Sección header */
    ".sh{font-size:11px;color:var(--ag);font-weight:600;letter-spacing:1px;"
        "text-transform:uppercase;margin:4px 0 8px}\r\n"
    /* Placeholder */
    ".ph{text-align:center;padding:36px 20px}\r\n"
    ".ph-ic{font-size:52px;opacity:.35;margin-bottom:12px}\r\n"
    ".ph-t{font-size:16px;font-weight:600;color:var(--tk);margin-bottom:8px}\r\n"
    ".ph-s{font-size:13px;color:var(--dim);line-height:1.7}\r\n"
    /* Botón guardar */
    ".sv{width:100%;margin-top:10px;padding:10px;background:var(--tk);color:#fff;"
        "border:none;border-radius:6px;font-size:14px;font-weight:600;cursor:pointer}\r\n"
    /* Devár — versículos renderizados */
    ".vv{margin-bottom:10px;padding-bottom:8px;border-bottom:1px solid var(--bd)}\r\n"
    ".vn{font-size:10px;font-weight:700;color:var(--ag);float:left;margin-top:4px;margin-right:6px;min-width:14px;text-align:center}\r\n"
    ".vht{font-size:15px;direction:rtl;text-align:right;line-height:1.9;color:var(--sh);display:block;clear:right}\r\n"
    ".vdf{font-size:11px;color:var(--dim);padding:1px 0 1px 8px;margin:1px 0;line-height:1.5}\r\n"
    ".vdf b{color:var(--tk)}\r\n"
    "</style></head>\r\n"
    "<body>\r\n"
    "<header>\r\n"
    "  <div><div class=\"logo\">HASH<em>MAL</em></div>\r\n"
    "  <div class=\"heb\">\xd7\x94\xd7\xa9\xd7\x9e\xd7\x9c \xe2\x80\x94 Red Soberana</div></div>\r\n"
    "  <div class=\"dot\" id=\"dot\"></div>\r\n"
    "</header>\r\n"
    /* 9 paneles vacíos — JS los construye lazy */
    "<div id=\"pm\" class=\"pnl on\"></div>\r\n"
    "<div id=\"pd\" class=\"pnl\"></div>\r\n"
    "<div id=\"po\" class=\"pnl\"></div>\r\n"
    "<div id=\"pc\" class=\"pnl\"></div>\r\n"
    "<div id=\"pp\" class=\"pnl\"></div>\r\n"
    "<div id=\"pk\" class=\"pnl\"></div>\r\n"
    "<div id=\"pn\" class=\"pnl\"></div>\r\n"
    "<div id=\"pf\" class=\"pnl\"></div>\r\n"
    "<div id=\"pa\" class=\"pnl\"></div>\r\n"
    /* Nav inferior */
    "<nav>\r\n"
    "<button class=\"nt on\" onclick=\"sw('m',this)\"><span class=\"ic\">\xe2\x9c\x89</span>Mensajes</button>\r\n"
    "<button class=\"nt\" onclick=\"sw('d',this)\"><span class=\"ic\">\xf0\x9f\x93\x96</span>Dev\xc3\xa1r</button>\r\n"
    "<button class=\"nt\" onclick=\"sw('o',this)\"><span class=\"ic\">\xf0\x9f\x93\x85</span>Moed\xc3\xadm</button>\r\n"
    "<button class=\"nt\" onclick=\"sw('c',this)\"><span class=\"ic\">\xf0\x9f\x91\xa5</span>Comunidad</button>\r\n"
    "<button class=\"nt\" onclick=\"sw('p',this)\"><span class=\"ic\">\xf0\x9f\x97\xba</span>Mapa</button>\r\n"
    "<button class=\"nt\" onclick=\"sw('k',this)\"><span class=\"ic\">\xf0\x9f\x93\xb7</span>C\xc3\xa1mara</button>\r\n"
    "<button class=\"nt\" onclick=\"sw('n',this)\"><span class=\"ic\">\xf0\x9f\x93\x9d</span>Notas</button>\r\n"
    "<button class=\"nt\" onclick=\"sw('f',this)\"><span class=\"ic\">\xf0\x9f\x93\x81</span>Archivos</button>\r\n"
    "<button class=\"nt\" onclick=\"sw('a',this)\"><span class=\"ic\">\xe2\x9a\x99</span>Ajustes</button>\r\n"
    "</nav>\r\n"

    "<script>\r\n"
    /* ── NAV ── */
    "function sw(id,btn){\r\n"
    "  document.querySelectorAll('.pnl').forEach(p=>p.classList.remove('on'));\r\n"
    "  document.querySelectorAll('.nt').forEach(b=>b.classList.remove('on'));\r\n"
    "  document.getElementById('p'+id).classList.add('on');\r\n"
    "  btn.classList.add('on');\r\n"
    "  if(id==='m')loadMsgs();\r\n"
    "  else if(id==='d')bldD();\r\n"
    "  else if(id==='o')bldO();\r\n"
    "  else if(id==='c')loadC();\r\n"
    "  else if(id==='p')bldP();\r\n"
    "  else if(id==='k')bldK();\r\n"
    "  else if(id==='n')bldN();\r\n"
    "  else if(id==='f')bldF();\r\n"
    "  else if(id==='a')loadA();\r\n"
    "}\r\n"
    /* ── UTILS ── */
    "function fmt(ms){var d=new Date(ms);"
    "return d.getHours().toString().padStart(2,'0')+':'+d.getMinutes().toString().padStart(2,'0');}\r\n"
    "function esc(s){return s.replace(/&/g,'&amp;').replace(/</g,'&lt;').replace(/>/g,'&gt;');}\r\n"
    "function H(t){return '<h3 class=\"sh\">'+t+'</h3>';}\r\n"
    /* ── MENSAJES ── */
    "var _mc=0;\r\n"
    "function loadMsgs(){\r\n"
    "  fetch('/api/mensajes').then(r=>r.json()).then(d=>{\r\n"
    "    var el=document.getElementById('ml');\r\n"
    "    if(!el)return;\r\n"
    "    if(!d.msgs||!d.msgs.length){el.innerHTML='<div class=\"empty\">\xf0\x9f\x95\x8a Sin mensajes a\xc3\xban...</div>';return;}\r\n"
    "    if(d.msgs.length!==_mc){\r\n"
    "      _mc=d.msgs.length;\r\n"
    "      var bot=(el.scrollTop+el.clientHeight>=el.scrollHeight-20);\r\n"
    "      el.innerHTML=d.msgs.map(m=>{\r\n"
    "        var tx=(m.tipo===240);\r\n"
    "        var meta=tx?'\xd7\x90\xd7\xa0\xd7\x99 \xc2\xb7 '+fmt(m.t):'Nodo '+m.o+' \xc2\xb7 '+fmt(m.t);\r\n"
    "        return '<div class=\"msg'+(tx?' tx':'')+'\"><div class=\"mmeta\">'+meta+'</div><div class=\"mtxt\">'+esc(m.d)+'</div></div>';\r\n"
    "      }).join('');\r\n"
    "      if(bot)el.scrollTop=el.scrollHeight;\r\n"
    "    }\r\n"
    "  }).catch(function(){});\r\n"
    "}\r\n"
    "function snd(){\r\n"
    "  var inp=document.getElementById('mi'),txt=inp.value.trim();\r\n"
    "  if(!txt)return;\r\n"
    "  inp.disabled=true;\r\n"
    "  fetch('/api/mensajes',{method:'POST',headers:{'Content-Type':'text/plain'},body:txt})\r\n"
    "    .then(r=>{inp.disabled=false;if(r.ok){inp.value='';setTimeout(loadMsgs,400);}})\r\n"
    "    .catch(function(){inp.disabled=false;});\r\n"
    "}\r\n"
    "(function(){\r\n"
    "  var p=document.getElementById('pm');\r\n"
    "  p.innerHTML='<div class=\"mlist\" id=\"ml\"><div class=\"empty\">Cargando...</div></div>'\r\n"
    "    +'<div class=\"mform\"><input class=\"minp\" id=\"mi\" type=\"text\"'\r\n"
    "    +' placeholder=\"Mensaje a la red LoRa...\" maxlength=\"200\" autocomplete=\"off\">'\r\n"
    "    +'<button class=\"mbtn\" onclick=\"snd()\">\xe2\x86\x91</button></div>';\r\n"
    "  document.getElementById('mi').addEventListener('keydown',function(e){if(e.key==='Enter')snd();});\r\n"
    "})();\r\n"
    /* ── DEVÁR YHWH — Lector 66 libros, 1189 capítulos
     * Éx 25:16 — "Pondrás en el Arca el Testimonio que YO te daré"
     * Dt  31:26 — "AL LADO del Arca": la Torá completa junto al núcleo
     * Partición devar (SPIFFS 8MB): /api/devar/index y /api/devar/cap       */
    "var DV={idx:null,cb:null};\r\n"
    "function bldD(){\r\n"
    "  var p=document.getElementById('pd');if(p.dataset.b)return;p.dataset.b='1';\r\n"
    "  p.innerHTML=H('\xf0\x9f\x93\x96 DEV\xc3\x81R YHWH \xe2\x80\x94 La Palabra del Padre')\r\n"
    "    +'<select id=\"dvb\" onchange=\"dvLib(this.value)\" style=\"width:100%;padding:8px;margin-bottom:8px;background:var(--bg2);color:var(--fg);border:1px solid var(--brd);border-radius:6px\"><option value=\"\">Cargando libros...</option></select>'\r\n"
    "    +'<select id=\"dvc\" onchange=\"dvCap(this.value)\" style=\"width:100%;padding:8px;margin-bottom:8px;background:var(--bg2);color:var(--fg);border:1px solid var(--brd);border-radius:6px\"><option value=\"\">\xe2\x80\x94 Selecciona cap\xc3\xadtulo \xe2\x80\x94</option></select>'\r\n"
    "    +'<div id=\"dvtxt\" style=\"font-size:12px;line-height:1.8;padding:4px 0\"></div>';\r\n"
    "  fetch('/api/devar/index').then(function(r){return r.json();}).then(function(d){\r\n"
    "    DV.idx=d;\r\n"
    "    var s=document.getElementById('dvb');\r\n"
    "    s.innerHTML='<option value=\"\">\xe2\x80\x94 Selecciona libro \xe2\x80\x94</option>'\r\n"
    "      +d.libros.map(function(l){\r\n"
    "        var n=String(l.num).padStart(2,'0');\r\n"
    "        return '<option value=\"'+n+'\">'+(l.nombre)+'</option>';\r\n"
    "      }).join('');\r\n"
    "  }).catch(function(){\r\n"
    "    document.getElementById('dvtxt').innerHTML='<div class=\"empty\">La Palabra se est\xc3\xa1 cargando...</div>';\r\n"
    "  });}\r\n"
    "function dvLib(b){\r\n"
    "  DV.cb=b;\r\n"
    "  document.getElementById('dvtxt').innerHTML='';\r\n"
    "  var s=document.getElementById('dvc');\r\n"
    "  if(!b||!DV.idx){s.innerHTML='<option value=\"\">\xe2\x80\x94 Selecciona cap\xc3\xadtulo \xe2\x80\x94</option>';return;}\r\n"
    "  var lib=DV.idx.libros.find(function(l){return String(l.num).padStart(2,'0')===b;});\r\n"
    "  if(!lib)return;\r\n"
    "  var o='<option value=\"\">\xe2\x80\x94 Selecciona cap\xc3\xadtulo \xe2\x80\x94</option>';\r\n"
    "  for(var i=1;i<=lib.caps;i++){o+='<option value=\"'+String(i).padStart(3,'0')+'\">Cap\xc3\xadtulo '+i+'</option>';}\r\n"
    "  s.innerHTML=o;}\r\n"
    /* mdToHtml: convierte el formato Markdown del vault → HTML limpio
     * Formato de los archivos .gz:
     *   **N** hebreo del versículo...
     *   > **palabra** — definición
     *   línea vacía entre versículos */
    "function mdToHtml(t){\r\n"
    "  var out='',cur=null;\r\n"
    "  var lines=t.split('\\n');\r\n"
    "  for(var i=0;i<lines.length;i++){\r\n"
    "    var l=lines[i].rTrim?lines[i].rTrim():lines[i].replace(/\\s+$/,'');\r\n"
    "    var vm=l.match(/^\\*\\*(\\d+)\\*\\*\\s*(.*)$/);\r\n"
    "    if(vm){\r\n"
    "      if(cur)out+=cur+'</div>';\r\n"
    "      cur='<div class=\"vv\"><span class=\"vn\">'+vm[1]+'</span>'\r\n"
    "         +'<span class=\"vht\">'+vm[2]+'</span>';\r\n"
    "    } else if(l.indexOf('> ')===0){\r\n"
    "      var df=l.slice(2).replace(/\\*\\*([^*]+)\\*\\*/g,'<b>$1</b>');\r\n"
    "      if(cur)cur+='<div class=\"vdf\">'+df+'</div>';\r\n"
    "    } else if(l.trim()===''&&cur){\r\n"
    "      out+=cur+'</div>';cur=null;\r\n"
    "    }\r\n"
    "  }\r\n"
    "  if(cur)out+=cur+'</div>';\r\n"
    "  return out||'<div class=\"empty\">Sin contenido</div>';}\r\n"
    "function dvCap(c){\r\n"
    "  if(!c||!DV.cb)return;\r\n"
    "  var p=document.getElementById('dvtxt');\r\n"
    "  p.innerHTML='<div class=\"empty\">Cargando...</div>';\r\n"
    "  fetch('/api/devar/cap?b='+DV.cb+'&c='+c)\r\n"
    "    .then(function(r){return r.text();})\r\n"
    "    .then(function(t){p.innerHTML=mdToHtml(t);})\r\n"
    "    .catch(function(){p.innerHTML='<div class=\"empty\">Error al cargar</div>';});}\r\n"
    /* ── MOEDÍM ── */
    "function nxtSha(){var d=new Date(),dy=d.getDay(),dif=dy===5?0:dy===6?6:(5-dy+7)%7;\r\n"
    "  if(dif===0&&d.getHours()>=18)dif=7;\r\n"
    "  d.setDate(d.getDate()+dif);return 'Viernes '+d.getDate()+'/'+(d.getMonth()+1)+' al anochecer';}\r\n"
    "function isPast(y,m,dy){return new Date()>new Date(y,m-1,dy);}\r\n"
    "function bldO(){var p=document.getElementById('po');if(p.dataset.b)return;p.dataset.b='1';\r\n"
    "  var y=new Date().getFullYear();\r\n"
    "  var MM=[\r\n"
    "   {i:'\xf0\x9f\x95\xaf',n:'Shabat',h:'\xd7\xa9\xd7\x91\xd7\xaa',d:nxtSha(),t:'sha'},\r\n"
    "   {i:'\xf0\x9f\x8d\x87',n:'P\xc3\xa9saj',h:'\xd7\xa4\xd7\xa1\xd7\x97',d:'14 Nis\xc3\xa1n \xc2\xb7 Lv 23:5',t:isPast(y,4,14)?'pas':'prx'},\r\n"
    "   {i:'\xf0\x9f\x8c\xbe',n:'Shavuot',h:'\xd7\xa9\xd7\x91\xd7\x95\xd7\xa2\xd7\x95\xd7\xaa',d:'6 Siw\xc3\xa1n \xc2\xb7 Lv 23:15',t:isPast(y,6,12)?'pas':'prx'},\r\n"
    "   {i:'\xf0\x9f\x8e\xba',n:'Yom Teru\xc3\xa1',h:'\xd7\x99\xd7\x95\xd7\x9d \xd7\xaa\xd7\xa8\xd7\x95\xd7\xa2\xd7\x94',d:'1 Tishrei \xc2\xb7 Lv 23:24',t:isPast(y,10,3)?'pas':'prx'},\r\n"
    "   {i:'\xf0\x9f\x99\x8f',n:'Yom Kippur',h:'\xd7\x99\xd7\x95\xd7\x9d \xd7\x9b\xd7\xa4\xd7\x95\xd7\xa8',d:'10 Tishrei \xc2\xb7 Lv 23:27',t:isPast(y,10,12)?'pas':'prx'},\r\n"
    "   {i:'\xf0\x9f\x8c\xbf',n:'Sukkot',h:'\xd7\xa1\xd7\x95\xd7\x9b\xd7\x95\xd7\xaa',d:'15 Tishrei \xc2\xb7 Lv 23:34',t:isPast(y,10,17)?'pas':'prx'}\r\n"
    "  ];\r\n"
    "  p.innerHTML=H('\xf0\x9f\x93\x85 MOED\xc3\x8dM \xe2\x80\x94 Las Fiestas de YHWH (Lv 23)')\r\n"
    "    +MM.map(m=>'<div class=\"card\"><div class=\"mo\"><div class=\"mo-ic\">'+m.i+'</div>'\r\n"
    "      +'<div style=\"flex:1\"><div style=\"font-weight:600;font-size:13px\">'+m.n+' <span style=\"color:var(--dim);font-size:11px\">'+m.h+'</span></div>'\r\n"
    "      +'<div style=\"font-size:11px;color:var(--dim);margin-top:2px\">'+m.d+'</div></div>'\r\n"
    "      +'<span class=\"mo-b '+m.t+'\">'+(m.t==='sha'?'Semanal':m.t==='prx'?'Pr\xc3\xb3ximo':'Pasado')+'</span>'\r\n"
    "      +'</div></div>').join('');}\r\n"
    /* ── COMUNIDAD ── */
    "function loadC(){\r\n"
    "  var p=document.getElementById('pc');\r\n"
    "  p.innerHTML=H('\xf0\x9f\x91\xa5 COMUNIDAD \xe2\x80\x94 Nodos de la red')\r\n"
    "    +'<div class=\"g2\"><div class=\"card\"><div class=\"cval\" id=\"cr\">\xe2\x80\x94</div><div class=\"clbl\">Recibidos</div></div>'\r\n"
    "    +'<div class=\"card\"><div class=\"cval ag\" id=\"ct\">\xe2\x80\x94</div><div class=\"clbl\">Enviados</div></div></div>'\r\n"
    "    +'<div id=\"nl\"><div class=\"empty\">Buscando nodos...</div></div>'\r\n"
    "    +'<div style=\"text-align:center;margin-top:10px\"><span class=\"badge ok\">LoRa 868 MHz</span>'\r\n"
    "    +'<span class=\"badge ok\">WiFi AP</span><span class=\"badge\">T-Deck Pro</span></div>';\r\n"
    "  fetch('/api/estado').then(r=>r.json()).then(d=>{\r\n"
    "    document.getElementById('cr').textContent=d.rx!==undefined?d.rx:'\xe2\x80\x94';\r\n"
    "    document.getElementById('ct').textContent=d.tx!==undefined?d.tx:'\xe2\x80\x94';\r\n"
    "    var nl=document.getElementById('nl');\r\n"
    "    if(d.nodos&&d.nodos.length){\r\n"
    "      nl.innerHTML=d.nodos.map(n=>'<div class=\"card\"><div class=\"nrow\">'\r\n"
    "        +'<div class=\"ndot '+(n.activo?'on':'off')+'\"></div>'\r\n"
    "        +'<div style=\"flex:1\"><div style=\"font-weight:600;font-size:13px\">Nodo '+n.id+'</div>'\r\n"
    "        +'<div style=\"font-size:11px;color:var(--dim)\">'+fmt(n.ultimo*1000)+'</div></div>'\r\n"
    "        +'<span style=\"font-size:12px;color:var(--tk)\">'+(n.rssi?n.rssi+'dBm':'\xe2\x80\x94')+'</span>'\r\n"
    "        +'</div></div>').join('');\r\n"
    "    } else {\r\n"
    "      nl.innerHTML='<div class=\"card\"><div class=\"nrow\"><div class=\"ndot on\"></div>'\r\n"
    "        +'<div style=\"flex:1\"><div style=\"font-weight:600;font-size:13px\">Este nodo ('+(d.nodo_id||'?')+')</div>'\r\n"
    "        +'<div style=\"font-size:11px;color:var(--dim)\">'+(d.shaar||'activo')+'</div></div>'\r\n"
    "        +'<span style=\"font-size:11px;color:var(--ok)\">LOCAL</span></div></div>'\r\n"
    "        +'<div class=\"empty\" style=\"padding:10px\">Sin otros nodos a\xc3\xban</div>';\r\n"
    "    }\r\n"
    "  }).catch(function(){});\r\n"
    "}\r\n"
    /* ── MAPA ── */
    "function bldP(){var p=document.getElementById('pp');if(p.dataset.b)return;p.dataset.b='1';\r\n"
    "  p.innerHTML=H('\xf0\x9f\x97\xba MAPA \xe2\x80\x94 Red de nodos')\r\n"
    "    +'<div class=\"ph\"><div class=\"ph-ic\">\xf0\x9f\x93\xa1</div>'\r\n"
    "    +'<div class=\"ph-t\">Diagrama de red</div>'\r\n"
    "    +'<div class=\"ph-s\">Cuando los nodos est\xc3\xa9n activos aparecer\xc3\xa1n aqu\xc3\xad.<br><br>'\r\n"
    "    +'<em style=\"color:var(--ag)\">Nm 9:17 \xe2\x80\x94 donde la nube mora,<br>all\xc3\xad el campamento.</em></div></div>';\r\n"
    "  fetch('/api/estado').then(r=>r.json()).then(d=>{\r\n"
    "    if(!d.nodos||!d.nodos.length)return;\r\n"
    "    var g='<div style=\"display:grid;grid-template-columns:repeat(3,1fr);gap:8px;margin:10px 0\">'\r\n"
    "      +'<div class=\"card\" style=\"border-color:var(--ag);text-align:center\"><div style=\"font-size:22px\">\xf0\x9f\x93\xa1</div>'\r\n"
    "      +'<div style=\"font-size:12px;margin-top:4px\">Este nodo<br><span style=\"color:var(--ag)\">LOCAL</span></div></div>';\r\n"
    "    d.nodos.forEach(n=>{g+='<div class=\"card\" style=\"text-align:center\"><div style=\"font-size:22px\">\xf0\x9f\x93\xa1</div>'\r\n"
    "      +'<div style=\"font-size:11px;margin-top:4px\">Nodo '+n.id+(n.rssi?'<br><span style=\"color:var(--tk)\">'+n.rssi+'dBm</span>':'')+'</div></div>';});\r\n"
    "    g+='</div>';\r\n"
    "    document.getElementById('pp').innerHTML=H('\xf0\x9f\x97\xba MAPA \xe2\x80\x94 Red de nodos')+g;\r\n"
    "  }).catch(function(){});\r\n"
    "}\r\n"
    /* ── CÁMARA ── */
    "function bldK(){var p=document.getElementById('pk');if(p.dataset.b)return;p.dataset.b='1';\r\n"
    "  p.innerHTML='<div class=\"ph\"><div class=\"ph-ic\">\xf0\x9f\x93\xb7</div>'\r\n"
    "    +'<div class=\"ph-t\">C\xc3\xa1mara \xe2\x80\x94 Pr\xc3\xb3ximamente</div>'\r\n"
    "    +'<div class=\"ph-s\">El T-Deck Pro tiene c\xc3\xa1mara integrada.<br>'\r\n"
    "    +'En la pr\xc3\xb3xima versi\xc3\xb3n podr\xc3\xa1s capturar y compartir<br>im\xc3\xa1genes por la red LoRa.<br><br>'\r\n"
    "    +'<em style=\"color:var(--ag)\">\xc3\x89x 38:8 \xe2\x80\x94 Los espejos de las mujeres<br>que serv\xc3\xadan a la entrada</em></div></div>';}\r\n"
    /* ── NOTAS ── */
    "function bldN(){var p=document.getElementById('pn');if(p.dataset.b){document.getElementById('ntx').value=localStorage.getItem('hm_n')||'';return;}p.dataset.b='1';\r\n"
    "  p.innerHTML=H('\xf0\x9f\x93\x9d NOTAS \xe2\x80\x94 Apuntes personales')\r\n"
    "    +'<textarea class=\"nota\" id=\"ntx\" placeholder=\"Escribe aqu\xc3\xad tus notas de estudio, revelaciones de la Palabra...\"></textarea>'\r\n"
    "    +'<div style=\"display:flex;gap:8px;margin-top:8px\">'\r\n"
    "    +'<button class=\"sv\" style=\"flex:1\" onclick=\"svN()\">Guardar \xe2\x9c\x93</button>'\r\n"
    "    +'<button class=\"sv\" style=\"flex:0 0 80px;background:var(--bg3);color:var(--dim)\" onclick=\"clN()\">Borrar</button></div>'\r\n"
    "    +'<div id=\"nst\" style=\"font-size:11px;color:var(--ok);padding:4px;text-align:center\"></div>';\r\n"
    "  document.getElementById('ntx').value=localStorage.getItem('hm_n')||'';}\r\n"
    "function svN(){localStorage.setItem('hm_n',document.getElementById('ntx').value);\r\n"
    "  var s=document.getElementById('nst');s.textContent='\xe2\x9c\x93 Guardado';setTimeout(function(){s.textContent='';},2000);}\r\n"
    "function clN(){document.getElementById('ntx').value='';localStorage.removeItem('hm_n');}\r\n"
    /* ── ARCHIVOS ── */
    "function bldF(){var p=document.getElementById('pf');if(p.dataset.b)return;p.dataset.b='1';\r\n"
    "  p.innerHTML=H('\xf0\x9f\x93\x81 ARCHIVOS \xe2\x80\x94 Documentos compartidos')\r\n"
    "    +'<div class=\"ph\"><div class=\"ph-ic\">\xf0\x9f\x93\x81</div>'\r\n"
    "    +'<div class=\"ph-t\">Archivos \xe2\x80\x94 Pr\xc3\xb3ximamente</div>'\r\n"
    "    +'<div class=\"ph-s\">Comparte documentos y archivos con toda la red LoRa sin internet.<br><br>'\r\n"
    "    +'<em style=\"color:var(--ag)\">Dt 6:7 \xe2\x80\x94 Shinnan \xe2\x80\x94 \"Ens\xc3\xa9\xc3\xb1alos diligentemente\"</em></div></div>';}\r\n"
    /* ── AJUSTES ── */
    "function loadA(){\r\n"
    "  var p=document.getElementById('pa');\r\n"
    "  fetch('/api/estado').then(r=>r.json()).then(d=>{\r\n"
    "    p.innerHTML=H('\xe2\x9a\x99 AJUSTES')\r\n"
    "      +'<div class=\"card\"><div class=\"sh\">Nodo</div>'\r\n"
    "      +'<div class=\"aj\"><span class=\"ajl\">ID del nodo</span><span class=\"ajv\">'+(d.nodo_id||'\xe2\x80\x94')+'</span></div>'\r\n"
    "      +'<div class=\"aj\"><span class=\"ajl\">MAC</span><span class=\"ajv\" style=\"font-size:10px\">'+(d.mac||'\xe2\x80\x94')+'</span></div>'\r\n"
    "      +'<div class=\"aj\"><span class=\"ajl\">RSSI</span><span class=\"ajv\">'+(d.rssi?d.rssi+' dBm':'\xe2\x80\x94')+'</span></div>'\r\n"
    "      +'<div class=\"aj\"><span class=\"ajl\">TX</span><span class=\"ajv\">'+(d.tx!==undefined?d.tx:'\xe2\x80\x94')+'</span></div>'\r\n"
    "      +'<div class=\"aj\"><span class=\"ajl\">RX</span><span class=\"ajv\">'+(d.rx!==undefined?d.rx:'\xe2\x80\x94')+'</span></div></div>'\r\n"
    "      +'<div class=\"card\"><div class=\"sh\">Red</div>'\r\n"
    "      +'<div class=\"aj\"><span class=\"ajl\">SSID</span><span class=\"ajv\">HASHMAL</span></div>'\r\n"
    "      +'<div class=\"aj\"><span class=\"ajl\">IP</span><span class=\"ajv\">192.168.4.1</span></div>'\r\n"
    "      +'<div class=\"aj\"><span class=\"ajl\">Canal LoRa</span><span class=\"ajv\">868 MHz</span></div></div>'\r\n"
    "      +'<div style=\"text-align:center;padding:12px;font-size:11px;color:var(--dim)\">\xc3\x89x 25:9 \xe2\x80\x94 Conforme a TODO lo que Yo te muestro</div>';\r\n"
    "  }).catch(function(){});}\r\n"
    "</script>\r\n"
    "</body>\r\n"
    "</html>\r\n";

/* ===================================================================
 * MONTAJE PARTICIÓN DEVAR — Palabra de YHWH
 *
 * "Pondrás en el Arca el Testimonio que YO te daré" — Éx 25:16
 * "AL LADO del Arca": la Torá completa junto al núcleo — Dt 31:26
 *
 * La partición 'devar' (SPIFFS, 8MB, offset 0x400000) contiene
 * los 66 libros, 1189 capítulos, hebreo original + definiciones.
 * Se monta una sola vez en init. Los handlers sirven por HTTP.
 * =================================================================== */

static void devar_mount_spiffs(void)
{
    if (g_devar_mounted) return;

    esp_vfs_spiffs_conf_t conf = {
        .base_path              = "/devar",
        .partition_label        = "devar",
        .max_files              = 4,
        .format_if_mount_failed = false,
    };

    esp_err_t ret = esp_vfs_spiffs_register(&conf);
    if (ret == ESP_OK) {
        g_devar_mounted = true;
        HTTP_LOG_I("DEVAR montado: /devar (Éx 25:16 — Testimonio en el Arca)");
    } else {
        HTTP_LOG_E("DEVAR mount falló (0x%x) — Dt 31:26 testigo", (unsigned)ret);
    }
}

/* ===================================================================
 * DETECCIÓN DE PORTAL CAUTIVO
 *
 * Cada SO moderno tiene una "prueba de conectividad" propia.
 * Cuando el T-Deck no da internet, el DNS redirige la prueba a
 * 192.168.4.1, y este servidor responde con un trigger especial
 * que hace al SO mostrar el portal automáticamente.
 *
 * "lo yissagéru" — Is 60:11: las puertas NO se cierran (tamíd).
 * Cada SO tiene su llave; esta función las reconoce todas.
 * =================================================================== */

static esp_err_t handler_captivo_apple(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/html");
    httpd_resp_sendstr(req, "<HTML><HEAD><TITLE>Success</TITLE></HEAD>"
                            "<BODY>Success</BODY></HTML>");
    return ESP_OK;
}

static esp_err_t handler_captivo_204(httpd_req_t *req)
{
    httpd_resp_set_status(req, "302 Found");
    httpd_resp_set_hdr(req, "Location", "http://192.168.4.1/");
    httpd_resp_send(req, NULL, 0);
    HTTP_LOG_I("GET /generate_204 -> 302 redirect (Android captive portal trigger)");
    return ESP_OK;
}

static esp_err_t handler_captivo_win(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/plain");
    httpd_resp_sendstr(req, "Microsoft NCSI");
    return ESP_OK;
}

static esp_err_t handler_captivo_linux(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/plain");
    httpd_resp_sendstr(req, "NetworkManager is online");
    return ESP_OK;
}

/* ===================================================================
 * HANDLERS DE LA API
 * =================================================================== */

/**
 * GET / — sirve el portal HTML5 completo.
 * "uleSHA'AR HEJATSER MASAKH" — Éx 27:16.
 */
static esp_err_t handler_portal_raiz(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, PORTAL_HTML, (ssize_t)sizeof(PORTAL_HTML) - 1);
    HTTP_LOG_I("GET / → portal HTML5 servido (Éx 27:16 roqém)");
    return ESP_OK;
}

/**
 * GET /api/mensajes — JSON de mensajes LoRa recibidos.
 * "lechem hapaním SIEMPRE delante" — Éx 25:30
 */
static esp_err_t handler_api_mensajes_get(httpd_req_t *req)
{
    static char json_buf[HTTP_JSON_BUF];

    int n = hashmal_perimeter_puente_rx_json(json_buf, sizeof(json_buf));
    if (n <= 0) {
        httpd_resp_set_type(req, "application/json");
        httpd_resp_sendstr(req, "{\"msgs\":[],\"rx\":0,\"tx\":0}");
        return ESP_OK;
    }

    httpd_resp_set_type(req, "application/json");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    httpd_resp_send(req, json_buf, (ssize_t)n);
    return ESP_OK;
}

/**
 * POST /api/mensajes — envía texto desde el portal hacia LoRa.
 * "veheír 'al 'éver panéiha" — Éx 25:37.
 */
static esp_err_t handler_api_mensajes_post(httpd_req_t *req)
{
    static char body[HTTP_POST_MAX + 1u];
    int content_len = (int)req->content_len;

    if (content_len <= 0 || content_len > (int)HTTP_POST_MAX) {
        httpd_resp_set_status(req, "400 Bad Request");
        httpd_resp_sendstr(req, "Mensaje muy largo o vacío");
        return ESP_OK;
    }

    int received = httpd_req_recv(req, body, (size_t)content_len);
    if (received <= 0) {
        httpd_resp_set_status(req, "500 Internal Server Error");
        httpd_resp_sendstr(req, "Error al leer body");
        return ESP_OK;
    }
    body[received] = '\0';

    int rc = hashmal_perimeter_puente_tx_send((const uint8_t *)body,
                                               (size_t)received);
    if (rc == 0) {
        httpd_resp_set_type(req, "application/json");
        httpd_resp_sendstr(req, "{\"ok\":true}");
        HTTP_LOG_I("POST /api/mensajes: %d bytes → LoRa (Éx 25:37 veheír)",
                   received);
    } else {
        httpd_resp_set_status(req, "500 Internal Server Error");
        httpd_resp_set_type(req, "application/json");
        httpd_resp_sendstr(req, "{\"ok\":false,\"error\":\"tx_failed\"}");
        HTTP_LOG_W("POST /api/mensajes: tx falló (rc=%d)", rc);
    }
    return ESP_OK;
}

/**
 * GET /api/estado — JSON de estado de la red.
 * "shiv'áh 'einéi YHWH" — Zac 4:10.
 */
static esp_err_t handler_api_estado(httpd_req_t *req)
{
    static char stat_buf[HTTP_STAT_BUF];

    int n = hashmal_perimeter_puente_estado_json(stat_buf, sizeof(stat_buf));
    if (n <= 0) {
        httpd_resp_set_type(req, "application/json");
        httpd_resp_sendstr(req, "{\"shaar\":\"error\",\"tamid\":false}");
        return ESP_OK;
    }

    httpd_resp_set_type(req, "application/json");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    httpd_resp_send(req, stat_buf, (ssize_t)n);
    return ESP_OK;
}

/**
 * GET /api/devar/index — índice de los 66 libros.
 * "Pondrás en el Arca el Testimonio" — Éx 25:16
 */
static esp_err_t handler_api_devar_index(httpd_req_t *req)
{
    static char buf[DEVAR_IDX_BUF];

    FILE *f = fopen("/devar/index.json", "r");
    if (!f) {
        httpd_resp_set_status(req, "503 Service Unavailable");
        httpd_resp_set_type(req, "application/json");
        httpd_resp_sendstr(req, "{\"error\":\"devar no montado\"}");
        HTTP_LOG_W("GET /api/devar/index: partición no montada");
        return ESP_OK;
    }

    size_t n = fread(buf, 1, sizeof(buf) - 1u, f);
    fclose(f);
    buf[n] = '\0';

    httpd_resp_set_type(req, "application/json");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    httpd_resp_set_hdr(req, "Cache-Control", "public, max-age=86400");
    httpd_resp_send(req, buf, (ssize_t)n);
    HTTP_LOG_I("GET /api/devar/index → %u bytes (Éx 25:16)", (unsigned)n);
    return ESP_OK;
}

/**
 * GET /api/devar/cap?b=XX&c=YYY — capítulo comprimido gzip.
 * "AL LADO del Arca": la Torá completa accesible — Dt 31:26
 * Los archivos son .gz; el navegador los descomprime automáticamente.
 */
static esp_err_t handler_api_devar_cap(httpd_req_t *req)
{
    static uint8_t buf[DEVAR_CAP_BUF];
    char query[32];
    char libro[4] = {0};
    char cap[5]   = {0};

    if (httpd_req_get_url_query_str(req, query, sizeof(query)) != ESP_OK) {
        httpd_resp_set_status(req, "400 Bad Request");
        httpd_resp_sendstr(req, "Faltan parámetros b y c");
        return ESP_OK;
    }

    httpd_query_key_value(query, "b", libro, sizeof(libro));
    httpd_query_key_value(query, "c", cap,   sizeof(cap));

    if (libro[0] == '\0' || cap[0] == '\0') {
        httpd_resp_set_status(req, "400 Bad Request");
        httpd_resp_sendstr(req, "Parámetros b y c requeridos");
        return ESP_OK;
    }

    char path[48];
    snprintf(path, sizeof(path), "/devar/%s/%s.gz", libro, cap);

    FILE *f = fopen(path, "rb");
    if (!f) {
        httpd_resp_set_status(req, "404 Not Found");
        httpd_resp_sendstr(req, "Capítulo no encontrado");
        HTTP_LOG_W("GET /api/devar/cap: %s no encontrado", path);
        return ESP_OK;
    }

    size_t n = fread(buf, 1, sizeof(buf), f);
    fclose(f);

    httpd_resp_set_type(req, "text/plain; charset=utf-8");
    httpd_resp_set_hdr(req, "Content-Encoding", "gzip");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    httpd_resp_set_hdr(req, "Cache-Control", "public, max-age=86400");
    httpd_resp_send(req, (const char *)buf, (ssize_t)n);
    HTTP_LOG_I("GET /api/devar/cap: %s → %u bytes gz", path, (unsigned)n);
    return ESP_OK;
}

/* ===================================================================
 * TABLA DE RUTAS
 *
 * Cada ruta es una "cortina" del atrio (qelaʻím, H7050).
 * Juntas forman el perímetro completo.
 * =================================================================== */

static const httpd_uri_t URI_RAIZ = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = handler_portal_raiz,
    .user_ctx  = NULL,
};

static const httpd_uri_t URI_API_MENSAJES_GET = {
    .uri       = "/api/mensajes",
    .method    = HTTP_GET,
    .handler   = handler_api_mensajes_get,
    .user_ctx  = NULL,
};

static const httpd_uri_t URI_API_MENSAJES_POST = {
    .uri       = "/api/mensajes",
    .method    = HTTP_POST,
    .handler   = handler_api_mensajes_post,
    .user_ctx  = NULL,
};

static const httpd_uri_t URI_API_ESTADO = {
    .uri       = "/api/estado",
    .method    = HTTP_GET,
    .handler   = handler_api_estado,
    .user_ctx  = NULL,
};

/* Devár YHWH — Palabra del Padre (Éx 25:16, Dt 31:26) */
static const httpd_uri_t URI_API_DEVAR_INDEX = {
    .uri       = "/api/devar/index",
    .method    = HTTP_GET,
    .handler   = handler_api_devar_index,
    .user_ctx  = NULL,
};

static const httpd_uri_t URI_API_DEVAR_CAP = {
    .uri       = "/api/devar/cap",
    .method    = HTTP_GET,
    .handler   = handler_api_devar_cap,
    .user_ctx  = NULL,
};

/* Detección captive portal — Android/Chrome */
static const httpd_uri_t URI_GEN204 = {
    .uri       = "/generate_204",
    .method    = HTTP_GET,
    .handler   = handler_captivo_204,
    .user_ctx  = NULL,
};

/* iOS / macOS — múltiples paths */
static const httpd_uri_t URI_HOTSPOT = {
    .uri       = "/hotspot-detect.html",
    .method    = HTTP_GET,
    .handler   = handler_captivo_apple,
    .user_ctx  = NULL,
};
static const httpd_uri_t URI_SUCCESS = {
    .uri       = "/success.html",
    .method    = HTTP_GET,
    .handler   = handler_captivo_apple,
    .user_ctx  = NULL,
};
static const httpd_uri_t URI_APPLE_CDN = {
    .uri       = "/library/test/success.html",
    .method    = HTTP_GET,
    .handler   = handler_captivo_apple,
    .user_ctx  = NULL,
};

/* Windows — ncsi y connecttest */
static const httpd_uri_t URI_NCSI = {
    .uri       = "/ncsi.txt",
    .method    = HTTP_GET,
    .handler   = handler_captivo_win,
    .user_ctx  = NULL,
};
static const httpd_uri_t URI_CONNECTTEST = {
    .uri       = "/connecttest.txt",
    .method    = HTTP_GET,
    .handler   = handler_captivo_win,
    .user_ctx  = NULL,
};

/* Linux NetworkManager */
static const httpd_uri_t URI_CAPTIVE_TXT = {
    .uri       = "/captive.txt",
    .method    = HTTP_GET,
    .handler   = handler_captivo_linux,
    .user_ctx  = NULL,
};

/* ===================================================================
 * HANDLER COMODÍN — cualquier ruta desconocida → portal
 *
 * "di' emoú eán tis eisélthē" — Jn 10:9: solo UNA puerta.
 * =================================================================== */
static esp_err_t handler_comodin(httpd_req_t *req, httpd_err_code_t err)
{
    (void)err;
    httpd_resp_set_status(req, "302 Found");
    httpd_resp_set_hdr(req, "Location", "http://192.168.4.1/");
    httpd_resp_send(req, NULL, 0);
    return ESP_OK;
}

/* ===================================================================
 * API PÚBLICA
 * =================================================================== */

/**
 * hashmal_perimeter_http_portal_init — arranca el servidor HTTP.
 *
 * "vayyitén et masak sha'ar hejatsér vayjál Moshé et hammelaják"
 *   — Éxodo 40:33.
 * Moisés PUSO la cortina de la puerta del atrio y TERMINÓ la obra.
 *
 * 2 testigos:
 *   Éx 40:33 — vayjál: la obra se TERMINÓ al poner la cortina
 *   Is 60:11 — tamíd: las puertas abiertas continuamente
 */
int hashmal_perimeter_http_portal_init(void)
{
    if (g_server != NULL) {
        HTTP_LOG_W("http_portal_init: ya inicializado (idempotente)");
        return 0;
    }

    /* Montar la Palabra de YHWH — Éx 25:16 */
    devar_mount_spiffs();

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.max_uri_handlers  = 18u;     /* 13 rutas + 5 nuevas (devar) */
    config.server_port       = 80u;
    config.lru_purge_enable  = true;
    config.max_open_sockets  = 13u;     /* LWIP_MAX_SOCKETS(16) - 3 internos = 13
                                         * Éx 27:9-19 — 60 columnas sostienen el atrio (Detalle 4)
                                         * Ap 11:2 — "no midas el atrio exterior": frontera FLEXIBLE
                                         * Is 60:11 — tamíd: puertas abiertas continuo */
    config.recv_wait_timeout = 5u;
    config.send_wait_timeout = 5u;
    config.uri_match_fn      = httpd_uri_match_wildcard;

    esp_err_t ret = httpd_start(&g_server, &config);
    if (ret != ESP_OK) {
        HTTP_LOG_E("http_portal_init: httpd_start falló (0x%x)", (unsigned)ret);
        return -1;
    }

    /* Registrar rutas de la API y portal */
    httpd_register_uri_handler(g_server, &URI_RAIZ);
    httpd_register_uri_handler(g_server, &URI_API_MENSAJES_GET);
    httpd_register_uri_handler(g_server, &URI_API_MENSAJES_POST);
    httpd_register_uri_handler(g_server, &URI_API_ESTADO);

    /* Devár YHWH — Palabra del Padre (Éx 25:16) */
    httpd_register_uri_handler(g_server, &URI_API_DEVAR_INDEX);
    httpd_register_uri_handler(g_server, &URI_API_DEVAR_CAP);

    /* Registrar rutas de detección captive portal */
    httpd_register_uri_handler(g_server, &URI_GEN204);
    httpd_register_uri_handler(g_server, &URI_HOTSPOT);
    httpd_register_uri_handler(g_server, &URI_SUCCESS);
    httpd_register_uri_handler(g_server, &URI_APPLE_CDN);
    httpd_register_uri_handler(g_server, &URI_NCSI);
    httpd_register_uri_handler(g_server, &URI_CONNECTTEST);
    httpd_register_uri_handler(g_server, &URI_CAPTIVE_TXT);

    /* Comodín: URL desconocida → redirect al portal */
    httpd_register_err_handler(g_server, HTTPD_404_NOT_FOUND,
                                handler_comodin);

    HTTP_LOG_I("═══ SHA'AR HTTP LEVANTADO ═══ (Éx 40:33 vayjál hammelaják)");
    HTTP_LOG_I("  Puerto: 80  |  Rutas: 13 + comodín");
    HTTP_LOG_I("  Portal:  http://192.168.4.1/");
    HTTP_LOG_I("  API:     /api/mensajes  /api/estado");
    HTTP_LOG_I("  DEVAR:   /api/devar/index  /api/devar/cap");
    HTTP_LOG_I("  HTML: %u bytes — obra de bordador (Éx 27:16 roqém)",
               (unsigned)(sizeof(PORTAL_HTML) - 1u));
    return 0;
}

/**
 * hashmal_perimeter_http_portal_deinit — detiene el servidor HTTP.
 * Solo para desmontaje ordenado (Nm 4 — empacado para migrar).
 */
int hashmal_perimeter_http_portal_deinit(void)
{
    if (g_server == NULL) return 0;
    esp_err_t ret = httpd_stop(g_server);
    if (ret == ESP_OK) {
        g_server = NULL;
        HTTP_LOG_I("http_portal_deinit: servidor detenido (Nm 4 — empacado)");
        return 0;
    }
    HTTP_LOG_E("http_portal_deinit: httpd_stop falló (0x%x)", (unsigned)ret);
    return -1;
}