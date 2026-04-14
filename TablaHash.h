#ifndef TABLAHASH_H
#define TABLAHASH_H

#include "Tablero.h"

// Nodo para manejar colisiones en la tabla hash (Lista enlazada simple)
struct NodoHash {
    char* representacion; // El "string" o ID único del tablero
    NodoHash* siguiente;
};

class TablaHash {
private:
    NodoHash** tabla;
    int capacidad;
    
    // Convierte la representación del tablero en un índice del arreglo
    unsigned int funcionHash(const char* rep);  // ← CAMBIO: ahora recibe const char*

public:
    TablaHash(int cap = 100003); // Usar números primos mejora la distribución
    ~TablaHash();

    void insertar(const Tablero& t);
    bool existe(const Tablero& t);
};

#endif