#ifndef PORTAL_H
#define PORTAL_H

/**
 * Clase Portal (Compuerta Dinámica)
 * Actúa como un filtro en el tablero que permite o bloquea el paso
 * según el color, el tiempo (g) y las dimensiones del bloque.
 */
class Portal {
private:
    int x, y;               // Posición en el tablero
    char orientacion;       // 'V' (Vertical) o 'H' (Horizontal)
    
    char* cicloColores;     // Arreglo dinámico de estados/colores
    int numColores;         // Tamaño del ciclo
    int pasoCambio;         // Cada cuántos turnos (g) cambia el color

public:
    // --- Constructores y Destructor ---
    Portal();
    Portal(int _x, int _y, int _p, char _ori);
    
    // Regla de los tres (Gestión de memoria profunda)
    Portal(const Portal& otro);
    Portal& operator=(const Portal& otro);
    ~Portal();

    // --- Lógica de Configuración ---
    void agregarColorAlCiclo(char c);

    // --- Lógica de Estado y Tiempo ---
    // Determina el color en el tiempo g mediante aritmética modular O(1)
    char getColorActual(int tiempoG) const;
    
    // Retorna el color inicial (tiempo 0) para dibujo o depuración
    char getColor() const; 

    // --- Lógica de Validación de Paso ---
    // Verifica color y estado de pared ('#')
    bool puedePasar(char colorBloque, int tiempoG) const;
    
    // Verifica si el tamaño del bloque es apto para la orientación de la compuerta
    bool verificarDimension(int anchoBloque, int altoBloque) const;

    // --- Getters Simples ---
    int getX() const;
    int getY() const;
    char getOrientacion() const;
};

#endif