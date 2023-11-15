cmake_minimum_required(VERSION 3.10)

project(moses)
set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules (JEMALLOC jemalloc)

pkg_search_module(JEMALLOC REQUIRED jemalloc)
include_directories(${JEMALLOC_INCLUDE_DIRS})


include_directories(${JEMALLOC_INCLUDE_DIR})
file(GLOB SOURCES "*.cpp")


add_executable(moses ${SOURCES})
target_link_libraries(moses ${JEMALLOC_LIBRARIES})

#target_link_libraries(moses ${JEMALLOC_LIBRARY})