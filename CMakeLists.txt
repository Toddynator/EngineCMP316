
## NOTES:
#  - Need to move the source_group files into exactly 'Source Files' and 'Header Files' to match VS2022's style of filter.
# That will require me sorting the .h and .cpp files into an include and src folder in the Engine directory.

cmake_minimum_required(VERSION 4.0)

project(
    Engine
    DESCRIPTION "Game Engine Specialized on Voxels, created for CMP316 University Module"
    HOMEPAGE_URL "https://github.com/Toddynator/EngineCMP316"
    LANGUAGES C CXX
)
file(GLOB_RECURSE ENGINE_SRC CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/Engine/*.cpp"
    "${CMAKE_SOURCE_DIR}/Engine/*.h"
)



# IDE Project will use the same Folder Structure as found in the source files (If the IDE supports it)
source_group(TREE "${CMAKE_SOURCE_DIR}/Engine" PREFIX "src" FILES ${ENGINE_SRC})
# C++ Language Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# C Language Standard
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
# Install directory, whatever that is
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install")
# Remove Unused Configurations (Use only Release & Debug)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)



add_executable(${PROJECT_NAME} ${ENGINE_SRC})
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/Engine")
# C/C++ Version
target_compile_features(Engine PUBLIC cxx_std_17)
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