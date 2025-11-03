#pragma once
namespace CMP316engine
{
	using Voxel = uint8_t;

	/*TODO: 
	Materials: Need to figure out how to make colourIndexes for a model map
	to a material. It should be customizable per model so that slight colour variations are allowed for the same material.
	Can introduce stuff like Volatile Voxels, and Resistance.
	I might not need to store any persistent data per voxel such as health, instead something like resistance
	could for example determine if a bullet destroys that voxel, and if it does, it removes force from a bullet
	e.g. Bullet has 21 force and each voxel has 5 resistance, result is 4 voxels destroyed and by the 5th voxel 
	the bullet has 1 force and fails to destroy.
	*/
}