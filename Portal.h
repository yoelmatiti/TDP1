#ifndef PORTAL_H
#define PORTAL_H

class Portal {
private:
    int x, y;
    char* cicloColores; 
    int numColores;
    int pasoCambio;

public:
    Portal();
    Portal(int _x, int _y, int _p);
    Portal(const Portal& otro);
    Portal& operator=(const Portal& otro);
    ~Portal();

    void agregarColorAlCiclo(char c);
    
    // Solo las declaraciones
    char getColorActual(int tiempoG) const;
    char getColor() const; 
    bool puedePasar(char colorBloque, int tiempoG) const;

    // Getters simples
    int getX() const { return x; }
    int getY() const { return y; }
};

#endif