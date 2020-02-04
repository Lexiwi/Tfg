#include <pthread.h> 
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include "analiza.h"
#include "hash.h"
#include "listControl.h"
#include "ruido.h"
#include "bdLite.h"

pcap_t *handle = NULL;      // Manejador pcap
sem_t mutex;                // Semaforo
int para = 1;

TablaHash* tabla = NULL;
ListControl* igmp = NULL;
ListControl* udp = NULL;
Ruido* ruido = NULL;
sqlite3 *db;

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

    TablaHash* aux = NULL;
    
    //Inicializamos la base de datos
    
    while( para == 1 ){
        sleep(10);
        aux = crearTablaHash(30);
        sem_wait(&mutex);
        copiarTablaHash(tabla, aux);
        sem_post(&mutex);
        volcarTabla(db, aux, igmp, udp, ruido);
        eliminarTablaHash(aux);
    }
    return NULL;
}

int main(int argc, char *argv[]) {

    //int fHandle = -1;             // 1 - Online | 2 - Fichero
    int accion = -1;                // 1 - Monitoriza | 2 - Calcula QoS | 3 - Genera perdidas
    int opt = -1;                   // Variable para argumentos
    int res = 0;
    int rc = 0;
    char filename[100];
    
    const u_char *packet;
    struct pcap_pkthdr *packet_header;

    PcapDrop *pd = NULL;            // Estructura para provocar perdidas

    //struct timeval t_ini, t_fin;

    pthread_t hilo_1;
    pthread_t hilo_2;

    while( (opt = getopt(argc, argv, ":OF:lsm::") ) != -1) {
        switch(opt) {
            case 'O':
                // Abrimos captura en vivo/ Liberar memoria de handle
                handle = abrir_captura_online();
                if(handle == NULL){
                    return -1;
                }
                //fHandle = 1;
                break;
            
            case 'F':
                // Abrimos captura de fichero/ Liberar memoria de handle
                strcpy(filename, optarg);
                handle = abrir_captura_offline(filename); //Fuga de memoria
                if(handle == NULL){
                    return -1;
                }
                //fHandle = 2;
                break;
            
            case 'l':
                // Abrimos fichero dump y provocamos perdidas/ Lberar memoria de handle y dump
                pd = (PcapDrop *)malloc(sizeof(PcapDrop));
                if (pd == NULL) {
                    fprintf(stdout, "Error al reservar memoria para PcapDrop\n");
                    pcap_close(handle);
                    free(handle);
                    return -1;
                }
                pd->dumpfile = pcap_dump_open(handle, DROP_FILE_NAME);
                if (pd->dumpfile == NULL) {
                    fprintf(stdout, "Error al abrir un fichero dump\n");
                    free(pd);
                    pcap_close(handle);
                    free(handle);
                    return -1;
                }
                pd->porcentaje = atoi(optarg);
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
            rc = sqlite3_open("data.db", &db);
            if(tabla == NULL || igmp == NULL || udp == NULL) {
                printf("Error al crear tabla hash o listControl");
                return -1;
            }
            if (rc != SQLITE_OK) {
                fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
                break;
            }
            
            if(reseteaDB(db) != 0)
                break;
            
            sem_init(&mutex, 0, 1);
            pthread_create(&hilo_1, NULL, *hilo_errIGMP, NULL);
            pthread_create(&hilo_2, NULL, *hilo_baseDatos, NULL);

            //signal(SIGINT, finaliza_monitorizacion);   <------------- TO DO
            while ((res = pcap_next_ex(handle, &packet_header, &packet)) >= 0) {

                if (res == 0) {
                    /*Sobrepasado timeout*/
                    continue;
                }
                sem_wait(&mutex);
                if ( leer_paquete(packet_header, packet, tabla, igmp, udp, ruido) != 0) {
                }
                sem_post(&mutex);
            }
            para = 0;
            pthread_join(hilo_1, NULL);
            pthread_join(hilo_2, NULL);
    
            ////////////////////
            //gettimeofday(&t_ini, NULL);
            //gettimeofday(&t_fin, NULL);
            //printf("%.16g milliseconds\n", ((double)(t_fin.tv_sec + (double)t_fin.tv_usec/1000000) - (double)(t_ini.tv_sec + (double)t_ini.tv_usec/1000000)) * 1000.0);
            //////////////7////
            eliminarTablaHash(tabla);
            listControl_free(igmp);
            listControl_free(udp);
            ruido_free(ruido);
            sqlite3_close(db);
            sem_destroy(&mutex);
            break;

        case 3:
            signal(SIGINT, finaliza_monitorizacion);
            pcap_loop(handle, 2, provoca_perdidas, (u_char*)pd);

            pcap_dump_close(pd->dumpfile);
            free(pd);
            break;

        default:
            break;
    }
    
    pcap_close(handle);
    return 0;
}