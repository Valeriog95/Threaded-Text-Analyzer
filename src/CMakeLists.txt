# Create a library from the source files
add_library(word word.c)

# If you use math.lib or threads, link them here
# target_link_libraries(word PRIVATE m)