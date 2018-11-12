cmake_minimum_required (VERSION 3.5)

set(BOOST_VERSION 1.58)

################################################################################
project(lux-lib-crypto)
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
        "src")


################################################################################
file(GLOB_RECURSE SRC
        "*.c"
        "*.h"
        "*.hpp"
        "*.tpp"
        "*.cpp")
add_library(${PROJECT_NAME} ${SRC})


################################################################################
ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)

add_executable(${PROJECT_NAME}_test src/test/storage_heap.cpp
        src/StorageHeap.cpp)

target_link_libraries(${PROJECT_NAME}_test
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        )


################################################################################
