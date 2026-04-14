# TDP1
trabajo 1 taller de programacion
/*
parametrizar
Estados
se busca que sea eficiente la tarea, a lo bruto
hacer clases tablero, bloque, salida, portal
cambiar State, significa cambiar test
hacer clases de operaciones
hacer clase Background, para la parte estatica
hacer clase para cargar archivos, menu
realizar test por cada clase para asegurarnos funcionamiento del programa
definiciones de bloques dentro de la parte estatica
Color va a cambiar, de inicial a final.
Inicialmente ciclos for pueden ser costoso, despues se optimizaŕa debido a binarios
color de A-Z
encontrar la solucion lo mas luego posible, operaciones utilizando 
archivo open debe ser un arbol binario bien balanceado, que tenga los nodos, lo que se llama heap, o cola de prioridad 
(implementacion estandar de A*) el asunto es generar un valor que mi indique cual es el mejor nodo posible
Algoritmo de busqueda logaritmica como arbol binario .

*/
State{
  Tablero t;
  ...
}
Tablero{
  cantidad de bloques;
  Bloques *b;
  cantidad restante; //marcamos lo bloques como que ya los dejamos, en vez de sacaros del arreglo
  numero de pasos;
  cantidad maxima de pasos;
  ubicaciones de las salidas;
  Salidas *s //variables
  Poral *p;
}
Solver{
  paredes;
  configuracion estatica del tablero; //Background objeto, esto es fijo | fin->Background
  open  
  all
  final //aca busco el estado de la solucion
  
}

