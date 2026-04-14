#ifndef PORTAL_H
#define PORTAL_H

class Portal {
private:
    int x, y;               // Posición en el tablero
    char* cicloColores;     // Arreglo dinámico de caracteres (colores)
    int numColores;         // Cantidad de colores en el ciclo
    int pasoCambio;         // Cada cuántos pasos cambia (p)

public:
    Portal();
    Portal(int _x, int _y, int _p);
    
    // Regla de los tres para manejo de cicloColores
    Portal(const Portal& otro);
    Portal& operator=(const Portal& otro);
    ~Portal();

    // Métodos
    void agregarColorAlCiclo(char c);
    char getColorActual(int tiempoG) const;
    
    // Getters
    int getX() const { return x; }
    int getY() const { return y; }
    
    char getColor() const { return (numColores > 0) ? cicloColores[0] : ' '; }
    
    // Un portal puede ser atravesado si el bloque es del mismo color
    bool puedePasar(char colorBloque, int tiempoG) const;
};

#endif