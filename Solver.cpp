#include "Solver.h"
#include "Movimiento.h"
#include <cstdio>   // Para printf
#include <cctype>   // Para tolower

// Constructor vacío
Solver::Solver() : open(100000), closed(500003), tableroMaestro(nullptr), estadoFinal(nullptr) {}

// Constructor con tablero (Recomendado)
Solver::Solver(Tablero* t) : open(100000), closed(500003), tableroMaestro(t), estadoFinal(nullptr) {}

Solver::~Solver() {
    // IMPORTANTE: El destructor de Solver o de la TablaHash (closed) 
    // debe liberar todos los punteros State generados.
}

/**
 * resolver() - Ahora utiliza el tablero guardado en el constructor.
 */
bool Solver::resolver() {
    // 1. Validar tablero maestro
    if (this->tableroMaestro == nullptr) {
        printf("[ERROR] No hay tablero cargado para resolver.\n");
        return false;
    }

    int estadosProcesados = 0;
    int nB = tableroMaestro->getNumBloques();

    // 2. Capturar estado inicial
    PosBloque* posIniciales = capturarPosicionesActuales(); 
    
    // Crear estado raíz (g=0, h=calculada)
    State* root = new State(nB, posIniciales, 0, 0, nullptr, (char*)"Inicio");
    delete[] posIniciales; // Liberamos el temporal usado por el constructor de State

    root->setH(calcularHeuristica(root));
    
    // 3. Inicializar búsqueda
    open.push(root);
    closed.insertar(root); 

    while (!open.estaVacio()) {
        State* actual = open.pop();
        if (!actual) continue;

        // 4. Condición de Victoria: ¿Todos los bloques están inactivos/salieron?
        if (esEstadoFinal(actual)) {
            printf("\n[SUCCESS] Meta alcanzada. Estados explorados: %d\n", estadosProcesados);
            this->estadoFinal = actual;
            
            // Sincronizamos el tablero para la vista final
            tableroMaestro->actualizarDesdeEstado(actual);
            return true;
        }
        
        estadosProcesados++;
        
        // 5. Expansión de estados sucesores
        expandirEstado(actual);

        // Monitor de progreso (Feedback para el usuario en consola)
        if (estadosProcesados % 5000 == 0) {
            printf("[INFO] Procesados: %d | En espera (Open): %d\n", 
                    estadosProcesados, open.getTamano());
        }
    }

    printf("\n[FAIL] No se hallo solucion tras explorar %d estados.\n", estadosProcesados);
    return false;
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

bool Solver::esEstadoFinal(State* s) {
    if (!s) return false;
    for (int i = 0; i < s->getNumBloques(); i++) {
        PosBloque p = s->getPosicion(i);
        if (p.activo) return false;
    }
    return true;
}

void Solver::expandirEstado(State* actual) {
    int nB = actual->getNumBloques();
    // Definimos las direcciones posibles
    Direccion direcciones[] = {Direccion::U, Direccion::D, Direccion::L, Direccion::R};

    for (int i = 0; i < nB; i++) {
        // Solo mover bloques que aún están en el tablero
        PosBloque p = actual->getPosicion(i);
        if (!p.activo) continue;

        for (int dIdx = 0; dIdx < 4; dIdx++) {
            Direccion d = direcciones[dIdx];
            
            // Intentar generar un nuevo estado
            State* nuevo = Movimiento::ejecutar(i, d, actual, tableroMaestro);
            
            if (nuevo != nullptr) {
                // Verificar si ya visitamos este estado (O(1) o O(log N))
                if (!closed.existe(nuevo)) {
                    nuevo->setH(calcularHeuristica(nuevo));
                    open.push(nuevo);     // Insertar en Heap
                    closed.insertar(nuevo); // Marcar como visitado
                } else {
                    delete nuevo; // Si ya existe, liberar memoria inmediatamente
                }
            }
        }
    }
}


int Solver::calcularHeuristica(State* s) {
    if (!s || !tableroMaestro) return 0;

    int hTotal = 0;
    int nB = s->getNumBloques();

    for (int i = 0; i < nB; i++) {
        PosBloque p = s->getPosicion(i);
        if (!p.activo) continue;

        // Obtenemos el color del bloque original
        char colorB = (char)tolower(tableroMaestro->getBloquePtr(i)->getColor());
        int minManhattan = 1000000; 
        bool tieneSalidaValida = false;

        // Buscar la salida más cercana del mismo color
        for (int j = 0; j < tableroMaestro->getNumSalidas(); j++) {
            Salida* sal = tableroMaestro->getSalidaPtr(j);
            if (sal && (char)tolower(sal->getColor()) == colorB) {
                // Distancia Manhattan: |x1-x2| + |y1-y2|
                int dx = p.x - sal->getX();
                int dy = p.y - sal->getY();
                if (dx < 0) dx = -dx;
                if (dy < 0) dy = -dy;
                
                int dist = dx + dy;
                if (dist < minManhattan) minManhattan = dist;
                tieneSalidaValida = true;
            }
        }
        // Si el bloque no tiene salida, penalizamos fuerte
        hTotal += tieneSalidaValida ? minManhattan : 100;
    }
    return hTotal;
}

void Solver::imprimirCamino(State* meta) {
    State* target = (meta != nullptr) ? meta : estadoFinal;
    if (!target) {
        printf("No hay camino que imprimir.\n");
        return;
    }

    // Contar profundidad para el arreglo
    int profundidad = 0;
    State* temp = target;
    while (temp != nullptr) {
        profundidad++;
        temp = temp->getPadre();
    }

    // Invertir el camino usando un arreglo dinámico manual (sustituto de vector)
    State** camino = new State*[profundidad];
    temp = target;
    for (int i = profundidad - 1; i >= 0; i--) {
        camino[i] = temp;
        temp = temp->getPadre();
    }

    printf("--- Secuencia de Solucion (%d pasos) ---\n", target->getG());
    for (int i = 0; i < profundidad; i++) {
        printf("Paso %d: %s\n", camino[i]->getG(), camino[i]->getOperacion());
    }

    delete[] camino;
}