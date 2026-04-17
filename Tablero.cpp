#include "Tablero.h"
#include <cstdio> // Para sprintf

// Constructor por defecto inicializando punteros a null
Tablero::Tablero() : width(0), height(0), matriz(nullptr), g(0), h(0), 
                     bloques(nullptr), numBloques(0), bloquesRestantes(0), 
                     salidas(nullptr), numSalidas(0), portales(nullptr), 
                     numPortales(0), representacion(nullptr) {}

// Destructor: Limpia la memoria dinámica [cite: 122]
Tablero::~Tablero() {
    liberarMemoria();
}

void Tablero::liberarMemoria() {
    if (matriz != nullptr) {
        for (int i = 0; i < height; i++) {
            delete[] matriz[i];
        }
        delete[] matriz;
        matriz = nullptr;
    }
    delete[] bloques;
    delete[] salidas;
    delete[] portales;
    delete[] representacion;
    representacion = nullptr;
}

// Constructor de Copia: Clave para el Algoritmo A* [cite: 18]
Tablero::Tablero(const Tablero& otra) {
    copiarDesde(otra);
}

// Operador de Asignación
Tablero& Tablero::operator=(const Tablero& otra) {
    if (this != &otra) {
        liberarMemoria();
        copiarDesde(otra);
    }
    return *this;
}

void Tablero::copiarDesde(const Tablero& otra) {
    this->width = otra.width;
    this->height = otra.height;
    this->g = otra.g;
    this->h = otra.h;
    this->bloquesRestantes = otra.bloquesRestantes;
    this->numBloques = otra.numBloques;
    this->numSalidas = otra.numSalidas;
    this->numPortales = otra.numPortales;
    this->representacion = nullptr;

    // Copiar la matriz de caracteres (Deep Copy)
    if (otra.matriz != nullptr) {
        this->matriz = new char*[height];
        for (int i = 0; i < height; i++) {
            this->matriz[i] = new char[width];
            for (int j = 0; j < width; j++) {
                this->matriz[i][j] = otra.matriz[i][j];
            }
        }
    } else {
        this->matriz = nullptr;
    }

    // Copiar arreglos de objetos (Asumiendo que Bloque, Salida y Portal 
    // tienen sus propios operadores de asignación)
    if (otra.bloques != nullptr) {
        this->bloques = new Bloque[numBloques];
        for (int i = 0; i < numBloques; i++) this->bloques[i] = otra.bloques[i];
    }
    if (otra.salidas != nullptr) {
        this->salidas = new Salida[numSalidas];
        for (int i = 0; i < numSalidas; i++) this->salidas[i] = otra.salidas[i];
    }
    if (otra.portales != nullptr) {
        this->portales = new Portal[numPortales];
        for (int i = 0; i < numPortales; i++) this->portales[i] = otra.portales[i];
    }
}

bool Tablero::esEstadoFinal() const {
    // El juego termina si todos los bloques salieron [cite: 116]
    // Esto depende de tu lógica interna, por ejemplo si bloquesRestantes llega a 0
    return bloquesRestantes == 0;
}

const char* Tablero::getRepresentacion() const {
    if (representacion == nullptr) {
        generarRepresentacion();
    }
    return representacion;
}

void Tablero::generarRepresentacion() const {
    // Estimar tamaño: matriz (width*height) + portales (numPortales * 10) + salidas (numSalidas * 10) + separadores
    int tam = width * height + numPortales * 10 + numSalidas * 10 + 100;
    representacion = new char[tam];
    int pos = 0;

    // Incluir la matriz
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            representacion[pos++] = matriz[i][j];
        }
    }
    representacion[pos++] = '|'; // Separador

    // Incluir largos de salidas
    for (int i = 0; i < numSalidas; i++) {
        int largo = salidas[i].getLongitudActual(0); // Usar longitud inicial
        pos += sprintf(representacion + pos, "%d,", largo);
    }
    representacion[pos++] = '|'; // Separador

    // Incluir colores de portales
    for (int i = 0; i < numPortales; i++) {
        char color = portales[i].getColor();
        pos += sprintf(representacion + pos, "%c,", color);
    }
    representacion[pos] = '\0'; // Terminar cadena
}

void Tablero::agregarPortal(Portal* portal) {
    // Implementación básica: redimensionar arreglo
    Portal* nuevo = new Portal[numPortales + 1];
    for (int i = 0; i < numPortales; i++) nuevo[i] = portales[i];
    nuevo[numPortales] = *portal;
    delete[] portales;
    portales = nuevo;
    numPortales++;
}

int Tablero::moverBloque(int id, Direccion dir, int celdas) {
    if (id < 0 || id >= numBloques) return 0;
    
    // Si el bloque no está activo, no se mueve
    if (!bloques[id].estaActivo()) return 0;

    int distanciaMax = 0;
    for (int dist = 1; dist <= celdas; dist++) {
        if (esMovimientoValido(id, dir, dist)) {
            distanciaMax = dist;
        } else {
            break;
        }
    }

    if (distanciaMax > 0) {
        aplicarMovimiento(id, dir, distanciaMax);
    }

    return distanciaMax;
}

bool Tablero::esMovimientoValido(int bloqueID, Direccion dir, int celdas) const {
    if (bloqueID < 0 || bloqueID >= numBloques) return false;
    const Bloque& bloque = bloques[bloqueID];

    // Calcular nueva posición del anclaje
    int dx = 0, dy = 0;
    switch (dir) {
        case Direccion::U: dy = -1; break;
        case Direccion::D: dy = 1; break;
        case Direccion::L: dx = -1; break;
        case Direccion::R: dx = 1; break;
    }
    int nuevaX = bloque.getX() + dx * celdas;
    int nuevaY = bloque.getY() + dy * celdas;

    // Verificar geometría: todas las celdas del bloque deben estar libres
    for (int relY = 0; relY < bloque.getAltoGeo(); relY++) {
        for (int relX = 0; relX < bloque.getAnchoGeo(); relX++) {
            if (!bloque.getGeometria(relY, relX)) continue; // Celda no ocupada por el bloque

            int absY = nuevaY + relY;
            int absX = nuevaX + relX;

            // Dentro del tablero
            if (absY < 0 || absY >= height || absX < 0 || absX >= width) return false;

            // No pared
            if (matriz[absY][absX] == '#') return false;

            // No otro bloque ocupando la celda
            for (int otroID = 0; otroID < numBloques; otroID++) {
                if (otroID == bloqueID) continue;
                if (bloques[otroID].ocupaCelda(absY, absX)) return false;
            }

            // Portales: verificar color
            bool hayPortal = false;
            for (int p = 0; p < numPortales; p++) {
                if (portales[p].getX() == absX && portales[p].getY() == absY) {
                    hayPortal = true;
                    if (!portales[p].puedePasar(bloque.getColor(), g)) return false;
                    break;
                }
            }
            // Si no hay portal, la celda debe estar vacía (excepto si es portal)
            if (!hayPortal && matriz[absY][absX] != ' ') return false;
        }
    }

    return true;
}

void Tablero::verificarRevelacionIncognitos() {
    int coloresActivos = 0;
    for (int i = 0; i < numBloques; i++) {
        // Contamos cuántos bloques con color definido siguen en el tablero
        if (bloques[i].estaActivo() && !bloques[i].getEsIncognito()) {
            coloresActivos++;
        }
    }

    // Si ya no quedan bloques de colores normales
    if (coloresActivos == 0) {
        for (int i = 0; i < numBloques; i++) {
            if (bloques[i].estaActivo() && bloques[i].getEsIncognito()) {
                // CORRECCIÓN: Llamada sin argumentos
                bloques[i].revelarColor(); 
            }
        }
    }
}

void Tablero::aplicarMovimiento(int bloqueID, Direccion dir, int distancia) {
    Bloque& bloque = bloques[bloqueID];
    
    int dx = 0, dy = 0;
    // ... tu switch de direcciones ...
    
    bloque.mover(dx * distancia, dy * distancia);
    this->g += distancia; // El tiempo avanza con cada celda recorrida

    // VERIFICAR SALIDA
    for (int s = 0; s < numSalidas; s++) {
        // Si el bloque de color 'A' toca la salida de color 'A'
        if (salidas[s].getColor() == bloque.getColor()) {
            // Verificar si la geometría del bloque coincide con la posición de la salida
            if (bloque.ocupaCelda(salidas[s].getY(), salidas[s].getX())) {
                bloque.setActivo(false); // Función que agregamos antes
                bloque.mover(-100, -100); // Sacar del mapa
                bloquesRestantes--;
                
                // REVELAR INCOGNITOS si era el último de color
                verificarRevelacionIncognitos(); 
                break;
            }
        }
    }
    
    // Al cambiar la posición, la representación antigua ya no sirve
    if (representacion != nullptr) {
        delete[] representacion;
        representacion = nullptr;
    }
}

void Tablero::actualizarCompuertas() {
    // Actualizar portales si es necesario (cambio de color con tiempo)
    // Por ahora vacío
}

void Tablero::actualizarSalidas() {
    // Actualizar salidas si cambian con tiempo
    // Por ahora vacío
}
