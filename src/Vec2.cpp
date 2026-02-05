#include <Brise/Vec2.h>

namespace Brise
{
	float& Vec2::operator[](int i)
	{
		return ((&x)[i]);
	}

	const float& Vec2::operator[](int i) const
	{
		return ((&x)[i]);
	}

	Vec2& Vec2::operator *=(float s)
	{
		x *= s; y *= s;
		return (*this);
	}
	
	Vec2& Vec2::operator /=(float s)
	{
		x /= s; y /= s;
		return (*this);
	}

	Vec2& Vec2::operator +=(const Vec2& v)
	{
		x += v.x, y += v.y;
		return (*this);
	}
	Vec2& Vec2::operator -=(const Vec2& v)
	{
		x -= v.x; y -= v.y;
		return (*this);
	}

	std::ostream& operator<<(std::ostream& os, const Vec2& v)
	{
		std::cout << "(" << v[0] << ", " << v[1] << ")";
		return os;
	}
	
	std::string Vec2::ToString()
	{
		std::ostringstream oss;
		oss << "(" << x << ", " << y << ")";
		return oss.str();
	}
}