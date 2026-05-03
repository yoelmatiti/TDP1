#include "Heap.h"
#include <iostream>

Heap::Heap(int capacidadInicial) : tamano(0), capacidad(capacidadInicial) {
    arreglo = new State*[capacidad];
}

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
    if (!nuevoEstado) return;
    if (tamano == capacidad) redimensionar();
    
    arreglo[tamano] = nuevoEstado;
    flotar(tamano);
    tamano++;
}

/**
 * pop() - Extrae el mínimo con validación de seguridad
 */
State* Heap::pop() {
    while (tamano > 0) {
        State* raiz = arreglo[0];
        tamano--;

        if (tamano > 0) {
            arreglo[0] = arreglo[tamano];
            hundir(0);
        }

        // --- VALIDACIÓN DE SEGURIDAD ---
        // Si el puntero es nulo o apunta a memoria corrupta (f negativo o absurdo)
        // lo ignoramos y sacamos el siguiente mejor.
        if (raiz == nullptr || raiz->getF() < 0 || raiz->getF() > 1000000) {
            // Log opcional para detectar si esto sucede durante el desarrollo

            continue; 
        }

        return raiz;
    }
    return nullptr;
}

void Heap::flotar(int indice) {
    while (indice > 0) {
        int padre = (indice - 1) / 2;
        
        // Prioridad: Menor F, luego mayor G (desempate hacia adelante)
        bool prioridad = (arreglo[indice]->getF() < arreglo[padre]->getF()) || 
                        (arreglo[indice]->getF() == arreglo[padre]->getF() && 
                         arreglo[indice]->getG() > arreglo[padre]->getG());

        if (!prioridad) break;

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

        if (hijoIzquierdo < tamano) {
            if (arreglo[hijoIzquierdo]->getF() < arreglo[menor]->getF() || 
               (arreglo[hijoIzquierdo]->getF() == arreglo[menor]->getF() && 
                arreglo[hijoIzquierdo]->getG() > arreglo[menor]->getG())) {
                menor = hijoIzquierdo;
            }
        }

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
            break;
        }
    }
}