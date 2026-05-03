#ifndef MOVIMIENTO_H
#define MOVIMIENTO_H

#include "State.h"
#include "Tablero.h"


class Movimiento {
public:
    /**
     * Determina si un movimiento es posible sin alterar el estado ni el tablero.
     */
    static bool esValido(int bloqueIdx, Direccion dir, State* estadoActual, Tablero* tableroEstatico);

    /**
     * Ejecuta un movimiento válido y devuelve el nuevo estado resultante.
     */
    static State* ejecutar(int bloqueIdx, Direccion dir, State* actual, Tablero* tableroEstatico);

};

#endif