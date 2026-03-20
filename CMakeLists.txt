cmake_minimum_required(VERSION 3.10)

# Project metadata
project(WordCounter 
        VERSION 1.0 
        DESCRIPTION "Word frequency counter for OS2 Exam"
        LANGUAGES C)

# Set the C standard to C11 (common for OS projects)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Force output binaries to a specific folder for clarity
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include directories so headers can be found easily
include_directories(include)

# Add subdirectories (we will create a CMakeLists.txt in each)
add_subdirectory(src)   # The logic library
add_subdirectory(apps)  # The main CLI application
add_subdirectory(tests) # The test suite

# Enable testing globally
enable_testing()