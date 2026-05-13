# ═══════════════════════════════════════════════════════════════════
# PIEZA 04 — LA MENORÁ — kikkár zaháv tahór
#
# "kikkár zaháv tahór yaʻasé otáh — ét kol hakkelím haʼélle"
#   — Éxodo 25:39
# "kullá miqshá ajat zaháv tahór" — Éxodo 25:36
#
# Un talento de oro puro con TODOS los instrumentos. Toda ella
# una sola pieza batida. Los 9 kelím de la Menorá multi-transporte
# (6 originales + 3 mutsaqót de Zac 4:2) se registran aquí como
# UNA sola compilación — miqshá ajat, no módulos sueltos.
# ═══════════════════════════════════════════════════════════════════

idf_component_register(
    SRCS
        # ── Los 6 kelím originales (Éx 25:31-40) ──
        "src/menora_hub.c"                  # gézaʻ — v.31 miqshá, v.36 kullá
        "src/menora_lora_driver.c"          # yaréj + qanáh — SPI SX1262
        "src/menora_canales.c"              # 7 nerot — v.37 shivʻá
        "src/menora_aceite.c"               # shémen zayit zaj — Éx 27:20
        "src/menora_routing.c"              # 22 geviʻím — v.33-34
        "src/menora_formato.c"              # tavnít — v.40 betavnitám

        # ── Los 3 kelím nuevos (Zac 4:2 shivʻá mutsaqót) ──
        "src/menora_mutsaqot.c"             # gullá — Zac 4:2 selector berují
        "src/menora_espnow_driver.c"        # ishá el ajotá — Éx 26:3 P2P
        "src/menora_ble_paamon.c"           # paʻamón — Éx 28:34 campanilla

        # ── Ventana al velo (Añadidura 01 - Modo Parokhet) ──
        "src/menora_parokhet.c"             # muestreo EM + Shabat cooperativo (Éx 26:31-33)

        # ── Puerta del atalaya (Añadidura 02 - Modo Tsofeh) ──
        "src/menora_tsofeh.c"               # TX lockdown + shofar dedicado (Ez 33:3)

    INCLUDE_DIRS "include"
    PRIV_INCLUDE_DIRS "src" "../../main"

    # ── kol hakkelím haʼélle: el oro de Egipto que cada kelí necesita ──
    REQUIRES
        driver               # v.31 yaréj — bus SPI (SX1262)
        log                  # logging
        hashmal_framework    # Éx 40:2 — estructura del Mishkán
        esp_timer            # tamíd — timestamps monotónicos
        esp_wifi             # Éx 26:3 ishá el ajotá — WiFi STA (sin conectar)
        espressif__esp-now    # Éx 26:17 meshullavót — P2P capa 2
        bt                   # Éx 28:34 paʻamón — NimBLE stack
        nvs_flash            # calibración de radio
        esp_netif            # netif base
        esp_event            # event loop base
        freertos             # task management
)
