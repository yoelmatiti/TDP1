#include "Tablero.h"
#include <cstdio>
#include <iostream>
// Eliminamos #include <string>
#include "State.h"

// ============================================================
// 1. CICLO DE VIDA (CONSTRUCTORES Y MEMORIA)
// ============================================================

Tablero::Tablero() 
    : width(0), height(0), pasoActual(0), bloquesRestantes(0),
      matrizContigua(nullptr), matriz(nullptr),
      bloques(nullptr), numBloques(0), capacidadBloques(0),
      salidas(nullptr), numSalidas(0), capacidadSalidas(0),
      portales(nullptr), numPortales(0), capacidadPortales(0) // Asegúrate de inicializar capacidadPortales
{}

Tablero::~Tablero() {
    liberarMemoria();
}

void Tablero::liberarMemoria() {
    // Liberar punteros de filas primero
    if (matriz) delete[] matriz;
    // Liberar bloque de memoria contiguo
    if (matrizContigua) delete[] matrizContigua;
    
    matriz = nullptr;
    matrizContigua = nullptr;

    if (bloques) {
        for (int i = 0; i < numBloques; i++) if (bloques[i]) delete bloques[i];
        delete[] bloques;
        bloques = nullptr;
    }

    if (salidas) {
        for (int i = 0; i < numSalidas; i++) if (salidas[i]) delete salidas[i];
        delete[] salidas;
        salidas = nullptr;
    }

    if (portales) {
        for (int i = 0; i < numPortales; i++) if (portales[i]) delete portales[i];
        delete[] portales;
        portales = nullptr;
    }
}

// ============================================================
// 2. CONFIGURACIÓN Y CARGA
// ============================================================

void Tablero::setDimensiones(int w, int h) {
    this->width = w;
    this->height = h;
    this->matrizContigua = new char[w * h];
    this->matriz = new char*[h];

    for (int i = 0; i < h; i++) {
        this->matriz[i] = &this->matrizContigua[i * w];
        for (int j = 0; j < w; j++) this->matriz[i][j] = ' ';
    }
}

void Tablero::setPared(int fila, int col, char valor) {
    if (fila >= 0 && fila < height && col >= 0 && col < width) {
        matriz[fila][col] = valor;
    }
}

// Implementación manual de redimensionamiento (Vector-like sin STL)
void Tablero::agregarBloque(Bloque* b) {
    if (!b) return;
    if (numBloques >= capacidadBloques) {
        int nuevaCap = (capacidadBloques == 0) ? 10 : capacidadBloques * 2;
        Bloque** nuevo = new Bloque*[nuevaCap];
        for (int i = 0; i < numBloques; i++) nuevo[i] = bloques[i];
        delete[] bloques;
        bloques = nuevo;
        capacidadBloques = nuevaCap;
    }
    bloques[numBloques++] = b;
}

void Tablero::agregarSalida(Salida* s) {
    if (!s) return;
    if (numSalidas >= capacidadSalidas) {
        int nuevaCap = (capacidadSalidas == 0) ? 5 : capacidadSalidas * 2;
        Salida** nuevo = new Salida*[nuevaCap];
        for (int i = 0; i < numSalidas; i++) nuevo[i] = salidas[i];
        delete[] salidas;
        salidas = nuevo;
        capacidadSalidas = nuevaCap;
    }
    salidas[numSalidas++] = s;
}

void Tablero::agregarPortal(Portal* p) {
    if (!p) return;
    if (numPortales >= capacidadPortales) {
        int nuevaCap = (capacidadPortales == 0) ? 5 : capacidadPortales * 2;
        Portal** nuevo = new Portal*[nuevaCap];
        for (int i = 0; i < nuevaCap; i++) {
            if (i < numPortales) nuevo[i] = portales[i];
            else nuevo[i] = nullptr;
        }
        delete[] portales;
        portales = nuevo;
        capacidadPortales = nuevaCap;
    }
    portales[numPortales++] = p;
    setPared(p->getY(), p->getX(), p->getColor());
}

// ============================================================
// 3. RENDERIZADO (SIN STL)
// ============================================================

void Tablero::actualizarDesdeEstado(State* s) {
    if (!s) return;
    PosBloque* nuevasPos = s->getPosiciones();
    int nS = s->getNumBloques();

    // 1. Reset
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (matriz[i][j] != '#') matriz[i][j] = ' ';
        }
    }

    // 2. Salidas
    for (int i = 0; i < numSalidas; i++) {
        Salida* sal = salidas[i];
        if (enLimites(sal->getX(), sal->getY())) {
            matriz[sal->getY()][sal->getX()] = (char)tolower(sal->getColor());
        }
    }

    // 3. Bloques
    this->bloquesRestantes = 0;
    for (int i = 0; i < numBloques && i < nS; i++) {
        Bloque* b = bloques[i];
        b->setX(nuevasPos[i].x);
        b->setY(nuevasPos[i].y);
        b->setActivo(nuevasPos[i].activo);

        if (b->estaActivo()) {
            this->bloquesRestantes++;
            char colorMayus = (char)toupper(b->getColor());
            for (int rY = 0; rY < b->getAltoGeo(); rY++) {
                for (int rX = 0; rX < b->getAnchoGeo(); rX++) {
                    if (b->getGeometria(rY, rX)) {
                        int ty = b->getY() + rY;
                        int tx = b->getX() + rX;
                        if (enLimites(tx, ty)) matriz[ty][tx] = colorMayus;
                    }
                }
            }
        }
    }
}

void Tablero::imprimir() {
    std::cout << "   ";
    for (int j = 0; j < width; j++) std::cout << j % 10;
    
    // Sustitución de std::string(width, '-') por bucle manual
    std::cout << "\n   +";
    for (int j = 0; j < width; j++) std::cout << '-';
    std::cout << "+\n";

    for (int i = 0; i < height; i++) {
        std::printf("%2d |", i);
        for (int j = 0; j < width; j++) std::cout << matriz[i][j];
        std::cout << "|\n";
    }

    std::cout << "   +";
    for (int j = 0; j < width; j++) std::cout << '-';
    std::cout << "+\n";
}

// ============================================================
// 4. CONSULTA
// ============================================================

char Tablero::getCeldaEstatica(int x, int y) const {
    if (!enLimites(x, y)) return '#';
    return matriz[y][x];
}

Salida* Tablero::getSalidaEn(int x, int y) const {
    for (int i = 0; i < numSalidas; i++) {
        if (salidas[i]->getX() == x && salidas[i]->getY() == y) return salidas[i];
    }
    return nullptr;
}

bool Tablero::esPortal(int y, int x) const {
    if (!enLimites(x, y)) return false;
    return getPortalEn(x, y) != nullptr;
}

Portal* Tablero::getPortalEn(int x, int y) const {
    if (!enLimites(x, y) || portales == nullptr) return nullptr;
    for (int i = 0; i < numPortales; i++) {
        if (portales[i] && portales[i]->getX() == x && portales[i]->getY() == y) 
            return portales[i];
    }
    return nullptr;
}

bool Tablero::enLimites(int x, int y) const {
    return (x >= 0 && x < width && y >= 0 && y < height);
}

bool Tablero::comprobarMeta(int id, int x, int y) const {
    Bloque* b = nullptr;
    for(int i=0; i<numBloques; i++) {
        if(bloques[i]->getId() == id) { b = bloques[i]; break; }
    }
    Salida* s = getSalidaEn(x, y);
    if (s && b) {
        return (tolower(s->getColor()) == tolower(b->getColor()));
    }
    return false;
}

Bloque* Tablero::getBloquePtr(int i) const { return (i>=0 && i<numBloques) ? bloques[i] : nullptr; }
Salida* Tablero::getSalidaPtr(int i) const { return (i>=0 && i<numSalidas) ? salidas[i] : nullptr; }
Portal* Tablero::getPortalPtr(int i) const { return (i>=0 && i<numPortales) ? portales[i] : nullptr; }