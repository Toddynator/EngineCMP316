#include "VoxelHelper.h"

namespace CMP316engine::VoxelHelper
{
	int Convert3DPositionToIndex(int x, int y, int z, int modelSizeX, int modelSizeY, int modelSizeZ)
	{
		return (x * modelSizeY * modelSizeZ) + (y * modelSizeZ) + (z);
	}
	void ConvertIndexTo3DPosition(int index, int& x, int& y, int& z, int modelSizeX, int modelSizeY, int modelSizeZ)
	{
		x = index / (modelSizeY * modelSizeZ); // X
		y = (index - (x * modelSizeY * modelSizeZ)) / modelSizeZ; // Y
		z = index - (x * modelSizeY * modelSizeZ) - (y * modelSizeZ); // Z
	}
}