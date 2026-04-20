#include "Solver.h"
#include <iostream>
#include <cstdio>

Solver::Solver() : open(10000), closed(100003) {}

// Ahora retorna el puntero al estado final (como el profe)
State* Solver::solve(Tablero* inicial) {
    if (!inicial) return nullptr;

    int h0 = calcularHeuristica(inicial);
    State* root = new State(inicial, 0, h0, nullptr, "Inicio");
    open.push(root);

    while (!open.estaVacio()) {
        State* actual = open.pop();

        // 1. Verificación de Meta
        if (actual->getTablero()->esEstadoFinal()) {
            return actual; 
        }

        // 2. Control de Ciclos (Closed List)
        const char* repr = actual->getTablero()->getRepresentacion();
        if (closed.existe(repr)) {
            // Importante: No borrar el tablero aquí si otros nodos lo usan, 
            // pero como cada nodo tiene su copia, borramos el nodo.
            delete actual; 
            continue;
        }
        closed.insertar(repr);

        // 3. Expansión
        generarSucesores(actual);
    }
    return nullptr; 
}

// 1. El destructor (aunque esté vacío por ahora)
Solver::~Solver() {
    // Aquí puedes limpiar memoria si fuera necesario
}

// 2. La función de heurística
int Solver::calcularHeuristica(Tablero* t) {
    // Por ahora, una heurística simple (distancia 0 para probar)
    // Luego puedes implementar Manhattan o algo más pro.
    return 0; 
}

void Solver::generarSucesores(State* actual) {
    Tablero* tPadre = actual->getTablero();
    Direccion dirs[] = {Direccion::U, Direccion::D, Direccion::L, Direccion::R};
    const char* nombres[] = {"Arriba", "Abajo", "Izquierda", "Derecha"};

    for (int i = 0; i < tPadre->getNumBloques(); i++) {
        if (!tPadre->getBloque(i).estaActivo()) continue;

        for (int d = 0; d < 4; d++) {
            // OPTIMIZACIÓN: Solo creamos la copia SI el movimiento es posible
            // (Esto ahorra miles de asignaciones de memoria innecesarias)
            Tablero* copia = new Tablero(*tPadre); 
            int dist = copia->moverBloque(i, dirs[d], 100);

            if (dist > 0) {
                const char* r = copia->getRepresentacion();
                if (!closed.existe(r)) {
                    int h = calcularHeuristica(copia);
                    char desc[64];
                    std::sprintf(desc, "B%d %s", i, nombres[d]);
                    
                    State* hijo = new State(copia, actual->getG() + 1, h, actual, desc);
                    open.push(hijo);
                } else {
                    delete copia; // Ya lo vimos, basura
                }
            } else {
                delete copia; // Movimiento inválido, basura
            }
        }
    }
}