#include "./memoriaCache.h"

int main() {
    int MAX = 1024;
    vector<vector<double>> A = move(crearMatrizAleatoria(MAX));
    vector<double> x = move(crearArregloAleatorio(MAX));
    vector<double> y(MAX, 0.0);

    auto start = chrono::high_resolution_clock::now();
    /* Primer par de loops */
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            y[i] += A[i][j] * x[j];
        }        
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duracion = end - start;
    cout << "Tiempo 1: " << duracion.count() << "ms" << '\n';

    fill(y.begin(), y.end(), 0.0);

    start = chrono::high_resolution_clock::now();
    /* Segundo par de loops */
    for (int j = 0; j < MAX; j++) {
        for (int i = 0; i < MAX; i++) {
            y[i] += A[i][j] * x[j];
        }        
    }
    end = chrono::high_resolution_clock::now();
    duracion = end - start;
    cout << "Tiempo 2: " << duracion.count() << "ms" << '\n';

    return 0;
}