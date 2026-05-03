#ifndef BLOQUE_H
#define BLOQUE_H

class Bloque {
private:
    int id;
    char caracterActual;
    int x, y;
    int anchoGeo, altoGeo;
    bool* geometria; // Arreglo dinámico 1D para eficiencia O(1)
    bool activo;
    bool esIncognito;
    char colorOculto;

public:
    // Constructor por defecto
    Bloque();

    // Constructor con 7 parámetros (Sincronizado con el Parser)
    Bloque(int _id, char _color, int _x, int _y, int _w, int _h, bool* _geo);

    // Regla de los Tres (Indispensable para A* y copias de estados)
    Bloque(const Bloque& otro);
    Bloque& operator=(const Bloque& otro);
    ~Bloque();

    // Métodos de lógica
    void mover(int dx, int dy);
    bool ocupaCelda(int tx, int ty) const;
    char getColorVisual() const; // Retorna '?' si es incógnito, o el color real si no
    void revelarColor();
    bool getEsIncognito() const { return esIncognito; }
    // Getters Constantes
    int getId() const;
    int getX() const { return x; }
    int getY() const { return y; }
    int getAnchoGeo() const { return anchoGeo; }
    int getAltoGeo() const { return altoGeo; }
    char getColor() const { return caracterActual; }
    bool estaActivo() const { return activo; }
    bool getGeometria(int fila, int col) const;
    void setGeometria(int fila, int col, bool ocupa);
    void setX(int nx) { this->x = nx; }
    void setY(int ny) { this->y = ny; }
    void setActivo(bool a) { this->activo = a; }
};

#endif