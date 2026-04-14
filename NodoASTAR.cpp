#include "NodoASTAR.h"

NodoASTAR::NodoASTAR(Tablero* t, int _g, int _h, NodoASTAR* p) 
    : tablero(t), g(_g), h(_h), padre(p) {
    f = g + h;
}

NodoASTAR::~NodoASTAR() {
    // Este destructor es VITAL para la integridad de memoria en A*
    // Cada NodoASTAR es propietario exclusivo de su Tablero.
    // Cuando el nodo se destruye (por ser descartado en la búsqueda),
    // debe liberar su Tablero para evitar fugas de 500MB+ en pocos segundos.
    if (tablero != nullptr) {
        delete tablero;
        tablero = nullptr;  // Null out para mayor seguridad
    }
    // padre NO se borra aquí: es responsabilidad de otro componente
}