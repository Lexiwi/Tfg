/*********************************************************** 
* File:   listControl.h
* Author: Jorge Gutierrez Diaz
* 
* Descripción: Lista para guardar informacion IGMP y ruido 
* del analisis.
***********************************************************/


#ifndef LISTCONTROL_H
#define LISTCONTROL_H


/**********************  Nuevo tipo de datos para ListaControl ***********************/
typedef struct _ListControl ListControl;
typedef struct _NodeControl NodeControl;

/********************* Declaracion de funciones públicas **********************/

/* Inicializa la lista */
ListControl* listControl_ini();

/* Libera la lista */
void listControl_free(ListControl* list);

/* Inserta al principio de la lista realizando una copia de los elemento. */
ListControl* listControl_insertFirst(ListControl* list, const char *canal, const double tiempo, const int info);

/* Inserta al final de la lista realizando una copia de los elemento. */
ListControl* listControl_insertLast(ListControl* list, const char *canal, const double tiempo, const int info);

/* Extrae del principio de la lista realizando una copia del canal. */
char* listControl_extractFirst(ListControl* list);

/* Extrae del final de la lista realizando una copia del canal. */
char* listControl_extractLast(ListControl* list);

/* Comprueba si la lista está vacía o no. */
int listControl_isEmpty(const ListControl* list);

/* Devuelve el tamaño de la lista. */
int listControl_size(const ListControl* list);

/* Imprime una lista devolviendo el tamanio. */
int listControl_print(const ListControl* list);

/* Comprueba si el canal esta en la lista. */
int listControl_check_element(const ListControl* list, char* ip);

/* Eliminamos el nodo que contiene el canal de la lista*/
int listControl_extractElement(ListControl* list, char* ip);

/* Devolvemos la ip del canal del nodo*/
char* getCanal(NodeControl* node);

/* Devolvemos el tiempo de llegada guardado en el nodo*/
double getTiempo(NodeControl* node);

/* Modificamos el tiempo de llegada guardado en el nodo*/
int setTiempo(NodeControl* node, double tmp);

/* Devolvemos la informacion guardada en el nodo*/
int getInfo(NodeControl* node);

/* Modificamos la informacion guardada en el nodo*/
int setInfo(NodeControl* node, int inf);

/* Devolvemos el nodo vecino */
NodeControl* getNext(NodeControl* node);

/* Devulve el nodo que tenga la ip */
NodeControl* getNode(ListControl* list, char* canal);

/* Devuelve un nodo dado una posicion */
NodeControl* getNodePos(ListControl* list, int pos);

/* Se realiza una copia de la lista */
ListControl* listControl_copy(const ListControl* list);

#endif /*LISTCONTROL_H*/