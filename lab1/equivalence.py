import numpy as np
import sys

def main(): 
    matrixA=np.loadtxt("matrixA.txt", skiprows=1)
    matrixB=np.loadtxt("matrixB.txt", skiprows=1)
    matrixAB=np.loadtxt("tmp.txt", skiprows=1)
    matrixRes=np.dot(matrixA,matrixB)
    if np.array_equal(matrixRes, matrixAB):
        return 0
    return 1
    
    
if __name__ == "__main__":
    result=main()
    sys.exit(result)