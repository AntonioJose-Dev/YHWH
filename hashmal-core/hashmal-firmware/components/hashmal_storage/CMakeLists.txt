idf_component_register(
    SRCS "src/arca_core.c" "src/arca_merkle.c" "src/arca_cifrado.c" "src/arca_zer.c" "src/arca_baddim.c" "src/arca_interfaz.c" "src/arca_sealed_core.c"
    INCLUDE_DIRS "include"
    PRIV_INCLUDE_DIRS "src"
    REQUIRES mbedtls esp_hw_support log nvs_flash
)
