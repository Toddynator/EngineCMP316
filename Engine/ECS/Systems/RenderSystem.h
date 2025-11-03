
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
		TextureShader* textureShader;

	public:
		RenderSystem(entt::registry* sceneRegistry, EngineContext* engineContext, Renderer_DirectX11* sceneRenderer, TextureShader* sceneDefaultShader) : System(sceneRegistry, engineContext), assetManager(engineContext->assetManager.get()), renderer(sceneRenderer), textureShader(sceneDefaultShader) {}

		bool Initialize() override;
		void Shutdown() override;

		void Update(float deltaTime) override;

		static void RenderModels(entt::registry* sceneRegistry, Renderer_DirectX11* sceneRenderer, AssetManager* assetManager, TextureShader* shader, DirectX::XMMATRIX viewMatrix);

	private:
		//void calculateVoxelMesh(MeshComponent& meshComponent, VoxelComponent& voxelComponent);
		void loadModel(ModelComponent& modelComponent, MeshComponent& meshComponent);
		void calculateBuffers(MeshComponent& meshComponent);
		void shutdownBuffers();
	};
}
