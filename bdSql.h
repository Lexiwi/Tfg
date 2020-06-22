/*********************************************************** 
* File:   bdsql.h
* Author: Jorge Gutierrez Diaz
* 
* Descripción: Modulo para operaciones con la base de datos
*
***********************************************************/


#ifndef BDSQL_H
#define BDSQL_H

#include <mysql/mysql.h>
#include "hash.h"
#include "listControl.h"
#include "ruido.h"
#include <math.h>

#define SERVIDOR "localhost"
#define USUARIO "INSERT_USER"
#define PASS "INSERT_PASSWORD"
#define TABLA "analisis"

#define DROP_TABLES "DROP TABLE IF EXISTS Canales, Igmp, Ruido"

#define CREATE_TABLE_CANALES "CREATE TABLE Canales (id INT AUTO_INCREMENT PRIMARY KEY,Ip TEXT NOT NULL,Tiempo REAL NOT NULL,NumPaq INT NOT NULL,NumPaqDif INT NOT NULL, NumPer INT NOT NULL,NumPerDif INT NOT NULL, PorPer INT NOT NULL,PorPerDif INT NOT NULL, Ret REAL NOT NULL,RetDif REAL NOT NULL,RetC REAL NOT NULL,NumErr INT NOT NULL,NumErrDif INT NOT NULL,Bytes INT NOT NULL,Throughput REAL NOT NULL,Jitter REAL NOT NULL,Mos REAL NOT NULL,Tipo INT NOT NULL)"
#define CREATE_TABLE_IGMP "CREATE TABLE Igmp (Ip TEXT NOT NULL,Tiempo REAL NOT NULL,Usuario TEXT NOT NULL)"
#define CREATE_TABLE_RUIDO "CREATE TABLE Ruido (Tiempo REAL NOT NULL, Num INT NOT NULL, Total INT NOT NULL)"


#define GET_RUIDO "SELECT Total FROM Ruido ORDER BY Total DESC LIMIT 1"

/********************* Declaracion de funciones públicas **********************/

/* Conectamos a la base de datos*/
MYSQL* conectaDB();

/* Limpiamos la base de datos */
int reseteaDB(MYSQL *db);

/* Volcamos la informacion a la base de datos*/
void volcarTabla(MYSQL *db, TablaHash* tabla, ListControl* igmp, Ruido* ruido);

#endif