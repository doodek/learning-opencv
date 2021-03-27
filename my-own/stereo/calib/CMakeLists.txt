cmake_minimum_required(VERSION 3.10)

project(calib)
find_package( OpenCV  REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} .)
add_executable(calib calib.cpp)
target_link_libraries(calib ${OpenCV_LIBS})

set_property(TARGET calib PROPERTY CXX_STANDARD 17)
