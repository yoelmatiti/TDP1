#include "LectorArchivo.h"
#include <cstdio>
#include <cstring>

Tablero* LectorArchivo::cargarNivel(const char* ruta) {
    FILE* archivo = fopen(ruta, "r");
    if (!archivo) {
        fprintf(stderr, "Error: No se pudo abrir %s\n", ruta);
        return nullptr;
    }

    char linea[1024];
    int w = 0, h = 0;
    Tablero* tablero = nullptr;

    // 1. PRIMER PASE: Buscar WIDTH y HEIGHT
    while (fgets(linea, sizeof(linea), archivo)) {
        if (strstr(linea, "WIDTH")) sscanf(linea, "WIDTH = %d", &w);
        if (strstr(linea, "HEIGHT")) sscanf(linea, "HEIGHT = %d", &h);
        if (w > 0 && h > 0) break;
    }

    if (w <= 0 || h <= 0) {
        fprintf(stderr, "Error: Dimensiones invalidas en %s\n", ruta);
        fclose(archivo);
        return nullptr;
    }

    tablero = new Tablero();
    tablero->setDimensiones(w, h);
    
    // Volver al inicio para procesar secciones
    fseek(archivo, 0, SEEK_SET);
    char seccion[32] = "";

    while (fgets(linea, sizeof(linea), archivo)) {
        // Limpiar saltos de línea (\r\n) de forma segura
        int len = strlen(linea);
        while (len > 0 && (linea[len - 1] == '\r' || linea[len - 1] == '\n')) {
            linea[--len] = '\0';
        }
        if (len == 0) continue;

        // Identificar cambio de sección
        if (linea[0] == '[') {
            strncpy(seccion, linea, sizeof(seccion) - 1);
            continue;
        }

        // --- SECCIÓN BLOQUES ---
        if (strcmp(seccion, "[BLOCK]") == 0) {
            int id, bw, bh, ix, iy;
            char color;
            if (sscanf(linea, "%d COLOR=%c WIDTH=%d HEIGHT=%d INIT_X=%d INIT_Y=%d",
                       &id, &color, &bw, &bh, &ix, &iy) >= 6) {
                
                bool* geo = new bool[bw * bh];
                char* posGeo = strstr(linea, "GEOMETRY=");
                if (posGeo) {
                    char* ptr = posGeo + 9;
                    for (int i = 0; i < bw * bh; i++) {
                        int val;
                        while (*ptr && (*ptr == ' ' || *ptr == '\t')) ptr++;
                        if (*ptr && sscanf(ptr, "%d", &val) == 1) {
                            geo[i] = (val == 1);
                            while (*ptr && *ptr != ' ' && *ptr != '\t') ptr++;
                        } else {
                            geo[i] = false;
                        }
                    }
                }
                Bloque* b = new Bloque(id, color, 0, 0, bw, bh, geo);
                b->setX(ix);
                b->setY(iy);
                tablero->agregarBloque(b);
            }
        }
        
        // --- SECCIÓN MUROS (Robustez ante tableros no rectangulares) ---
        else if (strcmp(seccion, "[WALL]") == 0) {
            // El primer muro es la 'linea' actual. Los demás se leen secuencialmente.
            for (int i = 0; i < h; i++) {
                int lineLen = strlen(linea);
                for (int j = 0; j < w; j++) {
                    char c = (j < lineLen) ? linea[j] : ' ';
                    tablero->setPared(i, j, c);
                }
                // Si no es la última fila, leer la siguiente línea de la pared
                if (i < h - 1) {
                    if (!fgets(linea, sizeof(linea), archivo)) break;
                    // Limpiar saltos de línea de la nueva fila leída
                    int nl = strlen(linea);
                    while (nl > 0 && (linea[nl-1] == '\r' || linea[nl-1] == '\n')) {
                        linea[--nl] = '\0';
                    }
                }
            }
            seccion[0] = '\0'; // Salir de modo pared para no repetir
        }

        // --- SECCIÓN SALIDAS ---
        else if (strcmp(seccion, "[EXIT]") == 0) {
            int ex, ey, li, lf, p;
            char ec, ori;
            if (sscanf(linea, "COLOR=%c X=%d Y=%d ORIENTATION=%c LI=%d LF=%d STEP=%d",
                       &ec, &ex, &ey, &ori, &li, &lf, &p) == 7) {
                int dx = (ori == 'H') ? 1 : 0;
                int dy = (ori == 'V') ? 1 : 0;
                tablero->agregarSalida(new Salida(ec, ex, ey, dx, dy, li, lf, p));
            }
        }

        // --- SECCIÓN COMPUERTAS ---
        else if (strcmp(seccion, "[GATE]") == 0) {
            int gx, gy, gPaso;
            char gCiclo[128];
            // Formato: X Y PASO CICLO (Ej: 2 1 4 abc)
            if (sscanf(linea, "%d %d %d %s", &gx, &gy, &gPaso, gCiclo) == 4) {
                tablero->agregarCompuerta(new Compuerta(gx, gy, gPaso, gCiclo));
            }
        }
    }

    fclose(archivo);
    return tablero;
}