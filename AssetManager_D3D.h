/*

*/

#include "Manager.h"

#include <d3d11.h>
#include <unordered_map>
#include <string.h>

namespace CMP316engine {

	class AssetManager_D3D
		: public Manager
	{
	public:
		AssetManager_D3D();
		~AssetManager_D3D() = default;

		bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*);
		void Shutdown();

		const ID3D11Texture2D LoadTexture();
		const ID3D11Texture2D GetTexture();

		const ID3D11Texture2D LoadModel();
		const ID3D11Texture2D GetModel();

	private:
		//std::unordered_map<std::string, ID3D11Texture2D*> textures;
		//std::unordered_map<std::string, Mesh*> models;
	};
}