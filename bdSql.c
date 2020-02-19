#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "bdSql.h"

double calculaVarianza(int numPaq, int numPaqQuery, double ret, double retQuery, double retC, double retCQuery);

MYSQL* conectaDB() {

    MYSQL *db = NULL;
    db = mysql_init(NULL);

    if (!mysql_real_connect(db, NULL, USUARIO, PASS, TABLA, 0, NULL, 0)) {
        fprintf(stderr, "Error al abrir la base de datos: %s\n", mysql_error(db));
        return NULL;
    }

    return db;

}

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

void volcarTabla(MYSQL *db, TablaHash* tabla, ListControl* igmp, Ruido* ruido) {

    int i, j, size;
    int rc = 0;
    int numPaqQuery = 0;
    double retQuery, retCQuery, var = 0.0; 
    char *err_msg = 0;
    char *eptr;
    char sql[500], query[100];
    char *canal = NULL;
    char** clientes = NULL;

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

                sprintf(query, "SELECT Ret,RetC,NumPaq FROM Canales WHERE Ip=\'%s\' ORDER BY NumPaq DESC LIMIT 1", getClave(aux));
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
                if(!row)
                    var = calculaVarianza(getNumRecibidos(aux), 0, getRetardo(aux), 0.0, getRetardoCuadrado(aux), 0.0);
                else {
                    retQuery = strtod(row[0], &eptr);
                    retCQuery = strtod(row[1], &eptr);
                    numPaqQuery = atoi(row[2]);
                    var = calculaVarianza(getNumRecibidos(aux), numPaqQuery, getRetardo(aux), retQuery, getRetardoCuadrado(aux), retCQuery);
                }

                mysql_free_result(result);
				sprintf(sql, "INSERT INTO Canales VALUES(\'%s\', %.f, %d, %d, %.f, %.f, %d, %d, %.f)",
                    getClave(aux), getLlegadaAnterior(aux),  getNumRecibidos(aux), getNumPerdidos(aux), 
                    getRetardo(aux), getRetardoCuadrado(aux), getNumIgmpErr(aux), getNumBytes(aux), var);
                rc = mysql_query(db, sql);
                if (rc != 0 ) {
                    fprintf(stderr, "SQL error: %s\n", mysql_error(db));      
                    return;
                }
                memset(sql, 0, sizeof(sql));
                memset(sql, 0, sizeof(query));
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
                canal, getTiempo(pn), clientes[i]);
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

    sprintf(sql, "INSERT INTO Ruido VALUES(%.f, %d)", getRuidoTiempo(ruido), getRuidoCount(ruido));
    rc = mysql_query(db, sql);
    if (rc != 0) {
        fprintf(stderr, "SQL error: %s\n", mysql_error(db));   
        return;
    }
    memset(sql, 0, sizeof(sql));

    return;
    
}


double calculaVarianza(int numPaq, int numPaqQuery, double ret, double retQuery, double retC, double retCQuery) {
    
    double eRet, eRetC, vRet, var = 0.0;
    int difNumPaq = 0;

    if(numPaq == numPaqQuery || ret < retQuery || retC < retCQuery)
        return 0.0;

    printf("num1: %d\n", numPaq);
    printf("num2: %d\n", numPaqQuery);
    printf("num3: %.f\n", ret);
    printf("num4: %.f\n", retQuery);
    printf("num5: %.f\n", retC);
    printf("num6: %.f\n", retCQuery);

    difNumPaq = numPaq - numPaqQuery;
    eRet = (ret - retQuery)/difNumPaq;
    eRetC = (retC - retCQuery)/difNumPaq;

    vRet = eRetC - (eRet*eRet);
    var = sqrt(vRet);
    return var;

}