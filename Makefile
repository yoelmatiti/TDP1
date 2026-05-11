# Variables de compilación
CC = g++
CFLAGS = -Wall -I. -g

# Objetos necesarios [cite: 230, 236]
OBJS = Bloque.o Heap.o State.o Tablero.o  Compuerta.o Salida.o TablaHash.o Solver.o Movimiento.o LectorArchivo.o

# Ejecutables de test
TEST_EXES = tests/test_state_exe tests/test_heap_exe tests/test_bloque_exe \
            tests/test_tablero_exe tests/test_hash_exe LectorArchivo.o\
            tests/test_compuerta_exe tests/test_mov_exe tests/test_solver_exe

# Regla principal 
all: $(OBJS) $(TEST_EXES)

# --- REGLAS DE ENLACE (LINKING) ---
# ============================================================
# ALIASES (ACCESOS DIRECTOS)
# ============================================================
.PHONY: test_state test_heap test_bloque test_tablero \
     test_hash test_compuerta test_mov test_lector\
        test_salida test_solver run_game all_tests

# ============================================================
# REGLAS DE EJECUCIÓN (LOS ALIAS)
# ============================================================

test_state: tests/test_state_exe
	@echo Ejecutando test de State...
	./tests/test_state_exe

test_lector: tests/test_lector_exe
	@echo Ejecutando test de LectorArchivo...
	./tests/test_lector_exe

test_heap: tests/test_heap_exe
	@echo Ejecutando test de Heap...
	./tests/test_heap_exe

test_bloque: tests/test_bloque_exe
	@echo Ejecutando test de Bloque...
	./tests/test_bloque_exe

test_tablero: tests/test_tablero_exe
	@echo Ejecutando test de Tablero...
	./tests/test_tablero_exe

test_hash: tests/test_hash_exe
	@echo Ejecutando test de Tabla Hash...
	./tests/test_hash_exe

test_compuerta: tests/test_compuerta_exe
	@echo Ejecutando test de Compuerta...
	./tests/test_compuerta_exe

test_mov: tests/test_mov_exe
	@echo Ejecutando test de Movimiento...
	./tests/test_mov_exe

test_solver: tests/test_solver_exe
	@echo Ejecutando test de Solver...
	./tests/test_solver_exe

test_salida: tests/test_salida_exe
	@echo Ejecutando test de Salida...
	./tests/test_salida_exe

run_game: cbj_game
	@echo Iniciando el juego...
	./cbj_game.exe

# Bonus: Ejecutar absolutamente todos los tests con un solo comando
all_tests: test_state test_heap test_bloque test_tablero test_hash test_compuerta test_mov test_solver test_lector

# ============================================================
# REGLAS DE COMPILACIÓN (LOS EJECUTABLES REALES)
# ============================================================

cbj_game: main.cpp $(OBJS)
	$(CC) $(CFLAGS) $^ -o cbj_game.exe

tests/test_state_exe: tests/test_State.cpp $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

tests/test_heap_exe: tests/test_Heap.cpp $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

tests/test_bloque_exe: tests/test_Bloque.cpp Bloque.o
	$(CC) $(CFLAGS) $^ -o $@

tests/test_tablero_exe: tests/test_Tablero.cpp Bloque.o Tablero.o Compuerta.o Salida.o  State.o Movimiento.o LectorArchivo.o
	$(CC) $(CFLAGS) $^ -o $@

tests/test_hash_exe: tests/test_TablaHash.cpp $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

tests/test_compuerta_exe: tests/test_Compuerta.cpp $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

tests/test_mov_exe: tests/test_Movimiento.cpp Movimiento.o State.o Tablero.o Bloque.o Salida.o Compuerta.o
	$(CC) $(CFLAGS) $^ -o $@

tests/test_solver_exe: tests/test_Solver.cpp $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@
tests/test_salida_exe: tests/test_Salida.cpp Salida.o
	$(CC) $(CFLAGS) $^ -o $@
tests/test_lector_exe: tests/test_LectorArchivo.cpp LectorArchivo.o Tablero.o Bloque.o Compuerta.o Salida.o State.o Movimiento.o
	$(CC) $(CFLAGS) $^ -o $@

# --- COMPILACIÓN SEPARADA (.o)  ---

%.o: %.cpp %.h
	$(CC) $(CFLAGS) -c $< -o $@

# --- COMANDOS DE EJECUCIÓN (Útiles para ti) ---

test_all: all
	@for exe in $(TEST_EXES); do ./$$exe; done

# --- LIMPIEZA COMPATIBLE CON LINUX  ---
clean:
	rm -f *.o tests/*.o $(TEST_EXES)