#include <iostream>
#include "Compuerta.h"

/**
 * Función auxiliar para reportar resultados de manera clara.
 */
void reportar(const char* msg, bool condicion) {
    std::cout << (condicion ? "  [OK] " : "  [ERROR] ") << msg << std::endl;
}

int main() {
    std::cout << "--- PRUEBAS UNITARIAS: CLASE COMPUERTA (Sin STL) ---" << std::endl;

    // 1. Configuración Inicial
    // Compuerta en (3,4), frecuencia de cambio: 3 pasos, orientación: Vertical
    Compuerta p(3, 4, 3, 'V');
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
    // En T=0 el compuerta es 'a'
    reportar("Bloque 'a' intenta pasar compuerta 'a' (T=0): PERMITIDO", p.puedePasar('a', 0));
    // En T=3 el compuerta es 'b'
    reportar("Bloque 'b' intenta pasar compuerta 'b' (T=3): PERMITIDO", p.puedePasar('b', 3));
    // En T=6 el compuerta es 'c'
    reportar("Bloque 'c' intenta pasar compuerta 'c' (T=6): PERMITIDO", p.puedePasar('c', 6));

    std::cout << "\nTest 3: Interacción con Bloques (Distinto Color / Pared)" << std::endl;
    // En T=0 el compuerta es 'a', actúa como pared para cualquier otro color
    reportar("Bloque 'b' intenta pasar compuerta 'a' (T=0): BLOQUEADO", p.puedePasar('b', 0) == false);
    reportar("Bloque 'c' intenta pasar compuerta 'a' (T=1): BLOQUEADO", p.puedePasar('c', 1) == false);
    reportar("Bloque 'a' intenta pasar compuerta 'b' (T=4): BLOQUEADO", p.puedePasar('a', 4) == false);

    std::cout << "\nTest 4: Casos Especiales (Comodín y Mayúsculas)" << std::endl;
    // El comodín '?' siempre debe pasar
    reportar("Bloque comodín '?' siempre pasa (T=10): PERMITIDO", p.puedePasar('?', 10));
    // Verificación de robustez (si tu código maneja mayúsculas/minúsculas)
    reportar("Bloque 'A' vs compuerta 'a' (T=0): PERMITIDO", p.puedePasar('A', 0));

    std::cout << "\nTest 5: Integridad de Memoria (Copia Profunda)" << std::endl;
    // El Solver necesitará copiar compuertas miles de veces, la copia debe ser independiente
    Compuerta* pCopia = new Compuerta(p);
    
    reportar("Copia mantiene estado en T=3", pCopia->getColorActual(3) == 'b');
    
    // Modificar la copia
    pCopia->agregarColorAlCiclo('z'); 
    // p original tiene 3 colores (mod 3), pCopia ahora tiene 4 (mod 4)
    
    bool independencia = (p.getColorActual(9) == 'a') && (pCopia->getColorActual(9) == 'z');
    reportar("Modificar ciclo de la copia no afecta al original", independencia);


    std::cout << "\nTest 6: Lógica de Atravesar (Salto de Posición)" << std::endl;
    // Supongamos un bloque de 2x1 (ancho x alto) en la posición (2,4)
    // que se mueve a la Derecha (R) para cruzar la compuerta en (3,4)
    int bX = 2, bY = 4, bAncho = 2, bAlto = 1;
    int destinoX, destinoY;

    // Probamos cruzar a la derecha
    p.calcularDestino(bX, bY, bAncho, bAlto, Direccion::R, destinoX, destinoY);
    
    // Si el bloque mide 2 de ancho y está en X=2, ocupa X=2 y X=3. 
    // Para saltar la pared (X=4), debe aparecer en X=5.
    // Cálculo: 2 (pos inicial) + (2 (ancho) + 1 (pared)) = 5.
    reportar("Cálculo de destino a la Derecha (X=5)", destinoX == 5 && destinoY == 4);

    // Probamos cruzar hacia abajo (suponiendo misma posición inicial)
    p.calcularDestino(bX, bY, bAncho, bAlto, Direccion::D, destinoX, destinoY);
    // Cálculo: 4 (pos inicial Y) + (1 (alto) + 1 (pared)) = 6.
    reportar("Cálculo de destino hacia Abajo (Y=6)", destinoY == 6 && destinoX == 2);



    delete pCopia;
    std::cout << "\n--- Fin de Pruebas de Compuerta ---" << std::endl;

    return 0;
}