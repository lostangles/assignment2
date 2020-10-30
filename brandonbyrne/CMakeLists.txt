cmake_minimum_required(VERSION 3.17)
set (CMAKE_CXX_STANDARD 11)
project(assignment2)
add_compile_options(
  "-Wall" "-Wpedantic" "-Wextra" "-fexceptions" "-g"
  "$<$<CONFIG:DEBUG>:-O0;-g;-ggdb>"
)
add_subdirectory(src)
add_subdirectory(googletest)
add_subdirectory(test)
