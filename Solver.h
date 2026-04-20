#ifndef SOLVER_H
#define SOLVER_H

#include "Tablero.h"
#include "State.h"
#include "Heap.h"      // Tu Cola de Prioridad (o ColaPrioridad.h)
#include "TablaHash.h"

class Solver {
private:
    Heap open;            // Lista de estados por explorar (Frontera)
    TablaHash closed;     // Lista de estados ya visitados (Historial)
    
    // Variables de configuración del tablero actual
    int ancho, alto;
    char** paredes;

public:
    Solver();
    ~Solver();

    // Método principal: Retorna el State final o nullptr si falla
    State* solve(Tablero* inicial);

    // Genera los posibles movimientos desde un estado actual
    void generarSucesores(State* actual);

    // Calcula la distancia estimada a la meta (Heurística)
    int calcularHeuristica(Tablero* t);
};

#endif