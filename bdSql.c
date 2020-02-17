#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bdSql.h"

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
    char *err_msg = 0;
    char sql[500];
    char *canal = NULL;
    char** clientes = NULL;

    NodoHash* aux = NULL;
    NodeControl *pn = NULL;
    const List* lista = NULL;

    if(tabla == NULL || igmp == NULL || ruido == NULL)
        return;

    for(i = 0; i < tabla->tam; i++) {

		if(tabla->nodos[i] != NULL){

			aux = tabla->nodos[i];
			while(aux != NULL){

				sprintf(sql, "INSERT INTO Canales VALUES(\'%s\', %f, %d, %d, %f, %f, %d, %d)",
                    getClave(aux), getLlegadaAnterior(aux),  getNumRecibidos(aux), getNumPerdidos(aux), 
                    getRetardo(aux), getRetardoCuadrado(aux), getNumIgmpErr(aux), getNumBytes(aux));
                rc = mysql_query(db, sql);
                //rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
                if (rc != 0 ) {
                    fprintf(stderr, "SQL error: %s\n", mysql_error(db));
                    //sqlite3_free(err_msg);        
                    return;
                }
                //sqlite3_free(err_msg);
                memset(sql, 0, sizeof(sql));
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

            sprintf(sql, "INSERT INTO Igmp VALUES(\'%s\', %f, \'%s\')",
                canal, getTiempo(pn), clientes[i]);
            rc = mysql_query(db, sql);
            //rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
            if (rc != 0 ) {
                fprintf(stderr, "SQL error: %s\n", mysql_error(db));
                free(clientes);
                //sqlite3_free(err_msg);        
                return;
            }
            free(clientes);
            //sqlite3_free(err_msg);
            memset(sql, 0, sizeof(sql));
        }
        
        pn = getNext(pn);
    }

    sprintf(sql, "INSERT INTO Ruido VALUES(%f, %d)", getRuidoTiempo(ruido), getRuidoCount(ruido));
    rc = mysql_query(db, sql);
    //rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != 0) {
        fprintf(stderr, "SQL error: %s\n", mysql_error(db));
        //sqlite3_free(err_msg);        
        return;
    }
    //sqlite3_free(err_msg);
    memset(sql, 0, sizeof(sql));

    return;
    
}