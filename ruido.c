/*********************************************************** 
* File:   ruido.c
* Author: Jorge Gutierrez Diaz
* 
* Descripción: TAD para el ruido de los paquetes UDP.
*
***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include "ruido.h"

/******************* Definicion de las estructuras de datos *******************/
struct _Ruido{
    int count;          // Cantidad de ruido
    double tiempo;      // Hora de la ultima actualizacion
};

/********************* Definición de funciones públicas ***********************/

/************************************************************************
* @Title: ruido_ini
* @Description: Reserva memoria e inicializa la estructura
* @Input: Ninguno
* @Output: Ruido *: Puntero a la estructura creada
************************************************************************/
Ruido* ruido_ini(){

    Ruido* ruido=NULL;
    
    ruido=(Ruido *)malloc(sizeof(Ruido));
    
    if(ruido == NULL)
        return NULL;

    ruido->count = 0;
    ruido->tiempo = 0.0;

    return ruido;
}

/************************************************************************
* @Title: ruido_free
* @Description: Libera la memoria asociada a la estructura
* @Input: Ruido* : Puntero a la estructura
* @Output: void
************************************************************************/
void ruido_free(Ruido* ruido){

    if(!ruido)
        return;
    
    free(ruido);
    return;
}


/************************************************************************
* @Title: getRuidoCount
* @Description: Devuelve la cantidad de paquetes ruido
* @Input: Ruido* : Puntero a la estructura
* @Output: int: -1 si ocurre un fallo
************************************************************************/
int getRuidoCount(Ruido* ruido) {

    if(!ruido)
        return -1;

    return ruido->count;

}

/************************************************************************
* @Title: setRuidoCount
* @Description: Actualiza la cantidad de paquetes ruido
* @Input: Ruido* : Puntero a la estructura
*         int: Nuevo valor
* @Output: int: -1 si ocurre un fallo, 0 si no
************************************************************************/
int setRuidoCount(Ruido* ruido, int cnt) {

    if(!ruido || cnt < 0)
        return -1;

    ruido->count = cnt;
    return 0;
}

/************************************************************************
* @Title: getRuidoTiempo
* @Description: Devuelve la hora de la ultima actualizacion
* @Input: Ruido* : Puntero a la estructura
* @Output: double: -1.0 si ocurre un fallo
************************************************************************/
double getRuidoTiempo(Ruido* ruido) {

    if(!ruido)
        return -1.0;

    return ruido->tiempo;

}

/************************************************************************
* @Title: setRuidoTiempo
* @Description: Actualiza la hora de la ultima actualizacion
* @Input: Ruido* : Puntero a la estructura
*         double: Nuevo tiempo
* @Output: int: -1 si ocurre un fallo, 0 si no
************************************************************************/
int setRuidoTiempo(Ruido* ruido, double tmp) {

    if(!ruido || tmp < 0.0)
        return -1;

    ruido->tiempo = tmp;
    return 0;
}

/************************************************************************
* @Title: actualizaRuido
* @Description: Actualiza todos los valores de la estructura
* @Input: Ruido* : Puntero a la estructura
*         double: Nuevo tiempo
* @Output: int: -1 si ocurre un fallo, 0 si no
************************************************************************/
int actualizaRuido(Ruido* ruido, double tmp) {

    if(!ruido || tmp < 0.0)
        return -1;
    
    ruido->tiempo = tmp;
    ruido->count++;
    return 0;
}

/************************************************************************
* @Title: ruido_print
* @Description: Imprime la estructura de Ruido
* @Input: Ruido* : Puntero a la estructura
* @Output: void
************************************************************************/
void ruido_print(Ruido* ruido) {

    if(!ruido)
        return;
    
    printf("\nNumero de paquetes: %d\n", ruido->count);
    printf("Ultimo paquete recibido a: %f (ms)\n", ruido->tiempo);

    return;
}

/************************************************************************
* @Title: ruido_copy
* @Description: Copia la estructura de Ruido
* @Input: Ruido* : Puntero a la estructura
* @Output: Ruido*: copia de la estructura
************************************************************************/
Ruido* ruido_copy(const Ruido* ruido) {

    Ruido* aux=NULL;

    if(!ruido)
        return NULL;

    aux = ruido_ini();
    if(!aux)
        return NULL;

    aux->count =  ruido->count;
    aux->tiempo = ruido->tiempo;

    return aux;
}
 

 