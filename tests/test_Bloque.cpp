#include <iostream>
#include "Bloque.h"

int main() {
    std::cout << "--- PRUEBA UNITARIA: CREACION DE BLOQUE PERSONALIZADO ---" << std::endl;

    // Datos del ejemplo: 
    // ID=1, COLOR=a, WIDTH=4, HEIGHT=4, INIT_X=1, INIT_Y=1
    // GEOMETRY= 1 1 1 1 
    //           1 1 1 1 
    //           0 0 1 1 
    //           0 0 1 1
    
    int id = 1;
    char color = 'a';
    int width = 4;
    int height = 4;
    int init_x = 1;
    int init_y = 1;

    // 1. Crear el arreglo de geometría (GEOMETRY)
    bool* forma = new bool[width * height];
    
    bool valores[16] = {
        1, 1, 1, 1, 
        1, 1, 1, 1, 
        0, 0, 1, 1, 
        0, 0, 1, 1
    };

    for (int i = 0; i < 16; i++) {
        forma[i] = valores[i];
    }

    // 2. Instanciar el Bloque
    Bloque b1(id, color, init_x, init_y, width, height, forma);

    // 3. Liberar la memoria temporal
    delete[] forma;

    // 4. Validaciones de atributos básicos
    std::cout << "ID esperado: 1 | Obtenido: " << b1.getId() << std::endl;
    std::cout << "Posicion esperada: (1,1) | Obtenida: (" << b1.getX() << "," << b1.getY() << ")" << std::endl;
    std::cout << "Color esperado: a | Obtenido: " << b1.getColor() << std::endl;

    // 5. Validar Geometria usando el nuevo ocupaCelda(tx, ty, bX, bY)
    bool ok_geo = true;

    // Caso A: Probamos un punto que debe ser TRUE
    // Celda tablero (1,1). Posicion bloque (1,1). Relativo es (0,0) -> GEOMETRY[0]=1
    if (!b1.ocupaCelda(1, 1, b1.getX(), b1.getY())) {
        std::cout << "[Fallo] Celda (1,1) deberia estar ocupada." << std::endl;
        ok_geo = false;
    }
    
    // Caso B: Probamos un punto que debe ser FALSE
    // Celda tablero (1,3). Posicion bloque (1,1). Relativo es (0,2) -> GEOMETRY[8]=0
    if (b1.ocupaCelda(1, 3, b1.getX(), b1.getY())) {
        std::cout << "[Fallo] Celda (1,3) deberia estar vacia." << std::endl;
        ok_geo = false;
    }

    // Caso C: Prueba de independencia (¿Que pasaria si el bloque estuviera en 0,0?)
    // Si el bloque estuviera en (0,0), la celda (2,3) seria relativa (2,3) -> GEOMETRY[14]=1
    if (!b1.ocupaCelda(2, 3, 0, 0)) {
        std::cout << "[Fallo] Prueba de traslacion: en pos(0,0), celda(2,3) deberia ser TRUE." << std::endl;
        ok_geo = false;
    }

    if (ok_geo) {
        std::cout << "[OK] La geometria y el sistema de coordenadas funcionan correctamente." << std::endl;
    } else {
        std::cout << "[ERROR] Fallo en la validacion de geometria dinamica." << std::endl;
    }

    std::cout << "--- Fin del Test ---" << std::endl;

    return 0;
}