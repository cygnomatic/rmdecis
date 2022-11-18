cmake_minimum_required(VERSION 3.23)
project(Cygnoides_Decision)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED NO_MODULE)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(Cygnoides_Decision reconstruct_3d/cam_3D_reconstructor.cpp reconstruct_3d/cam_3D_reconstructor.h reconstruct_3d/objects/armor.h utils/equation_solver.cpp utils/equation_solver.h)
link_directories(${OpenCV_LIBRARY_DIRS})
target_link_libraries(Cygnoides_Decision ${OpenCV_LIBS} Eigen3::Eigen)

add_executable(util_calibrate_camera utils_contrib/calibrate_camera.cpp)
link_directories(${OpenCV_LIBRARY_DIRS})
target_link_libraries(util_calibrate_camera ${OpenCV_LIBS})

add_executable(test_equation_solver tests/test_equation_solver.cpp utils/equation_solver.cpp utils/equation_solver.h)