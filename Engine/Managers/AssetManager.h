/*
Stores in Memory any assets loaded as a resource in an engine readable format.
When loading an asset, it will automatically determine what type of asset it is.
And create a corresponding engine resource to store it in memory.
E.g. an .obj file will be loaded via an obj loader and then stored as an engine Model, which contains the engines definition of vertices, etc.
If the asset that is retrieved hasn't been loaded yet, then it will load the asset automatically.
It also provides functionality to preload an asset so that there are no lagspikes during a game, and the ability
to unload assets to free up memory.

IMPORTANT:
- I've designed this so that the application developer chooses how and when to load their assets.
They can choose to let assets load during runtime as they are requested, load them all at once at the very start OR 
load some at a time, and unload at checkpoints of their choosing.

ASSET TYPES:
- Models
- Textures
- Audio
- Shaders
- Voxels

TODO:
- Currently looking at Entt for resource caching, as I like the idea that I could have
storages specific to the type of data automatically created via entt's hashing system (however it works).
I then just use a quick lookup to retrieve the data. Will probably still need templating to handle return types.
https://github.com/skypjack/entt/wiki/Resource-management
- Also this link: https://gamedev.stackexchange.com/questions/97747/c-exensible-resource-manager-class-with-dynamic-registration-of-factories
provides some cool ideas such as dynamically registering factories for the resource manager.
Could be a good idea to have a map of factories based on string fileExtension.
- Should have default assets and factory functions created on initialization. E.g. white pixel png, defaultShader, Engine-UI-Audio, engineDebugModel. Etc.

NOTE:
- I probably want to separate the loading for specific asset types into their own classes, e.g. TextureLoader, ModelLoader, etc.
For specific types of those asseets, if a library doesn't handle all of them, I can then make functions for the different types, e.g. LoadObj, LoadVox)
*/

#include "Manager.h"

#include <d3d11.h>
#include <unordered_map>
#include <string.h>
#include "../Graphics/Mesh.h"
#include "../Graphics/Texture.h"
#include <d3d11.h>

namespace CMP316engine {
	enum AssetType
	{
		NONE,
		IMAGE,
		MESH,
		AUDIO,
		SHADER
	};

	class AssetManager
		: public Manager
	{
	private:
		ID3D11Device* device = nullptr; 
		ID3D11DeviceContext* deviceContext = nullptr;

		std::unordered_map<std::string, Texture*> textures;
		std::unordered_map<std::string, std::vector<Mesh>> models;

	public:
		AssetManager() {};
		~AssetManager() = default;

		bool Initialize(ID3D11Device* rendererDevice, ID3D11DeviceContext* rendererDeviceContext) { device = rendererDevice; deviceContext = rendererDeviceContext; return true; }
		void Shutdown() {};

		//@brief If typename (resource to retrieve) exists, it will check if it is stored in a relevant map, if not it attempts to load the resource.
		//@return nullptr on failure.
		//@note Did this with template instead of a switchcase so that I could handle the different return types a bit cleaner. Alternative was void* return.
		template<typename Resource>
		Resource* GetResource(std::string filepath)
		{
			std::cout << "\nAssetManager::GetResource() attempted to load an unrecognized type, check if there is a template definition for the type being requested!";
			return nullptr;
		}
		// Automatically loads type of asset based on file extensions passed in.
		bool LoadAsset(std::string filePath); // std::filesystem::path = filePath; if filePath.extension == fileType then do something, else invalid filetype, no asset loaded.
		void UnloadAsset(std::string filepath, AssetType assetType);
		// Allows targetting of a specific type of asset.
		void UnloadAssets(AssetType assetType);
		void UnloadAllAssets();

		bool LoadModel(std::string filepath);
	};

	///////////////////////////////////
	///// RESOURCE TEMPLATE DEFINITIONS

	template<>
	inline Texture* AssetManager::GetResource<Texture>(std::string filepath)
	{
		// Check if resource already exists
		auto iterator = textures.find(filepath);
		if (iterator != textures.end())
		{
			return iterator->second;
		}
		else {
			// Asset wasn't loaded yet, load it now.
			if (!LoadAsset(filepath))
			{
				return nullptr;
			}
			else
			{
				// Try again now that asset is loaded as a resource
				auto iterator = textures.find(filepath);
				if (iterator != textures.end())
				{
					return iterator->second;
				}
			}
		}
	}
	template<>
	inline std::vector<Mesh>* AssetManager::GetResource<std::vector<Mesh>>(std::string filepath)
	{
		// Check if resource already exists
		auto iterator = models.find(filepath);
		if (iterator != models.end())
		{
			return &iterator->second;
		}
		else {
			// Asset wasn't loaded yet, load it now.
			if (!LoadAsset(filepath))
			{
				return nullptr;
			}
			else
			{
				// Try again now that asset is loaded as a resource
				auto iterator = models.find(filepath);
				if (iterator != models.end())
				{
					return &iterator->second;
				}
				return nullptr;
			}
		}
	}
}