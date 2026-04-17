#include <iostream>
#include "AlgoritmoASTAR.h"
#include "Tablero.h"

int main(int argc, char* argv[]) {
    std::cout << "--- Buscador de Soluciones A* ---" << std::endl;

    // 1. Aquí deberías llamar a tu Parser para cargar el tablero desde un archivo
    // Por ahora, para probar que compile, creamos un puntero nulo
    Tablero* tableroInicial = nullptr; 

    AlgoritmoASTAR solver;
    
    std::cout << "Iniciando busqueda..." << std::endl;
    
    // Intentar resolver (esto fallara ahora porque el tablero es null, 
    // pero nos sirve para probar el enlazado de archivos)
    bool exito = solver.resolver(tableroInicial, 10000);

    if (exito) {
        std::cout << "Solucion encontrada!" << std::endl;
    } else {
        std::cout << "No se encontro solucion o el tablero estaba vacio." << std::endl;
    }

    return 0;
}