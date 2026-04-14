#include "Salida.h"

Salida::Salida() : color(' '), x(0), y(0), direccionX(0), direccionY(0), 
                   longitudInicial(0), longitudFinal(0), pasoCambio(0) {}

Salida::Salida(char c, int _x, int _y, int dx, int dy, int li, int lf, int p)
    : color(c), x(_x), y(_y), direccionX(dx), direccionY(dy), 
      longitudInicial(li), longitudFinal(lf), pasoCambio(p) {}

// Constructor de copia
Salida::Salida(const Salida& otro) {
    *this = otro;
}

// Operador de asignación
Salida& Salida::operator=(const Salida& otro) {
    if (this != &otro) {
        color = otro.color;
        x = otro.x;
        y = otro.y;
        direccionX = otro.direccionX;
        direccionY = otro.direccionY;
        longitudInicial = otro.longitudInicial;
        longitudFinal = otro.longitudFinal;
        pasoCambio = otro.pasoCambio;
    }
    return *this;
}

// Destructor
Salida::~Salida() {
    // No hay memoria dinámica
}

int Salida::getLongitudActual(int tiempoG) const {
    // Si el paso de cambio es 0 o el tiempo es menor al paso, es longitud inicial
    if (pasoCambio <= 0) return longitudInicial;
    
    // Determinar en qué ciclo estamos
    // (tiempoG / pasoCambio) % 2 == 0 -> Longitud Inicial
    // (tiempoG / pasoCambio) % 2 == 1 -> Longitud Final
    if ((tiempoG / pasoCambio) % 2 == 0) {
        return longitudInicial;
    } else {
        return longitudFinal;
    }
}

bool Salida::esParteDeSalida(int f, int c, int tiempoG) const {
    int L = getLongitudActual(tiempoG);
    
    // Verificar si la celda (f, c) está en la línea de la salida
    for (int i = 0; i < L; i++) {
        int currX = x + (i * direccionX);
        int currY = y + (i * direccionY);
        if (currX == c && currY == f) return true;
    }
    return false;
}