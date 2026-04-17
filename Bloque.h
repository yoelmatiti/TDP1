#ifndef BLOQUE_H
#define BLOQUE_H

class Bloque {
private:
    int id;
    char caracterActual;
    char colorOculto;
    int x, y;
    int anchoGeo;
    int altoGeo;
    bool** geometria;
    bool esIncognito;
    bool activo;

public:
    Bloque();
    Bloque(int _id, char _c, int _x, int _y, int _wG, int _hG);
    Bloque(const Bloque& otro);
    Bloque& operator=(const Bloque& otro);
    ~Bloque();

    void setGeometria(int fila, int col, bool ocupa);
    bool ocupaCelda(int filaAbs, int colAbs) const;
    void mover(int dx, int dy);
    
    // Getters
    bool estaActivo() const { return activo; }
    bool getEsIncognito() const { return esIncognito; }
    int getX() const { return x; }
    int getY() const { return y; }
    char getColor() const { return caracterActual; }
    int getAltoGeo() const { return altoGeo; }
    int getAnchoGeo() const { return anchoGeo; }
    bool getGeometria(int fila, int col) const;
   
    // Setters
    void setColorReal(char c) { colorOculto = c; }
    void setActivo(bool estado) { activo = estado; }

    // SOLO LA DECLARACIÓN AQUÍ
    void revelarColor(); 
};

#endif