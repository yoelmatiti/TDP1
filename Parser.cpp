#include "Parser.h"
#include <iostream>
#include <cstring>

Tablero* Parser::cargarEscenario(const char* rutaArchivo) {
    std::ifstream archivo(rutaArchivo);
    if (!archivo.is_open()) return nullptr;

    Tablero* nuevoTablero = new Tablero();
    char etiqueta[50];
    int w_temp = 0, h_temp = 0;

    while (archivo >> etiqueta) {
        if (strcmp(etiqueta, "WIDTH") == 0) {
            archivo.ignore(3); archivo >> w_temp;
        }
        else if (strcmp(etiqueta, "HEIGHT") == 0) {
            archivo.ignore(3); archivo >> h_temp;
            // Cuando ya tenemos ambos, llamamos a tu método:
            if (w_temp > 0 && h_temp > 0) {
                nuevoTablero->setDimensiones(w_temp, h_temp);
            }
        }
        else if (strcmp(etiqueta, "[BLOCK]") == 0) {
            leerBloques(archivo, nuevoTablero);
        }
        else if (strcmp(etiqueta, "[WALL]") == 0) {
            leerParedes(archivo, nuevoTablero);
        }
    }
    archivo.close();
    return nuevoTablero;
}

void Parser::leerParedes(std::ifstream& archivo, Tablero* t) {
    archivo.ignore(); // Saltar el salto de línea después de [WALL]
    for (int i = 0; i < t->getHeight(); i++) {
        char linea[512];
        archivo.getline(linea, 512);
        for (int j = 0; j < t->getWidth(); j++) {
            // matriz[i][j] es char**, asegúrate de tener un setter en Tablero
            t->setPared(i, j, linea[j]); 
        }
    }
}
void Parser::leerBloques(std::ifstream& archivo, Tablero* t) {
    char linea[512];
    while (archivo.getline(linea, 512) && linea[0] != '[') {
        if (linea[0] == '\0' || isspace(linea[0])) continue;

        int id, w, h, x, y;
        char color;
        // sscanf es mucho más rápido que archivo >> para parsing de formatos fijos
        if (sscanf(linea, "%d COLOR=%c WIDTH=%d HEIGHT=%d INIT_X=%d INIT_Y=%d", 
                   &id, &color, &w, &h, &x, &y) == 6) {
            
            // Localizamos GEOMETRY en la cadena para leer los bits
            const char* geoPtr = strstr(linea, "GEOMETRY=");
            if (geoPtr) {
                geoPtr += 9; // Saltamos "GEOMETRY="
                bool* geo = new bool[w * h];
                for(int i = 0; i < w * h; i++) {
                    geo[i] = (*geoPtr == '1');
                    geoPtr += 2; // Saltamos el número y el espacio
                }
                t->agregarBloque(new Bloque(id, color, x, y, w, h, geo));
            }
        }
    }
}