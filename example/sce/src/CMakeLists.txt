# Add emulation library
add_library(Emulation packet.cpp emulatedblock.cpp emulatedline.cpp emulationcore.cpp)

# Add subdirectories
add_subdirectory(modules)
add_subdirectory(utils)


# Enable gui if necessary
if (BUILD_QT_GUI)
    add_subdirectory(gui/qt)
endif()

# link QT when build with GUI
if (BUILD_QT_GUI)
    target_link_libraries(Emulation PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
endif ()

if (BUILD_PACKETVIEWER)
    add_subdirectory(gui/packetview)
endif()

add_executable(Main main.cpp)

if (BUILD_QT_GUI)
target_link_libraries(Main PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
endif ()

target_link_libraries(Main PRIVATE Emulation)

