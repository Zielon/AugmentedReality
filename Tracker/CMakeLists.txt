cmake_minimum_required(VERSION 3.10)
project(Tracker)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED )

set(SOURCE_FILES main.cpp)

configure_file(resources/movie.MP4 resources/movie.MP4 COPYONLY)

add_executable(Tracker ${SOURCE_FILES})

include_directories(${OpenCV_INCLUDE_DIRS})

target_link_libraries(Tracker ${OpenCV_LIBS} )