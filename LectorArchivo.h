#ifndef LECTORARCHIVO_H
#define LECTORARCHIVO_H

#include <string>
#include <fstream>
#include "Tablero.h"

class LectorArchivo {
public:
    // Lee el archivo y devuelve un Tablero configurado.
    // El llamador es responsable de liberar la memoria del Tablero.
    static Tablero* cargarNivel(const std::string& ruta);
};

#endif