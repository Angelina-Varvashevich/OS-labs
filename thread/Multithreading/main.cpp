#include <iostream>
#include <chrono>
#include <iomanip>

#include "Matrix.h"
#include "BlockMatrix.h"

int main() {
  const int size = 100;
  Matrix a(size, size);
  a.fillMatrix();
  //a.print();
  Matrix b(size, size);
  b.fillMatrix();
  //b.print();
  auto start = std::chrono::steady_clock::now();
  Matrix result = a * b;
  //result.print();
  auto end = std::chrono::steady_clock::now();
  int time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
  std::cout << "1 thread calculation" << std::setw(5) << " time in microseconds: " << time << std::endl;

  for (int blockSize = 1; blockSize <= size; blockSize++) {
    auto start = std::chrono::steady_clock::now();
    Matrix result = BlockMatrix::multiply(a, b, blockSize);
    //result.print();
    auto end = std::chrono::steady_clock::now();
    int time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "size of block: " << blockSize << std::setw(5) << " time in microseconds: " << time << std::endl;
  }
  return 0;
}
