idf_component_register(
    SRCS
        "src/incienso_firewall.c"
        "src/incienso_formato.c"
        "src/incienso_circuitbreaker.c"
        "src/incienso_heartbeat.c"
        "src/incienso_penalizacion.c"
        "src/incienso_shemitah.c"           # Añadidura 14 — Lv 25 + Dt 15 (reset)
        "src/incienso_pesaj.c"              # Añadidura 15 — Ex 12:7 (dam al hamezuzot)
    INCLUDE_DIRS "include"
    PRIV_INCLUDE_DIRS "src"
    REQUIRES hashmal_framework hashmal_keygen log esp_timer
)
