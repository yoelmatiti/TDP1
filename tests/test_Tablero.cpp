#include <iostream>
#include "Tablero.h"
#include "LectorArchivo.h" 
#include "State.h"
#include "Movimiento.h"

void imprimirResultado(const char* msg, bool exito) {
    std::cout << (exito ? "[OK] " : "[ERROR] ") << msg << std::endl;
}

int main() {
    std::cout << "--- Test de Tablero: Validacion de Fisica y Movimiento ---" << std::endl;

    // 1. Cargar el escenario usando LectorArchivo
    LectorArchivo lector;
    Tablero* tab = lector.cargarNivel("dificil1.txt");
    
    if (!tab) {
        std::cerr << "Error critico: No se pudo cargar el archivo de nivel." << std::endl;
        return 1;
    }

    // 2. Captura automatizada de posiciones para el Estado Inicial
    int nB = tab->getNumBloques();
    PosBloque* posIniciales = new PosBloque[nB];
    
    for (int i = 0; i < nB; i++) {
        Bloque* b = tab->getBloquePtr(i);
        if (b) {
            posIniciales[i].x = b->getX();
            posIniciales[i].y = b->getY();
            posIniciales[i].activo = b->estaActivo();
        }
    }

    // Crear el estado inicial (Costo G=0, H=0, Padre=nullptr, Operacion="Inicial")
    State* estadoInicial = new State(nB, posIniciales, 0, 0, nullptr, "Inicial");
    delete[] posIniciales; // El constructor de State ya debió hacer su copia profunda

    // 3. Render Inicial
    std::cout << "\n--- ESTADO INICIAL ---" << std::endl;
    tab->actualizarDesdeEstado(estadoInicial);
    tab->imprimir();

    // 4. TEST DE MOVIMIENTO POSIBLE (Hacia abajo - Direccion::D)
 
    std::cout << "\nTest 3: Movimiento hacia ABAJO (Bloque 0, Direccion::D)" << std::endl;
    State* estadoHijo = Movimiento::ejecutar(0, Direccion::D, estadoInicial, tab);

    if (estadoHijo) {
        imprimirResultado("Movimiento validado por la fisica del motor", true);
        
        // Verificamos si la coordenada Y aumentó en el nuevo State
        bool movio = (estadoHijo->getPosicion(0).y > estadoInicial->getPosicion(0).y);
        imprimirResultado("El nuevo estado registra el cambio de coordenada Y", movio);

        std::cout << "Visualizando Tablero despues del movimiento:" << std::endl;
        tab->actualizarDesdeEstado(estadoHijo);
        tab->imprimir();
        
        delete estadoHijo;
    } else {
        std::cout << "[ERROR] El motor bloqueo un movimiento que deberia ser legal." << std::endl;
        std::cout << "Pistas: Revisar Tablero::esPared o Bloque::ocupaCelda." << std::endl;
    }

    // 5. TEST DE COLISIÓN REAL (Hacia arriba, contra la pared de la fila 0)
    std::cout << "\nTest 4: Intento de colision (Hacia ARRIBA contra el techo)" << std::endl;
    
    // El bloque está en Y=4. Para chocar con el techo (Y=0), 
    // movemos varias veces hasta que deba chocar.
    State* temp = estadoInicial;
    State* sig = nullptr;
    
    // Movemos hacia arriba (U) 4 veces. En la 4ta debería chocar.
    for(int i=0; i<5; i++) {
        sig = Movimiento::ejecutar(0, Direccion::U, temp, tab);
        if(!sig) {
            imprimirResultado("Colision detectada contra el techo correctamente", true);
            break;
        }
        if(temp != estadoInicial) delete temp;
        temp = sig;
    }

    if (sig) {
        imprimirResultado("ERROR: El bloque atraveso el techo del tablero.", false);
        delete sig;
    }

    // 6. Limpieza final
    delete estadoInicial;
    delete tab;

    std::cout << "\n--- Pruebas finalizadas con exito ---" << std::endl;
    return 0;
}