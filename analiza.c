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

void obtener_igmp(const struct pcap_pkthdr *header, const u_char *packet, TablaHash* tabla, ListControl* igmp) {

    const u_char *igmp_header;
    double ret = 0.0;
    char clave[16], cliente[16], aux[16];
    char *token;
    const char z[2] = "0";
    const char p[2] = ".";
    const struct sniff_ip *ip;
    const u_char *ip_header;
    int ip_header_length;

    NodoHash* nodo = NULL;
    NodeControl* node = NULL;
    List* lista = NULL;

    ip = (struct sniff_ip*)(packet + LEN_ETH);
    strcpy(clave, inet_ntoa(ip->ip_dst));
    strcpy(aux, clave);
    // Evitamos canales que no sean rtp
    token = strtok(aux, p);
    token = strtok(NULL, p);
    if(strcmp(token, z) != 0){
        printf("HEYYYYY: %s", clave);
        return;
    }
        
    
    strcpy(cliente, inet_ntoa(ip->ip_src));

    //Microsegundos
    ret = ((header->ts.tv_sec)*1000000L+(header->ts.tv_usec));
    /* Nos colocamos al principio de la cabecera IP */
    ip_header = packet + LEN_ETH;
    /* Obtenemos el campo IHL para averiguar el tamanio de la cabecera IP */
    ip_header_length = ((*ip_header) & 0x0F);
    /* El campo IHL es un segmento de 32 bits. Multiplicamos por 4 para obtener un puntero aritmetico*/
    ip_header_length = ip_header_length * 4;
    igmp_header = packet + LEN_ETH + ip_header_length;

    // IGMP Report Group
    if( ((*igmp_header) & 0xFF) == 0x16 ){
        nodo = buscarNodoHash(tabla, clave);
        //Si el arbol no esta registrado, lo guardamos
        if (nodo == NULL) {
            insertarNodoHash(tabla, clave, cliente, ret);
            //Guardamos el paquete IGMP en la lista
            listControl_insertFirst(igmp, clave, ret, 1);
        } else {
            lista = nodoGetInfo(nodo);
            // Si el arbol esta registrado pero el cliente no, lo añadimos
            if(list_check_element(lista, cliente) == 0)
                list_insertFirst(lista, cliente);
            // Actualizamos el paquete IGMP
            node = getNode(igmp, clave);
            setTiempo(node, ret);
            setInfo(node, 1);
                
        }
   
    }
    // IGMP Leave Group
    else if( ((*igmp_header) & 0xFF) == 0x17 ) {
        nodo = buscarNodoHash(tabla, clave);
        //Si obtenemos un leave y el arbol esta registrado, comprobamos el cliente
        if (nodo != NULL) {
            lista = nodoGetInfo(nodo);
            list_extractElement(lista, cliente);
            // Si no quedan clientes en el arbol, indicamos que 
            // no esperamos paquetes de el
            if(list_isEmpty(lista) == 1){
                node = getNode(igmp, clave);
                setTiempo(node, ret);
                setInfo(node, 0);
            }
            
        }
    } 
    else
        return;

    return;
}

void obtener_rtp(const struct pcap_pkthdr *header, const u_char *packet, TablaHash* tabla, ListControl* igmp, ListControl* udp, Ruido* ruido) {

    double ret = 0.0;
    double retA = 0.0;
    double interArrival = 0.0;
    const struct sniff_ip *ip;
    const u_char *ip_header;
    const u_char *rtp_header;
    char clave[16];
    char cliente[16];
    int ip_header_length;
    uint16_t numSeq;

    NodoHash* nodo = NULL;
    NodeControl* node = NULL;

    ip = (struct sniff_ip*)(packet + LEN_ETH);
    strcpy(clave, inet_ntoa(ip->ip_dst));
    strcpy(cliente, inet_ntoa(ip->ip_src));

    //Microsegundos 1000000L | Mili 1000
    ret = ((header->ts.tv_sec)*1000000L+(header->ts.tv_usec));
    /* Nos colocamos al principio de la cabecera IP */
    ip_header = packet + LEN_ETH;
    /* Obtenemos el campo IHL para averiguar el tamanio de la cabecera IP */
    ip_header_length = ((*ip_header) & 0x0F);
    /* El campo IHL es un segmento de 32 bits. Multiplicamos por 4 para obtener un puntero aritmetico*/
    ip_header_length = ip_header_length * 4;

    if ((nodo = buscarNodoHash(tabla, clave)) != NULL) {

        // Comprobacion de error IGMP
        node = getNode(igmp, clave);
        if( getInfo(node) == 0) {
            setNumIgmpErr(nodo, 1);
            return;
        }

        setNumRecibidos(nodo, 1);
        retA = getLlegadaAnterior(nodo);
        setLlegadaAnterior(nodo, ret);
        interArrival = ret - retA;
        setRetardo(nodo, interArrival);
        setRetardoCuadrado(nodo, (interArrival*interArrival));
        setNumBytes(nodo, header->len);
        
        // Actualizamos el paquete UDP
        node = getNode(udp, clave);
        rtp_header = packet + LEN_ETH + ip_header_length + LEN_UDP;
        numSeq = rtp_header[RAW_OFF] * 256 + rtp_header[RAW_OFF + 1];
        if(node == NULL)
            listControl_insertFirst(udp, clave, ret, numSeq);
        else {
            setTiempo(node, ret);
            //Comprobación de paquetes perdidos
            if(numSeq - getInfo(node) > 1)
                setNumPerdidos(nodo, numSeq - getInfo(node) - 1);
            setInfo(node, numSeq);
        }
        
    } 
    else
        actualizaRuido(ruido, ret); //Ruido

    return;
}

int leer_paquete(const struct pcap_pkthdr *header, const u_char *packet, TablaHash* tabla, ListControl* igmp, ListControl* udp, Ruido* ruido) {

    struct ether_header *eth_header;
    u_char protocolo;
    const u_char *ip_header;

    /* Nos aseguramos que sea un paquete IP */
    eth_header = (struct ether_header *) packet;
    if (ntohs(eth_header->ether_type) != ETHERTYPE_IP) {
        printf("No es un paquete IP. Saltando...\n\n");
        return -1;
    }

    /* Nos colocamos al principio de la cabecera IP */
    ip_header = packet + LEN_ETH;
    /* Vamos que protocolo se esta utilizando*/
    protocolo = (u_char)*(ip_header + 9);

    // Paquete IGMP
    if (protocolo == IPPROTO_IGMP) 
        obtener_igmp(header, packet, tabla, igmp);
    // Caso UDP
    else if (protocolo == IPPROTO_UDP)
        obtener_rtp(header, packet, tabla, igmp, udp, ruido);
    else 
        printf("Ni IGMP ni UDP\n");
    
    return 0;
}

void errorIgmp(TablaHash* tabla, ListControl* igmp, ListControl* udp) {

    int size = 0;
    int i = 0;
    double tmpUDP, tmpIGMP = 0.0;
    char* clave = NULL;
    NodeControl* nodeIGMP = NULL;
    NodeControl* nodeUDP = NULL;
    NodoHash* nodo = NULL;

    if(tabla == NULL || igmp == NULL || udp == NULL)
        return;

    size = listControl_size(igmp);
    for(i = 0; i < size; i++) {
        
        nodeIGMP = getNodePos(igmp, i);
        if(nodeIGMP != NULL){
            clave = getCanal(nodeIGMP);
            nodeUDP = getNode(udp, clave);
            tmpUDP = getTiempo(nodeUDP);
            tmpIGMP = getTiempo(nodeIGMP);
            //Supongo que seria mas 5ms ya que es cuando se envia un paquete
            //if(tmpUDP > tmpIGMP + 5000.0){
            if(tmpUDP < tmpIGMP && nodeUDP != NULL){
                nodo = buscarNodoHash(tabla, clave);
                setNumIgmpErr(nodo, 1);
            }
        }

    }
}
