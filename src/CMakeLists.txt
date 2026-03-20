# ==============================================================================
# Core Library Configuration
# ==============================================================================


add_library(analyzer_core STATIC
    word.c
)

target_include_directories(analyzer_core PUBLIC ../include)