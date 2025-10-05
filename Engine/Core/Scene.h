/*
Where levels, aka scenes will go inside an application.
Scenes will handle the 'Scene Tree' where all GameObjects are stored.

Current plan is that games will simply override this and create their own definitions of levels.
However,
may consider having levels be entirely serialized, and created through an engine level editor, in which case you would only need one scene class for all levels in a game.



CURRENTLY: Passes in Engine Context via constructor, means derivations need to create their own constructor (boilerplate). 
However I could use a setter and have a sceneManager handle this in the future.
*/

#pragma once
#include "EngineContext.h"
#include <entt.hpp>
#include "../ECS/GameObject.h"
#include "../ECS/System.h"

namespace CMP316engine {
	class Scene
	{
	public:
		explicit Scene(EngineContext& context) : engineContext(context) {}
		Scene() = delete;  // MUST pass the context in ~ Otherwise Game will have no access to the engines systems
		~Scene() = default;

		virtual bool Initialize() = 0;
		virtual void Shutdown() = 0;

		virtual void HandleInput() = 0;
		virtual void HandleImGui() = 0;
		virtual void Update(float deltaTime) = 0;
		virtual void Render() = 0;

	protected:
		EngineContext& engineContext;
		entt::registry registry;
		std::unique_ptr<GameObject> sceneTree; // All scenes have a single root object, it has children which as a result forms the hierarchy / scene graph.
		std::vector<std::unique_ptr<System>> systems;
	};
}

