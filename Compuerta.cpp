#include "Compuerta.h"

#include <cctype>

Compuerta::Compuerta() : x(0), y(0), orientacion('V'), cicloColores(nullptr), numColores(0), pasoCambio(0) {}

Compuerta::Compuerta(int _x, int _y, int _p, char _ori) 
    : x(_x), y(_y), orientacion(_ori), cicloColores(nullptr), numColores(0), pasoCambio(_p) {}

// Copia profunda
Compuerta::Compuerta(const Compuerta& otro) : cicloColores(nullptr) {
    *this = otro;
}

Compuerta& Compuerta::operator=(const Compuerta& otro) {
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

Compuerta::~Compuerta() {
    delete[] cicloColores;
}


/**
 * Lógica modular para determinar el color/estado en el tiempo g.
 */
char Compuerta::getColorActual(int tiempoG) const {
    if (numColores <= 0 || pasoCambio <= 0) 
        return (numColores > 0) ? cicloColores[0] : ' ';
    return cicloColores[(tiempoG / pasoCambio) % numColores];
}

/**
 * REGLA: Verificación de Tamaño y Orientación
 * Verifica si las dimensiones del bloque son compatibles con la apertura de la compuerta.
 */
bool Compuerta::verificarDimension(int anchoBloque, int altoBloque) const {
    if (orientacion == 'H') {
        // En una compuerta horizontal, el flujo depende del ancho
        return anchoBloque == 1; // Ejemplo: solo pasan bloques de ancho 1
    } else if (orientacion == 'V') {
        // En una compuerta vertical, el flujo depende del alto
        return altoBloque == 1; // Ejemplo: solo pasan bloques de alto 1
    }
    return false;
}

void Compuerta::agregarColorAlCiclo(char c) {
    char* nuevoCiclo = new char[numColores + 1];
    for (int i = 0; i < numColores; i++) nuevoCiclo[i] = cicloColores[i];
    nuevoCiclo[numColores] = c;
    
    delete[] cicloColores;
    cicloColores = nuevoCiclo;
    numColores++;
}

bool Compuerta::puedePasar(char colorBloque, int paso) const {
    if (colorBloque == '?') return true; // Comodín siempre pasa

    char colorCompuerta = getColorActual(paso);
    
    // Convertimos ambos a minúsculas antes de comparar
    return std::tolower((unsigned char)colorBloque) == std::tolower((unsigned char)colorCompuerta);
}

// Getters básicos
int Compuerta::getX() const { return x; }
int Compuerta::getY() const { return y; }
char Compuerta::getOrientacion() const { return orientacion; }
char Compuerta::getColor() const { return getColorActual(0); }

/**
 * Calcula la posición de destino al atravesar la compuerta.
 * El bloque debe quedar justo al otro lado de la pared.
 */
void Compuerta::calcularDestino(int bloqueX, int bloqueY, int anchoB, int altoB, Direccion dir, int& outX, int& outY) const {
    outX = bloqueX;
    outY = bloqueY;

    // Dependiendo de la dirección del movimiento, el bloque "salta" su propio tamaño
    // más el ancho de la pared (1 celda) para aparecer al otro lado.
    if (dir == Direccion::U) outY -= (altoB + 1);
    else if (dir == Direccion::D) outY += (altoB + 1);
    else if (dir == Direccion::L) outX -= (anchoB + 1);
    else if (dir == Direccion::R) outX += (anchoB + 1);
}