#include <iostream>
#include "Vector.h"


int main()
{
    vector::Vector a = vector::Vector(number::createNumber(2), number::createNumber(2));
    vector::Vector b = vector::Vector(number::createNumber(10.5), number::createNumber(12));
    vector::Vector sum = vector::Vector::sum(a, b);
    std::cout << "Sum of two vectors: " << sum.getX() << " " << sum.getY() << std::endl;
    std::cout << "Sum of two vectors in polar coordinates: " << sum.getRadius() << " " << sum.getAngle() << std::endl;
    vector::Vector zero_vector = vector::ONE_VECTOR;
    std::cout <<"Null vector: " << zero_vector.getX() << " " << zero_vector.getY() << std::endl;

    number::Number ten = number::createNumber(10);
    number::Number eight = number::createNumber(8.5);
    number::Number result = (ten * eight + 4.5 +number::ONE)/ten + number::ZERO;
    std::cout << "Result of calculation: "<<result<< std::endl;

   
}
