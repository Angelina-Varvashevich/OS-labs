#include "BlockMatrix.h"

CRITICAL_SECTION section;

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
    int j, std::vector<HANDLE>& threadsVector) {
    for (int k = 0; k < matrixB.getColumn(); k++) {
        ThreadParams* params = new ThreadParams(matrixA, matrixB, matrixResult, i, j, k);
        HANDLE thread = CreateThread(NULL, 0, &currentMultiplyBlock, params, 0, NULL);
        threadsVector.push_back(thread); 
    }
}

DWORD WINAPI BlockMatrix::currentMultiplyBlock(LPVOID lpParams){
    ThreadParams params = *((ThreadParams*)lpParams);
    EnterCriticalSection(&section);
    Matrix currentBlock = (params.matrixResult.getMatrix(params.i, params.j) + params.matrixA.getMatrix(params.i, params.k) * params.matrixB.getMatrix(params.k, params.j));
    params.matrixResult.setMatrix(currentBlock, params.i, params.j);
    LeaveCriticalSection(&section);
    delete (ThreadParams*)lpParams;
    return 0;
}

void BlockMatrix::multiplyMatrices(const BlockMatrix& matrixA, const BlockMatrix& matrixB, BlockMatrix& matrixResult) {
    InitializeCriticalSection(&section);
    std::vector<HANDLE> threadsVector;
    for (int i = 0; i < matrixResult.getRow(); i++) {
        for (int j = 0; j < matrixResult.getColumn(); j++) {
            multiplyBlock(matrixA, matrixB, matrixResult, i, j, threadsVector);
        }
    }

    for (int i = 0; i < threadsVector.size(); i++) {
        WaitForSingleObject(threadsVector[i], INFINITE);
    }
    for (int i = 0; i < threadsVector.size(); i++) {
        CloseHandle(threadsVector[i]);
    }
    DeleteCriticalSection(&section);
}

Matrix BlockMatrix::multiply(const Matrix& a, const Matrix& b, int blockSize) {
    BlockMatrix A(a, blockSize);
    BlockMatrix B(b, blockSize);
    Matrix result(a.getRow(), a.getColumn());
    BlockMatrix Result(result, blockSize);
    multiplyMatrices(A, B, Result);
    return Result.toMatrix(result.getRow(), result.getColumn());
}

Matrix BlockMatrix::toMatrix(int m, int n) const {
    Matrix result(m, n);
    int blockSize = (std::max)(getMatrix(0, 0).getRow(), getMatrix(0, 0).getColumn());
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
