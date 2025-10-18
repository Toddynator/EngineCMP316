#include "pch.h"
#include "RenderSystem.h"
#include "CameraSystem.h"
#include "../../ResourceLoading/TextureLoader.h"
#include "../../ResourceLoading/OBJ_Loader.h"

bool CMP316engine::RenderSystem::Initialize()
{
	return true;
}

void CMP316engine::RenderSystem::Shutdown()
{
	shutdownBuffers();
}

void CMP316engine::RenderSystem::Update(float deltaTime)
{
	auto modelEntities = registry->view<ModelComponent, MeshComponent>();
	for (auto& entity : modelEntities) {
		auto [modelComponent, meshComponent] = registry->get<ModelComponent, MeshComponent>(entity);

		if (modelComponent.modelLoaded == false) { loadModel(modelComponent, meshComponent); }
	}

	auto meshEntities = registry->view<MeshComponent>();
	for (auto& entity : meshEntities) {
		auto& meshComponent = registry->get<MeshComponent>(entity);

		if (meshComponent.meshNeedsCalculated) 
		{ 
			/// CALCULATE EDITOR COLLIDER MESH
			// TODO: Maybe this should be encapsulated in the LevelEditorSystem instead, but then unsure how to cleanly handle
			// updating the collider when the mesh changes.

			if (auto* editorCollider = registry->try_get<LevelEditorColliderComponent>(entity))
			{
				for (auto& mesh : meshComponent.meshes)
				{
					for (auto& vertex : mesh.vertices)
					{
						// Compare each vertex position to the existing min and max positions of the bounding box. Expand bounding box if vertex is outside of it.
						DirectX::XMVECTOR vertexPositionVector = DirectX::XMVector3Length(DirectX::XMLoadFloat3(&vertex.position));
						DirectX::XMVECTOR minVector = DirectX::XMVectorMin(DirectX::XMLoadFloat3(&editorCollider->min), vertexPositionVector);
						DirectX::XMVECTOR maxVector = DirectX::XMVectorMax(DirectX::XMLoadFloat3(&editorCollider->max), vertexPositionVector);
						DirectX::XMStoreFloat3(&editorCollider->min, minVector);
						DirectX::XMStoreFloat3(&editorCollider->max, maxVector);
					}
				}
				std::cout << "\nCollider Min: " << editorCollider->min.x << ", " << editorCollider->min.y << ", " << editorCollider->min.z << 
					"\nCollider Max: " << editorCollider->max.x << ", " << editorCollider->max.y << ", " << editorCollider->max.z; // DEBUG
			}

			/// CALCULATE BUFFERS FOR RENDERING

			calculateBuffers(meshComponent); 
		}
	}
}

void CMP316engine::RenderSystem::RenderModels(entt::registry* sceneRegistry, Renderer_DirectX11* sceneRenderer, Shader* shader, DirectX::XMMATRIX viewMatrix)
{
	auto meshEntities = sceneRegistry->view<MeshComponent, TransformComponent>();
	for (auto& entity : meshEntities) 
	{
		auto [transformComponent, meshComponent] = sceneRegistry->get<TransformComponent, MeshComponent>(entity);

		// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
		unsigned int stride;
		unsigned int offset;
		stride = sizeof(Vertex);
		offset = 0;
		auto* deviceContext = sceneRenderer->GetDeviceContext();
		deviceContext->IASetVertexBuffers(0, 1, &meshComponent.vertexBuffer, &stride, &offset);
		deviceContext->IASetIndexBuffer(meshComponent.indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Render each mesh using the shader
		// Compared to buffers this needs explicitly handled for each mesh as to handle different textures.
		int meshVertexOffset = 0;
		for (auto& mesh : meshComponent.meshes)
		{
			if (!shader->Render(deviceContext, static_cast<int>(mesh.indices.size()), transformComponent.worldMatrix, viewMatrix, sceneRenderer->GetProjectionMatrix(), meshComponent.textures[mesh.textureName]->GetTextureView(), meshVertexOffset))
			{
				std::cout << "\nShader failed to render the mesh";
				break;
			}
			meshVertexOffset += static_cast<int>(mesh.indices.size());
		}
	}
}

void CMP316engine::RenderSystem::loadModel(ModelComponent& modelComponent, MeshComponent& meshComponent)
{
	meshComponent.meshes.clear();
	meshComponent.textures.clear();

	objl::Loader objLoader;
	bool success = objLoader.LoadFile(modelComponent.filepath);
	if (!success) { std::cout << "\nUnable to load or find model"; return; }

	for (auto& loadedMesh : objLoader.LoadedMeshes)
	{
		meshComponent.meshes.push_back(CMP316engine::Mesh());
		auto& mesh = meshComponent.meshes.back();
		mesh.name = loadedMesh.MeshName;

		//// VERTICES
		for (auto& loadedVertex : loadedMesh.Vertices) {
			CMP316engine::Vertex vertex;
			vertex.position = DirectX::XMFLOAT3(loadedVertex.Position.X, loadedVertex.Position.Y, loadedVertex.Position.Z);
			vertex.normal = DirectX::XMFLOAT3(loadedVertex.Normal.X, loadedVertex.Normal.Y, loadedVertex.Normal.Z);
			vertex.uv = DirectX::XMFLOAT2(loadedVertex.TextureCoordinate.X, loadedVertex.TextureCoordinate.Y);

			mesh.vertices.push_back(vertex);
			mesh.vertices.back().uv.y = 1 - mesh.vertices.back().uv.y;
			//mesh.vertices.back().Normal = glm::normalize(mesh.vertices.back().Normal); // Is it obvious now that I stole this from an old project :P
			//mesh.vertices.back().Normal *= -1;
		}
		//// INDICES
		for (auto& index : loadedMesh.Indices) {
			mesh.indices.push_back(index);
		}
		std::reverse(mesh.indices.begin(), mesh.indices.end());

		//// MATERIALS
		//std::cout << "\nMaterial Name: " << loadedMesh.MeshMaterial.name;
		//std::cout << "\nMaterial mapb: " << loadedMesh.MeshMaterial.map_bump;
		//std::cout << "\nMaterial mapd: " << loadedMesh.MeshMaterial.map_d;
		//std::cout << "\nMaterial mapKa: " << loadedMesh.MeshMaterial.map_Ka;
		//std::cout << "\nMaterial mapKd: " << loadedMesh.MeshMaterial.map_Kd;
		//std::cout << "\nMaterial mapKs: " << loadedMesh.MeshMaterial.map_Ks;
		//std::cout << "\nMaterial mapNs: " << loadedMesh.MeshMaterial.map_Ns;

		//// TEXTURES

		const std::string textureName = loadedMesh.MeshMaterial.map_Kd;

		/// Get Model filepath, then replace obj name with texture name
		std::filesystem::path projectFilepath = std::filesystem::current_path();
		std::filesystem::path modelFilepath = modelComponent.filepath;
		std::filesystem::path modelDirectory = modelFilepath.parent_path();
		std::filesystem::path textureFilepath = modelDirectory / textureName;
		//std::wcout << L"\nTexture Filepath: " << textureFilepath; // DEBUG

		char textureFilepathChar[128];
		strcpy_s(textureFilepathChar, textureFilepath.string().c_str());
		Texture* texture = TextureLoader::LoadTexture(textureFilepathChar, renderer->GetDevice(), renderer->GetDeviceContext());
		meshComponent.textures.insert({ textureName, texture });
		mesh.textureName = textureName;
	}

	modelComponent.modelLoaded = true;

	std::cout << "\nModel Successfully Loaded"; // TEST
}

void CMP316engine::RenderSystem::calculateBuffers(MeshComponent& meshComponent)
{
	meshComponent.meshNeedsCalculated = false;

	std::vector<CMP316engine::Vertex> allVertices;
	std::vector<unsigned long> allIndices;

	/*
	This packs all the vertices and indices from each mesh into a single vertex and index buffer.
	In Theory this should be more performance friendly?
	*/
	int meshVertexOffset = 0;
	for (auto& mesh : meshComponent.meshes)
	{
		allVertices.insert(allVertices.end(), mesh.vertices.begin(), mesh.vertices.end());

		// Need to adjust indices for each mesh passed in
		for (auto i : mesh.indices)
		{
			allIndices.push_back(i + meshVertexOffset);
		}

		meshVertexOffset = static_cast<int>(allVertices.size());
	}

	/////////////////////
	/// VERTEX BUFFER ///

	// Set up the description of the static vertex buffer.
	D3D11_BUFFER_DESC vertexBufferDesc{};
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * static_cast<UINT>(meshComponent.GetVertexCount());
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	D3D11_SUBRESOURCE_DATA vertexData{};
	vertexData.pSysMem = allVertices.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	HRESULT result = renderer->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &meshComponent.vertexBuffer);
	if (FAILED(result))
	{
		std::cout << "\nFailed to create Vertex Buffer with Error: " << result;
		return;
	}

	////////////////////
	/// INDEX BUFFER ///

	// Set up the description of the static index buffer.
	D3D11_BUFFER_DESC indexBufferDesc{};
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * static_cast<UINT>(meshComponent.GetIndexCount());
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	D3D11_SUBRESOURCE_DATA indexData{};
	indexData.pSysMem = allIndices.data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = renderer->GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &meshComponent.indexBuffer);
	if (FAILED(result))
	{
		std::cout << "\nFailed to create Index Buffer with Error: " << result;
		return;
	}
}

void CMP316engine::RenderSystem::shutdownBuffers()
{
	auto meshEntities = registry->view<MeshComponent>();
	for (auto& entity : meshEntities) {
		auto& meshComponent = registry->get<MeshComponent>(entity);

		if (meshComponent.vertexBuffer) { meshComponent.vertexBuffer->Release(); }
		if (meshComponent.indexBuffer) { meshComponent.indexBuffer->Release(); }
		meshComponent.vertexBuffer = nullptr;
		meshComponent.indexBuffer = nullptr;
	}
}