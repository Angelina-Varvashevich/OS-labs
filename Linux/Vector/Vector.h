#pragma once
#include "Number.h"
#pragma once

namespace vector {
	class Vector {
	public:
		Vector(number::Number x_, number::Number y_);
		number::Number getAngle();
		number::Number getRadius();
		number::Number getX();
		number::Number getY();
		static Vector sum(Vector& v1, Vector& v2);
	private:
		number::Number x;
		number::Number y;
	};

	extern const Vector ZERO_VECTOR;
	extern const Vector ONE_VECTOR;

}