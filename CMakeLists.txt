cmake_minimum_required(VERSION 3.9)
project(Cygnoides_Decision VERSION 0.1.0 DESCRIPTION "BUPT-RM-Cygnoides decision part.")

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED NO_MODULE)

include_directories(${OpenCV_INCLUDE_DIRS})

add_library(Cygnoides_Decision reconstructor/camera_calib.cpp reconstructor/camera_calib.h utils/equation_solver.cpp utils/equation_solver.h compensator/ballistic_compensator.cpp compensator/ballistic_compensator.h tracker/tracked_armor.cpp tracker/tracked_armor.h tracker/tracker.cpp tracker/tracker.h typing/general.h interface.h test_interface.h utils/construct_armor.h typing/armor.h)
link_directories(${OpenCV_LIBRARY_DIRS})
target_link_libraries(Cygnoides_Decision ${OpenCV_LIBS} Eigen3::Eigen)

add_executable(util_calibrate_camera utils_contrib/calibrate_camera.cpp)
link_directories(${OpenCV_LIBRARY_DIRS})
target_link_libraries(util_calibrate_camera ${OpenCV_LIBS})

add_executable(test_equation_solver tests/test_equation_solver.cpp utils/equation_solver.cpp utils/equation_solver.h)