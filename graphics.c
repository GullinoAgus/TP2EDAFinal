
#include "graphics.h"
#include <stdbool.h>

/*Funcion graficarPuntos: grafica los puntos que se le pasen tomando de referencia un origen
    Parametro posOrigX: coordenada en x de pantalla donde se encuentra el origen de coordenadas
    Parametro posOrigY: coordenada en y de pantalla donde se encuentra el origen de coordenadas. Tener en cuenta que dibuja de abajo hacia arriba
    Parametro listaPuntos: Lista de puntos con el puntero al siguiente del ultimo punto = NULL
    Parametro avancePorUnidadX: Valor de una unidad de grafico en el eje X, o sea la escala quetiene ese eje
    Parametro avancePorUnidadY: Valor de una unidad de grafico en el eje Y, o sea la escala quetiene ese eje
    Parametro escala: tamanio en pixeles de una unidad de division del grafico

    En caso de parametros invalidos no hace nada
    */
static void graficarPuntos(int posOrigX, int posOrigy, punto_t* listaPuntos, double avancePorUnidadX, double avancePorUnidadY, double escala);

/*Funcion graficarCuadricula: Dibuja una cuadricula en pantalla segun los parametros recibidos
    Parametro posx: coordenada en x de pantalla donde se quiere comenzar a dibujar la cuadricula
    Parametro posy: coordenada en y de pantalla donde se quiere comenzar a dibujar la cuadricula
    Parametro width: Cantidad de celdas de ancho que quiere que tenga la cuadricula
    Parametro height: Cantidad de celdas de alto que quiere que tenga la cuadricula
    Parametro cellWidth: Ancho en pixeles de cada celda
    Parametro cellHeight: Alto en pixeles de cada celda

    En caso de parametros invalidos no hace nada
    */
static void graficarCuadricula(int posx, int posy, int width, int height, int cellWidth, int cellHeight);

/*Funcion graficarRobots: grafica los robots trasladando sus coordenadas a una cuadricula(piso) en pantalla
    Parametro cuadOriginX: coordenada en x de pantalla donde esta la esquina superior izquierda del piso de baldosas
    Parametro cuadOriginY: coordenada en y de pantalla donde esta la esquina superior izquierda del piso de baldosas
    Parametro escala: tamanio en pixeles de cada baldosa cuadrada
    Parametro robotList: Arreglo con los n robots, donde el n+1 tenga coordenada en x = -1 para usar como fin de lista

    En caso de parametros invalidos no hace nada
    */
static void graficarRobots(int cuadOriginX, int cuadOriginY,int escala, robot_t *robotList);

/*Funcion draw_vector: un vector tomando como origen un punto en pantalla pasado por parametro
    Parametro origx: coordenada en x del punto en pantalla desde el cual se dibujara el vector 
    Parametro origy: coordenada en y del punto en pantalla desde el cual se dibujara el vector
    Parametro modulo: modulo en unidades arbitrarias
    Parametro angulo: angulo en grado sexagesimales (0 a 360)
    Parametro color: color de allegro para el vector
    Parametro escala: tamanio en pixeles de una unidad arbritraria del modulo

    En caso de parametros invalidos no hace nada
    */
static void draw_vector(double origx, double origy, double modulo, double angulo, ALLEGRO_COLOR color, double escala);

/*Funcion puntosFueraDe1erCuad: analiza si hay algun punto en la lista que este fuera del primer cuadrante de R2
    Parametro listaPuntos: Lista de puntos con el puntero al siguiente del ultimo punto = NULL
    Devuelve: 1 si hay un punto fuera del primer cuadrante y 0 si no
*/
static int puntosFueraDe1erCuad(punto_t *listaPuntos);

/*Funcion getMaxY: Obtiene el valor mas alto en Y de la lista de puntos
    Parametro listaPuntos: Lista de puntos con el puntero al siguiente del ultimo punto = NULL
    Devuelve: El valor mas alto en Y encontrado
*/
static double getMaxY(punto_t *listaPuntos);

/*Funcion getMaxX: Obtiene el valor mas alto en X de la lista de puntos
    Parametro listaPuntos: Lista de puntos con el puntero al siguiente del ultimo punto = NULL
    Devuelve: El valor mas alto en X encontrado
*/
static double getMaxX(punto_t *listaPuntos);

void graficarPiso(int posx, int posy, piso_t *piso, robot_t *listaRobots, double escala){

    for (int i = 0; i < piso->h ; ++i) {                        //Recorremos las baldosas del piso
        for (int j = 0; j < piso->w; ++j) {
            if (piso->baldosas[i * piso->w + j]){               //Si esta limpia( = 1 ) la dibujamos de azul

                al_draw_filled_rectangle(posx +  j*escala, posy + ((double)piso->h-1)*escala - i*escala, posx + escala +  j*escala, posy + escala + ((double)piso->h-1)*escala - i*escala, al_map_rgb(100,150,255));

            }
            else{                                               //Si esta sucia( = 0 ) la dibujamos gris

                al_draw_filled_rectangle(posx +  j*escala, posy + ((double)piso->h-1)*escala - i*escala, posx + escala + j*escala, posy + escala + ((double)piso->h-1)*escala - i*escala, al_map_rgb(150,150,150));

            }
        }

    }
    //Dibujamos una cuadricula sobre las baldosas para darles un limite mas marcado
    graficarCuadricula(posx, posy, piso->w, piso->h, escala, escala);

    //Dibujamos los robots
    graficarRobots(posx, posy + piso->h*escala, escala, listaRobots);

}

void graficarFuncion(int posx, int posy, punto_t *listaPuntos, double escala){

    ALLEGRO_TRANSFORM t;                        //Transformacion para girar el texto
    ALLEGRO_FONT* font = al_load_ttf_font(FONTDIRECTORY, escala / 4, 0);     //Carga de la fuente para el grafico
    double maxXvalue = getMaxX(listaPuntos);            //obtenemos el valor maximo en X
    double maxYvalue = getMaxY(listaPuntos);            //Obtenemos el valor maximo en Y

    if (puntosFueraDe1erCuad(listaPuntos) || listaPuntos == NULL || escala < 1) //Si hay algun punto fuera del primer cuadrante, o la lista de puntos esta vacia, o la escala es demasiado pequenia
    {
        al_destroy_font(font);              // salimos sin hacer nada
        return;
    }

    posx += 1.05 * escala;                              //Ajuste de posicion para acumodar por el texto en los ejes
    posy += escala;

    al_draw_line(posx, posy - escala, posx, posy + escala* ((double)DIVISIONESDEGRAFICO + 1), al_map_rgb(0, 0, 0), escala / 40);          //dibujo el Eje Y
    al_draw_line(posx - escala, posy + escala*DIVISIONESDEGRAFICO, posx + escala*((double)DIVISIONESDEGRAFICO+1), posy + escala * (double)DIVISIONESDEGRAFICO, al_map_rgb(0, 0, 0), escala / 40);   //dibujo el eje x
    
    //Dibujo una cuadricula para seguir mejor el grafico
    graficarCuadricula(posx, posy, DIVISIONESDEGRAFICO, DIVISIONESDEGRAFICO, escala, escala);
    
    //escribo el 0 en el origen de coordenadas
    al_draw_text(font, al_map_rgb(0, 0, 0), (double)posx - escala * 0.1, (double)posy + escala * ((double)DIVISIONESDEGRAFICO + 0.1), ALLEGRO_ALIGN_RIGHT, "0");


    for (int i = 1; i <= DIVISIONESDEGRAFICO; i++)      //Escribo el eje Y con los valores de cada division
    {
        al_draw_textf(font, al_map_rgb(0, 0, 0), posx, posy + ((double)DIVISIONESDEGRAFICO - i) * escala - al_get_font_line_height(font) / 2, ALLEGRO_ALIGN_RIGHT, "%.2E", (maxYvalue / (double)DIVISIONESDEGRAFICO) * (double)i);
    }
    
    al_identity_transform(&t);              //Aplico una transformacion sobre el sistema de coordenadas que lo gira 90 grados y lo posiciona para escribir el texto en el eje X
    al_rotate_transform(&t, PI / 2.0f);
    al_translate_transform(&t, posx + escala * DIVISIONESDEGRAFICO + al_get_font_line_height(font) / 2, posy + escala * DIVISIONESDEGRAFICO);
    al_use_transform(&t);
    for (int i = 1; i <= DIVISIONESDEGRAFICO; i++)      //Escribo el eje X con los valores de cada division
    {
        al_draw_textf(font, al_map_rgb(0, 0, 0), 0, ((double)DIVISIONESDEGRAFICO - i) * escala, ALLEGRO_ALIGN_LEFT, "%.2E", (maxXvalue/(double)DIVISIONESDEGRAFICO)*i);
    }
    al_identity_transform(&t);                          //Devuelvo el sistema de coordenadas a la normalidad con la transformacion identidad
    al_use_transform(&t);
    
    //Dibujo los puntos en el grafico
    graficarPuntos(posx, posy + escala * DIVISIONESDEGRAFICO, listaPuntos, maxXvalue /((double) DIVISIONESDEGRAFICO), maxYvalue / ((double)DIVISIONESDEGRAFICO), escala);

    al_destroy_font(font);      //Destruyo la fuente antes de salir
 }

int initAllegro5(ALLEGRO_DISPLAY** p2display) {


    if (!al_init()) {                   //Inicializacion del display y los addons de allegro
        return -1;
    }
    if (!al_init_primitives_addon()) {
        return -1;
    }
    if (!al_init_font_addon()) {
        return -1;
    }
    if (!al_init_ttf_addon()) {
        return -1;
    }
    if (!al_install_keyboard()) {
        return -1;
    }
    *p2display = al_create_display(WINDOWWIDHT, WINDOWHEIGHT);
    if (*p2display == NULL) {
        return -1;
    }

    return 0;

}

static void graficarPuntos(int posOrigX, int posOrigY, punto_t *listaPuntos, double avancePorUnidadX, double avancePorUnidadY, double escala){

    punto_t* auxPunto;

    //Si hay algun error con los parametros salimos de la funcion
    if (listaPuntos == NULL || avancePorUnidadX <= 0 || avancePorUnidadY <= 0 || escala < 1)
    {
        return;
    }

    do          //Recorremos la lista de puntos graficando un circulo pequenio en la coordenada especificada por el punto trasladando todo a la pantalla
    {
        al_draw_filled_circle(posOrigX + (listaPuntos->x * escala)/avancePorUnidadX, posOrigY - (listaPuntos->y * escala) / avancePorUnidadY, escala/10, al_map_rgb(255,0,0));
        auxPunto = listaPuntos->next;
        listaPuntos = auxPunto;
    } while (listaPuntos != NULL);

}

static void graficarCuadricula(int posx, int posy, int width, int height, int cellWidth, int cellHeight){
    
    //Si hay algun error en los parametros salimos sin hacer nada
    if (width <= 0 || height <= 0 || cellHeight <= 0 || cellWidth <= 0)
    {
        return;
    }

    for (int i = 0; i <= width; ++i) {      //Dibujamos las lineas verticales de la tabla de acuerdo a las especificaciones del usuario

        al_draw_line(posx + i*cellWidth, posy, posx + i*cellWidth, posy + height*cellHeight, al_map_rgb(100,100,100), 1);

    }
    for (int i = 0; i <= height; ++i) {     //Dibujamos las lineas horizontales de acuerdo a las especificaciones del usuario

        al_draw_line(posx, posy + i*cellHeight, posx + width*cellWidth, posy + i*cellHeight, al_map_rgb(100,100,100), 1);

    }

}

static void graficarRobots(int cuadOriginX, int cuadOriginY, int escala, robot_t *robotList){

    double posXaux, posYaux;

    int escalaGraficaAux = escala;  //Escala auxiliar para el tamanio del punto que representa al robot
    if (escala < 20)                //Si la escala es mas chica que 20, forzamos un valor de 20 solo para los puntos que representan robots
    {                               //si no lo hicieramos y la escala fuera muy pequenia, serian imperceptibles
        escalaGraficaAux = 20;
    }

    for (int i = 0; robotList[i].x != -1 ; ++i) {           //Recorremos el arreglo de robots hasta encontrar el de final de arreglo

        posXaux = cuadOriginX + robotList[i].x*escala;      //Calculamos su posicion en pantalla
        posYaux = cuadOriginY - robotList[i].y*escala;

        draw_vector(posXaux, posYaux, 1, robotList[i].angle, al_map_rgb(0,0,255), escala);  //Dibujamos el vector de direccion del robot

        al_draw_filled_circle( posXaux, posYaux, escalaGraficaAux/10, al_map_rgb(255,0,0)); //Dibujamos al punto que representa al robot


    }

}

static void draw_vector(double origx, double origy, double modulo, double angulo, ALLEGRO_COLOR color, double escala){


    double cosAngl = cos(DEG2RAD(angulo))*escala;           //Valores auxiliares para grafico del vectopr
    double senAngl = -sin(DEG2RAD(angulo))*escala;

    double cosAnglM30 = cos(DEG2RAD(angulo + 180 - 30))*escala; //Valores auxiliares para el dibujado de la punta de la flecha del vector
    double senAnglM30 = -sin(DEG2RAD(angulo + 180 - 30))*escala;
    double cosAnglP30 = cos(DEG2RAD(angulo + 180 + 30))*escala;
    double senAnglP30 = -sin(DEG2RAD(angulo + 180 + 30))*escala;

    al_draw_line(origx, origy, origx + modulo*cosAngl, origy + modulo*senAngl, color, escala/25.0); //Dibujamos la linea como funcion de la escala

    //Dibujamos la punta del vector. Para esto pensamos que es un triangulo equilatero y los dos vertices de la base son la punta del vector menos dos vectores pequenios que 
    //representan los lados laterales del triangulo. Estos lados estan a 30 grados del vector a dibujar y por eso necesitamos el seno y cose a mas y menos 30 grados
    al_draw_filled_triangle(origx + cosAngl*modulo, origy + senAngl*modulo, origx + cosAngl*modulo + cosAnglM30/5, origy + senAngl*modulo + senAnglM30/5, origx + cosAngl*modulo + cosAnglP30/5, origy + senAngl*modulo + senAnglP30/5, color);
}

static int puntosFueraDe1erCuad(punto_t *listaPuntos){

    punto_t* puntoAux;

    if (listaPuntos == NULL)        //Chequeamos por lista vacia
    {
        return 1;
    }

    while(listaPuntos != NULL){         //Recorremos la lista de puntos buscando uno fuera de lprimer cuadrante de R2, y si lo hay salimos devolviendo 1
        if (listaPuntos->x < 0 || listaPuntos->y < 0)
        {
            return 1;
        }
        puntoAux = listaPuntos->next;
        listaPuntos = puntoAux;
    }

    return 0;
}

//LAS SIGUIENTES FUNCIONES SON PRACTICAMENTE IDENTICAS ASI QUE SOLO SE COMENTARA UNA. SABER QUE LA OTRA SIGUE LA MISMA LOGICA
static double getMaxY(punto_t *listaPuntos){

    double Yvalue = 0;
    punto_t *puntoAux;

    while(listaPuntos != NULL){             //Recorro la lista de puntos para buscar el valor de Y mas alto
        if (listaPuntos->y > Yvalue){
            Yvalue = listaPuntos->y;
        }
        puntoAux = listaPuntos->next;
        listaPuntos = puntoAux;
    }

    return Yvalue;                          //Si la lista era NULL devuelve 0

}

static double getMaxX(punto_t *listaPuntos){

    double Xvalue = 0;
    punto_t *puntoAux;

    while(listaPuntos != NULL){
        if (listaPuntos->x > Xvalue){
            Xvalue = listaPuntos->x;
        }
        puntoAux = listaPuntos->next;
        listaPuntos = puntoAux;
    }

    return Xvalue;

}