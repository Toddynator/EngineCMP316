# Create a static library
file(GLOB_RECURSE JOLT_SRC CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.cpp"
)
add_library(Jolt STATIC ${JOLT_SRC})

target_include_directories(Jolt PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")