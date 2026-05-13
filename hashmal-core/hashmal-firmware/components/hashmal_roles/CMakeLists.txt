idf_component_register(
    SRCS
        "src/vestiduras_roles.c"
        "src/vestiduras_efod.c"
        "src/vestiduras_pectoral.c"
        "src/vestiduras_monitor.c"
        "src/vestiduras_coordinador.c"
        "src/vestiduras_shutdown.c"
        "src/vestiduras_tzitz.c"
        "src/vestiduras_tsofeh.c"           # Añadidura 02 — Ez 33:2-3 (atalaya)
        "src/vestiduras_tzitzit.c"          # Añadidura 06 — Nm 15:38 (flecos)
        "src/vestiduras_jatsotsrot.c"       # Añadidura 08 — Nm 10:1-10 (trompetas)
        "src/vestiduras_escalacion.c"       # Añadidura 09 — Mt 18:15-17 (3 niveles)
        "src/vestiduras_nehemias.c"         # Añadidura 10 — Neh 4:13-20 (rally)
        "src/vestiduras_goel.c"             # Añadidura 12 — Lv 25:25 + Rut 4 (redentor)
        "src/vestiduras_neder.c"            # Añadidura 20 — Nm 30:3 (voto)
    INCLUDE_DIRS "include"
    PRIV_INCLUDE_DIRS "src"
    REQUIRES
        hashmal_framework
        hashmal_comm                        # Añadidura 02 — shofar + vecinos + RSSI
        hashmal_healthcheck                 # Añadidura 02 — parokhet_indice
        hashmal_firewall                    # Añadidura 02 — veredicto ZAR
        hashmal_cache                       # Añadidura 06 — moedim + hashmal_moedim_tiempo_t
        log
        esp_timer
)
