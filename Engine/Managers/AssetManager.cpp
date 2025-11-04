#include "pch.h"
#include "AssetManager.h"
#include "ResourceLoading/TextureLoader.h"
#include "ResourceLoading/OBJ_Loader.h"
#include "ResourceLoading/VoxImporter.h"

namespace CMP316engine {
	bool AssetManager::Initialize(ID3D11Device* rendererDevice, ID3D11DeviceContext* rendererDeviceContext) 
	{ 
		device = rendererDevice; 
		deviceContext = rendererDeviceContext; 
		LoadAsset("data/Textures/default.png");
		return true; 
	}

	bool AssetManager::LoadAsset(std::string filepath)
	{
		std::filesystem::path path = filepath;
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
			//std::cout << "\nNo Loader set for passed in filetype or invalid filepath passed in" << " FILEPATH: " << filePath;
			return false;
		}

		/// LOAD ASSET

		switch (assetType)
		{
		case AssetType::IMAGE:
		{
			textures[filepath] = TextureLoader::LoadTexture(filepath.c_str(), device, deviceContext);
			break;
		}
		case AssetType::MESH:
		{
			return LoadModel(filepath);
			break;
		}
		case AssetType::AUDIO:
		{
			/// TODO
			break;
		}
		case AssetType::SHADER:
		{
			/// TODO
			break;
		}
		}
		return true;
	}
	void AssetManager::UnloadAsset(std::string filepath, AssetType assetType)
	{
		switch (assetType)
		{
		case AssetType::IMAGE:
		{
			auto iterator = textures.find(filepath);
			if (iterator != textures.end())
			{
				textures.erase(iterator);
			}
			break;
		}
		case AssetType::MESH:
		{
			auto iterator = models.find(filepath);
			if (iterator != models.end())
			{
				models.erase(iterator);
			}
			break;
		}
		case AssetType::AUDIO:
		{
			/// TODO
			break;
		}
		case AssetType::SHADER:
		{
			/// TODO
			break;
		}
		}
	}
	void AssetManager::UnloadAssets(AssetType assetType)
	{
		switch (assetType)
		{
		case AssetType::IMAGE:
			textures.clear();
			break;
		case AssetType::MESH:
			models.clear();
			break;
		case AssetType::AUDIO:
			/// TODO
			break;
		case AssetType::SHADER:
			/// TODO
			break;
		}
	}
	void AssetManager::UnloadAllAssets()
	{
		textures.clear();
		models.clear();
		/// AUDIO TODO
		/// SHADERS TODO
	}

	bool AssetManager::LoadModel(std::string filepath)
	{
		objl::Loader objLoader;
		bool success = objLoader.LoadFile(filepath);
		if (!success) { std::cout << "\nUnable to load or find .obj model"; return false; }

		std::vector<Mesh> model;
		for (auto& loadedMesh : objLoader.LoadedMeshes)
		{
			model.push_back(CMP316engine::Mesh());
			auto& mesh = model.back();
			mesh.name = loadedMesh.MeshName;

			//// VERTICES
			for (auto& loadedVertex : loadedMesh.Vertices) {
				CMP316engine::Vertex vertex;
				vertex.position = DirectX::XMFLOAT3(loadedVertex.Position.X, loadedVertex.Position.Y, loadedVertex.Position.Z);
				vertex.normal = DirectX::XMFLOAT3(loadedVertex.Normal.X, loadedVertex.Normal.Y, loadedVertex.Normal.Z);
				vertex.uv = DirectX::XMFLOAT2(loadedVertex.TextureCoordinate.X, loadedVertex.TextureCoordinate.Y);

				mesh.vertices.push_back(vertex);
				mesh.vertices.back().uv.y = 1 - mesh.vertices.back().uv.y;
				DirectX::XMVECTOR adjustedNormal = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&mesh.vertices.back().normal));
				adjustedNormal = DirectX::XMVectorScale(adjustedNormal, -1);
				DirectX::XMStoreFloat3(&mesh.vertices.back().normal, adjustedNormal);
			}
			//// INDICES
			for (auto& index : loadedMesh.Indices) {
				mesh.indices.push_back(index);
			}
			std::reverse(mesh.indices.begin(), mesh.indices.end());

			//// MATERIALS
			//std::cout << "\nMaterial Name: " << loadedMesh.MeshMaterial.name;
			//std::cout << "\nMaterial mapb: " << loadedMesh.MeshMaterial.map_bump;
			//std::cout << "\nMaterial mapd: " << loadedMesh.MeshMaterial.map_d;
			//std::cout << "\nMaterial mapKa: " << loadedMesh.MeshMaterial.map_Ka;
			//std::cout << "\nMaterial mapKd: " << loadedMesh.MeshMaterial.map_Kd;
			//std::cout << "\nMaterial mapKs: " << loadedMesh.MeshMaterial.map_Ks;
			//std::cout << "\nMaterial mapNs: " << loadedMesh.MeshMaterial.map_Ns;

			//// TEXTURES

			const std::string textureName = loadedMesh.MeshMaterial.map_Kd;
			if (textureName != "")
			{
				/// Get Model filepath, then replace obj name with texture name
				std::filesystem::path projectFilepath = std::filesystem::current_path();
				std::filesystem::path modelFilepath = filepath;
				std::filesystem::path modelDirectory = modelFilepath.parent_path();
				std::filesystem::path textureFilepath = modelDirectory / textureName;
				std::wcout << L"\nTexture Filepath: " << textureFilepath; // DEBUG

				char textureFilepathChar[128];
				strcpy_s(textureFilepathChar, textureFilepath.string().c_str());
				Texture* texture = GetResource<Texture>(textureFilepathChar);
				mesh.textureName = textureFilepath.string();
			}
		}

		models[filepath] = model;

		return true;
	}
}