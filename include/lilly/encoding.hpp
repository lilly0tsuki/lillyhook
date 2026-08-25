#pragma once

#include "lilly/string.hpp"
#include "lilly/vectors.h"

#include <cstdlib>
#include <cstring>
#include <span>

inline uint32_t zigzagEncode(const int32_t value) noexcept {
  return static_cast<uint32_t>(value << 1 ^ value >> 31);
}

inline uint64_t zigzagEncode(const int64_t value) noexcept {
  return static_cast<uint64_t>(value << 1 ^ value >> 63);
}

inline int32_t zigzagDecode(const uint32_t value) noexcept {
  return static_cast<int32_t>((value >> 1) ^ static_cast<uint32_t>(-static_cast<int32_t>(value & 1)));
}

inline int64_t zigzagDecode(const uint64_t value) noexcept {
  return static_cast<int64_t>((value >> 1) ^ static_cast<uint64_t>(-static_cast<int64_t>(value & 1)));
}

inline size_t varintEncode(uint32_t value, uint8_t* out) noexcept {
  size_t i = 0;
  while (value >= 0x80) {
    out[i++] = (value & 0x7F) | 0x80;
    value >>= 7;
  }
  out[i++] = value & 0x7F;
  return i;
}

inline size_t varintEncode(uint64_t value, uint8_t* out) noexcept {
  size_t i = 0;
  while (value >= 0x80) {
    out[i++] = (value & 0x7F) | 0x80;
    value >>= 7;
  }
  out[i++] = value & 0x7F;
  return i;
}

inline size_t varintDecode(const uint8_t* in, uint8_t* out) noexcept {
  *out = 0;
  size_t shift = 0;

  for (size_t i = 0; i < 5; ++i) {
    const uint8_t byte = *in++;
    *out |= static_cast<uint64_t>(byte & 0x7F) << shift;
    shift += 7;

    if (!(byte & 0x80))
      return i + 1;
  }

  return 0;
}

inline size_t varintDecode(const uint8_t* in, uint64_t* out) noexcept {
  *out = 0;
  size_t shift = 0;

  for (size_t i = 0; i < 10; ++i) {
    const uint8_t byte = *in++;
    *out |= static_cast<uint64_t>(byte & 0x7F) << shift;
    shift += 7;

    if (!(byte & 0x80))
      return i + 1;
  }

  return 0;
}

static constexpr int32_t PM_MULTIPLIER = 16807;
static constexpr int32_t PM_MODULUS = 0x7FFFFFFF;
static constexpr int32_t PM_QUOTIENT = (PM_MODULUS / PM_MULTIPLIER);
static constexpr int32_t PM_REMAINDER = (PM_MODULUS % PM_MULTIPLIER);

static constexpr uint32_t FNV_OFFSET = 0x811C9DC5;
static constexpr int32_t FNV_PRIME = 0x1000193;

namespace Trove {
  template <typename T>
  struct hash {
    size_t operator()(const T& key) const noexcept {
      return hash<T>{}(key);
    }
  };

  template <>
  struct hash<const char*> {
    size_t operator()(const char* str, const size_t len) const noexcept {
      size_t hash = FNV_OFFSET;
      const std::span<const char> data(str, len);
      unsigned int i = 0;

      for (; i + 3 < data.size(); i += 4) {
        unsigned int chunk = 0;
        std::memcpy(&chunk, data.subspan(i, 4).data(), sizeof(chunk));
        hash = (hash ^ chunk) * FNV_PRIME;
      }

      unsigned int rem = 0;
      for (unsigned int j = 0; j < data.size() - i; ++j) {
        rem |= data[i + j] << (8 * (data.size() - i - j - 1));
      }
      if (data.size() - i > 0) {
        hash = (hash ^ rem) * FNV_PRIME;
      }

      return hash;
    }
  };

  template <>
  struct hash<int16_t> {
    size_t operator()(const int16_t& key) const noexcept {
      const auto [quot, rem] = std::ldiv(key, PM_QUOTIENT);
      int32_t mix = PM_MULTIPLIER * rem - PM_REMAINDER * quot;
      if (mix < 0) mix += PM_MODULUS;
      return static_cast<size_t>(mix);
    }
  };

  template <>
  struct hash<int32_t> {
    size_t operator()(const int32_t& key) const noexcept {
      const auto [quot, rem] = std::ldiv(key, PM_QUOTIENT);
      int32_t mix = PM_MULTIPLIER * rem - PM_REMAINDER * quot;
      if (mix < 0) mix += PM_MODULUS;
      return static_cast<size_t>(mix);
    }
  };

  template <>
  struct hash<int64_t> {
    size_t operator()(const int64_t& key) const noexcept {
      const int32_t low = static_cast<int32_t>(key & 0xFFFFFFFF);
      const auto [quot, rem] = std::ldiv(low, PM_QUOTIENT);
      int32_t mix = PM_MULTIPLIER * rem - PM_REMAINDER * quot;
      if (mix < 0) mix += PM_MODULUS;
      return static_cast<size_t>(mix);
    }
  };

  template <>
  struct hash<Trove::String> {
    size_t operator()(const Trove::String& str) const noexcept {
      return hash<const char*>{}(str.c_str(), str.size());
    }
  };

  template <>
  struct hash<IVec3> {
    size_t operator()(const IVec3& v) const noexcept {
      size_t hash = FNV_OFFSET;
      hash = FNV_PRIME * ((v.x / 16) ^ hash);
      hash = FNV_PRIME * ((v.y / 16) ^ hash);
      hash = FNV_PRIME * ((v.z / 16) ^ hash);
      return hash;
    }
  };
}
