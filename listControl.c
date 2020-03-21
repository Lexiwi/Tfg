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
 
#include "listControl.h"

#define next(pnodo) (pnodo)->next
#define canal(pnodo) (pnodo)->canal
#define tiempo(pnodo) (pnodo)->tiempo
#define info(pnodo) (pnodo)->info
#define node(plista) (plista)->node

/******************* Definicion de las estructuras de datos *******************/
struct _NodeControl{
    char* canal;                    // Ip del canal
    double tiempo;                  // Ultimo tiempo de llegada
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

/************************************************************************
* @Title: nodeControl_ini
* @Description: Reserva memoria e inicializa un nodo
* @Input: Ninguno
* @Output: NodeControl *: Puntero al nodo creado
************************************************************************/
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
 
/*************************************************************************
* @Title: nodeControl_free
* @Description: Libera la memoria asociada a un nodo
* @Input: NodeControl *: Puntero al nodo a liberar
* @Output: void
*************************************************************************/
void nodeControl_free(NodeControl *node){
    if(node){
        free(canal(node));
        free(node);
    }
}

/********************* Definición de funciones públicas ***********************/
/*************************************************************************
* @Title: listControl_ini
* @Description: Reserva memoria e inicializa una lista
* @Input: Ninguno
* @Output: ListControl *: Puntero a la lista creada
*************************************************************************/
ListControl* listControl_ini(){
    ListControl *list=NULL;
    
    list=(ListControl *)malloc(sizeof(ListControl));
    
    if(list)
        list->node=NULL;
        
    return list;
}

/**************************************************************************
* @Title: listControl_free
* @Description: Libera la memoria asociada a la lista
* @Input: ListControl *: Puntero a la lista a borrar.
* @Output: void
**************************************************************************/
 void listControl_free(ListControl* list){
    if(!list)
        return;
    
    while(listControl_isEmpty(list)==0)
        free(listControl_extractFirst(list));
    free(list);
 }
 
/**************************************************************************
* @Title: listControl_insertFirst
* @Description: Inserta informacion al principio de la lista
* @Input: ListControl *: Puntero a la lista a modificar
*          char *: Ip del canal
*          double: Ultimo tiempo de recepcion
*          int: Informacion adicional 
* @Output: ListControl *: Puntero de la lista modificada.
**************************************************************************/
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
 
/**************************************************************************
* @Title: listControl_insertLast
* @Description: Inserta informacion al final de la lista
* @Input: ListControl *: Puntero a la lista a modificar
*          char *: Ip del canal
*          double: Ultimo tiempo de recepcion
*          int: Informacion adicional 
* @Output: ListControl *: Puntero de la lista modificada.
**************************************************************************/
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

/***************************************************************************
* @Title: lisControlt_extractFirst
* @Description: extrae el nodo de la primera posicion devolviendo la direccion del canal.
* @Input: ListControl *: Puntero a la lista a extraer.
* @Output: char *: direccion del canal extraido
***************************************************************************/
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

/***************************************************************************
* @Title: lisControlt_extractLast
* @Description: extrae el nodo de la ultima posicion devolviendo la direccion del canal.
* @Input: ListControl *: Puntero a la lista a extraer.
* @Output: char *: direccion del canal extraido
***************************************************************************/
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

/***************************************************************************
 * @Title: listControl_isEmpty
 * @Description: Comprueba si una lista está vacía
 * @Input: ListControl *: Puntero a la lista a  comprobar
 * @Output: int: 1 si la lista está vacía y 0 si no
***************************************************************************/
int listControl_isEmpty(const ListControl* list){
    if(!list)
        return 1;
        
    if(!node(list))
        return 1;
    return 0;
}

/***************************************************************************
* @Title: listControl_size
* @Description: Devuelve el tamaño de una lista
* @Input: ListControl *: Puntero a la lista a comprobar.
* @Output: int: Número de nodos presentes en la lista
***************************************************************************/
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

/****************************************************************************
* @Title: listControl_print
* @Description: Imprime la lista pasada como argumento
* @Input:  ListControl *: Puntero a la lista a imprimir.
* @Output: int: Número de nodos en la lista.
*****************************************************************************/
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

/****************************************************************************
* @Title: listControl_check_element
* @Description: Comprueba si un canal esta en lista.
* @Input:  ListControl *: Puntero a la lista.
*          char*: Canal a buscar.
* @Output: int: 1 si se encuentra, 0 si no.
*****************************************************************************/
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

        pe = canal(pn);
        if(strcmp(pe, ip) == 0){
            return 1;
        }
        pn = next(pn);
    }

    return 0;
}

/****************************************************************************
* @Title: listControl_extractElement
* @Description: extraemos de la lista el canal pasado por argumento.
* @Input:  ListControl *: Puntero a la lista.
*          char*: Canal a extraer.
* @Output: int: 1 si se encuentra, 0 si no.
*****************************************************************************/
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

/****************************************************************************
* @Title: getCanal
* @Description: devolvemos el nombre del canal del nodo.
* @Input:  NodeControl *: nodo.
* @Output: char*
*****************************************************************************/
char* getCanal(NodeControl* node) {

    if(node == NULL || canal(node) == NULL)
        return NULL;
    
    return canal(node);
}

/****************************************************************************
* @Title: getTiempo
* @Description: devolvemos el tiempo de recepcion del nodo.
* @Input:  NodeControl *: nodo.
* @Output: double
*****************************************************************************/
double getTiempo(NodeControl* node) {

    if(node == NULL)
        return -1;

    return tiempo(node);
}

/****************************************************************************
* @Title: setTiempo
* @Description: modificamos el tiempo de recepcion del nodo.
* @Input:  NodeControl *: nodo.
*          double: nuevo tiempo
* @Output: int: 0 si todo fue bien, -1 si no.
*****************************************************************************/
int setTiempo(NodeControl* node, double tmp) {
    if(node == NULL || tmp < 0)
        return -1;

    tiempo(node) = tmp;
    return 0;
}

/****************************************************************************
* @Title: getInfo
* @Description: devolvemos la informacion del nodo.
* @Input:  NodeControl *: nodo.
* @Output: int
*****************************************************************************/
int getInfo(NodeControl* node) {

    if(node == NULL)
        return -1;

    return info(node);
}

/****************************************************************************
* @Title: setInfo
* @Description: modificamos la informacion del nodo.
* @Input:  NodeControl *: nodo.
*          int: nueva informacion
* @Output: int: 0 si todo fue bien, -1 si no.
*****************************************************************************/
int setInfo(NodeControl* node, int inf) {
    if(node == NULL || inf < 0)
        return -1;

    info(node) = inf;
    return 0;
}

/****************************************************************************
* @Title: getNext
* @Description: devolvemos el siguiente nodo.
* @Input:  NodeControl *: nodo.
* @Output: NodeControl *: nodo vecino.
*****************************************************************************/
NodeControl* getNext(NodeControl* node) {

    if(node == NULL)
        return NULL;

    return node->next;
}

/****************************************************************************
* @Title: getNode
* @Description: devolvemos el nodo que contenga el mismo nombre
* @Input:  ListControl *: puntero a la lista.
           char*: nombre del canal a buscar.
* @Output: NodeControl *
*****************************************************************************/
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
    
    return NULL;

}

/****************************************************************************
* @Title: getNodePos
* @Description: devolvemos el nodo que se situe en una determinada posicion
* @Input:  ListControl *: puntero a la lista.
*           int: posicion del nodo a devolver.
* @Output: NodeControl *
*****************************************************************************/
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

/****************************************************************************
* @Title: listControl_copy
* @Description: copiamos la lista
* @Input:  ListControl *: puntero a la lista.
* @Output: ListControl *
*****************************************************************************/
ListControl* listControl_copy(const ListControl* list) {

    NodeControl *pn =NULL;
    NodeControl *pa =NULL;
    ListControl* aux = NULL;

    if(list == NULL)
        return NULL;

    
    aux = listControl_ini();
    if(aux == NULL)
        return NULL;

    
    pn = node(list);
    while(pn != NULL) {

        if((aux = listControl_insertLast(aux, canal(pn), tiempo(pn), info(pn))) == NULL) {
            fprintf(stderr, "Error al copiar en ListControl_copy");
            return NULL;
        }
        pa = pn;
        pn = next(pa);
            
    }
    return aux;
}