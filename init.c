#include "init.h"


/*Funcion set_number: recibe un arreglo de las cifras de un numero con datos de tipo 
    char y se encarga de ordenarlo y devolverlo como un numero (int).
    Parametro value: es un puntero al primer elemento del arreglo donde esta el numero.
    
    En caso de error devuelve -1. Los errores tienen en cuenta caracteres invalidos.
    */
int set_number(char* value);


int recieve_data(char* key, char* value, void* userData) 
{
    int ret_value = 1; //Si todo sale bien este valor no cambia
    int number;
    if (key == NULL) //si hay un parametro suelto devuelve error
    {
        ret_value = 0;
    }
    else //es opcion
    {
        number = set_number(value); //se obtiene el numero ingresado en el valor
        if ((number == -1)||(number==0))  //si se ingreso un valor invalido se marca un error
        {
            ret_value = 0;
        }
        //recorre las distintas claves validas
        else if (!strcmp("largo", key)) 
        {
            if ((number > 70)||((data_t*)userData)->largo)  //si el valor recibido esta en el rango incorrecto o ya se recibio esta clave marca error
            {
                ret_value = 0;
            }
            else
            {
                ((data_t*)userData)->largo = number; //se llena en la estructura el dato correspondiente
            }
        }
        else if (!strcmp("ancho", key))
        {
            if ((number > 100) || ((data_t*)userData)->ancho)   //si el valor recibido esta en el rango incorrecto o ya se recibio esta clave marca error
            {
                ret_value = 0;
            }
            else
            {
                ((data_t*)userData)->ancho = number; //se llena en la estructura el dato correspondiente
            }
        }
        else if (!strcmp("modo", key))
        {
            if (((number != 1) && (number != 2))|| ((data_t*)userData)->modo)  //verifica que el modo sea o 1 o 2 y que solo se haya ingresado una vez
            {
                ret_value = 0;
            }
            else
            {
                ((data_t*)userData)->modo = number; //se llena en la estructura el dato correspondiente
            }
        }
        else if (!strcmp("robots", key)) 
        {
            if (((data_t*)userData)->robots) //si se ingresa la clave dos veces marca error
            {
                ret_value = 0;
            }
            else 
            {
                ((data_t*)userData)->robots = number; //se llena en la estructura el dato correspondiente
            }
        }
        else //si la clave no existe
        {
            ret_value = 0;
        }
    }
    return ret_value;
}


int set_number(char* value)
{
    int number = 0;
    int error = 0;
    int cifra = 0;
    while ((value[cifra] != 0) && (!error))  //Se verifica que no sea el terminador del string y que no haya error
    {
        if ((value[cifra] < 48) || (value[cifra] > 57)) //Se verifica que no sea un caracter invalido. Solo se aceptan los numeros del 0 al 9, en ASCII del 48 al 57.
        {
            error = 1; //Si es un caracter invalido
        }
        else
        {
            number *= 10; //Se agrega una cifra mas al numero
            number += (value[cifra] - 48); //Se le suma la ultima cifra. Como se recibe de tipo char, se acomoda a tipo int.
            cifra++; //Para seguir leyendo el arreglo
        }
    }
    if (error)
    {
        number = -1; //Si hubo un error se devuelve -1
    }
    return number;
}

int init_floor(piso_t* floor, data_t* userData)
{
    if (userData->ancho <= 0 || userData->largo <= 0) //Se verifican los valores del ancho y del largo
    {
        return -1;
    }

    floor->h = userData->largo; //Se carga en la estructura del piso el valor del largo
    floor->w = userData->ancho; //Se carga en la estructura del piso el valor del ancho
    int n_baldosas = floor->h * floor->w; //Se calcula el numero de baldosas
    floor->baldosas = (baldosa_t*)calloc(n_baldosas, sizeof(baldosa_t)); //Se crea una lista de baldosas en el heap, inicializadas en 0
    if (floor->baldosas == NULL) //Si no se pudo reservar el espacio devuelve error
    {
        return -1;
    }

    return 0; 
}


robot_t* init_robot(int n_robots, piso_t* floor)
{
    robot_t* robot_list; 
    int contador;

    if (n_robots <= 0 || floor == NULL) //verifica que los parametros recibidos sean correctos
    {
        return NULL; //Si no devuelve error
    }

    robot_list = (robot_t*)calloc(n_robots + 1, sizeof(robot_t)); //Reserva un lugar en el heap para los robots mas un ultimo valor que funciona como terminador

    for (contador = 0; contador < n_robots; contador++) //Recorre robot por robot
    {
        robot_list[contador].x = (double)(rand() % (floor->w * 10)) / 10.0; //Le carga coordenadas random dentro de la pantalla a los robots 
        robot_list[contador].y = (double)(rand() % (floor->h * 10)) / 10.0; 
        robot_list[contador].angle = (double)(rand() % 3599) / 10;   //Le carga una direccion random entre 0 y 359,9 a los robots
    }

    robot_list[n_robots].x = -1; //Este es el terminador de la lista

    return robot_list;
}



