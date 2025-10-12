#pragma once
#include <type_traits>
#include <cstdio>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <optional>

/*
ORDER OF SERIALIZATION MUST MATCH THE DESERIALIZATION ARCHIVE

Used by Scene Saving,
works in conjunction with the Basic_Snapshot under entt namespace.
Stream should be created at the start of the Save function, with a file to output the data.

NOTE:
If Component has DISABLE_COPY, you will need to add it back manually to the scene instead of using entt::snapshot!
 */

class BinarySerializeArchive
{
private:
	std::ofstream& stream;

public:
	BinarySerializeArchive(std::ofstream& fileStream, int version) : stream(fileStream) { (*this)(version); }
	~BinarySerializeArchive() = default;

	// Handles serialization of POD types which do not need special handling.
	template<typename Type>
	void operator()(const Type& data) {
		// Check if data is POD, if not then it can't be serialized / needs a custom operator implemented.
		if (std::is_trivially_copyable<Type>::value && std::is_standard_layout<Type>::value) {
			stream.write(reinterpret_cast<const char*>(&data), sizeof(data));
		}
		else {
			//std::cout << "\nNo Suitable Operator Found for Serialization"; //// DEBUG
			assert(false && "No Suitable Operator Found for Deserialization");
		}
	}

	template<typename Type>
	void operator()(const std::vector<Type>& data) {
		size_t vectorSize = data.size();
		(*this)(vectorSize);
		for (auto& element : data) {
			(*this)(element);
		}
	}

	template<typename Type>
	void operator()(const std::unique_ptr<Type>& data) {
		bool isNullptr = (data == nullptr);
		(*this)(isNullptr);

		if (!isNullptr) {
			(*this)(*data);
		}
	}

	template<typename Key, typename Value>
	void operator()(const std::map<Key, Value>& data) {
		size_t mapSize = data.size();
		(*this)(mapSize);

		for (auto& [key, value] : data) {
			(*this)(key);
			(*this)(value);
		}
	}

	template<typename Key, typename Value>
	void operator()(const std::unordered_map<Key, Value>& data) {
		size_t mapSize = data.size();
		(*this)(mapSize);

		// Sort to ensure consistent order on serialization
		std::vector<Key> sortedKeys;
		sortedKeys.reserve(mapSize);
		for (auto& [key, value] : data) {
			sortedKeys.push_back(key);
		}
		std::sort(sortedKeys.begin(), sortedKeys.end());

		for (auto& key : sortedKeys) {
			auto& value = data.at(key);
			(*this)(key);
			(*this)(value);
		}
	}

	void operator()(const std::string& data)
	{
		size_t length = data.size();
		(*this)(length);
		stream.write(data.c_str(), length);
	}

	template<typename type>
	void operator()(const std::optional<type>& data)
	{
		bool isNull = (data == std::nullopt);
		(*this)(isNull);

		if (!isNull) {
			(*this)(data.value());
		}
	}
};