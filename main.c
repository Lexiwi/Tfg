#include <pthread.h> 
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include "analiza.h"
#include "hash.h"
#include "listControl.h"
#include "ruido.h"
#include "bdSql.h"

pcap_t *handle = NULL;      // Manejador pcap
sem_t mutex;                // Semaforo
sem_t wake;                 // Semaforo
int para = 1;

TablaHash* tabla = NULL;
ListControl* igmp = NULL;
ListControl* udp = NULL;
Ruido* ruido = NULL;
MYSQL *db = NULL;

void* hilo_errIGMP(void* arg);
void* hilo_baseDatos(void* arg);
void finaliza_monitorizacion(int signum);

void finaliza_monitorizacion(int signum) {
    pcap_breakloop(handle);
}

void* hilo_errIGMP(void* arg) {

    while( para == 1 ){
        sleep(1);
        sem_wait(&mutex);
        errorIgmp(tabla, igmp, udp);
        sem_post(&mutex);
    }
    return NULL;
}

void* hilo_baseDatos(void* arg) {

    //TablaHash* auxt = NULL;
    //ListControl* auxi = NULL;
    //Ruido* auxr = NULL;
    
    //Inicializamos la base de datos
    
    while( para == 1 ){
        sem_wait(&wake);
        sem_wait(&mutex);
        //auxt = copiarTablaHash(tabla);
        //auxi = listControl_copy(igmp);
        //auxr = ruido_copy(ruido);
        //sem_post(&mutex);
        //volcarTabla(db, auxt, auxi, auxr);
        //listControl_free(auxi);
        //ruido_free(auxr);
        //eliminarTablaHash(auxt);
        volcarTabla(db, tabla, igmp, ruido);
        sem_post(&mutex);
    }
    return NULL;
}

int main(int argc, char *argv[]) {

    int accion = -1;                // 1 - Monitoriza | 2 - Calcula QoS | 3 - Genera perdidas
    int opt = -1;                   // Variable para argumentos
    int res = 0;
    int por = 0;
    char filename[100];
    
    const u_char *packet;
    struct pcap_pkthdr *packet_header;
    pcap_dumper_t *dumpfile;

    pthread_t hilo_1;
    pthread_t hilo_2;

    while( (opt = getopt(argc, argv, ":OF:l:sm") ) != -1) {
        switch(opt) {
            case 'O':
                // Abrimos captura en vivo/ Liberar memoria de handle
                handle = abrir_captura_online(DEV);
                if(handle == NULL){
                    return -1;
                }
                break;
            
            case 'F':
                // Abrimos captura de fichero/ Liberar memoria de handle
                strcpy(filename, optarg);
                handle = abrir_captura_offline(filename);
                if(handle == NULL){
                    return -1;
                }
                break;
            
            case 'l':
                // Abrimos fichero dump y provocamos perdidas/ Lberar memoria de handle y dump
                accion = 3;
                dumpfile = pcap_dump_open(handle, DROP_FILE_NAME);
                if (dumpfile == NULL) {
                    fprintf(stdout, "Error al abrir un fichero dump\n");
                    pcap_close(handle);
                    free(handle);
                    return -1;
                }
                por = atoi(optarg);
                break;
            
            case 's':
                // Observamos el trafico
                accion = 1;
                break;
            
            case 'm':
                // Calculamos perdidas y jitter
                accion = 2;
                break;

            default:
                fprintf(stdout, "Error al introducir parametros: ./TFG O|F <filename> l <porcentaje>|s|m\n");
                return -1;
        }
    }

    switch (accion) {

        case 1:
            signal(SIGINT, finaliza_monitorizacion);
            pcap_loop(handle, 0, obtener_trafico_entrante, NULL);
            break;
        
        case 2:

            // Creacion de la tabla
	        tabla = crearTablaHash(30);
            // Creacion de las listas-tablas
            igmp = listControl_ini();
            udp = listControl_ini();
            ruido = ruido_ini();
            if(tabla == NULL || igmp == NULL || udp == NULL) {
                printf("Error al crear tabla hash o listControl");
                return -1;
            }
            db = conectaDB();
            if (!db) {
                fprintf(stderr, "Error al abrir la base de datos: %s\n", mysql_error(db));
                break;
            }
            
            if(reseteaDB(db) != 0)
                break;
            
            sem_init(&mutex, 0, 1);
            sem_init(&wake, 0, 0);
            pthread_create(&hilo_1, NULL, *hilo_errIGMP, NULL);
            pthread_create(&hilo_2, NULL, *hilo_baseDatos, NULL);

            signal(SIGINT, finaliza_monitorizacion);
            while ((res = pcap_next_ex(handle, &packet_header, &packet)) >= 0) {

                if (res == 0) {
                    continue; /*Sobrepasado timeout*/
                }
                sem_wait(&mutex);
                if ( leer_paquete(packet_header, packet, tabla, igmp, udp, ruido) == 1) {
                    sem_post(&wake);
                }
                sem_post(&mutex);
            }
            para = 0;
            sem_post(&wake);
            pthread_join(hilo_1, NULL);
            pthread_join(hilo_2, NULL);

            eliminarTablaHash(tabla);
            listControl_free(igmp);
            listControl_free(udp);
            ruido_free(ruido);
            mysql_close(db);
            //Libera mysql_library_init() que se ejecuta dentro de mysql_init()
            mysql_library_end();
            sem_destroy(&mutex);
            sem_destroy(&wake);
            break;

        case 3:
            signal(SIGINT, finaliza_monitorizacion);
            while ((res = pcap_next_ex(handle, &packet_header, &packet)) >= 0) {

                if (res == 0) {
                    /*Sobrepasado timeout*/
                    continue;
                }
                provoca_perdidas(dumpfile, por, packet_header, packet);
            }
            //pcap_loop(handle, 2, provoca_perdidas, (u_char*)pd);
            pcap_dump_close(dumpfile);
            break;

        default:
            break;
    }
    
    pcap_close(handle);
    return 0;
}