#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

#include "textureclass.h"

class ModelClass
{
private:
	// Definition of the vertex type that will be used with the vertex buffer.
	// MUST match the layout in the ShaderClass.
	struct Vertex
	{
		XMFLOAT3 position;
		XMFLOAT2 uv; // Texture coord's between 0 and 1
		//XMFLOAT4 colour = XMFLOAT3(1.f, 1.f, 1.f, 1.f); // Multiples the texture colour
		//XMFLOAT3 normal;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*);
	void Shutdown();
	// Puts the model geometry onto the video card to prepare it for drawing by the color shader.
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();

	XMMATRIX GetWorldMatrix() { return worldMatrix; }

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();

private:
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
	int m_vertexCount, m_indexCount;

	TextureClass* m_Texture;

	XMMATRIX worldMatrix; // The World matrix is used to define the position of objects in the 3d scene. (Scale, Rotation, Translation, etc)
	// Probably should move this to a 'Transform Object' class later.
};

#endif