#include "hash.h"

/**
* @brief Estructura para un nodo hash
*/
struct _NodoHash {
	char clave[MAX_CLAVE];    	/*!< Identificador del nodo */
	List *lista;   				/*!< Lista de clientes */
	int paqRec;					/*!< Num. paquetes recibidos */
	int paqPer;					/*!< Num. paquetes perdidos */
	double ret;					/*!< Retardo total */
	double retCuadrado;			/*!< Retardo al cuadrado total */
	double retAnterior;			/*!< Retardo del paquete anterior */
	int bytes;					/*!< Num. bytes total */
	int igmpErr;				/*!< NUm. errores IGMP */
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
	free_list(nodo->lista);
	free(nodo);

	return OK;
}

int funcionHash(TablaHash* tabla, char *clave) {
	return strlen(clave) % tabla->tam;
}

//NodoHash* crearNodoHash(char *clave, void *info) {
NodoHash* crearNodoHash(char *clave) {
	NodoHash* nodo =  NULL;

	nodo = (NodoHash*) malloc(sizeof(NodoHash));
	if (nodo == NULL)
		return NULL;

	strcpy(nodo->clave, clave);

	nodo->lista = list_ini();
	if(nodo->lista == NULL)
		return NULL;

	nodo->paqRec = 0;					
	nodo->paqPer = 0;
	nodo->ret = 0.0;
	nodo->retCuadrado = 0.0;
	nodo->retAnterior = 0.0;
	nodo->bytes = 0;
	nodo->igmpErr = 0;

	nodo->siguiente = NULL;

	return nodo;
}

int insertarNodoHash(TablaHash *tabla, char *clave, void *info) {
	NodoHash* nodo;
	NodoHash* nodo_aux;
	int pos;

	if(tabla == NULL || clave == NULL || info == NULL)
		return ERROR;

	//nodo = crearNodoHash(clave, info);
	nodo = crearNodoHash(clave);
	list_insertFirst(nodo->lista, (char*)info);
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

NodoHash* buscarNodoHash(TablaHash *tabla, char *clave) {
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

	return nodo_aux;
}

int checkNodoHash(TablaHash *tabla, char *clave) {
	//NodoHash* nodo;
	NodoHash* nodo_aux;
	int pos;

	pos = funcionHash(tabla, clave);

	if(tabla->nodos[pos] == NULL)
		return -1;

	for (nodo_aux = tabla->nodos[pos]; nodo_aux->siguiente != NULL; nodo_aux = nodo_aux->siguiente)
		if(strcmp(nodo_aux->clave, clave) == 0)
			break;

	//Si no encontramos el nodo, devolvemos NULL
	if(strcmp(nodo_aux->clave, clave) != 0)
		return ERROR;

	return OK;
}

NodoHash **getAllNodes(TablaHash *tabla) {
    NodoHash **ret = NULL, *aux = NULL;
    int i, count = 0;
    if(tabla == NULL)
        return NULL;

    ret = (NodoHash**) malloc(sizeof(NodoHash*)*MAX_HASH_RET);
    if(ret == NULL)
        return NULL;

    memset(ret, 0, sizeof(**ret));

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


List* nodoGetInfo(NodoHash* nodo) {
	return nodo->lista;
}

char* getClave(NodoHash* nodo) {
	
	if(nodo == NULL)
		return NULL;

	return nodo->clave;
}

int getNumRecibidos(NodoHash* nodo) {

	if (nodo == NULL)
		return -1;

	return nodo->paqRec;
}

int setNumRecibidos(NodoHash* nodo, int num) {
	
	if (nodo == NULL)
		return -1;

	nodo->paqRec = nodo->paqRec + num;
	return 0;
}

int getNumPerdidos(NodoHash* nodo) {

	if (nodo == NULL)
		return -1;

	return nodo->paqPer;
}

int setNumPerdidos(NodoHash* nodo, int num) {
	
	if (nodo == NULL)
		return -1;

	nodo->paqPer = nodo->paqPer + num;
	return 0;
}

double getRetardo(NodoHash* nodo) {

	if (nodo == NULL)
		return -1;
	
	return nodo->ret;
}

int setRetardo(NodoHash* nodo, double ret) {
	
	if (nodo == NULL)
		return -1;

	//nodo->ret = nodo->ret + ret;
	nodo->ret = ret;
	return 0;
}

double getRetardoCuadrado(NodoHash* nodo) {

	if (nodo == NULL)
		return -1;

	return nodo->retCuadrado;
}

int setRetardoCuadrado(NodoHash* nodo, double ret) {
	
	if (nodo == NULL)
		return -1;
	
	//nodo->retCuadrado = nodo->retCuadrado + ret;
	nodo->retCuadrado = ret;
	return 0;
}

double getLlegadaAnterior(NodoHash* nodo) {

	if (nodo == NULL)
		return -1;

    return nodo->retAnterior;
}

int setLlegadaAnterior(NodoHash* nodo, double ret) {
	
	if (nodo == NULL)
		return -1;

    nodo->retAnterior = ret;
	return 0;
}

int getNumBytes(NodoHash* nodo) {

	if (nodo == NULL)
		return -1;

    return nodo->bytes;
}

int setNumBytes(NodoHash* nodo, int num) {
	
	if (nodo == NULL)
		return -1;

    nodo->bytes = nodo->bytes + num;
    return 0;
}

int getNumIgmpErr(NodoHash* nodo) {

	if (nodo == NULL)
		return -1;

    return nodo->igmpErr;
}

int setNumIgmpErr(NodoHash* nodo, int num) {
	
	if (nodo == NULL)
		return -1;

    nodo->igmpErr = nodo->igmpErr + num;
    return 0;
}

NodoHash* getSiguiente(NodoHash *nodo) {

	if (nodo == NULL)
		return NULL;

	return nodo->siguiente;
}

void printTablaHash(TablaHash *tabla) {

	NodoHash* aux = NULL;
	int i = 0;

	if(tabla == NULL)
		return;

	for(i = 0; i < tabla->tam; i++) {

		if(tabla->nodos[i] != NULL){

			aux = tabla->nodos[i];
			while(aux != NULL){
			//for (aux = tabla->nodos[i]; aux->siguiente != NULL; aux = aux->siguiente) {

				fprintf(stdout, "******** %s ********\n\n", aux->clave);
				fprintf(stdout, "Clientes:");
				list_print(aux->lista);
				fprintf(stdout, "Paquetes recibidos: %d\n", aux->paqRec);
				fprintf(stdout, "Paquetes perdidos: %d\n", aux->paqPer);
				fprintf(stdout, "Retardo total: %f\n", aux->ret);
				fprintf(stdout, "Retardo cuadrado: %f\n", aux->retCuadrado);
				fprintf(stdout, "Bytes totales: %d\n", aux->bytes);
				fprintf(stdout, "Errores IGMP totales: %d\n", aux->igmpErr);
				fprintf(stdout, "\n\n\n");

				aux = aux->siguiente;
			}
		}

	}

	return;

}

TablaHash* copiarTablaHash(const TablaHash *tabla1) {

	int i;
	TablaHash* tabla2 = NULL;

	if(tabla1 == NULL)
		return NULL;

	tabla2 = crearTablaHash(tabla1->tam);
	if(tabla2 == NULL)
		return NULL;

	for(i = 0; i < tabla1->tam; i++){

		tabla2->nodos[i] = copiarNodoHash(tabla1->nodos[i]);
	}

	return tabla2;
}

NodoHash* copiarNodoHash(NodoHash *nodo) {

	char* clave = NULL;
	NodoHash *aux = NULL;

	if( nodo == NULL )
		return NULL;

	clave = getClave(nodo);
	if(clave == NULL)
		return NULL;

	aux = crearNodoHash(clave);
	if(aux == NULL)
		return NULL;
	
	if(list_copy(nodoGetInfo(nodo), nodoGetInfo(aux)) == 0)
		return NULL;

	aux->paqRec = nodo->paqRec;					
	aux->paqPer = nodo->paqPer;
	aux->ret = nodo->ret;
	aux->retCuadrado = nodo->retCuadrado;
	aux->retAnterior = nodo->retAnterior;
	aux->bytes = nodo->bytes;
	aux->igmpErr = nodo->igmpErr;
	aux->siguiente = copiarNodoHash(nodo->siguiente);
	
	return aux;
}
