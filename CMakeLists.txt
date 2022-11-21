cmake_minimum_required(VERSION 3.9)
project(Cygnoides_Decision VERSION 0.1.0 DESCRIPTION "BUPT-RM-Cygnoides decision part.")

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED NO_MODULE)

include_directories(${OpenCV_INCLUDE_DIRS})

add_library(Cygnoides_Decision reconstructor/reconstructor.cpp reconstructor/reconstructor.h reconstructor/objects/armor.h utils/equation_solver.cpp utils/equation_solver.h compensator/ballistic_compensator.cpp compensator/ballistic_compensator.h tracker/armor_tracker.cpp tracker/armor_tracker.h tracker/armor_tracker_mgr.cpp tracker/armor_tracker_mgr.h general/typing.h)
link_directories(${OpenCV_LIBRARY_DIRS})
target_link_libraries(Cygnoides_Decision ${OpenCV_LIBS} Eigen3::Eigen)

add_executable(util_calibrate_camera utils_contrib/calibrate_camera.cpp)
link_directories(${OpenCV_LIBRARY_DIRS})
target_link_libraries(util_calibrate_camera ${OpenCV_LIBS})

add_executable(test_equation_solver tests/test_equation_solver.cpp utils/equation_solver.cpp utils/equation_solver.h)