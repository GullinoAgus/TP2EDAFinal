

#ifndef PARSER_H
#define PARSER_H
/*******DEFINICON DE TIPO DE DATOS*******/
typedef int (*pCallback_t) (char *, char *, void *);       //Tipo de dato para funcion callback.

/*******PROTOTIPO DE FUNCION GLOBAL******/
/* Funcion parseCmdLine: Esta funcion recibe las opciones y parametros ingresados al momento de ejecutar el programa y las procesa a traves de un callback.
 * Parametro argc: Cantidad de strings que se ingresan en argv
 * Parametro argv: lista de strings ingresados al momento de ejecutar, es la misma lista con la que el sistema llama al main(int argc, char *argv[])
 * Parametro p: Funcion de callback definida como pCallback_t
 * Parametro userData: puntero que se pasa al callback como tercer parametro
 * Devuelve: -1 si hubo un error o la cantidad de opciones y parametros procesados en caso de lectura correcta
 *
 * La funcion le pasa al callback la informacion de esta manera:
 *  (*p)(clave, valor, userData);
 *  En caso de que se encuentre un parametro, clave sera NULL y el parametro se pasara por valor.
 *  La funcion debe devolver 0 si hubo un error y 1 en caso de que este todo bien
 *  */
int parseCmdLine(int argc, char *argv[], pCallback_t p, void *userData);

#endif