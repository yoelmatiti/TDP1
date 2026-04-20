#include "State.h"
#include <cstring>

// Constructor optimizado: calcula f al nacer
State::State(Tablero* tablero, int g_val, int h_val, State* p, const char* op) 
    : t(tablero), padre(p), g(g_val), h(h_val) {
    f = g + h;
    
    if (op) {
        operacion = new char[strlen(op) + 1];
        strcpy(operacion, op);
    } else {
        operacion = nullptr;
    }
}

State::~State() {
    if (t) delete t; // El estado es dueño de su tablero
    if (operacion) delete[] operacion;
}

// Método del profesor para imprimir el camino recursivamente
void State::printOperaciones() {
    if (padre != nullptr) {
        padre->printOperaciones();
    }
    if (operacion) {
        std::cout << " -> " << operacion << std::endl;
    } else {
        std::cout << "Estado Inicial" << std::endl;
    }
}