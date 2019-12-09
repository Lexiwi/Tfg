#ifndef HASH
#define HASH

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OK 0			  /* OK */
#define ERROR -1		  /* ERROR */

#define MAX_CLAVE 100	  /* Maximo del string clave */
#define MAX_HASH_RET 100  /* Tamaño maximo de nodos que caben en la tabla hash */
#define TAM_HASH  50      /* Tamaño maximo de la tabla Hash*/

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
NodoHash* crearNodoHash(char *clave, void *info);

/**
*  Inserta en la tabla hash un nodo en un indice calculado por funcionHash
* Parametro: tabla. Tabla hash
* Parametro: clave. Clave para el nodo
* Parametro: info. Informacion del nodo
* Devuelve: Devuelve OK en caso de que se borre correctamente y ERROR en caso contrario
*/
int insertarNodoHash(TablaHash *tabla, char *clave, void *info);

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
*  Devuelve la info de un nodo
* Parametro: nodo. Nodo Hash
* Devuelve: Informacion del nodo
*/
void* nodoGetInfo(NodoHash* nodo);

/**
*  Devuelve todos los nodos contenidos en la tabla hash y sus sublistas
* Parametro: Tabla hash
* Devuelve: Lista de nodos de la tabla hash
*/
NodoHash **getAllNodes(TablaHash *tabla);

/**
*  Devuelve el contenido de los nodos contenidos en la tabla hash
* Parametro: Tabla hash
* Devuelve: Lista del contenido de los nodos de la tabla hash
*/
//void **getAllNodesInfo(TablaHash *tabla);

/**
*  Devuelve el numero de nodos contenidos en la tabla hash y sus sublistas
* Parametro: Tabla hash
* Devuelve: Numero de nodos de la tabla hash
*/
int getNumNodes(TablaHash *tabla);
#endif