#include <iostream>
#include "Bloque.h"

void testBasicos() {
    std::cout << "--- Test 1: Creacion y Propiedades Basicas ---" << std::endl;
    
    // ID=1, Color='a', X=4, Y=4, Width=2, Height=2
    // 1. Crear arreglo de forma (cuadrado 2x2)
    bool* forma = new bool[4]{true, true, true, true}; 

    // 2. Pasar como 7mo argumento al constructor
    Bloque b1(1, 'a', 4, 4, 2, 2, forma); 

    // 3. Liberar memoria temporal (b1 ya hizo su propia copia interna)
    delete[] forma;
    
    if (b1.getX() == 4 && b1.getY() == 4 && b1.getColor() == 'a') {
        std::cout << "[OK] Propiedades iniciales correctas." << std::endl;
    } else {
        std::cout << "[ERROR] Propiedades iniciales incorrectas." << std::endl;
    }
}

void testGeometriaYMovimiento() {
    std::cout << "\n--- Test 2: Geometria y Movimiento ---" << std::endl;
    
    // Definimos una geometría inicial vacía (todos false) para luego usar setGeometria
    bool* formaVacia = new bool[4]{false, false, false, false};
    Bloque b1(1, 'b', 0, 0, 2, 2, formaVacia);
    delete[] formaVacia; 
    
    // Probar setGeometria (mapeo 1D interno)
    b1.setGeometria(0, 0, true);
    b1.setGeometria(0, 1, true);
    b1.setGeometria(1, 0, true);
    b1.setGeometria(1, 1, true);

    std::cout << "Esta en (0,0)? " << (b1.ocupaCelda(0, 0) ? "SI" : "NO") << " (Esperado: SI)" << std::endl;
    
    // Probar movimiento
    b1.mover(2, 3); // dx=2, dy=3 -> Nueva pos (2,3)
    
    if (b1.getX() == 2 && b1.getY() == 3) {
        std::cout << "[OK] Movimiento realizado a (2,3)." << std::endl;
        std::cout << "Ocupa ahora (2,3)? " << (b1.ocupaCelda(2, 3) ? "SI" : "NO") << " (Esperado: SI)" << std::endl;
    } else {
        std::cout << "[ERROR] Fallo en el desplazamiento." << std::endl;
    }
}

void testMemoria() {
    std::cout << "\n--- Test 3: Gestion de Memoria (Copia) ---" << std::endl;
    
    bool* formaZ = new bool[4]{true, false, false, true};
    // Creamos el original en el heap
    Bloque* original = new Bloque(5, 'z', 1, 1, 2, 2, formaZ);
    delete[] formaZ;
    
    // Probar constructor de copia (Deep Copy)
    // Esto es lo que usara el Algoritmo A* para generar sucesores
    Bloque copia = *original; 
    
    // Borramos el original. Si la copia es "Shallow", esto hara que 'copia' falle
    delete original; 
    
    if (copia.getGeometria(0, 0) == true && copia.getColor() == 'z') {
        std::cout << "[OK] Deep Copy exitosa. La memoria de la copia es independiente." << std::endl;
    } else {
        std::cout << "[ERROR] Error en constructor de copia (posible puntero colgado)." << std::endl;
    }
}

int main() {
    try {
        testBasicos();
        testGeometriaYMovimiento();
        testMemoria();
        std::cout << "\n>>> ¡Todos los tests de Bloque finalizaron exitosamente!" << std::endl;
    } catch (...) {
        std::cout << "Ocurrio un error inesperado durante las pruebas." << std::endl;
    }
    return 0;
}