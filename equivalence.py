import numpy as np
import sys


def main(mat: str):
    matrixA = np.loadtxt(mat, skiprows=1, dtype=np.int64)
    matrixB = np.loadtxt(mat, skiprows=1, dtype=np.int64)
    matrixAB = np.loadtxt("tmp.txt", skiprows=1, dtype=np.int64)
    matrixRes = np.dot(matrixA, matrixB)
    if np.array_equal(matrixRes, matrixAB):
        return 0
    return 1


if __name__ == "__main__":
    result = main(sys.argv[1])
    sys.exit(result)
