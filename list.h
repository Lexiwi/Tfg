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


#ifndef LIST_H
#define LIST_H


/**********************  Nuevo tipo de datos para Lista ***********************/
typedef struct _List List;

/********************* Declaracion de funciones públicas **********************/
/* Inicializa la lista reservando memoria e inicializa todos sus elementos. */
List* list_ini();

/* Libera la lista y todos sus elementos. */
void free_list(List* list);

/* Inserta al principio de la lista realizando una copia del elemento. */
List* list_insertFirst(List* list, const char *elem);

/* Inserta al final de la lista realizando una copia del elemento. */
List* list_insertLast(List* list, const char *elem);

/* Extrae del principio de la lista realizando una copia del elemento almacenado en dicho nodo. */
char* list_extractFirst(List* list);

/* Extrae del final de la lista realizando una copia del elemento almacenado en dicho nodo. */
char* list_extractLast(List* list);

/* Comprueba si una lista está vacía o no. */
int list_isEmpty(const List* list);

/* Devuelve el tamaño de una lista. */
int list_size(const List* list);

/* Imprime una lista devolviendo el número de caracteres escritos. */
int list_print(const List* list);

/* Comprueba si el elemento esta en la lista. */
int list_check_element(const List* list, char* ip);

/* Eliminamos un elemento de la lista*/
int list_extractElement(List* list, char* ip);

char** list_getClientes(const List* list);

/* Copiamos la lista 1 en la lista 2*/
int list_copy(List* list1, List* list2);

#endif /*LIST_H*/