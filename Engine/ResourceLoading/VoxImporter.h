/*
Importer for .Vox (MagicaVoxel) files.
.Vox files store just Voxels, the engine is responsible for generation of the mesh.

MagicaVoxel vox file format Documentation:
https://github.com/ephtracy/voxel-model/blob/master/MagicaVoxel-file-format-vox.txt
*/

#pragma once
#include <vector>
#include "Graphics/Mesh.h" 
#include "Utility/VoxelHelper.h"

namespace CMP316engine
{
    /*
    Store attributes in descending order of size for optimal packing
    Minimum space wasted by padding.
    If Colour Index is default, then voxel is not valid.
    */
    struct Voxel
    {
        /// ATTRIBUTES
        float health;
        //public float weight; // kg
        //public float resistance;
        //public bool explosive;
        /// CORE
        char colourIndex = -1;
    };

    struct VoxelAsset
    {
        std::vector<Voxel> voxels;
        VoxelHelper::Vector3Int modelSize;
    };

    class VoxImporter
    {
    public:
        static VoxelAsset LoadVox(const char* filepath);
        static std::vector<Mesh> GenerateVoxelMesh(std::vector<Voxel> voxels);
    };
}