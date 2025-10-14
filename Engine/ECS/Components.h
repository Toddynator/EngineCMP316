#pragma once
#include "entt.hpp"
#include <directxmath.h>
#include "../Graphics/Mesh.h"
#include "../Graphics/Texture.h"
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/Body.h>

using namespace entt::literals;

/*
Components should purely store data, only Systems should handle functionality.

COMPONENT IDEAS:
- ShaderMaterial component? Mesh objects without one use a default shader for rendering.
*/

namespace CMP316engine 
{
	///// REFLECTION

	enum Traits : uint16_t
	{
		EDITOR = 1 << 0, // Set to any reflected member variables that should have an editor control
		SERIALIZE = 1 << 1 // Set to any reflected member variables that should be serialized
	};
	using PropertiesMap = std::unordered_map<entt::id_type, entt::meta_any>;
	void InitializeReflection();

	///// COMPONENTS

	/*
	The component that all entities should use to define their position in the scene hierarchy.
	If I want propagating changes from parent to child, then this must be added to an entity.
	Important for a scene graph if I make a Level Editor.
	*/
	struct HierarchyComponent
	{
		std::string name = "Undefined";
		entt::entity parent = entt::null; // The root entity, that owns the current entity as a child.
		entt::entity firstChild = entt::null; // Entities store children, but they only point to the first child.
		entt::entity prevNeighbour = entt::null; // The previous adjacent entity with the same parent (if null then it is the first child).
		entt::entity nextNeighbour = entt::null; // The adjacent entity with the same parent (if null then it is the last child).
	};

	struct TransformComponent
	{
		DirectX::XMMATRIX worldMatrix{};
		DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0.f,0.f,0.f);
		DirectX::XMFLOAT3 rotation = DirectX::XMFLOAT3(0.f, 0.f, 0.f); // TODO: Probably should replace with Quaternions in the future.
		DirectX::XMFLOAT3 scale = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		DirectX::XMFLOAT3 origin = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		DirectX::XMFLOAT3 up = DirectX::XMFLOAT3(0.f, 1.f, 0.f);
		DirectX::XMFLOAT3 forward = DirectX::XMFLOAT3(0.f, 0.f, 1.f);
		DirectX::XMFLOAT3 right = DirectX::XMFLOAT3(1.f, 0.f, 0.f);

		float testFloatInTransform = 1.f;
	};

	struct ModelComponent
	{
		std::string filepath = ""; // This is the filepath from inside the data/Models directory!
		bool modelLoaded = false;
	};

	struct MeshComponent
	{
		std::vector<Mesh> meshes;
		ID3D11Buffer* vertexBuffer = nullptr;
		ID3D11Buffer* indexBuffer = nullptr;
		std::unordered_map<std::string, Texture*> textures; // TODO: Remove once AssetManager is created, mesh struct holds texture name, it looks up texture in unordered map (which will be in assetManager at some point). Shutdown then handled by assetManager, clean!
		bool meshNeedsCalculated = true; // Can be used for the initial mesh calculation at the start or for run-time recalculation of the mesh, e.g. a voxel mesh has lost some voxels!.

		int GetIndexCount()
		{
			int total = 0;
			for (auto& mesh : meshes) {
				total += static_cast<int>(mesh.indices.size());
			}
			return total;
		}
		int GetVertexCount()
		{
			int total = 0;
			for (auto& mesh : meshes) {
				total += static_cast<int>(mesh.vertices.size());
			}
			return total;
		}
	};

	struct CameraComponent
	{
		// Camera needs just position and rotation
		DirectX::XMMATRIX viewMatrix;
		bool active = false;
	};

	struct RigidBodyComponent
	{
		bool bodyCreated = false;
		JPH::BodyID physicsBodyHandle; // Hardcoded to Jolt for now
		//int physicsBodyHandle = -1;
	};

	struct MovementComponent
	{
		DirectX::XMFLOAT3 linearVelocity; // CURRENTLY JUST ONE VELOCITY ALL DIRECTIONS, TODO: More settings
	};
}

