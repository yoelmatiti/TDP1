#ifndef STATE_H
#define STATE_H

#include <iostream>
#include <cstring>

/**
 * Estructura ligera para representar la ubicación y estado de un bloque.
 */
struct PosBloque {
    int x, y;
    bool activo;
};

class State {
private:
    // --- ATRIBUTOS (Ordenados para evitar -Wreorder) ---
    PosBloque* posiciones;  // Arreglo dinámico de posiciones
    int numBloques;         // Tamaño del arreglo
    int g;                  // Costo del camino (pasos dados)
    int h;                  // Heurística (distancia estimada a meta)
    int f;                  // g + h (prioridad para A*)
    State* padre;           // Puntero al estado predecesor (árbol de búsqueda)
    char* operacion;        // Descripción del movimiento

public:
    // --- CICLO DE VIDA (Regla de los Tres / Copy-and-Swap) ---
    
    /** Constructor principal */
    State(int nBloques, PosBloque* posNuevas, int g_val, int h_val, State* p, const char* op);
    
    /** Constructor de copia (Deep Copy) */
    State(const State& otro);
    
    /** * Operador de asignación corregido.
     * Recibe por valor para facilitar el idioma copy-and-swap.
     */
    State& operator=(State otro); 

    /** Intercambia el contenido de dos estados de forma segura. */
    void swap(State& primero, State& segundo) noexcept;

    /** Destructor: libera posiciones y operacion. */
    ~State();

    // --- LÓGICA DE COMPARACIÓN Y HASH ---

    /** Genera una firma numérica única basada en primos para evitar colisiones. */
    long long getHash() const;

    /** Compara si dos estados tienen la misma configuración física. */
    bool operator==(const State& other) const;

    // --- GETTERS (Inline para rendimiento en bucles críticos) ---

    inline int getG() const                 { return g; }
    inline int getH() const                 { return h; }
    void setH(int valorH) { this->h = valorH; }
    inline int getF() const                 { return f; }
    inline int getNumBloques() const        { return numBloques; }
    inline State* getPadre() const          { return padre; }
    inline PosBloque* getPosiciones() const { return posiciones; }
    inline const char* getOperacion() const { return operacion; }

    // --- UTILIDADES ---

    /** Imprime recursivamente la ruta de solución. */
    void printOperaciones();
    bool estaOcupado(int x, int y, int bloqueIgnorado) const;
};

#endif