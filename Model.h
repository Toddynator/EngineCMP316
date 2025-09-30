
#pragma once
#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"
#include <unordered_map>

namespace CMP316engine {
	class Model
	{
	private:

	public:
		Model();
		Model(const Model&);
		~Model();

		bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
		void Shutdown();
		// Puts the model geometry onto the video card to prepare for drawing then draws using the shader.
		bool Render(Shader* shader, ID3D11DeviceContext* deviceContext, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);
		void RenderImGuiControls();

		int GetIndexCount();
		int GetVertexCount();
		XMMATRIX GetWorldMatrix() { return worldMatrix; }
		void SetPosition(XMFLOAT3 newPosition) { position = newPosition; }

	private:
		bool generateVerticesAndIndices(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
		bool InitializeBuffers(ID3D11Device*);
		void ShutdownBuffers();
		void RenderBuffers(ID3D11DeviceContext*);

		bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*, std::string textureName);
		void ReleaseTexture();

		bool loadModel(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string filepath);

	private:
		std::vector<Mesh> meshes;
		ID3D11Buffer* vertexBuffer;
		ID3D11Buffer* indexBuffer;
		std::unordered_map<std::string, Texture*> textures; // TODO: Remove once AssetManager is created, mesh struct holds texture name, it looks up texture in unordered map (which will be in assetManager at some point)

		XMFLOAT3 position = { 0, 0, 0 };
		XMFLOAT3 rotation = { 0, 0, 0 };
		XMMATRIX worldMatrix; // The World matrix is used to define the position of objects in the 3d scene. (Scale, Rotation, Translation, etc)
		// Probably should move this to a 'Transform Object' class later.
	};
}