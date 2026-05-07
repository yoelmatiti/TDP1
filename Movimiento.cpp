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
    // 1. VALIDACIÓN DE IDENTIDAD Y ESTADO ACTIVO
    if (!actual || bloqueIdx < 0 || bloqueIdx >= actual->getNumBloques()) return nullptr;
    
    PosBloque posActual = actual->getPosicion(bloqueIdx);
    if (!posActual.activo) return nullptr;

    // 2. CÁLCULO DE POSICIÓN TENTATIVA
    int dx = (dir == Direccion::R) ? 1 : (dir == Direccion::L ? -1 : 0);
    int dy = (dir == Direccion::D) ? 1 : (dir == Direccion::U ? -1 : 0);
    int NX = posActual.x + dx;
    int NY = posActual.y + dy;

    // 3. DETECCIÓN DE COLISIONES (Física de Geometría Completa)
    Bloque* b = tableroEstatico->getBloquePtr(bloqueIdx);
    
    for (int rY = 0; rY < b->getAltoGeo(); rY++) {
        for (int rX = 0; rX < b->getAnchoGeo(); rX++) {
            if (b->getGeometria(rY, rX)) {
                int cx = NX + rX; 
                int cy = NY + rY; 

                // A. Colisión con Muros o fuera de límites
                if (!tableroEstatico->enLimites(cx, cy) || tableroEstatico->esPared(cy, cx)) {
                    return nullptr;
                }

                // B. Colisión con otros bloques activos
                for (int j = 0; j < actual->getNumBloques(); j++) {
                    if (j == bloqueIdx) continue;
                    PosBloque bOtro = actual->getPosicion(j);
                    if (bOtro.activo) {
                        if (tableroEstatico->getBloquePtr(j)->ocupaCelda(cx, cy, bOtro.x, bOtro.y)) {
                            return nullptr;
                        }
                    }
                }

                // C. Portales Dinámicos
                if (tableroEstatico->esPortal(cx, cy)) {
                    Portal* p = tableroEstatico->getPortalEn(cx, cy);
                    if (p && !p->puedePasar(b->getColor(), actual->getG())) {
                        return nullptr; 
                    }
                }
            }
        }
    }

    // 4. TRANSICIÓN DE ESTADO Y LÓGICA DE META
    int nB = actual->getNumBloques();
    PosBloque* nuevasPos = new PosBloque[nB];
    
    // Copiamos todas las posiciones actuales al nuevo arreglo
    for (int i = 0; i < nB; i++) {
        nuevasPos[i] = actual->getPosicion(i);
    }

    // Actualizamos la posición del bloque que se movió
    nuevasPos[bloqueIdx].x = NX;
    nuevasPos[bloqueIdx].y = NY;

    // DETERMINAR SI ES META (Desactivación)
    if (tableroEstatico->comprobarMeta(bloqueIdx, NX, NY)) {
        // El bloque llega a la salida: se marca como inactivo
        nuevasPos[bloqueIdx].activo = false;
    } else {
        // Movimiento normal: sigue activo
        nuevasPos[bloqueIdx].activo = true;
    }

    // 5. GESTIÓN DE COSTOS Y GENERACIÓN DE OPERACIÓN
    const char* nombres[] = {"U", "D", "L", "R"};
    char desc[32];
    // Formato B{ID}-{DIR} (ej: B1-D)
    sprintf(desc, "B%d-%s", bloqueIdx + 1, nombres[(int)dir]);

    // Crear el nuevo estado con G + 1 y h = 0 (la heurística la calculará el Solver)
    State* sucesor = new State(nB, nuevasPos, actual->getG() + 1, 0, actual, desc);

    // Liberar el arreglo temporal (el constructor de State ya hizo su copia profunda)
    delete[] nuevasPos;
    
    return sucesor;
}