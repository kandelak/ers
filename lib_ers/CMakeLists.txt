# Find required packages
find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED COMPONENTS timer filesystem system)

include_directories(${OpenCV_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

add_library(ers MERCCInput.cpp
MERCDisjointSet.cpp
MERCFunctions.cpp
MERCLazyGreedy.cpp
MERCOutput.cpp
Tools.cpp)

# Link libraries
target_link_libraries(ers ${OpenCV_LIBS} ${Boost_LIBRARIES})
