cmake_minimum_required(VERSION 3.12)

project(Task1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(INTERNAL_LIBS_PATH "internal")

if(MSVC)
	add_compile_options(/W4 /WX /DEBUG)
else()
	add_compile_options(-Werror -Wall -Wextra -Wpedantic)
endif()

add_executable(Task1 Main.cpp)
add_subdirectory(${INTERNAL_LIBS_PATH})
target_link_libraries(Task1 m ${INTERNAL_LIBS})
