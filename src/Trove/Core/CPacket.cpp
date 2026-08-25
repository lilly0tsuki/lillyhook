#include "Trove/Core/CPacket.h"
#include "lilly/string.hpp"
#include "lilly/vectors.h"
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace Trove {
	void ComponentPacket::packInt32(const int value) {
		buffer.write_zigzagvarint32(DATATYPE_INT32);
		buffer.write_zigzagvarint32(value);
	}
	void ComponentPacket::packUInt32(const unsigned int value) {
		buffer.write_zigzagvarint32(DATATYPE_UINT32);
		buffer.write_varint32(value);
	}
	void ComponentPacket::packBool(const bool value) {
		buffer.write_zigzagvarint32(DATATYPE_BOOL);
		buffer.write_zigzagvarint32(value ? 1 : 0);
	}
	void ComponentPacket::packFloat(const float value) {
		buffer.write_zigzagvarint32(DATATYPE_FLOAT);
		buffer.write_float(value);
	}
	void ComponentPacket::packDouble(const double value) {
		buffer.write_zigzagvarint32(DATATYPE_DOUBLE);
		buffer.write_double(value);
	}
	void ComponentPacket::packInt64(const long long value) {
		buffer.write_zigzagvarint32(DATATYPE_INT64);
		buffer.write_int64(value);
	}
	void ComponentPacket::packIVec3(const IVec3& value) {
		buffer.write_zigzagvarint32(DATATYPE_IVEC3);
		buffer.write_int32(value.x);
		buffer.write_int32(value.y);
		buffer.write_int32(value.z);
	}
	void ComponentPacket::packVec3(const Vec3& value) {
		buffer.write_zigzagvarint32(DATATYPE_VEC3);
		buffer.write_float(value.x);
		buffer.write_float(value.y);
		buffer.write_float(value.z);
	}
	void ComponentPacket::packVec4(const Vec4& value) {
		buffer.write_zigzagvarint32(DATATYPE_VEC4);
		buffer.write_float(value.x);
		buffer.write_float(value.y);
		buffer.write_float(value.z);
		buffer.write_float(value.t);
	}

	void ComponentPacket::packTString(const Trove::String& string) {
		buffer.write_zigzagvarint32(DATATYPE_TSTRING);
		buffer.write_string(string);
	}

	void ComponentPacket::packColor(const unsigned int color) {
		buffer.write_zigzagvarint32(DATATYPE_COLOR);
		buffer.write_int32(color);
	}
	void ComponentPacket::write_typed_arg(const Trove::String& type, const Trove::String& value) {

		if (type == "int" || type == "int32" || type == "int") {
			packInt32(value);
		} else if (type == "long" || type == "int64" || type == "long long") {
			packInt64(value);
		} else if (type == "float") {
			packFloat(value);
		} else if (type == "double") {
			packDouble(value);
		} else if (type == "bool") {
			bool boolVal = (value != "false" && value != "0");
			packBool(boolVal);
		} else if (type == "uint" || type == "uint32" || type == "unsigned int") {
			packUInt32(value);
		} else if (type == "tstring" || type == "string" || type == "trovestring") {
			packTString(value);
		} else if (type == "color") {
			packColor(value);
		} else if (type == "vec3" || type == "vec4" || type == "ivec3") {
			std::string val = value;
			std::vector<Trove::String> tokens;
			std::stringstream ss(val);
			std::string token;

			char delimiter = value.find(',') != std::string::npos ? ',' : ' ';
			while (std::getline(ss, token, delimiter)) {
				token.erase(0, token.find_first_not_of(" \t\n\r\f\v"));
				token.erase(token.find_last_not_of(" \t\n\r\f\v") + 1);
				if (!token.empty()) {
					tokens.emplace_back(token);
				}
			}

			if (type == "vec3") {
				if (tokens.size() == 3) {
					Vec3 vec3(tokens[0], tokens[1], tokens[2]);
					packVec3(vec3);
				} else {
					throw std::invalid_argument("Vec3 requires exactly 3 components");
				}
			} else if (type == "vec4") {
				if (tokens.size() == 4) {
					Vec4 vec4(tokens[0], tokens[1], tokens[2], tokens[3]);
					packVec4(vec4);
				} else {
					throw std::invalid_argument("Vec4 requires exactly 4 components");
				}
			} else if (type == "ivec3") {
				if (tokens.size() == 3) {
					IVec3 ivec3(tokens[0], tokens[1], tokens[2]);
					packIVec3(ivec3);
				} else {
					throw std::invalid_argument("IVec3 requires exactly 3 components");
				}
			}
		} else if (type == "vector") {
			std::string val = value;
			std::vector<int> values;
			std::stringstream ss(val);
			std::string token;

			char delimiter = value.find(',') != std::string::npos ? ',' : ' ';
			while (std::getline(ss, token, delimiter)) {
				token.erase(0, token.find_first_not_of(" \t\n\r\f\v"));
				token.erase(token.find_last_not_of(" \t\n\r\f\v") + 1);
				if (!token.empty()) {
					values.push_back(std::stoi(token));
				}
			}

			std::vector<int> intRange(values);
			packVector(intRange);
		} else {
			throw std::invalid_argument("Unsupported type.");
		}
	}
}
