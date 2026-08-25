#pragma once

struct Vec2 {
	float x, z;

	bool operator==(const Vec2& other) const noexcept {
		return x == other.x && z == other.z;
	}

	bool operator!=(const Vec2& other) const noexcept {
		return !(*this == other);
	}

	bool operator<(const Vec2& other) const noexcept {
		if (x < other.x) return true;
		if (x > other.x) return false;
		return z < other.z;
	}

	bool operator<=(const Vec2& other) const noexcept {
		return *this < other || *this == other;
	}

	bool operator>(const Vec2& other) const noexcept {
		return other < *this;
	}

	bool operator>=(const Vec2& other) const noexcept {
		return other <= *this;
	}

	float& operator[](const unsigned int index) {
		switch (index) {
			case 0: return x;
			case 1: return z;
			default: throw "Vec3 index out of range";
		}
	}

	const float& operator[](const unsigned int index) const {
		switch (index) {
			case 0: return x;
			case 1: return z;
			default: throw "Vec3 index out of range";
		}
	}

	[[nodiscard]] Vec2 delta(const Vec2& other) const noexcept {
		return Vec2{ x - other.x, z - other.z };
	}

	[[nodiscard]] float magnitude() const noexcept {
		const float v = (x * x + z * z);

		union {
			float f; int i;
		} u = { v };
		u.i = (u.i >> 1) + 0x1fc00000;
		return u.f;
	}

	static float distance(const Vec2& a, const Vec2& b) noexcept {
		return a.delta(b).magnitude();
	}

	[[nodiscard]] Vec2 normalize() const noexcept {
		const float mag = magnitude();
		return mag > 0 ? Vec2{ x / mag, z / mag } : Vec2{ 0, 0 };
	}

	[[nodiscard]] Vec2 scale(const float factor) const noexcept {
		return Vec2{ x * factor, z * factor };
	}

	Vec2 operator+(const Vec2& other) const noexcept {
		return Vec2{ x + other.x, z + other.z };
	}

	Vec2 operator-(const Vec2& other) const noexcept {
		return Vec2{ x - other.x, z - other.z };
	}

	Vec2 operator*(const float factor) const noexcept {
		return scale(factor);
	}

	Vec2 operator/(const float factor) const noexcept {
		if (factor != 0)
			return Vec2{ x / factor, z / factor };
		return *this;
	}

	Vec2& operator+=(const Vec2& other) noexcept {
		x += other.x;
		z += other.z;
		return *this;
	}

	Vec2& operator-=(const Vec2& other) noexcept {
		x -= other.x;
		z -= other.z;
		return *this;
	}

	Vec2& operator*=(const float factor) noexcept {
		x *= factor;
		z *= factor;
		return *this;
	}

	Vec2& operator/=(const float factor) noexcept {
		if (factor != 0) {
			x /= factor;
			z /= factor;
		}
		return *this;
	}

	static Vec2 add(const Vec2& a, const Vec2& b) noexcept {
		return Vec2{ a.x + b.x, a.z + b.z };
	}

	static Vec2 subtract(const Vec2& a, const Vec2& b) noexcept {
		return Vec2{ a.x - b.x, a.z - b.z };
	}

	[[nodiscard]] float dot(const Vec2& other) const noexcept {
		return x * other.x + z * other.z;
	}
};
struct Vec3 {
	float x, y, z;

	bool operator==(const Vec3& other) const noexcept {
		return x == other.x && y == other.y && z == other.z;
	}

	bool operator!=(const Vec3& other) const noexcept {
		return !(*this == other);
	}

	bool operator<(const Vec3& other) const noexcept {
		if (x < other.x) return true;
		if (x > other.x) return false;
		if (y < other.y) return true;
		if (y > other.y) return false;
		return z < other.z;
	}

	bool operator<=(const Vec3& other) const noexcept {
		return *this < other || *this == other;
	}

	bool operator>(const Vec3& other) const noexcept {
		return other < *this;
	}

	bool operator>=(const Vec3& other) const noexcept {
		return other <= *this;
	}

	float& operator[](const unsigned int index) {
		switch (index) {
			case 0: return x;
			case 1: return y;
			case 2: return z;
			default: throw "Vec3 index out of range";
		}
	}

	const float& operator[](const unsigned int index) const {
		switch (index) {
			case 0: return x;
			case 1: return y;
			case 2: return z;
			default: throw "Vec3 index out of range";
		}
	}

	[[nodiscard]] Vec3 delta(const Vec3& other) const noexcept {
		return Vec3{ x - other.x, y - other.y, z - other.z };
	}

	[[nodiscard]] float magnitude() const noexcept {
		const float v = (x * x + y * y + z * z);

		union {
			float f; int i;
		} u = { v };
		u.i = (u.i >> 1) + 0x1fc00000;
		return u.f;
	}

	static float distance(const Vec3& a, const Vec3& b) noexcept {
		return a.delta(b).magnitude();
	}

	[[nodiscard]] Vec3 normalize() const noexcept {
		const float mag = magnitude();
		return mag > 0 ? Vec3{ x / mag, y / mag, z / mag } : Vec3{ 0, 0, 0 };
	}

	[[nodiscard]] Vec3 scale(const float factor) const noexcept {
		return Vec3{ x * factor, y * factor, z * factor };
	}

	Vec3 operator+(const Vec3& other) const noexcept {
		return Vec3{ x + other.x, y + other.y, z + other.z };
	}

	Vec3 operator-(const Vec3& other) const noexcept {
		return Vec3{ x - other.x, y - other.y, z - other.z };
	}

	Vec3 operator*(const float factor) const noexcept {
		return scale(factor);
	}

	Vec3 operator/(const float factor) const noexcept {
		if (factor != 0)
			return Vec3{ x / factor, y / factor, z / factor };
		return *this;
	}

	Vec3& operator+=(const Vec3& other) noexcept {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Vec3& operator-=(const Vec3& other) noexcept {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	Vec3& operator*=(const float factor) noexcept {
		x *= factor;
		y *= factor;
		z *= factor;
		return *this;
	}

	Vec3& operator/=(const float factor) noexcept {
		if (factor != 0) {
			x /= factor;
			y /= factor;
			z /= factor;
		}
		return *this;
	}

	static Vec3 add(const Vec3& a, const Vec3& b) noexcept {
		return Vec3{ a.x + b.x, a.y + b.y, a.z + b.z };
	}

	static Vec3 subtract(const Vec3& a, const Vec3& b) noexcept {
		return Vec3{ a.x - b.x, a.y - b.y, a.z - b.z };
	}

	[[nodiscard]] float dot(const Vec3& other) const noexcept {
		return x * other.x + y * other.y + z * other.z;
	}

	[[nodiscard]] Vec3 cross(const Vec3& other) const noexcept {
		return Vec3{
			y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x
		};
	}
};
struct Vec4 {
	float x, y, z, t;

	bool operator==(const Vec4& other) const noexcept {
		return x == other.x && y == other.y && z == other.z && t == other.t;
	}

	bool operator!=(const Vec4& other) const noexcept {
		return !(*this == other);
	}

	bool operator<(const Vec4& other) const noexcept {
		if (x < other.x) return true;
		if (x > other.x) return false;
		if (y < other.y) return true;
		if (y > other.y) return false;
		if (z < other.z) return true;
		if (z > other.z) return false;
		return t < other.t;
	}

	bool operator<=(const Vec4& other) const noexcept {
		return *this < other || *this == other;
	}

	bool operator>(const Vec4& other) const noexcept {
		return other < *this;
	}

	bool operator>=(const Vec4& other) const noexcept {
		return other <= *this;
	}

	float& operator[](const unsigned int index) {
		switch (index) {
			case 0: return x;
			case 1: return y;
			case 2: return z;
			case 3: return t;
			default: throw "Vec3 index out of range";
		}
	}

	const float& operator[](const unsigned int index) const {
		switch (index) {
			case 0: return x;
			case 1: return y;
			case 2: return z;
			case 3: return t;
			default: throw "Vec3 index out of range";
		}
	}

	[[nodiscard]] Vec4 delta(const Vec4& other) const noexcept {
		return Vec4{ x - other.x, y - other.y, z - other.z, t - other.t };
	}

	[[nodiscard]] float magnitude() const noexcept {
		const float v = (x * x + y * y + z * z + t * t);

		union {
			float f; int i;
		} u = { v };
		u.i = (u.i >> 1) + 0x1fc00000;
		return u.f;
	}

	static float distance(const Vec4& a, const Vec4& b) noexcept {
		return a.delta(b).magnitude();
	}

	[[nodiscard]] Vec4 normalize() const noexcept {
		float mag = magnitude();
		return mag > 0 ? Vec4{ x / mag, y / mag, z / mag, t / mag } : Vec4{ 0, 0, 0, 0 };
	}

	[[nodiscard]] Vec4 scale(const float factor) const noexcept {
		return Vec4{ x * factor, y * factor, z * factor, t * factor };
	}

	Vec4 operator+(const Vec4& other) const noexcept {
		return Vec4{ x + other.x, y + other.y, z + other.z, t + other.t };
	}

	Vec4 operator-(const Vec4& other) const noexcept {
		return Vec4{ x - other.x, y - other.y, z - other.z, t - other.t };
	}

	Vec4 operator*(const float factor) const noexcept {
		return scale(factor);
	}

	Vec4 operator/(const float factor) const noexcept {
		if (factor != 0)
			return Vec4{ x / factor, y / factor, z / factor, t / factor };
		return *this;
	}

	Vec4& operator+=(const Vec4& other) noexcept {
		x += other.x;
		y += other.y;
		z += other.z;
		t += other.t;
		return *this;
	}

	Vec4& operator-=(const Vec4& other) noexcept {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		t -= other.t;
		return *this;
	}

	Vec4& operator*=(const float factor) noexcept {
		x *= factor;
		y *= factor;
		z *= factor;
		t *= factor;
		return *this;
	}

	Vec4& operator/=(const float factor) noexcept {
		if (factor != 0) {
			x /= factor;
			y /= factor;
			z /= factor;
			t /= factor;
		}
		return *this;
	}

	static Vec4 add(const Vec4& a, const Vec4& b) noexcept {
		return Vec4{ a.x + b.x, a.y + b.y, a.z + b.z, a.t + b.t };
	}

	static Vec4 subtract(const Vec4& a, const Vec4& b) noexcept {
		return Vec4{ a.x - b.x, a.y - b.y, a.z - b.z, a.t - b.t };
	}

	[[nodiscard]] float dot(const Vec4& other) const noexcept {
		return x * other.x + y * other.y + z * other.z + t * other.t;
	}
};
struct IVec3 {
	int x, y, z;

	bool operator==(const IVec3& other) const noexcept {
		return x == other.x && y == other.y && z == other.z;
	}

	bool operator!=(const IVec3& other) const noexcept {
		return !(*this == other);
	}

	bool operator<(const IVec3& other) const noexcept {
		if (x < other.x) return true;
		if (x > other.x) return false;
		if (y < other.y) return true;
		if (y > other.y) return false;
		return z < other.z;
	}

	bool operator<=(const IVec3& other) const noexcept {
		return *this < other || *this == other;
	}

	bool operator>(const IVec3& other) const noexcept {
		return other < *this;
	}

	bool operator>=(const IVec3& other) const noexcept {
		return other <= *this;
	}

	int& operator[](const unsigned int index) {
		switch (index) {
			case 0: return x;
			case 1: return y;
			case 2: return z;
			default: throw "IVec3 index out of range";
		}
	}

	const int& operator[](const unsigned int index) const {
		switch (index) {
			case 0: return x;
			case 1: return y;
			case 2: return z;
			default: throw "IVec3 index out of range";
		}
	}

	[[nodiscard]] IVec3 delta(const IVec3& other) const noexcept {
		return IVec3{ x - other.x, y - other.y, z - other.z };
	}

	[[nodiscard]] int magnitude() const noexcept {
		const int v = (x * x + y * y + z * z);

		union {
			int f; int i;
		} u = { v };
		u.i = (u.i >> 1) + 0x1fc00000;
		return u.f;
	}

	static double distance(const IVec3& a, const IVec3& b) noexcept {
		return a.delta(b).magnitude();
	}

	[[nodiscard]] IVec3 scale(const int factor) const noexcept {
		return IVec3{ x * factor, y * factor, z * factor };
	}

	IVec3 operator+(const IVec3& other) const noexcept {
		return IVec3{ x + other.x, y + other.y, z + other.z };
	}

	IVec3 operator-(const IVec3& other) const noexcept {
		return IVec3{ x - other.x, y - other.y, z - other.z };
	}

	IVec3 operator*(const int factor) const noexcept {
		return scale(factor);
	}

	IVec3 operator/(const int factor) const noexcept {
		if (factor != 0)
			return IVec3{ x / factor, y / factor, z / factor };
		return *this;
	}

	IVec3& operator+=(const IVec3& other) noexcept {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	IVec3& operator-=(const IVec3& other) noexcept {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	IVec3& operator*=(const int factor) noexcept {
		x *= factor;
		y *= factor;
		z *= factor;
		return *this;
	}

	IVec3 operator&(const int factor) const noexcept {
		return IVec3{ x & factor, y & factor, z & factor };
	}

	static IVec3 add(const IVec3& a, const IVec3& b) noexcept {
		return IVec3{ a.x + b.x, a.y + b.y, a.z + b.z };
	}

	static IVec3 subtract(const IVec3& a, const IVec3& b) noexcept {
		return IVec3{ a.x - b.x, a.y - b.y, a.z - b.z };
	}

	[[nodiscard]] int dot(const IVec3& other) const noexcept {
		return x * other.x + y * other.y + z * other.z;
	}

	[[nodiscard]] IVec3 cross(const IVec3& other) const noexcept {
		return IVec3{
			y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x
		};
	}
};
