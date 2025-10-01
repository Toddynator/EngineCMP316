#include "pch.h"
#include "Model.h"
#include "OBJ_Loader.h"
#include "TextureLoader.h"
#include "imgui.h"

CMP316engine::Model::Model()
{
	worldMatrix = XMMatrixIdentity();
}


CMP316engine::Model::Model(const Model& other)
{
}


CMP316engine::Model::~Model()
{
}

bool CMP316engine::Model::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	bool result;

	// Initialize the vertex and index buffers.
	result = generateVerticesAndIndices(device, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

void CMP316engine::Model::Shutdown()
{
	// Release the model texture.
	ReleaseTexture();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	return;
}

bool CMP316engine::Model::Render(Shader* shader, ID3D11DeviceContext* deviceContext, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	// Update models world matrix to any transforms that occurred during the frame, e.g. it has moved position or changed rotation
	calculateWorldMatrix();

	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	// Render each mesh using the shader
	// Compared to buffers this needs explicitly handled for each mesh as to handle different textures.
	int meshVertexOffset = 0;
	for (auto& mesh : meshes)
	{
		if (!shader->Render(deviceContext, static_cast<int>(mesh.indices.size()), worldMatrix, viewMatrix, projectionMatrix, textures[mesh.textureName]->GetTextureView(), meshVertexOffset))
		{
			return false;
		}

		meshVertexOffset += static_cast<int>(mesh.indices.size());
	}

	return true;
}

int CMP316engine::Model::GetIndexCount()
{
	int total = 0;
	for (auto& mesh : meshes) {
		total += static_cast<int>(mesh.indices.size());
	}
	return total;
}

int CMP316engine::Model::GetVertexCount()
{
	int total = 0;
	for (auto& mesh : meshes) {
		total += static_cast<int>(mesh.vertices.size());
	}
	return total;
}

void CMP316engine::Model::RenderImGuiControls()
{	
	if (ImGui::InputFloat3("Position", &position.x))
	{
		//calculateWorldMatrix(); // NOTE: May just leave this to be handled by the Render() call.
	}

	if (ImGui::SliderFloat3("Rotation", &rotation.x, 0.f, 6.3f))
	{
		//calculateWorldMatrix();
	}
}

bool CMP316engine::Model::generateVerticesAndIndices(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	/*
	NOTE: Currently hard coded to draw a specific model, should instead have this handled by derived classes or a file model loader
	*/

	/// TEMP DEFAULT TEXTURE
	// TODO: Should have this handled by an assetManager later
	char textureFilepath[128];
	std::filesystem::path filepath = std::filesystem::current_path();
	std::string assetFilepath = filepath.string() + "/data/stone01.tga";
	strcpy_s(textureFilepath, assetFilepath.c_str());
	Texture* defaultTexture = TextureLoader::LoadTexture(textureFilepath, device, deviceContext);
	textures.insert({ "", defaultTexture });
	///

	//loadModel("data/box_stack.obj");
	if (!loadModel(device, deviceContext, "data/Dug/Dug.obj")) { return false; }

	/// TEST TRANSFORMS ///
	calculateWorldMatrix();
	///

	/* //// MANUALLY DEFINING THE MODEL
	mesh = new Mesh();

	/// VERTICES ///

	for (int i = 0; i < 4; i++) {
		mesh->vertices.push_back(CMP316engine::Vertex());
	}

	mesh->vertices[0].position = XMFLOAT3(-1.0f, 1.0f, 0.0f);  // Top left.
	mesh->vertices[1].position = XMFLOAT3(1.0f, 1.0f, 0.0f);  // Top right.
	mesh->vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	mesh->vertices[3].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.

	XMFLOAT4 vertexColour = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f); // RED
	mesh->vertices[0].colour = vertexColour;
	mesh->vertices[1].colour = vertexColour;
	mesh->vertices[2].colour = vertexColour;
	mesh->vertices[3].colour = vertexColour;

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
	*/

	if (!InitializeBuffers(device)) { return false; }

	return true;
}

void CMP316engine::Model::calculateWorldMatrix()
{
	XMMATRIX translationMatrix = XMMatrixTranslation(position.x, position.y, position.z);
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	worldMatrix = rotationMatrix * translationMatrix;
}

bool CMP316engine::Model::InitializeBuffers(ID3D11Device* device)
{
	D3D11_BUFFER_DESC vertexBufferDesc{};
	D3D11_BUFFER_DESC indexBufferDesc{};
	D3D11_SUBRESOURCE_DATA vertexData{};
	D3D11_SUBRESOURCE_DATA indexData{};

	std::vector<CMP316engine::Vertex> allVertices;
	std::vector<unsigned long> allIndices;

	/*
	This packs all the vertices and indices from each mesh into a single vertex and index buffer.
	In Theory this should be more performance friendly?
	*/
	int meshVertexOffset = 0;
	for (auto& mesh : meshes)
	{
		allVertices.insert(allVertices.end(), mesh.vertices.begin(), mesh.vertices.end());	

		// Need to adjust indices for each mesh passed in
		for (auto i : mesh.indices)
		{
			allIndices.push_back(i + meshVertexOffset);
		}

		meshVertexOffset = static_cast<int>(allVertices.size());
	}

	/// VERTEX BUFFER ///

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * static_cast<UINT>(GetVertexCount());
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = allVertices.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	HRESULT result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	/// INDEX BUFFER ///

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * static_cast<UINT>(GetIndexCount());
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = allIndices.data();
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

void CMP316engine::Model::ShutdownBuffers()
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

void CMP316engine::Model::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	/*
	Purpose of the function is to set the vertex and index buffer as active on the input assembler in the GPU.
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

void CMP316engine::Model::ReleaseTexture()
{
	// Release the texture objects.
	for (auto& [key,texture] : textures)
	{
		texture->Shutdown();
	}	

	return;
}

bool CMP316engine::Model::loadModel(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string filepath)
{
	meshes.clear();
	textures.clear();

	objl::Loader objLoader;
	bool success = objLoader.LoadFile(filepath);
	if (!success) { return false; }

	for (auto& loadedMesh : objLoader.LoadedMeshes)
	{
		meshes.push_back(CMP316engine::Mesh());
		auto& mesh = meshes.back();
		mesh.name = loadedMesh.MeshName;

		//// VERTICES
		for (auto& loadedVertex : loadedMesh.Vertices) {
			CMP316engine::Vertex vertex;
			vertex.position = XMFLOAT3(loadedVertex.Position.X, loadedVertex.Position.Y, loadedVertex.Position.Z);
			vertex.normal = XMFLOAT3(loadedVertex.Normal.X, loadedVertex.Normal.Y, loadedVertex.Normal.Z);
			vertex.uv = XMFLOAT2(loadedVertex.TextureCoordinate.X, loadedVertex.TextureCoordinate.Y);

			mesh.vertices.push_back(vertex);
			mesh.vertices.back().uv.y = 1 - mesh.vertices.back().uv.y;
			//mesh.vertices.back().Normal = glm::normalize(mesh.vertices.back().Normal);
			//mesh.vertices.back().Normal *= -1;
		}
		//// INDICES
		for (auto& index : loadedMesh.Indices) {
			mesh.indices.push_back(index);
		}
		std::reverse(mesh.indices.begin(), mesh.indices.end());

		//// MATERIALS
		std::cout << "\nMaterial Name: " << loadedMesh.MeshMaterial.name; // TEST
		std::cout << "\nMaterial mapb: " << loadedMesh.MeshMaterial.map_bump; // TEST
		std::cout << "\nMaterial mapd: " << loadedMesh.MeshMaterial.map_d; // TEST
		std::cout << "\nMaterial mapKa: " << loadedMesh.MeshMaterial.map_Ka; // TEST
		std::cout << "\nMaterial mapKd: " << loadedMesh.MeshMaterial.map_Kd; // TEST
		std::cout << "\nMaterial mapKs: " << loadedMesh.MeshMaterial.map_Ks; // TEST
		std::cout << "\nMaterial mapNs: " << loadedMesh.MeshMaterial.map_Ns; // TEST

		//// TEXTURES
		
		const std::string textureName = loadedMesh.MeshMaterial.map_Kd;

		/// Get Model filepath, then replace obj name with texture name
		std::filesystem::path projectFilepath = std::filesystem::current_path();
		std::filesystem::path modelFilepath = filepath;
		std::filesystem::path modelDirectory = modelFilepath.parent_path();
		std::filesystem::path textureFilepath = modelDirectory / textureName;
		std::wcout << L"\nTexture Filepath: " << textureFilepath; // DEBUG

		char textureFilepathChar[128];
		strcpy_s(textureFilepathChar, textureFilepath.string().c_str());
		Texture* texture = TextureLoader::LoadTexture(textureFilepathChar, device, deviceContext);
		textures.insert({ textureName, texture });
		mesh.textureName = textureName;
	}

	return true;
}
