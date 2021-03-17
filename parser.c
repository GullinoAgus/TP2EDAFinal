#include "parser.h"
#include <stdio.h>


int parseCmdLine(int argc, char *argv[], pCallback_t p, void *userData){

    int optParamCounter = 0;            //contador de parametros y opciones

    for (int i = 1; i < argc; i++)      //para cada string de argv chequeamos...
    {
        
        if (*argv[i] == '-')            //Si el primer caracter es un guion tenemos una clave
        {
            if (*(argv[i]+1) == '\0' || i+1 == argc)    //mientras que el segundo caracter no sea el terminador de string, y haya un string despues de este vamos bien, sino error
            {
                return -1;
            }
            
            optParamCounter++;                          //Si no hubo error lo contamos
            if((*p)(argv[i]+1, argv[i+1], userData) == 0)   //lo pasamoas al callback y vemos como responde para evaluar si cortar la lectura o no
            {
                return -1;
            }
            i++;                                        //Como leimos una clave que tiene asociado un valor movemos el contador un lugar para q cuando se incremente
                                                        //al final del for apunte a la opcion despues del valor
        }
        else{                                           //Si no habia guion es un parametro
            optParamCounter++;                          //Como los parametros no generan errores directamente contamos
            if((*p)(NULL, argv[i], userData) == 0)      //Lo enviamos al callback y en caso de error salimos
            {
                return -1;
            }
        }
        

    }
    
    return optParamCounter;                             //En caso de terminar con toda la lista de strings de forma correcta devolvemos la cuenta de opciones y parametros
}