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

CURRENT ASSET TYPES:
- Models
- Textures
- Audio
- Shaders

TODO:
- Currently looking at Entt for resource caching, as I like the idea that I could have
storages specific to the type of data automatically created via entt's hashing system (however it works).
I then just use a quick lookup to retrieve the data.
https://github.com/skypjack/entt/wiki/Resource-management
- Also this link: https://gamedev.stackexchange.com/questions/97747/c-exensible-resource-manager-class-with-dynamic-registration-of-factories
provides some cool ideas such as dynamically registering factories for the resource manager.
Could be a good idea to have a map of factories based on string fileExtension.
- Should have default assets and factory functions created on initialization. E.g. white pixel png, defaultShader, Engine-UI-Audio, engineDebugModel. Etc.
*/

#include "Manager.h"

#include <d3d11.h>
#include <unordered_map>
#include <string.h>

namespace CMP316engine {

	class AssetManager
		: public Manager
	{
	public:
		AssetManager() {};
		~AssetManager() = default;

		bool Initialize() { return true; }
		void Shutdown() {}

		void GetResource() {}
		// Automatically loads type of asset based on file extensions passed in.
		bool LoadAsset(std::string filePath) {} // std::filesystem::path = filePath; if filePath.extension == fileType then do something, else invalid filetype, no asset loaded.
		void UnloadAsset() {}
		// Allows targetting of a specific type of asset.
		void UnloadAssets() {}
		void UnloadAllAssets() {}


		//const Texture LoadTexture();
		//const Texture GetTexture();

		//const Model LoadModel();
		//const Model GetModel();

	private:
		//std::unordered_map<std::string, Texture*> textures;
		//std::unordered_map<std::string, Model*> models;
	};
}