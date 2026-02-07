#pragma once 

#include <iostream>
#include <string>
#include <sstream>

namespace Brise
{
	struct Vec2
	{
		float x, y;

		Vec2() = default;

		Vec2(float _x, float _y)
			: x(_x), y(_y) {
		}

		std::string ToString();

		// Operators overloads ------------------

		// Access components by index
		float& operator [](int i);
		const float& operator [](int i) const;

		// Scalar multiplication
		Vec2& operator /=(float s);
		Vec2& operator *=(float s);

		// Addition & substraction
		Vec2& operator +=(const Vec2& v);
		Vec2& operator -=(const Vec2& v);
	};

	// Vec2 operations functions
	inline Vec2 operator *(const Vec2& v, float s)
	{
		return (Vec2(v.x * s, v.y * s));
	}

	inline Vec2 operator /(const Vec2& v, float s)
	{
		s = 1.0f / s;
		return (Vec2(v.x * s, v.y * s));
	}

	inline Vec2 operator -(const Vec2& v)
	{
		return (Vec2(-v.x, -v.y));
	}

	inline float Magnitude(const Vec2& v)
	{
		return (sqrt(v.x * v.x + v.y * v.y));
	}

	inline Vec2 Normalize(const Vec2& v)
	{
		return (v / Magnitude(v));
	}

	inline Vec2 operator+(const Vec2& v1, const Vec2& v2)
	{
		return (Vec2(v1.x + v2.x, v1.y + v2.y));
	}
	
	inline Vec2 operator-(const Vec2& v1, const Vec2& v2)
	{
		return (Vec2(v1.x - v2.x, v1.y - v2.y));
	}

	inline float Dot(const Vec2& v1, const Vec2& v2)
	{
		return (v1.x * v2.x + v1.y * v2.y);
	}

	inline Vec2 Project(const Vec2& v1, const Vec2 v2)
	{
		return (v2 * (Dot(v1, v2) / Dot(v2, v2)));
	}

	inline Vec2 Reject(const Vec2& v1, const Vec2& v2)
	{
		return (v1 - v2 * (Dot(v1, v2) / Dot(v2, v2)));
	}

	inline float DistanceSquared(const Vec2& v1, const Vec2 v2)
	{
		return (powf(v2.x - v1.x, 2) + powf(v2.y - v1.y, 2));
	}
	
	// << operator overload to cout vector components
	std::ostream& operator<<(std::ostream& os, const Vec2& v);
}