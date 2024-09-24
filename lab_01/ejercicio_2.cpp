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

int main() {
    int N = 0;
    for (int i = 128; i <= 1024; i += 128) {
        N = i;
        vector<vector<double>> M1 = crearMatrizAleatoria(N);
        vector<vector<double>> M2 = crearMatrizAleatoria(N);
        vector<vector<double>> ProdM1M2(N, vector<double>(N, 0.0));

        auto start = chrono::high_resolution_clock::now();
        multiplicarMatrices(M1, M2, ProdM1M2, N);
        auto end = chrono::high_resolution_clock::now();

        chrono::duration<double, milli> duracion = end - start;
        cout << "N = " << N << " " << duracion.count() << "ms" << '\n';
    }

    return 0;
}


