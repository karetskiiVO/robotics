cmake_minimum_required(VERSION 3.12)

project(Task1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(INTERNAL_LIBS_PATH "internal")

set(INTERNAL_LIBS
	"ExampleLibrary"
)

add_compile_options(-Werror -Wall -Wextra -Wpedantic)

add_executable(Task1 Main.cpp)
target_link_libraries(Task1 m ${INTERNAL_LIBS})

add_subdirectory(${INTERNAL_LIBS_PATH})

list(TRANSFORM INTERNAL_LIBS PREPEND ${INTERNAL_LIBS_PATH}/)
include_directories(${INTERNAL_LIBS})
