# Add executable for testing
add_executable(LineTest line.cpp)

# Link everything to executable
target_link_libraries(LineTest PRIVATE Emulation GTest::gtest_main)

# link QT when build with GUI
if (BUILD_QT_GUI)
    target_link_libraries(LineTest PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
endif ()

# Add tests to CTest
gtest_add_tests(LineTest "" AUTO)
