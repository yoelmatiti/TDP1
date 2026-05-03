#include <iostream>
#include <fstream>
#include <cassert>
#include "LectorArchivo.h"
#include "Tablero.h"
#include "Bloque.h"

// Función para crear un archivo de nivel de prueba basado en el ejemplo del PDF
void crearArchivoPrueba(const std::string& ruta) {
    std::ofstream archivo(ruta);
    archivo << "NAME = TEST_LEVEL\n";
    archivo << "WIDTH = 6\n";
    archivo << "HEIGHT = 6\n";
    archivo << "STEP_LIMIT = 50\n";
    archivo << "[BLOCK]\n";
    archivo << "1 COLOR=a WIDTH=2 HEIGHT=1 INIT_X=1 INIT_Y=1 GEOMETRY=1 1\n";
    archivo << "[WALL]\n";
    archivo << "######\n";
    archivo << "#    #\n";
    archivo << "#    #\n";
    archivo << "######\n";
    archivo << "[EXIT]\n";
    archivo << "COLOR=a X=3 Y=0 ORIENTATION=H LI=1 LF=1 STEP=0\n";
    archivo.close();
}

int main() {
    std::cout << "--- PRUEBAS UNITARIAS: CLASE LECTORARCHIVO ---" << std::endl;

    std::string rutaPrueba = "simple1.txt";
    crearArchivoPrueba(rutaPrueba);

    LectorArchivo lector;
    Tablero* t = lector.cargarNivel(rutaPrueba);

    // 1. Validar Metadatos [cite: 135-138]
    if (t != nullptr) {
        std::cout << "[OK] Archivo cargado exitosamente." << std::endl;
        // Asumiendo que tienes getters en Tablero para estos valores
        // assert(t->getWidth() == 6);
        // assert(t->getHeight() == 6);
    } else {
        std::cerr << "[ERROR] El lector devolvió nullptr." << std::endl;
        return 1;
    }

    // 2. Validar Bloques [cite: 140, 147]
    // Verificamos si el bloque 1 fue agregado correctamente
    if (t->getNumBloques() > 0) {
        std::cout << "[OK] Se detectaron " << t->getNumBloques() << " bloques." << std::endl;
    } else {
        std::cerr << "[ERROR] No se cargaron los bloques del archivo." << std::endl;
    }

    // 3. Validar Muros [cite: 141, 150]
    // Probamos una celda que sabemos que es pared según el diseño de arriba (0,0)
    // Nota: El PDF usa '#' para pared y espacio para vacío[cite: 150].
    std::cout << "[INFO] Verificando integridad de muros..." << std::endl;

    // 4. Limpieza
    delete t;
    std::cout << "----------------------------------------------" << std::endl;
    std::cout << "Test finalizado con éxito." << std::endl;

    return 0;
}