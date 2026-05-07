#include <iostream>
#include <cstdio>
#include <cstring>
#include "LectorArchivo.h"
#include "Tablero.h"
#include "Bloque.h"

int main() {
    std::cout << "--- PRUEBAS UNITARIAS: CLASE LECTORARCHIVO ---" << std::endl;

    // Usamos el archivo real adjunto en tu prompt
    const char* rutaReal = "simple1.txt"; 

    LectorArchivo lector;
    Tablero* t = lector.cargarNivel(rutaReal);

    // 1. Validar Carga General
    if (t != nullptr) {
        std::cout << "[OK] Archivo '" << rutaReal << "' cargado exitosamente." << std::endl;
    } else {
        std::cerr << "[ERROR] No se pudo cargar el archivo. Verifique que 'simple1.txt' existe." << std::endl;
        return 1;
    }

    // 2. Validar Dimensiones según simple1.txt 
    // simple1.txt define WIDTH=8 y HEIGHT=8
    if (t->getWidth() == 8 && t->getHeight() == 8) {
        std::cout << "[OK] Dimensiones detectadas correctamente (8x8)." << std::endl;
    } else {
        std::cerr << "[ERROR] Dimensiones incorrectas. Esperado 8x8, obtenido: " 
                  << t->getWidth() << "x" << t->getHeight() << std::endl;
    }

    // 3. Validar Bloques 
    // simple1.txt tiene 1 bloque COLOR=a
    if (t->getNumBloques() == 1) {
        Bloque* b = t->getBloquePtr(0);
        if (b != nullptr && b->getColor() == 'a') {
            std::cout << "[OK] Bloque 'a' cargado correctamente." << std::endl;
        } else {
            std::cout << "[ERROR] Los datos del bloque no coinciden." << std::endl;
        }
    } else {
        std::cerr << "[ERROR] No se detecto el bloque. Cantidad: " << t->getNumBloques() << std::endl;
    }

    // 4. Validar Muros (Integridad del mapa) 
    // En simple1.txt, (0,0) es '#' y (4,4) es espacio libre (donde inicia el bloque)
    std::cout << "[INFO] Verificando integridad del mapa..." << std::endl;
    if (t->esPared(0, 0) && !t->esPared(4, 4)) {
        std::cout << "[OK] Muros y espacios libres verificados correctamente." << std::endl;
    } else {
        std::cerr << "[ERROR] Fallo en la verificacion de muros." << std::endl;
    }

    // 5. Limpieza
    delete t;

    std::cout << "----------------------------------------------" << std::endl;
    std::cout << "Test de LectorArchivo finalizado." << std::endl;

    return 0;
}