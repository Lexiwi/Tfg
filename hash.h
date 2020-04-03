/*********************************************************** 
* File:   hash.h
* Author: Jorge Gutierrez Diaz
* 
* Descripción: Hash para guardar la informacion de cada arbol.
*
***********************************************************/

#ifndef HASH
#define HASH

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

#define OK 0			  /* OK */
#define ERROR -1		  /* ERROR */

#define MAX_CLAVE 100	  /* Maximo del string clave */
#define MAX_HASH_RET 30  /* Tamaño maximo de nodos que caben en la tabla hash */

/**
* Estructura para un nodo hash
*/
typedef struct _NodoHash NodoHash;

/**
* Estructura para una tabla hash
*/
typedef struct _TablaHash {
	int tam;    		/* Tamanio de la tabla hash */
	NodoHash** nodos; 	/* Elementos de la tabla */
} TablaHash;

/**
* Recibe un tamaño y crea una tabla de dicha longitud
* Parametros: tam Tamanio de la tabla
* Devuelve: Tabla hash creada
*/
TablaHash* crearTablaHash(int tam);

/**
* Elimina la tabla Hash
* Parametros: Tabla hash a eliminar
* Devuelve: Devuelve OK en caso de que se borre correctamente y ERROR en caso contrario
*/
int eliminarTablaHash(TablaHash *tabla);

/**
* Recibe el identificador de un nodo y devuelve su indice para la tabla hash
* Parametro: tabla. Tabla hash
* Parametro: clave. Clave del elemento
* Devuelve: Devuelve la posicion que le corresponde
*/
int funcionHash(TablaHash* tabla, char *clave);

/**
*  Recibe la clave y la informacion, y devuelve un nuevo NodoHash Reservara memoria y rellenara la estructura de NodoHash
* Parametro: clave. Clave para el nodo
* Parametro: info. Informacion a guardar en el nodo
* Devuelve: NodoHash creado
*/
NodoHash* crearNodoHash(char *clave);

/**
*  Inserta en la tabla hash un nodo en un indice calculado por funcionHash
* Parametro: tabla. Tabla hash
* Parametro: clave. Clave para el nodo
* Parametro: info. Informacion del nodo
* Devuelve: Devuelve OK en caso de que se borre correctamente y ERROR en caso contrario
*/
int insertarNodoHash(TablaHash *tabla, char *clave, void *info, double tiempo);

/**
*  Busca en la tabla hash un nodo
* Parametro: tabla. Tabla hash
* Parametro: clave. Clave del nodo
* Devuelve: Devuelve el contenido del nodo o NULL en caso de no encontrarlo
*/

NodoHash* buscarNodoHash(TablaHash *tabla, char *clave);

/**
*  Busca en la tabla hash un nodo
* Parametro: tabla. Tabla hash
* Parametro: clave. Clave del nodo
* Devuelve: Devuelve OK si encuentra el nodo o ERROR en caso de no encontrarlo
*/

int checkNodoHash(TablaHash *tabla, char *clave);


/**
*  Devuelve todos los nodos contenidos en la tabla hash y sus sublistas
* Parametro: Tabla hash
* Devuelve: Lista de nodos de la tabla hash
*/
NodoHash **getAllNodes(TablaHash *tabla);

/**
*  Devuelve el numero de nodos contenidos en la tabla hash y sus sublistas
* Parametro: Tabla hash
* Devuelve: Numero de nodos de la tabla hash
*/
int getNumNodes(TablaHash *tabla);

/**
*  Devuelve la info de un nodo
* Parametro: nodo. Nodo Hash
* Devuelve: Informacion del nodo
*/
List* nodoGetInfo(NodoHash* nodo);

/**
*  Devuelve la ip del arbol
* Parametro: nodo. Nodo Hash
* Devuelve: Ip del arbol
*/
char* getClave(NodoHash* nodo);

/**
*  Devuelve el numero de paquetes recibidos de ese arbol
* Parametro: nodo. Nodo Hash
* Devuelve: Numero de paquetes
*/
int getNumRecibidos(NodoHash* nodo);

/**
*  Actualiza el numero de paquetes recibidos de ese arbol
* Parametro: nodo. Nodo Hash
* Parametro: num. Numero de paquetes
* Devuelve: 0 si fue todo bien, -1 si no
*/
int setNumRecibidos(NodoHash* nodo, int num);

/**
*  Devuelve el numero de paquetes perdidos de ese arbol
* Parametro: nodo. Nodo Hash
* Devuelve: Numero de paquetes perdidos
*/
int getNumPerdidos(NodoHash* nodo);

/**
*  Actualiza el numero de paquetes perdidos de ese arbol
* Parametro: nodo. Nodo Hash
* Parametro: num. Numero de paquetes
* Devuelve: 0 si fue todo bien, -1 si no
*/
int setNumPerdidos(NodoHash* nodo, int num);

/**
*  Devuelve el tiempo del ultimo intervalo
* Parametro: nodo. Nodo Hash
* Devuelve: Tiempo
*/
double getRetardo(NodoHash* nodo);

/**
*  Actualiza el tiempo del ultimo intervalo
* Parametro: nodo. Nodo Hash
* Parametro: double. Tiempo
* Devuelve: 0 si fue todo bien, -1 si no
*/
int setRetardo(NodoHash* nodo, double ret);

/**
*  Devuelve el tiempo del ultimo intervalo al cuadrado
* Parametro: nodo. Nodo Hash
* Devuelve: Tiempo
*/
double getRetardoCuadrado(NodoHash* nodo);

/**
*  Actualiza el tiempo del ultimo intervalo al cuadrado
* Parametro: nodo. Nodo Hash
* Parametro: double. Tiempo
* Devuelve: 0 si fue todo bien, -1 si no
*/
int setRetardoCuadrado(NodoHash* nodo, double ret);

/**
*  Devuelve el tiempo de llegada del ultimo paquete
* Parametro: nodo. Nodo Hash
* Devuelve: Tiempo
*/
double getLlegadaAnterior(NodoHash* nodo);

/**
*  Actualiza el tiempo de llegada del ultimo paquete
* Parametro: nodo. Nodo Hash
* Parametro: double. Tiempo
* Devuelve: 0 si fue todo bien, -1 si no
*/
int setLlegadaAnterior(NodoHash* nodo, double ret);

/**
*  Devuelve el numero de bytes acumulado
* Parametro: nodo. Nodo Hash
* Devuelve: Bytes
*/
int getNumBytes(NodoHash* nodo);

/**
*  Actualiza el numero de bytes acumulado
* Parametro: nodo. Nodo Hash
* Parametro: int. Bytes
* Devuelve: 0 si fue todo bien, -1 si no
*/
int setNumBytes(NodoHash* nodo, int num);

/**
*  Devuelve el numero de anomalias igmp
* Parametro: nodo. Nodo Hash
* Devuelve: anomalias
*/
int getNumIgmpErr(NodoHash* nodo);

/**
*  Actualiza el numero de anomalias igmp
* Parametro: nodo. Nodo Hash
* Parametro: int. Anomalias
* Devuelve: 0 si fue todo bien, -1 si no
*/
int setNumIgmpErr(NodoHash* nodo, int num);

/**
*  Devuelve el numero de anomalias igmp
* Parametro: nodo. Nodo Hash
* Devuelve: anomalias
*/
int getTipo(NodoHash* nodo);

/**
*  Actualiza el numero de anomalias igmp
* Parametro: nodo. Nodo Hash
* Parametro: int. Anomalias
* Devuelve: 0 si fue todo bien, -1 si no
*/
int setTipo(NodoHash* nodo, int tipo);

/**
*  Devuelve el nodo vecino
* Parametro: nodo. Nodo Hash
* Devuelve: nodo vecinp
*/
NodoHash* getSiguiente(NodoHash *nodo);

/**
*  Imprime la tabla Hash
* Parametro: tabla. Tabla Hash
*/
void printTablaHash(TablaHash *tabla);

/**
*  Coapia la tabla Hash
* Parametro: nodo. Nodo Hash
* Devuelve: Tabla Hash copiada
*/
TablaHash* copiarTablaHash(const TablaHash *tabla1);

/**
*  Coapia nodo Hash
* Parametro: nodo. Nodo Hash
* Devuelve: Nodo Hash copiado
*/
NodoHash* copiarNodoHash(NodoHash *nodo);
#endif
