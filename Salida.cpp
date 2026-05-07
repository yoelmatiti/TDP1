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

    // 1. Calcular distancia relativa desde el origen de la salida
    int diffX = c - x;
    int diffY = f - y;

    int pasoK; // Representa cuántos pasos hay desde el origen (x,y)
    
    if (direccionX != 0) {
        if (diffY != 0 && direccionY == 0) return false; // Desalineado en Y
        pasoK = diffX / direccionX;
    } else if (direccionY != 0) {
        if (diffX != 0) return false; // Desalineado en X
        pasoK = diffY / direccionY;
    } else {
        // Salida de un solo punto
        return (diffX == 0 && diffY == 0);
    }

    // 3. Verificar si el paso K es válido (está dentro de la línea y de la longitud L)
    return (pasoK >= 0 && pasoK < L && 
            diffX == pasoK * direccionX && 
            diffY == pasoK * direccionY);
}