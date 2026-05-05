#include "LectorArchivo.h"
#include <cstdio>  // Reemplaza ifstream
#include <cstring> // Reemplaza string methods

Tablero* LectorArchivo::cargarNivel(const char* ruta) {
    FILE* archivo = fopen(ruta, "r");
    if (!archivo) return nullptr;

    char linea[512]; // Buffer para leer cada línea
    char seccion[32] = "";
    int w = 0, h = 0;
    int filaMuro = 0;
    Tablero* tablero = nullptr;

    // Leemos línea por línea usando fgets
    while (fgets(linea, sizeof(linea), archivo)) {
        // Eliminar el salto de línea al final si existe
        linea[strcspn(linea, "\r\n")] = 0;

        // Saltar líneas vacías o comentarios
        if (linea[0] == '\0' || linea[0] == '#') continue;

        // 1. Detección de dimensiones
        if (strstr(linea, "WIDTH")) {
            sscanf(linea, "WIDTH = %d", &w);
        } else if (strstr(linea, "HEIGHT")) {
            sscanf(linea, "HEIGHT = %d", &h);
        }

        // Inicializar tablero una vez tengamos dimensiones
        if (w > 0 && h > 0 && tablero == nullptr) {
            tablero = new Tablero();
            tablero->setDimensiones(w, h);
        }

        // 2. Cambio de sección
        if (linea[0] == '[') {
            strcpy(seccion, linea);
            continue;
        }

        if (tablero == nullptr) continue;

        // 3. Procesamiento de secciones
        if (strcmp(seccion, "[BLOCK]") == 0) {
            int id, bw, bh, ix, iy;
            char color;
            if (sscanf(linea, "%d COLOR=%c WIDTH=%d HEIGHT=%d INIT_X=%d INIT_Y=%d", 
                       &id, &color, &bw, &bh, &ix, &iy) == 6) {
                
                bool* geo = new bool[bw * bh];
                char* posGeo = strstr(linea, "GEOMETRY=");
                
                if (posGeo) {
                    // Saltamos "GEOMETRY=" (9 caracteres)
                    char* ptr = posGeo + 9;
                    for (int i = 0; i < (bw * bh); i++) {
                        int val;
                        // sscanf devuelve cuántos elementos leyó; usamos %d y avanzamos el puntero
                        if (sscanf(ptr, "%d", &val) == 1) {
                            geo[i] = (val == 1);
                            // Avanzar el puntero hasta el siguiente espacio o fin
                            while (*ptr != ' ' && *ptr != '\0') ptr++;
                            while (*ptr == ' ') ptr++; 
                        }
                    }
                }
                
                Bloque* b = new Bloque(id, color, ix, iy, bw, bh, geo);
                tablero->agregarBloque(b);
            }
        } 
        else if (strcmp(seccion, "[WALL]") == 0) {
            if (filaMuro < h) {
                int len = strlen(linea);
                for (int col = 0; col < len && col < w; col++) {
                    tablero->setPared(filaMuro, col, linea[col]);
                }
                filaMuro++;
            }
        }
        else if (strcmp(seccion, "[EXIT]") == 0) {
            int ex, ey, li, lf, p;
            char ec, ori;
            if (sscanf(linea, "COLOR=%c X=%d Y=%d ORIENTATION=%c LI=%d LF=%d STEP=%d",
                       &ec, &ex, &ey, &ori, &li, &lf, &p) == 7) {
                
                int dx = (ori == 'H') ? 1 : 0;
                int dy = (ori == 'V') ? 1 : 0;
                
                Salida* s = new Salida(ec, ex, ey, dx, dy, li, lf, p);
                tablero->agregarSalida(s);
            }
        }
    }

    fclose(archivo);
    return tablero;
}