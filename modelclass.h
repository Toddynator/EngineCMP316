
#pragma once
#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

#include "Texture.h"
#include "Mesh.h"

namespace CMP316engine {
	class ModelClass
	{
	private:

	public:
		ModelClass();
		ModelClass(const ModelClass&);
		~ModelClass();

		bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* textureFilepath);
		void Shutdown();
		// Puts the model geometry onto the video card to prepare it for drawing by the color shader.
		void Render(ID3D11DeviceContext*);

		int GetIndexCount();
		int GetVertexCount();

		ID3D11ShaderResourceView* GetTextureView(int textureNum);

		XMMATRIX GetWorldMatrix() { return worldMatrix; }

	private:
		bool generateVerticesAndIndices(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
		bool InitializeBuffers(ID3D11Device*);
		void ShutdownBuffers();
		void RenderBuffers(ID3D11DeviceContext*);

		bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
		void ReleaseTexture();

		bool loadModel(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string filepath);

	private:
		std::vector<Mesh> meshes;
		ID3D11Buffer* vertexBuffer;
		ID3D11Buffer* indexBuffer;
		std::vector<Texture*> textures; // Later replace with strings, as the model should not hold the actual texture, an asset manager should be doing this so that it prevents duplicates of a texture.

		XMMATRIX worldMatrix; // The World matrix is used to define the position of objects in the 3d scene. (Scale, Rotation, Translation, etc)
		// Probably should move this to a 'Transform Object' class later.
	};
}