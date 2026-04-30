#ifndef _VECTOR2_H_
#define _VECTOR2_H_

#include <cmath>

class Vector2
{
public:
	double x = 0;
	double y = 0;



public:
	Vector2() = default;
	~Vector2() = default;

	Vector2(double x,double y)
		: x(x),y(y) { }

	Vector2 operator+(const Vector2& vec) const
	{
		return Vector2(x + vec.x, y + vec.y);
	}

	void operator+=(const Vector2& vec)
	{
		x += vec.x, y += vec.y;
	}

	Vector2 operator-(const Vector2& vec) const
	{
		return Vector2(x - vec.x, y - vec.y);
	}

	void operator-=(const Vector2& vec)
	{
		x -= vec.x, y -= vec.y;
	}

	double operator*(const Vector2& vec) const
	{
		return x * vec.x + y * vec.y;
	}

	Vector2 operator*(double val) const
	{
		return Vector2(x * val, y * val);
	}

	void operator*=(double val)
	{
		x *= val, y *= val;
	}

	bool operator==(const Vector2& vec) const
	{
		return x == vec.x && y == vec.y;
	}

	bool operator>(const Vector2& vec) const
	{
		return length() > vec.length();
	}

	bool operator<(const Vector2& vec) const
	{
		return length() < vec.length();
	}

	double length() const
	{
		return sqrt(x * x + y * y);
	}

	Vector2 normalize() const
	{
		double len = length();

		if (len == 0)
			return Vector2(0, 0);

		return Vector2(x / len, y / len);
	}

	bool approx_zero() const
	{
		return length() < 0.0001;
	}
private:

};







#endif // !_VECTOR2_H_

