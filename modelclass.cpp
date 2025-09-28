#include "modelclass.h"

CMP316engine::ModelClass::ModelClass()
{
	vertexBuffer = NULL;
	indexBuffer = NULL;
	texture = NULL;
	worldMatrix = XMMatrixIdentity();
}


CMP316engine::ModelClass::ModelClass(const ModelClass& other)
{
}


CMP316engine::ModelClass::~ModelClass()
{
}

bool CMP316engine::ModelClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* textureFilename)
{
	bool result;


	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(device, deviceContext, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

void CMP316engine::ModelClass::Shutdown()
{
	// Release the model texture.
	ReleaseTexture();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	delete mesh;

	return;
}

void CMP316engine::ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	/// TEST TRANSFORMS ///
	XMMATRIX translationMatrix = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(0.3f, 0.6f, 0.0f);
	worldMatrix = translationMatrix * rotationMatrix;
	///

	return;
}

int CMP316engine::ModelClass::GetIndexCount()
{
	return static_cast<int>(mesh->indices.size());
}

ID3D11ShaderResourceView* CMP316engine::ModelClass::GetTexture()
{
	return texture->GetTexture();
}

bool CMP316engine::ModelClass::InitializeBuffers(ID3D11Device* device)
{
	/*
	NOTE: Currently set to always draw a single quad, should instead have this handled by derived classes or a file model loader 
	*/

	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	mesh = new Mesh();

	/// VERTICES ///
	
	for (int i = 0; i < 4; i++) {
		mesh->vertices.push_back(CMP316engine::Vertex());
	}

	mesh->vertices[0].position = XMFLOAT3(-1.0f, 1.0f, 0.0f);  // Top left.
	mesh->vertices[1].position = XMFLOAT3(1.0f, 1.0f, 0.0f);  // Top right.
	mesh->vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	mesh->vertices[3].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.

	//XMFLOAT4 vertexColour = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f); // RED
	//vertices[0].color = vertexColour;
	//vertices[1].color = vertexColour;
	//vertices[2].color = vertexColour;
	//vertices[3].color = vertexColour;

	mesh->vertices[0].uv = XMFLOAT2(0.0f, 0.0f);
	mesh->vertices[1].uv = XMFLOAT2(1.0f, 0.0f);
	mesh->vertices[2].uv = XMFLOAT2(1.0f, 1.0f);
	mesh->vertices[3].uv = XMFLOAT2(0.0f, 1.0f);

	/// INDICES ///

	for (int i = 0; i < 6; i++) {
		mesh->indices.push_back(unsigned long());
	}

	// Triangle 1
	mesh->indices[0] = 0;  // Top left.
	mesh->indices[1] = 1;  // Top Right.
	mesh->indices[2] = 2;  // Bottom right.
	// Triangle 2
	mesh->indices[3] = 2;  // Bottom right.
	mesh->indices[4] = 3;  // Bottom left
	mesh->indices[5] = 0;  // Top left.

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * static_cast<UINT>(mesh->vertices.size());
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = mesh->vertices.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	HRESULT result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * static_cast<UINT>(mesh->indices.size());
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = mesh->indices.data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void CMP316engine::ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if (indexBuffer)
	{
		indexBuffer->Release();
		indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (vertexBuffer)
	{
		vertexBuffer->Release();
		vertexBuffer = 0;
	}

	return;
}

void CMP316engine::ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	/*
	Purpose of the function is to set the vertex and index buffer as active on the input asembler in the GPU.
	Once the GPU has an active vertex buffer it can use the shader to render that buffer. 
	This function also defines how the buffers should be drawn such as triangles, lines, etc.
	*/

	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(Vertex);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool CMP316engine::ModelClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	bool result;


	// Create and initialize the texture object.
	texture = new TextureClass;

	result = texture->Initialize(device, deviceContext, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void CMP316engine::ModelClass::ReleaseTexture()
{
	// Release the texture object.
	if (texture)
	{
		texture->Shutdown();
		delete texture;
		texture = 0;
	}

	return;
}
