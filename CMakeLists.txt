# ---------------------------------------------------------------------------
# IMLAB
# ---------------------------------------------------------------------------

project(imlab)
cmake_minimum_required(VERSION 3.7)

# ---------------------------------------------------------------------------
# Environment
# ---------------------------------------------------------------------------

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -fsanitize=address")

# Unfortunately, macOS (<= High Sierra) ships with a too old bison version.
# You need to manually install bison and flex via homebrew.
# (It's a good thing to have homebrew installed anyway)
# https://brew.sh/index_de.html
# - brew update
# - brew install bison flex
#
# As homebrew won't override the system's default bison, we need to tell
# cmake where it should look.
if (APPLE)
    list(APPEND CMAKE_PREFIX_PATH /usr/local/opt/bison)
    list(APPEND CMAKE_PREFIX_PATH /usr/local/opt/flex)
endif (APPLE)

# ---------------------------------------------------------------------------
# Dependencies
# ---------------------------------------------------------------------------

find_package(Threads REQUIRED)
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG ON)

include("${CMAKE_SOURCE_DIR}/vendor/benchmark.cmake")
include("${CMAKE_SOURCE_DIR}/vendor/googletest.cmake")
include("${CMAKE_SOURCE_DIR}/vendor/gflags.cmake")
include("${CMAKE_SOURCE_DIR}/vendor/cpplint.cmake")

# ---------------------------------------------------------------------------
# Includes
# ---------------------------------------------------------------------------

include("${CMAKE_SOURCE_DIR}/include/local.cmake")
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${GTEST_INCLUDE_DIR}
    ${GMOCK_INCLUDE_DIR}
    ${GFLAGS_INCLUDE_DIR}
    ${BENCHMARK_INCLUDE_DIR}
)

# ---------------------------------------------------------------------------
# Sources
# ---------------------------------------------------------------------------

include("${CMAKE_SOURCE_DIR}/src/local.cmake")
include("${CMAKE_SOURCE_DIR}/test/local.cmake")
include("${CMAKE_SOURCE_DIR}/tools/local.cmake")
include("${CMAKE_SOURCE_DIR}/bench/local.cmake")

# ---------------------------------------------------------------------------
# Linting
# ---------------------------------------------------------------------------

add_custom_target(lint)
add_dependencies(lint ${lint_targets})

# ---------------------------------------------------------------------------
# Configuration
# ---------------------------------------------------------------------------

message(STATUS "[IMLAB] settings")
message(STATUS "    GFLAGS_INCLUDE_DIR          = ${GFLAGS_INCLUDE_DIR}")
message(STATUS "    GFLAGS_LIBRARY_PATH         = ${GFLAGS_LIBRARY_PATH}")
message(STATUS "[TEST] settings")
message(STATUS "    GTEST_INCLUDE_DIR           = ${GTEST_INCLUDE_DIR}")
message(STATUS "    GTEST_LIBRARY_PATH          = ${GTEST_LIBRARY_PATH}")
message(STATUS "    GMOCK_INCLUDE_DIR           = ${GMOCK_INCLUDE_DIR}")
message(STATUS "    GMOCK_LIBRARY_PATH          = ${GMOCK_LIBRARY_PATH}")

