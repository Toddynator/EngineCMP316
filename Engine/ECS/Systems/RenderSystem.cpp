#include "pch.h"
#include "RenderSystem.h"

bool CMP316engine::RenderSystem::Initialize()
{
	return true;
}

void CMP316engine::RenderSystem::Shutdown()
{
	shutdownBuffers();
}

void CMP316engine::RenderSystem::Update()
{
	auto modelEntities = registry->view<ModelComponent, MeshComponent>();
	for (auto& entity : modelEntities) {
		auto [modelComponent, meshComponent] = registry->get<ModelComponent, MeshComponent>(entity);

		if (modelComponent.modelLoaded == false) { loadModel(modelComponent, meshComponent); }
	}

	auto transformableMeshEntities = registry->view<TransformComponent, MeshComponent>();
	for (auto& entity : transformableMeshEntities) {
		auto [transformComponent, meshComponent] = registry->get<TransformComponent, MeshComponent>(entity);

		calculateWorldMatrix(transformComponent, meshComponent);
	}

	auto meshEntities = registry->view<MeshComponent>();
	for (auto& entity : meshEntities) {
		auto meshComponent = registry->get<MeshComponent>(entity);

		if (meshComponent.meshNeedsCalculated) { calculateBuffers(); }
	}
}

void CMP316engine::RenderSystem::calculateWorldMatrix(TransformComponent& transformComponent, MeshComponent& meshComponent)
{
	auto& position = transformComponent.position;
	auto& rotation = transformComponent.rotation;
	DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	meshComponent.worldMatrix = rotationMatrix * translationMatrix;
}

void CMP316engine::RenderSystem::loadModel(ModelComponent& modelComponent, MeshComponent& meshComponent)
{
	modelComponent.modelLoaded = true;
	meshComponent.meshNeedsCalculated = false;
}

void CMP316engine::RenderSystem::calculateBuffers()
{

}

void CMP316engine::RenderSystem::shutdownBuffers()
{
	auto meshEntities = registry->view<MeshComponent>();
	for (auto& entity : meshEntities) {
		auto meshComponent = registry->get<MeshComponent>(entity);

		if (meshComponent.vertexBuffer) { meshComponent.vertexBuffer->Release(); }
		if (meshComponent.indexBuffer) { meshComponent.indexBuffer->Release(); }
		meshComponent.vertexBuffer = nullptr;
		meshComponent.indexBuffer = nullptr;
	}
}