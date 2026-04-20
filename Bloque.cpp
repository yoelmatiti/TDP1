#include "Bloque.h"
#include <iostream>

// Constructor por defecto
Bloque::Bloque() 
    : id(-1), caracterActual(' '), colorOculto(' '), x(0), y(0), anchoGeo(0), altoGeo(0), 
      geometria(nullptr), esIncognito(false), activo(false) {}

// Constructor parametrizado (Optimizado a 1D)
Bloque::Bloque(int _id, char _c, int _x, int _y, int _wG, int _hG) 
    : id(_id), caracterActual(_c), x(_x), y(_y), anchoGeo(_wG), altoGeo(_hG) {
    
    // Un solo NEW: más rápido y menos fragmentación
    geometria = new bool[anchoGeo * altoGeo]; 
    for (int i = 0; i < anchoGeo * altoGeo; i++) geometria[i] = false;
    
    activo = true;
    esIncognito = false;
}

// Constructor de Copia
Bloque::Bloque(const Bloque& otro) {
    geometria = nullptr; 
    *this = otro; // Llama al operador de asignación corregido
}

// Operador de Asignación (Copia Profunda Optimizada)
Bloque& Bloque::operator=(const Bloque& otro) {
    if (this != &otro) {
        // 1. Liberar memoria vieja (ahora es un delete simple)
        delete[] geometria;

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

        // 3. Copiar geometría 1D
        if (otro.geometria != nullptr) {
            this->geometria = new bool[anchoGeo * altoGeo];
            for (int i = 0; i < anchoGeo * altoGeo; i++) {
                this->geometria[i] = otro.geometria[i];
            }
        } else {
            geometria = nullptr;
        }
    }
    return *this;
}

// Destructor
Bloque::~Bloque() {
    delete[] geometria; 
}

// Acceso con mapeo de 2D a 1D: fila * ancho + columna
void Bloque::setGeometria(int fila, int col, bool ocupa) {
    if (geometria != nullptr && fila >= 0 && fila < altoGeo && col >= 0 && col < anchoGeo) {
        geometria[fila * anchoGeo + col] = ocupa;
    }
}

bool Bloque::getGeometria(int fila, int col) const {
    if (geometria != nullptr && fila >= 0 && fila < altoGeo && col >= 0 && col < anchoGeo) {
        return geometria[fila * anchoGeo + col];
    }
    return false;
}

bool Bloque::ocupaCelda(int fTablero, int cTablero) const {
    if (!activo) return false; 

    int fRel = fTablero - y;
    int cRel = cTablero - x;
    
    if (fRel >= 0 && fRel < altoGeo && cRel >= 0 && cRel < anchoGeo) {
        return (geometria != nullptr && geometria[fRel * anchoGeo + cRel]);
    }
    return false;
}

void Bloque::mover(int dx, int dy) {
    if (activo) { 
        x += dx;
        y += dy;
    }
}

void Bloque::revelarColor() {
    if (esIncognito) {
        caracterActual = colorOculto;
        esIncognito = false;
    }
}