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

/**
 * OPTIMIZACIÓN: Cálculo de color O(1)
 * No usa bucles, solo aritmética modular para determinar el estado
 * del portal en cualquier punto del tiempo (g).
 */
char Portal::getColorActual(int tiempoG) const {
    if (numColores <= 0 || pasoCambio <= 0) return (numColores > 0) ? cicloColores[0] : ' ';
    // Aritmética modular O(1)
    return cicloColores[(tiempoG / pasoCambio) % numColores];
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

/**
 * COMPATIBILIDAD: Para el dibujo inicial o consultas fuera del Solver.
 */
char Portal::getColor() const {
    return getColorActual(0);
}

bool Portal::puedePasar(char colorBloque, int tiempoG) const {
    char colorPortal = getColorActual(tiempoG);
    // Un bloque pasa si es del mismo color o si es un comodín '?'
    return (colorBloque == colorPortal || colorBloque == '?');
}