#include "Movimiento.h"
#include <cstdio> // Para sprintf

bool Movimiento::esValido(int bloqueIdx, Direccion dir, State* estadoActual, Tablero* tableroEstatico) {
    if (!estadoActual || !tableroEstatico) return false;

    PosBloque* posiciones = estadoActual->getPosiciones();
    int nB = estadoActual->getNumBloques();
    
    if (bloqueIdx < 0 || bloqueIdx >= nB) return false;
    if (!posiciones[bloqueIdx].activo) return false;

    Bloque* b = tableroEstatico->getBloquePtr(bloqueIdx);
    if (!b) return false;

    // 1. Cálculo de la nueva posición hipotética (nx, ny)
    int dx = (dir == Direccion::R) - (dir == Direccion::L);
    int dy = (dir == Direccion::D) - (dir == Direccion::U);

    int nx = posiciones[bloqueIdx].x + dx;
    int ny = posiciones[bloqueIdx].y + dy;

    // 2. Validación de Geometría por celda
    for (int rY = 0; rY < b->getAltoGeo(); rY++) {
        for (int rX = 0; rX < b->getAnchoGeo(); rX++) {
            if (b->getGeometria(rY, rX)) {
                int tx = nx + rX;
                int ty = ny + rY;

                // A. Límites del Tablero
                if (tx < 0 || tx >= tableroEstatico->getWidth() || 
                    ty < 0 || ty >= tableroEstatico->getHeight()) return false;

                // B. Muros y Portales
                char celda = tableroEstatico->getCeldaEstatica(tx, ty); 
                if (celda == '#') return false;

                // C. Seguridad en Portales
                if (tableroEstatico->esPortal(ty, tx)) {
                    Portal* p = tableroEstatico->getPortalEn(tx, ty);
                    if (p == nullptr) return false; 

                    // Comparación manual de caracteres (sin depender de std::string)
                    char cP = p->getColor();
                    char cB = b->getColor();
                    if (cP >= 'A' && cP <= 'Z') cP += 32; // To lower manual
                    if (cB >= 'A' && cB <= 'Z') cB += 32; // To lower manual

                    if (cP != cB) return false; 
                }

                // D. Colisión Dinámica (Otros Bloques)
                for (int j = 0; j < nB; j++) {
                    if (j == bloqueIdx || !posiciones[j].activo) continue;
                    
                    Bloque* otroB = tableroEstatico->getBloquePtr(j);
                    if (!otroB) continue; 
                    
                    int relX = tx - posiciones[j].x;
                    int relY = ty - posiciones[j].y;

                    if (relX >= 0 && relX < otroB->getAnchoGeo() && 
                        relY >= 0 && relY < otroB->getAltoGeo()) {
                        if (otroB->getGeometria(relY, relX)) return false;
                    }
                }
            }
        }
    }
    return true; 
}

State* Movimiento::ejecutar(int bloqueIdx, Direccion dir, State* actual, Tablero* tableroEstatico) {
    if (!esValido(bloqueIdx, dir, actual, tableroEstatico)) return nullptr;

    int nB = actual->getNumBloques();
    PosBloque* nuevasPos = new PosBloque[nB];
    
    for (int i = 0; i < nB; i++) {
        nuevasPos[i] = actual->getPosiciones()[i];
    }

    int dx = (dir == Direccion::R) - (dir == Direccion::L);
    int dy = (dir == Direccion::D) - (dir == Direccion::U);

    nuevasPos[bloqueIdx].x += dx;
    nuevasPos[bloqueIdx].y += dy;

    int nx = nuevasPos[bloqueIdx].x;
    int ny = nuevasPos[bloqueIdx].y;
    Bloque* b = tableroEstatico->getBloquePtr(bloqueIdx);

    if (tableroEstatico->comprobarMeta(b->getId(), nx, ny)) {
        nuevasPos[bloqueIdx].activo = false;
    }

    // Usamos sprintf de C (sin std::)
    const char* nombres[] = {"Arriba", "Abajo", "Izquierda", "Derecha"};
    char desc[64];
    sprintf(desc, "Bloque %d movido a %s", b->getId(), nombres[(int)dir]);

    int nuevoG = actual->getG() + 1;
    State* sucesor = new State(nB, nuevasPos, nuevoG, 0, actual, desc);

    delete[] nuevasPos;
    return sucesor;
}