#include "Movimiento.h"
#include <cstdio> 
#include "Tablero.h"

bool Movimiento::esValido(int bloqueIdx, Direccion dir, State* estadoActual, Tablero* tableroEstatico) {
    if (!estadoActual || !tableroEstatico) return false;

    // --- AGREGAMOS ESTAS DECLARACIONES QUE FALTABAN ---
    int nB = estadoActual->getNumBloques();
    PosBloque posB = estadoActual->getPosicion(bloqueIdx);
    if (!posB.activo) return false;

    Bloque* b = tableroEstatico->getBloquePtr(bloqueIdx);
    if (!b) return false;

    // Calculamos el desplazamiento
    int dx = (dir == Direccion::R) ? 1 : (dir == Direccion::L ? -1 : 0);
    int dy = (dir == Direccion::D) ? 1 : (dir == Direccion::U ? -1 : 0);
    
    // Declaramos nx y ny (la posición destino)
    int nx = posB.x + dx;
    int ny = posB.y + dy;
    // --------------------------------------------------

    for (int rY = 0; rY < b->getAltoGeo(); rY++) {
        for (int rX = 0; rX < b->getAnchoGeo(); rX++) {
            if (b->getGeometria(rY, rX)) {
                int tx = nx + rX;
                int ty = ny + rY;

                // A. Obstáculos y Compuertas
                if (tableroEstatico->esObstaculo(tx, ty, estadoActual->getG(), b->getColor())) {
                    return false; 
                }

                // B. Colisión con otros bloques
                for (int j = 0; j < nB; j++) {
                    if (j == bloqueIdx) continue;
                    PosBloque posOtro = estadoActual->getPosicion(j);
                    if (!posOtro.activo) continue;

                    Bloque* otroB = tableroEstatico->getBloquePtr(j);
                    if (otroB->ocupaCelda(tx, ty, posOtro.x, posOtro.y)) return false;
                }
            }
        }
    }
    return true; 
}

State* Movimiento::ejecutar(int bloqueIdx, Direccion dir, State* actual, Tablero* tableroEstatico) {
    if (!esValido(bloqueIdx, dir, actual, tableroEstatico)) return nullptr;

    PosBloque posActual = actual->getPosicion(bloqueIdx);
    int dx = (dir == Direccion::R) ? 1 : (dir == Direccion::L ? -1 : 0);
    int dy = (dir == Direccion::D) ? 1 : (dir == Direccion::U ? -1 : 0);
    int NX = posActual.x + dx;
    int NY = posActual.y + dy;

    int nB = actual->getNumBloques();
    PosBloque* nuevasPos = new PosBloque[nB];
    for (int i = 0; i < nB; i++) { nuevasPos[i] = actual->getPosicion(i); }

    nuevasPos[bloqueIdx].x = NX;
    nuevasPos[bloqueIdx].y = NY;

    // --- CORRECCIÓN DEL ERROR DE COMPROBARMETA ---
    // Quitamos el cuarto argumento (G) porque tu Tablero.h no lo tiene
    nuevasPos[bloqueIdx].activo = !tableroEstatico->comprobarMeta(bloqueIdx, NX, NY, actual->getG() + 1);

    const char* nombres[] = {"U", "D", "L", "R"};
    char desc[32];
    sprintf(desc, "B%d-%s", bloqueIdx + 1, nombres[(int)dir]);

    State* sucesor = new State(nB, nuevasPos, actual->getG() + 1, 0, actual, desc);
    
    delete[] nuevasPos;
    return sucesor;
}