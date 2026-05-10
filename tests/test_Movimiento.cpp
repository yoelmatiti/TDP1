#include <iostream>
// Eliminado <vector> para cumplir con la regla de No STL
#include "Tablero.h"
#include "Movimiento.h"
#include "State.h"
#include "Bloque.h"
#include "Salida.h"
#include "Compuerta.h" 
#include <cstring>
/**
 * Función auxiliar para crear una geometría simple de 1x1.
 * En un entorno real, Bloque debería encargarse de su propia memoria,
 * pero para el test lo manejamos así.
 */
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

    // ID, Color, X, Y, W, H, Geo
    Bloque* b1 = new Bloque(1, 'A', 1, 1, 1, 1, geo1); 
    Bloque* b2 = new Bloque(2, 'B', 2, 1, 1, 1, geo2); 
    
    t.agregarBloque(b1);
    t.agregarBloque(b2);

    // Definimos el estado inicial
    PosBloque* pos = new PosBloque[2];
    pos[0] = {1, 1, true};
    pos[1] = {2, 1, true};
    State* s_inicial = new State(2, pos, 0, 0, nullptr, "Inicial");

    // Intentar mover bloque 0 (A) a la derecha (hacia la posición del bloque B)
    State* s_fallido = Movimiento::ejecutar(0, Direccion::R, s_inicial, &t);
    
    if (s_fallido == nullptr) {
        std::cout << "[OK] Movimiento bloqueado: A no pudo atravesar a B." << std::endl;
    } else {
        std::cout << "[ERROR] A atraveso a B!" << std::endl;
        delete s_fallido;
    }

    delete s_inicial;
    delete[] pos;
    // Nota: El destructor de Tablero debería liberar b1 y b2 si así lo diseñaste.
}

void test_meta_desactivacion() {
    std::cout << "\nEjecutando: Test de llegada a meta..." << std::endl;
    
    Tablero t;
    t.setDimensiones(10, 10);
    
    bool* geoA = crearGeoSimple();
    Bloque* b1 = new Bloque(1, 'A', 1, 1, 1, 1, geoA);
    t.agregarBloque(b1);
    
    // Salida: color, x, y, direccionX, direccionY, longitudInicial, longitudFinal, pasoCambio
    // En este caso: exitida 'A' en (2,1), extendida horizontalmente (hacia la derecha), longitud inicial 1, final 1, paso 0
    Salida* s1 = new Salida('A', 2, 1, 1, 0, 1, 1, 0);
    t.agregarSalida(s1);

    PosBloque* pos = new PosBloque[1];
    pos[0] = {1, 1, true};
    State* s_inicial = new State(1, pos, 0, 0, nullptr, "Inicial");

    // Movimiento al este donde está la salida
    State* s_meta = Movimiento::ejecutar(0, Direccion::R, s_inicial, &t);

    if (s_meta != nullptr && s_meta->getPosicion(0).activo == false) {
        std::cout << "[OK] El bloque se desactivo al llegar a la meta." << std::endl;
    } else {
        std::cout << "[ERROR] El bloque sigue activo o no se genero el estado correctamente." << std::endl;
    }

    delete s_inicial;
    if(s_meta) delete s_meta;
    delete[] pos;
}

void test_compuerta_color() {
    std::cout << "\nEjecutando: Test de filtro de Compuerta..." << std::endl;
    
    Tablero t;
    t.setDimensiones(10, 10);
    
    bool* geoA = crearGeoSimple();
    // Bloque 'A' (Rojo/id 1)
    Bloque* b1 = new Bloque(1, 'A', 1, 1, 1, 1, geoA);
    t.agregarBloque(b1);
    
    // Compuerta de color 'B' (Azul) en el camino (2, 1)
    // Constructor asume: X, Y, Paso_o_ID, Color
    Compuerta* p1 = new Compuerta(2, 1, 0, "B"); 
    t.agregarCompuerta(p1);

    PosBloque* pos = new PosBloque[1];
    pos[0] = {1, 1, true};
    State* s_inicial = new State(1, pos, 0, 0, nullptr, "Inicial");

    // Bloque A intenta entrar a Compuerta B -> Debe retornar nullptr
    State* s_resultado = Movimiento::ejecutar(0, Direccion::R, s_inicial, &t);

    if (s_resultado == nullptr) {
        std::cout << "[OK] Compuerta bloqueado: Bloque A rebotó en Compuerta B correctamente." << std::endl;
    } else {
        std::cout << "[ERROR] El bloque A atravesó un compuerta de color distinto (B)!" << std::endl;
        delete s_resultado;
    }

    delete s_inicial;
    delete[] pos;
}

int main() {
    std::cout << "--- PRUEBAS UNITARIAS: SISTEMA DE MOVIMIENTO ---" << std::endl;
    
    test_colision_bloques();
    test_meta_desactivacion();
    test_compuerta_color();
    
    std::cout << "-----------------------------------------------" << std::endl;
    return 0;
}