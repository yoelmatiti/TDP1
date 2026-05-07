#ifndef TABLERO_H
#define TABLERO_H

#include "Bloque.h"
#include "Salida.h"
#include "Portal.h"

/**
 * Forward Declaration para evitar ciclos de inclusión.
 * Se usa porque actualizarDesdeEstado recibe un puntero a State.
 */
class State; 

/**
 * Direcciones de movimiento para los bloques.
 */
enum class Direccion { U, D, L, R }; 

class Tablero {
private:
    int width, height;
    int pasoActual;
    int bloquesRestantes;

    // Mapa base: Contiene muros '#', espacios ' ' y caracteres de portales.
    char* matrizContigua;
    char** matriz; 

    // Estructuras de datos dinámicas manuales (Sustituyen a std::vector)
    Bloque** bloques;
    int numBloques;
    int capacidadBloques;

    Salida** salidas;
    int numSalidas;
    int capacidadSalidas;

    Portal** portales;
    int numPortales;
    int capacidadPortales;

    // Métodos privados de gestión de memoria
    void liberarMemoria();
    // Nota: copiarDesde es opcional si no permites copias de tablero (recomendado para evitar fugas)
    void copiarDesde(const Tablero& otra);

public:
    Tablero();
    // Constructor de copia y operador de asignación para cumplir con la Regla de los Tres
    Tablero(const Tablero& otra);
    Tablero& operator=(const Tablero& otra);
    ~Tablero();

    // --- Métodos de Estado y Lógica ---
    // Sincroniza la posición de los objetos físicos del tablero con un estado lógico del Solver
    void actualizarDesdeEstado(State* s); 

    // --- Métodos de Configuración (Usados por LectorArchivo) ---
    void setDimensiones(int w, int h);
    void setPared(int fila, int col, char valor);
    void agregarBloque(Bloque* b);
    void agregarSalida(Salida* s);
    void agregarPortal(Portal* p);

    // --- API de Consulta (Utilizada por Movimiento.cpp y Solver.cpp) ---
    
    /**
     * Retorna el carácter almacenado en la matriz estática (muros o portales).
     */
    char getCeldaEstatica(int x, int y) const;
    
    /**
     * Busca y retorna una salida en coordenadas específicas.
     */
    Salida* getSalidaEn(int x, int y) const;
    
    /**
     * Busca y retorna un portal en coordenadas específicas.
     */
    Portal* getPortalEn(int x, int y) const;
    
    /**
     * Determina si hay un portal en la posición indicada.
     */
    bool esPortal(int x, int y) const;

    /**
     * Verifica que las coordenadas no se salgan del arreglo de la matriz.
     */
    bool enLimites(int x, int y) const;

    /**
     * Verifica si el bloque con id dado coincide en color con la salida en (x,y).
     */
    bool comprobarMeta(int idBloque, int x, int y) const;

    // --- Getters Lineales ---
    inline int getWidth() const       { return width; }
    inline int getHeight() const      { return height; }
    inline int getNumBloques() const  { return numBloques; }
    inline int getNumSalidas() const  { return numSalidas; }
    inline int getNumPortales() const { return numPortales; }
    
    // Acceso a punteros por índice
    Bloque* getBloquePtr(int i) const;
    Salida* getSalidaPtr(int i) const;
    Portal* getPortalPtr(int i) const;

    bool esPared(int f, int c) const {
    // Retorna true si la celda contiene '#'
    if (f < 0 || f >= height || c < 0 || c >= width) return false;
    return matriz[f][c] == '#'; 
    }

    // --- Visualización ---
    // Imprime el tablero en consola usando printf para cumplir "Sin STL"
    void imprimir();
};

#endif