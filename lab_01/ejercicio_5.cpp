#include "./memoriaCache.h"

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
    int N = 256;
    vector<vector<double>> M1 = crearMatrizAleatoria(N);
    vector<vector<double>> M2 = crearMatrizAleatoria(N);
    vector<vector<double>> ProdM1M2(N, vector<double>(N, 0.0));

    multiplicarMatrices(M1, M2, ProdM1M2, N);

    fill(ProdM1M2.begin(), ProdM1M2.end(), vector<double>(N, 0.0));

    int blockSize = 32;
    multiplicarPorBloques(M1, M2, ProdM1M2, N, blockSize);

    return 0;
}