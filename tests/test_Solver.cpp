#include <cstdio>
#include "LectorArchivo.h"
#include "Tablero.h"
#include "Solver.h"

int main() {
    printf("--- Test de Integracion: Solver A* ---\n");

    LectorArchivo lector;
    Tablero* tablero = lector.cargarNivel("dificil1.txt");

    if (!tablero) {
        printf("[ERROR] No se pudo cargar el nivel.\n");
        return 1;
    }

    // El Solver ya captura las posiciones iniciales internamente en resolver()
    Solver solver(tablero);
    
    if (solver.resolver()) {
        printf("\n[SUCCESS] ¡Solución encontrada!\n");
        solver.imprimirCamino(); // Usamos la función interna del Solver
    } else {
        printf("\n[FAIL] No se pudo resolver el nivel.\n");
    }

    delete tablero;
    return 0;
}