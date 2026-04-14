#include "TablaHash.h"
#include <cstring> // Para strcmp y strcpy

// Constructor
TablaHash::TablaHash(int cap) : capacidad(cap) {
    tabla = new NodoHash*[capacidad];
    for (int i = 0; i < capacidad; i++) {
        tabla[i] = nullptr;
    }
}

// Destructor
TablaHash::~TablaHash() {
    for (int i = 0; i < capacidad; i++) {
        NodoHash* actual = tabla[i];
        while (actual != nullptr) {
            NodoHash* temp = actual;
            actual = actual->siguiente;
            delete[] temp->representacion;
            delete temp;
        }
    }
    delete[] tabla;
}

// Función hash simple basada en la representación (OPTIMIZADA)
unsigned int TablaHash::funcionHash(const char* rep) {
    unsigned int hash = 0;
    for (size_t i = 0; rep[i] != '\0'; i++) {
        hash = hash * 31 + rep[i];
    }
    return hash % capacidad;
}

// Insertar un tablero en la tabla (OPTIMIZADA: Una sola llamada a getRepresentacion())
void TablaHash::insertar(const Tablero& t) {
    const char* rep = t.getRepresentacion();  // ← UNA SOLA LLAMADA aquí
    unsigned int indice = funcionHash(rep);   // ← Pasar const char* en lugar de Tablero

    // Verificar si ya existe
    NodoHash* actual = tabla[indice];
    while (actual != nullptr) {
        if (strcmp(actual->representacion, rep) == 0) {  // ← Reutilizar 'rep'
            return; // Ya existe
        }
        actual = actual->siguiente;
    }

    // Insertar al inicio de la lista
    NodoHash* nuevo = new NodoHash;
    nuevo->representacion = new char[strlen(rep) + 1];
    strcpy(nuevo->representacion, rep);  // ← Reutilizar 'rep'
    nuevo->siguiente = tabla[indice];
    tabla[indice] = nuevo;
}

// Verificar si un tablero existe en la tabla (OPTIMIZADA: Una sola llamada a getRepresentacion())
bool TablaHash::existe(const Tablero& t) {
    const char* rep = t.getRepresentacion();  // ← UNA SOLA LLAMADA aquí
    unsigned int indice = funcionHash(rep);   // ← Pasar const char* en lugar de Tablero

    NodoHash* actual = tabla[indice];
    while (actual != nullptr) {
        if (strcmp(actual->representacion, rep) == 0) {  // ← Reutilizar 'rep'
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}