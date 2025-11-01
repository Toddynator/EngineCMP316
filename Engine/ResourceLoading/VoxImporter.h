/*
Importer for .Vox (MagicaVoxel) files.
.Vox files store just Voxels, the engine is responsible for generation of the mesh.

MagicaVoxel vox file format Documentation:
https://github.com/ephtracy/voxel-model/blob/master/MagicaVoxel-file-format-vox.txt
*/

#pragma once
#include <vector>
#include "Graphics/Mesh.h" 

namespace CMP316engine
{
    struct Voxel
    {
        /// CORE
        char colourIndex;
        /// ATTRIBUTES
        float health;
        //public float weight; // kg
        //public float resistance;
        //public bool explosive;
    };

    class VoxImporter
    {
    public:
        static std::vector<Voxel> LoadVox(const char* filepath);
        static std::vector<Mesh> GenerateVoxelMesh(std::vector<Voxel> voxels);
    };
}