#ifndef TABLERO_H
#define TABLERO_H

#include "Bloque.h"
#include "Salida.h"
#include "Portal.h"

/**
 * DECLARACIÓN DE AVANCE (Forward Declaration)
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

    // Mapa base: Solo contiene muros '#' y espacios vacíos.
    char* matrizContigua;
    char** matriz; 

    Bloque** bloques;
    int numBloques;
    int capacidadBloques;

    Salida** salidas;
    int numSalidas;
    int capacidadSalidas;

    Portal** portales;
    int numPortales;
    int capacidadPortales;

    void liberarMemoria();
    void copiarDesde(const Tablero& otra);

public:
    Tablero();
    Tablero(const Tablero& otra);
    Tablero& operator=(const Tablero& otra);
    ~Tablero();

    // --- Métodos de Estado y Lógica ---
    void actualizarDesdeEstado(State* s); // Sincroniza y estampa bloques para renderizar

    // --- Métodos de Configuración (Carga del Nivel) ---
    void setDimensiones(int w, int h);
    void setPared(int fila, int col, char valor);
    void agregarBloque(Bloque* b);
    void agregarSalida(Salida* s);
    void agregarPortal(Portal* p);

    // --- API de Consulta Estática (Para Clase Movimiento) ---
    /**
     * Retorna el contenido del mapa base en (x, y). Útil para muros.
     */
    char getCeldaEstatica(int x, int y) const;
    
    /**
     * Retorna puntero a salida en (x, y) si existe, nullptr si no.
     */
    Salida* getSalidaEn(int x, int y) const;
    
    /**
     * Retorna puntero a portal en (x, y) si existe, nullptr si no.
     */
    Portal* getPortalEn(int x, int y) const;
    bool esPortal(int y, int x) const;

    /**
     * Verifica si una coordenada está dentro de los límites del arreglo.
     */
    bool enLimites(int x, int y) const;

    /**
     * Compara el bloque (id) con la salida en (x, y) para verificar meta.
     */
    bool comprobarMeta(int id, int x, int y) const;

    // --- Getters ---
    inline int getWidth() const       { return width; }
    inline int getHeight() const      { return height; }
    inline int getNumBloques() const  { return numBloques; }
    inline int getNumSalidas() const  { return numSalidas; }
    inline int getNumPortales() const { return numPortales; }
    
    Bloque* getBloquePtr(int i) const;
    Salida* getSalidaPtr(int i) const;
    Portal* getPortalPtr(int i) const;

    // --- Visualización ---
    void imprimir();
};

#endif