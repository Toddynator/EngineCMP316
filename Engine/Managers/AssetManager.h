/*
CURRENTLY UNUSED
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

		bool Initialize() {};
		void Shutdown() {};

		//const Texture LoadTexture();
		//const Texture GetTexture();

		//const Model LoadModel();
		//const Model GetModel();

	private:
		//std::unordered_map<std::string, Texture*> textures;
		//std::unordered_map<std::string, Model*> models;
	};
}