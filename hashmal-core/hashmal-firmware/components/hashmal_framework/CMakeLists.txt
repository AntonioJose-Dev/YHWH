idf_component_register(
    SRCS "src/estructura_capas.c" "src/estructura_tablas.c" "src/estructura_barras.c" "src/estructura_velo.c" "src/estructura_ejad.c"
    INCLUDE_DIRS "include"
    PRIV_INCLUDE_DIRS "src"
    REQUIRES log
)
