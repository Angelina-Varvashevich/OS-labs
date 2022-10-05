#include <iostream>
#include <chrono>
#include <iomanip>

#include "Matrix.h"
#include "BlockMatrix.h"

int main() {
  const int size = 50;
  Matrix a(size, size);
  a.fillMatrix();
  Matrix b(size, size);
  b.fillMatrix();
  for (int blockSize = 1; blockSize <= size; blockSize++) {
    //
    auto start = std::chrono::steady_clock::now();
    Matrix result = BlockMatrix::multiply(a, b, blockSize);
    auto end = std::chrono::steady_clock::now();
    int time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "size of block: " << blockSize << std::setw(5) << " time in microseconds: " << time << std::endl;
    //
  }
  return 0;
}
