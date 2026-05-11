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
    Solver* solver = nullptr;

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
                    tableroActual = nullptr;
                }
                if (solver != nullptr) {
                    delete solver;
                    solver = nullptr;
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
                    clock_t tInicio = clock();
                    
                    // Liberar solver previo
                    if (solver != nullptr) {
                        delete solver;
                    }
                    // Se asocia el tablero al solver y se ejecuta la búsqueda
                    solver = new Solver(tableroActual);
                    if (solver->resolver()) {
                        clock_t tFin = clock();
                        double tiempoMs = ((double)(tFin - tInicio) / CLOCKS_PER_SEC) * 1000;
                        
                        printf("Solucion encontrada.\n");
                        printf("Tiempo resolucion: %.2f [mseg]\n", tiempoMs);
                        printf("Pasos: ");
                        solver->imprimirCamino(); // Imprime el camino
                    } else {
                        printf("Juego sin solucion.\n");
                    }
                } else {
                    printf("[!] Debe cargar un tablero primero (Opcion 1).\n");
                }
                break;

            case 3:
                if (tableroActual != nullptr && solver != nullptr && solver->getEstadoFinal() != nullptr) {
                    printf("Mostrando visualizacion de la solucion paso a paso...\n");
                    // Reiniciamos una copia del tablero para no perder el original
                    Tablero visualizador(*tableroActual);
                    // Por ahora, solo reimprimir el camino
                    solver->imprimirCamino();
                } else {
                    printf("[!] No hay una solucion cargada para visualizar. Ejecute la opcion 2 primero.\n");
                }
                break;

            case 4:
                printf("Opcion no implementada aun.\n");
                break;

            case 5:
                printf("Saliendo del programa...\n");
                break;

            default:
                printf("Opcion no valida.\n");
                break;
        }
    } while (opcion != 5);

    if (tableroActual != nullptr) delete tableroActual;
    if (solver != nullptr) delete solver;
    return 0;
}