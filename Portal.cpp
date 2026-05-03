#include "Portal.h"

Portal::Portal() : x(0), y(0), orientacion('V'), cicloColores(nullptr), numColores(0), pasoCambio(0) {}

Portal::Portal(int _x, int _y, int _p, char _ori) 
    : x(_x), y(_y), orientacion(_ori), cicloColores(nullptr), numColores(0), pasoCambio(_p) {}

// Copia profunda
Portal::Portal(const Portal& otro) : cicloColores(nullptr) {
    *this = otro;
}

Portal& Portal::operator=(const Portal& otro) {
    if (this != &otro) {
        delete[] cicloColores;
        x = otro.x;
        y = otro.y;
        orientacion = otro.orientacion;
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
 * Lógica modular para determinar el color/estado en el tiempo g.
 */
char Portal::getColorActual(int tiempoG) const {
    if (numColores <= 0 || pasoCambio <= 0) 
        return (numColores > 0) ? cicloColores[0] : ' ';
    return cicloColores[(tiempoG / pasoCambio) % numColores];
}

/**
 * REGLA: Lógica de Paso (Color y Estado Pared)
 * Verifica si el bloque puede interactuar con la compuerta.
 */
bool Portal::puedePasar(char colorBloque, int tiempoG) const {
    char colorPort = getColorActual(tiempoG);
    
    // Si el ciclo actual es '#', funciona como pared infranqueable
    if (colorPort == '#') return false;

    // El bloque pasa si el color coincide o si es el comodín '?'
    return (colorBloque == colorPort || colorBloque == '?');
}

/**
 * REGLA: Verificación de Tamaño y Orientación
 * Verifica si las dimensiones del bloque son compatibles con la apertura de la compuerta.
 */
bool Portal::verificarDimension(int anchoBloque, int altoBloque) const {
    if (orientacion == 'H') {
        // En una compuerta horizontal, el flujo depende del ancho
        return anchoBloque == 1; // Ejemplo: solo pasan bloques de ancho 1
    } else if (orientacion == 'V') {
        // En una compuerta vertical, el flujo depende del alto
        return altoBloque == 1; // Ejemplo: solo pasan bloques de alto 1
    }
    return false;
}

void Portal::agregarColorAlCiclo(char c) {
    char* nuevoCiclo = new char[numColores + 1];
    for (int i = 0; i < numColores; i++) nuevoCiclo[i] = cicloColores[i];
    nuevoCiclo[numColores] = c;
    
    delete[] cicloColores;
    cicloColores = nuevoCiclo;
    numColores++;
}

// Getters básicos
int Portal::getX() const { return x; }
int Portal::getY() const { return y; }
char Portal::getOrientacion() const { return orientacion; }
char Portal::getColor() const { return getColorActual(0);
}