
## TODO:
#  - Need to move the source_group files into exactly 'Source Files' and 'Header Files' to match VS2022's style of filter.
# That will require me sorting the .h and .cpp files into an include and src folder in the Engine directory.
# - Should make it that the game library name can be set on build through the cmake gui.



cmake_minimum_required(VERSION 4.0)
project(
    Engine
    DESCRIPTION "Game Engine Specialized on Voxels, created for CMP316 University Module"
    HOMEPAGE_URL "https://github.com/Toddynator/EngineCMP316"
    LANGUAGES CXX C
)



## GLOBAL SETTINGS



# C / C++ Language Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 17) # Don't think this is actually working
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")



## ENGINE



# Get the Enginee Source Code
file(GLOB_RECURSE ENGINE_SRC CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/Engine/*.cpp"
    "${CMAKE_SOURCE_DIR}/Engine/*.h"
)
# IDE Project will use the same Folder Structure as found in the source files (If the IDE supports it)
source_group(TREE "${CMAKE_SOURCE_DIR}/Engine" PREFIX "src" FILES ${ENGINE_SRC})
# Install directory, whatever that is
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install")
# Remove Unused Configurations (Use only Release & Debug)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
# Create the project file
add_executable(${PROJECT_NAME} ${ENGINE_SRC})
# Include the Engine Source Code
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/Engine")
# Precompiled Header
target_precompile_headers(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/Engine/Core/pch.h")
# Engine Project Output Directory ~ Binaries + Build Files
set(OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Bin")
set_target_properties(${PROJECT_NAME} PROPERTIES
    # ENGINE PROJECT BINARIES 
    RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIRECTORY}/${CMAKE_VS_PLATFORM_NAME}/$<CONFIG>" #EXE
    ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_DIRECTORY}/${CMAKE_VS_PLATFORM_NAME}/$<CONFIG>" #LIB
    LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIRECTORY}/${CMAKE_VS_PLATFORM_NAME}/$<CONFIG>" #DLL
)



## GAME # OPTIONAL



if(EXISTS "${CMAKE_SOURCE_DIR}/Game")
    # Get the Game Source Code
    file(GLOB_RECURSE GAME_SRC CONFIGURE_DEPENDS
        "${CMAKE_SOURCE_DIR}/Game/*.cpp"
        "${CMAKE_SOURCE_DIR}/Game/*.h"
    )
    # Creat the Library Project
    add_library(Game STATIC ${GAME_SRC})
    # Include the Game source Code
    target_include_directories(Game PUBLIC ${CMAKE_SOURCE_DIR}/Game)
    # Inlcude the Engine source Code
    target_include_directories(Game PUBLIC ${CMAKE_SOURCE_DIR}/Engine)
    # Create Folder Structure for Game Project
    source_group(TREE "${CMAKE_SOURCE_DIR}/Game" PREFIX "src" FILES ${GAME_SRC})
    # Link Game to Engine
    target_link_libraries(Engine PRIVATE Game)
    # Define in the engine via Preprocessor Macro that the game is linked. Used so that the engine can run standalone
    target_compile_definitions(Engine PRIVATE GAME_LINKED=1)
    # Precompiled Header
    target_precompile_headers(Game PRIVATE "${CMAKE_SOURCE_DIR}/Game/pch.h")
endif()



## EXTERNAL LIBRARIES / PROJECTS



# ENTT
 target_include_directories(Engine PUBLIC ${CMAKE_SOURCE_DIR}/external/Entt)
 target_include_directories(Game PUBLIC ${CMAKE_SOURCE_DIR}/external/Entt)
# SDL3
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/exterenal/SDL3/cmake")
find_package(SDL3 CONFIG REQUIRED)
target_link_libraries(Engine PRIVATE SDL3)
target_link_libraries(Game PRIVATE SDL3)
# Add Post-Build Command to move DLL next to the Executable
add_custom_command(TARGET Engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE:SDL3::SDL3>"
    "$<TARGET_FILE_DIR:Engine>"
)