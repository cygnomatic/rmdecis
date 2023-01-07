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

#file(GLOB_RECURSE Main_Lib CONFIGURE_DEPENDS "src/compensator/*.h" "src/compensator/*.cpp" "src/reconstructor/*.h" "src/reconstructor/*.cpp" "src/tracker/*.h" "src/tracker/*.cpp" "src/typing/*.h" "src/typing/*.cpp" "src/utils/*.h" "src/utils/*.cpp")
file(GLOB_RECURSE Main_Lib CONFIGURE_DEPENDS LIST_DIRECTORIES true "src/*.h" "src/*.cpp")
file(GLOB_RECURSE Utils_Contrib CONFIGURE_DEPENDS "utils_contrib/*.h" "utils_contrib/*.cpp")

add_library(Cygnoides_Decision src/reconstructor/camera_calib.cpp src/reconstructor/camera_calib.h src/utils/equation_solver.cpp src/utils/equation_solver.h src/compensator/ballistic_compensator.cpp src/compensator/ballistic_compensator.h src/tracker/armor_track.cpp src/tracker/armor_track.h src/tracker/tracker.cpp src/tracker/tracker.h src/typing/general.h interface.h test_interface.h src/typing/armor.h src/utils/cv_utils.h src/reconstructor/transformer.cpp src/contrib/munkres-cpp/matrix.cpp src/contrib/munkres-cpp/munkres.cpp src/utils/cv_utils.cpp)
link_directories(${OpenCV_LIBRARY_DIRS})
target_link_libraries(Cygnoides_Decision ${OpenCV_LIBS} Eigen3::Eigen)

add_executable(util_calibrate_camera tests/calibrate_camera.cpp)
link_directories(${OpenCV_LIBRARY_DIRS})
target_link_libraries(util_calibrate_camera ${OpenCV_LIBS})

add_executable(test_equation_solver tests/test_equation_solver.cpp src/utils/equation_solver.cpp src/utils/equation_solver.h)

add_executable(test_video_playback utils_contrib/simple_video_player.cpp utils_contrib/simple_video_player.h tests/test_video_playback.cpp utils_contrib/simulate_vision_result.h utils_contrib/simulate_vision_result.cpp src/utils/cv_utils.cpp)
link_directories(${OpenCV_LIBRARY_DIRS})
target_link_libraries(test_video_playback ${OpenCV_LIBS} ${CONAN_LIBS})

add_executable(test_reconstructor ${Main_lib} ${Utils_Contrib} tests/test_reconstructor.cpp src/reconstructor/camera_calib.cpp)
link_directories(${OpenCV_LIBRARY_DIRS})
target_link_libraries(test_reconstructor ${OpenCV_LIBS} ${CONAN_LIBS})

add_executable(test_armor_track ${Main_lib} ${Utils_Contrib} tests/test_armor_track.cpp src/tracker/armor_track.cpp src/utils/cv_utils.cpp)
link_directories(${OpenCV_LIBRARY_DIRS})
target_link_libraries(test_armor_track ${OpenCV_LIBS} ${CONAN_LIBS})