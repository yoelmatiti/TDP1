#ifndef SOLVER_H
#define SOLVER_H

#include "Tablero.h"
#include "State.h"
#include "Heap.h"      // Cola de Prioridad (Open Set)
#include "TablaHash.h" // Tabla para estados visitados (Closed Set)

/**
 * Clase Solver
 * Implementa el algoritmo A* utilizando la nueva arquitectura donde el Tablero
 * es una consulta estática y el State contiene la información dinámica.
 */
class Solver {
private:
    Heap open;            // Frontera de exploración (Prioridad f = g + h)
    TablaHash closed;     // Registro de estados ya procesados (Evita ciclos)
    Tablero* tableroMaestro;
    State* estadoFinal;   // Puntero al estado meta encontrado

    /**
     * Calcula la distancia Manhattan de cada bloque a su salida correspondiente.
     * Ahora recibe un State* para consultar posiciones sin tocar el Tablero físico.
     */
    int calcularHeuristica(State* s);

    /**
     * Crea un arreglo de PosBloque leyendo los datos iniciales de los objetos
     * Bloque dentro del tablero. Solo se usa para generar el estado raíz.
     */
    PosBloque* capturarPosicionesActuales();
    bool esEstadoFinal(State* s);

public:
    Solver();
    Solver(Tablero* t);
    ~Solver();

    /**
     * Ejecuta el algoritmo A*.
     * @return true si encontró una solución, false en caso contrario.
     */
    bool resolver();

    /**
     * Genera todos los movimientos posibles para un estado dado.
     * Utiliza la clase Movimiento como árbitro de reglas.
     */
    void expandirEstado(State* actual);

    /**
     * Verifica si en el estado dado todos los bloques han llegado a su meta.
     */
  

    /**
     * Retorna el costo g(n) (número de pasos) del estado solución.
     */
    int getPasosSolucion(); 
    
    /**
     * Reconstruye el camino desde la raíz hasta el estado meta e imprime las operaciones.
     */
    void imprimirCamino(State* meta = nullptr);

    // Getters básicos
    inline State* getEstadoFinal() const { return estadoFinal; }

};

#endif