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

    /*handle = (pcap_t*)malloc(sizeof(handle));
    if (handle == NULL) {
        fprintf(stdout, "Error al reservar memoria para handle en live\n");
        return NULL;
    }*/

    handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuff);
    if(handle == NULL) {
        fprintf(stdout, "Error al obtener el manejador en captura en vivo: %s\n", errbuff);
        return NULL;
    }

    return handle;
}

pcap_t * abrir_captura_offline(char* filename) {

    char errbuff[PCAP_ERRBUF_SIZE]; /* String de mensaje de error*/
    pcap_t *handle = NULL;          /* Sesion*/
  
    /*handle = (pcap_t *)malloc(sizeof(pcap_t));
    if (handle == NULL) {
        fprintf(stdout, "Error al reservar memoria para handle en offline\n");
        return NULL;
    }*/

    handle = pcap_open_offline(filename, errbuff);
    if(handle == NULL) {
        fprintf(stdout, "Error al obtener el manejador en captura en vivo: %s\n", errbuff);
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

void obtener_rtp(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {

    struct ether_header *eth_header;
    //const struct sniff_ip *ip;          /* Cabecera IP */
    const u_char *ip_header;
    const u_char *rtp_header;
    u_char protocolo;

    int ip_header_length;
    uint16_t numSeq;
    unsigned int raw_offset = 2;

    //FILE *fp = NULL;

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
    if (protocolo != IPPROTO_UDP) {
        printf("No es un paquete UDP. Saltando...\n\n");
        return;
    } 

    rtp_header = packet + LEN_ETH + ip_header_length + LEN_UDP;
    numSeq = rtp_header[raw_offset] * 256 + rtp_header[raw_offset + 1];
    printf("%d\n", numSeq);
    printf("%.6ld\n" ,header->ts.tv_usec);
    
    
    return;
}

int leer_paquete(const struct pcap_pkthdr *header, const u_char *packet, TablaHash* tabla) {

    struct ether_header *eth_header;
    const struct sniff_ip *ip;          /* Cabecera IP */
    u_char protocolo;
    const u_char *ip_header;
    const u_char *rtp_header;
    const u_char *igmp_header;
    char clave[16];
    char cliente[16];
    char fichero[20];
    FILE *fp = NULL;

    int ip_header_length;
    uint16_t numSeq;
    unsigned int raw_offset = 2;

    NodoHash* nodo = NULL;
    List* lista = NULL;

    /* Nos aseguramos que sea un paquete IP */
    eth_header = (struct ether_header *) packet;
    if (ntohs(eth_header->ether_type) != ETHERTYPE_IP) {
        printf("No es un paquete IP. Saltando...\n\n");
        return -1;
    }

    ip = (struct sniff_ip*)(packet + LEN_ETH);
    strcpy(clave, inet_ntoa(ip->ip_dst));
    strcpy(cliente, inet_ntoa(ip->ip_src));

    /* Nos colocamos al principio de la cabecera IP */
    ip_header = packet + LEN_ETH;
    /* Obtenemos el campo IHL para averiguar el tamanio de la cabecera IP */
    ip_header_length = ((*ip_header) & 0x0F);
    /* El campo IHL es un segmento de 32 bits. Multiplicamos por 4 para obtener un puntero aritmetico*/
    ip_header_length = ip_header_length * 4;

    /* Vamos que protocolo se esta utilizando*/
    protocolo = (u_char)*(ip_header + 9);

    // Paquete IGMP
    if (protocolo == IPPROTO_IGMP) {
        
        igmp_header = packet + LEN_ETH + ip_header_length;
        // IGMP Report Group
        if( ((*igmp_header) & 0xFF) == 0x16 ){
            nodo = buscarNodoHash(tabla, clave);

            //Si el arbol no esta registrado, lo guardamos
            if (nodo == NULL) {
                insertarNodoHash(tabla, clave, cliente);
            } else {
                lista = nodoGetInfo(nodo);
                // Si el arbol esta registrado pero el cliente no, lo añadimos
                if(list_check_element(lista, cliente) == 0){
                    list_insertFirst(lista, cliente);
                }
                    
            }
        // IGMP Leave Group
        } else if( ((*igmp_header) & 0xFF) == 0x17 ) {
            nodo = buscarNodoHash(tabla, clave);
            //Si obtenemos un leave y el arbol no esta registrado, comprobamos el cliente
            if (nodo != NULL) {
                lista = nodoGetInfo(nodo);
                list_extractElement(lista, cliente);
                // Si no quedan clientes en el arbol, metemos un indicador
                if(list_isEmpty(lista) == 1){
                    sprintf(fichero, "%s_udp.txt", clave);
                    fp = fopen(fichero, "a");
                    if (fp == NULL){
                        printf("Error al abrir el fichero %s.\n", fichero);
                        return -1;
                    }
                    fprintf(fp, "=======\n");
                    fclose(fp);
                }
                
            }

        } else{}

        sprintf(fichero, "%s_igmp.txt", clave);
        fp = fopen(fichero, "a");
        if (fp == NULL){
            printf("Error al abrir el fichero %s.\n", fichero);
            return -1;
        }
        fprintf(fp, "%s", cliente);
        fprintf(fp, " %s", clave);
        fprintf(fp, " %d", header->len);
        fprintf(fp, " %ld\n", ((header->ts.tv_sec)*1000000L+(header->ts.tv_usec)));
        fclose(fp);
        
    }
    // Caso UDP
    else if (protocolo == IPPROTO_UDP) {

        if (checkNodoHash(tabla, clave) == 0) {
            sprintf(fichero, "%s_udp.txt", clave);
            fp = fopen(fichero, "a");
            if (fp == NULL){
                printf("Error al abrir el fichero: %s.\n", fichero);
                return -1;
            }
            rtp_header = packet + LEN_ETH + ip_header_length + LEN_UDP;
            numSeq = rtp_header[raw_offset] * 256 + rtp_header[raw_offset + 1];
            fprintf(fp, "%s", inet_ntoa(ip->ip_src));
            fprintf(fp, " %s", clave);
            fprintf(fp, " %d", numSeq);
            fprintf(fp, " %d", header->len);
            fprintf(fp, " %ld\n", ((header->ts.tv_sec)*1000000L+(header->ts.tv_usec)));
            fclose(fp);
        } else {

            fp = fopen("ruido.txt", "a");
            if (fp == NULL){
                printf("Error al abrir el fichero: %s.\n", fichero);
                return -1;
            }
            fprintf(fp, "%s", inet_ntoa(ip->ip_src));
            fprintf(fp, " %s", clave);
            fprintf(fp, " %d", header->len);
            fprintf(fp, " %ld\n", ((header->ts.tv_sec)*1000000L+(header->ts.tv_usec)));
            fclose(fp);
        }
    }
    else {
        printf("Ni IGMP ni UDP\n");
    }

    
    return 0;
}