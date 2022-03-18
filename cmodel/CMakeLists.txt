cmake_minimum_required(VERSION 3.16)
project(cmodel)

find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)

message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")


#include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 11)

add_executable(cmodel main.cpp  src/imgproc.cpp inc/imgproc.h src/core.cpp inc/core.h inc/myalgorithm.h src/levelset.cpp inc/levelset.h src/upsampling.cpp inc/upsampling.h)


target_link_libraries(cmodel ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT})


