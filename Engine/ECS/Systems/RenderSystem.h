#pragma once
#include "../System.h"

namespace CMP316engine {
	class RenderSystem : public System
	{
	public:
		RenderSystem(entt::registry* sceneRegistry) : System(sceneRegistry) {}

		bool Initialize() override;
		void Shutdown() override;

		void Update() override;

	private:
		void calculateWorldMatrix(TransformComponent& transformComponent, MeshComponent& meshComponent);
		//void calculateVoxelMesh(MeshComponent& meshComponent, VoxelComponent& voxelComponent);
		void loadModel(ModelComponent& modelComponent, MeshComponent& meshComponent);
		void calculateBuffers();
		void shutdownBuffers();
	};
}
