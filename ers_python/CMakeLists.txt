cmake_minimum_required(VERSION 4.0)

project(ers_python)

# Find dependencies
find_package(pybind11 REQUIRED)
find_package(OpenCV REQUIRED)

# Include headers from lib_ers
include_directories(${PROJECT_SOURCE_DIR}/../lib_ers)

# Define the Python module
pybind11_add_module(ers_py ers_wrapper.cpp)

target_link_libraries(ers_py PRIVATE ${OpenCV_LIBS})