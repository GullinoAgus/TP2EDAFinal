#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "limpiador.h"

#define WINDOWWIDHT 800                 //Ancho de pantalla
#define WINDOWHEIGHT 600                //Alto de pantalla
#define DIVISIONESDEGRAFICO 10          //Cantidad divisiones a utilizar en los graficos de puntos que se dibujan
#define FONTDIRECTORY ".//monofonto.ttf"    //Directorio de la fuente para el grafico de puntos

typedef struct punto {                      //Estructura de punto para utilizar al momento de hacer los graficos de puntos

    double x;
    double y;
    struct punto *next;

} punto_t;

/*
 * Funciones globales
 */

/*Funcion initAllegro: Inicializacion de allegro, sus addons y el display
    Parametro p2display: referencia al puntero al display que se va a inicializar
    Devuelve: -1 si hubo algun error, 0 si todo esta correcto
    */
int initAllegro5(ALLEGRO_DISPLAY** p2display);

/*Funcion graficarPiso: Dibujado del piso junto con los robots
    Parametro posx: coordenada en x de pantalla donde se dibujara el piso
    Parametro posy: coordenada en y de pantalla donde se dibujara el piso
    Parametro piso: estructura de piso para dibujar.
    Parametro listaRobots: Arreglo con los n robots, donde el n+1 tenga coordenada en x = -1 para usar como fin de lista
    Parametro escala: tamanio en pixeles de cada baldosa
    
    En caso de parametros invalidos no hace nada
    */
void graficarPiso(int posx, int posy, piso_t *piso, robot_t *listaRobots, double escala);

/*Funcion graficarFuncion: Dibujado de un lista de puntos con valores en el primer cuadrante de R2 en un eje decoordenadas
    Parametro posx: coordenada en x de pantalla donde se dibujara el grafica
    Parametro posy: coordenada en y de pantalla donde se dibujara el grafica
    Parametro listaPuntos: Lista de puntos con el puntero al siguiente del ultimo punto = NULL
    Parametro escala: tamanio en pixeles de una unidad de division del grafico

    En caso de parametros invalidos no hace nada
    */
void graficarFuncion(int posx, int posy, punto_t *listaPuntos, double escala);


#endif