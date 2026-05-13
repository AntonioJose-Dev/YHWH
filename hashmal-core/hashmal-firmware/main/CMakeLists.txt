idf_component_register(
    SRCS "hashmal_main.c" "hashmal_epd.c" "hashmal_menu.c"
    INCLUDE_DIRS "."
    REQUIRES
        hashmal_framework
        hashmal_storage
        hashmal_consensus
        hashmal_cache
        hashmal_comm
        hashmal_intercession
        hashmal_auth
        hashmal_healthcheck
        hashmal_perimeter
        hashmal_power
        hashmal_keygen
        hashmal_roles
        hashmal_onboarding
        hashmal_firewall
        driver
        esp_driver_spi
        esp_driver_gpio
        esp_driver_i2c
        esp_adc
        nvs_flash
)
