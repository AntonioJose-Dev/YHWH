idf_component_register(
    SRCS "src/consagracion_onboarding.c" "src/consagracion_fases.c" "src/consagracion_sellos.c" "src/consagracion_lockdown.c" "src/consagracion_cuarentena.c"
    INCLUDE_DIRS "include"
    REQUIRES hashmal_auth hashmal_roles log esp_timer
)
