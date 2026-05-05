#include <cstdio>
#include "LectorArchivo.h"
#include "Tablero.h"
#include "Solver.h"

// El menú debe incluir opciones de carga, resolución, visualización y salida 
void mostrarMenu() {
    printf("\n========================================\n");
    printf("        COLOR BLOCK JAM - MENU\n");
    printf("========================================\n");
    printf("1. Cargar nivel desde archivo\n");
    printf("2. Resolver nivel y mostrar pasos (Algoritmo A*)\n");
    printf("3. Visualizar tablero paso a paso (Solucion previa)\n");
    printf("4. Ingresar secuencia de movimientos manual\n");
    printf("5. Salir\n");
    printf("Seleccione una opcion: ");
}

int main() {
    int opcion = 0;
    // Se utiliza un arreglo de char en lugar de std::string [cite: 123]
    char nombreArchivo[256];
    char secuenciaManual[512];
    Tablero* tableroActual = nullptr;
    LectorArchivo lector;
    Solver solver;

    do {
        mostrarMenu();
        // Uso de scanf para evitar iostream [cite: 123]
        if (scanf("%d", &opcion) != 1) {
            printf("Error: Opcion no valida.\n");
            // Limpieza del buffer de entrada
            while (getchar() != '\n'); 
            continue;
        }

        switch (opcion) {
            case 1:
                printf("Ingrese nombre del archivo: ");
                scanf("%s", nombreArchivo);
                
                // Liberar memoria previa antes de cargar un nuevo tablero [cite: 244]
                if (tableroActual != nullptr) {
                    delete tableroActual;
                }
                
                tableroActual = lector.cargarNivel(nombreArchivo);
                if (tableroActual == nullptr) {
                    printf("[ERROR] El archivo '%s' no pudo ser cargado.\n", nombreArchivo);
                } else {
                    printf("[OK] Nivel cargado exitosamente.\n");
                }
                break;

            case 2:
                if (tableroActual != nullptr) {
                    printf("Iniciando Solver A*...\n");
                    // El solver debe encontrar la secuencia eficiente [cite: 126, 248]
                    if (solver.resolver(tableroActual)) {
                        // Al resolver, se debe mostrar el tiempo y la secuencia 
                        printf("Solucion encontrada.\n");
                    } else {
                        printf("Juego sin solucion.\n"); // [cite: 133]
                    }
                } else {
                    printf("[!] Primero debe cargar un nivel (Opcion 1).\n");
                }
                break;

            case 3:
                if (tableroActual != nullptr) {
                    // El menú debe permitir mostrar el tablero según los pasos resueltos 
                    printf("Mostrando visualizacion de la solucion...\n");
                    // Aquí se invoca la lógica de visualización paso a paso
                } else {
                    printf("[!] No hay una solucion cargada para visualizar.\n");
                }
                break;

            case 4:
                if (tableroActual != nullptr) {
                    // El menú debe permitir ingresar una secuencia manual de movimientos 
                    printf("Ingrese secuencia (ej. U1,2D2,1): ");
                    scanf("%s", secuenciaManual);
                    printf("Procesando secuencia: %s\n", secuenciaManual);
                    // Lógica para aplicar los movimientos al tablero inicial
                } else {
                    printf("[!] Debe cargar un tablero primero.\n");
                }
                break;

            case 5:
                printf("Saliendo del programa...\n"); // [cite: 128]
                break;

            default:
                printf("Opcion no valida.\n");
                break;
        }
    } while (opcion != 5);

    // Limpieza final de memoria [cite: 244]
    if (tableroActual != nullptr) {
        delete tableroActual;
    }

    return 0;
}