cmake_minimum_required(VERSION 3.9)
project(Cygnoides_Decision VERSION 2023.2 DESCRIPTION "CygnoidesRM Decision lib")

set(CMAKE_CXX_STANDARD 17)

if (EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else ()
    message(WARNING "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif ()
find_package(OpenCV REQUIRED)

include_directories(PRIVATE ${OpenCV_INCLUDE_DIRS})
include_directories(PRIVATE ${PROJECT_SOURCE_DIR}/contrib)
include_directories(PRIVATE ${PROJECT_SOURCE_DIR}/src)
include_directories(PUBLIC ${PROJECT_SOURCE_DIR}/include)

link_libraries(${OpenCV_LIBS})
link_libraries(${CONAN_LIBS})

file(GLOB_RECURSE rmdecis_LIBS CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE rmdecis_HEADERS CONFIGURE_DEPENDS "include/*.h")
file(GLOB_RECURSE rmdecis_UTILS CONFIGURE_DEPENDS "utils_contrib/*.cpp")

message("-- ProjectSourceDir: ${PROJECT_SOURCE_DIR}")
message("-- GLOB: \${rmdecis_LIBS}: ${rmdecis_LIBS}")
message("-- GLOB: \${rmdecis_HEADERS}: ${rmdecis_HEADERS}")
message("-- GLOB: \${rmdecis_UTILS}: ${rmdecis_UTILS}")

### Library

add_library(rmdecis SHARED ${rmdecis_LIBS})
set_target_properties(rmdecis PROPERTIES VERSION ${PROJECT_VERSION})
target_link_libraries(rmdecis PUBLIC ${OpenCV_LIBS})

## Install library

install(TARGETS rmdecis LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/rmdecis)
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/rmdecis)

## Tests

add_executable(tool_calibrate_camera tests/calibrate_camera.cpp)

add_executable(test_video_playback ${rmdecis_UTILS} ${rmdecis_LIBS} tests/test_video_playback.cpp)

add_executable(test_reconstructor ${rmdecis_UTILS} ${rmdecis_LIBS} tests/test_reconstructor.cpp)

add_executable(test_armor_track ${rmdecis_LIBS} ${rmdecis_UTILS} tests/test_armor_track.cpp)

add_executable(test_tracker ${rmdecis_LIBS} ${rmdecis_UTILS} tests/test_tracker.cpp)

add_executable(test_detection ${rmdecis_LIBS} ${rmdecis_UTILS} tests/test_detection.cpp)

add_executable(test_basic_aiming ${rmdecis_LIBS} ${rmdecis_UTILS} tests/test_basic_aiming.cpp)

add_executable(test_config ${rmdecis_LIBS} ${rmdecis_UTILS} tests/test_config.cpp)

add_executable(test_kissfft ${rmdecis_LIBS} ${rmdecis_UTILS} tests/test_kissfft.cpp)