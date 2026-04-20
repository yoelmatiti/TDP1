#include "Heap.h"

// Inicialización del montículo con la capacidad dada
Heap::Heap(int capacidadInicial) : tamano(0), capacidad(capacidadInicial) {
    arreglo = new State*[capacidad]; // Reserva de memoria para punteros
}

// Liberación del contenedor de punteros
Heap::~Heap() {
    delete[] arreglo; 
}

bool Heap::estaVacio() const {
    return tamano == 0;
}

void Heap::redimensionar() {
    capacidad *= 2;
    State** nuevoArreglo = new State*[capacidad];
    for (int i = 0; i < tamano; i++) {
        nuevoArreglo[i] = arreglo[i];
    }
    delete[] arreglo;
    arreglo = nuevoArreglo;
}

void Heap::push(State* nuevoEstado) {
    if (tamano == capacidad) redimensionar();
    
    arreglo[tamano] = nuevoEstado; // Se coloca al final
    flotar(tamano);               // Se sube hasta su posición correcta
    tamano++;
}

State* Heap::pop() {
    if (tamano == 0) return nullptr;

    State* raiz = arreglo[0];    // El de menor F (prioridad)
    tamano--;

    if (tamano > 0) {
        arreglo[0] = arreglo[tamano]; // El último pasa a la raíz
        hundir(0);                    // Se baja hasta su posición correcta
    }
    return raiz;
}

void Heap::flotar(int indice) {
    while (indice > 0) {
        int padre = (indice - 1) / 2;
        
        // Criterio de orden: Menor F es prioridad.
        // Desempate: A igual F, preferimos el que tenga mayor G (más pasos).
        bool prioridad = (arreglo[indice]->getF() < arreglo[padre]->getF()) || 
                        (arreglo[indice]->getF() == arreglo[padre]->getF() && 
                         arreglo[indice]->getG() > arreglo[padre]->getG());

        if (!prioridad) break;

        // Intercambio de punteros
        State* temporal = arreglo[indice];
        arreglo[indice] = arreglo[padre];
        arreglo[padre] = temporal;
        indice = padre;
    }
}

void Heap::hundir(int indice) {
    while (true) {
        int hijoIzquierdo = 2 * indice + 1;
        int hijoDerecho = 2 * indice + 2;
        int menor = indice;

        // Evaluar hijo izquierdo
        if (hijoIzquierdo < tamano) {
            if (arreglo[hijoIzquierdo]->getF() < arreglo[menor]->getF() || 
               (arreglo[hijoIzquierdo]->getF() == arreglo[menor]->getF() && 
                arreglo[hijoIzquierdo]->getG() > arreglo[menor]->getG())) {
                menor = hijoIzquierdo;
            }
        }

        // Evaluar hijo derecho
        if (hijoDerecho < tamano) {
            if (arreglo[hijoDerecho]->getF() < arreglo[menor]->getF() || 
               (arreglo[hijoDerecho]->getF() == arreglo[menor]->getF() && 
                arreglo[hijoDerecho]->getG() > arreglo[menor]->getG())) {
                menor = hijoDerecho;
            }
        }

        if (menor != indice) {
            State* temporal = arreglo[indice];
            arreglo[indice] = arreglo[menor];
            arreglo[menor] = temporal;
            indice = menor;
        } else {
            break; // Ya está en la posición correcta
        }
    }
}