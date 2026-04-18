#include <cuda_runtime.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <cstdlib>

using namespace std;

#define CUDA_CHECK(call)                                                     \
    do                                                                       \
    {                                                                        \
        cudaError_t error = call;                                            \
        if (error != cudaSuccess)                                            \
        {                                                                    \
            cerr << "CUDA error at " << __FILE__ << ":" << __LINE__ << " - " \
                 << cudaGetErrorString(error);                               \
            exit(1);                                                         \
        }                                                                    \
    } while (0)

int *readMatrix(const string &filename, int &N, int &M)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: Cannot open file " << filename;
        return nullptr;
    }

    file >> N >> M;
    int *matrix = new int[N * M];
    for (int i = 0; i < N * M; i++)
    {
        file >> matrix[i];
    }

    file.close();
    return matrix;
}

void WriteResult(const string &filename, int n, int m, long long t_mc_s, bool cor)
{

    ofstream file(filename, ios::app);

    if (!file.is_open())
    {
        cerr << "Cannot open file: " << filename;
        return;
    }
    double t_ms = static_cast<double>(t_mc_s) / 1000.0;

    file << n << " " << m << " Time: " << t_ms << " Correctly: " << cor << "\n";
    file.close();
}

void WriteMatrix(const string &filename, int n, int m, const int *matrix)
{

    ofstream file(filename);

    if (!file.is_open())
    {
        cerr << "Error: Cannot open file " << filename;
        return;
    }

    file << n << " " << m << "\n";

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            file << matrix[i * m + j];

            if (j < m - 1)
                file << " ";
        }
        file << "\n";
    }

    file.close();
}

__global__ void MatrixMultiply(const int *A, const int *B, int *C, int M, int N, int K)
{
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    int row = blockIdx.y * blockDim.y + threadIdx.y;

    if (col < N && row < M)
    {
        int sum = 0;
        for (int i = 0; i < K; i++)
        {
            sum += A[row * K + i] * B[col + N * i];
        }
        C[row * N + col] = sum;
    }
}

void multiplyMatricesCUDA(int *A, int *B, int *C, int M, int N, int K, int block_x, int bloc_y)
{

    int *d_A, *d_B, *d_C;
    CUDA_CHECK(cudaMalloc(&d_A, M * K * sizeof(int)));
    CUDA_CHECK(cudaMalloc(&d_B, K * N * sizeof(int)));
    CUDA_CHECK(cudaMalloc(&d_C, M * N * sizeof(int)));

    CUDA_CHECK(cudaMemcpy(d_A, A, M * K * sizeof(int), cudaMemcpyHostToDevice));
    CUDA_CHECK(cudaMemcpy(d_B, B, K * N * sizeof(int), cudaMemcpyHostToDevice));

    dim3 threadsPerBlock(block_x, bloc_y);
    dim3 blocksPerGrid((N + block_x - 1) / block_x, (M + bloc_y - 1) / bloc_y);

    MatrixMultiply<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C, M, N, K);

    CUDA_CHECK(cudaGetLastError());
    CUDA_CHECK(cudaDeviceSynchronize());

    CUDA_CHECK(cudaMemcpy(C, d_C, M * N * sizeof(int), cudaMemcpyDeviceToHost));

    CUDA_CHECK(cudaFree(d_A));
    CUDA_CHECK(cudaFree(d_B));
    CUDA_CHECK(cudaFree(d_C));
}

int main()
{
    vector<string> matrix = {"matrix100.txt", "matrix200.txt", "matrix400.txt", "matrix800.txt", "matrix1200.txt", "matrix1600.txt", "matrix2000.txt"};
    ofstream clear("result.txt", ios::trunc);

    for (const string &mat : matrix)
    {
        int nA = 0,
            mA = 0, nB = 0, mB = 0;
        int *A = readMatrix(mat, nA, mA);
        int *B = readMatrix(mat, nB, mB);

        if (mA != nB)
        {
            cout << "Cannot multiply: A columns (" << mA << ") != B rows (" << nB << ")";
        }

        vector<int> C(nA * mB);

        auto t_start = chrono::high_resolution_clock::now();
        multiplyMatricesCUDA(A, B, C.data(), nA, mA, mB, 24, 24);
        auto t_end = chrono::high_resolution_clock::now();

        long long total_time = chrono::duration_cast<chrono::microseconds>(t_end - t_start).count();
        WriteMatrix("tmp.txt", nA, mB, C.data());
        bool cor = (system(("python3 equivalence.py " + mat).c_str()) == 0);
        WriteResult("result.txt", nA, mB, total_time, cor);
        delete[] A;
        delete[] B;
    }
    return 0;
}