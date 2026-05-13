idf_component_register(
    SRCS "src/uncion_keygen.c" "src/uncion_secuencia.c" "src/uncion_prohibicion.c" "src/uncion_activacion.c"
    INCLUDE_DIRS "include"
    REQUIRES mbedtls log esp_timer
)
