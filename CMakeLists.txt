cmake_minimum_required(VERSION 4.0)
# Add the project() command after cmake_minimum_required
project(ers-python)

set(CMAKE_CXX_FLAGS  "-O4 -fpermissive")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

#
# To build the project, install CMake, then change into 'build' and
# call 'cmake ../'.
#

# Entropy Rate Superpixels
add_subdirectory(lib_ers)
add_subdirectory(ers_cli)
add_subdirectory(ers_python)
