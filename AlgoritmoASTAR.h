#ifndef ALGORITMOASTAR_H
#define ALGORITMOASTAR_H

#include "NodoASTAR.h"
#include "COLAPRIORIDAD.h"
#include "TablaHash.h"

class AlgoritmoASTAR {
private:
    ColaPrioridad* abierta;
    TablaHash* cerrada;
    NodoASTAR* solucion;
    int nodoGenerados;
    int nodoExpandidos;
    
    NodoASTAR** todosLosNodos;
    int capacidadNodos;
    int cantidadNodos;
    
    int calcularHeuristica(const Tablero& t);
    void generarVecinos(NodoASTAR* nodoPadre);
    NodoASTAR* reconstruirCamino(NodoASTAR* nodoFinal);
    void limpiar();
    bool esAncestroDe(NodoASTAR* posibleAncestro, NodoASTAR* nodo);
    
public:
    AlgoritmoASTAR();
    ~AlgoritmoASTAR();
    
    bool resolver(Tablero* estadoInicial, int maxNodos);
    
    NodoASTAR* getSolucion() const { return solucion; }
    int getNodoGenerados() const { return nodoGenerados; }
    int getNodoExpandidos() const { return nodoExpandidos; }
};

#endif