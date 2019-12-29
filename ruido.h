#ifndef RUIDO_H
#define RUIDO_H


/**********************  Nuevo tipo de datos para Lista ***********************/
typedef struct _Ruido Ruido;

/********************* Declaracion de funciones públicas **********************/
Ruido* ruido_ini();
void ruido_free(Ruido* ruido);
int getRuidoCount(Ruido* ruido);
int setRuidoCount(Ruido* ruido, int cnt);
double getRuidoTiempo(Ruido* ruido);
int setRuidoTiempo(Ruido* ruido, double tmp);
int actualizaRuido(Ruido* ruido, double tmp);

#endif /*RUIDO_H*/