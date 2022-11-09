#pragma once
#include<vector>
#include<windows.h>

#include"Matrix.h"

class BlockMatrix {
public:
    explicit BlockMatrix(std::vector<std::vector<Matrix>> blockMatrix_);

    explicit BlockMatrix(const Matrix& m, int blockSize);

    [[nodiscard]] int getRow() const;

    [[nodiscard]] int getColumn() const;

    [[nodiscard]] Matrix getMatrix(int i, int j) const;

    void setMatrix(const Matrix& currentMatrix, int i, int j);

    [[nodiscard]] Matrix toMatrix(int m, int n) const;

    static Matrix multiply(const Matrix& a, const Matrix& b, int block_size);

private:
    static void multiplyBlock(const BlockMatrix& matrixA,
        const BlockMatrix& matrixB,
        BlockMatrix& matrixResult, int i, int j, std::vector<HANDLE>& threadsVector);

    static void multiplyMatrices(const BlockMatrix& matrixA,
        const BlockMatrix& matrixB, BlockMatrix& matrixResult);


    static DWORD WINAPI currentMultiplyBlock(LPVOID lpParams);

    Matrix createBlock(const Matrix& matrix, int i, int j, int block_size);

    std::vector<std::vector<Matrix>> blockMatrix;

};

struct ThreadParams {
    const BlockMatrix& matrixA;
    const BlockMatrix& matrixB;
    BlockMatrix& matrixResult;
    int i;
    int j;
    int k;
    ThreadParams(const BlockMatrix& matrixA,
        const BlockMatrix& matrixB, BlockMatrix& matrixResult,
        int i,
        int j, int k): matrixA(matrixA), matrixB(matrixB),
        matrixResult(matrixResult), i(i), j(j), k(k) {}
};

