#include "VoxelHelper.h"

namespace NomadEngine::VoxelHelper
{
	int Convert3DPositionToIndex(Vector3Int position, Vector3Int modelSize)
	{
		return (position.x * modelSize.y * modelSize.z) + (position.y * modelSize.z) + (position.z);
	}
	Vector3Int ConvertIndexTo3DPosition(int index, Vector3Int modelSize)
	{
		Vector3Int position;
		position.x = index / (modelSize.y * modelSize.z); // X
		position.y = (index - (position.x * modelSize.y * modelSize.z)) / modelSize.z; // Y
		position.z = index - (position.x * modelSize.y * modelSize.z) - (position.y * modelSize.z); // Z
		return position;
	}
}