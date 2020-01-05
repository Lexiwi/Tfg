#ifndef ANALIZA_H
#define ANALIZA_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pcap.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <inttypes.h>
#include <getopt.h>
#include <arpa/inet.h>
#include <signal.h>
#include <time.h>
#include "hash.h"
#include "listControl.h"
#include "ruido.h"
/* constantes */

/*#ifndef ERR
  #define ERR -1
  #define OK (!(ERR))
#endif */

#define LEN_NAME 256
#define LEN_DEV 10
#define LEN_ETH 14
#define LEN_UDP 8
#define RAW_OFF 2
#define DROP_FILE_NAME "file_drop"
#define FILTER_IGMP "igmp"
#define FILTER_RTP "udp and dst"

struct sniff_ip {
    u_char ip_vhl;		/* version << 4 | header length >> 2 */
    u_char ip_tos;		/* type of service */
    u_short ip_len;		/* total length */
    u_short ip_id;		/* identification */
    u_short ip_off;		/* fragment offset field */
#define IP_RF 0x8000		/* reserved fragment flag */
#define IP_DF 0x4000		/* dont fragment flag */
#define IP_MF 0x2000		/* more fragments flag */
#define IP_OFFMASK 0x1fff	/* mask for fragmenting bits */
    u_char ip_ttl;		/* time to live */
    u_char ip_p;		/* protocol */
    u_short ip_sum;		/* checksum */
    struct in_addr ip_src; /* source address */
    struct in_addr ip_dst; /* dest address */
};

typedef struct _PcapDrop{

  int porcentaje;
  pcap_dumper_t *dumpfile;

} PcapDrop;

char * conseguir_dev();
bpf_u_int32 conseguir_direccion_red();
pcap_t * abrir_captura_online();
pcap_t * abrir_captura_offline(char* filename);
void obtener_trafico_entrante(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);
void provoca_perdidas(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);
void obtener_igmp(const struct pcap_pkthdr *header, const u_char *packet, TablaHash* tabla, ListControl* igmp);
void obtener_rtp(const struct pcap_pkthdr *header, const u_char *packet, TablaHash* tabla, ListControl* igmp, ListControl* udp, Ruido* ruido);
int leer_paquete(const struct pcap_pkthdr *header, const u_char *packet, TablaHash* tabla, ListControl* igmp, ListControl* udp, Ruido* ruido);
void errorIgmp(TablaHash* tabla, ListControl* igmp, ListControl* udp);

#endif