/*
File for common data and functions required by voxel logic in the engine.
*/

#pragma once
#include "Graphics/Mesh.h"
#include "Core/Voxel.h"
namespace CMP316engine::VoxelHelper
{
	/*
	Convert Voxel Positions into a 1D array for increased performance at the cost of wasted memory.
	*/
	int Convert3DPositionToIndex(Vector3Int position, Vector3Int modelSize);
    Vector3Int ConvertIndexTo3DPosition(int index, Vector3Int modelSize);

    static const int COLOUR_PALLETE_SIZE = 256; // This is the total size of Magicavoxel Colour Palletes.

    enum VoxelFace
    {
        Back = 0,
        Front = 1,
        Right = 2,
        Left = 3,
        Bottom = 4,
        Top = 5
    };
    static const Vector3Int FaceDirections[6] =
    {
        Vector3Int(0,  0, -1), // Back
        Vector3Int(0,  0,  1), // Front
        Vector3Int(1,  0,  0), // Right
        Vector3Int(-1,  0,  0), // Left
        Vector3Int(0, -1,  0), // Bottom
        Vector3Int(0,  1,  0)  // Top
    };
    static const DirectX::XMFLOAT3 FaceVertexOffsets[6][4] =
    {
        // Back // -Z
        {
            DirectX::XMFLOAT3(0.5f, -0.5f, -0.5f),
            DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f),
            DirectX::XMFLOAT3(-0.5f,  0.5f, -0.5f),
            DirectX::XMFLOAT3(0.5f,  0.5f, -0.5f)
        },
        // Front // +Z
        {
            DirectX::XMFLOAT3(-0.5f, -0.5f,  0.5f),
            DirectX::XMFLOAT3(0.5f, -0.5f,  0.5f),
            DirectX::XMFLOAT3(0.5f,  0.5f,  0.5f),
            DirectX::XMFLOAT3(-0.5f,  0.5f,  0.5f)
        },
        // Right // +X
        {
            DirectX::XMFLOAT3(0.5f, -0.5f, -0.5f),
            DirectX::XMFLOAT3(0.5f,  0.5f, -0.5f),
            DirectX::XMFLOAT3(0.5f,  0.5f,  0.5f),
            DirectX::XMFLOAT3(0.5f, -0.5f,  0.5f)
        },
        // Left // -X
        {
            DirectX::XMFLOAT3(-0.5f,  0.5f, -0.5f),
            DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f),
            DirectX::XMFLOAT3(-0.5f, -0.5f,  0.5f),
            DirectX::XMFLOAT3(-0.5f,  0.5f,  0.5f)
        },
        // Bottom // -Y
        {
            DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f),
            DirectX::XMFLOAT3(0.5f, -0.5f, -0.5f),
            DirectX::XMFLOAT3(0.5f, -0.5f,  0.5f),
            DirectX::XMFLOAT3(-0.5f, -0.5f,  0.5f)
        },
        // Top // +Y
        {
            DirectX::XMFLOAT3(0.5f,  0.5f, -0.5f),
            DirectX::XMFLOAT3(-0.5f,  0.5f, -0.5f),
            DirectX::XMFLOAT3(-0.5f,  0.5f,  0.5f),
            DirectX::XMFLOAT3(0.5f,  0.5f,  0.5f)
        }
    };

    static void GenerateVoxelFaceVertices(Voxel voxel, Vector3Int position, std::vector<Vertex>& vertices, std::vector<unsigned long>& indices, VoxelFace voxelFace)
    {
        int o = vertices.size(); // Offset between indices for each face
        DirectX::XMFLOAT3 vertexOffset;
        Vector3Int faceDirection = FaceDirections[(int)voxelFace];
        DirectX::XMFLOAT3 normal = { static_cast<float>(faceDirection.x), static_cast<float>(faceDirection.y), static_cast<float>(faceDirection.z) };
        
        float colourOffset = 1.0f / COLOUR_PALLETE_SIZE;
        float colourPosition = colourOffset * static_cast<float>(voxel-0.5f);

        //std::cout << "Voxel: " << static_cast<float>(voxel);

        vertices.push_back({});
        vertexOffset = FaceVertexOffsets[(int)voxelFace][0];
        vertices.back().position = { position.x + static_cast<float>(vertexOffset.x), position.y + static_cast<float>(vertexOffset.y), position.z + static_cast<float>(vertexOffset.z) };
        vertices.back().normal = normal;
        vertices.back().uv = { colourPosition, 0.f };
        vertices.push_back({});
        vertexOffset = FaceVertexOffsets[(int)voxelFace][1];
        vertices.back().position = { position.x + static_cast<float>(vertexOffset.x), position.y + static_cast<float>(vertexOffset.y), position.z + static_cast<float>(vertexOffset.z) };
        vertices.back().normal = normal;
        vertices.back().uv = { colourPosition, 0 };
        vertices.push_back({});
        vertexOffset = FaceVertexOffsets[(int)voxelFace][2];
        vertices.back().position = { position.x + static_cast<float>(vertexOffset.x), position.y + static_cast<float>(vertexOffset.y), position.z + static_cast<float>(vertexOffset.z) };
        vertices.back().normal = normal;
        vertices.back().uv = { colourPosition,  0.f };
        vertices.push_back({});
        vertexOffset = FaceVertexOffsets[(int)voxelFace][3];
        vertices.back().position = { position.x + static_cast<float>(vertexOffset.x), position.y + static_cast<float>(vertexOffset.y), position.z + static_cast<float>(vertexOffset.z) };
        vertices.back().normal = normal;
        vertices.back().uv = { colourPosition,  0.f };

        int newIndices[6] = {0 + o, 1 + o, 2 + o, 2 + o, 3 + o, 0 + o};
        //int newIndices[6] = { 2 + o, 1 + o, 0 + o, 0 + o, 3 + o, 2 + o };
        indices.insert(indices.end(), std::begin(newIndices), std::end(newIndices));
    }
}