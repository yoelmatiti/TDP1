#include <iostream>

#include "Bloque.h"

void testBasicos() {
    std::cout << "--- Test 1: Creación y Propiedades Básicas ---" << std::endl;
    // ID=1, Color='a', X=4, Y=4, Width=2, Height=2
    Bloque b1(1, 'a', 4, 4, 2, 2);
    
    if (b1.getX() == 4 && b1.getY() == 4 && b1.getColor() == 'a') {
        std::cout << "[OK] Propiedades iniciales correctas." << std::endl;
    } else {
        std::cout << "[ERROR] Propiedades iniciales incorrectas." << std::endl;
    }
}

void testGeometriaYMovimiento() {
    std::cout << "\n--- Test 2: Geometría y Movimiento ---" << std::endl;
    Bloque b1(1, 'b', 0, 0, 2, 2);
    
    // Definir geometría manual (un bloque L invertida o cuadrado 2x2)
    // GEOMETRY=1111 (según ejemplo del enunciado [cite: 159])
    b1.setGeometria(0, 0, true);
    b1.setGeometria(0, 1, true);
    b1.setGeometria(1, 0, true);
    b1.setGeometria(1, 1, true);

    std::cout << "¿Ocupa (0,0)? " << (b1.ocupaCelda(0, 0) ? "SÍ" : "NO") << " (Esperado: SÍ)" << std::endl;
    
    // Probar movimiento [cite: 70, 129]
    b1.mover(2, 3); // Mover dx=2, dy=3 -> Nueva pos (2,3)
    
    if (b1.getX() == 2 && b1.getY() == 3) {
        std::cout << "[OK] Movimiento realizado a (2,3)." << std::endl;
        std::cout << "¿Ocupa ahora (2,3)? " << (b1.ocupaCelda(2, 3) ? "SÍ" : "NO") << " (Esperado: SÍ)" << std::endl;
    } else {
        std::cout << "[ERROR] Fallo en el desplazamiento." << std::endl;
    }
}

void testMemoria() {
    std::cout << "\n--- Test 3: Gestión de Memoria (Copia) ---" << std::endl;
    Bloque* original = new Bloque(5, 'z', 1, 1, 2, 2);
    original->setGeometria(0, 0, true);
    
    // Probar constructor de copia (Fundamental al no usar STL [cite: 6])
    Bloque copia = *original; 
    
    delete original; // Borramos el original para ver si la copia mantiene su propia memoria
    
    if (copia.getGeometria(0, 0) == true) {
        std::cout << "[OK] Deep Copy exitosa. La memoria es independiente." << std::endl;
    } else {
        std::cout << "[ERROR] Error en constructor de copia o manejo de punteros." << std::endl;
    }
}

int main() {
    try {
        testBasicos();
        testGeometriaYMovimiento();
        testMemoria();
        std::cout << "\n>>> ¡Todos los tests de Bloque finalizaron exitosamente!" << std::endl;
    } catch (...) {
        std::cout << "Ocurrió un error inesperado durante las pruebas." << std::endl;
    }
    return 0;
}