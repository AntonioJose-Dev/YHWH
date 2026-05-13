idf_component_register(
    SRCS
        "src/mesa_cache.c"
        "src/mesa_api.c"
        "src/mesa_misgeret.c"
        "src/mesa_disponibilidad.c"
        "src/mesa_moedim.c"                 # Añadidura 03 — Lv 23 + Lv 25:9 calendario
        "src/mesa_man.c"                    # Añadidura 11 — Ex 16 omer por cabeza
        "src/mesa_shinnan.c"                # Añadidura 17 — Dt 6:7 propagación
        "src/mesa_leket.c"                  # Añadidura 18 — Lv 19:9-10 espigueo
        "src/mesa_maaser.c"                 # Añadidura 19 — Mal 3:10 diezmo
    INCLUDE_DIRS "include"
    PRIV_INCLUDE_DIRS "src"
    REQUIRES mbedtls hashmal_storage esp_timer log
)
