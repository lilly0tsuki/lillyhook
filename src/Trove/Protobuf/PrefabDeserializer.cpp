#include "Trove/Protobuf/PrefabDeserializer.h"
#include "lilly/string.hpp"
#include "lilly/vectors.h"
#include <vector>

bool PrefabDeserializer::readTag(int& field_id_out, unsigned int& wire_type_out) const {
	if (int tag; buffer.read_zigzagvarint32(tag)) {
		field_id_out = tag >> 3;
		wire_type_out = tag & 7;
		if (wire_type_out == 7) {
			field_id_out = tag >> 6;
			wire_type_out = tag & 63;
			if (wire_type_out == 15) {
				field_id_out = 2147483647;
			}
		}
		return true;
	}
	field_id_out = 2147483647;
	wire_type_out = WireType::WT_Terminator;
	return false;
}
bool PrefabDeserializer::readTag() {
	if (int tag; buffer.read_zigzagvarint32(tag)) {
		field_id = tag >> 3;
		wire_type = tag & 7;
		if (wire_type == 7) {
			field_id = tag >> 6;
			wire_type = tag & 63;
			if (wire_type == 15) {
				field_id = 2147483647;
			}
		}
		return true;
	}
	field_id = 2147483647;
	wire_type = WireType::WT_Terminator;
	return false;
}
bool PrefabDeserializer::skipField(unsigned int wire) const {
	switch (wire) {
		case WireType::WT_VarintZigZag: {
			return buffer.read_zigzagvarint32(reinterpret_cast<int&>(wire));
		}
		case WireType::WT_Varint: {
			return buffer.read_varint32(wire);
		}
		case WireType::WT_Fixed32: {
			return buffer.advanceRead(4);
		}
		case WireType::WT_Fixed64: {
			return buffer.advanceRead(8);
		}
		case WireType::WT_LengthDelimited: {
			Trove::String tempstr;
			return buffer.read_string(tempstr);
		}
		case WireType::WT_Vector: {
			return false;
		}
		default: {
			return true;
		}
	}
}
bool PrefabDeserializer::readInt(int& out) const {
	if (wire_type == WireType::WT_VarintZigZag) return buffer.read_zigzagvarint32(out);
	if (wire_type == WireType::WT_Varint) return buffer.read_varint32(reinterpret_cast<unsigned int&>(out));
	return skipField(wire_type);
}
bool PrefabDeserializer::readUInt(unsigned int& out) const {
	if (wire_type == WireType::WT_VarintZigZag) return buffer.read_zigzagvarint32(reinterpret_cast<int&>(out));
	if (wire_type == WireType::WT_Varint) return buffer.read_varint32(out);
	return skipField(wire_type);
}

bool PrefabDeserializer::readInt64(long long& out) const {
	if (wire_type == WireType::WT_Fixed64) return buffer.read_zigzagvarint64(out);
	return skipField(wire_type);
}
bool PrefabDeserializer::readUInt64(unsigned long long& out) const {
	if (wire_type == WireType::WT_Fixed64) return buffer.read_int64(reinterpret_cast<long long&>(out));
	return skipField(wire_type);
}

bool PrefabDeserializer::readFloat(float& out) const {
	if (wire_type == WireType::WT_Fixed32) return buffer.read_float(out);
	return skipField(wire_type);
}
bool PrefabDeserializer::readDouble(double& out) const {
	if (wire_type == WireType::WT_Fixed64) return buffer.read_double(out);
	return skipField(wire_type);
}

bool PrefabDeserializer::readBool(bool& out) const {
	if (wire_type == WireType::WT_VarintZigZag) {
		int temp;
		const bool success = buffer.read_zigzagvarint32(temp);
		out = temp != 0;
		return success;
	}
	return skipField(wire_type);
}

bool PrefabDeserializer::readString(Trove::String& out) const {
	if (wire_type == WireType::WT_LengthDelimited) return buffer.read_string(out);
	return skipField(wire_type);
}
bool PrefabDeserializer::readFormatString(std::vector<Trove::String>& out) {
	if (wire_type == WireType::WT_LengthDelimited) {
		Trove::String str;
		if (!buffer.read_string(str)) return false;
		out.push_back(str);
		return true;
	}
	if (wire_type == WireType::WT_Array) return readArrayStr(out);
	return skipField(wire_type);
}
bool PrefabDeserializer::readVector2(Vec2& out) const {
	return skipField(wire_type) && out.x;
}
bool PrefabDeserializer::readVector3(Vec3& out) const {

	int fieldCount = 0;
	unsigned int wire = 0;

	if (!readTag(fieldCount, wire))
		return false;

	if (fieldCount != 3 || wire != 2)
		return skipField(wire_type);

	if (!buffer.read_float(out.x)) return false;
	if (!buffer.read_float(out.y)) return false;
	if (!buffer.read_float(out.z)) return false;

	return true;
}

bool PrefabDeserializer::readVector4(Vec4& out) const {
	return skipField(wire_type) && out.x;
}
bool PrefabDeserializer::readQuaternion(Vec4& out) const {
	return skipField(wire_type) && out.x;
}
bool PrefabDeserializer::readColor(unsigned int& out) const {
	return skipField(wire_type) && out;
}
bool PrefabDeserializer::readCoordinate(IVec3& out) const {
	return skipField(wire_type) && out.x;
}

bool PrefabDeserializer::readArrayStr(std::vector<Trove::String>& out) const {
	int item_count;
	unsigned int element_wire;
	if (!readTag(item_count, element_wire))
		return false;
	if (element_wire != ValueType::String)
		return false;

	out.clear();
	out.resize(item_count);
	std::vector<bool> present(item_count, false);

	while (true) {
		int index = 0;
		unsigned int wire = 0;

		if (!readTag(index, wire))
			break;
		if (wire != ValueType::String)
			break;

		if (index >= item_count)
			return false;

		Trove::String str;
		if (!buffer.read_string(str))
			return false;

		out[index] = str;
		present[index] = true;
	}
	return true;
}
