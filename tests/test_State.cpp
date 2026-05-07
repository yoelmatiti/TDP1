#include "Tablero.h"
#include "State.h"
#include "LectorArchivo.h"
#include <cstdio>

int main() {
    LectorArchivo lector;
    Tablero* tableroMaestro = lector.cargarNivel("facil2_corregido.txt");

    if (tableroMaestro == nullptr) {
        printf("[ERROR] No se pudo cargar el nivel.\n");
        return 1;
    }

    printf("--- Test de Consistencia de Estado ---\n");

    // 2. Preparar datos iniciales
    int nB = tableroMaestro->getNumBloques();
    PosBloque* posIniciales = new PosBloque[nB];
    for (int i = 0; i < nB; i++) {
        Bloque* b = tableroMaestro->getBloquePtr(i);
        posIniciales[i].x = b->getX();
        posIniciales[i].y = b->getY();
        posIniciales[i].activo = true;
    }

    // 3. Crear s1 y su copia s2
    State* s1 = new State(nB, posIniciales, 0, 0, nullptr, "Inicio");
    State* s2 = new State(*s1); // Constructor de copia (Deep Copy)

    printf("Verificando independencia de memoria...\n");
    
    // Validamos que al inicio son iguales
    if (s1->operator==(*s2)) {
        printf("[OK] El operador == detecta que son identicos tras copiar.\n");
    }

    // 4. Probar independencia creando un s3 con cambios (Simulando un movimiento)
    // En lugar de modificar s2 (que es inmutable), creamos s3 basado en s1 pero con una diff
    posIniciales[0].x += 1; // Simulamos que el primer bloque se movio a la derecha
    State* s3 = new State(nB, posIniciales, 1, 10, s1, "B0 R");

    // Verificamos que s1 NO cambio sus valores internos por culpa de s3
    if (s1->getPosicion(0).x != s3->getPosicion(0).x) {
        printf("[OK] Independencia confirmada: s1 mantiene x=%d, s3 tiene x=%d\n", 
                s1->getPosicion(0).x, s3->getPosicion(0).x);
    }

    // 5. Probar el rastro de operaciones (Recursividad estilo profesor)
    printf("\nVisualizando rastro de movimientos (de s3 hacia atras):\n");
    s3->printOperaciones();

    // 6. Probar esFinal
    if (!s1->esFinal()) {
        printf("\n[OK] s1 detecta que aun quedan bloques activos.\n");
    }

    // Limpieza
    delete[] posIniciales;
    delete s1;
    delete s2;
    delete s3;
    delete tableroMaestro;

    printf("--- Test finalizado ---\n");
    return 0;
}