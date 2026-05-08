#ifndef Compuerta_H
#define Compuerta_H

#include "Movimiento.h"

/**
 * Clase Compuerta (Compuerta Dinámica)
 * Actúa como un filtro en el tablero que permite o bloquea el paso
 * según el color, el tiempo (g) y las dimensiones del bloque.
 */
class Compuerta {
private:
    int x, y;               // Posición en el tablero
    char orientacion;       // 'V' (Vertical) o 'H' (Horizontal)
    
    char* cicloColores;     // Arreglo dinámico de estados/colores
    int numColores;         // Tamaño del ciclo
    int pasoCambio;         // Cada cuántos turnos (g) cambia el color

public:
    // --- Constructores y Destructor ---
    Compuerta();
    Compuerta(int _x, int _y, int _p, char _ori);
    
    // Regla de los tres (Gestión de memoria profunda)
    Compuerta(const Compuerta& otro);
    Compuerta& operator=(const Compuerta& otro);
    ~Compuerta();

    // --- Lógica de Configuración ---
    void agregarColorAlCiclo(char c);

    // --- Lógica de Estado y Tiempo ---
    // Determina el color en el tiempo g mediante aritmética modular O(1)
    char getColorActual(int tiempoG) const;
    
    // Retorna el color inicial (tiempo 0) para dibujo o depuración
    char getColor() const; 

    // --- Lógica de Validación de Paso ---
    // Verifica color y estado de pared ('#')
   
    
    // Verifica si el tamaño del bloque es apto para la orientación de la compuerta
    bool verificarDimension(int anchoBloque, int altoBloque) const;
    bool puedePasar(char colorBloque, int paso) const;
    // --- Getters Simples ---
    int getX() const;
    int getY() const;
    char getOrientacion() const;
    void calcularDestino(int bloqueX, int bloqueY, int anchoB, int altoB, Direccion dir, int& outX, int& outY) const;
};

#endif