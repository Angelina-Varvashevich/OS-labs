#include <iostream>
#include <sstream>

int main()
{
    int sum = 0;
    std::string xString;
    std::getline(std::cin, xString);
    std::stringstream ss(xString);
    int num;
    while (ss >> num) {
        sum += num;
    }
    std::cout << sum << std::endl;
    return 0;
}