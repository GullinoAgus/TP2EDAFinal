#include "limpiador.h"
#include "SenCosTable.h"
//Devuelve 1 si el robot puede avanzar, o 0 si no puede.
static int puedeAvanzar(double x, double y, int alto, int ancho);

int fisicas (piso_t *piso, robot_t *r)
{
    baldosa_t *baldosas = piso->baldosas;
    double robotX, robotY;
    int res;
    int baldosasLimpiadas = 0;

    while( (r->x) != (-1.0) )
    {
        //A que (x, y) avanzaria el robot:
        robotX = r->x + cosT[(int)(r->angle * 10.0)];//cos( DEG2RAD(r->angle) );
        robotY = r->y + sinT[(int)(r->angle * 10.0)];//sin( DEG2RAD(r->angle) );
        res = puedeAvanzar(robotX, robotY, piso->h, piso->w);

        if (res == 1) //Pudede avanzar
        {
            r->x = robotX;
            r->y = robotY;
            //Ahora debe limpiar la baldosa
            if (baldosas[((int)robotY) * (piso->w) + ((int)robotX)] == 0)
            {
                baldosasLimpiadas++;
                baldosas[((int)robotY) * (piso->w) + ((int)robotX)] = 1;  //Limpia baldosa
            }
            
        }
        else    //Debe cambiar su orientacion
        {
            r->angle = (rand() % 3600) / 10; //Angulo entre 0 y 359.9
        }
        ++r;
    }

    return baldosasLimpiadas;
}

static int puedeAvanzar (double x, double y, int alto, int ancho)    
{
    int res = 1;    
    //Primero analiza la coordenada x
    if ((x < 0) || (x >= (double)ancho)) //Quiere decir que el robot chocaria
    {
        res = 0;    //Robot no puede avanzar
    }

    //Analiza la coordenada y
    if ((y < 0) || (y >= (double)alto)) //Quiere decir que el robot chocaria
    {
        res = 0;    //Robot no puede avanzar
    }
    return res;
}