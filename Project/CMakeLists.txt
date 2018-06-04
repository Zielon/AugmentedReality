cmake_minimum_required(VERSION 3.10)
project(Project)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)

configure_file(resources/lena resources/lena COPYONLY)
configure_file(resources/movie resources/movie COPYONLY)
configure_file(resources/stuff resources/stuff COPYONLY)

add_executable(Project main.cpp neutral_net/source/neutral_network.cpp neutral_net/headers/neutral_network.h)
target_link_libraries (Project Eigen3::Eigen)