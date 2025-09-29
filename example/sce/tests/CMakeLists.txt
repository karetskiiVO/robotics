# Include GoogleTest
include(GoogleTest)

# Enable tests
enable_testing()

# Add tests for libraries
add_subdirectory(modules)
add_subdirectory(core)
