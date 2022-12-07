#include <iostream>
#include <sstream>

int main()
{
    std::string xString;
    std::getline(std::cin, xString);
    std::stringstream ss(xString);
    int num;
    while (ss >> num){
        std::cout << num * num * num  << " ";
    }
    std::cout << std::endl;
    return 0;
}
