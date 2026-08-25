#pragma once

#include "lilly/encoding.hpp"
#include "lilly/string.hpp"

#include <algorithm>
#include <cstring>
#include <span>
#include <vector>

struct BufferView {
	uint8_t* data = nullptr;
	size_t size = 0;
	bool ownsData = false;

	void set(uint8_t* dataPtr, const size_t dataSize) noexcept {
		data = dataPtr;
		size = dataSize;
		ownsData = false;
	}
};
struct IWriteBuffer {

	virtual void dtrIWriteBuffer() {};
	virtual bool getWriteView(BufferView& view) = 0;
	virtual bool advanceWrite(size_t bytes) = 0;

	bool write_bytes(const uint8_t* data, const size_t size) {
		size_t totalWritten = 0;
		while (totalWritten < size) {
			BufferView destView;
			if (!this->getWriteView(destView)) return false;

			size_t remaining = size - totalWritten;
			const size_t toCopy = std::min(destView.size, remaining);
			if (toCopy == 0) return false;

			std::memcpy(destView.data, data + totalWritten, toCopy);
			this->advanceWrite(toCopy);
			totalWritten += toCopy;
		}
		return true;
	}
	bool write_byte(const uint8_t value) {
		return write_bytes(&value, sizeof(uint8_t));
	}
	bool write_int32(int32_t value) {
		return write_bytes(reinterpret_cast<uint8_t*>(&value), sizeof(int32_t));
	}
	bool write_int64(int64_t value) {
		return write_bytes(reinterpret_cast<uint8_t*>(&value), sizeof(int64_t));
	}
	bool write_float(float value) {
		return write_bytes(reinterpret_cast<uint8_t*>(&value), sizeof(float));
	}
	bool write_double(double value) {
		return write_bytes(reinterpret_cast<uint8_t*>(&value), sizeof(double));
	}
	bool write_varint32(const uint32_t value) {
		uint8_t result[5];
		const size_t size = varintEncode(value, result);
		return write_bytes(result, size);
	}
	bool write_varint64(const uint64_t value) {
		uint8_t result[10];
		const size_t size = varintEncode(value, result);
		return write_bytes(result, size);
	}
	bool write_zigzagvarint32(const int32_t value) {
		uint8_t result[5];
		const size_t size = varintEncode(zigzagEncode(value), result);
		return write_bytes(result, size);
	}
	bool write_zigzagvarint64(const int64_t value) {
		uint8_t result[10];
		const size_t size = varintEncode(zigzagEncode(value), result);
		return write_bytes(result, size);
	}
	bool write_string(const Trove::String& value) {
		if (!write_varint64(value.size())) return false;
		if (value.empty()) return true;
		return write_bytes(reinterpret_cast<const uint8_t*>(value.c_str()), value.size());
	}
};
struct IReadBuffer {

	virtual void dtrIReadBuffer() {};
	virtual bool getReadView(BufferView& view) = 0;
	virtual bool advanceRead(size_t bytes) = 0;

	bool read_bytes(uint8_t* out, const size_t size) {
		size_t totalRead = 0;
		while (totalRead < size) {
			BufferView srcView;
			if (!this->getReadView(srcView)) return false;

			const size_t remaining = size - totalRead;
			const size_t toCopy = std::min(srcView.size, remaining);
			if (toCopy == 0) return false;

			std::memcpy(out + totalRead, srcView.data, toCopy);

			this->advanceRead(toCopy);
			totalRead += toCopy;
		}
		return true;
	}
	bool read_int32(int32_t& out) {
		return read_bytes(reinterpret_cast<uint8_t*>(&out), sizeof(int32_t));
	}
	bool read_int64(int64_t& out) {
		return read_bytes(reinterpret_cast<uint8_t*>(&out), sizeof(int64_t));
	}
	bool read_float(float& out) {
		return read_bytes(reinterpret_cast<uint8_t*>(&out), sizeof(float));
	}
	bool read_double(double& out) {
		return read_bytes(reinterpret_cast<uint8_t*>(&out), sizeof(double));
	}
	bool read_varint32(uint32_t& out) {
		out = 0;
		uint32_t shift = 0;
		for (uint32_t i = 0; i < 5; ++i) {
			uint8_t byte;
			if (!read_bytes(&byte, 1)) {
				return false;
			}
			out |= static_cast<uint32_t>(byte & 0x7F) << shift;
			shift += 7;
			if (!(byte & 0x80)) {
				return true;
			}
		}
		return false;
	}
	bool read_varint64(uint64_t& out) {
		out = 0;
		uint64_t shift = 0;
		for (uint64_t i = 0; i < 10; ++i) {
			uint8_t byte;
			if (!read_bytes(&byte, 1)) {
				return false;
			}
			out |= static_cast<uint64_t>(byte & 0x7F) << shift;
			shift += 7;
			if (!(byte & 0x80)) {
				return true;
			}
		}
		return false;
	}
	bool read_zigzagvarint32(int32_t& out) {
		uint32_t value;
		if (!read_varint32(value)) {
			return false;
		}
		out = zigzagDecode(value);
		return true;
	}
	bool read_zigzagvarint64(int64_t& out) {
		uint64_t value;
		if (!read_varint64(value)) {
			return false;
		}
		out = zigzagDecode(value);
		return true;
	}
	bool read_string(Trove::String& out) {
		size_t size;
		if (!read_varint64(size) || size > 0xffffffffu - 2) {
			return false;
		}
		out.resize(size);
		if (size > 0 && !read_bytes(reinterpret_cast<uint8_t*>(&out[0]), size)) {
			return false;
		}
		return true;
	}
};

inline size_t CopyBuffer(IReadBuffer* src, IWriteBuffer* dest) {
	if (!src || !dest) return 0;
	BufferView srcView, destView;
	size_t bytesWritten = 0;
	while (src->getReadView(srcView)) {
		if (!dest->getWriteView(destView)) break;
		size_t size = std::min(srcView.size, destView.size);
		std::memcpy(destView.data, srcView.data, size);
		bytesWritten += size;
		src->advanceRead(size);
		dest->advanceWrite(size);
	}
	return bytesWritten;
}
struct BufferWriter final : IWriteBuffer {

	BufferWriter(const size_t size = 1024) noexcept : buffer(size), writePosition(0) {}
	bool getWriteView(BufferView& view) override {
		if (buffer.size() - writePosition <= 64)
			ensureCapacity(writePosition + 64);
		view.set(buffer.data() + writePosition, buffer.size() - writePosition);
		return true;
	}
	bool getReadView(BufferView& view) const {
		if (writePosition == 0) {
			return false;
		}
		view.set(const_cast<uint8_t*>(buffer.data()), writePosition);
		return true;
	}
	bool advanceWrite(const size_t bytes) override {
		if (writePosition + bytes > buffer.size())
			return false;
		writePosition += bytes;
		return true;
	}
	[[nodiscard]] const uint8_t* data() const {
		return buffer.data();
	}
	[[nodiscard]] size_t getSize() const {
		return writePosition;
	}
	void ensureCapacity(const size_t required) {
		if (buffer.size() >= required) return;

		size_t newCapacity = buffer.size();
		do newCapacity = (3 * newCapacity) >> 1;
		while (newCapacity < required);

		buffer.resize(newCapacity);
	}

	std::vector<uint8_t> buffer;
	size_t writePosition;
};
struct BufferReader final : IReadBuffer {

	BufferReader(const uint8_t* buffer, size_t size) :
		bufferSpan(buffer, size),
		readPosition(0) {}
	[[nodiscard]] size_t getRemaining() const {
		return bufferSpan.size() - readPosition;
	}
	bool getReadView(BufferView& view) override {
		const size_t availableSize = bufferSpan.size() - readPosition;
		if (availableSize == 0) {
			return false;
		}
		view.set(const_cast<uint8_t*>(bufferSpan.data() + readPosition), availableSize);
		return true;
	}
	bool advanceRead(const size_t bytes) override {
		if (readPosition + bytes > bufferSpan.size()) {
			return false;
		}
		readPosition += bytes;
		return true;
	}

	std::span<const uint8_t> bufferSpan;
	size_t readPosition;
};
struct BufferNode {
	uint8_t* dataPtr = nullptr;
	size_t size = 0;
	uint8_t* readPosition = nullptr;
	uint8_t* writePosition = nullptr;
	BufferNode* next = nullptr;
	uint8_t data[1]{};

	[[nodiscard]] size_t getWriteSize() const {
		return size - (writePosition - dataPtr);
	}
	[[nodiscard]] size_t getReadSize() const {
		return writePosition - readPosition;
	}
};
struct NodeBufferWriter final : IWriteBuffer {

	size_t availableReadBytes = 0;
	size_t writeCapacity = 0;
	BufferNode* headNode = nullptr;
	BufferNode* tailNode = nullptr;
	BufferNode* lastNode = nullptr;
	size_t lastNodeSize = 64;
	size_t nodeSizeLimit = 4076;
	int maxSize = -1;
	size_t totalAllocatedBytes = 0;
	NodeBufferWriter(const size_t maxSize = -1) : maxSize(maxSize) {}
	void dtrNodeBufferWriter() {
		clearNodes();
	}
	bool getWriteView(BufferView& view) override {
		if (!writeCapacity) {
			ensureWriteCapacity(nodeSizeLimit);
		}

		if (!tailNode || !writeCapacity) {
			return false;
		}

		view.set(
			tailNode->writePosition,
			tailNode->getWriteSize()
		);

		return true;
	}
	bool advanceWrite(const size_t bytes) override {
		if (bytes == 0) return true;
		if (writeCapacity < bytes || tailNode == nullptr) {
			return false;
		}

		size_t remaining = bytes;
		while (remaining > 0) {
			if (!tailNode) break;

			size_t writable = tailNode->getWriteSize();
			if (writable == 0) {
				if (tailNode->next) tailNode = tailNode->next;
				break;
			}

			const size_t step = writable < remaining ? writable : remaining;

			tailNode->writePosition += step;
			remaining -= step;

			writeCapacity -= step;
			availableReadBytes += step;

			if (tailNode->getWriteSize() == 0) {
				tailNode = tailNode->next;
			}
		}

		return remaining == 0;
	}
	bool ensureWriteCapacity(const size_t required) {
		auto npot = [](size_t n) -> size_t {
			if (n == 0) return 1;
			n--;
			n |= n >> 1;
			n |= n >> 2;
			n |= n >> 4;
			n |= n >> 8;
			n |= n >> 16;
			return n + 1;
		};

		if (required == 0) return true;

		while (writeCapacity < required) {
			const size_t deficit = required - writeCapacity;
			const size_t neededTotal = deficit + sizeof(BufferNode);

			size_t calcSize = std::max(npot(neededTotal), lastNodeSize);
			size_t nodeSize = std::min(calcSize, defaultNodeSize);
			if (nodeSize < neededTotal) {
				nodeSize = npot(neededTotal);
			}

			auto* newNode = static_cast<BufferNode*>(std::malloc(nodeSize));
			if (!newNode) return false;

			newNode->dataPtr = newNode->data;
			newNode->size = nodeSize - sizeof(BufferNode);
			newNode->readPosition = newNode->data;
			newNode->writePosition = newNode->data;
			newNode->next = nullptr;

			if (lastNode) {
				lastNode->next = newNode;
			}
			lastNode = newNode;

			if (!headNode) {
				headNode = newNode;
			}

			if (!tailNode) {
				tailNode = newNode;
			}

			writeCapacity += newNode->size;
			totalAllocatedBytes += nodeSize;

			lastNodeSize = nodeSize;
		}

		if (tailNode && tailNode->getWriteSize() == 0 && tailNode->next) {
			tailNode = tailNode->next;
		}

		return true;
	}
	void clearNodes() {
		BufferNode* node = headNode;

		while (node) {
			BufferNode* next = node->next;

			totalAllocatedBytes -= node->size;
			std::free(node);

			node = next;
		}

		headNode = nullptr;
		tailNode = nullptr;
		lastNode = nullptr;

		availableReadBytes = 0;
		writeCapacity = 0;
	}
	static constexpr size_t defaultNodeSize = 0x100000;
};
struct NodeBuffer final : IReadBuffer {

	NodeBufferWriter buffer;

	void dtrNodeBuffer() {
		buffer.clearNodes();
	}
	bool getReadView(BufferView& view) override {
		if (!buffer.headNode || buffer.availableReadBytes == 0) {
			return false;
		}

		view.set(
			buffer.headNode->readPosition,
			buffer.headNode->getReadSize()
		);

		return true;
	}
	bool advanceRead(size_t bytes) override {
		if (bytes == 0) return true;

		if (buffer.availableReadBytes < bytes || buffer.headNode == nullptr) {
			return false;
		}

		size_t remaining = bytes;

		while (remaining > 0) {
			BufferNode* node = buffer.headNode;
			if (!node) {
				break;
			}

			size_t readable = node->getReadSize();
			if (readable == 0) {
				break;
			}

			size_t step = readable < remaining ? readable : remaining;

			buffer.availableReadBytes -= step;
			remaining -= step;
			node->readPosition += step;

			bool nodeFullyRead = (buffer.headNode->writePosition == buffer.headNode->readPosition);
			bool nodeFullyWritten = (buffer.headNode->getWriteSize() == 0);

			if (nodeFullyRead && nodeFullyWritten) {
				BufferNode* old = detachHeadNode();

				bool canRecycle = (buffer.maxSize == -1 || buffer.totalAllocatedBytes + old->size < static_cast<size_t>(buffer.maxSize));
				canRecycle ? recycleNode(old) : std::free(old);
			}
		}

		return remaining == 0;
	}
	bool advanceReadToEnd() {
		return advanceRead(buffer.availableReadBytes);
	}
	BufferNode* detachHeadNode() {
		BufferNode* head = buffer.headNode;
		if (!head) return nullptr;

		buffer.availableReadBytes -= (head->writePosition - head->readPosition);

		size_t remainingWriteSpace = head->size - (head->writePosition - head->dataPtr);
		buffer.writeCapacity -= remainingWriteSpace;

		buffer.totalAllocatedBytes -= head->size;

		BufferNode* nextNode = head->next;
		buffer.headNode = nextNode;

		if (buffer.tailNode == head) {
			buffer.tailNode = nextNode;
		}
		if (buffer.lastNode == head) {
			buffer.lastNode = nextNode;
		}

		if (!nextNode) {
			buffer.tailNode = nullptr;
			buffer.lastNode = nullptr;
		}

		return head;
	}

	void recycleNode(BufferNode* node) {
		if (!node) return;

		node->readPosition = node->dataPtr;
		node->writePosition = node->dataPtr;
		node->next = nullptr;

		if (buffer.lastNode) {
			buffer.lastNode->next = node;
		}
		buffer.lastNode = node;

		if (buffer.headNode == nullptr) {
			buffer.headNode = node;
		}

		if (buffer.tailNode == nullptr) {
			buffer.tailNode = node;
		}

		buffer.writeCapacity += node->size;
		buffer.totalAllocatedBytes += node->size;
	}
};
struct InlineBuffer : IWriteBuffer {

	BufferView view;
	NodeBufferWriter* parent;
	struct {
		bool field_0;
		int EVP_CIPHER_CTX_ctx;
	} evp;
	int field_1C;
	uint8_t data[8192];
	uint8_t* dataPtr;

	bool getWriteView(BufferView&) override {
		return false;
	}
	bool advanceWrite(size_t) override {
		return false;
	}

	bool sub_92B090() {
		const int v2 = field_1C;
		const int v3 = dataPtr - reinterpret_cast<uint8_t*>(this) - 0x20;
		const unsigned int v4 = v2 - v3 % v2;
		if (static_cast<int>(v4 + v3) >= 4096)
			return advanceWrite(0);
		uint8_t* v5 = &data[v3];
		if (static_cast<int>(v4 - 1) > 0) {
			std::memset(&data[v3], 0, v4 - 1);
			v5 = &data[v3 - 1 + v4];
		}
		*reinterpret_cast<unsigned int*>(v5) = v4;
		return advanceWrite(v4);
	}
};
struct NetworkBufferManager : IReadBuffer {

	unsigned int field_4;
	uint64_t totalBytesWritten;
	uint64_t totalBytesRead;
	bool field_18;
	unsigned int field_1C;
	NodeBuffer writeBuffer;
	NodeBuffer middlemanBuffer;
	NodeBuffer readBuffer;
	Trove::String string;
	InlineBuffer* middleman_inlineBuffer;
	unsigned int field_B8;
	InlineBuffer* p_middleman_inlineBuffer;
	unsigned int field_C0;
	unsigned int field_C4;

	bool getReadView(BufferView& view) override {
		return readBuffer.getReadView(view);
	}
	bool advanceRead(const size_t bytes) override {
		totalBytesRead += bytes;
		return readBuffer.advanceRead(bytes);
	}
	bool copyWriteToRead() {
		CopyBuffer(&writeBuffer, &middlemanBuffer.buffer);
		writeBuffer.advanceReadToEnd();
		p_middleman_inlineBuffer->sub_92B090();
		writeBuffer.buffer.write_int32(static_cast<int>(middlemanBuffer.buffer.availableReadBytes));
		writeBuffer.buffer.write_byte((!field_1C || (field_18 && field_1C == 2)));
		CopyBuffer(&middlemanBuffer, &readBuffer.buffer);
		return middlemanBuffer.advanceReadToEnd();
	}
};
