# ==============================================================================
# Unit Tests Configuration - Word Module
# ==============================================================================

# Test for the Word Module
add_executable(test_word 
    test_word.c 
    ../src/word.c
)
target_include_directories(test_word PRIVATE ../include)

# Ensure binaries go to build/bin
set_target_properties(test_word PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

enable_testing()
add_test(NAME WordTests COMMAND test_word)