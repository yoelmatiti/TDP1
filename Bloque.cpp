#include "Bloque.h"

Bloque::Bloque() : id(-1), caracter(' '), x(0), y(0), anchoGeo(0), altoGeo(0), geometria(nullptr) {}

Bloque::Bloque(int _id, char _c, int _x, int _y, int _wG, int _hG) 
    : id(_id), caracter(_c), x(_x), y(_y), anchoGeo(_wG), altoGeo(_hG) {
    
    esIncognito = (caracter == '?');
    
    // Crear matriz de geometría
    geometria = new bool*[altoGeo];
    for (int i = 0; i < altoGeo; i++) {
        geometria[i] = new bool[anchoGeo];
        for (int j = 0; j < anchoGeo; j++) geometria[i][j] = false;
    }
}

// Constructor de Copia
Bloque::Bloque(const Bloque& otro) {
    geometria = nullptr;
    *this = otro; // Llama al operador de asignación
}

// Operador de Asignación
Bloque& Bloque::operator=(const Bloque& otro) {
    if (this != &otro) {
        // Liberar memoria actual
        if (geometria != nullptr) {
            for (int i = 0; i < altoGeo; i++) delete[] geometria[i];
            delete[] geometria;
        }

        // Copiar datos simples
        this->id = otro.id;
        this->caracter = otro.caracter;
        this->x = otro.x;
        this->y = otro.y;
        this->anchoGeo = otro.anchoGeo;
        this->altoGeo = otro.altoGeo;
        this->esIncognito = otro.esIncognito;

        // Copiar matriz de geometría (Copia Profunda)
        this->geometria = new bool*[altoGeo];
        for (int i = 0; i < altoGeo; i++) {
            this->geometria[i] = new bool[anchoGeo];
            for (int j = 0; j < anchoGeo; j++) {
                this->geometria[i][j] = otro.geometria[i][j];
            }
        }
    }
    return *this;
}

Bloque::~Bloque() {
    if (geometria != nullptr) {
        for (int i = 0; i < altoGeo; i++) delete[] geometria[i];
        delete[] geometria;
    }
}

void Bloque::setGeometria(int fila, int col, bool ocupa) {
    if (fila >= 0 && fila < altoGeo && col >= 0 && col < anchoGeo) {
        geometria[fila][col] = ocupa;
    }
}

// Verifica si el bloque ocupa una posición específica en el tablero
bool Bloque::ocupaCelda(int fTablero, int cTablero) const {
    int fRel = fTablero - y;
    int cRel = cTablero - x;
    
    if (fRel >= 0 && fRel < altoGeo && cRel >= 0 && cRel < anchoGeo) {
        return geometria[fRel][cRel];
    }
    return false;
}

void Bloque::mover(int dx, int dy) {
    x += dx;
    y += dy;
}

void Bloque::revelarColor(char nuevoColor) {
    caracter = nuevoColor;
    esIncognito = false;
}

bool Bloque::getGeometria(int fila, int col) const {
    if (fila >= 0 && fila < altoGeo && col >= 0 && col < anchoGeo) {
        return geometria[fila][col];
    }
    return false;
}