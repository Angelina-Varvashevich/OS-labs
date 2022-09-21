#include <iostream>
#include "Vector.h"

using vector::Vector;
using number::Number;

int main()
{
    Vector a = Vector(number::createNumber(2), number::createNumber(2));
    Vector b = Vector(number::createNumber(10.5), number::createNumber(12));
    Vector sum = Vector::sum(a, b);
    std::cout << "Sum of two vectors: " << sum.getX() << " " << sum.getY() << std::endl;
    std::cout << "Sum of two vectors in polar coordinates: " << sum.getRadius() << " " << sum.getAngle() << std::endl;
    Vector zero_vector = vector::ZERO_VECTOR;
    std::cout <<"Null vector: " << zero_vector.getX() << " " << zero_vector.getY() << std::endl;

    Number ten = number::createNumber(10);
    Number eight = number::createNumber(8.5);
    Number result = (ten * eight + 4.5 +number::ONE)/ten + number::ZERO;
    std::cout << "Result of calculation: "<<result<< std::endl;


}


