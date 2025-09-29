cmake_minimum_required(VERSION 3.16)

project(
  QtGUI
  VERSION 0.1
  LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Svg)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Svg)

set(PROJECT_SOURCES
    ../../abstractsettingswidget.h
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    mainresources.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(QtGUI MANUAL_FINALIZATION
      DemodulatorSettingsWidget/demodulatorsettingswidget.h DemodulatorSettingsWidget/demodulatorsettingswidget.cpp DemodulatorSettingsWidget/demodulatorsettingswidget.ui)
  # Define target properties for Android with Qt 6 as: set_property(TARGET qt
  # APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
  # ${CMAKE_CURRENT_SOURCE_DIR}/android) For more information, see
  # https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
  if(ANDROID)
    add_library(QtGUI SHARED)
    # Define properties for Android with Qt 5 after find_package() calls as:
    # set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
  else()
    add_executable(QtGUI)
  endif()
endif()

target_link_libraries(QtGUI PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(QtGUI PRIVATE Qt${QT_VERSION_MAJOR}::Svg)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1. If
# you are developing for iOS or macOS you should consider setting an explicit,
# fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.qt)
endif()
set_target_properties(
  QtGUI
  PROPERTIES ${BUNDLE_ID_OPTION} MACOSX_BUNDLE_BUNDLE_VERSION
             ${PROJECT_VERSION} MACOSX_BUNDLE_SHORT_VERSION_STRING
             ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR} MACOSX_BUNDLE
             TRUE WIN32_EXECUTABLE
             TRUE)

include(GNUInstallDirs)
install(
  TARGETS QtGUI
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(QtGUI)
endif()

target_sources(QtGUI PRIVATE ${PROJECT_SOURCES})

target_link_libraries(QtGUI PRIVATE MODCOD Emulation)
add_subdirectory(QLongSpinBox)
add_subdirectory(BaseBlockWidget)
add_subdirectory(BlocksAreaWidget)
add_subdirectory(SettingsWidget)
add_subdirectory(InputSettingsWidget)
add_subdirectory(ModulatorSettingsWidget)
add_subdirectory(BUCSettingsWidget)
add_subdirectory(LineSettingsWidget)
add_subdirectory(DemodulatorSettingsWidget)
add_subdirectory(DefaultSettingsWidget)
add_subdirectory(OutputSettingsWidget)
add_subdirectory(SatelliteSettingsWidget)
add_subdirectory(ClientsPage)
add_subdirectory(EmulationWorker)
