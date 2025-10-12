/*
Precompiled Header
* INCLUDE AT THE VERY START OF ALL CPP FILES, BEFORE OTHER INCLUDES
* INCLUDE ANY FILES THAT ARE UNLIKELY TO CHANGE
* The header of files will still need to include the listed libraries if anything in 
the header uses them, but in building it will be using the precompiled header anyway.
*/

#pragma once
#include <memory>
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <functional>
#include <optional>

#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <SDL3/SDL.h>
#include <ImGui.h>
#include <entt.hpp>
#include <ImGui/ImGuizmo/ImGuizmo.h>