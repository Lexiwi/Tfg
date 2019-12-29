#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include "ruido.h"

/******************* Definicion de las estructuras de datos *******************/
struct _Ruido{
    int count;
    double tiempo;
};

/********************* Definición de funciones públicas ***********************/

Ruido* ruido_ini(){

    Ruido* ruido=NULL;
    
    ruido=(Ruido *)malloc(sizeof(Ruido));
    
    if(ruido == NULL)
        return NULL;

    ruido->count = 0;
    ruido->tiempo = 0.0;

    return ruido;
}

void ruido_free(Ruido* ruido){

    if(!ruido)
        return;
    
    free(ruido);
    return;
}

int getRuidoCount(Ruido* ruido) {

    if(!ruido)
        return -1;

    return ruido->count;

}

int setRuidoCount(Ruido* ruido, int cnt) {

    if(!ruido || cnt < 0)
        return -1;

    ruido->count = cnt;
    return 0;
}

double getRuidoTiempo(Ruido* ruido) {

    if(!ruido)
        return -1.0;

    return ruido->tiempo;

}

int setRuidoTiempo(Ruido* ruido, double tmp) {

    if(!ruido || tmp < 0.0)
        return -1;

    ruido->tiempo = tmp;
    return 0;
}

int actualizaRuido(Ruido* ruido, double tmp) {

    if(!ruido || tmp < 0.0)
        return -1;
    
    ruido->tiempo = tmp;
    ruido->count++;
    return 0;
}
 

 