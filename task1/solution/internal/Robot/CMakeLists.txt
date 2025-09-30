add_library(Robot STATIC Robot.cpp)
target_link_libraries(Robot PUBLIC ${INTERNAL_LIBS})
target_include_directories(Robot PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
