#include "Tablero.h"
#include <cstdio> // Para sprintf

// Constructor por defecto inicializando punteros a null
Tablero::Tablero() : width(0), height(0), matriz(nullptr), g(0), h(0), 
                     bloques(nullptr), numBloques(0), salidas(nullptr), 
                     numSalidas(0), compuertas(nullptr), numCompuertas(0), 
                     representacion(nullptr) {}

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
    // Nota: bloques, salidas y compuertas también deben ser liberados 
    // si fueron creados con 'new[]'
    delete[] bloques;
    delete[] salidas;
    delete[] compuertas;
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
    this->numCompuertas = otra.numCompuertas;
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

    // Copiar arreglos de objetos (Asumiendo que Bloque, Salida y Compuerta 
    // tienen sus propios operadores de asignación)
    if (otra.bloques != nullptr) {
        this->bloques = new Bloque[numBloques];
        for (int i = 0; i < numBloques; i++) this->bloques[i] = otra.bloques[i];
    }
    // ... repetir para salidas y compuertas ...
}

bool Tablero::esEstadoFinal() const {
    // El juego termina si todos los bloques salieron [cite: 116]
    // Esto depende de tu lógica interna, por ejemplo si numBloques llega a 0
    return numBloques == 0;
}

const char* Tablero::getRepresentacion() const {
    if (representacion == nullptr) {
        generarRepresentacion();
    }
    return representacion;
}

void Tablero::generarRepresentacion() const {
    // Estimar tamaño: matriz (width*height) + compuertas (numCompuertas * 10) + salidas (numSalidas * 10) + separadores
    int tam = width * height + numCompuertas * 10 + numSalidas * 10 + 100;
    representacion = new char[tam];
    int pos = 0;

    // Incluir la matriz
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            representacion[pos++] = matriz[i][j];
        }
    }
    representacion[pos++] = '|'; // Separador

    // Incluir colores de compuertas
    for (int i = 0; i < numCompuertas; i++) {
        // Asumiendo Compuerta tiene int color;
        int color = compuertas[i].color; // Ajusta según la estructura real
        pos += sprintf(representacion + pos, "%d,", color);
    }
    representacion[pos++] = '|'; // Separador

    // Incluir largos de salidas
    for (int i = 0; i < numSalidas; i++) {
        // Asumiendo Salida tiene int largo;
        int largo = salidas[i].largo; // Ajusta según la estructura real
        pos += sprintf(representacion + pos, "%d,", largo);
    }
    representacion[pos] = '\0'; // Terminar cadena
}