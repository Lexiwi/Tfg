/*********************************************************** 
* File:   elelist.h
* Author: Manuel Jiménez y Marcos Asenjo
* 
* Descripción: Define el elemento del TAD lista
*
* Date: 23/03/2016
* Asignatura: Programación II
* Práctica: 3
* Ejercicio: 4
***********************************************************/

#ifndef ELELIST_H
#define ELELIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 19


/****************  Nuevo tipo de datos para elemento de Lista *****************/
typedef struct _EleList EleList;

/********************* Declaracion de funciones públicas **********************/
/* Inicializa un EleList reservando memoria e inicializando todos sus elementos. */
EleList* elelist_ini();

/* Libera un EleList y todos sus elementos. */
void elelist_free(EleList * ele);

/* Modifica los datos de un EleList . */
EleList* elelist_setInfo(EleList * e, char* p);

/* Devuelve el contenido de un EleList . */
char* elelist_getInfo(const EleList * e);

/* Copia reservando memoria un EleList . */
EleList* elelist_copy(const EleList * src);

/* Devuelve un número positivo, negativo o cero según si ele1 es mayor, menor o igual que ele2. */
int elelist_cmp(const EleList * ele1, const EleList * ele2);

/* Imprime un EleList devolviendo el número de caracteres escritos. */
int elelist_print(FILE* pf, const EleList * ele);

#endif /*ELELIST_H*/