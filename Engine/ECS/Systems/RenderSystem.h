#pragma once
#include "../System.h"
#include "../../Graphics/Renderer_DirectX11.h"
#include "../../Graphics/Shader.h"

namespace CMP316engine {
	class RenderSystem : public System
	{
	private:
		Renderer_DirectX11* renderer;
		Shader* shader;

	public:
		RenderSystem(entt::registry* sceneRegistry, Renderer_DirectX11* sceneRenderer, Shader* sceneDefaultShader) : System(sceneRegistry), renderer(sceneRenderer), shader(sceneDefaultShader) {}

		bool Initialize() override;
		void Shutdown() override;

		void Update() override;

		static void RenderModels(entt::registry* sceneRegistry, Renderer_DirectX11* sceneRenderer, Shader* shader, DirectX::XMMATRIX viewMatrix);

	private:
		void calculateWorldMatrix(TransformComponent& transformComponent, MeshComponent& meshComponent);
		//void calculateVoxelMesh(MeshComponent& meshComponent, VoxelComponent& voxelComponent);
		void loadModel(ModelComponent& modelComponent, MeshComponent& meshComponent);
		void calculateBuffers(MeshComponent& meshComponent);
		void shutdownBuffers();
	};
}
