#ifndef BLOQUE_H
#define BLOQUE_H

class Bloque {
private:
    int id;
    char caracterActual;
    char colorOculto;
    int x, y;
    int anchoGeo, altoGeo;
    bool* geometria; // Arreglo 1D
    bool esIncognito, activo;

public:
    Bloque();
    Bloque(int _id, char _c, int _x, int _y, int _wG, int _hG);
    Bloque(const Bloque& otro);
    Bloque& operator=(const Bloque& otro);
    ~Bloque();

    // Métodos de lógica
    void setActivo(bool estado) { activo = estado; }
    void setGeometria(int fila, int col, bool ocupa);
    bool ocupaCelda(int fTablero, int cTablero) const;
    void mover(int dx, int dy);
    void revelarColor();

    // GETTERS PÚBLICOS (Para corregir errores de Tablero.cpp)
    int getX() const { return x; }
    int getY() const { return y; }
    int getAnchoGeo() const { return anchoGeo; }
    int getAltoGeo() const { return altoGeo; }
    bool getEsIncognito() const { return esIncognito; }
    bool estaActivo() const { return activo; }
    char getColor() const { return caracterActual; }
    bool getGeometria(int fila, int col) const;
};

#endif