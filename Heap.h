#ifndef HEAP_H
#define HEAP_H

#include "State.h"

class Heap {
private:
    State** arreglo;       // Arreglo de punteros a estados
    int tamano;            // Número actual de elementos en el montículo
    int capacidad;         // Capacidad máxima actual

    void flotar(int indice);    // Reorganizar hacia arriba (heapifyUp)
    void hundir(int indice);    // Reorganizar hacia abajo (heapifyDown)
    void redimensionar();       // Duplicar el espacio del arreglo

public:
    Heap(int capacidadInicial = 10000); // Constructor
    ~Heap();                            // Destructor

    void push(State* s);        // Insertar un nuevo estado
    State* pop();               // Extraer el estado con menor f
    bool estaVacio() const;     // Verificar si no hay elementos
};

#endif