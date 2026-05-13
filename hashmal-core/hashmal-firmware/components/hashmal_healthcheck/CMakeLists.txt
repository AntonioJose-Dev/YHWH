idf_component_register(
    SRCS
        "src/lavacro_core.c"
        "src/lavacro_vectores.c"
        "src/lavacro_niveles.c"
        "src/lavacro_espejos.c"
        "src/lavacro_ataques.c"
        "src/lavacro_dos_niveles.c"
        "src/lavacro_parokhet.c"            # Añadidura 01 — Éx 26:31-33 (Modo Parokhet)
        "src/lavacro_meshulash.c"           # Añadidura 13 — Ec 4:9-12 (cordón triple)
        "src/lavacro_tsur.c"                # Añadidura 16 — Éx 17:6 (mayim min hatsur)
        "src/lavacro_joq_olam.c"            # P17 — Éx 30:21 (joq ʻolám, sin bypass)
        "src/lavacro_challenge.c"           # P36+P37 — Nm 5:17,5:23 (ephemeral challenge)
        "src/lavacro_meshulash_sajar.c"    # P54+P56 — Ec 4:9-12 (umbrales + sajár)
    INCLUDE_DIRS "include"
    PRIV_INCLUDE_DIRS "src"
    REQUIRES hashmal_auth hashmal_comm hashmal_perimeter esp_timer log esp_hw_support
)
