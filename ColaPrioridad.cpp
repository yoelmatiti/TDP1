#include "COLAPRIORIDAD.h"

// Constructor
ColaPrioridad::ColaPrioridad(int capInitial) : capacidad(capInitial), tamano(0) {
    heap = new NodoASTAR*[capacidad];
}

// Destructor (CORREGIDO: Libera todos los nodos restantes)
ColaPrioridad::~ColaPrioridad() {
    // Liberar cada nodo en el arreglo antes de liberar el arreglo
    for (int i = 0; i < tamano; i++) {
        delete heap[i];
    }
    delete[] heap;
}

// Subir un elemento en el heap (para mantener la propiedad de min-heap por f)
void ColaPrioridad::subir(int idx) {
    while (idx > 0) {
        int padre = (idx - 1) / 2;
        if (heap[idx]->f >= heap[padre]->f) break;
        // Intercambiar
        NodoASTAR* temp = heap[idx];
        heap[idx] = heap[padre];
        heap[padre] = temp;
        idx = padre;
    }
}

// Bajar un elemento en el heap
void ColaPrioridad::bajar(int idx) {
    if (tamano <= 1) return;

    int hijoIzq = 2 * idx + 1;
    int hijoDer = 2 * idx + 2;
    int menor = idx;

    if (hijoIzq < tamano && heap[hijoIzq]->f < heap[menor]->f) {
        menor = hijoIzq;
    }
    if (hijoDer < tamano && heap[hijoDer]->f < heap[menor]->f) {
        menor = hijoDer;
    }
    if (menor != idx) {
        // Intercambiar
        NodoASTAR* temp = heap[idx];
        heap[idx] = heap[menor];
        heap[menor] = temp;
        bajar(menor);
    }
}

// Redimensionar el arreglo si se llena
void ColaPrioridad::redimensionar() {
    capacidad *= 2;
    NodoASTAR** nuevoHeap = new NodoASTAR*[capacidad];
    for (int i = 0; i < tamano; i++) {
        nuevoHeap[i] = heap[i];
    }
    delete[] heap;
    heap = nuevoHeap;
}

// Insertar un nodo
void ColaPrioridad::push(NodoASTAR* nodo) {
    if (tamano == capacidad) {
        redimensionar();
    }
    heap[tamano] = nodo;
    subir(tamano);
    tamano++;
}

// Extraer el nodo con menor f (CORREGIDO: Maneja caso tamano == 1)
NodoASTAR* ColaPrioridad::pop() {
    if (tamano == 0) return nullptr;
    
    NodoASTAR* raiz = heap[0];
    tamano--;
    
    // Solo necesita reheapificar si hay más de un elemento
    if (tamano > 0) {
        heap[0] = heap[tamano];
        bajar(0);
    }
    
    return raiz;
}