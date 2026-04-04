# ==============================================================================
# Core Library Configuration
# ==============================================================================


add_library(analyzer_core STATIC
    word.c
    node.c
    map.c
    map_utils.c
    parser.c
    analyzer.c
)

target_link_libraries(analyzer_core PRIVATE Threads::Threads)

target_include_directories(analyzer_core PUBLIC ../include)