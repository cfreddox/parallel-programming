#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

using namespace std;

vector<vector<int>> ReadMatrix(int &n, int &m, const string filename)
{

    ifstream file(filename);

    if (!file.is_open())
    {
        cout << "couldn't open the file" << filename << "\n";
        exit(1);
    }

    file >> n;
    file >> m;

    vector<vector<int>> matrix(n, vector<int>(m));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            file >> matrix[i][j];
        }
    }

    file.close();
    return matrix;
}

vector<vector<int>> MultiplyMatrix(int n, int m, const vector<vector<int>> A, const vector<vector<int>> B)
{

    vector<vector<int>> res(n, vector<int>(m));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            for (int k = 0; k < n; k++)
            {
                res[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return res;
}

void WriteMatrix(string filename, int n, int m, const vector<vector<int>> A, chrono::milliseconds t, bool cor)
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
            file << A[i][j];

            if (j < n - 1)
                file << " ";
        }
        file << "\n";
    }
    file << "Time: " << t;
    file << "\nCorrectly: " << cor;
    file.close();
}

int main()
{
    int nA, mA, nB, mB;
    vector<vector<int>> A = ReadMatrix(nA, mA, "matrixA.txt");
    vector<vector<int>> B = ReadMatrix(nB, mB, "matrixB.txt");

    if (nB != mA)
    {
        cout << "it is impossible to multiply matrices";
        return 1;
    }

    auto start = chrono::high_resolution_clock::now();
    vector<vector<int>> res = MultiplyMatrix(nA, mB, A, B);
    auto finish = chrono::high_resolution_clock::now();

    auto time = chrono::duration_cast<chrono::milliseconds>(finish - start);

    WriteMatrix("MatrixAB.txt", nA, mB, res, time, 1);
}