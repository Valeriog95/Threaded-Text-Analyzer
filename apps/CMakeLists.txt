# Define the executable name
add_executable(word-counter main.c)

# Link the executable with our logic library
target_link_libraries(word-counter PRIVATE word_lib)