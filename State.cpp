#include "State.h"
#include <iostream>
#include <cstring>

// Constructor por defecto
State::State() {
    this->numBloques = 0;
    this->posiciones = nullptr;
    this->operacion = nullptr;
    this->padre = nullptr;
    this->g = 0;
    this->h = 0;
    this->f = 0;
}

// Constructor requerido por Solver y Movimiento (6 parámetros)
State::State(int nBloques, PosBloque* posNuevas, int g_val, int h_val, State* p, const char* op) {
    this->numBloques = nBloques;
    this->g = g_val;
    this->h = h_val;
    this->f = (float)(g_val + h_val);
    this->padre = p;

    if (posNuevas && nBloques > 0) {
        this->posiciones = new PosBloque[nBloques];
        for (int i = 0; i < nBloques; i++) {
            this->posiciones[i] = posNuevas[i];
        }
    } else {
        this->posiciones = nullptr;
    }

    if (op) {
        this->operacion = new char[strlen(op) + 1];
        strcpy(this->operacion, op);
    } else {
        this->operacion = nullptr;
    }
}

// Constructor de Copia (Requerido por test_State.cpp)
State::State(const State& otro) {
    this->numBloques = otro.numBloques;
    this->g = otro.g;
    this->h = otro.h;
    this->f = otro.f;
    this->padre = otro.padre;

    if (otro.posiciones) {
        this->posiciones = new PosBloque[numBloques];
        for (int i = 0; i < numBloques; i++) {
            this->posiciones[i] = otro.posiciones[i];
        }
    } else {
        this->posiciones = nullptr;
    }

    if (otro.operacion) {
        this->operacion = new char[strlen(otro.operacion) + 1];
        strcpy(this->operacion, otro.operacion);
    } else {
        this->operacion = nullptr;
    }
}

State::~State() {
    delete[] posiciones;
    delete[] operacion;
}

// getHash: Requerido por TablaHash.cpp para eficiencia O(log N) o O(1) [cite: 249]
long long State::getHash() const {
    unsigned long long h_val = 17;
    for (int i = 0; i < numBloques; i++) {
        h_val = h_val * 31 + posiciones[i].x;
        h_val = h_val * 31 + posiciones[i].y;
        h_val = h_val * 31 + (posiciones[i].activo ? 1 : 0);
    }
    return (long long)h_val;
}

bool State::esFinal() {
    for (int i = 0; i < numBloques; i++) {
        if (posiciones[i].activo) return false;
    }
    return true;
}

void State::print() {
    std::cout << "F: " << f << " | Op: " << (operacion ? operacion : "None") << std::endl;
}

void State::printOperaciones() {
    if (padre != nullptr) {
        padre->printOperaciones();
    }
    print();
}

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