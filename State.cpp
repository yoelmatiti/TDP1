#include "State.h"
#include <cstring>
#include <iostream>
#include <algorithm> // Necesario para std::swap

/**
 * Constructor Principal
 */
State::State(int nBloques, PosBloque* posNuevas, int g_val, int h_val, State* p, const char* op) 
    : posiciones(nullptr), 
      numBloques(nBloques), 
      g(g_val), 
      h(h_val), 
      f(g_val + h_val), 
      padre(p), 
      operacion(nullptr) 
{
    // 1. Copia profunda de las posiciones
    if (posNuevas && numBloques > 0) {
        this->posiciones = new PosBloque[numBloques];
        for (int i = 0; i < numBloques; i++) {
            this->posiciones[i] = posNuevas[i];
        }
    }

    // 2. Copia profunda de la cadena operacion
    if (op) {
        this->operacion = new char[strlen(op) + 1];
        strcpy(this->operacion, op);
    }
}

/**
 * Constructor de Copia (Deep Copy)
 * Invocado automáticamente cuando se pasa un objeto por valor.
 */
State::State(const State& otro) 
    : posiciones(nullptr),
      numBloques(otro.numBloques),
      g(otro.g),
      h(otro.h),
      f(otro.f),
      padre(otro.padre),
      operacion(nullptr)
{
    if (otro.posiciones && otro.numBloques > 0) {
        this->posiciones = new PosBloque[numBloques];
        for (int i = 0; i < numBloques; i++) {
            this->posiciones[i] = otro.posiciones[i];
        }
    }

    if (otro.operacion) {
        this->operacion = new char[strlen(otro.operacion) + 1];
        strcpy(this->operacion, otro.operacion);
    }
}

/**
 * Función swap (No-throw)
 * Intercambia los punteros y valores primitivos. Es el núcleo de la seguridad.
 */
void State::swap(State& primero, State& segundo) noexcept {
    using std::swap; // Habilita ADL (Argument Dependent Lookup)
    swap(primero.posiciones, segundo.posiciones);
    swap(primero.operacion, segundo.operacion);
    swap(primero.numBloques, segundo.numBloques);
    swap(primero.g, segundo.g);
    swap(primero.h, segundo.h);
    swap(primero.f, segundo.f);
    swap(primero.padre, segundo.padre);
}

/**
 * Operador de Asignación (Copy-and-Swap)
 * Al recibir 'otro' por valor, se crea una copia temporal automáticamente.
 */
State& State::operator=(State otro) {
    // Intercambiamos los datos de 'this' con la copia temporal 'otro'
    swap(*this, otro);
    
    // Al salir de la función, 'otro' (que ahora tiene los datos viejos de 'this')
    // se destruye automáticamente invocando el destructor, limpiando la memoria vieja.
    return *this;
}

/**
 * Destructor
 */
State::~State() {
    delete[] posiciones;
    delete[] operacion;
}

/**
 * Firma numérica (Hash) basada en primos
 */
/**
 * Firma numérica (Hash) basada en la fórmula polinomial de Java/C#
 * Es robusta para múltiples bloques y distingue el orden de las posiciones.
 */
long long State::getHash() const {
    // Usamos unsigned para evitar comportamientos indefinidos por desbordamiento
    unsigned long long h_val = 17; 

    for (int i = 0; i < numBloques; i++) {
        // Multiplicamos por 31 (un primo común para hashes) en cada paso
        h_val = h_val * 31 + (unsigned int)posiciones[i].x;
        h_val = h_val * 31 + (unsigned int)posiciones[i].y;
        
        // Incluimos el estado activo/inactivo para que el hash cambie 
        // drásticamente cuando un bloque sale del tablero.
        h_val = h_val * 31 + (posiciones[i].activo ? 1 : 0);

        // OPCIONAL: Si tus bloques tienen IDs únicos y quieres ser extra precavido, 
        // podrías añadir el ID si PosBloque lo contiene, pero con el índice del bucle
        // ya se suele diferenciar suficientemente el "rol" de cada bloque.
        h_val = h_val * 31 + (unsigned int)i; 
    }

    return (long long)h_val;
}

/**
 * Operador de Igualdad
 */
bool State::operator==(const State& other) const {
    if (this->numBloques != other.numBloques) return false;
    for (int i = 0; i < numBloques; i++) {
        if (posiciones[i].x != other.posiciones[i].x ||
            posiciones[i].y != other.posiciones[i].y ||
            posiciones[i].activo != other.posiciones[i].activo) {
            return false;
        }
    }
    return true;
}

/**
 * Impresión recursiva del camino
 */
void State::printOperaciones() {
    if (padre != nullptr) {
        padre->printOperaciones();
    }
    
    if (operacion) {
        std::cout << " -> " << operacion << " (g=" << g << ")" << std::endl;
    } else {
        std::cout << "Estado Inicial" << std::endl;
    }
}
/**
 * Verifica si una coordenada (x, y) está ocupada por algún bloque activo,
 * ignorando un bloque específico (el que se está intentando mover).
 */
bool State::estaOcupado(int x, int y, int bloqueIgnorado) const {
    // Si no hay posiciones guardadas, no hay nada que verificar
    if (!posiciones) return false;

    for (int i = 0; i < numBloques; i++) {
        // 1. Ignoramos el bloque que realiza la consulta
        if (i == bloqueIgnorado) continue;

        // 2. Verificamos si el bloque está activo en este estado
        if (posiciones[i].activo) {
            // 3. Comparamos coordenadas
            // Nota: Esto asume bloques de 1x1. Si usas geometrías complejas,
            // esta función es una validación rápida de "punto de anclaje".
            if (posiciones[i].x == x && posiciones[i].y == y) {
                return true;
            }
        }
    }

    return false; // La celda está libre de otros bloques
}