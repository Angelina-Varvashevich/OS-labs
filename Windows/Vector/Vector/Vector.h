#pragma once
#include "Number.h"
#pragma once

#ifdef VECTOR_EXPORTS
#define VECTOR_API __declspec(dllexport)
#else
#define VECTOR_API __declspec(dllimport)
#endif

namespace vector {
	class VECTOR_API Vector {
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

	extern const VECTOR_API Vector ZERO_VECTOR;
	extern const VECTOR_API Vector ONE_VECTOR;

}