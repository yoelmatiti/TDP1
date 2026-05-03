#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include "Tablero.h"

class Parser {
public:
    // Firma que espera tu test_Tablero.cpp
    static Tablero* cargarEscenario(const char* rutaArchivo);

private:
    static void leerParedes(std::ifstream& archivo, Tablero* t);
    static void leerBloques(std::ifstream& archivo, Tablero* t);
    static bool compararEtiqueta(const char* linea, const char* etiqueta);
    static void leerSalidas(std::ifstream& archivo, Tablero* t);
};

#endif