#include "memoriaCache.h"

void multiplicarMatrices(
    const vector<vector<double>>& A,
    const vector<vector<double>>& B,
    vector<vector<double>>& C,
    const int MAX) {
    
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            for (int k = 0; k < MAX; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }            
        }     
    }
}

void multiplicarPorBloques(
    const vector<vector<double>>& A, const vector<vector<double>>& B, 
    vector<vector<double>>& C, int N, int blockSize) {

    for (int ii = 0; ii < N; ii += blockSize) {
        for (int jj = 0; jj < N; jj += blockSize) {
            for (int kk = 0; kk < N; kk += blockSize) {
                /* Multiplicar por bloques */
                for (int i = ii; i < min(ii + blockSize, N); i++) {
                    for (int j = jj; j < min(jj + blockSize, N); j++) {
                        int sum = 0;
                        for (int k = kk; k < min(kk + blockSize, N); k++) {
                            sum += A[i][k] * B[k][j];
                        }
                        C[i][j] += sum;
                    }
                }
            }
        }
    }
}

int main() {
    int N = 1024;

    int doubleSize = sizeof(double);
    cout << "doubleSize = " << doubleSize << '\n';
    
    for (int i = 128; i <= 1024; i += 128) {
        N = i;
        vector<vector<double>> M1 = crearMatrizAleatoria(N);
        vector<vector<double>> M2 = crearMatrizAleatoria(N);
        vector<vector<double>> ProdM1M2(N, vector<double>(N, 0.0));

        auto start = chrono::high_resolution_clock::now();
        multiplicarMatrices(M1, M2, ProdM1M2, N);
        auto end = chrono::high_resolution_clock::now();

        chrono::duration<double, milli> duracion = end - start;

        fill(ProdM1M2.begin(), ProdM1M2.end(), vector<double>(N, 0.0));

        /* Multiplicar por bloques */
        int blockSize = 32;
        auto start1 = chrono::high_resolution_clock::now();
        multiplicarPorBloques(M1, M2, ProdM1M2, N, blockSize);
        auto end1 = chrono::high_resolution_clock::now();

        chrono::duration<double, milli> duracion1 = end1 - start1;
        cout << "N = " << N << '\n'
            << "T_clasico = " << duracion.count()
            << '\n' << "T_porBloques = " << duracion1.count()
            << "ms" << '\n';        
    }
    
    return 0;
}