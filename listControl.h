/*********************************************************** 
 * File:   list.h
 * Author: Manuel Jiménez y Marcos Asenjo
 * 
 * Descripción: Define el TAD lista
 *
 * Date: 22/03/2016
 * Asignatura: Programación II
 * Práctica: 3
 * Ejercicio: 4
 ***********************************************************/


#ifndef LISTCONTROL_H
#define LISTCONTROL_H


/**********************  Nuevo tipo de datos para Lista ***********************/
typedef struct _ListControl ListControl;
typedef struct _NodeControl NodeControl;

/********************* Declaracion de funciones públicas **********************/
/* Inicializa la lista reservando memoria e inicializa todos sus elementos. */
ListControl* listControl_ini();

/* Libera la lista y todos sus elementos. */
void listControl_free(ListControl* list);

/* Inserta al principio de la lista realizando una copia del elemento. */
ListControl* listControl_insertFirst(ListControl* list, const char *canal, const double tiempo, const int info);

/* Inserta al final de la lista realizando una copia del elemento. */
ListControl* listControl_insertLast(ListControl* list, const char *canal, const double tiempo, const int info);

/* Extrae del principio de la lista realizando una copia del elemento almacenado en dicho nodo. */
char* listControl_extractFirst(ListControl* list);

/* Extrae del final de la lista realizando una copia del elemento almacenado en dicho nodo. */
char* listControl_extractLast(ListControl* list);

/* Comprueba si una lista está vacía o no. */
int listControl_isEmpty(const ListControl* list);

/* Devuelve el tamaño de una lista. */
int listControl_size(const ListControl* list);

/* Imprime una lista devolviendo el número de caracteres escritos. */
int listControl_print(const ListControl* list);

/* Comprueba si el elemento esta en la lista. */
int listControl_check_element(const ListControl* list, char* ip);

/* Eliminamos un elemento de la lista*/
int listControl_extractElement(ListControl* list, char* ip);

char* getCanal(NodeControl* node);
double getTiempo(NodeControl* node);
int setTiempo(NodeControl* node, double tmp);
int getInfo(NodeControl* node);
int setInfo(NodeControl* node, int inf);
NodeControl* getNext(NodeControl* node);
NodeControl* getNode(ListControl* list, char* canal);
NodeControl* getNodePos(ListControl* list, int pos);
ListControl* listControl_copy(const ListControl* list);

#endif /*LIST_H*/