#ifndef STATE_H
#define STATE_H

#include "Tablero.h"
#include <iostream>

class State {
private:
    Tablero* t;            // El estado del mundo (Tablero)
    State* padre;          // Puntero al estado anterior
    int g;                 // Costo real (pasos dados)
    int h;                 // Estimación (heurística)
    int f;                 // g + h
    char* operacion;       // Descripción del movimiento (ej: "B0 Arriba")

public:
    // Constructor que recibe el tablero, costos, padre y descripción
    State(Tablero* tablero, int g_val, int h_val, State* p, const char* op = nullptr);
    ~State();

    // Getters rápidos
    Tablero* getTablero() const { return t; }
    State* getPadre() const { return padre; }
    int getG() const { return g; }
    int getF() const { return f; }

    // Métodos de utilidad (estilo profesor)
    void printOperaciones(); // Imprime el camino desde el inicio usando recursión
};

#endif