cmake_minimum_required(VERSION 3.10)

project(moses)
set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES "*.cpp")
add_executable(moses ${SOURCES})

set(JEMALLOC_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/jemalloc/include")
set(JEMALLOC_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/jemalloc/lib/libjemalloc.dylib")
set(JEMALLOC_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/jemalloc/lib/")

target_include_directories(moses PRIVATE ${JEMALLOC_INCLUDE_DIR})
target_link_libraries(moses ${JEMALLOC_LIBRARIES})
target_link_directories(moses PRIVATE ${JEMALLOC_LIB_DIR})
