#include "pch.h"
#include "Vector.h"

namespace vector {
	Vector::Vector(number::Number x_, number::Number y_) : x(x_), y(y_)
	{
	}

	number::Number Vector::getAngle()
	{
		number::Number z = y / x;
		return z.ATAN();
	}

	number::Number Vector::getRadius()
	{
		number::Number z = x * x + y * y;
		return z.SQRT();
	}

	number::Number Vector::getX()
	{
		return x;
	}

	number::Number Vector::getY()
	{
		return y;
	}

	Vector Vector::sum(Vector& v1, Vector& v2)
	{
		return Vector(v1.getX() + v2.getX(), v1.getY() + v2.getY());
	}

	const Vector ONE_VECTOR = Vector(number::ONE, number::ONE);

	const Vector ZERO_VECTOR = Vector(number::ZERO, number::ZERO);
}