#include "pch.h"
#include "PhysicsSystem.h"

CMP316engine::PhysicsSystem::PhysicsSystem(entt::registry* sceneRegistry, CMP316engine::EngineContext* engineContext) : System(sceneRegistry, engineContext), physicsManager(engineContext->physicsManager.get())
{

}

bool CMP316engine::PhysicsSystem::Initialize()
{
	return true;
}

void CMP316engine::PhysicsSystem::Shutdown()
{

}

void CMP316engine::PhysicsSystem::Update(float deltaTime)
{
	auto physicsEntities = registry->view<RigidBodyComponent, TransformComponent>();
	for (auto& entity : physicsEntities) {
		auto [rigidBodyComponent, transformComponent] = registry->get<RigidBodyComponent, TransformComponent>(entity);

		/// INITIALIZATION

		if (rigidBodyComponent.bodyCreated == false)
		{
			auto& t = transformComponent;
			auto& r = rigidBodyComponent;

			/// GET GLOBAL TRANSFORMS

			XMVECTOR scale;
			XMVECTOR rotationQuat;
			XMVECTOR translation;
			XMMatrixDecompose(&scale, &rotationQuat, &translation, transformComponent.worldMatrix);
			XMFLOAT3 position; DirectX::XMStoreFloat3(&position, translation);

			/// CREATE BODY 

			JPH::BodyCreationSettings sphere_settings(new JPH::SphereShape(0.5f), JPH::RVec3(0.0f, 0.0f, 0.0f), JPH::Quat::sIdentity(), JPH::EMotionType::Dynamic, 1);
			r.physicsBodyHandle = physicsManager->GetBodyInterface().CreateAndAddBody(sphere_settings, JPH::EActivation::Activate);

			/// SET INITIAL VALUES TO STORED TRANSFORMS
			// Should update the physics body internal transforms to the transforms in the loaded scene.
			// E.g. Level editor sets the transforms for where entities should start, when scene is loaded 
			// in the game, it should ensure the physics bodies are placed at those positions.
			// After that, the internal physics transforms should take over and be setting the transformComponent.

			JPH::Vec3 bodyPosition = { position.x, position.y, position.z };
			physicsManager->GetBodyInterface().SetPosition(r.physicsBodyHandle, bodyPosition, JPH::EActivation::Activate);

			rigidBodyComponent.bodyCreated = true;
		}

		/// UPDATE TRANSFORMS

		JPH::RVec3 position = physicsManager->GetBodyInterface().GetPosition(rigidBodyComponent.physicsBodyHandle);
		JPH::Quat rotation = physicsManager->GetBodyInterface().GetRotation(rigidBodyComponent.physicsBodyHandle);
		transformComponent.position = DirectX::XMFLOAT3(position.GetX(), position.GetY(), position.GetZ());
		//transformComponent.rotation = DirectX::XMFLOAT4(rotation.GetX, rotation.GetY(), rotation.GetZ(), rotation.GetW());

		/// UPDATE VELOCITIES / FORCES

		if (auto moveComponent = registry->try_get<MovementComponent>(entity))
		{
			auto& m = moveComponent;
			JPH::Vec3 bodyVelocity = { m->linearVelocity.x, m->linearVelocity.y, m->linearVelocity.z };
			physicsManager->GetBodyInterface().SetLinearVelocity(rigidBodyComponent.physicsBodyHandle, bodyVelocity);
		}
	}
}
