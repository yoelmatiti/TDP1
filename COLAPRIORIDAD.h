#ifndef COLAPRIORIDAD_H
#define COLAPRIORIDAD_H

#include "NodoASTAR.h"

class ColaPrioridad {
private:
    NodoASTAR** heap;   // Arreglo dinámico de punteros
    int capacidad;
    int tamano;

    void subir(int idx);   
    void bajar(int idx);   
    void redimensionar();  

public:
    ColaPrioridad(int capInitial = 1000);
    ~ColaPrioridad();

    void push(NodoASTAR* nodo);
    NodoASTAR* pop();      
    
    bool estaVacia() const { return tamano == 0; }
    int getTamano() const { return tamano; }
};

#endif