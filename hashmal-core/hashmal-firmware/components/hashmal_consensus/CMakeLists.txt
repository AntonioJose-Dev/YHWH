idf_component_register(
    SRCS "src/kaporet_core.c" "src/kaporet_validadores.c" "src/kaporet_ataques.c" "src/kaporet_verificacion.c" "src/kaporet_rendezvous.c" "src/kaporet_relay.c"
    INCLUDE_DIRS "include"
    PRIV_INCLUDE_DIRS "src"
    REQUIRES mbedtls hashmal_storage esp_timer log
)
