#include "Compuerta.h"
#include <cctype>
#include <cstring> // <--- AGREGADO para strlen y strcpy

// Constructor por defecto
Compuerta::Compuerta() : x(0), y(0), orientacion('V'), cicloColores(nullptr), numColores(0), pasoCambio(0) {}

// Constructor con parámetros (Corregido el orden para evitar warnings)
Compuerta::Compuerta(int _x, int _y, int _p, const char* _ciclo) 
    : x(_x), y(_y), orientacion('V'), cicloColores(nullptr), numColores(0), pasoCambio(_p) {
    
    if (_ciclo != nullptr) {
        numColores = strlen(_ciclo);
        cicloColores = new char[numColores + 1];
        strcpy(cicloColores, _ciclo);
    }
}

// Copia profunda
Compuerta::Compuerta(const Compuerta& otro) : cicloColores(nullptr) {
    *this = otro;
}

Compuerta& Compuerta::operator=(const Compuerta& otro) {
    if (this != &otro) {
        delete[] cicloColores; 
        
        x = otro.x;
        y = otro.y;
        orientacion = otro.orientacion;
        pasoCambio = otro.pasoCambio;
        numColores = otro.numColores;
        
        if (otro.cicloColores) {
            cicloColores = new char[numColores + 1];
            for (int i = 0; i < numColores; i++) {
                cicloColores[i] = otro.cicloColores[i];
            }
            cicloColores[numColores] = '\0'; 
        } else {
            cicloColores = nullptr;
        }
    }
    return *this;
}

Compuerta::~Compuerta() {
    delete[] cicloColores;
}

char Compuerta::getColorActual(int tiempoG) const {
    if (!cicloColores || numColores <= 0) return ' '; 
    if (pasoCambio <= 0) return cicloColores[0];
    return cicloColores[(tiempoG / pasoCambio) % numColores];
}

void Compuerta::agregarColorAlCiclo(char c) {
    char* nuevoCiclo = new char[numColores + 2]; 
    for (int i = 0; i < numColores; i++) {
        nuevoCiclo[i] = cicloColores[i];
    }
    nuevoCiclo[numColores] = c;          
    nuevoCiclo[numColores + 1] = '\0';   
    
    delete[] cicloColores;
    cicloColores = nuevoCiclo;
    numColores++;
}

bool Compuerta::verificarDimension(int anchoBloque, int altoBloque) const {
    if (orientacion == 'H') return anchoBloque == 1; 
    if (orientacion == 'V') return altoBloque == 1; 
    return false;
}

// --- SE ELIMINÓ LA SEGUNDA DEFINICIÓN DE agregarColorAlCiclo QUE ESTABA AQUÍ ---

bool Compuerta::puedePasar(char colorBloque, int paso) const {
    if (colorBloque == '?') return true; 
    char colorCompuerta = getColorActual(paso);
    return std::tolower((unsigned char)colorBloque) == std::tolower((unsigned char)colorCompuerta);
}

int Compuerta::getX() const { return x; }
int Compuerta::getY() const { return y; }
char Compuerta::getOrientacion() const { return orientacion; }
char Compuerta::getColor() const { return getColorActual(0); }

void Compuerta::calcularDestino(int bloqueX, int bloqueY, int anchoB, int altoB, Direccion dir, int& outX, int& outY) const {
    outX = bloqueX;
    outY = bloqueY;
    if (dir == Direccion::U) outY -= (altoB + 1);
    else if (dir == Direccion::D) outY += (altoB + 1);
    else if (dir == Direccion::L) outX -= (anchoB + 1);
    else if (dir == Direccion::R) outX += (anchoB + 1);
}