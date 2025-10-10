## NOTE:
# - Cmake makes its own precompiled header which it force includes, don't need to add pch.h to the beginning of every file!
# I still have a pch file, but their precompiled header now just includes that on its own, and I define in the file itself the includes.

## TODO:
# - Remove my precompiled header and replace with target_precompiled_header entirely, just pass in the includes instead of my own pch file.
# - Should make it that the game library name can be set on build through the cmake gui.



cmake_minimum_required(VERSION 3.29)
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
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
# Set the output directory for all binaries
set(OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
# Set the output directory to sort by platform
foreach(config DEBUG RELEASE)
    # Sorts by the different types of binaries
    # DLL, EXE
    foreach(var CMAKE_LIBRARY_OUTPUT_DIRECTORY_${config} CMAKE_RUNTIME_OUTPUT_DIRECTORY_${config})
        set(${var} "${OUTPUT_DIRECTORY}/${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}-${config}")
        string(TOLOWER "${${var}}" ${var})
    endforeach()

    # Lib's should stay in build folder, not needed for shipping / after the executable has been generated.
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${config} "${CMAKE_SOURCE_DIR}/out/lib/${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}-${config}")
endforeach()
# Install directory, whatever that is
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install")
# Remove Unused Configurations (Use only Release & Debug)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)



## ENGINE



# Get the Enginee Source Code
file(GLOB_RECURSE ENGINE_SRC CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/Engine/*.cpp"
    "${CMAKE_SOURCE_DIR}/Engine/*.h"
)
# IDE Project will use the same Folder Structure as found in the source files (If the IDE supports it)
# In VS2022 for example, this ONLY applies to CMake Targets view. But it is unlikely I will use that.
source_group(TREE "${CMAKE_SOURCE_DIR}/Engine" PREFIX "src" FILES ${ENGINE_SRC})
# Create the project file
add_executable(${PROJECT_NAME} ${ENGINE_SRC})
# Include the Engine Source Code
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/Engine")
# Precompiled Header
target_precompile_headers(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/Engine/Core/pch.h")
# Copy all data / resources into bin folder with the executable
add_custom_command(
    TARGET Engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/data"
        "$<TARGET_FILE_DIR:Engine>/data"
)



## GAME



# Create game project if it doesn't exist
if (NOT EXISTS "${CMAKE_SOURCE_DIR}/Game")
    message(WARNING "No Game Folder Found, Generating new Game Project")
    # Create the Game directory
    file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/Game")
    # Create a basic application implementation
    file(WRITE "${CMAKE_SOURCE_DIR}/Game/Game.h" [[
        #pragma once
        #include "Core/Application.h"
        
        class Game
        	: public CMP316engine::Application
        {
        public:
        	Game(CMP316engine::EngineContext& context) : Application(context) {}
        
        	bool Initialize() override { return true; }
        	void Shutdown() override {}
        
        	void HandleInput() override {}
        	void HandleImGui() override {}
        	void Update(float deltaTime) override {}
        	void Render() override {}
        
        private:
        	bool wireframeEnabled = false;
        };
    ]])
    file(WRITE "${CMAKE_SOURCE_DIR}/Game/Game.cpp" [[
        #include "pch.h"
        #include "Game.h"
        std::unique_ptr<CMP316engine::Application> CMP316engine::CreateApp(CMP316engine::EngineContext& engineContext)
        {
        	return std::make_unique<Game>(engineContext);
        }
    ]])
    file(WRITE "${CMAKE_SOURCE_DIR}/Game/pch.h" [[
        #pragma once
        #include <string>
        #include <unordered_map>
    ]])
    file(WRITE "${CMAKE_SOURCE_DIR}/Game/pch.cpp" [[
        #include "pch.h"
    ]])
endif()
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
# Precompiled Header
target_precompile_headers(Game PRIVATE "${CMAKE_SOURCE_DIR}/Game/pch.h") # CMAKE uses its own precompiled header and this just sets what it contains, but I've just leveraged my pch so it practically works the same.



## EXTERNAL LIBRARIES / PROJECTS



## ENTT
target_include_directories(Engine PUBLIC ${CMAKE_SOURCE_DIR}/external/Entt/include)
target_include_directories(Game PUBLIC ${CMAKE_SOURCE_DIR}/external/Entt/include)
## JOLT
add_subdirectory("external/Jolt" EXCLUDE_FROM_ALL)
## SDL3
# Force SDL3 to build as a static library
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_TEST OFF CACHE BOOL "" FORCE)
set(SDL_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory("external/SDL3" EXCLUDE_FROM_ALL)
## IMGUI
add_subdirectory("external/ImGui" EXCLUDE_FROM_ALL)
## SOLOUD
set(SOLOUD_BACKEND_NULL OFF CACHE BOOL "" FORCE)
set(SOLOUD_BACKEND_SDL2 OFF CACHE BOOL "" FORCE)
set(SOLOUD_BACKEND_WITH_MINIAUDIO ON CACHE BOOL "" FORCE)
add_subdirectory("external/SoLoud/contrib" EXCLUDE_FROM_ALL) 
# Add the MiniAudio files to the SoLoud project (Its CMake file doesn't do this)
target_sources(soloud PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/external/SoLoud/src/backend/miniaudio/soloud_miniaudio.cpp")
# Set SoLoud to MiniAudio
target_compile_definitions(soloud PUBLIC WITH_MINIAUDIO)

# Link libraries to game and engine project
target_link_libraries(Engine
    PRIVATE Jolt
    PRIVATE SDL3-static
    PRIVATE ImGui
    PRIVATE soloud
)
target_link_libraries(Game
    PRIVATE Jolt
    PRIVATE SDL3-static
    PRIVATE ImGui
    PRIVATE soloud
)
# Manually add soLoud, which doesn't seem to link normally
target_include_directories(Engine PUBLIC  ${CMAKE_SOURCE_DIR}/external/SoLoud/include)

# Move the external projects into folders to keep the main solution clean.
set_target_properties(Jolt PROPERTIES FOLDER "external")
set_target_properties(SDL3-static PROPERTIES FOLDER "external")
set_target_properties(SDL_uclibc PROPERTIES FOLDER "external")
set_target_properties(SDL3_test PROPERTIES FOLDER "external")
set_target_properties(ImGui PROPERTIES FOLDER "external")
set_target_properties(soloud PROPERTIES FOLDER "external")