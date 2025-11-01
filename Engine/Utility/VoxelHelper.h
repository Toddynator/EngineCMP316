#pragma once
namespace CMP316engine::VoxelHelper
{
	/*
	Convert Voxel Positions into a 1D array for increased performance at the cost of wasted memory.
	*/
	int Convert3DPositionToIndex(int x, int y, int z, int modelSizeX, int modelSizeY, int modelSizeZ);
	void ConvertIndexTo3DPosition(int index, int& x, int& y, int& z, int modelSizeX, int modelSizeY, int modelSizeZ);
}