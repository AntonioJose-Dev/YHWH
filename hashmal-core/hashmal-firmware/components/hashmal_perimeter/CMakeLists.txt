idf_component_register(
    SRCS
        "src/atrio_perimetro.c"
        "src/atrio_puerta.c"
        "src/atrio_columnas.c"
        "src/atrio_intencion.c"
        "src/atrio_miqlat.c"                # Añadidura 05 — ciudad de refugio (Nm 35)
        "src/atrio_orchim.c"                # Añadidura 21 — hospitalidad (Gn 18)
        "src/atrio_shaar.c"                 # Añadidura 22 — sha'ar (lógica sesiones)
        "src/atrio_puente.c"                # Añadidura 22 — puente WiFi↔LoRa (Zac 4:12)
        "src/atrio_wifi_ap.c"               # Añadidura 22 — AP "HASHMAL" (Is 60:11)
        "src/atrio_dns_cautivo.c"           # Añadidura 22 — DNS cautivo (Jn 10:9)
        "src/atrio_http_portal.c"           # Añadidura 22 — portal HTML5 (Éx 27:16 roqém)
    INCLUDE_DIRS "include"
    PRIV_INCLUDE_DIRS "src"
    REQUIRES
        esp_timer
        log
        esp_wifi
        esp_event
        esp_netif
        esp_http_server
        lwip
        nvs_flash                           # atrio_wifi_ap.c (include nvs_flash.h)
        freertos                            # atrio_dns_cautivo.c (FreeRTOS tasks)
        hashmal_comm                        # puente_tx_send → hashmal_comm_enviar
)
