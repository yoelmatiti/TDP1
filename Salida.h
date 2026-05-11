#ifndef SALIDA_H
#define SALIDA_H

/**
 * Clase Salida (Meta Dinámica)
 * Representa una zona de victoria que puede cambiar de longitud en el tiempo.
 * Se define como una línea que parte de (x, y) en una dirección (dx, dy).
 */
class Salida {
private:
    char color;         // Color del bloque que puede salir por aquí
    int x, y;           // Origen de la salida (Coordenadas en el tablero)
    int direccionX;     // Dirección de expansión: -1 (izq), 0, 1 (der)
    int direccionY;     // Dirección de expansión: -1 (arriba), 0, 1 (abajo)
    int longitudInicial;// Longitud en el estado A del ciclo
    int longitudFinal;  // Longitud en el estado B del ciclo
    int pasoCambio;     // Cada cuántos turnos (g) alterna la longitud

public:
    // --- Constructores ---
    Salida();
    
    // Constructor completo sincronizado con Salida.cpp
    Salida(char color, int x, int y, int direccionX, int direccionY, 
           int longitudInicial, int longitudFinal, int pasoCambio);

    // Regla de los Tres: Constructor de Copia, Operador de Asignación, Destructor
    Salida(const Salida& otro);
    Salida& operator=(const Salida& otro);
    ~Salida() {} // Destructor trivial (no hay memoria dinámica)

    // --- Getters Constantes (Inline para eficiencia en el Solver) ---
    inline char getColor() const { return color; }
    inline int getX() const      { return x; }
    inline int getY() const      { return y; }
    inline int getDx() const     { return direccionX; }
    inline int getDy() const     { return direccionY; }

    // --- Lógica Dinámica (Implementada en Salida.cpp) ---
    
    /**
     * Determina la longitud de la salida en el tiempo g mediante aritmética modular.
     */
    int getLongitudActual(int tiempoG) const;
    
    /**
     * Verifica si una coordenada específica (f, c) es parte de esta salida
     * en el turno actual, utilizando cálculo vectorial O(1).
     */
    bool esParteDeSalida(int f, int c, int tiempoG) const;
    bool bloqueCubreSalida(int anchoB, int altoB) const;
};

#endif