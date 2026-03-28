# Define the test executable
# add_executable(run_unit_tests test_logic.c)

# Link with the library to test its functions
# target_link_libraries(run_unit_tests PRIVATE word_lib)

# Register the test so you can run it with 'ctest'
# add_test(NAME UnitTests COMMAND run_unit_tests)

add_executable(test_word test_word.c ../src/word.c)
target_include_directories(test_word PRIVATE ../include)