#ifndef MULTITHREADING__MATRIX_H_
#define MULTITHREADING__MATRIX_H_

#include<vector>

class Matrix {
 public:
  explicit Matrix(int m, int n);

  explicit Matrix(std::vector<std::vector<double>> matrix_);

  [[nodiscard]] int getRow() const;

  [[nodiscard]] int getColumn() const;

  [[nodiscard]] double getElement(int i, int j) const;

  void setElement(int i, int j, double value);

  Matrix& operator=(const Matrix& anotherMatrix);

  friend Matrix operator+(const Matrix& matrixA, const Matrix& matrixB);

  friend Matrix operator*(const Matrix& matrixA, const Matrix& matrixB);

  void print() const;

  void fillMatrix();

 private:

  int n;

  int m;

  std::vector<std::vector<double>> matrix;
};

#endif //MULTITHREADING__MATRIX_H_
