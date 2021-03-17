#include <stdio.h>
#include <time.h>
#include "graphics.h"
#include "init.h"


#define TPS 2.0                     // Ticks per second utilizados en el modo 1
#define REPESPARAPROMEDIO 1000      // Cantidad de repeticiones a promediar para el modo 2



int main (int argc, char *argv[]){
    
    srand(time(NULL));              //Semilla para la funcion rand que se utiliza en init.c y limpiador.c
    
    int exit = 0;                   //Variable de salida para cerrar la ventana
    int error = 0;
    int cantRobots = 1;             //Contador para la cantidad de robots en uso. Se utiliza en el modo 2
    int calcDone = false;           
    int contBaldosasLimpias = 0;    
    double tickCount = 0;
    piso_t piso;
    robot_t* robots = NULL;
    punto_t* listaPuntos = NULL, *puntoNuevo = NULL, *puntoAnterior = NULL; //Punteros para generar la lista de puntos a graficar en el modo 2


    /*DECLARACION DE VARIABLE PARA ALLEGRO*/
    ALLEGRO_DISPLAY *display;
    ALLEGRO_TIMER *timer;
    ALLEGRO_EVENT evento;
    ALLEGRO_EVENT_QUEUE *colaEventos;
    ALLEGRO_FONT* font;
    
    //Declaracion e inicializacion necesaria de la estructura data para pasar al callback
    data_t data = { .largo = 0, .ancho = 0, .modo = 0 , .robots = 0 };  

    int var = parseCmdLine(argc, argv, &recieve_data, &data);        //Llamada al parser para recibir la info de linea de comandos
    if (var == -1 || (data.modo == 1 && data.robots == 0) || data.ancho == 0 || data.largo == 0) {  //Si hay un error o la informacion no se completo correctamente 
        printf("Hay un error o faltan con los parametros recibidos\n");                               //Imprimimos un aviso y salimos del programa
        return 0;
    }
                      
    if (data.modo == 1)                             //En caso de que se haya seleccionado el modo 1 
    {
        error = init_floor(&piso, &data);                   //Inicializamos el piso y los robots aqui
        if (error == -1)
        {
            printf("No se pudo inicializar el piso");
            return 0;
        }
        robots = init_robot(data.robots, &piso);
        if (robots == NULL)
        {
            printf("No se pudieron inicializar los Robots\n");
            free(piso.baldosas);
            return 0;
        }
    }


    
    if (initAllegro5(&display) == -1) {             //Si falla la inicializacion de los componentes de allegro, liberamos lo reservado si era modo 1
        printf("Hay un error en la inicializacion de ALLEGRO\n"); //e imprimimos un mensaje de error antes de salir del programaa
        if (data.modo == 1)
        {
            free(robots);
            free(piso.baldosas);
        }
        return 0;
    }

    font = al_load_ttf_font(".//monofonto.ttf", 15, 0);
    timer = al_create_timer(1 / TPS);                           //Inicializamos el timer que se utiliza para el modo 1
    colaEventos = al_create_event_queue();                      
    if (timer == NULL || colaEventos == NULL || font == NULL)
    {
        printf("Error al inicializar la fuente, el timer o la cola de eventos\n");
        if (data.modo == 1)
        {
            free(robots);
            free(piso.baldosas);
        }
        return 0;
    }

    al_register_event_source(colaEventos, al_get_display_event_source(display));    //Registramos el timer y el display como fuente de eventos
    al_register_event_source(colaEventos, al_get_timer_event_source(timer));
    if (data.modo == 1) {           //Si estamos en modo 1 iniciamos el timer
        al_start_timer(timer);
    }
    while (!exit && !error) {                                 // Loop para GUI

        

        if (!al_event_queue_is_empty(colaEventos))      //revisamos la cola de eventos
        {
            al_get_next_event(colaEventos, &evento);    //Tomamos el evento
            switch (evento.type) {                      
            case ALLEGRO_EVENT_TIMER:                   //Si fue el timer procesamos y dibujamos los robots en pantalla
                al_clear_to_color(al_map_rgb(255, 255, 255));   //Ponemos el fondo blanco. Con esto se cega al usuario un poquito

                contBaldosasLimpias += fisicas(&piso, robots);  //Corremos un tick
                tickCount++;

                al_draw_textf(font, al_map_rgb(0, 0, 0), WINDOWWIDHT / 2, 5, ALLEGRO_ALIGN_CENTER, "Simulando...  Ticks: %.0f", tickCount);

                //Dibujamos el piso junto con los robots
                graficarPiso(WINDOWWIDHT/2 - (WINDOWHEIGHT/piso.h)*piso.w/2, 20, &piso, robots, (WINDOWHEIGHT - 20)/piso.h);

                if (contBaldosasLimpias == piso.h*piso.w)   //En caso de haber terminado de limpiar el piso paramos el timer y liberamos los bots
                {
                    al_draw_textf(font, al_map_rgb(0, 0, 0), WINDOWWIDHT / 2, 5, ALLEGRO_ALIGN_CENTER, "Simulacion completada. Ticks totales: %.0f", tickCount);
                    al_stop_timer(timer);
                    free(robots);
                    free(piso.baldosas);
                }
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:           //Si se presiona cerrar en el display salimos del loop
                exit = 1;
                break;
            }
        }
        if (data.modo == 2 && !calcDone)                //mientras estemos en modo 2 y los calculos no esten completos
        {
            tickCount = 0;                              
            for (int i = 0; i < REPESPARAPROMEDIO; i++)     //Corremos las repeticiones
            {
                contBaldosasLimpias = 0;
                init_floor(&piso, &data);                   //inicializamos el piso y los robots para generar una nueva distribucion en cada repeticion
                error = init_floor(&piso, &data);                   //Inicializamos el piso y los robots aqui
                if (error == -1)
                {
                    printf("Abortando, piso todo roto");
                    error = 1;
                    break;
                }
                robots = init_robot(cantRobots, &piso);
                if (robots == NULL)
                {
                    free(piso.baldosas);
                    printf("Abortando, se quemaron los robots\n");
                    error = 1;
                    break;
                }
                while (piso.h * piso.w != contBaldosasLimpias)  //Corremos la simulacion y contamos los ticks hasta que es piso este limpio
                {
                    contBaldosasLimpias += fisicas(&piso, robots);
                    tickCount++;
                }
                free(robots);                               //Liberamos los datos que ya no vamos a usar
                free(piso.baldosas);
            }
            if (cantRobots == 1)                            //En caso de que sea la primera pasada hay q inicializar la lista de puntos
            {
                listaPuntos = calloc(1, sizeof(punto_t));
                puntoNuevo = listaPuntos;
                puntoNuevo->x = cantRobots;                 //Cargamos los datos en el primer punto de la lista
                puntoNuevo->y = tickCount / REPESPARAPROMEDIO;
                puntoAnterior = puntoNuevo;                 //Para luego chequear si es la primera pasada
            }
            else{                                           
                puntoAnterior = puntoNuevo;                 //Guardamos el punto anterior
                puntoNuevo = calloc(1, sizeof(punto_t));    //Generamos el punto actual
                puntoAnterior->next = puntoNuevo;           //Lo agregamos a la lista y llenamos los datos
                puntoNuevo->x = cantRobots;
                puntoNuevo->y = tickCount / REPESPARAPROMEDIO;
            }
            cantRobots++;                                   //Aumentamos la cantidad de robots para la siguiente iteracion

            //Imprimimos en consola la diferencia actual y el numero de robots
            printf("t(n)-t(n-1) = %.2f   n = %d\n", fabs(puntoNuevo->y - puntoAnterior->y), cantRobots - 1);

            al_clear_to_color(al_map_rgb(255, 255, 255));   //Ponemos el fondo blanco. Con esto se cega al usuario un poquito
            if (fabs(puntoNuevo->y - puntoAnterior->y) < 0.1 && puntoAnterior != puntoNuevo)    //en caso de que no sea la primera pasada y la diferencia de ticks entre los puntos n y n-1 < 0.1
            {
                calcDone = 1;                   //Damos por finalizados los calculos
                al_draw_textf(font, al_map_rgb(0, 0, 0), WINDOWWIDHT / 2, 10, ALLEGRO_ALIGN_CENTER, "Procesamiento completado");

            }
            else                            //Si no se finalizo la simulacion lo indicamos
            {
                al_draw_textf(font, al_map_rgb(0, 0, 0), WINDOWWIDHT / 2, 10, ALLEGRO_ALIGN_CENTER, "Procesando...");
            }
            graficarFuncion(100, 100, listaPuntos, WINDOWHEIGHT / 15);      //A medida que se obtienen los punto los vamos graficando
            
        }
        al_flip_display();              //Dibujamos todo lo escrito en el display
    }

    //Liberamos todos los datos de allegro y salimos
    al_destroy_display(display);
    al_destroy_event_queue(colaEventos);
    al_destroy_timer(timer);
    al_destroy_font(font);
    return 0;
}


