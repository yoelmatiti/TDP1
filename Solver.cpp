#include "Solver.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cmath>

Solver::Solver()
    : open(10000), closed(100003), paredes(nullptr), ancho(0), alto(0), tableroInicial(nullptr) {
}

Solver::~Solver() {
    // Limpieza de paredes (si las usas fuera del Tablero)
    if (paredes != nullptr) {
        for (int i = 0; i < alto; i++) delete[] paredes[i];
        delete[] paredes;
    }
    // Ojo: el tableroInicial debería borrarse si no se borra en otro lado
    delete tableroInicial;
}

// Implementación del ciclo A*
void Solver::resolver() {
    if (!tableroInicial) return;

    // 1. Crear nodo inicial
    int h0 = calcularHeuristica(tableroInicial);
    NodoASTAR* inicial = new NodoASTAR(tableroInicial, 0, h0, nullptr, "Inicio");
    open.push(inicial);

    while (!open.estaVacia()) {
        NodoASTAR* actual = open.pop();

        // 2. ¿Es meta?
        if (actual->getTablero()->esEstadoFinal()) {
            std::cout << "¡SOLUCIÓN ENCONTRADA!" << std::endl;
            actual->imprimirCamino(); // Debes tener este método en NodoASTAR
            return; 
        }

        // 3. Cerrar estado (Tabla Hash) para evitar ciclos
        const char* repr = actual->getTablero()->getRepresentacion();
        if (closed.existe(repr)) {
            delete actual; 
            continue;
        }
        closed.insertar(repr);

        // 4. Expandir sucesores
        generarSucesores(actual);
    }
    std::cout << "No se encontro solucion." << std::endl;
}

void Solver::generarSucesores(NodoASTAR* actual) {
    Tablero* tPadre = actual->getTablero();
    
    // Intentar mover cada bloque en las 4 direcciones
    Direccion direcciones[] = {Direccion::U, Direccion::D, Direccion::L, Direccion::R};
    const char* nombres[] = {"Arriba", "Abajo", "Izquierda", "Derecha"};

    for (int i = 0; i < tPadre->getNumBloques(); i++) {
        if (!tPadre->getBloque(i).estaActivo()) continue;

        for (int d = 0; d < 4; d++) {
            Tablero* copiaTablero = new Tablero(*tPadre); // Constructor de copia profundo
            
            // Simular deslizamiento: mover hasta chocar (distancia grande)
            int distReal = copiaTablero->moverBloque(i, direcciones[d], 100);

            if (distReal > 0) {
                // Verificar si este nuevo estado ya lo vimos (opcional aquí para ahorrar memoria antes de crear el nodo)
                if (!closed.existe(copiaTablero->getRepresentacion())) {
                    int h = calcularHeuristica(copiaTablero);
                    int g = actual->getG() + 1; // Costo por movimiento (no por celda)
                    
                    char desc[50];
                    std::sprintf(desc, "Bloque %d -> %s", i, nombres[d]);
                    
                    NodoASTAR* hijo = new NodoASTAR(copiaTablero, g, h, actual, desc);
                    open.push(hijo);
                } else {
                    delete copiaTablero;
                }
            } else {
                delete copiaTablero;
            }
        }
    }
}

int Solver::calcularHeuristica(Tablero* t) {
    int h = 0;
    // Distancia Manhattan de cada bloque a su salida más cercana
    for (int i = 0; i < t->getNumBloques(); i++) {
        const Bloque& b = t->getBloque(i);
        if (!b.estaActivo()) continue;

        int minDist = 1e6;
        for (int j = 0; j < t->getNumSalidas(); j++) {
            const Salida& s = t->getSalida(j);
            if (s.getColor() == b.getColor() || b.getEsIncognito()) {
                int d = std::abs(b.getX() - s.getX()) + std::abs(b.getY() - s.getY());
                if (d < minDist) minDist = d;
            }
        }
        h += (minDist == 1e6) ? 0 : minDist;
    }
    return h;
}