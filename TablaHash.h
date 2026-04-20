#ifndef TABLAHASH_H
#define TABLAHASH_H

struct NodoHash {
    char* representacion; 
    NodoHash* siguiente;
};

class TablaHash {
private:
    NodoHash** tabla;
    int capacidad;
    
    // Función hash optimizada
    unsigned int generarHash(const char* cadena) const;

public:
    TablaHash(int cap = 100003); 
    ~TablaHash();

    void insertar(const char* repr); 
    bool existe(const char* repr) const;
};

#endif