#include <iostream>
#include "Compuerta.h"

/**
 * Función auxiliar para reportar resultados de manera clara.
 */
void reportar(const char* msg, bool condicion) {
    std::cout << (condicion ? "  [OK] " : "  [ERROR] ") << msg << std::endl;
}

int main() {
    std::cout << "--- PRUEBAS UNITARIAS: CLASE COMPUERTA (Actualizado) ---" << std::endl;

    // 1. Configuración Inicial
    // CORRECCIÓN: Ahora pasamos "abc" como cadena en el 4to argumento.
    // El carácter de orientación 'V' u 'H' se maneja internamente o se puede omitir 
    // si tu constructor actual no lo pide, pero según tu .h actual es:
    // Compuerta(int _x, int _y, int _p, const char* _ciclo)
    
    Compuerta p(3, 4, 3, "abc"); 

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
    reportar("Bloque 'a' intenta pasar compuerta 'a' (T=0): PERMITIDO", p.puedePasar('a', 0));
    reportar("Bloque 'b' intenta pasar compuerta 'b' (T=3): PERMITIDO", p.puedePasar('b', 3));
    reportar("Bloque 'c' intenta pasar compuerta 'c' (T=6): PERMITIDO", p.puedePasar('c', 6));

    std::cout << "\nTest 3: Interacción con Bloques (Distinto Color / Pared)" << std::endl;
    reportar("Bloque 'b' intenta pasar compuerta 'a' (T=0): BLOQUEADO", p.puedePasar('b', 0) == false);
    reportar("Bloque 'c' intenta pasar compuerta 'a' (T=1): BLOQUEADO", p.puedePasar('c', 1) == false);
    reportar("Bloque 'a' intenta pasar compuerta 'b' (T=4): BLOQUEADO", p.puedePasar('a', 4) == false);

    std::cout << "\nTest 4: Casos Especiales (Comodín y Mayúsculas)" << std::endl;
    reportar("Bloque comodín '?' siempre pasa (T=10): PERMITIDO", p.puedePasar('?', 10));
    reportar("Bloque 'A' vs compuerta 'a' (T=0): PERMITIDO", p.puedePasar('A', 0));

    std::cout << "\nTest 5: Integridad de Memoria (Copia Profunda)" << std::endl;
    // Probamos el constructor de copia
    Compuerta pCopia = p; // Llama al constructor de copia
    
    reportar("Copia mantiene estado en T=3", pCopia.getColorActual(3) == 'b');
    
    // Modificar la copia agregando un color
    pCopia.agregarColorAlCiclo('z'); 
    
    // Si la copia es profunda, el original sigue teniendo mod 3 y la copia mod 4
    // T=9 para original (mod 3): 9/3 = 3. 3%3 = 0 -> ciclo[0] = 'a'
    // T=9 para copia (mod 4): 9/3 = 3. 3%4 = 3 -> ciclo[3] = 'z'
    bool independencia = (p.getColorActual(9) == 'a') && (pCopia.getColorActual(9) == 'z');
    reportar("Modificar ciclo de la copia no afecta al original", independencia);


    std::cout << "\nTest 6: Lógica de Atravesar (Salto de Posición)" << std::endl;
    int bX = 2, bY = 4, bAncho = 2, bAlto = 1;
    int destinoX, destinoY;

    p.calcularDestino(bX, bY, bAncho, bAlto, Direccion::R, destinoX, destinoY);
    reportar("Cálculo de destino a la Derecha (X=5)", destinoX == 5 && destinoY == 4);

    p.calcularDestino(bX, bY, bAncho, bAlto, Direccion::D, destinoX, destinoY);
    reportar("Cálculo de destino hacia Abajo (Y=6)", destinoY == 6 && destinoX == 2);

    std::cout << "\n--- Fin de Pruebas de Compuerta ---" << std::endl;

    return 0;
}