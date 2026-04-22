CC = g++
CFLAGS = -Wall -I.

# Objetos necesarios para los tests
# Nota: Si aún no tienes Tablero.cpp, podrías tener errores con State.o
OBJS = Bloque.o Heap.o State.o Tablero.o Parser.o Portal.o Salida.o

# Regla por defecto
all: test_heap

# Regla para el TEST del Heap
test_heap: tests/test_Heap.cpp $(OBJS)
	$(CC) $(CFLAGS) tests/test_Heap.cpp $(OBJS) -o tests/test_heap_exe
	./tests/test_heap_exe

# Regla para el TEST del Bloque
test_bloque: tests/test_Bloque.cpp Bloque.o
	$(CC) $(CFLAGS) tests/test_Bloque.cpp Bloque.o -o tests/test_bloque_exe
	./tests/test_bloque_exe

test_tablero: tests/test_Tablero.cpp Bloque.o Tablero.o Portal.o Salida.o
	$(CC) $(CFLAGS) tests/test_Tablero.cpp Bloque.o Tablero.o Portal.o Salida.o -o tests/test_tablero_exe.exe
	./tests/test_tablero_exe.exe

test_parser: tests/test_Parser.cpp $(OBJS)
	$(CC) $(CFLAGS) tests/test_Parser.cpp $(OBJS) -o tests/test_parser_exe.exe
	./tests/test_parser_exe.exe
	
# Regla genérica corregida: 
# Si el .h no existe para algún .cpp, esta regla fallará.
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o tests/*.o tests/test_heap_exe tests/test_bloque_exe