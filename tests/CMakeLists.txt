# ==============================================================================
# Unit Tests Configuration
# ==============================================================================

enable_testing()

add_executable(test_word test_word.c)
target_link_libraries(test_word PRIVATE analyzer_core)

add_executable(test_rb_tree test_rb_tree.c)
target_link_libraries(test_rb_tree PRIVATE analyzer_core)

add_executable(test_rb_iterator test_rb_iterator.c)
target_link_libraries(test_rb_iterator PRIVATE analyzer_core)

add_executable(test_rb_tree_multi_thread test_rb_tree_multi_thread.c)
target_link_libraries(test_rb_tree_multi_thread PRIVATE analyzer_core)

set_target_properties(test_word test_rb_tree PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

add_test(NAME WordModuleTest COMMAND test_word)
add_test(NAME RBTreeModuleTest COMMAND test_rb_tree)
add_test(NAME RBTreeIteratorTest COMMAND test_rb_iterator)
add_test(NAME RBTreeMTTest COMMAND test_rb_tree_multi_thread)
