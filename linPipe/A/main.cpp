#include <iostream>
#include <sstream>
int main()
{
    int N = 5;
    std::string xString;
    std::getline(std::cin, xString);
    std::stringstream ss(xString);
    int num;
    while (ss >> num) {
        std::cout << num + N << " ";
    }
    std::cout << std::endl;
    return 0;
}
