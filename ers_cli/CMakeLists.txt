include_directories(../lib_ers/)

find_package(Boost COMPONENTS system filesystem program_options REQUIRED)
find_package(OpenCV REQUIRED)

# Print the value of OpenCV_INCLUDE_DIRS
message(STATUS "OpenCV_INCLUDE_DIRS: ${OpenCV_INCLUDE_DIRS}")

include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(${EIGEN_INCLUDE_DIRS})

add_executable(ers_cli main.cpp)
target_link_libraries(ers_cli ${Boost_LIBRARIES} ${OpenCV_LIBS} ers)
