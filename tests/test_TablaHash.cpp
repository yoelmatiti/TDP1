#include <iostream>
#include "TablaHash.h"
#include "State.h"

void reportar(const char* msg, bool condicion) {
    std::cout << (condicion ? "[OK] " : "[ERROR] ") << msg << std::endl;
}

/**
 * Utilidad para crear un estado de prueba rápidamente.
 * Nota: En un entorno real, el Solver gestiona esta memoria.
 */
State* crearEstadoTest(int x, int y, int g, const char* op) {
    int nB = 1;
    PosBloque* pos = new PosBloque[nB];
    pos[0].x = x;
    pos[0].y = y;
    pos[0].activo = true;
    
    // El constructor de State hace su propia copia interna de 'pos'
    State* s = new State(nB, pos, g, 0, nullptr, op);
    delete[] pos; 
    return s;
}

int main() {
    std::cout << "--- Iniciando Test de TablaHash (Comparacion Fisica de Estados) ---" << std::endl;

    // Capacidad pequeña para testear colisiones en el encadenamiento
    TablaHash memoria(10); 

    // 1. Crear estados para el test
    // estado1 y estado3 son DIFERENTES objetos en memoria, pero REPRESENTAN la misma posicion
    State* s1 = crearEstadoTest(5, 5, 0, "Inicial");
    State* s2 = crearEstadoTest(5, 6, 1, "Mover Abajo");
    State* s3 = crearEstadoTest(5, 5, 2, "Llegue aqui por otra ruta");

    // 2. Prueba de inserción y existencia
    memoria.insertar(s1);
    reportar("S1 insertado y encontrado", memoria.existe(s1));

    // 3. Prueba de diferencia (Estado no explorado)
    reportar("S2 no deberia existir aun", !memoria.existe(s2));

    // 4. PRUEBA CRITICA: Duplicado Logico
    // La TablaHash DEBE reconocer que s3 es lo mismo que s1 aunque tengan diferente G y operacion.
    // Esto evita que el Solver entre en bucles infinitos.
    reportar("S3 (mismas pos que S1) detectado como ya explorado", memoria.existe(s3));

    // 5. Insercion de nuevos estados
    memoria.insertar(s2);
    reportar("S2 insertado y encontrado tras su insercion", memoria.existe(s2));

    // 6. Caso borde: Seguridad de punteros
    reportar("Busqueda de nullptr no produce crash", !memoria.existe(nullptr));

 

    std::cout << "--- Test de TablaHash finalizado ---" << std::endl;
    return 0;
}