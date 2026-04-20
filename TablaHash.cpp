#include "TablaHash.h"
#include <cstring>

// Constructor: Reserva espacio para las "cubetas" (buckets)
TablaHash::TablaHash(int cap) : capacidad(cap) {
    tabla = new NodoHash*[capacidad];
    for (int i = 0; i < capacidad; i++) {
        tabla[i] = nullptr;
    }
}

// Destructor: Limpieza profunda de cadenas y nodos
TablaHash::~TablaHash() {
    for (int i = 0; i < capacidad; i++) {
        NodoHash* actual = tabla[i];
        while (actual != nullptr) {
            NodoHash* temporal = actual;
            actual = actual->siguiente;
            delete[] temporal->representacion; 
            delete temporal;
        }
    }
    delete[] tabla;
}

/**
 * OPTIMIZACIÓN: Algoritmo DJB2
 * Es significativamente más rápido y produce menos colisiones que el 
 * multiplicador simple de 31 para cadenas de texto largas.
 */
unsigned int TablaHash::generarHash(const char* cadena) const {
    unsigned long hash = 5381;
    int c;
    while ((c = *cadena++)) {
        // hash * 33 + c
        hash = ((hash << 5) + hash) + c;
    }
    return hash % capacidad;
}

/**
 * OPTIMIZACIÓN: Inserción sin redundancia
 * No volvemos a calcular el hash dentro de insertar si ya lo calculamos 
 * para verificar existencia (aunque aquí se mantienen separadas por claridad).
 */
void TablaHash::insertar(const char* repr) {
    if (repr == nullptr) return;

    unsigned int indice = generarHash(repr);

    // Verificamos si ya existe para evitar duplicados en la misma cubeta
    NodoHash* actual = tabla[indice];
    while (actual != nullptr) {
        if (std::strcmp(actual->representacion, repr) == 0) {
            return; 
        }
        actual = actual->siguiente;
    }

    // Insertar al inicio de la lista (O(1))
    NodoHash* nuevo = new NodoHash;
    nuevo->representacion = new char[std::strlen(repr) + 1];
    std::strcpy(nuevo->representacion, repr);
    nuevo->siguiente = tabla[indice];
    tabla[indice] = nuevo;
}

// Búsqueda rápida
bool TablaHash::existe(const char* repr) const {
    if (repr == nullptr) return false;

    unsigned int indice = generarHash(repr);

    NodoHash* actual = tabla[indice];
    while (actual != nullptr) {
        // strcmp es rápido, pero las colisiones bajas lo hacen más rápido aún
        if (std::strcmp(actual->representacion, repr) == 0) {
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}