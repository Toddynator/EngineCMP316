#pragma once
#include <string>
#include <d3d11.h>
#include <directxmath.h>
#include <vector>

namespace CMP316engine {
	// Definition of the vertex type that will be used with the vertex buffer.
	// MUST match the layout in the ShaderClass.
	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 uv; // Texture coord's between 0 and 1
		//XMFLOAT4 colour = XMFLOAT3(1.f, 1.f, 1.f, 1.f); // Multiples the texture colour
		//XMFLOAT3 normal;
	};

	struct Mesh
	{
		std::string name = "Unnamed Mesh";
		std::vector<CMP316engine::Vertex> vertices;
		std::vector<unsigned long> indices;
		std::string textureName = ""; // Default nothing, engine should have handling for this
	};
}
