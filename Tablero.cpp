#include "Tablero.h"
#include <cstdio> // Para sprintf

// Constructor por defecto inicializando punteros a null
Tablero::Tablero() 
    : width(0), height(0), pasoActual(0), matriz(nullptr), representacion(nullptr),
      bloques(nullptr), numBloques(0), bloquesRestantes(0), 
      salidas(nullptr), numSalidas(0), 
      portales(nullptr), numPortales(0) 
{
}

// Destructor: Limpia la memoria dinámica [cite: 122]
Tablero::~Tablero() {
    liberarMemoria();
}

void Tablero::liberarMemoria() {
    //Solo borramos matrizContigua y el arreglo de punteros matriz
    delete[] matrizContigua; 
    delete[] matriz; 

    // Borrar los bloques que fueron creados con 'new' en el Parser
    for(int i = 0; i < numBloques; i++) {
        delete bloques[i]; 
    }
    delete[] bloques;
    delete[] salidas;
    delete[] portales;
    delete[] representacion;
    representacion = nullptr;
}

// Constructor de Copia: Clave para el Algoritmo A* [cite: 18]
Tablero::Tablero(const Tablero& otra) {
    copiarDesde(otra);
}

// Operador de Asignación
Tablero& Tablero::operator=(const Tablero& otra) {
    if (this != &otra) {
        liberarMemoria();
        copiarDesde(otra);
    }
    return *this;
}

void Tablero::copiarDesde(const Tablero& otra) {
    this->width = otra.width;
    this->height = otra.height;
    this->pasoActual = otra.pasoActual;

    this->bloquesRestantes = otra.bloquesRestantes;
    this->numBloques = otra.numBloques;
    this->numSalidas = otra.numSalidas;
    this->numPortales = otra.numPortales;
    this->representacion = nullptr;

    // Copiar la matriz de caracteres (Deep Copy)
    if (otra.matriz != nullptr) {
        this->matriz = new char*[height];
        for (int i = 0; i < height; i++) {
            this->matriz[i] = new char[width];
            for (int j = 0; j < width; j++) {
                this->matriz[i][j] = otra.matriz[i][j];
            }
        }
    } else {
        this->matriz = nullptr;
    }

    // Copiar arreglos de objetos (Asumiendo que Bloque, Salida y Portal 
    // tienen sus propios operadores de asignación)
    if (otra.bloques != nullptr) {
        this->bloques = new Bloque[numBloques];
        for (int i = 0; i < numBloques; i++) this->bloques[i] = otra.bloques[i];
    }
    if (otra.salidas != nullptr) {
        this->salidas = new Salida[numSalidas];
        for (int i = 0; i < numSalidas; i++) this->salidas[i] = otra.salidas[i];
    }
    if (otra.portales != nullptr) {
        this->portales = new Portal[numPortales];
        for (int i = 0; i < numPortales; i++) this->portales[i] = otra.portales[i];
    }
}


void Tablero::setDimensiones(int w, int h) {
    this->width = w;
    this->height = h;

    // 1. Matriz Contigua (Optimización de Caché)
    matrizContigua = new char[width * height];
    matriz = new char*[height];
    
    for(int i = 0; i < height; i++) {
        matriz[i] = &matrizContigua[i * width];
        // Inicializar cada celda como vacía
        for(int j = 0; j < width; j++) {
            matriz[i][j] = ' '; 
        }
    }

    // 2. Inicializar arreglos de objetos (Punteros dobles)
    // Usamos una capacidad fija o podrías calcularla según el archivo
    this->capacidadBloques = 100; 
    this->bloques = new Bloque*[capacidadBloques];
    this->numBloques = 0;

    this->numSalidas = 0;
    this->salidas = new Salida*[20]; // Capacidad estimada

    this->numPortales = 0;
    this->portales = new Portal*[20];
}

void Tablero::agregarBloque(Bloque* nuevoBloque) {
    // 1. Verificación de capacidad
    if (numBloques < 100) {
        bloques[numBloques] = nuevoBloque;
        numBloques++;
        bloquesRestantes++;

        // 2. REGISTRO EN MATRIZ: Marcamos la posición inicial
        // Esto permite que esMovimientoValido NO tenga que iterar sobre otros bloques.
        int xIni = nuevoBloque->getX();
        int yIni = nuevoBloque->getY();
        char color = nuevoBloque->getColor();

        for (int rY = 0; rY < nuevoBloque->getAltoGeo(); rY++) {
            for (int rX = 0; rX < nuevoBloque->getAnchoGeo(); rX++) {
                // Solo marcamos si la geometría del bloque ocupa esa celda
                if (nuevoBloque->getGeometria(rY, rX)) {
                    int absY = yIni + rY;
                    int absX = xIni + rX;
                    
                    // Verificación de seguridad (dentro de los límites)
                    if (absY >= 0 && absY < height && absX >= 0 && absX < width) {
                        matriz[absY][absX] = color;
                    }
                }
            }
        }
    }
}


bool Tablero::esEstadoFinal() const {
    // El juego termina si todos los bloques salieron [cite: 116]
    // Esto depende de tu lógica interna, por ejemplo si bloquesRestantes llega a 0
    return bloquesRestantes == 0;
}

const char* Tablero::getRepresentacion() const {
    if (representacion == nullptr) {
        generarRepresentacion();
    }
    return representacion;
}

void Tablero::generarRepresentacion() const {
    // Estimar tamaño: matriz (width*height) + portales (numPortales * 10) + salidas (numSalidas * 10) + separadores
    int tam = width * height + numPortales * 10 + numSalidas * 10 + 100;
    representacion = new char[tam];
    int pos = 0;

    // Incluir la matriz
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            representacion[pos++] = matriz[i][j];
        }
    }
    representacion[pos++] = '|'; // Separador

    // Incluir largos de salidas
    for (int i = 0; i < numSalidas; i++) {
        int largo = salidas[i].getLongitudActual(0); // Usar longitud inicial
        pos += sprintf(representacion + pos, "%d,", largo);
    }
    representacion[pos++] = '|'; // Separador

    // Incluir colores de portales
    for (int i = 0; i < numPortales; i++) {
        char color = portales[i].getColor();
        pos += sprintf(representacion + pos, "%c,", color);
    }
    representacion[pos] = '\0'; // Terminar cadena
}

void Tablero::agregarPortal(Portal* portal) {
    // Implementación básica: redimensionar arreglo
    Portal* nuevo = new Portal[numPortales + 1];
    for (int i = 0; i < numPortales; i++) nuevo[i] = portales[i];
    nuevo[numPortales] = *portal;
    delete[] portales;
    portales = nuevo;
    numPortales++;
}

int Tablero::moverBloque(int id, Direccion dir, int celdas) {
    if (id < 0 || id >= numBloques) return 0;
    
    // Si el bloque no está activo, no se mueve
    if (!bloques[id].estaActivo()) return 0;

    int distanciaMax = 0;
    for (int dist = 1; dist <= celdas; dist++) {
        if (esMovimientoValido(id, dir, dist)) {
            distanciaMax = dist;
        } else {
            break;
        }
    }

    if (distanciaMax > 0) {
        aplicarMovimiento(id, dir, distanciaMax);
    }

    return distanciaMax;
}

bool Tablero::esMovimientoValido(int bloqueID, Direccion dir, int celdas) const {
    // 1. Validación rápida de entrada
    if (bloqueID < 0 || bloqueID >= numBloques) return false;
    
    // Acceso por puntero para evitar copias (bloques es Bloque**)
    const Bloque* b = bloques[bloqueID];
    if (!b->estaActivo()) return false;

    // 2. Pre-calcular el desplazamiento (Branchless o Switch simple)
    int dx = 0, dy = 0;
    if (dir == Direccion::U) dy = -celdas;
    else if (dir == Direccion::D) dy = celdas;
    else if (dir == Direccion::L) dx = -celdas;
    else if (dir == Direccion::R) dx = celdas;

    int nuevaX = b->getX() + dx;
    int nuevaY = b->getY() + dy;

    // 3. Verificar límites del tablero (el bloque completo)
    if (nuevaX < 0 || nuevaY < 0 || 
        (nuevaX + b->getAnchoGeo()) > width || 
        (nuevaY + b->getAltoGeo()) > height) return false;

    // 4. Verificar geometría contra la matriz de ocupación
    for (int rY = 0; rY < b->getAltoGeo(); ++rY) {
        for (int rX = 0; rX < b->getAnchoGeo(); ++rX) {
            if (!b->getGeometria(rY, rX)) continue;

            int absY = nuevaY + rY;
            int absX = nuevaX + rX;
            char celda = matriz[absY][absX];

            // Si la celda es una pared, fuera.
            if (celda == '#') return false;

            // Si la celda contiene OTRO bloque. 
            // (Asumimos que en la matriz guardas el ID del bloque o un char distintivo)
            if (celda != ' ' && celda != b->getColor()) {
                // Aquí podrías tener una lógica donde los bloques 
                // se marcan con su color en la matriz.
                // Si la celda no es espacio y no es MI color, es un obstáculo.
                
                // NOTA: Para portales, revisamos si el char en la matriz es una 'A'...'Z'
                if (celda >= 'A' && celda <= 'Z') {
                    // Solo entramos al bucle de portales si detectamos uno en la matriz
                    for (int p = 0; p < numPortales; ++p) {
                        if (portales[p]->getX() == absX && portales[p]->getY() == absY) {
                            if (!portales[p]->puedePasar(b->getColor(), pasoActual)) return false;
                            break; 
                        }
                    }
                } else {
                    // Si no es portal y no es espacio, es otro bloque o pared
                    return false;
                }
            }
        }
    }
    return true;
}
void Tablero::verificarRevelacionIncognitos() {
    int coloresActivos = 0;
    for (int i = 0; i < numBloques; i++) {
        // Contamos cuántos bloques con color definido siguen en el tablero
        if (bloques[i].estaActivo() && !bloques[i].getEsIncognito()) {
            coloresActivos++;
        }
    }

    // Si ya no quedan bloques de colores normales
    if (coloresActivos == 0) {
        for (int i = 0; i < numBloques; i++) {
            if (bloques[i].estaActivo() && bloques[i].getEsIncognito()) {
                // CORRECCIÓN: Llamada sin argumentos
                bloques[i].revelarColor(); 
            }
        }
    }
}

void Tablero::aplicarMovimiento(int bloqueID, Direccion dir, int distancia) {
    // 1. Obtener puntero al bloque (asumiendo Bloque** bloques)
    Bloque* b = bloques[bloqueID];
    char colorB = b->getColor();

    // 2. LIMPIAR posición antigua en la matriz
    // Solo limpiamos las celdas donde la geometría era verdadera
    for (int rY = 0; rY < b->getAltoGeo(); rY++) {
        for (int rX = 0; rX < b->getAnchoGeo(); rX++) {
            if (b->getGeometria(rY, rX)) {
                matriz[b->getY() + rY][b->getX() + rX] = ' '; 
            }
        }
    }

    // 3. CALCULAR desplazamiento
    int dx = 0, dy = 0;
    switch (dir) {
        case Direccion::U: dy = -1; break;
        case Direccion::D: dy = 1; break;
        case Direccion::L: dx = -1; break;
        case Direccion::R: dx = 1; break;
    }

    // 4. ACTUALIZAR estado del bloque y tablero
    b->mover(dx * distancia, dy * distancia);
    this->pasoActual += distancia;

    // 5. MARCAR posición nueva en la matriz (si el bloque sigue activo)
    // Esto es lo que permite que esMovimientoValido sea O(G)
    bool llegoASalida = false;
    
    // Verificamos salida ANTES de marcar para no dejar basura si el bloque se va
    for (int s = 0; s < numSalidas; s++) {
        if (salidas[s]->getColor() == colorB) {
            if (b->ocupaCelda(salidas[s]->getY(), salidas[s]->getX())) {
                llegoASalida = true;
                break;
            }
        }
    }

    if (llegoASalida) {
        b->setActivo(false);
        b->mover(-100, -100); // Coordenadas "limbo"
        bloquesRestantes--;
        verificarRevelacionIncognitos();
    } else {
        // Si no salió, marcamos su nueva posición en la matriz
        for (int rY = 0; rY < b->getAltoGeo(); rY++) {
            for (int rX = 0; rX < b->getAnchoGeo(); rX++) {
                if (b->getGeometria(rY, rX)) {
                    matriz[b->getY() + rY][b->getX() + rX] = colorB;
                }
            }
        }
    }

    // 6. GESTIÓN DE MEMORIA: Invalidar la representación del estado
    if (representacion != nullptr) {
        delete[] representacion;
        representacion = nullptr;
    }
}
void Tablero::actualizarCompuertas() {
    // Actualizar portales si es necesario (cambio de color con tiempo)
    // Por ahora vacío
}

void Tablero::actualizarSalidas() {
    // Actualizar salidas si cambian con tiempo
    // Por ahora vacío
}

void Tablero::setDimensiones(int w, int h) {
    this->width = w;
    this->height = h;
    
    // Inicializar matriz de char**
    matriz = new char*[height];
    for(int i = 0; i < height; i++) {
        matriz[i] = new char[width];
        for(int j = 0; j < width; j++) {
            matriz[i][j] = ' '; // Inicializar vacío
        }
    }
    
    // Inicializar el arreglo de punteros a bloques (capacidad inicial 50)
    this->bloques = new Bloque*[50]; 
    this->numBloques = 0;
}

void Tablero::setPared(int fila, int col, char valor) {
    if (fila >= 0 && fila < height && col >= 0 && col < width) {
        matriz[fila][col] = valor;
    }
}

