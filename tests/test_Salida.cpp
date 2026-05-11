#include <cstdio>
#include <cassert>
#include "Salida.h"

// Eliminamos "using namespace std" ya que no usaremos la STL

void test_constructor() {
    printf("Probando constructor... ");
    // Parámetros: color, x, y, dirX, dirY, lenIni, lenFin, paso
    Salida s('R', 5, 10, 1, 0, 3, 1, 5);
    
    assert(s.getX() == 5);
    assert(s.getY() == 10);
    assert(s.getColor() == 'R');
    printf("PASADO\n");
}

void test_longitud_dinamica() {
    printf("Probando longitud segun tiempo... ");
    // Inicia en 3, cambia a 1 cada 5 turnos
    Salida s('G', 0, 0, 0, 1, 3, 1, 5);

    assert(s.getLongitudActual(0) == 3);  // Tiempo 0: Inicial
    assert(s.getLongitudActual(4) == 3);  // Tiempo 4: Sigue Inicial
    assert(s.getLongitudActual(5) == 1);  // Tiempo 5: Cambia a Final
    assert(s.getLongitudActual(9) == 1);  // Tiempo 9: Sigue Final
    assert(s.getLongitudActual(10) == 3); // Tiempo 10: Vuelve a Inicial
    printf("PASADO\n");
}

void test_colision_lineal() {
    printf("Probando deteccion de puntos (esParteDeSalida)... ");
    // Salida horizontal hacia la derecha (dirX=1, dirY=0), longitud 3
    Salida s('B', 2, 2, 1, 0, 3, 1, 10);

    // Tiempo 0 (longitud 3): debería cubrir (2,2), (2,3) y (2,4) 
    // Nota: en tu esParteDeSalida usas (f, c), aquí asumo f=y, c=x
    assert(s.esParteDeSalida(2, 2, 0) == true);  // Origen
    assert(s.esParteDeSalida(2, 3, 0) == true);  // Medio
    assert(s.esParteDeSalida(2, 4, 0) == true);  // Fin
    assert(s.esParteDeSalida(2, 5, 0) == false); // Fuera por longitud
    assert(s.esParteDeSalida(3, 2, 0) == false); // Fuera por fila (Y)

    // Tiempo 10 (longitud 1): solo debería cubrir el origen
    assert(s.esParteDeSalida(2, 2, 10) == true);
    assert(s.esParteDeSalida(2, 3, 10) == false);
    printf("PASADO\n");
}

// Sugerencia para test_colision_lineal o uno nuevo:
void test_identidad() {
    Salida s('R', 5, 5, 1, 0, 3, 1, 5);
    // Verificar que solo acepta bloques rojos
    char colorBloqueCorrecto = 'R';
    char colorBloqueIncorrecto = 'B';
    
    assert(s.getColor() == colorBloqueCorrecto);
    assert(s.getColor() != colorBloqueIncorrecto);
    printf("Test Identidad: PASADO\n");
}

int main() {
    printf("--- INICIANDO TESTS DE SALIDA (Sin STL) ---\n");
    
    test_constructor();
    test_longitud_dinamica();
    test_colision_lineal();
    test_identidad();


    printf("--- TODOS LOS TESTS PASARON CON EXITO ---\n");
    return 0;
}