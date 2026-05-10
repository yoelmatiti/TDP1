#ifndef TABLAHASH_H
#define TABLAHASH_H

// Forward declaration para no incluir State.h aquí
class State;

/**
 * Nodo para el encadenamiento de la Tabla Hash (Closed List).
 * Ahora guarda un puntero al estado ligero en lugar de un string pesado.
 */
struct NodoHash {
    State* estado;
    NodoHash* siguiente;

    // Constructor directo
    NodoHash(State* s) : estado(s), siguiente(nullptr) {}
};

class TablaHash {
private:
    NodoHash** tabla;
    int capacidad;
    
    /**
     * Calcula el índice de la tabla a partir del hash numérico del estado.
     * Usa una máscara de bits para asegurar que el índice sea positivo.
     */
    unsigned int obtenerIndice(long long hashEstado) const;

public:
    /**
     * Constructor: Se recomienda un número primo para la capacidad.
     * Ejemplo: 100003, 200003, 500009.
     */
    TablaHash(int cap = 200003); 
    ~TablaHash();

    /**
     * Inserta un puntero al estado en la tabla.
     * La TablaHash asume propiedad del State cuando se inserta en closed.
     */
    bool insertar(State* s); 

    /**
     * Verifica si un estado (por su contenido de posiciones) ya fue visitado.
     * Compara el hash numérico y luego usa el operator== para confirmar.
     */
    bool existe(State* s) const;
};

#endif