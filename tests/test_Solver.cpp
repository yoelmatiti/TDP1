#include <iostream>
#include <vector> // Necesario para almacenar el camino
#include <cstdio> // Para getchar()
#include "Tablero.h"
#include "Bloque.h"
#include "Salida.h"
#include "Solver.h"

int main() {
    std::cout << "--- Test de Integracion: Solver A* (Multi-Bloque) ---" << std::endl;

    // 1. Crear el Tablero
    Tablero* tableroInicial = new Tablero();
    tableroInicial->setDimensiones(6, 6);

    // 2. Crear Bloques (A en 1,1 y B en 1,2)
    bool* geoA = new bool[1]{true}; 
    Bloque* bA = new Bloque(1, 'A', 1, 1, 1, 1, geoA);

    bool* geoB = new bool[1]{true}; 
    Bloque* bB = new Bloque(2, 'B', 2, 1, 1, 1, geoB); // Cambiado a (2,1) para dar espacio

    // 3. Crear Salidas
    Salida* sA = new Salida('A', 3, 3, 0, 0, 1, 1, 0);
    Salida* sB = new Salida('B', 4, 4, 0, 0, 1, 1, 0);

    tableroInicial->agregarBloque(bA);
    tableroInicial->agregarBloque(bB);
    tableroInicial->agregarSalida(sA);
    tableroInicial->agregarSalida(sB);

    // 4. Configuracion del Solver
    Solver solver(tableroInicial);
    std::cout << "Iniciando busqueda de solucion para 2 bloques..." << std::endl;

    // 5. Ejecucion y Visualización
    if (solver.resolver()) {
        std::cout << "\n[SUCCESS] Solucion encontrada!" << std::endl;
        
        // --- RECONSTRUCCIÓN DEL CAMINO ---
        std::vector<State*> camino;
        State* actual = solver.getEstadoFinal();
        
        while (actual != nullptr) {
            camino.push_back(actual);
            actual = actual->getPadre();
        }

        std::cout << "Mostrando secuencia de movimientos (Presiona ENTER para el siguiente paso):" << std::endl;

        // Recorremos el vector en reversa (del inicio a la meta)
        for (int i = camino.size() - 1; i >= 0; i--) {
            std::cout << "\n================================" << std::endl;
            std::cout << "PASO " << (camino.size() - 1 - i) << " / " << (camino.size() - 1);
            std::cout << " | Movimiento: " << camino[i]->getOperacion() << std::endl;

            // Sincronizar el tablero maestro con los datos del estado actual
            tableroInicial->actualizarDesdeEstado(camino[i]);
            
            // Imprimir visualmente
            tableroInicial->imprimir();

            //std::cout << "Presiona ENTER para continuar..." << std::endl;
            //std::getchar(); // Espera entrada del usuario
        }

        std::cout << "\n¡Meta alcanzada con éxito!" << std::endl;
        
    } else {
        std::cout << "\n[FAIL] El Solver no pudo encontrar una ruta." << std::endl;
    }

    // 6. Limpieza
    delete tableroInicial; 
    std::cout << "--- Fin del Test: Memoria liberada ---" << std::endl;

    return 0;
}