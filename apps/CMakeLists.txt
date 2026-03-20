# ==============================================================================
# Main Application Configuration
# ==============================================================================

add_executable(ThreadedTextAnalyzer main.c)

target_link_libraries(ThreadedTextAnalyzer PRIVATE analyzer_core)