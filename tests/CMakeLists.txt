# ==============================================================================
# Unit Tests Configuration
# ==============================================================================

# 1. Test for the Word Module
add_executable(test_word 
    test_word.c 
    ../src/word.c
)
target_include_directories(test_word PRIVATE ../include)

# 2. Test for the RB-Tree Map Module
add_executable(test_rb_tree 
    test_rb_tree.c 
    ../src/map.c 
    ../src/word.c
)
target_include_directories(test_rb_tree PRIVATE ../include)

# (Optional) Global configuration for all test binaries
# This ensures all test executables go into build/bin
set_target_properties(test_word test_rb_tree PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Enable CTest if you want to use 'make test' or 'ctest'
enable_testing()
add_test(NAME WordTests COMMAND test_word)
add_test(NAME MapTests COMMAND test_rb_tree)
