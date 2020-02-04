#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "bdLite.h"

int reseteaDB(sqlite3 *db) {

    char *drop = "DROP TABLE IF EXISTS Canales;"
                 "DROP TABLE IF EXISTS Igmp;"
                 "DROP TABLE IF EXISTS Ruido;"
                 "CREATE TABLE 'Canales' ('Ip' TEXT NOT NULL,'Tiempo' REAL NOT NULL,'NumPaq' INTEGER NOT NULL,'NumPer' INTEGER NOT NULL,'Ret' REAL NOT NULL,'RetC' REAL NOT NULL,'NumErr' INTEGER NOT NULL,'Bytes' INTEGER NOT NULL, PRIMARY KEY('Ip','Tiempo'));"
                 "CREATE TABLE 'Igmp' ('Id'	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, 'Canal'	TEXT NOT NULL, 'Tiempo'	REAL NOT NULL,'Usuario'	TEXT NOT NULL);"
                 "CREATE TABLE 'Ruido' ('Tiempo' REAL NOT NULL, 'Num' INTEGER NOT NULL, PRIMARY KEY('Tiempo'));";
    char *err_msg = 0;
    int rc = 0;

    if(db == NULL) {
        fprintf(stderr, "Controlador de la base de datos a NULL");
        return -1;
    }

    rc = sqlite3_exec(db, drop, 0, 0, &err_msg);
    if (rc != SQLITE_OK ) {
        sqlite3_free(err_msg);
        fprintf(stderr, "SQL error eliminando tablas: %s\n", err_msg);  
        //sqlite3_close(db);
        return -1;
    }

    return 0;
}