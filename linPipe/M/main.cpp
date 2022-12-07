#include <iostream>
#include <string>
#include <sstream>

int main()
{
    std::string xString;
    std::getline(std::cin, xString);
    std::stringstream ss(xString);
    int num;
    while (ss >> num) {
        std::cout << num * 7 << " ";
    }
    std::cout << std::endl;
    return 0;
}