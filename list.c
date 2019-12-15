/*********************************************************** 
* File:   list.c
* Author: Manuel Jiménez y Marcos Asenjo
* 
* Descripción: Define el TAD lista
*
* Date: 22/03/2016
* Asignatura: Programación II
* Práctica: 3
* Ejercicio: 4
***********************************************************/
 
#include <stdio.h>
#include <stdlib.h>
 
#include "list.h"

#define next(pnodo) (pnodo)->next
#define data(pnodo) (pnodo)->data
#define node(plista) (plista)->node

/******************* Definicion de las estructuras de datos *******************/
typedef struct _Node{
    EleList* data;
    struct _Node *next;
} Node;

struct _List{
    Node *node;
};
/********************* Declaracion de funciones privadas **********************/
Node * node_ini();
void node_free(Node *node);

/********************* Definición de funciones privadas ***********************/
/*------------------------------------------------------------------------
 * @Title: node_ini
 * @Description: Reserva memoria e inicializa un nodo
 * @Input: Ninguno
 * @Output: Node *: Puntero al nodo creado
------------------------------------------------------------------------*/
Node *node_ini(){
    Node *node=NULL;
    
    node=(Node *)malloc(sizeof(Node));
    if(node){
        next(node)=NULL;
        data(node)=NULL;
    }
    return node;
}
 
/*------------------------------------------------------------------------
 * @Title: node_free
 * @Description: Libera la memoria asociada a un nodo
 * @Input: Node *: Puntero al nodo a liberar
 * @Output: Node *: void
------------------------------------------------------------------------*/
void node_free(Node *node){
    if(node){
        elelist_free(data(node));
        free(node);
    }
}

/********************* Definición de funciones públicas ***********************/
/*------------------------------------------------------------------------
 * @Title: list_ini
 * @Description: Reserva memoria e inicializa una lista
 * @Input: Ninguno
 * @Output: List *: Puntero a la lista creada
 ------------------------------------------------------------------------*/
List* list_ini(){
    List *list=NULL;
    
    list=(List *)malloc(sizeof(List));
    
    if(list)
        list->node=NULL;
        
    return list;
}

/*------------------------------------------------------------------------
 * @Title: list_free
 * @Description: Libera la memoria asociada a un laberinto
 * @Input: List *: Puntero a la lista a borrar.
 * @Output: void
 ------------------------------------------------------------------------*/
 void list_free(List* list){
    if(!list)
        return;
    
    while(list_isEmpty(list)==0)
        elelist_free(list_extractFirst(list));
    free(list);
 }
 
 /*------------------------------------------------------------------------
 * @Title: list_insertFirst
 * @Description: Inserta un elemento al comienzo de la lista
 * @Input: List *: Puntero a la lista a modificar
           EleList *:Puntero del elemento a introducir
 * @Output: List *: Puntero de la lista modificada.
 ------------------------------------------------------------------------*/
 List* list_insertFirst(List* list, const EleList *elem){
    Node *pn;
    
    if(!list || !elem)
        return NULL;
        
    pn=node_ini();
    
    if(!pn)
        return NULL;
    
   
    data(pn)=elelist_copy(elem);
    if(!data(pn)){
        node_free(pn);
        return NULL;
    }
    
    next(pn)=node(list);
    node(list)=pn;
    
    return list;
 }
 
/*------------------------------------------------------------------------
 * @Title: list_insertLast
 * @Description: Inserta un elemento al final de una lista
 * @Input: List *: Puntero a la lista a modificar.
           EleList *: Puntero al elemento a introducir.
 * @Output: List *: Puntero a la lista modificada.
------------------------------------------------------------------------*/
List* list_insertLast(List* list, const EleList *elem){
    Node *pn, *naux;
    if(!list || !elem)
        return NULL;
    
    pn=node_ini();
    if(!pn)
        return NULL;
        
    data(pn)=elelist_copy(elem);
    if(!data(pn)){
        node_free(pn);
        return NULL;
    }
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

/*------------------------------------------------------------------------
 * @Title: list_insertInOrder
 * @Description: Inserta un elemento en orden copiando el elemento.
 * @Input: List *: Puntero a la lista a modificar.
           EleList *: Puntero al elemento a introducir.
 * @Output: List *: Puntero a la lista modificada.
------------------------------------------------------------------------*/
List* list_insertInOrder(List *list, const EleList *pElem){
    Node *pn, *naux;
    if(!list || !pElem)
        return NULL;
    
    pn=node_ini();
    
    if(!pn)
        return NULL;
        
    data(pn)=elelist_copy(pElem);
    
    if(!data(pn)){
        node_free(pn);
        return NULL;
    }
    
    /*Caso 1: Lista vacía*/
    if(list_isEmpty(list)==1){
        node(list)=pn;
        return list;
    }
    
    /*Caso 2: El elemento a introducir es menor que el primer elemento de la lista*/
   
    if(*((int*)elelist_getInfo(pElem)) < *((int *)elelist_getInfo(data(node(list))))){
        next(pn)=node(list);
        node(list)=pn;
        return list;
    }
    
    /*Caso 3: El elemento no se introduce al inicio*/
    naux=node(list);
    /*A través de un bucle se ve cuándo es menor que el siguiente
      y se introduce justo antes*/
    while(next(naux)!=NULL){
        if(*((int*)elelist_getInfo(pElem)) < *((int *)elelist_getInfo(data(next(naux))))){
            next(pn)=next(naux);
            next(naux)=pn;
            return list;
        }
        else{
            /*Si no es menor se salta a la siguiente posición*/
            naux=next(naux);
        }
    }
    /*Si no es menor que ninguno se pone al final de la lista*/
    next(naux)=pn;
    return list;
}

/*------------------------------------------------------------------------
 * @Title: list_extractFirst
 * @Description: Inserta un elemento en orden copiando el elemento.
 * @Input: List *: Puntero a la lista a modificar.
           EleList *: Puntero al elemento a introducir.
 * @Output: List *: Puntero a la lista modificada.
------------------------------------------------------------------------*/
EleList* list_extractFirst(List* list){
    Node *pn =NULL;
    EleList *pe = NULL;
    
    if(!list || list_isEmpty(list)==1)
        return NULL;
    
    pn=node(list);
    pe=elelist_copy(data(pn));
    
    if(!pe){
        return NULL;
    }
    node(list)=next(pn);
    node_free(pn);
    
    return pe;
}

/*------------------------------------------------------------------------
 * @Title: list_extractLast
 * @Description: Extrae un elemento del final de una lista
 * @Input: List *: Puntero a la lista a extraer.
 * @Output: EleList *: Puntero al elemento extraído
------------------------------------------------------------------------*/
EleList* list_extractLast(List* list){
    Node *pn;
    EleList *pe;
    
    if(!list || list_isEmpty(list))
        return NULL;
    
    /*Caso 1: Sólo un elemento en la lista*/
    if(!next(node(list))){
        pe=elelist_copy(data(node(list)));
        if(!pe)
            return NULL;
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
    pe=elelist_copy(data(next(pn)));
    if(!pe)
        return NULL;
    node_free(next(pn));
    next(pn)=NULL;
    return pe;
}

/*------------------------------------------------------------------------
 * @Title: list_isEmpty
 * @Description: Comprueba si una lista está vacía
 * @Input: List *: Puntero a la lista a  comprobar
 * @Output: Bool: TRUE si la lista está vacía y FALSE si no;
------------------------------------------------------------------------*/
int list_isEmpty(const List* list){
    if(!list)
        return 1;
        
    if(!node(list))
        return 1;
    return 0;
}

/*------------------------------------------------------------------------
 * @Title: list_size
 * @Description: Devuelve el tamaño de una lista
 * @Input: List *: Puntero a la lista a comprobar.
 * @Output: int: Número de elementos presentes en la lista
------------------------------------------------------------------------*/
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

/*------------------------------------------------------------------------
 * @Title: list_print
 * @Description: Imprime la lista pasada como argumento
 * @Input:  FILE *: Puntero al archivo en el que se va a imprimir la lista.
            List *: Puntero a la lista a imprimir.
 * @Output: int: Número de caracteres impresos.
------------------------------------------------------------------------*/
int list_print(FILE *fd, const List* list){
    Node *pn;
    int contador, aux;
    if(!fd || !list)
        return -1;
    
    fprintf(fd, "\nLista con %d elementos:\n", list_size(list));
    if(list_isEmpty(list))
        return 0;
    
    pn=node(list);
    
    while(pn!=NULL){
        aux=elelist_print(fd, data(pn));
        fprintf(fd,"\n");
        contador+=(aux+1);
        pn=next(pn);
    }
    return contador;
}

int list_check_element(const List* list, char* ip){
    Node *pn;
    int contador, aux;

    if(!ip || !list)
        return -1;
    
    if(list_isEmpty(list))
        return -1;
    
    pn=node(list);
    
    while(pn!=NULL){

        if(strcmp(pn->info, ip) == 0){
            return 1;
        }
        pn=next(pn);
    }

    return 0;
}

int list_extractElement(List* list, char* ip){

    Node *pn =NULL;
    Node *pa =NULL;
    EleList *pe = NULL;
    
    if(!list || list_isEmpty(list)==1)
        return NULL;
    
    pn=node(list);
    
    pe = data(pn);
    if(strcmp(pe->info, ip) == 0){
        pe = list_extractFirst(list);
        if(pe == NULL)
            return 0;
        free(pe);
        return 1;
    }

    pa = pn
    while(pn != NULL){
        pe = data(pn);
        if(strcmp(pe->info, ip) == 0){
            if(pn->next == NULL)
                pa->next = NULL;
            else
                pa->next = pn->next;
            node_free(pn);
            return 1;
        }
        pa = pn
        pn = next(pa)
    }
    
    return 1;
}