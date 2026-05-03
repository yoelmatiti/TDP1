#include "Solver.h"
#include "Movimiento.h"
#include <iostream>
#include <cstdio>
#include <cmath>

// Eliminamos #include <vector> y <algorithm>

Solver::Solver() : open(100000), closed(500003), tableroMaestro(nullptr), estadoFinal(nullptr) {}

Solver::Solver(Tablero* t) : open(100000), closed(500003), tableroMaestro(t), estadoFinal(nullptr) {}

Solver::~Solver() {}

/**
 * resolver(Tablero* t) - Implementación con A* * Ahora recibe el tablero desde el menú (main)
 */
bool Solver::resolver(Tablero* t) {
    // 1. Validar y asignar el tablero maestro
    if (!t) {
        std::cerr << "[ERROR] Puntero a tablero nulo." << std::endl;
        return false;
    }
    this->tableroMaestro = t; // Guardamos la referencia para expandirEstado y heurística

    // Limpiar estructuras por si se llama más de una vez (Opcional pero recomendado)
    // open.limpiar(); 
    // closed.limpiar();

    int estadosProcesados = 0;
    int nB = tableroMaestro->getNumBloques();

    // 2. Capturar posiciones iniciales desde el tablero que acabamos de recibir
    PosBloque* posIniciales = capturarPosicionesActuales(); 
    
    // Crear estado raíz (g=0, h=calculada)
    State* root = new State(nB, posIniciales, 0, 0, nullptr, "Inicio");
    delete[] posIniciales; 

    root->setH(calcularHeuristica(root));
    
    // 3. Inicializar búsqueda
    open.push(root);
    closed.insertar(root); 

    while (!open.estaVacio()) {
        State* actual = open.pop();
        if (!actual) continue;

        // 4. Condición de Victoria
        if (esEstadoFinal(actual)) {
            std::cout << "\n[¡ÉXITO!] Meta alcanzada. Estados explorados: " << estadosProcesados << std::endl;
            this->estadoFinal = actual;
            
            // Sincronizamos el tablero maestro con la foto final para mostrarlo
            tableroMaestro->actualizarDesdeEstado(actual);
            
            // Método para mostrar el camino de vuelta (opcional)
            // imprimirCamino(actual); 
            
            return true;
        }
        
        estadosProcesados++;
        
        // 5. Expansión: Aquí es donde State y Tablero interactúan
        expandirEstado(actual);

        // Monitor de progreso para el profesor
        if (estadosProcesados % 5000 == 0) {
            std::cout << "[INFO] Procesados: " << estadosProcesados 
                      << " | En espera (Open): " << open.getTamano() << std::endl;
        }
    }

    std::cout << "\n[DERROTA] No se halló solución tras explorar " << estadosProcesados << " estados." << std::endl;
    return false;
}

void Solver::expandirEstado(State* actual) {
    int nB = actual->getNumBloques();
    Direccion direcciones[] = {Direccion::U, Direccion::D, Direccion::L, Direccion::R};

    for (int i = 0; i < nB; i++) {
        if (!actual->getPosiciones()[i].activo) continue;

        for (Direccion d : direcciones) {
            State* nuevo = Movimiento::ejecutar(i, d, actual, tableroMaestro);
            
            if (nuevo != nullptr) {
                if (!closed.existe(nuevo)) {
                    nuevo->setH(calcularHeuristica(nuevo));
                    open.push(nuevo);
                    closed.insertar(nuevo); 
                } else {
                    delete nuevo;
                }
            }
        }
    }
}

int Solver::calcularHeuristica(State* s) {
    if (!s || !tableroMaestro) return 0;

    int hTotal = 0;
    int nB = s->getNumBloques();
    PosBloque* posiciones = s->getPosiciones();

    for (int i = 0; i < nB; i++) {
        if (!posiciones[i].activo) continue;

        Bloque* bTemplate = tableroMaestro->getBloquePtr(i);
        // Implementación manual de tolower si es necesario, o usar cctype
        char colorB = (char)tolower(bTemplate->getColor());

        int minManhattan = 1000000; 
        bool tieneSalidaValida = false;

        for (int j = 0; j < tableroMaestro->getNumSalidas(); j++) {
            Salida* sal = tableroMaestro->getSalidaPtr(j);
            
            if (sal && (char)tolower(sal->getColor()) == colorB) {
                int dist = std::abs(posiciones[i].x - sal->getX()) + 
                           std::abs(posiciones[i].y - sal->getY());
                
                if (dist < minManhattan) minManhattan = dist;
                tieneSalidaValida = true;
            }
        }
        hTotal += tieneSalidaValida ? minManhattan : 100;
    }
    return hTotal;
}

/**
 * imprimirCamino() - Refactorizada para no usar std::vector
 */
void Solver::imprimirCamino(State* meta) {
    State* target = (meta != nullptr) ? meta : estadoFinal;
    if (!target) {
        std::cout << "No hay camino que imprimir." << std::endl;
        return;
    }

    // 1. Contar cuántos estados hay en el camino
    int profundidad = 0;
    State* temp = target;
    while (temp != nullptr) {
        profundidad++;
        temp = temp->getPadre();
    }

    // 2. Crear un arreglo dinámico simple
    State** camino = new State*[profundidad];
    temp = target;
    for (int i = profundidad - 1; i >= 0; i--) {
        camino[i] = temp;
        temp = temp->getPadre();
    }

    // 3. Imprimir en orden correcto
    std::cout << "--- Secuencia de Solucion (" << target->getG() << " pasos) ---" << std::endl;
    for (int i = 0; i < profundidad; i++) {
        std::cout << "Paso " << camino[i]->getG() << ": " << camino[i]->getOperacion() << std::endl;
    }

    // 4. Liberar el arreglo temporal (no los States, que están en closed)
    delete[] camino;
}

PosBloque* Solver::capturarPosicionesActuales() {
    int n = tableroMaestro->getNumBloques();
    PosBloque* pos = new PosBloque[n];
    for (int i = 0; i < n; i++) {
        Bloque* b = tableroMaestro->getBloquePtr(i);
        pos[i].x = b->getX();
        pos[i].y = b->getY();
        pos[i].activo = b->estaActivo();
    }
    return pos;
}

int Solver::getPasosSolucion() {
    return (estadoFinal != nullptr) ? estadoFinal->getG() : -1;
}

bool Solver::esEstadoFinal(State* s) {
    if (!s) return false;
    PosBloque* posiciones = s->getPosiciones();
    for (int i = 0; i < s->getNumBloques(); i++) {
        if (posiciones[i].activo) return false;
    }
    return true;
}