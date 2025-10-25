#include "pch.h"
#include "AssetManager.h"

#include "../ResourceLoading/TextureLoader.h"
#include "../ResourceLoading/OBJ_Loader.h"

namespace CMP316engine {
	void AssetManager::GetResource()
	{

	}
	bool AssetManager::LoadAsset(std::string filePath)
	{
		std::filesystem::path path = filePath; 
		std::string extension = path.extension().string();

		static const std::unordered_map<std::string, AssetType> supportedExtensions = {
		{".png", AssetType::IMAGE},
		{".jpg", AssetType::IMAGE},
		{".jpeg", AssetType::IMAGE},
		{".tga", AssetType::IMAGE},
		{".bmp", AssetType::IMAGE},
		{".hdr", AssetType::IMAGE},
		{".obj", AssetType::MESH},
		{".wav", AssetType::AUDIO}
		};

		/// GET TYPE

		AssetType assetType = AssetType::NONE;
		auto iterator = supportedExtensions.find(extension);
		if (iterator != supportedExtensions.end())
		{
			assetType = iterator->second;
		}
		else
		{
			/// NO ASSET LOADED
			std::cout << "\nNo Loader set for passed in filetype or invalid filepath passed in" << " FILEPATH: " << filePath;
			return false;
		}

		/// LOAD ASSET

		switch (assetType)
		{
		case AssetType::IMAGE:
			//textures[filePath] = TextureLoader::LoadTexture(filePath.c_str(), renderer->GetDevice(), renderer->GetDeviceContext());
			break;
		case AssetType::MESH:
			LoadModel(filePath);
			break;
		case AssetType::AUDIO:

			break;
		}
		return true;
	}
	void AssetManager::UnloadAsset()
	{

	}
	void AssetManager::UnloadAssets()
	{

	}
	void AssetManager::UnloadAllAssets()
	{

	}

	void AssetManager::LoadModel(std::string filepath)
	{
	//	objl::Loader objLoader;
	//	bool success = objLoader.LoadFile(filepath);
	//	if (!success) { std::cout << "\nUnable to load or find .obj model"; return; }

	//	for (auto& loadedMesh : objLoader.LoadedMeshes)
	//	{
	//		meshComponent.meshes.push_back(CMP316engine::Mesh());
	//		auto& mesh = meshComponent.meshes.back();
	//		mesh.name = loadedMesh.MeshName;

	//		//// VERTICES
	//		for (auto& loadedVertex : loadedMesh.Vertices) {
	//			CMP316engine::Vertex vertex;
	//			vertex.position = DirectX::XMFLOAT3(loadedVertex.Position.X, loadedVertex.Position.Y, loadedVertex.Position.Z);
	//			vertex.normal = DirectX::XMFLOAT3(loadedVertex.Normal.X, loadedVertex.Normal.Y, loadedVertex.Normal.Z);
	//			vertex.uv = DirectX::XMFLOAT2(loadedVertex.TextureCoordinate.X, loadedVertex.TextureCoordinate.Y);

	//			mesh.vertices.push_back(vertex);
	//			mesh.vertices.back().uv.y = 1 - mesh.vertices.back().uv.y;
	//			//mesh.vertices.back().Normal = glm::normalize(mesh.vertices.back().Normal); // Is it obvious now that I stole this from an old project :P
	//			//mesh.vertices.back().Normal *= -1;
	//		}
	//		//// INDICES
	//		for (auto& index : loadedMesh.Indices) {
	//			mesh.indices.push_back(index);
	//		}
	//		std::reverse(mesh.indices.begin(), mesh.indices.end());

	//		//// MATERIALS
	//		//std::cout << "\nMaterial Name: " << loadedMesh.MeshMaterial.name;
	//		//std::cout << "\nMaterial mapb: " << loadedMesh.MeshMaterial.map_bump;
	//		//std::cout << "\nMaterial mapd: " << loadedMesh.MeshMaterial.map_d;
	//		//std::cout << "\nMaterial mapKa: " << loadedMesh.MeshMaterial.map_Ka;
	//		//std::cout << "\nMaterial mapKd: " << loadedMesh.MeshMaterial.map_Kd;
	//		//std::cout << "\nMaterial mapKs: " << loadedMesh.MeshMaterial.map_Ks;
	//		//std::cout << "\nMaterial mapNs: " << loadedMesh.MeshMaterial.map_Ns;

	//		//// TEXTURES

	//		const std::string textureName = loadedMesh.MeshMaterial.map_Kd;

	//		/// Get Model filepath, then replace obj name with texture name
	//		std::filesystem::path projectFilepath = std::filesystem::current_path();
	//		std::filesystem::path modelFilepath = modelComponent.filepath;
	//		std::filesystem::path modelDirectory = modelFilepath.parent_path();
	//		std::filesystem::path textureFilepath = modelDirectory / textureName;
	//		//std::wcout << L"\nTexture Filepath: " << textureFilepath; // DEBUG

	//		char textureFilepathChar[128];
	//		strcpy_s(textureFilepathChar, textureFilepath.string().c_str());
	//		Texture* texture = TextureLoader::LoadTexture(textureFilepathChar, renderer->GetDevice(), renderer->GetDeviceContext());
	//		meshComponent.textures.insert({ textureName, texture });
	//		mesh.textureName = textureName;
	//	}
	}
}