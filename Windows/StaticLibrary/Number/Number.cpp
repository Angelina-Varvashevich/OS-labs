#include "Number.h"
#include <cmath>

namespace number {

	Number::Number(double value_) : value(value_)
	{
	}

	Number::Number(const Number& CopyNumber)
	{
		value = CopyNumber.value;
	}

	const Number Number::operator+(const Number& AnotherNumber)
	{
		return value + AnotherNumber.value;
	}

	const Number Number::operator-(const Number& AnotherNumber)
	{
		return value - AnotherNumber.value;
	}

	const Number Number::operator*(const Number& AnotherNumber)
	{
		return value * AnotherNumber.value;
	}

	const Number Number::operator/(const Number& AnotherNumber)
	{
		return value / AnotherNumber.value;
	}

	Number createNumber(double value) {
		return Number(value);
	}

	Number Number::SQRT()
	{
		return Number(sqrt(value));
	}

	Number Number::ATAN()
	{
		return Number(atan(value));
	}

	double Number::getValue() {
		return value;
	}

	Number& Number::operator = (const Number& number)
	{
		value = number.value;
		return *this;
	}

	const Number ONE = Number(1);

	const Number ZERO = Number(0);
}


