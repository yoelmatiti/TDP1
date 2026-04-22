#ifndef TABLERO_H
#define TABLERO_H

#include "Bloque.h"
#include "Salida.h"
#include "Portal.h"

// Direcciones de movimiento
enum class Direccion { U, D, L, R };

class Tablero {
private:
    // 1. Dimensiones y Estado Básico
    int width;              // Ancho W
    int height;             // Alto H
    int pasoActual;         // Turno actual del juego
    int bloquesRestantes;   // Contador para esEstadoFinal() O(1)

    // 2. Representación y Memoria Eficaz
    char* matrizContigua;         // Un solo bloque de memoria (eficacia de caché)
    char** matriz;                // Indexación matriz[y][x]
    mutable char* representacion; // Buffer para Tabla Hash / ClosedSet

    // 3. Objetos del Juego (Gestión mediante punteros para evitar copias pesadas)
    Bloque** bloques;       // Arreglo de punteros a Bloques
    int numBloques;
    int capacidadBloques;   // Para control del arreglo dinámico

    Salida** salidas;       // Arreglo de punteros a Salidas
    int numSalidas;

    Portal** portales;      // Arreglo de punteros a Portales
    int numPortales;

    // 4. Métodos Privados Auxiliares
    void liberarMemoria();
    void copiarDesde(const Tablero& otra);
    void generarRepresentacion() const;
    bool esMovimientoValido(int bloqueID, Direccion dir, int celdas) const;
    void verificarRevelacionIncognitos();
    void aplicarMovimiento(int bloqueID, Direccion dir, int distancia);
    void actualizarCompuertas();
    void actualizarSalidas();

public:
    // Constructores y Destructor (Regla de los tres)
    Tablero();
    Tablero(const Tablero& otra);
    Tablero& operator=(const Tablero& otra);
    ~Tablero();

    // --- Lógica Principal (Algoritmo A*) ---
    int moverBloque(int id, Direccion dir, int celdas);
    void actualizarAmbiente();
    bool esEstadoFinal() const { return bloquesRestantes == 0; }
    
    // --- Métodos para el Parser (Carga Eficiente) ---
    void setDimensiones(int w, int h);
    void agregarBloque(Bloque* b);
    void agregarSalida(Salida* s);
    void agregarPortal(Portal* p);
    void setPared(int fila, int col, char valor);

    // --- Getters Const (Acceso rápido para Heurística/Solver) ---
    int getWidth() const      { return width; }
    int getHeight() const     { return height; }
    int getPasoActual() const { return pasoActual; }
    char** getMatriz() const  { return matriz; }
    
    int getNumBloques() const { return numBloques; }
    Bloque* getBloque(int i)  { return bloques[i]; } // Retorna puntero al bloque i

    int getNumSalidas() const  { return numSalidas; }
    Salida* getSalida(int i)   { return salidas[i]; }

    int getNumPortales() const { return numPortales; }
    Portal* getPortal(int i)   { return portales[i]; }

    // --- Salida y Persistencia ---
    const char* getRepresentacion() const;
    void imprimirTablero() const;
};

#endif