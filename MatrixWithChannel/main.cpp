#include <iostream>
#include <chrono>
#include <iomanip>

#include "Matrix.h"
#include "BlockMatrix.h"

int main() {
  const int size = 70;
  Matrix a(size, size);
  a.fillMatrix();
  Matrix b(size, size);
  b.fillMatrix();
  const int blockSize = 2;
  for (int countOfThreads = 1; countOfThreads <= 150; countOfThreads++) {
    auto start = std::chrono::steady_clock::now();
    Matrix result = BlockMatrix::multiply(a, b, blockSize, countOfThreads);
    auto end = std::chrono::steady_clock::now();
    int time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "count of threads: " << countOfThreads << std::setw(5) << " time in microseconds: " << time
              << std::endl;
  }
  return 0;
}
