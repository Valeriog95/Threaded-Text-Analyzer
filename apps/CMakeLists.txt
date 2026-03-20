# Define the executable name
add_executable(ThreadedTextAnalyzer main.c)

# Link the executable with our logic library
target_link_libraries(ThreadedTextAnalyzer PRIVATE word)