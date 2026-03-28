# ==============================================================================
# Unit Tests Configuration
# ==============================================================================

enable_testing()

add_executable(test_word test_word.c)
target_link_libraries(test_word PRIVATE analyzer_core)

add_executable(test_rb_tree test_rb_tree.c)
target_link_libraries(test_rb_tree PRIVATE analyzer_core)

set_target_properties(test_word test_rb_tree PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

add_test(NAME WordModuleTest COMMAND test_word)
add_test(NAME RBTreeModuleTest COMMAND test_rb_tree)
