#ifndef SALIDA_H
#define SALIDA_H

class Salida {
private:
    char color;         // Color que acepta ('A', 'B', etc.)
    int x, y;           // Posición de la celda de origen (donde está en el muro)
    int direccionX;     // Dirección de crecimiento (1, -1 o 0)
    int direccionY;     // Dirección de crecimiento (1, -1 o 0)
    int longitudInicial;// L_i
    int longitudFinal;  // L_f
    int pasoCambio;     // p_s (cada cuántos pasos cambia)

public:
    Salida();
    Salida(char c, int _x, int _y, int dx, int dy, int li, int lf, int p);

    // Regla de los tres
    Salida(const Salida& otro);
    Salida& operator=(const Salida& otro);
    ~Salida();

    // Retorna la longitud actual basada en el tiempo g del tablero
    int getLongitudActual(int tiempoG) const;

    // Verifica si una posición (f, c) es parte de la salida en un tiempo dado
    bool esParteDeSalida(int f, int c, int tiempoG) const;

    char getColor() const { return color; }
    int getX() const { return x; }
    int getY() const { return y; }
};

#endif