#include <iostream>

#include "Matrix.h"

Matrix& Matrix::operator=(const Matrix& anotherMatrix) {
  if (this == &anotherMatrix)
    return *this;
  this->matrix = anotherMatrix.matrix;
  this->m = anotherMatrix.m;
  this->n = anotherMatrix.n;
  return (*this);
}

Matrix operator+(const Matrix& matrixA, const Matrix& matrixB) {
  std::vector<std::vector<double>> matrixC;
  for (int i = 0; i < matrixA.m; i++) {
    std::vector<double> row;
    for (int j = 0; j < matrixA.n; j++) {
      row.push_back(matrixA.matrix[i][j] + matrixB.matrix[i][j]);
    }
    matrixC.push_back(row);
  }
  return Matrix(matrixC);
}

Matrix operator*(const Matrix& matrixA, const Matrix& matrixB) {
  std::vector<std::vector<double>> matrixC;
  for (int i = 0; i < matrixA.m; i++) {
    std::vector<double> row;
    for (int j = 0; j < matrixB.n; j++) {
      int c_i_j = 0;
      for (int k = 0; k < matrixA.n; k++) {
        c_i_j += matrixA.matrix[i][k] * matrixB.matrix[k][j];
      }
      row.push_back(c_i_j);
    }
    matrixC.push_back(row);
  }
  return Matrix(matrixC);
}

Matrix::Matrix(int m_, int n_) : m(m_), n(n_) {
  for (int i = 0; i < m; i++) {
    std::vector<double> row(n_, 0);
    matrix.push_back(row);
  }
}

Matrix::Matrix(std::vector<std::vector<double>> matrix_)
    : matrix(std::move(matrix_)), m(matrix_.size()), n(matrix_[0].size()) {
}

void Matrix::print() const {
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      std::cout << matrix[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

void Matrix::fillMatrix() {
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      matrix[i][j] = rand() % 100 + 1;
    }
  }
}

int Matrix::getRow() const {
  return m;
}

int Matrix::getColumn() const {
  return n;
}

double Matrix::getElement(int i, int j) const {
  return matrix[i][j];
}

void Matrix::setElement(int i, int j, double value) {
  matrix[i][j] = value;
}
