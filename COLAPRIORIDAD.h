#ifndef COLAPRIORIDAD_H
#define COLAPRIORIDAD_H

#include "NodoASTAR.h"

class ColaPrioridad {
private:
    NodoASTAR** heap;   // Arreglo de punteros a nodos
    int capacidad;
    int tamano;

    void subir(int idx);   // Reheap up
    void bajar(int idx);   // Reheap down
    void redimensionar();  // Si se llena el arreglo

public:
    ColaPrioridad(int capInitial = 1000);
    ~ColaPrioridad();

    void push(NodoASTAR* nodo);
    NodoASTAR* pop();      // Extrae el de menor F
    bool estaVacia() const { return tamano == 0; }
};

#endif