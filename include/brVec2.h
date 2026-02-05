#pragma once 

#include <iostream>
#include <string>
#include <sstream>

namespace Brise
{
	struct brVec2
	{
		float x, y;

		brVec2() = default;

		brVec2(float _x, float _y)
			: x(_x), y(_y) {
		}

		std::string ToString();

		// Operators overloads ------------------

		// Access components by index
		float& operator [](int i);
		const float& operator [](int i) const;

		// Scalar multiplication
		brVec2& operator /=(float s);
		brVec2& operator *=(float s);

		// Addition & substraction
		brVec2& operator +=(const brVec2& v);
		brVec2& operator -=(const brVec2& v);
	};

	// brVec2 operations functions
	inline brVec2 operator *(const brVec2& v, float s)
	{
		return (brVec2(v.x * s, v.y * s));
	}

	inline brVec2 operator /(const brVec2& v, float s)
	{
		s = 1.0f / s;
		return (brVec2(v.x * s, v.y * s));
	}

	inline brVec2 operator -(const brVec2& v)
	{
		return (brVec2(-v.x, -v.y));
	}

	inline float Magnitude(const brVec2& v)
	{
		return (sqrt(v.x * v.x + v.y * v.y));
	}

	inline brVec2 Normalize(const brVec2& v)
	{
		return (v / Magnitude(v));
	}

	inline brVec2 operator+(const brVec2& v1, const brVec2& v2)
	{
		return (brVec2(v1.x + v2.x, v1.y + v2.y));
	}
	
	inline brVec2 operator-(const brVec2& v1, const brVec2& v2)
	{
		return (brVec2(v1.x - v2.x, v1.y - v2.y));
	}

	inline float Dot(const brVec2& v1, const brVec2& v2)
	{
		return (v1.x * v2.x + v1.y * v2.y);
	}

	inline brVec2 Project(const brVec2& v1, const brVec2 v2)
	{
		return (v2 * (Dot(v1, v2) / Dot(v2, v2)));
	}

	inline brVec2 Reject(const brVec2& v1, const brVec2& v2)
	{
		return (v1 - v2 * (Dot(v1, v2) / Dot(v2, v2)));
	}

	
	// << operator overload to cout vector components
	std::ostream& operator<<(std::ostream& os, const brVec2& v);
}