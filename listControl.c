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
#include <string.h>
 
#include "listControl.h"

#define next(pnodo) (pnodo)->next
#define canal(pnodo) (pnodo)->canal
#define tiempo(pnodo) (pnodo)->tiempo
#define info(pnodo) (pnodo)->info
#define node(plista) (plista)->node

/******************* Definicion de las estructuras de datos *******************/
struct _NodeControl{
    char* canal;
    double tiempo;
    int info;
    struct _NodeControl *next;
};

struct _ListControl{
    NodeControl *node;
};
/********************* Declaracion de funciones privadas **********************/
NodeControl * nodeControl_ini();
void nodeControl_free(NodeControl *node);

/********************* Definición de funciones privadas ***********************/
/*------------------------------------------------------------------------
 * @Title: nodeControl_ini
 * @Description: Reserva memoria e inicializa un nodo
 * @Input: Ninguno
 * @Output: Node *: Puntero al nodo creado
------------------------------------------------------------------------*/
NodeControl *nodeControl_ini(){
    NodeControl *node=NULL;
    
    node=(NodeControl *)malloc(sizeof(NodeControl));
    if(node){
        next(node)=NULL;
        canal(node)=NULL;
        tiempo(node)=0.0;
        info(node)=0;
    }
    return node;
}
 
/*------------------------------------------------------------------------
 * @Title: nodeControl_free
 * @Description: Libera la memoria asociada a un nodo
 * @Input: Node *: Puntero al nodo a liberar
 * @Output: Node *: void
------------------------------------------------------------------------*/
void nodeControl_free(NodeControl *node){
    if(node){
        free(canal(node));
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
ListControl* listControl_ini(){
    ListControl *list=NULL;
    
    list=(ListControl *)malloc(sizeof(ListControl));
    
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
 void listControl_free(ListControl* list){
    if(!list)
        return;
    
    while(listControl_isEmpty(list)==0)
        free(listControl_extractFirst(list));
    free(list);
 }
 
 /*------------------------------------------------------------------------
 * @Title: list_insertFirst
 * @Description: Inserta un elemento al comienzo de la lista
 * @Input: List *: Puntero a la lista a modificar
           EleList *:Puntero del elemento a introducir
 * @Output: List *: Puntero de la lista modificada.
 ------------------------------------------------------------------------*/
 ListControl* listControl_insertFirst(ListControl* list, const char *canal, const double tiempo, const int info){
    NodeControl *pn;
    
    if(!list || !canal)
        return NULL;
        
    pn=nodeControl_ini();
    
    if(!pn)
        return NULL;
    
    canal(pn) = (char*)malloc(sizeof(char)*(strlen(canal)+1));
    if(!canal(pn)){
        nodeControl_free(pn);
        return NULL;
    }
    strcpy(canal(pn), canal);
    tiempo(pn) = tiempo;
    info(pn) = info;
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
ListControl* listControl_insertLast(ListControl* list, const char *canal, const double tiempo, const int info){
    NodeControl *pn, *naux;
    if(!list || !canal)
        return NULL;
    
    pn=nodeControl_ini();
    if(!pn)
        return NULL;
    
    canal(pn) = (char*)malloc(sizeof(char)*(strlen(canal)+1));
    if(!canal(pn)){
        nodeControl_free(pn);
        return NULL;
    }
    strcpy(canal(pn), canal);
    tiempo(pn) = tiempo;
    info(pn) = info;
    if(listControl_isEmpty(list)==1){
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
 * @Title: list_extractFirst
 * @Description: Inserta un elemento en orden copiando el elemento.
 * @Input: List *: Puntero a la lista a modificar.
           EleList *: Puntero al elemento a introducir.
 * @Output: List *: Puntero a la lista modificada.
------------------------------------------------------------------------*/
char* listControl_extractFirst(ListControl* list){
    NodeControl *pn =NULL;
    char *pe = NULL;
    
    if(!list || listControl_isEmpty(list)==1)
        return NULL;
    
    pn=node(list);
    pe=(char*)malloc(sizeof(char)*(strlen(canal(pn))+1));
    
    if(!pe){
        return NULL;
    }
    strcpy(pe, canal(pn));
    node(list)=next(pn);
    nodeControl_free(pn);
    
    return pe;
}

/*------------------------------------------------------------------------
 * @Title: list_extractLast
 * @Description: Extrae un elemento del final de una lista
 * @Input: List *: Puntero a la lista a extraer.
 * @Output: EleList *: Puntero al elemento extraído
------------------------------------------------------------------------*/
char* listControl_extractLast(ListControl* list){
    NodeControl *pn = NULL;
    char *pe = NULL;
    
    if(!list || listControl_isEmpty(list))
        return NULL;
    
    /*Caso 1: Sólo un elemento en la lista*/
    if(!next(node(list))){
        pe=(char*)malloc(sizeof(char)*(strlen(canal(pn))+1));
        
        if(!pe)
            return NULL;
        strcpy(pe, canal(pn));
        nodeControl_free(node(list));
        node(list)=NULL;
        return pe;
    }
    
    /*Caso 2: Más de un elemento en la lista*/
    pn=node(list);
    /*Se sitúa en el penúltimo lugar*/
    while(next(next(pn))!=NULL){
        pn=next(pn);
    }
    pe=(char*)malloc(sizeof(char)*(strlen(canal(pn))+1));
    if(!pe)
        return NULL;
    strcpy(pe, canal(pn));
    nodeControl_free(next(pn));
    next(pn)=NULL;
    return pe;
}

/*------------------------------------------------------------------------
 * @Title: list_isEmpty
 * @Description: Comprueba si una lista está vacía
 * @Input: List *: Puntero a la lista a  comprobar
 * @Output: Bool: TRUE si la lista está vacía y FALSE si no;
------------------------------------------------------------------------*/
int listControl_isEmpty(const ListControl* list){
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
int listControl_size(const ListControl* list){
    NodeControl *pn;
    int contador=0;
    if(!list)
        return -1;
    
    if(listControl_isEmpty(list)==1)
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
int listControl_print(const ListControl* list){
    NodeControl *pn;
    int contador, aux;
    if(!list)
        return -1;
    
    fprintf(stdout, "\nLista con %d elementos:\n", listControl_size(list));
    if(listControl_isEmpty(list))
        return 0;
    
    pn=node(list);
    
    while(pn!=NULL){
        fprintf(stdout, "%s", canal(pn));
        fprintf(stdout, " %f", tiempo(pn));
        fprintf(stdout, " %d", info(pn));
        fprintf(stdout,"\n");
        contador+=(aux+1);
        pn=next(pn);
    }
    return contador;
}

int listControl_check_element(const ListControl* list, char* ip){
    NodeControl *pn = NULL;
    char *pe = NULL;
    int contador, aux;

    if(!ip || !list)
        return 0;
    
    if(listControl_isEmpty(list))
        return 0;
    
    pn = node(list);
    
    while(pn!=NULL){

        //if(strcmp(pn->data, ip) == 0){
        //    return 1;
        //}
        pe = canal(pn);
        if(strcmp(pe, ip) == 0){
            return 1;
        }
        pn = next(pn);
    }

    return 0;
}

int listControl_extractElement(ListControl* list, char* ip){

    NodeControl *pn =NULL;
    NodeControl *pa =NULL;
    char *pe = NULL;
    
    if(!list || listControl_isEmpty(list)==1)
        return 0;
    
    pn=node(list);
    
    pe = canal(pn);
    if(strcmp(pe, ip) == 0){
        pe = listControl_extractFirst(list);
        if(pe == NULL)
            return 0;
        free(pe);
        return 1;
    }

    pa = pn;
    while(pn != NULL){
        pe = canal(pn);
        if(strcmp(pe, ip) == 0){
            if(pn->next == NULL)
                pa->next = NULL;
            else
                pa->next = pn->next;
            nodeControl_free(pn);
            return 1;
        }
        pa = pn;
        pn = next(pa);
    }
    
    return 0;
}

char* getCanal(NodeControl* node) {

    if(node == NULL || canal(node) == NULL)
        return NULL;
    
    return canal(node);
}

double getTiempo(NodeControl* node) {

    if(node == NULL)
        return -1;

    return tiempo(node);
}

int setTiempo(NodeControl* node, double tmp) {
    if(node == NULL || tmp < 0)
        return -1;

    tiempo(node) = tmp;
    return 0;
}

int getInfo(NodeControl* node) {

    if(node == NULL)
        return -1;

    return info(node);
}

int setInfo(NodeControl* node, int inf) {
    if(node == NULL || inf < 0)
        return -1;

    info(node) = inf;
    return 0;
}

NodeControl* getNode(ListControl* list, char* canal) {

    NodeControl *pn =NULL;
    NodeControl *pa =NULL;
    char *pe = NULL;
    
    if(!list || listControl_isEmpty(list)==1)
        return NULL;
    
    pn=node(list);
    
    pe = canal(pn);
    if(strcmp(pe, canal) == 0){
        return pn;
    }

    pa = pn;
    while(pn != NULL){
        pe = canal(pn);
        if(strcmp(pe, canal) == 0){
            return pn;
        }
        pa = pn;
        pn = next(pa);
    }
    
    return 0;

}

NodeControl* getNodePos(ListControl* list, int pos) {

    NodeControl *pn =NULL;
    NodeControl *pa =NULL;
    int i = 0;

    if(!list || listControl_isEmpty(list)==1)
        return NULL;
    
    pn=node(list);

    if(pos == 0)
        return pn;

    while(i < pos) {

        pa = pn;
        pn = next(pa);
        if(pn == NULL)
            return NULL;
        i++;
    }
    
    return pn;
}