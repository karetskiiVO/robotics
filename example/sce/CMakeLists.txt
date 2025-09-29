cmake_minimum_required(VERSION 3.22)
project(satellite_communication_emulator
        LANGUAGES CXX)

option(BUILD_TESTS "Build the test suite" ON)
option(BUILD_QT_GUI "Build the Qt GUI" OFF)
option(BUILD_PACKETVIEWER "Build the packet viewer" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Enable all compiler checks and treat warnings as errors
if (MSVC)
    add_compile_options(/W4 /WX /DEBUG)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif ()

# Define USE_QT_GUI for all subprojects if the option is enabled
if(BUILD_QT_GUI)
    add_compile_definitions(USE_QT_GUI)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)

    find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
endif()

# Add include directories
include_directories(src)
include_directories("include")

# Add subdirectories
add_subdirectory(src)

# Enable tests if necessary
if(BUILD_TESTS)
    find_package(GTest QUIET)
    if (NOT GTest_FOUND)
        message(STATUS "system GTest not found, using submodule")
        include_directories(extern/googletest/googletest/include)
        add_subdirectory(extern/googletest)
    endif()
    add_subdirectory(tests)
endif()
