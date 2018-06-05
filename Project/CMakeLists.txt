cmake_minimum_required(VERSION 3.10)
project(Project)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(glfw3 REQUIRED)

configure_file(resources/lena resources/lena COPYONLY)
configure_file(resources/movie resources/movie COPYONLY)
configure_file(resources/stuff resources/stuff COPYONLY)

add_executable(Project main.cpp neutral_net/source/neutral_network.cpp neutral_net/headers/neutral_network.h tracker/source/tracker.cpp tracker/headers/tracker.h drawing/draw_primitives.h)

target_link_libraries (Project Eigen3::Eigen)
target_link_libraries(Project ${OPENGL_gl_LIBRARY})
target_link_libraries(Project ${OPENGL_glu_LIBRARY})
target_link_libraries(Project glfw)