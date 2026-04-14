#ifndef STATE_H
#define STATE_H

#include "Tablero.h"

struct State {
    Tablero* t;         // Estado físico del juego
    State* padre;       // Para reconstruir el camino al final
    
    int g;              // Costo acumulado (distancia recorrida)
    int h;              // Heurística (estimado al final)
    int f;              // f = g + h

    // Información del movimiento que generó este estado
    int bloqueID;
    char direccion;
    int celdas;

    State(Tablero* tablero, int g_val, int h_val, State* p = nullptr);
    ~State(); // Al destruir el State, se destruye su Tablero
};

#endif