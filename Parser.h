#ifndef PARSER_H
#define PARSER_H

#include "Tablero.h"
#include <fstream> // IMPORTANTE: Debe estar aquí para reconocer std::ifstream

class Parser {
public:
    static Tablero* cargarEscenario(const char* rutaArchivo);

private:
    // Asegúrate de usar std::ifstream& (referencia)
    static void leerBloques(std::ifstream& archivo, Tablero* t);
    static void leerParedes(std::ifstream& archivo, Tablero* t);
    static void leerSalidas(std::ifstream& archivo, Tablero* t);
};

#endif