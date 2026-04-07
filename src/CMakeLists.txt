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
    generator.c
    thread_manager.c
    argtable3.c
    argtable_parser.c
)

# 'm' required for argtable 
target_link_libraries(analyzer_core PRIVATE m Threads::Threads)

target_include_directories(analyzer_core PUBLIC ../include)