#pragma once
#include "lilly/buffers.hpp"
#include "lilly/string.hpp"
#include <vector>

struct Packet;
struct PacketSerializer {
	IWriteBuffer* buffer;

	static uint32_t make_tag(uint16_t field_id, int32_t wire_type);

	PacketSerializer(IWriteBuffer*& wr) : buffer(wr) {}

	bool write_tag(uint16_t field_id, uint32_t wire_type) const;
	bool write_int32(uint16_t field_id, const int32_t& value) const;
	bool write_int64(uint16_t field_id, const int64_t& value) const;
	bool write_bool(uint16_t field_id, const bool& value) const;
	bool write_float(uint16_t field_id, const float& value) const;
	bool write_double(uint16_t field_id, const double& value) const;
	bool write_string(uint16_t field_id, const Trove::String& value) const;
	bool write_packet(uint16_t field_id, Packet*& value);
	bool write_view(uint16_t field_id, BufferView*& value) const;

	template<typename T>
	bool write_vector(const uint16_t field_id, const std::vector<T>& vec) {
		if (!write_tag(field_id, 31))
			return false;

		if (!buffer->write_varint32(static_cast<uint32_t>(vec.size())))
			return false;

		for (const T& element : vec) {
			if constexpr (std::is_same_v<T, int32_t>) {
				if (!write_int32(0, element)) return false;
			} else if constexpr (std::is_same_v<T, int64_t>) {
				if (!write_int64(0, element)) return false;
			} else if constexpr (std::is_same_v<T, bool>) {
				if (!write_bool(0, element)) return false;
			} else if constexpr (std::is_same_v<T, float>) {
				if (!write_float(0, element)) return false;
			} else if constexpr (std::is_same_v<T, Trove::String>) {
				if (!write_string(0, element)) return false;
			} else {
				static_assert(sizeof(T) == 0, "Unsupported type");
			}
		}

		return true;
	}

	bool write_typed_arg(uint16_t field_id, const Trove::String& type, const Trove::String& value) const noexcept;
	template<typename T>
	void write_arg(uint16_t field_id, const T& value) {
		using Type = std::decay_t<T>;

		if constexpr (std::is_same_v<Type, int32_t>) {
			if (value != 0) write_int32(field_id, value);
		} else if constexpr (std::is_same_v<Type, int64_t>) {
			if (value != 0) write_int64(field_id, value);
		} else if constexpr (std::is_same_v<Type, uint32_t>) {
			if (const int32_t signed_val = static_cast<int32_t>(value); signed_val != 0)
				write_int32(field_id, signed_val);
		} else if constexpr (std::is_same_v<Type, float>) {
			if (value != 0.0f) write_float(field_id, value);
		} else if constexpr (std::is_same_v<Type, bool>) {
			if (value) write_bool(field_id, value);
		} else if constexpr (std::is_same_v<Type, Trove::String>) {
			if (value) write_string(field_id, value);
		} else {
			static_assert(sizeof(T) == 0, "Unsupported type");
		}
	}
};
