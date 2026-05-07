#include "Tablero.h"
#include <cstdio>   // Sustituto de iostream
#include <cctype>   // Para tolower/toupper
#include "State.h"

// ============================================================
// 1. CICLO DE VIDA
// ============================================================

Tablero::Tablero() 
    : width(0), height(0), pasoActual(0), bloquesRestantes(0),
      matrizContigua(nullptr), matriz(nullptr),
      bloques(nullptr), numBloques(0), capacidadBloques(0),
      salidas(nullptr), numSalidas(0), capacidadSalidas(0),
      portales(nullptr), numPortales(0), capacidadPortales(0)
{}

Tablero::~Tablero() {
    liberarMemoria();
}

void Tablero::liberarMemoria() {
    if (matriz) delete[] matriz;
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

Portal* Tablero::getPortalPtr(int i) const { 
    return (i >= 0 && i < numPortales) ? portales[i] : nullptr; 
}
bool Tablero::esPortal(int x, int y) const {
    // IMPORTANTE: Debe llamar a getPortalEn con el mismo orden (x, y)
    return (this->getPortalEn(x, y) != nullptr);
}
// ============================================================
// 2. CONFIGURACIÓN
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
        for (int i = 0; i < numPortales; i++) nuevo[i] = portales[i];
        delete[] portales;
        portales = nuevo;
        capacidadPortales = nuevaCap;
    }
    portales[numPortales++] = p;
    setPared(p->getY(), p->getX(), p->getColor());
}

// ============================================================
// 3. RENDERIZADO (CORREGIDO Y SIN STL)
// ============================================================
void Tablero::actualizarDesdeEstado(State* s) {
    if (!s) return;
    
    // 1. LIMPIEZA TOTAL: Borrar rastros de bloques anteriores
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // Mantenemos solo los muros fijos
            if (matriz[i][j] != '#') {
                matriz[i][j] = ' ';
            }
        }
    }

    // 2. CAPA DE INFRAESTRUCTURA (Dibujado antes que los bloques)
    
    // 2.1 Salidas (Minúsculas estáticas)
    for (int i = 0; i < numSalidas; i++) {
        Salida* sal = salidas[i];
        if (enLimites(sal->getX(), sal->getY())) {
            matriz[sal->getY()][sal->getX()] = (char)tolower(sal->getColor());
        }
    }

    // 2.2 Portales DINÁMICOS (Cambian según el tiempo 'g')
    for (int i = 0; i < numPortales; i++) {
        Portal* p = portales[i];
        if (enLimites(p->getX(), p->getY())) {
            // Se usa s->getG() para obtener el paso actual del Solver
            // El portal devuelve el color (carácter) que le corresponde en ese paso
            matriz[p->getY()][p->getX()] = p->getColorActual(s->getG());
        }
    }

    // 3. CAPA DE ACTORES (Bloques con geometría compleja)
    this->bloquesRestantes = 0;
    int nS = s->getNumBloques();

    for (int i = 0; i < numBloques && i < nS; i++) {
        Bloque* b = bloques[i];
        
        // Sincronizar coordenadas del objeto Bloque con el Estado lógico
        PosBloque p = s->getPosicion(i); 
        b->setX(p.x);
        b->setY(p.y);
        b->setActivo(p.activo);

        // Solo dibujamos si el bloque NO ha salido del tablero (activo == true)
        if (b->estaActivo()) {
            this->bloquesRestantes++;
            char colorMayus = (char)toupper(b->getColor());
            
            // Renderizado por Geometría: recorre el ancho y alto definido en el .txt
            for (int rY = 0; rY < b->getAltoGeo(); rY++) {
                for (int rX = 0; rX < b->getAnchoGeo(); rX++) {
                    // Si la celda de la matriz de geometría es 1, se pinta
                    if (b->getGeometria(rY, rX)) { 
                        int ty = b->getY() + rY;
                        int tx = b->getX() + rX;
                        
                        if (enLimites(tx, ty)) {
                            // El bloque se dibuja sobre la salida o el portal
                            matriz[ty][tx] = colorMayus;
                        }
                    }
                }
            }
        }
    }
}

void Tablero::imprimir() {
    // 1. Números de columnas
    printf("    ");
    for (int j = 0; j < width; j++) printf("%d", j % 10);
    
    // 2. Borde superior
    printf("\n    +");
    for (int j = 0; j < width; j++) putchar('-');
    printf("+\n");

    // 3. Contenido de las filas
    for (int i = 0; i < height; i++) {
        printf("%3d |", i);
        for (int j = 0; j < width; j++) {
            putchar(matriz[i][j]);
        }
        printf("|\n");
    }

    // 4. Borde inferior
    printf("    +");
    for (int j = 0; j < width; j++) putchar('-');
    printf("+\n");
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

bool Tablero::enLimites(int x, int y) const {
    return (x >= 0 && x < width && y >= 0 && y < height);
}

Bloque* Tablero::getBloquePtr(int i) const { return (i>=0 && i<numBloques) ? bloques[i] : nullptr; }
Salida* Tablero::getSalidaPtr(int i) const { return (i>=0 && i<numSalidas) ? salidas[i] : nullptr; }
// 2. Para obtener un portal sabiendo su ubicación en el mapa (Coordenadas)
// Este es el que busca Movimiento.cpp
Portal* Tablero::getPortalEn(int x, int y) const {
    if (!enLimites(x, y) || portales == nullptr) return nullptr;
    
    for (int i = 0; i < numPortales; i++) {
        if (portales[i] != nullptr && 
            portales[i]->getX() == x && 
            portales[i]->getY() == y) {
            return portales[i]; // Encontrado
        }
    }
    return nullptr; // No hay portal en esa coordenada
}

bool Tablero::comprobarMeta(int idBloque, int x, int y) const {
    // 1. Buscar el bloque por ID
    Bloque* b = nullptr;
    for(int i = 0; i < numBloques; i++) {
        if(bloques[i]->getId() == idBloque) { 
            b = bloques[i]; 
            break; 
        }
    }
    
    // 2. Buscar salida en esa posición
    Salida* s = getSalidaEn(x, y);
    
    // 3. Validar coincidencia de color (Case Insensitive)
    if (s != nullptr && b != nullptr) {
        char colorS = (char)tolower(s->getColor());
        char colorB = (char)tolower(b->getColor());
        return (colorS == colorB);
    }
    
    return false;
}