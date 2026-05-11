#ifndef HEAP_H
#define HEAP_H

#include "State.h"

class Heap {
private:
    State** arreglo;       // Arreglo de punteros a estados
    int tamano;            // Número actual de elementos
    int capacidad;         // Capacidad máxima

    void flotar(int indice);
    void hundir(int indice);
    void redimensionar();

public:
    // Sugerencia: Usar explícitamente el tamaño inicial
    Heap(int capacidadInicial = 10000); 
    ~Heap();

    void push(State* s);
    State* pop();
    
    // Sugerencia: marcar como const si no modifica el heap
    bool estaVacio() const; 
    
    // Opcional: Para debugging
    int getTamano() const { return tamano; }
};

#endif