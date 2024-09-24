#include <iostream>
#include <vector>
#include <random>
#include <chrono>
using namespace std;

vector<double> crearArregloAleatorio(const int MAX) {
    vector<double> arreglo(MAX);

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);

    for (auto &a : arreglo) {
        a = dis(gen);
    }

    return move(arreglo);
}

vector<vector<double>> crearMatrizAleatoria(const int MAX) {
    vector<vector<double>> matriz(MAX, vector<double>(MAX));

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);

    for (auto &vec : matriz) {
        for (auto &i : vec) {
            i = dis(gen);
        }
    }

    return move(matriz);
}