#include <iostream>
#include "Tablero.h"
#include "Parser.h"
#include "State.h"
#include "Movimiento.h"

void imprimirResultado(const char* msg, bool exito) {
    std::cout << (exito ? "[OK] " : "[ERROR] ") << msg << std::endl;
}

int main() {
    std::cout << "--- Test de Tablero: Validacion de Fisica y Movimiento ---" << std::endl;

    // 1. Cargar el escenario
    Tablero* tab = Parser::cargarEscenario("facil2_corregido.txt");
    if (!tab) return 1;

    // 2. Captura automatizada de posiciones para el Estado Inicial
    int nB = tab->getNumBloques();
    PosBloque* posIniciales = new PosBloque[nB];
    for (int i = 0; i < nB; i++) {
        Bloque* b = tab->getBloquePtr(i);
        posIniciales[i].x = b->getX();
        posIniciales[i].y = b->getY();
        posIniciales[i].activo = b->estaActivo();
    }

    State* estadoInicial = new State(nB, posIniciales, 0, 0, nullptr, "Inicial");
    delete[] posIniciales;

    // 3. Render Inicial
    std::cout << "\nESTADO INICIAL:" << std::endl;
    tab->actualizarDesdeEstado(estadoInicial);
    tab->imprimir();

    // 4. TEST DE MOVIMIENTO POSIBLE (Hacia abajo)
    // El bloque 0 (AAAA) tiene aire abajo en las filas 3, 4 y 5.
    std::cout << "\nTest 3: Movimiento hacia ABAJO (Direccion::D)" << std::endl;
    State* estadoHijo = Movimiento::ejecutar(0, Direccion::D, estadoInicial, tab);

    if (estadoHijo) {
        imprimirResultado("Movimiento validado por la fisica del motor", true);
        
        // Verificamos si las coordenadas cambiaron en el nuevo objeto State
        bool movio = (estadoHijo->getPosiciones()[0].y > estadoInicial->getPosiciones()[0].y);
        imprimirResultado("El nuevo estado registra el cambio de coordenada Y", movio);

        std::cout << "Visualizando Tablero despues del movimiento:" << std::endl;
        tab->actualizarDesdeEstado(estadoHijo);
        tab->imprimir();
        
        delete estadoHijo;
    } else {
        std::cout << "[ERROR] El motor bloqueo un movimiento que parecia libre." << std::endl;
        std::cout << "Revisar: Bloque::ocupaCelda o Tablero::getCeldaEstatica" << std::endl;
    }

    // 5. TEST DE COLISIÓN (Hacia la derecha - Debería fallar)
    std::cout << "\nTest 4: Intento de colision (Derecha contra bloque CC)" << std::endl;
    State* estadoInvalido = Movimiento::ejecutar(0, Direccion::R, estadoInicial, tab);
    
    if (!estadoInvalido) {
        imprimirResultado("Colision detectada correctamente (Bloqueo exitoso)", true);
    } else {
        imprimirResultado("Colision NO detectada (Los bloques se atraviesan)", false);
        delete estadoInvalido;
    }

    // 6. Limpieza
    delete estadoInicial;
    delete tab;

    std::cout << "\n--- Pruebas finalizadas con exito ---" << std::endl;
    return 0;
}