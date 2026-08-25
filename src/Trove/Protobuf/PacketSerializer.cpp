#include "Trove/Core/Packet.h"
#include "Trove/Protobuf/PacketSerializer.h"
#include "lilly/buffers.hpp"
#include "lilly/encoding.hpp"
#include "lilly/string.hpp"

uint32_t PacketSerializer::make_tag(const uint16_t field_id, const int32_t wire_type) {
	return (wire_type < 7) ?
		(wire_type & 7) | (field_id << 3) :
		(wire_type & 63) | (field_id << 6);
}

bool PacketSerializer::write_tag(const uint16_t field_id, const uint32_t wire_type) const {
	return buffer->write_varint32(make_tag(field_id, wire_type));
}

bool PacketSerializer::write_int32(const uint16_t field_id, const int32_t& value) const {
	if (value == 0) return write_tag(field_id, 0);
	if (value == 1) return write_tag(field_id, 1);
	if (value < 0) return write_tag(field_id, 2) && buffer->write_zigzagvarint32(value);
	return write_tag(field_id, 3) && buffer->write_varint32(value);
}

bool PacketSerializer::write_int64(const uint16_t field_id, const int64_t& value) const {
	if (value == 0) return write_tag(field_id, 0);
	if (value == 1) return write_tag(field_id, 1);
	if (value < 0) return write_tag(field_id, 2) && buffer->write_zigzagvarint64(value);
	return write_tag(field_id, 3) && buffer->write_varint64(value);
}

bool PacketSerializer::write_bool(const uint16_t field_id, const bool& value) const {
	return value ?
		write_tag(field_id, 1) :
		write_tag(field_id, 0);
}

bool PacketSerializer::write_float(const uint16_t field_id, const float& value) const {
	if (value == 0.0f) return write_tag(field_id, 0);
	else if (value != 1.0f) return write_tag(field_id, 5) && buffer->write_float(value);

	return write_tag(field_id, 1);
}
bool PacketSerializer::write_double(const uint16_t field_id, const double& value) const {
	if (value == 0.0) return write_tag(field_id, 0);
	else if (value != 1.0f) return write_tag(field_id, 5) && buffer->write_double(value);

	return write_tag(field_id, 1);
}
bool PacketSerializer::write_string(const uint16_t field_id, const Trove::String& value) const {
	return write_tag(field_id, 6) && buffer->write_string(value);
}

bool PacketSerializer::write_packet(const uint16_t field_id, Packet*& value) {
	if (!value || !value->verifyKey()) return false;
	return write_tag(field_id, 23) && buffer->write_varint32(value->getKey()) && value->write(buffer);
}
bool PacketSerializer::write_view(const uint16_t field_id, BufferView*& value) const {
	uint8_t size[10];
	const size_t encodedSize = varintEncode(value->size, size);
	return write_tag(field_id, 6) &&
		buffer->write_bytes(size, encodedSize) &&
		buffer->write_bytes(value->data, value->size);
}

bool PacketSerializer::write_typed_arg(const uint16_t field_id, const Trove::String& type, const Trove::String& value) const noexcept {
	if (type == "int" || type == "int32") {
		const int32_t v = value;
		return v != 0 && write_int32(field_id, v);
	} else if (type == "int64") {
		const int64_t v = value;
		return v != 0 && write_int64(field_id, v);
	} else if (type == "float") {
		const float v = value;
		return v != 0.0f && write_float(field_id, v);
	} else if (type == "double") {
		const double v = value;
		return v != 0.0 && write_double(field_id, v);
	} else if (type == "bool") {
		const bool v = (value != "0" && value.lower() != "false");
		return v != false && write_bool(field_id, v);
	} else if (type == "uint" || type == "uint32") {
		const uint32_t v = value;
		const int32_t asSigned = static_cast<int32_t>(v);
		return asSigned != 0 && write_int32(field_id, asSigned);
	} else if (type == "string") {
		return value && write_string(field_id, value);
	} else {
		return false;
	}
}
