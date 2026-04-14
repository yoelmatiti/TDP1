#ifndef BLOQUE_H
#define BLOQUE_H

class Bloque {
private:
    int id;                 // ID numérico o índice
    char caracter;          // Caracter representativo ('A', 'B', '?', etc.)
    int x, y;               // Posición de la esquina superior izquierda (anclaje)
    int anchoGeo;           // Ancho de la matriz de geometría
    int altoGeo;            // Alto de la matriz de geometría
    bool** geometria;       // Matriz dinámica que define la forma del bloque
    
    // El color puede ser un enum o el mismo caracter
    bool esIncognito;       // True si el caracter es '?'

public:
    Bloque();
    // Constructor con parámetros básicos
    Bloque(int _id, char _c, int _x, int _y, int _wG, int _hG);
    
    // Regla de los tres (Indispensable para copia profunda)
    Bloque(const Bloque& otro);
    Bloque& operator=(const Bloque& otro);
    ~Bloque();

    // Métodos
    void setGeometria(int fila, int col, bool ocupa);
    bool ocupaCelda(int filaAbsoluta, int colAbsoluta) const;
    void mover(int dx, int dy);
    
    // Getters
    int getX() const { return x; }
    int getY() const { return y; }
    int getId() const { return id; }
    char getColor() const { return caracter; }
    int getAltoGeo() const { return altoGeo; }
    int getAnchoGeo() const { return anchoGeo; }
    bool getGeometria(int fila, int col) const;
    void revelarColor(char nuevoColor);
};

#endif
