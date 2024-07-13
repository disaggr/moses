cmake_minimum_required(VERSION 3.10)
include(ExternalProject)

project(moses)
set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES "*.cpp")
add_executable(moses ${SOURCES})

# begin jemalloc integration
# define source and target directories
set(JEMALLOC_PREFIX_DIR ${CMAKE_BINARY_DIR}/Jemalloc)
set(JEMALLOC_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/jemalloc)
set(JEMALLOC_INSTALL_DIR ${JEMALLOC_PREFIX_DIR}/install)

# define external project build characteristics
ExternalProject_Add(Jemalloc
    SOURCE_DIR          ${JEMALLOC_SRC_DIR}
    PREFIX              ${JEMALLOC_PREFIX_DIR}
    CONFIGURE_COMMAND   cd ${JEMALLOC_SRC_DIR} && ./autogen.sh && ./configure
                            --with-jemalloc-prefix=je_
                            --prefix=${JEMALLOC_INSTALL_DIR}
    BUILD_COMMAND       cd ${JEMALLOC_SRC_DIR} && make && make install_lib_static install_include
    INSTALL_COMMAND     "")

# set up targets for CMAKE to understand the jemalloc libs
add_library(libjemalloc STATIC IMPORTED GLOBAL)
add_library(libjemalloc-pic STATIC IMPORTED GLOBAL)

# associate the CMAKE targets with the built external static libs
set_target_properties(libjemalloc PROPERTIES
        "IMPORTED_LOCATION" "${JEMALLOC_INSTALL_DIR}/lib/libjemalloc.a")
set_target_properties(libjemalloc-pic PROPERTIES
        "IMPORTED_LOCATION" "${JEMALLOC_INSTALL_DIR}/lib/libjemalloc-pic.a")

# tell cmake to build the external project to produce the library files
add_dependencies(libjemalloc Jemalloc)
add_dependencies(libjemalloc-pic Jemalloc)

# set include directories for moses
set(JEMALLOC_INCLUDE_DIR ${JEMALLOC_INSTALL_DIR}/include)
set(JEMALLOC_INTERNAL_INCLUDE_DIR ${JEMALLOC_SRC_DIR}/include)
target_include_directories(moses PRIVATE ${JEMALLOC_INCLUDE_DIR} ${JEMALLOC_INTERNAL_INCLUDE_DIR})

# link moses to static libjemalloc
target_link_libraries(moses libjemalloc)
