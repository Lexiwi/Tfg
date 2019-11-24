#include "analiza.h"

char * conseguir_dev() {

    char *dev;
    bpf_u_int32 mascara_subred, ip;
    char errbuff[PCAP_ERRBUF_SIZE]; /* String de mensaje de error*/

    /* Define un device por defecto */
    dev = pcap_lookupdev(errbuff);
    if (dev == NULL) {
        fprintf(stderr, "Couldn't find default device: %s\n", errbuff);
        return NULL;
    }
    /* Find the properties for the device */
    if (pcap_lookupnet(dev, &ip, &mascara_subred, errbuff) == -1) {
        fprintf(stderr, "Couldn't get netmask for device %s: %s\n", dev, errbuff);
        ip = 0;
        mascara_subred = 0;
        return NULL;
    }

    return dev;
}

bpf_u_int32 conseguir_direccion_red() {

    char *dev;
    bpf_u_int32 mascara_subred, ip;
    char errbuff[PCAP_ERRBUF_SIZE]; /* String de mensaje de error*/

    /* Define un device por defecto */
    dev = pcap_lookupdev(errbuff);
    if (dev == NULL) {
        fprintf(stderr, "Couldn't find default device: %s\n", errbuff);
        return -1;
    }
    /* Find the properties for the device */
    if (pcap_lookupnet(dev, &ip, &mascara_subred, errbuff) == -1) {
        fprintf(stderr, "Couldn't get netmask for device %s: %s\n", dev, errbuff);
        ip = 0;
        mascara_subred = 0;
        return -1;
    }

    return ip;
}

pcap_t * abrir_captura_online() {

    char *dev;
    char errbuff[PCAP_ERRBUF_SIZE]; /* String de mensaje de error*/
    pcap_t *handle = NULL;          /* Sesion*/

    dev = conseguir_dev();
    if (dev == NULL) {
        fprintf(stdout, "Error al obtener una interfaz de captura\n");
    }

    handle = (pcap_t*)malloc(sizeof(handle));
    if (handle == NULL) {
        fprintf(stdout, "Error al reservar memoria para handle en live\n");
        return NULL;
    }

    handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuff);
    if(handle == NULL) {
        fprintf(stdout, "Error al obtener el manejador en captura en vivo: %s\n", errbuff);
        free(handle);
        return NULL;
    }

    return handle;
}

pcap_t * abrir_captura_offline(char* filename) {

    char errbuff[PCAP_ERRBUF_SIZE]; /* String de mensaje de error*/
    pcap_t *handle = NULL;          /* Sesion*/
  
    handle = (pcap_t *)malloc(sizeof(handle));
    if (handle == NULL) {
        fprintf(stdout, "Error al reservar memoria para handle en offline\n");
        return NULL;
    }

    handle = pcap_open_offline(filename, errbuff);
    if(handle == NULL) {
        fprintf(stdout, "Error al obtener el manejador en captura en vivo: %s\n", errbuff);
        free(handle);
        return NULL;
    }

    return handle;
} 

void obtener_trafico_entrante(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {

    
    struct ether_header *eth_header;    /* Cabecera Ethernet */
    const struct sniff_ip *ip;          /* Cabecera IP */
    //const u_char *ip_header;            /* Puntero a la cabecera IP */

    /* Primero comprobamos que sea un paquete IP */
    eth_header = (struct ether_header *) packet;
    if (ntohs(eth_header->ether_type) != ETHERTYPE_IP) {
        printf("No es un paquete IP. Saltando al siguiente...\n\n");
        return;
    }
    
    /* Igualamos la estructura con el inicio de la cabecera IP y obtenemos 
     * la direccion IP origen y destino*/
    ip = (struct sniff_ip*)(packet + LEN_ETH);
    printf("%s - ", inet_ntoa(ip->ip_src));
    printf("%s\n", inet_ntoa(ip->ip_dst));

    return;
}

void provoca_perdidas(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {

    double randomNumber = 0.0;
    double porcentaje = 0.0;
    u_char *dumpfile;
    PcapDrop *pd = NULL;

    pd = (PcapDrop*)args;
    dumpfile = (u_char*)pd->dumpfile;
    porcentaje = pd->porcentaje;

    randomNumber = (double)rand() / (double)((unsigned)RAND_MAX + 1);

    if(randomNumber > (double)(porcentaje/100)) {
        pcap_dump(dumpfile, header, packet);
    }

    return;
}

void obtener_igmp(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {

    struct ether_header *eth_header;
    const struct sniff_ip *ip;          /* Cabecera IP */
    const u_char *ip_header;
    const u_char *igmp_header;
    u_char protocolo;

    int ip_header_length;

    FILE *fp = NULL;

    /* Nos aseguramos que sea un paquete IP */
    eth_header = (struct ether_header *) packet;
    if (ntohs(eth_header->ether_type) != ETHERTYPE_IP) {
        printf("No es un paquete IP. Saltando...\n\n");
        return;
    }    

    /* Nos colocamos al principio de la cabecera IP */
    ip_header = packet + LEN_ETH;
    /* Obtenemos el campo IHL para averiguar el tamanio de la cabecera IP */
    ip_header_length = ((*ip_header) & 0x0F);
    /* El campo IHL es un segmento de 32 bits. Multiplicamos por 4 para obtener un puntero aritmetico*/
    ip_header_length = ip_header_length * 4;

    /* Vamos que protocolo se esta utilizando*/
    protocolo = (u_char)*(ip_header + 9);
    if (protocolo != IPPROTO_IGMP) {
        printf("No es un paquete IGMP. Saltando...\n\n");
        return;
    } 

    igmp_header = packet + LEN_ETH + ip_header_length;
    if( ((*igmp_header) & 0xFF) == 0x16 ) {
        fp = fopen(IGMP_FIELDS, "a");
        if (fp == NULL){
            printf("Error al abrir el fichero %s. Saltando...\n\n", IGMP_FIELDS);
            return;
        }
        /* Igualamos la estructura con el inicio de la cabecera IP y obtenemos 
        * la direccion IP origen y destino*/
        ip = (struct sniff_ip*)(packet + LEN_ETH);
        fprintf(fp, "%s", inet_ntoa(ip->ip_src));
        fprintf(fp, " %s", inet_ntoa(ip->ip_dst));
        fprintf(fp, " %ld\n", ((header->ts.tv_sec)*1000000L+(header->ts.tv_usec)));
        fclose(fp);
    }
    return;
}