#include <iostream>
#include "Heap.h"
#include "State.h"

/**
 * TEST_HEAP
 * Este programa valida que el montículo mantenga la prioridad f(n) = g + h.
 * Según la rúbrica, el pop() debe ser O(log N).
 */

void testPrioridadMinima() {
    std::cout << "Iniciando Test de Prioridad (Min-Heap)..." << std::endl;
    
    // Creamos un Heap con capacidad inicial pequeña para forzar redimensionar()
    Heap miHeap(5); 

    // Insertamos estados con diferentes valores de f (g + h)
    // Constructor: State(Tablero*, g, h, Padre, Operacion)
    std::cout << "-> Insertando f=50, f=10, f=30, f=5, f=20..." << std::endl;
    
    miHeap.push(new State(nullptr, 50, 0, nullptr, "M1")); // f=50
    miHeap.push(new State(nullptr, 5, 5, nullptr, "M2"));  // f=10
    miHeap.push(new State(nullptr, 30, 0, nullptr, "M3")); // f=30
    miHeap.push(new State(nullptr, 2, 3, nullptr, "M4"));  // f=5
    miHeap.push(new State(nullptr, 10, 10, nullptr, "M5"));// f=20

    std::cout << "-> Extrayendo elementos (esperado orden ascendente):" << std::endl;

    int anteriorF = -1;
    bool errorDetectado = false;

    while (!miHeap.estaVacio()) {
        State* actual = miHeap.pop();
        int fActual = actual->getF();
        
        std::cout << "Extraido f = " << fActual << std::endl;

        // Verificamos que sea mayor o igual al anterior (orden min-heap)
        if (fActual < anteriorF) {
            errorDetectado = true;
        }
        anteriorF = fActual;

        delete actual; // Limpiamos memoria del estado extraido
    }

    if (!errorDetectado) {
        std::cout << "\n[RESULTADO] OK: El Heap mantiene el orden de prioridad f(n)." << std::endl;
    } else {
        std::cout << "\n[RESULTADO] ERROR: El Heap no entrego los valores en orden correcto." << std::endl;
    }
}

int main() {
    try {
        testPrioridadMinima();
    } catch (...) {
        std::cerr << "Error critico durante la ejecucion del test." << std::endl;
        return 1;
    }
    return 0;
}