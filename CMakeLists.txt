cmake_minimum_required(VERSION 3.9)
project(Cygnoides_Decision VERSION 0.1.0 DESCRIPTION "BUPT-RM-Cygnoides decision part.")

set(CMAKE_CXX_STANDARD 14)

if (EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else ()
    message(WARNING "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif ()

find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED NO_MODULE)

include_directories(${OpenCV_INCLUDE_DIRS})

file(GLOB Main_Lib CONFIGURE_DEPENDS "compensator/*.h" "compensator/*.cpp" "reconstructor/*.h" "reconstructor/*.cpp" "tracker/*.h" "tracker/*.cpp" "typing/*.h" "typing/*.cpp" "utils/*.h" "utils/*.cpp")
file(GLOB Utils_Contrib CONFIGURE_DEPENDS "utils_contrib/*.h" "utils_contrib/*.cpp")

add_library(Cygnoides_Decision reconstructor/camera_calib.cpp reconstructor/camera_calib.h utils/equation_solver.cpp utils/equation_solver.h compensator/ballistic_compensator.cpp compensator/ballistic_compensator.h tracker/armor_track.cpp tracker/armor_track.h tracker/tracker.cpp tracker/tracker.h typing/general.h interface.h test_interface.h typing/armor.h utils/cv_utils.h)
link_directories(${OpenCV_LIBRARY_DIRS})
target_link_libraries(Cygnoides_Decision ${OpenCV_LIBS} Eigen3::Eigen)

add_executable(util_calibrate_camera tests/calibrate_camera.cpp)
link_directories(${OpenCV_LIBRARY_DIRS})
target_link_libraries(util_calibrate_camera ${OpenCV_LIBS})

add_executable(test_equation_solver tests/test_equation_solver.cpp utils/equation_solver.cpp utils/equation_solver.h)

add_executable(test_video_playback utils_contrib/simple_video_player.cpp utils_contrib/simple_video_player.h tests/test_video_playback.cpp utils_contrib/simulate_vision_result.h utils_contrib/simulate_vision_result.cpp)
link_directories(${OpenCV_LIBRARY_DIRS})
target_link_libraries(test_video_playback ${OpenCV_LIBS} ${CONAN_LIBS})

add_executable(test_reconstructor tests/test_reconstructor.cpp ${Main_lib} ${Utils_Contrib} reconstructor/camera_calib.cpp reconstructor/camera_calib.h)
link_directories(${OpenCV_LIBRARY_DIRS})
target_link_libraries(test_reconstructor ${OpenCV_LIBS} ${CONAN_LIBS})