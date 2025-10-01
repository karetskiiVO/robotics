set(EXTERNAL_LIBS
    rustcxx
)

add_subdirectory(RustCxx)
set(EXTERNAL_LIBS ${EXTERNAL_LIBS} PARENT_SCOPE)
