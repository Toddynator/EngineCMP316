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
#include "Core/Voxel.h"

namespace NomadEngine
{
    class VoxImporter
    {
    public:
        static VoxelResource LoadVox(const char* filepath);
    };
}