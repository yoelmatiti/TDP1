#include <iostream>
#include "Portal.h"

/**
 * Función auxiliar para reportar resultados de manera clara.
 */
void reportar(const char* msg, bool condicion) {
    std::cout << (condicion ? "  [OK] " : "  [ERROR] ") << msg << std::endl;
}

int main() {
    std::cout << "--- PRUEBAS UNITARIAS: CLASE PORTAL (Sin STL) ---" << std::endl;

    // 1. Configuración Inicial
    // Portal en (3,4), frecuencia de cambio: 3 pasos, orientación: Vertical
    Portal p(3, 4, 3, 'V');
    p.agregarColorAlCiclo('a'); // Índice 0
    p.agregarColorAlCiclo('b'); // Índice 1
    p.agregarColorAlCiclo('c'); // Índice 2

    std::cout << "\nTest 1: Lógica de Tiempo y Aritmética Modular" << std::endl;
    // Frecuencia 3 significa: 
    // T=0,1,2 -> 'a'
    // T=3,4,5 -> 'b'
    // T=6,7,8 -> 'c'
    // T=9 -> 'a' (reinicio)
    reportar("Tiempo 0 (Inicio): Color es 'a'", p.getColorActual(0) == 'a');
    reportar("Tiempo 2 (Límite): Color sigue siendo 'a'", p.getColorActual(2) == 'a');
    reportar("Tiempo 3 (Cambio): Color es ahora 'b'", p.getColorActual(3) == 'b');
    reportar("Tiempo 7 (Fase 3): Color es 'c'", p.getColorActual(7) == 'c');
    reportar("Tiempo 9 (Reinicio): Vuelve a 'a'", p.getColorActual(9) == 'a');

    std::cout << "\nTest 2: Interacción con Bloques (Mismo Color)" << std::endl;
    // En T=0 el portal es 'a'
    reportar("Bloque 'a' intenta pasar portal 'a' (T=0): PERMITIDO", p.puedePasar('a', 0));
    // En T=3 el portal es 'b'
    reportar("Bloque 'b' intenta pasar portal 'b' (T=3): PERMITIDO", p.puedePasar('b', 3));
    // En T=6 el portal es 'c'
    reportar("Bloque 'c' intenta pasar portal 'c' (T=6): PERMITIDO", p.puedePasar('c', 6));

    std::cout << "\nTest 3: Interacción con Bloques (Distinto Color / Pared)" << std::endl;
    // En T=0 el portal es 'a', actúa como pared para cualquier otro color
    reportar("Bloque 'b' intenta pasar portal 'a' (T=0): BLOQUEADO", p.puedePasar('b', 0) == false);
    reportar("Bloque 'c' intenta pasar portal 'a' (T=1): BLOQUEADO", p.puedePasar('c', 1) == false);
    reportar("Bloque 'a' intenta pasar portal 'b' (T=4): BLOQUEADO", p.puedePasar('a', 4) == false);

    std::cout << "\nTest 4: Casos Especiales (Comodín y Mayúsculas)" << std::endl;
    // El comodín '?' siempre debe pasar
    reportar("Bloque comodín '?' siempre pasa (T=10): PERMITIDO", p.puedePasar('?', 10));
    // Verificación de robustez (si tu código maneja mayúsculas/minúsculas)
    reportar("Bloque 'A' vs portal 'a' (T=0): PERMITIDO", p.puedePasar('A', 0));

    std::cout << "\nTest 5: Integridad de Memoria (Copia Profunda)" << std::endl;
    // El Solver necesitará copiar portales miles de veces, la copia debe ser independiente
    Portal* pCopia = new Portal(p);
    
    reportar("Copia mantiene estado en T=3", pCopia->getColorActual(3) == 'b');
    
    // Modificar la copia
    pCopia->agregarColorAlCiclo('z'); 
    // p original tiene 3 colores (mod 3), pCopia ahora tiene 4 (mod 4)
    
    bool independencia = (p.getColorActual(9) == 'a') && (pCopia->getColorActual(9) == 'z');
    reportar("Modificar ciclo de la copia no afecta al original", independencia);

    delete pCopia;
    std::cout << "\n--- Fin de Pruebas de Portal ---" << std::endl;

    return 0;
}