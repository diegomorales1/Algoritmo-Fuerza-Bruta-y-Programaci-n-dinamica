#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <climits>
#include <chrono> 

#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace std::chrono;

// Declaración de matrices de costo
int** matriz_cost_insert;
int** matriz_cost_delete;
int** matriz_cost_replace;
int** matriz_cost_transpose;

// Función para cargar las matrices desde un archivo
int** cargar_matriz(const string& nombre_archivo) {
    ifstream archivo(nombre_archivo);
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo: " << nombre_archivo << endl;
        exit(1);
    }

    // Crear una matriz de 26x26
    int** matriz = new int*[26];
    for (int i = 0; i < 26; ++i) {
        matriz[i] = new int[26];
    }

    // Leer los valores del archivo
    for (int i = 0; i < 26; ++i) {
        for (int j = 0; j < 26; ++j) {
            archivo >> matriz[i][j];
        }
    }

    archivo.close();
    return matriz;
}

char abecedario[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

// Función que devuelve el índice de una letra en el abecedario
int obtener_indice(char letra) {
    for (int i = 0; i < 26; ++i) {
        if (abecedario[i] == letra) {
            return i; // Si encuentra la letra, devuelve su índice
        }
    }
    return 0;
}

int costo_ins(char b) {
    int fila = obtener_indice(b); // Convertimos el caracter en un índice válido
    return matriz_cost_insert[fila][0];  // Costos de inserción están en la primera columna
}

int costo_del(char a) {
    int fila = obtener_indice(a); // Convertimos el caracter en un índice válido
    return matriz_cost_delete[fila][0];  // Costos de eliminación están en la primera columna
}

int costo_sub(char a, char b) {
    int fila = obtener_indice(a);  // Convertimos 'a' en un índice válido
    int columna = obtener_indice(b);  // Convertimos 'b' en un índice válido
    return matriz_cost_replace[fila][columna];  // Costos de sustitución
}

int costo_trans(char a, char b) {
    int fila = obtener_indice(a);  // Convertimos 'a' en un índice válido
    int columna = obtener_indice(b);  // Convertimos 'b' en un índice válido
    return matriz_cost_transpose[fila][columna];  // Costos de transposición
}

// Fuerza Bruta Recursiva
int FuerzaBruta(const string &S1, const string &S2, int i, int j) {
    if (i == 0) return j * costo_ins(S2[j - 1]);
    if (j == 0) return i * costo_del(S1[i - 1]);

    int costo = INT_MAX;

    costo = min(costo, FuerzaBruta(S1, S2, i - 1, j - 1) + costo_sub(S1[i - 1], S2[j - 1]));
    costo = min(costo, FuerzaBruta(S1, S2, i, j - 1) + costo_ins(S2[j - 1]));
    costo = min(costo, FuerzaBruta(S1, S2, i - 1, j) + costo_del(S1[i - 1]));

    if (i > 1 && j > 1 && S1[i - 1] == S2[j - 2] && S1[i - 2] == S2[j - 1]) {
        costo = min(costo, FuerzaBruta(S1, S2, i - 2, j - 2) + costo_trans(S1[i - 1], S1[i - 2]));
    }

    return costo;
}

// Programación Dinámica
int distanciaMinimaDinamica(const string &S1, const string &S2) {
    int n = S1.size();
    int m = S2.size();

    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

    for (int i = 0; i <= n; ++i) dp[i][0] = i * costo_del(S1[i - 1]);
    for (int j = 0; j <= m; ++j) dp[0][j] = j * costo_ins(S2[j - 1]);

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            dp[i][j] = dp[i - 1][j - 1] + costo_sub(S1[i - 1], S2[j - 1]);
            dp[i][j] = min(dp[i][j], dp[i][j - 1] + costo_ins(S2[j - 1]));
            dp[i][j] = min(dp[i][j], dp[i - 1][j] + costo_del(S1[i - 1]));

            if (i > 1 && j > 1 && S1[i - 1] == S2[j - 2] && S1[i - 2] == S2[j - 1]) {
                dp[i][j] = min(dp[i][j], dp[i - 2][j - 2] + costo_trans(S1[i - 1], S1[i - 2]));
            }
        }
    }

    return dp[n][m];
}

int main() {
    matriz_cost_insert = cargar_matriz("cost_insert.txt");
    matriz_cost_delete = cargar_matriz("cost_delete.txt");
    matriz_cost_replace = cargar_matriz("cost_replace.txt");
    matriz_cost_transpose = cargar_matriz("cost_transpose.txt");
    string S1, S2;

    ifstream file1("s1.txt");
    if (!file1) {
        cerr << "Error: No se pudo abrir el archivo s1.txt" << endl;
        return 1;
    }
    getline(file1, S1);
    file1.close();

    ifstream file2("s2.txt");
    if (!file2) {
        cerr << "Error: No se pudo abrir el archivo s2.txt" << endl;
        return 1;
    }
    getline(file2, S2);
    file2.close();

    ofstream resultados("resultados.txt");
    if (!resultados) {
        cerr << "Error: No se pudo crear el archivo resultados.txt" << endl;
        return 1;
    }

    // Aqui se calcula el tiempo para fuerza Bruta
    auto inicio = high_resolution_clock::now();
    int resultadoFB = FuerzaBruta(S1, S2, S1.size(), S2.size());
    auto fin = high_resolution_clock::now();
    auto duracionFB = duration_cast<nanoseconds>(fin - inicio).count();
    double duracion_ms1 = duracionFB / 1e6;
    resultados << "Fuerza Bruta:\n";
    resultados << "Costo de las operaciones: " << resultadoFB << "\n";
    resultados << "Tiempo: " << duracion_ms1 << " ns\n\n";

    // el siguiente apartado calcula el tiempo para programacion dinamica
    inicio = high_resolution_clock::now();
    int resultadoDP = distanciaMinimaDinamica(S1, S2);
    fin = high_resolution_clock::now();
    auto duracionDP = duration_cast<nanoseconds>(fin - inicio).count();
    double duracion_ms2 = duracionDP / 1e6;
    resultados << "Programación Dinámica:\n";
    resultados << "Costo de las operaciones: " << resultadoDP << "\n";
    resultados << "Tiempo: " << duracion_ms2 << " ns\n";

    resultados.close();
    cout << "Resultados guardados en resultados.txt" << endl;

    return 0;
}
