#include "Tablero.h"
#include "State.h"
#include "LectorArchivo.h"
#include <cstdio>
#include <iostream>

int main() {
    LectorArchivo lector;
    Tablero* tableroMaestro = lector.cargarNivel("simple1.txt");

    if (tableroMaestro == nullptr) {
        printf("[ERROR] No se pudo cargar el nivel.\n");
        return 1;
    }

    printf("--- Test de Consistencia de Estado ---\n");

    int nB = tableroMaestro->getNumBloques();
    PosBloque* posIniciales = new PosBloque[nB];
    for (int i = 0; i < nB; i++) {
        Bloque* b = tableroMaestro->getBloquePtr(i);
        posIniciales[i].x = b->getX();
        posIniciales[i].y = b->getY();
        posIniciales[i].activo = true;
    }

    State* s1 = new State(nB, posIniciales, 0, 0, nullptr, "Inicio");
    State* s2 = new State(*s1); // Deep Copy

    printf("Verificando independencia de memoria...\n");
    if (s1->operator==(*s2)) {
        printf("[OK] El operador == detecta que son identicos tras copiar.\n");
    }

    // --- MODIFICACIÓN AQUÍ ---
    // Simulamos movimiento: incrementamos X del primer bloque
    posIniciales[0].x += 1; 
    State* s3 = new State(nB, posIniciales, 1, 10, s1, "B0 R");

    // Bloque de depuración adaptado: usamos s3 en lugar de s_meta
    if (s3 != nullptr) {
        std::cout << "\n[DEBUG State] Datos de s3 (estado movido):" << std::endl;
        std::cout << "  - Posicion Bloque 0: (" 
                  << s3->getPosicion(0).x << "," 
                  << s3->getPosicion(0).y << ")" << std::endl;
        std::cout << "  - Estado activo: " 
                  << (s3->getPosicion(0).activo ? "SI" : "NO") << std::endl;
    }
    // -------------------------

    if (s1->getPosicion(0).x != s3->getPosicion(0).x) {
        printf("[OK] Independencia confirmada: s1 mantiene x=%d, s3 tiene x=%d\n", 
                s1->getPosicion(0).x, s3->getPosicion(0).x);
    }

    printf("\nVisualizando rastro de movimientos (de s3 hacia atras):\n");
    s3->printOperaciones();

    if (!s1->esFinal()) {
        printf("\n[OK] s1 detecta que aun quedan bloques activos.\n");
    }

    delete[] posIniciales;
    delete s1;
    delete s2;
    delete s3;
    delete tableroMaestro;

    printf("--- Test finalizado ---\n");
    return 0;
}