/*
HOW TO USE:
- ALWAYS INCLUDE BASE IMPLEMENTATION IN DERIVED CLASS FOR ALL OVERRIDABLE FUNCTIONS. 
- INITIALIZATION SHOULD ALWAYS BE CALLED FIRST.
*/

#pragma once
#include "Scene.h"
#include <entt.hpp>
#include "../ECS/System.h"

namespace CMP316engine {
	class ECSScene :
		public Scene
	{
	public:
		ECSScene(CMP316engine::EngineContext& context);

		bool Initialize() override;
		void Shutdown() override;

		void HandleInput() override;
		void HandleImGui() override;
		void Update(float deltaTime) override;
		void Render() override;

	protected:
		entt::registry registry;
		entt::entity sceneRoot; // All scenes have a single root object, it has children which as a result forms the hierarchy / scene graph.
		std::vector<std::unique_ptr<System>> systems;
	};
}
