#include "State.h"

State::State(Tablero* tablero, int g_val, int h_val, State* p) 
    : t(tablero), padre(p), g(g_val), h(h_val) {
    f = g + h;
}

State::~State() {
    if (t) delete t; // Gestión de memoria manual
}