#include "Solver.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

Solver::Solver()
    : open(1000), closed(100003), paredes(nullptr), ancho(0), alto(0) {
}

Solver::~Solver() {
    if (paredes != nullptr) {
        for (int i = 0; i < alto; i++) {
            delete[] paredes[i];
        }
        delete[] paredes;
        paredes = nullptr;
    }
}

void Solver::cargarProblema(const char* archivo) {
    // Lectura básica de archivo, el formato debe definirse según el problema.
    FILE* f = std::fopen(archivo, "r");
    if (!f) {
        return;
    }

    // Formato de ejemplo: ancho alto
    if (std::fscanf(f, "%d %d", &ancho, &alto) == 2) {
        paredes = new char*[alto];
        for (int i = 0; i < alto; i++) {
            paredes[i] = new char[ancho];
            for (int j = 0; j < ancho; j++) {
                int c = std::fgetc(f);
                if (c == EOF) c = ' ';
                if (c == '\n') {
                    j--;
                    continue;
                }
                paredes[i][j] = static_cast<char>(c);
            }
        }
    }

    std::fclose(f);
}

void Solver::resolver() {
    // Método de alto nivel para iniciar la búsqueda
    // Actualmente solo existe como esqueleto.
}

void Solver::generarSucesores(State* actual) {
    // Genera sucesores a partir del estado actual.
    // Debe implementar la lógica de movimiento usando el tablero y las reglas.
    (void)actual;
}

int Solver::calcularHeuristica(Tablero* t) {
    // Heurística simple por defecto: 0.
    // Debe reemplazarse con una heurística admisible.
    (void)t;
    return 0;
}
