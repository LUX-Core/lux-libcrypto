cmake_minimum_required (VERSION 3.5)

set(BOOST_VERSION 1.58)
include(ExternalProject)

# For some reason we need to define both of these
add_definitions(-DBOOST_COROUTINES_NO_DEPRECATION_WARNING)
add_definitions(-DBOOST_COROUTINE_NO_DEPRECATION_WARNING)

################################################################################
project(lux-lib-crypto)

#find_package(OpenSSL REQUIRED)
find_package(Boost ${BOOST_VERSION} REQUIRED COMPONENTS filesystem
        regex
        unit_test_framework
        program_options
        coroutine
        context
        thread
        date_time
        system)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread -Wall -ggdb ${SANITIZE}")


################################################################################
include_directories(
        "${Boost_INCLUDE_DIR}"
        "src"
)


################################################################################
file(GLOB_RECURSE SRC
        "*.c"
        "*.h"
        "*.hpp"
        "*.tpp"
        "*.cpp")
add_library(${PROJECT_NAME} ${SRC})
