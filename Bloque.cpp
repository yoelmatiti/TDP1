#include "Bloque.h"

// Constructor por defecto
Bloque::Bloque() 
    : id(-1), caracterActual(' '), x(0), y(0), anchoGeo(0), altoGeo(0), 
      geometria(nullptr), activo(false), esIncognito(false), colorOculto(' ') {}

// Constructor de 7 parámetros (Sincronización Crítica de ID)
Bloque::Bloque(int id, char _color, int _x, int _y, int _w, int _h, bool* _geo)
    : id(id), caracterActual(_color), x(_x), y(_y), anchoGeo(_w), altoGeo(_h), activo(true) {
    
    // 1. Asignación inmediata y explícita
    this->id = id; 
    
    // 2. Inicialización de estado
    this->esIncognito = false;
    this->colorOculto = _color;

    // 3. Gestión de memoria de la geometría
    int tam = anchoGeo * altoGeo;
    this->geometria = new bool[tam]; 
    
    if (_geo != nullptr) {
        for (int i = 0; i < tam; ++i) {
            this->geometria[i] = _geo[i]; 
        }
    }
}

// Getter crítico para la identificación en el Tablero
int Bloque::getId() const {
    return this->id;
}

// Operador de Asignación (Actualizado para incluir estados de incógnito)
Bloque& Bloque::operator=(const Bloque& otro) {
    if (this != &otro) {
        delete[] geometria; 

        this->id = otro.id;
        this->caracterActual = otro.caracterActual;
        this->x = otro.x;
        this->y = otro.y;
        this->anchoGeo = otro.anchoGeo;
        this->altoGeo = otro.altoGeo;
        this->activo = otro.activo;
        this->esIncognito = otro.esIncognito;
        this->colorOculto = otro.colorOculto;

        if (otro.geometria != nullptr) {
            int tam = anchoGeo * altoGeo;
            geometria = new bool[tam];
            for (int i = 0; i < tam; i++) {
                geometria[i] = otro.geometria[i];
            }
        } else {
            geometria = nullptr;
        }
    }
    return *this;
}
// Constructor de Copia (Vital para que A* no corrompa estados anteriores)
Bloque::Bloque(const Bloque& otro) : geometria(nullptr) {
    *this = otro;
}



// Destructor para cumplir con la eficiencia de memoria
Bloque::~Bloque() {
    delete[] geometria;
}

// Mapeo de 2D a 1D: fila * ancho + columna (Eficiencia O(1)) 
bool Bloque::getGeometria(int fila, int col) const {
    if (geometria != nullptr && fila >= 0 && fila < altoGeo && col >= 0 && col < anchoGeo) {
        return geometria[fila * anchoGeo + col];
    }
    return false;
}

// Determina si el bloque está presente en una celda específica del tablero
bool Bloque::ocupaCelda(int tx, int ty) const {
    // 1. Calcular coordenadas relativas al origen del bloque
    int relX = tx - this->x;
    int relY = ty - this->y;

    // 2. Verificar si está dentro de la caja (bounding box) del bloque
    if (relX >= 0 && relX < anchoGeo && relY >= 0 && relY < altoGeo) {
        // 3. Acceso a matriz plana: (fila * ancho) + columna
        // Esto corrige el error de "invalid types"
        return geometria[relY * anchoGeo + relX];
    }
    
    return false;
}

void Bloque::mover(int dx, int dy) {
    if (activo) {
        x += dx;
        y += dy;
    }
}
char Bloque::getColorVisual() const {
    return esIncognito ? '?' : colorOculto;
}
void Bloque::revelarColor() {
    if (this->esIncognito) {
        this->esIncognito = false;
        this->caracterActual = this->colorOculto;
    }
}
void Bloque::setGeometria(int fila, int col, bool ocupa) {
    if (geometria != nullptr) {
        geometria[fila * anchoGeo + col] = ocupa;
    }
}
