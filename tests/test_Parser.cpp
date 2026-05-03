#include <iostream>
#include "Parser.h"
#include "Tablero.h"

int main() {
    // 1. Usar el nombre de función correcto: cargarEscenario
    Tablero* tab = Parser::cargarEscenario("simple1.txt");

    if (tab != nullptr) {
        std::cout << "[OK] Archivo leido correctamente." << std::endl;
        
        // 2. Usar los nombres de tus variables o getters en Tablero
        // Si no tienes getAncho(), usa directamente los miembros o agrégalos
        std::cout << "Dimensiones cargadas: " << tab->getWidth() << "x" << tab->getHeight() << std::endl;        
        // 3. Limpieza de memoria (importante para la rúbrica)
        delete tab;
    } else {
        std::cerr << "[ERROR] No se pudo cargar el escenario. Revisa la ruta del archivo." << std::endl;
    }
    return 0;
}