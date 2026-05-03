#include <iostream>
#include <string>
#include "LectorArchivo.h"
#include "Tablero.h"
#include "Solver.h"

void mostrarMenu() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "      COLOR BLOCK JAM - SOLVER A*" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "1. Cargar nivel desde archivo (.txt)" << std::endl;
    std::cout << "2. Resolver nivel actual" << std::endl;
    std::cout << "3. Salir" << std::endl;
    std::cout << "Seleccione una opcion: ";
}

int main() {
    int opcion = 0;
    std::string nombreArchivo;
    Tablero* tableroActual = nullptr;
    LectorArchivo lector;

    do {
        mostrarMenu();
        if (!(std::cin >> opcion)) {
            std::cout << "Opcion no valida." << std::endl;
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }

        switch (opcion) {
            case 1:
                std::cout << "Ingrese nombre del archivo: ";
                std::cin >> nombreArchivo;
                tableroActual = lector.cargarNivel(nombreArchivo);
                if (tableroActual == nullptr) {
                    std::cout << "[ERROR] El archivo '" << nombreArchivo << "' no existe o esta corrupto." << std::endl;
                } else {
                    std::cout << "[OK] Nivel cargado. Dimensiones: " << tableroActual->getWidth() << "x" << tableroActual->getHeight() << std::endl;
                }
                break;

            case 2:
                if (tableroActual != nullptr) {
                    std::cout << "Iniciando Solver A*..." << std::endl;
                    Solver solver;
                    // Ahora el compilador encontrará esta función:
                    solver.resolver(tableroActual);
                } else {
                    std::cout << "[!] Primero debes cargar un nivel (Opcion 1)." << std::endl;
                }
                break;

            case 3:
                std::cout << "Saliendo del programa..." << std::endl;
                break;

            default:
                std::cout << "Opcion no valida." << std::endl;
        }
    } while (opcion != 3);

    if (tableroActual != nullptr) delete tableroActual;
    return 0;
}