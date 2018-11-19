project(lux-lib-crypto)

cmake_minimum_required (VERSION 3.5)

if (NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "Doesn't support non 64-bit platform")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/)


################################################################################
include(ExternalProject)
include(FindPkgConfig)

include_directories("src")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -W")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -W -ggdb ${SANITIZE}")


################################################################################
#set(boost_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/external/boost)
#ExternalProject_Add(
#        boost
#        PREFIX ${boost_PREFIX}
#        URL "http://dl.bintray.com/boostorg/release/1.63.0/source/boost_1_63_0.tar.gz"
#        CONFIGURE_COMMAND ${boost_PREFIX}/src/boost/bootstrap.sh --with-libraries=thread,system --prefix=${boost_PREFIX}
#        BUILD_COMMAND ./bjam link=static
#        INSTALL_COMMAND ./bjam link=static install
#        BUILD_IN_SOURCE 1
#)
#include_directories(SYSTEM ${boost_PREFIX}/include)
#set(BOOST_LIBRARIES ${boost_PREFIX}/lib/libboost_thread.a)
#list(APPEND BOOST_LIBRARIES ${boost_PREFIX}/lib/libboost_system.a)
find_package(Boost ${BOOST_VERSION} REQUIRED COMPONENTS filesystem
        unit_test_framework
        system)

################################################################################
#set(cryptopp_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/external/cryptopp)
#ExternalProject_Add(
#        cryptopp
#        PREFIX ${cryptopp_PREFIX}
#        URL "http://www.cryptopp.com/cryptopp700.zip"
#        CONFIGURE_COMMAND ""
#        BUILD_COMMAND make
#        INSTALL_COMMAND make -p ${cryptopp_PREFIX} install
#        BUILD_IN_SOURCE 1
#)
#
#include_directories(SYSTEM ${cryptopp_PREFIX}/include)
#set(CRYPTOPP_LIBRARIES ${cryptopp_PREFIX}/lib/libcryptopp.a)

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

add_executable(${PROJECT_NAME}_test
        src/test/storage_heap.cpp
        src/StorageHeap.cpp
        src/CryptoSplitter.cpp)

target_link_libraries(${PROJECT_NAME}_test
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        pthread
        ssl
)


################################################################################
