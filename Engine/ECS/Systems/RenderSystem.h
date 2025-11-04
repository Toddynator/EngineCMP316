
#pragma once
#include "ECS/System.h"
#include "Graphics/Renderer_DirectX11.h"
#include "Graphics/Shaders/TextureShader.h"

namespace CMP316engine {
	class RenderSystem : public System
	{
	private:
		AssetManager* assetManager;
		Renderer_DirectX11* renderer;

	public:
		RenderSystem(entt::registry* sceneRegistry, EngineContext* engineContext, Renderer_DirectX11* sceneRenderer) : System(sceneRegistry, engineContext), assetManager(engineContext->assetManager.get()), renderer(sceneRenderer) {}

		bool Initialize() override;
		void Shutdown() override;

		void Update(float deltaTime) override;

		static void RenderModels(entt::registry* sceneRegistry, Renderer_DirectX11* sceneRenderer, AssetManager* assetManager, LightShader* shader, DirectX::XMMATRIX viewMatrix, DirectX::XMFLOAT3 cameraPosition);

	private:
		//void calculateVoxelMesh(MeshComponent& meshComponent, VoxelComponent& voxelComponent);
		void loadModel(ModelComponent& modelComponent, MeshComponent& meshComponent);
		void calculateBuffers(MeshComponent& meshComponent);
		void shutdownBuffers();
	};
}
