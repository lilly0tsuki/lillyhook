#pragma once
#include <lilly/hashmap.hpp>
enum class FieldValueType {
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
	Custom,
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
	Unknown = -1
};

struct SerializableField {
	int fieldId = 0x7FFFFFFF;
	char _field_8 = -1;
	unsigned short _field_A = 0;
	FieldValueType type = FieldValueType::Unknown;
	void* object0x18ptr = nullptr;
	SerializableField* manager = nullptr;

	static void initFieldManager(SerializableField*) {

	}
	virtual void dtrSerializableField() {
		if (manager) free(manager);
	}
	virtual const char* getType() {
		switch (this->type) {
			case FieldValueType::Int:
				return "Int";
			case FieldValueType::Float:
				return "Float";
			case FieldValueType::String:
				return "String";
			case FieldValueType::Int64:
				return "Int64";
			case FieldValueType::Vector3:
				return "Vector3";
			case FieldValueType::Vector4:
				return "Vector4";
			case FieldValueType::Bool:
				return "Bool";
			case FieldValueType::Coordinate:
				return "Coordinate";
			case FieldValueType::Object:
				return "Object";
			case FieldValueType::ObjectArray:
				return "ObjectArray";
			case FieldValueType::Map:
				return "Map";
			case FieldValueType::Array:
				return "Array";
			case FieldValueType::Custom:
				return "Custom";
			case FieldValueType::Color:
				return "Color";
			case FieldValueType::Enum:
				return "Enum";
			case FieldValueType::Vector2:
				return "Vector2";
			case FieldValueType::Double:
				return "Double";
			case FieldValueType::BigMap:
				return "BigMap";
			case FieldValueType::UInt:
				return "UInt";
			case FieldValueType::IntPair:
				return "IntPair";
			case FieldValueType::Quaternion:
				return "Quaternion";
			case FieldValueType::StringMap:
				return "StringMap";
			case FieldValueType::FormatString:
				return "FormatString";
			case FieldValueType::UInt64:
				return "UInt64";
			case FieldValueType::Unknown:
			default:
				return "Unknown";
		}
	}
	virtual void initializeManager() {
		if (!this->manager) {
			this->manager = reinterpret_cast<SerializableField*>(malloc(sizeof(SerializableField)));
			initFieldManager(this);
		}
	}
};

template <typename T>
struct TField : SerializableField {
	T data;

	TField() = default;
	TField(const T& value) : data(value) {}
	TField(T&& value) : data(value) {}

	T& get() noexcept {
		return this->data;
	}
	const T& get() const noexcept {
		return this->data;
	}
	operator T() noexcept {
		return this->data;
	}
	operator T() const noexcept {
		return this->data;
	}
};
template <
typename Key,
typename Value,
typename Hash,
typename KeyEqual,
typename Allocator
>
struct TField<Trove::hashmap<Key, Value, Hash, KeyEqual, Allocator>> : public SerializableField {
	struct TMapTrackerNode {
		uint32_t flags;
		uint32_t pad;
		TMapTrackerNode *parent;
		TMapTrackerNode *left;
		TMapTrackerNode *right;
		size_t size;
		size_t extra;
		size_t field_30;
	};

	struct Map {
		void** vtable;
		Trove::hashmap<Key, Value, Hash, KeyEqual, Allocator> m_hashmap;
		TMapTrackerNode tracker_modified;
		TMapTrackerNode tracker_removed;
	} value;
};
