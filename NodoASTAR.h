#ifndef NODOASTAR_H
#define NODOASTAR_H

#include "Tablero.h"

class NodoASTAR {
public:
    Tablero* tablero;
    int g;
    int h;
    int f;
    NodoASTAR* padre;

    NodoASTAR(Tablero* t, int _g, int _h, NodoASTAR* p);
    
    // Destructor: RESPONSABLE de liberar el Tablero* asociado
    // Cada NodoASTAR es propietario de su Tablero y debe destruirlo
    // Esto es CRÍTICO para evitar fugas masivas de memoria en A*
    ~NodoASTAR();
};

#endif