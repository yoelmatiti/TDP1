#ifndef TABLERO_H
#define TABLERO_H

#include "Bloque.h"
#include "Salida.h"
#include "Compuerta.h"

// Usamos un enum para las direcciones de movimiento
enum class Direccion { U, D, L, R };

class Tablero {
private:
    int width;              // Ancho W
    int height;             // Alto H
    char** matriz;          // Representación visual y de colisiones
    
    // NUEVO: Variable de paso/turno actual
    int pasoActual;

    // Estructuras manuales (reemplazan a std::vector)
    Bloque* bloques;        
    int numBloques;
    Salida* salidas;
    int numSalidas;
    Compuerta* compuertas;
    int numCompuertas;

    // Buffer interno para representación única
    mutable char* representacion;

    // Métodos privados auxiliares
    void liberarMemoria();
    void copiarDesde(const Tablero& otra);
    void generarRepresentacion() const;
    bool esMovimientoValido(int bloqueID, Direccion dir, int celdas) const;
    void aplicarMovimiento(int bloqueID, Direccion dir, int distancia);
    void actualizarCompuertas();
    void actualizarSalidas();

public:
    // Constructor para el estado inicial
    Tablero();
    Tablero(int w, int h);

    // Regla de los tres (Indispensable sin STL)
    Tablero(const Tablero& otra);            // Constructor de copia
    Tablero& operator=(const Tablero& otra); // Operador de asignación
    ~Tablero();                              // Destructor

    // Métodos principales del juego
    bool moverBloque(int id, Direccion dir, int celdas);
    void actualizarAmbiente(); // Para compuertas y salidas temporales
    bool esEstadoFinal() const;

    // Getters
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getNumBloques() const { return numBloques; }
    int getNumSalidas() const { return numSalidas; }
    int getNumCompuertas() const { return numCompuertas; }
    int getPasoActual() const { return pasoActual; }
    
    Bloque* getBloques() { return bloques; }
    Salida* getSalidas() { return salidas; }
    Compuerta* getCompuertas() { return compuertas; }
    char** getMatriz() { return matriz; }

    // Setters para inicialización desde parser
    void setDimensiones(int w, int h);
    void agregarBloque(Bloque* bloque);
    void agregarSalida(Salida* salida);
    void agregarCompuerta(Compuerta* compuerta);

    // Representación única del estado
    const char* getRepresentacion() const;
    void imprimirTablero() const;
};

#endif