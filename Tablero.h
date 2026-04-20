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

    // 2. Representación y Memoria
    char** matriz;          // Matriz de caracteres (colisiones/visual)
    mutable char* representacion; // Buffer para Tabla Hash

    // 3. Objetos del Juego (Gestión manual de memoria)
    Bloque* bloques;
    int numBloques;
    int bloquesRestantes;

    Salida* salidas;
    int numSalidas;

    Portal* portales;
    int numPortales;

    // Métodos privados auxiliares
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
    Tablero(int w, int h);
    Tablero(const Tablero& otra);
    Tablero& operator=(const Tablero& otra);
    ~Tablero();

    // Lógica Principal
    int moverBloque(int id, Direccion dir, int celdas);
    void actualizarAmbiente();
    bool esEstadoFinal() const;

    // Getters de Dimensiones y Estado
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getPasoActual() const { return pasoActual; }
    char** getMatriz() const { return matriz; }

    // Getters de Objetos (Individuales para el Solver/Heurística)
    int getNumBloques() const { return numBloques; }
    const Bloque& getBloque(int i) const { return bloques[i]; }

    int getNumSalidas() const { return numSalidas; }
    const Salida& getSalida(int i) const { return salidas[i]; }

    int getNumPortales() const { return numPortales; }
    
    // Getters de Arreglos Completos
    Bloque* getBloques() const { return bloques; }
    Salida* getSalidas() const { return salidas; }
    Portal* getPortales() const { return portales; }

    // Setters e Inicialización (Parser)
    void setDimensiones(int w, int h);
    void agregarBloque(Bloque* bloque);
    void agregarSalida(Salida* salida);
    void agregarPortal(Portal* portal);

    // Salida y Persistencia
    const char* getRepresentacion() const;
    void imprimirTablero() const;
};

#endif