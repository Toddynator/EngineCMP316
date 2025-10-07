# Create a static library
file(GLOB_RECURSE IMGUI_SRC CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)
add_library(ImGui STATIC ${IMGUI_SRC})

target_include_directories(ImGui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")