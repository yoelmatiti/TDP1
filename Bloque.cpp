#include "Bloque.h"

// Constructor por defecto
Bloque::Bloque() 
    : id(-1), caracterActual(' '), colorOculto(' '), x(0), y(0), anchoGeo(0), altoGeo(0), 
      geometria(nullptr), esIncognito(false), activo(false) {}

// Constructor parametrizado
Bloque::Bloque(int _id, char _c, int _x, int _y, int _wG, int _hG) 
    : id(_id), caracterActual(_c), x(_x), y(_y), anchoGeo(_wG), altoGeo(_hG), activo(true) {
    
    esIncognito = (caracterActual == '?');
    colorOculto = ' '; // Se asignará vía setColorReal en el Parser
    
    // Crear matriz de geometría
    geometria = new bool*[altoGeo];
    for (int i = 0; i < altoGeo; i++) {
        geometria[i] = new bool[anchoGeo];
        for (int j = 0; j < anchoGeo; j++) {
            geometria[i][j] = false;
        }
    }
}

// Constructor de Copia
Bloque::Bloque(const Bloque& otro) {
    geometria = nullptr; // Crucial para evitar que operator= borre basura
    *this = otro; 
}

// Operador de Asignación (Copia Profunda)
Bloque& Bloque::operator=(const Bloque& otro) {
    if (this != &otro) {
        // 1. Liberar memoria actual si existe
        if (geometria != nullptr) {
            for (int i = 0; i < altoGeo; i++) delete[] geometria[i];
            delete[] geometria;
        }

        // 2. Copiar datos simples
        this->id = otro.id;
        this->caracterActual = otro.caracterActual;
        this->colorOculto = otro.colorOculto;
        this->x = otro.x;
        this->y = otro.y;
        this->anchoGeo = otro.anchoGeo;
        this->altoGeo = otro.altoGeo;
        this->esIncognito = otro.esIncognito;
        this->activo = otro.activo;

        // 3. Copiar matriz de geometría
        if (otro.geometria != nullptr) {
            this->geometria = new bool*[altoGeo];
            for (int i = 0; i < altoGeo; i++) {
                this->geometria[i] = new bool[anchoGeo];
                for (int j = 0; j < anchoGeo; j++) {
                    this->geometria[i][j] = otro.geometria[i][j];
                }
            }
        } else {
            geometria = nullptr;
        }
    }
    return *this;
}

// Destructor
Bloque::~Bloque() {
    if (geometria != nullptr) {
        for (int i = 0; i < altoGeo; i++) delete[] geometria[i];
        delete[] geometria;
    }
}

void Bloque::setGeometria(int fila, int col, bool ocupa) {
    if (geometria != nullptr && fila >= 0 && fila < altoGeo && col >= 0 && col < anchoGeo) {
        geometria[fila][col] = ocupa;
    }
}

bool Bloque::ocupaCelda(int fTablero, int cTablero) const {
    if (!activo) return false; 

    int fRel = fTablero - y;
    int cRel = cTablero - x;
    
    if (fRel >= 0 && fRel < altoGeo && cRel >= 0 && cRel < anchoGeo) {
        return (geometria != nullptr && geometria[fRel][cRel]);
    }
    return false;
}

void Bloque::mover(int dx, int dy) {
    if (activo) { 
        x += dx;
        y += dy;
    }
}

// Ya no recibe parámetro porque usa colorOculto definido previamente
void Bloque::revelarColor() {
    if (esIncognito) {
        caracterActual = colorOculto;
        esIncognito = false;
    }
}

bool Bloque::getGeometria(int fila, int col) const {
    if (geometria != nullptr && fila >= 0 && fila < altoGeo && col >= 0 && col < anchoGeo) {
        return geometria[fila][col];
    }
    return false;
}