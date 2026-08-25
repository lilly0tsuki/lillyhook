#pragma once

#include <algorithm>
#include <cassert>
#include <compare>
#include <cstdlib>
#include <cstring>
#include <cwchar>
#include <cctype>
#include <cwctype>
#include <format>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <locale>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

inline const std::locale& formatNumbers() {
	static const std::locale loc = [] {
		try {
			return std::locale("en_US.UTF-8");
		} catch (const std::exception&) {
			return std::locale::classic();
		}
	}();
	return loc;
}

namespace Trove {
	template <
	typename CharT = char,
	typename Traits = std::char_traits<CharT>,
	typename Allocator = std::allocator<CharT>
	>
	struct string {
		using traits_type = Traits;
		using allocator_type = Allocator;
		using value_type = CharT;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		using view_type = std::basic_string_view<CharT, Traits>;
		using std_string_type = std::basic_string<CharT, Traits, Allocator>;
		using allocator_traits = std::allocator_traits<Allocator>;

		using iterator = CharT*;
		using const_iterator = const CharT*;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		static constexpr size_type npos = static_cast<size_type>(-1);

		size_type m_size{ 0 };
		size_type m_capacity{ 0 };
		value_type* m_data{ nullptr };
		Allocator m_alloc{};

		inline static value_type m_safe_null{};

		static constexpr const CharT* default_delimiters() noexcept {
			if constexpr (std::is_same_v<CharT, wchar_t>) {
				return L" ";
			} else {
				return " ";
			}
		}

		static bool is_space(const CharT c) noexcept {
			if constexpr (std::is_same_v<CharT, char>) {
				return std::isspace(static_cast<unsigned char>(c)) != 0;
			} else if constexpr (std::is_same_v<CharT, wchar_t>) {
				return std::iswspace(c) != 0;
			} else {
				return c == CharT(' ') || c == CharT('\t') || c == CharT('\n') || c == CharT('\r') || c == CharT('\f') || c == CharT('\v');
			}
		}

		static CharT lower_char(CharT c) noexcept {
			if constexpr (std::is_same_v<CharT, char>) {
				return static_cast<CharT>(std::tolower(static_cast<unsigned char>(c)));
			} else if constexpr (std::is_same_v<CharT, wchar_t>) {
				return static_cast<CharT>(std::towlower(c));
			} else {
				return c;
			}
		}

		static CharT upper_char(CharT c) noexcept {
			if constexpr (std::is_same_v<CharT, char>) {
				return static_cast<CharT>(std::toupper(static_cast<unsigned char>(c)));
			} else if constexpr (std::is_same_v<CharT, wchar_t>) {
				return static_cast<CharT>(std::towupper(c));
			} else {
				return c;
			}
		}

		[[nodiscard]] const_pointer safe_c_str() const noexcept {
			return m_data ? m_data : &m_safe_null;
		}

		[[nodiscard]] bool aliases_internal_buffer(const CharT* str) const noexcept {
			return m_data != nullptr && str >= m_data && str < m_data + m_size;
		}

		[[nodiscard]] bool would_overflow(const size_type n) const noexcept {
			return n > max_size() - m_size;
		}

		pointer allocate_buffer(size_type count) noexcept {
			try {
				return allocator_traits::allocate(m_alloc, count);
			} catch (...) {
				return nullptr;
			}
		}

		void release() noexcept {
			if (m_data) {
				try {
					allocator_traits::deallocate(m_alloc, m_data, m_capacity);
				} catch (...) {
				}
				m_data = nullptr;
			}
			m_size = 0;
			m_capacity = 0;
		}

		void reallocate(const size_type newCapacity) noexcept {
			if (newCapacity == m_capacity) return;

			if (newCapacity == 0) {
				release();
				return;
			}

			const auto newData = allocate_buffer(newCapacity);
			if (!newData) return;

			const size_type copyLen = (m_data && newCapacity > 0) ? std::min(m_size, newCapacity - 1) : 0;
			if (m_data && copyLen > 0) {
				Traits::copy(newData, m_data, copyLen);
			}
			newData[copyLen] = CharT{};

			release();
			m_data = newData;
			m_size = copyLen;
			m_capacity = newCapacity;
		}

		string() noexcept = default;

		string(const CharT* str) : string(str, str ? Traits::length(str) : 0) {}

		string(const CharT* str, const size_type n) {
			assign(str, n);
		}

		explicit string(const size_type count, const CharT c) {
			assign(count, c);
		}

		string(const string& other)
		: m_alloc(allocator_traits::select_on_container_copy_construction(other.m_alloc)) {
			assign(other.m_data, other.m_size);
		}

		string(string&& other) noexcept
		: m_size(other.m_size), m_capacity(other.m_capacity), m_data(other.m_data), m_alloc(std::move(other.m_alloc)) {
			other.m_size = 0;
			other.m_capacity = 0;
			other.m_data = nullptr;
		}

		string(const std_string_type& str) : string(str.c_str(), str.length()) {}

		string(const std::initializer_list<CharT> ilist) : string(ilist.begin(), ilist.size()) {}

		template<typename InputIt>
		string(InputIt first, InputIt last) {
			if constexpr (std::random_access_iterator<InputIt>) {
				reserve(static_cast<size_type>(std::distance(first, last)));
			}
			while (first != last) {
				push_back(*first);
				++first;
			}
		}

		~string() {
			release();
		}

		string& operator=(const string& other) {
			if (this != &other) assign(other.m_data, other.m_size);
			return *this;
		}

		string& operator=(string&& other) noexcept {
			if (this != &other) {
				release();
				m_size = other.m_size;
				m_capacity = other.m_capacity;
				m_data = other.m_data;
				m_alloc = std::move(other.m_alloc);
				other.m_size = 0;
				other.m_capacity = 0;
				other.m_data = nullptr;
			}
			return *this;
		}

		string& operator=(const CharT* str) {
			return assign(str);
		}

		string& operator=(const CharT c) {
			return assign(1, c);
		}

		string& assign(const size_type count, const CharT c) noexcept {
			if (count == 0) {
				clear();
				return *this;
			}
			if (count > max_size()) return *this;

			if (count + 1 <= m_capacity && m_data) {
				Traits::assign(m_data, count, c);
				m_size = count;
				m_data[m_size] = CharT{};
				return *this;
			}

			const auto newData = allocate_buffer(count + 1);
			if (!newData) return *this;

			release();
			m_data = newData;
			m_capacity = count + 1;
			m_size = count;
			Traits::assign(m_data, m_size, c);
			m_data[m_size] = CharT{};
			return *this;
		}

		string& assign(const CharT* str, const size_type n) noexcept {
			if (!str && n > 0) return *this;
			if (n == 0) {
				clear();
				return *this;
			}
			if (n > max_size()) return *this;

			if (str >= m_data && str < m_data + m_size) {
				return assign(string(str, n));
			}

			if (n + 1 <= m_capacity && m_data) {
				Traits::copy(m_data, str, n);
				m_size = n;
				m_data[m_size] = CharT{};
				return *this;
			}

			const auto newData = allocate_buffer(n + 1);
			if (!newData) return *this;

			release();
			m_data = newData;
			m_capacity = n + 1;
			m_size = n;
			Traits::copy(m_data, str, m_size);
			m_data[m_size] = CharT{};
			return *this;
		}

		string& assign(const CharT* str) {
			return assign(str, str ? Traits::length(str) : 0);
		}

		string& assign(const string& str) {
			return assign(str.m_data, str.m_size);
		}

		[[nodiscard]] iterator begin() noexcept { return m_data; }
		[[nodiscard]] const_iterator begin() const noexcept { return m_data; }
		[[nodiscard]] const_iterator cbegin() const noexcept { return m_data; }

		[[nodiscard]] iterator end() noexcept { return m_data ? m_data + m_size : nullptr; }
		[[nodiscard]] const_iterator end() const noexcept { return m_data ? m_data + m_size : nullptr; }
		[[nodiscard]] const_iterator cend() const noexcept { return m_data ? m_data + m_size : nullptr; }

		[[nodiscard]] reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
		[[nodiscard]] const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
		[[nodiscard]] const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }

		[[nodiscard]] reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
		[[nodiscard]] const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
		[[nodiscard]] const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }

		[[nodiscard]] bool empty() const noexcept { return m_size == 0; }

		[[nodiscard]] size_type size() const noexcept { return m_size; }
		[[nodiscard]] size_type length() const noexcept { return m_size; }

		[[nodiscard]] static size_type max_size() noexcept { return std::numeric_limits<size_type>::max() - 1; }

		[[nodiscard]] size_type capacity() const noexcept { return m_capacity > 0 ? m_capacity - 1 : 0; }

		void reserve(const size_type newCapacity) noexcept {
			if (newCapacity > max_size()) return;
			if (newCapacity + 1 > m_capacity) {
				reallocate(newCapacity + 1);
			}
		}

		void shrink_to_fit() noexcept {
			if (m_capacity > m_size + 1) {
				reallocate(m_size + 1);
			}
		}

		void clear() noexcept {
			m_size = 0;
			if (m_data) m_data[0] = CharT{};
		}

		reference operator[](const size_type pos) noexcept { return m_data[pos]; }
		const_reference operator[](const size_type pos) const noexcept { return m_data[pos]; }

		reference at(const size_type pos) {
			if (pos >= m_size || !m_data) throw std::out_of_range("Trove::string::at: pos out of range");
			return m_data[pos];
		}
		const_reference at(const size_type pos) const {
			if (pos >= m_size || !m_data) throw std::out_of_range("Trove::string::at: pos out of range");
			return m_data[pos];
		}

		reference front() noexcept {
			assert(!empty() && "Trove::string::front() called on an empty string");
			return m_data[0];
		}
		const_reference front() const noexcept { return !empty() ? m_data[0] : m_safe_null; }

		reference back() noexcept {
			assert(!empty() && "Trove::string::back() called on an empty string");
			return m_data[m_size - 1];
		}
		const_reference back() const noexcept { return !empty() ? m_data[m_size - 1] : m_safe_null; }

		[[nodiscard]] const_pointer data() const noexcept { return m_data; }
		[[nodiscard]] pointer data() noexcept { return m_data; }

		[[nodiscard]] const_pointer c_str() const noexcept { return safe_c_str(); }

		explicit operator view_type() const noexcept {
			return { safe_c_str(), size() };
		}

		string& append(const CharT* str, const size_type n) noexcept {
			if (str && n > 0) {
				if (aliases_internal_buffer(str)) {
					const string temp(str, n);
					return append(temp.m_data, temp.m_size);
				}
				if (would_overflow(n)) return *this;

				if (const size_type needed = m_size + n + 1; needed > m_capacity) {
					size_type nextCap = m_capacity == 0 ? needed : m_capacity * 2;
					if (nextCap < needed) nextCap = needed;

					const auto newData = allocate_buffer(nextCap);
					if (!newData) return *this;

					const size_type oldSize = m_size;
					if (m_data) {
						Traits::copy(newData, m_data, oldSize);
						release();
					}
					m_data = newData;
					m_capacity = nextCap;
					m_size = oldSize;
				}

				Traits::copy(m_data + m_size, str, n);
				m_size += n;
				m_data[m_size] = CharT{};
			}
			return *this;
		}

		string& append(const CharT* str) {
			return append(str, str ? Traits::length(str) : 0);
		}

		string& append(const string& str) {
			return append(str.m_data, str.m_size);
		}

		string& append(const size_type count, const CharT c) noexcept {
			const string temp(count, c);
			return append(temp);
		}

		void push_back(const CharT c) {
			append(&c, 1);
		}

		void pop_back() {
			if (m_size > 0) erase(m_size - 1, 1);
		}

		string& operator+=(const string& str) { return append(str); }
		string& operator+=(const CharT* str) { return append(str); }
		string& operator+=(const CharT c) { return append(&c, 1); }

		string& insert(size_type pos, const CharT* str, const size_type n) noexcept {
			if (pos > m_size) pos = m_size;
			if (!str || n == 0) return *this;

			if (aliases_internal_buffer(str)) {
				const string temp(str, n);
				return insert(pos, temp.m_data, temp.m_size);
			}
			if (would_overflow(n)) return *this;

			if (const size_type needed = m_size + n + 1; needed > m_capacity) {
				size_type nextCap = m_capacity == 0 ? needed : m_capacity * 2;
				if (nextCap < needed) nextCap = needed;

				const auto newData = allocate_buffer(nextCap);
				if (!newData) return *this;

				const size_type oldSize = m_size;
				if (pos > 0) Traits::copy(newData, m_data, pos);
				Traits::copy(newData + pos, str, n);
				if (pos < oldSize) Traits::copy(newData + pos + n, m_data + pos, oldSize - pos);

				release();
				m_data = newData;
				m_capacity = nextCap;
				m_size = oldSize;
			} else {
				Traits::move(m_data + pos + n, m_data + pos, m_size - pos);
				Traits::copy(m_data + pos, str, n);
			}
			m_size += n;
			m_data[m_size] = CharT{};
			return *this;
		}

		string& insert(const size_type pos, const string& str) {
			return insert(pos, str.m_data, str.m_size);
		}

		string& insert(const size_type pos, const CharT* str) {
			return insert(pos, str, str ? Traits::length(str) : 0);
		}

		string& erase(const size_type pos = 0, size_type len = npos) noexcept {
			if (pos >= m_size) return *this;
			if (len == npos || pos + len > m_size) len = m_size - pos;
			if (len == 0) return *this;

			Traits::move(m_data + pos, m_data + pos + len, m_size - pos - len + 1);
			m_size -= len;
			return *this;
		}

		iterator erase(const const_iterator p) {
			const size_type pos = static_cast<size_type>(p - begin());
			erase(pos, 1);
			return begin() + pos;
		}

		iterator erase(const const_iterator first, const const_iterator last) {
			const size_type pos = static_cast<size_type>(first - begin());
			const size_type len = static_cast<size_type>(last - first);
			erase(pos, len);
			return begin() + pos;
		}

		string& replace(const size_type pos, size_type len, const CharT* str, const size_type n) noexcept {
			if (pos > m_size) return *this;
			if (len == npos || pos + len > m_size) len = m_size - pos;

			if (aliases_internal_buffer(str)) {
				const string temp(str, n);
				erase(pos, len);
				insert(pos, temp.m_data, temp.m_size);
				return *this;
			}

			erase(pos, len);
			insert(pos, str, n);
			return *this;
		}

		string& replace(const size_type pos, const size_type len, const string& str) {
			return replace(pos, len, str.c_str(), str.size());
		}

		string& replace(const size_type pos, const size_type len, const CharT* str) {
			return replace(pos, len, str, str ? Traits::length(str) : 0);
		}

		void resize(const size_type count, const CharT c = CharT{}) noexcept {
			if (count == m_size) return;
			if (count < m_size) {
				m_size = count;
				if (m_data) m_data[m_size] = CharT{};
			} else {
				reserve(count);
				if (m_capacity > count) {
					Traits::assign(m_data + m_size, count - m_size, c);
					m_size = count;
					m_data[m_size] = CharT{};
				}
			}
		}

		void swap(string& other) noexcept {
			std::swap(m_size, other.m_size);
			std::swap(m_capacity, other.m_capacity);
			std::swap(m_data, other.m_data);
			std::swap(m_alloc, other.m_alloc);
		}

		size_type copy(CharT* dest, const size_type count, const size_type pos = 0) const noexcept {
			if (pos >= m_size) return 0;
			const size_type copyLen = std::min(count, m_size - pos);
			Traits::copy(dest, m_data + pos, copyLen);
			return copyLen;
		}

		[[nodiscard]] string substr(const size_type pos = 0, size_type len = npos) const {
			if (pos >= m_size) return {};
			len = std::min(len, m_size - pos);
			return { m_data + pos, len };
		}

		[[nodiscard]] int compare(const string& str) const noexcept {
			return compare(0, npos, str.c_str(), str.size());
		}
		[[nodiscard]] int compare(const size_type pos1, const size_type count1, const string& str) const noexcept {
			return compare(pos1, count1, str.c_str(), str.size());
		}
		[[nodiscard]] int compare(const CharT* s) const noexcept {
			return compare(0, npos, s, s ? Traits::length(s) : 0);
		}
		[[nodiscard]] int compare(size_type pos1, size_type count1, const CharT* s, size_type count2 = npos) const noexcept {
			if (pos1 >= m_size) pos1 = m_size;
			if (count1 == npos || pos1 + count1 > m_size) count1 = m_size - pos1;

			if (!s) {
				count2 = 0;
			} else if (count2 == npos) {
				count2 = Traits::length(s);
			}

			const size_type rlen = std::min(count1, count2);
			const auto* lhs = m_data ? m_data : &m_safe_null;
			if (rlen > 0) {
				if (const int res = Traits::compare(lhs + pos1, s, rlen); res != 0) return res;
			}

			if (count1 < count2) return -1;
			if (count1 > count2) return 1;
			return 0;
		}

		auto operator<=>(const string& other) const noexcept {
			const int cmp = compare(other);
			if (cmp < 0) return std::strong_ordering::less;
			if (cmp > 0) return std::strong_ordering::greater;
			return std::strong_ordering::equal;
		}
		bool operator==(const string& other) const noexcept { return compare(other) == 0; }
		bool operator==(const CharT* other) const noexcept { return compare(other) == 0; }

		[[nodiscard]] size_type find(const CharT* str, size_type pos = 0, size_type n = npos) const noexcept {
			if (!str || pos >= m_size) return npos;
			if (n == npos) n = Traits::length(str);
			if (n == 0) return pos;

			for (; pos + n <= m_size; ++pos) {
				if (Traits::compare(m_data + pos, str, n) == 0) return pos;
			}
			return npos;
		}
		[[nodiscard]] size_type find(const string& str, const size_type pos = 0) const noexcept {
			return find(str.c_str(), pos, str.size());
		}
		[[nodiscard]] size_type find(const CharT c, const size_type pos = 0) const noexcept {
			if (pos >= m_size) return npos;
			const auto* res = Traits::find(m_data + pos, m_size - pos, c);
			return res ? static_cast<size_type>(res - m_data) : npos;
		}

		[[nodiscard]] size_type rfind(const CharT* str, size_type pos = npos, size_type n = npos) const noexcept {
			if (!str) return npos;
			if (n == npos) n = Traits::length(str);
			if (n > m_size) return npos;

			if (pos == npos || pos > m_size - n) pos = m_size - n;

			for (difference_type i = static_cast<difference_type>(pos); i >= 0; --i) {
				if (Traits::compare(m_data + i, str, n) == 0) return static_cast<size_type>(i);
			}
			return npos;
		}
		[[nodiscard]] size_type rfind(const string& str, const size_type pos = npos) const noexcept {
			return rfind(str.c_str(), pos, str.size());
		}
		[[nodiscard]] size_type rfind(const CharT c, size_type pos = npos) const noexcept {
			if (m_size == 0) return npos;
			if (pos == npos || pos >= m_size) pos = m_size - 1;
			for (difference_type i = static_cast<difference_type>(pos); i >= 0; --i) {
				if (m_data[i] == c) return static_cast<size_type>(i);
			}
			return npos;
		}

		[[nodiscard]] size_type find_first_of(const CharT* s, size_type pos = 0) const noexcept {
			if (pos >= m_size || !s) return npos;
			for (; pos < m_size; ++pos) {
				for (const CharT* p = s; *p; ++p) {
					if (m_data[pos] == *p) return pos;
				}
			}
			return npos;
		}

		[[nodiscard]] size_type find_first_not_of(const CharT* s, size_type pos = 0) const noexcept {
			if (pos >= m_size || !s) return npos;
			for (; pos < m_size; ++pos) {
				bool found = false;
				for (const CharT* p = s; *p; ++p) {
					if (m_data[pos] == *p) {
						found = true;
						break;
					}
				}
				if (!found) return pos;
			}
			return npos;
		}

		[[nodiscard]] size_type find_last_of(const CharT* s, size_type pos = npos) const noexcept {
			if (empty() || !s) return npos;
			if (pos >= m_size) pos = m_size - 1;
			for (difference_type i = static_cast<difference_type>(pos); i >= 0; --i) {
				for (const CharT* p = s; *p; ++p) {
					if (m_data[i] == *p) return static_cast<size_type>(i);
				}
			}
			return npos;
		}

		[[nodiscard]] size_type find_last_not_of(const CharT* s, size_type pos = npos) const noexcept {
			if (empty() || !s) return npos;
			if (pos >= m_size) pos = m_size - 1;
			for (difference_type i = static_cast<difference_type>(pos); i >= 0; --i) {
				bool found = false;
				for (const CharT* p = s; *p; ++p) {
					if (m_data[i] == *p) {
						found = true;
						break;
					}
				}
				if (!found) return static_cast<size_type>(i);
			}
			return npos;
		}

		[[nodiscard]] bool starts_with(const CharT* prefix) const noexcept {
			const size_type len = prefix ? Traits::length(prefix) : 0;
			return len <= m_size && Traits::compare(m_data ? m_data : &m_safe_null, prefix, len) == 0;
		}

		[[nodiscard]] bool ends_with(const CharT* suffix) const noexcept {
			const size_type len = suffix ? Traits::length(suffix) : 0;
			return len <= m_size && Traits::compare((m_data ? m_data : &m_safe_null) + m_size - len, suffix, len) == 0;
		}

		[[nodiscard]] bool contains(const CharT* s) const noexcept {
			return find(s) != npos;
		}

		string& trim() {
			return trim_start().trim_end();
		}

		string& trim_start() {
			if (empty()) return *this;
			size_type start = 0;
			while (start < m_size && is_space(m_data[start])) ++start;
			if (start > 0) {
				Traits::move(m_data, m_data + start, m_size - start + 1);
				m_size -= start;
			}
			return *this;
		}

		string& trim_end() {
			if (empty()) return *this;
			while (m_size > 0 && is_space(m_data[m_size - 1])) --m_size;
			m_data[m_size] = CharT{};
			return *this;
		}

		[[nodiscard]] string trimmed() const { return string(*this).trim(); }
		[[nodiscard]] string trimmed_start() const { return string(*this).trim_start(); }
		[[nodiscard]] string trimmed_end() const { return string(*this).trim_end(); }

		string& to_lower() {
			for (size_type i = 0; i < m_size; ++i) m_data[i] = lower_char(m_data[i]);
			return *this;
		}

		string& to_upper() {
			for (size_type i = 0; i < m_size; ++i) m_data[i] = upper_char(m_data[i]);
			return *this;
		}

		[[nodiscard]] string lower() const { return string(*this).to_lower(); }
		[[nodiscard]] string upper() const { return string(*this).to_upper(); }

		[[nodiscard]] std::vector<string> split(const CharT* delimiters) const {
			std::vector<string> result;
			if (empty()) return result;
			const CharT* delims = delimiters ? delimiters : default_delimiters();

			size_type start = 0;
			size_type end = find_first_of(delims);

			while (end != npos) {
				if (end > start) result.push_back(substr(start, end - start));
				start = end + 1;
				end = find_first_of(delims, start);
			}
			if (start < m_size) result.push_back(substr(start));

			return result;
		}

		[[nodiscard]] std::vector<string> split() const {
			return split(default_delimiters());
		}

		template<typename T>
		[[nodiscard]] T to_integer() const noexcept {
			static_assert(std::is_integral_v<T>, "T must be integral");
			if (empty()) return T{};

			if constexpr (std::is_same_v<CharT, char>) {
				char* end = nullptr;
				long long res = std::strtoll(c_str(), &end, 0);
				if (end == c_str()) return T{};
				return static_cast<T>(res);
			} else if constexpr (std::is_same_v<CharT, wchar_t>) {
				wchar_t* end = nullptr;
				long long res = std::wcstoll(c_str(), &end, 0);
				if (end == c_str()) return T{};
				return static_cast<T>(res);
			} else {
				return T{};
			}
		}

		template<typename T>
		[[nodiscard]] T to_floating_point() const noexcept {
			static_assert(std::is_floating_point_v<T>, "T must be floating point");
			if (empty()) return T{};

			if constexpr (std::is_same_v<CharT, char>) {
				char* end = nullptr;
				long double res = std::strtold(c_str(), &end);
				if (end == c_str()) return T{};
				return static_cast<T>(res);
			} else if constexpr (std::is_same_v<CharT, wchar_t>) {
				wchar_t* end = nullptr;
				long double res = std::wcstold(c_str(), &end);
				if (end == c_str()) return T{};
				return static_cast<T>(res);
			} else {
				return T{};
			}
		}

		template <typename T>
		[[nodiscard]] T to_number() const noexcept {
			if constexpr (std::is_integral_v<T>) return to_integer<T>();
			else if constexpr (std::is_floating_point_v<T>) return to_floating_point<T>();
			else return T{};
		}

		friend string operator+(const string& lhs, const string& rhs) {
			string res;
			res.reserve(lhs.size() + rhs.size());
			res.append(lhs);
			res.append(rhs);
			return res;
		}

		friend string operator+(const CharT* lhs, const string& rhs) {
			string res(lhs);
			res.append(rhs);
			return res;
		}

		friend string operator+(const string& lhs, const CharT* rhs) {
			string res(lhs);
			res.append(rhs);
			return res;
		}

		explicit operator bool() const noexcept { return !empty(); }

		operator std_string_type() const { return { c_str(), size() }; }

		operator short() const noexcept { return to_number<short>(); }
		operator int() const noexcept { return to_number<int>(); }
		operator long() const noexcept { return to_number<long>(); }
		operator long long() const noexcept { return to_number<long long>(); }
		operator unsigned short() const noexcept { return to_number<unsigned short>(); }
		operator unsigned int() const noexcept { return to_number<unsigned int>(); }
		operator unsigned long() const noexcept { return to_number<unsigned long>(); }
		operator unsigned long long() const noexcept { return to_number<unsigned long long>(); }
		operator float() const noexcept { return to_number<float>(); }
		operator double() const noexcept { return to_number<double>(); }
		operator long double() const noexcept { return to_number<long double>(); }
	};

	using String = string<char>;
	using WString = string<wchar_t>;
}

template<class T>
struct std::formatter<std::vector<T>> : std::formatter<std::string> {
	template<class FormatContext>
	auto format(const std::vector<T>& v, FormatContext& ctx) const {
		std::string out = "[";
		for (size_t i = 0; i < v.size(); i++) {
			out += std::format("{}", v[i]);
			if (i + 1 < v.size()) out += ", ";
		}
		out += "]";
		return std::formatter<std::string>::format(out, ctx);
	}
};

namespace std {
	template<typename CharT, typename Traits, typename Allocator>
	struct formatter<Trove::string<CharT, Traits, Allocator>, CharT> : formatter<basic_string_view<CharT, Traits>, CharT> {
		auto format(const Trove::string<CharT, Traits, Allocator>& value, auto& ctx) const {
			return formatter<basic_string_view<CharT, Traits>, CharT>::format(basic_string_view<CharT, Traits>(value.c_str(), value.size()), ctx);
		}
	};

	template<typename CharT, typename Traits, typename Allocator>
	struct hash<Trove::string<CharT, Traits, Allocator>> {
		size_t operator()(const Trove::string<CharT, Traits, Allocator>& s) const noexcept {
			size_t hash = 0x811C9DC5u;
			constexpr size_t FNV_PRIME = 0x1000193u;
			for (size_t i = 0; i < s.size(); ++i) {
				hash ^= static_cast<size_t>(std::hash<CharT>{}(s[i]));
				hash *= FNV_PRIME;
			}
			return hash;
		}
	};
}
