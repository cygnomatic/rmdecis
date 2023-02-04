cmake_minimum_required(VERSION 3.9)
set(CMAKE_CXX_STANDARD 11)
project(test_rmdecis)

### Link OpenCV
find_package(OpenCV REQUIRED)
link_libraries(${OpenCV_LIBS})

#### Link rmdecis
find_package(PkgConfig REQUIRED)
pkg_check_modules(RMDECIS REQUIRED rmdecis)
link_libraries(${RMDECIS_LIBRARIES})
include_directories(${RMDECIS_INCLUDE_DIRS})

add_executable(test_rmdecis test_rmdecis.cpp)