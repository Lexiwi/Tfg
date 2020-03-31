/*********************************************************** 
* File:   bdSql.c
* Author: Jorge Gutierrez Diaz
* 
* Descripción: Modulo para operaciones con la base de datos
*
***********************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "bdSql.h"

/********************* Declaracion de funciones privadas **********************/
double calculaVarianza(int numPaq, int numPaqQuery, double ret, double retQuery, double retC, double retCQuery);
double calculaMOSS(double tasaPerdidas);

/********************* Definición de funciones privadas ***********************/

/************************************************************************
* @Title: conectaDB
* @Description: Nos conectamos a la base de datos
* @Input: Ninguno
* @Output: MYSQL *: Puntero a la base de datos
************************************************************************/
MYSQL* conectaDB() {

    MYSQL *db = NULL;
    db = mysql_init(NULL);

    if (!mysql_real_connect(db, NULL, USUARIO, PASS, TABLA, 0, NULL, 0)) {
        fprintf(stderr, "Error al abrir la base de datos: %s\n", mysql_error(db));
        return NULL;
    }

    return db;

}

/************************************************************************
* @Title: reseteaDB
* @Description: Limpiamos la base de datos
* @Input: MYSQL *: Puntero a la base de datos
* @Output: int: 0 si salio todo bien, -1 si no.
************************************************************************/
int reseteaDB(MYSQL *db) {

    int rc = 0;

    if(db == NULL) {
        fprintf(stderr, "Controlador de la base de datos a NULL");
        return -1;
    }

    rc = mysql_query(db, DROP_TABLES);
    if (rc) {
        fprintf(stderr, "SQL error eliminando tablas: %s\n", mysql_error(db));  
        //mysql_close(db);
        return -1;
    }

    rc = mysql_query(db, CREATE_TABLE_CANALES);
    if (rc) {
        fprintf(stderr, "SQL error creando tabla canales: %s\n", mysql_error(db));  
        //mysql_close(db);
        return -1;
    }

    rc = mysql_query(db, CREATE_TABLE_IGMP);
    if (rc) {
        fprintf(stderr, "SQL error creando tabla igmp: %s\n", mysql_error(db));  
        //mysql_close(db);
        return -1;
    }

    rc = mysql_query(db, CREATE_TABLE_RUIDO);
    if (rc) {
        fprintf(stderr, "SQL error creando tabla ruido: %s\n", mysql_error(db));  
        //mysql_close(db);
        return -1;
    }

    return 0;
}

/************************************************************************
* @Title: volcarTabla
* @Description: Volcamos la informacion a la base de datos
* @Input: MYSQL *: Puntero a la base de datos
          TablaHash*: Tabla hash
          ListControl*: Tabla con la informacion IGMP almacenada
          Ruido*: Ruido udp almacenado 
* @Output: void
************************************************************************/
void volcarTabla(MYSQL *db, TablaHash* tabla, ListControl* igmp, Ruido* ruido) {

    int i, j, size;
    int rc = 0;
    int numPaqH = 0, numPerH = 0, numErrH = 0,bytesH = 0;
    int numPaqQuery = 0,numPerQuery = 0,numErrQuery = 0,bytesQuery = 0;
    double retQuery, retCQuery, var = 0.0; 
    double retH = 0.0;
    double porH, porQ = 1.0;
    double moss = 0.0;
    char *err_msg = 0;
    char *eptr;
    char sql[350], query[150];
    char *canal = NULL;
    char **clientes = NULL;

    NodoHash* aux = NULL;
    NodeControl *pn = NULL;
    const List* lista = NULL;

    MYSQL_RES *result = NULL;   //Variable para guardar resultado de la query
    MYSQL_ROW row = NULL;       //Variable para recorrer las filas del resultado      

    if(tabla == NULL || igmp == NULL || ruido == NULL)
        return;

    for(i = 0; i < tabla->tam; i++) {

		if(tabla->nodos[i] != NULL){

			aux = tabla->nodos[i];
			while(aux != NULL){

                sprintf(query, "SELECT Ret,RetC,NumPaq,NumPer,NumErr,Bytes FROM Canales WHERE Ip=\'%s\' ORDER BY NumPaq DESC LIMIT 1", getClave(aux));
                rc = mysql_query(db, query);
                if (rc != 0 ) {
                    fprintf(stderr, "SQL error: %s\n", mysql_error(db));       
                    return;
                }
                result = mysql_store_result(db);
                if (!result) {
                    fprintf(stderr, "SQL error: %s\n", mysql_error(db));
                    mysql_free_result(result);   
                    return;
                }
                row = mysql_fetch_row(result);
                if(!row){
                    retQuery = 0.0;
                    retCQuery = 0.0;
                    numPaqQuery = 0;
                    numPerQuery = 0;
                    numErrQuery = 0;
                    bytesQuery = 0;
                    var = calculaVarianza(getNumRecibidos(aux), 0, getRetardo(aux), 0.0, getRetardoCuadrado(aux), 0.0);
                }
                else {
                    retQuery = strtod(row[0], &eptr);
                    retCQuery = strtod(row[1], &eptr);
                    numPaqQuery = atoi(row[2]);
                    numPerQuery = atoi(row[3]);
                    numErrQuery = atoi(row[4]);
                    bytesQuery = atoi(row[5]);
                    var = calculaVarianza(getNumRecibidos(aux), numPaqQuery, getRetardo(aux), retQuery, getRetardoCuadrado(aux), retCQuery);
                }
                retH = getRetardo(aux);
                numPaqH = getNumRecibidos(aux);
                numPerH = getNumPerdidos(aux);
                numErrH = getNumIgmpErr(aux);
                bytesH = getNumBytes(aux);

                numPaqQuery = numPaqH-numPaqQuery;
                numPerQuery = numPerH-numPerQuery;
                numErrQuery = numErrH-numErrQuery;
                bytesQuery = bytesH-bytesQuery;

                if(numPaqQuery == 0.0)
                    retQuery = retH-retQuery;
                else
                    retQuery = (double)((retH-retQuery)/numPaqQuery);

                if((numPaqH+numPerH) == 0)
                    porH = 0.0;
                else{
                    porH = ((double)numPerH/(numPaqH+numPerH))*100;
                }
                    
                if((numPerQuery+numPaqQuery) == 0)
                    porQ = 0.0;
                else{
                    porQ = ((double)numPerQuery/(numPerQuery+numPaqQuery))*100;
                }
                moss = calculaMOSS(porQ);
                mysql_free_result(result);
				sprintf(sql, "INSERT INTO Canales VALUES(\'%s\', %.f, %d, %d, %d, %d, %.f, %.f, %.f, %.f, %.f, %d, %d, %d, %d, %.f, %.1f)",
                    getClave(aux), getLlegadaAnterior(aux)/1000000,  numPaqH, numPaqQuery, numPerH, numPerQuery, porH, porQ,
                    retH, retQuery, getRetardoCuadrado(aux), numErrH, numErrQuery, bytesH, bytesQuery, var, moss);
                rc = mysql_query(db, sql);
                if (rc != 0 ) {
                    fprintf(stderr, "SQL error en Canales: %s\n", mysql_error(db));      
                    return;
                }
                memset(sql, 0, sizeof(sql));
                memset(query, 0, sizeof(query));
				aux = getSiguiente(aux);
			}
		}

	}
    
    pn = getNodePos(igmp, 0);
    while(pn != NULL) {

        canal = getCanal(pn);
        aux = buscarNodoHash(tabla, canal);
        lista = nodoGetInfo(aux);
        size = list_size(lista);
        clientes = list_getClientes(lista);

        for(i = 0; i < size; i++) {

            sprintf(sql, "INSERT INTO Igmp VALUES(\'%s\', %.f, \'%s\')",
                canal, getTiempo(pn)/1000000, clientes[i]);
            rc = mysql_query(db, sql);
            if (rc != 0 ) {
                fprintf(stderr, "SQL error: %s\n", mysql_error(db));
                free(clientes);     
                return;
            }
            free(clientes);
            memset(sql, 0, sizeof(sql));
        }
        
        pn = getNext(pn);
    }
    
    rc = mysql_query(db, GET_RUIDO);
    if (rc != 0 ) {
        fprintf(stderr, "SQL error: %s\n", mysql_error(db));       
        return;
    }
    result = mysql_store_result(db);
    if (!result) {
        fprintf(stderr, "SQL error: %s\n", mysql_error(db));
        mysql_free_result(result);   
        return;
    }
    row = mysql_fetch_row(result);
    
    i = getRuidoCount(ruido);
    if(!row)
        j = 0;
    else
        j = atoi(row[0]);
    sprintf(sql, "INSERT INTO Ruido VALUES(%.f, %d, %d)", getRuidoTiempo(ruido)/1000000, i-j, i);
    rc = mysql_query(db, sql);
    if (rc != 0) {
        fprintf(stderr, "SQL error: %s\n", mysql_error(db));   
        return;
    }
    mysql_free_result(result);
    memset(sql, 0, sizeof(sql));

    return;
    
}

/************************************************************************
* @Title: calculaVarianza
* @Description: Calculamos el jitter del intervalo
* @Input: int: Numero de paquetes del intervalo
          int: Numero de paquetes del intervalo anterior
          double: retardo acumulado del intervalo
          double: retardo acumulado del intervalo anterior
          double: retardo al cuadrado acumulado del intervalo
          double: retardo al cuadrado acumulado del intervalo anterior
* @Output: double: jitter
************************************************************************/
double calculaVarianza(int numPaq, int numPaqQuery, double ret, double retQuery, double retC, double retCQuery) {
    
    double eRet, eRetC, vRet, var = 0.0;
    int difNumPaq = 0;

    if(numPaq == numPaqQuery || ret < retQuery || retC < retCQuery)
        return 0.0;

    difNumPaq = numPaq - numPaqQuery;
    eRet = (ret - retQuery)/difNumPaq;
    eRetC = (retC - retCQuery)/difNumPaq;

    vRet = eRetC - (eRet*eRet);
    var = sqrt(vRet);
    return var;

}

double calculaMOSS(double tasaPerdidas) {
    double r = 0.0;
    double p = 0.0;

    r = (tasaPerdidas/2.0)+1.0;
    p = pow(r,3);
    r = 1.0+4.0/p;
    return r;
}