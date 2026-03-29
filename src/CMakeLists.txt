# ==============================================================================
# Core Library Configuration
# ==============================================================================


add_library(analyzer_core STATIC
    word.c
    node.c
    map.c
    map_utils.c
)

target_include_directories(analyzer_core PUBLIC ../include)