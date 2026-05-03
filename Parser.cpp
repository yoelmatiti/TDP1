#include "Parser.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>

Tablero* Parser::cargarEscenario(const char* nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << nombreArchivo << std::endl;
        return nullptr;
    }

    Tablero* t = nullptr;
    char linea[512];
    int w = 0, h = 0;

    while (archivo.getline(linea, sizeof(linea))) {
        // Ignorar líneas vacías o comentarios
        if (linea[0] == '\0' || linea[0] == '\r' || linea[0] == '\n' || linea[0] == '#') continue;

        // 1. Capturar dimensiones (WIDTH / HEIGHT)
        if (sscanf(linea, "WIDTH = %d", &w) == 1) continue;
        if (sscanf(linea, "HEIGHT = %d", &h) == 1) {
            // VALIDACIÓN CRÍTICA: Solo instanciar si no existe y tenemos datos válidos
            if (w > 0 && h > 0 && t == nullptr) { 
                t = new Tablero();
                t->setDimensiones(w, h);
                // No necesitamos 'continue' aquí para dejar que el bucle siga buscando etiquetas
            }
            continue;
        }

        // 2. Procesar secciones usando etiquetas de búsqueda
        // IMPORTANTE: t debe estar validado para evitar Segmentation Fault
        if (t != nullptr) {
            if (compararEtiqueta(linea, "[WALL]")) {
                leerParedes(archivo, t);
            } 
            else if (compararEtiqueta(linea, "[BLOCK]")) {
                leerBloques(archivo, t);
            }
            // Agrega aquí [PORTALS] o [EXIT] si fuera necesario
        } else {
            // Si encontramos una etiqueta de sección pero t es nulo, el archivo está mal formado
            if (linea[0] == '[') {
                std::cerr << "Error: Sección " << linea << " encontrada antes de definir WIDTH/HEIGHT." << std::endl;
            }
        }
    }

    archivo.close();

    // Verificación final de consistencia
    if (t == nullptr) {
        std::cerr << "Error: El archivo no contenía dimensiones válidas." << std::endl;
    }

    return t;
}

void Parser::leerParedes(std::ifstream& archivo, Tablero* t) {
    char linea[512];
    int h = t->getHeight();
    int w = t->getWidth();

    for (int i = 0; i < h; i++) {
        if (archivo.getline(linea, sizeof(linea))) {
            for (int j = 0; j < w; j++) {
                // Solo registramos caracteres sólidos (#) o especiales para no 'limpiar'
                // la matriz si el archivo de paredes tiene espacios extra.
                if (linea[j] != ' ' && linea[j] != '\0' && linea[j] != '\r') {
                    t->setPared(i, j, linea[j]);
                }
            }
        }
    }
    std::cout << "DEBUG: Paredes cargadas correctamente." << std::endl;
}

void Parser::leerBloques(std::ifstream& archivo, Tablero* t) {
    char linea[1024];
    while (archivo.peek() != '[' && archivo.getline(linea, sizeof(linea))) {
        if (linea[0] == '\0' || linea[0] == '\r' || linea[0] == '\n') continue;

        int id = -1, x = 0, y = 0, bw = 0, bh = 0;
        char color = ' ';

        // Capturamos el ID directamente del inicio de la línea
        if (sscanf(linea, "%d COLOR=%c WIDTH=%d HEIGHT=%d INIT_X=%d INIT_Y=%d", 
                   &id, &color, &bw, &bh, &x, &y) >= 6) {
            
            const char* geoPtr = strstr(linea, "GEOMETRY=");
            if (geoPtr) {
                geoPtr += 9; 
                
                int tam = bw * bh;
                bool* geo = new bool[tam];
                
                for (int i = 0; i < tam; i++) {
                    int valorBit;
                    if (sscanf(geoPtr, "%d", &valorBit) == 1) {
                        geo[i] = (valorBit == 1);
                        
                        // Avanzar el puntero geoPtr al siguiente número de forma segura
                        while (*geoPtr != '\0' && !(*geoPtr >= '0' && *geoPtr <= '9')) {
                            geoPtr++;
                        }
                        while (*geoPtr != '\0' && (*geoPtr >= '0' && *geoPtr <= '9')) {
                            geoPtr++;
                        }
                    }
                }
                
                // CRÍTICO: id se pasa tal cual se leyó, sin alteraciones.
                t->agregarBloque(new Bloque(id, color, x, y, bw, bh, geo));
                
                std::cout << "DEBUG: Bloque " << id << " cargado correctamente." << std::endl;
            }
        }
    }
}
bool Parser::compararEtiqueta(const char* linea, const char* etiqueta) {
    int i = 0;
    // Saltamos espacios en blanco al inicio de la línea si los hubiera
    while (linea[i] == ' ' || linea[i] == '\t') i++;

    int j = 0;
    while (linea[i] != '\0' && etiqueta[j] != '\0' && linea[i] != '\r' && linea[i] != '\n') {
        if (linea[i] != etiqueta[j]) return false;
        i++;
        j++;
    }
    
    // Es válida si llegamos al final de la etiqueta buscada
    // Y lo que queda en la línea es un fin de cadena, un espacio o un salto de línea
    return (etiqueta[j] == '\0' && (linea[i] == '\0' || linea[i] == '\r' || linea[i] == '\n' || linea[i] == ' '));
}