add_library(Networking OBJECT TCPServerLinux.cpp UDPClientLinux.cpp ControllerPort.cpp TelemetryPort.cpp)

# В эту переменную впишите зависимости этой библиотеки, см. ExampleLibrary2
set(DEPENDENCIES "")
target_include_directories(Networking PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Networking PRIVATE ${DEPENDENCIES} ${EXTERNAL_LIBS})
