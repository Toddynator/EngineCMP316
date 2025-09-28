#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

#include "textureclass.h"
#include "Mesh.h"

namespace CMP316engine {
	class ModelClass
	{
	private:

	public:
		ModelClass();
		ModelClass(const ModelClass&);
		~ModelClass();

		bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*);
		void Shutdown();
		// Puts the model geometry onto the video card to prepare it for drawing by the color shader.
		void Render(ID3D11DeviceContext*);

		int GetIndexCount();
		int GetVertexCount();

		ID3D11ShaderResourceView* GetTexture();

		XMMATRIX GetWorldMatrix() { return worldMatrix; }

	private:
		bool InitializeBuffers(ID3D11Device*);
		void ShutdownBuffers();
		void RenderBuffers(ID3D11DeviceContext*);

		bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
		void ReleaseTexture();

		bool loadModel(std::string filepath);

	private:
		std::vector<Mesh> meshes;
		ID3D11Buffer* vertexBuffer;
		ID3D11Buffer* indexBuffer;
		std::vector<TextureClass> textures;

		XMMATRIX worldMatrix; // The World matrix is used to define the position of objects in the 3d scene. (Scale, Rotation, Translation, etc)
		// Probably should move this to a 'Transform Object' class later.
	};
}
#endif