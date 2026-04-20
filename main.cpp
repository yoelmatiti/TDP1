#include <iostream>
#include "LectorArchivo.h"
#include "Solver.h"
#include "State.h"

int main() {
    // 1. Cargar el nivel desde el TXT
    Tablero* inicial = LectorArchivo::cargarNivel("nivel1.txt");
    
    if (!inicial) {
        std::cerr << "Error al cargar el nivel." << std::endl;
        return 1;
    }

    // 2. Configurar el Solver
    Solver solver;
    std::cout << "Buscando solucion..." << std::endl;

    // 3. Resolver
    State* resultado = solver.solve(inicial);

    // 4. Resultado
    if (resultado) {
        std::cout << "¡Exito! Pasos: " << resultado->getG() << std::endl;
        resultado->printOperaciones();
    } else {
        std::cout << "No se encontro solucion dentro del limite de pasos." << std::endl;
    }

    delete inicial; // Limpieza
    return 0;
}