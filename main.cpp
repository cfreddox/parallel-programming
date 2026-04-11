#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <mpi.h>

using namespace std;

vector<long long> ReadMatrix(int &n, int &m, const string filename)
{

    ifstream file(filename);

    if (!file.is_open())
    {
        cout << "couldn't open the file" << filename << "\n";
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    file >> n;
    file >> m;

    vector<long long> matrix(n * m);

    for (int i = 0; i < n * m; i++)
    {
        file >> matrix[i];
    }

    file.close();
    return matrix;
}

void WriteResult(string filename, int n, int m, chrono::milliseconds t, bool cor)
{

    ofstream file(filename, ios::app);

    if (!file.is_open())
    {
        cout << "couldn't open the file" << filename << "\n";
        return;
    }

    file << n << " " << m << " Time: " << t.count() << " Correctly: " << cor << "\n";
    file.close();
}

void WriteMatrix(string filename, int n, int m, const vector<long long> &A)
{

    ofstream file(filename);

    if (!file.is_open())
    {
        cout << "couldn't open the file" << filename << "\n";
        return;
    }

    file << n << " " << m << "\n";

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            file << A[i * m + j];

            if (j < m - 1)
                file << " ";
        }
        file << "\n";
    }

    file.close();
}

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    vector<string> matrix = {"matrix100.txt", "matrix200.txt", "matrix400.txt", "matrix800.txt", "matrix1200.txt", "matrix1600.txt", "matrix2000.txt"};

    if (rank == 0)
    {
        ofstream clear("result.txt", ios::trunc);
    }

    for (const string &mat : matrix)
    {

        int nA = 0,
            mA = 0, nB = 0, mB = 0;
        vector<long long> A, B;

        if (rank == 0)
        {
            A = ReadMatrix(nA, mA, mat);
            B = ReadMatrix(nB, mB, mat);
            if (nB != mA)
            {
                cout << "it is impossible to multiply matrices";
                MPI_Abort(MPI_COMM_WORLD, 1);
            }
        }

        MPI_Bcast(&nA, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&mA, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&mB, 1, MPI_INT, 0, MPI_COMM_WORLD);

        if (rank != 0)
            B.resize(mA * mB);

        MPI_Barrier(MPI_COMM_WORLD);
        double t_start = MPI_Wtime();

        MPI_Bcast(B.data(), mA * mB, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
        if (rank == 0)
            B.clear();

        int base_rows = nA / size;
        int rem_rows = nA % size;

        vector<int> send_cnt(size), displs(size);
        int cur = 0;

        for (int i = 0; i < size; i++)
        {
            int rows = base_rows + (i < rem_rows ? 1 : 0);
            send_cnt[i] = rows * mA;
            displs[i] = cur;
            cur += send_cnt[i];
        }

        int my_rows = base_rows + (rank < rem_rows ? 1 : 0);
        vector<long long> A_local(my_rows * mA);

        MPI_Scatterv(rank == 0 ? A.data() : nullptr, send_cnt.data(), displs.data(), MPI_LONG_LONG, A_local.data(), my_rows * mA, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

        if (rank == 0)
            A.clear();

        vector<long long> AB_local(my_rows * mB, 0);
        for (int i = 0; i < my_rows; i++)
        {
            for (int j = 0; j < mA; j++)
            {
                long long val = A_local[i * mA + j];
                for (int k = 0; k < mB; k++)
                {
                    AB_local[i * mB + k] += val * B[j * mB + k];
                }
            }
        }

        vector<int> AB_cnt(size), AB_displs(size);
        cur = 0;

        for (int i = 0; i < size; i++)
        {
            int rows = base_rows + (i < rem_rows ? 1 : 0);
            AB_cnt[i] = rows * mB;
            AB_displs[i] = cur;
            cur += AB_cnt[i];
        }

        vector<long long> AB_global;
        if (rank == 0)
            AB_global.resize(nA * mB);
        MPI_Gatherv(AB_local.data(), my_rows * mB, MPI_LONG_LONG,
                    rank == 0 ? AB_global.data() : nullptr, AB_cnt.data(), AB_displs.data(), MPI_LONG_LONG,
                    0, MPI_COMM_WORLD);

        MPI_Barrier(MPI_COMM_WORLD);
        double t_end = MPI_Wtime();
        if (rank == 0)
        {
            auto time_ms = chrono::milliseconds((int)((t_end - t_start) * 1000));
            WriteMatrix("tmp.txt", nA, mB, AB_global);
            bool cor = (system(("python3 equivalence.py " + mat).c_str()) == 0);
            WriteResult("result.txt", nA, mB, time_ms, cor);
        }
    }
    MPI_Finalize();
}