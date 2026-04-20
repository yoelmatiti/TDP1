#include "Salida.h"

// ... Constructores se mantienen igual, no tienen memoria dinámica ...
// Constructor por defecto
Salida::Salida() : color(' '), x(0), y(0), direccionX(0), direccionY(0), 
                   longitudInicial(0), longitudFinal(0), pasoCambio(0) {
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

    // 2. Verificar si el punto está alineado con la dirección de la salida
    // Si direccionX es 1, diffX debe ser positivo. Si es 0, diffX debe ser 0.
    
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
    // El punto debe estar en la dirección correcta y no exceder la longitud actual L
    return (pasoK >= 0 && pasoK < L && 
            diffX == pasoK * direccionX && 
            diffY == pasoK * direccionY);
}