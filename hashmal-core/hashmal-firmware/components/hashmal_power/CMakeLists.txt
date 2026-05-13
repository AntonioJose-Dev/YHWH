idf_component_register(
    SRCS
        "src/aceite_energia.c"
        "src/aceite_tamid.c"
        "src/aceite_supervivencia.c"
        "src/aceite_mantenimiento.c"
        "src/aceite_ashmurot.c"             # Añadidura 04 — vigilias de la noche (Sal 63:6)
        "src/aceite_anan_esh.c"             # Añadidura 07 — Ex 13:21-22 (nube/fuego)
    INCLUDE_DIRS "include"
    PRIV_INCLUDE_DIRS "src"
    REQUIRES
        driver
        esp_adc                             # Éx 27:20 katít — shíbolei hazzahav (Zac 4:12) ADC real
        hashmal_cache                       # Añadidura 03 — moedim pull + Meeus publico
        hashmal_comm                        # Añadidura 07 — vecinos mesh
        hashmal_healthcheck                 # Añadidura 07 — parokhet shaon
        hashmal_perimeter                   # Añadidura 07 — miqlat local
        esp_timer
        log
)
