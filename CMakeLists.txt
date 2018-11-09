cmake_minimum_required (VERSION 3.5)

set(BOOST_VERSION 1.67)
include(ExternalProject)

# For some reason we need to define both of these
add_definitions(-DBOOST_COROUTINES_NO_DEPRECATION_WARNING)
add_definitions(-DBOOST_COROUTINE_NO_DEPRECATION_WARNING)

################################################################################
project(ouinet)

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
#find_package(Threads)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread -Wall -ggdb ${SANITIZE}")


################################################################################
include_directories(
        "${Boost_INCLUDE_DIR}"
)


################################################################################
# Config file directory


set(config_targets )

if(NOT ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    add_custom_target(config ALL DEPENDS test)
    foreach(config_file ${config_files})
        get_filename_component(config_dir ${config_file} DIRECTORY)
        add_custom_command(
                OUTPUT ${CMAKE_BINARY_DIR}/${config_file}
                MAIN_DEPENDENCY ${CMAKE_SOURCE_DIR}/${config_file}
                COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/${config_dir}
                COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/${config_file} ${CMAKE_BINARY_DIR}/${config_file}
        )
        set(config_targets ${config_targets} ${CMAKE_BINARY_DIR}/${config_file})
    endforeach()
    add_custom_target(appel ALL DEPENDS ${config_targets})
endif()

################################################################################
