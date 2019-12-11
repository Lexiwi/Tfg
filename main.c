#include "analiza.h"
#include "hash.h"

pcap_t *handle = NULL;      // Manejador pcap

void finaliza_monitorizacion(int signum) {
    pcap_breakloop(handle);
}


int main(int argc, char *argv[]) {

    //int fHandle = -1;             // 1 - Online | 2 - Fichero
    int accion = -1;                // 1 - Monitoriza | 2 - Calcula QoS | 3 - Genera perdidas
    int opt = -1;                   // Variable para argumentos
    PcapDrop *pd = NULL;            // Estructura para provocar perdidas
    //bpf_u_int32 ip;
    //struct bpf_program fp;          // Mantiene la compilación del programa

    char filename[100];

    int res = 0;
    const u_char *packet;
    struct pcap_pkthdr *packet_header;

    TablaHash* tabla;
    

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
            if(tabla == NULL) {
                printf("Error al crear tabla hash");
                return -1;
            }

            //signal(SIGINT, finaliza_monitorizacion);
            while ((res = pcap_next_ex(handle, &packet_header, &packet)) >= 0) {

                if (res == 0) {
                    /*Sobrepasado timeout*/
                    continue;
                }

                if ( leer_paquete(packet_header, packet, tabla) != 0) {
                    //printf("Mosquis\n");
                }

            }
            printf("Numero total de IPs de IGMP: %d\n", getNumNodes(tabla));
            break;

        case 3:
            signal(SIGINT, finaliza_monitorizacion);
            pcap_loop(handle, 2, provoca_perdidas, (u_char*)pd);
            break;

        default:
            break;
    }

    if(accion == 2){
        eliminarTablaHash(tabla);
    } 
    else if(accion == 3) {
        pcap_dump_close(pd->dumpfile);
        free(pd);
    } else {
        
    }
    pcap_close(handle);
    return 0;
}