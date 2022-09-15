#pragma once
#include <iostream>

namespace number {

	class Number {
	public:
		Number(double value_);
		Number(const Number& AnotherNumber);
		const Number operator+(const Number& AnotherNumber);
		const Number operator-(const Number& AnotherNumber);
		const Number operator*(const Number& AnotherNumber);
		const Number operator/(const Number& AnotherNumber);
		Number SQRT();
		Number ATAN();
		double getValue();
		operator double() const { return value; }
		Number& operator = (const Number& number);
	private:
		double value;
	};
	
	Number createNumber(double value_);

	extern const Number ONE;
	extern const Number ZERO;
}



