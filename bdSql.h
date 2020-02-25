#ifndef BDSQL_H
#define BDSQL_H

#include <mysql/mysql.h>
#include "hash.h"
#include "listControl.h"
#include "ruido.h"

#define SERVIDOR "localhost"
#define USUARIO "lexo"
#define PASS "b453_TFG_d4t05"
#define TABLA "analisis"

#define DROP_TABLES "DROP TABLE IF EXISTS Canales, Igmp, Ruido"

#define CREATE_TABLE_CANALES "CREATE TABLE Canales (Ip TEXT NOT NULL,Tiempo REAL NOT NULL,NumPaq INT NOT NULL,NumPer INT NOT NULL,Ret REAL NOT NULL,RetC REAL NOT NULL,NumErr INT NOT NULL,Bytes INT NOT NULL, Jitter REAL NOT NULL)"
#define CREATE_TABLE_IGMP "CREATE TABLE Igmp (Ip TEXT NOT NULL,Tiempo REAL NOT NULL,Usuario TEXT NOT NULL)"
#define CREATE_TABLE_RUIDO "CREATE TABLE Ruido (Tiempo REAL NOT NULL, Num INT NOT NULL)"

MYSQL* conectaDB();
int reseteaDB(MYSQL *db);
void volcarTabla(MYSQL *db, TablaHash* tabla, ListControl* igmp, Ruido* ruido);

#endif