# Add subdirectories
add_library(MODCOD modcod.cpp)

target_sources(Emulation PRIVATE modcod.cpp)