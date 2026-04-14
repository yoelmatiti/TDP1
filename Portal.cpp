#include "Portal.h"

Portal::Portal() : x(0), y(0), cicloColores(nullptr), numColores(0), pasoCambio(0) {}

Portal::Portal(int _x, int _y, int _p) 
    : x(_x), y(_y), cicloColores(nullptr), numColores(0), pasoCambio(_p) {}

// Copia profunda del ciclo de colores
Portal::Portal(const Portal& otro) : cicloColores(nullptr) {
    *this = otro;
}

Portal& Portal::operator=(const Portal& otro) {
    if (this != &otro) {
        delete[] cicloColores;
        x = otro.x;
        y = otro.y;
        pasoCambio = otro.pasoCambio;
        numColores = otro.numColores;
        
        if (otro.cicloColores) {
            cicloColores = new char[numColores];
            for (int i = 0; i < numColores; i++) cicloColores[i] = otro.cicloColores[i];
        } else {
            cicloColores = nullptr;
        }
    }
    return *this;
}

Portal::~Portal() {
    delete[] cicloColores;
}

void Portal::agregarColorAlCiclo(char c) {
    // Redimensionar arreglo manual (estilo Vector)
    char* nuevoCiclo = new char[numColores + 1];
    for (int i = 0; i < numColores; i++) nuevoCiclo[i] = cicloColores[i];
    nuevoCiclo[numColores] = c;
    
    delete[] cicloColores;
    cicloColores = nuevoCiclo;
    numColores++;
}

char Portal::getColorActual(int tiempoG) const {
    if (numColores == 0) return ' ';
    if (pasoCambio <= 0) return cicloColores[0];

    // Lógica: (Tiempo total / p) nos da el índice del ciclo
    int indice = (tiempoG / pasoCambio) % numColores;
    return cicloColores[indice];
}

bool Portal::puedePasar(char colorBloque, int tiempoG) const {
    // Un bloque solo pasa si su color coincide con el color del portal en ese instante
    return colorBloque == getColorActual(tiempoG);
}