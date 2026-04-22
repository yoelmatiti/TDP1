#include <iostream>
#include "Tablero.h"

void testCargaYColision() {
    std::cout << "--- Test Tablero: Carga y Física ---" << std::endl;
    
    Tablero tab;
    // Asumiendo que tienes un método cargarArchivo
    if (!tab.cargarArchivo("mapa_simple.txt")) {
        std::cerr << "[ERROR] No se pudo cargar el archivo de prueba." << std::endl;
        return;
    }

    std::cout << "Tablero cargado con éxito." << std::endl;

    // Test 1: Colisión con pared
    // Intentar mover un bloque hacia una pared '#'
    bool movValido = tab.intentarMover(1, "ARRIBA"); 
    if (!movValido) {
        std::cout << "[OK] El sistema bloqueó el movimiento contra una pared." << std::endl;
    } else {
        std::cout << "[ERROR] El bloque atravesó una pared." << std::endl;
    }

    // Test 2: Portales (Compuertas)
    // Intentar mover un bloque de color 'a' hacia un portal 'B' (diferente color)
    if (!tab.intentarMover(1, "DERECHA")) {
        std::cout << "[OK] El sistema bloqueó el paso por un portal de distinto color." << std::endl;
    }
}

int main() {
    testCargaYColision();
    return 0;
}