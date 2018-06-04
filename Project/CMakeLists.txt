cmake_minimum_required(VERSION 3.10)
project(Project)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

configure_file(resources/lena resources/lena COPYONLY)
configure_file(resources/movie resources/movie COPYONLY)
configure_file(resources/stuff resources/stuff COPYONLY)

add_executable(Project main.cpp neutral_net/source/neutral_network.cpp neutral_net/headers/neutral_network.h)