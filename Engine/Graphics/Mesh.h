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
		DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		DirectX::XMFLOAT2 uv = DirectX::XMFLOAT2(0.f, 0.f);; // Texture coordinates between 0 and 1
		DirectX::XMFLOAT4 colour = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f); // Multiples the texture colour
		DirectX::XMFLOAT3 normal = DirectX::XMFLOAT3(1.f, 1.f, 1.f);
	};

	struct Mesh
	{
		std::string name = "Unnamed Mesh";
		std::vector<CMP316engine::Vertex> vertices;
		std::vector<unsigned long> indices;
		std::string textureName = ""; // Default nothing, engine should have handling for this / a default texture to load, e.g. white pixel so that colour property has control
	};
}
