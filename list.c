/*********************************************************** 
* File:   list.c
* Author: Jorge Gutierrez Diaz
* 
* Descripción: Lista para guardar los clientes en las tablas hash.
*
***********************************************************/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include "list.h"

#define next(pnodo) (pnodo)->next
#define data(pnodo) (pnodo)->data
#define node(plista) (plista)->node

/******************* Definicion de las estructuras de datos *******************/
typedef struct _Node{
    char* data;         // Ip del cliente
    struct _Node *next;
} Node;

struct _List{
    Node *node;
};
/********************* Declaracion de funciones privadas **********************/
Node * node_ini();
void node_free(Node *node);

/********************* Definición de funciones privadas ***********************/

/************************************************************************
* @Title: node_ini
* @Description: Reserva memoria e inicializa un nodo
* @Input: Ninguno
* @Output: Node *: Puntero al nodo creado
************************************************************************/
Node *node_ini(){
    Node *node=NULL;
    
    node=(Node *)malloc(sizeof(Node));
    if(node){
        next(node)=NULL;
        data(node)=NULL;
    }
    return node;
}
 
/************************************************************************
* @Title: node_free
* @Description: Libera la memoria asociada al nodo
* @Input: Node *: Puntero al nodo a liberar
* @Output: Node *: void
*************************************************************************/
void node_free(Node *node){
    if(node){
        free(data(node));
        free(node);
    }
}

/********************* Definición de funciones públicas ***********************/

/**************************************************************************
* @Title: list_ini
* @Description: Reserva memoria e inicializa una lista
* @Input: Ninguno
* @Output: List *: Puntero a la lista creada
***************************************************************************/
List* list_ini(){
    List *list=NULL;
    
    list=(List *)malloc(sizeof(List));
    
    if(list)
        list->node=NULL;
        
    return list;
}

/**************************************************************************
* @Title: list_free
* @Description: Libera la memoria de la lista
* @Input: List *: Puntero de la lista a borrar.
* @Output: void
**************************************************************************/
 void free_list(List* list){
    if(!list)
        return;
    
    while(list_isEmpty(list)==0)
        free(list_extractFirst(list));
    free(list);
 }
 
 /***********************************************************************
* @Title: list_insertFirst
* @Description: Inserta un elemento al comienzo de la lista
* @Input: List *: Puntero a la lista a insertar
*         char *: Puntero del elemento a introducir
* @Output: List *: Puntero de la lista modificada.
************************************************************************/
 List* list_insertFirst(List* list, const char *elem){
    Node *pn;
    
    if(!list || !elem)
        return NULL;
        
    pn=node_ini();
    
    if(!pn)
        return NULL;
    
    data(pn) = (char*)malloc(sizeof(char)*(strlen(elem)+1));
    if(!data(pn)){
        node_free(pn);
        return NULL;
    }
    strcpy(data(pn), elem);
    next(pn)=node(list);
    node(list)=pn;
    
    return list;
 }
 
/************************************************************************
* @Title: list_insertLast
* @Description: Inserta un elemento al final de una lista
* @Input: List *: Puntero de la lista a modificar.
*         char *: Puntero al elemento a introducir.
* @Output: List *: Puntero a la lista modificada.
*************************************************************************/
List* list_insertLast(List* list, const char *elem){
    Node *pn, *naux;
    if(!list || !elem)
        return NULL;
    
    pn=node_ini();
    if(!pn)
        return NULL;
    
    data(pn) = (char*)malloc(sizeof(char)*(strlen(elem)+1));
    if(!data(pn)){
        node_free(pn);
        return NULL;
    }
    strcpy(data(pn), elem);
    if(list_isEmpty(list)==1){
        node(list)=pn;
        return list;
    }
    
    naux=node(list);
    while(next(naux)!=NULL){
        naux=next(naux);
    }
    next(naux)=pn;
    return list;
}

/************************************************************************
* @Title: list_extractFirst
* @Description: Extrae el primer nodo de la lista y devuleve su informacion.
* @Input: List *: Puntero de la lista.
* @Output: char*: Informacion del nodo.
*************************************************************************/
char* list_extractFirst(List* list){
    Node *pn =NULL;
    char *pe = NULL;
    
    if(!list || list_isEmpty(list)==1)
        return NULL;
    
    pn=node(list);
    pe=(char*)malloc(sizeof(char)*(strlen(data(pn))+1));
    
    if(!pe){
        return NULL;
    }
    strcpy(pe, data(pn));
    node(list)=next(pn);
    node_free(pn);
    
    return pe;
}

/************************************************************************
* @Title: list_extractLast
* @Description: Extrae el ultimo nodo de la lista y devuleve su informacion.
* @Input: List *: Puntero de la lista.
* @Output: char *: Puntero al elemento extraído
**************************************************************************/
char* list_extractLast(List* list){
    Node *pn = NULL;
    char *pe = NULL;
    
    if(!list || list_isEmpty(list))
        return NULL;
    
    /*Caso 1: Sólo un elemento en la lista*/
    if(!next(node(list))){
        pe=(char*)malloc(sizeof(char)*(strlen(data(pn))+1));
        
        if(!pe)
            return NULL;
        strcpy(pe, data(pn));
        node_free(node(list));
        node(list)=NULL;
        return pe;
    }
    
    /*Caso 2: Más de un elemento en la lista*/
    pn=node(list);
    /*Se sitúa en el penúltimo lugar*/
    while(next(next(pn))!=NULL){
        pn=next(pn);
    }
    pe=(char*)malloc(sizeof(char)*(strlen(data(pn))+1));
    if(!pe)
        return NULL;
    strcpy(pe, data(pn));
    node_free(next(pn));
    next(pn)=NULL;
    return pe;
}

/************************************************************************
* @Title: list_isEmpty
* @Description: Comprueba si una lista está vacía
* @Input: List *: Puntero a la lista a  comprobar
* @Output: int: 1 si la lista está vacía y 0 si no.
*************************************************************************/
int list_isEmpty(const List* list){
    if(!list)
        return 1;
        
    if(!node(list))
        return 1;
    return 0;
}

/************************************************************************
* @Title: list_size
* @Description: Devuelve el tamanio de una lista
* @Input: List *: Puntero a la lista a comprobar.
* @Output: int: Tamanio de la lista
*************************************************************************/
int list_size(const List* list){
    Node *pn;
    int contador=0;
    if(!list)
        return -1;
    
    if(list_isEmpty(list)==1)
        return 0;
    
    pn=node(list);
    contador++;
    
    while(next(pn)!=NULL){
        pn=next(pn);
        contador++;
    }
    return contador;
}

/*************************************************************************
* @Title: list_print
* @Description: Imprime el contenido de lista y devuelve tu tamanio
* @Input:  List *: Puntero a la lista a imprimir.
* @Output: int: Tamanio de la lista
*************************************************************************/
int list_print(const List* list){
    Node *pn;
    int contador, aux;
    if(!list)
        return -1;
    
    if(list_isEmpty(list))
        return 0;
    
    pn=node(list);
    
    while(pn!=NULL){
        fprintf(stdout, "%s", data(pn));
        contador+=(aux+1);
        pn=next(pn);
    }
    fprintf(stdout,"\n");
    return contador;
}

/*************************************************************************
* @Title: list_check_element
* @Description: Comprueba si el elemento esta en la lista
* @Input:  List*: Puntero a la lista a comprobar.
*          char*: Elemento a buscar
* @Output: int: 1 si se encuentra, 0 en caso contrario
*************************************************************************/
int list_check_element(const List* list, char* ip){
    Node *pn = NULL;
    char *pe = NULL;
    int contador, aux;

    if(!ip || !list)
        return 0;
    
    if(list_isEmpty(list))
        return 0;
    
    pn = node(list);
    
    while(pn!=NULL){

        pe = data(pn);
        if(strcmp(pe, ip) == 0){
            return 1;
        }
        pn = next(pn);
    }

    return 0;
}

/*************************************************************************
* @Title: list_extractElement
* @Description: Extrae el nodo de la lista que contenga el elemento
* @Input:  List*: Puntero a la lista.
*          char*: Elemento a buscar
* @Output: int: 1 si se extrajo, 0 en caso contrario
*************************************************************************/
int list_extractElement(List* list, char* ip){

    Node *pn =NULL;
    Node *pa =NULL;
    char *pe = NULL;
    
    if(!list || list_isEmpty(list)==1)
        return 0;
    
    pn=node(list);
    
    pe = data(pn);
    if(strcmp(pe, ip) == 0){
        pe = list_extractFirst(list);
        if(pe == NULL)
            return 0;
        free(pe);
        return 1;
    }

    pa = pn;
    while(pn != NULL){
        pe = data(pn);
        if(strcmp(pe, ip) == 0){
            if(pn->next == NULL)
                pa->next = NULL;
            else
                pa->next = pn->next;
            node_free(pn);
            return 1;
        }
        pa = pn;
        pn = next(pa);
    }
    
    return 0;
}

/*************************************************************************
* @Title: list_getClientes
* @Description: Devuelve los elementos guardados en la lista
* @Input:  List*: Puntero a la lista.
* @Output: char**: Elementos de la lista
*************************************************************************/
char** list_getClientes(const List* list){

    int i, j, size;
    char **clientes;
    Node *pn =NULL;
    char* pe = NULL;

    if(!list || list_isEmpty(list))
        return NULL;

    size = list_size(list);


    clientes = (char**)malloc(size*sizeof(char*));
    if(!clientes){
        fprintf(stderr, "Error al obtener los clientes de un nodo Hash");
        return NULL;
    }
    pn = node(list);
    
    for(i = 0; i < size; i++){

        pe = data(pn);
        clientes[i] = pe;
        pn = next(pn);
    }

    return clientes;
}

/*************************************************************************
* @Title: list_copy
* @Description: Copia la lista 1 en la lista 2
* @Input:  List*: Lista a copiar
*          List*: Nueva lista
* @Output: int: 1 si se realizo la copia, 0 en caso contrario
*************************************************************************/
int list_copy(List* list1, List* list2) {

    Node *pn =NULL;
    Node *pa =NULL;

    if(list1 == NULL || list2 == NULL)
        return 0;

    pn = node(list1);
    while(pn != NULL){
        if(list_insertFirst(list2, data(pn)) == NULL)
            return 0;
        pa = pn;
        pn = next(pa);
    }

    return 1;
}