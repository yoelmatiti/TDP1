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
      compuertas(nullptr), numCompuertas(0), capacidadCompuertas(0)
{}

Tablero::~Tablero() {
    liberarMemoria();
}

// Constructor de Copia (PROFUNDA Y EN CASCADA para A*)
Tablero::Tablero(const Tablero& otra) 
    : width(0), height(0), pasoActual(0), bloquesRestantes(0),
      matrizContigua(nullptr), matriz(nullptr),
      bloques(nullptr), numBloques(0), capacidadBloques(0),
      salidas(nullptr), numSalidas(0), capacidadSalidas(0),
      compuertas(nullptr), numCompuertas(0), capacidadCompuertas(0)
{
    *this = otra;
}

// Operador de Asignación (Implementa la copia profunda en cascada)
Tablero& Tablero::operator=(const Tablero& otra) {
    if (this != &otra) {
        // 1. Liberar memoria existente
        liberarMemoria();

        // 2. Copiar dimensiones y pasos
        this->width = otra.width;
        this->height = otra.height;
        this->pasoActual = otra.pasoActual;
        this->bloquesRestantes = otra.bloquesRestantes;

        // 3. Copiar matrices (matrizContigua y matriz)
        if (otra.matrizContigua && width > 0 && height > 0) {
            int tam = width * height;
            this->matrizContigua = new char[tam];
            for (int i = 0; i < tam; i++) {
                this->matrizContigua[i] = otra.matrizContigua[i];
            }

            this->matriz = new char*[height];
            for (int i = 0; i < height; i++) {
                this->matriz[i] = &this->matrizContigua[i * width];
            }
        }

        // 4. Copiar BLOQUES (Copia Profunda: llama al constructor de copia de Bloque)
        if (otra.bloques && otra.numBloques > 0) {
            this->capacidadBloques = otra.capacidadBloques;
            this->numBloques = otra.numBloques;
            this->bloques = new Bloque*[capacidadBloques];
            
            for (int i = 0; i < numBloques; i++) {
                if (otra.bloques[i]) {
                    // Llamar al constructor de copia de Bloque (Deep Copy)
                    this->bloques[i] = new Bloque(*otra.bloques[i]);
                } else {
                    this->bloques[i] = nullptr;
                }
            }
        }

        // 5. Copiar SALIDAS
        if (otra.salidas && otra.numSalidas > 0) {
            this->capacidadSalidas = otra.capacidadSalidas;
            this->numSalidas = otra.numSalidas;
            this->salidas = new Salida*[capacidadSalidas];
            
            for (int i = 0; i < numSalidas; i++) {
                if (otra.salidas[i]) {
                    // Suponiendo que Salida tiene constructor de copia
                    this->salidas[i] = new Salida(*otra.salidas[i]);
                } else {
                    this->salidas[i] = nullptr;
                }
            }
        }

        // 6. Copiar COMPUERTAS
        if (otra.compuertas && otra.numCompuertas > 0) {
            this->capacidadCompuertas = otra.capacidadCompuertas;
            this->numCompuertas = otra.numCompuertas;
            this->compuertas = new Compuerta*[capacidadCompuertas];
            
            for (int i = 0; i < numCompuertas; i++) {
                if (otra.compuertas[i]) {
                    // Suponiendo que Compuerta tiene constructor de copia
                    this->compuertas[i] = new Compuerta(*otra.compuertas[i]);
                } else {
                    this->compuertas[i] = nullptr;
                }
            }
        }
    }
    return *this;
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

    if (compuertas) {
        for (int i = 0; i < numCompuertas; i++) if (compuertas[i]) delete compuertas[i];
        delete[] compuertas;
        compuertas = nullptr;
    }
}

Compuerta* Tablero::getCompuertaPtr(int i) const { 
    return (i >= 0 && i < numCompuertas) ? compuertas[i] : nullptr; 
}
bool Tablero::esCompuerta(int x, int y) const {
    // IMPORTANTE: Debe llamar a getCompuertaEn con el mismo orden (x, y)
    return (this->getCompuertaEn(x, y) != nullptr);
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

void Tablero::agregarCompuerta(Compuerta* p) {
    if (!p) return;
    if (numCompuertas >= capacidadCompuertas) {
        int nuevaCap = (capacidadCompuertas == 0) ? 5 : capacidadCompuertas * 2;
        Compuerta** nuevo = new Compuerta*[nuevaCap];
        for (int i = 0; i < numCompuertas; i++) nuevo[i] = compuertas[i];
        delete[] compuertas;
        compuertas = nuevo;
        capacidadCompuertas = nuevaCap;
    }
    compuertas[numCompuertas++] = p;
    
}

bool Tablero::esObstaculo(int x, int y, int tiempoG, char colorBloque) const {
    if (!enLimites(x, y)) return true;
    if (matriz[y][x] == '#') return true;

    Compuerta* cp = getCompuertaEn(x, y);
    if (cp) {
        if (!cp->puedePasar(colorBloque, tiempoG)) return true;
    }
    return false;
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
    
    
    // 2.1 Salidas DINÁMICAS
    for (int i = 0; i < numSalidas; i++) {
        Salida* sal = salidas[i];
        int L = sal->getLongitudActual(s->getG()); // Usar el tiempo del estado
        char colorSalida = (char)tolower(sal->getColor());
        
        // Dibujar la línea de la salida según su dirección y longitud actual
        for (int k = 0; k < L; k++) {
            int tx = sal->getX() + (k * sal->getDx());
            int ty = sal->getY() + (k * sal->getDy());
            if (enLimites(tx, ty)) {
                matriz[ty][tx] = colorSalida;
            }
        }
    }

    // 2.2 Compuertas DINÁMICOS (Cambian según el tiempo 'g')
    for (int i = 0; i < numCompuertas; i++) {
        Compuerta* p = compuertas[i];
        if (enLimites(p->getX(), p->getY())) {
            // Se usa s->getG() para obtener el paso actual del Solver
            // El compuerta devuelve el color (carácter) que le corresponde en ese paso
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
                            // El bloque se dibuja sobre la salida o el compuerta
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
// 2. Para obtener un compuerta sabiendo su ubicación en el mapa (Coordenadas)
// Este es el que busca Movimiento.cpp
Compuerta* Tablero::getCompuertaEn(int x, int y) const {
    if (!enLimites(x, y) || compuertas == nullptr) return nullptr;
    
    for (int i = 0; i < numCompuertas; i++) {
        if (compuertas[i] != nullptr && 
            compuertas[i]->getX() == x && 
            compuertas[i]->getY() == y) {
            return compuertas[i]; // Encontrado
        }
    }
    return nullptr; // No hay compuerta en esa coordenada
}

bool Tablero::comprobarMeta(int idxBloque, int x, int y, int tiempoG) const {
    if (idxBloque < 0 || idxBloque >= numBloques) return false;
    Bloque* b = bloques[idxBloque];
    
    // El bloque sale si TODAS sus celdas ocupadas están sobre una salida válida
    for (int rY = 0; rY < b->getAltoGeo(); rY++) {
        for (int rX = 0; rX < b->getAnchoGeo(); rX++) {
            if (b->getGeometria(rY, rX)) {
                int tx = x + rX;
                int ty = y + rY;
                
                bool enSalidaValida = false;
                for (int i = 0; i < numSalidas; i++) {
                    // Verificamos: ¿Esta celda es parte de la salida en este tiempo G?
                    // ¿Y el color coincide?
                    if (salidas[i]->esParteDeSalida(ty, tx, tiempoG) && 
                        tolower(salidas[i]->getColor()) == tolower(b->getColor())) {
                        enSalidaValida = true;
                        break;
                    }
                }
                if (!enSalidaValida) return false; 
            }
        }
    }
    return true; 
}