#ifndef SOLVER_H
#define SOLVER_H

#include "Tablero.h"
#include "ColaPrioridad.h"
#include "TablaHash.h"
#include "NodoASTAR.h"

class Solver {
private:
    // Estructuras de búsqueda
    ColaPrioridad open;
    TablaHash closed;
    
    // Datos del problema actual
    Tablero* tableroInicial;
    int ancho, alto;
    char** paredes; // Si decides mantener las paredes aparte

public:
    Solver();
    ~Solver();

    void cargarProblema(const char* archivo);
    void resolver();
    void generarSucesores(NodoASTAR* actual);
    int calcularHeuristica(Tablero* t);
};

#endif