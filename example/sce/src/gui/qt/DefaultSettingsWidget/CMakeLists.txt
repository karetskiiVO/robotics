target_sources(QtGUI PRIVATE
    defaultsettingswidget.cpp
    defaultsettingswidget.h
    defaultsettingswidget.ui
)

target_sources(Emulation PRIVATE
    defaultsettingswidget.cpp
    defaultsettingswidget.h
    defaultsettingswidget.ui
)

target_include_directories(QtGUI PRIVATE .)
