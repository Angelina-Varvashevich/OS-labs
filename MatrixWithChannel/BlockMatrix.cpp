#include <thread>
#include<mutex>
#include <cmath>
#include <map>

#include "BlockMatrix.h"

static std::map<std::pair<int, int>, std::mutex> mutex_map;

BlockMatrix::BlockMatrix(std::vector<std::vector<Matrix>> blockMatrix_) : blockMatrix(std::move(blockMatrix_)) {

}

Matrix BlockMatrix::createBlock(const Matrix& matrix, const int i, const int j, int blockSize) {
  std::vector<std::vector<double>> blockOfMatrix;
  int k_i = i;
  int currentBlockSizeI = 0;
  int currentBlockSizeJ = 0;
  if (i + blockSize > matrix.getRow()) currentBlockSizeI = matrix.getRow() % blockSize;
  else currentBlockSizeI = blockSize;
  while (k_i < i + currentBlockSizeI) {
    std::vector<double> rowOfBlock;
    int k_j = j;
    if (j + blockSize > matrix.getColumn()) currentBlockSizeJ = matrix.getRow() % blockSize;
    else currentBlockSizeJ = blockSize;
    while (k_j < j + currentBlockSizeJ) {
      rowOfBlock.push_back(matrix.getElement(k_i, k_j));
      k_j++;
    }
    blockOfMatrix.push_back(rowOfBlock);
    k_i++;
  }
  return Matrix(blockOfMatrix);
}

BlockMatrix::BlockMatrix(const Matrix& mtr, int blockSize) {
  for (int i = 0; i < mtr.getRow(); i += blockSize) {
    std::vector<Matrix> blockMatrixRow;
    for (int j = 0; j < mtr.getColumn(); j += blockSize) {
      blockMatrixRow.push_back(createBlock(mtr, i, j, blockSize));
    }
    blockMatrix.push_back(blockMatrixRow);
  }
}

void BlockMatrix::multiplyBlock(const BlockMatrix& matrixA,
                                const BlockMatrix& matrixB,
                                BlockMatrix& matrixResult,
                                int i,
                                int j, BufferedChannel<std::function<void()>>& channel) {
  for (int k = 0; k < matrixB.getColumn(); k++) {
    channel.Send([&matrixA, &matrixB, &matrixResult, i, j, k]() {
      currentMultiplyBlock(matrixA, matrixB, matrixResult, i, j, k);
    });
  }
}

void BlockMatrix::currentMultiplyBlock(const BlockMatrix& matrixA,
                                       const BlockMatrix& matrixB, BlockMatrix& matrixResult,
                                       int i,
                                       int j, int k) {
  static std::mutex mutex_;
  mutex_.lock();
  Matrix currentBlock = (matrixResult.getMatrix(i, j) + matrixA.getMatrix(i, k) * matrixB.getMatrix(k, j));
  matrixResult.setMatrix(currentBlock, i, j);
  mutex_.unlock();
}

void BlockMatrix::multiplyMatrices(const BlockMatrix& matrixA,
                                   const BlockMatrix& matrixB,
                                   BlockMatrix& matrixResult,
                                   BufferedChannel<std::function<void()>>& channel, int countOfThreads) {
  std::thread tr([&]() {
    for (int i = 0; i < matrixResult.getRow(); i++) {
      for (int j = 0; j < matrixResult.getColumn(); j++) {
        multiplyBlock(matrixA, matrixB, matrixResult, i, j, channel);
      }
    }
    channel.Close();
  });
  std::vector<std::thread> threadsVector;
  for (int i = 0; i < countOfThreads; i++) {
    threadsVector.emplace_back([&channel]() {
      std::pair<std::function<void()>, bool> currentPair = channel.Recv();
      bool currentSecond = currentPair.second;
      while (currentSecond) {
        currentPair.first();
        currentPair = channel.Recv();
        currentSecond = currentPair.second;
      }
    });
  }
  tr.join();
  for (auto& thread : threadsVector)
    thread.join();
}

Matrix BlockMatrix::multiply(const Matrix& a,
                             const Matrix& b,
                             int blockSize,
                             int countOfThreads) {
  BufferedChannel<std::function<void()>> channel(200);
  BlockMatrix A(a, blockSize);
  BlockMatrix B(b, blockSize);
  Matrix result(a.getRow(), a.getColumn());
  BlockMatrix Result(result, blockSize);
  multiplyMatrices(A, B, Result, channel, countOfThreads);
  return Result.toMatrix(result.getRow(), result.getColumn());
}

Matrix BlockMatrix::toMatrix(int m, int n) const {
  Matrix result(m, n);
  int blockSize = std::max(getMatrix(0, 0).getRow(), getMatrix(0, 0).getColumn());
  for (int i = 0; i < getRow(); i++) {
    for (int j = 0; j < getColumn(); j++) {
      for (int k_i = 0; k_i < blockMatrix[i][j].getRow(); k_i++) {
        for (int k_j = 0; k_j < blockMatrix[i][j].getColumn(); k_j++) {
          result.setElement(k_i + blockSize * i, k_j + blockSize * j, blockMatrix[i][j].getElement(k_i, k_j));
        }
      }
    }
  }
  return result;
}

int BlockMatrix::getRow() const {
  return blockMatrix.size();
}

int BlockMatrix::getColumn() const {
  return blockMatrix[0].size();
}

Matrix BlockMatrix::getMatrix(int i, int j) const {
  return blockMatrix[i][j];
}

void BlockMatrix::setMatrix(const Matrix& currentMatrix, int i, int j) {
  blockMatrix[i][j] = currentMatrix;
}


