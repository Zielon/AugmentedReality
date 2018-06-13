cmake_minimum_required(VERSION 3.10)
project(Project)

find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(glfw3 REQUIRED)

configure_file(resources/lena resources/lena COPYONLY)
configure_file(resources/movie resources/movie COPYONLY)
configure_file(resources/stuff resources/stuff COPYONLY)
configure_file(data/train-images-idx3-ubyte data/train-images-idx3-ubyte COPYONLY)
configure_file(data/train-labels-idx1-ubyte data/train-labels-idx1-ubyte COPYONLY)

add_executable(Project
        main.cpp
        neural_net_plain/source/neutral_network.cpp
        neural_net_plain/headers/neutral_network.h
        tracker/source/tracker.cpp
        tracker/headers/tracker.h
        drawing/draw_primitives.h
        drawing/draw_snowman.h
        mnist_loader/data_loader.h
        mnist_loader/data_loader.cpp
        data_models/digit.cpp
        neural_net_plain/headers/solver.h
        neural_net_plain/source/solver.cpp
        data_models/consts.h
        neural_net_plain/headers/sandwicher.h
        neural_net_plain/source/sandwicher.cpp neural_net_opencv/network.cpp neural_net_opencv/network.h)

target_link_libraries(Project Eigen3::Eigen)
target_link_libraries(Project ${OPENGL_gl_LIBRARY})
target_link_libraries(Project ${OPENGL_glu_LIBRARY})
target_link_libraries(Project glfw)