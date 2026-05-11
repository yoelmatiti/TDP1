#include "Salida.h"

// Constructor por defecto
Salida::Salida() : color(' '), x(0), y(0), direccionX(0), direccionY(0), 
                    longitudInicial(0), longitudFinal(0), pasoCambio(0) {
}

// Constructor completo de 8 parámetros
Salida::Salida(char color, int x, int y, int direccionX, int direccionY, 
               int longitudInicial, int longitudFinal, int pasoCambio) {
    this->color = color;
    this->x = x;
    this->y = y;
    this->direccionX = direccionX;
    this->direccionY = direccionY;
    this->longitudInicial = longitudInicial;
    this->longitudFinal = longitudFinal;
    this->pasoCambio = pasoCambio;
}

// Alternativamente, usando lista de inicialización (más eficiente):
/*
Salida::Salida(char _color, int _x, int _y, int _dX, int _dY, 
               int _lI, int _lF, int _pC) 
    : color(_color), x(_x), y(_y), direccionX(_dX), direccionY(_dY),
      longitudInicial(_lI), longitudFinal(_lF), pasoCambio(_pC) {
}
*/

// Constructor de Copia (Aunque Salida no tiene memoria dinámica, cumplimos Regla de los Tres)
Salida::Salida(const Salida& otro) 
    : color(otro.color), x(otro.x), y(otro.y), direccionX(otro.direccionX), 
      direccionY(otro.direccionY), longitudInicial(otro.longitudInicial), 
      longitudFinal(otro.longitudFinal), pasoCambio(otro.pasoCambio) {}

// Operador de Asignación
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

int Salida::getLongitudActual(int tiempoG) const {
    if (pasoCambio <= 0) return longitudInicial;
    
    // Matemática modular: alterna entre Inicial y Final cada 'pasoCambio'
    return ((tiempoG / pasoCambio) % 2 == 0) ? longitudInicial : longitudFinal;
}

/**
 * OPTIMIZACIÓN MAESTRA:
 * En lugar de un bucle FOR, verificamos si el punto (f,c) está en la línea
 * usando la distancia relativa y la dirección.
 */
bool Salida::esParteDeSalida(int f, int c, int tiempoG) const {
    int L = getLongitudActual(tiempoG);
    if (L <= 0) return false;

    int diffX = c - x;
    int diffY = f - y;

    // Caso 1: Salida Horizontal (direccionX != 0, direccionY == 0)
    if (direccionX != 0 && direccionY == 0) {
        if (diffY != 0) return false; // No está en la misma fila
        int pasoK = diffX / direccionX;
        return (pasoK >= 0 && pasoK < L && (diffX % direccionX == 0));
    }
    // Caso 2: Salida Vertical (direccionX == 0, direccionY != 0)
    else if (direccionY != 0 && direccionX == 0) {
        if (diffX != 0) return false; // No está en la misma columna
        int pasoK = diffY / direccionY;
        return (pasoK >= 0 && pasoK < L && (diffY % direccionY == 0));
    }
    // Caso 3: Salida de punto único (ambas direcciones son 0)
    else if (direccionX == 0 && direccionY == 0) {
        return (diffX == 0 && diffY == 0);
    }

    return false;
}

// En Salida.h / Salida.cpp
bool Salida::bloqueCubreSalida(int anchoB, int altoB) const {
    // Si la salida es vertical (direccionX == 0), el alto del bloque 
    // debe ser igual a la longitud de la salida para "taparla" o pasar por ella.
    // Esto depende de cómo interpretes "coincidir en dimensiones" del PDF.
    
    if (direccionX != 0) return altoB == 1; // Salida horizontal, bloque debe tener alto 1
    if (direccionY != 0) return anchoB == 1; // Salida vertical, bloque debe tener ancho 1
    return true;
}