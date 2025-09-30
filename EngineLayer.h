
#pragma once
#include <iostream>
#include <Windows.h>
#include <filesystem>
#include <memory.h>

#include "Application.h"
#include "TempApplication.h"

#include "InputManager.h"
#include "TimeManager.h"
#include "AudioManager.h"
#include "AudioManager_SoLoud.h"
#include "WindowManager.h"
#include "WindowManager_SDL.h"
#include "RenderManager.h"

#include <SDL3/SDL.h>

#include <../ImGui/imgui.h>
#include "ImGui/imgui_impl_sdl3.h"
#include "ImGui/imgui_impl_dx11.h"

#include "d3dclass.h" // RENDERER
#include "Camera.h"
#include "Model.h"
#include "Shader.h"

// Jolt Includes
#include "Jolt/Jolt.h" // Must be included anytime other jolt libraries are included ~ Recommended to be in the precompiled header
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>
//#include <Jolt/Physics/Collision/Shape/ConvexShape.h> // I added this in an attempt to fix a linker issue, not sure if it helps
// JOLT STL Includes // Probably redundant but playing it safe here.
#include <iostream>
#include <cstdarg>
#include <thread>

//// PHYSICS TEST

// Layer that objects can be in, determines which other objects it can collide with
// Typically you at least want to have 1 layer for moving bodies and 1 layer for static bodies, but you can have more
// layers if you want. E.g. you could have a layer for high detail collision (which is not used by the physics simulation
// but only if you do collision testing).
namespace Layers
{
	static constexpr JPH::ObjectLayer NON_MOVING = 0;
	static constexpr JPH::ObjectLayer MOVING = 1;
	static constexpr JPH::ObjectLayer NUM_LAYERS = 2;
};

/// Class that determines if two object layers can collide
class ObjectLayerPairFilterImpl : public JPH::ObjectLayerPairFilter
{
public:
	virtual bool					ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const override
	{
		switch (inObject1)
		{
		case Layers::NON_MOVING:
			return inObject2 == Layers::MOVING; // Non moving only collides with moving
		case Layers::MOVING:
			return true; // Moving collides with everything
		default:
			JPH_ASSERT(false);
			return false;
		}
	}
};

// Each broadphase layer results in a separate bounding volume tree in the broad phase. You at least want to have
// a layer for non-moving and moving objects to avoid having to update a tree full of static objects every frame.
// You can have a 1-on-1 mapping between object layers and broadphase layers (like in this case) but if you have
// many object layers you'll be creating many broad phase trees, which is not efficient. If you want to fine tune
// your broadphase layers define JPH_TRACK_BROADPHASE_STATS and look at the stats reported on the TTY.
namespace BroadPhaseLayers
{
	static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
	static constexpr JPH::BroadPhaseLayer MOVING(1);
	static constexpr JPH::uint NUM_LAYERS(2);
};

// BroadPhaseLayerInterface implementation
// This defines a mapping between object and broadphase layers.
class BPLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface
{
public:
	BPLayerInterfaceImpl()
	{
		// Create a mapping table from object to broad phase layer
		mObjectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
		mObjectToBroadPhase[Layers::MOVING] = BroadPhaseLayers::MOVING;
	}

	virtual JPH::uint GetNumBroadPhaseLayers() const override
	{
		return BroadPhaseLayers::NUM_LAYERS;
	}

	virtual JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override
	{
		JPH_ASSERT(inLayer < Layers::NUM_LAYERS);
		return mObjectToBroadPhase[inLayer];
	}

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
	virtual const char* GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const override
	{
		switch ((JPH::BroadPhaseLayer::Type)inLayer)
		{
		case (JPH::BroadPhaseLayer::Type)BroadPhaseLayers::NON_MOVING:	return "NON_MOVING";
		case (JPH::BroadPhaseLayer::Type)BroadPhaseLayers::MOVING:		return "MOVING";
		default:													JPH_ASSERT(false); return "INVALID";
		}
	}
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

private:
	JPH::BroadPhaseLayer					mObjectToBroadPhase[Layers::NUM_LAYERS];
};

/// Class that determines if an object layer can collide with a broadphase layer
class ObjectVsBroadPhaseLayerFilterImpl : public JPH::ObjectVsBroadPhaseLayerFilter
{
public:
	virtual bool				ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override
	{
		switch (inLayer1)
		{
		case Layers::NON_MOVING:
			return inLayer2 == BroadPhaseLayers::MOVING;
		case Layers::MOVING:
			return true;
		default:
			JPH_ASSERT(false);
			return false;
		}
	}
};

//// PHYSICS TEST END

class EngineLayer
{
public:
	EngineLayer() {};
	~EngineLayer() = default;

	bool Initialize();
	void Run();
	void Shutdown();

private:
	bool processEvents();
	void Update();
	void Render();

	/// STUFF TO MOVE TO MANAGERS
	bool createRenderer(HWND hwnd); // Note: In the future this may need carefully handled by a platformManager of sorts, as HWND won't be relevant to non-windows OS platforms.

private:
	std::unique_ptr<CMP316engine::Application> application = nullptr; // The actual game

	/// MANAGERS
	std::unique_ptr<CMP316engine::InputManager> inputManager = nullptr;
	std::unique_ptr<CMP316engine::TimeManager> timeManager = nullptr;
	std::unique_ptr<CMP316engine::WindowManager> windowManager = nullptr;
	std::unique_ptr<CMP316engine::AudioManager> audioManager = nullptr;
	//std::unique_ptr<CMP316engine::RenderManager> renderManager = nullptr;

	/// GRAPHICS
	// NOTE: Move to Application class?
	std::unique_ptr<D3DClass> renderer = nullptr;
	std::unique_ptr<Shader> shader = nullptr;

	/// SCENE
	// NOTE: Temp, should be in scene class only
	std::unique_ptr<Camera> camera = nullptr;
	std::unique_ptr<CMP316engine::Model> model = nullptr;

	/// TESTING VARIABLES
	// NOTE: Temp, just for testing
	bool audioPlayed = false;
	bool physicsTested = false;

	bool wireframeEnabled = false;
	const bool VSYNC_ENABLED = false; // Locks to 60fps ~ Renderer
	const float SCREEN_DEPTH = 1000.0f;
	const float SCREEN_NEAR = 0.3f;
};

