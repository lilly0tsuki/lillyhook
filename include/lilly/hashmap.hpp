#pragma once

#include "lilly/encoding.hpp"

#include <stdexcept>
#include <iterator>
#include <memory>
#include <utility>

namespace Trove {
	template <
		typename Key,
		typename V,
		typename Hash = Trove::hash<Key>,
		typename KeyEqual = std::equal_to<Key>,
		typename Allocator = std::allocator<std::pair<const Key, V>>
	>
	struct hashmap {
		using key_type = Key;
		using mapped_type = V;
		using value_type = std::pair<const Key, V>;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using hasher = Hash;
		using key_equal = KeyEqual;
		using allocator_type = Allocator;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = std::allocator_traits<Allocator>::pointer;
		using const_pointer = std::allocator_traits<Allocator>::const_pointer;

		template <typename K, typename V_> struct Node;
		using Node_t = Node<Key, V>;

		template <typename K, typename V_>
		struct Node {
			size_type field;
			std::pair<const K, V_> data;

			[[nodiscard]] bool is_free() const noexcept {
				return field & 1u;
			}

			[[nodiscard]] Node<K, V_>* next() const noexcept {
				auto* ptr = reinterpret_cast<Node<K, V_>*>(field & ~1u);
				return (ptr && !ptr->is_free()) ? ptr : nullptr;
			}
		};

		struct iterator {
			using iterator_category = std::forward_iterator_tag;
			using value_type = std::pair<const Key, V>;
			using difference_type = std::ptrdiff_t;
			using pointer = std::pair<const Key, V>*;
			using reference = std::pair<const Key, V>&;

			iterator() noexcept : m_map(nullptr), m_current_node(nullptr), m_bucket_index(0) {}

			explicit iterator(hashmap* map, Node_t* node, size_type index) noexcept
				: m_map(map), m_current_node(node), m_bucket_index(index) {}

			[[nodiscard]] bool operator==(const iterator& other) const noexcept {
				return m_current_node == other.m_current_node;
			}

			iterator& operator++() noexcept {
				if (!m_current_node) return *this;
				Node_t* next_node = m_current_node->next();
				if (next_node) {
					m_current_node = next_node;
					return *this;
				}
				for (size_type i = m_bucket_index + 1; i < m_map->bucket_count(); ++i) {
					Node_t& node = m_map->nodeAt(i);
					if (!node.is_free()) {
						m_bucket_index = i;
						m_current_node = &node;
						return *this;
					}
				}
				m_bucket_index = m_map->bucket_count();
				m_current_node = nullptr;
				return *this;
			}

			iterator operator++(int) noexcept {
				iterator tmp = *this;
				++(*this);
				return tmp;
			}

			reference operator*() const {
				return m_current_node->data;
			}
			pointer operator->() const {
				return &m_current_node->data;
			}

		
			hashmap* m_map;
			Node_t* m_current_node;
			size_type m_bucket_index;

			friend struct const_iterator;
		};
		struct const_iterator {
		
			using iterator_category = std::forward_iterator_tag;
			using value_type = const std::pair<const Key, V>;
			using difference_type = std::ptrdiff_t;
			using pointer = const std::pair<const Key, V>*;
			using reference = const std::pair<const Key, V>&;

			const_iterator() noexcept : m_map(nullptr), m_current_node(nullptr), m_bucket_index(0) {}

			explicit const_iterator(const hashmap* map, const Node_t* node, size_type index) noexcept
				: m_map(map), m_current_node(node), m_bucket_index(index) {}

			const_iterator(const iterator& it) noexcept
				: m_map(it.m_map), m_current_node(it.m_current_node), m_bucket_index(it.m_bucket_index) {}

			[[nodiscard]] bool operator==(const const_iterator& other) const noexcept {
				return m_current_node == other.m_current_node;
			}

			const_iterator& operator++() noexcept {
				if (!m_current_node) return *this;
				const Node_t* next_node = m_current_node->next();
				if (next_node) {
					m_current_node = next_node;
					return *this;
				}
				for (size_type i = m_bucket_index + 1; i < m_map->bucket_count(); ++i) {
					const Node_t& node = m_map->nodeAt(i);
					if (!node.is_free()) {
						m_bucket_index = i;
						m_current_node = &node;
						return *this;
					}
				}
				m_bucket_index = m_map->bucket_count();
				m_current_node = nullptr;
				return *this;
			}

			const_iterator operator++(int) noexcept {
				const_iterator tmp = *this;
				++(*this);
				return tmp;
			}

			reference operator*() const {
				return m_current_node->data;
			}
			pointer operator->() const {
				return &m_current_node->data;
			}

		
			const hashmap* m_map;
			const Node_t* m_current_node;
			size_type m_bucket_index;
		};

		[[nodiscard]] iterator begin() noexcept {
			for (size_type i = 0; i < bucket_count(); ++i) {
				if (!nodeAt(i).is_free()) {
					return iterator(this, &nodeAt(i), i);
				}
			}
			return end();
		}

		[[nodiscard]] const_iterator begin() const noexcept {
			for (size_type i = 0; i < bucket_count(); ++i) {
				if (!nodeAt(i).is_free()) {
					return const_iterator(this, &nodeAt(i), i);
				}
			}
			return end();
		}

		[[nodiscard]] iterator end() noexcept {
			return iterator(this, nullptr, bucket_count());
		}

		[[nodiscard]] const_iterator end() const noexcept {
			return const_iterator(this, nullptr, bucket_count());
		}

		[[nodiscard]] const_iterator cbegin() const noexcept {
			return begin();
		}
		[[nodiscard]] const_iterator cend() const noexcept {
			return end();
		}

		[[nodiscard]] bool empty() const noexcept {
			return size() == 0;
		}
		[[nodiscard]] size_type size() const noexcept {
			return m_element_count;
		}
		[[nodiscard]] size_type max_size() const noexcept {
			return std::allocator_traits<allocator_type>::max_size(get_allocator());
		}

		[[nodiscard]] const mapped_type& at(const key_type& key) const {
			const Node_t* node = find_node(key);
			if (!node) {
				throw std::out_of_range("invalid hashmap<K, T> key");
			}
			return node->data.second;
		}

		[[nodiscard]] mapped_type& at(const key_type& key) {
			auto* node = const_cast<Node_t*>(find_node(key));
			if (!node) {
				throw std::out_of_range("invalid hashmap<K, T> key");
			}
			return node->data.second;
		}

		[[nodiscard]] iterator find(const key_type& key) noexcept {
			if (empty()) return end();

			size_type index = hash_function()(key) % bucket_count();
			Node_t* current_node = &nodeAt(index);

			while (current_node) {
				if (!current_node->is_free() && key_eq()(current_node->data.first, key)) {
					return iterator(this, current_node, index);
				}
				current_node = current_node->next();
			}
			return end();
		}

		[[nodiscard]] const_iterator find(const key_type& key) const noexcept {
			if (empty()) return end();

			size_type index = hash_function()(key) % bucket_count();
			const Node_t* current_node = &nodeAt(index);

			while (current_node) {
				if (!current_node->is_free() && key_eq()(current_node->data.first, key)) {
					return const_iterator(this, current_node, index);
				}
				current_node = current_node->next();
			}

			return end();
		}

		[[nodiscard]] size_type count(const key_type& key) const noexcept {
			return find_node(key) ? 1 : 0;
		}

		[[nodiscard]] bool contains(const key_type& key) const noexcept {
			return find_node(key) != nullptr;
		}

		[[nodiscard]] std::pair<iterator, iterator> equal_range(const key_type& key) noexcept {
			iterator it = find(key);
			if (it == end()) return { end(), end() };
			iterator next_it = it;
			++next_it;
			return { it, next_it };
		}

		[[nodiscard]] std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const noexcept {
			const_iterator it = find(key);
			if (it == end()) return { end(), end() };
			const_iterator next_it = it;
			++next_it;
			return { it, next_it };
		}

		[[nodiscard]] size_type bucket_count() const noexcept {
			return m_bucket_count;
		}
		[[nodiscard]] size_type max_bucket_count() const noexcept {
			return m_bucket_count;
		}

		[[nodiscard]] float load_factor() const noexcept {
			return m_load_factor;
		}
		[[nodiscard]] hasher hash_function() const noexcept {
			return m_hasher;
		}
		[[nodiscard]] key_equal key_eq() const noexcept {
			return m_key_equal;
		}

		[[nodiscard]] allocator_type get_allocator() const noexcept {
			return m_allocator;
		}

	
		[[nodiscard]] const Node_t* find_node(const key_type& key) const noexcept {
			if (empty()) return nullptr;

			size_type index = hash_function()(key) % bucket_count();
			const Node_t* current_node = &nodeAt(index);
			while (current_node) {
				if (!current_node->is_free() && key_eq()(current_node->data.first, key)) {
					return current_node;
				}
				current_node = current_node->next();
			}

			return nullptr;
		}

		[[nodiscard]] Node_t& nodeAt(size_type index) {
			if (index >= bucket_count()) throw std::out_of_range("invalid hashmap<K, T> node index");
			return m_data[index];
		}

		[[nodiscard]] const Node_t& nodeAt(size_type index) const {
			if (index >= bucket_count()) throw std::out_of_range("invalid hashmap<K, T> node index");
			return m_data[index];
		}

		Node_t* m_data = nullptr;
		size_type		m_stride = 0;
		size_type		m_bucket_count = 0;
		size_type		m_element_count = 0;
		float			m_load_factor = 0.75;
		size_type		m_pad[2]{};

		hasher			m_hasher;
		key_equal		m_key_equal;
		allocator_type	m_allocator;
	};
}
