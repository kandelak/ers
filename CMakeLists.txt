cmake_minimum_required(VERSION 3.14)
project(ers_python_bindings VERSION 0.1.0 LANGUAGES CXX)

# Enable C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use pybind11 from PyPI or local
find_package(pybind11 REQUIRED)
find_package(OpenCV REQUIRED)

# Platform-specific settings
if(WIN32)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    add_definitions(-DNOMINMAX)
endif()

# Source files
set(SOURCES
    src/bindings.cpp
    src/ers_wrapper.cpp
)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
)

# Define the Python module
pybind11_add_module(ers MODULE ${SOURCES})

# Link libraries
target_link_libraries(ers PRIVATE ${OpenCV_LIBS})

# Set output directories
set_target_properties(ers PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/python
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/python
)

# Install rules
install(TARGETS ers
    LIBRARY DESTINATION ${PYTHON_SITE_PACKAGES_DIR}
    RUNTIME DESTINATION ${PYTHON_SITE_PACKAGES_DIR}
)

# Enable testing
enable_testing()
add_subdirectory(tests)
