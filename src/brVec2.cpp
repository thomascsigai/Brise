#include <brVec2.h>

namespace Brise
{
	float& brVec2::operator[](int i)
	{
		return ((&x)[i]);
	}

	const float& brVec2::operator[](int i) const
	{
		return ((&x)[i]);
	}

	brVec2& brVec2::operator *=(float s)
	{
		x *= s; y *= s;
		return (*this);
	}
	
	brVec2& brVec2::operator /=(float s)
	{
		x /= s; y /= s;
		return (*this);
	}

	brVec2& brVec2::operator +=(const brVec2& v)
	{
		x += v.x, y += v.y;
		return (*this);
	}
	brVec2& brVec2::operator -=(const brVec2& v)
	{
		x -= v.x; y -= v.y;
		return (*this);
	}

	std::ostream& operator<<(std::ostream& os, const brVec2& v)
	{
		std::cout << "(" << v[0] << ", " << v[1] << ")";
		return os;
	}
	
	std::string brVec2::ToString()
	{
		std::ostringstream oss;
		oss << "(" << x << ", " << y << ")";
		return oss.str();
	}
}