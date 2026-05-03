#include <iostream>
#include "Portal.h"
#include <cassert>

void reportar(const char* msg, bool condicion) {
    std::cout << (condicion ? "[OK] " : "[ERROR] ") << msg << std::endl;
}

int main() {
    std::cout << "--- Iniciando Test de Clase Portal (Compuerta Dinamica) ---" << std::endl;

    // 1. Crear un portal en (3,4) que cambia cada 2 pasos
    // Añadimos 'V' (Vertical) como cuarto argumento
    Portal p(3, 4, 2, 'V');    p.agregarColorAlCiclo('a'); // Ciclo: a, b
    p.agregarColorAlCiclo('b');

    // 2. Probar color en diferentes tiempos (Aritmética Modular)
    // Tiempo 0 y 1 -> 'a'
    // Tiempo 2 y 3 -> 'b'
    reportar("Tiempo 0: Color es 'a'", p.getColorActual(0) == 'a');
    reportar("Tiempo 1: Color sigue siendo 'a'", p.getColorActual(1) == 'a');
    reportar("Tiempo 2: Color cambia a 'b'", p.getColorActual(2) == 'b');
    reportar("Tiempo 4: Ciclo reinicia a 'a'", p.getColorActual(4) == 'a');

    // 3. Probar lógica de paso de bloques
    reportar("Bloque 'a' pasa en tiempo 0", p.puedePasar('a', 0));
    reportar("Bloque 'b' NO pasa en tiempo 0", !p.puedePasar('b', 0));
    reportar("Bloque 'b' pasa en tiempo 2", p.puedePasar('b', 2));
    
    // 4. Probar Comodín
    reportar("Bloque comodin '?' siempre pasa", p.puedePasar('?', 5));

    // 5. Probar Copia Profunda (Importante para el Solver)
    Portal* p2 = new Portal(p);
    reportar("Copia mantiene color actual", p2->getColorActual(2) == 'b');
    
    p2->agregarColorAlCiclo('c');
    reportar("Modificar copia no afecta al original (Independencia)", p.getColorActual(0) == 'a');

    delete p2;
    std::cout << "--- Test de Portal finalizado ---" << std::endl;
    return 0;
}