/*********************************************************** 
* File:   elelist.c
* Author: Manuel Jiménez y Marcos Asenjo
* 
* Descripción: Define el elemento del TAD lista
*
* Date: 23/03/2016
* Asignatura: Programación II
* Práctica: 3
* Ejercicio: 4
***********************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "elelist.h"

 /******************* Definicion de las estructuras de datos ******************/
struct _EleList{
    char *info;
};

/*------------------------------------------------------------------------
 * @Title: elelist_ini
 * @Description: Reserva memoria para un entero, elemento de la lista
 * @Input: Nada
 * @Output: EleList*: Puntero al elemento creado.
 ------------------------------------------------------------------------*/
EleList* elelist_ini(){
    EleList *pl=NULL;
    
    pl=(EleList *)malloc(sizeof(EleList));
    if(pl){
        pl->info=(char *)malloc(sizeof(char)*MAX_SIZE);
        if(pl->info == NULL){
            free(pl);
            pl=NULL;
        }
    }
    return pl;
}

/*------------------------------------------------------------------------
 * @Title: elelist_free
 * @Description: Libera la memoria asociada a un elemento de la lista
 * @Input: EleList* :Puntero al elemento a liberar
 * @Output: void
 ------------------------------------------------------------------------*/
void elelist_free(EleList * ele){
    if(ele){
        free(ele->info);
        free(ele);
        ele=NULL;
    }
}

/*------------------------------------------------------------------------
 * @Title: elelist_setInfo
 * @Description: Introduce información en un elemento de la lista
 * @Input: EleList*: Elemento en el que queremos introducir la información.
           void*: puntero al lugar de memoria en el que se encuentra la
           información que queremos introducir.
 * @Output: EleList *: Puntero al elemento que queremos rellenar.
 ------------------------------------------------------------------------*/
 
 EleList* elelist_setInfo(EleList * e, char* p){
    if(!e || !p)
        return NULL;
    
    //*(e->info)=*((int*)p);
    strcpy(e->info, p);
    
    return e;
 }
 
 /*------------------------------------------------------------------------
 * @Title: elelist_getInfo
 * @Description: Devuelve la información de un elemento de la lista
 * @Input: Elelist *: Puntero al elemento del que queremos conocer la información.
 * @Output: Void *: Puntero con la información del elemento.
 ------------------------------------------------------------------------*/
 char* elelist_getInfo(const EleList * e){
     if(!e)
        return NULL;
        
    return e->info;
 }
 
 /*------------------------------------------------------------------------
 * @Title: elelist_copy
 * @Description: Copia un elemento de una lista en otro
 * @Input: Elelist *:Puntero al elemento que queremos copiar
 * @Output: Elelist *: Puntero al elemento copiado.
 ------------------------------------------------------------------------*/
 EleList* elelist_copy(const EleList * src){
    EleList *dest;
    if(!src)
        return NULL;
    
    dest=elelist_ini();
    
    if(elelist_setInfo(dest, elelist_getInfo(src))==NULL){
        elelist_free(dest);
        return NULL;
    }
    return dest;
 }
 
 /*------------------------------------------------------------------------
 * @Title: elelist_cmp
 * @Description: Comprueba si dos elementos son iguales, mayores o menores
                    entre sí.
 * @Input EleList* ele1: primer elemento
 *        EleList* ele2: segundo elemento
 * @Output: int: Devuelve un número positivo, negativo o cero según si ele1
                    es mayor, menor o igual que ele2.
 ------------------------------------------------------------------------*/
 int elelist_cmp(const EleList * ele1, const EleList * ele2){
    int resultado;
    if(!ele1 || !ele2)
        return -1;
        
    //resultado = *(ele1->info) - *(ele2->info);
    resultado = strcmp(ele1->info, ele2->info);
    return resultado;
 }
 
 /*------------------------------------------------------------------------
 * @Title: elelist_print
 * @Description: Imprime un elemento en un archivo dado
 * @Input FILE *pf: Destino a imprimir
 *        EleList* ele: elemento a imprimir
 * @Output: int: Numero de caracteres impresos
 ------------------------------------------------------------------------*/
 int elelist_print(FILE* pf, const EleList * ele){
    int numCaract;
    if(!pf || !ele || !(ele->info))
        return -1;
    
    numCaract=fprintf(pf, "[%d]", *(ele->info));
    
    return numCaract;
 }