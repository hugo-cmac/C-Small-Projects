#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <linux/if_packet.h>

int rawSocket(){
    int s = 0;
                    
    if ((s = socket ( AF_PACKET         //Data link level packets
                    , SOCK_RAW          //Whole packet(headers included)
                    , htons(ETH_P_IP)   //Only listens for IP packets
                    )) < 0) {
        perror ("Error openning raw socket.");
        return -1;
    }
    return s;
}

int IPSocket(char *interface, int on){
    int s = 0;

    if ((s = socket (AF_INET           //IPv4 family
                    , SOCK_RAW         //Whole IP datagram
                    , htons(ETH_P_IP)  //Only listens for IP packets
                    )) < 0) {
        perror ("Error openning IP socket.");
        return -1;
    }

    ///////////////////////
    //Set interface to bind
    if (setsockopt (s, SOL_SOCKET, SO_BINDTODEVICE, interface, 4) < 0) {	
        perror ("Error on interface binding.");
        return -1;
    }

    //////////////////
    //Included header?
    if (setsockopt (s, 0, IP_HDRINCL, &on, sizeof(on)) < 0) {
        perror ("Error on including IP header.");
        return -1;
    }
    return s;
}

void printEthernetHeader(struct ethhdr *eth){
    printf("\nEthernet Header\n");
    printf("\t|-Source Address :      %.2x-%.2x-%.2x-%.2x-%.2x-%.2x\n", eth->h_source[0],   eth->h_source[1],   eth->h_source[2],   eth->h_source[3],   eth->h_source[4],   eth->h_source[5]);
    printf("\t|-Destination Address : %.2x-%.2x-%.2x-%.2x-%.2x-%.2x\n", eth->h_dest[0],     eth->h_dest[1],     eth->h_dest[2],     eth->h_dest[3],     eth->h_dest[4],     eth->h_dest[5]);
    printf("\t|-Protocol : %d\n", eth->h_proto);
}

void printIpHeader(struct iphdr * ip) {
    struct sockaddr_in source,dest;
    source.sin_addr.s_addr = ip->saddr;
    dest.sin_addr.s_addr = ip->daddr;

    printf("\nIP Header\n");
    printf("\t|-Version : %d\n", (unsigned int) ip->version);
    printf("\t|-Internet Header Length : %d DWORDS or %d Bytes\n", (unsigned int) ip->ihl, ((unsigned int) (ip->ihl))*4);
    printf("\t|-Type Of Service : %d\n", (unsigned int) ip->tos);
    printf("\t|-Total Length : %d Bytes\n", ntohs(ip->tot_len));
    printf("\t|-Identification : %d\n", ntohs(ip->id));
    printf("\t|-Time To Live : %d\n", (unsigned int) ip->ttl);
    printf("\t|-Protocol : %d\n", (unsigned int) ip->protocol);
    printf("\t|-Header Checksum : %d\n", ntohs(ip->check));
    printf("\t|-Source IP : %s\n", inet_ntoa(source.sin_addr));
    printf("\t|-Destination IP : %s\n", inet_ntoa(dest.sin_addr));
}

int main(int argc, char const *argv[]){
    
    unsigned char *buffer =  malloc(65536);
    
    int rawsocket = rawSocket();

    struct sockaddr_in source = {0};
    socklen_t len = sizeof(source);

    recv( rawsocket, buffer, 65536, 0); //Listen to the whole packet (eth + ip + transport)
    
    close(rawsocket);

    struct ethhdr *eth = (struct ethhdr *)(buffer); //ETH header
    printEthernetHeader(eth);
    
    struct iphdr *ip = (struct iphdr *)(buffer + sizeof(struct ethhdr)); //IP header
    printIpHeader(ip);

    buffer = (unsigned char *)(buffer + sizeof(struct ethhdr)); //IP datagram

    int ipsocket = IPSocket("eth0", 1);

    struct sockaddr_in dest = {0};

    dest.sin_family = AF_INET;			    //IPv4 family
    dest.sin_addr.s_addr = ip->daddr;	    //Dest IP address
    int datagramLen = ntohs(ip->tot_len);   //Datagram length          
    
    len = sizeof(dest);

    sendto(ipsocket, buffer, datagramLen, 0, (struct sockaddr *) &dest, len);
 
    close(ipsocket);

    return 0;
}
