#include "hash.h"

/**
* @brief Estructura para un nodo hash
*/
struct _NodoHash {
	char clave[MAX_CLAVE];    	/*!< Identificador del nodo */
	char *info;     			/*!< Información del nodo */
	NodoHash *siguiente;		/*!< Nodo siguiente si hay colisión. NULL si no */
};

/**
* @brief Elimina un nodo y sus siguientes
* @param nodo Nodo a eliminar
* @return Devuelve OK en caso de que se borre correctamente y ERROR en caso contrario.
*/
int eliminarNodo(NodoHash* nodo);

/**
* @brief Elimina un nodo y sus siguientes de forma recursiva
* @param nodo Nodo a eliminar
* @return Devuelve OK en caso de que se borre correctamente y ERROR en caso contrario.
*/
int eliminarNodoRC(NodoHash* nodo);


TablaHash* crearTablaHash(int tam) {
	TablaHash* hash = NULL;
	int i;

	hash = (TablaHash*) malloc(sizeof(TablaHash));
	if (hash == NULL)
		return NULL;

	hash->tam = tam;
	hash->nodos = (NodoHash**) malloc(sizeof(NodoHash*) * tam);
	if (hash->nodos == NULL) {
		free(hash);
		return NULL;
	}

	for (i = 0; i < tam; i++)
		hash->nodos[i] = NULL;

	return hash;
}

int eliminarTablaHash(TablaHash *tabla) {
	int i;

	if (tabla == NULL)
		return ERROR;

	for (i = 0; i < tabla->tam; i++)
		eliminarNodo(tabla->nodos[i]);

	free(tabla->nodos);
	free(tabla);

	return OK;
}

int eliminarNodo(NodoHash* nodo) {
	//NodoHash* nodo_aux;

	if (nodo == NULL)
		return ERROR;

	eliminarNodoRC(nodo);

	return OK;
}

int eliminarNodoRC(NodoHash* nodo) {

	if (nodo == NULL)
		return ERROR;

	if (nodo->siguiente != NULL)
		eliminarNodoRC(nodo->siguiente);

	//if(nodo->info->nParam > 0){
	//	free(nodo->info->tipo_args);
	//}
	free(nodo->info);
	free(nodo);

	return OK;
}

int funcionHash(TablaHash* tabla, char *clave) {
	return strlen(clave) % tabla->tam;
}

NodoHash* crearNodoHash(char *clave, void *info) {
	NodoHash* nodo =  NULL;

	nodo = (NodoHash*) malloc(sizeof(NodoHash));
	if (nodo == NULL)
		return NULL;

	strcpy(nodo->clave, clave);

	nodo->info = (char*)malloc(sizeof(char)*(strlen(info)+1));
	if (nodo->info == NULL) {
		free(nodo);
		return NULL;
	}
	
	nodo->siguiente = NULL;

	return nodo;
}

int insertarNodoHash(TablaHash *tabla, char *clave, char *info) {
	NodoHash* nodo;
	NodoHash* nodo_aux;
	int pos;

	if(tabla == NULL || clave == NULL || info == NULL)
		return ERROR;

	nodo = crearNodoHash(clave, info);
	pos = funcionHash(tabla, clave);

	if (tabla->nodos[pos] != NULL) {
		nodo_aux = tabla->nodos[pos];
		while(nodo_aux->siguiente != NULL)
			nodo_aux = nodo_aux->siguiente;

		nodo_aux->siguiente = nodo;

	} else
		tabla->nodos[pos] = nodo;

	return OK;
}

void* buscarNodoHash(TablaHash *tabla, char *clave) {
	//NodoHash* nodo;
	NodoHash* nodo_aux;
	int pos;

	pos = funcionHash(tabla, clave);

	if(tabla->nodos[pos] == NULL)
		return NULL;

	for (nodo_aux = tabla->nodos[pos]; nodo_aux->siguiente != NULL; nodo_aux = nodo_aux->siguiente)
		if(strcmp(nodo_aux->clave, clave) == 0)
			break;

	//Si no encontramos el nodo, devolvemos NULL
	if(strcmp(nodo_aux->clave, clave) != 0)
		return NULL;

	return nodo_aux->info;
}

void* nodoGetInfo(NodoHash* nodo) {
	return nodo->info;
}

NodoHash **getAllNodes(TablaHash *tabla) {
    NodoHash **ret = NULL, *aux = NULL;
    int i, count = 0;
    if(tabla == NULL)
        return NULL;

    ret = (NodoHash**) malloc(sizeof(NodoHash*)*MAX_HASH_RET);
    if(ret == NULL)
        return NULL;

    memset(ret, 0, sizeof(ret));

    for(i = 0; i < tabla->tam; i++)
        ret[i] = NULL;

    /* Recorremos toda lista de nodos de la tabla hash */
    for(i = 0; i < tabla->tam; i++)
        /* Si encontramos un nodo no NULL, entonces recorremos su sublista */
        if(tabla->nodos[i] != NULL) {
            ret[count] = tabla->nodos[i];
            count++;
            aux = tabla->nodos[i]->siguiente;
            while(aux != NULL) {
                ret[count] = aux;
                count++;
                aux = aux->siguiente;
            }
        }

    return ret;
}

/*void **getAllNodesInfo(TablaHash *tabla) {
    void **ret = NULL;
    NodoHash **nodos = NULL;
    int num, i;
    if(tabla == NULL)
        return NULL;

    ret = (void**) malloc(sizeof(void*)*MAX_HASH_RET);
    if(ret == NULL)
        return NULL;

    num = getNumNodes(tabla);

    nodos = getAllNodes(tabla);
    if(nodos == NULL) {
        free(ret);
        return NULL;
    }

    for(i = 0; i < num && nodos[i] != NULL; i++)
        ret[i] = nodos[i]->info;

    while(i < MAX_HASH_RET) {
        ret[i] = NULL;
        i++;
    }

    return ret;
}*/

int getNumNodes(TablaHash *tabla) {
    NodoHash *aux = NULL;
    int i, j;
    if(tabla == NULL)
        return -1;

    /* Recorremos toda lista de nodos de la tabla hash */
    for(i = 0, j = 0; i < tabla->tam; i++)
        /* Si encontramos un nodo no NULL, entonces recorremos su sublista */
        if(tabla->nodos[i] != NULL) {
        	j++;
            aux = tabla->nodos[i]->siguiente;
            while(aux != NULL) {
            	j++;
                aux = aux->siguiente;
            }
        }

    return j;
}
