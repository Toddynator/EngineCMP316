#include "VoxelHelper.h"

namespace CMP316engine::VoxelHelper
{
	int Convert3DPositionToIndex(int x, int y, int z, int modelSizeX, int modelSizeY, int modelSizeZ)
	{
		return (x * modelSizeY * modelSizeZ) + (y * modelSizeZ) + (z);
	}
	//void ConvertIndexTo3DPosition(int index, int& x, int& y, int& z, int modelSizeX, int modelSizeY, int modelSizeZ)
	//{
	//	x = index / (modelSizeY * modelSizeZ); // X
	//	y = (index - (x * modelSizeY * modelSizeZ)) / modelSizeZ; // Y
	//	z = index - (x * modelSizeY * modelSizeZ) - (y * modelSizeZ); // Z
	//}
	Vector3Int ConvertIndexTo3DPosition(int index, Vector3Int modelSize)
	{
		Vector3Int position;
		position.x = index / (modelSize.y * modelSize.z); // X
		position.y = (index - (position.x * modelSize.y * modelSize.z)) / modelSize.z; // Y
		position.z = index - (position.x * modelSize.y * modelSize.z) - (position.y * modelSize.z); // Z
		return position;
	}
}