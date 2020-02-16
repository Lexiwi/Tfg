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