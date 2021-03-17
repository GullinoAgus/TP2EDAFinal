
#ifndef LIMPIADOR_H
#define LIMPIADOR_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//Macros
#define PI 3.14159265359
#define DEG2RAD(x) ((x) * (PI)/(180))	//Convierte angulo en grados a radianes.

typedef char baldosa_t;

//Estructuras
typedef struct 
{
    double x;	    //Coordenadas del Robot.
    double y;
    double angle;   //Angulo en grados.

} robot_t;	
//Luego en la incializacion de robots, la lista de robots finalizara con un robot extra con coordenada x = -1.

typedef struct 
{
    int h;	//Alto del piso (en cantidad de baldosas).
    int w;	//Ancho del piso (en cantidad de baldosas).
    baldosa_t *baldosas; //Baldosa en 1 --> Esta limpia.

} piso_t;

//Prototipos
/*Funcion fisicas: Se encarga del movimiento de los robots, cambiar la direccion de ellos en caso.
de colision, y limpia las baldosas
    Parametro piso: puntero a estructura piso_t ya inicializada
    Parametro r: Arreglo con los n robots, donde el n+1 tenga coordenada en x = -1 para usar como fin de lista
    Deveulve: Cantidad de baldosas limpiadas en dicho llamado a esta funcion
    */
int fisicas(piso_t *piso, robot_t *r);     

#endif