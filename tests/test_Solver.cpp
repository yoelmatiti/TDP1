#include <cstdio>
#include "LectorArchivo.h"
#include "Tablero.h"
#include "Bloque.h"
#include "Salida.h"
#include "Solver.h"
#include "State.h"

int main() {
    printf("--- Test de Integracion: Solver A* (Carga desde Archivo) ---\n");

    // 1. Instanciar el lector y cargar el nivel
    LectorArchivo lector;
    const char* nombreArchivo = "simple1.txt";
    
    // El método cargarNivel ahora se encarga de crear el 'new Tablero'
    // y configurar sus dimensiones, bloques, paredes y salidas.
    Tablero* tableroInicial = lector.cargarNivel(nombreArchivo);

    // Verificación de seguridad
    if (tableroInicial == nullptr) {
        printf("[ERROR] No se pudo cargar el archivo: %s\n", nombreArchivo);
        return 1;
    }

    printf("Archivo '%s' cargado con éxito.\n", nombreArchivo);
    printf("Estado Inicial:\n");
    tableroInicial->imprimir();

   // 2. Configuración del Solver
    // Pasamos el tablero al constructor para que sea el 'tableroMaestro'
    Solver solver(tableroInicial);
    printf("\nIniciando busqueda de solucion con A*...\n");

    // 3. Ejecución: Llamada sin argumentos según la nueva estructura
    if (solver.resolver()) { 
        printf("\n[SUCCESS] Solucion encontrada!\n");
        
        // --- RECONSTRUCCIÓN DEL CAMINO (Sin STL) ---
        State* final = solver.getEstadoFinal();
        int totalPasos = 0;
        State* temp = final;

        // Contar profundidad del camino
        while (temp != nullptr) {
            totalPasos++;
            temp = temp->getPadre();
        }

        // Crear arreglo dinámico para invertir el orden (de inicio a fin)
        State** camino = new State*[totalPasos];
        temp = final;
        for (int i = totalPasos - 1; i >= 0; i--) {
            camino[i] = temp;
            temp = temp->getPadre();
        }

        printf("Mostrando secuencia de pasos detallada:\n");

        for (int i = 0; i < totalPasos; i++) {
            printf("\n================================\n");
            printf("PASO %d / %d", i, totalPasos - 1);
            
            // Mostrar la operación que llevó a este estado
            if (camino[i]->getOperacion() != nullptr) {
                printf(" | Movimiento: %s\n", camino[i]->getOperacion());
            } else {
                printf(" | Estado Inicial\n");
            }

            // 1. Sincronizar el tablero físico con la "foto" del estado lógico
            tableroInicial->actualizarDesdeEstado(camino[i]);

            // 2. Imprimir cabecera de columnas dinámica (opcional si imprimir() no lo hace)
            printf("    ");
            for (int c = 0; c < tableroInicial->getWidth(); c++) {
                printf("%d", c % 10);
            }
            printf("\n");

            // 3. Dibujar el tablero
            tableroInicial->imprimir();
        }

        printf("\n¡Meta alcanzada con éxito!\n");
        
        // Liberar el arreglo temporal de punteros (No los estados en sí)
        delete[] camino; 

    } else {
        printf("\n[FAIL] El Solver no pudo encontrar una ruta para este nivel.\n");
    }

    // 4. Limpieza de memoria
    // Al borrar tableroInicial, asegúrate de que su destructor limpie 
    // la lista de bloques y salidas creadas por el lector.
    delete tableroInicial; 
    printf("--- Fin del Test: Memoria liberada ---\n");

    return 0;
}