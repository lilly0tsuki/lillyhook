#pragma once

#include "windows_types.h"

#include <intrin.h>

#include <mutex>
#include <string>
#include <vector>

namespace aobscan {
	struct AOB {
		std::vector<unsigned char> bytes;
		std::string mask;
	};
	struct MemoryRange {
		unsigned char* start;
		unsigned char* end;

		explicit MemoryRange(const char* moduleName) : start(nullptr), end(nullptr) {
			unsigned char* baseAddress = GetModuleHandleA(moduleName);

			const auto* dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(baseAddress);
			if (!dosHeader || dosHeader->e_magic != IMAGE_DOS_SIGNATURE) return;

			const auto ntHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(baseAddress + dosHeader->e_lfanew);
			if (!ntHeaders || ntHeaders->Signature != IMAGE_NT_SIGNATURE) return;

			start = baseAddress;
			end = baseAddress + ntHeaders->OptionalHeader.SizeOfImage;
		}
	};
	static bool cmpPattern(const unsigned char* address, const AOB& pattern) {
		for (unsigned int i = 0; i < pattern.bytes.size(); ++i) {
			if (pattern.mask[i] == 'x' && pattern.bytes[i] != address[i]) {
				return false;
			}
		}
		return true;
	}
	static AOB parseaob(const char* AOBString) {
		AOB pattern;

		const char* p = AOBString;
		while (*p) {
			while (*p == ' ') p++;
			if (*p == 0) break;

			const char* start = p;
			while (*p && *p != ' ') p++;

			if (std::string token(start, p - start);
				token == "?" || token == "??") {
				pattern.bytes.push_back(0);
				pattern.mask += '?';
			} else {
				try {
					auto value = static_cast<unsigned char>(std::stoi(token, nullptr, 16));
					pattern.bytes.push_back(value);
					pattern.mask += 'x';
				} catch (...) {
					return {};
				}
			}
		}

		return pattern;
	}

	static unsigned char* find_avx2(const AOB& pattern, const MemoryRange range) {
		const unsigned int patternLen = pattern.mask.length();
		if (patternLen == 0) return nullptr;

		const unsigned char firstByte = pattern.bytes[0];
		const unsigned char* end = range.end - patternLen;
		const __m256i needle = _mm256_set1_epi8(firstByte);

		for (unsigned char* address = range.start; address < end; address += 32) {
			const __m256i haystack = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(address));
			const __m256i cmp = _mm256_cmpeq_epi8(needle, haystack);
			int mask = _mm256_movemask_epi8(cmp);

			while (mask) {
				const unsigned int index = _tzcnt_u32(mask);

				if (unsigned char* match = address + index;
					match < end && cmpPattern(match, pattern)) {
					return match;
				}
				mask &= mask - 1;
			}
		}

		return nullptr;
	}

	static unsigned char* find_sse2(const AOB& pattern, const MemoryRange range) {
		const unsigned int patternLen = pattern.mask.length();
		if (patternLen == 0) return nullptr;

		const unsigned char firstByte = pattern.bytes[0];
		const unsigned char* end = range.end - patternLen;
		const __m128i needle = _mm_set1_epi8(firstByte);

		for (unsigned char* address = range.start; address < end; address += 16) {
			const __m128i haystack = _mm_loadu_si128(reinterpret_cast<const __m128i*>(address));
			const __m128i cmp = _mm_cmpeq_epi8(needle, haystack);
			int mask = _mm_movemask_epi8(cmp);

			while (mask) {
				const unsigned int index = _tzcnt_u32(mask);

				if (unsigned char* match = address + index;
					match < end && cmpPattern(match, pattern)) {
					return match;
				}
				mask &= mask - 1;
			}
		}

		return nullptr;
	}

	static unsigned char* find_fallback(const AOB& pattern, const MemoryRange range) {
		const unsigned int patternLen = pattern.mask.length();
		if (patternLen == 0) return nullptr;

		const unsigned char* end = range.end - patternLen;

		if (pattern.mask[0] == 'x') {
			const unsigned char firstByte = pattern.bytes[0];
			for (unsigned char* address = range.start; address < end; address++) {
				if (*address == firstByte) {
					if (cmpPattern(address, pattern)) {
						return address;
					}
				}
			}
		} else {
			for (unsigned char* address = range.start; address < end; address++) {
				if (cmpPattern(address, pattern)) {
					return address;
				}
			}
		}

		return nullptr;
	}

	inline unsigned char* (*findFunc)(const AOB&, MemoryRange range) = nullptr;

	static bool startsWithWildcard(const AOB& pattern) {
		return !pattern.mask.empty() && pattern.mask[0] == '?';
	}
}
namespace aobscan {
	inline void initFind() {
		if (findFunc) return;
		int cpuInfo[4] = {};
		__cpuid(cpuInfo, 0);
		const int maxLeaf = cpuInfo[0];
		bool hasAVX2 = false;
		bool hasSSE2 = false;
		if (maxLeaf >= 1) {
			__cpuid(cpuInfo, 1);
			hasSSE2 = (cpuInfo[3] & (1 << 26)) != 0;
			const bool hasAVX = (cpuInfo[2] & (1 << 28)) != 0;
			if (maxLeaf >= 7) {
				__cpuidex(cpuInfo, 7, 0);
				hasAVX2 = hasAVX && (cpuInfo[1] & (1 << 5)) != 0;
			}
		}
		if (hasAVX2)
			findFunc = find_avx2;
		else if (hasSSE2)
			findFunc = find_sse2;
		else
			findFunc = find_fallback;
	}

	inline unsigned char* find(const AOB& pattern, const MemoryRange range) {
		return startsWithWildcard(pattern) ?
			find_fallback(pattern, range) :
			findFunc(pattern, range);
	}

	inline unsigned char* find(const char* AOBString, const char* moduleName = nullptr) {
		const MemoryRange range(moduleName);
		return find(parseaob(AOBString), range);
	}
};

template <typename T>
struct ExtFunc {
	mutable T m_ptr = nullptr;
	const char* m_aob = nullptr;
	mutable std::once_flag m_onceFlag;

	void init() const {
		if (m_aob) m_ptr = reinterpret_cast<T>(aobscan::find(m_aob));
	}

	explicit ExtFunc(const char* aob) : m_aob(aob) {}
	explicit ExtFunc(T ptr) : m_ptr(ptr) {}
	explicit ExtFunc(void* ptr) : m_ptr(reinterpret_cast<T>(ptr)) {}
	explicit ExtFunc(size_t offset) {
		size_t module = (size_t)GetModuleHandleA(0);
		if (module) {
			size_t addr = module + offset;
			m_ptr = reinterpret_cast<T>(addr);
		}
	}

	template <typename... Args>
	auto operator()(Args&&... args) const -> std::invoke_result_t<T, Args...> {
		using ReturnType = std::invoke_result_t<T, Args...>;
		std::call_once(m_onceFlag, [this] { init(); });

		if (!m_ptr) {
			if constexpr (!std::is_void_v<ReturnType>) {
				return ReturnType{};
			} else {
				return;
			}
		}
		return m_ptr(std::forward<Args>(args)...);
	}

	bool exists() const {
		std::call_once(m_onceFlag, [this] { init(); });
		return m_ptr != nullptr;
	}

	explicit operator bool() const = delete;
};
template <typename T>
struct ExtVar {
	mutable T m_value = nullptr;
	const char* m_aob = nullptr;
	mutable std::once_flag m_onceFlag;

	void init() const {
		if (m_aob) {
			void* addr = aobscan::find(m_aob);
			if (addr) m_value = reinterpret_cast<T>(addr);
		}
	}

	static T fallback() {
		return nullptr;
	}


	explicit ExtVar(const char* aob) : m_aob(aob) {}
	explicit ExtVar(T value) : m_value(value) {}
	explicit ExtVar(void* ptr) : m_value(reinterpret_cast<T>(ptr)) {}
	explicit ExtVar(unsigned int addr) : m_value(reinterpret_cast<T>(addr)) {}

	T get() const {
		std::call_once(m_onceFlag, [this] { init(); });
		return m_value ? m_value : fallback();
	}

	void set(T value) const {
		std::call_once(m_onceFlag, [this] { init(); });
		m_value = value;
	}

	bool exists() const {
		std::call_once(m_onceFlag, [this] { init(); });
		return m_value != nullptr;
	}

	operator T() const {
		return get();
	}

	T operator->() const {
		return get();
	}

	auto operator*() const -> decltype(*std::declval<T>()) {
		return *get();
	}

	ExtVar& operator=(T value) {
		set(value);
		return *this;
	}

	explicit operator bool() const = delete;
};

