#include "LectorArchivo.h"
#include <iostream>
#include <sstream>

Tablero* LectorArchivo::cargarNivel(const std::string& ruta) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir " << ruta << std::endl;
        return nullptr;
    }

    std::string linea, seccion;
    int w = 0, h = 0, limite = 0;
    Tablero* tablero = nullptr;

    while (std::getline(archivo, linea)) {
        if (linea.empty() || linea[0] == '#') continue;

        // Lectura de cabecera
        if (linea.find("WIDTH") != std::string::npos) {
            sscanf(linea.c_str(), "WIDTH = %d", &w);
        } else if (linea.find("HEIGHT") != std::string::npos) {
            sscanf(linea.c_str(), "HEIGHT = %d", &h);
        } else if (linea.find("STEP_LIMIT") != std::string::npos) {
            sscanf(linea.c_str(), "STEP_LIMIT = %d", &limite);
        }
        
        // Cuando ya tenemos dimensiones, creamos el tablero
        if (w > 0 && h > 0 && tablero == nullptr) {
            tablero = new Tablero(w, h, limite);
        }

        // Cambio de sección
        if (linea[0] == '[') {
            seccion = linea;
            continue;
        }

        if (tablero == nullptr) continue;

        // Parseo por sección
        if (seccion == "[BLOCK]") {
            int id, bw, bh, ix, iy;
            char color;
            // Formato: 1 COLOR=a WIDTH=2 HEIGHT=2 INIT_X=4 INIT_Y=4 GEOMETRY=1 1 1 1
            if (sscanf(linea.c_str(), "%d COLOR=%c WIDTH=%d HEIGHT=%d INIT_X=%d INIT_Y=%d", 
                       &id, &color, &bw, &bh, &ix, &iy) == 6) {
                
                Bloque nuevo(id, color, ix, iy, bw, bh);
                
                // Leer geometría después del texto "GEOMETRY="
                size_t posGeo = linea.find("GEOMETRY=");
                if (posGeo != std::string::npos) {
                    std::stringstream ss(linea.substr(posGeo + 9));
                    int val;
                    for (int f = 0; f < bh; f++) {
                        for (int c = 0; c < bw; c++) {
                            ss >> val;
                            nuevo.setGeometria(f, c, val == 1);
                        }
                    }
                }
                tablero->agregarBloque(nuevo);
            }
        } 
        else if (seccion == "[WALL]") {
            // Lee el mapa de caracteres directamente
            static int filaMuro = 0;
            if (filaMuro < h) {
                for (int col = 0; col < (int)linea.length() && col < w; col++) {
                    if (linea[col] == '#') tablero->setCeldaMuro(filaMuro, col, true);
                }
                filaMuro++;
            }
        }
        else if (seccion == "[EXIT]") {
            int ex, ey, li, lf, p;
            char ec, ori;
            // COLOR=a X=4 Y=7 ORIENTATION=V LI=2 LF=2 STEP=0
            if (sscanf(linea.c_str(), "COLOR=%c X=%d Y=%d ORIENTATION=%c LI=%d LF=%d STEP=%d",
                       &ec, &ex, &ey, &ori, &li, &lf, &p) == 7) {
                int dx = (ori == 'H') ? 1 : 0;
                int dy = (ori == 'V') ? 1 : 0;
                Salida s(ec, ex, ey, dx, dy, li, lf, p);
                tablero->agregarSalida(s);
            }
        }
    }

    archivo.close();
    return tablero;
}