idf_component_register(
    SRCS "src/altar_gateway.c" "src/altar_reshet.c" "src/altar_cuernos.c" "src/altar_fuego.c" "src/altar_utensilios.c" "src/altar_ledger.c"
    INCLUDE_DIRS "include"
    PRIV_INCLUDE_DIRS "src"
    REQUIRES mbedtls hashmal_perimeter esp_timer log esp_hw_support
)
