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

    // 1. PRIMER PASE: Buscar WIDTH y HEIGHT obligatoriamente
    while (fgets(linea, sizeof(linea), archivo)) {
        int len = strlen(linea);
        while (len > 0 && (linea[len - 1] == '\r' || linea[len - 1] == '\n')) {
            linea[--len] = '\0';
        }
        if (strstr(linea, "WIDTH")) sscanf(linea, "WIDTH = %d", &w);
        if (strstr(linea, "HEIGHT")) sscanf(linea, "HEIGHT = %d", &h);
        if (w > 0 && h > 0) break;
    }

    if (w <= 0 || h <= 0) {
        fprintf(stderr, "Error: Dimensiones invalidas o no encontradas.\n");
        fclose(archivo);
        return nullptr;
    }

    tablero = new Tablero();
    tablero->setDimensiones(w, h);
    
    fseek(archivo, 0, SEEK_SET);
    char seccion[32] = "";

    while (fgets(linea, sizeof(linea), archivo)) {
        int len = strlen(linea);
        while (len > 0 && (linea[len - 1] == '\r' || linea[len - 1] == '\n')) {
            linea[--len] = '\0';
        }
        if (len == 0) continue;

        if (linea[0] == '[') {
            strncpy(seccion, linea, sizeof(seccion) - 1);
            seccion[sizeof(seccion) - 1] = '\0';
            continue;
        }

        if (strcmp(seccion, "[BLOCK]") == 0) {
            if (linea[0] == '#') continue;

            int id, bw, bh, ix, iy;
            char color;
            if (sscanf(linea, "%d COLOR=%c WIDTH=%d HEIGHT=%d INIT_X=%d INIT_Y=%d",
                       &id, &color, &bw, &bh, &ix, &iy) >= 6) {
                bool* geo = new bool[bw * bh];
                char* posGeo = strstr(linea, "GEOMETRY=");
                if (posGeo) {
                    char* ptr = posGeo + 9;
                    int geoIdx = 0;
                    
                    while (geoIdx < bw * bh) {
                        int val;
                        
                        // Saltar espacios y caracteres de control
                        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\0') {
                            if (*ptr == '\0') {
                                // Necesitamos la siguiente línea
                                if (!fgets(linea, sizeof(linea), archivo)) break;
                                int len = strlen(linea);
                                while (len > 0 && (linea[len - 1] == '\r' || linea[len - 1] == '\n')) {
                                    linea[--len] = '\0';
                                }
                                ptr = linea;
                            } else {
                                ptr++;
                            }
                        }
                        
                        // Si llegamos al final sin encontrar más números, terminar
                        if (*ptr == '\0') break;
                        
                        // Leer el valor
                        if (sscanf(ptr, "%d", &val) == 1) {
                            geo[geoIdx++] = (val == 1);
                            // Avanzar ptr hasta el siguiente no-dígito
                            while (*ptr != '\0' && *ptr >= '0' && *ptr <= '9') ptr++;
                        } else {
                            ptr++;
                        }
                    }
                    
                    // Rellenar con false si faltaron valores
                    while (geoIdx < bw * bh) {
                        geo[geoIdx++] = false;
                    }
                } else {
                    for (int i = 0; i < bw * bh; i++) geo[i] = false;
                }
                tablero->agregarBloque(new Bloque(id, color, ix, iy, bw, bh, geo));
                printf("DEBUG: Bloque %d cargado.\n", id);
            }
        }
        else if (strcmp(seccion, "[WALL]") == 0) {
            // Importante: El fgets anterior leyó "[WALL]\n", así que el puntero está en la primera
            // línea de pared. Sin embargo, el SIGUIENTE fgets (en la próxima iteración del while)
            // va a leer ESA primera línea en 'linea'. Debemos procesarla aquí.
            
            // La variable 'linea' ya contiene la primera línea de pared (sin \n)
            for (int j = 0; j < w && j < (int)strlen(linea); j++) {
                tablero->setPared(0, j, linea[j]);
            }
            // Rellenar lo que falta de la fila 0
            for (int j = strlen(linea); j < w; j++) {
                tablero->setPared(0, j, ' ');
            }
            
            // Ahora leer las filas 1 a h-1 con fgetc
            for (int i = 1; i < h; i++) {
                for (int j = 0; j < w; j++) {
                    int ch = fgetc(archivo);
                    
                    if (ch == EOF) {
                        for (; j < w; j++) tablero->setPared(i, j, ' ');
                        break;
                    }
                    
                    if (ch == '\n' || ch == '\r') {
                        if (ch == '\r') {
                            int next = fgetc(archivo);
                            if (next != '\n' && next != EOF) ungetc(next, archivo);
                        }
                        for (; j < w; j++) tablero->setPared(i, j, ' ');
                        break;
                    }
                    
                    tablero->setPared(i, j, (char)ch);
                }
                
                // Consumir caracteres sobrantes hasta el próximo \n
                if (i < h - 1) {
                    int extra;
                    while ((extra = fgetc(archivo)) != EOF && extra != '\n' && extra != '\r');
                    if (extra == '\r') {
                        int next = fgetc(archivo);
                        if (next != '\n' && next != EOF) ungetc(next, archivo);
                    }
                }
            }
            seccion[0] = '\0';
        }
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
    }

    fclose(archivo);
    return tablero;
}