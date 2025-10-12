#pragma once
#include <type_traits>
#include <cstdio>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <optional>

//////////////////////////////////////////////////////////////////////////////////////////

/*
ORDER OF DESERIALIZATION MUST MATCH THE SERIALIZATION ARCHIVE

Used by Scene Loading,
works in conjunction with the Basic_Snapshot_Loader under entt namespace.
Stream should be created at the start of the Load function, with a file to read the data.

NOTE:
If Component has DISABLE_COPY, you will need to add it back manually to the scene instead of using entt::snapshot!

FILE VERSION & BACKWORD COMPATIBILITY:
~ Refer to Example Code
If you update a component that is pre-existing and want to keep old save files compatible (e.g. level files), you will need to ensure it has an operator in this class and the new variable is wrapped in a
conditional to check the file version is matching. You should be incrementing the file version for each set of additions / changes you make.
This is also relevant to components that were trivially serializable and didn't need a custom operator, of which you will now need to make one so that you can add the file version check.
You don't need to change the file version if the component was never serialized.

VARIABLE REMOVED:
If a variable in a component is removed, when deserializing do a file version check as normal, but load into a dummy variable (Since the old variable is no longer defined).

VARIABLE MODIFIED:
Recreate deserialized variable into the new format based on old format. Can do this by creating a temporary variable in the old data format for deserialization.

VARIABLE ADDED:
Wrap in file version, ensure only deserialized on that version or greater.
 */

//////////////////////////////////////////////////////////////////////////////////////////

/* 
EXAMPLE OF FILE_VERSION HANDLING

if (saveFileVersion >= 2) {
	(*this)(component.skilltrees); // This is an unordered_map of skillTrees, compared to the old file version which only handled 1.
}
else {
	std::vector<Skill> legacySkillTree; (*this)(legacySkillTree);
	component.skilltrees["Weapons"] = legacySkillTree;
}
(*this)(component.skillPoints);
if (saveFileVersion <= 1) { bool temp; (*this)(temp); } // Obsolete uiOpen variable, use dummy variable to deserialize

if (saveFileVersion >= 2) {
	(*this)(component.nodesPerColumn); // Like skilltrees variable, this now handles multiple skilltrees
}
else {
	std::map<int, int> legacyNodesPerColumn; (*this)(legacyNodesPerColumn);
	component.nodesPerColumn["Weapons"] = legacyNodesPerColumn;
}
*/

//////////////////////////////////////////////////////////////////////////////////////////

class BinaryDeserializeArchive
{
private:
	std::ifstream& stream;
	/* This allows for backward compatibility, simply wrap new serialized code in
	conditionals to ensure old loaded files maintain the same serialization and hence are still readable.
	Update File Version in the RendererScene.h (Or whatever class contains the version for that type of file).
	*/
	int saveFileVersion = 0;

public:
	BinaryDeserializeArchive(std::ifstream& fileStream) : stream(fileStream) { (*this)(saveFileVersion); std::cout << "\nLoading File Version: " << saveFileVersion; }
	~BinaryDeserializeArchive() = default;

	int getFileVersion() { return saveFileVersion; };

	// Handles serialization of POD types which do not need special handling.
	template<typename Type>
	void operator()(Type& data)
	{
		// Check if data is POD, if not then it can't be serialized / needs a custom operator implemented.
		if (std::is_trivially_copyable<Type>::value && std::is_standard_layout<Type>::value) {
			stream.read(reinterpret_cast<char*>(&data), sizeof(data));
		}
		else {
			//std::cout << "\nNo Suitable Operator Found for Deserialization"; //// DEBUG
			assert(false && "No Suitable Operator Found for Deserialization");
		}
	}

	template<typename Type>
	void operator()(std::vector<Type>& data) {
		size_t vectorSize;
		(*this)(vectorSize);
		data.reserve(vectorSize);
		// Load each object from file into the vector.
		for (int i = 0; i < vectorSize; i++) {
			Type temp;
			(*this)(temp);
			data.push_back(temp);
		}
	}

	template<typename Type>
	void operator()(std::unique_ptr<Type>& data) {
		bool isNullptr;
		(*this)(isNullptr);

		if (!isNullptr) {
			(*this)(*data);
		}
	}

	template<typename Key, typename Value>
	void operator()(std::map<Key, Value>& data) {
		size_t mapSize;
		(*this)(mapSize);
		//data.reserve(mapSize);

		for (int i = 0; i < mapSize; i++) {
			Key tempK;
			Value tempV;
			(*this)(tempK);
			(*this)(tempV);
			data[tempK] = tempV;
		}
	}

	template<typename Key, typename Value>
	void operator()(std::unordered_map<Key, Value>& data) {
		size_t mapSize;
		(*this)(mapSize);
		//data.reserve(mapSize);

		for (int i = 0; i < mapSize; i++) {
			Key tempK;
			Value tempV;
			(*this)(tempK);
			(*this)(tempV);
			data[tempK] = tempV;
		}
	}

	void operator()(std::string& data)
	{
		size_t length;
		(*this)(length);
		data.resize(length);
		stream.read(&data[0], length);
	}

	template<typename type>
	void operator()(std::optional<type>& data)
	{
		bool isNull;
		(*this)(isNull);

		if (!isNull) {
			type temp;
			(*this)(temp);
			data = temp;
		}
		else { data = std::nullopt; }
	}
};