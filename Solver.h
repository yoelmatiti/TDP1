#ifndef SOLVER_H
#define SOLVER_H

#include "State.h"
#include "COLAPRIORIDAD.h"
#include "TablaHash.h"

class Solver {
private:
    ColaPrioridad open;    // Nodos por explorar
    TablaHash closed;      // Tableros ya visitados
    
    // Configuración estática (Background)
    char** paredes; 
    int ancho, alto;

    void generarSucesores(State* actual);
    int calcularHeuristica(Tablero* t);

public:
    Solver();
    ~Solver();
    void cargarProblema(const char* archivo);
    void resolver();
};

#endif