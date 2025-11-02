#include "VoxImporter.h"
#include "Utility/BinaryDeserializeArchive.h"
#include "Utility/VoxelHelper.h"

namespace CMP316engine
{
    VoxelAsset VoxImporter::LoadVox(const char* filepath)
	{
        VoxelAsset voxelAsset = {};
		std::vector<Voxel> voxels;

		std::ifstream file(filepath, std::ios::binary);
		if (!file.is_open()) {
			std::cout << "\nFailed to open file for reading";
			return voxelAsset;
		}
		BinaryDeserializeArchive deserializeArchive(file);
		
		//// DESERIALIZE VOX FILE

        std::cout << "\nVox Deserialization Started for " << filepath;

        /// PREP VARIABLES

        int colourPallete[256];
        VoxelHelper::Vector3Int modelSize;

		/// GET FILE SIZE

		file.seekg(0, std::ios::end); // Move read cursor to the end of the file
		std::streampos filesize = file.tellg();  // get position to determine file size.
		file.seekg(0, std::ios::beg); // Back to the start to begin reading the file.
		std::cout << "\nFileSize: " << filesize; // DEBUG

        /// FILE STRUCTURE / HEADER

		char id[4];
		deserializeArchive(id);
		int versionNum;
		deserializeArchive(versionNum);
		std::cout << "\nid: " << id[0] << id[1] << id[2] << id[3]; // DEBUG
		std::cout << "\nversion: " << versionNum; // DEBUG
        
        /// CHUNK STRUCTURE
        // id of 'MAIN' should be initial chunk, and parent of all other chunks, the importer should read children chunk recursively.

        bool customPalleteExists = false;
		while (file.tellg() < filesize)
		{
            char chunkID[4];
            deserializeArchive(chunkID);
            int numBytesOfChunkContent; 
            deserializeArchive(numBytesOfChunkContent);
            int numBytesOfChildrenContent;
            deserializeArchive(numBytesOfChildrenContent);

            std::string chunkIDString(chunkID, 4);
            std::cout << "\nchunkIDString: " << chunkIDString; // DEBUG
            if (chunkIDString == "MAIN")
            {

            }
            else if (chunkIDString == "SIZE")
            {
                // X, Y, Z
                // Note MagicaVoxel's vertical axis is z instead of y.
                deserializeArchive(modelSize.x);
                deserializeArchive(modelSize.z);
                deserializeArchive(modelSize.y);
            }
            else if (chunkIDString == "XYZI")
            {
                int numVoxels;
                deserializeArchive(numVoxels);
                std::cout << "\nNum of Voxels: " << numVoxels; // DEBUG

                voxels.resize(modelSize.x * modelSize.y * modelSize.z);

                std::cout << "\nVoxelArray Size: " << voxels.size(); // DEBUG

                for (int voxelNum = 0; voxelNum < numVoxels; voxelNum++)
                {
                    char x, y, z, colorIndex;
                    deserializeArchive(x);
                    deserializeArchive(y);
                    deserializeArchive(z);
                    deserializeArchive(colorIndex); // Position in RGBA Colour Pallete

                    Voxel voxel;
                    voxel.colourIndex = static_cast<uint8_t>(colorIndex);
                    // MagicaVoxel uses Z direction for up and down (Gravity direction).
                    voxels[CMP316engine::VoxelHelper::Convert3DPositionToIndex(x, y, z, modelSize.x, modelSize.y, modelSize.z)] = voxel; // TODO
                }
            }
            else if (chunkIDString == "RGBA") // This is how the colour index gets the correct colour (index based on position in pallete).
            {
                customPalleteExists = true;

                // NOTE: color [0-254] are mapped to palette index [1-255]
                for (int i = 0; i < 256; i++)
                {
                    int rgba;
                    deserializeArchive(rgba);
                    colourPallete[i] = rgba;
                    /*char red;
                    char green;
                    char blue;
                    char alpha;
                    deserializeArchive(red);
                    deserializeArchive(green);
                    deserializeArchive(blue);
                    deserializeArchive(alpha);
                    colourPallete[i] = (int)((alpha << 24) | (red << 16) | (green << 8) | blue);*/
                }
            }
            else // UNKNOWN CHUNK
            {
                std::cout << "\nUnknown MagicaVoxel Chunk!";
                file.seekg(numBytesOfChunkContent, std::ios::cur);
            }
		}
        file.close();

		/// DESERIALIZE COMPLETE

        std::cout << "\nVox Deserialization Complete\n"; // DEBUG

        voxelAsset.voxels = voxels;
        voxelAsset.modelSize = modelSize;
		return voxelAsset;
	}

	std::vector<Mesh> GenerateVoxelMesh(std::vector<Voxel> voxels)
	{
		return std::vector<Mesh> {};
	}
}