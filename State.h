#ifndef STATE_H
#define STATE_H

/**
 * Estructura para almacenar los datos variables de cada bloque en un estado.
 * El PDF indica que el estado debe representar el sistema de manera unívoca[cite: 15].
 */
struct PosBloque {
    int x, y;
    bool activo;
};

class State {
private:
    int numBloques;
    PosBloque* posiciones; // Arreglo dinámico (Sin STL) [cite: 123]
    
    char* operacion;       // Descripción del movimiento (ej: "R1,1") [cite: 130]
    State* padre;          // Puntero para reconstruir el camino [cite: 31]
    
    float f;               // f(n) = g(n) + h(n) 
    int g;                 // Costo acumulado (pasos/distancia)
    int h;                 // Heurística hacia la meta [cite: 50]

public:
    /**
     * Constructores siguiendo el estilo del profesor.
     */
    State(); // Constructor vacío para inicialización segura.
    
    // Constructor principal para generar nuevos nodos en el grafo[cite: 11, 16].
    State(int nBloques, PosBloque* posNuevas, int g_val, int h_val, State* p, const char* op);
    long long getHash() const;
    // Constructor de copia para asegurar independencia de memoria (Deep Copy)[cite: 232].
    State(const State& otro);
    
    ~State();

    // Regla de los tres: Operador de asignación.
    State& operator=(const State& otro);

    /**
     * Lógica de Estado y A*
     */
    
    // Verifica si todos los bloques han salido (Condición Final)[cite: 116].
    bool esFinal();
    
    // Compara si dos estados tienen las mismas posiciones (para el ClosedSet)[cite: 21].
    bool operator==(const State& other) const;

    /**
     * Visualización y Depuración
     */
    void print();            // Imprime el estado actual.
    void printOperaciones(); // Imprime el camino de forma recursiva[cite: 13, 134].

    /**
     * Getters y Setters
     */
    inline int getNumBloques() const { return numBloques; }
    inline float getF() const { return f; }
    inline int getG() const { return g; }
    inline State* getPadre() const { return padre; }
    inline void setH(int h_val) { this->h = h_val; this->f = (float)(g + h_val); }
    
    // Permite acceder a la posición de un bloque específico para validaciones físicas.
    PosBloque getPosicion(int indice) const { return posiciones[indice]; }
    
    // Getter para la operación que generó este estado
    const char* getOperacion() const { return operacion ? operacion : ""; }
};

#endif