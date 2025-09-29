# Add executable for testing
add_executable(ModulesTest modulator_test.cpp demodulator_test.cpp line_test.cpp udp_input_test.cpp defer_output.cpp)

# Link everything to executable
target_link_libraries(ModulesTest PRIVATE Emulation GTest::gtest_main)

# link QT when build with GUI
if (BUILD_QT_GUI)
    target_link_libraries(ModulesTest PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
endif ()

# Add tests to CTest
gtest_add_tests(ModulesTest "" AUTO)
