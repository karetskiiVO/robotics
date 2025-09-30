add_library(ExampleLibrary2 OBJECT ExampleLibrary2.cpp)

set(DEPENDENCIES ExampleLibrary)
target_include_directories(ExampleLibrary2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ExampleLibrary2 PRIVATE ${DEPENDENCIES})
