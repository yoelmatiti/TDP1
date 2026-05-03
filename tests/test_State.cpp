#include <iostream>
#include <cstring>
#include "Tablero.h"
#include "State.h"
#include "LectorArchivo.h" // Usamos tu LectorArchivo en lugar de Parser

void reportar(const char* test, bool condicion) {
    std::cout << (condicion ? "[OK] " : "[ERROR] ") << test << std::endl;
}

int main() {
    std::cout << "--- Iniciando Test de Clase State (Nueva Arquitectura) ---" << std::endl;

    // 1. Cargar Tablero Maestro (Solo para obtener dimensiones y bloques iniciales)
    // Cambiado Parser::cargarEscenario por LectorArchivo::cargarNivel
    Tablero* tableroMaestro = LectorArchivo::cargarNivel("simple1.txt");
    if (!tableroMaestro) {
        std::cerr << "Error: No se pudo cargar simple1.txt para el test." << std::endl;
        return 1;
    }

    int nB = tableroMaestro->getNumBloques();

    // 2. Crear Estado Inicial (Raíz)
    // Capturamos manualmente las posiciones del tablero recién cargado
    PosBloque* posIniciales = new PosBloque[nB];
    for (int i = 0; i < nB; i++) {
        Bloque* b = tableroMaestro->getBloquePtr(i);
        posIniciales[i].x = b->getX();
        posIniciales[i].y = b->getY();
        posIniciales[i].activo = b->estaActivo();
    }

    // Constructor: nBloques, posiciones, g, h, padre, operacion
    State* s0 = new State(nB, posIniciales, 0, 10, nullptr, "Inicio");
    
    // Liberamos el arreglo temporal porque State ya hizo su copia interna
    delete[] posIniciales;

    reportar("Creacion de estado inicial", s0->getG() == 0 && s0->getH() == 10 && s0->getF() == 10);

    // 3. Simular un movimiento y crear Estado Hijo (s1)
    // Simulamos que el bloque 0 se mueve 1 unidad a la derecha
    PosBloque* posHijo = new PosBloque[nB];
    for (int i = 0; i < nB; i++) {
        posHijo[i] = s0->getPosiciones()[i]; // Copiamos del padre
    }
    posHijo[0].x += 1; // El movimiento simulado

    State* s1 = new State(nB, posHijo, 1, 9, s0, "Bloque 0 Derecha");
    delete[] posHijo;

    // 4. Verificar Integridad
    reportar("Estado hijo tiene padre correcto", s1->getPadre() == s0);
    reportar("Costo G aumento correctamente", s1->getG() == 1);
    reportar("F calculado correctamente (1+9)", s1->getF() == 10);

    // 5. Verificar independencia de posiciones (Copia Profunda)
    bool sonDiferentes = (s0->getPosiciones()[0].x != s1->getPosiciones()[0].x);
    reportar("Independencia de posiciones (Copia Profunda)", sonDiferentes);

    // 6. Probar la reconstrucción del camino
    // Como eliminamos std::vector, este test solo verifica que la cadena de padres exista
    std::cout << "\nCamino reconstruido (recorrido manual):" << std::endl;
    State* temp = s1;
    while (temp != nullptr) {
        std::cout << " -> " << temp->getOperacion() << " (g=" << temp->getG() << ")" << std::endl;
        temp = temp->getPadre();
    }

    // 7. Limpieza
    // El Tablero maestro se borra aparte
    delete tableroMaestro;
    
    // Los estados se borran. Nota: El destructor de State NO debe borrar al padre.
    delete s1; 
    delete s0;

    std::cout << "--- Test de State finalizado ---" << std::endl;
    return 0;
}