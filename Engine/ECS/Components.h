#pragma once
#include "entt.hpp"
#include <directxmath.h>
#include "../Graphics/Mesh.h"
#include "../Graphics/Texture.h"

/*
Components should purely store data, only Systems should handle functionality.

COMPONENT IDEAS:
- ShaderMaterial component? Mesh objects without one use a default shader for rendering.
*/

namespace CMP316engine 
{
	/*
	The component that all entities should use to define their position in the scene hierarchy.
	If I want propagating changes from parent to child, then this must be added to an entity.
	Important for a scene graph if I make a Level Editor.
	*/
	struct HierarchyComponent
	{
		entt::entity parent = entt::null;
	};

	struct TransformComponent
	{
		DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0.f,0.f,0.f);
		DirectX::XMFLOAT3 rotation = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		DirectX::XMFLOAT3 scale = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		DirectX::XMFLOAT3 origin = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		DirectX::XMFLOAT3 upVector = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		DirectX::XMFLOAT3 forwardVector = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		DirectX::XMFLOAT3 rightVector = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	};

	struct ModelComponent
	{
		std::string filepath = ""; // This is the filepath from inside the data/Models directory!
		bool modelLoaded = false;
	};

	struct MeshComponent
	{
		std::vector<Mesh> meshes;
		DirectX::XMMATRIX worldMatrix{};
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

	/*
	Most important role this component serves is to define which entity the player is. When systems see this component, they can define the player functionality.
	*/
	struct PlayerComponent
	{
		int playerNumber;
	};
}

