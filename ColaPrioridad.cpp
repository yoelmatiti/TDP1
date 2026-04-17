#include "COLAPRIORIDAD.h"

// Constructor: Inicializa el arreglo de punteros
ColaPrioridad::ColaPrioridad(int capInitial) : capacidad(capInitial), tamano(0) {
    heap = new NodoASTAR*[capacidad];
}

// Destructor: Solo libera el contenedor (arreglo de punteros)
// Los NodoASTAR deben ser liberados por el Solver/TablaHash para evitar Double Free
ColaPrioridad::~ColaPrioridad() {
    delete[] heap;
}

// Implementación del desempate (Optimización sugerida en clase)
// Prioriza menor F. Si F es igual, prioriza mayor G.
void ColaPrioridad::subir(int idx) {
    while (idx > 0) {
        int padre = (idx - 1) / 2;
        
        bool debeSubir = false;
        if (heap[idx]->f < heap[padre]->f) {
            debeSubir = true;
        } else if (heap[idx]->f == heap[padre]->f && heap[idx]->g > heap[padre]->g) {
            debeSubir = true; // Desempate por movimiento más largo
        }

        if (!debeSubir) break;

        // Intercambio manual
        NodoASTAR* temp = heap[idx];
        heap[idx] = heap[padre];
        heap[padre] = temp;
        idx = padre;
    }
}

// Versión iterativa de Bajar (Más eficiente que la recursiva)
void ColaPrioridad::bajar(int idx) {
    while (true) {
        int hijoIzq = 2 * idx + 1;
        int hijoDer = 2 * idx + 2;
        int menor = idx;

        // Verificar hijo izquierdo
        if (hijoIzq < tamano) {
            if (heap[hijoIzq]->f < heap[menor]->f || 
               (heap[hijoIzq]->f == heap[menor]->f && heap[hijoIzq]->g > heap[menor]->g)) {
                menor = hijoIzq;
            }
        }

        // Verificar hijo derecho
        if (hijoDer < tamano) {
            if (heap[hijoDer]->f < heap[menor]->f || 
               (heap[hijoDer]->f == heap[menor]->f && heap[hijoDer]->g > heap[menor]->g)) {
                menor = hijoDer;
            }
        }

        if (menor != idx) {
            NodoASTAR* temp = heap[idx];
            heap[idx] = heap[menor];
            heap[menor] = temp;
            idx = menor;
        } else {
            break;
        }
    }
}

void ColaPrioridad::redimensionar() {
    int nuevaCapacidad = capacidad * 2;
    NodoASTAR** nuevoHeap = new NodoASTAR*[nuevaCapacidad];
    for (int i = 0; i < tamano; i++) {
        nuevoHeap[i] = heap[i];
    }
    delete[] heap;
    heap = nuevoHeap;
    capacidad = nuevaCapacidad;
}

void ColaPrioridad::push(NodoASTAR* nodo) {
    if (tamano == capacidad) {
        redimensionar();
    }
    heap[tamano] = nodo;
    subir(tamano);
    tamano++;
}

NodoASTAR* ColaPrioridad::pop() {
    if (tamano == 0) return nullptr;
    
    NodoASTAR* raiz = heap[0];
    tamano--;
    
    if (tamano > 0) {
        heap[0] = heap[tamano];
        bajar(0);
    }
    
    return raiz;
}