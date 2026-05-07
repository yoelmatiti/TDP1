#include "Movimiento.h"
#include <cstdio> 

bool Movimiento::esValido(int bloqueIdx, Direccion dir, State* estadoActual, Tablero* tableroEstatico) {
    if (!estadoActual || !tableroEstatico) return false;

    int nB = estadoActual->getNumBloques();
    if (bloqueIdx < 0 || bloqueIdx >= nB) return false;

    PosBloque posB = estadoActual->getPosicion(bloqueIdx); // Uso de getter individual
    if (!posB.activo) return false;

    Bloque* b = tableroEstatico->getBloquePtr(bloqueIdx);
    if (!b) return false;

    int dx = 0, dy = 0;
    if (dir == Direccion::U) dy = -1;
    else if (dir == Direccion::D) dy = 1;
    else if (dir == Direccion::L) dx = -1;
    else if (dir == Direccion::R) dx = 1;

    int nx = posB.x + dx;
    int ny = posB.y + dy;

    for (int rY = 0; rY < b->getAltoGeo(); rY++) {
        for (int rX = 0; rX < b->getAnchoGeo(); rX++) {
            if (b->getGeometria(rY, rX)) {
                int tx = nx + rX;
                int ty = ny + rY;

                // A. Límites y Muros (Uso de métodos seguros de Tablero)
                if (!tableroEstatico->enLimites(tx, ty)) return false;
                if (tableroEstatico->esPared(ty, tx)) return false; 

                // B. Portales
                if (tableroEstatico->esPortal(tx, ty)) {
                    Portal* p = tableroEstatico->getPortalEn(tx, ty);
                    if (p) {
                        char cP = p->getColor();
                        char cB = b->getColor();
                        // Normalizar a minúscula sin STL
                        if (cP >= 'A' && cP <= 'Z') cP += 32;
                        if (cB >= 'A' && cB <= 'Z') cB += 32;
                        if (cP != cB) return false;
                    }
                }

                // C. Colisión Dinámica
                for (int j = 0; j < nB; j++) {
                    if (j == bloqueIdx) continue;
                    PosBloque posOtro = estadoActual->getPosicion(j);
                    if (!posOtro.activo) continue;

                    Bloque* otroB = tableroEstatico->getBloquePtr(j);
                    // ¿La celda tx, ty está ocupada por el otro bloque?
                    if (otroB->ocupaCelda(tx, ty, posOtro.x, posOtro.y)) return false;
                }
            }
        }
    }
    return true; 
}
State* Movimiento::ejecutar(int bloqueIdx, Direccion dir, State* actual, Tablero* tableroEstatico) {
    if (!actual || bloqueIdx < 0 || bloqueIdx >= actual->getNumBloques()) return nullptr;
    
    PosBloque posActual = actual->getPosicion(bloqueIdx);
    if (!posActual.activo) return nullptr;

    int dx = (dir == Direccion::R) ? 1 : (dir == Direccion::L ? -1 : 0);
    int dy = (dir == Direccion::D) ? 1 : (dir == Direccion::U ? -1 : 0);
    int NX = posActual.x + dx;
    int NY = posActual.y + dy;

    Bloque* b = tableroEstatico->getBloquePtr(bloqueIdx);
    
    // --- DEBUG: Inicio de movimiento ---
     printf("[DEBUG] Intentando mover Bloque %d a (%d, %d)\n", bloqueIdx, NX, NY);

    for (int rY = 0; rY < b->getAltoGeo(); rY++) {
        for (int rX = 0; rX < b->getAnchoGeo(); rX++) {
            if (b->getGeometria(rY, rX)) {
                int cx = NX + rX; 
                int cy = NY + rY; 

                if (!tableroEstatico->enLimites(cx, cy) || tableroEstatico->esPared(cy, cx)) {
                     printf("[DEBUG] Colisión con pared/limite en (%d, %d)\n", cx, cy);
                    return nullptr;
                }

                for (int j = 0; j < actual->getNumBloques(); j++) {
                    if (j == bloqueIdx) continue;
                    PosBloque bOtro = actual->getPosicion(j);
                    if (bOtro.activo) {
                        if (tableroEstatico->getBloquePtr(j)->ocupaCelda(cx, cy, bOtro.x, bOtro.y)) {
                            // printf("[DEBUG] Colisión con Bloque %d en (%d, %d)\n", j, cx, cy);
                            return nullptr;
                        }
                    }
                }

                if (tableroEstatico->esPortal(cx, cy)) {
                    Portal* p = tableroEstatico->getPortalEn(cx, cy);
                    if (p && !p->puedePasar(b->getColor(), actual->getG())) {
                        // printf("[DEBUG] Portal bloqueado por color/tiempo en (%d, %d)\n", cx, cy);
                        return nullptr; 
                    }
                }
            }
        }
    }

    int nB = actual->getNumBloques();
    PosBloque* nuevasPos = new PosBloque[nB];
    for (int i = 0; i < nB; i++) { nuevasPos[i] = actual->getPosicion(i); }

    nuevasPos[bloqueIdx].x = NX;
    nuevasPos[bloqueIdx].y = NY;

    // --- DEBUG: Verificación de Meta ---
    bool esMeta = tableroEstatico->comprobarMeta(bloqueIdx, NX, NY);
    // printf("[DEBUG] Resultado comprobarMeta para Bloque %d en (%d, %d): %s\n", 
    //        bloqueIdx, NX, NY, esMeta ? "EXITO" : "FALLIDO");

    if (esMeta) {
        nuevasPos[bloqueIdx].activo = false;
        // printf("[DEBUG] Bloque %d marcado como INACTIVO\n", bloqueIdx);
    } else {
        nuevasPos[bloqueIdx].activo = true;
    }

    const char* nombres[] = {"U", "D", "L", "R"};
    char desc[32];
    sprintf(desc, "B%d-%s", bloqueIdx + 1, nombres[(int)dir]);

    State* sucesor = new State(nB, nuevasPos, actual->getG() + 1, 0, actual, desc);
    
    // --- DEBUG: Verificación final del objeto sucesor ---
    // printf("[DEBUG] Sucesor creado. Activo en pos[0]: %s\n", 
    //        sucesor->getPosicion(bloqueIdx).activo ? "SI" : "NO");

    delete[] nuevasPos;
    return sucesor;
}