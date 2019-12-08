/*#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pcap.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <inttypes.h>*/
#include "analiza.h"

void funcion_prueba(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {

    /* First, lets make sure we have an IP packet */
    struct ether_header *eth_header;
    eth_header = (struct ether_header *) packet;
    if (ntohs(eth_header->ether_type) != ETHERTYPE_IP) {
        printf("Not an IP packet. Skipping...\n\n");
        return;
    }

    /* The total packet length, including all headers
       and the data payload is stored in
       header->len and header->caplen. Caplen is
       the amount actually available, and len is the
       total packet length even if it is larger
       than what we currently have captured. If the snapshot
       length set with pcap_open_live() is too small, you may
       not have the whole packet. */
    //printf("Total packet available: %d bytes\n", header->caplen);
    //printf("Expected packet size: %d bytes\n", header->len);

    /* Pointers to start point of various headers */
    const u_char *ip_header;
    const u_char *udp_header;
    const u_char *rtp_header;

    /* Header lengths in bytes */
    int ethernet_header_length = 14; /* Doesn't change */
    int ip_header_length;
    int udp_header_length = 8; /* No cambia*/
    //int payload_length;
    uint16_t src_prt;
    uint16_t numSeq;
    unsigned int raw_offset = 0;
    raw_offset += 2;

    /* Find start of IP header */
    ip_header = packet + ethernet_header_length;
    /* The second-half of the first byte in ip_header
       contains the IP header length (IHL). */
    ip_header_length = ((*ip_header) & 0x0F);
    /* The IHL is number of 32-bit segments. Multiply
       by four to get a byte count for pointer arithmetic */
    ip_header_length = ip_header_length * 4;
    //printf("IP header length (IHL) in bytes: %d\n", ip_header_length);

    /* Now that we know where the IP header is, we can 
       inspect the IP header for a protocol number to 
       make sure it is TCP before going any further. 
       Protocol is always the 10th byte of the IP header */
    u_char protocol = *(ip_header + 9);
    if (protocol != IPPROTO_UDP) {
        printf("Not a UDP packet. Skipping...\n\n");
        return;
    }

    /* Add the ethernet and ip header length to the start of the packet
       to find the beginning of the UDP header */
    udp_header = packet + ethernet_header_length + ip_header_length;
    src_prt = ntohs((*udp_header));
    printf("Puerto origen: %"PRIu16"\n", src_prt);
    
    rtp_header = packet + ethernet_header_length + ip_header_length + udp_header_length;
    numSeq = rtp_header[raw_offset] * 256 + rtp_header[raw_offset + 1];
    printf("Numero de secuencia: %u\n", numSeq);

    /* Add up all the header sizes to find the payload offset */
    /*int total_headers_size = ethernet_header_length+ip_header_length+tcp_header_length;
    printf("Size of all headers combined: %d bytes\n", total_headers_size);
    payload_length = header->caplen -
        (ethernet_header_length + ip_header_length + tcp_header_length);
    printf("Payload size: %d bytes\n", payload_length);
    payload = packet + total_headers_size;
    printf("Memory address where payload begins: %p\n\n", payload);*/

    /* Print payload in ASCII */
    /*  
    if (payload_length > 0) {
        const u_char *temp_pointer = payload;
        int byte_count = 0;
        while (byte_count++ < payload_length) {
            printf("%c", *temp_pointer);
            temp_pointer++;
        }
        printf("\n");
    }
    */

    return;
}

//void my_callback(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {
//
//    provoca_perdidas(args, header, packet);
//}


int main(int argc, char *argv[]) {

    int fHandle = -1;                                   // 1 - Online / 2 - Fichero
    int porcentaje = -1;                                // Porcentaje de perdidas    
    char filename[LEN_NAME], drop_filename[LEN_NAME];   // Nombre de los ficheros



    while( (opt = getopt(argc, argv, ":OF:lsm:") ) != -1) {
        switch(opt) {
            case 'O':
                // Abrimos captura en vivo/ Liberar memoria de handle
                break;
            
            case 'F':
                //Abrimos fichero/ Liberar memoria de handle
                break;
            
            case 'l':
                // Abrimos fichero dump y provocamos perdidas
                break;
            
            case 's':
                // Vemos las IPs que entran a nuestro PC
                break;
            
            case 'm':
                // Calculamos perdidas y jitter
                break;

            default:
                fprintf(stdout, "Error al introducir parametros: ./TFG O|F <filename> l <drop_filename>|s|m|p <porcentaje>");
                return -1;
        }
    }

    return 0;
}














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