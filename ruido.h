/*********************************************************** 
* File:   ruido.h
* Author: Jorge Gutierrez Diaz
* 
* Descripción: TAD para guardar el ruido de paquetes udp.
*
***********************************************************/

#ifndef RUIDO_H
#define RUIDO_H


/**********************  Nuevo tipo de datos para Lista ***********************/
typedef struct _Ruido Ruido;

/********************* Declaracion de funciones públicas **********************/

/* Inicializamos la estructura*/
Ruido* ruido_ini();

/* Liberamos la estructura*/
void ruido_free(Ruido* ruido);

/* Devolvemos la cantidad de ruido*/
int getRuidoCount(Ruido* ruido);

/* Modificamos la cantidad de ruido*/
int setRuidoCount(Ruido* ruido, int cnt);

/* Devolvemos la hora */
double getRuidoTiempo(Ruido* ruido);

/* Modificamos la hora*/
int setRuidoTiempo(Ruido* ruido, double tmp);

/* Actualizamos la estructura completamente*/
int actualizaRuido(Ruido* ruido, double tmp);

/* Imprimimos la estructura*/
void ruido_print(Ruido* ruido);

/* Copiamos la estructura*/
Ruido* ruido_copy(const Ruido* ruido);

#endif /*RUIDO_H*/