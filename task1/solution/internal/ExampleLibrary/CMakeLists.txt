add_library(ExampleLibrary OBJECT ExampleLibrary.cpp)

# В эту переменную впишите зависимости этой библиотеки, см. ExampleLibrary2
set(DEPENDENCIES "")
target_include_directories(ExampleLibrary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ExampleLibrary PRIVATE ${DEPENDENCIES})
