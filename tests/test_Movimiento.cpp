#include <iostream>
#include <vector>
#include "Tablero.h"
#include "Movimiento.h"
#include "State.h"
#include "Bloque.h"
#include "Salida.h"
#include "Portal.h" // Asegúrate de incluir el header de Portal

// Función auxiliar para crear una geometría simple de 1x1
bool* crearGeoSimple() {
    bool* g = new bool[1];
    g[0] = true;
    return g;
}

void test_colision_bloques() {
    std::cout << "Ejecutando: Test de colision entre bloques..." << std::endl;
    
    Tablero t;
    t.setDimensiones(10, 10);
    
    bool* geo1 = crearGeoSimple();
    bool* geo2 = crearGeoSimple();

    Bloque* b1 = new Bloque(1, 'A', 1, 1, 1, 1, geo1); 
    Bloque* b2 = new Bloque(2, 'B', 2, 1, 1, 1, geo2); 
    
    t.agregarBloque(b1);
    t.agregarBloque(b2);

    PosBloque* pos = new PosBloque[2];
    pos[0] = {1, 1, true};
    pos[1] = {2, 1, true};
    State* s_inicial = new State(2, pos, 0, 0, nullptr, "Inicial");

    Movimiento arbitro;

    State* s_fallido = arbitro.ejecutar(0, Direccion::R, s_inicial, &t);
    
    if (s_fallido == nullptr) {
        std::cout << "[OK] Movimiento bloqueado: A no pudo atravesar a B." << std::endl;
    } else {
        std::cout << "[ERROR] A atraveso a B!" << std::endl;
        delete s_fallido;
    }

    delete s_inicial;
    delete[] pos;
}

void test_meta_desactivacion() {
    std::cout << "\nEjecutando: Test de llegada a meta..." << std::endl;
    
    Tablero t;
    t.setDimensiones(10, 10);
    
    bool* geoA = crearGeoSimple();
    Bloque* b1 = new Bloque(1, 'A', 1, 1, 1, 1, geoA);
    t.agregarBloque(b1);
    
    Salida* s1 = new Salida('A', 2, 1, 0, 0, 0, 0, 0);
    t.agregarSalida(s1);

    PosBloque* pos = new PosBloque[1];
    pos[0] = {1, 1, true};
    State* s_inicial = new State(1, pos, 0, 0, nullptr, "Inicial");

    Movimiento arbitro;

    State* s_meta = arbitro.ejecutar(0, Direccion::R, s_inicial, &t);

    if (s_meta != nullptr && s_meta->getPosiciones()[0].activo == false) {
        std::cout << "[OK] El bloque se desactivo al llegar a la meta." << std::endl;
    } else {
        std::cout << "[ERROR] El bloque sigue activo o no se genero el estado." << std::endl;
    }

    delete s_inicial;
    if(s_meta) delete s_meta;
    delete[] pos;
}

/**
 * NUEVA PRUEBA: TEST DEL PORTAL
 */
void test_portal_color() {
    std::cout << "\nEjecutando: Test de filtro de Portal..." << std::endl;
    
    Tablero t;
    t.setDimensiones(10, 10);
    
    bool* geoA = crearGeoSimple();
    // Bloque 'A' en (1, 1)
    Bloque* b1 = new Bloque(1, 'A', 1, 1, 1, 1, geoA);
    t.agregarBloque(b1);
    
    // Portal en (2, 1). 
    // Argumentos: x=2, y=1, p=0 (id/tipo), ori='B' (color azul)
    Portal* p1 = new Portal(2, 1, 0, 'B'); 
    t.agregarPortal(p1);

    PosBloque* pos = new PosBloque[1];
    pos[0] = {1, 1, true};
    State* s_inicial = new State(1, pos, 0, 0, nullptr, "Inicial");

    Movimiento arbitro;

    // Intentar mover hacia la derecha (donde está el portal 'B')
    State* s_resultado = arbitro.ejecutar(0, Direccion::R, s_inicial, &t);

    if (s_resultado == nullptr) {
        std::cout << "[OK] Portal bloqueado: Bloque A rebotó en Portal B correctamente." << std::endl;
    } else {
        std::cout << "[ERROR] El bloque A atravesó un portal de color distinto (B)!" << std::endl;
        delete s_resultado;
    }

    delete s_inicial;
    delete[] pos;
}

int main() {
    std::cout << "--- PRUEBAS UNITARIAS: CLASE MOVIMIENTO ---" << std::endl;
    test_colision_bloques();
    test_meta_desactivacion();
    test_portal_color(); // Llamada a la nueva prueba
    std::cout << "------------------------------------------" << std::endl;
    return 0;
}