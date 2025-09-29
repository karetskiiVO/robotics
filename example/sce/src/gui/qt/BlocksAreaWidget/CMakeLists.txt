add_subdirectory(RunStopButton)
target_sources(QtGUI PRIVATE
    blocksareawidget.cpp
    blocksareawidget.h
    blocksareawidget.ui
)

target_include_directories(QtGUI PRIVATE .)
