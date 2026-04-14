#include "AlgoritmoASTAR.h"
#include <cstring>
#include <cstdlib>
#include <climits>

AlgoritmoASTAR::AlgoritmoASTAR() 
    : abierta(nullptr), cerrada(nullptr), solucion(nullptr), 
      nodoGenerados(0), nodoExpandidos(0),
      todosLosNodos(nullptr), capacidadNodos(10000), cantidadNodos(0) {
    abierta = new ColaPrioridad(100003);
    cerrada = new TablaHash(100003);
    todosLosNodos = new NodoASTAR*[capacidadNodos];
}

AlgoritmoASTAR::~AlgoritmoASTAR() {
    // NO limpiar si hay solución - será responsabilidad del main
    if (!solucion) {
        limpiar();
    }
    if (abierta) delete abierta;
    if (cerrada) delete cerrada;
}

int AlgoritmoASTAR::calcularHeuristica(const Tablero& t) {
    int heuristica = 0;
    
    for (int i = 0; i < t.getNumBloques(); i++) {
        int xBloque = t.getBloques()[i].getX();
        int yBloque = t.getBloques()[i].getY();
        
        int distMin = INT_MAX;
        for (int j = 0; j < t.getNumSalidas(); j++) {
            if (t.getSalidas()[j].getColor() == t.getBloques()[i].getColor()) {
                int xSalida = t.getSalidas()[j].getX();
                int ySalida = t.getSalidas()[j].getY();
                int dist = abs(xBloque - xSalida) + abs(yBloque - ySalida);
                if (dist < distMin) distMin = dist;
            }
        }
        if (distMin != INT_MAX) heuristica += distMin;
    }
    
    return heuristica;
}

void AlgoritmoASTAR::generarVecinos(NodoASTAR* nodoPadre) {
    Direccion direcciones[] = { Direccion::U, Direccion::D, Direccion::L, Direccion::R };
    
    for (int bloqueID = 0; bloqueID < nodoPadre->tablero->getNumBloques(); bloqueID++) {
        for (int dir = 0; dir < 4; dir++) {
            for (int celdas = 1; celdas <= 10; celdas++) {
                Tablero* nuevoTablero = new Tablero(*nodoPadre->tablero);
                
                if (nuevoTablero->moverBloque(bloqueID, direcciones[dir], celdas)) {
                    if (cerrada->existe(*nuevoTablero)) {
                        delete nuevoTablero;
                        continue;
                    }
                    
                    cerrada->insertar(*nuevoTablero);
                    
                    int nuevoG = nodoPadre->g + celdas;
                    int nuevoH = calcularHeuristica(*nuevoTablero);
                    NodoASTAR* nuevoNodo = new NodoASTAR(nuevoTablero, nuevoG, nuevoH, nodoPadre);
                    
                    if (cantidadNodos < capacidadNodos) {
                        todosLosNodos[cantidadNodos] = nuevoNodo;
                        cantidadNodos++;
                    }
                    
                    nodoGenerados++;
                    abierta->push(nuevoNodo);
                } else {
                    delete nuevoTablero;
                    break;
                }
            }
        }
    }
}

NodoASTAR* AlgoritmoASTAR::reconstruirCamino(NodoASTAR* nodoFinal) {
    return nodoFinal;
}

bool AlgoritmoASTAR::esAncestroDe(NodoASTAR* posibleAncestro, NodoASTAR* nodo) {
    while (nodo != nullptr) {
        if (nodo == posibleAncestro) return true;
        nodo = nodo->padre;
    }
    return false;
}

void AlgoritmoASTAR::limpiar() {
    // Liberar todos los nodos EXCEPTO solución y sus ancestros
    for (int i = 0; i < cantidadNodos; i++) {
        if (todosLosNodos[i]) {
            if (todosLosNodos[i] != solucion && !esAncestroDe(todosLosNodos[i], solucion)) {
                delete todosLosNodos[i];
            }
        }
    }
    
    if (todosLosNodos) {
        delete[] todosLosNodos;
        todosLosNodos = nullptr;
    }
    
    cantidadNodos = 0;
}

bool AlgoritmoASTAR::resolver(Tablero* estadoInicial, int maxNodos) {
    if (!estadoInicial) return false;
    
    int hInicial = calcularHeuristica(*estadoInicial);
    NodoASTAR* nodoInicial = new NodoASTAR(estadoInicial, 0, hInicial, nullptr);
    nodoGenerados = 1;
    nodoExpandidos = 0;
    
    if (cantidadNodos < capacidadNodos) {
        todosLosNodos[cantidadNodos] = nodoInicial;
        cantidadNodos++;
    }
    
    abierta->push(nodoInicial);
    cerrada->insertar(*estadoInicial);
    
    while (!abierta->estaVacia() && nodoGenerados < maxNodos) {
        NodoASTAR* actual = abierta->pop();
        if (!actual) break;
        
        nodoExpandidos++;
        
        if (actual->tablero->esEstadoFinal()) {
            solucion = reconstruirCamino(actual);
            return true;
        }
        
        generarVecinos(actual);
    }
    
    return false;
}