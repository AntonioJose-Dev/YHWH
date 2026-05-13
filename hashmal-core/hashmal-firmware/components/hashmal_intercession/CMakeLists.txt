idf_component_register(
    SRCS "src/altar_oro_escudo.c" "src/altar_oro_ascendente.c" "src/altar_oro_formula.c" "src/altar_oro_circuitbreaker.c" "src/altar_oro_oracion.c"
    INCLUDE_DIRS "include"
    PRIV_INCLUDE_DIRS "src"
    REQUIRES mbedtls hashmal_framework hashmal_comm hashmal_consensus hashmal_auth hashmal_firewall esp_timer log
)
