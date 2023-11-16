cmake_minimum_required(VERSION 3.10)

project(moses)
set(CMAKE_CXX_STANDARD 17)

set(JEMALLOC_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/jemalloc/include")
set(JEMALLOC_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/jemalloc/lib/libjemalloc.dylib")
set(JEMALLOC_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/jemalloc/lib/")

link_directories(${JEMALLOC_LIB_DIR})
include_directories(${JEMALLOC_INCLUDE_DIR})
file(GLOB SOURCES "*.cpp")

add_executable(moses ${SOURCES})
target_link_libraries(moses ${JEMALLOC_LIBRARIES})