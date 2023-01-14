cmake_minimum_required(VERSION 3.9)
project(Cygnoides_Decision)

set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_VERBOSE_MAKEFILE ON)

if (EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else ()
    message(WARNING "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif ()

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/contrib)

file(GLOB_RECURSE MAIN_LIBS CONFIGURE_DEPENDS "src/*.h" "src/*.cpp")
file(GLOB_RECURSE UTILS_CONTRIB CONFIGURE_DEPENDS "utils_contrib/*.h" "utils_contrib/*.cpp")

message("-- ProjectSourceDir: ${PROJECT_SOURCE_DIR}")
message("-- GLOB: \${MAIN_LIBS}: ${MAIN_LIBS}")
message("-- GLOB: \${UTILS_CONTRIB}: ${UTILS_CONTRIB}")

add_library(Cygnoides_Decision ${MAIN_LIBS})
target_link_libraries(Cygnoides_Decision ${OpenCV_LIBS})

add_executable(tool_calibrate_camera tests/calibrate_camera.cpp)
target_link_libraries(tool_calibrate_camera ${OpenCV_LIBS} ${CONAN_LIBS})

add_executable(test_video_playback ${UTILS_CONTRIB} ${MAIN_LIBS} tests/test_video_playback.cpp)
target_link_libraries(test_video_playback ${OpenCV_LIBS} ${CONAN_LIBS})

add_executable(test_reconstructor ${UTILS_CONTRIB} ${MAIN_LIBS} tests/test_reconstructor.cpp)
target_link_libraries(test_reconstructor ${OpenCV_LIBS} ${CONAN_LIBS})

add_executable(test_armor_track ${MAIN_LIBS} ${UTILS_CONTRIB} tests/test_armor_track.cpp)
target_link_libraries(test_armor_track ${OpenCV_LIBS} ${CONAN_LIBS})

add_executable(test_tracker ${UTILS_CONTRIB} )
target_link_libraries(test_tracker ${OpenCV_LIBS} ${CONAN_LIBS} tests/test_tracker.cpp)

add_executable(test_detection ${MAIN_LIBS} ${UTILS_CONTRIB} tests/test_detection.cpp)
target_link_libraries(test_detection ${OpenCV_LIBS} ${CONAN_LIBS})