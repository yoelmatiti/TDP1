#ifndef SALIDA_H
#define SALIDA_H

class Salida {
private:
    char color;
    int x, y;           // Origen
    int direccionX;     // -1, 0, o 1
    int direccionY;     // -1, 0, o 1
    int longitudInicial;
    int longitudFinal;
    int pasoCambio;

public:
    Salida();
    Salida(char c, int _x, int _y, int dx, int dy, int li, int lf, int p);

    // Getters constantes para el Solver
    char getColor() const { return color; }
    int getX() const { return x; }
    int getY() const { return y; }

    // OPTIMIZACIÓN: Cálculo matemático de longitud
    int getLongitudActual(int tiempoG) const;
    
    // OPTIMIZACIÓN: Comprobación sin bucles for
    bool esParteDeSalida(int f, int c, int tiempoG) const;
};

#endif