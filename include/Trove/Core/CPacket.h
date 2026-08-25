#pragma once
#include "lilly/buffers.hpp"
#include "lilly/string.hpp"
#include "lilly/vectors.h"
#include <string>
#include <vector>

namespace Trove {
	enum DataType : int {
		DATATYPE_INT32 = 0,
		DATATYPE_UINT32 = 1,
		DATATYPE_BOOL = 1,
		DATATYPE_INT64 = 3,
		DATATYPE_TSTRING = 4,
		DATATYPE_FLOAT = 5,
		DATATYPE_DOUBLE = 6,
		DATATYPE_VEC3 = 7,
		DATATYPE_IVEC3 = 8,
		DATATYPE_VEC4 = 9,
		DATATYPE_COLOR = 10,
		DATATYPE_VECTOR = 11,
		DATATYPE_MAT3x3 = 12,
	};
	struct ComponentPacket {
		void packInt32(int value);
		void packUInt32(unsigned int value);
		void packBool(bool value);
		void packFloat(float value);
		void packDouble(double value);
		void packInt64(long long value);
		void packVec3(const Vec3& value);
		void packVec4(const Vec4& value);
		void packIVec3(const IVec3& value);
		void packTString(const Trove::String& string);
		void packColor(unsigned int color);

		template<typename T>
		void packVector(const std::vector<T>& vec) {
			buffer.write_zigzagvarint32(DATATYPE_VECTOR);
			packUInt32(static_cast<unsigned int>(vec.size()));

			for (const auto& val : vec) {
				packSingle(val);
			}
		}

		template<typename T>
		void packSingle(const T& value) {
			if constexpr (std::is_same_v<T, int>) {
				packInt32(value);
			} else if constexpr (std::is_same_v<T, unsigned int>) {
				packUInt32(value);
			} else if constexpr (std::is_same_v<T, bool>) {
				packBool(value);
			} else if constexpr (std::is_same_v<T, float>) {
				packFloat(value);
			} else if constexpr (std::is_same_v<T, double>) {
				packDouble(value);
			} else if constexpr (std::is_same_v<T, long long>) {
				packInt64(value);
			} else if constexpr (std::is_same_v<T, Vec3>) {
				packVec3(value);
			} else if constexpr (std::is_same_v<T, Vec4>) {
				packVec4(value);
			} else if constexpr (std::is_same_v<T, IVec3>) {
				packIVec3(value);
			} else if constexpr (std::is_same_v<T, Trove::String>) {
				packTString(value);
			} else if constexpr (std::is_same_v<T, const char*> || std::is_same_v<T, char*>) {
				packTString(value);
			} else if constexpr (std::is_same_v<T, std::string>) {
				packTString(value);
			} else if constexpr (std::is_same_v<std::remove_cv_t<std::remove_reference_t<T>>, std::vector<int>> or
				std::is_same_v<std::remove_cv_t<std::remove_reference_t<T>>, std::vector<unsigned int>> or
				std::is_same_v<std::remove_cv_t<std::remove_reference_t<T>>, std::vector<long long>> or
				std::is_same_v<std::remove_cv_t<std::remove_reference_t<T>>, std::vector<float>> or
				std::is_same_v<std::remove_cv_t<std::remove_reference_t<T>>, std::vector<double>> or
				std::is_same_v<std::remove_cv_t<std::remove_reference_t<T>>, std::vector<bool>> or
				std::is_same_v<std::remove_cv_t<std::remove_reference_t<T>>, std::vector<std::string>> or
				std::is_same_v<std::remove_cv_t<std::remove_reference_t<T>>, std::vector<Trove::String>>) {
				packVector(value);
			} else {
				static_assert(sizeof(T) == 0, "Unsupported type for Packet. Check documentation for supported types.");
			}
		}

		template <typename... Args>
		void pack(Args&&... args) {
			(packSingle(std::forward<Args>(args)), ...);
		}

	
		struct ComponentPacketHeader {
			const char* msg;
			int type;
			int id;
			unsigned int field_c;
		};

		ComponentPacketHeader header{};
		BufferWriter buffer;

		template <typename... Args>
		ComponentPacket(const char* name, Args&&... args) {
			header.msg = name;
			header.type = 3;
			header.id = 0;
			header.field_c = 0;

			if constexpr (sizeof...(args) > 0) {
				pack(std::forward<Args>(args)...);
			}
		}

		void write_typed_arg(const Trove::String& type, const Trove::String& value);
	};
}
