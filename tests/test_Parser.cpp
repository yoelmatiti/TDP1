#include <iostream>
#include "Parser.h"
#include "Tablero.h"

int main() {
    std::cout << "Intentando cargar archivo: simple1" << std::endl;
    
    // Usamos ../ porque el ejecutable corre desde la carpeta 'tests'
    Tablero* t = Parser::cargarEscenario("../simple1");

    if (t == nullptr) {
        std::cerr << "[ERROR] No se pudo abrir o leer el archivo 'simple1'." << std::endl;
        return 1;
    }

    std::cout << "--- Datos cargados del Tablero ---" << std::endl;
    std::cout << "Ancho: " << t->getAncho() << std::endl;
    std::cout << "Alto: " << t->getAlto() << std::endl;
    
    // Verificar si el bloque 1 se cargo
    if (t->getBloquePorId(1) != nullptr) {
        std::cout << "[OK] Bloque 1 detectado." << std::endl;
    } else {
        std::cout << "[ERROR] Bloque 1 no encontrado en el tablero." << std::endl;
    }

    delete t;
    return 0;
}