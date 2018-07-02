cmake_minimum_required(VERSION 3.10)
project(Project)

set(CMAKE_CXX_STANDARD 14)

set(GCC_COVERAGE_COMPILE_FLAGS -pthread)

find_package(Threads REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Bullet REQUIRED)

configure_file(resources/lena resources/lena COPYONLY)
configure_file(resources/movie resources/movie COPYONLY)
configure_file(resources/stuff resources/stuff COPYONLY)
configure_file(resources/CameraMatrix resources/CameraMatrix COPYONLY)
#configure_file(data/train-images-idx3-ubyte data/train-images-idx3-ubyte COPYONLY)
#configure_file(data/train-labels-idx1-ubyte data/train-labels-idx1-ubyte COPYONLY)

link_directories(${BULLET_LIBRARY_DIRS})
include_directories(${BULLET_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(Project
        main.cpp
        neural_net_plain/source/neural_network.cpp
        neural_net_plain/headers/neural_network.h
        tracker/source/tracker.cpp
        tracker/headers/tracker.h
        mnist_loader/data_loader.h
        mnist_loader/data_loader.cpp
        data_models/digit.cpp
        neural_net_plain/headers/solver.h
        neural_net_plain/source/solver.cpp
        data_models/consts.h
        neural_net_plain/headers/sandwicher.h
        neural_net_plain/source/sandwicher.cpp
        neural_net_opencv/network.cpp
        neural_net_opencv/network.h
        bounce/source/ball.cpp
        bounce/headers/ball.h
        bounce/headers/application.h
        bounce/source/application.cpp
        bounce/headers/drawing.h
        bounce/source/drawing.cpp
        bounce/headers/grid.h
        bounce/headers/scene_object.h
        bounce/headers/scene.h
        bounce/source/scene.cpp
        bounce/source/grid.cpp
        tracker/headers/camera.h
        tracker/source/camera.cpp)

target_link_libraries(Project ${OpenCV_LIBS})
target_link_libraries(Project ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(Project Eigen3::Eigen)
target_link_libraries(Project glfw)
target_link_libraries(Project ${OPENGL_gl_LIBRARY})
target_link_libraries(Project ${OPENGL_glu_LIBRARY})
target_link_libraries(Project ${BULLET_LIBRARIES})