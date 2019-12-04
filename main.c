#include "analiza.h"

pcap_t *handle = NULL;      // Manejador pcap

void finaliza_monitorizacion(int signum) {
    pcap_breakloop(handle);
}


int main(int argc, char *argv[]) {

    //int fHandle = -1;             // 1 - Online | 2 - Fichero
    int accion = -1;                // 1 - Monitoriza | 2 - Calcula QoS | 3 - Genera perdidas
    int opt = -1;                   // Variable para argumentos
    PcapDrop *pd = NULL;            // Estructura para provocar perdidas
    bpf_u_int32 ip;
    struct bpf_program fp;          // Mantiene la compilación del programa

    FILE *f = NULL;
    char *line = NULL;
    char *token = NULL;
    char filter[28];
    char filename[100];
    size_t len = 0;
    

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

            signal(SIGINT, finaliza_monitorizacion);
            ////////////////////////IGMP//////////////////////////////
            ip = conseguir_direccion_red();
            if (pcap_compile(handle,&fp,FILTER_IGMP,0,ip) == -1) {
                fprintf(stderr,"Error compilando el filtro: %s\n", pcap_geterr(handle));
                pcap_close(handle);
                return -1;
            }
            // Aplicamos el filtro
            if (pcap_setfilter(handle,&fp) == -1) {
                fprintf(stderr,"Error aplicando el filtro\n");
                pcap_close(handle);
                return -1;
            }
            pcap_freecode(&fp);
            ////////////////////////////////////////////////////
            pcap_loop(handle, 0, obtener_igmp, NULL);
            system(COMMAND);
            /////////////////////RTP/////////////////////
            f = fopen(IGMP_IPS, "r");
            if (f == NULL){
                printf("Error al abrir el fichero %s.\n", IGMP_IPS);
                pcap_close(handle);
                return -1;
            }

            while (getline(&line, &len, f) != -1) {
            
                token = strtok(line, " ");
                token = strtok(NULL, " ");

                sprintf(filter, "%s %s", FILTER_RTP, token);
                /////////////////////////////
                pcap_close(handle);
                handle = abrir_captura_offline(filename); //Fuga de memoria
                if(handle == NULL){
                    return -1;
                }
                ////////////////////////////
                if (pcap_compile(handle,&fp,filter,0,ip) == -1) {
                    fprintf(stderr,"Error compilando el filtro: %s\n", pcap_geterr(handle));
                    pcap_close(handle);
                    fclose(f);
                    return -1;
                }
                // Aplicamos el filtro
                if (pcap_setfilter(handle,&fp) == -1) {
                    fprintf(stderr,"Error aplicando el filtro\n");
                    pcap_close(handle);
                    fclose(f);
                    return -1;
                }
                printf("%s\n", filter);
                pcap_loop(handle, 0, obtener_rtp, NULL);
                pcap_freecode(&fp);
                free(line);
                line = NULL;
                len = 0;
                
            }
            fclose(f);
            free(line);
            line = NULL;
            len = 0;
            /////////////////////////////////////////////
            break;

        case 3:
            signal(SIGINT, finaliza_monitorizacion);
            pcap_loop(handle, 2, provoca_perdidas, (u_char*)pd);
            break;

        default:
            break;
    }

    if(accion == 3) {
        pcap_dump_close(pd->dumpfile);
        free(pd);
    }
    pcap_close(handle);
    return 0;
}