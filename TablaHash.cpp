#include "TablaHash.h"
#include "State.h"

TablaHash::TablaHash(int cap) : capacidad(cap) {
    tabla = new NodoHash*[capacidad];
    for (int i = 0; i < capacidad; i++) {
        tabla[i] = nullptr;
    }
}

/**
 * Destructor: Responsable de la limpieza total de la memoria del Solver.
 * Recorre cada cubeta y elimina tanto el envoltorio (NodoHash) como el 
 * dato pesado (State).
 */
TablaHash::~TablaHash() {
    for (int i = 0; i < capacidad; i++) {
        NodoHash* actual = tabla[i];
        while (actual != nullptr) {
            NodoHash* temporal = actual;
            actual = actual->siguiente;

            // 1. Liberamos el objeto State almacenado.
            // Gracias a que en State::~State() NO borramos al padre, 
            // no habrá recursión infinita ni Double Free aquí.
            if (temporal->estado) {
                delete temporal->estado;
                temporal->estado = nullptr;
            }

            // 2. Liberamos el nodo de la lista ligada.
            delete temporal;
        }
    }
    // 3. Liberamos el arreglo de punteros principal.
    delete[] tabla;
}

/**
 * Genera el índice de la cubeta usando el hash del estado.
 * Aplicamos una máscara de bits para asegurar que el índice sea positivo.
 */
unsigned int TablaHash::obtenerIndice(long long hashEstado) const {
    return (unsigned int)(hashEstado ) % capacidad;
}

/**
 * Inserción de estados únicos.
 */
bool TablaHash::insertar(State* s) {
    if (!s || existe(s)) return false;

    unsigned int indice = obtenerIndice(s->getHash());

    // Insertamos al inicio de la cubeta (O(1))
    NodoHash* nuevo = new NodoHash(s);
    nuevo->siguiente = tabla[indice];
    tabla[indice] = nuevo;
    return true;
}

/**
 * Búsqueda optimizada:
 * Compara por Hash (rápido) y luego por contenido (seguro).
 */
bool TablaHash::existe(State* s) const {
    if (!s) return false;

    unsigned int indice = obtenerIndice(s->getHash());
    NodoHash* actual = tabla[indice];

    while (actual != nullptr) {
        // Primero comparamos el hash numérico
        if (actual->estado->getHash() == s->getHash()) {
            // Si el hash coincide, usamos el operator== (Deep Comparison)
            if (*(actual->estado) == *s) {
                return true;
            }
        }
        actual = actual->siguiente;
    }
    return false;
}