#ifndef INIT_H
#define INIT_H

/**************HEADER FILES************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "limpiador.h"
#include "parser.h"

/**************STRUCTURES**************/
/*Esta va a ser la estructura donde se guarden los datos recibidos por linea de comandos*/
typedef struct {

	int largo;
	int ancho;
	int modo;
	int robots;

}data_t;


/*Funcion recieve_data: es el callback que llama parseCmdLine. Se encarga de verificar
	si las opciones ingresadas son correctas y llena la estructura con los datos recibidos.
	Parametro key: un puntero a la clave de la opcion, si se recibio un parametro es NULL.
	Parametro value: un puntero al valor de la opcion, o al parametro.
	Parametro userData: se usa como un puntero a la estructura de tipo data_t.
	
	En el caso de error devuelve 1, si todo salio bien devuelve 0. Los errores suceden si 
	se ingresa un parametro suelto o una clave invalida, si el valor es 0 o tiene un
	caracter invalido, si se ingresa dos veces la misma clave, o si el valor excede 
	los limites del programa. */
int recieve_data(char* key, char* value, void* userData);

/*Funcion init_floor: se encarga de reservar en el heap un lugar para las baldosas 
	y las inicializa en 0, indicando que estan sucias. Tambien carga en la estructura
	del piso los valores del largo y del ancho
	Parametro floor: estructura que contiene los datos del piso.
	Parametro userData: estructura que contiene los datos ingresados
	
	Devuelve -1 si no se pudo reservar el lugar en el heap y 0 si se pudo*/
int init_floor(piso_t* floor, data_t* userData);

/*Funcion init_robot: Se encarga de reservar un lugar en el heap para los robots. 
	Crea la lista de robots e inicializa sus coordenadas y direccion de manera random.
	Parametro n_robots: es el numero de robots ingresado.
	Parametro floor: es un puntero a la estructura del piso.
	
	Devuelve un puntero a NULL si no se pudo reservar el lugar en el heap, y si no devuelve
	un puntero a la lista de robots.*/
robot_t* init_robot(int n_robots, piso_t* floor);


#endif
