#pragma once
#include "lilly/buffers.hpp"
#include "lilly/string.hpp"
#include "lilly/vectors.h"
#include <map>
#include <vector>

struct PrefabDeserializer {
	int field_id;
	unsigned int wire_type;
	BufferReader& buffer;
	bool skipField(unsigned int wire) const;
	bool readTag(int& fieldOut, unsigned int& wireOut) const;
	bool readTag();
	bool readInt(int& out) const;
	bool readUInt(unsigned int& out) const;
	bool readInt64(long long& out) const;
	bool readUInt64(unsigned long long& out) const;

	bool readFloat(float& out) const;
	bool readDouble(double& out) const;

	bool readBool(bool& out) const;

	bool readString(Trove::String& out) const;
	bool readFormatString(std::vector<Trove::String>& out);

	bool readVector2(Vec2& out) const;
	bool readVector3(Vec3& out) const;
	bool readVector4(Vec4& out) const;
	bool readQuaternion(Vec4& out) const;
	bool readColor(unsigned int& out) const;
	bool readCoordinate(IVec3& out) const;

	bool readMap(std::map<Trove::String, std::nullptr_t>& out) {return out.empty(); }
	bool readStringMap(std::map<Trove::String, Trove::String>& out) { return out.empty(); }
	bool readArrayStr(std::vector<Trove::String>& out) const;
	bool readEnum(int& out) { return out; }

	enum ValueType : unsigned int {
		Int, 
		Float, 
		String, 
		Int64,
		Vector3, 
		Vector4, 
		Bool, 
		Coordinate, 
		Object,
		ObjectArray, 
		Map, 
		Array, 
		Custom , 
		Color,
		Enum, 
		Vector2, 
		Double, 
		BigMap, 
		UInt,
		IntPair, 
		Quaternion, 
		StringMap, 
		FormatString,
		UInt64, 
		Unknown = 4294967295U
	};

	enum WireType : unsigned int {
		WT_VarintZigZag = 0,
		WT_Varint = 1,
		WT_Fixed32 = 2,
		WT_Fixed64 = 3,
		WT_LengthDelimited = 4,
		WT_Vector = 6,
		WT_Array = 31,
		WT_Terminator = 4294967295U
	};

};
