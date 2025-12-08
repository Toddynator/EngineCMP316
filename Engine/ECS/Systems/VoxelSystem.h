#pragma once
#include "ECS/System.h"
#include "Graphics/Renderer_DirectX11.h"

namespace CMP316engine {
	class VoxelSystem : public System
	{
	private:
		AssetManager* assetManager;

	public:
		VoxelSystem(entt::registry* sceneRegistry, EngineContext* engineContext) : System(sceneRegistry, engineContext), assetManager(engineContext->assetManager.get()) {}

		bool Initialize() override;
		void Shutdown() override;

		void Update(float deltaTime) override;

	private:
		void loadVoxelModel(VoxelComponent& voxelComponent);
		void calculateVoxelMesh(VoxelComponent& voxelComponent, MeshComponent& meshComponent);
	};
}
