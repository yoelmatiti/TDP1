#ifndef Compuerta_H
#define Compuerta_H

#include "Movimiento.h"

/**
 * @class Compuerta
 * @brief Representa un obstáculo dinámico que cambia de color según el tiempo.
 * * Las compuertas actúan como filtros: solo permiten el paso de bloques cuyo 
 * color coincida con el color actual de la compuerta en un turno (paso) específico.
 */
class Compuerta {
private:
    // El orden de declaración aquí debe coincidir con la lista de inicialización en el .cpp
    int x, y;               ///< Posición de la celda de la compuerta en el tablero.
    char orientacion;       ///< 'V' (Vertical) o 'H' (Horizontal). Define la restricción física.
    char* cicloColores;     ///< Arreglo dinámico (C-string) que almacena la secuencia de colores.
    int numColores;         ///< Cantidad total de estados de color en el ciclo.
    int pasoCambio;         ///< Frecuencia de cambio (cada cuántos turnos 'g' cambia el color).

public:
    // --- Gestión de Ciclo de Vida (Regla de los Tres) ---

    /** @brief Constructor por defecto. Inicializa una compuerta vacía. */
    Compuerta();

    /** * @brief Constructor parametrizado.
     * @param _x Coordenada X.
     * @param _y Coordenada Y.
     * @param _p Paso de cambio (velocidad).
     * @param _ciclo Cadena de caracteres con los colores (ej: "rgb").
     */
    Compuerta(int _x, int _y, int _p, const char* _ciclo);
    
    /** @brief Constructor de copia para asegurar duplicación profunda de cicloColores. */
    Compuerta(const Compuerta& otro);

    /** @brief Operador de asignación. Maneja la liberación de memoria previa y copia profunda. */
    Compuerta& operator=(const Compuerta& otro);

    /** @brief Destructor. Libera la memoria dinámica de cicloColores. */
    ~Compuerta();

    // --- Lógica de Estado y Tiempo ---

    /** * @brief Determina qué color muestra la compuerta en un turno específico.
     * @param tiempoG El tiempo actual del estado (g).
     * @return El carácter que representa el color activo.
     */
    char getColorActual(int tiempoG) const;
    
    /** @brief Obtiene el color de la compuerta en el tiempo 0. */
    char getColor() const; 

    // --- Lógica de Validación de Movimiento ---
    
    /** * @brief Comprueba si un bloque puede atravesar la compuerta.
     * @param colorBloque Color del bloque que intenta pasar.
     * @param paso Turno actual del sistema.
     * @return true si los colores coinciden o el bloque es un comodín.
     */
    bool puedePasar(char colorBloque, int paso) const;

    /** * @brief Verifica si el tamaño del bloque es físicamente compatible con la apertura.
     */
    bool verificarDimension(int anchoBloque, int altoBloque) const;

    /**
     * @brief Calcula dónde aparecerá el bloque tras atravesar la compuerta.
     * @param outX Referencia para devolver la nueva coordenada X.
     * @param outY Referencia para devolver la nueva coordenada Y.
     */
    void calcularDestino(int bloqueX, int bloqueY, int anchoB, int altoB, Direccion dir, int& outX, int& outY) const;

    // --- Configuración Dinámica ---

    /** @brief Permite expandir el ciclo de colores agregando uno nuevo al final. */
    void agregarColorAlCiclo(char c);

    // --- Consultores (Getters) ---
    int getX() const;
    int getY() const;
    char getOrientacion() const;
};

#endif