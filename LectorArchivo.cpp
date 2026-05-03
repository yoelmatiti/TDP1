#include "LectorArchivo.h"
#include <iostream>
#include <sstream>

Tablero* LectorArchivo::cargarNivel(const std::string& ruta) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) return nullptr;

    std::string linea, seccion;
    int w = 0, h = 0;
    Tablero* tablero = nullptr;

    while (std::getline(archivo, linea)) {
        if (linea.empty() || linea[0] == '#') continue;

        if (linea.find("WIDTH") != std::string::npos) sscanf(linea.c_str(), "WIDTH = %d", &w);
        else if (linea.find("HEIGHT") != std::string::npos) sscanf(linea.c_str(), "HEIGHT = %d", &h);
        
        if (w > 0 && h > 0 && tablero == nullptr) {
            tablero = new Tablero();
            tablero->setDimensiones(w, h); // Usamos tu método existente
        }

        if (linea[0] == '[') { seccion = linea; continue; }
        if (tablero == nullptr) continue;

        if (seccion == "[BLOCK]") {
            int id, bw, bh, ix, iy;
            char color;
            if (sscanf(linea.c_str(), "%d COLOR=%c WIDTH=%d HEIGHT=%d INIT_X=%d INIT_Y=%d", 
                       &id, &color, &bw, &bh, &ix, &iy) == 6) {
                
                // 1. Crear geometría dinámica (se borra en el destructor de Bloque)
                bool* geo = new bool[bw * bh];
                size_t posGeo = linea.find("GEOMETRY=");
                if (posGeo != std::string::npos) {
                    std::stringstream ss(linea.substr(posGeo + 9));
                    int val;
                    for (int i = 0; i < (bw * bh); i++) {
                        ss >> val;
                        geo[i] = (val == 1);
                    }
                }
                
                // 2. IMPORTANTE: Usar 'new' porque agregarBloque pide Bloque*
                Bloque* b = new Bloque(id, color, ix, iy, bw, bh, geo);
                tablero->agregarBloque(b);
            }
        } 
        else if (seccion == "[WALL]") {
            static int filaMuro = 0;
            if (filaMuro < h) {
                for (int col = 0; col < (int)linea.length() && col < w; col++) {
                    // Usamos setPared de tu Tablero.h
                    tablero->setPared(filaMuro, col, linea[col]);
                }
                filaMuro++;
            }
        }
        else if (seccion == "[EXIT]") {
            int ex, ey, li, lf, p;
            char ec, ori;
            if (sscanf(linea.c_str(), "COLOR=%c X=%d Y=%d ORIENTATION=%c LI=%d LF=%d STEP=%d",
                       &ec, &ex, &ey, &ori, &li, &lf, &p) == 7) {
                int dx = (ori == 'H') ? 1 : 0;
                int dy = (ori == 'V') ? 1 : 0;
                
                // 3. Usar 'new' porque agregarSalida pide Salida*
                Salida* s = new Salida(ec, ex, ey, dx, dy, li, lf, p);
                tablero->agregarSalida(s);
            }
        }
    }
    return tablero;
}