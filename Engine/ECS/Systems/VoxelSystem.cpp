#include "VoxelSystem.h"
#include "Utility/VoxelHelper.h"
#include <chrono>

/// BITWISE GREEDY MESHER ~ TODO
//#define BM_IMPLEMENTATION
//#include "mesher.h"


bool CMP316engine::VoxelSystem::Initialize()
{
	return true;
}

void CMP316engine::VoxelSystem::Shutdown()
{

}

void CMP316engine::VoxelSystem::Update(float deltaTime)
{
	auto voxelEntities = registry->view<VoxelComponent, MeshComponent>();
	for (auto& entity : voxelEntities) {
		auto [voxelComponent, meshComponent] = registry->get<VoxelComponent, MeshComponent>(entity);

		if (voxelComponent.voxelModelLoaded == false) { loadVoxelModel(voxelComponent); }
		if (voxelComponent.voxelMeshNeedsCalculated) { calculateVoxelMesh(voxelComponent, meshComponent); }
	}
}

void CMP316engine::VoxelSystem::loadVoxelModel(VoxelComponent& voxelComponent)
{
	if (VoxelResource* voxelModel = assetManager->GetResource<VoxelResource>(voxelComponent.filepath))
	{
		voxelComponent.voxelResource = *voxelModel;
		voxelComponent.voxelMeshNeedsCalculated = true;
	}
	voxelComponent.voxelModelLoaded = true; // Set regardless of whether it was successful, it is more about checking ONCE. If I keep attempting this will become very laggy.
}

void CMP316engine::VoxelSystem::calculateVoxelMesh(VoxelComponent& voxelComponent, MeshComponent& meshComponent)
{
	meshComponent.meshes.clear();
	auto& meshStruct = meshComponent.meshes.emplace_back();
	meshStruct.textureName = voxelComponent.filepath; // Since I used the filepath of the voxel model to store the texture. Notably voxel models typically only have one texture (pallete).
	auto& voxels = voxelComponent.voxelResource.voxels;
	auto& modelSize = voxelComponent.voxelResource.modelSize;
	Vector3Int halfModelSizeOffset = { modelSize.x / 2, modelSize.y / 2, modelSize.z / 2 }; // So that I can centre the mesh
	auto start = std::chrono::high_resolution_clock::now();

	/// NAIVE MESHING 
	/* TODO
	* Culling meshing (Don't generate quads on hidden voxels)
	* Greedy Meshing (Combine faces to save on vertices and indices)
	* Multi-Threading (Voxels are perfect for this, the benefit should be pretty massive)
	* Bitwise Greedy Meshing ~ I can either try and make my own, or I use an existing bitwise greedy mesher, but this would require formatting my voxels correctly (which may be for the best).
	*/

	for (int i = 0; i < voxels.size(); i++)
	{
		auto& voxel = voxels[i];
		if (voxel == 0) { continue; }

		Vector3Int position = VoxelHelper::ConvertIndexTo3DPosition(i, modelSize) - halfModelSizeOffset;
		VoxelHelper::GenerateVoxelFaceVertices(voxel, position, meshStruct.vertices, meshStruct.indices, VoxelHelper::VoxelFace::Front);
		VoxelHelper::GenerateVoxelFaceVertices(voxel, position, meshStruct.vertices, meshStruct.indices, VoxelHelper::VoxelFace::Back);
		VoxelHelper::GenerateVoxelFaceVertices(voxel, position, meshStruct.vertices, meshStruct.indices, VoxelHelper::VoxelFace::Left);
		VoxelHelper::GenerateVoxelFaceVertices(voxel, position, meshStruct.vertices, meshStruct.indices, VoxelHelper::VoxelFace::Right);
		VoxelHelper::GenerateVoxelFaceVertices(voxel, position, meshStruct.vertices, meshStruct.indices, VoxelHelper::VoxelFace::Top);
		VoxelHelper::GenerateVoxelFaceVertices(voxel, position, meshStruct.vertices, meshStruct.indices, VoxelHelper::VoxelFace::Bottom);
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto ns = duration_cast<std::chrono::nanoseconds>(end - start).count();
	auto ms = duration_cast<std::chrono::milliseconds>(end - start).count();
	std::cout << "\nNaive Voxel Mesh Calculation Time: " << ns << " ns (" << ms << " ms)";

	voxelComponent.voxelMeshNeedsCalculated = false;
	meshComponent.meshNeedsCalculated = true;
}
